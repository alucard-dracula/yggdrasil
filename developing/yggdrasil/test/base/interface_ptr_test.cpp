//interface_ptr_test.cpp

#include <cassert>
#include <iostream>
#include <yggr/base/interface_ptr.hpp>

struct A
{
	A(void){}
	virtual ~A(void){}
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
};

typedef yggr::interface_ptr<A> Aptr_type;

int main(int argc, char* argvp[])
{
	Aptr_type ptr(new B());

	B* p = ptr;
	assert(p);

	const Aptr_type& cref_ptr = ptr;

	const B* cp = cref_ptr;
	assert(cp);

	std::cout << "successed" << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
