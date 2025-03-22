//get_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/mpl/int.hpp>

#include <yggr/utility/pair.hpp>
#include <yggr/tuple_ex/tuple.hpp>
#include <yggr/container_ex/array.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

void pair_get_test(void)
{
	typedef boost::mpl::int_<0> tag_0;
	typedef boost::mpl::int_<1> tag_1;

	std::pair<int, int> pair(10, 20);

	yggr_test_assert(yggr::get<0>(pair) == 10);
	yggr_test_assert(yggr::get<1>(pair) == 20);

	yggr_test_assert(yggr::get<tag_0>(pair) == 10);
	yggr_test_assert(yggr::get<tag_1>(pair) == 20);

	const std::pair<int, int>& cref_pair = pair;

	yggr_test_assert(yggr::get<0>(cref_pair) == 10);
	yggr_test_assert(yggr::get<1>(cref_pair) == 20);

	yggr_test_assert(yggr::get<tag_0>(cref_pair) == 10);
	yggr_test_assert(yggr::get<tag_1>(cref_pair) == 20);

	yggr_test_assert(&yggr::get<0>(pair) == &pair.first);
	yggr_test_assert(&yggr::get<1>(pair) == &pair.second);

	yggr_test_assert(&yggr::get<tag_0>(pair) == &pair.first);
	yggr_test_assert(&yggr::get<tag_1>(pair) == &pair.second);
}

struct tag_2 {};
YGGR_INDEX_T_VALUE_DEF(tag_2, 2)

void tuple_get_test(void)
{
	typedef boost::mpl::int_<0> tag_0;
	typedef boost::mpl::int_<1> tag_1;

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
	{
		typedef std::tuple<int, int, int> tuple_type;
		tuple_type tuple = std::make_tuple(1, 2, 3);

		yggr_test_assert(yggr::get<0>(tuple) == boost::get<0>(tuple));
		yggr_test_assert(yggr::get<1>(tuple) == boost::get<1>(tuple));
		yggr_test_assert(yggr::get<2>(tuple) == boost::get<2>(tuple));

		yggr_test_assert(yggr::get<tag_0>(tuple) == boost::get<tag_0>(tuple));
		yggr_test_assert(yggr::get<tag_1>(tuple) == boost::get<tag_1>(tuple));
		yggr_test_assert(yggr::get<tag_2>(tuple) == boost::get<tag_2>(tuple));
	}
#endif // YGGR_NO_CXX11_HDR_TUPLE

	{
		typedef boost::tuple<int, int, int> tuple_type;
		tuple_type tuple = boost::make_tuple(1, 2, 3);

		yggr_test_assert(yggr::get<0>(tuple) == boost::get<0>(tuple));
		yggr_test_assert(yggr::get<1>(tuple) == boost::get<1>(tuple));
		yggr_test_assert(yggr::get<2>(tuple) == boost::get<2>(tuple));

		yggr_test_assert(yggr::get<tag_0>(tuple) == boost::get<tag_0>(tuple));
		yggr_test_assert(yggr::get<tag_1>(tuple) == boost::get<tag_1>(tuple));
		yggr_test_assert(yggr::get<tag_2>(tuple) == boost::get<tag_2>(tuple));
	}

	{
		typedef yggr::tuple<int, int, int> tuple_type;
		tuple_type tuple = yggr::make_tuple(1, 2, 3);

		yggr_test_assert(yggr::get<0>(tuple) == boost::get<0>(tuple));
		yggr_test_assert(yggr::get<1>(tuple) == boost::get<1>(tuple));
		yggr_test_assert(yggr::get<2>(tuple) == boost::get<2>(tuple));

		yggr_test_assert(yggr::get<tag_0>(tuple) == boost::get<tag_0>(tuple));
		yggr_test_assert(yggr::get<tag_1>(tuple) == boost::get<tag_1>(tuple));
		yggr_test_assert(yggr::get<tag_2>(tuple) == boost::get<tag_2>(tuple));
	}
}

void array_get_test(void)
{
	typedef boost::mpl::int_<0> tag_0;
	typedef boost::mpl::int_<1> tag_1;

	{
		int arr[5] = {1, 2, 3};

		yggr_test_assert(yggr::get<0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<2>(arr) == arr[2]);

		yggr_test_assert(yggr::get<tag_0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<tag_1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<tag_2>(arr) == arr[2]);
	}

#ifndef YGGR_NO_CXX11_HDR_ARRAY
	{
		std::array<int, 5> arr = {1, 2, 3};

		yggr_test_assert(yggr::get<0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<2>(arr) == arr[2]);

		yggr_test_assert(yggr::get<tag_0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<tag_1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<tag_2>(arr) == arr[2]);
	}
#endif // YGGR_NO_CXX11_HDR_ARRAY

	{
		boost::array<int, 5> arr = {1, 2, 3};

		yggr_test_assert(yggr::get<0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<2>(arr) == arr[2]);

		yggr_test_assert(yggr::get<tag_0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<tag_1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<tag_2>(arr) == arr[2]);
	}

	{
		yggr::array<int, 5> arr = {1, 2, 3};

		yggr_test_assert(yggr::get<0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<2>(arr) == arr[2]);

		yggr_test_assert(yggr::get<tag_0>(arr) == arr[0]);
		yggr_test_assert(yggr::get<tag_1>(arr) == arr[1]);
		yggr_test_assert(yggr::get<tag_2>(arr) == arr[2]);
	}
}

int main(int argc, char* argv[])
{
	pair_get_test();
	tuple_get_test();
	array_get_test();

	wait_any_key(argc, argv);
	return 0;
}
