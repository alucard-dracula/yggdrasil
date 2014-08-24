//foo_pointer_test.cpp

#include <iostream>
#include <yggr/mplex/foo_type_info.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/vector.hpp>

#include <boost/mpl/at.hpp>

struct A
{
	template<typename T>
	void test(void) const
	{
		std::cout << typeid(T).name();
	}

	void ntest(void) const
	{
		std::cout << typeid(int).name();
	}
};


typedef yggr::mplex::foo_typdef_helper<void (A::*)(void) const>::type foo_type;

typedef boost::mpl::vector<int, float, double> vt_type;
typedef boost::mpl::vector_c<int, 10, 20, 30> vt_c_type;

int main(int argc, char* argv[])
{
	//typedef boost::mpl::at_c<vt_type, 0>::type ntype;
	//std::cout << typeid(boost::mpl::at_c<vt_type, 1>::type).name() << std::endl;
	std::cout << boost::mpl::at_c<vt_c_type, 1>::type::value << std::endl;
	std::cout << boost::mpl::size<vt_c_type>::value << std::endl;

	foo_type foo = &A::test<char>;

	A a;
	A* p = &a;
	(p->*foo)();
	char cc = 0;
	std::cin >> cc;
	return 0;
}