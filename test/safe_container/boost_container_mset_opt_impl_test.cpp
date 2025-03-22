// boost_container_mset_opt_impl_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/safe_container/detail/boost_set_opt.hpp>
#include <vector>

#ifdef _MSC_VER

#	include <vld.h>

#endif // _MSC_VER

void test1(void)
{
	typedef boost::container::multiset<int> org_type;
	typedef yggr::safe_container::detail::cast_to_multiset_opt<org_type>::type opt_type;

	typedef opt_type::node_ptr_type node_ptr_type;
	typedef opt_type::destroy_holder_type destroy_holder_type;
	typedef opt_type::key_type key_type;

//	org_type* porg = 0;
//	opt_type* popt = 0;

	org_type org_cont;
	opt_type& opt_cont = reinterpret_cast<opt_type&>(org_cont);

	{
		//opt_cont.insert(10);

		node_ptr_type pnode = opt_type::create_node(org_cont.get_allocator(), 10);

		opt_cont.insert(pnode);

		yggr_test_assert(org_cont.size() == 1);
		yggr_test_assert(org_cont.find(10) != org_cont.end());
	}

	{
		//opt_cont.insert(org_cont.begin(), 20);

		node_ptr_type pnode = opt_type::create_node(org_cont.get_allocator(), 20);
		opt_cont.insert(org_cont.begin(), pnode);

		yggr_test_assert(org_cont.size() == 2);
		yggr_test_assert(org_cont.find(20) != org_cont.end());

	}

	{
		typedef boost::container::vector<node_ptr_type> ins_vt_type;

		ins_vt_type ins_vt;
		ins_vt.push_back(opt_type::create_node(org_cont.get_allocator(), 30));
		ins_vt.push_back(opt_type::create_node(org_cont.get_allocator(), 30));
		ins_vt.push_back(opt_type::create_node(org_cont.get_allocator(), 30));

		opt_cont.insert(ins_vt.begin(), ins_vt.end());

		yggr_test_assert(org_cont.size() == 5);
		yggr_test_assert(org_cont.find(30) != org_cont.end());
	}

	{
		//opt_cont.erase(key);

		destroy_holder_type holder;
		opt_cont.erase(yggr::safe_container::detail::allocator_destroy_holder_ref(holder),
						20);

		yggr_test_assert(org_cont.size() == 4);
		yggr_test_assert(org_cont.find(20) == org_cont.end());
	}

	//{
	//	//opt_cont.erase(key);

	//	destroy_holder_type holder;
	//	opt_cont.erase(yggr::safe_container::detail::allocator_destroy_holder_ref(holder),
	//					30);

	//	yggr_test_assert(org_cont.size() == 1);
	//	yggr_test_assert(org_cont.find(30) == org_cont.end());
	//}

	{
		//opt_cont.erase(org_cont.begin());

		destroy_holder_type holder;
		opt_cont.erase(yggr::safe_container::detail::allocator_destroy_holder_ref(holder),
						org_cont.begin());

		yggr_test_assert(org_cont.size() == 3);
		yggr_test_assert(org_cont.find(10) == org_cont.end());
	}

	{
		//org_cont.erase(org_cont.begin(), org_cont.end());
		destroy_holder_type holder;
		opt_cont.erase(yggr::safe_container::detail::allocator_destroy_holder_ref(holder),
						org_cont.begin(), org_cont.end() );

		yggr_test_assert(org_cont.empty());
	}
}

int main(int argc, char* argv[])
{
	test1();

	wait_any_key(argc, argv);
	return 0;
}
