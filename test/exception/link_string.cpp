//link_string.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <string>

#include <yggr/base/error_make.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(base)

class A
{
	ERROR_MAKER_BEGIN("A")
		ERROR_CODE_DEF_BEGIN(100)
			ERROR_CODE_DEF(E_101)
			ERROR_CODE_DEF(E_102)
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_101, "error 101")
			ERROR_CODE_MSG(E_102, "error 102")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	static A::error_type gen_err(void)
	{
		return error_maker_type::make_error(error_maker_type::E_101);
	}
};

int main(int argc, char* argv[])
{
	std::cout << A::gen_err().what() << std::endl;

	wait_any_key(argc, argv);
	return 0;
}
