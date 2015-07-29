#ifndef _PPK_NODE_HPP
#define	_PPK_NODE_HPP

#include <string>
#include <map>

#include "NodeIterators.hpp"

namespace ppk
{

class FS;

/**
 * @brief The Converter is a class you should specialise for any type you want read or write.
 * @tparam Type tells for which type is the converter
 */
template <class Type>
class Converter
{
public:
	/// Name of type to be used in error messages.
	static const char * const type_name;
	
	/**
	 * @brief fromNode converts Node into Type
	 * @param node -- input Node
	 * @param out -- output Type
	 * @return true if conversion was successful
	 */
	static bool fromNode(const Node & node, Type & out);
	
	/**
	 * @brief toNode converts Type into Node
	 * 
	 * It must succeed.
	 * 
	 * @param node -- output Node
	 * @param in -- input Type
	 */
	static void toNode(Node & node, const Type & in);
};



/**
 * @brief The Node class represents node of data file tree.
 */
class Node  // TODO It should remember it's file and line
{
	friend class ppk::FS;
	
public:
	// --------- CONSTRUCTORS &c. --------- //
	/**
	 * @brief Default constructor.
	 * 
	 * Beware that this is the only place where you can set name! If name is empty identifier also must be such.
	 * 
	 * @param name -- name to be set
	 * @param identifier -- identifier to be set
	 * 
	 * @throws std::domain_error when name is empty but identifier isn't.
	 */
	Node(const std::string & name = "",
	     const std::string & identifier = "");
	
	/// It's not copyable
	Node(const Node &) = delete;
	
	/// It's not assignable
    Node & operator=(const Node &) = delete;
	
	/**
	 * @brief Standard destructor.
	 * 
	 * Deletes all children of the Node.
	 */
	virtual ~Node();
	
	
	// -------------- ABOUT -------------- //
	/**
	 * @brief The type of Node
	 */
	enum class Type
	{
		Null,  ///< Empty node, can become anything
		Scalar,  ///< Contains a scalar
		List,  ///< Have children, all of them are unnamed
		Group  ///< Have children, each one has name
	};
	
	
	/// Returns type of the node
	Type getType() const;
	
	
	/// Checks if the node is named
	bool hasName() const;
	
	/// Checks if the node has identifier
	bool hasIdentifier() const;
	
	/**
	 * @brief Checks if the Node is root of its tree
	 * @return true if Node hasn't got a parent
	 */
	bool isRoot() const;
	
	
	/// Returns number of its children.
	unsigned size() const;
	
	/// Returns number of its children of given name.
	unsigned count(const std::string & name) const;
	
	/// Checks if it has child of given name.
	bool hasKey(const std::string & name) const;
	
	
	// ------------- GETTERS ------------- //
	/// Returns its name.
	const std::string & getName() const;
	
	/// Returns its identifier.
	const std::string & getIdentifier() const;
	
	
	/**
	 * @brief Returns its parent.
	 * @return NULL if it hasn't got one.
	 */
	Node * getParent();
	
	/**
	 * @brief Returns its parent.
	 * @return NULL if it hasn't got one.
	 */
	const Node * getParent() const;
	
	
	/**
	 * @brief Returns contained scalar.
	 * 
	 * Returns a string as read from file. Its main use is in Converter%s.
	 * 
	 * @return "" if it is not Scalar.
	 */
	const std::string & getScalar() const;
	
	
	/**
	 * @brief Checks if scalar is convertible to given type.
	 */
	template <class T> bool is() const;
	
	
	/**
	 * @brief Converts scalar to given type.
	 * @throws std::invalid_argument when conversion failed.
	 */
	template <class T> T as() const;
	
	/**
	 * @brief Casts scalar to given type.
	 * @throws std::invalid_argument when conversion failed.
	 */
	template <class T> operator T() const;
	
	
	/**
	 * @brief Returns the last child of given name
	 * @throws std::out_of_range if there is no such child
	 */
	Node & operator[](const std::string & name);
	
	/**
	 * @brief Returns the last child of given name
	 * @throws std::out_of_range if there is no such child
	 */
	const Node & operator[](const std::string & name) const;
	
	
	/**
	 * @brief Gets value from child or default one.
	 * 
	 * If there are multiple children of given name it uses the last one.
	 * Beware especially when using numeric constants: 7 is integer and passing
	 * it as default value while getting a float will probably result in exception.
	 * If T is const char* it returns std::string.
	 * 
	 * @return value of given child or default_value if the child does not exist
	 * @throws std::invalid_argument when child exists but conversion failed.
	 */
	template <class T> T operator()(const std::string & name, const T & default_val) const;
	
	///@cond PRIVATE
	std::string operator()(const std::string & name, const char * default_val) const;
	///@endcond
	
	
	/**
	 * @brief Returns index-th element in chronological order.
	 * @throws std::out_of_range when `index >= size()`
	 */
	Node & operator[](unsigned index);
	
