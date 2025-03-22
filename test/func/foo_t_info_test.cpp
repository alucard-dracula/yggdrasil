//foo_t_info_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/func/foo_t_info.hpp>
//#include <yggr/ppex/pp_debug.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/equal.hpp>

#include <cassert>
#include <functional>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void foo0(void)
{
}

int foo1(int)
{
	return 0;
}

short foo2(int, int)
{
	return 0;
}

void nefoo0(void) YGGR_NOEXCEPT
{
}

int nefoo1(int) YGGR_NOEXCEPT
{
	return 0;
}

short nefoo2(int, int) YGGR_NOEXCEPT
{
	return 0;
}


struct A
{
	A(void)
	{
	}

	A(int)
	{
	}

	A(int, int)
	{
	}

	void foo0(void)
	{
	}

	int foo1(int)
	{
		return 0;
	}

	short foo2(int, int)
	{
		return 0;
	}

	void cfoo0(void) const
	{
	}

	int cfoo1(int) const
	{
		return 0;
	}

	short cfoo2(int, int) const
	{
		return 0;
	}

	void vfoo0(void) volatile
	{
	}

	int vfoo1(int) volatile
	{
		return 0;
	}

	short vfoo2(int, int) volatile
	{
		return 0;
	}

	void nefoo0(void) YGGR_NOEXCEPT
	{
	}

	int nefoo1(int) YGGR_NOEXCEPT
	{
		return 0;
	}

	short nefoo2(int, int) YGGR_NOEXCEPT
	{
		return 0;
	}

	void cvfoo0(void) const volatile
	{
	}

	int cvfoo1(int) const volatile
	{
		return 0;
	}

	short cvfoo2(int, int) const volatile
	{
		return 0;
	}

	void cnefoo0(void) const YGGR_NOEXCEPT
	{
	}

	int cnefoo1(int) const YGGR_NOEXCEPT
	{
		return 0;
	}

	short cnefoo2(int, int) const YGGR_NOEXCEPT
	{
		return 0;
	}

	void vnefoo0(void) volatile YGGR_NOEXCEPT
	{
	}

	int vnefoo1(int) volatile YGGR_NOEXCEPT
	{
		return 0;
	}

	short vnefoo2(int, int) volatile YGGR_NOEXCEPT
	{
		return 0;
	}

	void cvnefoo0(void) const volatile YGGR_NOEXCEPT
	{
	}

	int cvnefoo1(int) const volatile YGGR_NOEXCEPT
	{
		return 0;
	}

	short cvnefoo2(int, int) const volatile YGGR_NOEXCEPT
	{
		return 0;
	}
};

template<typename T>
void test_not_foo(const T&)
{
	typedef T now_foo_type;
	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT_NOT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, typename info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, typename info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, typename info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, typename info_type::null_type>));

	BOOST_MPL_ASSERT_NOT((typename info_type::arg_list_size_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::arg_holder_list_size_type));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<0> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::arg_list_size>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::arg_holder_list_size>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<0>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void test0(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, void>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<0> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 0>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void test1(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ptr_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<1> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<1> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 1>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 1>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void test2(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ref_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int, int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void ne_test0(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

//	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
//	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
//	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
//	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
//	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
//#if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
//	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));
//#else
//	BOOST_MPL_ASSERT((typename info_type::is_noexcept_type));
//#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
//
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, void>));
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, typename info_type::null_type>));
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<>::type >));
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<>::type >));
//
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));
//
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));
//
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));
//
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<0> >));
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));
//
//	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
//	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
//	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));
//
//	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
//	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
//	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 0>));
//	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));
//
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, typename info_type::null_type>));
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
//	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));
//
//#ifdef _DEBUG
//	std::cout << info_type() << std::endl;
//#endif // _DEBUG

}

