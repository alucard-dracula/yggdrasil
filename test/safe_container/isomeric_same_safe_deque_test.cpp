//isomeric_same_safe_deque_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/bind.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/safe_container/safe_deque.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)

#define THREAD_TEST_USING_MANUAL() 0

#define THREAD_TEST_THREAD_SIZE() 10
#define THREAD_TEST_THREAD_LOOP_SIZE() 10

//-------------------------test base-------------------------

template<typename Cont>
void test_constructor(void)
{
	typedef Cont dq_type;
	typedef typename dq_type::value_type val_type;
	typedef typename dq_type::allocator_type alloc_type;
	typedef typename dq_type::base_type base_dq_type;
	typedef
		yggr::safe_container::safe_deque
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::deque
		> isomeric_same_dq_type;
	typedef typename isomeric_same_dq_type::base_type isomeric_same_base_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	{
		isomeric_same_base_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont(right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_base_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont(right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void test_operator_set(void)
{
	typedef Cont dq_type;
	typedef typename dq_type::value_type val_type;
	typedef typename dq_type::allocator_type alloc_type;
	typedef typename dq_type::base_type base_dq_type;
	typedef
		yggr::safe_container::safe_deque
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::deque
		> isomeric_same_dq_type;
	typedef typename isomeric_same_dq_type::base_type isomeric_same_base_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	{
		isomeric_same_base_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_dq_type right_cont(arr + 0, arr + 10);
		dq_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	std::cout << "---------------test_operator_set end-----------------" << std::endl;
}

template<typename Cont>
void test_operator_cmp(void)
{
	typedef Cont dq_type;
	typedef typename dq_type::value_type val_type;
	typedef typename dq_type::allocator_type alloc_type;
	typedef typename dq_type::base_type base_dq_type;
	typedef
		yggr::safe_container::safe_deque
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::deque
		> isomeric_same_dq_type;
	typedef typename isomeric_same_dq_type::base_type isomeric_same_base_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	dq_type cont(arr + 0, arr + 10);

	dq_type cont_equal(cont);
	isomeric_same_dq_type isomeric_same_cont_equal(cont);

	dq_type cont_less(cont);

	cont_less[9] = cont_less[9] - 1;
	isomeric_same_dq_type isomeric_same_cont_less(cont);
	isomeric_same_cont_less[9] = isomeric_same_cont_less[9] - 1;

	dq_type cont_greater(cont);
	cont_greater[9] = cont_greater[9] + 1;
	isomeric_same_dq_type isomeric_same_cont_greater(cont);
	isomeric_same_cont_greater[9] = isomeric_same_cont_greater[9] + 1;

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
	typedef Cont dq_type;
	typedef typename dq_type::value_type val_type;
	typedef typename dq_type::allocator_type alloc_type;
	typedef typename dq_type::base_type base_dq_type;
	typedef
		yggr::safe_container::safe_deque
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::deque
		> isomeric_same_dq_type;
	typedef typename isomeric_same_dq_type::base_type isomeric_same_base_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	dq_type cont(arr + 0, arr + 10);

	// operator base
	{
		isomeric_same_base_dq_type isomeric_same_base_cont;
		isomeric_same_base_cont = cont;
		yggr_test_assert(isomeric_same_base_cont == cont);
	}


	// copy_to_base
	{
		isomeric_same_base_dq_type isomeric_same_base_cont;
		cont.copy_to_base(isomeric_same_base_cont);
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	{
		isomeric_same_base_dq_type isomeric_same_base_cont;
		cont.copy_to_base(isomeric_same_base_cont, 0, cont.size());
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	// load
	{
		isomeric_same_base_dq_type isomeric_same_base_cont;
		isomeric_same_base_cont = cont.template load<isomeric_same_base_dq_type>();
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	// store
	{
		isomeric_same_base_dq_type isomeric_same_base_cont = cont;
		dq_type other_cont;
		other_cont.store(boost::move(isomeric_same_base_cont));
		yggr_test_assert(other_cont == cont);
	}

	{
		isomeric_same_base_dq_type isomeric_same_base_cont = cont;
		dq_type other_cont;
		other_cont.store(isomeric_same_base_cont);
		yggr_test_assert(other_cont == cont);
	}


	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}


template<typename Cont>
void test_swap(void)
{
	typedef Cont dq_type;
	typedef typename dq_type::value_type val_type;
	typedef typename dq_type::allocator_type alloc_type;
	typedef typename dq_type::base_type base_dq_type;
	typedef
		yggr::safe_container::safe_deque
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::deque
		> isomeric_same_dq_type;
	typedef typename isomeric_same_dq_type::base_type isomeric_same_base_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	dq_type dq_chk(arr1 + 0, arr1 + 10);
	base_dq_type base_dq_chk = dq_chk.load();

	isomeric_same_dq_type isomeric_same_dq_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_dq_type isomeric_same_base_dq_chk = isomeric_same_dq_chk.load();

	{
		dq_type dq = dq_chk;
		isomeric_same_base_dq_type right_dq = isomeric_same_base_dq_chk;

		dq.swap(boost::move(right_dq));

		yggr_test_assert(dq == isomeric_same_dq_chk);
		yggr_test_assert(right_dq == base_dq_chk);
	}

	{
		dq_type dq = dq_chk;
		isomeric_same_base_dq_type right_dq = isomeric_same_base_dq_chk;

		dq.swap(right_dq);

		yggr_test_assert(dq == isomeric_same_dq_chk);
		yggr_test_assert(right_dq == base_dq_chk);
	}

	{
		dq_type dq = dq_chk;
		isomeric_same_dq_type right_dq = isomeric_same_dq_chk;

		dq.swap(boost::move(right_dq));

		yggr_test_assert(dq == isomeric_same_dq_chk);
		yggr_test_assert(right_dq == dq_chk);
	}

	{
		dq_type dq = dq_chk;
		isomeric_same_dq_type right_dq = isomeric_same_dq_chk;

		dq.swap(right_dq);

		yggr_test_assert(dq == isomeric_same_dq_chk);
		yggr_test_assert(right_dq == dq_chk);
	}

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_stl_swap(void)
{
	typedef Cont dq_type;
	typedef typename dq_type::value_type val_type;
	typedef typename dq_type::allocator_type alloc_type;
	typedef typename dq_type::base_type base_dq_type;
	typedef
		yggr::safe_container::safe_deque
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::deque
		> isomeric_same_dq_type;
	typedef typename isomeric_same_dq_type::base_type isomeric_same_base_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	dq_type dq_chk(arr1 + 0, arr1 + 10);
	base_dq_type base_dq_chk = dq_chk.load();

	isomeric_same_dq_type isomeric_same_dq_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_dq_type isomeric_same_base_dq_chk = isomeric_same_dq_chk.load();

	// dq, base_dq
	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_base_dq_type dq2 = isomeric_same_base_dq_chk;

		std::swap(dq1, boost::move(dq2));

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_base_dq_type dq2 = isomeric_same_base_dq_chk;

		std::swap(boost::move(dq2), dq1);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_base_dq_type dq2 = isomeric_same_base_dq_chk;

		std::swap(boost::move(dq1), dq2);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	// dq, dq
	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_dq_type dq2 = isomeric_same_base_dq_chk;

		std::swap(dq1, boost::move(dq2));

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_dq_type dq2 = isomeric_same_base_dq_chk;

		std::swap(boost::move(dq2), dq1);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_dq_type dq2 = isomeric_same_base_dq_chk;

		std::swap(dq1, dq2);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	std::cout << "---------------test_stl_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_boost_swap(void)
{
	typedef Cont dq_type;
	typedef typename dq_type::value_type val_type;
	typedef typename dq_type::allocator_type alloc_type;
	typedef typename dq_type::base_type base_dq_type;
	typedef
		yggr::safe_container::safe_deque
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::deque
		> isomeric_same_dq_type;
	typedef typename isomeric_same_dq_type::base_type isomeric_same_base_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	dq_type dq_chk(arr1 + 0, arr1 + 10);
	base_dq_type base_dq_chk = dq_chk.load();

	isomeric_same_dq_type isomeric_same_dq_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_dq_type isomeric_same_base_dq_chk = isomeric_same_dq_chk.load();

	// dq, base_dq
	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_base_dq_type dq2 = isomeric_same_base_dq_chk;

		boost::swap(dq1, boost::move(dq2));

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_base_dq_type dq2 = isomeric_same_base_dq_chk;

		boost::swap(boost::move(dq2), dq1);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_base_dq_type dq2 = isomeric_same_base_dq_chk;

		boost::swap(boost::move(dq1), dq2);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	// dq, dq
	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_dq_type dq2 = isomeric_same_base_dq_chk;

		boost::swap(dq1, boost::move(dq2));

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_dq_type dq2 = isomeric_same_base_dq_chk;

		boost::swap(boost::move(dq2), dq1);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_dq_type dq2 = isomeric_same_base_dq_chk;

		boost::swap(dq1, dq2);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	std::cout << "---------------test_boost_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_yggr_swap(void)
{
	typedef Cont dq_type;
	typedef typename dq_type::value_type val_type;
	typedef typename dq_type::allocator_type alloc_type;
	typedef typename dq_type::base_type base_dq_type;
	typedef
		yggr::safe_container::safe_deque
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::deque
		> isomeric_same_dq_type;
	typedef typename isomeric_same_dq_type::base_type isomeric_same_base_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	dq_type dq_chk(arr1 + 0, arr1 + 10);
	base_dq_type base_dq_chk = dq_chk.load();

	isomeric_same_dq_type isomeric_same_dq_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_dq_type isomeric_same_base_dq_chk = isomeric_same_dq_chk.load();

	// dq, base_dq
	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_base_dq_type dq2 = isomeric_same_base_dq_chk;

		yggr::swap(dq1, boost::move(dq2));

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_base_dq_type dq2 = isomeric_same_base_dq_chk;

		yggr::swap(boost::move(dq2), dq1);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_base_dq_type dq2 = isomeric_same_base_dq_chk;

		yggr::swap(boost::move(dq1), dq2);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	// dq, dq
	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_dq_type dq2 = isomeric_same_base_dq_chk;

		yggr::swap(dq1, boost::move(dq2));

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_dq_type dq2 = isomeric_same_base_dq_chk;

		yggr::swap(boost::move(dq2), dq1);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	{
		dq_type dq1 = base_dq_chk;
		isomeric_same_dq_type dq2 = isomeric_same_base_dq_chk;

		yggr::swap(dq1, dq2);

		yggr_test_assert(dq1 == isomeric_same_dq_chk);
		yggr_test_assert(dq2 == dq_chk);
	}

	std::cout << "---------------test_yggr_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_safe_element_reference(void)
{
	typedef Cont dq_type;
	typedef typename dq_type::value_type val_type;
	typedef typename dq_type::allocator_type alloc_type;
	typedef typename dq_type::base_type base_dq_type;
	typedef
		yggr::safe_container::safe_deque
		<
			val_type,
			boost::mutex,
			alloc_type,
			yggr::container::deque
		> isomeric_same_dq_type;
	typedef typename isomeric_same_dq_type::base_type isomeric_same_base_dq_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_dq_type, isomeric_same_base_dq_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_deque<base_dq_type, isomeric_same_base_dq_type>));
#endif // (BOOST_VERSION < 107000)

	val_type chk = 0;
	float chk2 = 0;

	{
		int arr[10] = {1, 2, 3, 4, 5};
		dq_type cont(arr + 0, arr + 10);

		chk = cont[0];
		assert(chk == 1);
	}

	// + -
	{
		int arr[10] = {1, 2, 3, 4, 5};
		dq_type cont(arr + 0, arr + 10);
		
		// +
		chk = +cont[0];
		assert(chk == +arr[0]);

		// -
		chk = -cont[0];
		assert(chk == -arr[0]);
	}

	// ++ --
	{
		int arr[10] = {1, 2, 3, 4, 5};
		dq_type cont(arr + 0, arr + 10);
		
		// +
		chk = ++cont[0];
		assert(chk == ++arr[0]);

		chk = cont[0]++;
		assert(chk == arr[0]++);

		// -
		chk = --cont[0];
		assert(chk == --arr[0]);

		chk = cont[0]--;
		assert(chk == arr[0]--);
	}

	// + - * / %
	{
		int arr[10] = {1, 2, 3, 4, 5};
		dq_type cont(arr + 0, arr + 10);

		// +
		chk = cont[0] + 1;
		assert(chk == arr[0] + 1);

		chk = 1 + cont[0];
		assert(chk == 1 + arr[0]);

		chk2 = cont[0] + 1.5f;
		assert(chk2 == arr[0] + 1.5f);

		chk2 = 1.5f + cont[0];
		assert(chk2 == 1.5f + arr[0]);

		chk = cont[0] + cont[1];
		assert(chk == arr[0] + arr[1]);

		// -
		chk = cont[0] - 1;
		assert(chk == arr[0] - 1);

		chk = 1 - cont[0];
		assert(chk == 1 - arr[0]);

		chk2 = cont[0] - 1.5f;
		assert(chk2 == arr[0] - 1.5f);

		chk2 = 1.5f - cont[0];
		assert(chk2 == 1.5f - arr[0]);

		chk = cont[0] - cont[1];
		assert(chk == arr[0] - arr[1]);

		// *
		chk = cont[0] * 1;
		assert(chk == arr[0] * 1);

		chk = 1 * cont[0];
		assert(chk == 1 * arr[0]);

		chk2 = cont[0] * 1.5f;
		assert(chk2 == arr[0] * 1.5f);

		chk2 = 1.5f * cont[0];
		assert(chk2 == 1.5f * arr[0]);

		chk = cont[0] * cont[1];
		assert(chk == arr[0] * arr[1]);

		// /
		chk = cont[0] / 1;
		assert(chk == arr[0] / 1);

		chk = 1 / cont[0];
		assert(chk == 1 / arr[0]);

		chk2 = cont[0] / 1.5f;
		assert(chk2 == arr[0] / 1.5f);

		chk2 = 1.5f / cont[0];
		assert(chk2 == 1.5f / arr[0]);

		chk = cont[0] / cont[1];
		assert(chk == arr[0] / arr[1]);

		// %
		chk = cont[0] % 1;
		assert(chk == arr[0] % 1);

		chk = 1 % cont[0];
		assert(chk == 1 % arr[0]);

		chk2 = std::fmod(cont[0], 1.5f);
		assert(chk2 == std::fmod(arr[0], 1.5f));

		chk2 = std::fmod(1.5f, cont[0]);
		assert(chk2 == std::fmod(1.5f, arr[0]));

		chk = cont[0] % cont[1];
		assert(chk == arr[0] % arr[1]);
	}

	// += -= *= /= %=
	{
		// +=
		{
			int arr[10] = {1, 2, 3, 4, 5};
			dq_type cont(arr + 0, arr + 10);

			chk = 1;
			chk += cont[0];
			assert(chk == 1 + arr[0]);

			chk = (cont[0] += 1);
			assert(chk == arr[0] + 1);

			chk2 = 1.5f;
			chk2 += cont[1];
			assert(chk2 == 1.5f + arr[1]);

			chk2 = (cont[1] += 1.5f);
			assert(chk2 == static_cast<int>(arr[1] + 1.5));

			chk = (cont[2] += cont[3]);
			assert(chk == arr[2] + arr[3]);
		}

		// -=
		{
			int arr[10] = {1, 2, 3, 4, 5};
			dq_type cont(arr + 0, arr + 10);

			chk = 1;
			chk -= cont[0];
			assert(chk == 1 - arr[0]);

			chk = (cont[0] -= 1);
			assert(chk == arr[0] - 1);

			chk2 = 1.5f;
			chk2 -= cont[1];
			assert(chk2 == 1.5f - arr[1]);

			chk2 = (cont[1] -= 1.5f);
			assert(chk2 == static_cast<int>(arr[1] - 1.5));

			chk = (cont[2] -= cont[3]);
			assert(chk == arr[2] - arr[3]);
		}

		// *=
		{
			int arr[10] = {1, 2, 3, 4, 5};
			dq_type cont(arr + 0, arr + 10);

			chk = 1;
			chk *= cont[0];
			assert(chk == 1 * arr[0]);

			chk = (cont[0] -= 1);
			assert(chk == arr[0] - 1);

			chk2 = 1.5f;
			chk2 *= cont[1];
			assert(chk2 == 1.5f * arr[1]);

			chk2 = (cont[1] *= 1.5f);
			assert(chk2 == static_cast<int>(arr[1] * 1.5));

			chk = (cont[2] *= cont[3]);
			assert(chk == arr[2] * arr[3]);
		}

		// /=
		{
			int arr[10] = {1, 2, 3, 4, 5};
			dq_type cont(arr + 0, arr + 10);

			chk = 1;
			chk /= cont[0];
			assert(chk == 1 / arr[0]);

			chk = (cont[0] /= 1);
			assert(chk == arr[0] / 1);

			chk2 = 1.5f;
			chk2 /= cont[1];
			assert(chk2 == 1.5f / arr[1]);

			chk2 = (cont[1] /= 1.5f);
			assert(chk2 == static_cast<int>(arr[1] / 1.5));

			chk = (cont[2] /= cont[3]);
			assert(chk == arr[2] / arr[3]);
		}

		// %=
		{
			int arr[10] = {1, 2, 3, 4, 5};
			dq_type cont(arr + 0, arr + 10);

			chk = 1;
			chk %= cont[0];
			assert(chk == 1 % arr[0]);

			chk = (cont[0] %= 1);
			assert(chk == arr[0] % 1);

			// float %= other, cpp not supported
			//chk2 = 1.5f;
			//chk2 %= cont[1];
			//assert(chk2 == std::fmod(1.5f % arr[1]));

			chk2 = (cont[1] %= 1.5f);
			assert(chk2 == static_cast<int>(std::fmod(arr[1], 1.5f)));

			chk = (cont[2] %= cont[3]);
			assert(chk == arr[2] % arr[3]);
		}
	}

	bool bchk = false;
	// && || !
	{
		int arr[10] = {1, 2, 3, 4, 5};
		dq_type cont(arr + 0, arr + 10);

		// &&

		bchk = (1 && cont[0]);
		assert(bchk == (1 && arr[0]));

		bchk = (cont[0] && 1);
		assert(bchk == (arr[0] && 1));

		bchk = (cont[0] && cont[1]);
		assert(bchk == (arr[0] && arr[1]));

		// ||

		bchk = (0 || cont[0]);
		assert(bchk == (0 || arr[0]));

		bchk = (cont[0] || 0);
		assert(bchk == (arr[0] || 0));

		bchk = (cont[0] || cont[1]);
		assert(bchk == (arr[0] || arr[1]));

		// !

		bchk = !cont[0];
		assert(bchk == (!arr[0]));

	}

	// & | ^ << >> ~
	{
		int arr[10] = {1, 2, 3, 4, 5};
		dq_type cont(arr + 0, arr + 10);

		// &
		chk = (cont[0] & 1);
		assert(chk == (arr[0] & 1));

		chk = (1 & cont[0]);
		assert(chk == (1 & arr[0]));

		chk = (cont[0] & cont[1]);
		assert(chk == (arr[0] & arr[1]));

		// |
		chk = (cont[0] | 1);
		assert(chk == (arr[0] | 1));

		chk = (1 | cont[0]);
		assert(chk == (1 | arr[0]));

		chk = (cont[0] | cont[1]);
		assert(chk == (arr[0] | arr[1]));

		// ^
		chk = (cont[0] ^ 1);
		assert(chk == (arr[0] ^ 1));

		chk = (1 ^ cont[0]);
		assert(chk == (1 ^ arr[0]));

		chk = (cont[0] ^ cont[1]);
		assert(chk == (arr[0] ^ arr[1]));

		// <<
		chk = (cont[0] << 1);
		assert(chk == (arr[0] << 1));

		chk = (1 << cont[0]);
		assert(chk == (1 << arr[0]));

		chk = (cont[0] << cont[1]);
		assert(chk == (arr[0] << arr[1]));

		std::cout << cont[0] << std::endl;

		// >>
		chk = (cont[0] >> 1);
		assert(chk == (arr[0] >> 1));

		chk = (1 >> cont[0]);
		assert(chk == (1 >> arr[0]));

		chk = (cont[0] >> cont[1]);
		assert(chk == (arr[0] >> arr[1]));

		// ~
		chk = ~cont[0];
		assert(chk == (~arr[0]));

	}

	// &= |= ^= <<= >>=
	{
		// &=
		{
			int arr[10] = {1, 2, 3, 4, 5};
			dq_type cont(arr + 0, arr + 10);

			chk = 1;
			chk &= cont[0];
			assert(chk == (1 & arr[0]));

			chk = (cont[0] &= 1);
			assert(chk == (arr[0] & 1));

			chk = (cont[2] &= cont[3]);
			assert(chk == (arr[2] & arr[3]));
		}

		// |=
		{
			int arr[10] = {1, 2, 3, 4, 5};
			dq_type cont(arr + 0, arr + 10);

			chk = 1;
			chk |= cont[0];
			assert(chk == (1 | arr[0]));

			chk = (cont[0] |= 1);
			assert(chk == (arr[0] | 1));

			chk = (cont[2] |= cont[3]);
			assert(chk == (arr[2] | arr[3]));
		}

		// ^=
		{
			int arr[10] = {1, 2, 3, 4, 5};
			dq_type cont(arr + 0, arr + 10);

			chk = 1;
			chk ^= cont[0];
			assert(chk == (1 ^ arr[0]));

			chk = (cont[0] ^= 1);
			assert(chk == (arr[0] ^ 1));

			chk = (cont[2] ^= cont[3]);
			assert(chk == (arr[2] ^ arr[3]));
		}

		// <<=
		{
			int arr[10] = {1, 2, 3, 4, 5};
			dq_type cont(arr + 0, arr + 10);

			chk = 1;
			chk <<= cont[0];
			assert(chk == (1 << arr[0]));

			chk = (cont[0] <<= 1);
			assert(chk == (arr[0] << 1));

			chk = (cont[2] <<= cont[3]);
			assert(chk == (arr[2] << arr[3]));
		}

		// >>=
		{
			int arr[10] = {1, 2, 3, 4, 5};
			dq_type cont(arr + 0, arr + 10);

			chk = 1;
			chk >>= cont[0];
			assert(chk == (1 >> arr[0]));

			chk = (cont[0] >>= 1);
			assert(chk == (arr[0] >> 1));

			chk = (cont[2] >>= cont[3]);
			assert(chk == (arr[2] >> arr[3]));
		}
	}

	// cmper
	{
		int arr[10] = {1, 2, 3, 4, 5};
		dq_type cont(arr + 0, arr + 10);

		// ==
		bchk = (cont[0] == arr[0]);
		assert(bchk);

		bchk = (arr[0] == cont[0]);
		assert(bchk);

		bchk = (cont[0] == cont[0]);
		assert(bchk);

		// !=
		bchk = (cont[0] != arr[1]);
		assert(bchk);

		bchk = (arr[0] != cont[1]);
		assert(bchk);

		bchk = (cont[0] != cont[1]);
		assert(bchk);

		// <
		bchk = (cont[0] < arr[1]);
		assert(bchk);

		bchk = (arr[0] < cont[1]);
		assert(bchk);

		bchk = (cont[0] < cont[1]);
		assert(bchk);

		// <=
		bchk = (cont[0] <= arr[1]);
		assert(bchk);

		bchk = (arr[0] <= cont[1]);
		assert(bchk);

		bchk = (cont[0] <= cont[1]);
		assert(bchk);

		// >
		bchk = (cont[0] > arr[1]);
		assert(!bchk);

		bchk = (arr[0] > cont[1]);
		assert(!bchk);

		bchk = (cont[0] > cont[1]);
		assert(!bchk);

		// >=
		bchk = (cont[0] > arr[1]);
		assert(!bchk);

		bchk = (arr[0] > cont[1]);
		assert(!bchk);

		bchk = (cont[0] > cont[1]);
		assert(!bchk);
	}


	std::cout << "---------------test_safe_element_reference end-----------------" << std::endl;
}

template<typename Cont>
void debug_test(void)
{
	typedef Cont dq_type;

	test_constructor<dq_type>();
	test_operator_set<dq_type>();
	test_operator_cmp<dq_type>();
	test_safe_other<dq_type>();

	test_swap<dq_type>();
	test_stl_swap<dq_type>();
	test_boost_swap<dq_type>();
	test_yggr_swap<dq_type>();

	test_safe_element_reference<dq_type>();
}


int main(int argc, char* argv[])
{
	{
		typedef yggr::safe_container::safe_deque<int> dq_type;
		debug_test<dq_type>();
	}


	wait_any_key(argc, argv);
	return 0;
}
