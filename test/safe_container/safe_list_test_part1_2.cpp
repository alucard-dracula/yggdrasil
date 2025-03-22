//safe_list_test_part1.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/safe_container/safe_list.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)

#define THREAD_TEST_USING_MANUAL() 0

#define THREAD_TEST_THREAD_SIZE() 10
#define THREAD_TEST_THREAD_LOOP_SIZE() 10

//-------------------------test base-------------------------

template<typename Iter> inline
void check_iter_values(const Iter& iter, int n)
{
	yggr_test_assert(*iter == n);
}

template<typename Val> inline
void check_values(const Val& val, int n)
{
	yggr_test_assert(val == n);
}

template<typename Cont>
void test_constructor(void)
{
	typedef Cont list_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	list_type cont1;
	yggr_test_assert(cont1.empty());

	list_type cont2(cont1.get_allocator());
	yggr_test_assert(cont2.empty());

	list_type cont3(10, 1);
	cont3.for_each_iterator(boost::bind(&check_iter_values<citer_type>, _1, 1));
	cont3.for_each_reverse(boost::bind(&check_values<val_type>, _1, 1));

	list_type cont4(10, 1, cont2.get_allocator());
	cont4.for_each_iterator(boost::bind(&check_iter_values<citer_type>, _1, 1));

	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont5(arr + 0, arr + 10);
	yggr_test_assert(cont5.size() == 10);

	list_type cont6(arr + 0, arr + 10, cont5.get_allocator());
	yggr_test_assert(cont6.size() == 10);

	typename list_type::base_type base_cont7(arr + 0, arr + 10);
	list_type cont7(base_cont7);
	yggr_test_assert(cont7.size() == 10 && base_cont7.size() == 10);

	typename list_type::base_type base_cont8(arr + 0, arr + 10);
	list_type cont8(boost::move(base_cont8));
	yggr_test_assert(cont8.size() == 10);

	typename list_type::base_type base_cont9(arr + 0, arr + 10);
	list_type cont9(base_cont9, cont7.get_allocator());
	yggr_test_assert(cont9.size() == 10 && base_cont9.size() == 10);

	typename list_type::base_type base_cont10(arr + 0, arr + 10);
	list_type cont10(boost::move(base_cont10), base_cont9.get_allocator());
	yggr_test_assert(cont10.size() == 10);

	{
		typename list_type::base_type base_cont11(arr + 0, arr + 10);
		list_type cont11(boost::move(base_cont11), base_cont11.get_allocator());
		yggr_test_assert(cont11.size() == 10);
		base_cont11.assign(arr + 0, arr + 10);

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

		list_type cont12(cont11);
		yggr_test_assert(cont12.size() == 10);

		// safe_copy_safe
		yggr_test_assert(cont11 == cont12);
		yggr_test_assert(!(cont11 != cont12));
		yggr_test_assert(cont11 <= cont12);
		yggr_test_assert(!(cont11 < cont12));
		yggr_test_assert(cont11 >= cont12);
		yggr_test_assert(!(cont11 > cont12));

		list_type cont13(boost::move(cont12));
		yggr_test_assert(cont13.size() == 10);
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void check_range_equal(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont list_type;
	typedef typename Cont::base_type::const_iterator iter_type;
	////typename list_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		yggr_test_assert(*i == *j);
	}
}

template<typename Cont>
void test_capacity(void)
{
	typedef Cont list_type;

	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont1(arr + 0, arr + 10);
	typename list_type::base_type base_cont1(arr + 0, arr + 10);

	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.max_size() == base_cont1.max_size());

	cont1.resize(20);
	base_cont1.resize(20);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.max_size() == base_cont1.max_size());

	yggr_test_assert(cont1.empty() == base_cont1.empty());

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

template<typename Cont> inline
typename Cont::iterator
	iter_handler(typename Cont::base_type& base,
					const typename Cont::value_type& src)
{
	return std::find(base.begin(), base.end(), src);
}

template<typename Cont> inline
bool ins_handler(typename Cont::base_type& base,
					const std::pair<typename Cont::iterator, bool>& pair,
					const typename Cont::value_type& chk_val)
{
	yggr_test_assert(pair.second);
	yggr_test_assert(chk_val == base.back());
	return pair.second && (pair.first != base.end());
}

