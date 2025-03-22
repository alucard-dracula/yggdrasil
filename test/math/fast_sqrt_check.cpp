//fast_sqrt_check.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#define YGGR_FAST_MATH_ITERATE_TWICE

#include <yggr/math/sqrt.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

int main(int argc, char* argv[])
{
	{
		yggr::f32 fsqrt, sqrt;
		for(yggr::f32 i = 1; i != 10; ++i)
		{
			std::cout 
				<< "fsqut = " << (fsqrt = yggr::math::fast_sqrt(i)) 
				<< ", sqrt = " << (sqrt = std::sqrt(i))
				<< ", miss = " << std::abs(fsqrt - sqrt) << std::endl;
		}
	}

	std::cout << "--------------" << std::endl;

	{
		yggr::f64 fsqrt, sqrt;
		for(yggr::f64 i = 1; i != 10; ++i)
		{
			std::cout 
				<< "fsqut = " << (fsqrt = yggr::math::fast_sqrt(i)) 
				<< ", sqrt = " << (sqrt = std::sqrt(i))
				<< ", miss = " << std::abs(fsqrt - sqrt) << std::endl;
		}
	}

	std::cout << "--------------" << std::endl;

	{
		yggr::f32 frsqrt, rsqrt;
		for(yggr::f32 i = 1; i != 10; ++i)
		{
			std::cout 
				<< "frsqut = " << (frsqrt = yggr::math::fast_rsqrt(i)) 
				<< ", rsqrt = " << (rsqrt = 1.0f / std::sqrt(i))
				<< ", miss = " << std::abs(frsqrt - rsqrt) << std::endl;
		}
	}

	std::cout << "--------------" << std::endl;

	{
		yggr::f64 frsqrt, rsqrt;
		for(yggr::f64 i = 1; i != 10; ++i)
		{
			std::cout 
				<< "frsqut = " << (frsqrt = yggr::math::fast_rsqrt(i)) 
				<< ", rsqrt = " << (rsqrt = 1.0 / std::sqrt(i))
				<< ", miss = " << std::abs(frsqrt - rsqrt) << std::endl;
		}
	}

	wait_any_key(argc, argv);
	return 0;
}