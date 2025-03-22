// umap_node_handler_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/type_traits.hpp>
#include <boost/mpl/assert.hpp>

#include <yggr/charset/string.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/container/unordered_map.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

// extract and insert
template<typename Map, typename Rst> inline
typename
	boost::enable_if
	<
		yggr::iterator_ex::is_iterator<Rst>,
		bool
	>::type
	check_insert_result(Map& map, Rst rst)
{
	return rst != map.end();
}

template<typename Map, typename Rst> inline
typename
	boost::disable_if
	<
		yggr::iterator_ex::is_iterator<Rst>,
		bool
	>::type
	check_insert_result(Map& map, const Rst& rst)
{
	return rst.inserted;
}


template<typename Map>
void test_extract_insert_tpl(void)
{
	typedef Map map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::value_type value_type;

	typedef typename map_type::iterator now_iter_type;
	typedef typename map_type::const_iterator now_citer_type;
	typedef typename yggr::container::node_handle_traits<map_type> node_handle_traits_type;
	typedef typename node_handle_traits_type::node_type now_node_type;
	typedef typename node_handle_traits_type::insert_return_type now_insert_return_type;
	typedef typename node_handle_traits_type::hint_insert_return_type now_hint_insert_return_type;

	key_type arr[] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};

	map_type map;
	for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type); i != isize; ++i)
	{
		map.insert(value_type(*i, (*i) * 10));
	}

	{
		now_node_type node = yggr::container::extract(map, 1);
		std::cout << node.key() << std::endl;
		std::cout << node.mapped() << std::endl;
		yggr_test_assert(node.key() == 1);
		yggr_test_assert(node.mapped() == node.key() * 10);

		now_insert_return_type iret = yggr::container::insert(map, boost::move(node));

		yggr_test_assert(check_insert_result(map, iret));
		yggr_test_assert(map.find(1) != map.end());
	}

	{
		now_citer_type iter = map.find(2);
		yggr_test_assert(iter != map.end());
		now_node_type node = yggr::container::extract(map, iter);
		std::cout << node.key() << std::endl;
		std::cout << node.mapped() << std::endl;
		yggr_test_assert(node.key() == 2);
		yggr_test_assert(node.mapped() == node.key() * 10);

		now_hint_insert_return_type iret = yggr::container::insert(map, map.end(), boost::move(node));

		yggr_test_assert(check_insert_result(map, iret));
		yggr_test_assert(map.find(2) != map.end());
	}
}

void test_extract_insert(void)
{

	std::cout << "------------test_extract_insert start-----------" << std::endl;

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP) && (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
	test_extract_insert_tpl< std::unordered_map<int, int> >();
	test_extract_insert_tpl< std::unordered_multimap<int, int> >();
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

	test_extract_insert_tpl< boost::unordered_map<int, int> >();
	test_extract_insert_tpl< boost::unordered_multimap<int, int> >();
	test_extract_insert_tpl< yggr::unordered_map<int, int> >();
	test_extract_insert_tpl< yggr::unordered_multimap<int, int> >();

	std::cout << "------------test_extract_insert end-----------" << std::endl;
}

// merge