template<typename Cont>
void test_element_access(void)
{
	typedef Cont list_type;

	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont(arr + 0, arr + 10);
	typename list_type::base_type base_cont(arr + 0, arr + 10);

	int f = 0, b = 0;
	list_type cont2;
	list_type& cont_ref = cont2;
	const list_type& cont_cref = cont2;

	try
	{
		yggr_test_assert(cont2.empty());
		cont_ref.front() = 10;
		yggr_test_assert(false);
	}
	catch(const typename list_type::error_type&)
	{
	}

	try
	{
		yggr_test_assert(cont2.empty());
		cont_cref.front();
		yggr_test_assert(false);
	}
	catch(const typename list_type::error_type&)
	{
	}

	try
	{
		yggr_test_assert(cont2.empty());
		cont_ref.back() = 10;
		yggr_test_assert(false);
	}
	catch(const typename list_type::error_type&)
	{
	}

	try
	{
		yggr_test_assert(cont2.empty());
		cont_cref.back();
		yggr_test_assert(false);
	}
	catch(const typename list_type::error_type&)
	{
	}

	yggr_test_assert(!cont2.get_front(f));
	yggr_test_assert(!cont2.get_back(b));

	yggr_test_assert(!cont2.get_begin_value(f));
	yggr_test_assert(!cont2.get_rbegin_value(b));

	try
	{
		cont2.begin_value();
		yggr_test_assert(false);
	}
	catch(const typename list_type::error_type&)
	{
	}

	try
	{
		cont2.rbegin_value();
		yggr_test_assert(false);
	}
	catch(const typename list_type::error_type&)
	{
	}

	yggr_test_assert(cont.get_front(f));
	yggr_test_assert(cont.get_back(b));
	yggr_test_assert(f == base_cont.front());
	yggr_test_assert(b == base_cont.back());

	yggr_test_assert(cont.get_begin_value(f));
	yggr_test_assert(cont.get_rbegin_value(b));
	yggr_test_assert(f == cont.begin_value());
	yggr_test_assert(b == cont.rbegin_value());

	yggr_test_assert(cont.front() == base_cont.front());
	yggr_test_assert(cont.back() == base_cont.back());

	yggr_test_assert(base_cont.front() == cont.front());
	yggr_test_assert(base_cont.back() == cont.back());

	yggr_test_assert(cont.front() == cont.front());
	yggr_test_assert(cont.back() == cont.back());

	//emplace

	{
		list_type ins_cont;
		ins_cont.emplace(boost::bind(&iter_handler<list_type>, _1, 0), yggr::non_handler );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 0);
	}

	{
		list_type ins_cont;
		yggr_test_assert(ins_cont.emplace(
						boost::bind(&iter_handler<list_type>, _1, 0),
						boost::bind(&ins_handler<list_type>, _1, _2, 0) ) );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 0);

	}

	{
		list_type ins_cont;
		yggr_test_assert(ins_cont.emplace(
						boost::bind(&iter_handler<list_type>, _1, 10),
						boost::bind(&ins_handler<list_type>, _1, _2, 10), 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		list_type ins_cont;
		ins_cont.emplace(boost::bind(&iter_handler<list_type>, _1, 10), yggr::non_handler, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		list_type ins_cont;
		ins_cont.emplace(boost::bind(&iter_handler<list_type>, _1, 10), yggr::non_handler, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		list_type ins_cont;
		ins_cont.emplace_front();
		ins_cont.emplace_back();
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.front() == 0);
		yggr_test_assert(ins_cont.back() == 0);
	}

	{
		list_type ins_cont;
		ins_cont.emplace_front(10);
		ins_cont.emplace_back(20);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.front() == 10);
		yggr_test_assert(ins_cont.back() == 20);
	}

	// insert
	{
		int val = 20;
		list_type ins_cont;
		ins_cont.insert(boost::bind(&iter_handler<list_type>, _1, 10), 10);
		ins_cont.insert(boost::bind(&iter_handler<list_type>, _1, 20), boost::move(val));

		yggr_test_assert(ins_cont.size() == 2);
	}

	{
		int val = 20;
		list_type ins_cont;

		ins_cont.insert(boost::bind(&iter_handler<list_type>, _1, 10),
							10,
							boost::bind(&ins_handler<list_type>, _1, _2, 10));

		ins_cont.insert(boost::bind(&iter_handler<list_type>, _1, 20),
							boost::move(val),
							boost::bind(&ins_handler<list_type>, _1, _2, 20));

		yggr_test_assert(ins_cont.size() == 2);
	}

	{
		//int val = 20;
		list_type ins_cont;
		ins_cont.insert(boost::bind(&iter_handler<list_type>, _1, 10), 3, 10);
		yggr_test_assert(ins_cont.size() == 3);
	}

	{
		list_type ins_cont;
		ins_cont.insert(boost::bind(&iter_handler<list_type>, _1, 10), arr + 0, arr + 10);
		yggr_test_assert(ins_cont.size() == 10);
	}

	std::cout << "---------------test_element_access end-----------------" << std::endl;
}

template<typename Cont>
void test_safe_element_reference(void)
{
	typedef Cont list_type;

	int arr1[10] = {10, 11, 12};
	int arr2[10] = {20, 21, 22};

	list_type cont1(arr1 + 0, arr1 + 10);
	list_type cont2(arr2 + 0, arr2 + 10);

#define TEST_OPERATOR( __op__ ) \
	a = cont1.front() __op__ cont1.front(); \
	yggr_test_assert(a == (arr1[0] __op__ arr1[0])); \
	a = cont1.front() __op__ cont2.front(); \
	yggr_test_assert(a == (arr1[0] __op__ arr2[0]));

	{
		int a = 0;
		{
			a = cont1.front() + cont1.front();
			yggr_test_assert(a == arr1[0]+ arr1[0]);

			a = cont1.front() + cont2.front();
			yggr_test_assert(a == arr1[0] + arr2[0]);
		}

		{
			a = cont1.back() - cont1.back();
			yggr_test_assert(a == arr1[9] - arr1[9]);

			a = cont1.back() - cont2.back();
			yggr_test_assert(a == arr1[9] - arr2[9]);
		}

		TEST_OPERATOR(+);
		TEST_OPERATOR(-);
		TEST_OPERATOR(*);
		TEST_OPERATOR(/);
		TEST_OPERATOR(%);

		TEST_OPERATOR(&);
		TEST_OPERATOR(|);
		TEST_OPERATOR(^);
		TEST_OPERATOR(<<);
		TEST_OPERATOR(>>);
	}
#undef TEST_OPERATOR

#define TEST_OPERATOR( __op__ ) \
		cont1.back() __op__ cont1.front(); \
		yggr_test_assert(cont1.back() == (a __op__ cont1.front())); \
		yggr_test_assert(cont1.back() == (b __op__ arr1[0])); \
		cont1.back() = 0; \
		a = 0; b = 0; \
		\
		cont1.back() __op__ cont2.front(); \
		yggr_test_assert(cont1.back() == (a __op__ cont2.front())); \
		yggr_test_assert(cont1.back() == (b __op__ arr2[0])); \
		cont1.back() = 0; \
		a = 0; b = 0;

	{
		int a = 0, b = 0;

		cont1.back() = cont1.front();
		yggr_test_assert(cont1.back() == cont1.front());
		yggr_test_assert(cont1.back() == arr1[0]);
		cont1.back() = 0;

		cont1.back() = cont2.front();
		yggr_test_assert(cont1.back() == cont2.front());
		yggr_test_assert(cont1.back() == arr2[0]);
		cont1.back() = 0;

		TEST_OPERATOR(=);
		TEST_OPERATOR(+=);
		TEST_OPERATOR(-=);
		TEST_OPERATOR(*=);
		TEST_OPERATOR(*=);
		TEST_OPERATOR(%=);

		TEST_OPERATOR(&=);
		TEST_OPERATOR(|=);
		TEST_OPERATOR(^=);
		TEST_OPERATOR(<<=);
		TEST_OPERATOR(>>=);
	}
#undef TEST_OPERATOR


	// mix
	{
		{
			int num = (cont1.back() = cont1.front() + cont2.front() + cont2.back() * cont1.front());
			yggr_test_assert(num == (arr1[0] + arr2[0] + arr2[9] * arr1[0]));
		}

		{
			int num = (cont1.back() = 1 + cont1.front() + cont2.back() * 3);
			yggr_test_assert(num == (1 + arr1[0] + arr2[9] * 3));
		}
	}

	{
		typedef yggr::safe_container::safe_list<yggr::shared_ptr<int> > ptr_vt_type;

		ptr_vt_type cont3;
		cont3.push_back(yggr::shared_ptr<int>(yggr_nothrow_new int(1)));

		yggr_test_assert(*cont3.front() == 1);
		yggr::shared_ptr<int> ptr = cont3.front();
		*ptr = 100;
		yggr_test_assert(*cont3.front() == 100);
		yggr_test_assert(*cont3.back() == 100);

	}

	std::cout << "---------------test_safe_element_reference end-----------------" << std::endl;
}

template<typename Cont> inline
bool handler_erase(typename Cont::base_type& base, const std::pair<typename Cont::iterator, bool>& rst)
{
	return rst.second;
}

template<typename Cont>
std::pair<typename Cont::iterator, typename Cont::iterator>
	make_erase_iter_pair(typename Cont::base_type& base)
{
	return std::make_pair(base.begin(), base.end());
}

template<typename Cont>
void test_modifiers(void)
{
	typedef Cont list_type;
	typedef typename list_type::base_type base_cont_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	typedef iter_type (base_cont_type::* begin_end_foo_type)(void);
	typedef citer_type (base_cont_type::* cbegin_end_foo_type)(void) const;

	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont;
	base_cont_type base_cont;

	cont.assign(7, 100);
	base_cont.assign(7, 100);

	yggr_test_assert(cont.size() == base_cont.size());
	cont.for_each_iterator(boost::bind(&check_iter_values<citer_type>, _1, 100));

	cont.assign(arr + 0, arr + 10);
	base_cont.assign(arr + 0, arr + 10);

	yggr_test_assert(cont.size() == base_cont.size());

	{
		cont.push_back(200);
		base_cont.push_back(200);

		int n = 300;
		cont.push_back(boost::move(n));
		base_cont.push_back(300);
		yggr_test_assert(cont.size() == base_cont.size());

		cont.push_back(1000);
		base_cont.push_back(1000);

		int m = 2000;
		cont.push_back(boost::move(m));
		base_cont.push_back(2000);
	}

	yggr_test_assert(cont.size() == base_cont.size());

	{
		cont.push_front(600);
		base_cont.push_front(600);

		int n = 500;
		cont.push_front(boost::move(n));
		base_cont.push_front(500);
		yggr_test_assert(cont.size() == base_cont.size());

		cont.push_front(5000);
		base_cont.push_front(5000);

		int m = 6000;
		cont.push_front(boost::move(m));
		base_cont.push_front(6000);
	}

	yggr_test_assert(cont.size() == base_cont.size());

	{
		int b = 0;
		yggr_test_assert(cont.pop_back(b));
		yggr_test_assert(b == 2000);
		base_cont.pop_back();
		yggr_test_assert(cont.size() == base_cont.size());

		list_type cont2;
		yggr_test_assert(!cont2.pop_back(b));
		yggr_test_assert(b == 2000);
	}

	{
		int b = 0;
		yggr_test_assert(cont.pop_front(b));
		yggr_test_assert(b == 6000);
		base_cont.pop_front();
		yggr_test_assert(cont.size() == base_cont.size());

		list_type cont2;
		yggr_test_assert(!cont2.pop_front(b));
		yggr_test_assert(b == 6000);
	}

	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));
	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	typename list_type::base_type base_cont2(base_cont);

    yggr_test_assert(!cont.empty());
    yggr_test_assert(!base_cont2.empty());
	cont.swap(boost::move(base_cont2));
	yggr_test_assert(!cont.empty() && !base_cont2.empty());
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	list_type cont3(base_cont);

	cont.swap(cont3);
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));
	cont3.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	{
		cont.emplace_front(555);
		base_cont.push_front(555);

		cont.emplace_back(666);
		base_cont.push_back(666);

		int chk_val = 0;

		cont.get_front(chk_val);
		yggr_test_assert(chk_val == base_cont.front());

		cont.get_back(chk_val);
		yggr_test_assert(chk_val == base_cont.back());
	}

	{
		std::size_t now_size = cont.size();
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::begin), _1));
		yggr_test_assert(cont.size() == now_size - 1);

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::begin), _1),
					boost::bind(&handler_erase<list_type>, _1, _2));
		yggr_test_assert(cont.size() == now_size - 2);

		cont.erase(boost::bind(&make_erase_iter_pair<list_type>, _1),
					boost::bind(&handler_erase<list_type>, _1, _2));

		yggr_test_assert(cont.size() == 0);

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::begin), _1),
					boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::end), _1));

		yggr_test_assert(cont.size() == 0);

		cont.push_back(100);
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::begin), _1),
					boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::end), _1),
					boost::bind(&handler_erase<list_type>, _1, _2));

		yggr_test_assert(cont.empty());
	}

	cont.clear();
	yggr_test_assert(cont.empty());

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

