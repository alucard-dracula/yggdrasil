// boost_unordered_map_opt_ha_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <vector>
#include <cassert>

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/safe_container/detail/boost_umap_ha_opt.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER


#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

void test(void)
{
	typedef boost::unordered_map<int, int> org_type;
	typedef yggr::safe_container::detail::cast_to_umap_ha_opt<org_type>::type opt_type;

	typedef opt_type::node_allocator node_allocator;
	typedef opt_type::node_pointer node_pointer;
	typedef opt_type::destroy_holder_type destroy_holder_type;
	typedef opt_type::multi_destroy_holder_type multi_destroy_holder_type;

	typedef std::vector< std::pair<int, int> > val_vt_type;
	typedef std::vector<std::size_t> hash_vt_type;
	typedef std::vector<node_pointer> node_vt_type;

	yggr_test_assert((sizeof(org_type) == sizeof(opt_type)));

	{
		org_type org_cont;
		org_cont.insert(org_type::value_type(1, 1));
		org_cont.insert(org_type::value_type(2, 2));

		org_type::value_type val(3, 3);
		org_cont.insert(val);

//		org_type* porg = 0;
//		opt_type* popt = 0;

		opt_type& opt_cont = reinterpret_cast<opt_type&>(org_cont);

		{
			destroy_holder_type d_holder;
			std::size_t key_hash1 = 0, key_hash2 = 0;
			node_pointer pnode1 = opt_type::create_node_1(key_hash1, org_cont.get_allocator(), org_cont.hash_function(), 4, 4);
			node_pointer pnode2 = opt_type::create_node_1(key_hash2, org_cont.get_allocator(), org_cont.hash_function(), 4, 4);

			opt_cont.insert(d_holder, key_hash1, pnode1);
			opt_cont.insert(d_holder, key_hash2, pnode2);

			yggr_test_assert(!org_cont.insert(org_type::value_type(4, 4)).second);
			yggr_test_assert(org_cont.find(4) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4)->second == 4);
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 4), 4)->second == 4);
			yggr_test_assert(opt_cont.count(opt_type::s_hash(org_cont.hash_function(), 4), 4) == 1);
		}

		{
			val_vt_type val_vt;
			val_vt.push_back(org_type::value_type(5, 5));
			val_vt.push_back(org_type::value_type(6, 6));
			val_vt.push_back(org_type::value_type(6, 6));


			destroy_holder_type holder;
			node_vt_type node_vt;
			hash_vt_type hash_vt;

			org_type tmp(val_vt.begin(), val_vt.end());
			opt_type& opt_tmp = reinterpret_cast<opt_type&>(tmp);
			opt_tmp.erase(holder, tmp.begin(), tmp.end());

			std::pair<int, int> tmp1;
			holder.get_value(0, tmp1);

			node_vt.reserve(holder.size());
			holder.copy_to(node_vt);

			holder.non_free_clear();

			opt_type::s_hash_range(hash_vt, org_cont.hash_function(), node_vt.begin(), node_vt.end());

			opt_cont.insert(holder, hash_vt.begin(), hash_vt.end(), node_vt.begin(), node_vt.end());

			yggr_test_assert(org_cont.size() == 6);

			yggr_test_assert(org_cont.find(5) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 5), 5));
			yggr_test_assert(org_cont.find(5) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 5), 5, org_cont.key_eq()));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 5), 5)->second == 5);

			yggr_test_assert(org_cont.find(6) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 6), 6));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 6), 6)->second == 6);
		}

		{
			org_cont[8] = 8;
			opt_cont[opt_type::s_hash_index(org_cont.hash_function(), 7)] = 7;

			yggr_test_assert(org_cont.find(7) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 7), 7));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 7), 7)->second == 7);

			yggr_test_assert(org_cont.find(8) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 8), 8));
			yggr_test_assert(opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 8), 8)->second == 8);
		}

		{
			destroy_holder_type holder;
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

		{
			const opt_type& fake_map_cref = opt_cont;
			yggr_test_assert(org_cont.at(6) == opt_cont.at(opt_type::s_hash(org_cont.hash_function(), 6), 6));
			yggr_test_assert(org_cont.at(7) == opt_cont.at(opt_type::s_hash(org_cont.hash_function(), 7), 7));

			yggr_test_assert(&org_cont.at(6) == &opt_cont.at(opt_type::s_hash(org_cont.hash_function(), 6), 6));
			yggr_test_assert(&org_cont.at(7) == &opt_cont.at(opt_type::s_hash(org_cont.hash_function(), 7), 7));

			yggr_test_assert(org_cont.at(6) == fake_map_cref.at(opt_type::s_hash(org_cont.hash_function(), 6), 6));
			yggr_test_assert(org_cont.at(7) == fake_map_cref.at(opt_type::s_hash(org_cont.hash_function(), 7), 7));

			yggr_test_assert(&org_cont.at(6) == &fake_map_cref.at(opt_type::s_hash(org_cont.hash_function(), 6), 6));
			yggr_test_assert(&org_cont.at(7) == &fake_map_cref.at(opt_type::s_hash(org_cont.hash_function(), 7), 7));

			try
			{
				yggr_test_assert(org_cont.at(100) == opt_cont.at(opt_type::s_hash(org_cont.hash_function(), 100), 100));
				yggr_test_assert(false);
			}
			catch(...)
			{
				yggr_test_assert(true);
			}
		}

		yggr_test_assert(org_cont.find(1) == opt_cont.find(opt_type::s_hash(org_cont.hash_function(), 1), 1));
		org_cont.erase(1);

		org_cont[10] = 10;

		{
			multi_destroy_holder_type holder;
			opt_cont.clear(holder);
			yggr_test_assert(org_cont.empty());
		}
	}
}
#else

#include <boost/unordered_map.hpp>

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
