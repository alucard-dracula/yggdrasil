//pointer_to_reference_t_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/smart_ptr_ex/weak_ptr.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <yggr/mplex/pointer_to_reference_t.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif //_MSC_VER

struct A
{
};

void test(void)
{
#if !defined(YGGR_NO_CXX11_SMART_PTR)

	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<std::shared_ptr<int> >::type, int&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<std::unique_ptr<int> >::type, int&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<std::weak_ptr<int> >::type, int&>));

	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<std::shared_ptr<A> >::type, A&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<std::unique_ptr<A> >::type, A&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<std::weak_ptr<A> >::type, A&>));
#endif // !YGGR_NO_CXX11_SMART_PTR

#if !defined(YGGR_NO_AUTO_PTR)
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<std::auto_ptr<A> >::type, A&>));
#endif // YGGR_NO_AUTO_PTR
	
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<int*>::type, int&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<const int*>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<int const*>::type, int const&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<yggr::shared_ptr<int> >::type, int&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<yggr::weak_ptr<int> >::type, int&>));

	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<boost::shared_ptr<int> >::type, int&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<boost::weak_ptr<int> >::type, int&>));

	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<A*>::type, A&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<yggr::shared_ptr<A> >::type, A&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<yggr::weak_ptr<A> >::type, A&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<boost::shared_ptr<A> >::type, A&>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::pointer_to_reference_t<boost::weak_ptr<A> >::type, A&>));

	BOOST_MPL_ASSERT_NOT((boost::is_same<yggr::mplex::pointer_to_reference_t<A>::type, A&>));

	std::cout << "test end" << std::endl;
}
int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}