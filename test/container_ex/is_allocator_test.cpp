//is_allocator_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container_ex/is_allocator.hpp>


#include <yggr/container/vector.hpp>


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
		BOOST_MPL_ASSERT_NOT((yggr::is_allocator<test_type>));
	}

	{
		typedef A test_type;
		BOOST_MPL_ASSERT_NOT((yggr::is_allocator<test_type>));
	}

	{
		typedef yggr::vector<int>::allocator_type test_type;
		BOOST_MPL_ASSERT((yggr::is_allocator<test_type>));
	}

	wait_any_key(argc, argv);
	return 0;
}