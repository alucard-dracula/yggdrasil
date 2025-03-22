// default_value_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>

#include <yggr/typeof/typeof.hpp>
#include <yggr/typeof/default_value.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

struct A
{
	A(void) : n() {}
	A(int n0) : n(n0) {}
	A(int n0, int n1) : n(n1) {}

	int n;
};

void foo()
{
	return; //0;
}
int main(int argc, char* argv[])
{
	yggr::default_value<void>();
	yggr::default_value<void>(1, 2, 3, 4, 5);
	
	BOOST_MPL_ASSERT((boost::is_same<int, YGGR_TYPEOF((yggr::default_value<int>()))>));
	BOOST_MPL_ASSERT((boost::is_same<int, YGGR_TYPEOF((yggr::default_value<int>(1)))>));

	yggr_test_assert((0 == yggr::default_value<int>()));
	yggr_test_assert((1 == yggr::default_value<int>(1)));

	BOOST_MPL_ASSERT((boost::is_same<A, YGGR_TYPEOF((yggr::default_value<A>()))>));
	BOOST_MPL_ASSERT((boost::is_same<A, YGGR_TYPEOF((yggr::default_value<A>(1)))>));
	BOOST_MPL_ASSERT((boost::is_same<A, YGGR_TYPEOF((yggr::default_value<A>(1, 2)))>));

	yggr_test_assert((0 == yggr::default_value<A>().n));
	yggr_test_assert((1 == yggr::default_value<A>(1).n));
	yggr_test_assert((2 == yggr::default_value<A>(1, 2).n));

	wait_any_key(argc, argv);
	return 0;
}