template<typename Cont>
void test_operators(void)
{
	typedef Cont list_type;
	typedef typename list_type::base_type base_cont_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	typedef iter_type (base_cont_type::* begin_end_foo_type)(void);
	typedef citer_type (base_cont_type::* cbegin_end_foo_type)(void) const;

	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont(arr + 0, arr + 10);
	base_cont_type base_cont(arr + 0, arr + 10);

	{
		list_type cont1(arr + 0, arr + 10);
		list_type cont2(arr + 0, arr + 5);

		cont1.splice(cont2);
		cont1.splice(boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::end), _1),
						cont2 );

		cont1.splice(boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::end), _1),
						cont2,
						boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::begin), _1) );

		cont1.splice(boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::end), _1),
						cont2,
						boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::begin), _1),
						boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::end), _1) );

		cont1.splice(boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::end), _1),
						cont2,
						boost::bind(&make_erase_iter_pair<list_type>, _1) );

		yggr_test_assert(cont1.size() == 15);
		int tmp = 0;
		bool b = false;
		for(int i = 0; i != 15; ++i)
		{
			b = cont1.pop_front(tmp);
			yggr_test_assert(b);
			yggr_test_assert(tmp == arr[i % 10]);
		}

		cont1.assign(arr + 0, arr + 10);
		yggr_test_assert(cont1.size() == 10);
		cont1.erase(boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::begin), _1),
						boost::bind(static_cast<begin_end_foo_type>(&base_cont_type::end), _1));
		yggr_test_assert(cont1.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10);
		typename list_type::base_type cont2(arr + 0, arr + 5);

		cont1.splice(boost::move(cont2));

		yggr_test_assert(cont1.size() == 15);
	}

	cont.remove(3);
	base_cont.remove(3);
	yggr_test_assert(cont.size() == base_cont.size());

	cont.remove(100);
	base_cont.remove(100);
	yggr_test_assert(cont.size() == base_cont.size());

	std::less<int> lss;
	cont.remove_if(boost::bind(&std::less<int>::operator(), &lss, _1, 2));
	base_cont.remove_if(boost::bind(&std::less<int>::operator(), &lss, _1, 2));

	yggr_test_assert(cont.size() == base_cont.size());

	cont.remove_if(boost::bind(&std::less<int>::operator(), &lss, _1, 0));
	base_cont.remove_if(boost::bind(&std::less<int>::operator(), &lss, _1, 0));

	yggr_test_assert(cont.size() == base_cont.size());
	cont.push_back(100);
	cont.push_back(100);

	base_cont.push_back(100);
	base_cont.push_back(100);

	yggr_test_assert(cont.size() == base_cont.size());
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	cont.unique();
	base_cont.unique();

	yggr_test_assert(cont.size() == base_cont.size());
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	cont.push_back(1000);
	cont.push_back(1001);
	cont.push_back(1002);

	base_cont.push_back(1000);
	base_cont.push_back(1001);
	base_cont.push_back(1002);

	cont.unique(std::less<int>());
	base_cont.unique(std::less<int>());

	yggr_test_assert(cont.size() == base_cont.size());
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	typename list_type::base_type base_cont1(arr + 0, arr + 5);
	typename list_type::base_type base_cont2(arr + 0, arr + 5);
	typename list_type::base_type base_cont3(arr + 0, arr + 5);
	typename list_type::base_type base_cont4(arr + 0, arr + 5);

	cont.merge(boost::move(base_cont1));
	cont.merge(base_cont2);

	base_cont.merge(boost::move(base_cont3));
	base_cont.merge(base_cont4);

	yggr_test_assert(base_cont1.size() == base_cont3.size());
	yggr_test_assert(base_cont2.size() == base_cont4.size());

	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	typename list_type::base_type base_cont5(arr + 0, arr + 5);
	typename list_type::base_type base_cont6(arr + 0, arr + 5);
	typename list_type::base_type base_cont7(arr + 0, arr + 5);
	typename list_type::base_type base_cont8(arr + 0, arr + 5);

	// boost list support unordered merge but don't use it,
	// if you want to use unordred_merge please use splite

	if(yggr::container::is_std_list<typename list_type::base_type>::value)
	{
		typename list_type::base_type base_tmp_cont = base_cont;

		cont.merge(boost::move(base_cont5), false, false);

		cont.merge(base_cont6, false, false);

		base_cont.merge(boost::move(base_cont7));
		base_cont.merge(base_cont8);

		yggr_test_assert(base_cont5.size() == base_cont7.size());
		yggr_test_assert(base_cont6.size() == base_cont8.size());


		yggr_test_assert(cont.size() == base_cont.size());
		cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

		list_type tmp_cont1(arr + 0, arr + 5);
		list_type tmp_cont2(arr + 0, arr + 5);
		typename list_type::base_type tmp_base_cont1(arr + 0, arr + 5);
		typename list_type::base_type tmp_base_cont2(arr + 0, arr + 5);

		cont.merge(tmp_cont1, false, false);
		cont.merge(tmp_cont2, false, false);

		base_cont.merge(tmp_base_cont1);
		base_cont.merge(tmp_base_cont2);

		yggr_test_assert(tmp_cont1.size() == tmp_base_cont1.size());
		yggr_test_assert(tmp_cont2.size() == tmp_base_cont2.size());

		cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));
	}
	else
	{
		std::not_equal_to<int> foo_ne;

		typename list_type::base_type base_tmp_cont = base_cont;

		cont.merge(boost::move(base_cont5),
					boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3),
					false, false);
		cont.merge(base_cont6,
					boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3),
					false, false);

		base_cont.merge(boost::move(base_cont7),
							boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));
		base_cont.merge(base_cont8,
							boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 3));

		yggr_test_assert(base_cont5.size() == base_cont7.size());
		yggr_test_assert(base_cont6.size() == base_cont8.size());


		yggr_test_assert(cont.size() == base_cont.size());
		cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

		list_type tmp_cont1(arr + 0, arr + 5);
		list_type tmp_cont2(arr + 0, arr + 5);
		typename list_type::base_type tmp_base_cont1(arr + 0, arr + 5);
		typename list_type::base_type tmp_base_cont2(arr + 0, arr + 5);

		cont.merge(tmp_cont1, false, false);
		cont.merge(tmp_cont2,
					boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 2),
					false, false);

		base_cont.merge(tmp_base_cont1);
		base_cont.merge(tmp_base_cont2,
							boost::bind(&std::not_equal_to<int>::operator(), &foo_ne, _1, 2));

		yggr_test_assert(tmp_cont1.size() == tmp_base_cont1.size());
		yggr_test_assert(tmp_cont2.size() == tmp_base_cont2.size());

		cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));
	}

	cont.sort();
	base_cont.sort();

	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	cont.sort(std::greater<int>());
	base_cont.sort(std::greater<int>());

	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	cont.reverse();
	base_cont.reverse();
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));


	std::cout << "---------------test_operators end-----------------" << std::endl;
}

