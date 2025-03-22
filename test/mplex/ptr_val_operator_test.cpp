//ptr_val_operator_test.cpp


#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/ptr_val/ptr_val_operator.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/foo_params.hpp>


#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

struct call_test
{
	typedef call_test this_type;
	void operator()(void)
	{
	}

#	define BOOST_PP_LOCAL_MACRO(__n__) \
	template< typename Handler, YGGR_PP_FOO_TYPES_DEF(__n__) > \
	const this_type& operator()(const Handler& handler, YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_CREF_PARAMS)) const{ \
		std::cout << (handler(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA))) << std::endl; \
		return *this; }
#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME
};

typedef yggr::shared_ptr<int> ptr_type;

int main(int argc, char* argv[])
{
	ptr_type ptr1(new int(1));
	ptr_type ptr2(new int(2));

	call_test test;
	test(yggr::ptr_val::ptr_val_negate<ptr_type>(), ptr1)								// -1
		(yggr::ptr_val::ptr_val_plus<ptr_type, ptr_type>(), ptr1, ptr2)					// 3
		(yggr::ptr_val::ptr_val_minus<ptr_type, ptr_type>(), ptr1, ptr2)				// -1
		(yggr::ptr_val::ptr_val_multiplies<ptr_type, ptr_type>(), ptr1, ptr2)			// 2
		(yggr::ptr_val::ptr_val_divides<ptr_type, ptr_type>(), ptr1, ptr2)				// 0
		(yggr::ptr_val::ptr_val_modulus<ptr_type>(), ptr1, ptr2)						// 1

		(yggr::ptr_val::ptr_val_less<ptr_type, ptr_type>(), ptr1, ptr2)					// 1
		(yggr::ptr_val::ptr_val_less_equal<ptr_type, ptr_type>(), ptr1, ptr2)			// 1
		(yggr::ptr_val::ptr_val_greater<ptr_type, ptr_type>(), ptr1, ptr2)				// 0
		(yggr::ptr_val::ptr_val_greater_equal<ptr_type, ptr_type>(), ptr1, ptr2)		// 0
		(yggr::ptr_val::ptr_val_equal_to<ptr_type, ptr_type>(), ptr1, ptr2)				// 0
		(yggr::ptr_val::ptr_val_not_equal_to<ptr_type, ptr_type>(), ptr1, ptr2)			// 1
		(yggr::ptr_val::ptr_val_logical_and<ptr_type, ptr_type>(), ptr1, ptr2)			// 1
		(yggr::ptr_val::ptr_val_logical_or<ptr_type, ptr_type>(), ptr1, ptr2)			// 1
		(yggr::ptr_val::ptr_val_logical_not<ptr_type>(), ptr1)							// 0
		;

	wait_any_key(argc, argv);
	return 0;
}
