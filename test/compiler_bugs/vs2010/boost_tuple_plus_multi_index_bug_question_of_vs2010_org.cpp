// test.cpp 

#include <iostream>


struct A
{
	template<int n>
	void foo(void) const
	{
		std::cout << n << std::endl;
	}
};

template<typename T, void(T::*foo)(void) const>
struct caller
{
	void operator()(const T& t) const
	{
		return (t.*foo)();
	}
};

int main(int argc, char* argv[])
{
	typedef caller<A, &A::foo<2> > call_1_type;
	typedef caller<A, &A::foo<1> > call_2_type;
	A a;

	a.foo<1>();
	a.foo<2>();

	call_1_type()(a);
	call_2_type()(a);
	char cc = 0;
	std::cin >> cc;

	return 0;
}