// geo_polygon_test.cpp

#include <iostream>
#include <yggr/base/yggrdef.h>
#include <yggr/math/vector2d.hpp>
#include <yggr/math/vector3d.hpp>
#include <yggr/math/matrix2d.hpp>
#include <yggr/math/matrix3d.hpp>

#include <yggr/geometry/geo_polygon.hpp>

#if _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

void test_polygon_2d(void)
{
	typedef yggr::math::vector2d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;

	polygon_type polygon(pos_type(0, 0), pos_type(100, 0), pos_type(0, 100));

	for(polygon_citer_type i = polygon.begin(), isize = polygon.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	std::cout << "prop = " << polygon.prop() << std::endl;
	std::cout << " == " << (polygon == polygon) << std::endl;
	std::cout << " != " << (polygon != polygon) << std::endl;
	std::cout << " is_eligibility " << polygon.is_eligibility() << std::endl;
}

void test_polygon_3d(void)
{
	typedef yggr::math::vector3d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;

	polygon_type polygon(pos_type(0, 0, 0), pos_type(100, 0, 0), pos_type(0, 100, 0));

	for(polygon_citer_type i = polygon.begin(), isize = polygon.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	std::cout << "prop = " << polygon.prop() << std::endl;
	std::cout << " == " << (polygon == polygon) << std::endl;
	std::cout << " != " << (polygon != polygon) << std::endl;
	std::cout << " is_eligibility " << polygon.is_eligibility() << std::endl;
}

void test_polygon_2d_2(void)
{
	typedef yggr::math::vector2d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;

	polygon_type polygon(pos_type(0, 0), pos_type(100, 0), pos_type(100, 100), pos_type(0, 100));
	polygon_type polygon2(pos_type(0, 0), pos_type(100, 0), pos_type(0, 100), pos_type(100, 100));
	std::cout << " is_eligibility1 " << polygon.is_eligibility() << std::endl;
	std::cout << " is_eligibility2 " << polygon2.is_eligibility() << std::endl;
}

void test_polygon_3d_2(void)
{
	typedef yggr::math::vector3d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;

	polygon_type polygon(pos_type(0, 0, 0), pos_type(100, 0, 0), pos_type(100, 100, 0), pos_type(0, 100, 0));
	polygon_type polygon2(pos_type(0, 0, 0), pos_type(100, 0, 0), pos_type(0, 100, 0), pos_type(100, 100, 0));

	std::cout << " is_eligibility1 " << polygon.is_eligibility() << std::endl;
	std::cout << " is_eligibility2 " << polygon2.is_eligibility() << std::endl;
}

void test_polygon_2d_3(void)
{
	typedef yggr::math::vector2d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;

	polygon_type polygon(pos_type(0, 0), pos_type(100, 0), pos_type(100, 100), pos_type(0, 100));
	polygon_type polygon2(pos_type(0, 0), pos_type(100, 0), pos_type(0, 100), pos_type(100, 100));
	std::cout << " is_in_it1 " << polygon.is_in_it(pos_type(50, 50)) << std::endl; // 1
	std::cout << " is_in_it2 " << polygon2.is_in_it(pos_type(50, 50)) << std::endl; // 0
}

void test_polygon_3d_3(void)
{
	typedef yggr::math::vector3d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;

	polygon_type polygon(pos_type(0, 0, 0), pos_type(100, 0, 0), pos_type(100, 100, 0), pos_type(0, 100, 0));
	polygon_type polygon2(pos_type(0, 0, 0), pos_type(100, 0, 0), pos_type(0, 100, 0), pos_type(100, 100, 0));

	std::cout << " is_in_it1 " << polygon.is_in_it(pos_type(50, 50, 0)) << std::endl; // 1
	std::cout << " is_in_it2 " << polygon2.is_in_it(pos_type(50, 50, 0)) << std::endl; // 0

	std::cout << " is_in_it1_2 " << polygon.is_in_it(pos_type(50, 50, 1)) << std::endl; // 0
	std::cout << " is_in_it2_2 " << polygon2.is_in_it(pos_type(50, 50, 1)) << std::endl; // 0
}

void test_polygon_2d_4(void)
{
	typedef yggr::math::vector2d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;

	polygon_type polygon(pos_type(0, 0), pos_type(100, 0), pos_type(100, 100), pos_type(0, 100));
	polygon_type polygon2(pos_type(100, 0), pos_type(100, 100), pos_type(0, 100), pos_type(0, 0));
	std::cout << " == " << (polygon == polygon2) << std::endl; // 1
	std::cout << " != " << (polygon != polygon2) << std::endl; // 0
}

void test_polygon_3d_4(void)
{
	typedef yggr::math::vector3d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;

	polygon_type polygon(pos_type(0, 0, 0), pos_type(100, 0, 0), pos_type(100, 100, 0), pos_type(0, 100, 0));
	polygon_type polygon2(pos_type(100, 0, 0), pos_type(100, 100, 0), pos_type(0, 100, 0), pos_type(0, 0, 0));

	std::cout << " == " << (polygon == polygon2) << std::endl; // 1
	std::cout << " != " << (polygon != polygon2) << std::endl; // 0
}

void test_polygon_2d_5(void)
{
	typedef yggr::math::vector2d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;

	polygon_type polygon(pos_type(0, 0), pos_type(100, 0), pos_type(100, 100), pos_type(0, 100));
	polygon_type polygon2(pos_type(99, 1), pos_type(99, 99), pos_type(1, 99), pos_type(1, 1));
	polygon_type polygon3(pos_type(100, 0), pos_type(100, 100), pos_type(0, 100), pos_type(0, 0));

	polygon_type polygon4(pos_type(-1, -1), pos_type(101, -1), pos_type(101, 101), pos_type(-1, 101));

	std::cout << " is_encircling12 " << polygon.is_encircling(polygon2) << std::endl; // 1
	std::cout << " is_encircling13 " << polygon.is_encircling(polygon3) << std::endl; // 1
	std::cout << " is_encircling14 " << polygon.is_encircling(polygon4) << std::endl; // 0
}

void test_polygon_3d_5(void)
{
	typedef yggr::math::vector3d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;

	polygon_type polygon(pos_type(0, 0, 0), pos_type(100, 0, 0), pos_type(100, 100, 0), pos_type(0, 100, 0));
	polygon_type polygon2(pos_type(99, 1, 0), pos_type(99, 99, 0), pos_type(1, 99, 0), pos_type(1, 1, 0));
	polygon_type polygon3(pos_type(100, 0, 0), pos_type(100, 100, 0), pos_type(0, 100, 0), pos_type(0, 0, 0));

	polygon_type polygon4(pos_type(-1, -1, 0), pos_type(101, -1, 0), pos_type(101, 101, 0), pos_type(-1, 101, 0));
	polygon_type polygon5(pos_type(99, 1, 1), pos_type(99, 99, 1), pos_type(1, 99, 0), pos_type(1, 1, 0));

	std::cout << " is_encircling12 " << polygon.is_encircling(polygon2) << std::endl; // 1
	std::cout << " is_encircling13 " << polygon.is_encircling(polygon3) << std::endl; // 1
	std::cout << " is_encircling14 " << polygon.is_encircling(polygon4) << std::endl; // 0
	std::cout << " is_encircling15 " << polygon.is_encircling(polygon5) << std::endl; // 0
}

void test_polygon_2d_6(void)
{
	typedef yggr::math::vector2d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;
	typedef yggr::math::matrix2d<yggr::s32> mat_type;


	polygon_type polygon(pos_type(0, 0), pos_type(100, 0), pos_type(0, 100));

	polygon *= mat_type::make_translate(100, 100);

	for(polygon_citer_type i = polygon.begin(), isize = polygon.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	polygon_type polygon2(pos_type(0, 0), pos_type(100, 0), pos_type(0, 100));
	polygon2 = polygon2 * mat_type::make_translate(100, 100);

	assert(polygon == polygon2);
}

void test_polygon_3d_6(void)
{
	typedef yggr::math::vector3d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_polygon<pos_type> polygon_type;
	typedef polygon_type::const_iterator polygon_citer_type;
	typedef yggr::math::matrix3d<yggr::s32> mat_type;

	polygon_type polygon(pos_type(0, 0, 0), pos_type(100, 0, 0), pos_type(0, 100, 0));

	polygon *= mat_type::make_translate(100, 100, 100);

	for(polygon_citer_type i = polygon.begin(), isize = polygon.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	polygon_type polygon2(pos_type(0, 0, 0), pos_type(100, 0, 0), pos_type(0, 100, 0));
	polygon2 = polygon2 * mat_type::make_translate(100, 100, 100);

	assert(polygon == polygon2);
}

int main(int argc, char* argv[])
{
	test_polygon_2d();
	std::cout << "-----------------------------" << std::endl;
	test_polygon_3d();

	test_polygon_2d_2();
	std::cout << "-----------------------------" << std::endl;
	test_polygon_3d_2();

	test_polygon_2d_3();
	std::cout << "-----------------------------" << std::endl;
	test_polygon_3d_3();

	test_polygon_2d_4();
	std::cout << "-----------------------------" << std::endl;
	test_polygon_3d_4();

	test_polygon_2d_5();
	std::cout << "-----------------------------" << std::endl;
	test_polygon_3d_5();

	test_polygon_2d_6();
	std::cout << "-----------------------------" << std::endl;
	test_polygon_3d_6();

	char cc = 0;
	std::cin >> cc;
	return 0;
}

