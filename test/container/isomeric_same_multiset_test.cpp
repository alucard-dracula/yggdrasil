// isomeric_same_multiset_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <algorithm>

#include <yggr/container/set.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_isomeric_same_construct(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::multiset<int> default_set_type;
	typedef boost::container::multiset<int> boost_default_set_type;
	typedef
		yggr::multiset
		<
			default_set_type::value_type,
			default_set_type::key_compare,
			default_set_type::allocator_type
		> regular_set_type;

	typedef
		boost::container::multiset
		<
			default_set_type::value_type,
			default_set_type::key_compare,
			default_set_type::allocator_type
		> boost_regular_set_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_set_type, regular_set_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_multiset<default_set_type, regular_set_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_set_type, regular_set_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_multiset<default_set_type, regular_set_type>));
#endif // BOOST_VERSION < 107000

	regular_set_type set0(arr + 0, arr + 5);

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_regular_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_regular_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(def_set);
		yggr_test_assert(set == set0);
	}

	// allocator style
	{
		boost_regular_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(boost::move(def_set), def_set.get_allocator());
		yggr_test_assert(set == set0);
	}

	{
		boost_regular_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(def_set, def_set.get_allocator());
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(boost::move(def_set), def_set.get_allocator());
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(def_set, def_set.get_allocator());
		yggr_test_assert(set == set0);
	}

	{
		boost_regular_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(boost::move(def_set), def_set.get_allocator());
		yggr_test_assert(set == set0);
	}

	{
		boost_regular_set_type def_set(arr + 0, arr + 5);
		regular_set_type set(def_set, def_set.get_allocator());
		yggr_test_assert(set == set0);
	}

	std::cout << "------------test_isomeric_same_construct end---------------" << std::endl;
}

void test_isomeric_same_operator_set(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::multiset<int> default_set_type;
	typedef boost::container::multiset<int> boost_default_set_type;
	typedef
		yggr::multiset
		<
			default_set_type::value_type,
			default_set_type::key_compare,
			default_set_type::allocator_type
		> regular_set_type;

	typedef
		boost::container::multiset
		<
			default_set_type::value_type,
			default_set_type::key_compare,
			default_set_type::allocator_type
		> boost_regular_set_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_set_type, regular_set_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_multiset<default_set_type, regular_set_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_set_type, regular_set_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_multiset<default_set_type, regular_set_type>));
#endif // BOOST_VERSION < 107000

	regular_set_type set0(arr + 0, arr + 5);

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		set = boost::move(def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		set = def_set;
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		set = boost::move(def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		set = def_set;
		yggr_test_assert(set == set0);
	}

	{
		boost_regular_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		set = boost::move(def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_regular_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		set = def_set;
		yggr_test_assert(set == set0);
	}

	std::cout << "------------test_isomeric_same_operator_set end---------------" << std::endl;
}

void test_isomeric_same_operator_swap(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::multiset<int> default_set_type;
	typedef boost::container::multiset<int> boost_default_set_type;
	typedef
		yggr::multiset
		<
			default_set_type::value_type,
			default_set_type::key_compare,
			default_set_type::allocator_type
		> regular_set_type;

	typedef
		boost::container::multiset
		<
			default_set_type::value_type,
			default_set_type::key_compare,
			default_set_type::allocator_type
		> boost_regular_set_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_set_type, regular_set_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_multiset<default_set_type, regular_set_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_set_type, regular_set_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_multiset<default_set_type, regular_set_type>));
