// isomeric_same_liat_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <algorithm>

#include <yggr/container/list.hpp>


#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_isomeric_same_construct(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::list<int> default_list_type;
	typedef boost::container::list<int> boost_default_list_type;
	typedef yggr::list<default_list_type::value_type, default_list_type::allocator_type> regular_list_type;
	typedef boost::container::list<default_list_type::value_type, default_list_type::allocator_type> boost_regular_list_type;

#if(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_list_type, regular_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<default_list_type, regular_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_list_type, regular_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<default_list_type, regular_list_type>));
#endif // BOOST_VERSION < 107000

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_regular_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_regular_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// allocator style
	{
		boost_regular_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(boost::move(def_lst), def_lst.get_allocator());
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_regular_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(def_lst, def_lst.get_allocator());
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(boost::move(def_lst), def_lst.get_allocator());
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(def_lst, def_lst.get_allocator());
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_regular_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(boost::move(def_lst), def_lst.get_allocator());
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_regular_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst(def_lst, def_lst.get_allocator());
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	std::cout << "------------test_isomeric_same_construct end---------------" << std::endl;
}

void test_isomeric_same_operator_set(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::list<int> default_list_type;
	typedef boost::container::list<int> boost_default_list_type;
	typedef yggr::list<default_list_type::value_type, default_list_type::allocator_type> regular_list_type;
	typedef boost::container::list<default_list_type::value_type, default_list_type::allocator_type> boost_regular_list_type;

#if(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_list_type, regular_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<default_list_type, regular_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_list_type, regular_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<default_list_type, regular_list_type>));
#endif // BOOST_VERSION < 107000

	//{
	//	boost_default_list_type def_lst(arr + 0, arr + 5);
	//	regular_list_type lst;
	//	lst = boost::move(def_lst);
	//	yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	//}

	//{
	//	boost_default_list_type def_lst(arr + 0, arr + 5);
	//	regular_list_type lst;
	//	lst = def_lst;
	//	yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	//}

	//{
	//	default_list_type def_lst(arr + 0, arr + 5);
	//	regular_list_type lst;
	//	lst = boost::move(def_lst);
	//	yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	//}

	//{
	//	default_list_type def_lst(arr + 0, arr + 5);
	//	regular_list_type lst;
	//	lst = def_lst;
	//	yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	//}

	//{
	//	boost_regular_list_type def_lst(arr + 0, arr + 5);
	//	regular_list_type lst;
	//	lst = boost::move(def_lst);
	//	yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	//}

	//{
	//	boost_regular_list_type def_lst(arr + 0, arr + 5);
	//	regular_list_type lst;
	//	lst = def_lst;
	//	yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	//}

	// yggr::swap
	// boost_def boost_def
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		yggr::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		yggr::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// boost_def boost_regular
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		yggr::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		yggr::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// boost_def yggr_def
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		yggr::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		yggr::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// boost_def yggr_regular
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def boost_def

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		yggr::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		yggr::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def boost_regular

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		yggr::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		yggr::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def yggr_def

	{
		default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		yggr::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		yggr::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def yggr_regular

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// boost::swap
	// boost_def boost_def
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		boost::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		boost::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		boost::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// boost_def boost_regular
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		boost::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		boost::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		boost::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// boost_def yggr_def
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		boost::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		boost::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		boost::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// boost_def yggr_regular
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		boost::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		boost::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		boost::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def boost_def

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		boost::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		boost::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		boost::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def boost_regular

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		boost::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		boost::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		boost::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def yggr_def

	{
		default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		boost::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		boost::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		boost::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def yggr_regular

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		boost::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		boost::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		boost::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// std::swap
	// boost_def boost_def
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		std::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		std::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		std::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// boost_def boost_regular
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		std::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		std::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		std::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// boost_def yggr_def
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		std::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		std::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		std::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// boost_def yggr_regular
	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		std::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		std::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		std::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def boost_def

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		std::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		std::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_default_list_type lst;
		std::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def boost_regular

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		std::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		std::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		boost_regular_list_type lst;
		std::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def yggr_def

	{
		default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		std::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		std::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		default_list_type lst;
		std::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	// yggr_def yggr_regular

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		std::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		std::swap(boost::move(lst), def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		std::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	std::cout << "------------test_isomeric_same_operator_set end---------------" << std::endl;
}

void test_isomeric_same_operator_swap(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::list<int> default_list_type;
	typedef boost::container::list<int> boost_default_list_type;
	typedef yggr::list<default_list_type::value_type, default_list_type::allocator_type> regular_list_type;
	typedef boost::container::list<default_list_type::value_type, default_list_type::allocator_type> boost_regular_list_type;

#if(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_list_type, regular_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<default_list_type, regular_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_list_type, regular_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<default_list_type, regular_list_type>));
#endif // BOOST_VERSION < 107000

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(boost::move(def_lst), lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		default_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_regular_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(lst, boost::move(def_lst));
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	{
		boost_regular_list_type def_lst(arr + 0, arr + 5);
		regular_list_type lst;
		yggr::swap(lst, def_lst);
		yggr_test_assert(std::equal(lst.begin(), lst.end(), arr));
	}

	std::cout << "------------test_isomeric_same_operator_swap end---------------" << std::endl;
}

void test_isomeric_same_operator_cmp(void)
{
	int arr[5] = {1, 2, 3, 4, 5};
	int arr_l[5] = {1, 2, 3, 4, 4};
	int arr_g[5] = {1, 2, 3, 4, 6};

	typedef yggr::list<int> default_list_type;
	typedef boost::container::list<int> boost_default_list_type;
	typedef yggr::list<default_list_type::value_type, default_list_type::allocator_type> regular_list_type;
	typedef boost::container::list<default_list_type::value_type, default_list_type::allocator_type> boost_regular_list_type;

#if(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_list_type, regular_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<default_list_type, regular_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_list_type, regular_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<default_list_type, regular_list_type>));
#endif // BOOST_VERSION < 107000

	default_list_type def_lst_l(arr_l, arr_l + 5);
	default_list_type def_lst(arr + 0, arr + 5);
	default_list_type def_lst_g(arr_g, arr_g + 5);

	boost_default_list_type boost_def_lst_l(arr_l, arr_l + 5);
	boost_default_list_type boost_def_lst(arr + 0, arr + 5);
	boost_default_list_type boost_def_lst_g(arr_g, arr_g + 5);

	regular_list_type reg_lst_l(arr_l, arr_l + 5);
	regular_list_type reg_lst(arr + 0, arr + 5);
	regular_list_type reg_lst_g(arr_g, arr_g + 5);

	boost_regular_list_type boost_reg_lst_l(arr_l, arr_l + 5);
	boost_regular_list_type boost_reg_lst(arr + 0, arr + 5);
	boost_regular_list_type boost_reg_lst_g(arr_g, arr_g + 5);

	//==
	yggr_test_assert(def_lst == def_lst);
	yggr_test_assert(def_lst == boost_def_lst);
	yggr_test_assert(def_lst == reg_lst);
	yggr_test_assert(def_lst == boost_reg_lst);

	yggr_test_assert(def_lst == def_lst);
	yggr_test_assert(boost_def_lst == def_lst);
	yggr_test_assert(reg_lst == def_lst);
	yggr_test_assert(boost_reg_lst == def_lst);

	// !=
	yggr_test_assert(def_lst != def_lst_l);
	yggr_test_assert(def_lst != boost_def_lst_l);
	yggr_test_assert(def_lst != reg_lst_l);
	yggr_test_assert(def_lst != boost_reg_lst_l);

	yggr_test_assert(def_lst_g != def_lst);
	yggr_test_assert(boost_def_lst_g != def_lst);
	yggr_test_assert(reg_lst_g != def_lst);
	yggr_test_assert(boost_reg_lst_g != def_lst);

	// <
	yggr_test_assert(def_lst < def_lst_g);
	yggr_test_assert(def_lst < boost_def_lst_g);
	yggr_test_assert(def_lst < reg_lst_g);
	yggr_test_assert(def_lst < boost_reg_lst_g);

	yggr_test_assert(def_lst_l < def_lst);
	yggr_test_assert(boost_def_lst_l < def_lst);
	yggr_test_assert(reg_lst_l < def_lst);
	yggr_test_assert(boost_reg_lst_l < def_lst);

	// <=
	yggr_test_assert(def_lst <= def_lst);
	yggr_test_assert(def_lst <= boost_def_lst);
	yggr_test_assert(def_lst <= reg_lst);
	yggr_test_assert(def_lst <= boost_reg_lst);

	yggr_test_assert(def_lst_l <= def_lst);
	yggr_test_assert(boost_def_lst_l <= def_lst);
	yggr_test_assert(reg_lst_l <= def_lst);
	yggr_test_assert(boost_reg_lst_l <= def_lst);

	//>
	yggr_test_assert(def_lst > def_lst_l);
	yggr_test_assert(def_lst > boost_def_lst_l);
	yggr_test_assert(def_lst > reg_lst_l);
	yggr_test_assert(def_lst > boost_reg_lst_l);

	yggr_test_assert(def_lst_g > def_lst);
	yggr_test_assert(boost_def_lst_g > def_lst);
	yggr_test_assert(reg_lst_g > def_lst);
	yggr_test_assert(boost_reg_lst_g > def_lst);

	//>=
	yggr_test_assert(def_lst >= def_lst);
	yggr_test_assert(def_lst >= boost_def_lst);
	yggr_test_assert(def_lst >= reg_lst);
	yggr_test_assert(def_lst >= boost_reg_lst);

	yggr_test_assert(def_lst_g >= def_lst);
	yggr_test_assert(boost_def_lst_g >= def_lst);
	yggr_test_assert(reg_lst_g >= def_lst);
	yggr_test_assert(boost_reg_lst_g >= def_lst);

	std::cout << "------------test_isomeric_same_operator_cmp end---------------" << std::endl;
}


int main(int argc, char* argv[])
{
	test_isomeric_same_construct();
	test_isomeric_same_operator_set();
	test_isomeric_same_operator_swap();
	test_isomeric_same_operator_cmp();

	wait_any_key(argc, argv);
	return 0;
}
