// handler_center_test_u32_id.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/handler_center/handler_center.hpp>
#include <yggr/time/time.hpp>
#include <yggr/time/runtime_performance_tester.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(base)


int foo_eins(int n)
{
	return n;
}


void foo_zwei(int n,  int& m)
{
	m += n;
}

typedef yggr::time::time time_type;
typedef yggr::time::runtime_performance_tester<time_type, yggr::f64> tester_type;

typedef yggr::handler_center::handler_center<yggr::u32, boost::shared_mutex> handler_center_type;
typedef yggr::ptr_single<handler_center_type> handler_center_ptr_signal_type;
typedef handler_center_ptr_signal_type::obj_ptr_type handler_center_ptr_type;


void test_reg(void)
{
	handler_center_ptr_type ptr = handler_center_ptr_signal_type::init_ins();
	yggr_test_assert(ptr);

	ptr->register_handler(yggr::u32(1), boost::bind(&foo_eins, _1));
	ptr->register_handler(yggr::u32(2), boost::bind(&foo_zwei, _1, _2));

	yggr_test_assert(2 == ptr->size());
	yggr_test_assert(!ptr->empty());


	std::cout << "test_reg end" << std::endl;
}


void test_call(void)
{

	const unsigned long long test_count = 1000000;

	int a = 0, b = 0, c = 1;
	bool bchk = false;

	handler_center_ptr_type ptr = handler_center_ptr_signal_type::get_ins();
	yggr_test_assert(ptr);

	{
		tester_type test(test_count);
		//tester_type test(1.0);
		for(unsigned long long i = 0; i != test_count; ++i)
		{
			bchk = ptr->call(yggr::const_args_anchor(2), c, boost::ref(b));
			yggr_test_assert(bchk);
		}
	}

	yggr_test_assert(b);
	std::cout << "b is " << b << std::endl;

	{
		tester_type test(test_count);
		//tester_type test(1.0);
		for(int i = 0; i != test_count; ++i)
		{
			bchk = ptr->call(a, yggr::const_args_anchor(1), c);
			yggr_test_assert(bchk);
		}
	}

	yggr_test_assert(a == 1);
	std::cout << "a is " << a << std::endl;

	
	std::cout << "test_call end" << std::endl;
}

int main(int argc, char* argv[])
{
	test_reg();
	test_call();

	wait_any_key(argc, argv);
	return 0;
}
