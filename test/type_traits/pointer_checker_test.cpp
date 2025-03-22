//reference_checker_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <boost/mpl/assert.hpp>
#include <yggr/type_traits/pointer_checker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename Ptr>
void typename_pointer(Ptr ref)
{
	BOOST_MPL_ASSERT((yggr::is_pointer<Ptr>));
}

template<typename Ptr>
void not_typename_pointer(Ptr ref)
{
	BOOST_MPL_ASSERT_NOT((yggr::is_pointer<Ptr>));
}

template<typename Ptr>
void typename_nude_pointer(Ptr ref)
{
	BOOST_MPL_ASSERT_NOT((yggr::is_native_pointer<Ptr>));
}

void test(void)
{
	BOOST_MPL_ASSERT_NOT((yggr::is_pointer<int>));
	BOOST_MPL_ASSERT_NOT((yggr::is_native_pointer<int>));
	BOOST_MPL_ASSERT((yggr::is_pointer<int*>));
	BOOST_MPL_ASSERT((yggr::is_native_pointer<int*>));
	BOOST_MPL_ASSERT((yggr::is_pointer<const int*>));
	BOOST_MPL_ASSERT((yggr::is_native_pointer<int*>));

	//int n = 100;
	typename_pointer(yggr::shared_ptr<int>(new int(100)));
	typename_nude_pointer(yggr::shared_ptr<int>(new int(100)));

	typename_pointer(boost::weak_ptr<int>(yggr::shared_ptr<int>(new int(100))));
	typename_nude_pointer(boost::weak_ptr<int>(yggr::shared_ptr<int>(new int(100))));

	typename_pointer(yggr::interface_ptr<int>(new int(100)));
	typename_nude_pointer(yggr::interface_ptr<int>(new int(100)));

	std::cout << "test end" << std::endl;
}

int main(int argc, char* argv[])
{
	test();
	
	wait_any_key(argc, argv);
	return 0;
}

