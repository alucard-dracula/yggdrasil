//geo_rectangle_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/math/vector2d.hpp>
#include <yggr/math/vector3d.hpp>
#include <yggr/math/matrix2d.hpp>
#include <yggr/math/matrix3d.hpp>

#include <yggr/geometry/geo_rectangle.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void test_rect_2d(void)
{
	typedef yggr::math::vector2d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_rectangle<pos_type> rect_type;


	rect_type rect(pos_type(100, 200), pos_type(200, 100));

	std::cout << rect.min_pos << "," << rect.max_pos << std::endl;
	std::cout << rect.is_rectangle() << std::endl;
	std::cout << rect.is_box() << std::endl;

	typedef yggr::math::vector2d<yggr::f32> f32_pos_type;
	rect_type rect2(f32_pos_type(100, 200), f32_pos_type(200, 100));
	std::cout << rect2.min_pos << "," << rect2.max_pos << std::endl;
}

void test_rect_3d(void)
{
	typedef yggr::math::vector3d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_rectangle<pos_type> rect_type;


	rect_type rect(pos_type(100, 200, 300), pos_type(200, 100, 100));

	std::cout << rect.min_pos << "," << rect.max_pos << std::endl;
	std::cout << rect.is_rectangle() << std::endl;
	std::cout << rect.is_box() << std::endl;

	//BOOST_MPL_ASSERT compile error
	//typedef yggr::math::vector2d<yggr::f32> f32_pos_type;
	//rect_type rect2(f32_pos_type(100, 200), f32_pos_type(200, 100));
	//std::cout << rect2.min_pos << "," << rect2.max_pos << std::endl;

	typedef yggr::math::vector3d<yggr::f32> f32_pos_type;
	rect_type rect2(f32_pos_type(100, 200, 300), f32_pos_type(200, 100, 100));
	std::cout << rect2.min_pos << "," << rect2.max_pos << std::endl;
}

void test_rect_2d_pos_foo(void)
{
	typedef yggr::math::vector2d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_rectangle<pos_type> rect_type;

	rect_type rect(pos_type(100, 200), pos_type(200, 100));

	std::cout << rect.min_pos << "," << rect.max_pos << std::endl;
	std::cout << (rect.vertex<0, 0>()) << std::endl;
	std::cout << (rect.vertex<0, 1>()) << std::endl;
	std::cout << (rect.vertex<1, 0>()) << std::endl;
	std::cout << (rect.vertex<1, 1>()) << std::endl;


	//std::cout << (rect.vertex<0, 0, 0>()) << std::endl; // BOOST_MPL_ASSERT compile error
}

void test_rect_3d_pos_foo(void)
{
	typedef yggr::math::vector3d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_rectangle<pos_type> rect_type;

	rect_type rect(pos_type(100, 200, 300), pos_type(200, 100, 100));

	std::cout << rect.min_pos << "," << rect.max_pos << std::endl;
	std::cout << (rect.vertex<0, 0, 0>()) << std::endl;
	std::cout << (rect.vertex<0, 1, 0>()) << std::endl;
	std::cout << (rect.vertex<1, 0, 0>()) << std::endl;
	std::cout << (rect.vertex<1, 1, 0>()) << std::endl;

	std::cout << (rect.vertex<0, 0, 1>()) << std::endl;
	std::cout << (rect.vertex<0, 1, 1>()) << std::endl;
	std::cout << (rect.vertex<1, 0, 1>()) << std::endl;
	std::cout << (rect.vertex<1, 1, 1>()) << std::endl;


	//std::cout << (rect.vertex<0, 0>()) << std::endl; // BOOST_MPL_ASSERT compile error
}

void test_rect_2d_mul_matrix(void)
{
	typedef yggr::math::vector2d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_rectangle<pos_type> rect_type;
	typedef yggr::math::matrix2d<yggr::s32> mat_type;

	rect_type rect(pos_type(100, 200), pos_type(200, 100));
	rect_type rst_rect = rect * mat_type::make_translate(100, 100);

	std::cout << rst_rect.min_pos << "," << rst_rect.max_pos << std::endl;

	rect_type rect2(pos_type(100, 200), pos_type(200, 100));
	rect2 *= mat_type::make_translate(100, 100);

	yggr_test_assert(rst_rect == rect2);

}

void test_rect_3d_mul_matrix(void)
{
	typedef yggr::math::vector3d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_rectangle<pos_type> rect_type;
	typedef yggr::math::matrix3d<yggr::s32> mat_type;

	rect_type rect(pos_type(100, 200, 300), pos_type(200, 100, 100));
	rect_type rst_rect = rect * mat_type::make_translate(100, 100, 100);

	std::cout << rst_rect.min_pos << "," << rst_rect.max_pos << std::endl;

	rect_type rect2(pos_type(100, 200, 300), pos_type(200, 100, 100));
	rect2 *= mat_type::make_translate(100, 100, 100);
	yggr_test_assert(rst_rect == rect2);
}

#include <vector>
int main(int argc, char* argv[])
{
	test_rect_2d();
	std::cout << "----------------------------------------" << std::endl;
	test_rect_3d();

	test_rect_2d_pos_foo();
	std::cout << "----------------------------------------" << std::endl;
	test_rect_3d_pos_foo();

	test_rect_2d_mul_matrix();
	std::cout << "----------------------------------------" << std::endl;
	test_rect_3d_mul_matrix();

	wait_any_key(argc, argv);
	return 0;
}
