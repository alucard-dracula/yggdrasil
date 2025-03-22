//static_var_des_test.cpp

#include <iostream>
#include <yggr/base/ptr_single.hpp>
#include <boost/any.hpp>

#ifdef _WIN32
#   include <vld.h>
#   pragma comment(lib, "vld.lib")
#endif //_WIN32

class B
{
public:
	B(void)
	{
	}

	~B(void)
	{
        std::cout << "~B" << std::endl;
		int nnn = 0;
	}
};


class A
{
public:
	typedef int itype;
	A(void)
	{
	}

	~A(void)
	{
	    std::cout << "~A" << std::endl;
		int nnn = 0;
	}
};


int main(int argc, char* argv[])
{
	typedef yggr::ptr_single<A> as_type;
	typedef as_type::obj_ptr_type ptr_type;
	typedef yggr::ptr_single<B> bs_type;


	bs_type::init_ins();
	as_type::init_ins();

	//static A a;
	//static B b;

	std::cout << typeid(int).name() << std::endl;
	std::cout << typeid(A::itype).name() << std::endl;

	char cc = 0;
	std::cin >> cc;
	return 0;
}
