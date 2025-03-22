// segment_safe_list_test.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/safe_container/safe_list.hpp>

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
void delete_segment(const std::string& name)
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

template<typename Cont, typename SegCont>
void test_constructor(SegCont& seg_cont)
{
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	typedef typename list_type::base_type base_list_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	{
		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);

		list_type& cont = *pcont;
		yggr_test_assert(cont.empty());

		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				10, 1, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);

		list_type& cont = *pcont;
		cont.for_each_iterator(boost::bind(&check_iter_values<citer_type>, _1, 1));
		cont.for_each_reverse(boost::bind(&check_values<val_type>, _1, 1));

		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				10, 1, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				10, 1, cont.get_allocator());
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		cont2.for_each_iterator(boost::bind(&check_iter_values<citer_type>, _1, 1));

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);

		list_type& cont = *pcont;

		yggr_test_assert(cont.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				arr + 0, arr + 10, cont.get_allocator());
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		yggr_test_assert(cont2.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		base_list_type* pcont =
			seg_cont.segment().template construct<base_list_type>("base_list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		base_list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				boost::move(cont));
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		//yggr_test_assert(cont.size() == 0); // ub
		yggr_test_assert(cont2.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		base_list_type* pcont =
			seg_cont.segment().template construct<base_list_type>("base_list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		base_list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				cont);
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(cont2.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		base_list_type* pcont =
			seg_cont.segment().template construct<base_list_type>("base_list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		base_list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				boost::move(cont), cont.get_allocator());
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		//yggr_test_assert(cont.size() == 0); // ub
		yggr_test_assert(cont2.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		base_list_type* pcont =
			seg_cont.segment().template construct<base_list_type>("base_list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		base_list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				cont, cont.get_allocator());
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(cont2.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				boost::move(cont));
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		//yggr_test_assert(cont.size() == 0); // ub
		yggr_test_assert(cont2.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				cont);
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(cont2.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				boost::move(cont), cont.get_allocator());
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		//yggr_test_assert(cont.size() == 0); // ub
		yggr_test_assert(cont2.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};

		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);
		list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				cont, cont.get_allocator());
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		yggr_test_assert(cont.size() == 10);
		yggr_test_assert(cont2.size() == 10);

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	// operator set

	{
		int arr[10] = {1, 2, 3, 4, 5};
		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);

		list_type& cont = *pcont;

		base_list_type* pcont2 =
			seg_cont.segment().template construct<base_list_type>("base_list2")(
				arr + 0, arr + 5, cont.get_allocator());
		yggr_test_assert(pcont2);

		base_list_type& cont2 = *pcont2;

		cont = boost::move(cont2);
		yggr_test_assert(cont.size() != cont2.size());
		yggr_test_assert(cont.size() == 5);
		//yggr_test_assert(cont2.size() == 10); // this is ub, only test it

		seg_cont.segment().destroy_ptr(pcont2);
		seg_cont.segment().destroy_ptr(pcont);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};
		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);

		list_type& cont = *pcont;

		base_list_type* pcont2 =
			seg_cont.segment().template construct<base_list_type>("base_list2")(
				arr + 0, arr + 5, cont.get_allocator());
		yggr_test_assert(pcont2);

		base_list_type& cont2 = *pcont2;

		cont = cont2;
		yggr_test_assert(cont.size() == cont2.size());
		cont2.push_back(10);
		cont = cont2;
		yggr_test_assert(cont.size() == cont2.size());

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	{
		int arr[10] = {1, 2, 3, 4, 5};
		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);

		list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				arr + 0, arr + 5, cont.get_allocator());
		yggr_test_assert(pcont2);

		yggr_test_assert(pcont != pcont2);

		list_type& cont2 = *pcont2;

		cont = boost::move(cont2);
		yggr_test_assert(cont.size() != cont2.size());
		yggr_test_assert(cont.size() == 5);
		//yggr_test_assert(cont2.size() == 10); // this is ub, only test it

		seg_cont.segment().destroy_ptr(pcont);
		seg_cont.segment().destroy_ptr(pcont2);
	}

	// cmper
	{
		int arr1[10] = {1, 2, 3, 4, 5};
		int arr2[10] = {1, 2, 3, 4, 6};
		list_type* pcont =
			seg_cont.segment().template construct<list_type>("list")(
				arr1 + 0, arr1 + 10, alloc_type(seg_cont.segment().get_segment_manager()));
		yggr_test_assert(pcont);

		list_type& cont = *pcont;

		list_type* pcont2 =
			seg_cont.segment().template construct<list_type>("list2")(
				arr1 + 0, arr1 + 10, cont.get_allocator());
		yggr_test_assert(pcont2);

		list_type& cont2 = *pcont2;

		yggr_test_assert(cont.load() == cont2);
		yggr_test_assert(cont == cont2.load());
		yggr_test_assert(cont == cont2);

		cont2.assign(arr2 + 0, arr2 + 10);

		yggr_test_assert(cont.load() != cont2);
		yggr_test_assert(cont != cont2.load());
		yggr_test_assert(cont != cont2);

		yggr_test_assert(cont.load() < cont2);
		yggr_test_assert(cont < cont2.load());
		yggr_test_assert(cont < cont2);

		yggr_test_assert(cont.load() <= cont2);
		yggr_test_assert(cont <= cont2.load());
		yggr_test_assert(cont <= cont2);

		cont.assign(arr2 + 0, arr2 + 10);
		cont2.assign(arr1 + 0, arr1 + 10);

		yggr_test_assert(cont.load() > cont2);
		yggr_test_assert(cont > cont2.load());
		yggr_test_assert(cont > cont2);

		yggr_test_assert(cont.load() >= cont2);
		yggr_test_assert(cont >= cont2.load());
		yggr_test_assert(cont >= cont2);

		seg_cont.segment().destroy_ptr(pcont2);
		seg_cont.segment().destroy_ptr(pcont);
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void check_range_equal(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont list_type;
	typedef typename Cont::base_type::const_iterator iter_type;
	//typename list_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		yggr_test_assert(*i == *j);
	}
}

template<typename Cont, typename SegCont>
void test_capacity(SegCont& seg_cont)
{
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	typedef typename list_type::base_type base_list_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	int arr[10] = {1, 2, 3, 4, 5};

	//list_type cont1(arr + 0, arr + 10);
	//typename list_type::base_type base_cont1(arr + 0, arr + 10);

	list_type* pcont =
		seg_cont.segment().template construct<list_type>("list")(
			arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
	yggr_test_assert(pcont);

	list_type& cont1 = *pcont;

	base_list_type base_cont1 = cont1.load();

	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.max_size() == base_cont1.max_size());

	cont1.resize(20);
	base_cont1.resize(20);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.max_size() == base_cont1.max_size());

	yggr_test_assert(cont1.empty() == base_cont1.empty());

	seg_cont.segment().destroy_ptr(pcont);

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

template<typename Cont, typename SegCont>
void test_element_access(SegCont& seg_cont)
{
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	typedef typename list_type::base_type base_list_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	int arr[10] = {1, 2, 3, 4, 5};

	list_type* pcont =
		seg_cont.segment().template construct<list_type>("list")(
			arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
	yggr_test_assert(pcont);

	//list_type cont(arr + 0, arr + 10);
	//base_list_type base_cont(arr + 0, arr + 10);

	list_type& cont = *pcont;
	base_list_type base_cont = cont.load();

	int f = 0, b = 0;
	list_type cont2 = cont;
	cont2.clear();
	yggr_test_assert(cont.size());
	yggr_test_assert(!cont2.size());
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
		list_type ins_cont = cont.load();
		ins_cont.clear();
		yggr_test_assert(ins_cont.size() == 0);
		ins_cont.emplace(boost::bind(&iter_handler<list_type>, _1, 0), yggr::non_handler );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 0);
	}

	{
		list_type ins_cont = cont.load();
		ins_cont.clear();
		yggr_test_assert(ins_cont.size() == 0);
		yggr_test_assert(ins_cont.emplace(
				boost::bind(&iter_handler<list_type>, _1, 0),
				boost::bind(&ins_handler<list_type>, _1, _2, 0) ) );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 0);
	}

	{
		list_type ins_cont = cont.load();
		ins_cont.clear();
		yggr_test_assert(ins_cont.emplace(
				boost::bind(&iter_handler<list_type>, _1, 10),
				boost::bind(&ins_handler<list_type>, _1, _2, 10), 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		list_type ins_cont = cont.load();
		ins_cont.clear();
		ins_cont.emplace(boost::bind(&iter_handler<list_type>, _1, 10), yggr::non_handler, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		list_type ins_cont = cont.load();
		ins_cont.clear();
		ins_cont.emplace(boost::bind(&iter_handler<list_type>, _1, 10), yggr::non_handler, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.front() == 10);
	}

	{
		list_type ins_cont = cont.load();
		ins_cont.clear();
		ins_cont.emplace_front();
		ins_cont.emplace_back();
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.front() == 0);
		yggr_test_assert(ins_cont.back() == 0);
	}

	{
		list_type ins_cont = cont.load();
		ins_cont.clear();
		ins_cont.emplace_front(10);
		ins_cont.emplace_back(20);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.front() == 10);
		yggr_test_assert(ins_cont.back() == 20);
	}

	// insert
	{
		int val = 20;
		list_type ins_cont = cont.load();
		ins_cont.clear();
		ins_cont.insert(boost::bind(&iter_handler<list_type>, _1, 10), 10);
		ins_cont.insert(boost::bind(&iter_handler<list_type>, _1, 20), boost::move(val));

		yggr_test_assert(ins_cont.size() == 2);
	}

	{
		int val = 20;
		list_type ins_cont = cont.load();
		ins_cont.clear();

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
		list_type ins_cont = cont.load();
		ins_cont.clear();
		ins_cont.insert(boost::bind(&iter_handler<list_type>, _1, 10), 3, 10);
		yggr_test_assert(ins_cont.size() == 3);
	}

	{
		list_type ins_cont = cont.load();
		ins_cont.clear();
		ins_cont.insert(boost::bind(&iter_handler<list_type>, _1, 10), arr + 0, arr + 10);
		yggr_test_assert(ins_cont.size() == 10);
	}

	seg_cont.segment().destroy_ptr(pcont);

	std::cout << "---------------test_element_access end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_safe_element_reference(SegCont& seg_cont)
{
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	typedef typename list_type::base_type base_list_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	int arr1[10] = {10, 11, 12};
	int arr2[10] = {20, 21, 22};

	list_type* pcont1 =
		seg_cont.segment().template construct<list_type>("list1")(
			arr1 + 0, arr1 + 10, alloc_type(seg_cont.segment().get_segment_manager()));
	yggr_test_assert(pcont1);

	list_type* pcont2 =
		seg_cont.segment().template construct<list_type>("list2")(
			arr2 + 0, arr2 + 10, alloc_type(seg_cont.segment().get_segment_manager()));
	yggr_test_assert(pcont2);

	//list_type cont1(arr1 + 0, arr1 + 10);
	//list_type cont2(arr2 + 0, arr2 + 10);

	list_type& cont1 = *pcont1;
	list_type& cont2 = *pcont2;

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

	seg_cont.segment().destroy_ptr(pcont1);
	seg_cont.segment().destroy_ptr(pcont2);

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

template<typename Cont, typename SegCont>
void test_modifiers(SegCont& seg_cont)
{
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	typedef typename list_type::base_type base_list_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	typedef iter_type (base_list_type::* begin_end_foo_type)(void);
	typedef citer_type (base_list_type::* cbegin_end_foo_type)(void) const;

	int arr[10] = {1, 2, 3, 4, 5};

	list_type* pcont =
		seg_cont.segment().template construct<list_type>("list")(
			alloc_type(seg_cont.segment().get_segment_manager()));
	yggr_test_assert(pcont);

	list_type& cont = *pcont;
	base_list_type base_cont = cont.load();

	yggr_test_assert(cont.empty());
	yggr_test_assert(base_cont.empty());

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

		list_type cont2(cont.get_allocator());
		yggr_test_assert(!cont2.pop_back(b));
		yggr_test_assert(b == 2000);
	}

	{
		int b = 0;
		yggr_test_assert(cont.pop_front(b));
		yggr_test_assert(b == 6000);
		base_cont.pop_front();
		yggr_test_assert(cont.size() == base_cont.size());

		list_type cont2(cont.get_allocator());
		yggr_test_assert(!cont2.pop_front(b));
		yggr_test_assert(b == 6000);
	}

	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));
	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	base_list_type base_cont2(base_cont);

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
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
					boost::bind(&handler_erase<list_type>, _1, _2));

		cont.erase(boost::bind(&make_erase_iter_pair<list_type>, _1),
					boost::bind(&handler_erase<list_type>, _1, _2));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
					boost::bind(static_cast<begin_end_foo_type>(&base_list_type::end), _1));

		cont.push_back(100);
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
					boost::bind(static_cast<begin_end_foo_type>(&base_list_type::end), _1),
					boost::bind(&handler_erase<list_type>, _1, _2));

		yggr_test_assert(cont.empty());
	}

	cont.clear();
	yggr_test_assert(cont.empty());

	seg_cont.segment().destroy_ptr(pcont);

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_operators(SegCont& seg_cont)
{
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	typedef typename list_type::base_type base_list_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	typedef iter_type (base_list_type::* begin_end_foo_type)(void);
	typedef citer_type (base_list_type::* cbegin_end_foo_type)(void) const;

	int arr[10] = {1, 2, 3, 4, 5};
	//list_type cont(arr + 0, arr + 10);
	//base_list_type base_cont(arr + 0, arr + 10);

	list_type* pcont =
		seg_cont.segment().template construct<list_type>("list")(
			arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
	yggr_test_assert(pcont);

	list_type& cont = *pcont;
	base_list_type base_cont = cont.load();

	{
		list_type cont1(arr + 0, arr + 10, cont.get_allocator());
		list_type cont2(arr + 0, arr + 5, cont.get_allocator());

		cont1.splice(cont2);
		cont1.splice(boost::bind(static_cast<begin_end_foo_type>(&base_list_type::end), _1),
						cont2 );

		cont1.splice(boost::bind(static_cast<begin_end_foo_type>(&base_list_type::end), _1),
						cont2,
						boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1) );

		cont1.splice(boost::bind(static_cast<begin_end_foo_type>(&base_list_type::end), _1),
						cont2,
						boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
						boost::bind(static_cast<begin_end_foo_type>(&base_list_type::end), _1) );

		cont1.splice(boost::bind(static_cast<begin_end_foo_type>(&base_list_type::end), _1),
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
		cont1.erase(boost::bind(static_cast<begin_end_foo_type>(&base_list_type::begin), _1),
						boost::bind(static_cast<begin_end_foo_type>(&base_list_type::end), _1));
		yggr_test_assert(cont1.size() == 0);
	}

	{
		list_type cont1(arr + 0, arr + 10, cont.get_allocator());
		typename list_type::base_type cont2(arr + 0, arr + 5, cont.get_allocator());

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

	base_list_type base_cont1(arr + 0, arr + 5, cont.get_allocator());
	base_list_type base_cont2(arr + 0, arr + 5, cont.get_allocator());
	base_list_type base_cont3(arr + 0, arr + 5, cont.get_allocator());
	base_list_type base_cont4(arr + 0, arr + 5, cont.get_allocator());

	cont.merge(boost::move(base_cont1));
	cont.merge(base_cont2);

	base_cont.merge(boost::move(base_cont3));
	base_cont.merge(base_cont4);

	yggr_test_assert(base_cont1.size() == base_cont3.size());
	yggr_test_assert(base_cont2.size() == base_cont4.size());

	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

	base_list_type base_cont5(arr + 0, arr + 5, cont.get_allocator());
	base_list_type base_cont6(arr + 0, arr + 5, cont.get_allocator());
	base_list_type base_cont7(arr + 0, arr + 5, cont.get_allocator());
	base_list_type base_cont8(arr + 0, arr + 5, cont.get_allocator());

	// boost list support unordered merge but don't use it,
	// if you want to use unordred_merge please use splite

	if(yggr::container::is_std_list<base_list_type>::value)
	{
		base_list_type base_tmp_cont = base_cont;

		cont.merge(boost::move(base_cont5), false, false);

		cont.merge(base_cont6, false, false);

		base_cont.merge(boost::move(base_cont7));
		base_cont.merge(base_cont8);

		yggr_test_assert(base_cont5.size() == base_cont7.size());
		yggr_test_assert(base_cont6.size() == base_cont8.size());


		yggr_test_assert(cont.size() == base_cont.size());
		cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont)));

		list_type tmp_cont1(arr + 0, arr + 5, cont.get_allocator());
		list_type tmp_cont2(arr + 0, arr + 5, cont.get_allocator());
		base_list_type tmp_base_cont1(arr + 0, arr + 5, cont.get_allocator());
		base_list_type tmp_base_cont2(arr + 0, arr + 5, cont.get_allocator());

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

		list_type tmp_cont1(arr + 0, arr + 5, cont.get_allocator());
		list_type tmp_cont2(arr + 0, arr + 5, cont.get_allocator());
		base_list_type tmp_base_cont1(arr + 0, arr + 5, cont.get_allocator());
		base_list_type tmp_base_cont2(arr + 0, arr + 5, cont.get_allocator());

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

	seg_cont.segment().destroy_ptr(pcont);

	std::cout << "---------------test_operators end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_safe_other(SegCont& seg_cont)
{
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	typedef typename list_type::base_type base_list_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	int arr[10] = {1, 2, 3, 4, 5};

	list_type* pcont =
		seg_cont.segment().template construct<list_type>("list")(
			arr + 0, arr + 10, alloc_type(seg_cont.segment().get_segment_manager()));
	yggr_test_assert(pcont);

	list_type& cont = *pcont;

	//list_type cont(arr + 0, arr + 10);
	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(cont.is_exists(0));
	yggr_test_assert(!cont.is_exists(100));

	base_list_type base_cont1 = cont.load();
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont1)));

	base_list_type base_cont2 = cont;
	cont.using_handler(boost::bind(&check_range_equal<list_type>, _1, boost::cref(base_cont2)));

	seg_cont.segment().destroy_ptr(pcont);

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_swap(SegCont& seg_cont)
{
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	typedef typename list_type::base_type base_list_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	list_type* pcont1 =
		seg_cont.segment().template construct<list_type>("list1")(
			arr1 + 0, arr1 + 10, alloc_type(seg_cont.segment().get_segment_manager()));
	yggr_test_assert(pcont1);

	list_type* pcont2 =
		seg_cont.segment().template construct<list_type>("list2")(
			arr2 + 0, arr2 + 5, alloc_type(seg_cont.segment().get_segment_manager()));
	yggr_test_assert(pcont2);

	//list_type cont1(arr1 + 0, arr1 + 10);
	//list_type cont2(arr2 + 0, arr2 + 5);

	list_type& cont1 = *pcont1;
	list_type& cont2 = *pcont2;

	base_list_type base_cont1(arr1 + 0, arr1 + 10, cont1.get_allocator());
	base_list_type base_cont2(arr2 + 0, arr2 + 5, cont2.get_allocator());
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() != base_cont2.size());


	base_list_type base_cont3(base_cont1);
	base_list_type base_cont4(base_cont2);

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

	seg_cont.segment().destroy_ptr(pcont1);
	seg_cont.segment().destroy_ptr(pcont2);

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
	//typename list_type::size_type size = l.size();
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
	//typename list_type::size_type size = l.size();
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

