//utf8_string_args_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_reference.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

char g_chk_copy[] = "copy make";
char g_chk_ref[] = "reference make";
char g_chk_move[] = "move make";


template<typename String>
typename boost::enable_if<yggr::charset::utf8_string_constructable<String>, yggr::string>::type
	make_utf8_string(const String& str)
{
	typedef String now_string_type;
	YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(now_string_type, utf8_string1_type);

	utf8_string1_type utf8_str = yggr::charset::string_charset_helper_data(str);
	std::cout << utf8_str << std::endl;
	
	return boost::is_reference<utf8_string1_type>::value? g_chk_ref : g_chk_copy;
}

yggr::string make_utf8_string(BOOST_RV_REF(yggr::utf8_string) str)
{
	return g_chk_move;
}


int main(int argc, char* argv[])
{
	{
		char str[] = "utf8_string";

		yggr_test_assert(make_utf8_string(str) == g_chk_copy);
		yggr_test_assert(make_utf8_string(&str[0]) == g_chk_copy);
		yggr_test_assert(make_utf8_string(std::string(str)) == g_chk_copy);
		yggr_test_assert(make_utf8_string(yggr::string(str)) == g_chk_copy);
		yggr_test_assert(make_utf8_string(yggr::charset::make_string_charset_helper(&str[0], "gbk")) == g_chk_copy);

		yggr::utf8_string utf8_str(str);

		yggr_test_assert(make_utf8_string(utf8_str) == g_chk_ref);
		yggr_test_assert(make_utf8_string(yggr::charset::make_string_charset_helper(utf8_str)) == g_chk_ref);
		yggr_test_assert(make_utf8_string(boost::move(utf8_str)) == g_chk_move);
	}

#ifndef YGGR_NO_CWCHAR

	{
		wchar_t str[] = L"utf8_string";

		yggr_test_assert(make_utf8_string(str) == g_chk_copy);
		yggr_test_assert(make_utf8_string(&str[0]) == g_chk_copy);
		yggr_test_assert(make_utf8_string(std::wstring(str)) == g_chk_copy);
		yggr_test_assert(make_utf8_string(yggr::wstring(str)) == g_chk_copy);
		yggr_test_assert(make_utf8_string(yggr::charset::make_string_charset_helper(&str[0], "ucs-2le")) == g_chk_copy);

		yggr::utf8_string utf8_str(str);

		yggr_test_assert(make_utf8_string(utf8_str) == g_chk_ref);
		yggr_test_assert(make_utf8_string(yggr::charset::make_string_charset_helper(utf8_str)) == g_chk_ref);
		yggr_test_assert(make_utf8_string(boost::move(utf8_str)) == g_chk_move);
	}

#endif // YGGR_NO_CWCHAR

	wait_any_key(argc, argv);
	return 0;
}