//pointer_to_value_t_test.cpp

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

struct A
{
};

void test(void)
{
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_value_t<int*>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_value_t<const int*>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_value_t<int const*>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_value_t<boost::shared_ptr<int> >::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_value_t<A*>::type, A>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_value_t<boost::shared_ptr<A> >::type, A>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_value_t<A>::type, A>));

	std::cout << "test end" << std::endl;
}
int main(int argc, char* argv[])
{

	test();
	char cc = 0;
	std::cin >> cc;
	return 0;
}