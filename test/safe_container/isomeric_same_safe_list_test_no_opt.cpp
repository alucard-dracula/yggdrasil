// isomeric_same_safe_list_test.cpp

#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/safe_container/safe_list.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)

//-------------------------test base-------------------------

template<typename Cont>
void test_constructor(void)
{
	typedef Cont list_type;
	typedef typename list_type::value_type val_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::base_type base_list_type;
	typedef
		yggr::safe_container::safe_list
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::list
		> isomeric_same_list_type;
	typedef typename isomeric_same_list_type::base_type isomeric_same_base_list_type;

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	typedef typename base_list_type::base_type base_base_list_type;
	typedef typename isomeric_same_base_list_type::base_type isomeric_same_base_base_list_type;
#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	// cont(base_cont)
	{
		base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	// cont(base_cont, alloc)
	{
		base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	{
		isomeric_same_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_list_type right_cont(arr + 0, arr + 10);
		list_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_list_type right_cont(arr + 0, arr + 10);
		list_type cont(right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_list_type right_cont(arr + 0, arr + 10);
		list_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_list_type right_cont(arr + 0, arr + 10);
		list_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void test_operator_set(void)
{
	typedef Cont list_type;
	typedef typename list_type::value_type val_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::base_type base_list_type;
	typedef
		yggr::safe_container::safe_list
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::list
		> isomeric_same_list_type;
	typedef typename isomeric_same_list_type::base_type isomeric_same_base_list_type;

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	typedef typename base_list_type::base_type base_base_list_type;
	typedef typename isomeric_same_base_list_type::base_type isomeric_same_base_base_list_type;
#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	{
		base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	{
		isomeric_same_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_list_type right_cont(arr + 0, arr + 10);
		list_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_list_type right_cont(arr + 0, arr + 10);
		list_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_list_type right_cont(arr + 0, arr + 10);
		list_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	std::cout << "---------------test_operator_set end-----------------" << std::endl;
}

template<typename Cont>
void test_operator_cmp(void)
{
	typedef Cont list_type;
	typedef typename list_type::value_type val_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::base_type base_list_type;
	typedef
		yggr::safe_container::safe_list
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::list
		> isomeric_same_list_type;
	typedef typename isomeric_same_list_type::base_type isomeric_same_base_list_type;

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	typedef typename base_list_type::base_type base_base_list_type;
	typedef typename isomeric_same_base_list_type::base_type isomeric_same_base_base_list_type;
#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	list_type cont(arr + 0, arr + 10);

	list_type cont_equal(cont);
	isomeric_same_list_type isomeric_same_cont_equal(cont);


	list_type cont_less(cont);
	cont_less.back() = cont_less.back() - 1;
	isomeric_same_list_type isomeric_same_cont_less(cont_less);


	list_type cont_greater(cont);
	cont_greater.back() = cont_greater.back() + 1;
	isomeric_same_list_type isomeric_same_cont_greater(cont_greater);

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	// ==
	yggr_test_assert(cont == cont.load<base_base_list_type>());
	yggr_test_assert(cont == cont.load<isomeric_same_base_base_list_type>());
	yggr_test_assert(cont.load<base_base_list_type>() == cont);
	yggr_test_assert(cont.load<isomeric_same_base_base_list_type>() == cont);

	yggr_test_assert(!(cont == cont_less.load<base_base_list_type>()));
	yggr_test_assert(!(cont == cont_less.load<isomeric_same_base_base_list_type>()));
	yggr_test_assert(!(cont_less.load<base_base_list_type>() == cont));
	yggr_test_assert(!(cont_less.load<isomeric_same_base_base_list_type>() == cont));

	// !=
	yggr_test_assert(!(cont != cont.load<base_base_list_type>()));
	yggr_test_assert(!(cont != cont.load<isomeric_same_base_base_list_type>()));
	yggr_test_assert(!(cont.load<base_base_list_type>() != cont));
	yggr_test_assert(!(cont.load<isomeric_same_base_base_list_type>() != cont));

	yggr_test_assert(cont != cont_less.load<base_base_list_type>());
	yggr_test_assert(cont != cont_less.load<isomeric_same_base_base_list_type>());
	yggr_test_assert(cont.load<base_base_list_type>() != cont_less);
	yggr_test_assert(cont.load<isomeric_same_base_base_list_type>() != cont_less);

	// <
	yggr_test_assert(!(cont < cont.load<base_base_list_type>()));
	yggr_test_assert(!(cont < cont.load<isomeric_same_base_base_list_type>()));
	yggr_test_assert(!(cont.load<base_base_list_type>() < cont));
	yggr_test_assert(!(cont.load<isomeric_same_base_base_list_type>() < cont));

	yggr_test_assert(cont < cont_greater.load<base_base_list_type>());
	yggr_test_assert(cont < cont_greater.load<isomeric_same_base_base_list_type>());
	yggr_test_assert(cont.load<base_base_list_type>() < cont_greater);
	yggr_test_assert(cont.load<isomeric_same_base_base_list_type>() < cont_greater);

	// <=
	yggr_test_assert(cont <= cont.load<base_base_list_type>());
	yggr_test_assert(cont <= cont.load<isomeric_same_base_base_list_type>());
	yggr_test_assert(cont.load<base_base_list_type>() <= cont);
	yggr_test_assert(cont.load<isomeric_same_base_base_list_type>() <= cont);

	yggr_test_assert(cont <= cont_greater.load<base_base_list_type>());
	yggr_test_assert(cont <= cont_greater.load<isomeric_same_base_base_list_type>());
	yggr_test_assert(cont.load<base_base_list_type>() <= cont_greater);
	yggr_test_assert(cont.load<isomeric_same_base_base_list_type>() <= cont_greater);

	// >
	yggr_test_assert(!(cont > cont.load<base_base_list_type>()));
	yggr_test_assert(!(cont > cont.load<isomeric_same_base_base_list_type>()));
	yggr_test_assert(!(cont.load<base_base_list_type>() > cont));
	yggr_test_assert(!(cont.load<isomeric_same_base_base_list_type>() > cont));

	yggr_test_assert(cont > cont_less.load<base_base_list_type>());
	yggr_test_assert(cont > cont_less.load<isomeric_same_base_base_list_type>());
	yggr_test_assert(cont.load<base_base_list_type>() > cont_less);
	yggr_test_assert(cont.load<isomeric_same_base_base_list_type>() > cont_less);

	// >=
	yggr_test_assert(cont >= cont.load<base_base_list_type>());
	yggr_test_assert(cont >= cont.load<isomeric_same_base_base_list_type>());
	yggr_test_assert(cont.load<base_base_list_type>() >= cont);
	yggr_test_assert(cont.load<isomeric_same_base_base_list_type>() >= cont);

	yggr_test_assert(cont >= cont_less.load<base_base_list_type>());
	yggr_test_assert(cont >= cont_less.load<isomeric_same_base_base_list_type>());
	yggr_test_assert(cont.load<base_base_list_type>() >= cont_less);
	yggr_test_assert(cont.load<isomeric_same_base_base_list_type>() >= cont_less);

#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

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
	typedef Cont list_type;
	typedef typename list_type::value_type val_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::base_type base_list_type;
	typedef
		yggr::safe_container::safe_list
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::list
		> isomeric_same_list_type;
	typedef typename isomeric_same_list_type::base_type isomeric_same_base_list_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	list_type cont(arr + 0, arr + 10);

	// operator base
	{
		isomeric_same_base_list_type isomeric_same_base_cont;
		isomeric_same_base_cont = cont;
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	// copy_to_base
	{
		isomeric_same_base_list_type isomeric_same_base_cont;
		cont.copy_to_base(isomeric_same_base_cont);
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	// load
	{
		isomeric_same_base_list_type isomeric_same_base_cont;
		isomeric_same_base_cont = cont.template load<isomeric_same_base_list_type>();
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	// store
	{
		isomeric_same_base_list_type isomeric_same_base_cont = cont;
		list_type other_cont;
		other_cont.store(boost::move(isomeric_same_base_cont));
		yggr_test_assert(other_cont == cont);
	}

	{
		isomeric_same_base_list_type isomeric_same_base_cont = cont;
		list_type other_cont;
		other_cont.store(isomeric_same_base_cont);
		yggr_test_assert(other_cont == cont);
	}


	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont>
void test_swap(void)
{
	typedef Cont list_type;
	typedef typename list_type::value_type val_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::base_type base_list_type;
	typedef
		yggr::safe_container::safe_list
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::list
		> isomeric_same_list_type;
	typedef typename isomeric_same_list_type::base_type isomeric_same_base_list_type;

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	typedef typename base_list_type::base_type base_base_list_type;
	typedef typename isomeric_same_base_list_type::base_type isomeric_same_base_base_list_type;
#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	list_type lst_chk(arr1 + 0, arr1 + 10);
	base_list_type base_lst_chk = lst_chk.load();

	isomeric_same_list_type isomeric_same_lst_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_list_type isomeric_same_base_lst_chk = isomeric_same_lst_chk.load();

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	{
		list_type lst = lst_chk;
		base_base_list_type right_lst = isomeric_same_lst_chk.load<base_base_list_type>();

		lst.swap(boost::move(right_lst));

		yggr_test_assert(lst == isomeric_same_lst_chk);
		yggr_test_assert(right_lst == base_lst_chk);

		lst.swap(right_lst);

		yggr_test_assert(lst == base_lst_chk);
		yggr_test_assert(right_lst == isomeric_same_lst_chk);
	}

	{
		list_type lst = lst_chk;
		isomeric_same_base_base_list_type right_lst = isomeric_same_lst_chk.load<isomeric_same_base_base_list_type>();

		lst.swap(boost::move(right_lst));

		yggr_test_assert(lst == isomeric_same_lst_chk);
		yggr_test_assert(right_lst == base_lst_chk);

		lst.swap(right_lst);

		yggr_test_assert(lst == base_lst_chk);
		yggr_test_assert(right_lst == isomeric_same_lst_chk);
	}

#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	{
		list_type lst = lst_chk;
		isomeric_same_base_list_type right_lst = isomeric_same_base_lst_chk;

		lst.swap(boost::move(right_lst));

		yggr_test_assert(lst == isomeric_same_lst_chk);
		yggr_test_assert(right_lst == base_lst_chk);
	}

	{
		list_type lst = lst_chk;
		isomeric_same_base_list_type right_lst = isomeric_same_base_lst_chk;

		lst.swap(right_lst);

		yggr_test_assert(lst == isomeric_same_lst_chk);
		yggr_test_assert(right_lst == base_lst_chk);
	}

	{
		list_type lst = lst_chk;
		isomeric_same_list_type right_lst = isomeric_same_lst_chk;

		lst.swap(boost::move(right_lst));

		yggr_test_assert(lst == isomeric_same_lst_chk);
		yggr_test_assert(right_lst == lst_chk);
	}

	{
		list_type lst = lst_chk;
		isomeric_same_list_type right_lst = isomeric_same_lst_chk;

		lst.swap(right_lst);

		yggr_test_assert(lst == isomeric_same_lst_chk);
		yggr_test_assert(right_lst == lst_chk);
	}

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_stl_swap(void)
{
	typedef Cont list_type;
	typedef typename list_type::value_type val_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::base_type base_list_type;
	typedef
		yggr::safe_container::safe_list
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::list
		> isomeric_same_list_type;
	typedef typename isomeric_same_list_type::base_type isomeric_same_base_list_type;

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	typedef typename base_list_type::base_type base_base_list_type;
	typedef typename isomeric_same_base_list_type::base_type isomeric_same_base_base_list_type;
#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	list_type lst_chk(arr1 + 0, arr1 + 10);
	base_list_type base_lst_chk = lst_chk.load();

	isomeric_same_list_type isomeric_same_lst_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_list_type isomeric_same_base_lst_chk = isomeric_same_lst_chk.load();

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	{
		list_type lst1 = base_lst_chk;
		base_base_list_type lst2 = isomeric_same_lst_chk.load<base_base_list_type>();

		std::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		std::swap(lst1, lst2);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);

		std::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		std::swap(lst2, lst1);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_base_list_type lst2 = isomeric_same_lst_chk.load<isomeric_same_base_base_list_type>();

		std::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		std::swap(lst1, lst2);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);

		std::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		std::swap(lst2, lst1);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);
	}

#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	// lst, base_lst
	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_list_type lst2 = isomeric_same_base_lst_chk;

		std::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_list_type lst2 = isomeric_same_base_lst_chk;

		std::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_list_type lst2 = isomeric_same_base_lst_chk;

		std::swap(boost::move(lst1), lst2);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	// lst, lst
	{
		list_type lst1 = base_lst_chk;
		isomeric_same_list_type lst2 = isomeric_same_base_lst_chk;

		std::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_list_type lst2 = isomeric_same_base_lst_chk;

		std::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_list_type lst2 = isomeric_same_base_lst_chk;

		std::swap(lst1, lst2);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	std::cout << "---------------test_stl_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_boost_swap(void)
{
	typedef Cont list_type;
	typedef typename list_type::value_type val_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::base_type base_list_type;
	typedef
		yggr::safe_container::safe_list
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::list
		> isomeric_same_list_type;
	typedef typename isomeric_same_list_type::base_type isomeric_same_base_list_type;

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	typedef typename base_list_type::base_type base_base_list_type;
	typedef typename isomeric_same_base_list_type::base_type isomeric_same_base_base_list_type;
#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	list_type lst_chk(arr1 + 0, arr1 + 10);
	base_list_type base_lst_chk = lst_chk.load();

	isomeric_same_list_type isomeric_same_lst_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_list_type isomeric_same_base_lst_chk = isomeric_same_lst_chk.load();

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	{
		list_type lst1 = base_lst_chk;
		base_base_list_type lst2 = isomeric_same_lst_chk.load<base_base_list_type>();

		boost::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		boost::swap(lst1, lst2);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);

		boost::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		boost::swap(lst2, lst1);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_base_list_type lst2 = isomeric_same_lst_chk.load<isomeric_same_base_base_list_type>();

		boost::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		boost::swap(lst1, lst2);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);

		boost::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		boost::swap(lst2, lst1);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);
	}

#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	// lst, base_lst
	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_list_type lst2 = isomeric_same_base_lst_chk;

		boost::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_list_type lst2 = isomeric_same_base_lst_chk;

		boost::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_list_type lst2 = isomeric_same_base_lst_chk;

		boost::swap(boost::move(lst1), lst2);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	// lst, lst
	{
		list_type lst1 = base_lst_chk;
		isomeric_same_list_type lst2 = isomeric_same_base_lst_chk;

		boost::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_list_type lst2 = isomeric_same_base_lst_chk;

		boost::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_list_type lst2 = isomeric_same_base_lst_chk;

		boost::swap(lst1, lst2);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	std::cout << "---------------test_boost_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_yggr_swap(void)
{
	typedef Cont list_type;
	typedef typename list_type::value_type val_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::base_type base_list_type;
	typedef
		yggr::safe_container::safe_list
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::list
		> isomeric_same_list_type;
	typedef typename isomeric_same_list_type::base_type isomeric_same_base_list_type;

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	typedef typename base_list_type::base_type base_base_list_type;
	typedef typename isomeric_same_base_list_type::base_type isomeric_same_base_base_list_type;
#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	list_type lst_chk(arr1 + 0, arr1 + 10);
	base_list_type base_lst_chk = lst_chk.load();

	isomeric_same_list_type isomeric_same_lst_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_list_type isomeric_same_base_lst_chk = isomeric_same_lst_chk.load();

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	{
		list_type lst1 = base_lst_chk;
		base_base_list_type lst2 = isomeric_same_lst_chk.load<base_base_list_type>();

		yggr::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		yggr::swap(lst1, lst2);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);

		yggr::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		yggr::swap(lst2, lst1);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_base_list_type lst2 = isomeric_same_lst_chk.load<isomeric_same_base_base_list_type>();

		yggr::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		yggr::swap(lst1, lst2);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);

		yggr::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);

		yggr::swap(lst2, lst1);

		yggr_test_assert(lst1 == lst_chk);
		yggr_test_assert(lst2 == isomeric_same_lst_chk);
	}

#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	// lst, base_lst
	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_list_type lst2 = isomeric_same_base_lst_chk;

		yggr::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_list_type lst2 = isomeric_same_base_lst_chk;

		yggr::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_base_list_type lst2 = isomeric_same_base_lst_chk;

		yggr::swap(boost::move(lst1), lst2);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	// lst, lst
	{
		list_type lst1 = base_lst_chk;
		isomeric_same_list_type lst2 = isomeric_same_base_lst_chk;

		yggr::swap(lst1, boost::move(lst2));

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_list_type lst2 = isomeric_same_base_lst_chk;

		yggr::swap(boost::move(lst2), lst1);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	{
		list_type lst1 = base_lst_chk;
		isomeric_same_list_type lst2 = isomeric_same_base_lst_chk;

		yggr::swap(lst1, lst2);

		yggr_test_assert(lst1 == isomeric_same_lst_chk);
		yggr_test_assert(lst2 == lst_chk);
	}

	std::cout << "---------------test_yggr_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_operators(void)
{
	typedef Cont list_type;
	typedef typename list_type::value_type val_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::base_type base_list_type;
	typedef
		yggr::safe_container::safe_list
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::list
		> isomeric_same_list_type;
	typedef typename isomeric_same_list_type::base_type isomeric_same_base_list_type;

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	typedef typename base_list_type::base_type base_base_list_type;
	typedef typename isomeric_same_base_list_type::base_type isomeric_same_base_base_list_type;
#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_list_type, isomeric_same_base_list_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_list<base_list_type, isomeric_same_base_list_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	{
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2);

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2);

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2), cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2), cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2, cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2, cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2), cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2), cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2, cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2, cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			boost::move(cont2));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			boost::move(cont2));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			cont2);

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			cont2);

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			boost::move(cont2), cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			boost::move(cont2), cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			cont2, cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			cont2, cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			boost::move(cont2), cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			boost::move(cont2), cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			cont2, cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			cont2, cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	// splice base
	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2);

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2), cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2, cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2), cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2, cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			boost::move(cont2));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			cont2);

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			boost::move(cont2), cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			cont2, cont2.begin());

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			boost::move(cont2), cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
			cont2, cont2.begin(), cont2.end());

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	// splice
	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2);

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		typedef typename isomeric_same_base_list_type::iterator (isomeric_same_base_list_type::* isomeric_same_begin_end_foo_type)(void);

		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::move(cont2),
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1));

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		typedef typename isomeric_same_base_list_type::iterator (isomeric_same_base_list_type::* isomeric_same_begin_end_foo_type)(void);

		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			cont2,
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1));

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		typedef typename isomeric_same_base_list_type::iterator (isomeric_same_base_list_type::* isomeric_same_begin_end_foo_type)(void);

		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::move(cont2),
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1),
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::end), _1));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		typedef typename isomeric_same_base_list_type::iterator (isomeric_same_base_list_type::* isomeric_same_begin_end_foo_type)(void);

		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			cont2,
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1),
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::end), _1));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		typedef typename isomeric_same_base_list_type::iterator (isomeric_same_base_list_type::* isomeric_same_begin_end_foo_type)(void);

		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1),
						boost::move(cont2));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		typedef typename isomeric_same_base_list_type::iterator (isomeric_same_base_list_type::* isomeric_same_begin_end_foo_type)(void);

		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1),
						cont2);

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		typedef typename isomeric_same_base_list_type::iterator (isomeric_same_base_list_type::* isomeric_same_begin_end_foo_type)(void);

		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1),
			boost::move(cont2),
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1));

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		typedef typename isomeric_same_base_list_type::iterator (isomeric_same_base_list_type::* isomeric_same_begin_end_foo_type)(void);

		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1),
			cont2,
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1));

		yggr_test_assert(cont1.size() == 11);
		yggr_test_assert(cont2.size() == 4);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		typedef typename isomeric_same_base_list_type::iterator (isomeric_same_base_list_type::* isomeric_same_begin_end_foo_type)(void);

		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1),
			boost::move(cont2),
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1),
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::end), _1));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		typedef typename base_list_type::iterator (base_list_type::* begin_end_foo_type)(void);
		typedef typename isomeric_same_base_list_type::iterator (isomeric_same_base_list_type::* isomeric_same_begin_end_foo_type)(void);

		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 5);

		cont1.splice(
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1),
			cont2,
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::begin), _1),
			boost::bind(static_cast<isomeric_same_begin_end_foo_type>(&isomeric_same_base_list_type::end), _1));

		yggr_test_assert(cont1.size() == 15);
		yggr_test_assert(cont2.size() == 0);
	}

	// merge

