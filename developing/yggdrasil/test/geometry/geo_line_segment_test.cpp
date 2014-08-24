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

int main(int argc, char* argv[])
{
	algorithm_helper_test();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
