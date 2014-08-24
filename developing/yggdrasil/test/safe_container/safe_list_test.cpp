//safe_list_test.cpp

//#define BOOST_THREAD_USE_DLL

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER
#include <iostream>
#include <yggr/safe_container/safe_list.hpp>
#include <boost/bind.hpp>
#include <assert.h>
#include <boost/swap.hpp>

//-------------------------test base-------------------------
#ifdef _DEBUG

#define TEST_STL_LIST 1
#if TEST_STL_LIST
#   include <list>
    typedef yggr::safe_container::safe_list<int,  boost::mutex, std::allocator<int>, std::list> list_type;
#else
	typedef yggr::safe_container::safe_list<int> list_type;
#endif //

void check_values(const list_type::const_iterator iter, int n)
{
	assert(*iter == n);
}

void test_constructor(void)
{
	list_type cont1;
	assert(cont1.empty());

	list_type cont2(cont1.get_allocator());
	assert(cont2.empty());

	list_type cont3(10, 1);
	cont3.use_handler_of_all(boost::bind(&check_values, _1, 1));

	list_type cont4(10, 1, cont2.get_allocator());
	cont4.use_handler_of_all(boost::bind(&check_values, _1, 1));

	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont5(arr, arr + 10);
	assert(cont5.size() == 10);

	list_type cont6(arr, arr + 10, cont5.get_allocator());
	assert(cont6.size() == 10);

	list_type::base_type base_cont7(arr, arr + 10);
	list_type cont7(base_cont7);
	assert(cont7.size() == 10 && base_cont7.size() == 10);

	list_type::base_type base_cont8(arr, arr + 10);
	list_type cont8(boost::move(base_cont8));
	assert(cont8.size() == 10);

	list_type::base_type base_cont9(arr, arr + 10);
	list_type cont9(base_cont9, cont7.get_allocator());
	assert(cont9.size() == 10 && base_cont9.size() == 10);

	list_type::base_type base_cont10(arr, arr + 10);
	list_type cont10(boost::move(base_cont10), base_cont9.get_allocator());
	assert(cont10.size() == 10);

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

void check_range_equal(const list_type::base_type& l, const list_type::base_type& r)
{
	typedef list_type::base_type::const_iterator iter_type;
	list_type::size_type size = l.size();
	assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		assert(*i == *j);
	}
}

void test_capacity(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont1(arr, arr + 10);
	list_type::base_type base_cont1(arr, arr + 10);

	assert(cont1.size() == base_cont1.size());
	assert(cont1.max_size() == base_cont1.max_size());

	cont1.resize(20);
	base_cont1.resize(20);
	assert(cont1.size() == base_cont1.size());
	assert(cont1.max_size() == base_cont1.max_size());

	assert(cont1.empty() == base_cont1.empty());

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

void test_element_access(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont(arr, arr + 10);
	list_type::base_type base_cont(arr, arr + 10);

	int f = 0, b = 0;
	list_type cont2;
	assert(!cont2.front(f));
	assert(!cont2.back(b));

	assert(cont.front(f));
	assert(cont.back(b));
	assert(f == base_cont.front());
	assert(b == base_cont.back());

	std::cout << "---------------test_element_access end-----------------" << std::endl;
}

void check_last_element(const int& last_val, int check)
{
	assert(last_val == check);
}

void test_modifiers(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont;
	list_type::base_type base_cont;

	cont.assign(7, 100);
	base_cont.assign(7, 100);

	assert(cont.size() == base_cont.size());
	cont.use_handler_of_all(boost::bind(&check_values, _1, 100));

	cont.assign(arr, arr + 10);
	base_cont.assign(arr, arr + 10);

	assert(cont.size() == base_cont.size());

	{
		cont.push_back(200);
		base_cont.push_back(200);

		int n = 300;
		cont.push_back(boost::move(n));
		base_cont.push_back(300);
		assert(cont.size() == base_cont.size());

		cont.push_back(1000, boost::bind(&check_last_element, _1, 1000));
		base_cont.push_back(1000);

		int m = 2000;
		cont.push_back(boost::move(m), boost::bind(&check_last_element, _1, 2000));
		base_cont.push_back(2000);
	}

	assert(cont.size() == base_cont.size());

	{
		cont.push_front(600);
		base_cont.push_front(600);

		int n = 500;
		cont.push_front(boost::move(n));
		base_cont.push_front(500);
		assert(cont.size() == base_cont.size());

		cont.push_front(5000, boost::bind(&check_last_element, _1, 5000));
		base_cont.push_front(5000);

		int m = 6000;
		cont.push_front(boost::move(m), boost::bind(&check_last_element, _1, 6000));
		base_cont.push_front(6000);
	}

	assert(cont.size() == base_cont.size());

	{
		int b = 0;
		assert(cont.pop_back(b));
		assert(b == 2000);
		base_cont.pop_back();
		assert(cont.size() == base_cont.size());

		list_type cont2;
		assert(!cont2.pop_back(b));
		assert(b == 2000);
	}

	{
		int b = 0;
		assert(cont.pop_front(b));
		assert(b == 6000);
		base_cont.pop_front();
		assert(cont.size() == base_cont.size());

		list_type cont2;
		assert(!cont2.pop_front(b));
		assert(b == 6000);
	}

	cont.swap(base_cont);
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));
	cont.swap(base_cont);
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	list_type::base_type base_cont2(base_cont);

    assert(!cont.empty());
    assert(!base_cont2.empty());
	cont.swap(boost::move(base_cont2));
	assert(!cont.empty() && !base_cont2.empty());
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	list_type cont3(base_cont);

	cont.swap(cont3);
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));
	cont3.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	cont.clear();
	assert(cont.empty());

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

