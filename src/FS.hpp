#ifndef _PPK_FS_HPP
#define _PPK_FS_HPP

#include "Node.hpp"

namespace ppk
{

namespace detail {
class IFileIterator;
}

/**
 * @brief The FS class can read and write nodes from and to files.
 */
class FS
{
public:
	/// Standard constructor.
	FS();
	
	
	/// Noncopyable.
	FS(const FS &) = delete;
	
	/// Nonassignable
	FS & operator=(const FS&) = delete;
	
	/**
	 * @brief Standard destructor.
	 * 
	 * Deletes its root.
	 */
	virtual ~FS();
	
	
	/**
	 * @brief Reads data from given path.
	 * 
	 * Path can lead to both file and directory. In the latter case it reads all
	 * files in depth-first alphabetical order. Can be called multiple times, which
	 * adds new data to the end.
	 * 
	 * @param path
	 * @return true if no errors happened @see getError()
	 */
	bool read(const std::string & path);
	
	/// Returns the last error message.
	const std::string & getError() const;
	
	
	/// Writes data to given file.
	bool write(const std::string & path);
	
	
	/// Returns the root node. (It always exists, even if nothing was read).
	Node & getRoot();
	
	/// Returns the root node. (It always exists, even if nothing was read).
	const Node & getRoot() const;
	
	
	/// Prints data tree, for debugging.
	void print() const;
	
private:
	Node root;
	
	bool readDirectory(const std::string & path);
	bool readFile(const std::string & path);
	
	
	// Move iterator to first non whitespace character. Comments are considered whitespaces.
	void skipWhitespace(detail::IFileIterator & iterator);
	
	// Move iterator to first non comment character
	void skipComment(detail::IFileIterator & iterator);
	
	
	// Reads something that can be a value (i.e. scalar, block or list) and puts it in output node
	bool readValue(detail::IFileIterator & iterator, Node & output);
	
	// Reads a list into the node
	bool readList(detail::IFileIterator & iterator, Node & owner);
	
	// Reads a string checking if it is quoted and running according function
	bool readScalar(detail::IFileIterator & iterator, std::string & output);
	
	// Reads a string that can contain most of characters, with notable exception of whitespaces, '#', '=', and brackets
	bool readNotQuotedScalar(detail::IFileIterator & iterator, std::string & output);
	
	// Reads quoted string. String ends at it's beginning character, eg. "la la la" or ila la lai
	bool readQuotedScalar(detail::IFileIterator & iterator, std::string & output);
	
	// Parse single node
	bool readNode(detail::IFileIterator & iterator, Node & parent);
	
	// Parse block. If end != 0, skip first char and end at end character.
	bool readBlock(detail::IFileIterator & iterator, Node & owner, char end = 0);
	
	
	void writeRoot(std::ofstream & file) const;
	void writeNode(std::ofstream & file, const Node & node, int d=0) const;
	std::string escapeScalar(const std::string & str) const;
	
	
	
	
	std::string currentPath;
	std::string errorMsg; // Set if error occured.
	
	// Both sets errorMsg.
	void setError(const std::string & string);
	void setParsingError(const std::string & string, const detail::IFileIterator & iterator);
};
}

#endif //_PPK_FS_HPP
