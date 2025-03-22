//safe_unordered_set_test.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <functional>

#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/mplex/null_t.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/safe_container/safe_unordered_set.hpp>

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

template<typename Cont>
void check_range_equal(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont set_type;
	typedef typename set_type::const_iterator iter_type;
	//typename set_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
	{
		r.find(*i) != r.end();
	}
}

template<typename Cont, typename SegCont>
void test_constructor(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	{
		set_type cont(alloc);
		yggr_test_assert(cont.empty());
	}

	{
		set_type cont(key_equal_type(), alloc);
		yggr_test_assert(cont.empty());
	}

	{
		set_type cont(hasher_type(), key_equal_type(), alloc);
		yggr_test_assert(cont.empty());
	}

	{
		set_type cont(5, hasher_type(), key_equal_type(), alloc);
		yggr_test_assert(cont.empty());
	}


	int arr[10] = {1, 2, 3, 4, 5};

	{
		set_type cont(arr + 0, arr + 10, alloc);
		yggr_test_assert(cont.size() == 6);
		yggr_test_assert(cont.count(1) == 1);
	}

	{
		set_type cont(arr + 0, arr + 10, key_equal_type(), alloc);
		yggr_test_assert(cont.size() == 6);
	}

	{
		set_type cont(arr + 0, arr + 10, hasher_type(), key_equal_type(), alloc);
		yggr_test_assert(cont.size() == 6);
	}

	{
		set_type cont(arr + 0, arr + 10, 5, hasher_type(), key_equal_type(), alloc);
		yggr_test_assert(cont.size() == 6);
	}

	{
		base_set_type base_cont(arr + 0, arr + 10, 0, hasher_type(), key_equal_type(), alloc);
		set_type cont(boost::move(base_cont));
		yggr_test_assert(cont.size() == 6);
	}

	{
		base_set_type base_cont(arr + 0, arr + 10, 0, hasher_type(), key_equal_type(), alloc);
		set_type cont(base_cont);
		yggr_test_assert(cont.size() == 6 && base_cont.size() == 6);
		cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));
	}

	{
		base_set_type base_cont(arr + 0, arr + 10, 0, hasher_type(), key_equal_type(), alloc);
		set_type cont(boost::move(base_cont), alloc);
		yggr_test_assert(cont.size() == 6);
	}

	{
		base_set_type base_cont(arr + 0, arr + 10, 0, hasher_type(), key_equal_type(), alloc);
		set_type cont(base_cont, base_cont.get_allocator());
		yggr_test_assert(cont.size() == 6 && base_cont.size() == 6);
		cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));
	}

	{
		set_type cont11(arr + 0, arr + 10, 0, hasher_type(), key_equal_type(), alloc);
		base_set_type base_cont11 = cont11;

		// safe_copy_base
		yggr_test_assert(cont11 == base_cont11);
		yggr_test_assert(!(cont11 != base_cont11));
		
		// base_copy_safe
		yggr_test_assert(base_cont11 == cont11);
		yggr_test_assert(!(base_cont11 != cont11));

		// selfsafe_copy_selfsafe
		yggr_test_assert(cont11 == cont11);
		yggr_test_assert(!(cont11 != cont11));

		set_type cont12(cont11);

		// safe_copy_safe
		yggr_test_assert(cont11 == cont12);
		yggr_test_assert(!(cont11 != cont12));
	}

	{
		base_set_type base_cont13(arr + 0, arr + 10, 0, hasher_type(), key_equal_type(), alloc);
		set_type cont13(base_cont13, base_cont13.get_allocator());
		yggr_test_assert(cont13.size() == 6 && base_cont13.size() == 6);
		cont13.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont13)));

		set_type cont14(cont13);
		yggr_test_assert(cont14.size() == 6);
		cont14.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont13)));

		set_type cont15(boost::move(cont14));
		yggr_test_assert(cont15.size() == 6);
		cont15.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont13)));
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_capacity(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont1(arr + 0, arr + 10, alloc);
	base_set_type base_cont1(arr + 0, arr + 10, 0, hasher_type(), key_equal_type(), alloc);

	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.max_size() == base_cont1.max_size());

	yggr_test_assert(cont1.empty() == base_cont1.empty());

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