template<typename MapL, typename MapR>
void test_merge_tpl(void)
{
	typedef MapL left_map_type;
	typedef MapR right_map_type;

	typedef typename left_map_type::key_type left_key_type;
	typedef typename right_map_type::key_type right_key_type;

	typedef typename left_map_type::mapped_type left_mapped_type;
	typedef typename right_map_type::mapped_type right_mapped_type;

	typedef typename left_map_type::value_type left_value_type;
	typedef typename right_map_type::value_type right_value_type;

	left_key_type larr[] = {1, 1, 2, 2, 3, 3};
	right_key_type rarr[] = {3, 3, 4, 4, 5, 6};


	{
		left_map_type lmap;
		right_map_type rmap;

		{
			for(const left_key_type *i = larr, *isize = larr + sizeof(larr) / sizeof(left_key_type);
					i != isize; ++i)
			{
				lmap.insert(left_value_type(*i, (*i) * 10));
			}
		}

		{
			for(const right_key_type *i = rarr, *isize = rarr + sizeof(rarr) / sizeof(right_key_type);
				i != isize; ++i)
			{
				rmap.insert(right_value_type(*i, (*i) * 10));
			}
		}

		yggr::container::merge(lmap, rmap);

		yggr_test_assert(rmap.size() <= lmap.size());
		//yggr_test_assert((lmap.begin()->first == larr[0]));
		//yggr_test_assert((lmap.begin()->second == larr[0] * 10));
		//yggr_test_assert((lmap.rbegin()->first == rarr[5]));
		//yggr_test_assert((lmap.rbegin()->second == rarr[5] * 10));

		{
			for(typename left_map_type::key_type *i = larr, *isize = larr + 6; i != isize; ++i)
			{
				yggr_test_assert(lmap.find(*i) != lmap.end());
				yggr_test_assert(lmap.find(*i)->second == (*i) * 10);
			}
		}

		{
			for(typename right_map_type::key_type* i = rarr, *isize = rarr + 6; i != isize; ++i)
			{
				yggr_test_assert(lmap.find(*i) != lmap.end());
				yggr_test_assert(lmap.find(*i)->second == (*i) * 10);
			}
		}
	}

	{
		left_map_type lmap;
		right_map_type rmap;

		{
			for(const left_key_type *i = larr, *isize = larr + sizeof(larr) / sizeof(left_key_type);
					i != isize; ++i)
			{
				lmap.insert(left_value_type(*i, (*i) * 10));
			}
		}

		{
			for(const right_key_type *i = rarr, *isize = rarr + sizeof(rarr) / sizeof(right_key_type);
				i != isize; ++i)
			{
				rmap.insert(right_value_type(*i, (*i) * 10));
			}
		}

		yggr::container::merge(lmap, boost::move(rmap));

		yggr_test_assert(rmap.size() <= lmap.size());
		//yggr_test_assert((lmap.begin()->first == larr[0]));
		//yggr_test_assert((lmap.begin()->second == larr[0] * 10));
		//yggr_test_assert((lmap.rbegin()->first == rarr[5]));
		//yggr_test_assert((lmap.rbegin()->second == rarr[5] * 10));

		{
			for(typename left_map_type::key_type *i = larr, *isize = larr + 6; i != isize; ++i)
			{
				yggr_test_assert(lmap.find(*i) != lmap.end());
				yggr_test_assert(lmap.find(*i)->second == (*i) * 10);
			}
		}

		{
			for(typename right_map_type::key_type* i = rarr, *isize = rarr + 6; i != isize; ++i)
			{
				yggr_test_assert(lmap.find(*i) != lmap.end());
				yggr_test_assert(lmap.find(*i)->second == (*i) * 10);
			}
		}
	}
}

