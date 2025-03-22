// make_shared_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/func/foo_t_info.hpp>
#include <yggr/base/new.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

struct A
{
public:
	A(void)
		: n(0)
	{
	}

	template<typename T0>
	A(const T0&)
		: n(1)
	{
	}

	template<typename T0, typename T1>
	A(const T0&, const T1&)
		: n(2)
	{
	}

	A(const A& right)
		: n(right.n)
	{
	}

	int n;
};

void test_boost(void)
{
	{
		yggr::shared_ptr<A> ptr = yggr::make_shared<A>();
		yggr_test_assert(ptr->n == 0);
	}

	{
		yggr::shared_ptr<A> ptr = yggr::make_shared<A>(10);
		yggr_test_assert(ptr->n == 1);
	}

	{
		yggr::shared_ptr<A> ptr = yggr::make_shared<A>(10, 20);
		yggr_test_assert(ptr->n == 2);
	}

	{
		yggr::shared_ptr<A> ptr = yggr::make_shared<A>(A());
		yggr_test_assert(ptr->n == 0);
	}

	{
		yggr::shared_ptr<A> ptr = yggr::construct_shared<A>(yggr_nothrow_new A());
		yggr_test_assert(ptr->n == 0);
	}
}

#if !defined(YGGR_NO_CXX11_SMART_PTR)

void test_stl(void)
{
	{
		std::shared_ptr<A> ptr = yggr::smart_ptr_ex::make_shared_stl<A>();
		yggr_test_assert(ptr->n == 0);
	}

	{
		std::shared_ptr<A> ptr = yggr::smart_ptr_ex::make_shared_stl<A>(10);
		yggr_test_assert(ptr->n == 1);
	}

	{
		std::shared_ptr<A> ptr = yggr::smart_ptr_ex::make_shared_stl<A>(10, 20);
		yggr_test_assert(ptr->n == 2);
	}

	{
		std::shared_ptr<A> ptr = yggr::smart_ptr_ex::make_shared_stl<A>(A());
		yggr_test_assert(ptr->n == 0);
	}

	{
		std::shared_ptr<A> ptr = yggr::smart_ptr_ex::construct_shared_stl<A>(yggr_nothrow_new A());
		yggr_test_assert(ptr->n == 0);
	}
}

#else
void test_stl(void) {}
#endif // 

int main(int argc, char* argv[])
{
	test_boost();
	test_stl();

	wait_any_key(argc, argv);
	return 0;
}