void test_operators(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont(arr, arr + 10);
	list_type::base_type base_cont(arr, arr + 10);

	cont.remove(3);
	base_cont.remove(3);
	assert(cont.size() == base_cont.size());

	cont.remove(100);
	base_cont.remove(100);
	assert(cont.size() == base_cont.size());

	std::less<int> lss;
	cont.remove_if(boost::bind(&std::less<int>::operator(), &lss, _1, 2));
	base_cont.remove_if(boost::bind(&std::less<int>::operator(), &lss, _1, 2));

	assert(cont.size() == base_cont.size());

	cont.remove_if(boost::bind(&std::less<int>::operator(), &lss, _1, 0));
	base_cont.remove_if(boost::bind(&std::less<int>::operator(), &lss, _1, 0));

	assert(cont.size() == base_cont.size());
	cont.push_back(100);
	cont.push_back(100);

	base_cont.push_back(100);
	base_cont.push_back(100);

	assert(cont.size() == base_cont.size());
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	cont.unique();
	base_cont.unique();

	assert(cont.size() == base_cont.size());
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	cont.push_back(1000);
	cont.push_back(1001);
	cont.push_back(1002);

	base_cont.push_back(1000);
	base_cont.push_back(1001);
	base_cont.push_back(1002);

	cont.unique(std::less<int>());
	base_cont.unique(std::less<int>());

	assert(cont.size() == base_cont.size());
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	list_type::base_type base_cont1(arr, arr + 5);
	list_type::base_type base_cont2(arr, arr + 5);
	list_type::base_type base_cont3(arr, arr + 5);
	list_type::base_type base_cont4(arr, arr + 5);

	cont.merge(boost::move(base_cont1));
	cont.merge(base_cont2);

	base_cont.merge(boost::move(base_cont3));
	base_cont.merge(base_cont4);

	assert(base_cont1.size() == base_cont3.size());
	assert(base_cont2.size() == base_cont4.size());

	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	list_type::base_type base_cont5(arr, arr + 5);
	list_type::base_type base_cont6(arr, arr + 5);
	list_type::base_type base_cont7(arr, arr + 5);
	list_type::base_type base_cont8(arr, arr + 5);


#if !( defined(_MSC_VER) && (TEST_STL_LIST) ) // microsoft std::list::merge don't support unordered's list

	std::not_equal_to<int> foo_ne;

	list_type::base_type base_tmp_cont = base_cont;

	cont.merge(boost::move(base_cont5), boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));
	cont.merge(base_cont6, boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

	base_cont.merge(boost::move(base_cont7), boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));
	base_cont.merge(base_cont8, boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

	assert(base_cont5.size() == base_cont7.size());
	assert(base_cont6.size() == base_cont8.size());

    assert(cont.size() == base_cont.size());
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	list_type tmp_cont1(arr, arr + 5);
	list_type tmp_cont2(arr, arr + 5);
	list_type::base_type tmp_base_cont1(arr, arr + 5);
	list_type::base_type tmp_base_cont2(arr, arr + 5);

	cont.merge(tmp_cont1);
	cont.merge(tmp_cont2, boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 2));

	base_cont.merge(tmp_base_cont1);
	base_cont.merge(tmp_base_cont2, boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 2));

	assert(tmp_cont1.size() == tmp_base_cont1.size());
	assert(tmp_cont2.size() == tmp_base_cont2.size());

	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));