#if !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)
	{
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 10);

		cont1.merge(boost::move(cont2));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 10);

		cont1.merge(boost::move(cont2));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 10);

		cont1.merge(cont2);

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 10);

		cont1.merge(cont2);

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 10);

		std::not_equal_to<int> foo_ne;

		cont1.merge(boost::move(cont2), boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 10);

		std::not_equal_to<int> foo_ne;

		cont1.merge(boost::move(cont2), boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		base_base_list_type cont2(arr + 0, arr + 10);

		std::not_equal_to<int> foo_ne;

		cont1.merge(cont2, boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_base_list_type cont2(arr + 0, arr + 10);

		std::not_equal_to<int> foo_ne;

		cont1.merge(cont2, boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}
#endif // !(BOOST_VERSION < 107000) && !defined(YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION)

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 10);

		cont1.merge(boost::move(cont2));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 10);

		cont1.merge(cont2);

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 10);

		std::not_equal_to<int> foo_ne;

		cont1.merge(boost::move(cont2), boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_base_list_type cont2(arr + 0, arr + 10);

		std::not_equal_to<int> foo_ne;

		cont1.merge(cont2, boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 10);

		cont1.merge(boost::move(cont2));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 10);

		cont1.merge(cont2);

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 10);

		std::not_equal_to<int> foo_ne;

		cont1.merge(boost::move(cont2), boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		isomeric_same_list_type cont2(arr + 0, arr + 10);

		std::not_equal_to<int> foo_ne;

		cont1.merge(cont2, boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

		yggr_test_assert(cont1.size() == 20);
		yggr_test_assert(cont2.size() == 0);
	}

	std::cout << "---------------test_operators end-----------------" << std::endl;
}

template<typename Cont>
void debug_test(void)
{
	typedef Cont list_type;

	test_constructor<list_type>();
	test_operator_set<list_type>();
	test_operator_cmp<list_type>();
	test_safe_other<list_type>();

	test_swap<list_type>();
	test_stl_swap<list_type>();
	test_boost_swap<list_type>();
	test_yggr_swap<list_type>();

	test_operators<list_type>();
}


int main(int argc, char* argv[])
{
	{
		typedef yggr::safe_container::safe_list<int> list_type;
		debug_test<list_type>();
	}

	wait_any_key(argc, argv);
	return 0;
}
