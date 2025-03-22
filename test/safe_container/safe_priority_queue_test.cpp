//safe_priority_queue_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <functional>

#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/thread/boost_thread.hpp>

#include <yggr/container/deque.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/options_deque.hpp>
#include <yggr/container/options_vector.hpp>

#include <yggr/safe_container/safe_queue.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)

#define THREAD_TEST_USING_MANUAL() 0

#define THREAD_TEST_THREAD_SIZE() 10
#define THREAD_TEST_THREAD_LOOP_SIZE() 10

//-------------------------test base-------------------------

template<typename Cont>
void test_constructor(void)
{
	typedef Cont queue_type;
	typedef typename queue_type::value_type val_type;
	typedef typename queue_type::container_type inner_cont_type;

	val_type arr[5] = {1, 2, 3, 4, 5};

	queue_type cont1;
	yggr_test_assert(cont1.empty());

	queue_type cont2(cont1.value_comp());
	yggr_test_assert(cont2.empty());

	{
		queue_type cont2_2(cont1.get_allocator());
		yggr_test_assert(cont2_2.empty());
	}

	{
		queue_type cont2_2(cont1.value_comp(), cont1.get_allocator());
		yggr_test_assert(cont2_2.empty());
	}

	typename queue_type::container_type init_cont;
	queue_type cont3(init_cont);
	yggr_test_assert(cont3.empty());

	{
		typename queue_type::base_type base_cont4;
		base_cont4.push(10);
		base_cont4.push(20);

		queue_type cont4(boost::move(base_cont4.container()));
		yggr_test_assert(cont4.size() == 2);
	}

	{
		typename queue_type::base_type base_cont4;
		base_cont4.push(10);
		base_cont4.push(20);

		queue_type cont4(base_cont4.container());
		yggr_test_assert(cont4.size() == 2);
	}

	{
		typename queue_type::base_type base_cont4;
		base_cont4.push(10);
		base_cont4.push(20);

		queue_type cont4(base_cont4.value_comp(), boost::move(base_cont4.container()));
		yggr_test_assert(cont4.size() == 2);
	}

	{
		typename queue_type::base_type base_cont4;
		base_cont4.push(10);
		base_cont4.push(20);

		queue_type cont4(base_cont4.value_comp(), base_cont4.container());
		yggr_test_assert(cont4.size() == 2);
	}

	{
		typename queue_type::base_type base_cont4;
		base_cont4.push(10);
		base_cont4.push(20);

		queue_type cont4(base_cont4.value_comp(), boost::move(base_cont4.container()), base_cont4.get_allocator());
		yggr_test_assert(cont4.size() == 2);
	}

	{
		typename queue_type::base_type base_cont4;
		base_cont4.push(10);
		base_cont4.push(20);

		queue_type cont4(base_cont4.value_comp(), base_cont4.container(), base_cont4.get_allocator());
		yggr_test_assert(cont4.size() == 2);
	}

	{
		typename queue_type::base_type base_cont4;
		base_cont4.push(10);
		base_cont4.push(20);

		queue_type cont4(boost::move(base_cont4));
		yggr_test_assert(cont4.size() == 2);
	}

	{
		typename queue_type::base_type base_cont4;
		base_cont4.push(10);
		base_cont4.push(20);

		queue_type cont4(boost::move(base_cont4), base_cont4.get_allocator());
		yggr_test_assert(cont4.size() == 2);
	}

	typename queue_type::base_type base_cont5;
	base_cont5.push(10);
	base_cont5.push(20);

	{
		queue_type cont5(base_cont5);
		yggr_test_assert(cont5.size() == base_cont5.size());
	}

	{
		queue_type cont5(base_cont5, base_cont5.get_allocator());
		yggr_test_assert(cont5.size() == base_cont5.size());
	}

	{
		queue_type cont5(arr + 0, arr + 5);
		yggr_test_assert(cont5.size() == 5);
	}

	{
		queue_type cont5(arr + 0, arr + 5, base_cont5.get_allocator());
		yggr_test_assert(cont5.size() == 5);
	}

	{
		queue_type cont5(arr + 0, arr + 5, base_cont5.value_comp());
		yggr_test_assert(cont5.size() == 5);
	}

	{
		inner_cont_type inner_cont(arr + 0, arr + 5);
		queue_type cont5(arr + 0, arr + 5, base_cont5.value_comp(), boost::move(inner_cont));
		yggr_test_assert(cont5.size() == 10);
	}

	{
		inner_cont_type inner_cont(arr + 0, arr + 5);
		queue_type cont5(arr + 0, arr + 5, base_cont5.value_comp(), inner_cont);
		yggr_test_assert(cont5.size() == 10);
	}

	{
		inner_cont_type inner_cont(arr + 0, arr + 5);
		queue_type cont5(arr + 0, arr + 5, base_cont5.value_comp(), boost::move(inner_cont), inner_cont.get_allocator());
		yggr_test_assert(cont5.size() == 10);
	}

	{
		inner_cont_type inner_cont(arr + 0, arr + 5);
		queue_type cont5(arr + 0, arr + 5, base_cont5.value_comp(), inner_cont, inner_cont.get_allocator());
		yggr_test_assert(cont5.size() == 10);
	}

	{
		queue_type cont6(base_cont5);
		yggr_test_assert(cont6.size() == base_cont5.size());

		queue_type cont7(cont6);
		yggr_test_assert(cont7.size() == base_cont5.size());

		queue_type cont8(cont6, cont6.get_allocator());
		yggr_test_assert(cont8.size() == base_cont5.size());

		queue_type cont9(boost::move(cont7), cont7.get_allocator());
		yggr_test_assert(cont9.size() == base_cont5.size());

		queue_type cont10(boost::move(cont9), cont9.get_allocator());
		yggr_test_assert(cont10.size() == base_cont5.size());
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void test_capacity(void)
{
	typedef Cont queue_type;

	int arr[10] = {1, 2, 3, 4, 5};
	typename queue_type::container_type init_cont(arr + 0, arr + 10);

	queue_type cont1(init_cont);
	typename queue_type::base_type base_cont1(init_cont);

	yggr_test_assert(cont1.size() == base_cont1.size());

	int f = 0;
	yggr_test_assert(cont1.get_top(f));

	yggr_test_assert(f == base_cont1.top());

	cont1.clear();
	yggr_test_assert(!cont1.get_top(f));

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

template<typename Cont>
void test_modifiers(void)
{
	typedef Cont queue_type;

	queue_type cont;
	typename queue_type::base_type base_cont;

	cont.push(200);
	cont.push(20);
	cont.push(35);

	int n = 100;
	cont.push(boost::move(n));

	int f = 0;
	yggr_test_assert(cont.get_top(f));
	yggr_test_assert(f == 200);
	yggr_test_assert(cont.pop());

	int k = 0;
	yggr_test_assert(cont.pop(k));
	yggr_test_assert(k == 100);

	cont.clear();
	yggr_test_assert(cont.empty());
	yggr_test_assert(!cont.pop());
	int e = 0;
	yggr_test_assert(!cont.pop(e));

	cont.clear();
	int size = cont.size();
	cont.emplace();
	yggr_test_assert(cont.top() == 0);
	cont.emplace(100);

	yggr_test_assert(size + 2 == cont.size());

	//emplace
	{
		queue_type ins_cont;
		ins_cont.emplace();
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.top() == 0);
	}

	{
		queue_type ins_cont;
		yggr_test_assert(ins_cont.emplace(10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.top() == 10);
	}


	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

template<typename Cont>
void test_swap(void)
{
	typedef Cont queue_type;
	typedef typename queue_type::value_type val_type;

	val_type arr1[10] = {1, 2, 3, 4, 5};
	val_type arr2[10] = {6, 7, 8, 9, 0};

	typename queue_type::container_type init_cont1(arr1 + 0, arr1 + 10);
	typename queue_type::container_type init_cont2(arr2 + 0, arr2 + 5);

	queue_type cont1(init_cont1);
	queue_type cont2(init_cont2);

	typename queue_type::base_type base_cont1(init_cont1);
	typename queue_type::base_type base_cont2(init_cont2);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() != base_cont2.size());

	typename queue_type::base_type base_cont3(base_cont1);
	typename queue_type::base_type base_cont4(base_cont2);

	// ------------------ boost::swap test------------------------
	// base_type&& this_type&
	boost::swap(boost::move(base_cont4), cont1);
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	boost::swap(base_cont3, cont1);
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	boost::swap(cont1, boost::move(base_cont3));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	boost::swap(cont1, boost::move(base_cont4));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	boost::swap(cont1, cont2);
	boost::swap(cont1, cont2);

	// this_type&& this_type&
	boost::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	boost::swap(cont1, boost::move(cont2));

	// ------------------ std::swap test------------------------
	// base_type&& this_type&
	std::swap(boost::move(base_cont4), cont1);
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	std::swap(base_cont3, cont1);
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	std::swap(cont1, boost::move(base_cont3));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	std::swap(cont1, boost::move(base_cont4));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	std::swap(cont1, cont2);
	std::swap(cont1, cont2);

	// this_type&& this_type&
	std::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	std::swap(cont1, boost::move(cont2));

	// ------------------ yggr::swap test------------------------
	// base_type&& this_type&
	yggr::swap(boost::move(base_cont4), cont1);
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	yggr::swap(base_cont3, cont1);
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	yggr::swap(cont1, boost::move(base_cont3));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	yggr::swap(cont1, boost::move(base_cont4));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	yggr::swap(cont1, cont2);
	yggr::swap(cont1, cont2);

	// this_type&& this_type&
	yggr::swap(boost::move(cont1), cont2);

	// this_type& this_type&&
	yggr::swap(cont1, boost::move(cont2));

	std::cout << "---------------test_swap end-----------------" << std::endl;
}
//
//#endif // _DEBUG
//

template<typename Cont>
void test_safe_other(void)
{
	typedef Cont queue_type;
	typedef typename queue_type::value_type val_type;

	val_type arr1[10] = {1, 2, 3, 4, 5};

	typename queue_type::container_type init_cont1(arr1 + 0, arr1 + 10);

	queue_type cont(init_cont1);

	typename queue_type::base_type base_cont1 = cont.load();
	yggr_test_assert(base_cont1.top() == cont.top());

	typename queue_type::base_type base_cont2 = cont;
	yggr_test_assert(base_cont2.top() == cont.top());

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont>
void debug_test(void)
{
	typedef Cont queue_type;

	test_constructor<queue_type>();
	test_capacity<queue_type>();
	test_modifiers<queue_type>();
	test_swap<queue_type>();
	test_safe_other<queue_type>();
}

//-----------------------test safe---------------------------------------------

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <cstdlib>
#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/boost_thread.hpp>
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
	typedef typename container_type::value_type val_type;

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
			cont.get_top(val);
		}
		break;
	case 3: // get_allocator
		hvt.push_back(4 + E_read);
		cont.get_allocator();
		break;
	default:
		yggr_test_assert(false);
	}
}

template<typename Container>
void write_op(Container& cont, hvt_type& hvt)
{
	typedef Container container_type;
	typedef typename container_type::base_type container_base_type;
	typedef typename container_type::value_type val_type;
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
			try {val_type m = cont.top();} catch(const typename container_type::error_type&){};
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
			try {val_type m = cont.top();} catch(const typename container_type::error_type&){};
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
			try {val_type m = cont.top();} catch(const typename container_type::error_type&){};
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
			try {val_type m = cont.top();} catch(const typename container_type::error_type&){};
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
			//try {cont.top() = create_val<val_type>();} catch(const container_type::error_type&){};
			try {val_type m = cont.top();} catch(const typename container_type::error_type&){};
		}
		break;
	case 8: //clear
		hvt.push_back(8 + E_write);
		cont.clear();
		break;
	default:
		yggr_test_assert(false);
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

template<typename Cont>
void test_thread_safe_of_mutex(void)
{
	typedef Cont container_type;
	typedef typename container_type::value_type val_type;

	int loop_size = THREAD_TEST_THREAD_LOOP_SIZE();
	int trd_size = THREAD_TEST_THREAD_SIZE();

#if THREAD_TEST_USING_MANUAL()
	std::cout << "input loop_size:" << std::endl;
	std::cin >> loop_size;

	std::cout << "input thread_size:" << std::endl;
	std::cin >> trd_size;

#endif // THREAD_TEST_USING_MANUAL

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

template<typename Cont>
void test_thread_safe_of_shared_mutex(void)
{
	typedef Cont container_type;
	typedef typename container_type::value_type val_type;

	int loop_size = THREAD_TEST_THREAD_LOOP_SIZE();
	int trd_size = THREAD_TEST_THREAD_SIZE();

#if THREAD_TEST_USING_MANUAL()
	std::cout << "input loop_size:" << std::endl;
	std::cin >> loop_size;

	std::cout << "input thread_size:" << std::endl;
	std::cin >> trd_size;

#endif // THREAD_TEST_USING_MANUAL

	container_type cont;
	hvt_type hvt;

	boost::thread_group trds;

	trds.create_thread(boost::bind(&thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), true));

	for(int i = 0, isize = trd_size - 1; i != isize; ++i)
	{
		trds.create_thread(boost::bind(&thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), false));
	}
	trds.join_all();

	std::cout << "test_thread_safe_of_shared_mutex end" << std::endl;
}

