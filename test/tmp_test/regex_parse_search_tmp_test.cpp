//regex_parse_search_test.cpp

#include <iostream>
#include <list>
#include <vector>
#include <sstream>
#include <cassert>

#include <boost/bind.hpp>

#include <yggr/charset/const_strings.hpp>

#include <yggr/regular_parse/regex_parse.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)


template<typename Char>
typename boost::enable_if<boost::mpl::bool_<(sizeof(Char) == sizeof(char))>, std::basic_ostream<Char>& >::type
	get_ostream(void)
{
	return std::cout;
}

#ifndef YGGR_NO_CWCHAR

template<typename Char>
typename boost::enable_if<boost::mpl::bool_<(sizeof(Char) == sizeof(wchar_t))>, std::basic_ostream<Char>& >::type
	get_ostream(void)
{
	return std::wcout;
}

#endif // YGGR_NO_CWCHAR

int main(int argc, char* argv[])
{
#ifndef YGGR_NO_CWCHAR
	std::wcout.imbue(std::locale("chs"));
#endif // YGGR_NO_CWCHAR
	{
		yggr::string str_re("(a)|(b)");
		yggr::string str_fmt("(?1_)(?2_)");
		std::string text("aaacccbbb");

		try
		{
			std::cout << yggr::regular_parse::regex_parse::replace(text, str_re.c_str(), -1, str_fmt) << std::endl;
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	{
		yggr::string str_re("(a)|(b)");
		yggr::string str_fmt("(?1_)(?2_)");
		std::string text("aaacccbbb");

		typedef std::ostream_iterator<char> iterator_type;
		std::stringstream ss;

		iterator_type oiter(ss);
		//ss_helper helper(ss);

		try
		{
			yggr::regular_parse::regex_parse::replace(oiter, text, str_re, str_fmt);
		}
		catch(yggr::regular_parse::regex_parse::error_type& e)
		{
			std::cout << e.what() << std::endl;
		}

		std::cout << ss.str() << std::endl;
	}

	char cc = 0;
	std::cin >> cc;
	return 0;
}
