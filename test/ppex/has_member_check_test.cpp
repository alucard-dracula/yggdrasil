//has_member_check_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

//#include <yggr/ppex/pp_debug.hpp>
#include <yggr/ppex/has_member_check.hpp>

#include <cassert>
#include <vector>
#include <iostream>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

struct A
{
	typedef int value_type;

	void foo(void) const {};
	int foo(int) { return 0;}
	int foo(int, int) {return 0;}
	int foo(int, int, int) {return 0;}

	static void s_foo(void) {};
};

struct B
	: public A
{
	void foo(int, int, int, int) {}
};

struct C
{
	template<typename T>
	void foo(const T&)
	{
	}
};

// non-static

YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN()

#ifdef __VA_ARGS__
#	define _FOO_PARAMS() YGGR_PP_CONCAT(void) // c99 version
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS) // not c99 version
#	define  _ARGS void
#endif // __VA_ARGS__

YGGR_PP_HAS_MEMBER_FOO_DEF(foo, T, void, foo, YGGR_PP_SYMBOL_BOOL_TRUE(), _FOO_PARAMS())

YGGR_PP_HAS_ANY_MEMBER_FOO_DEF(foo, foo)

#undef _FOO_PARAMS
#ifndef __VA_ARGS__
#	undef _ARGS
#endif //

#ifdef __VA_ARGS__
#	define _FOO_PARAMS() YGGR_PP_CONCAT(int)
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS)
#	define _ARGS int
#endif // __VA_ARGS__

YGGR_PP_HAS_MEMBER_FOO_DEF(foo2, T, int, foo, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())

YGGR_PP_HAS_ANY_MEMBER_FOO_DEF(foo2, foo2)

#undef _FOO_PARAMS
#ifndef __VA_ARGS__
#	undef _ARGS
#endif // __VA_ARGS__

#ifdef __VA_ARGS__
#	define _FOO_PARAMS() YGGR_PP_CONCAT(int, int)
#else
#	define _FOO_PARAMS() YGGR_PP_CONCAT(_ARGS)
#	define _ARGS int, int
#endif // __VA_ARGS__

YGGR_PP_HAS_MEMBER_FOO_DEF(foo3, T, int, foo, YGGR_PP_SYMBOL_BOOL_FALSE(), _FOO_PARAMS())

#undef _FOO_PARAMS
#ifndef __VA_ARGS__
#	undef _ARGS
#endif // __VA_ARGS__

#ifdef __VA_ARGS__

YGGR_PP_HAS_MEMBER_FOO_DEF(begin, T, typename T::iterator, begin, 0, YGGR_PP_CONCAT(void)) // c99 simplified grammar

YGGR_PP_HAS_MEMBER_FOO_DEF(cbegin, T, typename T::const_iterator, begin, 1, YGGR_PP_CONCAT(void))

#endif // __VA_ARGS__

YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_END()

// static

YGGR_PP_HAS_STATIC_MEMBER_FOO_NAMESPACE_BEGIN()

YGGR_PP_HAS_STATIC_MEMBER_FOO_DEF(s_foo, void, s_foo, YGGR_PP_CONCAT(void))

YGGR_PP_HAS_STATIC_MEMBER_FOO_NAMESPACE_END()

// member_type

YGGR_PP_HAS_MEMBER_TYPE_NAMESPACE_BEGIN()

YGGR_PP_HAS_MEMBER_TYPE_DEF(value_t, value_type)

YGGR_PP_HAS_MEMBER_TYPE_NAMESPACE_END()

// member var

YGGR_PP_HAS_MEMBER_VAR_NAMESPACE_BEGIN()

YGGR_PP_HAS_MEMBER_VAR_DEF(n, T, int, n)

YGGR_PP_HAS_ANY_MEMBER_VAR_DEF(n, n)

YGGR_PP_HAS_STATIC_MEMBER_VAR_DEF(n, int, n)

YGGR_PP_HAS_MEMBER_VAR_NAMESPACE_END()

struct has_var_n
{
	int n;
};

struct has_s_var_n
{
	static int n;
};

struct not_has_any_n
{
};

