//safe_multimap_test.cpp

//#define BOOST_THREAD_USE_DLL

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER
#include <iostream>
#include <yggr/safe_container/safe_map.hpp>
#include <boost/bind.hpp>
#include <assert.h>
#include <boost/swap.hpp>
#include <functional>
#include <boost/bind.hpp>

//-------------------------test base-------------------------
#ifdef _DEBUG

#define USE_STL_MULTIMAP 1

#if USE_STL_MULTIMAP
#   include <map>
    typedef yggr::safe_container::safe_multimap<int, int,
                                                    boost::mutex,
                                                    std::less<int>,
                                                    std::allocator<std::pair<const int, int> >,
                                                    std::multimap
                                                    > map_type;
#else
    typedef yggr::safe_container::safe_multimap<int, int> map_type;
#endif // USE_STL_MULTIMAP

//void check_values(const map_type::const_iterator iter, int n)
//{
//	assert(*iter == n);
//}

void check_range_equal(const map_type::base_type& l, const map_type::base_type& r)
{
	typedef map_type::base_type::const_iterator iter_type;
	map_type::size_type size = l.size();
	assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		assert(i->first == i->first && i->second == i->second);
	}
}

void test_constructor(void)
{
	map_type cont1;
	assert(cont1.empty());

	map_type cont2(cont1.key_comp());
	assert(cont2.empty());

	map_type cont3(cont1.get_allocator());
	assert(cont3.empty());

	map_type cont4(cont3.key_comp(), cont2.get_allocator());
	assert(cont4.empty());

	map_type::base_type base_cont;
	base_cont.insert(map_type::value_type(1, 1));
	base_cont.insert(map_type::value_type(2, 2));
	base_cont.insert(map_type::value_type(3, 3));
	base_cont.insert(map_type::value_type(4, 4));
	base_cont.insert(map_type::value_type(5, 5));
	base_cont.insert(map_type::value_type(0, 0));
	map_type cont5(base_cont.begin(), base_cont.end());
	assert(cont5.size() == 6);

	map_type cont6(base_cont.begin(), base_cont.end(), cont5.key_comp());
	assert(cont6.size() == 6);

	map_type cont7(base_cont.begin(), base_cont.end(), cont5.get_allocator());
	assert(cont7.size() == 6);

	map_type cont8(base_cont.begin(), base_cont.end(), cont5.key_comp(), cont5.get_allocator());
	assert(cont8.size() == 6);

	map_type::base_type base_cont9(base_cont.begin(), base_cont.end());
	map_type cont9(boost::move(base_cont9));
	assert(cont9.size() == 6);

	map_type::base_type base_cont10(base_cont.begin(), base_cont.end());
	map_type cont10(base_cont10);
	assert(cont10.size() == 6 && base_cont10.size() == 6);
	cont10.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont10)));

	map_type::base_type base_cont11(base_cont.begin(), base_cont.end());
	map_type cont11(boost::move(base_cont11), base_cont11.get_allocator());
	assert(cont11.size() == 6);

	map_type::base_type base_cont12(base_cont.begin(), base_cont.end());
	map_type cont12(base_cont12, base_cont12.get_allocator());
	assert(cont12.size() == 6 && base_cont12.size() == 6);
	cont12.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont12)));

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

