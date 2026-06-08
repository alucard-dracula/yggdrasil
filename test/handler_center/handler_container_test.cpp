// handler_container_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

//#define YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST 1

#include <yggr/base/ptr_single.hpp>
#include <yggr/handler_center/handler_container.hpp>

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

void test_boject(void)
{
	typedef yggr::handler_center::basic_handler_container::invoke_result_type invoke_result_type;

	{
		invoke_result_type invoke_rst = {false};
		(yggr::handler_center::make_handler_container(boost::bind(&out))).invoke(invoke_rst);
		assert(invoke_rst);
	}

	{
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		(yggr::handler_center::make_handler_container(boost::bind(&get_eins, _1))).invoke(foo_ret, invoke_rst, 10);
		assert(invoke_rst);
		assert(foo_ret == 11);
	}

	{
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		(yggr::handler_center::make_handler_container(boost::bind(&get_zwei, _1, _2))).invoke(foo_ret, invoke_rst, 10, 20);
		assert(invoke_rst);
		assert(foo_ret == 30);
	}
#if !YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST
	{
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		(yggr::handler_center::make_handler_container(boost::bind(&get_zwei, _1, _2))).invoke(foo_ret, invoke_rst, 10, 20.5); // args mismatch
		assert(!invoke_rst);
		assert(foo_ret == 0);
	}
#endif // YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST

	{
		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		(yggr::handler_center::make_handler_container(boost::bind(&get_drei, _1, _2))).invoke(invoke_rst, boost::cref(foo_arg1), boost::ref(foo_arg2));
		assert(invoke_rst);
		assert(foo_arg2 == 40);
	}
}

void test_shared(void)
{
	typedef yggr::handler_center::basic_handler_container_sptr_type cont_ptr_type;
	typedef yggr::handler_center::basic_handler_container::invoke_result_type invoke_result_type;


	{
		cont_ptr_type pfoo_cont = yggr::handler_center::make_shared_handler_container(boost::bind(&out));
		assert(pfoo_cont);
		invoke_result_type invoke_rst = {false};
		pfoo_cont->invoke(invoke_rst);
		assert(invoke_rst);
	}

	{
		cont_ptr_type pfoo_cont = yggr::handler_center::make_shared_handler_container(boost::bind(&get_eins, _1));
		assert(pfoo_cont);
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		pfoo_cont->invoke(foo_ret, invoke_rst, 10);
		assert(invoke_rst);
		assert(foo_ret == 11);
	}

	{
		cont_ptr_type pfoo_cont = yggr::handler_center::make_shared_handler_container(boost::bind(&get_zwei, _1, _2));
		assert(pfoo_cont);
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		pfoo_cont->invoke(foo_ret, invoke_rst, 10, 20);
		assert(invoke_rst);
		assert(foo_ret == 30);
	}

#if !YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST
	{
		cont_ptr_type pfoo_cont = yggr::handler_center::make_shared_handler_container(boost::bind(&get_zwei, _1, _2));
		assert(pfoo_cont);
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		pfoo_cont->invoke(foo_ret, invoke_rst, 10, 20.5); // args mismatch
		assert(!invoke_rst);
		assert(foo_ret == 0);
	}
#endif // YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST

	{
		cont_ptr_type pfoo_cont = yggr::handler_center::make_shared_handler_container(boost::bind(&get_drei, _1, _2));
		assert(pfoo_cont);
		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		pfoo_cont->invoke(invoke_rst, boost::cref(foo_arg1), boost::ref(foo_arg2)); // args mismatch
		assert(invoke_rst);
		assert(foo_arg2 == 40);
	}
}


int main(int argc, char* argv[])
{
	test_boject();
	test_shared();

	wait_any_key(argc, argv);
	return 0;
}
