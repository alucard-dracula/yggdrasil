// isomeric_same_map_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <algorithm>

#include <yggr/container/options_map.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_isomeric_same_construct(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::options_map<int, int> default_map_type;
	typedef boost::container::map<int, int> boost_default_map_type;
	typedef
		yggr::options_map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type,
			default_map_type::options_type
		> regular_map_type;

#if (BOOST_VERSION < 105600)
	typedef
		boost::container::map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type
		> boost_regular_map_type;
#else
	typedef
		boost::container::map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type,
			default_map_type::options_type
		> boost_regular_map_type;
#endif // (BOOST_VERSION < 105600)

#if(BOOST_VERSION < 105600)
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#elif (BOOST_VERSION < 106700)
	BOOST_MPL_ASSERT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#elif(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#endif // BOOST_VERSION < 107000

	yggr::multimap<int, int> map_sample;

	for(int i = 0, isize = 5; i != isize; ++i)
	{
		map_sample.insert(std::make_pair(arr[i], arr[i]));
	}

	yggr_test_assert(map_sample.size() == 5);

	regular_map_type map0(map_sample.begin(), map_sample.end());
	yggr_test_assert(map0.size() == 5);

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_regular_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_regular_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(def_map);
		yggr_test_assert(map == map0);
	}

	// allocator style
	{
		boost_regular_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(boost::move(def_map), def_map.get_allocator());
		yggr_test_assert(map == map0);
	}

	{
		boost_regular_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(def_map, def_map.get_allocator());
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(boost::move(def_map), def_map.get_allocator());
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(def_map, def_map.get_allocator());
		yggr_test_assert(map == map0);
	}

	{
		boost_regular_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(boost::move(def_map), def_map.get_allocator());
		yggr_test_assert(map == map0);
	}

	{
		boost_regular_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map(def_map, def_map.get_allocator());
		yggr_test_assert(map == map0);
	}

	std::cout << "------------test_isomeric_same_construct end---------------" << std::endl;
}

