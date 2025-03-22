//safe_options_deque_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/bind.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/utility/swap.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/safe_container/safe_options_deque.hpp>

#include <yggr/compile_link/linker.hpp>


#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(segment)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)

template<typename Seg>
void delete_segment(const yggr::string& name)
{
	typedef Seg seg_type;
	typedef yggr::segment::segment_destroyer<seg_type> destroyer_type;

	if(name.empty())
	{
		return;
	}

	{ destroyer_type del(name.c_str()); }
}

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

template<typename Cont, typename SegCont>
void test_constructor(SegCont& seg_cont)
{
	typedef Cont deque_type;
	typedef SegCont seg_cont_type;

	typedef typename deque_type::base_type base_deque_type;
	typedef typename deque_type::allocator_type alloc_type;
	typedef typename deque_type::iterator iter_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::value_type val_type;

	{
		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;
		yggr_test_assert(cont.empty());
		
		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				10, 1, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;
		cont.for_each_iterator(boost::bind(&check_iter_values<citer_type>, _1, 1));
		cont.for_each(boost::bind(&check_values<val_type>, _1, 1));
		
		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;
		yggr_test_assert(cont.size() == 10);
		
		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;
		yggr_test_assert(cont.size() == 10);
		
		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		base_deque_type base_cont(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));

		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				boost::move(base_cont));
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;
		yggr_test_assert(cont.size() == 10);
		
		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		base_deque_type base_cont(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));

		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				boost::move(base_cont), base_cont.get_allocator());
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;
		yggr_test_assert(cont.size() == 10);
		
		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		base_deque_type base_cont(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				base_cont);
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;

		yggr_test_assert(cont.size() == 10 && base_cont.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		base_deque_type base_cont(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				base_cont, base_cont.get_allocator());
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;

		yggr_test_assert(cont.size() == 10 && base_cont.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		deque_type cont0(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));

		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				boost::move(cont0));
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;
		yggr_test_assert(cont.size() == 10);
		
		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		deque_type cont0(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));

		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				boost::move(cont0), cont0.get_allocator());
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;
		yggr_test_assert(cont.size() == 10);
		
		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		deque_type cont0(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				cont0);
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;

		yggr_test_assert(cont.size() == 10 && cont0.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		base_deque_type cont0(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		deque_type* pcont = 
			seg_cont.segment().template construct<deque_type>("deque")(
				cont0, cont0.get_allocator());
		yggr_test_assert(pcont);
		
		deque_type& cont = *pcont;

		yggr_test_assert(cont.size() == 10 && cont0.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
	}
	
	// cmper
	{
		int arr[10] = {1, 2, 3, 4, 5};

		base_deque_type base_cont11(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
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

	//typename deque_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(typename deque_type::size_type i = 0, isize = l.size(); i != isize; ++i)
	{
		yggr_test_assert(l[i] == r[i]);
	}
}

template<typename Cont, typename SegCont>
void test_capacity(SegCont& seg_cont)
{
	typedef Cont deque_type;
	typedef SegCont seg_cont_type;

	typedef typename deque_type::base_type base_deque_type;
	typedef typename deque_type::allocator_type alloc_type;
	typedef typename deque_type::iterator iter_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::value_type val_type;

	int arr[10] = {1, 2, 3, 4, 5};
	deque_type cont1(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
	base_deque_type base_cont1(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));

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

template<typename Cont, typename SegCont>
void test_element_access(SegCont& seg_cont)
{
	typedef Cont deque_type;
	typedef SegCont seg_cont_type;

	typedef typename deque_type::base_type base_deque_type;
	typedef typename deque_type::allocator_type alloc_type;
	typedef typename deque_type::iterator iter_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::value_type val_type;

	int arr[10] = {1, 2, 3, 4, 5};
	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	deque_type cont(arr + 0, arr + 10, now_alloc);
	base_deque_type base_cont(arr + 0, arr + 10, now_alloc);

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
	deque_type cont2(now_alloc);
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
		deque_type ins_cont(now_alloc);
		ins_cont.emplace(boost::bind(&iter_handler<deque_type>, _1, 0), yggr::non_handler );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 0);
	}

	{
		deque_type ins_cont(now_alloc);
		yggr_test_assert(ins_cont.emplace(
						boost::bind(&iter_handler<deque_type>, _1, 0),
						boost::bind(&ins_handler<deque_type>, _1, _2, 0)) );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 0);

	}

	{
		deque_type ins_cont(now_alloc);
		yggr_test_assert(ins_cont.emplace(
						boost::bind(&iter_handler<deque_type>, _1, 10),
						boost::bind(&ins_handler<deque_type>, _1, _2, 10), 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		deque_type ins_cont(now_alloc);
		ins_cont.emplace(boost::bind(&iter_handler<deque_type>, _1, 10), yggr::non_handler, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		deque_type ins_cont(now_alloc);
		ins_cont.emplace(boost::bind(&iter_handler<deque_type>, _1, 10), yggr::non_handler, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		deque_type ins_cont(now_alloc);
		ins_cont.emplace_front();
		ins_cont.emplace_back();
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.front() == 0);
		yggr_test_assert(ins_cont.back() == 0);
	}

	{
		deque_type ins_cont(now_alloc);
		ins_cont.emplace_front(10);
		ins_cont.emplace_back(20);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.front() == 10);
		yggr_test_assert(ins_cont.back() == 20);
	}

	// insert

	{
		int val = 20;
		deque_type ins_cont(now_alloc);
		ins_cont.insert(boost::bind(&iter_handler<deque_type>, _1, 10), 10);
		ins_cont.insert(boost::bind(&iter_handler<deque_type>, _1, 20), boost::move(val));

		yggr_test_assert(ins_cont.size() == 2);
	}

	{
		int val = 20;
		deque_type ins_cont(now_alloc);

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
		deque_type ins_cont(now_alloc);
		ins_cont.insert(boost::bind(&iter_handler<deque_type>, _1, 10), 3, 10);
		yggr_test_assert(ins_cont.size() == 3);
	}

	{
		deque_type ins_cont(now_alloc);
		ins_cont.insert(boost::bind(&iter_handler<deque_type>, _1, 10), arr + 0, arr + 10);
		yggr_test_assert(ins_cont.size() == 10);
	}

	std::cout << "---------------test_element_access end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_safe_element_reference(SegCont& seg_cont)
{
	typedef Cont deque_type;
	typedef SegCont seg_cont_type;

	typedef typename deque_type::base_type base_deque_type;
	typedef typename deque_type::allocator_type alloc_type;
	typedef typename deque_type::iterator iter_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	int arr1[10] = {10, 11, 12};
	int arr2[10] = {20, 21, 22};

	deque_type cont1(arr1 + 0, arr1 + 10, now_alloc);
	deque_type cont2(arr2 + 0, arr2 + 10, now_alloc);

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

template<typename Cont, typename SegCont>
void test_modifiers(SegCont& seg_cont)
{
	typedef Cont deque_type;
	typedef SegCont seg_cont_type;

	typedef typename deque_type::base_type base_deque_type;
	typedef typename deque_type::allocator_type alloc_type;
	typedef typename deque_type::iterator iter_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::value_type val_type;

	typedef iter_type (base_deque_type::* begin_end_foo_type)(void);
	typedef citer_type (base_deque_type::* cbegin_end_foo_type)(void) const;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());
	int arr[10] = {1, 2, 3, 4, 5};
	deque_type cont(now_alloc);
	base_deque_type base_cont(now_alloc);

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

		deque_type cont2(now_alloc);
		yggr_test_assert(!cont2.pop_back(b));
		yggr_test_assert(b == 2000);
	}

	{
		int b = 0;
		yggr_test_assert(cont.pop_front(b));
		yggr_test_assert(b == 6000);
		base_cont.pop_front();
		yggr_test_assert(cont.size() == base_cont.size());

		deque_type cont2(now_alloc);
		yggr_test_assert(!cont2.pop_front(b));
		yggr_test_assert(b == 6000);
	}

	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont)));
	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont)));

	base_deque_type base_cont2(base_cont, now_alloc);

	yggr_test_assert(!cont.empty());
	yggr_test_assert(!base_cont2.empty());
	cont.swap(boost::move(base_cont2));
	yggr_test_assert(!cont.empty() && !base_cont2.empty());
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont)));

	deque_type cont3(base_cont, now_alloc);

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

