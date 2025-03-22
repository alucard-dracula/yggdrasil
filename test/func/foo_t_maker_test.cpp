//foo_type_info_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

//#include <yggr/ppex/pp_debug.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/func/foo_t_maker.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <cassert>
#include <string>
#include <typeinfo>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

int foo0(void)
{
	std::cout << "foo0" << std::endl;
	return 0;
}

int foo1(const int&)
{
	std::cout << "foo1" << std::endl;
	return 0;
}

const int& foo2(const int& n, int&)
{
	std::cout << "foo2" << std::endl;
	return n;
}

int foo3(int, int, int)
{
	std::cout << "foo3" << std::endl;
	return 0;
}

void test_foo_type_info_0(void)
{
	typedef yggr::func::foo_t_maker<int> foo_type_info_type;
	foo_type_info_type::type pfoo = &foo0;
	foo_type_info_type::result_type ret = (*pfoo)();
	yggr_test_assert(ret == 0);
	yggr_test_assert(typeid(foo_type_info_type::null_type) == typeid(foo_type_info_type::arg<0>::type));
	yggr_test_assert(foo_type_info_type::arg_list_size == 0);
	yggr_test_assert(foo_type_info_type::is_const == 0);
	yggr_test_assert(foo_type_info_type::is_volatile == 0);
}

void test_foo_type_info_1(void)
{
	typedef yggr::func::foo_t_maker<int, const int&> foo_type_info_type;
	foo_type_info_type::type pfoo = &foo1;
	foo_type_info_type::result_type ret = (*pfoo)(10);
	yggr_test_assert(ret == 0);
	yggr_test_assert(typeid(const int&) == typeid(foo_type_info_type::arg<0>::type));
	yggr_test_assert(foo_type_info_type::arg_list_size == 1);
	yggr_test_assert(foo_type_info_type::is_const == 0);
	yggr_test_assert(foo_type_info_type::is_volatile == 0);
}

void test_foo_type_info_2(void)
{
	typedef yggr::func::foo_t_maker<const int&, const int&, int&> foo_type_info_type;
	int m = 10;
	foo_type_info_type::type pfoo = &foo2;
	foo_type_info_type::result_type ret = (*pfoo)(m, m);
	yggr_test_assert(ret == m);
	yggr_test_assert(typeid(int&) == typeid(foo_type_info_type::arg<1>::type));
	yggr_test_assert(foo_type_info_type::arg_list_size == 2);
	yggr_test_assert(foo_type_info_type::is_const == 0);
	yggr_test_assert(foo_type_info_type::is_volatile == 0);
}

void test_foo_type_info_3(void)
{
	typedef yggr::func::foo_t_maker<int, int, int, int> foo_type_info_type;
	foo_type_info_type::type pfoo = &foo3;
	foo_type_info_type::result_type ret = (*pfoo)(10, 10, 10);
	yggr_test_assert(ret == 0);
	yggr_test_assert(typeid(int) == typeid(foo_type_info_type::arg<2>::type));
	yggr_test_assert(foo_type_info_type::arg_list_size == 3);
	yggr_test_assert(foo_type_info_type::is_const == 0);
	yggr_test_assert(foo_type_info_type::is_volatile == 0);
}

struct A
{
	static int s_foo(int)
	{
		std::cout << "s_foo" << std::endl;
		return 0;
	}

	int foo(void)
	{
		std::cout << "A::foo" << std::endl;
		return 0;
	}

	int cfoo2(int, float) const
	{
		std::cout << "A:cfoo2" << std::endl;
		return 0;
	}

	int vfoo2(int, float) volatile
	{
		std::cout << "A:vfoo2" << std::endl;
		return 0;
	}

	int cvfoo2(int, float) const volatile
	{
		std::cout << "A:cvfoo2" << std::endl;
		return 0;
	}

	template<typename T1, typename T2, typename T3>
	void rvref_foo(YGGR_PP_FOO_ANYREF_PARAMS_TYPES(_, 1, _), YGGR_PP_FOO_ANYREF_PARAMS_TYPES(_, 2, _), YGGR_PP_FOO_ANYREF_PARAMS_TYPES(_, 3, _)) const
	{
	}
};

void test_s_foo_type_info_1(void)
{
	typedef yggr::func::foo_t_maker<int, int> foo_type_info_type;
	foo_type_info_type::type pfoo = &A::s_foo;
	foo_type_info_type::result_type ret = (*pfoo)(10);
	yggr_test_assert(ret == 0);
	yggr_test_assert(typeid(int) == typeid(foo_type_info_type::arg<0>::type));
	yggr_test_assert(foo_type_info_type::arg_list_size == 1);
	yggr_test_assert(foo_type_info_type::is_const == 0);
	yggr_test_assert(foo_type_info_type::is_volatile == 0);
}