	/**
	 * @brief Returns index-th element in chronological order.
	 * @throws std::out_of_range when `index >= size()`
	 */
	const Node & operator[](unsigned index) const;
	
	
	
	// ------------ IERATORS ------------ //
	/**
	 * @name Iterators
	 * 
	 * These are to be used with C++11 range-based for loops:
	 * @code{.cpp}
	 * for (auto & subnode : node.all())
	 *     subnode.doSth();
	 * @endcode
	 * or in uglier version:
	 * @code{.cpp}
	 * IteratorReturner<NodeIter> ir = node.all();
	 * for (NodeIter it = ir.begin(); it != ir.end(); ++it)
	 *     it->doSth();
	 * @endcode
	 * 
	 */
	///@{
	
	/// All children in chronological order.
	IteratorReturner<NodeIter> all();
	
	/// All children in chronological order.
	IteratorReturner<CNodeIter> all() const;
	
	/// All children in reversed chronological order.
	IteratorReturner<RNodeIter> rall();
	
	/// All children in reversed chronological order.
	IteratorReturner<CRNodeIter> rall() const;
	
	
	/// All children of given name in chronological order.
	IteratorReturner<NodeSortedIter> only(const std::string & name);
	
	/// All children of given name in chronological order.
	IteratorReturner<CNodeSortedIter> only(const std::string & name) const;
	
	/// All children of given name in reversed chronological order.
	IteratorReturner<RNodeSortedIter> ronly(const std::string & name);
	
	/// All children of given name in reversed chronological order.
	IteratorReturner<CRNodeSortedIter> ronly(const std::string & name) const;
	
	
	/// All children sorted by name (identifier doesn't matter), then in chronological order
	IteratorReturner<NodeSortedIter> sorted();
	
	/// All children sorted by name (identifier doesn't matter), then in chronological order
	IteratorReturner<CNodeSortedIter> sorted() const;
	
	/// All children sorted by name (descending, identifier doesn't matter), then in reversed chronological order
	IteratorReturner<RNodeSortedIter> rsorted();
	
	/// All children sorted by name (descending, identifier doesn't matter), then in reversed chronological order
	IteratorReturner<CRNodeSortedIter> rsorted() const;
	///@}
	
	
	
	// ------------- SETTERS ------------- //
	/**
	 * @brief Inserts subnode.
	 * 
	 * Scalars cannot have children, lists can have only unnamed ones and all children of groups must have names.
	 * 
	 * @param child
	 * @throws std::domain_error when aforementioned conditions are not respected.
	 */
	void insert(Node * child);
	
	/**
	 * @brief Creates new Node passing arguments to its constructor and inserts it.
	 * @param name 
	 * @param identifier
	 * @return reference to newly created node
	 * @throws std::domain_error (see @ref insert)
	 * @see Node()
	 */
	Node & emplace(const std::string & name="", const std::string & identifier="");
	
	/**
	 * @brief Returns the last node of given name or emplaces new one if there is none.
	 * @param name 
	 * @return reference to old node or newly created one
	 * @throws std::domain_error (see @ref insert)
	 */
	Node & operator()(const std::string & name);
	
	/**
	 * @brief Changes identifier.
	 * @param value
	 * @throws std::domain_error if the node is unnamed
	 */
	void setIdentifier(const std::string & value);
	
	/**
	 * @brief Changes scalar.
	 * 
	 * Its main use is in Converter%s.
	 * 
	 * @param value
	 * @throws std::domain_error if the node is a group or list
	 */
	void setScalar(const std::string & value);
	
	
	/// Sets node to T using proper Converter, having destroyed all current content.
	template <class T> Node & operator=(const T & value);
	
	///@cond PRIVATE
	const char * operator=(const char * value);
	///@endcond
	
	
	/// Removes given child
	void removePtr(Node * child);
	
	/**
	 * @brief Removes child at given index.
	 * @param index
	 * @throws std::out_of_range when `index >= size()`
	 */
	void remove(unsigned index);
	
	/**
	 * @brief Removes child the last child of given name.
	 * @param name
	 * @throws std::out_of_range when such child doesn't exist
	 */
	void remove(const std::string & name);
	
	/// Removes all children.
	void removeAll();
	
	/// Removes all children of given name
	void removeOnly(const std::string & name);
	
	
	/// Removes all content, deletes all children and sets type to Null.
	void clear();
	
	
	// -------------- DEBUG -------------- //
	/**
	 * @brief Prints tree. Its only for debugging.
	 * @param d -- indentation level
	 */
	void print(int d = 0) const;
	
private:
	Type type;
	
	const std::string name;
	std::string identifier;
	std::string scalar;
	
	Node * parent;
	detail::block_type block;
	detail::block_index_type block_index;
	
	void shake();  // creates anonymous child and gives it all parent's content. It is helper method for Parser.
};

}  // namespace ppk

#include "Node.tpp"

#endif //_PPK_NODE_HPP