template<typename Cont>
void test_safe_other(void)
{
	typedef Cont list_type;

	int arr[10] = {1, 2, 3, 4, 5};
	list_type cont(arr + 0, arr + 10);
	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(cont.is_exists(0));
	yggr_test_assert(!cont.is_exists(100));

	typename list_type::base_type base_cont1 = cont.load();
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));

	typename list_type::base_type base_cont2 = cont;
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont>
void test_swap(void)
{
	typedef Cont list_type;

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	list_type cont1(arr1 + 0, arr1 + 10);
	list_type cont2(arr2 + 0, arr2 + 5);

	typename list_type::base_type base_cont1(arr1 + 0, arr1 + 10);
	typename list_type::base_type base_cont2(arr2 + 0, arr2 + 5);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() != base_cont2.size());


	typename list_type::base_type base_cont3(base_cont1);
	typename list_type::base_type base_cont4(base_cont2);

	// ------------------ boost::swap test------------------------
	// base_type&& this_type&
	boost::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	boost::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	boost::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	boost::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	boost::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));

	boost::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	boost::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	boost::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));

	// ------------------ std::swap test------------------------
	// base_type&& this_type&
	std::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	std::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	std::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	std::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	std::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));

	std::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	std::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	std::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));

	// ------------------ yggr::swap test------------------------
	// base_type&& this_type&
	yggr::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	yggr::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	yggr::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	yggr::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	yggr::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));

	yggr::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	yggr::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	yggr::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