template<typename MapL, typename MapR>
void test_merge_tpl2(void)
{
	typedef MapL left_map_type;
	typedef MapR right_map_type;

	typedef typename left_map_type::key_type left_key_type;
	typedef typename right_map_type::key_type right_key_type;

	typedef typename left_map_type::mapped_type left_mapped_type;
	typedef typename right_map_type::mapped_type right_mapped_type;

	typedef typename left_map_type::value_type left_value_type;
	typedef typename right_map_type::value_type right_value_type;

	left_key_type larr[] = {1, 1, 2, 2, 3, 3};
	right_key_type rarr[] = {3, 3, 4, 4, 5, 6};


	{
		left_map_type lmap;
		right_map_type rmap;

		{
			for(const left_key_type *i = larr, *isize = larr + sizeof(larr) / sizeof(left_key_type);
					i != isize; ++i)
			{
				lmap.insert(left_value_type(*i, (*i) * 10));
			}
		}

		{
			for(const right_key_type *i = rarr, *isize = rarr + sizeof(rarr) / sizeof(right_key_type);
				i != isize; ++i)
			{
				rmap.insert(right_value_type(*i, (*i) * 10));
			}
		}

		lmap.merge(rmap);

		yggr_test_assert(rmap.size() <= lmap.size());
		//yggr_test_assert((lmap.begin()->first == larr[0]));
		//yggr_test_assert((lmap.begin()->second == larr[0] * 10));
		//yggr_test_assert((lmap.rbegin()->first == rarr[5]));
		//yggr_test_assert((lmap.rbegin()->second == rarr[5] * 10));

		{
			for(typename left_map_type::key_type *i = larr, *isize = larr + 6; i != isize; ++i)
			{
				yggr_test_assert(lmap.find(*i) != lmap.end());
				yggr_test_assert(lmap.find(*i)->second == (*i) * 10);
			}
		}

		{
			for(typename right_map_type::key_type* i = rarr, *isize = rarr + 6; i != isize; ++i)
			{
				yggr_test_assert(lmap.find(*i) != lmap.end());
				yggr_test_assert(lmap.find(*i)->second == (*i) * 10);
			}
		}
	}

	{
		left_map_type lmap;
		right_map_type rmap;

		{
			for(const left_key_type *i = larr, *isize = larr + sizeof(larr) / sizeof(left_key_type);
					i != isize; ++i)
			{
				lmap.insert(left_value_type(*i, (*i) * 10));
			}
		}

		{
			for(const right_key_type *i = rarr, *isize = rarr + sizeof(rarr) / sizeof(right_key_type);
				i != isize; ++i)
			{
				rmap.insert(right_value_type(*i, (*i) * 10));
			}
		}

		lmap.merge(boost::move(rmap));

		yggr_test_assert(rmap.size() <= lmap.size());
		//yggr_test_assert((lmap.begin()->first == larr[0]));
		//yggr_test_assert((lmap.begin()->second == larr[0] * 10));
		//yggr_test_assert((lmap.rbegin()->first == rarr[5]));
		//yggr_test_assert((lmap.rbegin()->second == rarr[5] * 10));

		{
			for(typename left_map_type::key_type *i = larr, *isize = larr + 6; i != isize; ++i)
			{
				yggr_test_assert(lmap.find(*i) != lmap.end());
				yggr_test_assert(lmap.find(*i)->second == (*i) * 10);
			}
		}

		{
			for(typename right_map_type::key_type* i = rarr, *isize = rarr + 6; i != isize; ++i)
			{
				yggr_test_assert(lmap.find(*i) != lmap.end());
				yggr_test_assert(lmap.find(*i)->second == (*i) * 10);
			}
		}
	}
}

