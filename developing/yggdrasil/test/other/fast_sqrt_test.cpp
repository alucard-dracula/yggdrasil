//fast_sqrt_test.cpp

#include <iostream>
#include <complex>
#include <yggr/base/yggrdef.h>
#include <yggr/math/sqrt.hpp>

template<typename T>
yggr::f32 sqrt(const T& t)
{
	yggr::f32 x = t;

	union
	{
		yggr::s32 intPart;
		yggr::f32 floatPart;
	} convertor;

	union
	{
		yggr::s32 intPart;
		yggr::f32 floatPart;
	} convertor2;

	convertor.floatPart = x;
	convertor2.floatPart = x;
	convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
	convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
	return 0.5f * (convertor.floatPart + (x * convertor2.floatPart));
}

inline float t_sqrtF( const float& x )
{
	float xHalf = 0.5f * x;
	int tmp = 0x5F3759DF - ( *(int*)&x >> 1 ); //initial guess
	float xRes = *(float*)&tmp;
	
	xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );
	//xRes *= ( 1.5f - ( xHalf * xRes * xRes ) );//コメントアウトを翌すと娼業が貧がる
	return xRes * x;
}

inline double t_sqrtD( const double &x) 
{
	double xHalf = 0.5 * x;
	long long int tmp = 0x5FE6EB50C7B537AAl - ( *(long long int*)&x >> 1);//initial guess
	double xRes = * (double*)&tmp;
	
	xRes *= ( 1.5 - ( xHalf * xRes * xRes ) );
	//xRes *= ( 1.5 - ( xHalf * xRes * xRes ) );//コメントアウトを翌すと娼業が貧がる
	return xRes * x;
}

void test_float(void)
{
	float f1 = 0.0f;
	float f2 = 0.0f;
	float f3 = 0.0f;

	float fm13 = 0.0f;
	float fm23 = 0.0f;

	double size = 0;
	double count = 10000000;

	double fm13_count_miss = 0;
	double fm13_max_miss = 0;
	double fm13_min_miss = 9999999999;

	double fm23_count_miss = 0;
	double fm23_max_miss = 0;
	double fm23_min_miss = 9999999999;

	for(float i = 0; i != count; ++i)
	{
		f1 = t_sqrtF(i);
		//f2 = sqrt(i);
		f2 = yggr::math::sqrt(i);
		f3 = std::sqrt(i);

		//std::cout << "value: "<< f1 << " | " << f2 << " | "<< f3 << std::endl;
		fm13 = std::abs(f1 - f3);
		fm23 = std::abs(f2 - f3);
		//std::cout << "miss: "<< fm13 << " | " << fm23 << " | "<< (fm13 < fm23) << std::endl;
		//std::cout << std::endl;

		fm13_count_miss += fm13;
		if(fm13_max_miss < fm13)
		{
			fm13_max_miss = fm13;
		}

		if(fm13_min_miss > fm13)
		{
			fm13_min_miss = fm13;
		}

		fm23_count_miss += fm23;
		if(fm23_max_miss < fm23)
		{
			fm23_max_miss = fm23;
		}

		if(fm23_min_miss > fm23)
		{
			fm23_min_miss = fm23;
		}

		if(!(fm13 < fm23))
		{
			++size;

			
			//std::cout << "idx: " << i << std::endl;
			//std::cout << "value: "<< f1 << " | " << f2 << " | "<< f3 << std::endl;
			//std::cout << "miss: "<< fm13 << " | " << fm23 << " | "<< (fm13 < fm23) << std::endl;
			//std::cout << std::endl;
		}
	}

	std::cout << "fm13 < fm23 size: " << size << " | less rate: " << (size / count) * 100 << "%"<< std::endl;
	std::cout << "fm13 fm13_min_miss: " << fm13_min_miss << " | fm13_max_miss: " << fm13_max_miss << " | fm13_avg_miss: " << fm13_count_miss / count << std::endl;
	std::cout << "fm23 fm23_min_miss: " << fm23_min_miss << " | fm23_max_miss: " << fm23_max_miss << " | fm23_avg_miss: " << fm23_count_miss / count << std::endl;
	std::cout << "test end !!!!!" << std::endl; 

}

void test_double(void)
{
	double f1 = 0.0f;
	double f2 = 0.0f;
	double f3 = 0.0f;

	double fm13 = 0.0f;
	double fm23 = 0.0f;

	double size = 0;
	double count = 10000000;

	double fm13_count_miss = 0;
	double fm13_max_miss = 0;
	double fm13_min_miss = 9999999999;

	double fm23_count_miss = 0;
	double fm23_max_miss = 0;
	double fm23_min_miss = 9999999999;

	for(double i = 0; i != count; ++i)
	{
		f1 = t_sqrtD(i);
		//f2 = sqrt(i);
		f2 = yggr::math::sqrt(i);
		f3 = std::sqrt(i);

		//std::cout << "value: "<< f1 << " | " << f2 << " | "<< f3 << std::endl;
		fm13 = std::abs(f1 - f3);
		fm23 = std::abs(f2 - f3);
		//std::cout << "miss: "<< fm13 << " | " << fm23 << " | "<< (fm13 < fm23) << std::endl;
		//std::cout << std::endl;

		fm13_count_miss += fm13;
		if(fm13_max_miss < fm13)
		{
			fm13_max_miss = fm13;
		}

		if(fm13_min_miss > fm13)
		{
			fm13_min_miss = fm13;
		}

		fm23_count_miss += fm23;
		if(fm23_max_miss < fm23)
		{
			fm23_max_miss = fm23;
		}

		if(fm23_min_miss > fm23)
		{
			fm23_min_miss = fm23;
		}

		if(!(fm13 < fm23))
		{
			++size;

			
			//std::cout << "idx: " << i << std::endl;
			//std::cout << "value: "<< f1 << " | " << f2 << " | "<< f3 << std::endl;
			//std::cout << "miss: "<< fm13 << " | " << fm23 << " | "<< (fm13 < fm23) << std::endl;
			//std::cout << std::endl;
		}
	}

	std::cout << "fm13 < fm23 size: " << size << " | less rate: " << (size / count) * 100 << "%"<< std::endl;
	std::cout << "fm13 fm13_min_miss: " << fm13_min_miss << " | fm13_max_miss: " << fm13_max_miss << " | fm13_avg_miss: " << fm13_count_miss / count << std::endl;
	std::cout << "fm23 fm23_min_miss: " << fm23_min_miss << " | fm23_max_miss: " << fm23_max_miss << " | fm23_avg_miss: " << fm23_count_miss / count << std::endl;
	std::cout << "test end !!!!!" << std::endl; 

}

#include <yggr/time/runtime_speed_test.hpp>
#include <yggr/time/time.hpp>
template<typename Handler>
void test_time(Handler h)
{
	yggr::time::runtime_speed_test<yggr::time::time> test;
	double count = 1000000;
	for(double i = 0; i != count; ++i)
	{
		((*h)(i));
		 //std::cout << ((*h)(i)) << std::endl;
	}
}

int main(int argc, char* argv[])
{
	/*test_float();
	std::cout << "------------------------------" << std::endl;
	test_double();*/

	test_time(std::sqrt<double>);
	test_time(yggr::math::sqrt<double>);

	char cc = 0;
	std::cin >> cc;
	return 0;
}