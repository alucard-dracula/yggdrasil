// transform_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/algorithm/transform.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/map.hpp>

#include <yggr/bind/bind.hpp>

#include <iterator>
#include <utility>
#include <cassert>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

yggr::s32 mul_ten(yggr::s32 n)
{
	return n * 10;
}

yggr::s32 mul(yggr::s32 n, yggr::s32 m)
{
	return n * m;
}

std::pair<yggr::s32, yggr::s32> make_pair_s32(yggr::s32 n)
{
	return std::make_pair(n, n);
}

std::pair<yggr::s32, yggr::s32> make_pair_s32_zwei(yggr::s32 n, yggr::s32 m)
{
	return std::make_pair(n, m);
}

int main(int argc, char* argv[])
{

	typedef std::vector<int> vt_type;
	typedef yggr::map<int, int> map_type;

	int arr[] = {1, 2, 3, 4};

	{
		vt_type vt;
		yggr::algorithm::transform(arr, arr + 4, std::inserter(vt, vt.end()), boost::bind(&mul_ten, _1));
		assert(vt[0] == 10);
		assert(vt[1] == 20);
		assert(vt[2] == 30);
		assert(vt[3] == 40);
	}

	{
		vt_type vt;
		yggr::algorithm::transform(arr, arr + 4, arr, std::inserter(vt, vt.end()), boost::bind(&mul, _1, _2));
		assert(vt[0] == 1);
		assert(vt[1] == 4);
		assert(vt[2] == 9);
		assert(vt[3] == 16);
	}

	{
		map_type map;
		yggr::algorithm::transform(arr, arr + 4, arr, std::inserter(map, map.end()), boost::bind(&make_pair_s32, _1));
		assert(map[1] == 1);
		assert(map[2] == 2);
		assert(map[3] == 3);
		assert(map[4] == 4);
	}

	{
		// bind(&std::make_pair<yggr::s32, yggr::s32>, _1, _!), Do not use this in non-special cases, because the implementation of std::make_pair is different after C++03 and C++11.
		map_type map;
		yggr::algorithm::transform(
			arr, arr + 4, arr, std::inserter(map, map.end()),
			boost::bind(&make_pair_s32_zwei, _1, _2));
		assert(map[1] == 1);
		assert(map[2] == 2);
		assert(map[3] == 3);
		assert(map[4] == 4);
	}

	wait_any_key(argc, argv);
	return 0;
}