void test_merge(void)
{

	std::cout << "------------test_merge start-----------" << std::endl;

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	// std::unordered_map
	test_merge_tpl< std::unordered_map<int, int>, std::unordered_map<int, int> >();
	test_merge_tpl< std::unordered_map<int, int>, std::unordered_multimap<int, int> >();

	test_merge_tpl< std::unordered_map<int, int>, boost::unordered_map<int, int> >();
	test_merge_tpl< std::unordered_map<int, int>, boost::unordered_multimap<int, int> >();

	test_merge_tpl< std::unordered_map<int, int>, yggr::unordered_map<int, int> >();
	test_merge_tpl< std::unordered_map<int, int>, yggr::unordered_multimap<int, int> >();

	// std::unordered_multimap
	test_merge_tpl< std::unordered_multimap<int, int>, std::unordered_map<int, int> >();
	test_merge_tpl< std::unordered_multimap<int, int>, std::unordered_multimap<int, int> >();

	test_merge_tpl< std::unordered_multimap<int, int>, boost::unordered_map<int, int> >();
	test_merge_tpl< std::unordered_multimap<int, int>, boost::unordered_multimap<int, int> >();

	test_merge_tpl< std::unordered_multimap<int, int>, yggr::unordered_map<int, int> >();
	test_merge_tpl< std::unordered_multimap<int, int>, yggr::unordered_multimap<int, int> >();

#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	// boost::contianer::map
	test_merge_tpl< boost::unordered_map<int, int>, std::unordered_map<int, int> >();
	test_merge_tpl< boost::unordered_map<int, int>, std::unordered_multimap<int, int> >();

#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

	test_merge_tpl< boost::unordered_map<int, int>, boost::unordered_map<int, int> >();
	test_merge_tpl< boost::unordered_map<int, int>, boost::unordered_multimap<int, int> >();

	test_merge_tpl< boost::unordered_map<int, int>, yggr::unordered_map<int, int> >();
	test_merge_tpl< boost::unordered_map<int, int>, yggr::unordered_multimap<int, int> >();

	// boost::unordered_multimap
#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	test_merge_tpl< boost::unordered_multimap<int, int>, std::unordered_map<int, int> >();
	test_merge_tpl< boost::unordered_multimap<int, int>, std::unordered_multimap<int, int> >();
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

	test_merge_tpl< boost::unordered_multimap<int, int>, boost::unordered_map<int, int> >();
	test_merge_tpl< boost::unordered_multimap<int, int>, boost::unordered_multimap<int, int> >();

	test_merge_tpl< boost::unordered_multimap<int, int>, yggr::unordered_map<int, int> >();
	test_merge_tpl< boost::unordered_multimap<int, int>, yggr::unordered_multimap<int, int> >();

	// yggr::unordered_map
#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	test_merge_tpl< yggr::unordered_map<int, int>, std::unordered_map<int, int> >();
	test_merge_tpl< yggr::unordered_map<int, int>, std::unordered_multimap<int, int> >();
#endif // yggr::unordered_map

	test_merge_tpl< yggr::unordered_map<int, int>, boost::unordered_map<int, int> >();
	test_merge_tpl< yggr::unordered_map<int, int>, boost::unordered_multimap<int, int> >();

	test_merge_tpl< yggr::unordered_map<int, int>, yggr::unordered_map<int, int> >();
	test_merge_tpl< yggr::unordered_map<int, int>, yggr::unordered_multimap<int, int> >();

#if !(BOOST_VERSION < 106400)

	test_merge_tpl2< yggr::unordered_map<int, int>, boost::unordered_map<int, int> >();
	test_merge_tpl2< yggr::unordered_map<int, int>, yggr::unordered_map<int, int> >();

#endif // !(BOOST_VERSION < 106400)

	// yggr::unordered_multimap
#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	test_merge_tpl< yggr::unordered_multimap<int, int>, std::unordered_map<int, int> >();
	test_merge_tpl< yggr::unordered_multimap<int, int>, std::unordered_multimap<int, int> >();
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

	test_merge_tpl< yggr::unordered_multimap<int, int>, boost::unordered_map<int, int> >();
	test_merge_tpl< yggr::unordered_multimap<int, int>, boost::unordered_multimap<int, int> >();

	test_merge_tpl< yggr::unordered_multimap<int, int>, yggr::unordered_map<int, int> >();
	test_merge_tpl< yggr::unordered_multimap<int, int>, yggr::unordered_multimap<int, int> >();

#if !(BOOST_VERSION < 106400)

	test_merge_tpl2< yggr::unordered_multimap<int, int>, boost::unordered_multimap<int, int> >();
	test_merge_tpl2< yggr::unordered_multimap<int, int>, yggr::unordered_multimap<int, int> >();

#endif // !(BOOST_VERSION < 105600)

	std::cout << "------------test_merge end-----------" << std::endl;
}

template<typename Map>
void test_contains_tpl(void)
{
	typedef Map map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::value_type value_type;

	key_type arr[] = {1, 1, 2, 2, 3, 3};

	{
		map_type map;
		{
			for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type);
					i != isize; ++i)
			{
				map.insert(value_type(*i, (*i) * 10));
			}
		}

		bool bchk = false;

		bchk = yggr::container::contains(map, 1);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(map, 2);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(map, 3);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(map, 4);
		yggr_test_assert(!bchk);
	}

	{
		map_type map;
		{
			for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type);
					i != isize; ++i)
			{
				map.insert(value_type(*i, (*i) * 10));
			}
		}

		bool bchk = false;

		bchk = yggr::container::contains(map, 1.0);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(map, 2.0);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(map, 3.0);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(map, 4.0);
		yggr_test_assert(!bchk);
	}
}

void test_contains(void)
{
	std::cout << "------------test_contains start-----------" << std::endl;

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	test_contains_tpl< std::unordered_map<int, int> >();
	test_contains_tpl< std::unordered_multimap<int, int> >();
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

	test_contains_tpl< boost::unordered_map<int, int> >();
	test_contains_tpl< boost::unordered_multimap<int, int> >();

	test_contains_tpl< yggr::unordered_map<int, int> >();
	test_contains_tpl< yggr::unordered_multimap<int, int> >();

	std::cout << "------------test_contains end-----------" << std::endl;
}

