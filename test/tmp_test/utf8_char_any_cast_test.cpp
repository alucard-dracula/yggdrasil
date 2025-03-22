// utf8_char_any_cast_test.cpp


#include <yggr/any_val/any_operator_mgr_boost_basic_t_reg.hpp>
#include <yggr/any_val/any_operator_mgr_reg_macro.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)

int main(int argc, char* argv[])
{
	{
		boost::any any_val = yggr::utf8_char('a');
		boost::any_cast<yggr::utf8_char>(&any_val);
	}

	{
		boost::any any_val = yggr::utf8_char('a');
		yggr::any_cast<yggr::utf8_char>(&any_val);
	}

	return 0;
}