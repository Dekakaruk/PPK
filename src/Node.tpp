#ifndef NODE_TPP
#define NODE_TPP


namespace ppk
{


template <class T>
bool Node::is() const
{
	T t;
	return Converter<T>::fromNode(*this, t);
}

template <class T>
T Node::as() const
{
	T t;
	if (!Converter<T>::fromNode(*this, t))
		throw std::invalid_argument(name + (identifier.empty() ? "" : " " + identifier) + " is not " + Converter<T>::type_name + "!");
	return t;
}


template <class T>
Node::operator T() const
{
	return as<T>();
}


template <class T>
T Node::operator()(const std::string & name, const T & default_val) const
{
	if (hasKey(name))
		return (*this)[name];
	return default_val;
}


template <class T>
Node & Node::operator=(const T & value)
{
	clear();
	Converter<T>::toNode(*this, value);
	return *this;
}
}

#endif // NODE_TPP

