//string_converter_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/string_converter.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


int main(int argc, char* argv[])
{
	typedef yggr::utf8_string utf8_string_type;
	typedef boost::container::string boost_string_type;
	typedef boost::container::wstring boost_wstring_type;
	typedef std::string std_string_type;
	typedef std::wstring std_wstring_type;
	
	typedef yggr::charset::string_converter conv_type;


	{
		std_string_type src("abc");

		std_wstring_type dst 
			= conv_type::s_conv<std_wstring_type>(src, 
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(std_string_type),
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(std_wstring_type));

		yggr_test_assert(dst == L"abc");
	}

	{
		std_string_type src("abc");

		boost_string_type dst 
			= conv_type::s_conv<boost_string_type>(src, 
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(std_string_type),
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(boost_string_type));

		yggr_test_assert(dst == "abc");
	}

	{
		boost_string_type src("abc");

		std_wstring_type dst 
			= conv_type::s_conv<std_wstring_type>(src, 
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(std_string_type),
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(std_wstring_type));

		yggr_test_assert(dst == L"abc");
	}

	{
		boost_string_type src("abc");

		utf8_string_type dst 
			= conv_type::s_conv<utf8_string_type>(src, 
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(boost_string_type),
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(utf8_string_type));

		yggr_test_assert(dst == "abc");
	}

	{
		const char* src = "abc";

		std_wstring_type dst 
			= conv_type::s_conv<std_wstring_type>(src, 
													YGGR_STR_UTF8_STRING_CHARSET_NAME(),
													YGGR_STR_UTF8_STRING_CHARSET_NAME());
		utf8_string_type chk(dst, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		chk.erase(chk.length() - 1);
		yggr_test_assert(chk == "abc");
	}

	{
		boost_string_type src("abc");

		boost_string_type dst;

		conv_type::s_conv(dst, src, 
							YGGR_STR_STRING_DEFAULT_CHARSET_NAME(boost_string_type),
							YGGR_STR_STRING_DEFAULT_CHARSET_NAME(boost_string_type));

		yggr_test_assert(dst == "abc");
	}

	{
		boost_string_type src("abc");

		boost_wstring_type dst
			= conv_type::s_conv<boost_wstring_type>(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(boost_string_type));

		yggr_test_assert(dst == L"abc");
	}

	{
		boost_string_type src("abc");

		boost_wstring_type dst;

		conv_type::s_conv(dst, src);
		yggr_test_assert(dst.size() == (src.size() + sizeof(wchar_t) - 1) / sizeof(wchar_t));
		yggr_test_assert(0 == memcmp(&src[0], &dst[0], src.size()));
		
        if(sizeof(wchar_t) == 2)
        {
            yggr_test_assert(dst.size() == 2);
            yggr_test_assert(*(reinterpret_cast<char*>(&dst[1]) + 1) == 0);
        }
        else
        {
             yggr_test_assert(sizeof(wchar_t) == 4);
             yggr_test_assert(dst.size() == 1);
             yggr_test_assert(*(reinterpret_cast<char*>(&dst[0]) + 3) == 0);
        }
	}

	{
		std_wstring_type src(L"abc");
		boost_string_type dst
			= conv_type::s_conv<boost_string_type>(src, 
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(boost_wstring_type),
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(boost_wstring_type));

		std_wstring_type dst2 
			= conv_type::s_conv<std_wstring_type>(src, 
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(boost_wstring_type),
													YGGR_STR_STRING_DEFAULT_CHARSET_NAME(std_wstring_type));

		yggr_test_assert(dst2 == std_wstring_type(src.begin(), src.end()));
	}
	
	wait_any_key(argc, argv);
	return 0;
}
