// member_var_modify_helper_test.cpp

#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/container/array.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/utility/member_var_modify_helper.hpp>

// test base

template<typename T>
struct member_wrapper
{
public:
	typedef T member_type;

private:
	typedef member_wrapper this_type;

public:
	typedef
		yggr::member_var_modify_helper
		<
			this_type,
			member_type,
			member_type,
			const member_type&
		> var_m_modify_helper_type;

public:
	member_wrapper(void)
		: _m()
	{
	}

	member_wrapper(const member_type& n)
		: _m(n)
	{
	}

	member_wrapper(const this_type& right)
		: _m(right._m)
	{
	}

	~member_wrapper(void)
	{
	}

public:
	inline this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_m = right._m;
		return *this;
	}

protected:
	const member_type& pro_get_member(void) const
	{
		return _m;
	}

	void pro_set_member(const member_type& val)
	{
		_m = val;
	}

public:
	const member_type& var_m(void) const
	{
		return _m;
	}

	var_m_modify_helper_type var_m(void)
	{
		return 
			var_m_modify_helper_type(
				*this, _m,
				&this_type::pro_set_member,
				&this_type::pro_get_member);
	}

public: //it is more convenient to use public in testing
	member_type _m;
};

void test_base(void)
{
	typedef member_wrapper<int> now_test_type;

	{
		now_test_type now_test;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(now_test_ref.var_m() == 0);
		yggr_test_assert(now_test_cref.var_m() == 0);

		now_test_ref.var_m() = 10;
		yggr_test_assert(now_test_ref.var_m() == 10);
		yggr_test_assert(now_test_cref.var_m() == 10);
	}

	std::cout << "test_base succeed" << std::endl;
}