int main(int argc, char* argv[])
{
	typedef std::vector<int> vt_type;

	{
		yggr_test_assert(!(YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(foo, int)));
		yggr_test_assert((YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(foo, A)));
		yggr_test_assert(!(YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(foo, B)));

		yggr_test_assert(!(YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_VALUE(foo, int)));
		yggr_test_assert(YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_VALUE(foo, A));
		yggr_test_assert(YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE_VALUE(foo, B));


		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_FOO_TYPE(foo, int)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_MEMBER_FOO_TYPE(foo, A)));
		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_FOO_TYPE(foo, B)));

		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(foo, int)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(foo, A)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(foo, B)));

		BOOST_MPL_ASSERT((YGGR_PP_HAS_ANY_MEMBER_FOO_TYPE(foo, C)));
	}

	{
		yggr_test_assert(!(YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(foo2, int)));
		yggr_test_assert((YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(foo2, A)));

		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_FOO_TYPE(foo2, int)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_MEMBER_FOO_TYPE(foo2, A)));
	}

	{
		yggr_test_assert(!(YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(foo3, int)));
		yggr_test_assert((YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(foo3, A)));

		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_FOO_TYPE(foo3, int)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_MEMBER_FOO_TYPE(foo3, A)));
	}

#ifdef __VA_ARGS__
	{
		yggr_test_assert(!(YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(begin, int)));
		yggr_test_assert((YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(begin, vt_type)));

		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_FOO_TYPE(begin, int)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_MEMBER_FOO_TYPE(begin, vt_type)));
	}

	{
		yggr_test_assert(!(YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(cbegin, int)));
		yggr_test_assert((YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(cbegin, vt_type)));

		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_FOO_TYPE(cbegin, int)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_MEMBER_FOO_TYPE(cbegin, vt_type)));
	}

#endif // __VA_ARGS__

#ifdef __VA_ARGS__
	{
		yggr_test_assert(!(YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_VALUE(s_foo, int)));
		yggr_test_assert((YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE_VALUE(s_foo, A)));

		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE(s_foo, int)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_STATIC_MEMBER_FOO_TYPE(s_foo, A)));
	}
#endif // __VA_ARGS__

	{
	    yggr_test_assert(!(YGGR_PP_HAS_MEMBER_FOO_TYPE_VALUE(foo, B)));
		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_FOO_TYPE(foo, B)));
	}

	//std::cout << (YGGR_PP_DEBUG(YGGR_PP_HAS_MEMBER_FOO_DEF(foo, T, void, foo, 1, void))) << std::endl;

	{
		yggr_test_assert(!(YGGR_PP_HAS_MEMBER_T_TYPE_VALUE(value_t, int)));
		yggr_test_assert((YGGR_PP_HAS_MEMBER_T_TYPE_VALUE(value_t, A)));

		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_T_TYPE(value_t, int)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_MEMBER_T_TYPE(value_t, A)));

	}

	{
		yggr_test_assert(YGGR_PP_HAS_MEMBER_VAR_TYPE_VALUE(n, has_var_n));
		yggr_test_assert(!YGGR_PP_HAS_MEMBER_VAR_TYPE_VALUE(n, int));
		yggr_test_assert(!YGGR_PP_HAS_MEMBER_VAR_TYPE_VALUE(n, has_s_var_n));
		yggr_test_assert(!YGGR_PP_HAS_MEMBER_VAR_TYPE_VALUE(n, not_has_any_n));

		BOOST_MPL_ASSERT((YGGR_PP_HAS_MEMBER_VAR_TYPE(n, has_var_n)));
		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_VAR_TYPE(n, int)));
		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_VAR_TYPE(n, has_s_var_n)));
		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_MEMBER_VAR_TYPE(n, not_has_any_n)));
	}

	{
		yggr_test_assert(YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE_VALUE(n, has_var_n));
		yggr_test_assert(!YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE_VALUE(n, int));
		yggr_test_assert(YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE_VALUE(n, has_s_var_n));
		yggr_test_assert(!YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE_VALUE(n, not_has_any_n));

		BOOST_MPL_ASSERT((YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE(n, has_var_n)));
		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE(n, int)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE(n, has_s_var_n)));
		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_ANY_MEMBER_VAR_TYPE(n, not_has_any_n)));
	}

	{
		yggr_test_assert(!YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE_VALUE(n, has_var_n));
		yggr_test_assert(!YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE_VALUE(n, int));
		yggr_test_assert(YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE_VALUE(n, has_s_var_n));
		yggr_test_assert(!YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE_VALUE(n, not_has_any_n));

		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE(n, has_var_n)));
		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE(n, int)));
		BOOST_MPL_ASSERT((YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE(n, has_s_var_n)));
		BOOST_MPL_ASSERT_NOT((YGGR_PP_HAS_STATIC_MEMBER_VAR_TYPE(n, not_has_any_n)));
	}

	wait_any_key(argc, argv);
	return 0;
}

