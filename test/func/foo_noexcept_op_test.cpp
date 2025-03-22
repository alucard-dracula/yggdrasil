//foo_noexcept_op_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/func/is_noexcept.hpp>
#include <yggr/func/remove_noexcept.hpp>
#include <yggr/func/add_noexcept.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

void foo0(void)
{
}

void foo1(int, ...)
{
}

void nefoo0(void) YGGR_NOEXCEPT
{
}

void nefoo1(int, ...) YGGR_NOEXCEPT
{
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

public:
	void foo0(void)
	{
	}

	void foo1(int, ...)
	{
	}

	void cfoo0(void) const
	{
	}

	void cfoo1(int, ...) const
	{
	}

	void vfoo0(void) volatile
	{
	}

	void vfoo1(int, ...) volatile
	{
	}

	void nefoo0(void) YGGR_NOEXCEPT 
	{
	}

	void nefoo1(int, ...) YGGR_NOEXCEPT
	{
	}

	void cvfoo0(void) const volatile
	{
	}

	void cvfoo1(int, ...) const volatile
	{
	}

	void cnefoo0(void) const YGGR_NOEXCEPT
	{
	}

	void cnefoo1(int, ...) const YGGR_NOEXCEPT
	{
	}

	void vnefoo0(void) volatile YGGR_NOEXCEPT
	{
	}

	void vnefoo1(int, ...) volatile YGGR_NOEXCEPT
	{
	}

	void cvnefoo0(void) const volatile YGGR_NOEXCEPT
	{
	}

	void cvnefoo1(int, ...) const volatile YGGR_NOEXCEPT
	{
	}
};


template<typename T>
void test_is_noexcept(void)
{
	// grobal foo
	{
		typedef YGGR_TYPEOF(&foo0) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&foo1) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&nefoo0) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&nefoo1) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	// member foo

	{
		typedef YGGR_TYPEOF(&A::foo0) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::foo1) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::cfoo0) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::cfoo1) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::vfoo0) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::vfoo1) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::nefoo0) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::nefoo1) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::cvfoo0) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::cvfoo1) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::cnefoo0) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::cnefoo1) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::vnefoo0) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::vnefoo1) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::cvnefoo0) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::cvnefoo1) foo_ptr_type;
		typedef yggr::func::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}
}

// test_add_and_remove_noexcept

template<typename T>
void test_add_and_remove_noexcept(void)
{
	// grobal foo
	{
		typedef YGGR_TYPEOF(&foo0) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&foo1) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&nefoo0) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&nefoo1) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	// member foo
	{
		typedef YGGR_TYPEOF(&A::foo0) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::foo1) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::cfoo0) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cfoo1) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::vfoo0) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::vfoo1) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::nefoo0) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::nefoo1) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvfoo0) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvfoo1) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cnefoo0) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cnefoo1) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::vnefoo0) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::vnefoo1) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvnefoo0) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvnefoo1) foo_ptr_type;
		typedef yggr::func::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef yggr::func::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((yggr::func::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((yggr::func::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}
}

#if (!(BOOST_VERSION < 106600) && (YGGR_CPP_VERSION >= YGGR_CPP_VER_11))


#include <boost/callable_traits/remove_noexcept.hpp>
#include <boost/callable_traits/is_noexcept.hpp>
#include <boost/callable_traits/add_noexcept.hpp>

template<typename T>
void test_boost_is_noexcept(void)
{
	// grobal foo
	{
		typedef YGGR_TYPEOF(&foo0) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&foo1) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&nefoo0) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&nefoo1) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	// member foo

	{
		typedef YGGR_TYPEOF(&A::foo0) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::foo1) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::cfoo0) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::cfoo1) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::vfoo0) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::vfoo1) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::nefoo0) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::nefoo1) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::cvfoo0) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::cvfoo1) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
		BOOST_MPL_ASSERT_NOT((check_type));
	}

	{
		typedef YGGR_TYPEOF(&A::cnefoo0) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::cnefoo1) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::vnefoo0) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::vnefoo1) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::cvnefoo0) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::cvnefoo1) foo_ptr_type;
		typedef boost::callable_traits::is_noexcept<foo_ptr_type> check_type;
#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((check_type));
#else
		BOOST_MPL_ASSERT((check_type));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}
}

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

// cpp 11-14 not support noexcept sign, so boost::callable_traits::add_noexcept not supported

template<typename T>
void test_boost_add_and_remove_noexcept(void)
{
	// grobal foo
	{
		typedef YGGR_TYPEOF(&foo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

	}

	{
		typedef YGGR_TYPEOF(&foo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
	}

	{
		typedef YGGR_TYPEOF(&nefoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&nefoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	// member foo
	{
		typedef YGGR_TYPEOF(&A::foo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
	}

	{
		typedef YGGR_TYPEOF(&A::foo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
	}

	{
		typedef YGGR_TYPEOF(&A::cfoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
	}

	{
		typedef YGGR_TYPEOF(&A::cfoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
	}

	{
		typedef YGGR_TYPEOF(&A::vfoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
	}

	{
		typedef YGGR_TYPEOF(&A::vfoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

	}

	{
		typedef YGGR_TYPEOF(&A::nefoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::nefoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvfoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

	}

	{
		typedef YGGR_TYPEOF(&A::cvfoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

	}

	{
		typedef YGGR_TYPEOF(&A::cnefoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cnefoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::vnefoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::vnefoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvnefoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvnefoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}
}

#else

template<typename T>
void test_boost_add_and_remove_noexcept(void)
{
	// grobal foo
	{
		typedef YGGR_TYPEOF(&foo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&foo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&nefoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&nefoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	// member foo
	{
		typedef YGGR_TYPEOF(&A::foo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::foo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	}

	{
		typedef YGGR_TYPEOF(&A::cfoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cfoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::vfoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::vfoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::nefoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::nefoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvfoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvfoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cnefoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cnefoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::vnefoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::vnefoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvnefoo0) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}

	{
		typedef YGGR_TYPEOF(&A::cvnefoo1) foo_ptr_type;
		typedef boost::callable_traits::remove_noexcept<foo_ptr_type>::type foo_ptr2_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr_type>));
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr2_type>));
		BOOST_MPL_ASSERT_NOT((boost::is_same<foo_ptr_type, foo_ptr2_type>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

		typedef boost::callable_traits::add_noexcept<foo_ptr2_type>::type foo_ptr3_type;

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
		BOOST_MPL_ASSERT_NOT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#else
		BOOST_MPL_ASSERT((boost::callable_traits::is_noexcept<foo_ptr3_type>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	}
}

#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
#endif // #if !(BOOST_VERSION < 106600)

int main(int argc, char* argv[])
{
	test_is_noexcept<int>();
	test_add_and_remove_noexcept<int>();

#if (!(BOOST_VERSION < 106600) && (YGGR_CPP_VERSION >= YGGR_CPP_VER_11))
	test_boost_is_noexcept<int>();
	test_boost_add_and_remove_noexcept<int>();
#endif // #if !(BOOST_VERSION < 106600

	wait_any_key(argc, argv);
	return 0;
}
