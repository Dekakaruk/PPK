#include "FS.hpp"

#include <boost/filesystem.hpp>
#include <fstream>

#include "utility.hpp"
#include "IFileIterator.hpp"

using namespace boost::filesystem;
using namespace ppk;
using namespace ppk::detail;

FS::FS() :
    root("<root>")
{
}

FS::~FS()
{
}

bool FS::read(const std::string & path)
{
	currentPath = path;
	
	if (!exists(path))
	{
		setError("path not existing");
		return false;
	}
	
	if (is_directory(path))
		return readDirectory(path);
	else if (is_regular_file(path))
		return readFile(path);
	else
	{
		setError("it isn't directory nor regular file");
		return false;
	}
}

const std::string & FS::getError() const
{
	return errorMsg;
}

bool FS::write(const std::string & path)
{
	currentPath = path;
	
	std::ofstream file(path.c_str());
	if (!file.is_open())
	{
		setError("can't open");
		return false;
	}
	
	writeRoot(file);
	
	file.close();
	
	return true;
}

Node & FS::getRoot()
{
	return root;
}

const Node & FS::getRoot() const
{
	return root;
}

bool FS::readDirectory(const std::string & path)
{
	std::vector<boost::filesystem::path> vec;
	std::copy(directory_iterator(path), directory_iterator(), back_inserter(vec));
	std::sort(vec.begin(), vec.end());
	
	for (auto & p : vec)
	{
		if (is_regular_file(p))
		{
			if (!readFile(p.string()))
				return false;
		}
		else if (is_directory(p))
		{
			if (!readDirectory(p.string()))
				return false;
		}
	}
	
	return true;
}

bool FS::readFile(const std::string & path)
{
	currentPath = path;
	
	try {
		IFileIterator it(path);
		return readBlock(it, root);
	}
	catch (const std::runtime_error & e)
	{
		setError(e.what());
		return false;
	}
}

void FS::skipWhitespace(IFileIterator & i)
{
	while (i.isValid() && isspace(*i))
		++i;
	while (*i == '#')
	{
		skipComment(i);
		while (i.isValid() && isspace(*i))
			++i;
	}
}

void FS::skipComment(IFileIterator & i)
{
	while (i.isValid() && *i != '\n')
		++i;
	if (i.isValid())
		++i;
}

bool FS::readValue(IFileIterator & iterator, Node & output)
{
	if (*iterator == '{')
	{
		if (!readBlock(iterator, output, '}'))
			return false;
	}
	else if (*iterator == '[')
	{
		if (!readList(iterator, output))
			return false;
	}
	else if (*iterator == ';')
	{
		++iterator;
		return true;
	}
	else
	{
		std::string str;
		if (!readScalar(iterator, str))
			return false;
		output.setScalar(str);
	}
	
	return true;
}

bool FS::readList(IFileIterator & iterator, Node & owner)
{
	iterator++;
	
	skipWhitespace(iterator);
	
	while (iterator.isValid() && *iterator != ']')
	{
		Node * node = new Node;
		owner.insert(node);
		
		if (!readValue(iterator, *node))
			return false;
		
		skipWhitespace(iterator);
		
		if (iterator.isValid() && *iterator != ']' && *iterator != ',')
		{
			setParsingError("lack of ','", iterator);
			return false;
		}
		else if (iterator.isValid() && *iterator == ',')
		{
			iterator++;
			skipWhitespace(iterator);
		}
	}
	
	if (*iterator != ']')
	{
		setParsingError("forgot to close block", iterator);
		return false;
	}
	
	iterator++;
	return true;
}

bool FS::readScalar(IFileIterator & iterator, std::string & output)
{
	if (*iterator == '"' || *iterator == '\'')
		return readQuotedScalar(iterator, output);
	else
		return readNotQuotedScalar(iterator, output);
}

bool FS::readNotQuotedScalar(IFileIterator & iterator, std::string & output)
{
	output.clear();
	
	while (iterator.isValid() && isgraph(*iterator)
	       && *iterator != '=' && *iterator != ';'
	       && *iterator != '{' && *iterator != '}'
	       && *iterator != '[' && *iterator != ']'
	       && *iterator != ','
	       && *iterator != '#')
	{
		output += *iterator;
		iterator++;
	}
	
	if (output.empty())
	{
		setParsingError("I expected a scalar", iterator);
		return false;
	}
	
	return true;
}

bool FS::readQuotedScalar(IFileIterator & iterator, std::string & output)
{
	char quote = *iterator;
	iterator++;
	
	output.clear();
	
	while (iterator.isValid() && *iterator != quote)
	{
		if (*iterator == '\\')
		{
			iterator++;
			if (!iterator.isValid())
			{
				setParsingError("incomplete escape sequence", iterator);
				return false;
			}
			
			switch (*iterator)
			{
			case 'b':
				output += '\b';
				break;
			case 'n':
				output += '\n';
				break;
			case 'r':
				output += '\r';
				break;
			case 't':
				output += '\t';
				break;
			default:
				output += *iterator;
			}
			iterator++;
		}
		else
		{
			output += *iterator;
			iterator++;
		}
	}
	
	if (*iterator != quote)
	{
		setParsingError("forgot to close string", iterator);
		return false;
	}
	
	iterator++;
	return true;
}

