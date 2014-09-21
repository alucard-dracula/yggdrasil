// handler_center_test.cpp

#include <iostream>
#include <yggr/base/ptr_single.hpp>
#include <yggr/handler_center/handler_center.hpp>

#ifdef _MSC_VER
#include <vld.h>
#endif // _MSC_VER

int get_eins(int n)
{
	return n + 1;
}

int get_zwei(int n, int m)
{
	return n + m;
}

void get_drei(const int& n,  int& m)
{
	m = n + m;
}

void out(void)
{
	std::cout << "abc" << std::endl;
}

typedef yggr::handler_center::handler_center<std::string> handler_center_type;
typedef yggr::ptr_single<handler_center_type> handler_center_ptr_signal_type;
typedef handler_center_ptr_signal_type::obj_ptr_type handler_center_ptr_type;

void test_reg(void)
{
	handler_center_ptr_type ptr = handler_center_ptr_signal_type::init_ins();
	assert(ptr);

	ptr->register_handler(YGGR_PP_HANDLER_NAME(out), boost::bind(&out));
	ptr->register_handler<int>(YGGR_PP_HANDLER_NAME(get_eins), boost::bind(&get_eins, _1));
	ptr->register_handler<int, int>(YGGR_PP_HANDLER_NAME(get_zwei), boost::bind(&get_zwei, _1, _2));

	// compile failed please use boost::reference_wrapper
	//ptr->register_handler<int const&, int& >(YGGR_PP_HANDLER_NAME(get_drei), boost::bind(&get_drei, _1, _2));
	ptr->register_handler<YGGR_PP_CREF_T(int), YGGR_PP_REF_T(int) >(YGGR_PP_HANDLER_NAME(get_drei), boost::bind(&get_drei, _1, _2));
	assert(4 == ptr->size());
	assert(!ptr->empty());


	std::cout << "test_reg end" << std::endl;
}


void test_call(void)
{
	handler_center_ptr_type ptr = handler_center_ptr_signal_type::get_ins();
	assert(ptr);

	{
		bool b = ptr->call(YGGR_PP_HANDLER_NAME(out));
		assert(b);
		ptr->unregister_handler(YGGR_PP_HANDLER_NAME(out));
		b = ptr->call(YGGR_PP_HANDLER_NAME(out));
		assert(!b);
	}

	{
		int n = 100;
		bool b = ptr->call(n, YGGR_PP_HANDLER_NAME(get_eins), n);
		assert(b);
		assert(n == 101);
		b = ptr->unregister_handler(YGGR_PP_HANDLER_NAME(out));
		assert(!b);
		b = ptr->unregister_handler(YGGR_PP_HANDLER_NAME(get_eins));
		assert(b);
		b = ptr->call(n, YGGR_PP_HANDLER_NAME(get_eins), n);
		assert(!b);
	}

	{
		int n = 100;
		int m = 200;
		bool b = ptr->call(n, YGGR_PP_HANDLER_NAME(get_zwei), n, m);
		assert(b);
		assert(n == 300);
	}

	{
		int n = 100;
		int m = 200;
		// use reference
		bool b = ptr->call(YGGR_PP_HANDLER_NAME(get_drei), boost::cref(n), boost::ref(m));
		assert(b);
		assert(m == 300);
	}

	{
		ptr->clear();
		assert(ptr->empty());
	}

	std::cout << "test_call end" << std::endl;
}

class A
	: public boost::enable_shared_from_this<A>
{
private:
	typedef A this_type;
public:

	template<typename Handler_Center>
	bool register_handler(Handler_Center& hc)
	{
		return hc.register_handler(YGGR_PP_HANDLER_NAME(A::foo), boost::bind(&this_type::foo, this_type::shared_from_this()));
	}

	template<typename Handler_Center>
	bool unregister_handler(Handler_Center& hc)
	{
		return hc.unregister_handler(YGGR_PP_HANDLER_NAME(A::foo));
	}

	void foo(void) const
	{
		std::cout << "A::foo called" << std::endl;
	}
};

void test_reg_A(void)
{
	boost::shared_ptr<A> pa(new A());
	handler_center_ptr_type ptr = handler_center_ptr_signal_type::get_ins();
	if(!ptr)
	{
		return;
	}

	ptr->clear();
	bool b = ptr->register_handler_object(*pa);
	assert(b);

	b = ptr->unregister_handler_object(*pa);
	assert(b);

	assert(ptr->empty());
	b = ptr->register_handler_object(*pa);
	assert(b);

	std::cout << "test_reg_A end" << std::endl;
}

void test_A(void)
{
	handler_center_ptr_type ptr = handler_center_ptr_signal_type::get_ins();
	if(!ptr)
	{
		return;
	}

	ptr->call(YGGR_PP_HANDLER_NAME(A::foo));
	ptr->clear();
	std::cout << "test_A end" << std::endl;
}

int main(int argc, char* argv[])
{
	test_reg();
	test_call();
	test_reg_A();
	test_A();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