inline void return_handler(int& count)
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

template<typename Iter, typename Cont>
void ref_all_check_handler(Iter iter, const typename Cont::base_type& r)
{
	typedef Cont list_type;

	typename list_type::base_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	yggr_test_assert(i != r.end());
	*iter = *i;
}

template<typename Iter, typename Cont>
void cref_all_check_handler(Iter iter, const typename Cont::base_type& r)
{
	typedef Cont list_type;
	typename list_type::base_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	yggr_test_assert(i != r.end());
}

template<typename Cont>
void check_handler_eins(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont list_type;
	typedef typename list_type::const_iterator iter_type;
	////typename list_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		yggr_test_assert(*i == *j);
	}
}

template<typename Cont, typename Handler>
void check_handler_zwei(const typename Cont::base_type& l, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont list_type;
	typedef typename list_type::const_iterator iter_type;
	////typename list_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		yggr_test_assert(*i == *j);
	}
	ret_handler();
}

template<typename Cont>
void set_handler_eins(typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont list_type;
	typedef typename list_type::const_iterator iter_type;
	if(!l.empty())
	{
		yggr_test_assert(l.size() == r.size());
		for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
		{
			yggr_test_assert(*i != *j);
		}
	}
	l = r;
	{
		for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
		{
			yggr_test_assert(*i == *j);
		}
	}
}

