#include "Node.hpp"

#include <cstdio>
#include <stdexcept>
#include <algorithm>

#include "utility.hpp"
#include "IFileIterator.hpp"
#include "StandardConverters.hpp"

using namespace ppk;
using namespace detail;

Node::Node(const std::string & name, const std::string & identifier) :
    name(name),
    identifier(identifier)
{
	if (name.empty() && !identifier.empty())
		throw std::domain_error("Nodes must have name before they could have an identifier!");
	
	type = Type::Null;
	
	parent = NULL;
}

Node::~Node()
{
	for (auto & it : block_index)
		delete it;
	
	block.clear();
	block_index.clear();
	parent = NULL;
}

Node::Type Node::getType() const
{
	return type;
}

void Node::insert(Node * child)
{
	if (!child->isRoot())
		throw std::domain_error("This node has got already parent!");
	
	if (type == Type::Scalar)
		throw std::domain_error("You cannot add nodes to scalars!");
	if (type == Type::List && (!child->name.empty() || !child->identifier.empty()))
		throw std::domain_error("Children in lists cannot have names nor identifiers!");
	if (type == Type::Group && child->name.empty())
		throw std::domain_error("In a group everything must have a name!");
	
	if (type == Type::Null)
	{
		if (child->name.empty())
			type = Type::List;
		else
			type = Type::Group;
	}
	
	block_index.push_back(child);
	block.insert(std::make_pair(child->name, child));
	child->parent = this;
}

Node &Node::emplace(const std::string & name, const std::string & identifier)
{
	Node * node = new Node(name, identifier);
	insert(node);
	return *node;
}

Node &Node::operator()(const std::string & name)
{
	if (hasKey(name))
		return (*this)[name];
	else
		return emplace(name);
}

void Node::setIdentifier(const std::string & value)
{
	if (!hasName())
		throw std::domain_error("Nodes must have name before they could have an identifier!");
	
	identifier = value;
}

void Node::setScalar(const std::string & value)
{
	if (type == Type::Group)
		throw std::domain_error("Groups cannot have values!");
	if (type == Type::List)
		throw std::domain_error("Lists cannot have values!");
	
	if (type == Type::Null)
		type = Type::Scalar;
	
	scalar = value;
}

const char * Node::operator=(const char * value)
{
	clear();
	setScalar(value);
	return value;
}

void Node::removePtr(Node * child)
{
	block_index.erase(std::remove(block_index.begin(), block_index.end(), child), block_index.end());
	
	std::pair<block_type::iterator, block_type::iterator> ret = block.equal_range(child->getName());
	for (detail::block_type::iterator iter = ret.first; iter != ret.second;)
	{
		auto erased = iter++;
		if (erased->second == child)
		{
			block.erase(erased);
			delete child;
			break;
		}
	}
}

void Node::remove(unsigned index)
{
	removePtr(&((*this)[index]));
}

void Node::remove(const std::string & name)
{
	removePtr(&((*this)[name]));
}

void Node::removeAll()
{
	for (auto & it : block_index)
		delete it;
	
	block.clear();
	block_index.clear();
}

void Node::removeOnly(const std::string & name)
{
	block_index.erase(std::remove_if( block_index.begin(), block_index.end(), 
	                                  [&name](Node * x){return x->getName() == name;}), block_index.end());
	
	for (auto & it : only(name))
		delete &it;
	
	block.erase(name);
}

void Node::clear()
{
	for (auto & it : block_index)
		delete it;
	
	block.clear();
	block_index.clear();
	
	scalar = "";
	
	type = Type::Null;
}

bool Node::hasName() const
{
	return !name.empty();
}

bool Node::hasIdentifier() const
{
	return !identifier.empty();
}

bool Node::isRoot() const
{
	return parent == NULL;
}

const std::string & Node::getName() const
{
	return name;
}

const std::string & Node::getIdentifier() const
{
	return identifier;
}

Node *Node::getParent()
{
	return parent;
}

const Node *Node::getParent() const
{
	return parent;
}

const std::string & Node::getScalar() const
{
	return scalar;
}

unsigned Node::count(const std::string & name) const
{
	return block.count(name);
}

Node & Node::operator[](const std::string & name)
{
	std::pair<block_type::iterator, block_type::iterator> ret = block.equal_range(name);
	
	if (ret.first == block.end())
		throw std::out_of_range("There is no such key as " + name + " in node " + getName() + (!hasIdentifier() ? " " + getIdentifier() : "") + "!");
	
	return *((--(ret.second))->second);
}