#endif // defined(_MSC_VER) && (TEST_STL_LIST)

	cont.sort();
	base_cont.sort();

	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	cont.sort(std::greater<int>());
	base_cont.sort(std::greater<int>());

	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	cont.reverse();
	base_cont.reverse();
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));


	std::cout << "---------------test_operators end-----------------" << std::endl;
}

void test_safe_other(void)
{
	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont(arr, arr + 10);
	assert(cont.is_exists(1));
	assert(cont.is_exists(0));
	assert(!cont.is_exists(100));

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

void test_swap(void)
{
	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	list_type cont1(arr1, arr1 + 10);
	list_type cont2(arr2, arr2 + 5);

	list_type::base_type base_cont1(arr1, arr1 + 10);
	list_type::base_type base_cont2(arr2, arr2 + 5);
	assert(cont1.size() == base_cont1.size());
	assert(cont1.size() != base_cont2.size());


	list_type::base_type base_cont3(base_cont1);
	list_type::base_type base_cont4(base_cont2);

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
void ref_all_check_handler(Iter iter, const list_type::base_type& r)
{
	list_type::base_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	assert(i != r.end());
	*iter = *i;
}

template<typename Iter>
void cref_all_check_handler(Iter iter, const list_type::base_type& r)
{
	list_type::base_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	assert(i != r.end());
}

void check_handler_eins(const list_type::base_type& l, const list_type::base_type& r)
{
	typedef list_type::const_iterator iter_type;
	list_type::size_type size = l.size();
	assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		assert(*i == *j);
	}
}

template<typename Handler>
void check_handler_zwei(const list_type::base_type& l, const list_type::base_type& r, const Handler& ret_handler)
{
	typedef list_type::const_iterator iter_type;
	list_type::size_type size = l.size();
	assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		assert(*i == *j);
	}
	ret_handler();
}

