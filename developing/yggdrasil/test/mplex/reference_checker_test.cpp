//reference_checker_test.cpp

#include <iostream>

#include <boost/mpl/assert.hpp>
#include <yggr/mplex/reference_checker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename Ref>
void typename_ref(Ref ref)
{
	BOOST_MPL_ASSERT((yggr::mplex::is_reference<Ref>));
}

template<typename Ref>
void not_typename_ref(Ref ref)
{
	BOOST_MPL_ASSERT_NOT((yggr::mplex::is_reference<Ref>));
}

template<typename Ref>
void typename_nude_ref(Ref ref)
{
	BOOST_MPL_ASSERT_NOT((yggr::mplex::is_nude_reference<Ref>));
}

void test(void)
{
	BOOST_MPL_ASSERT_NOT((yggr::mplex::is_reference<int>));
	BOOST_MPL_ASSERT_NOT((yggr::mplex::is_nude_reference<int>));
	BOOST_MPL_ASSERT((yggr::mplex::is_reference<int&>));
	BOOST_MPL_ASSERT((yggr::mplex::is_nude_reference<int&>));
	BOOST_MPL_ASSERT((yggr::mplex::is_reference<const int&>));
	BOOST_MPL_ASSERT((yggr::mplex::is_nude_reference<int&>));

	int n = 100;
	typename_ref(boost::cref(n));
	typename_nude_ref(boost::ref(n));

	std::cout << "test end" << std::endl;
}

int main(int argc, char* argv[])
{
	test();
	char cc = 0;
	std::cin >> cc;
	return 0;
}