template<typename Cont, typename SegCont>
void test_safe_other(SegCont& seg_cont)
{
	typedef Cont deque_type;
	typedef SegCont seg_cont_type;

	typedef typename deque_type::base_type base_deque_type;
	typedef typename deque_type::allocator_type alloc_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::value_type val_type;

	int arr[10] = {1, 2, 3, 4, 5};
	deque_type cont(arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(cont.is_exists(0));
	yggr_test_assert(!cont.is_exists(100));

	base_deque_type base_cont1 = cont.load();
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont1)));

	base_deque_type base_cont2 = cont;
	cont.using_handler(boost::bind(&check_range_equal<deque_type>, _1, boost::cref(base_cont2)));

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_swap(SegCont& seg_cont)
{
	typedef Cont deque_type;
	typedef SegCont seg_cont_type;

	typedef typename deque_type::base_type base_deque_type;
	typedef typename deque_type::allocator_type alloc_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	deque_type cont1(arr1 + 0, arr1 + 10, now_alloc);
	deque_type cont2(arr2 + 0, arr2 + 5, now_alloc);

	base_deque_type base_cont1(arr1 + 0, arr1 + 10, now_alloc);
	base_deque_type base_cont2(arr2 + 0, arr2 + 5, now_alloc);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() != base_cont2.size());


	base_deque_type base_cont3(base_cont1, now_alloc);
	base_deque_type base_cont4(base_cont2, now_alloc);

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
	typedef typename deque_type::base_type base_deque_type;

	typename base_deque_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	yggr_test_assert(i != r.end());
	*iter = *i;
}

