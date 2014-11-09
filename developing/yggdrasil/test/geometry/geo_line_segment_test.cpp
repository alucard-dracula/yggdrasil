//geo_line_test.cpp

#define YGGR_USE_FAST_MATH

#include <yggr/base/yggrdef.h>
#include <iostream>
#include <utility>
#include <yggr/math/vector2d.hpp>
#include <yggr/math/vector3d.hpp>
#include <yggr/geometry/geo_line_segment.hpp>

#include <yggr/geometry/algorithm_helper.hpp>

#include <list>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void algorithm_helper_test(void)
{
	typedef yggr::math::vector2d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_line_segment<pos_type> line_seg_type;
	typedef std::list<pos_type> pos_list_type;
	typedef pos_list_type::const_iterator pos_list_citer_type;

	line_seg_type line_seg(pos_type(0, 0), pos_type(10, 20));

	pos_list_type pos_list;
	yggr::geometry::algorithm_helper::bresenham_disperse(line_seg, pos_list);

	for(pos_list_citer_type i = pos_list.begin(), isize = pos_list.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}
}

void test_line_segment_2d_mul_matrix(void)
{
	typedef yggr::math::vector2d<yggr::f32> pos_type;
	typedef yggr::geometry::geo_line_segment<pos_type> line_seg_type;
	typedef yggr::math::matrix2d<yggr::s32> mat_type;

	line_seg_type line(pos_type(100, 200), pos_type(200, 100));
	line_seg_type rst_line = line * mat_type::make_translate(100, 100);

	std::cout << rst_line.src_pos << "," << rst_line.dst_pos << std::endl;

	line_seg_type line2(pos_type(100, 200), pos_type(200, 100));
	line2 *= mat_type::make_translate(100, 100);

	assert(rst_line == line2);
}

void test_line_segment_3d_mul_matrix(void)
{
	typedef yggr::math::vector3d<yggr::s32> pos_type;
	typedef yggr::geometry::geo_line_segment<pos_type> line_seg_type;
	typedef yggr::math::matrix3d<yggr::s32> mat_type;

	line_seg_type line(pos_type(100, 200, 300), pos_type(200, 100, 100));
	line_seg_type rst_line = line * mat_type::make_translate(100, 100, 100);

	std::cout << rst_line.src_pos << "," << rst_line.dst_pos << std::endl;

	line_seg_type line2(pos_type(100, 200, 300), pos_type(200, 100, 100));
	line2 *= mat_type::make_translate(100, 100, 100);

	assert(rst_line == line2);
}

int main(int argc, char* argv[])
{
	algorithm_helper_test();

	test_line_segment_2d_mul_matrix();
	test_line_segment_3d_mul_matrix();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