template<typename Map>
void test_try_emplace_tpl(void)
{
	typedef Map map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::size_type size_type;
	typedef typename map_type::iterator iter_type;

	key_type arr[] = {1, 1, 2, 2, 3, 4};

	{
		map_type map;
		{
			for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type);
					i != isize; ++i)
			{
				map.insert(value_type(*i, (*i) * 10));
			}
		}

		size_type old_size = map.size();

		std::pair<iter_type, bool> ret = yggr::container::try_emplace(map, 5, 50);

		yggr_test_assert(ret.first->first == 5);
		yggr_test_assert(ret.first->second == 50);
		yggr_test_assert(ret.second);

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, 5));

		key_type key = 5;
		mapped_type map_val = 60;

		ret = yggr::container::try_emplace(map, boost::move(key), boost::move(map_val));

		yggr_test_assert(ret.first->first == 5);
		yggr_test_assert(ret.first->second == 50);
		yggr_test_assert(!ret.second);

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, 5));
	}

	{
		map_type map;
		{
			for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type);
					i != isize; ++i)
			{
				map.insert(value_type(*i, (*i) * 10));
			}
		}

		size_type old_size = map.size();

		iter_type ret = yggr::container::try_emplace(map, map.begin(), 5, 50);

		yggr_test_assert(ret->first == 5);
		yggr_test_assert(ret->second == 50);

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, 5));

		key_type key = 5;
		mapped_type map_val = 60;

		ret = yggr::container::try_emplace(map, map.begin(), boost::move(key), boost::move(map_val));

		yggr_test_assert(ret->first == 5);
		yggr_test_assert(ret->second == 50);

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, 5));
	}
}

template<typename Map>
void test_try_emplace_tpl2(void)
{
	typedef Map map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::size_type size_type;
	typedef typename map_type::iterator iter_type;

	key_type arr[] = {"1", "1", "2", "2", "3", "4"};
	{
		map_type map;
		{
			for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type);
					i != isize; ++i)
			{
				map.insert(value_type(*i, *i));
			}
		}

		// test try_emplace(key_cref, args_rvref...) succeed
		key_type key1 = "5";
		mapped_type map_val1 = "50";

		size_type old_size = map.size();

		std::pair<iter_type, bool> ret =
			yggr::container::try_emplace(map, static_cast<const key_type&>(key1), boost::move(map_val1));

		yggr_test_assert(ret.first->first == "5");
		yggr_test_assert(ret.first->second == "50");
		yggr_test_assert(ret.second);
		yggr_test_assert(key1 == "5");
		yggr_test_assert(map_val1.empty());

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, "5"));

		// test try_emplace(key_rvref, args_rvref...) failed
		key_type key2 = "5";
		mapped_type map_val2 = "60";

		ret = yggr::container::try_emplace(map, boost::move(key2), boost::move(map_val2));

		yggr_test_assert(key2 == "5");
		yggr_test_assert(map_val2 == "60");

		yggr_test_assert(ret.first->first == "5");
		yggr_test_assert(ret.first->second == "50");
		yggr_test_assert(!ret.second);

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, "5"));

		// test try_emplace(key_rvref, args_rvref...) succeed
		key_type key3 = "6";
		mapped_type map_val3 = "60";

		ret = yggr::container::try_emplace(map, boost::move(key3), boost::move(map_val3));

		yggr_test_assert(key3.empty());
		yggr_test_assert(map_val3.empty());

		yggr_test_assert(ret.first->first == "6");
		yggr_test_assert(ret.first->second == "60");
		yggr_test_assert(ret.second);

		yggr_test_assert(map.size() == old_size + 2);
		yggr_test_assert(yggr::container::contains(map, "6"));
	}

	{
		map_type map;
		{
			for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type);
					i != isize; ++i)
			{
				map.insert(value_type(*i, *i));
			}
		}

		// test try_emplace(key_cref, args_rvref...) succeed
		key_type key1 = "5";
		mapped_type map_val1 = "50";

		size_type old_size = map.size();

		iter_type ret =
			yggr::container::try_emplace(map, map.begin(), static_cast<const key_type&>(key1), boost::move(map_val1));

		yggr_test_assert(ret->first == "5");
		yggr_test_assert(ret->second == "50");
		yggr_test_assert(ret != map.end());
		yggr_test_assert(key1 == "5");
		yggr_test_assert(map_val1.empty());

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, "5"));

		// test try_emplace(key_rvref, args_rvref...) failed
		key_type key2 = "5";
		mapped_type map_val2 = "60";

		ret = yggr::container::try_emplace(map, map.begin(), boost::move(key2), boost::move(map_val2));

		yggr_test_assert(key2 == "5");
		yggr_test_assert(map_val2 == "60");

		yggr_test_assert(ret->first == "5");
		yggr_test_assert(ret->second == "50");
		yggr_test_assert(ret != map.end());

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, "5"));

		// test try_emplace(key_rvref, args_rvref...) succeed
		key_type key3 = "6";
		mapped_type map_val3 = "60";

		ret = yggr::container::try_emplace(map, map.begin(), boost::move(key3), boost::move(map_val3));

