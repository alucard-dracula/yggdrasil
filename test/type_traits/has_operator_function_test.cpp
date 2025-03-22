//has_operator_function_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>

#include <yggr/type_traits/has_operator_function.hpp>

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

void foo(void)
{
}

void test(void)
{
	BOOST_MPL_ASSERT((yggr::has_operator_function<A>));
	BOOST_MPL_ASSERT((yggr::has_operator_function<B>));
	BOOST_MPL_ASSERT_NOT((yggr::has_operator_function<C>));
	BOOST_MPL_ASSERT_NOT((yggr::has_operator_function<int>));
	BOOST_MPL_ASSERT_NOT((yggr::has_operator_function<E>));
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}