// handler_center_test_same_id_and_out.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/base/ptr_single.hpp>
#include <yggr/handler_center/handler_center.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(base)


yggr::u32 get_eins(yggr::u32 n)
{
	return n + 1;
}

yggr::u32 get_zwei(yggr::u32 n, yggr::u32 m)
{
	return n + m;
}

void get_drei(const yggr::u32& n,  yggr::u32& m)
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


void test(void)
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

	{
		yggr::u32 n = 100;
		bool b = ptr->call(n, yggr::const_args_anchor(1), yggr::u32(100));
		yggr_test_assert(b);
		yggr_test_assert(n == 101);
	}

	{
		yggr::u32 n = 100;
		yggr::u32 m = 200;
		// use reference
		bool b = ptr->call(yggr::const_args_anchor(3), boost::cref(n), boost::ref(m));
		yggr_test_assert(b);
		yggr_test_assert(m == 300);
	}

	std::cout << "test end" << std::endl;
}

int main(int argc, char* argv[])
{
	test();

	wait_any_key(argc, argv);
	return 0;
}
