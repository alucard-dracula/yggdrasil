//safe_priority_queue_test.cpp

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER
#include <iostream>
#include <yggr/safe_container/safe_queue.hpp>
#include <boost/bind.hpp>
#include <assert.h>
#include <boost/swap.hpp>
#include <boost/thread.hpp>

//-------------------------test base-------------------------
#ifdef _DEBUG
typedef yggr::safe_container::safe_priority_queue<int> queue_type;

void test_constructor(void)
{
	queue_type cont1;
	assert(cont1.empty());

	queue_type cont2(cont1.value_comp());
	assert(cont2.empty());

	queue_type cont2_2(cont1.get_allocator());
	assert(cont2_2.empty());

	queue_type::container_type init_cont;
	queue_type cont3(init_cont);
	assert(cont2.empty());

	queue_type::base_type base_cont4;
	base_cont4.push(10);
	base_cont4.push(20);

	queue_type cont4(boost::move(base_cont4));
	assert(cont4.size() == 2);

	queue_type::base_type base_cont5;
	base_cont5.push(10);
	base_cont5.push(20);

	queue_type cont5(base_cont5);
	assert(cont5.size() == base_cont5.size());

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

void test_capacity(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	queue_type::container_type init_cont(arr, arr + 10);

	queue_type cont1(init_cont);
	queue_type::base_type base_cont1(init_cont);

	assert(cont1.size() == base_cont1.size());

	int f = 0;
	assert(cont1.top(f));

	assert(f == base_cont1.top());

	cont1.clear();
	assert(!cont1.top(f));

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

void test_modifiers(void)
{
	queue_type cont;
	queue_type::base_type base_cont;

	cont.push(200);
	cont.push(20);
	cont.push(35);

	int n = 100;
	cont.push(boost::move(n));

	int f = 0;
	assert(cont.top(f));
	assert(f == 200);
	assert(cont.pop());

	int k = 0;
	assert(cont.pop(k));
	assert(k == 100);

	cont.clear();
	assert(cont.empty());
	assert(!cont.pop());
	int e = 0;
	assert(!cont.pop(e));

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

void test_swap(void)
{
	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	queue_type::container_type init_cont1(arr1, arr1 + 10);
	queue_type::container_type init_cont2(arr2, arr2 + 5);

	queue_type cont1(init_cont1);
	queue_type cont2(init_cont2);

	queue_type::base_type base_cont1(init_cont1);
	queue_type::base_type base_cont2(init_cont2);
	assert(cont1.size() == base_cont1.size());
	assert(cont1.size() != base_cont2.size());

	queue_type::base_type base_cont3(base_cont1);
	queue_type::base_type base_cont4(base_cont2);

	// ------------------ boost::swap test------------------------
	// base_type&& this_type&
	boost::swap(boost::move(base_cont4), cont1);
	assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	boost::swap(base_cont3, cont1);
	assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	boost::swap(cont1, boost::move(base_cont3));
	assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	boost::swap(cont1, boost::move(base_cont4));
	assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	boost::swap(cont1, cont2);
	boost::swap(cont1, cont2);

	// ------------------ std::swap test------------------------
	// base_type&& this_type&
	std::swap(boost::move(base_cont4), cont1);
	assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	std::swap(base_cont3, cont1);
	assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	std::swap(cont1, boost::move(base_cont3));
	assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	std::swap(cont1, boost::move(base_cont4));
	assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	std::swap(cont1, cont2);
	std::swap(cont1, cont2);

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

#endif // _DEBUG

void debug_test(void)
{
#ifdef _DEBUG
	test_constructor();
	test_capacity();
	test_modifiers();
	test_swap();
#endif // _DEBUG
}

//-----------------------test safe---------------------------------------------

#include <boost/shared_ptr.hpp>
#include <cstdlib>
#include <boost/thread.hpp>
#include <boost/thread.hpp>
#include <yggr/safe_container/safe_vector.hpp>

typedef yggr::safe_container::safe_vector<int, boost::mutex, std::allocator<int>, std::vector> hvt_type;

enum
{
	E_read = 0,
	E_write = 100,
	E_max_elem = 20000
};

template<typename Val>
Val create_val(void)
{
	typedef Val val_type;
	typedef typename val_type::element_type elem_type;
	return val_type(new elem_type(std::rand()));
}

template<typename Container>
void read_op(const Container& cont, hvt_type& hvt)
{
	typedef Container container_type;
	typedef typename container_type::base_type container_base_type;
	typedef typename container_type::val_type val_type;

	if(E_max_elem < hvt.size())
	{
		hvt.clear();
	}

	int op = std::rand() % 4;
	switch(op)
	{
	case 0: // size
		hvt.push_back(0 + E_read);
		cont.size();
		break;
	case 1: // empty
		hvt.push_back(1 + E_read);
		cont.empty();
		break;
	case 2:
		{
			hvt.push_back(2 + E_read);
			val_type val;
			cont.top(val);
		}
		break;
	case 3: // get_allocator
		hvt.push_back(4 + E_read);
		cont.get_allocator();
		break;
	default:
		assert(false);
	}
}

template<typename Container>
void write_op(Container& cont, hvt_type& hvt)
{
	typedef Container container_type;
	typedef typename container_type::base_type container_base_type;
	typedef typename container_type::val_type val_type;
	if(E_max_elem < hvt.size())
	{
		hvt.clear();
	}

	int op = std::rand() % 9;
	switch(op)
	{
	case 0: // operator= &&
		{
			hvt.push_back(0 + E_write);
			container_base_type tmp;
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			cont = boost::move(tmp);
		}
		break;
	case 1: // operator = const &
		{
			hvt.push_back(1 + E_write);
			container_base_type tmp;
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			cont = tmp;
		}
		break;
	case 2: // push_back(int &&)
		hvt.push_back(2 + E_write);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		cont.push(boost::move(create_val<val_type>()));
#else
        {
            val_type val(create_val<val_type>());
            cont.push(boost::move(val));
        }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		break;
	case 3: // push_back
		hvt.push_back(3 + E_write);
		cont.push(create_val<val_type>());
		break;
	case 4:
		{
			hvt.push_back(4 + E_write);
			val_type p;
			cont.pop(p);
		}
		break;
	case 5: // swap(base&&)
		{
			hvt.push_back(5 + E_write);
			container_base_type tmp;
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			cont.swap(boost::move(tmp));
		}
		break;
	case 6: // swap(base&)
		{
			hvt.push_back(6 + E_write);
			container_base_type tmp;
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			cont.swap(tmp);
		}
		break;
	case 7: // swap(this&)
		{
			hvt.push_back(7 + E_write);
			container_type tmp;
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			tmp.push(create_val<val_type>());
			cont.swap(tmp);
		}
		break;
	case 8: //clear
		hvt.push_back(8 + E_write);
		cont.clear();
		break;
	default:
		assert(false);
	}

	if(cont.size() > E_max_elem)
	{
	    container_base_type tmp;
		cont.swap(tmp);
	}
}

template<typename Container>
void thread_foo_of_mutex(Container& cont, int size, hvt_type& hvt)
{
	boost::thread::id tid = boost::this_thread::get_id();
	for(int i = 0; i != size; ++i)
	{
		if(std::rand() % 2)
		{
			write_op(cont, hvt);
		}
		else
		{
			read_op(cont, hvt);
		}

		//std::cout << "thread " << tid << " run " << i << std::endl;
	}
}

void test_thread_safe_of_mutex(void)
{
	typedef boost::shared_ptr<int> val_type;
	typedef yggr::safe_container::safe_priority_queue<val_type> container_type;

	int loop_size = 100;
	int trd_size = 2;
	std::cout << "input loop_size:" << std::endl;
	std::cin >> loop_size;

	std::cout << "input thread_size:" << std::endl;
	std::cin >> trd_size;

	container_type cont;
	hvt_type hvt;

	boost::thread_group trds;

	for(int i = 0; i != trd_size; ++i)
	{
		trds.create_thread(boost::bind(&thread_foo_of_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt)));
	}
	trds.join_all();

	std::cout << "test_thread_safe_of_mutex end" << std::endl;
}

template<typename Container>
void thread_foo_of_shard_mutex(Container& cont, int size, hvt_type& hvt, bool mode)
{
	boost::thread::id tid = boost::this_thread::get_id();
	for(int i = 0; i != size; ++i)
	{
		if(mode)
		{
			write_op(cont, hvt);
		}
		else
		{
			read_op(cont, hvt);
		}

		//std::cout << "thread " << tid << " run " << i << std::endl;
	}
}

void thread_safe_test_of_shared_mutex(void)
{
	typedef boost::shared_ptr<int> val_type;
	typedef yggr::safe_container::safe_priority_queue<val_type, boost::shared_mutex> container_type;

	int loop_size = 100;
	int trd_size = 2;
	std::cout << "input loop_size:" << std::endl;
	std::cin >> loop_size;

	std::cout << "input thread_size:" << std::endl;
	std::cin >> trd_size;

	container_type cont;
	hvt_type hvt;

	boost::thread_group trds;

	trds.create_thread(boost::bind(&thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), true));

	for(int i = 0, isize = trd_size - 1; i != isize; ++i)
	{
		trds.create_thread(boost::bind(&thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), false));
	}
	trds.join_all();

	std::cout << "test_thread_safe_of_mutex end" << std::endl;
}

int main(int argc, char* argv[])
{
	debug_test();
	std::srand(std::time(0));

	test_thread_safe_of_mutex();
	thread_safe_test_of_shared_mutex();

	char cc = 0;
	std::cin >> cc;
	return 0;
}
