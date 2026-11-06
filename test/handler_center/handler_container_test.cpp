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

struct mem_foo_t
{
public:
	int nc_mf1(int n)
	{
		return n + 1;
	}

	int mf1(int n ) const
	{
		return n + 1;
	}

	int nc_mf2(int n, int m)
	{
		return n + m;
	}

	int mf2(int n, int m) const
	{
		return n + m;
	}

	int nc_mf2_mix_ref(const int& n, int& m)
	{
		return m += n;
	}

	int mf2_mix_ref(const int& n, int& m) const
	{
		return m += n;
	}

	void nc_nret_mf2_mix_ref(const int& n, int& m)
	{
		m += n;
	}

	void nret_mf2_mix_ref(const int& n, int& m) const
	{
		m += n;
	}

	int nc_mf0(void)
	{
		return 40;
	}

	int mf0(void) const
	{
		return 40;
	}

	void nc_nret_mf0(void)
	{
		std::cout << "nc_nret_mf0 = 40" << std::endl;
	}

	void nret_mf0(void) const
	{
		std::cout << "nret_mf0 = 40" << std::endl;
	}

};

void test_object(void)
{
	typedef yggr::handler_center::basic_handler_container::invoke_result_type invoke_result_type;

	{
		invoke_result_type invoke_rst = {false};
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&out))).invoke(invoke_rst);
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
	}

	{
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&get_eins, _1))).invoke(foo_ret, invoke_rst, 10);
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
		yggr_test_assert(foo_ret == 11);
	}

	{
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&get_zwei, _1, _2))).invoke(foo_ret, invoke_rst, 10, 20);
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
		yggr_test_assert(foo_ret == 30);
	}

#if !YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST
	{
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&get_zwei, _1, _2))).invoke(foo_ret, invoke_rst, 10, 20.5); // args mismatch
		yggr_test_assert(!invoke_rst);
		yggr_test_assert(!invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
		yggr_test_assert(foo_ret == 0);
	}
#endif // YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST

	{
		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&get_drei, _1, _2))).invoke(invoke_rst, boost::cref(foo_arg1), boost::ref(foo_arg2));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
		yggr_test_assert(foo_arg2 == 40);
	}
}