int main(int argc, char* argv[])
{
	{
		typedef yggr::safe_container::safe_priority_queue<int> queue_type;
		debug_test<queue_type>();
	}

	{
		typedef yggr::safe_container::safe_priority_queue<int, boost::mutex, yggr::vector<int> > queue_type;
		debug_test<queue_type>();
	}

	{
		typedef yggr::safe_container::safe_priority_queue<int, boost::mutex, yggr::options_vector<int> > queue_type;
		debug_test<queue_type>();
	}

	{
		typedef yggr::safe_container::safe_priority_queue<int, boost::mutex, boost::container::vector<int> > queue_type;
		debug_test<queue_type>();
	}

	{
		typedef yggr::safe_container::safe_priority_queue<int, boost::mutex, std::vector<int> > queue_type;
		debug_test<queue_type>();
	}

	{
		typedef yggr::safe_container::safe_priority_queue<int, boost::mutex, yggr::deque<int> > queue_type;
		debug_test<queue_type>();
	}

	{
		typedef yggr::safe_container::safe_priority_queue<int, boost::mutex, yggr::options_deque<int> > queue_type;
		debug_test<queue_type>();
	}

	{
		typedef yggr::safe_container::safe_priority_queue<int, boost::mutex, boost::container::deque<int> > queue_type;
		debug_test<queue_type>();
	}

#if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	{
		typedef yggr::safe_container::safe_priority_queue<int, boost::mutex, std::deque<int> > queue_type;
		debug_test<queue_type>();
	}