template<typename Cont>
bool check_last_element(typename Cont::base_type& base, 
							const std::pair<typename Cont::iterator, bool>& iter_pair, 
							int check)
{
	yggr_test_assert(base.end() != iter_pair.first);
	yggr_test_assert(*(iter_pair.first) == check);
	return (base.end() != iter_pair.first)
			&& (*(iter_pair.first) == check);
}

template<typename Cont>
bool find_handler(typename Cont::base_type& base, const typename Cont::iterator& iter)
{
	return (base.end() != iter);
}

template<typename Cont>
bool cfind_handler(const typename Cont::base_type& base, const typename Cont::const_iterator& iter)
{
	return (base.end() != iter);
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
bool ins_handler(typename Cont::base_type& base,
					const std::pair<typename Cont::iterator, bool>& pair,
					const typename Cont::value_type& chk_val)
{
	yggr_test_assert(pair.second);
	yggr_test_assert(chk_val == *pair.first);
	return pair.second && (pair.first != base.end());
}

template<typename Cont, typename SegCont>
void test_modifiers(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	typedef iter_type (base_set_type::* begin_end_foo_type)(void);
	typedef citer_type (base_set_type::* cbegin_end_foo_type)(void) const;

	set_type cont(alloc);
	base_set_type base_cont(0, hasher_type(), key_equal_type(), alloc);

	cont.insert(100);
	base_cont.insert(100);

	int n = 90;
	cont.insert(boost::move(n));
	base_cont.insert(90);

	cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));

	cont.insert(200, boost::bind(&check_last_element<set_type>, _1, _2, 200));
	base_cont.insert(200);

	int m = 300;
	cont.insert(boost::move(m), boost::bind(&check_last_element<set_type>, _1, _2, 300));
	base_cont.insert(300);

	yggr_test_assert(cont.size() == base_cont.size());

	cont.erase(200);
	base_cont.erase(200);

	yggr_test_assert(cont.size() == base_cont.size());

	yggr_test_assert(cont.replace(300, 3000));
	yggr_test_assert(cont.replace(3000, 300));
	yggr_test_assert(!cont.replace(9000, 8000));

	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));
	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));

	base_set_type base_cont2(base_cont);

	cont.swap(boost::move(base_cont2));
	yggr_test_assert(!cont.empty() && !base_cont2.empty());
	cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));

	set_type cont3(base_cont);

	cont.swap(cont3);
	cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));
	cont3.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));

	//emplace
	{
		set_type ins_cont(alloc);
		ins_cont.emplace(yggr::non_handler );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
		yggr_test_assert(ins_cont.find(0, ins_cont.hash_function(), ins_cont.key_eq()));
	}

	{
		set_type ins_cont(alloc);
		ins_cont.emplace(yggr::non_handler, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
	}

	{
		set_type ins_cont(alloc);
		yggr_test_assert(ins_cont.emplace(boost::bind(&ins_handler<set_type>, _1, _2, 0)));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
	}

	{
		set_type ins_cont(alloc);
		yggr_test_assert(ins_cont.emplace(boost::bind(&ins_handler<set_type>, _1, _2, 10), 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));

	}

	{
		set_type ins_cont(alloc);
		yggr_test_assert(ins_cont.emplace_hint(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1), yggr::non_handler));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
	}

	{
		set_type ins_cont(alloc);
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1), yggr::non_handler, 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
	}

	{
		set_type ins_cont(alloc);
		yggr_test_assert(ins_cont.empty());
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1),
						boost::bind(&ins_handler<set_type>, _1, _2, 0)));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
	}

	{
		set_type ins_cont(alloc);
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1),
						boost::bind(&ins_handler<set_type>, _1, _2, 10), 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
	}

	// insert
	{
		yggr_test_assert(cont.emplace(yggr::non_handler, 555));
		yggr_test_assert(cont.emplace_hint(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1), yggr::non_handler, 666));
		base_cont.insert(555);
		base_cont.insert(666);

		set_type& cont_ref = cont;
		const set_type& cont_cref = cont;

		yggr_test_assert(cont_ref.find(555, boost::bind(&find_handler<set_type>, _1, _2)));
		yggr_test_assert(cont_cref.find(555, boost::bind(&cfind_handler<set_type>, _1, _2)));

		yggr_test_assert(cont.find(555));
		yggr_test_assert(base_cont.find(555) != base_cont.end());

		yggr_test_assert(cont_ref.find(666, boost::bind(&find_handler<set_type>, _1, _2)));
		yggr_test_assert(cont_cref.find(666, boost::bind(&cfind_handler<set_type>, _1, _2)));

		yggr_test_assert(cont.find(666));
		yggr_test_assert(base_cont.find(666) != base_cont.end());
	}

	// erase
	{
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1),
					boost::bind(&handler_erase<set_type>, _1, _2));

		cont.erase(boost::bind(&make_erase_iter_pair<set_type>, _1),
					boost::bind(&handler_erase<set_type>, _1, _2));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1),
					boost::bind(static_cast<begin_end_foo_type>(&base_set_type::end), _1));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1),
					boost::bind(static_cast<begin_end_foo_type>(&base_set_type::end), _1),
					boost::bind(&handler_erase<set_type>, _1, _2));

		yggr_test_assert(cont.empty());
	}

	cont.clear();
	yggr_test_assert(cont.empty());


	// merge
	{
		int arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
		int arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};
		// unordered_set

		{
			typedef boost::unordered_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef boost::unordered_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::unordered_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::unordered_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			set_type cont_zwei(arr2 + 0, arr2 + 10, alloc);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			set_type cont_zwei(arr2 + 0, arr2 + 10, alloc);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		// unordered_multiset

		{
			typedef boost::unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef boost::unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef 
				yggr::safe_container::safe_unordered_multiset
				<
					int,
					boost::mutex,
					hasher_type,
					key_equal_type,
					alloc_type,
					yggr::unordered_multiset
				> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10, alloc);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef 
				yggr::safe_container::safe_unordered_multiset
				<
					int,
					boost::mutex,
					hasher_type,
					key_equal_type,
					alloc_type,
					yggr::unordered_multiset
				> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10, alloc);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10, alloc);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}
	}

	// erase_if
	{
		int arr[10] = {1, 2, 3, 4, 5};

		set_type cont(arr + 0, arr + 5, alloc);

		yggr_test_assert(cont.size() == 5);

		std::less<int> less;
		std::size_t rm_size = yggr::safe_container::erase_if(cont, boost::bind(less, _1, 3));
		yggr_test_assert(rm_size == 2);
		yggr_test_assert(!cont.contains(1));
		yggr_test_assert(!cont.contains(2));
		yggr_test_assert(cont.contains(3));
		yggr_test_assert(cont.contains(4));
		yggr_test_assert(cont.contains(5));
		yggr_test_assert(cont.size() == 3);
	}

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

