// interface_ptr_test.cpp

#include <memory>
#include <cassert>
#include <test/wait_any_key/wait_any_key.hpp>
#include <yggr/smart_ptr_ex/interface_ptr.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

struct A
{
	A(void){}
	virtual ~A(void){}

	virtual int foo(void) const
	{
		return 1;
	}

};

struct B
	: public A
{
	B(void)
	{
	}

	virtual ~B(void)
	{
	}

	virtual int foo(void) const
	{
		return 2;
	}
};

typedef yggr::interface_ptr<A> Aptr_type;

void test1(void)
{
	Aptr_type ptr = yggr::construct_interface<A, yggr::shared_ptr>(yggr_nothrow_new B());

	B* p = ptr;
	yggr_test_assert(p);

	const Aptr_type& cref_ptr = ptr;

	const B* cp = cref_ptr;
	yggr_test_assert(cp);

	yggr_test_assert(ptr->foo() == 2);

	yggr_test_assert(ptr == ptr);
	yggr_test_assert(!(ptr != ptr));

	yggr::swap(ptr, ptr);
	yggr_test_assert(ptr);
	yggr_test_assert(ptr->foo() == 2);

	std::cout << "test1 successed" << std::endl;
}

void test2(void)
{
	Aptr_type ptr = yggr::make_interface<A, B, yggr::shared_ptr>(B());

	B* p = ptr;
	yggr_test_assert(p);

	const Aptr_type& cref_ptr = ptr;

	const B* cp = cref_ptr;
	yggr_test_assert(cp);

	yggr_test_assert(ptr->foo() == 2);

	yggr_test_assert(ptr == ptr);
	yggr_test_assert(!(ptr != ptr));

	yggr::swap(ptr, ptr);
	yggr_test_assert(ptr);
	yggr_test_assert(ptr->foo() == 2);

	std::cout << "test2 successed" << std::endl;
}

void test3(void)
{
	Aptr_type ptr = yggr::allocate_interface<A, B, yggr::shared_ptr>(std::allocator<B>(), B());

	B* p = ptr;
	yggr_test_assert(p);

	const Aptr_type& cref_ptr = ptr;

	const B* cp = cref_ptr;
	yggr_test_assert(cp);

	yggr_test_assert(ptr->foo() == 2);

	yggr_test_assert(ptr == ptr);
	yggr_test_assert(!(ptr != ptr));

	yggr::swap(ptr, ptr);
	yggr_test_assert(ptr);
	yggr_test_assert(ptr->foo() == 2);

	std::cout << "test3 successed" << std::endl;
}

int main(int argc, char* argv[])
{
	test1();
	test2();
	test3();

	wait_any_key(argc, argv);
	return 0;
}