void test_op_index_member_access(void)
{
	{
		typedef member_wrapper<int*> now_test_type;
		typedef now_test_type::var_m_modify_helper_type var_m_modify_helper_type;
		BOOST_MPL_ASSERT((yggr::has_operator_index_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT_NOT((yggr::has_operator_pointer_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_callable<var_m_modify_helper_type>));

		int arr[10] = {0};

		now_test_type now_test;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(now_test_ref.var_m() == 0);
		yggr_test_assert(now_test_ref.var_m() == static_cast<int*>(0));
		yggr_test_assert(now_test_cref.var_m() == 0);

		now_test_ref.var_m() = 0;
		now_test_ref.var_m() = arr;
		now_test_ref.var_m() = static_cast<int*>(arr);
		yggr_test_assert(now_test_ref.var_m() == arr);
		yggr_test_assert(now_test_cref.var_m() == arr);

		(now_test_ref.var_m())[0] = 10;
		yggr_test_assert((now_test_ref.var_m())[0] == 10);
		yggr_test_assert((now_test_cref.var_m())[0] == 10);
		yggr_test_assert((now_test_ref.var_m())[1] == 0);
		yggr_test_assert((now_test_cref.var_m())[1] == 0);
	}

	{
		typedef yggr::array<int, 10> array_type;
		typedef member_wrapper<array_type> now_test_type;

		typedef now_test_type::var_m_modify_helper_type var_m_modify_helper_type;
		BOOST_MPL_ASSERT((yggr::has_operator_index_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT_NOT((yggr::has_operator_pointer_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_callable<var_m_modify_helper_type>));

		now_test_type now_test;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		(now_test_ref.var_m())[0] = 10;
		yggr_test_assert((now_test_ref.var_m())[0] == 10);
		yggr_test_assert((now_test_cref.var_m())[0] == 10);
		yggr_test_assert((now_test_ref.var_m())[1] == 0);
		yggr_test_assert((now_test_cref.var_m())[1] == 0);
	}

	{
		typedef yggr::vector<int> vt_type;
		typedef member_wrapper<vt_type> now_test_type;

		typedef now_test_type::var_m_modify_helper_type var_m_modify_helper_type;
		BOOST_MPL_ASSERT((yggr::has_operator_index_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT_NOT((yggr::has_operator_pointer_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_callable<var_m_modify_helper_type>));

		now_test_type now_test;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		now_test_ref.var_m().modify().resize(10, 0);
		(now_test_ref.var_m())[0] = 10;
		yggr_test_assert((now_test_ref.var_m())[0] == 10);
		yggr_test_assert((now_test_cref.var_m())[0] == 10);
		yggr_test_assert((now_test_ref.var_m())[1] == 0);
		yggr_test_assert((now_test_cref.var_m())[1] == 0);
	}

	std::cout << "test_op_index_member_access succeed" << std::endl;
}

struct c_pma
{
	int d1;
	int d2;
};


void test_op_pointer_member_access(void)
{
	{
		typedef member_wrapper<c_pma*> now_test_type;
		typedef now_test_type::var_m_modify_helper_type var_m_modify_helper_type;
		BOOST_MPL_ASSERT((yggr::has_operator_index_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT((yggr::has_operator_pointer_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_callable<var_m_modify_helper_type>));

		c_pma pma = {0, 0};

		now_test_type now_test;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(now_test_ref.var_m() == 0);
		yggr_test_assert(now_test_ref.var_m() == static_cast<c_pma*>(0));
		yggr_test_assert(now_test_cref.var_m() == 0);

		now_test_ref.var_m() = 0;
		now_test_ref.var_m() = &pma;
		yggr_test_assert(now_test_ref.var_m() == &pma);
		yggr_test_assert(now_test_cref.var_m() == &pma);

		(now_test_ref.var_m())->d1 = 10;
		(now_test_ref.var_m())->d2 = 20;
		yggr_test_assert((now_test_ref.var_m())->d1 == 10);
		yggr_test_assert((now_test_ref.var_m())->d2 == 20);
		yggr_test_assert((now_test_cref.var_m())->d1 == 10);
		yggr_test_assert((now_test_cref.var_m())->d2 == 20);
	}

	{
		typedef yggr::shared_ptr<c_pma> pma_ptr_type;
		typedef member_wrapper<pma_ptr_type> now_test_type;

		typedef now_test_type::var_m_modify_helper_type var_m_modify_helper_type;
		BOOST_MPL_ASSERT((yggr::has_operator_index_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT((yggr::has_operator_pointer_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT_NOT((yggr::func::is_callable<var_m_modify_helper_type>));

		pma_ptr_type pst(new c_pma);
		pst->d1 = 0;
		pst->d2 = 0;

		pst[0]; // return voi
		
		now_test_type now_test;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(now_test_ref.var_m() == pma_ptr_type());

		now_test_ref.var_m() = pma_ptr_type();
		now_test_ref.var_m() = pst;
		yggr_test_assert(now_test_ref.var_m() == pst);
		yggr_test_assert(now_test_cref.var_m() == pst);

		(now_test_ref.var_m())->d1 = 10;
		(now_test_ref.var_m())->d2 = 20;
		yggr_test_assert((now_test_ref.var_m())->d1 == 10);
		yggr_test_assert((now_test_ref.var_m())->d2 == 20);
		yggr_test_assert((now_test_cref.var_m())->d1 == 10);
		yggr_test_assert((now_test_cref.var_m())->d2 == 20);
	}

	std::cout << "test_op_pointer_member_access succeed" << std::endl;
}

int foo(void)
{
	return 10;
}

struct c_foo
{
	// not support overload
	//int operator()(void)
	//{
	//	return 10;
	//}

	int operator()(void) const
	{
		return 20;
	}
};

void test_op_call(void)
{
	{
		typedef int (*foo_ptr_type)(void);

		typedef member_wrapper<foo_ptr_type> now_test_type;
		typedef now_test_type::var_m_modify_helper_type var_m_modify_helper_type;
		BOOST_MPL_ASSERT_NOT((yggr::has_operator_index_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT_NOT((yggr::has_operator_pointer_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT((yggr::func::is_callable<var_m_modify_helper_type>));

		now_test_type now_test;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(now_test_ref.var_m() == 0);

		now_test_ref.var_m() = 0;
		now_test_ref.var_m() = &foo;
		yggr_test_assert(now_test_ref.var_m() == &foo);
		yggr_test_assert(now_test_cref.var_m() == &foo);

		yggr_test_assert((now_test_ref.var_m())() == 10);
		yggr_test_assert((now_test_cref.var_m())() == 10);
	}

	{
		typedef member_wrapper<c_foo> now_test_type;
		typedef now_test_type::var_m_modify_helper_type var_m_modify_helper_type;
		BOOST_MPL_ASSERT_NOT((yggr::has_operator_index_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT_NOT((yggr::has_operator_pointer_member_access<var_m_modify_helper_type>));
		BOOST_MPL_ASSERT((yggr::func::is_callable<var_m_modify_helper_type>));

		now_test_type now_test;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((now_test_ref.var_m())() == 20);
		yggr_test_assert((now_test_cref.var_m())() == 20);
	}

	std::cout << "test_op_call succeed" << std::endl;
}

void test_non_member_operator(void)
{
	//positive
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(+now_test_ref.var_m() == 10);
		yggr_test_assert(+now_test_cref.var_m() == 10);
	}

	// negate
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(-now_test_ref.var_m() == -10);
		yggr_test_assert(-now_test_cref.var_m() == -10);
	}

	//dereference
	{
		typedef member_wrapper<yggr::s32*> now_test_type;

		yggr::s32 n = 10;
		now_test_type now_test(&n);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(*(now_test_ref.var_m()) == 10);
		yggr_test_assert(*(now_test_cref.var_m()) == 10);
	}

	//addressof
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(&(now_test_ref.var_m()) == &(now_test_cref.var_m()));
		yggr_test_assert(&(now_test_ref.var_m()) == &(now_test_cref._m));
		yggr_test_assert(*(&(now_test_ref.var_m())) == 10);
		yggr_test_assert(*(&(now_test_cref.var_m())) == 10);
	}

	// increment
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(++now_test_ref.var_m() == 11);
		yggr_test_assert(now_test_cref.var_m() == 11);

		yggr_test_assert(now_test_ref.var_m()++ == 11);
		yggr_test_assert(now_test_cref.var_m() == 12);
	}

	// decrement
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(--now_test_ref.var_m() == 9);
		yggr_test_assert(now_test_cref.var_m() == 9);

		yggr_test_assert(now_test_ref.var_m()-- == 9);
		yggr_test_assert(now_test_cref.var_m() == 8);
	}

	//+
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(10 + now_test_ref.var_m() == 20);
		yggr_test_assert(10 + now_test_cref.var_m() == 20);

		yggr_test_assert(now_test_ref.var_m() + 10 == 20);
		yggr_test_assert(now_test_cref.var_m() + 10 == 20);

		yggr_test_assert(now_test_cref.var_m() + now_test_ref.var_m() == 20);
		yggr_test_assert(now_test_ref.var_m() + now_test_cref.var_m() == 20);
		yggr_test_assert(now_test_ref.var_m() + now_test_ref.var_m() == 20);
	}

	//+=
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		yggr::s32 n = 10;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((n += now_test_ref.var_m()) == 20);
		yggr_test_assert((n += now_test_cref.var_m()) == 30);

		yggr_test_assert((now_test_ref.var_m() += 10) == 20);
		yggr_test_assert(now_test_cref.var_m() == 20);

		yggr_test_assert((now_test_ref.var_m() += now_test_ref.var_m()) == 40);
		yggr_test_assert((now_test_ref.var_m() += now_test_cref.var_m()) == 80);
		yggr_test_assert(now_test_cref.var_m() == 80);
	}

	//-
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(100 - now_test_ref.var_m() == 90);
		yggr_test_assert(100 - now_test_cref.var_m() == 90);

		yggr_test_assert(now_test_ref.var_m() - 10 == 0);
		yggr_test_assert(now_test_cref.var_m() - 10 == 0);

		yggr_test_assert(now_test_cref.var_m() - now_test_ref.var_m() == 0);
		yggr_test_assert(now_test_ref.var_m() - now_test_cref.var_m() == 0);
		yggr_test_assert(now_test_ref.var_m() - now_test_ref.var_m() == 0);
	}

	//-=
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		yggr::s32 n = 100;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((n -= now_test_ref.var_m()) == 90);
		yggr_test_assert((n -= now_test_cref.var_m()) == 80);

		yggr_test_assert((now_test_ref.var_m() -= 10) == 0);
		yggr_test_assert(now_test_cref.var_m() == 0);

		yggr_test_assert((now_test_ref.var_m() -= now_test_ref.var_m()) == 0);
		yggr_test_assert((now_test_ref.var_m() -= now_test_cref.var_m()) == 0);
		yggr_test_assert(now_test_cref.var_m() == 0);
	}

	//*
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(10 * now_test_ref.var_m() == 100);
		yggr_test_assert(10 * now_test_cref.var_m() == 100);

		yggr_test_assert(now_test_ref.var_m() * 10 == 100);
		yggr_test_assert(now_test_cref.var_m() * 10 == 100);

		yggr_test_assert(now_test_cref.var_m() * now_test_ref.var_m() == 100);
		yggr_test_assert(now_test_ref.var_m() * now_test_cref.var_m() == 100);
		yggr_test_assert(now_test_ref.var_m() * now_test_ref.var_m() == 100);
	}

	//*=
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		yggr::s32 n = 10;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((n *= now_test_ref.var_m()) == 100);
		yggr_test_assert((n *= now_test_cref.var_m()) == 1000);

		yggr_test_assert((now_test_ref.var_m() *= 10) == 100);
		yggr_test_assert(now_test_cref.var_m() == 100);

		yggr_test_assert((now_test_ref.var_m() *= now_test_ref.var_m()) == 10000);
		yggr_test_assert((now_test_ref.var_m() *= now_test_cref.var_m()) == 100000000);
		yggr_test_assert(now_test_cref.var_m() == 100000000);
	}

	// /
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(10 / now_test_ref.var_m() == 1);
		yggr_test_assert(10 / now_test_cref.var_m() == 1);

		yggr_test_assert(now_test_ref.var_m() / 10 == 1);
		yggr_test_assert(now_test_cref.var_m() / 10 == 1);

		yggr_test_assert(now_test_cref.var_m() / now_test_ref.var_m() == 1);
		yggr_test_assert(now_test_ref.var_m() / now_test_cref.var_m() == 1);
		yggr_test_assert(now_test_ref.var_m() / now_test_ref.var_m() == 1);
	}

	// /=
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		yggr::s32 n = 100000;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((n /= now_test_ref.var_m()) == 10000);
		yggr_test_assert((n /= now_test_cref.var_m()) == 1000);

		yggr_test_assert((now_test_ref.var_m() /= 10) == 1);
		yggr_test_assert(now_test_cref.var_m() == 1);

		yggr_test_assert((now_test_ref.var_m() /= now_test_ref.var_m()) == 1);
		yggr_test_assert((now_test_ref.var_m() /= now_test_cref.var_m()) == 1);
		yggr_test_assert(now_test_cref.var_m() == 1);
	}

	// %
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(10 % now_test_ref.var_m() == 0);
		yggr_test_assert(10 % now_test_cref.var_m() == 0);

		yggr_test_assert(now_test_ref.var_m() % 10 == 0);
		yggr_test_assert(now_test_cref.var_m() % 10 == 0);

		yggr_test_assert(now_test_cref.var_m() % now_test_ref.var_m() == 0);
		yggr_test_assert(now_test_ref.var_m() % now_test_cref.var_m() == 0);
		yggr_test_assert(now_test_ref.var_m() % now_test_ref.var_m() == 0);
	}

	// %=
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(10);
		yggr::s32 n = 10;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((n %= now_test_ref.var_m()) == 0);
		yggr_test_assert((n %= now_test_cref.var_m()) == 0);

		yggr_test_assert((now_test_ref.var_m() %= 10) == 0);
		yggr_test_assert(now_test_cref.var_m() == 0);

		now_test_ref.var_m() = 10;
		yggr_test_assert((now_test_ref.var_m() %= now_test_ref.var_m()) == 0);
		now_test_ref.var_m() = 10;
		yggr_test_assert((now_test_ref.var_m() %= now_test_cref.var_m()) == 0);
		yggr_test_assert(now_test_cref.var_m() == 0);
	}

	//&
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(3);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((1 & now_test_ref.var_m()) == 1);
		yggr_test_assert((2 & now_test_cref.var_m()) == 2);

		yggr_test_assert((now_test_ref.var_m() & 1) == 1);
		yggr_test_assert((now_test_cref.var_m() & 2) == 2);

		yggr_test_assert((now_test_cref.var_m() & now_test_ref.var_m()) == 3);
		yggr_test_assert((now_test_ref.var_m() & now_test_cref.var_m()) == 3);
		yggr_test_assert((now_test_ref.var_m() & now_test_ref.var_m()) == 3);
	}

	//&=
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(3);
		yggr::s32 n = 1;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((n &= now_test_ref.var_m()) == 1);
		yggr_test_assert((n &= now_test_cref.var_m()) == 1);

		yggr_test_assert((now_test_ref.var_m() &= 1) == 1);
		yggr_test_assert(now_test_cref.var_m() == 1);

		yggr_test_assert((now_test_ref.var_m() &= now_test_ref.var_m()) == 1);
		yggr_test_assert((now_test_ref.var_m() &= now_test_cref.var_m()) == 1);
		yggr_test_assert(now_test_cref.var_m() == 1);
	}

	//|
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(1);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((1 | now_test_ref.var_m()) == 1);
		yggr_test_assert((2 | now_test_cref.var_m()) == 3);

		yggr_test_assert((now_test_ref.var_m() | 1) == 1);
		yggr_test_assert((now_test_cref.var_m() | 2) == 3);

		yggr_test_assert((now_test_cref.var_m() | now_test_ref.var_m()) == 1);
		yggr_test_assert((now_test_ref.var_m() | now_test_cref.var_m()) == 1);
		yggr_test_assert((now_test_ref.var_m() | now_test_ref.var_m()) == 1);
	}

	//|=
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(1);
		yggr::s32 n = 2;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((n |= now_test_ref.var_m()) == 3);
		yggr_test_assert((n |= now_test_cref.var_m()) == 3);

		yggr_test_assert((now_test_ref.var_m() |= 1) == 1);
		yggr_test_assert(now_test_cref.var_m() == 1);

		yggr_test_assert((now_test_ref.var_m() |= now_test_ref.var_m()) == 1);
		yggr_test_assert((now_test_ref.var_m() |= now_test_cref.var_m()) == 1);
		yggr_test_assert(now_test_cref.var_m() == 1);
	}

	// ^
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(1);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((1 ^ now_test_ref.var_m()) == 0);
		yggr_test_assert((2 ^ now_test_cref.var_m()) == 3);

		yggr_test_assert((now_test_ref.var_m() ^ 1) == 0);
		yggr_test_assert((now_test_cref.var_m() ^ 2) == 3);

		yggr_test_assert((now_test_cref.var_m() ^ now_test_ref.var_m()) == 0);
		yggr_test_assert((now_test_ref.var_m() ^ now_test_cref.var_m()) == 0);
		yggr_test_assert((now_test_ref.var_m() ^ now_test_ref.var_m()) == 0);
	}

	//^=
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(1);
		yggr::s32 n = 2;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((n ^= now_test_ref.var_m()) == 3);
		yggr_test_assert((n ^= now_test_cref.var_m()) == 2);

		yggr_test_assert((now_test_ref.var_m() ^= 1) == 0);
		yggr_test_assert(now_test_cref.var_m() == 0);

		yggr_test_assert((now_test_ref.var_m() ^= now_test_ref.var_m()) == 0);
		yggr_test_assert((now_test_ref.var_m() ^= now_test_cref.var_m()) == 0);
		yggr_test_assert(now_test_cref.var_m() == 0);
	}

	// <<
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(1);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((1 << now_test_ref.var_m()) == 2);
		yggr_test_assert((2 << now_test_cref.var_m()) == 4);

		yggr_test_assert((now_test_ref.var_m() << 1) == 2);
		yggr_test_assert((now_test_cref.var_m() << 2) == 4);

		yggr_test_assert((now_test_cref.var_m() << now_test_ref.var_m()) == 2);
		yggr_test_assert((now_test_ref.var_m() << now_test_cref.var_m()) == 2);
		yggr_test_assert((now_test_ref.var_m() << now_test_ref.var_m()) == 2);
	}

	//<<=
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(1);
		yggr::s32 n = 2;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((n <<= now_test_ref.var_m()) == 4);
		yggr_test_assert((n <<= now_test_cref.var_m()) == 8);

		yggr_test_assert((now_test_ref.var_m() <<= 1) == 2);
		yggr_test_assert(now_test_cref.var_m() == 2);

		yggr_test_assert((now_test_ref.var_m() <<= now_test_ref.var_m()) == 8);
		yggr_test_assert((now_test_ref.var_m() <<= now_test_cref.var_m()) == 2048);
		yggr_test_assert(now_test_cref.var_m() == 2048);
	}

	// <<
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(1);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((1 >> now_test_ref.var_m()) == 0);
		yggr_test_assert((2 >> now_test_cref.var_m()) == 1);

		yggr_test_assert((now_test_ref.var_m() >> 1) == 0);
		yggr_test_assert((now_test_cref.var_m() >> 2) == 0);

		yggr_test_assert((now_test_cref.var_m() >> now_test_ref.var_m()) == 0);
		yggr_test_assert((now_test_ref.var_m() >> now_test_cref.var_m()) == 0);
		yggr_test_assert((now_test_ref.var_m() >> now_test_ref.var_m()) == 0);
	}

	//>>=
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(1);
		yggr::s32 n = 1024;
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert((n >>= now_test_ref.var_m()) == 512);
		yggr_test_assert((n >>= now_test_cref.var_m()) == 256);

		yggr_test_assert((now_test_ref.var_m() >>= 1) == 0);
		yggr_test_assert(now_test_cref.var_m() == 0);

		yggr_test_assert((now_test_ref.var_m() >>= now_test_ref.var_m()) == 0);
		yggr_test_assert((now_test_ref.var_m() >>= now_test_cref.var_m()) == 0);
		yggr_test_assert(now_test_cref.var_m() == 0);
	}

	// bitwise_not
	{
		typedef member_wrapper<yggr::s32> now_test_type;

		now_test_type now_test(1);
		now_test_type& now_test_ref = now_test;
		const now_test_type& now_test_cref = now_test;

		yggr_test_assert(~now_test_ref.var_m() == -2);
		yggr_test_assert(~now_test_cref.var_m() == -2);
	}

	std::cout << "test_non_member_operator succeed" << std::endl;
}

void test(void)
{
	test_base();
	test_op_index_member_access();
	test_op_pointer_member_access();
	test_op_call();
	test_non_member_operator();
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}