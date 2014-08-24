//safe_multiset_test.cpp

//#define BOOST_THREAD_USE_DLL

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER
#include <iostream>
#include <yggr/safe_container/safe_set.hpp>
#include <boost/bind.hpp>
#include <assert.h>
#include <boost/swap.hpp>
#include <functional>
#include <boost/bind.hpp>

//-------------------------test base-------------------------
#ifdef _DEBUG

#define USE_STL_MULTISET 1

#if USE_STL_MULTISET
#	include <set>
	typedef yggr::safe_container::safe_multiset<int, boost::mutex,
													std::less<int>,
													std::allocator<int>,
													std::multiset> set_type;
#else
	typedef yggr::safe_container::safe_multiset<int> set_type;
#endif // USE_STL_MULTISET

//void check_values(const set_type::const_iterator iter, int n)
//{
//	assert(*iter == n);
//}

void check_range_equal(const set_type::base_type& l, const set_type::base_type& r)
{
	typedef set_type::base_type::const_iterator iter_type;
	set_type::size_type size = l.size();
	assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		assert(*i == *j);
	}
}

void test_constructor(void)
{
	set_type cont1;
	assert(cont1.empty());

	set_type cont2(cont1.key_comp());
	assert(cont2.empty());

	set_type cont3(cont1.get_allocator());
	assert(cont3.empty());

	set_type cont4(cont3.key_comp(), cont2.get_allocator());
	assert(cont4.empty());

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont5(arr, arr + 10);
	assert(cont5.size() == 10);

	set_type cont6(arr, arr + 10, cont5.key_comp());
	assert(cont6.size() == 10);

	set_type cont7(arr, arr + 10, cont5.get_allocator());
	assert(cont7.size() == 10);

	set_type cont8(arr, arr + 10, cont5.key_comp(), cont5.get_allocator());
	assert(cont8.size() == 10);

	set_type::base_type base_cont9(arr, arr + 10);
	set_type cont9(boost::move(base_cont9));
	assert(cont9.size() == 10);

	set_type::base_type base_cont10(arr, arr + 10);
	set_type cont10(base_cont10);
	assert(cont10.size() == 10 && base_cont10.size() == 10);
	cont10.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont10)));

	set_type::base_type base_cont11(arr, arr + 10);
	set_type cont11(boost::move(base_cont11), base_cont11.get_allocator());
	assert(cont11.size() == 10);

	set_type::base_type base_cont12(arr, arr + 10);
	set_type cont12(base_cont12, base_cont12.get_allocator());
	assert(cont12.size() == 10 && base_cont12.size() == 10);
	cont12.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont12)));

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

void test_capacity(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont1(arr, arr + 10);
	set_type::base_type base_cont1(arr, arr + 10);

	assert(cont1.size() == base_cont1.size());
	assert(cont1.max_size() == base_cont1.max_size());

	assert(cont1.empty() == base_cont1.empty());

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

void check_last_element(set_type::iterator& iter, int check)
{
	assert(*iter == check);
}

void test_modifiers(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont;
	set_type::base_type base_cont;

	cont.insert(100);
	base_cont.insert(100);

	int n = 90;
	cont.insert(boost::move(n));
	base_cont.insert(90);

	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	cont.insert(200, boost::bind(&check_last_element, _1, 200));
	base_cont.insert(200);

	int m = 300;
	cont.insert(boost::move(m), boost::bind(&check_last_element, _1, 300));
	base_cont.insert(300);

	assert(cont.size() == base_cont.size());

	cont.erase(200);
	base_cont.erase(200);

	assert(cont.size() == base_cont.size());

	cont.swap(base_cont);
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));
	cont.swap(base_cont);
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	set_type::base_type base_cont2(base_cont);

	cont.swap(boost::move(base_cont2));
	assert(!cont.empty() && !base_cont2.empty());
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	set_type cont3(base_cont);

	cont.swap(cont3);
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));
	cont3.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	cont.clear();
	assert(cont.empty());

	int arr2[10] = {1, 2, 2, 2, 1};
	set_type cont2(arr2, arr2 + 10);
	cont2.replace(1, 5);
	cont2.replace(2, 0);
	cont2.replace(0, 10);

	assert(cont2.count(1) == 0);
	assert(cont2.count(5) == 2);
	assert(cont2.count(10) == 3 + 5);


	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

