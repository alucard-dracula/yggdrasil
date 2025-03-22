// test_extern_macro.cpp

#include <iostream>

//#include <bson.h>
#define BSON_INSIDE
//#include <bson-dsl.h>
#include <bson-atomic.h>

#if defined(bsonBuildAppend)
//#	error "has it"
#endif // bsonBuildAppend

#define YGGR_PP_CAST( __x__, __to__ ) __to__( __x__ )

#define YGGR_PP_CAST_TO_STRING( __x__ ) #__x__

#define YGGR_PP_DEBUG( __x__ ) YGGR_PP_CAST( ( __x__ ), YGGR_PP_CAST_TO_STRING )

int main(void)
{

	std::cout << 
		YGGR_PP_DEBUG(
			DEF_ATOMIC_CMPEXCH_WEAK (
         Pointer, , __ATOMIC_SEQ_CST, ptr, expect, new_value) ) 
		<< std::endl;
	
	std::cout << 
		YGGR_PP_DEBUG(
			 DEF_ATOMIC_CMPEXCH_STRONG (Pointer,
									MSVC_MEMORDER_SUFFIX (_nf),
									__ATOMIC_RELAXED,
									ptr,
									expect,
									new_value) ) 
		<< std::endl;

	std::cout << 
		YGGR_PP_DEBUG(
				DEF_ATOMIC_CMPEXCH_STRONG (Pointer,
									MSVC_MEMORDER_SUFFIX (_acq),
									__ATOMIC_CONSUME,
									ptr,
									expect,
									new_value) ) 
		<< std::endl;

	std::cout << 
		YGGR_PP_DEBUG(
				DEF_ATOMIC_CMPEXCH_STRONG (Pointer,
									MSVC_MEMORDER_SUFFIX (_acq),
									__ATOMIC_ACQUIRE,
									ptr,
									expect,
									new_value) ) 
		<< std::endl;
	char cc = 0;
	std::cin >> cc;
	return 0;
}