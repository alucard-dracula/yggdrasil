// is_callable_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/mpl/assert.hpp>
#include <yggr/func/is_callable.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

struct A
{
	void operator()(void)
	{
	}

	void operator()(void) const
	{
	}
};

struct B
{
	void operator()(void)
	{
	}
};

struct C
{
};

enum E
{
	value = 0
};


void test(void)
{
	BOOST_MPL_ASSERT((yggr::is_callable<A>));
	BOOST_MPL_ASSERT((yggr::is_callable<B>));
	BOOST_MPL_ASSERT((yggr::is_callable<int (void)>));
	BOOST_MPL_ASSERT((yggr::is_callable<int (*)(void)>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable<C>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable<int>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable<E>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable<int*>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable<A*>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable<B*>));



	BOOST_MPL_ASSERT((yggr::is_callable_exclude_function_pointer<A>));
	BOOST_MPL_ASSERT((yggr::is_callable_exclude_function_pointer<B>));
	BOOST_MPL_ASSERT((yggr::is_callable_exclude_function_pointer<int (void)>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable_exclude_function_pointer<int (*)(void)>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable_exclude_function_pointer<C>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable_exclude_function_pointer<int>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable_exclude_function_pointer<E>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable_exclude_function_pointer<int*>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable_exclude_function_pointer<A*>));
	BOOST_MPL_ASSERT_NOT((yggr::is_callable_exclude_function_pointer<B*>));
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}