void test_operations(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr, arr + 10);
	assert(cont.count(0) == 5);
	std::cout << "---------------test_operations end-----------------" << std::endl;
}

void test_safe_other(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr, arr + 10);
	assert(cont.is_exists(1));
	assert(cont.is_exists(0));
	assert(!cont.is_exists(100));

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

void test_swap(void)
{
	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	set_type cont1(arr1, arr1 + 10);
	set_type cont2(arr2, arr2 + 5);

	set_type::base_type base_cont1(arr1, arr1 + 10);
	set_type::base_type base_cont2(arr2, arr2 + 5);
	assert(cont1.size() == base_cont1.size());
	assert(cont1.size() != base_cont2.size());


	set_type::base_type base_cont3(base_cont1);
	set_type::base_type base_cont4(base_cont2);

	// ------------------ boost::swap test------------------------
	// base_type&& this_type&
	boost::swap(boost::move(base_cont4), cont1);
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont2)));
	assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	boost::swap(base_cont3, cont1);
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont1)));
	assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	boost::swap(cont1, boost::move(base_cont3));
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont2)));
	assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	boost::swap(cont1, boost::move(base_cont4));
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont1)));
	assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	boost::swap(cont1, cont2);
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont2)));
	cont2.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont1)));

	boost::swap(cont1, cont2);
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont1)));
	cont2.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont2)));

	// ------------------ std::swap test------------------------
	// base_type&& this_type&
	std::swap(boost::move(base_cont4), cont1);
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont2)));
	assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	std::swap(base_cont3, cont1);
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont1)));
	assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	std::swap(cont1, boost::move(base_cont3));
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont2)));
	assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	std::swap(cont1, boost::move(base_cont4));
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont1)));
	assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	std::swap(cont1, cont2);
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont2)));
	cont2.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont1)));

	std::swap(cont1, cont2);
	cont1.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont1)));
	cont2.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont2)));

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

void return_handler(int& count)
{
	++count;
}

template<typename Iter, typename Cmp>
bool cdt_handler(Iter iter, const int& val, Cmp cmp, int& count)
{
	if(cmp(*iter, val))
	{
		++count;
		return true;
	}

	return false;
}

template<typename Iter>
void ref_all_check_handler(Iter iter, const set_type::base_type& r)
{
	set_type::base_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	assert(i != r.end());

#if !USE_STL_MULTISET
	*iter = *i; // multiset iter is cont_iter
#endif // USE_STL_MULTISET
}

template<typename Iter>
void cref_all_check_handler(Iter iter, const set_type::base_type& r)
{
	set_type::base_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	assert(i != r.end());
}

void check_handler_eins(const set_type::base_type& l, const set_type::base_type& r)
{
	typedef set_type::const_iterator iter_type;
	set_type::size_type size = l.size();
	assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		assert(*i == *j);
	}
}

template<typename Handler>
void check_handler_zwei(const set_type::base_type& l, const set_type::base_type& r, const Handler& ret_handler)
{
	typedef set_type::const_iterator iter_type;
	set_type::size_type size = l.size();
	assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		assert(*i == *j);
	}
	ret_handler();
}

void set_handler_eins(set_type::base_type& l, const set_type::base_type& r)
{
	typedef set_type::const_iterator iter_type;
	if(!l.empty())
	{
		assert(l.size() == r.size());
		for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
		{
			assert(*i != *j);
		}
	}
	l = r;
	{
		for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
		{
			assert(*i == *j);
		}
	}
}

template<typename Handler>
void set_handler_zwei(set_type::base_type& l, const set_type::base_type& r, const Handler& ret_handler)
{
	typedef set_type::const_iterator iter_type;
	if(!l.empty())
	{
		assert(l.size() == r.size());
		for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
		{
			assert(*i != *j);
		}
	}
	l = r;
	{
		for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
		{
			assert(*i == *j);
		}
	}

	ret_handler();
}