template<typename Cont>
typename Cont::insert_return_type
	ins_node_handler(typename Cont::base_type& base,
						typename Cont::base_insert_return_type& ins_ret)
{
	typedef Cont set_type;
	return typename set_type::insert_return_type(boost::move(ins_ret));
}

template<typename Cont>
bool ins_hint_node_handler(typename Cont::base_type& base,
							const std::pair<typename Cont::iterator, bool>& pair)
{
	return pair.second && pair.first != base.end();
}

template<typename Cont, typename SegCont>
void test_extract_insert(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	typedef iter_type (base_set_type::* begin_end_foo_type)(void);
	typedef citer_type (base_set_type::* cbegin_end_foo_type)(void) const;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// extract insert
	{
		int arr[10] = {1, 2, 3, 4, 5};
		{
			set_type ins_cont(alloc);
			ins_cont.insert(arr + 0, arr + 5);
			typename set_type::node_type node = ins_cont.extract(666);
			yggr_test_assert(node.empty());

			typename set_type::insert_return_type iret = ins_cont.insert(boost::move(node));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret.node.empty());
			yggr_test_assert(!iret.inserted);
		}

		{
			set_type ins_cont(alloc);
			ins_cont.insert(arr + 0, arr + 5);
			typename set_type::node_type node = ins_cont.extract(3);
			yggr_test_assert(node.value() == 3);
			yggr_test_assert(!ins_cont.find(3));

			typename set_type::insert_return_type iret = ins_cont.insert(boost::move(node));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret.node.empty());
			yggr_test_assert(iret.inserted);

			yggr_test_assert(ins_cont.find(3));
		}

		{
			set_type ins_cont(alloc);
			typename set_type::value_type ex_val = 0;
			ins_cont.insert(arr + 0, arr + 5);
			typename set_type::node_type node = 
				ins_cont.extract(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1));
			yggr_test_assert(!node.empty());
			ex_val = node.value();
			yggr_test_assert(!ins_cont.find(ex_val));

			typename set_type::insert_return_type iret = 
				ins_cont.insert(
					boost::move(node),
					boost::bind(&ins_node_handler<set_type>, _1, _2));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret.node.empty());
			yggr_test_assert(iret.inserted);

			yggr_test_assert(ins_cont.find(ex_val));
		}

		{
			set_type ins_cont(alloc);
			ins_cont.insert(arr + 0, arr + 5);
			typename set_type::node_type node = ins_cont.extract(3);
			yggr_test_assert(node.value() == 3);
			yggr_test_assert(!ins_cont.find(3));

			bool iret = 
				ins_cont.insert(
					boost::bind(static_cast<begin_end_foo_type>(&base_set_type::end), _1),
					boost::move(node));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret);

			yggr_test_assert(ins_cont.find(3));
		}

		{
			set_type ins_cont(alloc);
			ins_cont.insert(arr + 0, arr + 5);
			typename set_type::node_type node = ins_cont.extract(3);
			yggr_test_assert(node.value() == 3);
			yggr_test_assert(!ins_cont.find(3));

			bool iret = 
				ins_cont.insert(
					boost::bind(static_cast<begin_end_foo_type>(&base_set_type::end), _1),
					boost::move(node),
					boost::bind(&ins_hint_node_handler<set_type>, _1, _2));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret);

			yggr_test_assert(ins_cont.find(3));
		}
	}
}