template<typename T>
void ne_test1(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ptr_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));
#else
	BOOST_MPL_ASSERT((typename info_type::is_noexcept_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<1> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<1> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 1>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 1>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void ne_test2(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ref_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));
#else
	BOOST_MPL_ASSERT((typename info_type::is_noexcept_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int, int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void m_test0(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, void>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<0> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 0>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void m_test1(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ptr_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<1> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<1> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 1>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 1>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void m_test2(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ref_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int, int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void cm_test0(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, void>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<0> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 0>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void cm_test1(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ptr_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<1> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<1> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 1>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 1>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void cm_test2(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ref_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int, int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void vm_test0(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, void>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<0> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 0>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void vm_test1(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ptr_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<1> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<1> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 1>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 1>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void vm_test2(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ref_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int, int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void cvm_test0(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, void>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<0> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 0>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void cvm_test1(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ptr_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<1> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<1> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 1>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 1>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T>
void cvm_test2(const T&)
{
	typedef T now_foo_ptr_type;
	typedef typename boost::remove_pointer<now_foo_ptr_type>::type now_foo_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_ref_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int, int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_ptr_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T, typename NavT>
void b_test2_1(const T&, const NavT&)
{
	typedef T now_foo_type;
	typedef typename boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef NavT now_nav_foo_ptr_type;
	typedef typename boost::remove_pointer<now_nav_foo_ptr_type>::type now_nav_foo_type;
	typedef typename boost::add_reference<now_nav_foo_type>::type now_nav_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<int, int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_nav_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_nav_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_nav_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg_holder<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T, typename NavT>
void b_test2_2(const T&, const NavT&)
{
	typedef T now_foo_type;
	typedef typename boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef NavT now_nav_foo_ptr_type;
	typedef typename boost::remove_pointer<now_nav_foo_ptr_type>::type now_nav_foo_type;
	typedef typename boost::add_reference<now_nav_foo_type>::type now_nav_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, typename info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_nav_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_nav_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_nav_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T, typename NavT>
void bm_test2_1(const T&, const NavT&)
{
	typedef T now_foo_type;
	typedef typename boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef NavT now_nav_foo_ptr_type;
	typedef typename boost::remove_pointer<now_nav_foo_ptr_type>::type now_nav_foo_type;
	typedef typename boost::add_reference<now_nav_foo_type>::type now_nav_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_nav_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_nav_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_nav_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T, typename NavT>
void bm_test2_2(const T&, const NavT&)
{
	typedef T now_foo_type;
	typedef typename boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef NavT now_nav_foo_ptr_type;
	typedef typename boost::remove_pointer<now_nav_foo_ptr_type>::type now_nav_foo_type;
	typedef typename boost::add_reference<now_nav_foo_type>::type now_nav_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type,
											boost::mpl::vector<yggr::func::tag_object_reference<A>, int, int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_nav_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_nav_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_nav_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<3> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 3>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG
}

template<typename T, typename NavT>
void bm_test2_3(const T&, const NavT&)
{
	typedef T now_foo_type;
	typedef typename boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef NavT now_nav_foo_ptr_type;
	typedef typename boost::remove_pointer<now_nav_foo_ptr_type>::type now_nav_foo_type;
	typedef typename boost::add_reference<now_nav_foo_type>::type now_nav_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type,
										boost::mpl::vector<int, int, yggr::func::tag_object_reference<A> >::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_nav_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_nav_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_nav_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<3> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 3>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T, typename NavT>
void bcm_test2_1(const T&, const NavT&)
{
	typedef T now_foo_type;
	typedef typename boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef NavT now_nav_foo_ptr_type;
	typedef typename boost::remove_pointer<now_nav_foo_ptr_type>::type now_nav_foo_type;
	typedef typename boost::add_reference<now_nav_foo_type>::type now_nav_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_nav_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_nav_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_nav_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T, typename NavT>
void bcm_test2_2(const T&, const NavT&)
{
	typedef T now_foo_type;
	typedef typename boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef NavT now_nav_foo_ptr_type;
	typedef typename boost::remove_pointer<now_nav_foo_ptr_type>::type now_nav_foo_type;
	typedef typename boost::add_reference<now_nav_foo_type>::type now_nav_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type,
										boost::mpl::vector<yggr::func::tag_object_reference<const A>, int, int>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_nav_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_nav_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_nav_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<3> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 3>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

template<typename T, typename NavT>
void bcm_test2_3(const T&, const NavT&)
{
	typedef T now_foo_type;
	typedef typename boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef NavT now_nav_foo_ptr_type;
	typedef typename boost::remove_pointer<now_nav_foo_ptr_type>::type now_nav_foo_type;
	typedef typename boost::add_reference<now_nav_foo_type>::type now_nav_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, short>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, A>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<int, int>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type,
											boost::mpl::vector<int, int, yggr::func::tag_object_reference<const A> >::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_nav_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_nav_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_nav_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<3> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 3>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

void test_unary_function(void)
{
	typedef std::unary_function<const int&, const int&> now_foo_type;
	typedef boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT_NOT((info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<info_type::result_type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::class_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_type, boost::mpl::vector<const int&>::type >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_type, boost::mpl::vector<const int&>::type >));

	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_pointer_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_reference_type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_size_type, boost::mpl::size_t<1> >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_size_type, boost::mpl::size_t<1> >));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 1>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 1>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<0>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<1>::type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<2>::type, info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

void test_unary_function_extends(void)
{
	typedef std::negate<int> now_foo_type;
	typedef boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<info_type::result_type, int>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::class_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_type, boost::mpl::vector<const int&>::type >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_type, boost::mpl::vector<const int&>::type >));

	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_pointer_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_reference_type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_size_type, boost::mpl::size_t<1> >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_size_type, boost::mpl::size_t<1> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 1>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 1>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<0>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<1>::type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<2>::type, info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

void test_binary_function(void)
{
	typedef std::binary_function<const float&, const int&, float> now_foo_type;
	typedef boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT_NOT((info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_volatile_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_noexcept_type));

	BOOST_MPL_ASSERT((boost::is_same<info_type::result_type, float>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::class_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_type, boost::mpl::vector<const float&, const int&>::type >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_type, boost::mpl::vector<const float&, const int&>::type >));

	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_pointer_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_reference_type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<0>::type, const float&>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<1>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<2>::type, info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

void test_binary_function_extends(void)
{
	typedef std::less<int> now_foo_type;
	typedef boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_volatile_type));

#if defined(_MSC_VER)
#	if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((info_type::is_noexcept_type));
#	else
		BOOST_MPL_ASSERT((info_type::is_noexcept_type));
#	endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
#elif defined(__MINGW32__)
#	if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((info_type::is_noexcept_type));
#	else
#       if __GNUC__  < 14
            BOOST_MPL_ASSERT((info_type::is_noexcept_type));
#       else
            BOOST_MPL_ASSERT_NOT((info_type::is_noexcept_type));
#       endif // __GNUC__  < 14
#	endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
#elif defined(__linux__)
#	if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((info_type::is_noexcept_type));
#	else
#       if __GNUC__  < 14 && !(defined(__clang__) && (__clang__))
            BOOST_MPL_ASSERT((info_type::is_noexcept_type));
#       else
            BOOST_MPL_ASSERT_NOT((info_type::is_noexcept_type));
#       endif // __GNUC__  < 14
#	endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#    if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
        BOOST_MPL_ASSERT_NOT((info_type::is_noexcept_type));
#    else
#       if __GNUC__  < 14 && !(defined(__clang__) && (__clang__))
            BOOST_MPL_ASSERT((info_type::is_noexcept_type));
#       else
            BOOST_MPL_ASSERT_NOT((info_type::is_noexcept_type));
#       endif // __GNUC__  < 14
#    endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
#else
#	error "append other platform code!!!"
#endif //_MSC_VER

	BOOST_MPL_ASSERT((boost::is_same<info_type::result_type, bool>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::class_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_type, boost::mpl::vector<const int&, const int&>::type >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_type, boost::mpl::vector<const int&, const int&>::type >));

	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_pointer_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_reference_type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<0>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<1>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<2>::type, info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG
}

void test_boost_function(void)
{
	typedef boost::function<float(int, char)> now_foo_type;
	typedef boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef float(chk_foo_type)(int, char);
	typedef boost::add_pointer<chk_foo_type>::type chk_foo_ptr_type;
	typedef boost::add_reference<chk_foo_type>::type chk_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_volatile_type));

	BOOST_MPL_ASSERT((boost::is_same<info_type::result_type, float>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::class_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_type, boost::mpl::vector<int, char>::type >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_type, boost::mpl::vector<int, char>::type >));

	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_type, chk_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_pointer_type, chk_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_reference_type, chk_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<0>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<1>::type, char>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

void test_boost_function0(void)
{
	typedef boost::function0<float> now_foo_type;
	typedef boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_volatile_type));

	BOOST_MPL_ASSERT((boost::is_same<info_type::result_type, float>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::class_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_type, boost::mpl::vector<>::type >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_type, boost::mpl::vector<>::type >));

	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_pointer_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_reference_type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_size_type, boost::mpl::size_t<0> >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 0>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<0>::type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<1>::type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<2>::type, info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

void test_boost_function2(void)
{
	typedef boost::function2<float, float, float> now_foo_type;
	typedef boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_volatile_type));

	BOOST_MPL_ASSERT((boost::is_same<info_type::result_type, float>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::class_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_type, boost::mpl::vector< float, float>::type >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_type, boost::mpl::vector< float, float>::type >));

	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_pointer_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_reference_type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<0>::type, float>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<1>::type, float>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<2>::type, info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

// bind + functional

template<typename Handler, typename Nav_Handler>
void test_bind_functional_0(const Handler&, const Nav_Handler&)
{
	typedef Handler now_foo_type;
	typedef typename boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef Nav_Handler now_nav_foo_type;
	typedef typename boost::add_pointer<now_nav_foo_type>::type now_nav_foo_ptr_type;
	typedef typename boost::add_reference<now_nav_foo_type>::type now_nav_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, bool>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, std::less<int> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<const int&, const int&>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_nav_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_nav_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_nav_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 0>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG
}

template<typename Handler, typename Nav_Handler>
void test_bind_functional_1(const Handler&, const Nav_Handler&)
{

	typedef Handler now_foo_type;
	typedef typename boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef typename boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef Nav_Handler now_nav_foo_type;
	typedef typename boost::add_pointer<now_nav_foo_type>::type now_nav_foo_ptr_type;
	typedef typename boost::add_reference<now_nav_foo_type>::type now_nav_foo_ref_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT((typename info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((typename info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((typename info_type::is_volatile_type));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::result_type, bool>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::class_type, std::less<int> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_type, boost::mpl::vector<const int&, const int&>::type >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_type, boost::mpl::vector<const int&, const int&>::type >));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_type, now_nav_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_pointer_type, now_nav_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::native_foo_reference_type, now_nav_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<0>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<1>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<typename info_type::template arg<2>::type, typename info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG
}

struct non_callable { };

void test_not_callable_foo(void)
{
	typedef non_callable now_foo_type;

	typedef yggr::func::foo_t_info<now_foo_type> info_type;

	BOOST_MPL_ASSERT_NOT((info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_member_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_volatile_type));


	BOOST_MPL_ASSERT((boost::is_same<info_type::result_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::class_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_pointer_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_reference_type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_pointer_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_reference_type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_size_type, boost::mpl::size_t<0> >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_size_type, boost::mpl::size_t<0> >));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::arg_list_size>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::arg_holder_list_size>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<0>::type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<1>::type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<2>::type, info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG

}

struct callable
{
    bool operator()(const int&, const int&) const
    {
        return false;
    }
};

void test_callable_foo(void)
{
	typedef callable now_foo_type;

	typedef boost::add_pointer<now_foo_type>::type now_foo_ptr_type;
	typedef boost::add_reference<now_foo_type>::type now_foo_ref_type;

	typedef yggr::func::foo_t_info<callable> info_type;


	BOOST_MPL_ASSERT((info_type::is_callable_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_native_foo_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_member_foo_type));
	BOOST_MPL_ASSERT((info_type::is_const_type));
	BOOST_MPL_ASSERT_NOT((info_type::is_volatile_type));

	BOOST_MPL_ASSERT((boost::is_same<info_type::result_type, bool>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::class_type, now_foo_type >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_type, boost::mpl::vector<const int&, const int&>::type >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_type, boost::mpl::vector<const int&, const int&>::type >));

	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_type, now_foo_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_pointer_type, now_foo_ptr_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::foo_reference_type, now_foo_ref_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_pointer_type, info_type::null_type>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::native_foo_reference_type, info_type::null_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::type, now_foo_type>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_list_size_type, boost::mpl::size_t<2> >));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg_holder_list_size_type, boost::mpl::size_t<2> >));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_callable>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_native_foo>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_member_foo>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::is_const>));
	BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<info_type::is_volatile>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_list_size == 2>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<info_type::arg_holder_list_size == 2>));

	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<0>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<1>::type, const int&>));
	BOOST_MPL_ASSERT((boost::is_same<info_type::arg<2>::type, info_type::null_type>));

#ifdef _DEBUG
	std::cout << info_type() << std::endl;
#endif // _DEBUG
}

int main(int argc, char* argv[])
{
	test_not_foo(0);

	test0(&foo0);
	test1(&foo1);
	test2(&foo2);

	ne_test0(&nefoo0);
	//ne_test1(&nefoo1);
	//ne_test2(&nefoo2);

	m_test0(&A::foo0);
	m_test1(&A::foo1);
	m_test2(&A::foo2);

	cm_test0(&A::cfoo0);
	cm_test1(&A::cfoo1);
	cm_test2(&A::cfoo2);

	vm_test0(&A::vfoo0);
	vm_test1(&A::vfoo1);
	vm_test2(&A::vfoo2);

	cvm_test0(&A::cvfoo0);
	cvm_test1(&A::cvfoo1);
	cvm_test2(&A::cvfoo2);

	b_test2_1(boost::bind(&foo2, _1, _2), &foo2);
	b_test2_2(boost::bind(&foo2, 10, 20), &foo2);

	A a;
	bm_test2_1(boost::bind(&A::foo2, &a, 10, 20), &A::foo2);
	bm_test2_2(boost::bind(&A::foo2, _1, _2, _3), &A::foo2);
	bm_test2_3(boost::bind(&A::foo2, _3, _2, _1), &A::foo2);

	bcm_test2_1(boost::bind(&A::cfoo2, &a, 10, 20), &A::cfoo2);
	bcm_test2_2(boost::bind(&A::cfoo2, _1, _2, _3), &A::cfoo2);
	bcm_test2_3(boost::bind(&A::cfoo2, _3, _2, _1), &A::cfoo2);

	// !!!!boost::bind not_support volatile and const volatile!!!
	//a.vfoo2(10, 20);
	//boost::bind(&A::vfoo2, &a, 10, 20)();
	//a.cvfoo2(10, 20);
	//boost::bind(&A::cvfoo2, &a, 10, 20)();

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	test_unary_function();
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	test_unary_function_extends();

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	test_binary_function();
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	test_binary_function_extends();

	test_boost_function();

	test_boost_function0();
	test_boost_function2();

	test_bind_functional_0(boost::bind(std::less<int>(), 1, 2), std::less<int>());
	test_bind_functional_1(boost::bind(std::less<int>(), _1, _2), std::less<int>());

	test_not_callable_foo();
	test_callable_foo();

	wait_any_key(argc, argv);
	return 0;
}
