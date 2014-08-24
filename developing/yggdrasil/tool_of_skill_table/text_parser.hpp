//text_parse.hpp

#ifndef __TOOL_TMP_TEST_PARSE_HPP__
#define __TOOL_TMP_TEST_PARSE_HPP__

#include <assert.h>

template<typename String, typename From, typename Src_Parser>
class text_parser
{
public:

	typedef Src_Parser src_parser_type;

	typedef String string_type;
	typedef From from_type;

public:
	text_parser(const string_type& parse_str)
		:_parse_str(parse_str)
	{
		assert(typeid(string_type) == typeid(typename from_type::value_type));
	}

	text_parser(const text_parser& right)
		:_parse_str(right._parse_str)
	{
	}

	~text_parser(void)
	{
	}

	template<typename Container>
	yggr::size_type operator()(Container& container, const string_type& src_str)
	{
		from_type from;
		yggr::size_type size = src_parser_type::split(from, src_str, _parse_str);

		if(size)
		{
			container << from;
		}

		return size;
	}

private:
	string_type _parse_str;
};

#endif //__TOOL_TMP_TEST_PARSE_HPP__
