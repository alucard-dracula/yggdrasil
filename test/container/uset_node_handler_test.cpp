// uset_node_handler_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/container/unordered_set.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

// extract and insert
template<typename Set, typename Rst> inline
typename
	boost::enable_if
	<
		yggr::iterator_ex::is_iterator<Rst>,
		bool
	>::type
	check_insert_result(Set& set, Rst rst)
{
	return rst != set.end();
}

template<typename Set, typename Rst> inline
typename
	boost::disable_if
	<
		yggr::iterator_ex::is_iterator<Rst>,
		bool
	>::type
	check_insert_result(Set& set, const Rst& rst)
{
	return rst.inserted;
}

template<typename Set>
void test_extract_insert_tpl(void)
{
	typename Set::value_type arr[] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};

	typedef Set set_type;
	typedef typename set_type::iterator now_iter_type;
	typedef typename set_type::const_iterator now_citer_type;
	typedef typename yggr::container::node_handle_traits<set_type> node_handle_traits_type;
	typedef typename node_handle_traits_type::node_type now_node_type;
	typedef typename node_handle_traits_type::insert_return_type now_insert_return_type;
	typedef typename node_handle_traits_type::hint_insert_return_type now_hint_insert_return_type;

	set_type set;
	set.insert(arr + 0, arr + sizeof(arr) / sizeof(typename Set::value_type));

	{
		now_node_type node = yggr::container::extract(set, 1);
		std::cout << node.value() << std::endl;
		yggr_test_assert(node.value() == 1);

		now_insert_return_type iret = yggr::container::insert(set, boost::move(node));

		yggr_test_assert(check_insert_result(set, iret));
		yggr_test_assert(set.find(1) != set.end());
	}

	{
		now_citer_type iter = set.find(2);
		yggr_test_assert(iter != set.end());
		now_node_type node = yggr::container::extract(set, iter);
		std::cout << node.value() << std::endl;
		yggr_test_assert(node.value() == 2);

		now_hint_insert_return_type iret = yggr::container::insert(set, set.end(), boost::move(node));

		yggr_test_assert(check_insert_result(set, iret));
		yggr_test_assert(set.find(2) != set.end());
	}

	//{
	//	now_node_type node = yggr::container::extract(set, set.end()); // assert false
	//	yggr_test_assert(node.empty());
	//}

	{
		now_node_type node = yggr::container::extract(set, 666);
		yggr_test_assert(node.empty());
	}
}

void test_extract_insert(void)
{

	std::cout << "------------test_extract_insert start-----------" << std::endl;

#if defined(YGGR_USING_CXX17)
	test_extract_insert_tpl< std::unordered_set<int> >();
	test_extract_insert_tpl< std::unordered_multiset<int> >();
#endif // YGGR_USING_CXX17
	test_extract_insert_tpl< boost::unordered_set<int> >();
	test_extract_insert_tpl< boost::unordered_multiset<int> >();
	test_extract_insert_tpl< yggr::unordered_set<int> >();
	test_extract_insert_tpl< yggr::unordered_multiset<int> >();

	std::cout << "------------test_extract_insert end-----------" << std::endl;
}

// merge

template<typename SetL, typename SetR>
void test_merge_tpl(void)
{

	typedef SetL left_set_type;
	typedef SetR right_set_type;

	typename left_set_type::value_type larr[] = {1, 1, 2, 2, 3, 3};
	typename right_set_type::value_type rarr[] = {3, 3, 4, 4, 5, 6};


	{
		left_set_type lset;
		right_set_type rset;

		lset.insert(larr, larr + sizeof(larr) / sizeof(typename left_set_type::value_type));
		rset.insert(rarr, rarr + sizeof(rarr) / sizeof(typename right_set_type::value_type));

		yggr::container::merge(lset, rset);

		yggr_test_assert(rset.size() <= lset.size());

		{
			for(typename left_set_type::value_type *i = larr, *isize = larr + 6; i != isize; ++i)
			{
				yggr_test_assert(lset.find(*i) != lset.end());
			}
		}

		{
			for(typename right_set_type::value_type* i = rarr, *isize = rarr + 6; i != isize; ++i)
			{
				yggr_test_assert(lset.find(*i) != lset.end());
			}
		}

	}

	{
		left_set_type lset;
		right_set_type rset;

		lset.insert(larr, larr + sizeof(larr) / sizeof(typename left_set_type::value_type));
		rset.insert(rarr, rarr + sizeof(rarr) / sizeof(typename right_set_type::value_type));

		yggr::container::merge(lset, boost::move(rset));

		yggr_test_assert(rset.size() <= lset.size());
		{
			for(typename left_set_type::value_type* i = larr, *isize = larr + 6; i != isize; ++i)
			{
				yggr_test_assert(lset.find(*i) != lset.end());
			}
		}

		{
			for(typename right_set_type::value_type* i = rarr, *isize = rarr + 6; i != isize; ++i)
			{
				yggr_test_assert(lset.find(*i) != lset.end());
			}
		}
	}
}

