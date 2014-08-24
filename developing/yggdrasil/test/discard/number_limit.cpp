//number_limit.cpp

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/compile_cal/number_limit.hpp>
#include <limits>
#include <assert.h>

#define CHECK_RIGHT_ASSERT(__type__) \
	assert(std::numeric_limits< __type__ >::min() == yggr::compile_cal::number_limit< __type__ >::min_value); \
	assert(std::numeric_limits< __type__ >::max() == yggr::compile_cal::number_limit< __type__ >::max_value);


int main(int argc, char* argv[])
{
	CHECK_RIGHT_ASSERT(yggr::s8)
	CHECK_RIGHT_ASSERT(yggr::u8)
	CHECK_RIGHT_ASSERT(yggr::s16)
	CHECK_RIGHT_ASSERT(yggr::u16)
	CHECK_RIGHT_ASSERT(yggr::s32)
	CHECK_RIGHT_ASSERT(yggr::u32)
	//CHECK_RIGHT_ASSERT(yggr::s64)
	//CHECK_RIGHT_ASSERT(yggr::u64)
	//CHECK_RIGHT_ASSERT(yggr::f32)
	//CHECK_RIGHT_ASSERT(yggr::f64)

	//assert(yggr::compile_cal::number_limit< bool >::min_value == false);
	//assert(yggr::compile_cal::number_limit< bool >::max_value == true);

	//yggr::s64 s641 = std::numeric_limits< yggr::s64 >::max();
	//yggr::s64 s642 = yggr::compile_cal::number_limit< yggr::s64 >::max_value;

	char cc = 0;
	std::cin >> cc;
	return 0;
}