// handler_center_test_u32_id.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/handler_center/handler_center.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(base)


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

typedef yggr::handler_center::handler_center<yggr::u32> handler_center_type;
typedef yggr::ptr_single<handler_center_type> handler_center_ptr_signal_type;
typedef handler_center_ptr_signal_type::obj_ptr_type handler_center_ptr_type;


void test_reg(void)
{
	handler_center_ptr_type ptr = handler_center_ptr_signal_type::init_ins();
	yggr_test_assert(ptr);

	ptr->register_handler(yggr::u32(0), boost::bind(&out));
	ptr->register_handler(yggr::u32(1), boost::bind(&get_eins, _1));
	ptr->register_handler(yggr::u32(2), boost::bind(&get_zwei, _1, _2));

	// compile failed please use boost::reference_wrapper
	ptr->register_handler(yggr::u32(3), boost::bind(&get_drei, _1, _2));
	yggr_test_assert(4 == ptr->size());
	yggr_test_assert(!ptr->empty());


	std::cout << "test_reg end" << std::endl;
}


void test_call(void)
{
	handler_center_ptr_type ptr = handler_center_ptr_signal_type::get_ins();
	yggr_test_assert(ptr);

	{
		bool b = ptr->call(yggr::const_args_anchor(0));
		yggr_test_assert(b);
		ptr->unregister_handler(0);
		b = ptr->call(yggr::const_args_anchor(0));
		yggr_test_assert(!b);
	}

	{
		int n = 100;
		bool b = ptr->call(n, yggr::const_args_anchor(1), n);
		yggr_test_assert(b);
		yggr_test_assert(n == 101);
		b = ptr->unregister_handler(0);
		yggr_test_assert(!b);
		b = ptr->unregister_handler(1);
		yggr_test_assert(b);
		b = ptr->call(n, yggr::const_args_anchor(1), n);
		yggr_test_assert(!b);
	}

	{
		int n = 100;
		int m = 200;
		bool b = ptr->call(n, yggr::const_args_anchor(2), n, m);
		yggr_test_assert(b);
		yggr_test_assert(n == 300);

		b = ptr->call(yggr::const_args_anchor(2), n, m);
		yggr_test_assert(!b);
	}

	{
		int n = 100;
		int m = 200;
		// use reference
		bool b = ptr->call(yggr::const_args_anchor(3), boost::cref(n), boost::ref(m));
		yggr_test_assert(b);
		yggr_test_assert(m == 300);
	}

	{
		ptr->clear();
		yggr_test_assert(ptr->empty());
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
		return hc.register_handler(yggr::u32(10), boost::bind(&this_type::foo, this_type::shared_from_this()));
	}

	template<typename Handler_Center>
	bool unregister_handler(Handler_Center& hc)
	{
		return hc.unregister_handler(yggr::u32(10));
	}

	void foo(void) const
	{
		std::cout << "A::foo called" << std::endl;
	}
};

void test_reg_A(void)
{
	::yggr::shared_ptr<A> pa(new A());
	handler_center_ptr_type ptr = handler_center_ptr_signal_type::get_ins();
	if(!ptr)
	{
		return;
	}

	ptr->clear();
	bool b = ptr->register_handler_object(*pa);
	yggr_test_assert(b);

	b = ptr->unregister_handler_object(*pa);
	yggr_test_assert(b);

	yggr_test_assert(ptr->empty());
	b = ptr->register_handler_object(*pa);
	yggr_test_assert(b);

	std::cout << "test_reg_A end" << std::endl;
}

void test_A(void)
{
	handler_center_ptr_type ptr = handler_center_ptr_signal_type::get_ins();
	if(!ptr)
	{
		return;
	}

	ptr->call(yggr::const_args_anchor(10));
	ptr->clear();
	std::cout << "test_A end" << std::endl;
}

void test_swap(void)
{
	handler_center_ptr_type ptr = handler_center_ptr_signal_type::get_ins();
	if(!ptr)
	{
		return;
	}

	handler_center_type tmp;
	yggr::swap(*ptr, tmp);
	yggr::swap(*ptr, tmp);
}

int main(int argc, char* argv[])
{
	test_reg();
	test_swap();
	test_call();
	test_reg_A();
	test_swap();
	test_A();

	wait_any_key(argc, argv);
	return 0;
}