void set_handler_eins(list_type::base_type& l, const list_type::base_type& r)
{
	typedef list_type::const_iterator iter_type;
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
void set_handler_zwei(list_type::base_type& l, const list_type::base_type& r, const Handler& ret_handler)
{
	typedef list_type::const_iterator iter_type;
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
void return_handler_call_helper(list_type& cont, const list_type::base_type& r, const Handler& ret_handler)
{
	typedef Handler ret_handler_type;
	cont.use_handler(boost::bind(&set_handler_zwei<ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Handler>
void return_handler_call_helper(const list_type& cont, const list_type::base_type& r, const Handler& ret_handler)
{
	typedef Handler ret_handler_type;
	cont.use_handler(boost::bind(&check_handler_zwei<ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

void test_user_handler(void)
{
	int arr1[5] = {1, 2, 3, 4, 5};
	int arr2[5] = {6, 7, 8, 9, 10};

	list_type::base_type base_cont1(arr1, arr1 + 5);
	list_type::base_type base_cont2(arr2, arr2 + 5);

	list_type cont;
	list_type& ref_cont = cont;
	const list_type& cref_cont = cont;

	ref_cont.use_handler(boost::bind(&set_handler_eins, _1, boost::cref(base_cont1)));
	cref_cont.use_handler(boost::bind(&check_handler_eins, _1, boost::cref(base_cont1)));

	int ref_count = 0;
	return_handler_call_helper(ref_cont, base_cont2, boost::bind(&return_handler, boost::ref(ref_count)));
	assert(ref_count == 1);

	int cref_count = 0;
	return_handler_call_helper(cref_cont, base_cont2, boost::bind(&return_handler, boost::ref(cref_count)));
	assert(cref_count == 1);

	ref_cont.use_handler_of_all(boost::bind(&ref_all_check_handler<list_type::iterator>,
											_1, boost::cref(base_cont2)));
	cref_cont.use_handler_of_all(boost::bind(&cref_all_check_handler<list_type::const_iterator>,
												_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.use_handler_of_all(boost::bind(&ref_all_check_handler<list_type::iterator>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<list_type::iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(ref_count)));
	assert(ref_count == 2);

	cref_count = 0;
	cref_cont.use_handler_of_all(boost::bind(&cref_all_check_handler<list_type::const_iterator>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<list_type::const_iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(cref_count)));
	assert(cref_count == 2);

	ref_cont.use_handler_of_reverse_all(boost::bind(&ref_all_check_handler<list_type::reverse_iterator>,
													_1, boost::cref(base_cont2)));
	cref_cont.use_handler_of_reverse_all(boost::bind(&cref_all_check_handler<list_type::const_reverse_iterator>,
													_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.use_handler_of_reverse_all(boost::bind(&ref_all_check_handler<list_type::reverse_iterator>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<list_type::reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(ref_count)));
	assert(ref_count == 2);

	cref_count = 0;
	cref_cont.use_handler_of_reverse_all(boost::bind(&cref_all_check_handler<list_type::const_reverse_iterator>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<list_type::const_reverse_iterator, std::less<int> >,
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
	test_element_access();
	test_modifiers();
	test_operators();
	test_safe_other();
	test_swap();
	test_user_handler();
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
void push_handler(Val&, int& count)
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
	int op = std::rand() % 8;
	//int op = 3;
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
	case 3: // front
		{
			hvt.push_back(3 + E_read);
			val_type p;
			cont.front(p);
		}
		break;
	case 4: // back
		{
			hvt.push_back(4 + E_read);
			val_type p;
			cont.back(p);
		}
		break;
	case 5: // get_allocator
		hvt.push_back(5 + E_read);
		cont.get_allocator();
		break;
	case 6: // is_exisits
		{
			hvt.push_back(6 + E_read);
			int s = cont.size();
			val_type p;
			cont.front(p); // point don't geted
			cont.is_exists(p);
			cont.is_exists(create_val<val_type>());
		}
		break;
	case 7: // use_handler const
		hvt.push_back(7 + E_read);
		cont.use_handler(boost::bind(&cref_handler<container_type>, _1));
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

	int op = std::rand() % 26;
	//int op = 2;
	switch(op)
	{
	case 0: // operator= &&
		{
			hvt.push_back(0 + E_write);
			container_base_type tmp;
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			cont = boost::move(tmp);
		}
		break;
	case 1: // operator = const &
		{
			hvt.push_back(1 + E_write);
			container_base_type tmp;
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			cont = tmp;
		}
		break;
	case 2: // resize
		hvt.push_back(2 + E_write);
		cont.resize(std::rand() % 100);
		break;
	case 3: // assign(iter, iter)
		{
			hvt.push_back(3 + E_write);
			container_base_type tmp;
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			cont.assign(tmp.begin(), tmp.end());
		}
		break;
	case 4: // assign(size, val)
		{
			hvt.push_back(4 + E_write);
			cont.assign(rand() % 20, create_val<val_type>());
		}
		break;
	case 5: // push_back(int &&)
		hvt.push_back(5 + E_write);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		cont.push_front(boost::move(create_val<val_type>()));
#else
        {
            val_type val(create_val<val_type>());
            cont.push_front(boost::move(val));
        }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		break;
	case 6: // push_back
		hvt.push_back(6 + E_write);
		cont.push_front(create_val<val_type>());
		break;
	case 7: // push_back(val&&, handler)
		{
			hvt.push_back(7 + E_write);
			int count = 0;
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			cont.push_front(boost::move(create_val<val_type>()),
							boost::bind(&push_handler<val_type>, _1, boost::ref(count)));
#else
            {
                val_type val(create_val<val_type>());
                cont.push_front(boost::move(val),
                                boost::bind(&push_handler<val_type>, _1, boost::ref(count)));
            }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
			assert(count == 1);
		}
		break;
	case 8: // // push_back(const val&, handler)
		{
			hvt.push_back(8 + E_write);
			int count = 0;
			cont.push_front(create_val<val_type>(),
							boost::bind(&push_handler<val_type>, _1, boost::ref(count)));
			assert(count == 1);
		}
		break;
	case 9: // pop_back
		{
			hvt.push_back(9 + E_write);
			val_type p;
			cont.pop_front(p);
		}
		break;
	case 10: // push_back(int &&)
		hvt.push_back(10 + E_write);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		cont.push_back(boost::move(create_val<val_type>()));
#else
        {
            val_type val(create_val<val_type>());
            cont.push_back(boost::move(val));
        }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		break;
	case 11: // push_back
		hvt.push_back(11 + E_write);
		cont.push_back(create_val<val_type>());
		break;
	case 12: // push_back(val&&, handler)
		{
			hvt.push_back(12 + E_write);
			int count = 0;
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			cont.push_back(boost::move(create_val<val_type>()),
							boost::bind(&push_handler<val_type>, _1, boost::ref(count)));
#else
            {
                val_type val(create_val<val_type>());
                cont.push_back(boost::move(val),
							boost::bind(&push_handler<val_type>, _1, boost::ref(count)));
            }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
			assert(count == 1);
		}
		break;
	case 13: // // push_back(const val&, handler)
		{
			hvt.push_back(13 + E_write);
			int count = 0;
			cont.push_back(create_val<val_type>(),
							boost::bind(&push_handler<val_type>, _1, boost::ref(count)));
			assert(count == 1);
		}
		break;
	case 14: // pop_back
		{
			hvt.push_back(14 + E_write);
			val_type p;
			cont.pop_back(p);
		}
		break;
	case 15: // swap(base&&)
		{
			hvt.push_back(15 + E_write);
			container_base_type tmp;
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			cont.swap(boost::move(tmp));
		}
		break;
	case 16: // swap(base&)
		{
			hvt.push_back(16 + E_write);
			container_base_type tmp;
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			cont.swap(tmp);
		}
		break;
	case 17: // swap(this&)
		{
			hvt.push_back(17 + E_write);
			container_type tmp;
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			cont.swap(tmp);
		}
		break;
	case 18: //clear
		hvt.push_back(18 + E_write);
		cont.clear();
		break;
	case 19: //remove
		hvt.push_back(19 + E_write);
		{
			val_type p;
			cont.front(p);
			cont.remove(p);
			cont.remove(create_val<val_type>());
		}
		break;
	case 20: // remove_if
		{
			hvt.push_back(20 + E_write);
			std::equal_to<val_type> eq;
			cont.remove_if(boost::bind(&std::equal_to<val_type>::operator(), &eq, _1, val_type()));
		}
		break;
	case 21: // unique
		hvt.push_back(21 + E_write);
		cont.unique();
		break;
	case 22: // merge
		{
			hvt.push_back(22 + E_write);
			container_type tmp;
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());

			cont.merge(tmp);
		}
		break;
	case 23: // sort
		hvt.push_back(23 + E_write);
		cont.sort();
		break;
	case 24:
		hvt.push_back(24 + E_write);
		cont.reverse();
		break;
	case 25: // use handler
		hvt.push_back(25 + E_write);
		cont.use_handler(boost::bind(&ref_handler<container_type>, _1));
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

		//std::cout << "thread " << tid << " run " << i << std::endl;
	}
}

void test_thread_safe_of_mutex(void)
{
	typedef boost::shared_ptr<int> val_type;
	typedef yggr::safe_container::safe_list<val_type> container_type;

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

		//std::cout << "thread " << tid << " run " << i << std::endl;
	}
}

void thread_safe_test_of_shared_mutex(void)
{
	typedef boost::shared_ptr<int> val_type;
	typedef yggr::safe_container::safe_list<val_type, boost::shared_mutex> container_type;

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
