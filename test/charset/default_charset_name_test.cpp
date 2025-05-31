//utf8_char_arithmetic_test.cpp

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! if you want to test utf8, don't forgot add YGGR_USE_UTF8_EX_MODE to user_config.hpp !!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/string_view.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <yggr/compile_link/linker.hpp>

#include <iostream>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

void test(void)
{
	std::cout << "char:\t\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(char) << std::endl;

#if !defined(YGGR_NO_CWCHAR)
	std::cout << "wchar_t:\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t) << std::endl;
#endif // YGGR_NO_CWCHAR

	std::cout << "yggr::s8:\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::s8) << std::endl;
	std::cout << "yggr::u8:\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::u8) << std::endl;

	std::cout << "yggr::s16:\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::s16) << std::endl;
	std::cout << "yggr::u16:\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::u16) << std::endl;

	std::cout << "yggr::s32:\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::s32) << std::endl;
	std::cout << "yggr::u32:\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::u32) << std::endl;

#if !defined(YGGR_NO_CHAR8_T)
	std::cout << "yggr::c8:\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::c8) << std::endl;
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)
	std::cout << "yggr::c16:\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::c16) << std::endl;
#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR32_T)
	std::cout << "yggr::c32:\t" << YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::c32) << std::endl;
#endif // YGGR_NO_CHAR8_T

}


int main(int argc, char* argv[])
{
    test();

#if defined(_MSC_VER)
	std::cout << "--------------test_msvc end--------------" << std::endl;
#elif defined(__MINGW32__)
    std::cout << "--------------test_mingw end--------------" << std::endl;
#elif defined(__linux__)
    std::cout << "--------------test_linux end--------------" << std::endl;
#elif defined(__APPLE___) || defined(__APPLE_CC__)
    std::cout << "--------------test_macos end--------------" << std::endl;
#else
#	error "append other platfrom code"
#endif // _MSC_VER

	std::cout << "--------------all_test end--------------" << std::endl;
	wait_any_key(argc, argv);
	return 0;
}