template<typename SetL, typename SetR>
void test_merge_tpl2(void)
{
	typedef SetL left_set_type;
	typedef SetR right_set_type;

	typename left_set_type::value_type larr[] = {1, 1, 2, 2, 3, 3};
	typename right_set_type::value_type rarr[] = {3, 3, 4, 4, 5, 6};


	{
		left_set_type lset;
		right_set_type rset;

		lset.insert(larr, larr + sizeof(larr) / sizeof(typename left_set_type::value_type));
		rset.insert(rarr, rarr + sizeof(rarr) / sizeof(typename right_set_type::value_type));

		lset.merge(rset);

		yggr_test_assert(rset.size() <= lset.size());
		{
			for(typename left_set_type::value_type* i = larr, *isize = larr + 6; i != isize; ++i)
			{
				yggr_test_assert(lset.find(*i) != lset.end());
			}
		}

		{
			for(typename right_set_type::value_type* i = rarr, *isize = rarr + 6; i != isize; ++i)
			{
				yggr_test_assert(lset.find(*i) != lset.end());
			}
		}
	}

	{
		left_set_type lset;
		right_set_type rset;

		lset.insert(larr, larr + sizeof(larr) / sizeof(typename left_set_type::value_type));
		rset.insert(rarr, rarr + sizeof(rarr) / sizeof(typename right_set_type::value_type));

		lset.merge(boost::move(rset));

		yggr_test_assert(rset.size() <= lset.size());
		{
			for(typename left_set_type::value_type* i = larr, *isize = larr + 6; i != isize; ++i)
			{
				yggr_test_assert(lset.find(*i) != lset.end());
			}
		}

		{
			for(typename right_set_type::value_type* i = rarr, *isize = rarr + 6; i != isize; ++i)
			{
				yggr_test_assert(lset.find(*i) != lset.end());
			}
		}
	}
}

void test_merge(void)
{

	std::cout << "------------test_merge start-----------" << std::endl;

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
	// std::unordered_set
	test_merge_tpl< std::unordered_set<int>, std::unordered_set<int> >();
	test_merge_tpl< std::unordered_set<int>, std::unordered_multiset<int> >();

	test_merge_tpl< std::unordered_set<int>, boost::unordered_set<int> >();
	test_merge_tpl< std::unordered_set<int>, boost::unordered_multiset<int> >();

	test_merge_tpl< std::unordered_set<int>, yggr::unordered_set<int> >();
	test_merge_tpl< std::unordered_set<int>, yggr::unordered_multiset<int> >();

	// std::unordered_multiset
	test_merge_tpl< std::unordered_multiset<int>, std::unordered_set<int> >();
	test_merge_tpl< std::unordered_multiset<int>, std::unordered_multiset<int> >();

	test_merge_tpl< std::unordered_multiset<int>, boost::unordered_set<int> >();
	test_merge_tpl< std::unordered_multiset<int>, boost::unordered_multiset<int> >();

	test_merge_tpl< std::unordered_multiset<int>, yggr::unordered_set<int> >();
	test_merge_tpl< std::unordered_multiset<int>, yggr::unordered_multiset<int> >();

#endif // #if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

	// boost::contianer::unordered_set
#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

	//test_merge_tpl< boost::unordered_set<int>, std::unordered_set<int> >();
	test_merge_tpl< boost::unordered_set<int>, std::unordered_multiset<int> >();

#endif // #if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

	test_merge_tpl< boost::unordered_set<int>, boost::unordered_set<int> >();
	test_merge_tpl< boost::unordered_set<int>, boost::unordered_multiset<int> >();

	test_merge_tpl< boost::unordered_set<int>, yggr::unordered_set<int> >();
	test_merge_tpl< boost::unordered_set<int>, yggr::unordered_multiset<int> >();

//	// boost::unordered_multiset
#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
	test_merge_tpl< boost::unordered_multiset<int>, std::unordered_set<int> >();
	test_merge_tpl< boost::unordered_multiset<int>, std::unordered_multiset<int> >();
#endif // #if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

	test_merge_tpl< boost::unordered_multiset<int>, boost::unordered_set<int> >();
	test_merge_tpl< boost::unordered_multiset<int>, boost::unordered_multiset<int> >();

	test_merge_tpl< boost::unordered_multiset<int>, yggr::unordered_set<int> >();
	test_merge_tpl< boost::unordered_multiset<int>, yggr::unordered_multiset<int> >();

	// yggr::unordered_set
#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
	test_merge_tpl< yggr::unordered_set<int>, std::unordered_set<int> >();
	test_merge_tpl< yggr::unordered_set<int>, std::unordered_multiset<int> >();
#endif // #if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

	test_merge_tpl< yggr::unordered_set<int>, boost::unordered_set<int> >();
	test_merge_tpl< yggr::unordered_set<int>, boost::unordered_multiset<int> >();

	test_merge_tpl< yggr::unordered_set<int>, yggr::unordered_set<int> >();
	test_merge_tpl< yggr::unordered_set<int>, yggr::unordered_multiset<int> >();

#if !(BOOST_VERSION < 106400)

	test_merge_tpl2< yggr::unordered_set<int>, boost::unordered_set<int> >();
	test_merge_tpl2< yggr::unordered_set<int>, yggr::unordered_set<int> >();

#endif // !(BOOST_VERSION < 106400)

//	// yggr::unordered_multiset
#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
	test_merge_tpl< yggr::unordered_multiset<int>, std::unordered_set<int> >();
	test_merge_tpl< yggr::unordered_multiset<int>, std::unordered_multiset<int> >();
#endif // #if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

	test_merge_tpl< yggr::unordered_multiset<int>, boost::unordered_set<int> >();
	test_merge_tpl< yggr::unordered_multiset<int>, boost::unordered_multiset<int> >();

	test_merge_tpl< yggr::unordered_multiset<int>, yggr::unordered_set<int> >();
	test_merge_tpl< yggr::unordered_multiset<int>, yggr::unordered_multiset<int> >();

#if !(BOOST_VERSION < 106400)

	test_merge_tpl2< yggr::unordered_multiset<int>, boost::unordered_multiset<int> >();

	test_merge_tpl2< yggr::unordered_multiset<int>, yggr::unordered_multiset<int> >();

#endif // !(BOOST_VERSION < 106400)

	std::cout << "------------test_merge end-----------" << std::endl;
}


