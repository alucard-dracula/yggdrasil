//isomeric_same_safe_map_test_no_opt.cpp

#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <functional>

#include <boost/utility/enable_if.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/mplex/null_t.hpp>

#include <yggr/safe_container/safe_map.hpp>
#include <yggr/safe_container/safe_options_map.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)


//-------------------------test base-------------------------

template<typename Cont>
void test_constructor(void)
{
	typedef Cont map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::key_compare key_comp_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::base_type base_map_type;
	typedef
		yggr::safe_container::safe_map
		<
			key_type,
			mapped_type,
			boost::mutex,
			key_comp_type,
			alloc_type,
			yggr::map
		> isomeric_same_map_type;
	typedef typename isomeric_same_map_type::base_type isomeric_same_base_map_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#endif // (BOOST_VERSION < 107000)

	key_type arr[10] = {1, 2, 3, 4, 5};

	yggr::multimap<key_type, mapped_type> map_sample;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		map_sample.insert(std::make_pair(arr[i], arr[i]));
	}
	yggr_test_assert(map_sample.size() == 10);

	// isomeric_base
	{
		isomeric_same_base_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 6);
		//yggr_test_assert(right_cont.size() == 0); // ub, only test check
	}

	{
		isomeric_same_base_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont(right_cont);
		yggr_test_assert(cont.size() == 6);
		yggr_test_assert(right_cont.size() == 6);
	}

	{
		isomeric_same_base_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 6);
		//yggr_test_assert(right_cont.size() == 0); // ub, only test check
	}

	{
		isomeric_same_base_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 6);
		yggr_test_assert(right_cont.size() == 6);
	}

	// isomeric_this
	{
		isomeric_same_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 6);
		//yggr_test_assert(right_cont.size() == 0); // ub, only test check
	}

	{
		isomeric_same_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont(right_cont);
		yggr_test_assert(cont.size() == 6);
		yggr_test_assert(right_cont.size() == 6);
	}

	{
		isomeric_same_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 6);
		//yggr_test_assert(right_cont.size() == 0); // ub, only test check
	}

	{
		isomeric_same_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 6);
		yggr_test_assert(right_cont.size() == 6);
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void test_operator_map(void)
{
	typedef Cont map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::key_compare key_comp_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::base_type base_map_type;
	typedef
		yggr::safe_container::safe_map
		<
			key_type,
			mapped_type,
			boost::mutex,
			key_comp_type,
			alloc_type,
			yggr::map
		> isomeric_same_map_type;
	typedef typename isomeric_same_map_type::base_type isomeric_same_base_map_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	yggr::multimap<key_type, mapped_type> map_sample;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		map_sample.insert(std::make_pair(arr[i], arr[i]));
	}
	yggr_test_assert(map_sample.size() == 10);

	{
		isomeric_same_base_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 6);
		//yggr_test_assert(right_cont.size() == 0); // ub, only test check
	}

	{
		isomeric_same_base_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 6);
		yggr_test_assert(right_cont.size() == 6);
	}

	{
		isomeric_same_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 6);
		//yggr_test_assert(right_cont.size() == 0); // ub, only test check
	}

	{
		isomeric_same_map_type right_cont(map_sample.begin(), map_sample.end());
		map_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 6);
		yggr_test_assert(right_cont.size() == 6);
	}

	std::cout << "---------------test_operator_map end-----------------" << std::endl;
}

