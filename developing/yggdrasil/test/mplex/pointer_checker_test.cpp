//reference_checker_test.cpp

#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/mpl/assert.hpp>
#include <yggr/mplex/pointer_checker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename Ptr>
void typename_pointer(Ptr ref)
{
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<Ptr>));
}

template<typename Ptr>
void not_typename_pointer(Ptr ref)
{
	BOOST_MPL_ASSERT_NOT((yggr::mplex::is_pointer<Ptr>));
}

template<typename Ptr>
void typename_nude_pointer(Ptr ref)
{
	BOOST_MPL_ASSERT_NOT((yggr::mplex::is_nude_pointer<Ptr>));
}

void test(void)
{
	BOOST_MPL_ASSERT_NOT((yggr::mplex::is_pointer<int>));
	BOOST_MPL_ASSERT_NOT((yggr::mplex::is_nude_pointer<int>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<int*>));
	BOOST_MPL_ASSERT((yggr::mplex::is_nude_pointer<int*>));
	BOOST_MPL_ASSERT((yggr::mplex::is_pointer<const int*>));
	BOOST_MPL_ASSERT((yggr::mplex::is_nude_pointer<int*>));

	//int n = 100;
	typename_pointer(boost::shared_ptr<int>(new int(100)));
	typename_nude_pointer(boost::shared_ptr<int>(new int(100)));

	std::cout << "test end" << std::endl;
}

int main(int argc, char* argv[])
{
	test();
	char cc = 0;
	std::cin >> cc;
	return 0;
}