void test_capacity(void)
{
	map_type::base_type base_cont;
	base_cont.insert(map_type::value_type(1, 1));
	base_cont.insert(map_type::value_type(2, 2));
	base_cont.insert(map_type::value_type(3, 3));
	base_cont.insert(map_type::value_type(4, 4));
	base_cont.insert(map_type::value_type(5, 5));
	base_cont.insert(map_type::value_type(0, 0));

	map_type cont1(base_cont.begin(), base_cont.end());
	map_type::base_type base_cont1(base_cont.begin(), base_cont.end());

	assert(cont1.size() == base_cont1.size());
	assert(cont1.max_size() == base_cont1.max_size());

	assert(cont1.empty() == base_cont1.empty());

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

void check_last_element(map_type::iterator& iter, int check)
{
	assert(iter->second == check);
}

void test_modifiers(void)
{
	map_type cont;
	map_type::base_type base_cont;

	{
		cont.insert(100, 100);
		base_cont.insert(map_type::value_type(100, 100));

		int kn = 90, n = 90;
		cont.insert(kn, boost::move(n));
		base_cont.insert(map_type::value_type(kn, 90));

		cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

		cont.insert(200, 200, boost::bind(&check_last_element, _1, 200));
		base_cont.insert(map_type::value_type(200, 200));

		int km = 300, m = 300;
		cont.insert(km, boost::move(m), boost::bind(&check_last_element, _1, 300));
		base_cont.insert(map_type::value_type(km, 300));

		assert(cont.size() == base_cont.size());
	}

	{
		cont.insert(map_type::value_type(1000, 1000));
		base_cont.insert(map_type::value_type(1000, 1000));

		int kn = 900, n = 900;
		map_type::value_type v(kn, n);
		cont.insert(boost::move(v));
		base_cont.insert(map_type::value_type(kn, 900));

		cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

		cont.insert(map_type::value_type(2000, 2000), boost::bind(&check_last_element, _1, 2000));
		base_cont.insert(map_type::value_type(2000, 2000));

		int km = 3000, m = 3000;
		map_type::value_type v2(km, m);
		cont.insert(boost::move(v2), boost::bind(&check_last_element, _1, 3000));
		base_cont.insert(map_type::value_type(km, 3000));

		assert(cont.size() == base_cont.size());
	}

	cont.erase(200);
	base_cont.erase(200);

	assert(cont.size() == base_cont.size());

	cont.swap(base_cont);
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));
	cont.swap(base_cont);
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	map_type::base_type base_cont2(base_cont);

	cont.swap(boost::move(base_cont2));
	assert(!cont.empty() && !base_cont2.empty());
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	map_type cont3(base_cont);

	cont.swap(cont3);
	cont.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));
	cont3.use_handler(boost::bind(&check_range_equal, _1, boost::cref(base_cont)));

	cont.clear();
	assert(cont.empty());

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

void test_safe_other(void)
{
	map_type::base_type base_cont;
	base_cont.insert(map_type::value_type(1, 1));
	base_cont.insert(map_type::value_type(2, 2));
	base_cont.insert(map_type::value_type(3, 3));
	base_cont.insert(map_type::value_type(4, 4));
	base_cont.insert(map_type::value_type(5, 5));
	base_cont.insert(map_type::value_type(0, 0));

	map_type cont(base_cont.begin(), base_cont.end());
	assert(cont.is_exists(1));
	assert(cont.is_exists(0));
	assert(!cont.is_exists(100));

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

void test_swap(void)
{
	map_type::base_type init_cont1;
	init_cont1.insert(map_type::value_type(1, 1));
	init_cont1.insert(map_type::value_type(2, 2));
	init_cont1.insert(map_type::value_type(3, 3));
	init_cont1.insert(map_type::value_type(4, 4));
	init_cont1.insert(map_type::value_type(5, 5));

	map_type::base_type init_cont2;
	init_cont2.insert(map_type::value_type(6, 6));
	init_cont2.insert(map_type::value_type(7, 7));
	init_cont2.insert(map_type::value_type(8, 8));
	init_cont2.insert(map_type::value_type(9, 9));
	init_cont2.insert(map_type::value_type(0, 0));

	map_type cont1(init_cont1.begin(), init_cont1.end());
	map_type cont2(init_cont2.begin(), init_cont2.end());

	map_type::base_type base_cont1(init_cont1.begin(), init_cont1.end());
	map_type::base_type base_cont2(init_cont2.begin(), init_cont2.end());
	assert(cont1.size() == base_cont1.size());
	assert(cont1.size() == base_cont2.size());


	map_type::base_type base_cont3(base_cont1);
	map_type::base_type base_cont4(base_cont2);

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
	if(cmp(iter->second, val))
	{
		++count;
		return true;
	}

	return false;
}

template<typename Iter>
void ref_all_check_handler(Iter iter, const map_type::base_type& r)
{
	map_type::base_type::const_iterator i = r.find(iter->first);
	assert(i != r.end());
	iter->second = i->second;
}

template<typename Iter>
void cref_all_check_handler(Iter iter, const map_type::base_type& r)
{
	map_type::base_type::const_iterator i = r.find(iter->first);
	assert(i != r.end());
}

void check_handler_of_val_eins(map_type::const_iterator& iter, const map_type::val_type& val)
{
	assert(iter->second == val);
}

template<typename Handler>
void check_handler_of_val_zwei(map_type::const_iterator& iter, const map_type::val_type& val, const Handler& ret_handler)
{
	assert(iter->second == val);
	ret_handler();
}

void check_handler_eins(const map_type::base_type& l, const map_type::base_type& r)
{
	typedef map_type::const_iterator iter_type;
	map_type::size_type size = l.size();
	assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		assert(*i == *j);
	}
}

