// isomeric_same_deque_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/deque.hpp>


#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_isomeric_same_construct(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::deque<int> default_dq_type;
	typedef boost::container::deque<int> boost_default_dq_type;
	typedef yggr::deque<default_dq_type::value_type, default_dq_type::allocator_type> regular_dq_type;
	typedef boost::container::deque<default_dq_type::value_type, default_dq_type::allocator_type> boost_regular_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_dq_type, regular_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<default_dq_type, regular_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_dq_type, regular_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<default_dq_type, regular_dq_type>));
#endif // BOOST_VERSION < 107000

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}


	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}


	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_regular_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_regular_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// allocator style
	{
		boost_regular_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(boost::move(def_dq), def_dq.get_allocator());
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_regular_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(def_dq, def_dq.get_allocator());
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(boost::move(def_dq), def_dq.get_allocator());
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(def_dq, def_dq.get_allocator());
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_regular_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(boost::move(def_dq), def_dq.get_allocator());
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_regular_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq(def_dq, def_dq.get_allocator());
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	std::cout << "------------test_isomeric_same_construct end---------------" << std::endl;
}

void test_isomeric_same_operator_set(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::deque<int> default_dq_type;
	typedef boost::container::deque<int> boost_default_dq_type;
	typedef yggr::deque<default_dq_type::value_type, default_dq_type::allocator_type> regular_dq_type;
	typedef boost::container::deque<default_dq_type::value_type, default_dq_type::allocator_type> boost_regular_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_dq_type, regular_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<default_dq_type, regular_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_dq_type, regular_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<default_dq_type, regular_dq_type>));
#endif // BOOST_VERSION < 107000

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		dq = boost::move(def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		dq = def_dq;
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}


	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		dq = boost::move(def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		dq = def_dq;
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_regular_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		dq = boost::move(def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_regular_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		dq = def_dq;
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	std::cout << "------------test_isomeric_same_operator_set end---------------" << std::endl;
}

void test_isomeric_same_operator_swap(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::deque<int> default_dq_type;
	typedef boost::container::deque<int> boost_default_dq_type;
	typedef yggr::deque<default_dq_type::value_type, default_dq_type::allocator_type> regular_dq_type;
	typedef boost::container::deque<default_dq_type::value_type, default_dq_type::allocator_type> boost_regular_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_dq_type, regular_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<default_dq_type, regular_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_dq_type, regular_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<default_dq_type, regular_dq_type>));
#endif // BOOST_VERSION < 107000

	// yggr::swap
	// boost_def boost_def
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		yggr::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		yggr::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		yggr::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// boost_def boost_regular
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		yggr::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		yggr::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		yggr::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// boost_def yggr_def
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		yggr::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		yggr::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		yggr::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// boost_def yggr_regular
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		yggr::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		yggr::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		yggr::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def boost_def

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		yggr::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		yggr::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		yggr::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def boost_regular

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		yggr::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		yggr::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		yggr::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_def

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		yggr::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		yggr::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		yggr::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_regular

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		yggr::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		yggr::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		yggr::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// boost::swap
	// boost_def boost_def
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		boost::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		boost::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		boost::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// boost_def boost_regular
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		boost::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		boost::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		boost::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// boost_def yggr_def
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		boost::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		boost::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		boost::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// boost_def yggr_regular
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		boost::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		boost::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		boost::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def boost_def

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		boost::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		boost::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		boost::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def boost_regular

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		boost::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		boost::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		boost::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_def

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		boost::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		boost::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		boost::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_regular

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		boost::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		boost::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		boost::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// std::swap
	// boost_def boost_def
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		std::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		std::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		std::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// boost_def boost_regular
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		std::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		std::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		std::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// boost_def yggr_def
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		std::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		std::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		std::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// boost_def yggr_regular
	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		std::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		std::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		boost_default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		std::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def boost_def

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		std::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		std::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_default_dq_type dq;
		std::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def boost_regular

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		std::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		std::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		boost_regular_dq_type dq;
		std::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_def

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		std::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		std::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		default_dq_type dq;
		std::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_regular

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		std::swap(dq, boost::move(def_dq));
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		std::swap(boost::move(dq), def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	{
		default_dq_type def_dq(arr + 0, arr + 5);
		regular_dq_type dq;
		std::swap(dq, def_dq);
		yggr_test_assert(0 == memcmp(&dq[0], arr, sizeof(arr)));
	}

	std::cout << "------------test_isomeric_same_operator_swap end---------------" << std::endl;
}

