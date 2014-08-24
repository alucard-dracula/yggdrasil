//mpl_set_test.cpp

#include <iostream>
#include <cassert>

#include <list>
#include <algorithm>
#include <functional>

#include <typeinfo>

#include <boost/container/list.hpp>

#include <yggr/struct_support/bind.hpp>


//#include <yggr/network/network_mixer_handler.hpp>


void out_0(void)
{
}

void out_0_1(void)
{
}

void out_1(int n)
{
}

void out_1_1(int n)
{
}


struct A
{
	void out(void)
	{
	}

	void out_1(void)
	{
	}
};

template<typename Handler1, typename Handler2>
bool typeid_compare(const Handler1& handler1, const Handler2& handler2)
{
	return typeid(Handler1).name() == typeid(Handler2).name();
}

void test_compare(void)
{
	assert(boost::bind(&out_0) == boost::bind(&out_0));
	assert(boost::bind(&out_0) != boost::bind(&out_0_1));

	A a, a2;
	assert(boost::bind(&A::out, &a) == boost::bind(&A::out, &a));
	assert(boost::bind(&A::out, &a2) != boost::bind(&A::out, &a));
	assert(!typeid_compare(boost::bind(&A::out, &a), boost::bind(&A::out_1, _1)));

	std::cout << "test_compare completed" << std::endl;
}

template<typename Handler>
void test_container(const Handler& handler)
{
	typedef Handler handler_type;
	typedef std::list<handler_type> handler_list_type;

	handler_list_type list;

	list.push_back(handler);
	list.push_back(handler);

	assert(std::find(list.begin(), list.end(), handler) != list.end());

	std::equal_to<handler_type> eq;

	assert((eq(handler, handler)));

	std::cout << "test_container completed" << std::endl;

}

int main(int argc, char* argv[])
{
	test_compare();

	test_container(boost::bind(&out_0));

	char cc = 0;
	std::cin >> cc;
	return 0;
}
