//geo_line_test.cpp

#define YGGR_USE_FAST_MATH

#include <yggr/base/yggrdef.h>
#include <iostream>
#include <utility>
#include <yggr/math/vector2d.hpp>
#include <yggr/math/vector3d.hpp>
#include <yggr/geometry/geo_line.hpp>
#include <cassert>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void is_parallel_line_2d_test(void)
{
	typedef yggr::math::vector2d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10), vec_type(20, 20));
	line_type line2(vec_type(12, 10), vec_type(22, 20));
	line_type line3(vec_type(12, 10), vec_type(20, 20));

	assert(line1.is_parallel_line(line2) == 1);
	assert(line1.is_parallel_line(line3) == 0);
	std::cout << line1.is_parallel_line(line2)/*1*/ << ", " << line1.is_parallel_line(line3) /*0*/<< std::endl;
	std::cout << "-------is_parallel_line_2d_test end----------" << std::endl;
}

void is_parallel_line_3d_test(void)
{
	typedef yggr::math::vector3d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10, 0), vec_type(20, 20, 0));
	line_type line2(vec_type(12, 10, 0), vec_type(22, 20, 0));
	line_type line3(vec_type(12, 10, 0), vec_type(20, 20, 0));

	line_type line4(vec_type(20, 10, 10), vec_type(10, 20, 10));

	assert(line1.is_parallel_line(line2) == 1);
	assert(line1.is_parallel_line(line3) == 0);
	assert(line1.is_parallel_line(line4) == 0);

	std::cout << line1.is_parallel_line(line2)/*1*/ << ", " << line1.is_parallel_line(line3) /*0*/<< std::endl;
	std::cout << line1.is_parallel_line(line4)/*0*/ << std::endl;
	std::cout << "------is_parallel_line_3d_test end----------"  << std::endl;
}

void is_in_it_of_point_2d_test(void)
{
	typedef yggr::math::vector2d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10), vec_type(20, 20));
	vec_type vec1(15, 15);
	vec_type vec2(15, 16);

	assert(line1.is_in_it(vec1) == 1);
	assert(line1.is_in_it(vec2) == 0);
	assert(line_type::is_line(line1.src_pos, line1.dst_pos, vec1) == 1);
	assert(line_type::is_line(line1.src_pos, line1.dst_pos, vec2) == 0);

	std::cout << line1.is_in_it(vec1)/*1*/ << ", " << line1.is_in_it(vec2) /*0*/<< std::endl;
	std::cout << line_type::is_line(line1.src_pos, line1.dst_pos, vec1) /*1*/
				<< ", "
				<< line_type::is_line(line1.src_pos, line1.dst_pos, vec2) /*0*/  << std::endl;
	std::cout << "-------is_in_it_of_point_2d_test end----------" << std::endl;
}

void is_in_it_of_point_3d_test(void)
{
	typedef yggr::math::vector3d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10, 0), vec_type(20, 20, 0));
	vec_type vec1(15, 15, 0);
	vec_type vec2(15, 16, 0);
	vec_type vec3(15, 15, 10);

	assert(line1.is_in_it(vec1) == 1);
	assert(line1.is_in_it(vec2) == 0);
	assert(line1.is_in_it(vec3) == 0);
	assert(line_type::is_line(line1.src_pos, line1.dst_pos, vec1) == 1);
	assert(line_type::is_line(line1.src_pos, line1.dst_pos, vec2) == 0);

	std::cout << line1.is_in_it(vec1)/*1*/ << ", " << line1.is_in_it(vec2) /*0*/<< std::endl;
	std::cout << line1.is_in_it(vec3)/*0*/ << std::endl;
	std::cout << line_type::is_line(line1.src_pos, line1.dst_pos, vec1) /*1*/
				<< ", "
				<< line_type::is_line(line1.src_pos, line1.dst_pos, vec2) /*0*/  << std::endl;
	std::cout << "------is_in_it_of_point_3d_test end----------"  << std::endl;
}

