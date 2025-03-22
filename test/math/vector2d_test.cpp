//geo_vector2d_test.cpp

#define YGGR_USE_FAST_MATH

#include <test/wait_any_key/wait_any_key.hpp>
#include <algorithm>

#include <boost/bind.hpp>

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

#ifndef YGGR_NO_CWCHAR
	TEST_PP_WIO_IMBUE("chs");
#endif // YGGR_NO_CWCHAR

	//create_test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << vec1 << std::endl;

#ifndef YGGR_NO_CWCHAR
		std::wcout << vec1 << std::endl;
#endif // #ifndef YGGR_NO_CWCHAR
		std::cout << "------------create_test_end------------------------------" << std::endl;
	}

	//[] test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec1.x = 40;
		vec1.y = 50;

		yggr_test_assert(vec1 == vec_type(40, 50, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------[]_test_end------------------------------" << std::endl;
	}

	//+ test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 = vec1 + vec2;
		yggr_test_assert(vec1 == vec_type(40, 40, 2));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------+_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 = vec1 + vec2;
		yggr_test_assert(vec1 == vec_type(40, 40, 2));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------+_test_2_end------------------------------" << std::endl;
	}

	//+= test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 += vec2;
		yggr_test_assert(vec1 == vec_type(40, 40, 2));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------+=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 += vec2;
		yggr_test_assert(vec1 == vec_type(40, 40, 2));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------+=_test_2_end------------------------------" << std::endl;
	}

	//- test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 = vec1 - vec2;
		yggr_test_assert(vec1 == vec_type(-20, 0, 0));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ -_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 = vec1 - vec2;
		yggr_test_assert(vec1 == vec_type(-20, 0, 0));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ -_test_2_end------------------------------" << std::endl;
	}

	//-= test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 -= vec2;
		yggr_test_assert(vec1 == vec_type(-20, 0, 0));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ -=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 -= vec2;
		yggr_test_assert(vec1 == vec_type(-20, 0, 0));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ -=_test_2_end------------------------------" << std::endl;
	}

	//* test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 = vec1 * vec2;
		yggr_test_assert(vec1 == vec_type(300, 400, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 = vec1 * vec2;
		yggr_test_assert(vec1 == vec_type(300, 400, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec1 = vec1 * 100;
		vec_type vec2 = 100 * vec1;

		yggr_test_assert((vec1 * 100) == vec2);

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *_test_3_end------------------------------" << std::endl;
	}

	//*= test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 *= vec2;
		yggr_test_assert(vec1 == vec_type(300, 400, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 *= vec2;
		yggr_test_assert(vec1 == vec_type(300, 400, 1));


		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *=_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec1 *= 100;

		yggr_test_assert(vec1 == vec_type(1000, 2000, 100));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ *=_test_3_end------------------------------" << std::endl;
	}

	// / test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 = vec1 / vec2;
		yggr_test_assert(vec1 == vec_type(10.0f / 30.0f, 1, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 = vec1 / vec2;
		yggr_test_assert(vec1 == vec_type(10.0f / 30.0f, 1, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec1 = vec1 / 100;

		yggr_test_assert(vec1 == vec_type(10.0f / 100, 20.0f / 100, 1 / 100.0f));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /_test_3_end------------------------------" << std::endl;
	}

	// /= test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 /= vec2;
		yggr_test_assert(vec1 == vec_type(10.0f / 30.0f, 1, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 /= vec2;
		yggr_test_assert(vec1 == vec_type(10.0f / 30.0f, 1, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /=_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec1 /= 100;

		yggr_test_assert(vec1 == vec_type(10.0f / 100.0f, 20.0f / 100.0f, 1.0f / 100.0f));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ /=_test_3_end------------------------------" << std::endl;
	}

	// % test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 = vec1 % vec2;
		yggr_test_assert(vec1 == vec_type(10, 0, 0));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ %_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 = vec1 % vec2;
		yggr_test_assert(vec1 == vec_type(10, 0, 0));


		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ %_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);

		vec1 = vec1 % 100;
		yggr_test_assert(vec1 == vec_type(10, 20, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ %_test_3_end------------------------------" << std::endl;
	}

	// %= test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);

		vec1 %= vec2;
		yggr_test_assert(vec1 == vec_type(10, 0, 0));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ %=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);

		vec1 %= vec2;
		yggr_test_assert(vec1 == vec_type(10, 0, 0));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ %=_test_2_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);

		vec1 %= 100;
		yggr_test_assert(vec1 == vec_type(10, 20, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
		std::cout << vec1[0] << ", " << vec1[1] << ", " << vec1[2] << std::endl;
		std::cout << "------------ %=_test_3_end------------------------------" << std::endl;
	}

	// == test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);
		vec_type vec3(10, 20);

		yggr_test_assert(!(vec1 == vec2));
		yggr_test_assert((vec1 == vec3));

		std::cout << (vec1 == vec2) << "," << (vec1 == vec3) << std::endl;
		std::cout << "------------ ==_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);
		vec_d_type vec3(10, 20);

		yggr_test_assert(!(vec1 == vec2));
		yggr_test_assert((vec1 == vec3));

		std::cout << (vec1 == vec2) << "," << (vec1 == vec3) << std::endl;
		std::cout << "------------ ==_test_2_end------------------------------" << std::endl;
	}

	// != test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10, 20);
		vec_type vec2(30, 20);
		vec_type vec3(10, 20);

		yggr_test_assert((vec1 != vec2));
		yggr_test_assert(!(vec1 != vec3));

		std::cout << (vec1 != vec2) << "," << (vec1 != vec3) << std::endl;
		std::cout << "------------ !=_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20);
		vec_d_type vec2(30, 20);
		vec_d_type vec3(10, 20);

		yggr_test_assert((vec1 != vec2));
		yggr_test_assert(!(vec1 != vec3));

		std::cout << (vec1 != vec2) << "," << (vec1 != vec3) << std::endl;
		std::cout << "------------ !=_test_2_end------------------------------" << std::endl;
	}

	// round floor ceil test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10.5, 20.4);
		
		vec1.round();
		yggr_test_assert(vec1 == vec_type(11, 20, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
	}

	{
		vec_type vec1(10.5, 20.4, 1);

		vec1.ceil();
		yggr_test_assert(vec1 == vec_type(11, 21, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;
	}

	{
		vec_type vec1(10.5, 20.4);

		vec1.floor();
		yggr_test_assert(vec1 == vec_type(10, 20, 1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;

		std::cout << "------------ round floor ceil_test_end------------------------------" << std::endl;
	}

	// -val test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10.5, 20.4);

		vec1 = -vec1;
		yggr_test_assert(vec1 == vec_type(-10.5, -20.4, -1));

		std::cout << vec1.x << ", " <<vec1.y << ", " << vec1.w << std::endl;

		std::cout << "------------ -val_test_end------------------------------" << std::endl;
	}

	// cast_test-----------------------------------------------------------------------------------
	{
		vec_type vec1(10.5, 20.4);
		vec_d_type vec2 = vec1.type_cast<vec_d_type::value_type>();
		std::cout << vec2.x << ", " << vec2.y << ", " << vec2.w << std::endl;

		vec_d_type vec3 = vec1.type_cast<vec_d_type::value_type, boost::array>();
		std::cout << vec3.x << ", " << vec3.y << ", " << vec3.w << std::endl;
	}

	{
		vec_type vec1(10.5, 20.4);
		vec_i_type vec2 = vec1.type_cast<vec_i_type>(boost::bind(&conver_to_vec_i_type<vec_type>, _1));
		std::cout << vec2.x << ", " << vec2.y << ", " << vec2.w << std::endl;

		std::cout << "------------ cast_test_end------------------------------" << std::endl;
	}

	//length length_sq test
	{
		vec_type vec1(1, 1, 0);
		std::cout << vec1.length_sq() << "," << vec1.length() << std::endl;
		std::cout << "------------ length length_sq_end------------------------------" << std::endl;
	}

	// dot_test
	{
		vec_type vec1(10, 20, 0);
		vec_type vec2(30, 20, 0);

		yggr_test_assert(vec1.dot(vec2) == vec1 * dot(vec2));
		yggr_test_assert(vec1.dot(vec2) == dot(vec1) * vec2);
		yggr_test_assert(vec1.dot(vec2) == dot(vec1) * dot(vec2));

		std::cout << vec1.dot(vec2) << std::endl;
		std::cout << "------------ dot_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20, 0);
		vec_d_type vec2(30, 20, 0);

		yggr_test_assert(vec1.dot(vec2) == vec1 * dot(vec2));
		yggr_test_assert(vec1.dot(vec2) == dot(vec1) * vec2);
		yggr_test_assert(vec1.dot(vec2) == dot(vec1) * dot(vec2));

		std::cout << vec1.dot(vec2) << std::endl;
		std::cout << "------------ dot_test_2_end------------------------------" << std::endl;
	}

	// cross_test
	{
		vec_type vec1(10, 20, 0);
		vec_type vec2(30, 20, 0);
		std::cout << vec1.cross(vec2) << std::endl;

		yggr_test_assert(vec1.cross(vec2) == vec1 * cross(vec2));
		yggr_test_assert(vec1.cross(vec2) == cross(vec1) * vec2);
		yggr_test_assert(vec1.cross(vec2) == cross(vec1) * cross(vec2));

		std::cout << "------------ cross_test_1_end------------------------------" << std::endl;
	}

	{
		vec_type vec1(10, 20, 0);
		vec_d_type vec2(30, 20, 0);

		yggr_test_assert(vec1.cross(vec2) == vec1 * cross(vec2));
		yggr_test_assert(vec1.cross(vec2) == cross(vec1) * vec2);
		yggr_test_assert(vec1.cross(vec2) == cross(vec1) * cross(vec2));

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

		yggr_test_assert(vec1 == vec2);
		yggr_test_assert(vec3 == vec4);
		std::cout << "------------ swap_test_end------------------------------" << std::endl;
	}

	wait_any_key(argc, argv);
	return 0;
}