template<typename Cont, typename Iter>
void check_equal_range(const typename Cont::base_type&, 
						const std::pair<Iter, Iter>& iter_pair, 
						typename Cont::value_type now)
{
	Iter s = iter_pair.first;
	++s;
	yggr_test_assert(s == iter_pair.second);
	yggr_test_assert(*iter_pair.first == now);
}

template<typename Cont, typename SegCont>
void test_element_lookup(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr + 0, arr + 10, alloc);
	yggr_test_assert(cont.count(0) == 1);

	set_type& cont_ref = cont;
	const set_type& cont_cref = cont;

	cont_ref.equal_range(3, boost::bind(&check_equal_range<set_type, typename set_type::iterator>,
											_1, _2, 3));

	cont_cref.equal_range(3, boost::bind(&check_equal_range<set_type, typename set_type::const_iterator>,
											_1, _2, 3));

	std::cout << "---------------test_element_lookup end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_buckets(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr + 0, arr + 10, alloc);
	base_set_type base_cont(arr + 0, arr + 10, 0, hasher_type(), key_equal_type(), alloc);

	yggr_test_assert(cont.bucket_count() == base_cont.bucket_count());
	yggr_test_assert(cont.max_bucket_count() == base_cont.max_bucket_count());
	yggr_test_assert(cont.bucket_size(0) == base_cont.bucket_size(0));
	yggr_test_assert(cont.bucket_size(1) == base_cont.bucket_size(1));
	yggr_test_assert(cont.bucket_size(2) == base_cont.bucket_size(2));
	yggr_test_assert(cont.bucket(0) == base_cont.bucket(0));
	yggr_test_assert(cont.bucket(1) == base_cont.bucket(1));
	yggr_test_assert(cont.bucket(2) == base_cont.bucket(2));
	yggr_test_assert(cont.bucket(3) == base_cont.bucket(3));
	yggr_test_assert(cont.bucket(4) == base_cont.bucket(4));
	yggr_test_assert(cont.bucket(5) == base_cont.bucket(5));

	std::cout << "---------------test_buckets end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_hash_policy(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr + 0, arr + 10, alloc);
	base_set_type base_cont(arr + 0, arr + 10, 0, hasher_type(), key_equal_type(), alloc);

	yggr_test_assert(cont.load_factor() == base_cont.load_factor());
	yggr_test_assert(cont.max_load_factor() == base_cont.max_load_factor());
	cont.max_load_factor(cont.max_load_factor());
	base_cont.max_load_factor(base_cont.max_load_factor());

	yggr_test_assert(cont.load_factor() == base_cont.load_factor());
	yggr_test_assert(cont.max_load_factor() == base_cont.max_load_factor());

	cont.rehash(100);
	base_cont.rehash(100);
	yggr_test_assert(cont.load_factor() == base_cont.load_factor());
	yggr_test_assert(cont.max_load_factor() == base_cont.max_load_factor());