void test_isomeric_same_operator_set(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::options_map<int, int> default_map_type;
	typedef boost::container::map<int, int> boost_default_map_type;
	typedef
		yggr::options_map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type,
			default_map_type::options_type
		> regular_map_type;

#if (BOOST_VERSION < 105600)
	typedef
		boost::container::map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type
		> boost_regular_map_type;
#else
	typedef
		boost::container::map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type,
			default_map_type::options_type
		> boost_regular_map_type;
#endif // (BOOST_VERSION < 105600)

#if(BOOST_VERSION < 105600)
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#elif (BOOST_VERSION < 106700)
	BOOST_MPL_ASSERT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#elif(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#endif // BOOST_VERSION < 107000

	yggr::multimap<int, int> map_sample;

	for(int i = 0, isize = 5; i != isize; ++i)
	{
		map_sample.insert(std::make_pair(arr[i], arr[i]));
	}

	yggr_test_assert(map_sample.size() == 5);

	regular_map_type map0(map_sample.begin(), map_sample.end());

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		map = boost::move(def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		map = def_map;
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		map = boost::move(def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		map = def_map;
		yggr_test_assert(map == map0);
	}

	{
		boost_regular_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		map = boost::move(def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_regular_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		map = def_map;
		yggr_test_assert(map == map0);
	}

	std::cout << "------------test_isomeric_same_operator_set end---------------" << std::endl;
}

void test_isomeric_same_operator_swap(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::options_map<int, int> default_map_type;
	typedef boost::container::map<int, int> boost_default_map_type;
	typedef
		yggr::options_map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type,
			default_map_type::options_type
		> regular_map_type;

#if (BOOST_VERSION < 105600)
	typedef
		boost::container::map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type
		> boost_regular_map_type;
#else
	typedef
		boost::container::map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type,
			default_map_type::options_type
		> boost_regular_map_type;
#endif // (BOOST_VERSION < 105600)

#if(BOOST_VERSION < 105600)
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#elif (BOOST_VERSION < 106700)
	BOOST_MPL_ASSERT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#elif(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#endif // BOOST_VERSION < 107000

	yggr::multimap<int, int> map_sample;

	for(int i = 0, isize = 5; i != isize; ++i)
	{
		map_sample.insert(std::make_pair(arr[i], arr[i]));
	}

	regular_map_type map0(map_sample.begin(), map_sample.end());

	// yggr::swap
	// boost_def boost_def
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		yggr::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		yggr::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		yggr::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// boost_def boost_regular
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		yggr::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		yggr::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		yggr::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// boost_def yggr_def
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		yggr::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		yggr::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		yggr::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// boost_def yggr_regular
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		yggr::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		yggr::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		yggr::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def boost_def

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		yggr::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		yggr::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		yggr::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def boost_regular

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		yggr::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		yggr::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		yggr::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def yggr_def

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		yggr::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		yggr::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		yggr::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def yggr_regular

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		yggr::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		yggr::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		yggr::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// boost::swap
	// boost_def boost_def
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		boost::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		boost::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		boost::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// boost_def boost_regular
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		boost::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		boost::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		boost::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// boost_def yggr_def
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		boost::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		boost::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		boost::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// boost_def yggr_regular
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		boost::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		boost::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		boost::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def boost_def

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		boost::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		boost::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		boost::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def boost_regular

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		boost::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		boost::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		boost::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def yggr_def

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		boost::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		boost::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		boost::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def yggr_regular

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		boost::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		boost::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		boost::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// std::swap
	// boost_def boost_def
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		std::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		std::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		std::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// boost_def boost_regular
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		std::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		std::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		std::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// boost_def yggr_def
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		std::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		std::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		std::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// boost_def yggr_regular
	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		std::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		std::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		boost_default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		std::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def boost_def

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		std::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		std::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_default_map_type map;
		std::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def boost_regular

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		std::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		std::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		boost_regular_map_type map;
		std::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def yggr_def

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		std::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		std::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		default_map_type map;
		std::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	// yggr_def yggr_regular

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		std::swap(map, boost::move(def_map));
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		std::swap(boost::move(map), def_map);
		yggr_test_assert(map == map0);
	}

	{
		default_map_type def_map(map_sample.begin(), map_sample.end());
		regular_map_type map;
		std::swap(map, def_map);
		yggr_test_assert(map == map0);
	}

	std::cout << "------------test_isomeric_same_operator_swap end---------------" << std::endl;
}

void test_isomeric_same_operator_cmp(void)
{
	int arr[5] = {1, 2, 3, 4, 5};
	int arr_l[5] = {1, 2, 3, 4, 4};
	int arr_g[5] = {1, 2, 3, 4, 6};

	typedef yggr::options_map<int, int> default_map_type;
	typedef boost::container::map<int, int> boost_default_map_type;
	typedef
		yggr::options_map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type,
			default_map_type::options_type
		> regular_map_type;

#if (BOOST_VERSION < 105600)
	typedef
		boost::container::map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type
		> boost_regular_map_type;
#else
	typedef
		boost::container::map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type,
			default_map_type::options_type
		> boost_regular_map_type;
#endif // (BOOST_VERSION < 105600)

#if(BOOST_VERSION < 105600)
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#elif (BOOST_VERSION < 106700)
	BOOST_MPL_ASSERT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#elif(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#endif // BOOST_VERSION < 107000

	yggr::multimap<int, int> map_sample;
	yggr::multimap<int, int> map_sample_l;
	yggr::multimap<int, int> map_sample_g;

	for(int i = 0, isize = 5; i != isize; ++i)
	{
		map_sample.insert(std::make_pair(arr[i], arr[i]));
		map_sample_l.insert(std::make_pair(arr_l[i], arr_l[i]));
		map_sample_g.insert(std::make_pair(arr_g[i], arr_g[i]));
	}

	default_map_type def_map_l(map_sample_l.begin(), map_sample_l.end());
	default_map_type def_map(map_sample.begin(), map_sample.end());
	default_map_type def_map_g(map_sample_g.begin(), map_sample_g.end());

	boost_default_map_type boost_def_map_l(map_sample_l.begin(), map_sample_l.end());
	boost_default_map_type boost_def_map(map_sample.begin(), map_sample.end());
	boost_default_map_type boost_def_map_g(map_sample_g.begin(), map_sample_g.end());

	regular_map_type reg_map_l(map_sample_l.begin(), map_sample_l.end());
	regular_map_type reg_map(map_sample.begin(), map_sample.end());
	regular_map_type reg_map_g(map_sample_g.begin(), map_sample_g.end());

	boost_regular_map_type boost_reg_map_l(map_sample_l.begin(), map_sample_l.end());
	boost_regular_map_type boost_reg_map(map_sample.begin(), map_sample.end());
	boost_regular_map_type boost_reg_map_g(map_sample_g.begin(), map_sample_g.end());

	//==
	yggr_test_assert(def_map == def_map);
	yggr_test_assert(def_map == boost_def_map);
	yggr_test_assert(def_map == reg_map);
	yggr_test_assert(def_map == boost_reg_map);

	yggr_test_assert(def_map == def_map);
	yggr_test_assert(boost_def_map == def_map);
	yggr_test_assert(reg_map == def_map);
	yggr_test_assert(boost_reg_map == def_map);

	// !=
	yggr_test_assert(def_map != def_map_l);
	yggr_test_assert(def_map != boost_def_map_l);
	yggr_test_assert(def_map != reg_map_l);
	yggr_test_assert(def_map != boost_reg_map_l);

	yggr_test_assert(def_map_g != def_map);
	yggr_test_assert(boost_def_map_g != def_map);
	yggr_test_assert(reg_map_g != def_map);
	yggr_test_assert(boost_reg_map_g != def_map);

	// <
	yggr_test_assert(def_map < def_map_g);
	yggr_test_assert(def_map < boost_def_map_g);
	yggr_test_assert(def_map < reg_map_g);
	yggr_test_assert(def_map < boost_reg_map_g);

	yggr_test_assert(def_map_l < def_map);
	yggr_test_assert(boost_def_map_l < def_map);
	yggr_test_assert(reg_map_l < def_map);
	yggr_test_assert(boost_reg_map_l < def_map);

	// <=
	yggr_test_assert(def_map <= def_map);
	yggr_test_assert(def_map <= boost_def_map);
	yggr_test_assert(def_map <= reg_map);
	yggr_test_assert(def_map <= boost_reg_map);

	yggr_test_assert(def_map_l <= def_map);
	yggr_test_assert(boost_def_map_l <= def_map);
	yggr_test_assert(reg_map_l <= def_map);
	yggr_test_assert(boost_reg_map_l <= def_map);

	//>
	yggr_test_assert(def_map > def_map_l);
	yggr_test_assert(def_map > boost_def_map_l);
	yggr_test_assert(def_map > reg_map_l);
	yggr_test_assert(def_map > boost_reg_map_l);

	yggr_test_assert(def_map_g > def_map);
	yggr_test_assert(boost_def_map_g > def_map);
	yggr_test_assert(reg_map_g > def_map);
	yggr_test_assert(boost_reg_map_g > def_map);

	//>=
	yggr_test_assert(def_map >= def_map);
	yggr_test_assert(def_map >= boost_def_map);
	yggr_test_assert(def_map >= reg_map);
	yggr_test_assert(def_map >= boost_reg_map);

	yggr_test_assert(def_map_g >= def_map);
	yggr_test_assert(boost_def_map_g >= def_map);
	yggr_test_assert(reg_map_g >= def_map);
	yggr_test_assert(boost_reg_map_g >= def_map);

	std::cout << "------------test_isomeric_same_operator_cmp end---------------" << std::endl;
}

template<typename LCont, typename RCont>
void test_merge_ss(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::key_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	yggr::multimap<val_type, val_type> map_sample1;
	yggr::multimap<val_type, val_type> map_sample2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		map_sample1.insert(std::make_pair(arr1[i], arr1[i]));
		map_sample2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	{
		l_cont_type cont_eins(map_sample1.begin(), map_sample1.end());
		r_cont_type cont_zwei(map_sample2.begin(), map_sample2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		yggr::container::merge(cont_eins, boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}

	{
		l_cont_type cont_eins(map_sample1.begin(), map_sample1.end());
		r_cont_type cont_zwei(map_sample2.begin(), map_sample2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		yggr::container::merge(cont_eins, cont_zwei);
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}
}

void test_isomeric_same_operator_merge(void)
{
	typedef yggr::options_map<int, int> default_map_type;
	typedef boost::container::map<int, int> boost_default_map_type;
	typedef
		yggr::options_map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type,
			default_map_type::options_type
		> regular_map_type;

#if (BOOST_VERSION < 105600)
	typedef
		boost::container::map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type
		> boost_regular_map_type;
#else
	typedef
		boost::container::map
		<
			default_map_type::key_type,
			default_map_type::mapped_type,
			default_map_type::key_compare,
			default_map_type::allocator_type,
			default_map_type::options_type
		> boost_regular_map_type;
#endif // (BOOST_VERSION < 105600)

#if(BOOST_VERSION < 105600)
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#elif (BOOST_VERSION < 106700)
	BOOST_MPL_ASSERT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#elif(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_map_type, regular_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_options_map<default_map_type, regular_map_type>));
#endif // BOOST_VERSION < 107000

	test_merge_ss<default_map_type, default_map_type>();
	test_merge_ss<default_map_type, regular_map_type>();
	test_merge_ss<default_map_type, boost_default_map_type>();
	test_merge_ss<default_map_type, boost_regular_map_type>();

	test_merge_ss<regular_map_type, default_map_type>();
	test_merge_ss<regular_map_type, regular_map_type>();
	test_merge_ss<regular_map_type, boost_default_map_type>();
	test_merge_ss<regular_map_type, boost_regular_map_type>();

	test_merge_ss<boost_default_map_type, default_map_type>();
	test_merge_ss<boost_default_map_type, regular_map_type>();
	test_merge_ss<boost_default_map_type, boost_default_map_type>();
	test_merge_ss<boost_default_map_type, boost_regular_map_type>();

	test_merge_ss<boost_regular_map_type, default_map_type>();
	test_merge_ss<boost_regular_map_type, regular_map_type>();
	test_merge_ss<boost_regular_map_type, boost_default_map_type>();
	test_merge_ss<boost_regular_map_type, boost_regular_map_type>();


	std::cout << "------------test_isomeric_same_operator_merge end---------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_isomeric_same_construct();
	test_isomeric_same_operator_set();
	test_isomeric_same_operator_swap();
	test_isomeric_same_operator_cmp();
	test_isomeric_same_operator_merge();

	wait_any_key(argc, argv);
	return 0;
}
