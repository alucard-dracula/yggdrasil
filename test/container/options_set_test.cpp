// options_set_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/options_set.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_construct(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::options_set<int> set_type;
	typedef
		yggr::options_set
		<
			set_type::value_type, 
			set_type::key_compare, 
			set_type::allocator_type,
			set_type::options_type
		> isomeric_same_set_type;

	typedef set_type::base_type base_set_type;
	typedef isomeric_same_set_type::base_type isomeric_same_base_set_type;

	set_type set0(arr + 0, arr + 5);
	yggr_test_assert(set0.size() == 3);

	{
		set_type set;
		yggr_test_assert(set.empty());
	}

	{
		set_type set(set0.get_allocator());
		yggr_test_assert(set.empty());
	}

	{
		set_type set(set0.key_comp(), set0.get_allocator());
		yggr_test_assert(set.size() == 0);
	}

	{
		set_type set(arr + 0, arr + 5);
		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}

	
	{
		set_type set(arr + 0, arr + 5, set0.get_allocator());
		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}

	{
		set_type set(arr + 0, arr + 5, set0.key_comp(), set0.get_allocator());
		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	{
		set_type set({1, 2, 3, 4, 5});
		yggr_test_assert(set.size() == 5);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 5);
	}

	{
		set_type set({1, 2, 3, 4, 5}, set0.key_comp(), set0.get_allocator());
		yggr_test_assert(set.size() == 5);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 5);
	}

	{
		set_type set({1, 2, 3, 4, 5}, set0.get_allocator());
		yggr_test_assert(set.size() == 5);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 5);
	}

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	{
		base_set_type tmp(arr + 0, arr + 5);
		set_type set(tmp);

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(set == tmp);
		yggr_test_assert(tmp == set);
	}

	{
		set_type tmp(arr + 0, arr + 5);
		set_type set(tmp);

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(set == tmp);
		yggr_test_assert(tmp == set);
	}

	{
		isomeric_same_base_set_type tmp(arr + 0, arr + 5);
		set_type set(tmp);

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(set == tmp);
		yggr_test_assert(tmp == set);
	}

	{
		isomeric_same_set_type tmp(arr + 0, arr + 5);
		set_type set(tmp);

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(set == tmp);
		yggr_test_assert(tmp == set);
	}

	{
		base_set_type tmp(arr + 0, arr + 5);
		set_type set(boost::move(tmp));

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}

	{
		set_type tmp(arr + 0, arr + 5);
		set_type set(boost::move(tmp));

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}

	{
		isomeric_same_base_set_type tmp(arr + 0, arr + 5);
		set_type set(boost::move(tmp));

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}

	{
		isomeric_same_set_type tmp(arr + 0, arr + 5);
		set_type set(boost::move(tmp));

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}

	{
		base_set_type tmp(arr + 0, arr + 5);
		set_type set(tmp, tmp.get_allocator());

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(set == tmp);
		yggr_test_assert(tmp == set);
	}

	{
		set_type tmp(arr + 0, arr + 5);
		set_type set(tmp, tmp.get_allocator());

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(set == tmp);
		yggr_test_assert(tmp == set);
	}

	{
		isomeric_same_base_set_type tmp(arr + 0, arr + 5);
		set_type set(tmp, tmp.get_allocator());

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(set == tmp);
		yggr_test_assert(tmp == set);
	}

	{
		isomeric_same_set_type tmp(arr + 0, arr + 5);
		set_type set(tmp, tmp.get_allocator());

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(set == tmp);
		yggr_test_assert(tmp == set);
	}

	{
		base_set_type tmp(arr + 0, arr + 5);
		set_type set(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}


	{
		set_type tmp(arr + 0, arr + 5);
		set_type set(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}

	{
		isomeric_same_base_set_type tmp(arr + 0, arr + 5);
		set_type set(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}

	{
		isomeric_same_set_type tmp(arr + 0, arr + 5);
		set_type set(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(set.size() == 3);
		yggr_test_assert(*set.begin() == 1);
		yggr_test_assert(*set.rbegin() == 3);
	}

	std::cout << "------------test_construct end---------------" << std::endl;
}

void test_compare(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	int arr_less[5] = {1, 1, 2, 2, 2};
	int arr_greater[5] = {1, 1, 2, 2, 4};
	
	typedef yggr::options_set<int> set_type;
	typedef
		yggr::options_set
		<
			set_type::value_type, 
			set_type::key_compare, 
			set_type::allocator_type,
			set_type::options_type
		> isomeric_same_set_type;

	typedef set_type::base_type base_set_type;
	typedef isomeric_same_set_type::base_type isomeric_same_base_set_type;

	set_type set(arr + 0, arr + 5);

	set_type set_equal(arr + 0, arr + 5);
	isomeric_same_set_type set_same_equal(arr + 0, arr + 5);

	set_type set_less(arr_less, arr_less + 5);
	isomeric_same_set_type set_same_less(arr_less, arr_less + 5);

	set_type set_greater(arr_greater, arr_greater + 5);
	isomeric_same_set_type set_same_greater(arr_greater, arr_greater + 5);

	{
		// ==
		yggr_test_assert(static_cast<const base_set_type&>(set) == set);
		yggr_test_assert(set == static_cast<const base_set_type&>(set));
		yggr_test_assert(set == set);

		yggr_test_assert(static_cast<const base_set_type&>(set) == set_same_equal);
		yggr_test_assert(set == static_cast<const isomeric_same_base_set_type&>(set_same_equal));
		yggr_test_assert(set == set_same_equal);

		// != 
		yggr_test_assert(static_cast<const base_set_type&>(set) != set_less);
		yggr_test_assert(set != static_cast<const base_set_type&>(set_less));
		yggr_test_assert(set != set_less);

		yggr_test_assert(static_cast<const base_set_type&>(set) != set_same_less);
		yggr_test_assert(set != static_cast<const isomeric_same_base_set_type&>(set_same_less));
		yggr_test_assert(set != set_same_less);

		// <
		yggr_test_assert(static_cast<const base_set_type&>(set) < set_greater);
		yggr_test_assert(set < static_cast<const base_set_type&>(set_greater));
		yggr_test_assert(set < set_greater);

		yggr_test_assert(static_cast<const base_set_type&>(set) < set_same_greater);
		yggr_test_assert(set < static_cast<const isomeric_same_base_set_type&>(set_same_greater));
		yggr_test_assert(set < set_same_greater);

		// <=
		yggr_test_assert(static_cast<const base_set_type&>(set) <= set);
		yggr_test_assert(set <= static_cast<const base_set_type&>(set));
		yggr_test_assert(set <= set);

		yggr_test_assert(static_cast<const base_set_type&>(set) <= set_equal);
		yggr_test_assert(set <= static_cast<const base_set_type&>(set_equal));
		yggr_test_assert(set <= set_equal);

		yggr_test_assert(static_cast<const base_set_type&>(set) <= set_same_equal);
		yggr_test_assert(set <= static_cast<const isomeric_same_base_set_type&>(set_same_equal));
		yggr_test_assert(set <= set_same_equal);

		yggr_test_assert(static_cast<const base_set_type&>(set) <= set_greater);
		yggr_test_assert(set <= static_cast<const base_set_type&>(set_greater));
		yggr_test_assert(set <= set_greater);

		yggr_test_assert(static_cast<const base_set_type&>(set) <= set_same_greater);
		yggr_test_assert(set <= static_cast<const isomeric_same_base_set_type&>(set_same_greater));
		yggr_test_assert(set <= set_same_greater);

		// >
		yggr_test_assert(static_cast<const base_set_type&>(set) > set_less);
		yggr_test_assert(set > static_cast<const base_set_type&>(set_less));
		yggr_test_assert(set > set_less);

		yggr_test_assert(static_cast<const base_set_type&>(set) > set_same_less);
		yggr_test_assert(set > static_cast<const isomeric_same_base_set_type&>(set_same_less));
		yggr_test_assert(set > set_same_less);

		// >=
		yggr_test_assert(static_cast<const base_set_type&>(set) >= set);
		yggr_test_assert(set >= static_cast<const base_set_type&>(set));
		yggr_test_assert(set >= set);

		yggr_test_assert(static_cast<const base_set_type&>(set) >= set_equal);
		yggr_test_assert(set >= static_cast<const base_set_type&>(set_equal));
		yggr_test_assert(set >= set_equal);

		yggr_test_assert(static_cast<const base_set_type&>(set) >= set_same_equal);
		yggr_test_assert(set >= static_cast<const isomeric_same_base_set_type&>(set_same_equal));
		yggr_test_assert(set >= set_same_equal);

		yggr_test_assert(static_cast<const base_set_type&>(set) >= set_less);
		yggr_test_assert(set >= static_cast<const base_set_type&>(set_less));
		yggr_test_assert(set >= set_less);

		yggr_test_assert(static_cast<const base_set_type&>(set) >= set_same_less);
		yggr_test_assert(set >= static_cast<const isomeric_same_base_set_type&>(set_same_less));
		yggr_test_assert(set >= set_same_less);

	}

	std::cout << "------------test_compare end---------------" << std::endl;
}

template<typename LCont, typename RCont>
void test_member_merge_ss(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::value_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		cont_eins.merge(boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		cont_eins.merge(cont_zwei);
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}
}

template<typename LCont, typename RCont>
void test_member_merge_sm(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::value_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 10);
		cont_eins.merge(boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 6);
	}

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 10);
		cont_eins.merge(cont_zwei);
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 6);
	}
}