//#if !USE_STL_USET
	cont.reserve(1000);
	base_cont.reserve(1000);

	yggr_test_assert(cont.load_factor() == base_cont.load_factor());
	yggr_test_assert(cont.max_load_factor() == base_cont.max_load_factor());
//#endif // USE_STL_USET

	std::cout << "---------------test_hash_policy end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_safe_other(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr + 0, arr + 10, alloc);
	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(cont.is_exists(1, cont.hash_function(), cont.key_eq()));
	yggr_test_assert(cont.is_exists(0));
	yggr_test_assert(cont.is_exists(0, cont.hash_function(), cont.key_eq()));
	yggr_test_assert(!cont.is_exists(100));
	yggr_test_assert(!cont.is_exists(100, cont.hash_function(), cont.key_eq()));

	set_type empty_set(alloc);

	int beg_val = 0;

	yggr_test_assert(!empty_set.get_begin_value(beg_val));

	try
	{
		empty_set.begin_value();
		yggr_test_assert(false);
	}
	catch(const typename set_type::error_type&)
	{
	}

	cont.get_begin_value(beg_val);
	yggr_test_assert(beg_val == cont.begin_value());

	yggr_test_assert(cont.is_exists(beg_val));

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_swap(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	set_type cont1(arr1 + 0, arr1 + 10, alloc);
	set_type cont2(arr2 + 0, arr2 + 5, alloc);

	base_set_type base_cont1(arr1 + 0, arr1 + 10, 0, hasher_type(), key_equal_type(), alloc);
	base_set_type base_cont2(arr2 + 0, arr2 + 5, 0, hasher_type(), key_equal_type(), alloc);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() != base_cont2.size());

	base_set_type base_cont3(base_cont1);
	base_set_type base_cont4(base_cont2);

	// ------------------ boost::swap test------------------------
	// base_type&& this_type&
	boost::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	boost::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	boost::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	boost::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	boost::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));

	boost::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	boost::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	boost::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));

	// ------------------ std::swap test------------------------
	// base_type&& this_type&
	std::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	std::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	std::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	std::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	std::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));

	std::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	std::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	std::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));

	// ------------------ yggr::swap test------------------------
	// base_type&& this_type&
	yggr::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	yggr::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	yggr::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	yggr::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	yggr::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));

	yggr::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	yggr::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	yggr::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

template<typename Cont, typename Iter>
bool check_element(const typename Cont::base_type& base, 
						Iter i, int chk_val)
{
	return (i != base.end()) && (*i == chk_val);
}

