// wait_any_key.hpp

#ifndef __TEST_WAIT_ANY_KEY_HPP__
#define __TEST_WAIT_ANY_KEY_HPP__

#include <yggr/base/yggrdef.h>
#include <iostream>
#include <cassert>

namespace yggr_test
{

inline void release_assert(bool bchk, const char* msg, const char* func, const char* file, int line)
{
	char str[1024] = {0};
	if(!bchk)
	{
		std::cerr << "\nassert(false)\a" << std::endl;
		std::cerr << "msg:\t" << msg << std::endl;
		std::cerr << "func:\t" << func << std::endl;
		std::cerr << "file:\t" << file << " ";
		std::cerr << "line:\t" << line << std::endl;
		std::cin >> str;
	}
}

} // yggr_test

#if defined( _DEBUG ) && defined(_MSC_VER)

#	define yggr_test_assert assert

#else

#	define yggr_test_assert( __x__ ) ::yggr_test::release_assert(!!(__x__), (#__x__), (__FUNCTION__), (__FILE__), (__LINE__) )

#endif // _DEBUG

#include <yggr/charset/string.hpp>

namespace detail
{

inline bool wait_any_key_detail(int argc, char* argv[])
{
	if(!((argc > 1) && (yggr::string("--batch-test") == argv[argc - 1])))
	{
		char cc = 0;
		std::cin >> cc;
		return true;
	}

	return false;
}

} // namespace detail

#define wait_any_key(__argc__, __argv__) \
	std::cout << ( __FILE__ ) << " test_end" << std::endl; \
	detail::wait_any_key_detail(__argc__, __argv__)

#if defined(_MSC_VER)
#	define TEST_PP_WCOUT_IMBUE( __val__ ) std::wcout.imbue(std::locale( __val__ ));
#	define TEST_PP_WCIN_IMBUE( __val__ ) std::wcin.imbue(std::locale( __val__ ));

#	define TEST_PP_WIO_IMBUE( __val__ ) \
		TEST_PP_WCOUT_IMBUE( __val__ ) \
		TEST_PP_WCIN_IMBUE( __val__ )

#	define TEST_PP_WOS_IMBUE( __os__, __val__ ) __os__.imbue(std::locale( __val__ ));

#elif defined(__MINGW32__)

//#	define TEST_PP_WIO_IMBUE( __val__ ) \
//		setlocale(LC_ALL, ""); \
//		std::ios_base::sync_with_stdio(false); \
//		std::wcout.imbue(std::locale("")); \
//		std::wcin.imbue(std::locale(""));

#	define TEST_PP_WIO_IMBUE( __val__ ) \
		setlocale(LC_ALL, ""); \
		std::ios_base::sync_with_stdio(false);

#	define TEST_PP_WOS_IMBUE( __os__, __val__ ) __os__.imbue(std::wcout.getloc());
//#	define TEST_PP_WOS_IMBUE( __os__, __val__ )

#else

#	define TEST_PP_WIO_IMBUE( __val__ ) \
		setlocale(LC_ALL, ""); \
		std::ios_base::sync_with_stdio(false); \
		std::wcout.imbue(std::locale("")); \
		std::wcin.imbue(std::locale(""));

#	define TEST_PP_WOS_IMBUE( __os__, __val__ ) __os__.imbue(std::locale(""));

#endif // _MSC_VER

#endif // __TEST_WAIT_ANY_KEY_HPP__