template<typename Handler>
void check_handler_zwei(const map_type::base_type& l, const map_type::base_type& r, const Handler& ret_handler)
{
	typedef map_type::const_iterator iter_type;
	map_type::size_type size = l.size();
	assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		assert(*i == *j);
	}
	ret_handler();
}

void set_handler_of_val_eins(map_type::iterator& iter, const map_type::val_type& val)
{
	assert(iter->second != val);
	iter->second = val;
}

template<typename Handler>
void set_handler_of_val_zwei(map_type::iterator& iter, const map_type::val_type& val, const Handler& ret_handler)
{
	assert(iter->second != val);
	iter->second = val;
	ret_handler();
}

void set_handler_eins(map_type::base_type& l, const map_type::base_type& r)
{
	typedef map_type::const_iterator iter_type;
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
void set_handler_zwei(map_type::base_type& l, const map_type::base_type& r, const Handler& ret_handler)
{
	typedef map_type::const_iterator iter_type;
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
void return_handler_call_helper(map_type& cont, const map_type::base_type& r, const Handler& ret_handler)
{
	typedef Handler ret_handler_type;
	cont.use_handler(boost::bind(&set_handler_zwei<ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Handler>
void return_handler_call_helper(const map_type& cont, const map_type::base_type& r, const Handler& ret_handler)
{
	typedef Handler ret_handler_type;
	cont.use_handler(boost::bind(&check_handler_zwei<ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Handler>
void return_handler_call_helper2(map_type& cont, const map_type::key_type& key, const map_type::val_type& r, const Handler& ret_handler)
{
	typedef Handler ret_handler_type;
	cont.use_handler(key, boost::bind(&set_handler_of_val_zwei<ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Handler>
void return_handler_call_helper2(const map_type& cont, const map_type::key_type& key, const map_type::val_type& r, const Handler& ret_handler)
{
	typedef Handler ret_handler_type;
	cont.use_handler(key, boost::bind(&check_handler_of_val_zwei<ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

void test_user_handler(void)
{
	map_type::base_type init_cont1;
	init_cont1.insert(map_type::value_type(1, 1));
	init_cont1.insert(map_type::value_type(2, 2));
	init_cont1.insert(map_type::value_type(3, 3));
	init_cont1.insert(map_type::value_type(4, 4));
	init_cont1.insert(map_type::value_type(5, 5));

	map_type::base_type init_cont2;
	init_cont2.insert(map_type::value_type(6, 6));
	init_cont2.insert(map_type::value_type(7, 7));
	init_cont2.insert(map_type::value_type(8, 8));
	init_cont2.insert(map_type::value_type(9, 9));
	init_cont2.insert(map_type::value_type(10, 10));

	map_type::base_type base_cont1(init_cont1.begin(), init_cont1.end());
	map_type::base_type base_cont2(init_cont2.begin(), init_cont2.end());

	map_type cont;
	map_type& ref_cont = cont;
	const map_type& cref_cont = cont;

	ref_cont.use_handler(boost::bind(&set_handler_eins, _1, boost::cref(base_cont1)));
	cref_cont.use_handler(boost::bind(&check_handler_eins, _1, boost::cref(base_cont1)));

	int ref_count = 0;
	return_handler_call_helper(ref_cont, base_cont2, boost::bind(&return_handler, boost::ref(ref_count)));
	assert(ref_count == 1);

	int cref_count = 0;
	return_handler_call_helper(cref_cont, base_cont2, boost::bind(&return_handler, boost::ref(cref_count)));
	assert(cref_count == 1);

	ref_cont.use_handler_of_all(boost::bind(&ref_all_check_handler<map_type::iterator>,
											_1, boost::cref(base_cont2)));
	cref_cont.use_handler_of_all(boost::bind(&cref_all_check_handler<map_type::const_iterator>,
												_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.use_handler_of_all(boost::bind(&ref_all_check_handler<map_type::iterator>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<map_type::iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(ref_count)));
	assert(ref_count == 2);

	cref_count = 0;
	cref_cont.use_handler_of_all(boost::bind(&cref_all_check_handler<map_type::const_iterator>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<map_type::const_iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(cref_count)));
	assert(cref_count == 2);

	ref_cont.use_handler_of_reverse_all(boost::bind(&ref_all_check_handler<map_type::reverse_iterator>,
													_1, boost::cref(base_cont2)));
	cref_cont.use_handler_of_reverse_all(boost::bind(&cref_all_check_handler<map_type::const_reverse_iterator>,
													_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.use_handler_of_reverse_all(boost::bind(&ref_all_check_handler<map_type::reverse_iterator>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<map_type::reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(ref_count)));
	assert(ref_count == 2);

	cref_count = 0;
	cref_cont.use_handler_of_reverse_all(boost::bind(&cref_all_check_handler<map_type::const_reverse_iterator>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<map_type::const_reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(cref_count)));
	assert(cref_count == 2);

	ref_cont.use_handler(7, boost::bind(&set_handler_of_val_eins, _1, 2000));
	cref_cont.use_handler(7, boost::bind(&check_handler_of_val_eins, _1, 2000));

	ref_count = 0;
	return_handler_call_helper2(ref_cont, 7, 7, boost::bind(&return_handler, boost::ref(ref_count)));
	assert(ref_count == 1);

	cref_count = 0;
	return_handler_call_helper2(cref_cont, 7, 7, boost::bind(&return_handler, boost::ref(cref_count)));
	assert(cref_count == 1);

	std::cout << "---------------test_use_handler end-----------------" << std::endl;
}

#endif // _DEBUG

void debug_test(void)
{
#ifdef _DEBUG
	test_constructor();
	test_capacity();
	test_modifiers();
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
std::pair<int, Val> create_val(void)
{
	typedef Val val_type;
	typedef typename val_type::element_type elem_type;
	return std::pair<int, val_type>(std::rand() % 100, val_type(new elem_type(std::rand() % 100)));
	//return std::pair<int, val_type>(std::rand(), new elem_type(std::rand()));
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
			cont.is_exists(std::rand() % 100);
			cont.is_exists(std::rand() % 100);
		}
		break;
	case 7: // use_handler const
		hvt.push_back(7 + E_read);
		cont.use_handler(boost::bind(&cref_handler<container_type>, _1));
		break;
	case 8:
		hvt.push_back(8 + E_read);
		cont.count(std::rand() % 100);
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

	int op = std::rand() % 12;
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
            typename container_type::value_type val(create_val<val_type>());
            cont.insert(boost::move(val));
        }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		break;
	case 4: // insert(val&&, handler)
		{
			hvt.push_back(4 + E_write);
			int count = 0;
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			bool bret = cont.insert(boost::move(create_val<val_type>()),
							boost::bind(&push_handler<container_type>, _1, boost::ref(count)));
#else
            typename container_type::value_type val(create_val<val_type>());
            bool bret = cont.insert(boost::move(val),
							boost::bind(&push_handler<container_type>, _1, boost::ref(count)));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
			assert((count == 1 && bret == true) || (count == 0 && bret == false));
		}
		break;
	case 5: // // push_back(const val&, handler)
		{
			hvt.push_back(5 + E_write);
			int count = 0;
			bool bret =  cont.insert(create_val<val_type>(),
							boost::bind(&push_handler<container_type>, _1, boost::ref(count)));
			assert((count == 1 && bret == true) || (count == 0 && bret == false));
		}
		break;
	case 6: // pop_back
		{
			hvt.push_back(6 + E_write);
			cont.erase(std::rand() % 100);
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
		if(std::rand() %  2)
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
	typedef yggr::safe_container::safe_multimap<int, val_type> container_type;

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
	typedef yggr::safe_container::safe_multimap<int, val_type, boost::shared_mutex> container_type;

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