bool FS::readNode(IFileIterator & it, Node & parent)
{
	std::string name;
	if (!readScalar(it, name))
		return false;
	
	if (name.empty())
	{
		setParsingError("I expected a name", it);
		return false;
	}
	
	skipWhitespace(it);
	if (!it.isValid())
	{
		setParsingError("I would expect something more", it);
		return false;
	}
	
	std::string identifier;
	if (!(*it == '=' || *it == '{' || *it == ';'))
	{
		if (!readScalar(it, identifier))
			return false;
	}
	
	Node * node = new Node(name, identifier);
	parent.insert(node);
	
	skipWhitespace(it);
	if (!it.isValid())
	{
		setParsingError("I would expect something more", it);
		return false;
	}
	
	if (!(*it == '=' || *it == '{' || *it == ';'))
	{
		setParsingError("Only '=', '{' and ';' are allowed", it);
		return false;
	}
	
	if (*it == '=')
	{
		++it;
		skipWhitespace(it);
		if (!it.isValid())
		{
			setParsingError("I would expect something more", it);
			return false;
		}
	}
	
	if (!readValue(it, *node))
		return false;
	
	
	skipWhitespace(it);
	if (it.isValid() && *it == ',')
	{
		node->shake();
		
		while (it.isValid() && *it == ',')
		{
			it++;
			skipWhitespace(it);
			if (!it.isValid())
			{
				setParsingError("something is forgotten", it);
				return false;
			}
			
			Node * subnode = new Node;
			node->insert(subnode);
			
			if (!readValue(it, *subnode))
				return false;
			
			skipWhitespace(it);
		}
	}
	
	return true;
}

bool FS::readBlock(IFileIterator & it, Node & owner, char end)
{
	if (end)
		it++;
	skipWhitespace(it);
	
	while (it.isValid() && (end ? *it != end : true))
	{
		if (!readNode(it, owner))
			return false;
		
		skipWhitespace(it);
	}
	
	if (end && *it != end)
	{
		setParsingError("forgot to close block", it);
		return false;
	}
	
	if (end)
		it++;
	return true;
}

void FS::writeRoot(std::ofstream & file) const
{
	for (unsigned i = 0; i < getRoot().size(); i++)
	{
		file << "\n\n";
		writeNode(file, getRoot()[i]);
	}
}

void FS::writeNode(std::ofstream & file, const Node & node, int d) const
{
	for (int i = 0; i < d; i++)
		file << '\t';
	
	if (node.hasName())
	{
		file << node.getName() << ' ';
		
		if (node.hasIdentifier())
			file << node.getIdentifier() << ' ';
	
		file << "= ";
	}
	
	if (node.getType() == Node::Type::Null)
	{
		file << "{}";
	}
	
	if (node.getType() == Node::Type::Scalar)
	{
		file << escapeScalar(node.getScalar());
	}
	
	if (node.getType() == Node::Type::List)
	{
		file << "[\n";
		if (node.size() > 0)
			writeNode(file, node[0], d+1);
		
		for (unsigned i = 1; i < node.size(); i++)
		{
			file << ",\n";
			writeNode(file, node[i], d+1);
		}
		
		file << '\n';
		for (int i = 0; i < d; i++)
			file << '\t';
		file << ']';
	}
	
	if (node.getType() == Node::Type::Group)
	{
		file << "{";
		
		for (unsigned i = 0; i < node.size(); i++)
		{
			file << "\n";
			writeNode(file, node[i], d+1);
		}
		
		file << "\n\n";
		for (int i = 0; i < d; i++)
			file << '\t';
		file << '}';
	}
}

std::string FS::escapeScalar(const std::string & str) const
{
	std::string r;
	bool must_be_quoted = false;
	
	for (auto & i : str)
	{
		if (!(isgraph(i)  && i != '=' && i != '{' && i != '}'
		       && i != '[' && i != ']' && i != ',' && i != '#'))
			must_be_quoted = true;
		
		if (i == '\\')
		{
			must_be_quoted = true;
			r += "\\\\";
		}
		else if (i == '\b')
		{
			must_be_quoted = true;
			r += "\\b";
		}
		else if (i == '\n')
		{
			must_be_quoted = true;
			r += "\\n";
		}
		else if (i == '\r')
		{
			must_be_quoted = true;
			r += "\\r";
		}
		else if (i == '\t')
		{
			must_be_quoted = true;
			r += "\\t";
		}
		else if (i == '\"')
		{
			must_be_quoted = true;
			r += "\\\"";
		}
		else if (i == '\'')
		{
			must_be_quoted = true;
			r += "\\\'";
		}
		else
			r += i;
		
	}
	
	if (must_be_quoted)
		return "\"" + r + "\"";
	else
		return r;
}

void FS::print() const
{
	root.print();
}

void FS::setError(const std::string & string)
{
	errorMsg = "Error in \"";
	errorMsg += currentPath;
	errorMsg += "\": ";
	errorMsg += string;
}

void FS::setParsingError(const std::string & string, const IFileIterator & iterator)
{
	errorMsg = "Error in \"";
	errorMsg += currentPath;
	errorMsg += "\" at line ";
	errorMsg += to_string<int>(iterator.getLine());
	errorMsg += ", char ";
	errorMsg += to_string<int>(iterator.getChar());
	errorMsg += " (";
	errorMsg += to_string<int>(iterator.getIndex());
	errorMsg += "): ";
	errorMsg += string;
}
