#ifndef _PPK_IFILEITERATOR_HPP
#define _PPK_IFILEITERATOR_HPP

#include <fstream>


namespace ppk
{
namespace detail
{

// Very simple iterator, wrapper for ifstream
class IFileIterator
{
public:
	IFileIterator(const std::string & path);
	IFileIterator(const IFileIterator &) = delete;
	const IFileIterator & operator=(const IFileIterator &) = delete;
	~IFileIterator();
	
	void operator++();
	void operator++(int);
	const std::string::value_type & operator*() const;
	bool isValid() const;
	unsigned getIndex() const;
	unsigned getLine() const;
	unsigned getChar() const;

private:
	char current_char;
	std::ifstream stream;
	
	unsigned position;
	unsigned line;
	unsigned character; // Character in current line
};

}
}


#endif //_PPK_IFILEITERATOR_HPP
