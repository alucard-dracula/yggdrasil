//reference_to_pointer_t_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/ref.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <yggr/mplex/reference_to_pointer_t.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

struct A
{
};

void test(void)
{
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::reference_to_pointer_t<int&>::type, int*>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::reference_to_pointer_t<const int&>::type, const int*>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::reference_to_pointer_t<int const&>::type, int const*>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::reference_to_pointer_t<boost::reference_wrapper<int> >::type, int*>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::reference_to_pointer_t<A&>::type, A*>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::reference_to_pointer_t<boost::reference_wrapper<A> >::type, A*>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::reference_to_pointer_t<A>::type, A*>));

	std::cout << "test end" << std::endl;
}
int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}
