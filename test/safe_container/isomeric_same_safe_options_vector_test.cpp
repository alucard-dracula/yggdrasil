//isomeric_same_safe_options_vector_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/bind.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/safe_container/safe_options_vector.hpp>

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
	typedef Cont vt_type;
	typedef typename vt_type::value_type val_type;
	typedef typename vt_type::allocator_type alloc_type;
	typedef typename vt_type::base_type base_vt_type;
	typedef
		yggr::safe_container::safe_options_vector
		<
			val_type,
			boost::mutex,
			alloc_type,
			void,
			yggr::container::options_vector
		> isomeric_same_vt_type;
	typedef typename isomeric_same_vt_type::base_type isomeric_same_base_vt_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	{
		isomeric_same_base_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont(right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_base_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont(boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont(right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont(boost::move(right_cont), right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont(right_cont, right_cont.get_allocator());
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void test_operator_set(void)
{
	typedef Cont vt_type;
	typedef typename vt_type::value_type val_type;
	typedef typename vt_type::allocator_type alloc_type;
	typedef typename vt_type::base_type base_vt_type;
	typedef
		yggr::safe_container::safe_options_vector
		<
			val_type,
			boost::mutex,
			alloc_type,
			void,
			yggr::container::options_vector
		> isomeric_same_vt_type;
	typedef typename isomeric_same_vt_type::base_type isomeric_same_base_vt_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	{
		isomeric_same_base_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_base_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	{
		isomeric_same_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont;
		cont = (boost::move(right_cont));
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 0);
	}

	{
		isomeric_same_vt_type right_cont(arr + 0, arr + 10);
		vt_type cont;
		cont = (right_cont);
		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(right_cont.size() == 10);
	}

	std::cout << "---------------test_operator_set end-----------------" << std::endl;
}

template<typename Cont>
void test_operator_cmp(void)
{
	typedef Cont vt_type;
	typedef typename vt_type::value_type val_type;
	typedef typename vt_type::allocator_type alloc_type;
	typedef typename vt_type::base_type base_vt_type;
	typedef
		yggr::safe_container::safe_options_vector
		<
			val_type,
			boost::mutex,
			alloc_type,
			void,
			yggr::container::options_vector
		> isomeric_same_vt_type;
	typedef typename isomeric_same_vt_type::base_type isomeric_same_base_vt_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	vt_type cont(arr + 0, arr + 10);

	vt_type cont_equal(cont);
	isomeric_same_vt_type isomeric_same_cont_equal(cont);

	vt_type cont_less(cont);
	cont_less[9] = cont_less[9] - 1;
	isomeric_same_vt_type isomeric_same_cont_less(cont);
	isomeric_same_cont_less[9] = isomeric_same_cont_less[9] - 1;

	vt_type cont_greater(cont);
	cont_greater[9] = cont_greater[9] + 1;
	isomeric_same_vt_type isomeric_same_cont_greater(cont);
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
	typedef Cont vt_type;
	typedef typename vt_type::value_type val_type;
	typedef typename vt_type::allocator_type alloc_type;
	typedef typename vt_type::base_type base_vt_type;
	typedef
		yggr::safe_container::safe_options_vector
		<
			val_type,
			boost::mutex,
			alloc_type,
			void,
			yggr::container::options_vector
		> isomeric_same_vt_type;
	typedef typename isomeric_same_vt_type::base_type isomeric_same_base_vt_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#endif // (BOOST_VERSION < 107000)

	int arr[10] = {1, 2, 3, 4, 5};

	vt_type cont(arr + 0, arr + 10);

	// operator base
	{
		isomeric_same_base_vt_type isomeric_same_base_cont;
		isomeric_same_base_cont = cont;
		yggr_test_assert(isomeric_same_base_cont == cont);
	}


	// copy_to_base
	{
		isomeric_same_base_vt_type isomeric_same_base_cont;
		cont.copy_to_base(isomeric_same_base_cont);
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	{
		isomeric_same_base_vt_type isomeric_same_base_cont;
		cont.copy_to_base(isomeric_same_base_cont, 0, cont.size());
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	// load
	{
		isomeric_same_base_vt_type isomeric_same_base_cont;
		isomeric_same_base_cont = cont.template load<isomeric_same_base_vt_type>();
		yggr_test_assert(isomeric_same_base_cont == cont);
	}

	// store
	{
		isomeric_same_base_vt_type isomeric_same_base_cont = cont;
		vt_type other_cont;
		other_cont.store(boost::move(isomeric_same_base_cont));
		yggr_test_assert(other_cont == cont);
	}

	{
		isomeric_same_base_vt_type isomeric_same_base_cont = cont;
		vt_type other_cont;
		other_cont.store(isomeric_same_base_cont);
		yggr_test_assert(other_cont == cont);
	}


	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}


template<typename Cont>
void test_swap(void)
{
	typedef Cont vt_type;
	typedef typename vt_type::value_type val_type;
	typedef typename vt_type::allocator_type alloc_type;
	typedef typename vt_type::base_type base_vt_type;
	typedef
		yggr::safe_container::safe_options_vector
		<
			val_type,
			boost::mutex,
			alloc_type,
			void,
			yggr::container::options_vector
		> isomeric_same_vt_type;
	typedef typename isomeric_same_vt_type::base_type isomeric_same_base_vt_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	vt_type vt_chk(arr1 + 0, arr1 + 10);
	base_vt_type base_vt_chk = vt_chk.load();

	isomeric_same_vt_type isomeric_same_vt_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_vt_type isomeric_same_base_vt_chk = isomeric_same_vt_chk.load();

	{
		vt_type vt = vt_chk;
		isomeric_same_base_vt_type right_vt = isomeric_same_base_vt_chk;

		vt.swap(boost::move(right_vt));

		yggr_test_assert(vt == isomeric_same_vt_chk);
		yggr_test_assert(right_vt == base_vt_chk);
	}

	{
		vt_type vt = vt_chk;
		isomeric_same_base_vt_type right_vt = isomeric_same_base_vt_chk;

		vt.swap(right_vt);

		yggr_test_assert(vt == isomeric_same_vt_chk);
		yggr_test_assert(right_vt == base_vt_chk);
	}

	{
		vt_type vt = vt_chk;
		isomeric_same_vt_type right_vt = isomeric_same_vt_chk;

		vt.swap(boost::move(right_vt));

		yggr_test_assert(vt == isomeric_same_vt_chk);
		yggr_test_assert(right_vt == vt_chk);
	}

	{
		vt_type vt = vt_chk;
		isomeric_same_vt_type right_vt = isomeric_same_vt_chk;

		vt.swap(right_vt);

		yggr_test_assert(vt == isomeric_same_vt_chk);
		yggr_test_assert(right_vt == vt_chk);
	}

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_stl_swap(void)
{
	typedef Cont vt_type;
	typedef typename vt_type::value_type val_type;
	typedef typename vt_type::allocator_type alloc_type;
	typedef typename vt_type::base_type base_vt_type;
	typedef
		yggr::safe_container::safe_options_vector
		<
			val_type,
			boost::mutex,
			alloc_type,
			void,
			yggr::container::options_vector
		> isomeric_same_vt_type;
	typedef typename isomeric_same_vt_type::base_type isomeric_same_base_vt_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	vt_type vt_chk(arr1 + 0, arr1 + 10);
	base_vt_type base_vt_chk = vt_chk.load();

	isomeric_same_vt_type isomeric_same_vt_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_vt_type isomeric_same_base_vt_chk = isomeric_same_vt_chk.load();

	// vt, base_vt
	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_base_vt_type vt2 = isomeric_same_base_vt_chk;

		std::swap(vt1, boost::move(vt2));

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_base_vt_type vt2 = isomeric_same_base_vt_chk;

		std::swap(boost::move(vt2), vt1);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_base_vt_type vt2 = isomeric_same_base_vt_chk;

		std::swap(boost::move(vt1), vt2);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	// vt, vt
	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_vt_type vt2 = isomeric_same_base_vt_chk;

		std::swap(vt1, boost::move(vt2));

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_vt_type vt2 = isomeric_same_base_vt_chk;

		std::swap(boost::move(vt2), vt1);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_vt_type vt2 = isomeric_same_base_vt_chk;

		std::swap(vt1, vt2);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	std::cout << "---------------test_stl_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_boost_swap(void)
{
	typedef Cont vt_type;
	typedef typename vt_type::value_type val_type;
	typedef typename vt_type::allocator_type alloc_type;
	typedef typename vt_type::base_type base_vt_type;
	typedef
		yggr::safe_container::safe_options_vector
		<
			val_type,
			boost::mutex,
			alloc_type,
			void,
			yggr::container::options_vector
		> isomeric_same_vt_type;
	typedef typename isomeric_same_vt_type::base_type isomeric_same_base_vt_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	vt_type vt_chk(arr1 + 0, arr1 + 10);
	base_vt_type base_vt_chk = vt_chk.load();

	isomeric_same_vt_type isomeric_same_vt_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_vt_type isomeric_same_base_vt_chk = isomeric_same_vt_chk.load();

	// vt, base_vt
	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_base_vt_type vt2 = isomeric_same_base_vt_chk;

		boost::swap(vt1, boost::move(vt2));

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_base_vt_type vt2 = isomeric_same_base_vt_chk;

		boost::swap(boost::move(vt2), vt1);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_base_vt_type vt2 = isomeric_same_base_vt_chk;

		boost::swap(boost::move(vt1), vt2);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	// vt, vt
	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_vt_type vt2 = isomeric_same_base_vt_chk;

		boost::swap(vt1, boost::move(vt2));

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_vt_type vt2 = isomeric_same_base_vt_chk;

		boost::swap(boost::move(vt2), vt1);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_vt_type vt2 = isomeric_same_base_vt_chk;

		boost::swap(vt1, vt2);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	std::cout << "---------------test_boost_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_yggr_swap(void)
{
	typedef Cont vt_type;
	typedef typename vt_type::value_type val_type;
	typedef typename vt_type::allocator_type alloc_type;
	typedef typename vt_type::base_type base_vt_type;
	typedef
		yggr::safe_container::safe_options_vector
		<
			val_type,
			boost::mutex,
			alloc_type,
			void,
			yggr::container::options_vector
		> isomeric_same_vt_type;
	typedef typename isomeric_same_vt_type::base_type isomeric_same_base_vt_type;

#if (BOOST_VERSION < 107000)
	BOOST_MPL_ASSERT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#else
	BOOST_MPL_ASSERT_NOT((boost::is_same<base_vt_type, isomeric_same_base_vt_type>));
	BOOST_MPL_ASSERT((yggr::is_isomeric_same_vector<base_vt_type, isomeric_same_base_vt_type>));
#endif // (BOOST_VERSION < 107000)

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {1, 2, 3, 4, 6};

	vt_type vt_chk(arr1 + 0, arr1 + 10);
	base_vt_type base_vt_chk = vt_chk.load();

	isomeric_same_vt_type isomeric_same_vt_chk(arr2 + 0, arr2 + 10);
	isomeric_same_base_vt_type isomeric_same_base_vt_chk = isomeric_same_vt_chk.load();

	// vt, base_vt
	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_base_vt_type vt2 = isomeric_same_base_vt_chk;

		yggr::swap(vt1, boost::move(vt2));

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_base_vt_type vt2 = isomeric_same_base_vt_chk;

		yggr::swap(boost::move(vt2), vt1);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_base_vt_type vt2 = isomeric_same_base_vt_chk;

		yggr::swap(boost::move(vt1), vt2);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	// vt, vt
	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_vt_type vt2 = isomeric_same_base_vt_chk;

		yggr::swap(vt1, boost::move(vt2));

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_vt_type vt2 = isomeric_same_base_vt_chk;

		yggr::swap(boost::move(vt2), vt1);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	{
		vt_type vt1 = base_vt_chk;
		isomeric_same_vt_type vt2 = isomeric_same_base_vt_chk;

		yggr::swap(vt1, vt2);

		yggr_test_assert(vt1 == isomeric_same_vt_chk);
		yggr_test_assert(vt2 == vt_chk);
	}

	std::cout << "---------------test_yggr_swap end-----------------" << std::endl;
}

template<typename Cont>
void debug_test(void)
{
	typedef Cont vt_type;

	test_constructor<vt_type>();
	test_operator_set<vt_type>();
	test_operator_cmp<vt_type>();
	test_safe_other<vt_type>();

	test_swap<vt_type>();
	test_stl_swap<vt_type>();
	test_boost_swap<vt_type>();
	test_yggr_swap<vt_type>();
}


int main(int argc, char* argv[])
{
	{
		typedef yggr::safe_container::safe_options_vector<int> vt_type;
		debug_test<vt_type>();
	}


	wait_any_key(argc, argv);
	return 0;
}