template<typename Cont, typename SegCont>
void test_using_handler(SegCont& seg_cont)
{
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	typedef typename list_type::base_type base_list_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	int arr1[5] = {1, 2, 3, 4, 5};
	int arr2[5] = {6, 7, 8, 9, 10};

	base_list_type base_cont1(arr1 + 0, arr1 + 5, alloc_type(seg_cont.segment().get_segment_manager()));
	base_list_type base_cont2(arr2 + 0, arr2 + 5, base_cont1.get_allocator());

	list_type cont(base_cont1.get_allocator());
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

template<typename Cont, typename SegCont>
void test_cpp20_erase(SegCont& seg_cont)
{
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	typedef typename list_type::base_type base_list_type;
	typedef typename list_type::allocator_type alloc_type;
	typedef typename list_type::iterator iter_type;
	typedef typename list_type::const_iterator citer_type;
	typedef typename list_type::value_type val_type;

	int arr[5] = {1, 1, 2, 2, 3};

	{
		list_type cont(arr + 0, arr + 5, alloc_type(seg_cont.segment().get_segment_manager()));

		std::size_t s = 0;

		s = yggr::safe_container::erase(cont, 1);
		yggr_test_assert(s == 2);

		s = yggr::safe_container::erase(cont, 6);
		yggr_test_assert(s == 0);
	}

	{
		list_type cont(arr + 0, arr + 5, alloc_type(seg_cont.segment().get_segment_manager()));

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
	typedef Cont list_type;
	typedef SegCont seg_cont_type;

	test_constructor<list_type>(seg_cont);
	test_capacity<list_type>(seg_cont);
	test_element_access<list_type>(seg_cont);
	test_safe_element_reference<list_type>(seg_cont);
	test_modifiers<list_type>(seg_cont);
	test_operators<list_type>(seg_cont);
	test_safe_other<list_type>(seg_cont);
	test_swap<list_type>(seg_cont);
	test_using_handler<list_type>(seg_cont);
	test_cpp20_erase<list_type>(seg_cont);
}

int main(int argc, char* argv[])
{
	// managed_shared_memory
	{
		typedef boost::interprocess::managed_shared_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
		typedef
			yggr::safe_container::safe_list
			<
				int,
				boost::interprocess::interprocess_mutex,
				alloc_type
			> list_type;

		seg_cont_type seg_cont("test_managed_shared_memory_safe_list",
								yggr::segment::segment_op::create_only(),
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"test_managed_shared_memory_safe_list") );

		debug_test<list_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	{
		typedef boost::interprocess::managed_shared_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
		typedef
			yggr::safe_container::safe_list
			<
				int,
				boost::interprocess::interprocess_mutex,
				alloc_type,
				boost::container::list
			> list_type;

		seg_cont_type seg_cont("test_managed_shared_memory_safe_list",
								yggr::segment::segment_op::create_only(),
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"test_managed_shared_memory_safe_list") );

		debug_test<list_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	wait_any_key(argc, argv);
	return 0;
}
