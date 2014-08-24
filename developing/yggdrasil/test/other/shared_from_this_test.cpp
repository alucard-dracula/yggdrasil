//shared_from_this_test.cpp

#include <iostream>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <yggr/base/interface_ptr.hpp>
#include <list>
#include <boost/bind.hpp>

class base
{
public:
	virtual void run(void) = 0;
};

template<typename Handler>
class handler_container : public base
{
public:
	typedef Handler handler_type;

	handler_container(const handler_type& handler)
		: _handler(handler)
	{
	}
	virtual void run(void)
	{
		_handler();
	}

	handler_type _handler;
};

typedef yggr::interface_ptr<base> base_ptr_type;

class test : public boost::enable_shared_from_this<test>
{
public:

	test(void)
	{
	}

	~test(void)
	{
		std::cout << "~out" << std::endl;
	}

	void out(void)
	{
		std::cout << "out" << std::endl;
		n = 100;
	}

	template<typename List>
	void bind_to_list(List& list)
	{
		//bind_to_list_helper(list, boost::bind(&test::out, this));// error
		bind_to_list_helper(list, boost::bind(&test::out, shared_from_this()));
	}

	template<typename List, typename Handler>
	void bind_to_list_helper(List& list, Handler handler)
	{
		typedef Handler handler_type;
		typedef handler_container<handler_type> container_type;

		base_ptr_type ptr(new container_type(handler));
		list.push_back(ptr);
	}

	int n;
};

typedef boost::shared_ptr<test> test_ptr_type;

typedef std::list<test_ptr_type> test_ptr_list_type;
typedef std::list<base_ptr_type> base_ptr_list_type;

void fix_container(base_ptr_list_type& list)
{
	for(;!list.empty();)
	{
		list.front()->run();
		list.pop_front();
	}
}

int main(int argc, char* argv[])
{
	test_ptr_list_type test_list;
	base_ptr_list_type container_list;

	for(int i = 0; i != 10; ++i)
	{
		test_ptr_type ptr(new test());
		test_list.push_back(ptr);
		test_list.back()->bind_to_list(container_list);
	}

	test_list.clear();
	fix_container(container_list);

	char cc = 0;
	std::cin >> cc;

	return 0;
}
