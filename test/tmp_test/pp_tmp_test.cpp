//pp_tmp_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


int main(int argc, char* argv[])
{


	wait_any_key(argc, argv);
	return 0;
}


//#
//
//#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() yggr::mplex::null_type
//	template< typename R, typename C, u32 cv_mark,
//				YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_PP_FOO_TYPEINFO_PARAMS_LIMIT_LEN, typename Val ) >
//#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