#if (BOOST_VERSION < 106400)
	yggr_test_assert(key3.empty());
#else
#	if defined(YGGR_FIX_BOOST_UNORDERED_TRY_EMPLACE_HINT_NOT_MOVE_KEY)
		yggr_test_assert(key3.empty());
#	endif // YGGR_FIX_BOOST_UNORDERED_TRY_EMPLACE_HINT_NOT_MOVE_KEY
#endif // (BOOST_VERSION < 106400)

		yggr_test_assert(map_val3.empty());

		yggr_test_assert(ret->first == "6");
		yggr_test_assert(ret->second == "60");
		yggr_test_assert(ret != map.end());

		yggr_test_assert(map.size() == old_size + 2);
		yggr_test_assert(yggr::container::contains(map, "6"));
	}
}

void test_try_emplace(void)
{
	std::cout << "------------test_try_emplace start-----------" << std::endl;

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	test_try_emplace_tpl< std::unordered_map<int, int> >();
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP
	test_try_emplace_tpl< boost::unordered_map<int, int> >();
	test_try_emplace_tpl< yggr::unordered_map<int, int> >();

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	test_try_emplace_tpl2< std::unordered_map<yggr::string, yggr::string> >();
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP
	test_try_emplace_tpl2< boost::unordered_map<yggr::string, yggr::string> >();
	test_try_emplace_tpl2< yggr::unordered_map<yggr::string, yggr::string> >();

	std::cout << "------------test_try_emplace end-----------" << std::endl;
}

template<typename Map>
void test_insert_or_assign_tpl(void)
{
	typedef Map map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::size_type size_type;
	typedef typename map_type::iterator iter_type;

	key_type arr[] = {1, 1, 2, 2, 3, 4};

	{
		map_type map;
		{
			for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type);
					i != isize; ++i)
			{
				yggr::container::insert_or_assign(map, *i, (*i) * 10);
			}
		}

		size_type old_size = map.size();

		std::pair<iter_type, bool> ret = yggr::container::insert_or_assign(map, 5, 50);

		yggr_test_assert(ret.first->first == 5);
		yggr_test_assert(ret.first->second == 50);
		yggr_test_assert(ret.second);

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, 5));

		key_type key = 5;
		mapped_type map_val = 60;

		ret = yggr::container::try_emplace(map, boost::move(key), boost::move(map_val));

		yggr_test_assert(ret.first->first == 5);
		yggr_test_assert(ret.first->second == 50);
		yggr_test_assert(!ret.second);

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, 5));

		key_type key2 = 6;
		mapped_type map_val2 = 60;

		ret = yggr::container::try_emplace(map, boost::move(key2), boost::move(map_val2));

		yggr_test_assert(ret.first->first == 6);
		yggr_test_assert(ret.first->second == 60);
		yggr_test_assert(ret.second);

		yggr_test_assert(map.size() == old_size + 2);
		yggr_test_assert(yggr::container::contains(map, 6));
	}

	{
		map_type map;
		{
			for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type);
					i != isize; ++i)
			{
				yggr::container::insert_or_assign(map, *i, (*i) * 10);
			}
		}

		size_type old_size = map.size();

		iter_type ret = yggr::container::insert_or_assign(map, map.begin(), 5, 50);

		yggr_test_assert(ret->first == 5);
		yggr_test_assert(ret->second == 50);
		yggr_test_assert(ret != map.end());

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, 5));

		key_type key = 5;
		mapped_type map_val = 60;

		ret = yggr::container::try_emplace(map, map.begin(), boost::move(key), boost::move(map_val));

		yggr_test_assert(ret->first == 5);
		yggr_test_assert(ret->second == 50);
		yggr_test_assert(ret != map.end());

		yggr_test_assert(map.size() == old_size + 1);
		yggr_test_assert(yggr::container::contains(map, 5));

		key_type key2 = 6;
		mapped_type map_val2 = 60;

		ret = yggr::container::try_emplace(map, map.begin(), boost::move(key2), boost::move(map_val2));

		yggr_test_assert(ret->first == 6);
		yggr_test_assert(ret->second == 60);
		yggr_test_assert(ret != map.end());

		yggr_test_assert(map.size() == old_size + 2);
		yggr_test_assert(yggr::container::contains(map, 6));
	}
}

