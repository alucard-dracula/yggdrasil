//safe_wrap_test.cpp

#include <iostream>
#include <boost/bind.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

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
		assert(pa);
		assert(pa->n == 100);
	}

	{
		A a(100);
		safe_a_wrap_type awrap(a);
		a.n = 20;
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		assert(pa);
		assert(pa->n == 100);
	}

	{
		A a(100);
		safe_a_wrap_type awrap(boost::move(a));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		assert(pa);
		assert(pa->n == 100);
	}

	std::cout << "test_constructor end" << std::endl;
}

void test_get_set(void)
{
	safe_a_wrap_type awrap(new A(100));

	try
	{
		A a(awrap.get_value());
		assert(a.n == 100);
	}
	catch(const safe_a_wrap_type::error_type& e)
	{
		std::cout << e.what() << std::endl;
		assert(false);
	}

	A a2(200);
	awrap = a2;

	safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
	assert(pa);
	assert(pa->n == 200);

	std::cout << "test_get_set end" << std::endl;
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

void use_handler_test(void)
{
	safe_a_wrap_type awrap(new A(100));

	A a = awrap.use_handler(boost::bind(&A::get, _1));
	assert(a.n == 100);

	a.n = 20;

	try
	{
		awrap.use_handler(boost::bind(&A::set, _1, boost::cref(a.n)));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		assert(pa);
		assert(pa->n == 20);
	}
	catch(const safe_a_wrap_type::error_type& e)
	{
		std::cout << e.what() << std::endl;
		assert(false);
	}

	try
	{
		awrap.use_handler(boost::bind(&handler_set_value, _1, 10));
		safe_a_wrap_type::value_ptr_type pa = awrap.get_shared_ptr();
		assert(pa);
		assert(pa->n == 10);
	}
	catch(const safe_a_wrap_type::error_type& e)
	{
		std::cout << e.what() << std::endl;
		assert(false);
	}

	try
	{
		int n = awrap.use_handler(boost::bind(&handler_get_value, _1));
		assert(10 == n);
	}
	catch(const safe_a_wrap_type::error_type& e)
	{
		std::cout << e.what() << std::endl;
		assert(false);
	}

	std::cout << "use_handler_test end" << std::endl;
}


int main(int argc, char* argv[])
{
	test_constructor();
	test_get_set();
	use_handler_test();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