void test_isomeric_same_operator_cmp(void)
{
	int arr[5] = {1, 2, 3, 4, 5};
	int arr_l[5] = {1, 2, 3, 4, 4};
	int arr_g[5] = {1, 2, 3, 4, 6};

	typedef yggr::deque<int> default_dq_type;
	typedef boost::container::deque<int> boost_default_dq_type;
	typedef yggr::deque<default_dq_type::value_type, default_dq_type::allocator_type> regular_dq_type;
	typedef boost::container::deque<default_dq_type::value_type, default_dq_type::allocator_type> boost_regular_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_dq_type, regular_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<default_dq_type, regular_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_dq_type, regular_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<default_dq_type, regular_dq_type>));
#endif // BOOST_VERSION < 107000

	default_dq_type def_dq_l(arr_l, arr_l + 5);
	default_dq_type def_dq(arr + 0, arr + 5);
	default_dq_type def_dq_g(arr_g, arr_g + 5);

	boost_default_dq_type boost_def_dq_l(arr_l, arr_l + 5);
	boost_default_dq_type boost_def_dq(arr + 0, arr + 5);
	boost_default_dq_type boost_def_dq_g(arr_g, arr_g + 5);

	regular_dq_type reg_dq_l(arr_l, arr_l + 5);
	regular_dq_type reg_dq(arr + 0, arr + 5);
	regular_dq_type reg_dq_g(arr_g, arr_g + 5);

	boost_regular_dq_type boost_reg_dq_l(arr_l, arr_l + 5);
	boost_regular_dq_type boost_reg_dq(arr + 0, arr + 5);
	boost_regular_dq_type boost_reg_dq_g(arr_g, arr_g + 5);

	//==
	yggr_test_assert(def_dq == def_dq);
	yggr_test_assert(def_dq == boost_def_dq);
	yggr_test_assert(def_dq == reg_dq);
	yggr_test_assert(def_dq == boost_reg_dq);

	yggr_test_assert(def_dq == def_dq);
	yggr_test_assert(boost_def_dq == def_dq);
	yggr_test_assert(reg_dq == def_dq);
	yggr_test_assert(boost_reg_dq == def_dq);

	// !=
	yggr_test_assert(def_dq != def_dq_l);
	yggr_test_assert(def_dq != boost_def_dq_l);
	yggr_test_assert(def_dq != reg_dq_l);
	yggr_test_assert(def_dq != boost_reg_dq_l);

	yggr_test_assert(def_dq_g != def_dq);
	yggr_test_assert(boost_def_dq_g != def_dq);
	yggr_test_assert(reg_dq_g != def_dq);
	yggr_test_assert(boost_reg_dq_g != def_dq);

	// <
	yggr_test_assert(def_dq < def_dq_g);
	yggr_test_assert(def_dq < boost_def_dq_g);
	yggr_test_assert(def_dq < reg_dq_g);
	yggr_test_assert(def_dq < boost_reg_dq_g);

	yggr_test_assert(def_dq_l < def_dq);
	yggr_test_assert(boost_def_dq_l < def_dq);
	yggr_test_assert(reg_dq_l < def_dq);
	yggr_test_assert(boost_reg_dq_l < def_dq);

	// <=
	yggr_test_assert(def_dq <= def_dq);
	yggr_test_assert(def_dq <= boost_def_dq);
	yggr_test_assert(def_dq <= reg_dq);
	yggr_test_assert(def_dq <= boost_reg_dq);

	yggr_test_assert(def_dq_l <= def_dq);
	yggr_test_assert(boost_def_dq_l <= def_dq);
	yggr_test_assert(reg_dq_l <= def_dq);
	yggr_test_assert(boost_reg_dq_l <= def_dq);

	//>
	yggr_test_assert(def_dq > def_dq_l);
	yggr_test_assert(def_dq > boost_def_dq_l);
	yggr_test_assert(def_dq > reg_dq_l);
	yggr_test_assert(def_dq > boost_reg_dq_l);

	yggr_test_assert(def_dq_g > def_dq);
	yggr_test_assert(boost_def_dq_g > def_dq);
	yggr_test_assert(reg_dq_g > def_dq);
	yggr_test_assert(boost_reg_dq_g > def_dq);

	//>=
	yggr_test_assert(def_dq >= def_dq);
	yggr_test_assert(def_dq >= boost_def_dq);
	yggr_test_assert(def_dq >= reg_dq);
	yggr_test_assert(def_dq >= boost_reg_dq);

	yggr_test_assert(def_dq_g >= def_dq);
	yggr_test_assert(boost_def_dq_g >= def_dq);
	yggr_test_assert(reg_dq_g >= def_dq);
	yggr_test_assert(boost_reg_dq_g >= def_dq);

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
