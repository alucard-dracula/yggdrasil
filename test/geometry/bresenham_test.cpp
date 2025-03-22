// bresenham_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/geometry/bresenham_disperse.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

typedef yggr::math::vector2d<yggr::u64> pos2d_type;
typedef yggr::math::vector3d<yggr::u64> pos3d_type;

template<typename Pos>
void test(const Pos& src, const Pos& dst)
{
	typedef Pos pos_type;
	typedef yggr::geometry::geo_line_segment<pos_type> line_seg_type;
	typedef yggr::vector<pos_type> pos_vt_type;

	std::cout << "-----------test_"<< pos_type::E_LENGTH << "d---S--------------" << std::endl;
	pos_vt_type pos_vt;
	line_seg_type line_seg(src, dst);

	yggr::geometry::bresenham_disperse(pos_vt, line_seg);

	yggr_test_assert(pos_vt.size());

	std::cout << src << " -> " << dst << std::endl;

	for(typename pos_vt_type::const_iterator i = pos_vt.begin(), isize = pos_vt.end(); i != isize; ++i)
	{
		std::cout << *i << std::endl;
	}

	std::cout << "-----------test_"<< pos_type::E_LENGTH << "d---E--------------" << std::endl;
}

int main(int argc, char* argv[])
{
	// 2d_test
	test(pos2d_type(0, 0), pos2d_type(0, 5));
	test(pos2d_type(0, 0), pos2d_type(5, 5));
	test(pos2d_type(0, 0), pos2d_type(5, 0));
	test(pos2d_type(0, 0), pos2d_type(5, -5));
	test(pos2d_type(0, 0), pos2d_type(0, -5));
	test(pos2d_type(0, 0), pos2d_type(-5, -5));
	test(pos2d_type(0, 0), pos2d_type(-5, 0));
	test(pos2d_type(0, 0), pos2d_type(-5, 5));

	test(pos2d_type(0, 5), pos2d_type(0, 0));
	test(pos2d_type(5, 5), pos2d_type(0, 0));
	test(pos2d_type(5, 0), pos2d_type(0, 0));
	test(pos2d_type(5, -5), pos2d_type(0, 0));
	test(pos2d_type(0, -5), pos2d_type(0, 0));
	test(pos2d_type(-5, -5), pos2d_type(0, 0));
	test(pos2d_type(-5, 0), pos2d_type(0, 0));
	test(pos2d_type(-5, 5), pos2d_type(0, 0));

	test(pos2d_type(0, 0), pos2d_type(5, 2));
	test(pos2d_type(0, 0), pos2d_type(5, 4));
	test(pos2d_type(0, 0), pos2d_type(-5, 2));
	test(pos2d_type(0, 0), pos2d_type(-5, 4));

	test(pos2d_type(0, 0), pos2d_type(2, 5));
	test(pos2d_type(0, 0), pos2d_type(4, 5));
	test(pos2d_type(0, 0), pos2d_type(2, -5));
	test(pos2d_type(0, 0), pos2d_type(4, -5));

	// 3d_test
	test(pos3d_type(0, 0, 0), pos3d_type(0, 0, 5));
	test(pos3d_type(0, 0, 0), pos3d_type(0, 5, 0));
	test(pos3d_type(0, 0, 0), pos3d_type(0, 5, 5));
	test(pos3d_type(0, 0, 0), pos3d_type(5, 0, 0));
	test(pos3d_type(0, 0, 0), pos3d_type(5, 0, 5));
	test(pos3d_type(0, 0, 0), pos3d_type(5, 5, 0));
	test(pos3d_type(0, 0, 0), pos3d_type(5, 5, 5));

	test(pos3d_type(0, 0, 0), pos3d_type(0, 0, 0));

	wait_any_key(argc, argv);
	return 0;
}