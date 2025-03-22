//safe_deque_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/thread/boost_thread.hpp>
#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/safe_container/safe_deque.hpp>

#include <yggr/compile_link/linker.hpp>

//#include <yggr/ppex/pp_debug.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)

#define THREAD_TEST_USING_MANUAL() 0

#define THREAD_TEST_THREAD_SIZE() 10
#define THREAD_TEST_THREAD_LOOP_SIZE() 10

//-------------------------test base-------------------------

template<typename Iter>
void check_iter_values(const Iter& iter, int n)
{
	yggr_test_assert(*iter == n);
}

template<typename Iter>
void check_riter_values(const Iter& iter, int n)
{
	yggr_test_assert(*iter == n);
}

template<typename Val>
void check_values(const Val& val, int n)
{
	yggr_test_assert(val == n);
}

template<typename Cont>
void test_constructor(void)
{
	typedef Cont deque_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::const_reverse_iterator criter_type;
	typedef typename deque_type::value_type val_type;

	deque_type cont1;
	yggr_test_assert(cont1.empty());

	deque_type cont2(cont1.get_allocator());
	yggr_test_assert(cont2.empty());

	deque_type cont3(10, 1);
	cont3.for_each_iterator(boost::bind(&check_iter_values<citer_type>, _1, 1));
	cont3.for_each(boost::bind(&check_values<val_type>, _1, 1));

	cont3.for_each_iterator_reverse(boost::bind(&check_iter_values<criter_type>, _1, 1));
	cont3.for_each_reverse(boost::bind(&check_values<val_type>, _1, 1));

	deque_type cont4(10, 1, cont2.get_allocator());
	cont4.for_each_iterator(boost::bind(&check_iter_values<citer_type>, _1, 1));
	cont4.for_each(boost::bind(&check_values<val_type>, _1, 1));

	int arr[10] = {1, 2, 3, 4, 5};
	deque_type cont5(arr + 0, arr + 10);
	yggr_test_assert(cont5.size() == 10);

	deque_type cont6(arr + 0, arr + 10, cont5.get_allocator());
	yggr_test_assert(cont6.size() == 10);

	typename deque_type::base_type base_cont7(arr + 0, arr + 10);
	deque_type cont7(base_cont7);
	yggr_test_assert(cont7.size() == 10 && base_cont7.size() == 10);

	typename deque_type::base_type base_cont8(arr + 0, arr + 10);
	deque_type cont8(boost::move(base_cont8));
	yggr_test_assert(cont8.size() == 10);

	typename deque_type::base_type base_cont9(arr + 0, arr + 10);
	deque_type cont9(base_cont9, cont7.get_allocator());
	yggr_test_assert(cont9.size() == 10 && base_cont9.size() == 10);

	typename deque_type::base_type base_cont10(arr + 0, arr + 10);
	deque_type cont10(boost::move(base_cont10), base_cont9.get_allocator());
	yggr_test_assert(cont10.size() == 10);

	{
		typename deque_type::base_type base_cont11(arr + 0, arr + 10);
		deque_type cont11(base_cont11);
		yggr_test_assert(cont11.size() == 10);
		yggr_test_assert(base_cont11.size() == 10);

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

		deque_type cont12(cont11);
		yggr_test_assert(cont12.size() == 10);

		// safe_copy_safe
		yggr_test_assert(cont11 == cont12);
		yggr_test_assert(!(cont11 != cont12));
		yggr_test_assert(cont11 <= cont12);
		yggr_test_assert(!(cont11 < cont12));
		yggr_test_assert(cont11 >= cont12);
		yggr_test_assert(!(cont11 > cont12));

		deque_type cont13(boost::move(cont12));
		yggr_test_assert(cont13.size() == 10);
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void check_range_equal(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont deque_type;

	////typename deque_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(typename deque_type::size_type i = 0, isize = l.size(); i != isize; ++i)
	{
		yggr_test_assert(l[i] == r[i]);
	}
}

template<typename Cont>
void test_capacity(void)
{
	typedef Cont deque_type;

	int arr[10] = {1, 2, 3, 4, 5};
	deque_type cont1(arr + 0, arr + 10);
	typename deque_type::base_type base_cont1(arr + 0, arr + 10);

	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.max_size() == base_cont1.max_size());

	cont1.resize(20);
	base_cont1.resize(20);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.max_size() == base_cont1.max_size());

	cont1.resize(30, 100);
	base_cont1.resize(30, 100);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.max_size() == base_cont1.max_size());
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));

	yggr_test_assert(cont1.empty() == base_cont1.empty());

	cont1.shrink_to_fit();

