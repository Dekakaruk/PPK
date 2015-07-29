

template <class IteratorType>
IteratorReturner<IteratorType>::IteratorReturner(IteratorType begin, IteratorType end) : b(begin), e(end)
{
}

template <class IteratorType>
IteratorType IteratorReturner<IteratorType>::begin()
{
	return b;
}

template <class IteratorType>
IteratorType IteratorReturner<IteratorType>::end()
{
	return e;
}




namespace detail
{

template <class T>
GroupIterator<T>::GroupIterator(GroupIterator<T>::base_iterator it) :
    iter(it)
{
}

template <class T>
GroupIterator<T>::GroupIterator(GroupIterator<T>::const_base_iterator it) :
    iter(it)
{
}

template <class T>
bool GroupIterator<T>::operator!=(const GroupIterator<T> & scnd) const
{
	return iter != scnd.iter;
}

template <class T>
typename GroupIterator<T>::value_type & GroupIterator<T>::operator*() const
{
	return *iter->second;
}

template <class T>
typename GroupIterator<T>::value_type *GroupIterator<T>::operator->() const
{
	return iter->second;
}

template <class T>
GroupIterator<T> & GroupIterator<T>::operator++()
{
	++iter;
	return *this;
}

template <class T>
GroupIterator<T> GroupIterator<T>::operator++(int)
{
	GroupIterator<T> tmp(*this);
	++iter;
	return tmp;
}

template <class T>
GroupIterator<T> & GroupIterator<T>::operator--()
{
	--iter;
	return *this;
}

template <class T>
GroupIterator<T> GroupIterator<T>::operator--(int)
{
	GroupIterator<T> tmp(*this);
	--iter;
	return tmp;
}


template <class T>
ListIterator<T>::ListIterator(ListIterator<T>::base_iterator it) :
    iter(it)
{
	
}

template <class T>
ListIterator<T>::ListIterator(ListIterator<T>::const_base_iterator it) :
    iter(it)
{
	
}

template <class T>
bool ListIterator<T>::operator!=(const ListIterator<T> & scnd) const
{
	return iter != scnd.iter;
}

template <class T>
typename ListIterator<T>::value_type & ListIterator<T>::operator*() const
{
	return **iter;
}

template <class T>
typename ListIterator<T>::value_type *ListIterator<T>::operator->() const
{
	return *iter;
}

template <class T>
ListIterator<T> & ListIterator<T>::operator++()
{
	++iter;
	return *this;
}

template <class T>
ListIterator<T> ListIterator<T>::operator++(int)
{
	ListIterator<T> tmp(*this);
	++iter;
	return tmp;
}

template <class T>
ListIterator<T> & ListIterator<T>::operator--()
{
	--iter;
	return *this;
}

template <class T>
ListIterator<T> ListIterator<T>::operator--(int)
{
	ListIterator<T> tmp(*this);
	--iter;
	return tmp;
}


template<class BaseIter>
Reverse<BaseIter>::Reverse(BaseIter it) : iter(it)
{}

template<class BaseIter>
Reverse<BaseIter>::Reverse(typename BaseIter::base_iterator it) : iter(it)
{}

template<class BaseIter>
Reverse<BaseIter>::Reverse(typename BaseIter::const_base_iterator it) : iter(it)
{}

template<class BaseIter>
bool Reverse<BaseIter>::operator!=(const Reverse<BaseIter> & scnd) const
{
	return iter != scnd.iter;
}

template<class BaseIter>
typename BaseIter::value_type & Reverse<BaseIter>::operator*() const
{
	BaseIter tmp = iter;
	return *(--tmp);
}

template<class BaseIter>
typename BaseIter::value_type * Reverse<BaseIter>::operator->() const
{
	BaseIter tmp = iter;
	return (--tmp).operator->();
}

template<class BaseIter>
Reverse<BaseIter> & Reverse<BaseIter>::operator++()
{
	--iter;
	return *this;
}

template<class BaseIter>
Reverse<BaseIter> Reverse<BaseIter>::operator++(int)
{
	Reverse<BaseIter> tmp(*this);
	--iter;
	return tmp;
}

template<class BaseIter>
Reverse<BaseIter> & Reverse<BaseIter>::operator--()
{
	++iter;
	return *this;
}

template<class BaseIter>
Reverse<BaseIter> Reverse<BaseIter>::operator--(int)
{
	Reverse<BaseIter> tmp(*this);
	++iter;
	return tmp;
}

} // namespace _ppklib

