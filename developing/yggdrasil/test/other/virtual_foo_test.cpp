//virtual_foo_test.cpp

#include <iostream>

class A
{
public:
	virtual ~A(void) {};
	virtual void out(void) const = 0;
	void out_call(void)
	{
		this->out();
	}
};

class B : public A
{
public:
	virtual ~B(void) {};
	virtual void out(void) const
	{
		std::cout << "B" << std::endl;
	}
};

#include <boost/bind.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/mpl/at.hpp>

//template<typename T>
//struct C
//{
//};
//
//void test(int n)
//{
//}

//template<typename Handler>
//void test_foo(const Handler& handler)
//{
//	typedef Handler handler_type;
//	typedef typename boost::mpl::at_c<handler_type, 2>::type list_type;
//
//}

struct D
{
	template<std::size_t>
	void out(void) const;

	template<>
	void out<0>(void) const
	{
		std::cout << "0" << std::endl;
	}
	
};

//void D::out(void) const
//{
//	std::cout << "abc" << std::endl;
//}

//void D::F::out(void) const
//{
//	std::cout << "F" << std::endl;
//}
int main(int argc, char* argv[])
{
	A* p = new B;
	p->out_call();
	delete p;

	BOOST_MPL_ASSERT((boost::is_reference<const int&>));

	//typedef C<int> c_type;
	//typedef boost::mpl::at_c<c_type, 0>::type c_in_type;
	//c_in_type n = 100;

	//test_foo(boost::bind(&test, _1));

	D d;
	d.out<0>();

	//D::F f;
	//f.out();

	char cc = 0;
	std::cin >> cc;
	return 0;
}