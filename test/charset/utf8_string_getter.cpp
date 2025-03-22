//utf8_string_getter.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/charset/utf8_string.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
	typedef yggr::charset::utf8_string string_type;

	/*string_type str1 = "¹§Ï²";
	string_type str2 = "Éý¼¶µ½vip";

	{
		const string_type::base_type& str = str1;

		for(int i = 0, isize = str.size(); i != isize; ++i)
		{
			std::cout  << (int)str[i] << ",";
		}
	}

	std::cout << std::endl;

	{
		const string_type::base_type& str = str2;

		for(int i = 0, isize = str.size(); i != isize; ++i)
		{
			std::cout << (int)str[i] << ",";
		}
	}*/

#if defined(__clang__) && (__clang__) // clang error: constant expression evaluates to -83 which cannot be narrowed to type 'char' [-Wc++11-narrowing]
	char tstr[] = {static_cast<char>(-26), static_cast<char>(-127), static_cast<char>(-83), 
					static_cast<char>(-27), static_cast<char>(-106), static_cast<char>(-100), 0};

	char tstr1[] = {static_cast<char>(-27), static_cast<char>(-115), static_cast<char>(-121), 
					static_cast<char>(-25), static_cast<char>(-70), static_cast<char>(-89), 
					static_cast<char>(-27), static_cast<char>(-120), static_cast<char>(-80), 
					118, 105, 112, 0};
#else
	char tstr[] = {-26, -127, -83, -27, -106, -100, 0};
	char tstr1[] = {-27, -115, -121, -25, -70, -89, -27, -120, -80, 118, 105, 112, 0};
#endif // #if defined(__clang__) && (__clang__)

	string_type ustr(tstr, YGGR_STR_UTF8_STRING_CHARSET_NAME());
	string_type ustr1(tstr1, YGGR_STR_UTF8_STRING_CHARSET_NAME());

	std::cout << ustr << "," << ustr1 << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