#endif // BOOST_VERSION < 107000

	regular_set_type set0(arr + 0, arr + 5);

	// yggr::swap
	// boost_def boost_def
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		yggr::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		yggr::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		yggr::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// boost_def boost_regular
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		yggr::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		yggr::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		yggr::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// boost_def yggr_def
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		yggr::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		yggr::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		yggr::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// boost_def yggr_regular
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		yggr::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		yggr::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		yggr::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def boost_def

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		yggr::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		yggr::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		yggr::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def boost_regular

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		yggr::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		yggr::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		yggr::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def yggr_def

	{
		default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		yggr::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		yggr::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		yggr::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def yggr_regular

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		yggr::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		yggr::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		yggr::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// boost::swap
	// boost_def boost_def
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		boost::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		boost::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		boost::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// boost_def boost_regular
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		boost::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		boost::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		boost::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// boost_def yggr_def
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		boost::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		boost::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		boost::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// boost_def yggr_regular
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		boost::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		boost::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		boost::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def boost_def

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		boost::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		boost::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		boost::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def boost_regular

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		boost::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		boost::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		boost::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def yggr_def

	{
		default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		boost::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		boost::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		boost::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def yggr_regular

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		boost::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		boost::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		boost::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// std::swap
	// boost_def boost_def
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		std::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		std::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		std::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// boost_def boost_regular
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		std::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		std::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		std::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// boost_def yggr_def
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		std::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		std::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		std::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// boost_def yggr_regular
	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		std::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		std::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		boost_default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		std::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def boost_def

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		std::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		std::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_default_set_type set;
		std::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def boost_regular

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		std::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		std::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		boost_regular_set_type set;
		std::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def yggr_def

	{
		default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		std::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		std::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		default_set_type set;
		std::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	// yggr_def yggr_regular

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		std::swap(set, boost::move(def_set));
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		std::swap(boost::move(set), def_set);
		yggr_test_assert(set == set0);
	}

	{
		default_set_type def_set(arr + 0, arr + 5);
		regular_set_type set;
		std::swap(set, def_set);
		yggr_test_assert(set == set0);
	}

	std::cout << "------------test_isomeric_same_operator_swap end---------------" << std::endl;
}

void test_isomeric_same_operator_cmp(void)
{
	int arr[5] = {1, 2, 3, 4, 5};
	int arr_l[5] = {1, 2, 3, 4, 4};
	int arr_g[5] = {1, 2, 3, 4, 6};

	typedef yggr::multiset<int> default_set_type;
	typedef boost::container::multiset<int> boost_default_set_type;
	typedef
		yggr::multiset
		<
			default_set_type::value_type,
			default_set_type::key_compare,
			default_set_type::allocator_type
		> regular_set_type;

	typedef
		boost::container::multiset
		<
			default_set_type::value_type,
			default_set_type::key_compare,
			default_set_type::allocator_type
		> boost_regular_set_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_set_type, regular_set_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_multiset<default_set_type, regular_set_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_set_type, regular_set_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_multiset<default_set_type, regular_set_type>));