const Node & Node::operator[](const std::string & name) const
{
	std::pair<block_type::const_iterator, block_type::const_iterator> ret = block.equal_range(name);
	
	if (ret.first == block.end())
		throw std::out_of_range("There is no such key as " + name + " in node " + getName() + (!hasIdentifier() ? " " + getIdentifier() : "") + "!");
	
	return *((--(ret.second))->second);
}

std::string Node::operator()(const std::string & name, const char * default_val) const
{
	if (hasKey(name))
		return (*this)[name];
	
	return std::string(default_val);
}

Node &Node::operator[](unsigned index)
{
	if (index < block_index.size())
		return *block_index[index];
	
	throw std::out_of_range("There are only " + detail::to_string(block_index.size()) + " subnodes in node " + 
	                        name + (!identifier.empty() ? " " + identifier : "") + "! (Requested index: " + detail::to_string(index) + ")"
	                        );
}

const Node &Node::operator[](unsigned index) const
{
	if (index < block_index.size())
		return *block_index[index];
	
	throw std::out_of_range("There are less subnodes than " + detail::to_string(index) + " in node " + name + (!identifier.empty() ? " " + identifier : "") + "!");
}

IteratorReturner<NodeIter> Node::all()
{
	return IteratorReturner<NodeIter>(block_index.begin(), block_index.end());
}

IteratorReturner<CNodeIter > Node::all() const
{
	return IteratorReturner<CNodeIter>(block_index.begin(), block_index.end());
}

IteratorReturner<RNodeIter> Node::rall()
{
	return IteratorReturner<RNodeIter>(block_index.end(), block_index.begin());
}

IteratorReturner<CRNodeIter> Node::rall() const
{
	return IteratorReturner<CRNodeIter>(block_index.end(), block_index.begin());
}

IteratorReturner<NodeSortedIter> Node::only(const std::string & name)
{
	std::pair<block_type::iterator, block_type::iterator> ret = block.equal_range(name);
	return IteratorReturner<NodeSortedIter>(ret.first, ret.second);
}

IteratorReturner<CNodeSortedIter> Node::only(const std::string & name) const
{
	std::pair<block_type::const_iterator, block_type::const_iterator> ret = block.equal_range(name);
	return IteratorReturner<CNodeSortedIter>(ret.first, ret.second);
}

IteratorReturner<RNodeSortedIter> Node::ronly(const std::string & name)
{
	std::pair<block_type::iterator, block_type::iterator> ret = block.equal_range(name);
	return IteratorReturner<RNodeSortedIter>(ret.second, ret.first);
}

IteratorReturner<CRNodeSortedIter> Node::ronly(const std::string & name) const
{
	std::pair<block_type::const_iterator, block_type::const_iterator> ret = block.equal_range(name);
	return IteratorReturner<CRNodeSortedIter>(ret.second, ret.first);
}

IteratorReturner<NodeSortedIter> Node::sorted()
{
	return IteratorReturner<NodeSortedIter>(block.begin(), block.end());
}

IteratorReturner<CNodeSortedIter> Node::sorted() const
{
	return IteratorReturner<CNodeSortedIter>(block.begin(), block.end());
}

IteratorReturner<RNodeSortedIter> Node::rsorted()
{
	return IteratorReturner<RNodeSortedIter>(block.end(), block.begin());
}

IteratorReturner<CRNodeSortedIter> Node::rsorted() const
{
	return IteratorReturner<CRNodeSortedIter>(block.end(), block.begin());
}

bool Node::hasKey(const std::string & name) const
{
	return block.find(name) != block.end();
}

unsigned Node::size() const
{
	return block_index.size();
}

bool Node::hasDimensions(const std::list<size_t> & dimensions) const
{
	return hasDimensions(dimensions.cbegin(), dimensions.cend());
}

bool Node::hasDimensions(std::list<size_t>::const_iterator it, std::list<size_t>::const_iterator end) const
{
	if (size() != *it)
		return false;

	if (++it == end)
		return true;

	for (auto & child : all())
		if (!child.hasDimensions(it, end))
			return false;

	return true;
}

void Node::print(int d) const
{
	for (int i = 0; i < d; i++)
		printf("\t");
	if (hasIdentifier())
		printf("%s \"%s\"= %s\n", getName().c_str(), getIdentifier().c_str(), getScalar().c_str());
	else
		printf("%s = %s\n", getName().c_str(), getScalar().c_str());
	
	for (auto & it : all())
	{
		it.print(d + 1);
	}
}

void Node::shake()
{
	Node * node = new Node;
	
	node->type = type;
	type = Type::Null;
	
	node->scalar = scalar;
	scalar = "";
	
	node->block = block;
	block.clear();
	
	node->block_index = block_index;
	block_index.clear();
	
	insert(node);
}
