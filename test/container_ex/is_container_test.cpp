//is_container_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container_ex/is_container.hpp>

#include <yggr/container/array.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/set.hpp>
#include <yggr/container/map.hpp>
#include <yggr/container/unordered_set.hpp>
#include <yggr/container/unordered_map.hpp>

#include <yggr/safe_container/safe_vector.hpp>
#include <yggr/safe_container/safe_list.hpp>
#include <yggr/safe_container/safe_set.hpp>
#include <yggr/safe_container/safe_map.hpp>
#include <yggr/safe_container/safe_unordered_set.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

struct A
{
};

int main(int argc, char* argv[])
{
	{
		typedef int test_type;
		BOOST_MPL_ASSERT_NOT((yggr::is_container<test_type>));
	}

	{
		typedef A test_type;
		BOOST_MPL_ASSERT_NOT((yggr::is_container<test_type>));
	}

	{
		typedef int test_type[3];
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::array<int, 3> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::vector<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::list<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::set<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::multiset<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::map<int, int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::multimap<int, int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::unordered_set<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::unordered_multiset<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::unordered_map<int, int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::unordered_multimap<int, int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::safe_container::safe_vector<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::safe_container::safe_list<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::safe_container::safe_set<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::safe_container::safe_multiset<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::safe_container::safe_map<int, int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::safe_container::safe_multimap<int, int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::safe_container::safe_unordered_set<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::safe_container::safe_unordered_multiset<int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::safe_container::safe_unordered_map<int, int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	{
		typedef yggr::safe_container::safe_unordered_multimap<int, int> test_type;
		BOOST_MPL_ASSERT((yggr::is_container<test_type>));
	}

	wait_any_key(argc, argv);
	return 0;
}