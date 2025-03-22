// allocator_traits_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/memory_ex/allocator_traits.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

void test_std_allocator(void)
{
	typedef std::allocator<int> alloc_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
	BOOST_MPL_ASSERT((boost::is_same<alloc_type::value_type, yggr::allocator_traits<alloc_type>::value_type>));
	BOOST_MPL_ASSERT((boost::is_same<alloc_type::pointer, yggr::allocator_traits<alloc_type>::pointer>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::null_type, yggr::allocator_traits<alloc_type>::segment_manager>));
#else
	BOOST_MPL_ASSERT((boost::is_same<std::allocator_traits<alloc_type>::value_type, yggr::allocator_traits<alloc_type>::value_type>));
	BOOST_MPL_ASSERT((boost::is_same<std::allocator_traits<alloc_type>::pointer, yggr::allocator_traits<alloc_type>::pointer>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::mplex::null_type, yggr::allocator_traits<alloc_type>::segment_manager>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_20)

#if BOOST_VERSION < 106700
	BOOST_MPL_ASSERT((
		boost::is_same<
			boost::container::container_detail::version_type<alloc_type, 0>, 
			yggr::allocator_traits<alloc_type>::version>));
#else
	BOOST_MPL_ASSERT((
		boost::is_same<
			boost::container::dtl::version_type<alloc_type, 0>, 
			yggr::allocator_traits<alloc_type>::version>));
#endif // BOOST_VERSION < 106700
}

void test_interprocess_allocator(void)
{
	typedef boost::interprocess::managed_shared_memory seg_type;
	typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;

	BOOST_MPL_ASSERT((boost::is_same<alloc_type::value_type, yggr::allocator_traits<alloc_type>::value_type>));
	BOOST_MPL_ASSERT((boost::is_same<alloc_type::pointer, yggr::allocator_traits<alloc_type>::pointer>));
	BOOST_MPL_ASSERT((boost::is_same<alloc_type::segment_manager, yggr::allocator_traits<alloc_type>::segment_manager>));
	BOOST_MPL_ASSERT((boost::is_same<alloc_type::version, yggr::allocator_traits<alloc_type>::version>));

}

int main(int argc, char* argv[])
{
	test_std_allocator();
	test_interprocess_allocator();

	wait_any_key(argc, argv);
	return 0;
}
