//reference_checker_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>
#include <yggr/type_traits/reference_checker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename Ref>
void typename_ref(Ref ref)
{
	BOOST_MPL_ASSERT((yggr::is_reference<Ref>));
}

template<typename Ref>
void not_typename_ref(Ref ref)
{
	BOOST_MPL_ASSERT_NOT((yggr::is_reference<Ref>));
}

template<typename Ref>
void typename_nude_ref(Ref ref)
{
	BOOST_MPL_ASSERT_NOT((yggr::is_nude_reference<Ref>));
}

void test(void)
{
	BOOST_MPL_ASSERT_NOT((yggr::is_reference<int>));
	BOOST_MPL_ASSERT_NOT((yggr::is_nude_reference<int>));
	BOOST_MPL_ASSERT((yggr::is_reference<int&>));
	BOOST_MPL_ASSERT((yggr::is_nude_reference<int&>));
	BOOST_MPL_ASSERT((yggr::is_reference<const int&>));
	BOOST_MPL_ASSERT((yggr::is_nude_reference<int&>));

	int n = 100;
	typename_ref(boost::cref(n));
	typename_nude_ref(boost::ref(n));

	std::cout << "test end" << std::endl;
}

int main(int argc, char* argv[])
{
	test();
	
	wait_any_key(argc, argv);
	return 0;
}

