//utf8_string_test.cpp


//#define YGGR_NO_CWCHAR

//#define YGGR_USE_STL_STRING
//#define _YGGR_DEBUG_UTF8_STRING_TEMPLATE_METHOD_ONLY

#include <iostream>
#include <cassert>

#include <yggr/container/list.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_char.hpp>

#include <yggr/compile_link/linker.hpp>

//#include <yggr/ppex/pp_debug.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
	typedef yggr::charset::charset_name_t<const char*>::native_char_type val_type;

	val_type* pval = 0;

	int nnn = 0;

	BOOST_MPL_ASSERT((yggr::is_native_char<yggr::charset::charset_name_t<const char*>::native_char_type>));

//	{
//		yggr::utf8_string ins_str = " world";
//		yggr::utf8_string str = "hello";
//
//		str.append(ins_str.rbegin(), ins_str.rend());
//		assert(str.length() == 11);
//		assert(str == "hellodlrow ");
//	}

	{
        yggr::utf8_string str = "hello ";
        yggr::utf8_char ch = "w";

        str += ch;

        assert(str == "hello w");
	}

	std::cout << "all test successed" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
