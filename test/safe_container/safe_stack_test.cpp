//safe_stack_test.cpp

#include <assert.h>
#include <test/wait_any_key/wait_any_key.hpp>
#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/thread/boost_thread.hpp>

#include <yggr/container/deque.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/options_deque.hpp>
#include <yggr/container/options_vector.hpp>

#include <yggr/safe_container/safe_stack.hpp>

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
	typedef Cont stack_type;
	typedef typename stack_type::value_type val_type;

	val_type arr[5] = {1, 2, 3, 4, 5};

	stack_type cont1;
	yggr_test_assert(cont1.empty());

	stack_type cont2(cont1.get_allocator());
	yggr_test_assert(cont2.empty());

	typename stack_type::container_type init_cont;
	stack_type cont3(init_cont);
	yggr_test_assert(cont3.empty());

	{
		typename stack_type::base_type base_cont4;
		base_cont4.push(10);
		base_cont4.push(20);

		stack_type cont4(boost::move(base_cont4));
		yggr_test_assert(cont4.size() == 2/* && base_cont4.size() == 0*/);
	}

	{
		typename stack_type::base_type base_cont4;
		base_cont4.push(10);
		base_cont4.push(20);

		stack_type cont4(boost::move(base_cont4), base_cont4.get_allocator());
		yggr_test_assert(cont4.size() == 2/* && base_cont4.size() == 0*/);
	}

	typename stack_type::base_type base_cont5;
	base_cont5.push(10);
	base_cont5.push(20);

	{
		stack_type cont5(base_cont5);
		yggr_test_assert(cont5.size() == base_cont5.size());
	}

	{
		stack_type cont5(base_cont5, base_cont5.get_allocator());
		yggr_test_assert(cont5.size() == base_cont5.size());
	}

	stack_type cont6(arr + 0, arr + 5);
	yggr_test_assert(cont6.size() == 5);
	yggr_test_assert(cont6.top() == 5);

	stack_type cont7(arr + 0, arr + 5, cont1.get_allocator());
	yggr_test_assert(cont7.size() == 5);
	yggr_test_assert(cont7.top() == 5);

	{
		stack_type cont11(base_cont5, base_cont5.get_allocator());
		stack_type base_cont11(base_cont5);

		// safe_copy_base
		yggr_test_assert(cont11 == base_cont11);
		yggr_test_assert(!(cont11 != base_cont11));
		yggr_test_assert(cont11 <= base_cont11);
		yggr_test_assert(!(cont11 < base_cont11));
		yggr_test_assert(cont11 >= base_cont11);
		yggr_test_assert(!(cont11 > base_cont11));
		
		// base_copy_safe
		yggr_test_assert(base_cont11 == cont11);
		yggr_test_assert(!(base_cont11 != cont11));
		yggr_test_assert(base_cont11 <= cont11);
		yggr_test_assert(!(base_cont11 < cont11));
		yggr_test_assert(base_cont11 >= cont11);
		yggr_test_assert(!(base_cont11 > cont11));

		// selfsafe_copy_selfsafe
		yggr_test_assert(cont11 == cont11);
		yggr_test_assert(!(cont11 != cont11));
		yggr_test_assert(cont11 <= cont11);
		yggr_test_assert(!(cont11 < cont11));
		yggr_test_assert(cont11 >= cont11);
		yggr_test_assert(!(cont11 > cont11));

		stack_type cont12(cont11);

		// safe_copy_safe
		yggr_test_assert(cont11 == cont12);
		yggr_test_assert(!(cont11 != cont12));
		yggr_test_assert(cont11 <= cont12);
		yggr_test_assert(!(cont11 < cont12));
		yggr_test_assert(cont11 >= cont12);
		yggr_test_assert(!(cont11 > cont12));

		stack_type cont13(cont11, cont11.get_allocator());

		// safe_copy_safe
		yggr_test_assert(cont11 == cont13);
		yggr_test_assert(!(cont11 != cont13));
		yggr_test_assert(cont11 <= cont13);
		yggr_test_assert(!(cont11 < cont13));
		yggr_test_assert(cont11 >= cont13);
		yggr_test_assert(!(cont11 > cont13));

		stack_type cont14(boost::move(cont12));

		// safe_copy_safe
		yggr_test_assert(cont11 == cont14);
		yggr_test_assert(!(cont11 != cont14));
		yggr_test_assert(cont11 <= cont14);
		yggr_test_assert(!(cont11 < cont14));
		yggr_test_assert(cont11 >= cont14);
		yggr_test_assert(!(cont11 > cont14));

		stack_type cont15(boost::move(cont14), cont14.get_allocator());

		// safe_copy_safe
		yggr_test_assert(cont11 == cont15);
		yggr_test_assert(!(cont11 != cont15));
		yggr_test_assert(cont11 <= cont15);
		yggr_test_assert(!(cont11 < cont15));
		yggr_test_assert(cont11 >= cont15);
		yggr_test_assert(!(cont11 > cont15));
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void test_capacity(void)
{
	typedef Cont stack_type;

	int arr[10] = {1, 2, 3, 4, 5};
	typename stack_type::container_type init_cont(arr + 0, arr + 10);

	stack_type cont1(init_cont);
	typename stack_type::base_type base_cont1(init_cont);

	yggr_test_assert(cont1.size() == base_cont1.size());

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

template<typename Cont>
void test_modifiers(void)
{
	typedef Cont stack_type;

	stack_type cont;
	typename stack_type::base_type base_cont;

	cont.push(10);
	cont.push(20);
	cont.emplace(30);

	cont.emplace();
	yggr_test_assert(cont.top() == 0);
	cont.top() = 50;
	{
		int tmp = 0;
		yggr_test_assert(cont.get_top(tmp));
		yggr_test_assert(tmp == 50);
	}
	cont.pop();
	

	int n = 100;
	cont.push(boost::move(n));

	int m = 0;
	yggr_test_assert(cont.get_top(m));
	yggr_test_assert(m == n);
	yggr_test_assert(cont.pop());

	int k = 0;
	yggr_test_assert(cont.pop(k));
	yggr_test_assert(k == 30);

	cont.clear();
	yggr_test_assert(cont.empty());
	yggr_test_assert(!cont.pop());
	int e = 0;
	yggr_test_assert(!cont.pop(e));

	//emplace
	{
		stack_type ins_cont;
		ins_cont.emplace();
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.top() == 0);
	}

	{
		stack_type ins_cont;
		yggr_test_assert(ins_cont.emplace(10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.top() == 10);
	}

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

template<typename Cont>
void test_swap(void)
{
	typedef Cont stack_type;

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	typename stack_type::container_type init_cont1(arr1 + 0, arr1 + 10);
	typename stack_type::container_type init_cont2(arr2 + 0, arr2 + 5);

	stack_type cont1(init_cont1);
	stack_type cont2(init_cont2);

	typename stack_type::base_type base_cont1(init_cont1);
	typename stack_type::base_type base_cont2(init_cont2);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() != base_cont2.size());

	typename stack_type::base_type base_cont3(base_cont1);
	typename stack_type::base_type base_cont4(base_cont2);

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

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

template<typename Cont>
void test_safe_other(void)
{
	typedef Cont stack_type;

	int arr1[10] = {1, 2, 3, 4, 5};

	typename stack_type::container_type init_cont1(arr1 + 0, arr1 + 10);

	stack_type cont(init_cont1);

	typename stack_type::base_type base_cont1 = cont.load();
	yggr_test_assert(base_cont1.top() == cont.top());

	typename stack_type::base_type base_cont2 = cont;
	yggr_test_assert(base_cont2.top() == cont.top());

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont>
void debug_test(void)
{
	typedef Cont stack_type;

	test_constructor<stack_type>();
	test_capacity<stack_type>();
	test_modifiers<stack_type>();
	test_swap<stack_type>();
	test_safe_other<stack_type>();
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
	case 2: // top
		{
			hvt.push_back(2 + E_read);
			val_type val;
			cont.get_top(val);
		}
		break;
	case 3: // get_allocator
		hvt.push_back(3 + E_read);
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
			tmp.emplace(create_val<val_type>());
			tmp.emplace();
			cont = boost::move(tmp);
			try {cont.top() = create_val<val_type>();} catch(const typename container_type::error_type&){};
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
			tmp.emplace(create_val<val_type>());
			tmp.emplace();
			cont = tmp;
			try {cont.top() = create_val<val_type>();} catch(const typename container_type::error_type&){};
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
			try {cont.top() = create_val<val_type>();} catch(const typename container_type::error_type&){};
			try {val_type m = cont.top();} catch(const typename container_type::error_type&){};
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
			tmp.emplace(create_val<val_type>());
			tmp.emplace();
			cont.swap(boost::move(tmp));
			try {cont.top() = create_val<val_type>();} catch(const typename container_type::error_type&){};
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
			tmp.emplace(create_val<val_type>());
			tmp.emplace();
			cont.swap(tmp);
			try {cont.top() = create_val<val_type>();} catch(const typename container_type::error_type&){};
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
			tmp.emplace(create_val<val_type>());
			tmp.emplace();
			cont.swap(tmp);
			try {cont.top() = create_val<val_type>();} catch(const typename container_type::error_type&){};
			try {val_type m = cont.top();} catch(const typename container_type::error_type&){};
		}
		break;
	case 8: //clear
		{
			hvt.push_back(8 + E_write);
			cont.clear();
			try {cont.top() = create_val<val_type>();} catch(const typename container_type::error_type&){};
			try {val_type m = cont.top();} catch(const typename container_type::error_type&){};
		}
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
void thread_safe_test_of_shared_mutex(void)
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

	std::cout << "thread_safe_test_of_shared_mutex end" << std::endl;
}

int main(int argc, char* argv[])
{
	{
		typedef yggr::safe_container::safe_stack<int> stack_type;
		debug_test<stack_type>();
	}

	// yggr

	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, yggr::vector<int> > stack_type;
		debug_test<stack_type>();
	}

	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, yggr::options_vector<int> > stack_type;
		debug_test<stack_type>();
	}

	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, yggr::deque<int> > stack_type;
		debug_test<stack_type>();
	}

	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, yggr::options_deque<int> > stack_type;
		debug_test<stack_type>();
	}

	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, yggr::list<int> > stack_type;
		debug_test<stack_type>();
	}

	// boost

	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, boost::container::vector<int> > stack_type;
		debug_test<stack_type>();
	}

	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, boost::container::deque<int> > stack_type;
		debug_test<stack_type>();
	}

	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, boost::container::list<int> > stack_type;
		debug_test<stack_type>();
	}

	// std

	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, std::vector<int> > stack_type;
		debug_test<stack_type>();
	}

#if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, std::deque<int> > stack_type;
		debug_test<stack_type>();
	}
#endif // #if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))

	{
		typedef yggr::safe_container::safe_stack<int, boost::mutex, std::list<int> > stack_type;
		debug_test<stack_type>();
	}

	std::srand(std::time(0));

	// mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type> stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

	// yggr
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, yggr::vector<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, yggr::options_vector<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, yggr::deque<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, yggr::options_deque<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, yggr::list<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

	// boost

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, boost::container::vector<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, boost::container::deque<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, boost::container::list<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

	// std

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, std::vector<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

#if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, std::deque<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}
#endif // #if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::mutex, std::list<val_type> > stack_type;
		test_thread_safe_of_mutex<stack_type>();
	}

	// shared_mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex> stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	// yggr

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, yggr::vector<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, yggr::options_vector<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, yggr::deque<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, yggr::options_deque<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, yggr::list<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	// boost

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, boost::container::vector<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, boost::container::deque<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, boost::container::list<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	// std

#if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, std::deque<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}
#endif // #if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, std::vector<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_stack<val_type, boost::shared_mutex, std::list<val_type> > stack_type;
		thread_safe_test_of_shared_mutex<stack_type>();
	}

	wait_any_key(argc, argv);
	return 0;
}