template<typename Set>
void test_contains_tpl(void)
{
	typedef Set set_type;
	typename set_type::value_type arr[] = {1, 1, 2, 2, 3, 3};

	{
		set_type set;
		set.insert(arr + 0, arr + sizeof(arr) / sizeof(typename set_type::value_type));

		bool bchk = false;

		bchk = yggr::container::contains(set, 1);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(set, 2);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(set, 3);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(set, 4);
		yggr_test_assert(!bchk);
	}

	{
		set_type set;
		set.insert(arr + 0, arr + sizeof(arr) / sizeof(typename set_type::value_type));

		bool bchk = false;

		bchk = yggr::container::contains(set, 1.0);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(set, 2.0);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(set, 3.0);
		yggr_test_assert(bchk);

		bchk = yggr::container::contains(set, 4.0);
		yggr_test_assert(!bchk);
	}
}

void test_contains(void)
{
	std::cout << "------------test_contains start-----------" << std::endl;

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
	test_contains_tpl< std::unordered_set<int> >();
	test_contains_tpl< std::unordered_multiset<int> >();
#endif // #if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

	test_contains_tpl< boost::unordered_set<int> >();
	test_contains_tpl< boost::unordered_multiset<int> >();

	test_contains_tpl< yggr::unordered_set<int> >();
	test_contains_tpl< yggr::unordered_multiset<int> >();

	std::cout << "------------test_contains end-----------" << std::endl;
}

// gcc 14.2 and later not support function inner struct or class
template<typename Val>
struct cmper_val_less_3
{
    typedef Val value_type;

    inline bool operator()(const value_type& val) const
    {
        return val < 3;
    }
};

template<typename Val>
struct cmper_val_greater_4
{
    typedef Val value_type;

    inline bool operator()(value_type val) const
    {
        return val > 4;
    }
};

template<typename Set>
void test_erase_if_tpl(void)
{
	typedef Set set_type;
	typedef typename set_type::value_type value_type;
	typedef typename set_type::size_type size_type;

	value_type arr[] = {1, 1, 2, 2, 3, 4};

	{
		typedef cmper_val_less_3<value_type> now_cmper_type;

		set_type set;
		set.insert(arr + 0, arr + sizeof(arr) / sizeof(typename set_type::value_type));

		size_type old_size = set.size();
		size_type ret_size = 0;

		ret_size = yggr::container::erase_if(set, now_cmper_type());

		yggr_test_assert(ret_size);
		yggr_test_assert(set.size() == old_size - ret_size);
	}

	{
		typedef cmper_val_greater_4<value_type> now_cmper_type;

		set_type set;
		set.insert(arr + 0, arr + sizeof(arr) / sizeof(typename set_type::value_type));

		size_type old_size = set.size();
		size_type ret_size = 0;

		ret_size = yggr::container::erase_if(set, now_cmper_type());

		yggr_test_assert(ret_size == 0);
		yggr_test_assert(set.size() == old_size - ret_size);
	}
}

void test_erase_if(void)
{
	std::cout << "------------test_erase_if start-----------" << std::endl;

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)
	test_erase_if_tpl< std::unordered_set<int> >();
	test_erase_if_tpl< std::unordered_multiset<int> >();
#endif // #if !defined(YGGR_NO_CXX11_HDR_UNORDERED_SET)

	test_erase_if_tpl< boost::unordered_set<int> >();
	test_erase_if_tpl< boost::unordered_multiset<int> >();

	test_erase_if_tpl< yggr::unordered_set<int> >();
	test_erase_if_tpl< yggr::unordered_multiset<int> >();

	std::cout << "------------test_erase_if end-----------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_extract_insert();
	test_merge();
	test_contains();
	test_erase_if();

	wait_any_key(argc, argv);
	return 0;
}