void is_perpendicular_line_2d_test(void)
{
	typedef yggr::math::vector2d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10), vec_type(20, 10));
	line_type line2(vec_type(20, 10), vec_type(20, 20));
	line_type line3(vec_type(12, 10), vec_type(20, 20));

	assert(line1.is_perpendicular_line(line2) == 1);
	assert(line1.is_perpendicular_line(line3) == 0);

	std::cout << line1.is_perpendicular_line(line2)/*1*/ << ", " << line1.is_perpendicular_line(line3) /*0*/<< std::endl;
	std::cout << "-------is_perpendicular_line_2d_test end----------" << std::endl;
}

void is_perpendicular_line_3d_test(void)
{
	typedef yggr::math::vector3d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10, 0), vec_type(20, 10, 0));
	line_type line2(vec_type(20, 10, 0), vec_type(20, 20, 0));
	line_type line3(vec_type(12, 10, 0), vec_type(20, 20, 0));

	line_type line4(vec_type(20, 10, 10), vec_type(20, 20, 10));

	assert(line1.is_perpendicular_line(line2) == 1);
	assert(line1.is_perpendicular_line(line3) == 0);
	assert(line1.is_perpendicular_line(line4) == 1);

	std::cout << line1.is_perpendicular_line(line2)/*1*/ << ", " << line1.is_perpendicular_line(line3) /*0*/<< std::endl;
	std::cout << line1.is_perpendicular_line(line4)/*1*/ << std::endl;
	std::cout << "------is_perpendicular_line_3d_test end----------"  << std::endl;
}

void equal_to_line_2d_test(void)
{
	typedef yggr::math::vector2d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10), vec_type(20, 20));
	line_type line2(vec_type(12, 10), vec_type(22, 20));
	line_type line3(vec_type(15, 15), vec_type(20, 20));

	assert(line1 != line2);
	assert(line1 == line3);

	std::cout << (line1 == line2) /*0*/ << ", " << (line1 == line3) /*1*/ << std::endl;

	std::cout << "---------------equal_to_line_2d_test end--------------" << std::endl;

}

void equal_to_line_3d_test(void)
{
	typedef yggr::math::vector3d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10, 0), vec_type(20, 20, 0));
	line_type line2(vec_type(12, 10, 0), vec_type(22, 20, 0));
	line_type line3(vec_type(15, 15, 0), vec_type(20, 20, 0));

	line_type line4(vec_type(15, 15, 10), vec_type(20, 20, 10));

	std::cout << (line1 == line2) /*0*/ << ", " << (line1 == line3) /*1*/ << std::endl;
	std::cout << (line1 == line4) /*0*/<< std::endl;

	std::cout << "---------------equal_to_line_3d_test end--------------" << std::endl;
}

void is_pt_at_same_area_line_2d_test(void)
{
	typedef yggr::math::vector2d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10), vec_type(20, 20));
	vec_type pos1(20, 17);
	vec_type pos2(300, 18);
	vec_type pos3(5, 14);
	vec_type pos4(10, 14);

	assert(line1.is_point_at_same_area(pos1, pos2) == 1);
	assert(line1.is_point_at_same_area(pos3, pos4) == 1);
	assert(line1.is_point_at_same_area(pos1, pos3) == 0);
	assert(line1.is_point_at_same_area(pos2, pos4) == 0);
	//std::cout << line1.is_point_at_same_area(pos1, pos2) << std::endl;
	std::cout << line1.is_point_at_same_area(pos1, pos2) /*1*/<< ", "
				<< line1.is_point_at_same_area(pos3, pos4) /*1*/ << ", "
				<< line1.is_point_at_same_area(pos1, pos3) /*0*/ << ", "
				<< line1.is_point_at_same_area(pos2, pos4) /*0*/<< std::endl;
	std::cout << "---------------is_pt_at_same_area_line_2d_test end--------------" << std::endl;
}

void is_pt_at_same_area_line_3d_test(void)
{
	typedef yggr::math::vector3d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10, 0), vec_type(20, 20, 0));