template<typename LCont, typename RCont>
void test_member_merge_ms(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::value_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 5);
		cont_eins.merge(boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 15);
		yggr_test_assert(cont_zwei.size() == 0);
	}

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 5);
		cont_eins.merge(cont_zwei);
		yggr_test_assert(cont_eins.size() == 15);
		yggr_test_assert(cont_zwei.size() == 0);
	}
}

template<typename LCont, typename RCont>
void test_member_merge_mm(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::value_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 10);
		cont_eins.merge(boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 20);
		yggr_test_assert(cont_zwei.size() == 0);
	}

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 10);
		cont_eins.merge(cont_zwei);
		yggr_test_assert(cont_eins.size() == 20);
		yggr_test_assert(cont_zwei.size() == 0);
	}
}

template<typename LCont, typename RCont>
void test_merge_ss(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::value_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		yggr::container::merge(cont_eins, boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		yggr::container::merge(cont_eins, cont_zwei);
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}
}

template<typename LCont, typename RCont>
void test_merge_sm(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::value_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 10);
		yggr::container::merge(cont_eins, boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 6);
	}

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 10);
		yggr::container::merge(cont_eins, cont_zwei);
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 6);
	}
}

template<typename LCont, typename RCont>
void test_merge_ms(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::value_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 5);
		yggr::container::merge(cont_eins, boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 15);
		yggr_test_assert(cont_zwei.size() == 0);
	}

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 5);
		yggr::container::merge(cont_eins, cont_zwei);
		yggr_test_assert(cont_eins.size() == 15);
		yggr_test_assert(cont_zwei.size() == 0);
	}
}

