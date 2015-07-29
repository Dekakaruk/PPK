#ifndef _PPK_UTILITY_HPP
#define _PPK_UTILITY_HPP

#include <sstream>
#include <string>


namespace ppk
{
namespace detail
{

template <class T>
inline std::string to_string(const T& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

}
}

#endif //_PPK_UTILITY_HPP
