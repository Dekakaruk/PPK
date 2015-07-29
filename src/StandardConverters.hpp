/**
 * @file StandardConverters.hpp
 * @brief The file declares converters for built-in types and std::string.
 */

#ifndef _PPK_STANDARDCONVERTERS_HPP
#define _PPK_STANDARDCONVERTERS_HPP


#include "Node.hpp"
#include <sstream>
#include <limits>

namespace ppk
{

///@cond PRIVATE

template<>
class Converter<std::string>
{
public:
	static constexpr const char * type_name = "std::string";
	
	static bool fromNode(const Node & node, std::string & out)
	{
		out = node.getScalar();
		return node.getType() == Node::Type::Scalar;
	}
	
	static void toNode(Node & node, const std::string & in)
	{
		node.setScalar(in);
	}
};

#define PPK_DEFINE_STREAMABLE_CONVERTER(Streamable)\
	template<>\
	class Converter<Streamable>\
	{\
	public:\
		static constexpr const char * type_name = #Streamable;\
		\
		static bool fromNode(const Node & node, Streamable & out)\
		{\
			if (node.getType() != Node::Type::Scalar)\
				return false;\
			\
			const std::string & str = node.getScalar();\
			\
			if (std::numeric_limits<Streamable>::has_infinity)\
			{\
				if (str == "inf")\
				{\
					out = std::numeric_limits<Streamable>::infinity();\
					return true;\
				}\
				if (std::numeric_limits<Streamable>::is_signed && str == "-inf")\
				{\
					out = -std::numeric_limits<Streamable>::infinity();\
					return true;\
				}\
			}\
			\
			if (std::numeric_limits<Streamable>::has_quiet_NaN)\
			{\
				if (str == "nan")\
				{\
					out = std::numeric_limits<Streamable>::quiet_NaN();\
					return true;\
				}\
				if (std::numeric_limits<Streamable>::is_signed && str == "-nan")\
				{\
					out = -std::numeric_limits<Streamable>::quiet_NaN();\
					return true;\
				}\
			}\
			\
			std::stringstream stream(str);\
			stream.unsetf(std::ios::dec);\
			stream >> std::noskipws >> std::boolalpha >> out;\
			stream.peek();\
			\
			if (stream.eof())\
				return true;\
			\
			return false;\
		}\
		\
		static void toNode(Node & node, const Streamable & in)\
		{\
			std::stringstream stream;\
			stream << std::boolalpha << in;\
			node.setScalar(stream.str());\
		}\
	};

PPK_DEFINE_STREAMABLE_CONVERTER(bool)

PPK_DEFINE_STREAMABLE_CONVERTER(int)
PPK_DEFINE_STREAMABLE_CONVERTER(short)
PPK_DEFINE_STREAMABLE_CONVERTER(long)
PPK_DEFINE_STREAMABLE_CONVERTER(long long)
PPK_DEFINE_STREAMABLE_CONVERTER(unsigned)
PPK_DEFINE_STREAMABLE_CONVERTER(unsigned short)
PPK_DEFINE_STREAMABLE_CONVERTER(unsigned long)
PPK_DEFINE_STREAMABLE_CONVERTER(unsigned long long)

PPK_DEFINE_STREAMABLE_CONVERTER(char)
PPK_DEFINE_STREAMABLE_CONVERTER(signed char)
PPK_DEFINE_STREAMABLE_CONVERTER(unsigned char)

PPK_DEFINE_STREAMABLE_CONVERTER(float)
PPK_DEFINE_STREAMABLE_CONVERTER(double)
PPK_DEFINE_STREAMABLE_CONVERTER(long double)

#undef PPK_DEFINE_STREAMABLE_CONVERTER
///@endcond

}

#endif //_PPK_STANDARDCONVERTERS_HPP