#if !(defined(__GNUC__))
	base_cont1.shrink_to_fit();
#endif // __GNUC__

	yggr_test_assert(cont1.size() == base_cont1.size());

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

template<typename Cont>
typename Cont::iterator iter_handler(typename Cont::base_type& base, const typename Cont::value_type& src)
{
	return std::find(base.begin(), base.end(), src);
}

template<typename Cont>
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
	typedef Cont deque_type;

	int arr[10] = {1, 2, 3, 4, 5};
	deque_type cont(arr + 0, arr + 10);
	typename deque_type::base_type base_cont(arr + 0, arr + 10);

	int val =0;
	for(int i = 0, isize = base_cont.size(); i != isize ; ++i)
	{
		yggr_test_assert(cont.get_value(i, val));
		yggr_test_assert(val == base_cont[i]);
	}

	yggr_test_assert(cont.set_value(0, 100));
	int n = 200;
	yggr_test_assert(cont.set_value(1, boost::move(n)));
	yggr_test_assert(!cont.set_value(10, 0xeeeeeeee));

	base_cont[0] = 100;
	base_cont[1] = 200;

	int f = 0, b = 0;
	deque_type cont2;
	deque_type& cont_ref = cont2;
	const deque_type& cont_cref = cont2;

	try
	{
		yggr_test_assert(cont2.empty());
		cont_ref[0] = 10;
		yggr_test_assert(false);
	}
	catch(const typename deque_type::error_type&)
	{
	}

	try
	{
		yggr_test_assert(cont2.empty());
		cont_cref[0];
		yggr_test_assert(false);
	}
	catch(const typename deque_type::error_type&)
	{
	}

	try
	{
		yggr_test_assert(cont2.empty());
		cont_ref.front() = 10;
		yggr_test_assert(false);
	}
	catch(const typename deque_type::error_type&)
	{
	}

	try
	{
		yggr_test_assert(cont2.empty());
		cont_cref.front();
		yggr_test_assert(false);
	}
	catch(const typename deque_type::error_type&)
	{
	}

	try
	{
		yggr_test_assert(cont2.empty());
		cont_ref.back() = 10;
		yggr_test_assert(false);
	}
	catch(const typename deque_type::error_type&)
	{
	}

	try
	{
		yggr_test_assert(cont2.empty());
		cont_cref.back();
		yggr_test_assert(false);
	}
	catch(const typename deque_type::error_type&)
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
	catch(const typename deque_type::error_type&)
	{
	}

	try
	{
		cont2.rbegin_value();
		yggr_test_assert(false);
	}
	catch(const typename deque_type::error_type&)
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
	yggr_test_assert(cont[0] == cont.front());
	yggr_test_assert(cont[cont.size() - 1] == cont.back());

	yggr_test_assert(cont[0] == cont[0]);
	yggr_test_assert(cont.at(0) != cont.at(1));

	try
	{
		const deque_type& cref_cont = cont;
		cont[5] = 60;
		yggr_test_assert(cont[5] == 60);
		yggr_test_assert(cref_cont[5] == 60);
		cont.at(6) = 70;
		yggr_test_assert(cont.at(6) == 70);
		yggr_test_assert(cref_cont.at(6) == 70);
	}
	catch(const typename deque_type::error_type&)
	{
		yggr_test_assert(false);
	}

	//emplace
	{
		deque_type ins_cont;
		ins_cont.emplace(boost::bind(&iter_handler<deque_type>, _1, 0), yggr::non_handler );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 0);
	}

	{
		deque_type ins_cont;
		yggr_test_assert(ins_cont.emplace(
						boost::bind(&iter_handler<deque_type>, _1, 0),
						boost::bind(&ins_handler<deque_type>, _1, _2, 0)) );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 0);

	}

	{
		deque_type ins_cont;
		yggr_test_assert(ins_cont.emplace(
						boost::bind(&iter_handler<deque_type>, _1, 10),
						boost::bind(&ins_handler<deque_type>, _1, _2, 10), 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		deque_type ins_cont;
		ins_cont.emplace(boost::bind(&iter_handler<deque_type>, _1, 10), yggr::non_handler, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		deque_type ins_cont;
		ins_cont.emplace(boost::bind(&iter_handler<deque_type>, _1, 10), yggr::non_handler, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		deque_type ins_cont;
		ins_cont.emplace_front();
		ins_cont.emplace_back();
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.front() == 0);
		yggr_test_assert(ins_cont.back() == 0);
	}

	{
		deque_type ins_cont;
		ins_cont.emplace_front(10);
		ins_cont.emplace_back(20);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.front() == 10);
		yggr_test_assert(ins_cont.back() == 20);
	}

	// insert

	{
		int val = 20;
		deque_type ins_cont;
		ins_cont.insert(boost::bind(&iter_handler<deque_type>, _1, 10), 10);
		ins_cont.insert(boost::bind(&iter_handler<deque_type>, _1, 20), boost::move(val));

		yggr_test_assert(ins_cont.size() == 2);
	}

	{
		int val = 20;
		deque_type ins_cont;

		ins_cont.insert(boost::bind(&iter_handler<deque_type>, _1, 10),
							10,
							boost::bind(&ins_handler<deque_type>, _1, _2, 10));

		ins_cont.insert(boost::bind(&iter_handler<deque_type>, _1, 20),
							boost::move(val),
							boost::bind(&ins_handler<deque_type>, _1, _2, 20));

		yggr_test_assert(ins_cont.size() == 2);
	}

	{
		//int val = 20;
		deque_type ins_cont;
		ins_cont.insert(boost::bind(&iter_handler<deque_type>, _1, 10), 3, 10);
		yggr_test_assert(ins_cont.size() == 3);
	}

	{
		deque_type ins_cont;
		ins_cont.insert(boost::bind(&iter_handler<deque_type>, _1, 10), arr + 0, arr + 10);
		yggr_test_assert(ins_cont.size() == 10);
	}

	std::cout << "---------------test_element_access end-----------------" << std::endl;
}

template<typename Cont>
void test_safe_element_reference(void)
{
	typedef Cont deque_type;

	int arr1[10] = {10, 11, 12};
	int arr2[10] = {20, 21, 22};

	deque_type cont1(arr1 + 0, arr1 + 10);
	deque_type cont2(arr2 + 0, arr2 + 10);

#define TEST_OPERATOR( __op__ ) \
	a = cont1[0] __op__ cont1[0]; \
	yggr_test_assert(a == (arr1[0] __op__ arr1[0])); \
	a = cont1[0] __op__ cont2[0]; \
	yggr_test_assert(a == (arr1[0] __op__ arr2[0]));


	{
		int a = 0;
		{
			a = cont1[0] + cont1[0];
			yggr_test_assert(a == arr1[0]+ arr1[0]);

			a = cont1[0] + cont2[0];
			yggr_test_assert(a == arr1[0] + arr2[0]);
		}

		{
			a = cont1[0] - cont1[0];
			yggr_test_assert(a == arr1[0] - arr1[0]);

			a = cont1[0] - cont2[0];
			yggr_test_assert(a == arr1[0] - arr2[0]);
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
		cont1[5] __op__ cont1[0]; \
		yggr_test_assert(cont1[5] == (a __op__ cont1[0])); \
		yggr_test_assert(cont1[5] == (b __op__ arr1[0])); \
		cont1[5] = 0; \
		a = 0; b = 0; \
		\
		cont1[5] __op__ cont2[0]; \
		yggr_test_assert(cont1[5] == (a __op__ cont2[0])); \
		yggr_test_assert(cont1[5] == (b __op__ arr2[0])); \
		cont1[5] = 0; \
		a = 0; b = 0;


	{
		int a = 0;
		int b = 0;

		cont1[5] = cont1[0];
		yggr_test_assert(cont1[5] == cont1[0]);
		yggr_test_assert(cont1[5] == arr1[0]);
		cont1[5] = 0;

		cont1[5] = cont2[0];
		yggr_test_assert(cont1[5] == cont2[0]);
		yggr_test_assert(cont1[5] == arr2[0]);
		cont1[5] = 0;

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

	{
		// +
		cont1[5] = +cont1[0];
		yggr_test_assert(cont1[5] == cont1[0]);

		cont1[5] = +cont2[0];
		yggr_test_assert(cont1[5] == cont2[0]);

		// -
		cont1[5] = -cont1[0];
		yggr_test_assert(cont1[5] == -cont1[0]);

		cont1[5] = -cont2[0];
		yggr_test_assert(cont1[5] == -cont2[0]);

		// ++
		cont1[5] = ++cont1[1];
		yggr_test_assert(cont1[5] == cont1[1]);

		cont1[5] = ++cont2[1];
		yggr_test_assert(cont1[5] == cont2[1]);

		cont1[5] = cont1[1]++;
		yggr_test_assert(cont1[5] == cont1[1] - 1);

		cont1[5] = cont2[1]++;
		yggr_test_assert(cont1[5] == cont2[1] - 1);

		// --
		cont1[5] = --cont1[1];
		yggr_test_assert(cont1[5] == cont1[1]);

		cont1[5] = --cont2[1];
		yggr_test_assert(cont1[5] == cont2[1]);

		cont1[5] = cont1[1]--;
		yggr_test_assert(cont1[5] == cont1[1] + 1);

		cont1[5] = cont2[1]--;
		yggr_test_assert(cont1[5] == cont2[1] + 1);

		// ~
		cont1[5] = ~cont1[0];
		yggr_test_assert(cont1[5] == ~cont1[0]);

		cont1[5] = ~cont2[0];
		yggr_test_assert(cont1[5] == ~cont2[0]);
	}

	{
		yggr_test_assert(cont1[0] && cont1[1]);
		yggr_test_assert(cont1[0] && cont2[1]);

		yggr_test_assert(cont1[0] || cont1[8]);
		yggr_test_assert(cont1[0] || cont2[8]);

		yggr_test_assert(!(cont1[8] || cont1[8]));
		yggr_test_assert(!(cont1[8] || cont2[8]));

		yggr_test_assert(!cont1[8]);
		yggr_test_assert(cont1[0]);
	}

	{
		yggr_test_assert(cont1[0] < cont1[1]);
		yggr_test_assert(cont1[0] < cont2[1]);

		yggr_test_assert(cont1[0] <= cont1[0]);
		yggr_test_assert(cont1[0] <= cont2[0]);

		yggr_test_assert(cont1[0] > cont1[8]);
		yggr_test_assert(cont1[0] > cont2[8]);

		yggr_test_assert(cont1[0] >= cont1[0]);
		yggr_test_assert(cont1[0] >= cont2[8]);

		yggr_test_assert(cont1[0] == cont1[0]);
		yggr_test_assert(cont1[0] != cont2[8]);
	}

	// mix
	{
		{
			int num = (cont1[5] = cont1[8] + cont2[0] + cont2[1] * cont1[3]);
			yggr_test_assert(num == (arr1[8] + arr2[0] + arr2[1] * arr1[3]));
		}

		{
			int num = (cont1[5] = 1 + cont1[0] + cont2[1] * 3);
			yggr_test_assert(num == (1 + arr1[0] + arr2[1] * 3));
		}
	}

	{
		typedef yggr::safe_container::safe_deque<float> fdeque_type;

		fdeque_type cont3(arr1 + 0, arr1 + 10);
		cont1[5] = cont3[0];

		cont1[5] += cont3[0];
		yggr_test_assert(cont1[5] == cont3[0] * 2);
	}

	{
		typedef yggr::safe_container::safe_deque<yggr::shared_ptr<int> > ptr_deque_type;

		ptr_deque_type cont3;
		cont3.push_back(yggr::shared_ptr<int>(yggr_nothrow_new int(1)));

		yggr_test_assert(*cont3[0] == 1);
		yggr::shared_ptr<int> ptr = cont3[0];
		*ptr = 100;
		yggr_test_assert(*cont3.front() == 100);
		yggr_test_assert(*cont3.back() == 100);

	}

	std::cout << "---------------test_safe_element_reference end-----------------" << std::endl;
}

template<typename Cont>
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
	typedef Cont deque_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::value_type val_type;

	int arr[10] = {1, 2, 3, 4, 5};
	deque_type cont;
	typename deque_type::base_type base_cont;

	cont.assign(7, 100);
	base_cont.assign(7, 100);

	yggr_test_assert(cont.size() == base_cont.size());
	cont.for_each_iterator(boost::bind(&check_iter_values<citer_type>, _1, 100));
	cont.for_each(boost::bind(&check_values<val_type>, _1, 100));

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

		deque_type cont2;
		yggr_test_assert(!cont2.pop_back(b));
		yggr_test_assert(b == 2000);
	}

	{
		int b = 0;
		yggr_test_assert(cont.pop_front(b));
		yggr_test_assert(b == 6000);
		base_cont.pop_front();
		yggr_test_assert(cont.size() == base_cont.size());

		deque_type cont2;
		yggr_test_assert(!cont2.pop_front(b));
		yggr_test_assert(b == 6000);
	}

	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont)));
	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont)));

	typename deque_type::base_type base_cont2(base_cont);

	yggr_test_assert(!cont.empty());
	yggr_test_assert(!base_cont2.empty());
	cont.swap(boost::move(base_cont2));
	yggr_test_assert(!cont.empty() && !base_cont2.empty());
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont)));

	deque_type cont3(base_cont);

	cont.swap(cont3);
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont)));
	cont3.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont)));

	{
		cont.emplace_front(200);
		base_cont.push_front(200);

		int chk_val = 0;

		cont.get_front(chk_val);
		yggr_test_assert(chk_val == base_cont.front());

		cont.emplace_back(300);
		base_cont.push_back(300);

		cont.get_back(chk_val);
		yggr_test_assert(chk_val == base_cont.back());

		yggr_test_assert(cont.size() == base_cont.size());
	}

	{
		typedef typename deque_type::base_type base_deque_type;
		typedef typename base_deque_type::iterator iter_type;
		typedef typename base_deque_type::const_iterator citer_type;

		typedef iter_type (base_deque_type::* begin_end_foo_type)(void);
		typedef citer_type (base_deque_type::* cbegin_end_foo_type)(void) const;

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_deque_type::begin), _1));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_deque_type::begin), _1),
					boost::bind(&handler_erase<deque_type>, _1, _2));

		cont.erase(boost::bind(&make_erase_iter_pair<deque_type>, _1),
					boost::bind(&handler_erase<deque_type>, _1, _2));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_deque_type::begin), _1),
					boost::bind(static_cast<begin_end_foo_type>(&base_deque_type::end), _1));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_deque_type::begin), _1),
					boost::bind(static_cast<begin_end_foo_type>(&base_deque_type::end), _1),
					boost::bind(&handler_erase<deque_type>, _1, _2));

		yggr_test_assert(cont.empty());
	}

	cont.clear();
	yggr_test_assert(cont.empty());

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

