//geo_vector2d_test.cpp

#define YGGR_USE_FAST_MATH

#include <iostream>
#include <boost/bind.hpp>
#include <algorithm>
#include <yggr/math/vector2d.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

typedef yggr::math::vector2d<int> vec_i_type;

template<typename OVec>
vec_i_type conver_to_vec_i_type(const OVec& right)
{
	typedef OVec ovec_type;
	vec_i_type vec;
	for(int i = 0, isize = std::min<int>(ovec_type::E_LENGTH, vec_i_type::E_LENGTH); i != isize; ++i)
	{
		vec[i] = right[i];
	}

	return vec;
}

int main(int argc, char* argv[])
{
	typedef yggr::math::vector2d<float> vec_type;
	typedef yggr::math::vector2d<double> vec_d_type;

	//create_test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << vec1 << std::endl;

#ifdef _MSC_VER
		std::wcout.imbue(std::locale("chs"));
		std::wcout << vec1 << std::endl;
#endif // _MSC_VER
		std::cout << "------------create_test_end------------------------------" << std::endl;
	}

	//[] test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec1.x = 40;
		vec1.y = 50;

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------[]_test_end------------------------------" << std::endl;
	}

	//+ test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 = vec1 + vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------+_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 = vec1 + vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------+_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec1 = vec1 + 100;

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------+_test_3_end------------------------------" << std::endl;
	}

	//+= test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 += vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------+=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 += vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------+=_test_2_end------------------------------" << std::endl;
	}

	//{
	//	vec_type vec1(10, 20);
	//	vec1 += 100;

	//	std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
	//	std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
	//	std::cout << "------------+=_test_3_end------------------------------" << std::endl;
	//}

	//- test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 = vec1 - vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ -_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 = vec1 - vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ -_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec1 = vec1 - 100;

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ -_test_3_end------------------------------" << std::endl;
	}

	//-= test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 -= vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ -=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 -= vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ -=_test_2_end------------------------------" << std::endl;
	}

	//{
	//	vec_type vec1(10, 20);
	//	vec1 -= 100;

	//	std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
	//	std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
	//	std::cout << "------------ -=_test_3_end------------------------------" << std::endl;
	//}

	//* test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 = vec1 * vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 = vec1 * vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec1 = vec1 * 100;
		vec_type vec2 = 100 * vec1;

		assert((vec1 * 100) == vec2);

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *_test_3_end------------------------------" << std::endl;
	}

	//*= test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 *= vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 *= vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *=_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec1 *= 100;

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *=_test_3_end------------------------------" << std::endl;
	}

	// / test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 = vec1 / vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 = vec1 / vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec1 = vec1 / 100;

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /_test_3_end------------------------------" << std::endl;
	}

	// /= test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 /= vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 /= vec2;
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /=_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec1 /= 100;

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /=_test_3_end------------------------------" << std::endl;
	}

	// == test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);
		vec_type vec3(10, 20);

		std::cout << (vec1 == vec2) << "," << (vec1 == vec3) << std::endl;
		std::cout << "------------ ==_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);
		vec_d_type vec3(10, 20);

		std::cout << (vec1 == vec2) << "," << (vec1 == vec3) << std::endl;
		std::cout << "------------ ==_test_2_end------------------------------" << std::endl;
	}

	// != test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);
		vec_type vec3(10, 20);

		std::cout << (vec1 != vec2) << "," << (vec1 != vec3) << std::endl;
		std::cout << "------------ !=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);
		vec_d_type vec3(10, 20);

		std::cout << (vec1 != vec2) << "," << (vec1 != vec3) << std::endl;
		std::cout << "------------ !=_test_2_end------------------------------" << std::endl;
	}

	// round floor ceil test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10.5, 20.4);
		vec1.round();

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
	}

	{
		vec_type vec1(10.5, 20.4);
		vec1.ceil();

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
	}

	{
		vec_type vec1(10.5, 20.4);
		vec1.floor();

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;

		std::cout << "------------ round floor ceil_test_end------------------------------" << std::endl;
	}

	// -val test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10.5, 20.4);
		vec1 = -vec1;

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;

		std::cout << "------------ -val_test_end------------------------------" << std::endl;
	}

	// cast_test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10.5, 20.4);
		vec_d_type vec2 = vec1.type_cast<vec_d_type::val_type>();
		std::cout << vec2.x << ", " << vec2.y << ", " << vec2.w << std::endl;
	}

	{
		vec_type vec1(10.5, 20.4);
		vec_i_type vec2 = vec1.type_cast<vec_i_type>(boost::bind(&conver_to_vec_i_type<vec_type>, _1));
		std::cout << vec2.x << ", " << vec2.y << ", " << vec2.w << std::endl;

		std::cout << "------------ cast_test_end------------------------------" << std::endl;
	}

	//length length_sq test
	{
		vec_type vec1(1, 1);
		std::cout << vec1.length_sq() << "," << vec1.length() << std::endl;
		std::cout << "------------ length length_sq_end------------------------------" << std::endl;
	}

	// dot_test
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);
		std::cout << vec1.dot(vec2) << std::endl;
		std::cout << "------------ dot_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);
		std::cout << vec1.dot(vec2) << std::endl;
		std::cout << "------------ dot_test_2_end------------------------------" << std::endl;
	}
	// cross_test
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);
		std::cout << vec1.cross(vec2) << std::endl;
		std::cout << "------------ cross_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);
		std::cout << vec1.cross(vec2) << std::endl;
		std::cout << "------------ cross_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_type vec2(10, 20);

		vec_type vec3(100, 200);
		vec_type vec4(100, 200);

		vec1.swap(vec3);
		boost::swap(vec2, vec4);

		assert(vec1 == vec2);
		assert(vec3 == vec4);
		std::cout << "------------ swap_test_end------------------------------" << std::endl;
	}

	char cc = 0;
	std::cin >> cc;

	return 0;
}