#endif // #if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))

	std::srand(std::time(0));

	// mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type> queue_type;
		test_thread_safe_of_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::mutex, yggr::vector<val_type> > queue_type;
		test_thread_safe_of_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::mutex, yggr::options_vector<val_type> > queue_type;
		test_thread_safe_of_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::mutex, boost::container::vector<val_type> > queue_type;
		test_thread_safe_of_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::mutex, std::vector<val_type> > queue_type;
		test_thread_safe_of_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::mutex, yggr::deque<val_type> > queue_type;
		test_thread_safe_of_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::mutex, yggr::options_deque<val_type> > queue_type;
		test_thread_safe_of_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::mutex, boost::container::deque<val_type> > queue_type;
		test_thread_safe_of_mutex<queue_type>();
	}

#if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::mutex, std::deque<val_type> > queue_type;
		test_thread_safe_of_mutex<queue_type>();
	}
#endif // !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))

	// shared_mutex

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::shared_mutex> queue_type;
		test_thread_safe_of_shared_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::shared_mutex, yggr::vector<val_type> > queue_type;
		test_thread_safe_of_shared_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::shared_mutex, yggr::options_vector<val_type> > queue_type;
		test_thread_safe_of_shared_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::shared_mutex, boost::container::vector<val_type> > queue_type;
		test_thread_safe_of_shared_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::shared_mutex, std::vector<val_type> > queue_type;
		test_thread_safe_of_shared_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::shared_mutex, yggr::deque<val_type> > queue_type;
		test_thread_safe_of_shared_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::shared_mutex, yggr::options_deque<val_type> > queue_type;
		test_thread_safe_of_shared_mutex<queue_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::shared_mutex, boost::container::deque<val_type> > queue_type;
		test_thread_safe_of_shared_mutex<queue_type>();
	}

#if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_priority_queue<val_type, boost::shared_mutex, std::deque<val_type> > queue_type;
		test_thread_safe_of_shared_mutex<queue_type>();
	}

#endif // #if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))

	wait_any_key(argc, argv);
	return 0;
}