template<typename Cont>
void test_safe_other(void)
{
	typedef Cont deque_type;

	int arr[10] = {1, 2, 3, 4, 5};
	deque_type cont(arr + 0, arr + 10);
	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(cont.is_exists(0));
	yggr_test_assert(!cont.is_exists(100));

	typename deque_type::base_type base_cont1 = cont.load();
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));

	typename deque_type::base_type base_cont2 = cont;
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont>
void test_swap(void)
{
	typedef Cont deque_type;

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	deque_type cont1(arr1 + 0, arr1 + 10);
	deque_type cont2(arr2 + 0, arr2 + 5);

	typename deque_type::base_type base_cont1(arr1 + 0, arr1 + 10);
	typename deque_type::base_type base_cont2(arr2 + 0, arr2 + 5);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() != base_cont2.size());


	typename deque_type::base_type base_cont3(base_cont1);
	typename deque_type::base_type base_cont4(base_cont2);

	// ------------------ boost::swap test------------------------
	// base_type&& this_type&
	boost::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	boost::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	boost::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	boost::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	boost::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));

	boost::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	boost::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	boost::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));

	// ------------------ std::swap test------------------------
	// base_type&& this_type&
	std::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	std::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	std::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	std::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	std::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));

	std::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	std::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	std::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));

	// ------------------ yggr::swap test------------------------
	// base_type&& this_type&
	yggr::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	yggr::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	yggr::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	yggr::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	yggr::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));

	yggr::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	yggr::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	yggr::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));

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