template<typename Cont>
void test_operator_cmp(void)
{
	typedef Cont map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::key_compare key_comp_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::base_type base_map_type;
	typedef
		yggr::safe_container::safe_map
		<
			key_type,
			mapped_type,
			boost::mutex,
			key_comp_type,
			alloc_type,
			yggr::map
		> isomeric_same_map_type;
	typedef typename isomeric_same_map_type::base_type isomeric_same_base_map_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 6};
	int arr_less[10] = {1, 2, 3, 4, 5};
	int arr_greater[10] = {1, 2, 3, 4, 7};

	yggr::multimap<key_type, mapped_type> map_sample;
	yggr::multimap<key_type, mapped_type> map_sample_less;
	yggr::multimap<key_type, mapped_type> map_sample_greater;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		map_sample.insert(std::make_pair(arr[i], arr[i]));
		map_sample_less.insert(std::make_pair(arr_less[i], arr_less[i]));
		map_sample_greater.insert(std::make_pair(arr_greater[i], arr_greater[i]));
	}
	yggr_test_assert(map_sample.size() == 10);

	map_type cont(map_sample.begin(), map_sample.end());

	map_type cont_equal(cont);
	isomeric_same_map_type isomeric_same_cont_equal(cont);

	map_type cont_less(map_sample_less.begin(), map_sample_less.end());
	isomeric_same_map_type isomeric_same_cont_less(cont_less);

	map_type cont_greater(map_sample_greater.begin(), map_sample_greater.end());
	isomeric_same_map_type isomeric_same_cont_greater(cont_greater);

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

	yggr_test_assert(cont != cont_less.load());
	yggr_test_assert(cont.load() != cont_less);
	yggr_test_assert(cont != cont_less);

	yggr_test_assert(cont != isomeric_same_cont_less.load());
	yggr_test_assert(cont.load() != isomeric_same_cont_less);
	yggr_test_assert(cont != isomeric_same_cont_less);

	// <
	yggr_test_assert(!(cont < cont.load()));
	yggr_test_assert(!(cont.load() < cont));
	yggr_test_assert(!(cont < cont));

	yggr_test_assert(cont < cont_greater.load());
	yggr_test_assert(cont.load() < cont_greater);
	yggr_test_assert(cont < cont_greater);

	yggr_test_assert(cont < isomeric_same_cont_greater.load());
	yggr_test_assert(cont.load() < isomeric_same_cont_greater);
	yggr_test_assert(cont < isomeric_same_cont_greater);

	// <=
	yggr_test_assert(cont <= cont.load());
	yggr_test_assert(cont.load() <= cont);
	yggr_test_assert(cont <= cont);

	yggr_test_assert(cont <= cont_equal.load());
	yggr_test_assert(cont.load() <= cont_equal);
	yggr_test_assert(cont <= cont_equal);

	yggr_test_assert(cont <= isomeric_same_cont_equal.load());
	yggr_test_assert(cont.load() <= isomeric_same_cont_equal);
	yggr_test_assert(cont <= isomeric_same_cont_equal);

	yggr_test_assert(cont <= cont_greater.load());
	yggr_test_assert(cont.load() <= cont_greater);
	yggr_test_assert(cont <= cont_greater);

	yggr_test_assert(cont <= isomeric_same_cont_greater.load());
	yggr_test_assert(cont.load() <= isomeric_same_cont_greater);
	yggr_test_assert(cont <= isomeric_same_cont_greater);

	// >
	yggr_test_assert(!(cont > cont.load()));
	yggr_test_assert(!(cont.load() > cont));
	yggr_test_assert(!(cont > cont));

	yggr_test_assert(cont > cont_less.load());
	yggr_test_assert(cont.load() > cont_less);
	yggr_test_assert(cont > cont_less);

	yggr_test_assert(cont > isomeric_same_cont_less.load());
	yggr_test_assert(cont.load() > isomeric_same_cont_less);
	yggr_test_assert(cont > isomeric_same_cont_less);

	// >=
	yggr_test_assert(cont >= cont.load());
	yggr_test_assert(cont.load() >= cont);
	yggr_test_assert(cont >= cont);

	yggr_test_assert(cont >= cont_equal.load());
	yggr_test_assert(cont.load() >= cont_equal);
	yggr_test_assert(cont >= cont_equal);

	yggr_test_assert(cont >= isomeric_same_cont_equal.load());
	yggr_test_assert(cont.load() >= isomeric_same_cont_equal);
	yggr_test_assert(cont >= isomeric_same_cont_equal);

	yggr_test_assert(cont >= cont_less.load());
	yggr_test_assert(cont.load() >= cont_less);
	yggr_test_assert(cont >= cont_less);

	yggr_test_assert(cont >= isomeric_same_cont_less.load());
	yggr_test_assert(cont.load() >= isomeric_same_cont_less);
	yggr_test_assert(cont >= isomeric_same_cont_less);

	std::cout << "---------------test_operator_cmp end-----------------" << std::endl;
}

