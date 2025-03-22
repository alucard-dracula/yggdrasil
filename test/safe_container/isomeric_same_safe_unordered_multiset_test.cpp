// isomeric_same_safe_unordered_multiset_test.cpp

// #define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <functional>

#include <boost/utility/enable_if.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/mplex/null_t.hpp>

#include <yggr/safe_container/safe_unordered_set.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)


//-------------------------test base-------------------------

template<typename Cont>
void test_constructor(void)
{
	typedef Cont set_type;
	typedef typename set_type::value_type val_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::base_type base_set_type;
	typedef
		yggr::safe_container::safe_unordered_multiset
		<
			val_type,
			boost::mutex,
			hasher_type,
			key_equal_type,
			alloc_type,
			yggr::unordered_multiset
		> isomeric_same_set_type;
	typedef typename isomeric_same_set_type::base_type isomeric_same_base_set_type;

	BOOST_MPL_ASSERT((boost::is_same<base_set_type, isomeric_same_base_set_type>));

	val_type arr[10] = {1, 2, 3, 4, 5};

	// isomeric_this
	{
		isomeric_same_set_type right_cont(arr + 0, arr + 10);
		set_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		//yggr_test_assert(right_cont.size() == 0); // ub, only test check
	}

	{
		isomeric_same_set_type right_cont(arr + 0, arr + 10);
		set_type cont(right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_set_type right_cont(arr + 0, arr + 10);
		set_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		//yggr_test_assert(right_cont.size() == 0); // ub, only test check
	}

	{
		isomeric_same_set_type right_cont(arr + 0, arr + 10);
		set_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void test_operator_set(void)
{
	typedef Cont set_type;
	typedef typename set_type::value_type val_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::base_type base_set_type;
	typedef
		yggr::safe_container::safe_unordered_multiset
		<
			val_type,
			boost::mutex,
			hasher_type,
			key_equal_type,
			alloc_type,
			yggr::unordered_multiset
		> isomeric_same_set_type;
	typedef typename isomeric_same_set_type::base_type isomeric_same_base_set_type;

	BOOST_MPL_ASSERT((boost::is_same<base_set_type, isomeric_same_base_set_type>));

	int arr[10] = {1, 2, 3, 4, 5};

	{
		isomeric_same_base_set_type right_cont(arr + 0, arr + 10);
		set_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		//yggr_test_assert(right_cont.size() == 0); // ub, only test check
	}

	{
		isomeric_same_base_set_type right_cont(arr + 0, arr + 10);
		set_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_set_type right_cont(arr + 0, arr + 10);
		set_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		//yggr_test_assert(right_cont.size() == 0); // ub, only test check
	}

	{
		isomeric_same_set_type right_cont(arr + 0, arr + 10);
		set_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	std::cout << "---------------test_operator_set end-----------------" << std::endl;
}

template<typename Cont>
void test_operator_cmp(void)
{
	typedef Cont set_type;
	typedef typename set_type::value_type val_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::base_type base_set_type;
	typedef
		yggr::safe_container::safe_unordered_multiset
		<
			val_type,
			boost::mutex,
			hasher_type,
			key_equal_type,
			alloc_type,
			yggr::unordered_multiset
		> isomeric_same_set_type;
	typedef typename isomeric_same_set_type::base_type isomeric_same_base_set_type;

	BOOST_MPL_ASSERT((boost::is_same<base_set_type, isomeric_same_base_set_type>));

	int arr[10] = {1, 2, 3, 4, 6};
	int arr_not_equal[10] = {1, 2, 3, 4, 5};
	//int arr_greater[10] = {1, 2, 3, 4, 7};

	set_type cont(arr + 0, arr + 10);

	set_type cont_equal(cont);
	isomeric_same_set_type isomeric_same_cont_equal(cont);

	set_type cont_not_equal(arr_not_equal, arr_not_equal + 10);
	isomeric_same_set_type isomeric_same_cont_not_equal(cont_not_equal);

	// ==
	yggr_test_assert(cont == cont.load());
	yggr_test_assert(cont.load() == cont);
	yggr_test_assert(cont == cont);

	yggr_test_assert(cont == cont_equal.load());
	yggr_test_assert(cont.load() == cont_equal);
	yggr_test_assert(cont == cont_equal);

	yggr_test_assert(cont == isomeric_same_cont_equal.load());
	yggr_test_assert(cont.load() == isomeric_same_cont_equal);
	yggr_test_assert(cont == isomeric_same_cont_equal);

	// !=
	yggr_test_assert(!(cont != cont.load()));
	yggr_test_assert(!(cont.load() != cont));
	yggr_test_assert(!(cont != cont));

	yggr_test_assert(cont != cont_not_equal.load());
	yggr_test_assert(cont.load() != cont_not_equal);
	yggr_test_assert(cont != cont_not_equal);

	yggr_test_assert(cont != isomeric_same_cont_not_equal.load());
	yggr_test_assert(cont.load() != isomeric_same_cont_not_equal);
	yggr_test_assert(cont != isomeric_same_cont_not_equal);

	std::cout << "---------------test_operator_cmp end-----------------" << std::endl;
}


template<typename Cont>
void test_swap(void)
{
	typedef Cont set_type;
	typedef typename set_type::value_type val_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::base_type base_set_type;
	typedef
		yggr::safe_container::safe_unordered_multiset
		<
			val_type,
			boost::mutex,
			hasher_type,
			key_equal_type,
			alloc_type,
			yggr::unordered_multiset
		> isomeric_same_set_type;
	typedef typename isomeric_same_set_type::base_type isomeric_same_base_set_type;

	BOOST_MPL_ASSERT((boost::is_same<base_set_type, isomeric_same_base_set_type>));

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	set_type set_chk(arr1 + 0, arr1 + 10);
	base_set_type base_set_chk = set_chk.load();

	isomeric_same_set_type isomeric_same_set_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_set_type isomeric_same_base_set_chk = isomeric_same_set_chk.load();

	{
		set_type set = set_chk;
		isomeric_same_base_set_type right_set = isomeric_same_base_set_chk;

		set.swap(boost::move(right_set));

		yggr_test_assert(set == isomeric_same_set_chk);
		yggr_test_assert(right_set == base_set_chk);
	}

	{
		set_type set = set_chk;
		isomeric_same_base_set_type right_set = isomeric_same_base_set_chk;

		set.swap(right_set);

		yggr_test_assert(set == isomeric_same_set_chk);
		yggr_test_assert(right_set == base_set_chk);
	}

	{
		set_type set = set_chk;
		isomeric_same_set_type right_set = isomeric_same_set_chk;

		set.swap(boost::move(right_set));

		yggr_test_assert(set == isomeric_same_set_chk);
		yggr_test_assert(right_set == set_chk);
	}

	{
		set_type set = set_chk;
		isomeric_same_set_type right_set = isomeric_same_set_chk;

		set.swap(right_set);

		yggr_test_assert(set == isomeric_same_set_chk);
		yggr_test_assert(right_set == set_chk);
	}

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_stl_swap(void)
{
	typedef Cont set_type;
	typedef typename set_type::value_type val_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::base_type base_set_type;
	typedef
		yggr::safe_container::safe_unordered_multiset
		<
			val_type,
			boost::mutex,
			hasher_type,
			key_equal_type,
			alloc_type,
			yggr::unordered_multiset
		> isomeric_same_set_type;
	typedef typename isomeric_same_set_type::base_type isomeric_same_base_set_type;

	BOOST_MPL_ASSERT((boost::is_same<base_set_type, isomeric_same_base_set_type>));

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	set_type set_chk(arr1 + 0, arr1 + 10);
	base_set_type base_set_chk = set_chk.load();

	isomeric_same_set_type isomeric_same_set_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_set_type isomeric_same_base_set_chk = isomeric_same_set_chk.load();

	// set, base_set
	{
		set_type set1 = base_set_chk;
		isomeric_same_base_set_type set2 = isomeric_same_base_set_chk;

		std::swap(set1, boost::move(set2));

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_base_set_type set2 = isomeric_same_base_set_chk;

		std::swap(boost::move(set2), set1);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_base_set_type set2 = isomeric_same_base_set_chk;

		std::swap(boost::move(set1), set2);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	// set, set
	{
		set_type set1 = base_set_chk;
		isomeric_same_set_type set2 = isomeric_same_base_set_chk;

		std::swap(set1, boost::move(set2));

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_set_type set2 = isomeric_same_base_set_chk;

		std::swap(boost::move(set2), set1);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_set_type set2 = isomeric_same_base_set_chk;

		std::swap(set1, set2);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	std::cout << "---------------test_stl_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_boost_swap(void)
{
	typedef Cont set_type;
	typedef typename set_type::value_type val_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::base_type base_set_type;
	typedef
		yggr::safe_container::safe_unordered_multiset
		<
			val_type,
			boost::mutex,
			hasher_type,
			key_equal_type,
			alloc_type,
			yggr::unordered_multiset
		> isomeric_same_set_type;
	typedef typename isomeric_same_set_type::base_type isomeric_same_base_set_type;

	BOOST_MPL_ASSERT((boost::is_same<base_set_type, isomeric_same_base_set_type>));

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	set_type set_chk(arr1 + 0, arr1 + 10);
	base_set_type base_set_chk = set_chk.load();

	isomeric_same_set_type isomeric_same_set_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_set_type isomeric_same_base_set_chk = isomeric_same_set_chk.load();

	// set, base_set
	{
		set_type set1 = base_set_chk;
		isomeric_same_base_set_type set2 = isomeric_same_base_set_chk;

		boost::swap(set1, boost::move(set2));

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_base_set_type set2 = isomeric_same_base_set_chk;

		boost::swap(boost::move(set2), set1);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_base_set_type set2 = isomeric_same_base_set_chk;

		boost::swap(boost::move(set1), set2);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	// set, set
	{
		set_type set1 = base_set_chk;
		isomeric_same_set_type set2 = isomeric_same_base_set_chk;

		boost::swap(set1, boost::move(set2));

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_set_type set2 = isomeric_same_base_set_chk;

		boost::swap(boost::move(set2), set1);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_set_type set2 = isomeric_same_base_set_chk;

		boost::swap(set1, set2);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	std::cout << "---------------test_boost_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_yggr_swap(void)
{
	typedef Cont set_type;
	typedef typename set_type::value_type val_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::base_type base_set_type;
	typedef
		yggr::safe_container::safe_unordered_multiset
		<
			val_type,
			boost::mutex,
			hasher_type,
			key_equal_type,
			alloc_type,
			yggr::unordered_multiset
		> isomeric_same_set_type;
	typedef typename isomeric_same_set_type::base_type isomeric_same_base_set_type;

	BOOST_MPL_ASSERT((boost::is_same<base_set_type, isomeric_same_base_set_type>));

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	set_type set_chk(arr1 + 0, arr1 + 10);
	base_set_type base_set_chk = set_chk.load();

	isomeric_same_set_type isomeric_same_set_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_set_type isomeric_same_base_set_chk = isomeric_same_set_chk.load();

	// set, base_set
	{
		set_type set1 = base_set_chk;
		isomeric_same_base_set_type set2 = isomeric_same_base_set_chk;

		yggr::swap(set1, boost::move(set2));

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_base_set_type set2 = isomeric_same_base_set_chk;

		yggr::swap(boost::move(set2), set1);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_base_set_type set2 = isomeric_same_base_set_chk;

		yggr::swap(boost::move(set1), set2);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	// set, set
	{
		set_type set1 = base_set_chk;
		isomeric_same_set_type set2 = isomeric_same_base_set_chk;

		yggr::swap(set1, boost::move(set2));

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_set_type set2 = isomeric_same_base_set_chk;

		yggr::swap(boost::move(set2), set1);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	{
		set_type set1 = base_set_chk;
		isomeric_same_set_type set2 = isomeric_same_base_set_chk;

		yggr::swap(set1, set2);

		yggr_test_assert(set1 == isomeric_same_set_chk);
		yggr_test_assert(set2 == set_chk);
	}

	std::cout << "---------------test_yggr_swap end-----------------" << std::endl;
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

template<typename Cont>
void test_operator_merge(void)
{
	typedef Cont set_type;
	typedef typename set_type::value_type val_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::base_type base_set_type;
	typedef
		yggr::safe_container::safe_unordered_multiset
		<
			val_type,
			boost::mutex,
			hasher_type,
			key_equal_type,
			alloc_type,
			yggr::unordered_multiset
		> isomeric_same_set_type;
	typedef typename isomeric_same_set_type::base_type isomeric_same_base_set_type;

	BOOST_MPL_ASSERT((boost::is_same<base_set_type, isomeric_same_base_set_type>));

	typedef set_type default_set_type;
	typedef isomeric_same_set_type regular_set_type;

	typedef typename default_set_type::base_type default_base_set_type;
	typedef typename regular_set_type::base_type regular_base_set_type;

	test_merge_mm<default_set_type, default_base_set_type>();
	test_merge_mm<default_set_type, regular_base_set_type>();
	test_merge_mm<default_set_type, default_set_type>();
	test_merge_mm<default_set_type, regular_set_type>();

	test_merge_mm<regular_set_type, default_set_type>();
	test_merge_mm<regular_set_type, regular_set_type>();
	test_merge_mm<regular_set_type, default_base_set_type>();
	test_merge_mm<regular_set_type, regular_base_set_type>();

	std::cout << "---------------test_operator_merge end-----------------" << std::endl;
}

template<typename Cont>
void debug_test(void)
{
	typedef Cont set_type;

	test_constructor<set_type>();
	test_operator_set<set_type>();
	test_operator_cmp<set_type>();

	test_swap<set_type>();
	test_stl_swap<set_type>();
	test_boost_swap<set_type>();
	test_yggr_swap<set_type>();

	test_operator_merge<set_type>();
}

int main(int argc, char* argv[])
{
	{
		typedef yggr::safe_container::safe_unordered_multiset<int> set_type;
		debug_test<set_type>();
	}

	wait_any_key(argc, argv);
	return 0;
}
