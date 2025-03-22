//bind_comparer_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/bind_ex/bind.hpp>

#include <boost/container/list.hpp>

#include <typeinfo>

#include <list>
#include <algorithm>
#include <functional>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER


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
	std::cout << (&out_0) << std::endl;
	std::cout << (&out_0_1) << std::endl;
	yggr_test_assert(boost::bind(&out_0) == boost::bind(&out_0));

	if((&out_0) == (&out_0_1))
	{
		yggr_test_assert(boost::bind(&out_0) == boost::bind(&out_0_1));
	}
	else
	{
		yggr_test_assert(boost::bind(&out_0) != boost::bind(&out_0_1));
	}

	A a, a2;
	yggr_test_assert(boost::bind(&A::out, &a) == boost::bind(&A::out, &a));
	yggr_test_assert(boost::bind(&A::out, &a2) != boost::bind(&A::out, &a));
	yggr_test_assert(!typeid_compare(boost::bind(&A::out, &a), boost::bind(&A::out_1, _1)));
	a = a2;

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

	yggr_test_assert(std::find(list.begin(), list.end(), handler) != list.end());

	std::equal_to<handler_type> eq;

	bool bchk = eq(handler, handler);
	yggr_test_assert(bchk);

	std::cout << "test_container completed" << std::endl;

}

int main(int argc, char* argv[])
{
	test_compare();

	test_container(boost::bind(&out_0));

	wait_any_key(argc, argv);
	return 0;
}
