#include "IFileIterator.hpp"

using namespace ppk;
using namespace ppk::detail;

IFileIterator::IFileIterator(const std::string & path)
{
	stream.open(path.c_str());
	if (!stream.is_open())
		throw std::runtime_error("cannot open file");
	current_char = stream.get();
	line = 1;
	character = 1;
	position = 1;
}

IFileIterator::~IFileIterator()
{
	stream.close();
}

void IFileIterator::operator++()
{
	++position;
	++character;
	if (current_char == '\n')
	{
		++line;
		character = 1;
	}
	
	current_char = stream.get();
}

void IFileIterator::operator++(int)
{
	++position;
	++character;
	if (current_char == '\n')
	{
		++line;
		character = 1;
	}
	
	current_char = stream.get();
}

const std::string::value_type & IFileIterator::operator*() const
{
	return current_char;
}

bool IFileIterator::isValid() const
{
	return current_char != EOF;
}

unsigned IFileIterator::getIndex() const
{
	return position;
}

unsigned IFileIterator::getLine() const
{
	return line;
}

unsigned IFileIterator::getChar() const
{
	return character;
}
