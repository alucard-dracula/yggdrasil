//rect_map_test.cpp

#include <iostream>

#include <map/rect_map_way_info.h>
#include <map/rect_map_way.h>

#include <vector>

#ifdef _WIN32
#   include <vld.h>
#   pragma comment(lib, "vld.lib")
#endif // _WIN32

void test1(void)
{
	game::map::rect_map_way_info::pos_vt_type pos_vt;
	pos_vt.push_back(game::map::rect_map_way_info::pos_vt_type::value_type(0, 0));
	pos_vt.push_back(game::map::rect_map_way_info::pos_vt_type::value_type(0, 0));
	pos_vt.push_back(game::map::rect_map_way_info::pos_vt_type::value_type(0, 1));
	game::map::rect_map_way_info info(10000, 6000, 10, 10, 14, pos_vt);

	int nnn = 0;
}

void test2(void)
{
	game::map::rect_map_way_info::pos_vt_type pos_vt;
	pos_vt.push_back(game::map::rect_map_way_info::pos_vt_type::value_type(0, 0));
	pos_vt.push_back(game::map::rect_map_way_info::pos_vt_type::value_type(5, 5));
	pos_vt.push_back(game::map::rect_map_way_info::pos_vt_type::value_type(3, 3));
	pos_vt.push_back(game::map::rect_map_way_info::pos_vt_type::value_type(1, 1));
	game::map::rect_map_way_info info(20, 20, 10, 10, 14, pos_vt);

	game::map::rect_map_way map_way(info);

	int nnn = 0;
}

void test3(void)
{
	std::vector<int> vt;
	vt.push_back(1);
	vt.push_back(2);
	vt.push_back(3);
	std::vector<int>::iterator new_end = std::remove(vt.begin(), vt.end(), 2);
	vt.erase(new_end, vt.end());
	int nnn = 0;
}

int main(int argc, char* argv[])
{
	//test1();
	//test2();
	test3();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
