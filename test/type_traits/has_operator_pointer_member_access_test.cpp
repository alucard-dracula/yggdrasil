//has_operator_pointer_member_access_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>

#include <yggr/type_traits/has_operator_pointer_member_access.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

struct A
{
	int* operator->(void)
	{
		return 0;
	}

	const int* operator->(void) const
	{
		return 0;
	}
};

struct B
{
	int* operator->(void)
	{
		return 0;
	}
};

struct C
{
};

void test(void)
{
	BOOST_MPL_ASSERT((yggr::has_operator_pointer_member_access_class<A>));
	BOOST_MPL_ASSERT((yggr::has_operator_pointer_member_access_class<B>));
	BOOST_MPL_ASSERT_NOT((yggr::has_operator_pointer_member_access_class<C>));
	BOOST_MPL_ASSERT_NOT((yggr::has_operator_pointer_member_access_class<int*>));
	BOOST_MPL_ASSERT_NOT((yggr::has_operator_pointer_member_access_class<C*>));

	BOOST_MPL_ASSERT((yggr::has_operator_pointer_member_access<A>));
	BOOST_MPL_ASSERT((yggr::has_operator_pointer_member_access<B>));
	BOOST_MPL_ASSERT_NOT((yggr::has_operator_pointer_member_access<C>));
	BOOST_MPL_ASSERT_NOT((yggr::has_operator_pointer_member_access<int*>));
	BOOST_MPL_ASSERT((yggr::has_operator_pointer_member_access<C*>));
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}