template<typename Cont, typename SegCont>
void test_find(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	int arr[5] = {1, 2, 3, 4, 5};

	set_type cont(arr + 0, arr + 5, alloc);
	set_type& ref_cont = cont;
	const set_type& cref_cont = cont;

	yggr_test_assert(ref_cont.find(1, boost::bind(&check_element<set_type, typename set_type::iterator>, _1, _2, 1)));
	yggr_test_assert(cref_cont.find(2, boost::bind(&check_element<set_type, typename set_type::const_iterator>, _1, _2, 2)));

	yggr_test_assert(!ref_cont.find(100, boost::bind(&check_element<set_type, typename set_type::iterator>, _1, _2, 100)));
	yggr_test_assert(!cref_cont.find(100, boost::bind(&check_element<set_type, typename set_type::const_iterator>, _1, _2, 100)));

	yggr_test_assert(ref_cont.find(1, cont.hash_function(), cont.key_eq(), 
									boost::bind(&check_element<set_type, typename set_type::iterator>, _1, _2, 1)));
	yggr_test_assert(cref_cont.find(2, cont.hash_function(), cont.key_eq(), 
									boost::bind(&check_element<set_type, typename set_type::const_iterator>, _1, _2, 2)));

	yggr_test_assert(!ref_cont.find(100, cont.hash_function(), cont.key_eq(), 
									boost::bind(&check_element<set_type, typename set_type::iterator>, _1, _2, 100)));
	yggr_test_assert(!cref_cont.find(100, cont.hash_function(), cont.key_eq(), 
									boost::bind(&check_element<set_type, typename set_type::const_iterator>, _1, _2, 100)));

	std::cout << "---------------test_find end-----------------" << std::endl;

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
	typedef Cont set_type;
	typename set_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	yggr_test_assert(i != r.end());
}

template<typename Iter, typename Cont>
void cref_all_check_handler(Iter iter, const typename Cont::base_type& r)
{
	typedef Cont set_type;
	typename set_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	yggr_test_assert(i != r.end());
}

template<typename Cont>
void check_handler_eins(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont set_type;
	typedef typename set_type::const_iterator iter_type;
	//typename set_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
	{
		yggr_test_assert(r.find(*i) != r.end());
	}
}

template<typename Cont, typename Handler>
void check_handler_zwei(const typename Cont::base_type& l, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont set_type;
	typedef typename set_type::const_iterator iter_type;
	//typename set_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
	{
		yggr_test_assert(r.find(*i) != r.end());
	}
	ret_handler();
}

template<typename Cont>
void set_handler_eins(typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont set_type;
	typedef typename set_type::const_iterator iter_type;

	if(!l.empty())
	{
		yggr_test_assert(l.size() == r.size());
		for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
		{
			yggr_test_assert(r.find(*i) == r.end());
		}
	}
	l = r;
	{
		for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
		{
			yggr_test_assert(r.find(*i) != r.end());
		}
	}
}

template<typename Cont, typename Handler>
void set_handler_zwei(typename Cont::base_type& l, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont set_type;
	typedef typename set_type::const_iterator iter_type;
	if(!l.empty())
	{
		yggr_test_assert(l.size() == r.size());
		for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
		{
			yggr_test_assert(r.find(*i) == r.end());
		}
	}
	l = r;
	{
		for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
		{
			yggr_test_assert(r.find(*i) != r.end());
		}
	}

	ret_handler();
}