template<typename Cont>
void test_safe_other(void)
{
	typedef Cont map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::key_compare key_comp_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::base_type base_map_type;
	typedef
		yggr::safe_container::safe_map
		<
			key_type,
			mapped_type,
			boost::mutex,
			key_comp_type,
			alloc_type,
			yggr::map
		> isomeric_same_map_type;
	typedef typename isomeric_same_map_type::base_type isomeric_same_base_map_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	yggr::multimap<key_type, mapped_type> map_sample;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		map_sample.insert(std::make_pair(arr[i], arr[i]));
	}

	map_type cont(map_sample.begin(), map_sample.end());

	// operator base
	{
		isomeric_same_base_map_type isomeric_same_base_cont;
		isomeric_same_base_cont = cont;
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	// copy_to_base
	{
		isomeric_same_base_map_type isomeric_same_base_cont;
		cont.copy_to_base(isomeric_same_base_cont);
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	// load
	{
		isomeric_same_base_map_type isomeric_same_base_cont;
		isomeric_same_base_cont = cont.template load<isomeric_same_base_map_type>();
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	// store
	{
		isomeric_same_base_map_type isomeric_same_base_cont = cont;
		map_type other_cont;
		other_cont.store(boost::move(isomeric_same_base_cont));
		yggr_test_assert(other_cont == cont);
	}

	{
		isomeric_same_base_map_type isomeric_same_base_cont = cont;
		map_type other_cont;
		other_cont.store(isomeric_same_base_cont);
		yggr_test_assert(other_cont == cont);
	}


	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont>
void test_swap(void)
{
	typedef Cont map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::key_compare key_comp_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::base_type base_map_type;
	typedef
		yggr::safe_container::safe_map
		<
			key_type,
			mapped_type,
			boost::mutex,
			key_comp_type,
			alloc_type,
			yggr::map
		> isomeric_same_map_type;
	typedef typename isomeric_same_map_type::base_type isomeric_same_base_map_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	yggr::multimap<key_type, mapped_type> map_sample1;
	yggr::multimap<key_type, mapped_type> map_sample2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		map_sample1.insert(std::make_pair(arr1[i], arr1[i]));
		map_sample2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	map_type map_chk(map_sample1.begin(), map_sample1.end());
	base_map_type base_map_chk = map_chk.load();

	isomeric_same_map_type isomeric_same_map_chk(map_sample2.begin(), map_sample2.end());
	isomeric_same_base_map_type isomeric_same_base_map_chk = isomeric_same_map_chk.load();

	{
		map_type map = map_chk;
		isomeric_same_base_map_type right_map = isomeric_same_base_map_chk;

		map.swap(boost::move(right_map));

		yggr_test_assert(map == isomeric_same_map_chk);
		yggr_test_assert(right_map == base_map_chk);
	}

	{
		map_type map = map_chk;
		isomeric_same_base_map_type right_map = isomeric_same_base_map_chk;

		map.swap(right_map);

		yggr_test_assert(map == isomeric_same_map_chk);
		yggr_test_assert(right_map == base_map_chk);
	}

	{
		map_type map = map_chk;
		isomeric_same_map_type right_map = isomeric_same_map_chk;

		map.swap(boost::move(right_map));

		yggr_test_assert(map == isomeric_same_map_chk);
		yggr_test_assert(right_map == map_chk);
	}

	{
		map_type map = map_chk;
		isomeric_same_map_type right_map = isomeric_same_map_chk;

		map.swap(right_map);

		yggr_test_assert(map == isomeric_same_map_chk);
		yggr_test_assert(right_map == map_chk);
	}

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_stl_swap(void)
{
	typedef Cont map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::key_compare key_comp_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::base_type base_map_type;
	typedef
		yggr::safe_container::safe_map
		<
			key_type,
			mapped_type,
			boost::mutex,
			key_comp_type,
			alloc_type,
			yggr::map
		> isomeric_same_map_type;
	typedef typename isomeric_same_map_type::base_type isomeric_same_base_map_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	yggr::multimap<key_type, mapped_type> map_sample1;
	yggr::multimap<key_type, mapped_type> map_sample2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		map_sample1.insert(std::make_pair(arr1[i], arr1[i]));
		map_sample2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	map_type map_chk(map_sample1.begin(), map_sample1.end());
	base_map_type base_map_chk = map_chk.load();

	isomeric_same_map_type isomeric_same_map_chk(map_sample2.begin(), map_sample2.end());
	isomeric_same_base_map_type isomeric_same_base_map_chk = isomeric_same_map_chk.load();

	// map, base_map
	{
		map_type map1 = base_map_chk;
		isomeric_same_base_map_type map2 = isomeric_same_base_map_chk;

		std::swap(map1, boost::move(map2));

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_base_map_type map2 = isomeric_same_base_map_chk;

		std::swap(boost::move(map2), map1);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_base_map_type map2 = isomeric_same_base_map_chk;

		std::swap(boost::move(map1), map2);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	// map, map
	{
		map_type map1 = base_map_chk;
		isomeric_same_map_type map2 = isomeric_same_base_map_chk;

		std::swap(map1, boost::move(map2));

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_map_type map2 = isomeric_same_base_map_chk;

		std::swap(boost::move(map2), map1);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_map_type map2 = isomeric_same_base_map_chk;

		std::swap(map1, map2);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	std::cout << "---------------test_stl_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_boost_swap(void)
{
	typedef Cont map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::key_compare key_comp_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::base_type base_map_type;
	typedef
		yggr::safe_container::safe_map
		<
			key_type,
			mapped_type,
			boost::mutex,
			key_comp_type,
			alloc_type,
			yggr::map
		> isomeric_same_map_type;
	typedef typename isomeric_same_map_type::base_type isomeric_same_base_map_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	yggr::multimap<key_type, mapped_type> map_sample1;
	yggr::multimap<key_type, mapped_type> map_sample2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		map_sample1.insert(std::make_pair(arr1[i], arr1[i]));
		map_sample2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	map_type map_chk(map_sample1.begin(), map_sample1.end());
	base_map_type base_map_chk = map_chk.load();

	isomeric_same_map_type isomeric_same_map_chk(map_sample2.begin(), map_sample2.end());
	isomeric_same_base_map_type isomeric_same_base_map_chk = isomeric_same_map_chk.load();

	// map, base_map
	{
		map_type map1 = base_map_chk;
		isomeric_same_base_map_type map2 = isomeric_same_base_map_chk;

		boost::swap(map1, boost::move(map2));

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_base_map_type map2 = isomeric_same_base_map_chk;

		boost::swap(boost::move(map2), map1);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_base_map_type map2 = isomeric_same_base_map_chk;

		boost::swap(boost::move(map1), map2);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	// map, map
	{
		map_type map1 = base_map_chk;
		isomeric_same_map_type map2 = isomeric_same_base_map_chk;

		boost::swap(map1, boost::move(map2));

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_map_type map2 = isomeric_same_base_map_chk;

		boost::swap(boost::move(map2), map1);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_map_type map2 = isomeric_same_base_map_chk;

		boost::swap(map1, map2);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	std::cout << "---------------test_boost_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_yggr_swap(void)
{
	typedef Cont map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::key_compare key_comp_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::base_type base_map_type;
	typedef
		yggr::safe_container::safe_map
		<
			key_type,
			mapped_type,
			boost::mutex,
			key_comp_type,
			alloc_type,
			yggr::map
		> isomeric_same_map_type;
	typedef typename isomeric_same_map_type::base_type isomeric_same_base_map_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	yggr::multimap<key_type, mapped_type> map_sample1;
	yggr::multimap<key_type, mapped_type> map_sample2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		map_sample1.insert(std::make_pair(arr1[i], arr1[i]));
		map_sample2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	map_type map_chk(map_sample1.begin(), map_sample1.end());
	base_map_type base_map_chk = map_chk.load();

	isomeric_same_map_type isomeric_same_map_chk(map_sample2.begin(), map_sample2.end());
	isomeric_same_base_map_type isomeric_same_base_map_chk = isomeric_same_map_chk.load();

	// map, base_map
	{
		map_type map1 = base_map_chk;
		isomeric_same_base_map_type map2 = isomeric_same_base_map_chk;

		yggr::swap(map1, boost::move(map2));

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_base_map_type map2 = isomeric_same_base_map_chk;

		yggr::swap(boost::move(map2), map1);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_base_map_type map2 = isomeric_same_base_map_chk;

		yggr::swap(boost::move(map1), map2);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	// map, map
	{
		map_type map1 = base_map_chk;
		isomeric_same_map_type map2 = isomeric_same_base_map_chk;

		yggr::swap(map1, boost::move(map2));

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_map_type map2 = isomeric_same_base_map_chk;

		yggr::swap(boost::move(map2), map1);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	{
		map_type map1 = base_map_chk;
		isomeric_same_map_type map2 = isomeric_same_base_map_chk;

		yggr::swap(map1, map2);

		yggr_test_assert(map1 == isomeric_same_map_chk);
		yggr_test_assert(map2 == map_chk);
	}

	std::cout << "---------------test_yggr_swap end-----------------" << std::endl;
}

template<typename LCont, typename RCont>
void test_merge_ss(void)
{
	typedef LCont l_cont_type;
	typedef RCont r_cont_type;

	typedef typename l_cont_type::key_type key_type;
	typedef typename l_cont_type::mapped_type mapped_type;

	mapped_type arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
	mapped_type arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};

	yggr::multimap<key_type, mapped_type> map_sample1;
	yggr::multimap<key_type, mapped_type> map_sample2;

	for(int i = 0, isize = 10; i != isize; ++i)
	{
		map_sample1.insert(std::make_pair(arr1[i], arr1[i]));
		map_sample2.insert(std::make_pair(arr2[i], arr2[i]));
	}

	{
		l_cont_type cont_eins(map_sample1.begin(), map_sample1.end());
		r_cont_type cont_zwei(map_sample2.begin(), map_sample2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		cont_eins.merge(boost::move(cont_zwei));
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}

	{
		l_cont_type cont_eins(map_sample1.begin(), map_sample1.end());
		r_cont_type cont_zwei(map_sample2.begin(), map_sample2.end());

		yggr_test_assert(cont_eins.size() == 5);
		yggr_test_assert(cont_zwei.size() == 5);
		cont_eins.merge(cont_zwei);
		yggr_test_assert(cont_eins.size() == 9);
		yggr_test_assert(cont_zwei.size() == 1);
	}
}

template<typename Cont>
void test_operator_merge(void)
{
	typedef Cont map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::mapped_type mapped_type;
	typedef typename map_type::key_compare key_comp_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::base_type base_map_type;

	typedef
		yggr::safe_container::safe_map
		<
			key_type,
			mapped_type,
			boost::mutex,
			key_comp_type,
			alloc_type,
			yggr::map
		> isomeric_same_map_type;
	typedef typename isomeric_same_map_type::base_type isomeric_same_base_map_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_map_type, isomeric_same_base_map_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_map<base_map_type, isomeric_same_base_map_type>));
#endif // (BOOST_VERSION < 107000)

	typedef map_type default_map_type;
	typedef isomeric_same_map_type regular_map_type;

	typedef typename default_map_type::base_type default_base_map_type;
	typedef typename regular_map_type::base_type regular_base_map_type;

	test_merge_ss<default_map_type, default_base_map_type>();
	test_merge_ss<default_map_type, regular_base_map_type>();
	test_merge_ss<default_map_type, default_map_type>();
	test_merge_ss<default_map_type, regular_map_type>();

	test_merge_ss<regular_map_type, default_map_type>();
	test_merge_ss<regular_map_type, regular_map_type>();
	test_merge_ss<regular_map_type, default_base_map_type>();
	test_merge_ss<regular_map_type, regular_base_map_type>();

	std::cout << "---------------test_operator_merge end-----------------" << std::endl;
}

template<typename Cont>
void debug_test(void)
{
	typedef Cont map_type;

	test_constructor<map_type>();
	test_operator_map<map_type>();
	test_operator_cmp<map_type>();
	test_safe_other<map_type>();

	test_swap<map_type>();
	test_stl_swap<map_type>();
	test_boost_swap<map_type>();
	test_yggr_swap<map_type>();

	test_operator_merge<map_type>();
}

int main(int argc, char* argv[])
{
	{
		typedef yggr::safe_container::safe_map<int, int> map_type;
		debug_test<map_type>();
	}

	wait_any_key(argc, argv);
	return 0;
}