template<typename Iter, typename Cont>
void cref_all_check_handler(Iter iter, const typename Cont::base_type& r)
{
	typedef Cont deque_type;
	typedef typename deque_type::base_type base_deque_type;

	typename base_deque_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	yggr_test_assert(i != r.end());
}

template<typename Cont>
void check_handler_eins(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont deque_type;

	//typename deque_type::size_type size = l.size();
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

	//typename deque_type::size_type size = l.size();
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

template<typename Cont, typename SegCont>
void test_using_handler(SegCont& seg_cont)
{
	typedef Cont deque_type;
	typedef SegCont seg_cont_type;

	typedef typename deque_type::base_type base_deque_type;
	typedef typename deque_type::allocator_type alloc_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::value_type val_type;

	alloc_type now_alloc(seg_cont.segment().get_segment_manager());

	int arr1[5] = {1, 2, 3, 4, 5};
	int arr2[5] = {6, 7, 8, 9, 10};

	base_deque_type base_cont1(arr1 + 0, arr1 + 5, now_alloc);
	base_deque_type base_cont2(arr2 + 0, arr2 + 5, now_alloc);

	deque_type cont(now_alloc);
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

template<typename Cont, typename SegCont>
void test_cpp20_erase(SegCont& seg_cont)
{
	typedef Cont deque_type;
	typedef SegCont seg_cont_type;

	typedef typename deque_type::base_type base_deque_type;
	typedef typename deque_type::allocator_type alloc_type;
	typedef typename deque_type::const_iterator citer_type;
	typedef typename deque_type::value_type val_type;

	int arr[5] = {1, 1, 2, 2, 3};

	{
		deque_type cont(arr + 0, arr + 5, alloc_type(seg_cont.segment().get_segment_manager()));

		std::size_t s = 0;

		s = yggr::safe_container::erase(cont, 1);
		yggr_test_assert(s == 2);

		s = yggr::safe_container::erase(cont, 6);
		yggr_test_assert(s == 0);
	}

	{
		deque_type cont(arr + 0, arr + 5, alloc_type(seg_cont.segment().get_segment_manager()));

		std::size_t s = 0;

		s = yggr::safe_container::erase_if(cont, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::safe_container::erase_if(cont, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	std::cout << "---------------test_cpp20_erase end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void debug_test(SegCont& seg_cont)
{
	typedef Cont deque_type;

	test_constructor<deque_type>(seg_cont);
	test_capacity<deque_type>(seg_cont);
	test_element_access<deque_type>(seg_cont);
	test_safe_element_reference<deque_type>(seg_cont);
	test_modifiers<deque_type>(seg_cont);
	test_safe_other<deque_type>(seg_cont);
	test_swap<deque_type>(seg_cont);
	test_using_handler<deque_type>(seg_cont);
	test_cpp20_erase<deque_type>(seg_cont);
}

int main(int argc, char* argv[])
{

	// managed_shared_memory
	{
		typedef boost::interprocess::managed_shared_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
		typedef yggr::safe_container::safe_options_deque<int, boost::interprocess::interprocess_mutex, alloc_type> deque_type;
		
		seg_cont_type seg_cont("test_managed_shared_memory_safe_options_deque",
								yggr::segment::segment_op::create_only(), 
								65536,
								boost::bind(
									&delete_segment<seg_type>, 
									"test_managed_shared_memory_safe_options_deque") );

		debug_test<deque_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	// test_mamaged_mapped_file
	{

		typedef boost::interprocess::managed_mapped_file seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
		typedef yggr::safe_container::safe_options_deque<int, boost::interprocess::interprocess_mutex, alloc_type> deque_type;
		
		seg_cont_type seg_cont("../test_out/segment/test_mamaged_mapped_file_safe_options_deque",
								yggr::segment::segment_op::create_only(), 
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"../test_out/segment/test_mamaged_mapped_file_safe_options_deque"));

		debug_test<deque_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	// test_managed_heap_memory
	{
		typedef boost::interprocess::managed_heap_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
		typedef yggr::safe_container::safe_options_deque<int, boost::interprocess::interprocess_mutex, alloc_type> deque_type;
		
		seg_cont_type seg_cont(65536);

		debug_test<deque_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(!seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	wait_any_key(argc, argv);
	return 0;
}