template<typename LCont, typename RCont>
void test_merge_mm(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::value_type val_type;

	val_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	val_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 10);
		yggr::container::merge(cont_eins, boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 20);
		yggr_test_assert(cont_zwei.size() == 0);
	}

	{
		l_cont_type cont_eins(arr1 + 0, arr1 + 10);
		r_cont_type cont_zwei(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont_eins.size() == 10);
		yggr_test_assert(cont_zwei.size() == 10);
		yggr::container::merge(cont_eins, cont_zwei);
		yggr_test_assert(cont_eins.size() == 20);
		yggr_test_assert(cont_zwei.size() == 0);
	}
}

void test_merge(void)
{
	typedef int val_type;
	typedef std::set<val_type> stl_set_type;
	typedef std::multiset<val_type> stl_mset_type;

	typedef boost::container::set<val_type> boost_set_type;
	typedef boost::container::multiset<val_type> boost_mset_type;

	typedef yggr::set<val_type> yggr_set_type;
	typedef yggr::multiset<val_type> yggr_mset_type;

	typedef yggr::options_set<val_type> yggr_opt_set_type;
	typedef yggr::options_multiset<val_type> yggr_opt_mset_type;

	// ss
	test_merge_ss<stl_set_type, stl_set_type>();
	test_merge_ss<stl_set_type, boost_set_type>();
	test_merge_ss<stl_set_type, yggr_set_type>();
	test_merge_ss<stl_set_type, yggr_opt_set_type>();

	test_merge_ss<boost_set_type, stl_set_type>();
	test_merge_ss<boost_set_type, boost_set_type>();
	test_merge_ss<boost_set_type, yggr_set_type>();
	test_merge_ss<boost_set_type, yggr_opt_set_type>();

	test_merge_ss<yggr_set_type, stl_set_type>();
	test_merge_ss<yggr_set_type, boost_set_type>();
	test_merge_ss<yggr_set_type, yggr_set_type>();
	test_merge_ss<yggr_set_type, yggr_opt_set_type>();

	test_merge_ss<yggr_opt_set_type, stl_set_type>();
	test_merge_ss<yggr_opt_set_type, boost_set_type>();
	test_merge_ss<yggr_opt_set_type, yggr_set_type>();
	test_merge_ss<yggr_opt_set_type, yggr_opt_set_type>();

	// sm
	test_merge_sm<stl_set_type, stl_mset_type>();
	test_merge_sm<stl_set_type, boost_mset_type>();
	test_merge_sm<stl_set_type, yggr_mset_type>();
	test_merge_sm<stl_set_type, yggr_opt_mset_type>();

	test_merge_sm<boost_set_type, stl_mset_type>();
	test_merge_sm<boost_set_type, boost_mset_type>();
	test_merge_sm<boost_set_type, yggr_mset_type>();
	test_merge_sm<boost_set_type, yggr_opt_mset_type>();

	test_merge_sm<yggr_set_type, stl_mset_type>();
	test_merge_sm<yggr_set_type, boost_mset_type>();
	test_merge_sm<yggr_set_type, yggr_mset_type>();
	test_merge_sm<yggr_set_type, yggr_opt_mset_type>();

	test_merge_sm<yggr_opt_set_type, stl_mset_type>();
	test_merge_sm<yggr_opt_set_type, boost_mset_type>();
	test_merge_sm<yggr_opt_set_type, yggr_mset_type>();
	test_merge_sm<yggr_opt_set_type, yggr_opt_mset_type>();

	// ms
	test_merge_ms<stl_mset_type, stl_set_type>();
	test_merge_ms<stl_mset_type, boost_set_type>();
	test_merge_ms<stl_mset_type, yggr_set_type>();
	test_merge_ms<stl_mset_type, yggr_opt_set_type>();

	test_merge_ms<boost_mset_type, stl_set_type>();
	test_merge_ms<boost_mset_type, boost_set_type>();
	test_merge_ms<boost_mset_type, yggr_set_type>();
	test_merge_ms<boost_mset_type, yggr_opt_set_type>();

	test_merge_ms<yggr_mset_type, stl_set_type>();
	test_merge_ms<yggr_mset_type, boost_set_type>();
	test_merge_ms<yggr_mset_type, yggr_set_type>();
	test_merge_ms<yggr_mset_type, yggr_opt_set_type>();

	test_merge_ms<yggr_opt_mset_type, stl_set_type>();
	test_merge_ms<yggr_opt_mset_type, boost_set_type>();
	test_merge_ms<yggr_opt_mset_type, yggr_set_type>();
	test_merge_ms<yggr_opt_mset_type, yggr_opt_set_type>();

	// mm
	test_merge_mm<stl_mset_type, stl_mset_type>();
	test_merge_mm<stl_mset_type, boost_mset_type>();
	test_merge_mm<stl_mset_type, yggr_mset_type>();
	test_merge_mm<stl_mset_type, yggr_opt_mset_type>();

	test_merge_mm<boost_mset_type, stl_mset_type>();
	test_merge_mm<boost_mset_type, boost_mset_type>();
	test_merge_mm<boost_mset_type, yggr_mset_type>();
	test_merge_mm<boost_mset_type, yggr_opt_mset_type>();

	test_merge_mm<yggr_mset_type, stl_mset_type>();
	test_merge_mm<yggr_mset_type, boost_mset_type>();
	test_merge_mm<yggr_mset_type, yggr_mset_type>();
	test_merge_mm<yggr_mset_type, yggr_opt_mset_type>();

	test_merge_mm<yggr_opt_mset_type, stl_mset_type>();
	test_merge_mm<yggr_opt_mset_type, boost_mset_type>();
	test_merge_mm<yggr_opt_mset_type, yggr_mset_type>();
	test_merge_mm<yggr_opt_mset_type, yggr_opt_mset_type>();

#if !(BOOST_VERSION < 106200)
	// ss
	test_member_merge_ss<yggr_set_type, stl_set_type>();
	test_member_merge_ss<yggr_set_type, boost_set_type>();
	test_member_merge_ss<yggr_set_type, yggr_set_type>();
	test_member_merge_ss<yggr_set_type, yggr_opt_set_type>();

	test_member_merge_ss<yggr_opt_set_type, stl_set_type>();
	test_member_merge_ss<yggr_opt_set_type, boost_set_type>();
	test_member_merge_ss<yggr_opt_set_type, yggr_set_type>();
	test_member_merge_ss<yggr_opt_set_type, yggr_opt_set_type>();

	// sm
	test_member_merge_sm<yggr_set_type, stl_mset_type>();
	test_member_merge_sm<yggr_set_type, boost_mset_type>();
	test_member_merge_sm<yggr_set_type, yggr_mset_type>();
	test_member_merge_sm<yggr_set_type, yggr_opt_mset_type>();

	test_member_merge_sm<yggr_opt_set_type, stl_mset_type>();
	test_member_merge_sm<yggr_opt_set_type, boost_mset_type>();
	test_member_merge_sm<yggr_opt_set_type, yggr_mset_type>();
	test_member_merge_sm<yggr_opt_set_type, yggr_opt_mset_type>();

	// ms
	test_member_merge_ms<yggr_mset_type, stl_set_type>();
	test_member_merge_ms<yggr_mset_type, boost_set_type>();
	test_member_merge_ms<yggr_mset_type, yggr_set_type>();
	test_member_merge_ms<yggr_mset_type, yggr_opt_set_type>();

	test_member_merge_ms<yggr_opt_mset_type, stl_set_type>();
	test_member_merge_ms<yggr_opt_mset_type, boost_set_type>();
	test_member_merge_ms<yggr_opt_mset_type, yggr_set_type>();
	test_member_merge_ms<yggr_opt_mset_type, yggr_opt_set_type>();

	// mm
	test_member_merge_mm<yggr_mset_type, stl_mset_type>();
	test_member_merge_mm<yggr_mset_type, boost_mset_type>();
	test_member_merge_mm<yggr_mset_type, yggr_mset_type>();
	test_member_merge_mm<yggr_mset_type, yggr_opt_mset_type>();

	test_member_merge_mm<yggr_opt_mset_type, stl_mset_type>();
	test_member_merge_mm<yggr_opt_mset_type, boost_mset_type>();
	test_member_merge_mm<yggr_opt_mset_type, yggr_mset_type>();
	test_member_merge_mm<yggr_opt_mset_type, yggr_opt_mset_type>();

#endif // !(BOOST_VERSION < 106200)

	std::cout << "------------test_merge end---------------" << std::endl;
}

template<std::size_t V>
struct test_pred_t
{
	inline bool operator()(int n) const
	{
		return n == V;
	}
};

void test_erase_if(void)
{
	int arr[5] = {1, 1, 2, 2, 3};

	{
		typedef yggr::options_set<int> set_type;

		set_type set(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase_if(set, test_pred_t<1>());
		yggr_test_assert(s == 1);

		s = yggr::container::erase_if(set, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	std::cout << "------------test_erase_if end---------------" << std::endl;
}

int main(int argc, char* argv[])
{

	test_construct();
	test_compare();
	test_merge();

	test_erase_if();

	wait_any_key(argc, argv);
	return 0;
}