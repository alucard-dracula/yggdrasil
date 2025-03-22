// boost_unordered_multimap_ha_opt_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>
#include <cassert>

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/safe_container/detail/boost_ummap_ha_opt.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

void test(void)
{
	typedef boost::unordered_multimap<int, int> org_type;
	typedef yggr::safe_container::detail::cast_to_ummap_ha_opt<org_type>::type opt_type;
	typedef opt_type::node_ptr_type node_ptr_type;
	typedef opt_type::destroy_holder_type destroy_holder_type;
	typedef opt_type::multi_destroy_holder_type multi_destroy_holder_type;

	typedef std::vector< std::pair<int, int> > val_vt_type;
	typedef std::vector<std::size_t> hash_vt_type;
	typedef std::vector<node_ptr_type> node_ptr_vt_type;

	yggr_test_assert((sizeof(org_type) == sizeof(opt_type)));

	{
		org_type org_cont;
		org_cont.insert(org_type::value_type(1, 1));
		org_cont.insert(org_type::value_type(2, 2));

		org_type::value_type val(3, 3);
		org_cont.insert(val);

		opt_type& opt_cont = reinterpret_cast<opt_type&>(org_cont);

		{
			std::size_t key_hash1 = 0, key_hash2 = 0;
			node_ptr_type pnode1 = opt_type::create_node_1(key_hash1, org_cont.get_allocator(), org_cont.hash_function(), 4, 4);
			node_ptr_type pnode2 = opt_type::create_node_1(key_hash2, org_cont.get_allocator(), org_cont.hash_function(), 4, 4);

			opt_cont.insert(key_hash1, pnode1);
			opt_cont.insert(key_hash2, pnode2);

			yggr_test_assert(org_cont.find(4) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4));
			yggr_test_assert(org_cont.find(4) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4, org_cont.key_eq()));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4)->second == 4);
			yggr_test_assert(opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 4), 4) == 2);
		}

		{
			val_vt_type val_vt;
			val_vt.push_back(org_type::value_type(5, 5));
			val_vt.push_back(org_type::value_type(6, 6));
			val_vt.push_back(org_type::value_type(6, 6));

			node_ptr_vt_type pnode_vt;
			hash_vt_type hash_vt;

			opt_cont.insert(opt_type::create_node_range(org_cont.get_allocator(), org_cont.hash_function(), hash_vt, pnode_vt, val_vt.begin(), val_vt.end()));

			yggr_test_assert(org_cont.find(5) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 5), 5));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 5), 5)->second == 5);

			yggr_test_assert(org_cont.find(6) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 6), 6));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 6), 6)->second == 6);
		}

		{
			org_cont.insert(org_type::value_type(8, 8));

			std::size_t key_hash = 0;
			node_ptr_type pnode;

			pnode = opt_type::create_node_1(key_hash, org_cont.get_allocator(), org_cont.hash_function(), 7, 7);
		
			opt_cont.insert(key_hash, pnode);

			yggr_test_assert(org_cont.find(7) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 7), 7));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 7), 7)->second == 7);

			yggr_test_assert(org_cont.find(8) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 8), 8));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 8), 8)->second == 8);
		}

		{
			multi_destroy_holder_type holder;

			opt_cont.erase(holder, opt_type::s_hash(org_cont.hash_function(), 4), 4);
			yggr_test_assert(org_cont.find(4) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4) == org_cont.end());
		}

		{
			yggr_test_assert(org_cont.count(6) == opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 6), 6));
			yggr_test_assert(org_cont.count(7) == opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 7), 7));
			yggr_test_assert(org_cont.count(100) == opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 100), 100));
		}

		{
			yggr_test_assert(org_cont.bucket(6) == opt_cont.bucket(opt_type::s_hash(org_cont.hash_function(), 6)));
			yggr_test_assert(org_cont.bucket(7) == opt_cont.bucket(opt_type::s_hash(org_cont.hash_function(), 7)));
			yggr_test_assert(org_cont.bucket(100) == opt_cont.bucket(opt_type::s_hash(org_cont.hash_function(), 100)));
		}

		yggr_test_assert(org_cont.find(1) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 1), 1));
		org_cont.erase(1);
	}
}
#else
void test(void)
{
	std::cout << "!!! not using optimization !!!" << std::endl;
}
#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}