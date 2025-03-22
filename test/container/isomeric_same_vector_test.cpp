// isomeric_same_vector_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/vector.hpp>


#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_isomeric_same_construct(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::vector<int> default_vt_type;
	typedef boost::container::vector<int> boost_default_vt_type;
	typedef yggr::vector<default_vt_type::value_type, default_vt_type::allocator_type> regular_vt_type;
	typedef boost::container::vector<default_vt_type::value_type, default_vt_type::allocator_type> boost_regular_vt_type;

	// boost 106700 append options
	// boost 107000 allocator = void
#if(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_vt_type, regular_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<default_vt_type, regular_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_vt_type, regular_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<default_vt_type, regular_vt_type>));
#endif // BOOST_VERSION < 107000

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_regular_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_regular_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// allocator style
	{
		boost_regular_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(boost::move(def_vt), def_vt.get_allocator());
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_regular_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(def_vt, def_vt.get_allocator());
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(boost::move(def_vt), def_vt.get_allocator());
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(def_vt, def_vt.get_allocator());
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_regular_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(boost::move(def_vt), def_vt.get_allocator());
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_regular_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt(def_vt, def_vt.get_allocator());
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}
	
	std::cout << "------------test_isomeric_same_construct end---------------" << std::endl;
}

void test_isomeric_same_operator_set(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::vector<int> default_vt_type;
	typedef boost::container::vector<int> boost_default_vt_type;
	typedef yggr::vector<default_vt_type::value_type, default_vt_type::allocator_type> regular_vt_type;
	typedef boost::container::vector<default_vt_type::value_type, default_vt_type::allocator_type> boost_regular_vt_type;

	// boost 106700 append options
	// boost 106900 allocator = void
#if(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_vt_type, regular_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<default_vt_type, regular_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_vt_type, regular_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<default_vt_type, regular_vt_type>));
#endif // BOOST_VERSION < 107000

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		vt = boost::move(def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		vt = def_vt;
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		vt = boost::move(def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		vt = def_vt;
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_regular_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		vt = boost::move(def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_regular_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		vt = def_vt;
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}
	
	std::cout << "------------test_isomeric_same_operator_set end---------------" << std::endl;
}

void test_isomeric_same_operator_swap(void)
{
	int arr[5] = {1, 2, 3, 4, 5};

	typedef yggr::vector<int> default_vt_type;
	typedef boost::container::vector<int> boost_default_vt_type;
	typedef yggr::vector<default_vt_type::value_type, default_vt_type::allocator_type> regular_vt_type;
	typedef boost::container::vector<default_vt_type::value_type, default_vt_type::allocator_type> boost_regular_vt_type;

	// boost 106700 append options
	// boost 106900 allocator = void
#if(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_vt_type, regular_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<default_vt_type, regular_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_vt_type, regular_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<default_vt_type, regular_vt_type>));
#endif // BOOST_VERSION < 107000

	// yggr::swap
	// boost_def boost_def
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		yggr::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		yggr::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		yggr::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// boost_def boost_regular
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		yggr::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		yggr::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		yggr::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// boost_def yggr_def
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		yggr::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		yggr::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		yggr::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// boost_def yggr_regular
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		yggr::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		yggr::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		yggr::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def boost_def

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		yggr::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		yggr::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		yggr::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def boost_regular

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		yggr::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		yggr::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		yggr::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_def

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		yggr::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		yggr::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		yggr::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_regular

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		yggr::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		yggr::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		yggr::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// boost::swap
	// boost_def boost_def
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		boost::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		boost::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		boost::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// boost_def boost_regular
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		boost::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		boost::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		boost::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// boost_def yggr_def
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		boost::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		boost::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		boost::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// boost_def yggr_regular
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		boost::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		boost::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		boost::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def boost_def

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		boost::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		boost::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		boost::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def boost_regular

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		boost::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		boost::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		boost::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_def

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		boost::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		boost::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		boost::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_regular

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		boost::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		boost::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		boost::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// std::swap
	// boost_def boost_def
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		std::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		std::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		std::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// boost_def boost_regular
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		std::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		std::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		std::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// boost_def yggr_def
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		std::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		std::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		std::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// boost_def yggr_regular
	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		std::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		std::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		boost_default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		std::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def boost_def

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		std::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		std::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_default_vt_type vt;
		std::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def boost_regular

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		std::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		std::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		boost_regular_vt_type vt;
		std::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_def

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		std::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		std::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		default_vt_type vt;
		std::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	// yggr_def yggr_regular

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		std::swap(vt, boost::move(def_vt));
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		std::swap(boost::move(vt), def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}

	{
		default_vt_type def_vt(arr + 0, arr + 5);
		regular_vt_type vt;
		std::swap(vt, def_vt);
		yggr_test_assert(0 == memcmp(&vt[0], arr, sizeof(arr)));
	}
	
	std::cout << "------------test_isomeric_same_operator_swap end---------------" << std::endl;
}

void test_isomeric_same_operator_cmp(void)
{
	int arr[5] = {1, 2, 3, 4, 5};
	int arr_l[5] = {1, 2, 3, 4, 4};
	int arr_g[5] = {1, 2, 3, 4, 6};

	typedef yggr::vector<int> default_vt_type;
	typedef boost::container::vector<int> boost_default_vt_type;
	typedef yggr::vector<default_vt_type::value_type, default_vt_type::allocator_type> regular_vt_type;
	typedef boost::container::vector<default_vt_type::value_type, default_vt_type::allocator_type> boost_regular_vt_type;

	// boost 106700 append options
	// boost 106900 allocator = void
#if(BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<default_vt_type, regular_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<default_vt_type, regular_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<default_vt_type, regular_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<default_vt_type, regular_vt_type>));
#endif // BOOST_VERSION < 107000

	default_vt_type def_vt_l(arr_l, arr_l + 5);
	default_vt_type def_vt(arr + 0, arr + 5);
	default_vt_type def_vt_g(arr_g, arr_g + 5);

	boost_default_vt_type boost_def_vt_l(arr_l, arr_l + 5);
	boost_default_vt_type boost_def_vt(arr + 0, arr + 5);
	boost_default_vt_type boost_def_vt_g(arr_g, arr_g + 5);

	regular_vt_type reg_vt_l(arr_l, arr_l + 5);
	regular_vt_type reg_vt(arr + 0, arr + 5);
	regular_vt_type reg_vt_g(arr_g, arr_g + 5);

	boost_regular_vt_type boost_reg_vt_l(arr_l, arr_l + 5);
	boost_regular_vt_type boost_reg_vt(arr + 0, arr + 5);
	boost_regular_vt_type boost_reg_vt_g(arr_g, arr_g + 5);

	//==
	yggr_test_assert(def_vt == def_vt);
	yggr_test_assert(def_vt == boost_def_vt);
	yggr_test_assert(def_vt == reg_vt);
	yggr_test_assert(def_vt == boost_reg_vt);

	yggr_test_assert(def_vt == def_vt);
	yggr_test_assert(boost_def_vt == def_vt);
	yggr_test_assert(reg_vt == def_vt);
	yggr_test_assert(boost_reg_vt == def_vt);

	// !=
	yggr_test_assert(def_vt != def_vt_l);
	yggr_test_assert(def_vt != boost_def_vt_l);
	yggr_test_assert(def_vt != reg_vt_l);
	yggr_test_assert(def_vt != boost_reg_vt_l);

	yggr_test_assert(def_vt_g != def_vt);
	yggr_test_assert(boost_def_vt_g != def_vt);
	yggr_test_assert(reg_vt_g != def_vt);
	yggr_test_assert(boost_reg_vt_g != def_vt);

	// <
	yggr_test_assert(def_vt < def_vt_g);
	yggr_test_assert(def_vt < boost_def_vt_g);
	yggr_test_assert(def_vt < reg_vt_g);
	yggr_test_assert(def_vt < boost_reg_vt_g);

	yggr_test_assert(def_vt_l < def_vt);
	yggr_test_assert(boost_def_vt_l < def_vt);
	yggr_test_assert(reg_vt_l < def_vt);
	yggr_test_assert(boost_reg_vt_l < def_vt);

	// <=
	yggr_test_assert(def_vt <= def_vt);
	yggr_test_assert(def_vt <= boost_def_vt);
	yggr_test_assert(def_vt <= reg_vt);
	yggr_test_assert(def_vt <= boost_reg_vt);

	yggr_test_assert(def_vt_l <= def_vt);
	yggr_test_assert(boost_def_vt_l <= def_vt);
	yggr_test_assert(reg_vt_l <= def_vt);
	yggr_test_assert(boost_reg_vt_l <= def_vt);

	//>
	yggr_test_assert(def_vt > def_vt_l);
	yggr_test_assert(def_vt > boost_def_vt_l);
	yggr_test_assert(def_vt > reg_vt_l);
	yggr_test_assert(def_vt > boost_reg_vt_l);

	yggr_test_assert(def_vt_g > def_vt);
	yggr_test_assert(boost_def_vt_g > def_vt);
	yggr_test_assert(reg_vt_g > def_vt);
	yggr_test_assert(boost_reg_vt_g > def_vt);

	//>=
	yggr_test_assert(def_vt >= def_vt);
	yggr_test_assert(def_vt >= boost_def_vt);
	yggr_test_assert(def_vt >= reg_vt);
	yggr_test_assert(def_vt >= boost_reg_vt);

	yggr_test_assert(def_vt_g >= def_vt);
	yggr_test_assert(boost_def_vt_g >= def_vt);
	yggr_test_assert(reg_vt_g >= def_vt);
	yggr_test_assert(boost_reg_vt_g >= def_vt);
	
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