template<typename Iter, typename Cont>
void ref_all_check_handler(Iter iter, const typename Cont::base_type& r)
{
	typedef Cont deque_type;

	typename deque_type::base_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	yggr_test_assert(i != r.end());
	*iter = *i;
}

template<typename Iter, typename Cont>
void cref_all_check_handler(Iter iter, const typename Cont::base_type& r)
{
	typedef Cont deque_type;

	typename deque_type::base_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	yggr_test_assert(i != r.end());
}

template<typename Cont>
void check_handler_eins(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont deque_type;

	////typename deque_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(typename deque_type::size_type i = 0, isize = l.size(); i != isize; ++i)
	{
		yggr_test_assert(l[i] == r[i]);
	}
}

template<typename Cont, typename Handler>
void check_handler_zwei(const typename Cont::base_type& l,
						const typename Cont::base_type& r,
						const Handler& ret_handler)
{
	typedef Cont deque_type;

	////typename deque_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(typename deque_type::size_type i = 0, isize = l.size(); i != isize; ++i)
	{
		yggr_test_assert(l[i] == r[i]);
	}
	ret_handler();
}

template<typename Cont>
void set_handler_eins(typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont deque_type;

	if(!l.empty())
	{
		yggr_test_assert(l.size() == r.size());
		for(typename deque_type::size_type i = 0, isize = l.size(); i != isize; ++i)
		{
			yggr_test_assert(l[i] != r[i]);
		}
	}
	l = r;
	{
		for(typename deque_type::size_type i = 0, isize = l.size(); i != isize; ++i)
		{
			yggr_test_assert(l[i] == r[i]);
		}
	}
}