template<typename Cont, typename Handler>
void return_handler_call_helper(Cont& cont, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont set_type;
	typedef Handler ret_handler_type;
	cont.using_handler(boost::bind(&set_handler_zwei<set_type, ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Cont, typename Handler>
void return_handler_call_helper(const Cont& cont, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont set_type;
	typedef Handler ret_handler_type;
	cont.using_handler(boost::bind(&check_handler_zwei<set_type, ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Cont, typename SegCont>
void test_using_handler(SegCont& seg_cont)
{
	typedef Cont set_type;
	typedef SegCont seg_cont_type;

	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::hasher hasher_type;
	typedef typename set_type::key_equal key_equal_type;
	typedef typename set_type::allocator_type alloc_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;
	typedef typename set_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	int arr1[5] = {1, 2, 3, 4, 5};
	int arr2[5] = {6, 7, 8, 9, 10};

	base_set_type base_cont1(arr1 + 0, arr1 + 5, 0, hasher_type(), key_equal_type(), alloc);
	base_set_type base_cont2(arr2 + 0, arr2 + 5, 0, hasher_type(), key_equal_type(), alloc);

	set_type cont(alloc);
	set_type& ref_cont = cont;
	const set_type& cref_cont = cont;

	ref_cont.using_handler(boost::bind(&set_handler_eins<set_type>, _1, boost::cref(base_cont1)));
	cref_cont.using_handler(boost::bind(&check_handler_eins<set_type>, _1, boost::cref(base_cont1)));

	int ref_count = 0;
	return_handler_call_helper(ref_cont, base_cont2, boost::bind(&return_handler, boost::ref(ref_count)));
	yggr_test_assert(ref_count == 1);

	int cref_count = 0;
	return_handler_call_helper(cref_cont, base_cont2, boost::bind(&return_handler, boost::ref(cref_count)));
	yggr_test_assert(cref_count == 1);

	ref_cont.for_each_iterator(boost::bind(&ref_all_check_handler<typename set_type::iterator, set_type>,
											_1, boost::cref(base_cont2)));
	cref_cont.for_each_iterator(boost::bind(&cref_all_check_handler<typename set_type::const_iterator, set_type>,
												_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.for_each_iterator(boost::bind(&ref_all_check_handler<typename set_type::iterator, set_type>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<typename set_type::iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(ref_count)));
	yggr_test_assert(ref_count == 2);

	cref_count = 0;
	cref_cont.for_each_iterator(boost::bind(&cref_all_check_handler<typename set_type::const_iterator, set_type>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<typename set_type::const_iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(cref_count)));
	yggr_test_assert(cref_count == 2);


	std::cout << "---------------test_use_handler end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void debug_test(SegCont& seg_cont)
{
	typedef Cont set_type;

	test_constructor<set_type>(seg_cont);
	test_capacity<set_type>(seg_cont);
	test_modifiers<set_type>(seg_cont);
	test_extract_insert<set_type>(seg_cont);
	test_element_lookup<set_type>(seg_cont);
	test_buckets<set_type>(seg_cont);
	test_hash_policy<set_type>(seg_cont);
	test_safe_other<set_type>(seg_cont);
	test_swap<set_type>(seg_cont);
	test_find<set_type>(seg_cont);
	test_using_handler<set_type>(seg_cont);
}

int main(int argc, char* argv[])
{
	// managed_shared_memory
	{
		typedef boost::interprocess::managed_shared_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
		typedef 
			yggr::safe_container::safe_unordered_set
			<
				int,
				boost::interprocess::interprocess_mutex, 
				boost::hash<int>,
				std::equal_to<int>,
				alloc_type
			> set_type;
		
		seg_cont_type seg_cont("test_managed_shared_memory_safe_unordered_set",
								yggr::segment::segment_op::create_only(), 
								65536,
								boost::bind(
									&delete_segment<seg_type>, 
									"test_managed_shared_memory_safe_unordered_set") );

		debug_test<set_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	// test_mamaged_mapped_file
	{

		typedef boost::interprocess::managed_mapped_file seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
		
		typedef 
			yggr::safe_container::safe_unordered_set
			<
				int,
				boost::interprocess::interprocess_mutex, 
				boost::hash<int>,
				std::equal_to<int>,
				alloc_type
			> set_type;
		
		seg_cont_type seg_cont("../test_out/segment/test_mamaged_mapped_file_safe_unordered_set",
								yggr::segment::segment_op::create_only(), 
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"../test_out/segment/test_mamaged_mapped_file_safe_unordered_set"));

		debug_test<set_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	// test_managed_heap_memory
	{
		typedef boost::interprocess::managed_heap_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<int, seg_type::segment_manager> alloc_type;
		
		typedef 
			yggr::safe_container::safe_unordered_set
			<
				int,
				boost::interprocess::interprocess_mutex, 
				boost::hash<int>,
				std::equal_to<int>,
				alloc_type
			> set_type;
		
		seg_cont_type seg_cont(65536);

		debug_test<set_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(!seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	wait_any_key(argc, argv);
	return 0;
}