#if 1
	vec_type pos1(20, 17, 0);
	vec_type pos2(300, 18, 0);
	vec_type pos3(5, 14, 0);
	vec_type pos4(10, 14, 0);
	assert(line1.is_point_at_same_area(pos1, pos2) == 1);
	assert(line1.is_point_at_same_area(pos3, pos4) == 1);
	assert(line1.is_point_at_same_area(pos1, pos3) == 0);
	assert(line1.is_point_at_same_area(pos2, pos4) == 0);
#else
	vec_type pos1(17, 17, 10);
	vec_type pos2(300, 17, 10);
	vec_type pos3(14, 14, 10);
	vec_type pos4(10, 14, 10);

	assert(line1.is_point_at_same_area(pos1, pos2) == 0);
	assert(line1.is_point_at_same_area(pos3, pos4) == 0);
	assert(line1.is_point_at_same_area(pos1, pos3) == 1);
	assert(line1.is_point_at_same_area(pos2, pos4) == 0);
#endif // 1

	//std::cout << line1.is_point_at_same_area(pos1, pos2) << std::endl;

	std::cout << line1.is_point_at_same_area(pos1, pos2) /*1*/<< ", "
				<< line1.is_point_at_same_area(pos3, pos4) /*1*/<< ", "
				<< line1.is_point_at_same_area(pos1, pos3) /*0*/<< ", "
				<< line1.is_point_at_same_area(pos2, pos4) /*0*/<< std::endl;
	std::cout << "---------------is_pt_at_same_area_line_2d_test end--------------" << std::endl;
}

void cross_point_line_2d_test(void)
{
	typedef yggr::math::vector2d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10), vec_type(20, 20));
	line_type line2(vec_type(10, 20), vec_type(20, 10));

	//line_type line1(vec_type(10, 10), vec_type(20, 20));
	//line_type line2(vec_type(12, 12), vec_type(22, 22));

	typedef std::pair<vec_type, bool> rst_type;
	rst_type rst = line1.cross_point(line2);
	std::cout << rst.first << ", " << rst.second << std::endl;
	std::cout << line1.is_cross_line(line2) << std::endl;
	std::cout << "---------------cross_point_line_2d_test end--------------" << std::endl;
}

void cross_point_line_3d_test(void)
{
	typedef yggr::math::vector3d<yggr::f32> vec_type;
	typedef yggr::geometry::geo_line<vec_type> line_type;

	line_type line1(vec_type(10, 10, 10), vec_type(20, 20, 10));
	line_type line2(vec_type(10, 20, 10), vec_type(20, 10, 10));

	//line_type line1(vec_type(10, 10, 10), vec_type(20, 20, 10));
	//line_type line2(vec_type(12, 12, 10), vec_type(22, 22, 10));

	//line_type line1(vec_type(0, 0, 0), vec_type(1, 0, 0));
	//line_type line2(vec_type(0, 1, 0), vec_type(0, 3, 0));

	//line_type line1(vec_type(0, 0, 0), vec_type(1, 1, 1));
	//line_type line2(vec_type(0, 1, 0), vec_type(1, 0, 1));

	//line_type line1(vec_type(10, 10, 10), vec_type(20, 20, 10));
	//line_type line2(vec_type(15, 12, 30), vec_type(27, 22, 30));

	typedef std::pair<vec_type, bool> rst_type;
	rst_type rst = line1.cross_point(line2);
	std::cout << rst.first << ", " << rst.second << std::endl;
	std::cout << line1.is_cross_line(line2) << std::endl;

	std::cout << "---------------cross_point_line_3d_test end--------------" << std::endl;

	vec_type v1(1, 1, 1), v2(-2, -2, -2);
	std::abs(v2);
	std::cout << std::abs(v2) << std::endl;
}

int main(int argc, char* argv[])
{
	is_parallel_line_2d_test();
	is_parallel_line_3d_test();

	is_in_it_of_point_2d_test();
	is_in_it_of_point_3d_test();

	is_perpendicular_line_2d_test();
	is_perpendicular_line_3d_test();

	equal_to_line_2d_test();
	equal_to_line_3d_test();

	is_pt_at_same_area_line_2d_test();
	is_pt_at_same_area_line_3d_test();

	cross_point_line_2d_test();
	cross_point_line_3d_test();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