template<typename Cont, typename Handler>
void set_handler_zwei(typename Cont::base_type& l, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont deque_type;

	if(!l.empty())
	{
		yggr_test_assert(l.size() == r.size());
		for(typename deque_type::size_type i = 0, isize = l.size(); i != isize; ++i)
		{
			yggr_test_assert(l[i] != r[i]);
		}
	}
	l = r;
	{
		for(typename deque_type::size_type i = 0, isize = l.size(); i != isize; ++i)
		{
			yggr_test_assert(l[i] == r[i]);
		}
	}

	ret_handler();
}

template<typename Cont, typename Handler>
void return_handler_call_helper(Cont& cont, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Handler ret_handler_type;

	cont.using_handler(boost::bind(&set_handler_zwei<Cont, ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Cont, typename Handler>
void return_handler_call_helper(const Cont& cont, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Handler ret_handler_type;

	cont.using_handler(boost::bind(&check_handler_zwei<Cont, ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Cont>
void test_using_handler(void)
{
	typedef Cont deque_type;

	int arr1[5] = {1, 2, 3, 4, 5};
	int arr2[5] = {6, 7, 8, 9, 10};

	typename deque_type::base_type base_cont1(arr1 + 0, arr1 + 5);
	typename deque_type::base_type base_cont2(arr2 + 0, arr2 + 5);

	deque_type cont;
	deque_type& ref_cont = cont;
	const deque_type& cref_cont = cont;

	ref_cont.using_handler(boost::bind(&set_handler_eins<deque_type>, _1, boost::cref(base_cont1)));
	cref_cont.using_handler(boost::bind(&check_handler_eins<deque_type>, _1, boost::cref(base_cont1)));

	int ref_count = 0;
	return_handler_call_helper(ref_cont, base_cont2, boost::bind(&return_handler, boost::ref(ref_count)));
	yggr_test_assert(ref_count == 1);

	int cref_count = 0;
	return_handler_call_helper(cref_cont, base_cont2, boost::bind(&return_handler, boost::ref(cref_count)));
	yggr_test_assert(cref_count == 1);

	ref_cont.for_each_iterator(boost::bind(&ref_all_check_handler<typename deque_type::iterator, deque_type>,
											_1, boost::cref(base_cont2)));
	cref_cont.for_each_iterator(boost::bind(&cref_all_check_handler<typename deque_type::const_iterator, deque_type>,
												_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.for_each_iterator(boost::bind(&ref_all_check_handler<typename deque_type::iterator, deque_type>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<typename deque_type::iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(ref_count)));
	yggr_test_assert(ref_count == 2);

	cref_count = 0;
	cref_cont.for_each_iterator(boost::bind(&cref_all_check_handler<typename deque_type::const_iterator, deque_type>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<typename deque_type::const_iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(cref_count)));
	yggr_test_assert(cref_count == 2);

	ref_cont.for_each_iterator_reverse(boost::bind(&ref_all_check_handler<typename deque_type::reverse_iterator, deque_type>,
													_1, boost::cref(base_cont2)));
	cref_cont.for_each_iterator_reverse(boost::bind(&cref_all_check_handler<typename deque_type::const_reverse_iterator, deque_type>,
													_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.for_each_iterator_reverse(boost::bind(&ref_all_check_handler<typename deque_type::reverse_iterator, deque_type>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<typename deque_type::reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(ref_count)));
	yggr_test_assert(ref_count == 2);

	cref_count = 0;
	cref_cont.for_each_iterator_reverse(boost::bind(&cref_all_check_handler<typename deque_type::const_reverse_iterator, deque_type>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<typename deque_type::const_reverse_iterator, std::less<int> >,
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
	typedef Cont deque_type;

	int arr[5] = {1, 1, 2, 2, 3};

	{
		deque_type cont(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::safe_container::erase(cont, 1);
		yggr_test_assert(s == 2);

		s = yggr::safe_container::erase(cont, 6);
		yggr_test_assert(s == 0);
	}

	{
		deque_type cont(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::safe_container::erase_if(cont, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::safe_container::erase_if(cont, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	std::cout << "---------------test_cpp20_erase end-----------------" << std::endl;
}

//#endif // _DEBUG
//

template<typename Cont>
void debug_test(void)
{
	typedef Cont deque_type;

//#ifdef _DEBUG
	test_constructor<deque_type>();
	test_capacity<deque_type>();
	test_element_access<deque_type>();
	test_safe_element_reference<deque_type>();
	test_modifiers<deque_type>();
	test_safe_other<deque_type>();
	test_swap<deque_type>();
	test_using_handler<deque_type>();
	test_cpp20_erase<deque_type>();
//#endif // _DEBUG
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

template<typename Container>
void ref_handler(typename Container::base_type& cont)
{
	if(cont.size() % 2 == 0)
	{
		cont.erase(cont.begin(), cont.begin() + cont.size() / 2);
	}
}
template<typename Container>
void cref_handler(const typename Container::base_type& cont)
{
	int n = cont.size();
	std::cout << n <<std::endl;
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
	typedef typename container_type::value_type val_type;

	if(E_max_elem < hvt.size())
	{
		hvt.clear();
	}
	int op = std::rand() % 10;
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
	case 2:
	case 3: // empty
		hvt.push_back(3 + E_read);
		cont.empty();
		break;
	case 4: // get_value
		{
		    hvt.push_back(4 + E_read);
		    int s = cont.size();
			val_type val = val_type();
			s = s? std::rand() % s : 0;
			cont.get_value(s, val);
		}
		break;
	case 5: // front
		{
			hvt.push_back(5 + E_read);
			val_type p;
			cont.get_front(p);
			try { cont.front(); } catch(const typename container_type::error_type&) {}
		}
		break;
	case 6: // back
		{
			hvt.push_back(6 + E_read);
			val_type p;
			cont.get_back(p);
			try { cont.back(); } catch(const typename container_type::error_type&) {}
		}
		break;
	case 7: // get_allocator
		hvt.push_back(7 + E_read);
		cont.get_allocator();
		break;
	case 8: // is_exisits
		{
			hvt.push_back(8 + E_read);
			int s = cont.size();
			s = s? std::rand() % s : 0;
			val_type p;
			cont.get_value(s, p);
			cont.is_exists(p);
			cont.is_exists(create_val<val_type>());
		}
		break;
	case 9: // using_handler const
		hvt.push_back(9 + E_read);
		cont.using_handler(boost::bind(&cref_handler<container_type>, _1));
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
	typedef typename container_base_type::iterator iter_type;
	typedef typename container_base_type::const_iterator citer_type;

	typedef iter_type (container_base_type::* begin_end_foo_type)(void);
	typedef citer_type (container_base_type::* cbegin_end_foo_type)(void) const;

	if(E_max_elem < hvt.size())
	{
		hvt.clear();
	}

	int op = std::rand() % 20;
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
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			cont = tmp;
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
			try
			{
				val_type val = cont.back();
				if(val)
				{
					*val += *cont[1] + (*cont[2]) * (*cont[3]);
				}
				else
				{
					typename val_type::element_type m = *cont[1] + (*cont[2]) * (*cont[3]);
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
		}
		break;
	case 2: // resize
		hvt.push_back(2 + E_write);
		cont.resize(std::rand() % 100);
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
		break;
	case 3: // resize(size, val)
		hvt.push_back(3 + E_write);
		cont.resize(std::rand() % 100, create_val<val_type>());
		break;
	case 4:
	case 5: //shrink_to_fit
		hvt.push_back(5 + E_write);
		cont.shrink_to_fit();
		break;
	case 6: // set_value(idx, val&&)
		{
			hvt.push_back(6 + E_write);
			int s = cont.size();
			s = s? std::rand() % s : 0;
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
            cont.set_value(s, boost::move(create_val<val_type>()));
			cont.set_value(std::rand(), boost::move(create_val<val_type>()));
#else
            {
                val_type val1(create_val<val_type>()), val2(create_val<val_type>());
                cont.set_value(s, boost::move(val1));
                cont.set_value(s, boost::move(val2));
            }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
		}
		break;
	case 7: // set_value(idx, val)
		{
			hvt.push_back(7 + E_write);
			int s = cont.size();
			s = s? std::rand() % s : 0;
			cont.set_value(s, create_val<val_type>());
			cont.set_value(std::rand(), create_val<val_type>());
		}
		break;
	case 8: // assign(iter, iter)
		{
			hvt.push_back(8 + E_write);
			container_base_type tmp;
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			tmp.push_back(create_val<val_type>());
			cont.assign(tmp.begin(), tmp.end());
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
			cont.emplace(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1), yggr::non_handler);
			cont.emplace(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1), yggr::non_handler, create_val<val_type>());
			cont.emplace_front();
			cont.emplace_front(create_val<val_type>());
			cont.emplace_back();
			cont.emplace_back(create_val<val_type>());
		}
		break;
	case 9: // assign(size, val)
		{
			hvt.push_back(9 + E_write);
			cont.assign(rand() % 20, create_val<val_type>());
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
			//int count = 0;
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			cont.push_back(boost::move(create_val<val_type>()));
#else
            {
                val_type val(create_val<val_type>());
                cont.push_back(boost::move(val));
            }
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
			//yggr_test_assert(count == 1);
		}
		break;
	case 13: // // push_back(const val&, handler)
		{
			hvt.push_back(13 + E_write);
			//int count = 0;
			cont.push_back(create_val<val_type>());
			//yggr_test_assert(count == 1);
		}
		break;
	case 14:
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
			try
			{
				val_type val = cont.back();
				if(val)
				{
					*val += *cont[1] + (*cont[2]) * (*cont[3]);
				}
				else
				{
					typename val_type::element_type m = *cont[1] + (*cont[2]) * (*cont[3]);
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
		}
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
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
			try
			{
				val_type val = cont.back();
				if(val)
				{
					*val += *cont[1] + (*cont[2]) * (*cont[3]);
				}
				else
				{
					typename val_type::element_type m = *cont[1] + (*cont[2]) * (*cont[3]);
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}

			cont.emplace(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1), yggr::non_handler);
			cont.emplace(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1), yggr::non_handler, create_val<val_type>());
			cont.emplace_front();
			cont.emplace_front(create_val<val_type>());
			cont.emplace_back();
			cont.emplace_back(create_val<val_type>());
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
			try
			{
				val_type val = cont.back();
				if(val)
				{
					*val += *cont[1] + (*cont[2]) * (*cont[3]);
				}
				else
				{
					typename val_type::element_type m = *cont[1] + (*cont[2]) * (*cont[3]);
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}

			cont.emplace(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1), yggr::non_handler);
			cont.emplace(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1), yggr::non_handler, create_val<val_type>());
			cont.emplace_front();
			cont.emplace_front(create_val<val_type>());
			cont.emplace_back();
			cont.emplace_back(create_val<val_type>());
		}
		break;
	case 18: //clear
		hvt.push_back(18 + E_write);
		cont.clear();
		break;
	case 19: // use handler
		hvt.push_back(19 + E_write);
		cont.using_handler(boost::bind(&ref_handler<container_type>, _1));
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

	if(!trd_size)
	{
		return;
	}

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
		typedef yggr::safe_container::safe_deque<int> deque_type;
		debug_test<deque_type>();
	}

	{
		typedef yggr::safe_container::safe_deque<int, boost::mutex, std::allocator<int> > deque_type;
		debug_test<deque_type>();
	}

#if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	{
		typedef yggr::safe_container::safe_deque<int, boost::mutex, std::allocator<int>, std::deque> deque_type;
		debug_test<deque_type>();
	}

#endif // #if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))

	std::srand(std::time(0));

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_deque<val_type> deque_type;
		test_thread_safe_of_mutex<deque_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_deque<val_type, boost::mutex, std::allocator<val_type> > deque_type;
		test_thread_safe_of_mutex<deque_type>();
	}


#if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_deque<val_type, boost::mutex, std::allocator<val_type>, std::deque> deque_type;
		test_thread_safe_of_mutex<deque_type>();
	}
#endif // #if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_deque<val_type> deque_type;
		test_thread_safe_of_shared_mutex<deque_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_deque<val_type, boost::mutex, std::allocator<val_type> > deque_type;
		test_thread_safe_of_shared_mutex<deque_type>();
	}

#if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_deque<val_type, boost::mutex, std::allocator<val_type>, std::deque> deque_type;
		test_thread_safe_of_shared_mutex<deque_type>();
	}
#endif // #if !(defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940))

	wait_any_key(argc, argv);
	return 0;
}