void test_insert_or_assign(void)
{
	std::cout << "------------test_insert_or_assign start-----------" << std::endl;

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	test_insert_or_assign_tpl< std::unordered_map<int, int> >();
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP
	test_insert_or_assign_tpl< boost::unordered_map<int, int> >();
	test_insert_or_assign_tpl< yggr::unordered_map<int, int> >();

	std::cout << "------------test_insert_or_assign end-----------" << std::endl;
}

// gcc 14.2 and later not support function inner struct or class
template<typename Val>
struct cmper_first_less_3
{
    typedef Val value_type;

    inline bool operator()(const value_type& val) const
    {
        return val.first < 3;
    }
};

template<typename Val>
struct cmper_first_greater_4
{
    typedef Val value_type;

    inline bool operator()(value_type val) const
    {
        return val.first > 4;
    }
};

template<typename Map>
void test_erase_if_tpl(void)
{
	typedef Map map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::value_type value_type;
	typedef typename map_type::size_type size_type;

	key_type arr[] = {1, 1, 2, 2, 3, 4};

	{
		typedef cmper_first_less_3<value_type> now_cmper_type;

		map_type map;
		{
			for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type);
					i != isize; ++i)
			{
				map.insert(value_type(*i, (*i) * 10));
			}
		}

		size_type old_size = map.size();
		size_type ret_size = 0;

		ret_size = yggr::container::erase_if(map, now_cmper_type());

		yggr_test_assert(ret_size);
		yggr_test_assert(map.size() == old_size - ret_size);
	}

	{
		typedef cmper_first_greater_4<value_type> now_cmper_type;

		map_type map;
		{
			for(const key_type *i = arr, *isize = arr + sizeof(arr) / sizeof(key_type);
					i != isize; ++i)
			{
				map.insert(value_type(*i, (*i) * 10));
			}
		}

		size_type old_size = map.size();
		size_type ret_size = 0;

		ret_size = yggr::container::erase_if(map, now_cmper_type());

		yggr_test_assert(ret_size == 0);
		yggr_test_assert(map.size() == old_size - ret_size);
	}
}

void test_erase_if(void)
{
	std::cout << "------------test_erase_if start-----------" << std::endl;

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	test_erase_if_tpl< std::unordered_map<int, int> >();
	test_erase_if_tpl< std::unordered_multimap<int, int> >();
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

	test_erase_if_tpl< boost::unordered_map<int, int> >();
	test_erase_if_tpl< boost::unordered_multimap<int, int> >();

	test_erase_if_tpl< yggr::unordered_map<int, int> >();
	test_erase_if_tpl< yggr::unordered_multimap<int, int> >();

	std::cout << "------------test_erase_if end-----------" << std::endl;
}

int main(int argc, char* argv[])
{

	test_extract_insert();
	test_merge();
	test_contains();
	test_try_emplace();
	test_insert_or_assign();
	test_erase_if();

	wait_any_key(argc, argv);
	return 0;
}