template<typename Cont, typename Handler>
void set_handler_zwei(typename Cont::base_type& l, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont list_type;
	typedef typename list_type::const_iterator iter_type;
	if(!l.empty())
	{
		yggr_test_assert(l.size() == r.size());
		for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
		{
			yggr_test_assert(*i != *j);
		}
	}
	l = r;
	{
		for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
		{
			yggr_test_assert(*i == *j);
		}
	}

	ret_handler();
}

template<typename Cont, typename Handler>
void return_handler_call_helper(Cont& cont, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont list_type;
	typedef Handler ret_handler_type;
	cont.using_handler(boost::bind(&set_handler_zwei<list_type, ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Cont, typename Handler>
void return_handler_call_helper(const Cont& cont, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont list_type;
	typedef Handler ret_handler_type;
	cont.using_handler(boost::bind(&check_handler_zwei<list_type, ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Cont>
void test_using_handler(void)
{
	typedef Cont list_type;

	int arr1[5] = {1, 2, 3, 4, 5};
	int arr2[5] = {6, 7, 8, 9, 10};

	typename list_type::base_type base_cont1(arr1 + 0, arr1 + 5);
	typename list_type::base_type base_cont2(arr2 + 0, arr2 + 5);

	list_type cont;
	list_type& ref_cont = cont;
	const list_type& cref_cont = cont;

	ref_cont.using_handler(boost::bind(&set_handler_eins<list_type>, _1, boost::cref(base_cont1)));
	cref_cont.using_handler(boost::bind(&check_handler_eins<list_type>, _1, boost::cref(base_cont1)));

	int ref_count = 0;
	return_handler_call_helper(ref_cont, base_cont2, boost::bind(&return_handler, boost::ref(ref_count)));
	yggr_test_assert(ref_count == 1);

	int cref_count = 0;
	return_handler_call_helper(cref_cont, base_cont2, boost::bind(&return_handler, boost::ref(cref_count)));
	yggr_test_assert(cref_count == 1);

	ref_cont.for_each_iterator(boost::bind(&ref_all_check_handler<typename list_type::iterator, list_type>,
											_1, boost::cref(base_cont2)));
	cref_cont.for_each_iterator(boost::bind(&cref_all_check_handler<typename list_type::const_iterator, list_type>,
												_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.for_each_iterator(boost::bind(&ref_all_check_handler<typename list_type::iterator, list_type>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<typename list_type::iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(ref_count)));
	yggr_test_assert(ref_count == 2);

	cref_count = 0;
	cref_cont.for_each_iterator(boost::bind(&cref_all_check_handler<typename list_type::const_iterator, list_type>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<typename list_type::const_iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(cref_count)));
	yggr_test_assert(cref_count == 2);

	ref_cont.for_each_iterator_reverse(boost::bind(&ref_all_check_handler<typename list_type::reverse_iterator, list_type>,
													_1, boost::cref(base_cont2)));
	cref_cont.for_each_iterator_reverse(boost::bind(&cref_all_check_handler<typename list_type::const_reverse_iterator, list_type>,
													_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.for_each_iterator_reverse(boost::bind(&ref_all_check_handler<typename list_type::reverse_iterator, list_type>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<typename list_type::reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(ref_count)));
	yggr_test_assert(ref_count == 2);

	cref_count = 0;
	cref_cont.for_each_iterator_reverse(boost::bind(&cref_all_check_handler<typename list_type::const_reverse_iterator, list_type>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<typename list_type::const_reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(cref_count)));
	yggr_test_assert(cref_count == 2);


	std::cout << "---------------test_use_handler end-----------------" << std::endl;
}

template<std::size_t V>
struct test_pred_t
{
	inline bool operator()(int n) const
	{
		return n == V;
	}
};

template<typename Cont>
void test_cpp20_erase(void)
{
	typedef Cont list_type;

	int arr[5] = {1, 1, 2, 2, 3};

	{
		list_type cont(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::safe_container::erase(cont, 1);
		yggr_test_assert(s == 2);

		s = yggr::safe_container::erase(cont, 6);
		yggr_test_assert(s == 0);
	}

	{
		list_type cont(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::safe_container::erase_if(cont, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::safe_container::erase_if(cont, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	std::cout << "---------------test_cpp20_erase end-----------------" << std::endl;
}

template<typename Cont>
void debug_test(void)
{
	typedef Cont list_type;

	test_constructor<list_type>();
	test_capacity<list_type>();
	test_element_access<list_type>();
	test_safe_element_reference<list_type>();
	test_modifiers<list_type>();
	test_operators<list_type>();
	test_safe_other<list_type>();
	test_swap<list_type>();
	test_using_handler<list_type>();
	test_cpp20_erase<list_type>();
}

int main(int argc, char* argv[])
{
	//{
	//	typedef yggr::safe_container::safe_list<int> list_type;
	//	debug_test<list_type>();
	//}

	//{
	//	typedef yggr::safe_container::safe_list<int, boost::mutex> list_type;
	//	debug_test<list_type>();
	//}

	//{
	//	typedef yggr::safe_container::safe_list<int, boost::mutex, std::allocator<int> > list_type;
	//	debug_test<list_type>();
	//}

	//{
	//	typedef yggr::safe_container::safe_list<int, boost::mutex, std::allocator<int>, std::list> list_type;
	//	debug_test<list_type>();
	//}

	//{
	//	typedef yggr::safe_container::safe_list<int, boost::mutex, std::allocator<int>, boost::container::list> list_type;
	//	debug_test<list_type>();
	//}

	//{
	//	typedef yggr::safe_container::safe_list<int, boost::mutex, std::allocator<int>, yggr::list> list_type;
	//	debug_test<list_type>();
	//}

#if !(BOOST_VERSION < 105800)
	{
		typedef yggr::safe_container::safe_list<int, boost::mutex, boost::container::new_allocator<int> > list_type;
		debug_test<list_type>();
	}

	{
		typedef
			yggr::safe_container::safe_list
			<
				int,
				boost::mutex,
				boost::container::new_allocator<int>,
				boost::container::list
			> list_type;
		debug_test<list_type>();
	}

	{
		typedef
			yggr::safe_container::safe_list
			<
				int,
				boost::mutex,
				boost::container::new_allocator<int>,
				yggr::list
			> list_type;
		debug_test<list_type>();
	}
#endif // (BOOST_VERSION < 105800)

	wait_any_key(argc, argv);
	return 0;
}
