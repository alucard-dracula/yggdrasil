//args_holder_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/utility/args_holder.hpp>
#include <yggr/utility/args_holder_nv.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

typedef yggr::utility::basic_args_holder base_holder_type;
typedef yggr::utility::args_holder<int> args_holder_type;
typedef yggr::utility::args_holder<const int> cargs_holder_type;

void foo_dynamic(base_holder_type& holder)
{
	int* p = yggr::utility::args_holder_dynamic_cast<int>(holder);
	yggr_test_assert(p);
	std::cout << *p << std::endl;
	*p = 100;

	std::cout << "foo_dynamic" << std::endl;
}

void cfoo_dynamic(const base_holder_type& holder)
{
	const int* p = yggr::utility::args_holder_dynamic_cast<const int>(holder);
	yggr_test_assert(p);
	std::cout << *p << std::endl;
	yggr_test_assert(*p == 10);
	std::cout << "cfoo_dynamic" << std::endl;
}


void foo_static(base_holder_type& holder)
{
	int* p = yggr::utility::args_holder_static_cast<int>(holder);
	yggr_test_assert(p);
	std::cout << *p << std::endl;
	*p = 1000;
	std::cout << "foo_static" << std::endl;
}

void cfoo_static(const base_holder_type& holder)
{
	const int* p = yggr::utility::args_holder_static_cast<const int>(holder);
	yggr_test_assert(p);
	std::cout << *p << std::endl;
	yggr_test_assert(*p == 10);
	std::cout << "cfoo_static" << std::endl;
}

void test_args_holder(void)
{
	int m = 10;
	args_holder_type args_holder(m);
	cargs_holder_type cargs_holder(m);

	{

		foo_dynamic(args_holder);
		yggr_test_assert((*yggr::utility::args_holder_dynamic_cast<int>(args_holder)) == 100);
		cfoo_dynamic(cargs_holder);
	}

	{
		foo_static(args_holder);
		yggr_test_assert((*yggr::utility::args_holder_static_cast<int>(args_holder)) == 1000);
		cfoo_static(cargs_holder);
	}
}

typedef yggr::utility::basic_args_holder_nv basic_args_holder_nv_type;
typedef yggr::utility::args_holder_nv_ref_wrap<int> ref_args_holder_type;
typedef yggr::utility::args_holder_nv_ref_wrap<const int> cref_args_holder_type;
typedef yggr::utility::args_holder_nv<int> trans_args_holder_type;

void ref_foo_static(basic_args_holder_nv_type& holder)
{
	int* p = yggr::utility::args_holder_ref_wrap_static_cast<int>(holder);
	yggr_test_assert(p);
	std::cout << *p << std::endl;
	*p = 1000;
	std::cout << "ref_foo_static" << std::endl;
}

void cref_foo_static(const basic_args_holder_nv_type& holder)
{
	const int* p = yggr::utility::args_holder_ref_wrap_static_cast<const int>(holder);
	yggr_test_assert(p);
	std::cout << *p << std::endl;
	yggr_test_assert(*p == 1000);
	std::cout << "cref_foo_static" << std::endl;
}

void trans_foo_static(basic_args_holder_nv_type& holder)
{
	int* p = yggr::utility::args_holder_static_cast<int>(holder);
	yggr_test_assert(p);
	std::cout << *p << std::endl;
	yggr_test_assert(*p == 10);
	*p = 2000;
	std::cout << "trans_foo_static" << std::endl;
}

void test_args_holder_nv(void)
{
	int m = 10;
	ref_args_holder_type ref_args_holder(m);
	cref_args_holder_type cref_args_holder(m);
	trans_args_holder_type trans_args_holder(m);

	ref_foo_static(yggr::utility::args_holder_trans(ref_args_holder));
	yggr_test_assert(m == 1000);
	cref_foo_static(yggr::utility::args_holder_trans(cref_args_holder));
	trans_foo_static(yggr::utility::args_holder_trans(trans_args_holder));
	yggr_test_assert( (*trans_args_holder.get_pointer()) == 2000);
	
}

int main(int argc, char* argv[])
{
	test_args_holder();
	test_args_holder_nv();

	wait_any_key(argc, argv);
	return 0;
}