#ifndef	_PPK_NODEITERATORS_HPP
#define _PPK_NODEITERATORS_HPP

#include <map>
#include <vector>


namespace ppk
{

class Node;

/**
 * @brief Pair of iterators, suitable for C++11 range based for loops.
 * 
 * @code{.cpp}
 * for (auto & it : some_iterator_returner)
 *     it.doSth();
 * @endcode
 * 
 * @tparam IteratorType -- type of returned iterators
 */
template <class IteratorType>
class IteratorReturner
{
	IteratorType b, e;
	
public:
	/// Constructs a Returner from two iterators to be returned.
	IteratorReturner(IteratorType begin, IteratorType end);
	
	
	/// Returns the first iterator.
	IteratorType begin();
	
	/// Returns the second iterator.
	IteratorType end();
};



namespace detail
{
typedef std::multimap<std::string, Node*> block_type;
typedef std::vector<Node*> block_index_type;



template <class T>
class GroupIterator
{
public:
	typedef T value_type;
	typedef detail::block_type::iterator base_iterator;
	typedef detail::block_type::const_iterator const_base_iterator;
	
	GroupIterator(base_iterator it);
	GroupIterator(const_base_iterator it);
	
	bool operator!=(const GroupIterator & scnd) const;
	
	value_type & operator*() const;
	value_type * operator->() const;
	
	GroupIterator<T> & operator++();
	GroupIterator<T> operator++(int);
	GroupIterator<T> & operator--();
	GroupIterator<T> operator--(int);
	
private:
	const_base_iterator iter;
};


template <class T>
class ListIterator
{
public:
	typedef T value_type;
	typedef detail::block_index_type::iterator base_iterator;
	typedef detail::block_index_type::const_iterator const_base_iterator;
	
	ListIterator(base_iterator it);
	ListIterator(const_base_iterator it);
	
	bool operator!=(const ListIterator & scnd) const;
	
	value_type & operator*() const;
	value_type * operator->() const;
	
	ListIterator<T> & operator++();
	ListIterator<T> operator++(int);
	ListIterator<T> & operator--();
	ListIterator<T> operator--(int);
	
private:
	const_base_iterator iter;
};



template<class BaseIter>
class Reverse
{
	BaseIter iter;
	
public:
	Reverse(BaseIter it);
	Reverse(typename BaseIter::base_iterator it);
	Reverse(typename BaseIter::const_base_iterator it);
	
	bool operator!=(const Reverse<BaseIter> & scnd) const;
	
	typename BaseIter::value_type & operator*() const;
	typename BaseIter::value_type * operator->() const;
	
	Reverse<BaseIter> & operator++();
	Reverse<BaseIter> operator++(int);
	Reverse<BaseIter> & operator--();
	Reverse<BaseIter> operator--(int);
};

} // namespace _ppklib



typedef detail::ListIterator<Node> NodeIter;
typedef detail::ListIterator<const Node> CNodeIter;
typedef detail::GroupIterator<Node> NodeSortedIter;
typedef detail::GroupIterator<const Node> CNodeSortedIter;

typedef detail::Reverse<detail::ListIterator<Node>> RNodeIter;
typedef detail::Reverse<detail::ListIterator<const Node>> CRNodeIter;
typedef detail::Reverse<detail::GroupIterator<Node>> RNodeSortedIter;
typedef detail::Reverse<detail::GroupIterator<const Node>> CRNodeSortedIter;

#include "NodeIterators.tpp"

} // namespace ppk

#endif //_PPK_NODEITERATORS_HPP
