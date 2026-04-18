// c_str_test.cpp

#include <yggr/base/yggrdef.h>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/charset/c_str.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <cassert>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


int main(int argc, char *argv[])
{
	{
		char str[] = "abc";
		bool bchk = (&str[0] == yggr::charset::c_str(str));
		assert(bchk);
	}

	{
		const char* str = "abc";
		bool bchk = (str == yggr::charset::c_str(str));
		assert(bchk);
	}

	{
		std::string str = "abc";
		bool bchk = (str.c_str() == yggr::charset::c_str(str));
		assert(bchk);
	}

	{
		std::wstring str = L"abc";
		bool bchk = (str.c_str() == yggr::charset::c_str(str));
		assert(bchk);
	}

	{
		yggr::string str = "abc";
		bool bchk = (str.c_str() == yggr::charset::c_str(str));
		assert(bchk);
	}

	{
		yggr::wstring str = L"abc";
		bool bchk = (str.c_str() == yggr::charset::c_str(str));
		assert(bchk);
	}

	{
		yggr::utf8_string str = "abc";
		bool bchk = (str.c_str() == yggr::charset::c_str(str));
		assert(bchk);
	}
	
	wait_any_key(argc, argv);
	return 0;
}
