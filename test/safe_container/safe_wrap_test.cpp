//safe_wrap_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/bind.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(base)

struct A
{
	typedef A this_type;

	A(void)
		: n(0)
	{
	}

	A(int on)
		: n(on)
	{
	}

	A(const this_type& right)
		: n(right.n)
	{
	}

	this_type& operator=(const this_type& right)
	{
		n = right.n;
		return *this;
	}

	bool operator==(const this_type& right) const
	{
		return n == right.n;
	}

	void foo(void) const
	{
		std::cout << "run foo " << n << std::endl;
	}

	int get(void) const
	{
		return n;
	}

	void set(const int& on)
	{
		n = on;
	}

	int n;
};

std::ostream& operator<<(std::ostream& os, const A& na)
{
	os << na.n;
	return os;
}

typedef yggr::safe_container::safe_wrap<A> safe_a_wrap_type;


void test_constructor(void)
{
	{
		safe_a_wrap_type awrap(new A(100));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 100);
	}

	{
		A a(100);
		safe_a_wrap_type awrap(a);
		a.n = 20;
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 100);
	}

	{
		A a(100);
		safe_a_wrap_type awrap(boost::move(a));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 100);
	}

	{
		A a(100);
		safe_a_wrap_type awrap(boost::move(a));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 100);

		safe_a_wrap_type awrap2(awrap);
		safe_a_wrap_type::value_ptr_type pa2 = awrap2.get_shared_ptr();
		yggr_test_assert(pa2);
		yggr_test_assert(pa2->n == 100);
	}

	std::cout << "test_constructor end" << std::endl;
}

#include <yggr/smart_ptr_ex/default_deleter.hpp>

void test_operator_set(void)
{

	{
		A a(100);
		safe_a_wrap_type awrap;
		awrap = a;
		a.n = 20;
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 100);
	}

	{
		A a(100);
		safe_a_wrap_type awrap;
		awrap = boost::move(a);
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 100);
	}

	{
		A a(100);
		safe_a_wrap_type awrap(boost::move(a));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 100);

		safe_a_wrap_type awrap2;
		awrap2 = awrap;
		safe_a_wrap_type::value_ptr_type pa2 = awrap2.get_shared_ptr();
		yggr_test_assert(pa2);
		yggr_test_assert(pa2->n == 100);
	}

	{
		typedef yggr::smart_ptr_ex::default_deleter<A> deleter_type;
		typedef std::allocator<A> allocator_type;

		A a(100);
		safe_a_wrap_type awrap(boost::move(a));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 100);

		safe_a_wrap_type awrap2;

		awrap2 = safe_a_wrap_type::make_operator_set_helper(awrap, deleter_type());
		safe_a_wrap_type::value_ptr_type pa2 = awrap2.get_shared_ptr();
		yggr_test_assert(pa2);
		yggr_test_assert(pa != pa2);
		yggr_test_assert(pa2->n == 100);

		awrap2 = safe_a_wrap_type::make_operator_set_helper(awrap, deleter_type(), allocator_type());
		safe_a_wrap_type::value_ptr_type pa3 = awrap2.get_shared_ptr();
		yggr_test_assert(pa3);
		yggr_test_assert(pa != pa3);
		yggr_test_assert(pa2 != pa3);
		yggr_test_assert(pa3->n == 100);

	}

	std::cout << "test_operator_set end" << std::endl;
}

void test_swap(void)
{
	{
		A a(100);
		safe_a_wrap_type awrap(boost::move(a));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 100);

		safe_a_wrap_type awrap2;
		awrap2.swap(awrap);
		safe_a_wrap_type::value_ptr_type pa2 = awrap2.get_shared_ptr();
		yggr_test_assert(pa2);
		yggr_test_assert(pa2->n == 100);

		yggr_test_assert(awrap.empty());
	}

	std::cout << "test_swap end" << std::endl;
}

void test_get_set(void)
{
	safe_a_wrap_type awrap(new A(100));

	try
	{
		A a(awrap.load());
		yggr_test_assert(a.n == 100);
	}
	catch(const safe_a_wrap_type::error_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	A a2(200);
	awrap = a2;

	safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
	yggr_test_assert(pa);
	yggr_test_assert(pa->n == 200);

	yggr_test_assert(!awrap.compare_exchange_strong(A(300), A(200)));
	yggr_test_assert(awrap.compare_exchange_strong(A(200), A(300)));

	try
	{
		yggr_test_assert(awrap.load() == A(300));
	}
	catch(const safe_a_wrap_type::error_type&)
	{
		yggr_test_assert(false);
	}

	std::cout << "test_get_set end" << std::endl;
}

void test_get_set2(void)
{
	safe_a_wrap_type awrap;

	yggr_test_assert(awrap.store_or_compare_exchange_strong(A(300), A(200)));
	yggr_test_assert(awrap.compare_exchange_strong(A(200), A(300)));


	std::cout << "test_get_set2 end" << std::endl;
}


void handler_set_value(safe_a_wrap_type::base_type& pna, int n)
{
	if(!pna)
	{
		return;
	}
	pna->n = n;
}

int handler_get_value(const safe_a_wrap_type::base_type& pna)
{
	if(!pna)
	{
		return 0;
	}
	return pna->n;
}

void using_handler_test(void)
{
	safe_a_wrap_type awrap(new A(100));

	A a = awrap.using_handler(boost::bind(&A::get, _1));
	yggr_test_assert(a.n == 100);

	a.n = 20;

	try
	{
		awrap.using_handler(boost::bind(&A::set, _1, boost::cref(a.n)));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 20);
	}
	catch(const safe_a_wrap_type::error_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		awrap.using_handler(boost::bind(&handler_set_value, _1, 10));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		yggr_test_assert(pa);
		yggr_test_assert(pa->n == 10);
	}
	catch(const safe_a_wrap_type::error_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	try
	{
		int n = awrap.using_handler(boost::bind(&handler_get_value, _1));
		yggr_test_assert(10 == n);
	}
	catch(const safe_a_wrap_type::error_type& e)
	{
		std::cout << e.what() << std::endl;
		yggr_test_assert(false);
	}

	std::cout << "use_handler_test end" << std::endl;
}

#include <yggr/charset/utf8_string.hpp>

void yggr_string_set_test(void)
{
	typedef yggr::safe_container::safe_wrap<yggr::string> yggr_string_wrap_type;

	yggr_string_wrap_type wrap_str = yggr::string("str");

	wrap_str = yggr::string("str2");

	yggr_test_assert(wrap_str.load() == yggr::string("str2"));

	std::cout << "test_yggr_string_set end" << std::endl;
}

int main(int argc, char* argv[])
{
	test_constructor();
	test_operator_set();
	test_swap();
	test_get_set();
	test_get_set2();
	using_handler_test();

	yggr_string_set_test();

	wait_any_key(argc, argv);
	return 0;
}
