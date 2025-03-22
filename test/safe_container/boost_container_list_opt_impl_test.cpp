// boost_container_list_opt_impl_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/safe_container/detail/boost_list_opt.hpp>

#ifdef _MSC_VER

#	include <vld.h>

#endif // _MSC_VER

#define TEST_STD_ALLOCATOR 1

#if (BOOST_VERSION < 105800) && (!TEST_STD_ALLOCATOR)

#	undef TEST_STD_ALLOCATOR
#	define TEST_STD_ALLOCATOR 1

#endif // (BOOST_VERSION < 105800) && (!TEST_STD_ALLOCATOR)

void test1(void)
{

#if TEST_STD_ALLOCATOR
	typedef boost::container::list<int, std::allocator<int> > org_type;
#else
	typedef boost::container::list<int, boost::container::new_allocator<int> > org_type;
#endif //TEST_STD_ALLOCATOR

	typedef yggr::safe_container::detail::cast_to_list_opt<org_type>::type opt_type;

	typedef opt_type::allocator_type allocator_type;
	typedef opt_type::node_ptr_type node_ptr_type;
	typedef opt_type::destroy_holder_type destroy_holder_type;

//	org_type* porg = 0;
//	opt_type* popt = 0;
//	allocator_type* palloc = 0;

	org_type org_cont;


	opt_type& opt_cont = reinterpret_cast<opt_type&>(org_cont);

	{
		org_cont.push_back(10);

		node_ptr_type pnode = opt_cont.create_node(org_cont.get_allocator(), 20);

		opt_cont.insert(org_cont.end(), pnode);
		yggr_test_assert(org_cont.size() == 2);
		yggr_test_assert(org_cont.back() == 20);
	}

	{
		//opt_cont.erase(org_cont.begin());

		destroy_holder_type holder;
		opt_cont.erase(yggr::safe_container::detail::allocator_destroy_holder_ref(holder),
						org_cont.begin());

		yggr_test_assert(org_cont.size() == 1);
		yggr_test_assert(org_cont.front() == 20);
	}


	{
		//org_cont.clear();

		destroy_holder_type holder;
		opt_cont.erase(yggr::safe_container::detail::allocator_destroy_holder_ref(holder),
						org_cont.begin(), org_cont.end());

		yggr_test_assert(org_cont.empty());
		//holder.destroy();

	}

	{
		org_cont.push_back(10);
		org_cont.push_back(20);
		org_cont.push_back(30);

		destroy_holder_type holder;
		opt_cont.remove(yggr::safe_container::detail::allocator_destroy_holder_ref(holder),
						20);

		yggr_test_assert(org_cont.size() == 2);
		yggr_test_assert(org_cont.front() == 10);
		yggr_test_assert(org_cont.back() == 30);
		//holder.destroy();

	}

}

int main(int argc, char* argv[])
{
	test1();

	wait_any_key(argc, argv);
	return 0;
}
