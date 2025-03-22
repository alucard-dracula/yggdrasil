//type_awareness_test.cpp

#include<iostream>
#include<ctime>

#include <boost/bind.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/type_awareness/awareness_def.hpp>
//#include <yggr/type_awareness/t_container.hpp>
//#include <yggr/type_awareness/t_converter.hpp>
//#include <yggr/type_awareness/t_treater.hpp>

#ifdef _DEBUG
#   include <vld.h>
#endif //_DEBUG

struct A
{
	int a;
};

void out(const A& na)
{
	std::cout << na.a << std::endl;
}

typedef yggr::type_awareness::awareness_def<yggr::u32> aw_def_type;
typedef yggr::type_awareness::t_container<aw_def_type> t_container_type;
typedef yggr::type_awareness::t_treater<aw_def_type, t_container_type,
											yggr::type_awareness::base_t_converter,
											yggr::type_awareness::t_converter> t_treater_type;
void test_t_container(void)
{
	t_container_type container;

	A a= {100};

	container.save(1, a);

	std::cout << container.id() << std::endl;

	A *p = container.load<A>();

	if(p)
	{
		std::cout << p->a << std::endl;
	}
}

//-----------------------------------------------------------------
template<typename Handler>
Handler register_foo_type(t_treater_type& t, Handler handler)
{
	typedef Handler handler_type;
	t.register_converter<handler_type, A>(1, 1);
	return handler;
}

void test_t_treater(void)
{
	t_treater_type treater;
	//register_foo_type(treater, boost::bind(out, _1));

	t_container_type c1;
	c1.save(1, register_foo_type(treater, boost::bind(out, _1)));

	t_container_type c2;

	A a = {1000};

	c2.save(1, a);

	treater.conver(c1, c2);

}

//------------------------------------------------------------

int main()
{
	//test_t_container();
	test_t_treater();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