void test_m_foo_type_info_0(void)
{
	typedef yggr::func::member_foo_t_maker<int, A, 0> foo_type_info_type;
	foo_type_info_type::type pfoo = &A::foo;
	A a;
	foo_type_info_type::result_type ret = (a.*pfoo)();
	yggr_test_assert(ret == 0);
	yggr_test_assert(typeid(foo_type_info_type::null_type) == typeid(foo_type_info_type::arg<0>::type));
	yggr_test_assert(foo_type_info_type::arg_list_size == 0);
	yggr_test_assert(foo_type_info_type::is_const == 0);
	yggr_test_assert(foo_type_info_type::is_volatile == 0);
}

void test_m_cfoo_type_info_2(void)
{
	typedef yggr::func::member_foo_t_maker<int, A, yggr::func::cv_mark_t::E_cv_const, int, float> foo_type_info_type;
	foo_type_info_type::type pfoo = &A::cfoo2;
	A a;
	foo_type_info_type::result_type ret = (a.*pfoo)(10, 20.0f);
	yggr_test_assert(ret == 0);
	yggr_test_assert(typeid(float) == typeid(foo_type_info_type::arg<1>::type));
	yggr_test_assert(foo_type_info_type::arg_list_size == 2);
	yggr_test_assert(foo_type_info_type::is_const == 1);
	yggr_test_assert(foo_type_info_type::is_volatile == 0);
}

void test_m_vfoo_type_info_2(void)
{
	typedef yggr::func::member_foo_t_maker<int, A, yggr::func::cv_mark_t::E_cv_volatile, int, float> foo_type_info_type;
	foo_type_info_type::type pfoo = &A::vfoo2;
	A a;
	foo_type_info_type::result_type ret = (a.*pfoo)(10, 20.0f);
	yggr_test_assert(ret == 0);
	yggr_test_assert(typeid(float) == typeid(foo_type_info_type::arg<1>::type));
	yggr_test_assert(foo_type_info_type::arg_list_size == 2);
	yggr_test_assert(foo_type_info_type::is_const == 0);
	yggr_test_assert(foo_type_info_type::is_volatile == 1);
}

void test_m_cvfoo_type_info_2(void)
{
	typedef yggr::func::member_foo_t_maker<int, A, yggr::func::cv_mark_t::E_cv_const_volatile, int, float> foo_type_info_type;
	foo_type_info_type::type pfoo = &A::cvfoo2;
	A a;
	foo_type_info_type::result_type ret = (a.*pfoo)(10, 20.0f);
	yggr_test_assert(ret == 0);
	yggr_test_assert(typeid(float) == typeid(foo_type_info_type::arg<1>::type));
	yggr_test_assert(foo_type_info_type::arg_list_size == 2);
	yggr_test_assert(foo_type_info_type::is_const == 1);
	yggr_test_assert(foo_type_info_type::is_volatile == 1);
}

template<typename T1, typename T2, typename T3>
void test_m_rvref_foo_type_info(YGGR_PP_FOO_ANYREF_PARAMS_TYPES(_, 1, _) a,
									YGGR_PP_FOO_ANYREF_PARAMS_TYPES(_, 2, _) b,
									YGGR_PP_FOO_ANYREF_PARAMS_TYPES(_, 3, _) c)
{
	typedef
		yggr::func::member_foo_t_maker
		<
			void,
			A,
			static_cast<yggr::u32>(yggr::func::cv_mark_t::E_cv_const),
			YGGR_PP_FOO_ANYREF_PARAMS_TYPES(_, 1, _),
			YGGR_PP_FOO_ANYREF_PARAMS_TYPES(_, 2, _),
			YGGR_PP_FOO_ANYREF_PARAMS_TYPES(_, 3, _)
		> foo_type_info_type;
	typedef typename foo_type_info_type::type foo_type;

	std::cout << typeid(foo_type).name() << std::endl;
	std::cout << typeid(&A::rvref_foo<T1, T2, T3>).name() << std::endl;

	foo_type pfoo = static_cast<foo_type>(&A::rvref_foo<T1, T2, T3>);

	A d;
	(d.*pfoo)(boost::forward<T1>(a), boost::forward<T2>(b), boost::forward<T3>(c));
}

int main(int argc, char* argv[])
{
	test_foo_type_info_0();
	test_foo_type_info_1();
	test_foo_type_info_2();
	test_foo_type_info_3();

	test_s_foo_type_info_1();
	test_m_foo_type_info_0();
	test_m_cfoo_type_info_2();
	test_m_vfoo_type_info_2();
	test_m_cvfoo_type_info_2();


	{
		std::string str1, str2;
		int n = 0;
		test_m_rvref_foo_type_info(n, n, std::string("abc"));
	}

	wait_any_key(argc, argv);
	return 0;
}