void test_shared(void)
{
	typedef yggr::handler_center::basic_handler_container_sptr_type cont_ptr_type;
	typedef yggr::handler_center::basic_handler_container::invoke_result_type invoke_result_type;


	{
		cont_ptr_type pfoo_cont = yggr::handler_center::make_shared_handler_container(boost::bind(&out));
		yggr_test_assert(pfoo_cont);
		invoke_result_type invoke_rst = {false};
		const invoke_result_type& invoke_rst_cref = pfoo_cont->invoke(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
		yggr_test_assert(invoke_rst);
	}

	{
		cont_ptr_type pfoo_cont = yggr::handler_center::make_shared_handler_container(boost::bind(&get_eins, _1));
		yggr_test_assert(pfoo_cont);
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		const invoke_result_type& invoke_rst_cref = pfoo_cont->invoke(foo_ret, invoke_rst, 10);
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
		yggr_test_assert(foo_ret == 11);
	}

	{
		cont_ptr_type pfoo_cont = yggr::handler_center::make_shared_handler_container(boost::bind(&get_zwei, _1, _2));
		yggr_test_assert(pfoo_cont);
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		const invoke_result_type& invoke_rst_cref = pfoo_cont->invoke(foo_ret, invoke_rst, 10, 20);
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
		yggr_test_assert(foo_ret == 30);
	}

#if !YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST
	{
		cont_ptr_type pfoo_cont = yggr::handler_center::make_shared_handler_container(boost::bind(&get_zwei, _1, _2));
		yggr_test_assert(pfoo_cont);
		invoke_result_type invoke_rst = {false};
		int foo_ret = 0;
		const invoke_result_type& invoke_rst_cref = pfoo_cont->invoke(foo_ret, invoke_rst, 10, 20.5); // args mismatch
		yggr_test_assert(!invoke_rst);
		yggr_test_assert(!invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
		yggr_test_assert(foo_ret == 0);
	}
#endif // YGGR_HANDLER_CENTER_ARGS_HOLDER_CONV_USING_STATIC_CAST

	{
		cont_ptr_type pfoo_cont = yggr::handler_center::make_shared_handler_container(boost::bind(&get_drei, _1, _2));
		yggr_test_assert(pfoo_cont);
		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		const invoke_result_type& invoke_rst_cref = pfoo_cont->invoke(invoke_rst, boost::cref(foo_arg1), boost::ref(foo_arg2)); // args mismatch
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
		yggr_test_assert(foo_arg2 == 40);
	}
}

void test_mem_foo(void)
{
	typedef yggr::handler_center::basic_handler_container::invoke_result_type invoke_result_type;

	// test mem_foo

	// !!! don't support this return value discardable!!!
	//{
	//	mem_foo_t mem_foo_obj;

	//	invoke_result_type invoke_rst = {false};
	//	int foo_arg1 = 10;
	//	int foo_arg2 = 30;
	//	const invoke_result_type& invoke_rst_cref =
	//		(yggr::handler_center::make_handler_container(boost::bind(&mem_foo_t::nc_mf2_mix_ref, boost::ref(mem_foo_obj), _1, _2))).invoke(
	//			invoke_rst, boost::cref(foo_arg1), boost::ref(foo_arg2));
	//	yggr_test_assert(invoke_rst);
	//	yggr_test_assert(invoke_rst_cref);
	//	yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
	//	yggr_test_assert(foo_arg2 == 40);
	//}


	// ret
	// arg N
	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		int result = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&mem_foo_t::nc_mf2_mix_ref, boost::ref(mem_foo_obj), _1, _2))).invoke(
				result, invoke_rst, boost::cref(foo_arg1), boost::ref(foo_arg2));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(foo_arg2 == 40);
		yggr_test_assert(result == 40);
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		int result = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(&mem_foo_t::nc_mf2_mix_ref)).invoke(
				result, invoke_rst, boost::ref(mem_foo_obj), boost::cref(foo_arg1), boost::ref(foo_arg2));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(foo_arg2 == 40);
		yggr_test_assert(result == 40);
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		int result = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&mem_foo_t::mf2_mix_ref, boost::cref(mem_foo_obj), _1, _2))).invoke(
				result, invoke_rst, boost::cref(foo_arg1), boost::ref(foo_arg2));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(foo_arg2 == 40);
		yggr_test_assert(result == 40);
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		int result = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(&mem_foo_t::mf2_mix_ref)).invoke(
				result, invoke_rst, boost::cref(mem_foo_obj), boost::cref(foo_arg1), boost::ref(foo_arg2));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(foo_arg2 == 40);
		yggr_test_assert(result == 40);
	}

	// arg 0
	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int result = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&mem_foo_t::nc_mf0, boost::ref(mem_foo_obj)))).invoke(
				result, invoke_rst);
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(result == 40);
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int result = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(&mem_foo_t::nc_mf0)).invoke(
				result, invoke_rst, boost::ref(mem_foo_obj));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(result == 40);
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int result = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&mem_foo_t::mf0, boost::cref(mem_foo_obj)))).invoke(
				result, invoke_rst);
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(result == 40);
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int result = 0;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(&mem_foo_t::mf0)).invoke(
				result, invoke_rst, boost::cref(mem_foo_obj));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(result == 40);
	}

	// no ret

	// arg N
	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&mem_foo_t::nc_nret_mf2_mix_ref, boost::ref(mem_foo_obj), _1, _2))).invoke(
				invoke_rst, boost::cref(foo_arg1), boost::ref(foo_arg2));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(foo_arg2 == 40);
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(&mem_foo_t::nc_nret_mf2_mix_ref)).invoke(
				invoke_rst, boost::ref(mem_foo_obj), boost::cref(foo_arg1), boost::ref(foo_arg2));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(foo_arg2 == 40);
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&mem_foo_t::nret_mf2_mix_ref, boost::cref(mem_foo_obj), _1, _2))).invoke(
				invoke_rst, boost::cref(foo_arg1), boost::ref(foo_arg2));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(foo_arg2 == 40);
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		int foo_arg1 = 10;
		int foo_arg2 = 30;
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(&mem_foo_t::nret_mf2_mix_ref)).invoke(
				invoke_rst, boost::cref(mem_foo_obj), boost::cref(foo_arg1), boost::ref(foo_arg2));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));

		yggr_test_assert(foo_arg2 == 40);
	}

	// arg 0
	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&mem_foo_t::nc_nret_mf0, boost::ref(mem_foo_obj)))).invoke(
				invoke_rst);
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(&mem_foo_t::nc_nret_mf0)).invoke(
				invoke_rst, boost::ref(mem_foo_obj));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(boost::bind(&mem_foo_t::nret_mf0, boost::cref(mem_foo_obj)))).invoke(
				invoke_rst);
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
	}

	{
		mem_foo_t mem_foo_obj;

		invoke_result_type invoke_rst = {false};
		const invoke_result_type& invoke_rst_cref =
			(yggr::handler_center::make_handler_container(&mem_foo_t::nret_mf0)).invoke(
				invoke_rst, boost::cref(mem_foo_obj));
		yggr_test_assert(invoke_rst);
		yggr_test_assert(invoke_rst_cref);
		yggr_test_assert((boost::addressof(invoke_rst) == boost::addressof(invoke_rst_cref)));
	}
}

#include <typeinfo>

int main(int argc, char* argv[])
{
	test_object();
	test_shared();
	test_mem_foo();

	wait_any_key(argc, argv);
	return 0;
}