#endif // BOOST_VERSION < 107000

	default_set_type def_set_l(arr_l, arr_l + 5);
	default_set_type def_set(arr + 0, arr + 5);
	default_set_type def_set_g(arr_g, arr_g + 5);

	boost_default_set_type boost_def_set_l(arr_l, arr_l + 5);
	boost_default_set_type boost_def_set(arr + 0, arr + 5);
	boost_default_set_type boost_def_set_g(arr_g, arr_g + 5);

	regular_set_type reg_set_l(arr_l, arr_l + 5);
	regular_set_type reg_set(arr + 0, arr + 5);
	regular_set_type reg_set_g(arr_g, arr_g + 5);

	boost_regular_set_type boost_reg_set_l(arr_l, arr_l + 5);
	boost_regular_set_type boost_reg_set(arr + 0, arr + 5);
	boost_regular_set_type boost_reg_set_g(arr_g, arr_g + 5);

	//==
	yggr_test_assert(def_set == def_set);
	yggr_test_assert(def_set == boost_def_set);
	yggr_test_assert(def_set == reg_set);
	yggr_test_assert(def_set == boost_reg_set);

	yggr_test_assert(def_set == def_set);
	yggr_test_assert(boost_def_set == def_set);
	yggr_test_assert(reg_set == def_set);
	yggr_test_assert(boost_reg_set == def_set);

	// !=
	yggr_test_assert(def_set != def_set_l);
	yggr_test_assert(def_set != boost_def_set_l);
	yggr_test_assert(def_set != reg_set_l);
	yggr_test_assert(def_set != boost_reg_set_l);

	yggr_test_assert(def_set_g != def_set);
	yggr_test_assert(boost_def_set_g != def_set);
	yggr_test_assert(reg_set_g != def_set);
	yggr_test_assert(boost_reg_set_g != def_set);

	// <
	yggr_test_assert(def_set < def_set_g);
	yggr_test_assert(def_set < boost_def_set_g);
	yggr_test_assert(def_set < reg_set_g);
	yggr_test_assert(def_set < boost_reg_set_g);

	yggr_test_assert(def_set_l < def_set);
	yggr_test_assert(boost_def_set_l < def_set);
	yggr_test_assert(reg_set_l < def_set);
	yggr_test_assert(boost_reg_set_l < def_set);

	// <=
	yggr_test_assert(def_set <= def_set);
	yggr_test_assert(def_set <= boost_def_set);
	yggr_test_assert(def_set <= reg_set);
	yggr_test_assert(def_set <= boost_reg_set);

	yggr_test_assert(def_set_l <= def_set);
	yggr_test_assert(boost_def_set_l <= def_set);
	yggr_test_assert(reg_set_l <= def_set);
	yggr_test_assert(boost_reg_set_l <= def_set);

	//>
	yggr_test_assert(def_set > def_set_l);
	yggr_test_assert(def_set > boost_def_set_l);
	yggr_test_assert(def_set > reg_set_l);
	yggr_test_assert(def_set > boost_reg_set_l);

	yggr_test_assert(def_set_g > def_set);
	yggr_test_assert(boost_def_set_g > def_set);
	yggr_test_assert(reg_set_g > def_set);
	yggr_test_assert(boost_reg_set_g > def_set);

	//>=
	yggr_test_assert(def_set >= def_set);
	yggr_test_assert(def_set >= boost_def_set);
	yggr_test_assert(def_set >= reg_set);
	yggr_test_assert(def_set >= boost_reg_set);

	yggr_test_assert(def_set_g >= def_set);
	yggr_test_assert(boost_def_set_g >= def_set);
	yggr_test_assert(reg_set_g >= def_set);
	yggr_test_assert(boost_reg_set_g >= def_set);

	std::cout << "------------test_isomeric_same_operator_cmp end---------------" << std::endl;
}

template<typename LCont, typename RCont>
void test_merge_mm(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::value_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 10);
		yggr::container::merge(cont_eins, boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 20);
		yggr_test_assert(cont_zwei.size() == 0);
	}

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 10);
		yggr::container::merge(cont_eins, cont_zwei);
		yggr_test_assert(cont_eins.size() == 20);
		yggr_test_assert(cont_zwei.size() == 0);
	}
}

void test_isomeric_same_operator_merge(void)
{
	typedef yggr::multiset<int> default_set_type;
	typedef boost::container::multiset<int> boost_default_set_type;
	typedef
		yggr::multiset
		<
			default_set_type::value_type,
			default_set_type::key_compare,
			default_set_type::allocator_type
		> regular_set_type;

	typedef
		boost::container::multiset
		<
			default_set_type::value_type,
			default_set_type::key_compare,
			default_set_type::allocator_type
		> boost_regular_set_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_set_type, regular_set_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_multiset<default_set_type, regular_set_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_set_type, regular_set_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_multiset<default_set_type, regular_set_type>));
#endif // BOOST_VERSION < 107000

	test_merge_mm<default_set_type, default_set_type>();
	test_merge_mm<default_set_type, regular_set_type>();
	test_merge_mm<default_set_type, boost_default_set_type>();
	test_merge_mm<default_set_type, boost_regular_set_type>();

	test_merge_mm<regular_set_type, default_set_type>();
	test_merge_mm<regular_set_type, regular_set_type>();
	test_merge_mm<regular_set_type, boost_default_set_type>();
	test_merge_mm<regular_set_type, boost_regular_set_type>();

	test_merge_mm<boost_default_set_type, default_set_type>();
	test_merge_mm<boost_default_set_type, regular_set_type>();
	test_merge_mm<boost_default_set_type, boost_default_set_type>();
	test_merge_mm<boost_default_set_type, boost_regular_set_type>();

	test_merge_mm<boost_regular_set_type, default_set_type>();
	test_merge_mm<boost_regular_set_type, regular_set_type>();
	test_merge_mm<boost_regular_set_type, boost_default_set_type>();
	test_merge_mm<boost_regular_set_type, boost_regular_set_type>();

	std::cout << "------------test_merge end---------------" << std::endl;
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