template<typename Handler>
void return_handler_call_helper(set_type& cont, const set_type::base_type& r, const Handler& ret_handler)
{
	typedef Handler ret_handler_type;
	cont.use_handler(boost::bind(&set_handler_zwei<ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Handler>
void return_handler_call_helper(const set_type& cont, const set_type::base_type& r, const Handler& ret_handler)
{
	typedef Handler ret_handler_type;
	cont.use_handler(boost::bind(&check_handler_zwei<ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

void test_user_handler(void)
{
	int arr1[5] = {1, 2, 3, 4, 5};
	int arr2[5] = {6, 7, 8, 9, 10};

	set_type::base_type base_cont1(arr1, arr1 + 5);
	set_type::base_type base_cont2(arr2, arr2 + 5);

	set_type cont;
	set_type& ref_cont = cont;
	const set_type& cref_cont = cont;

	ref_cont.use_handler(boost::bind(&set_handler_eins, _1, boost::cref(base_cont1)));
	cref_cont.use_handler(boost::bind(&check_handler_eins, _1, boost::cref(base_cont1)));

	int ref_count = 0;
	return_handler_call_helper(ref_cont, base_cont2, boost::bind(&return_handler, boost::ref(ref_count)));
	assert(ref_count == 1);

	int cref_count = 0;
	return_handler_call_helper(cref_cont, base_cont2, boost::bind(&return_handler, boost::ref(cref_count)));
	assert(cref_count == 1);

	ref_cont.use_handler_of_all(boost::bind(&ref_all_check_handler<set_type::iterator>,
											_1, boost::cref(base_cont2)));
	cref_cont.use_handler_of_all(boost::bind(&cref_all_check_handler<set_type::const_iterator>,
												_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.use_handler_of_all(boost::bind(&ref_all_check_handler<set_type::iterator>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<set_type::iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(ref_count)));
	assert(ref_count == 2);

	cref_count = 0;
	cref_cont.use_handler_of_all(boost::bind(&cref_all_check_handler<set_type::const_iterator>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<set_type::const_iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(cref_count)));
	assert(cref_count == 2);

	ref_cont.use_handler_of_reverse_all(boost::bind(&ref_all_check_handler<set_type::reverse_iterator>,
													_1, boost::cref(base_cont2)));
	cref_cont.use_handler_of_reverse_all(boost::bind(&cref_all_check_handler<set_type::const_reverse_iterator>,
													_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.use_handler_of_reverse_all(boost::bind(&ref_all_check_handler<set_type::reverse_iterator>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<set_type::reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(ref_count)));
	assert(ref_count == 2);

	cref_count = 0;
	cref_cont.use_handler_of_reverse_all(boost::bind(&cref_all_check_handler<set_type::const_reverse_iterator>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<set_type::const_reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(cref_count)));
	assert(cref_count == 2);


	std::cout << "---------------test_use_handler end-----------------" << std::endl;
}

#endif // _DEBUG

void debug_test(void)
{
#ifdef _DEBUG
	test_constructor();
	test_capacity();
	test_modifiers();
	test_operations();
	test_safe_other();
	test_swap();
	test_user_handler();
#endif // _DEBUG
}


//-----------------------test safe---------------------------------------------

#include <boost/shared_ptr.hpp>
#include <cstdlib>
#include <boost/thread.hpp>
#include <yggr/safe_container/safe_vector.hpp>

typedef yggr::safe_container::safe_vector<int, boost::mutex, std::allocator<int>, std::vector> hvt_type;

enum
{
	E_read = 0,
	E_write = 100,
	E_max_elem = 20000
};

template<typename Container>
void push_handler(typename Container::iterator&, int& count)
{
	++count;
}

template<typename Container>
void ref_handler(typename Container::base_type& cont)
{
	if(cont.size() % 2 == 0)
	{
		cont.erase(cont.begin(), cont.end());
	}
}
template<typename Container>
void cref_handler(const typename Container::base_type& cont)
{
	int n = cont.size();
}

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
	int op = std::rand() % 9;
	switch(op)
	{
	case 0: // size
		hvt.push_back(0 + E_read);
		cont.size();
		break;
	case 1: // max_size
		hvt.push_back(1 + E_read);
		cont.max_size();
		break;
	case 2: // empty
		hvt.push_back(2 + E_read);
		cont.empty();
		break;
	case 3: // key_comp
		hvt.push_back(3 + E_read);
		cont.key_comp();
		break;
	case 4: // value_comp
		hvt.push_back(4 + E_read);
		cont.value_comp();
		break;
	case 5: // get_allocator
		hvt.push_back(5 + E_read);
		cont.get_allocator();
		break;
	case 6: // is_exisits
		{
			hvt.push_back(6 + E_read);
			val_type p;
			cont.is_exists(p);
			cont.is_exists(create_val<val_type>());
		}
		break;
	case 7: // use_handler const
		hvt.push_back(7 + E_read);
		cont.use_handler(boost::bind(&cref_handler<container_type>, _1));
		break;
	case 8: // count
		hvt.push_back(8 + E_read);
		cont.count(create_val<val_type>());
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

	int op = std::rand() % 13;
	//int op = 2;
	switch(op)
	{
	case 0: // operator= &&
		{
			hvt.push_back(0 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont = boost::move(tmp);
		}
		break;
	case 1: // operator = const &
		{
			hvt.push_back(1 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont = tmp;
		}
		break;
	case 2: // insert
		hvt.push_back(2 + E_write);
		cont.insert(create_val<val_type>());
		break;
	case 3: // insert(&&)
		hvt.push_back(3 + E_write);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		cont.insert(boost::move(create_val<val_type>()));
#else
        {
            val_type val(create_val<val_type>());
            cont.insert(boost::move(val));
        }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		break;
	case 4: // insert(val&&, handler)
		{
			hvt.push_back(4 + E_write);
			int count = 0;
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			cont.insert(boost::move(create_val<val_type>()),
							boost::bind(&push_handler<container_type>, _1, boost::ref(count)));
#else
            val_type val(create_val<val_type>());
            cont.insert(boost::move(val),
							boost::bind(&push_handler<container_type>, _1, boost::ref(count)));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
			assert(count == 1);
		}
		break;
	case 5: // // push_back(const val&, handler)
		{
			hvt.push_back(5 + E_write);
			int count = 0;
			cont.insert(create_val<val_type>(),
							boost::bind(&push_handler<container_type>, _1, boost::ref(count)));
			assert(count == 1);
		}
		break;
	case 6: // pop_back
		{
			hvt.push_back(6 + E_write);
			val_type p;
			cont.erase(p);
		}
		break;
	case 7: // swap(base&&)
		{
			hvt.push_back(7 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont.swap(boost::move(tmp));
		}
		break;
	case 8: // swap(base&)
		{
			hvt.push_back(8 + E_write);
			container_base_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont.swap(tmp);
		}
		break;
	case 9: // swap(this&)
		{
			hvt.push_back(9 + E_write);
			container_type tmp;
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			tmp.insert(create_val<val_type>());
			cont.swap(tmp);
		}
		break;
	case 10: //clear
		hvt.push_back(10 + E_write);
		cont.clear();
		break;
	case 11: // use handler
		hvt.push_back(11 + E_write);
		cont.use_handler(boost::bind(&ref_handler<container_type>, _1));
		break;
	case 12:
		hvt.push_back(12 + E_write);
		cont.replace(create_val<val_type>(), create_val<val_type>());
		break;
	default:
		assert(false);
	}

	if(cont.size() > E_max_elem)
	{
		cont.swap(container_base_type());
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

		std::cout << "thread " << tid << " run " << i << std::endl;
	}
}

void test_thread_safe_of_mutex(void)
{
	typedef boost::shared_ptr<int> val_type;
	typedef yggr::safe_container::safe_multiset<val_type> container_type;

	int loop_size = 1000;
	int trd_size = 10;
	std::cout << "input loop_size:" << std::endl;
	std::cin >> loop_size;

	std::cout << "input thread_size:" << std::endl;
	std::cin >> trd_size;

	container_type cont;
	hvt_type hvt;

	boost::thread_group trds;

	for(int i = 0; i != trd_size; ++i)
	{
		trds.create_thread(boost::bind(thread_foo_of_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt)));
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

		std::cout << "thread " << tid << " run " << i << std::endl;
	}
}

void thread_safe_test_of_shared_mutex(void)
{
	typedef boost::shared_ptr<int> val_type;
	typedef yggr::safe_container::safe_multiset<val_type, boost::shared_mutex> container_type;

	int loop_size = 1000;
	int trd_size = 20;
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
		trds.create_thread(boost::bind(thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), false));
	}
	trds.join_all();

	std::cout << "thread_safe_test_of_shared_mutex end" << std::endl;
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
