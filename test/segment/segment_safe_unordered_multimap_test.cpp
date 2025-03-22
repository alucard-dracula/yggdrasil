//safe_unordered_multimap_test.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/utility/pair_compare.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/mplex/null_t.hpp>

#include <yggr/segment/segment_info.hpp>
#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op_proxy.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_mode.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>

#include <boost/bind.hpp>

#include <functional>
#include <cassert>

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

//#if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)
//
//template<typename K1, typename V1, typename K2, typename V2> inline
//bool operator==(const std::pair<K1, V1>& l, const std::pair<K2, V2>& r)
//{
//	return l.first == r.first && l.second == r.second;
//}
//
//#endif // #if (YGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename Cont>
void check_range_equal(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
	//typename map_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
	{
		r.find(i->first) != r.end();
	}
}

template<typename Cont, typename SegCont>
void test_constructor(SegCont& seg_cont)
{
	typedef Cont map_type;
	typedef SegCont seg_cont_type;

	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::hasher hasher_type;
	typedef typename map_type::key_equal key_equal_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;
	typedef typename map_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	{
		map_type cont(alloc);
		yggr_test_assert(cont.empty());
	}

	{
		map_type cont(key_equal_type(), alloc);
		yggr_test_assert(cont.empty());
	}

	{
		map_type cont(hasher_type(), key_equal_type(), alloc);
		yggr_test_assert(cont.empty());
	}

	{
		map_type cont(5, hasher_type(), key_equal_type(), alloc);
		yggr_test_assert(cont.empty());
	}

	base_map_type init_cont(0, hasher_type(), key_equal_type(), alloc);
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));
	init_cont.insert(typename map_type::value_type(0, 0));

	{
		map_type cont(init_cont.begin(), init_cont.end(), alloc);
		yggr_test_assert(cont.size() == 6);
	}

	{
		map_type cont(init_cont.begin(), init_cont.end(), key_equal_type(), alloc);
		yggr_test_assert(cont.size() == 6);
	}

	{
		map_type cont(init_cont.begin(), init_cont.end(), hasher_type(), key_equal_type(), alloc);
		yggr_test_assert(cont.size() == 6);
	}

	{
		map_type cont(init_cont.begin(), init_cont.end(), 5, hasher_type(), key_equal_type(), alloc);
		yggr_test_assert(cont.size() == 6);
	}

	{
		base_map_type base_cont(init_cont.begin(), init_cont.end(), 0, hasher_type(), key_equal_type(), alloc);
		map_type cont(boost::move(base_cont));
		yggr_test_assert(cont.size() == 6);
	}

	{
		base_map_type base_cont(init_cont.begin(), init_cont.end(), 0, hasher_type(), key_equal_type(), alloc);
		map_type cont(base_cont);
		yggr_test_assert(cont.size() == 6 && base_cont.size() == 6);
		cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));
	}

	{
		base_map_type base_cont(init_cont.begin(), init_cont.end(), 0, hasher_type(), key_equal_type(), alloc);
		map_type cont(boost::move(base_cont), alloc);
		yggr_test_assert(cont.size() == 6);
	}

	{
		base_map_type base_cont(init_cont.begin(), init_cont.end(), 0, hasher_type(), key_equal_type(), alloc);
		map_type cont(base_cont, base_cont.get_allocator());
		yggr_test_assert(cont.size() == 6 && base_cont.size() == 6);
		cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));
	}

	{
		map_type cont11(init_cont.begin(), init_cont.end(), alloc);
		base_map_type base_cont11 = cont11;

		// safe_copy_base
		yggr_test_assert(cont11 == base_cont11);
		yggr_test_assert(!(cont11 != base_cont11));
		
		// base_copy_safe
		yggr_test_assert(base_cont11 == cont11);
		yggr_test_assert(!(base_cont11 != cont11));

		// selfsafe_copy_selfsafe
		yggr_test_assert(cont11 == cont11);
		yggr_test_assert(!(cont11 != cont11));

		map_type cont12(cont11);

		// safe_copy_safe
		yggr_test_assert(cont11 == cont12);
		yggr_test_assert(!(cont11 != cont12));
	}

	{
		base_map_type base_cont13(init_cont.begin(), init_cont.end(), 0, hasher_type(), key_equal_type(), alloc);
		map_type cont13(base_cont13, base_cont13.get_allocator());
		yggr_test_assert(cont13.size() == 6 && base_cont13.size() == 6);
		cont13.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont13)));

		map_type cont14(cont13);
		yggr_test_assert(cont14.size() == 6);
		cont14.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont13)));

		map_type cont15(boost::move(cont14));
		yggr_test_assert(cont15.size() == 6);
		cont15.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont13)));
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_capacity(SegCont& seg_cont)
{
	typedef Cont map_type;
	typedef SegCont seg_cont_type;

	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::hasher hasher_type;
	typedef typename map_type::key_equal key_equal_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;
	typedef typename map_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	base_map_type init_cont(0, hasher_type(), key_equal_type(), alloc);
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));
	init_cont.insert(typename map_type::value_type(0, 0));

	map_type cont1(init_cont.begin(), init_cont.end(), alloc);
	base_map_type base_cont1(init_cont.begin(), init_cont.end(), 0, hasher_type(), key_equal_type(), alloc);

	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.max_size() == base_cont1.max_size());

	yggr_test_assert(cont1.empty() == base_cont1.empty());

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

template<typename Cont>
void check_last_element(typename Cont::base_type& base,
							const std::pair<typename Cont::iterator, bool>& iter_pos, 
							int check)
{
	yggr_test_assert(iter_pos.second);
	yggr_test_assert(iter_pos.first != base.end());
	yggr_test_assert(iter_pos.first->second == check);
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

template<typename Cont>
struct less_pair
{
public:
	typedef Cont map_type;
	typedef typename map_type::key_type key_type;
	typedef typename map_type::value_type value_type;
public:
	less_pair(const key_type& key)
		: key_(key)
	{
	}

public:
	inline bool operator()(const value_type& val) const
	{
		return val.first < key_;
	}

public:
	key_type key_;
};

template<typename Cont, typename SegCont>
void test_modifiers(SegCont& seg_cont)
{
	typedef Cont map_type;
	typedef SegCont seg_cont_type;

	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::hasher hasher_type;
	typedef typename map_type::key_equal key_equal_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;
	typedef typename map_type::value_type val_type;

	typedef iter_type (base_map_type::* begin_end_foo_type)(void);
	typedef citer_type (base_map_type::* cbegin_end_foo_type)(void) const;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	map_type cont(alloc);
	base_map_type base_cont(0, hasher_type(), key_equal_type(), alloc);

	cont.insert(typename map_type::value_type(100, 100));
	base_cont.insert(typename map_type::value_type(100, 100));

	int kn = 90, n = 90;
	{
	    typename map_type::value_type tmp(kn, n);
        cont.insert(boost::move(tmp));
	}
	base_cont.insert(typename map_type::value_type(kn, n));

	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));

	cont.insert(typename map_type::value_type(200, 200), boost::bind(&check_last_element<map_type>, _1, _2, 200));
	base_cont.insert(typename map_type::value_type(200, 200));

	int km = 300, m = 300;
	{
	    typename map_type::value_type tmp(km, m);
        cont.insert(boost::move(tmp), boost::bind(&check_last_element<map_type>, _1, _2, 300));
	}
	base_cont.insert(typename map_type::value_type(300, 300));

	yggr_test_assert(cont.size() == base_cont.size());

	cont.erase(200);
	base_cont.erase(200);

	yggr_test_assert(cont.size() == base_cont.size());

	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));
	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));

	base_map_type base_cont2(base_cont);

	cont.swap(boost::move(base_cont2));
	yggr_test_assert(!cont.empty() && !base_cont2.empty());
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));

	map_type cont3(base_cont);

	cont.swap(cont3);
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));
	cont3.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));

	{
		yggr_test_assert(cont.emplace(yggr::non_handler, 555, 555));
		yggr_test_assert(cont.emplace_hint(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1), yggr::non_handler, 666, 666));
		base_cont.insert(std::make_pair(555, 555));
		base_cont.insert(std::make_pair(666, 666));

		map_type& cont_ref = cont;
		const map_type& cont_cref = cont;

		yggr_test_assert(cont_ref.find(555, boost::bind(&find_handler<map_type>, _1, _2)));
		yggr_test_assert(cont_cref.find(555, boost::bind(&cfind_handler<map_type>, _1, _2)));

		yggr_test_assert(cont.find(555));
		yggr_test_assert(base_cont.find(555) != base_cont.end());

		yggr_test_assert(cont_ref.find(666, boost::bind(&find_handler<map_type>, _1, _2)));
		yggr_test_assert(cont_cref.find(666, boost::bind(&cfind_handler<map_type>, _1, _2)));

		yggr_test_assert(cont.find(666));
		yggr_test_assert(base_cont.find(666) != base_cont.end());
	}

	{
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
					boost::bind(&handler_erase<map_type>, _1, _2));

		cont.erase(boost::bind(&make_erase_iter_pair<map_type>, _1),
					boost::bind(&handler_erase<map_type>, _1, _2));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
					boost::bind(static_cast<begin_end_foo_type>(&base_map_type::end), _1));

		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
					boost::bind(static_cast<begin_end_foo_type>(&base_map_type::end), _1),
					boost::bind(&handler_erase<map_type>, _1, _2));

		yggr_test_assert(cont.empty());
	}

	//emplace
	{
		map_type ins_cont(alloc);
		ins_cont.emplace(yggr::non_handler );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
	}

	{
		map_type ins_cont(alloc);
		ins_cont.emplace(yggr::non_handler, 10, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
	}

	{
		map_type ins_cont(alloc);
		yggr_test_assert(ins_cont.emplace(boost::bind(&ins_handler<map_type>, _1, _2, std::make_pair(0, 0))));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
	}

	{
		map_type ins_cont(alloc);
		yggr_test_assert(ins_cont.emplace(boost::bind(&ins_handler<map_type>, _1, _2, std::make_pair(10, 10)), 10, 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));

	}

	{
		map_type ins_cont(alloc);
		yggr_test_assert(ins_cont.emplace_hint(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1), yggr::non_handler));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
	}

	{
		map_type ins_cont(alloc);
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1), yggr::non_handler, 10, 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
	}

	{
		map_type ins_cont(alloc);
		yggr_test_assert(ins_cont.empty());
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
						boost::bind(&ins_handler<map_type>, _1, _2, std::make_pair(0, 0))));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
	}

	{
		map_type ins_cont(alloc);
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
						boost::bind(&ins_handler<map_type>, _1, _2, std::make_pair(10, 10)), 10, 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
	}

	cont.clear();
	yggr_test_assert(cont.empty());

	// merge
	{
		int arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
		int arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};
		// map

#ifndef YGGR_NO_CXX11_HDR_UNORDERED_MAP
		{
			typedef std::unordered_map<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 15);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef std::unordered_map<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 15);
			yggr_test_assert(cont_zwei.size() == 0);
		}

#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

		{
			typedef boost::unordered_map<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 15);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef boost::unordered_map<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 15);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef yggr::unordered_map<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 15);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef yggr::unordered_map<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 15);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef yggr::safe_container::safe_unordered_map<int, int> now_map_type;
			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(*i, *i * 10);
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 15);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef yggr::safe_container::safe_unordered_map<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(*i, *i * 10);
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 15);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		// unordered_multimap
#ifndef YGGR_NO_CXX11_HDR_UNORDERED_MAP
		{
			typedef std::unordered_multimap<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 20);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef std::unordered_multimap<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 20);
			yggr_test_assert(cont_zwei.size() == 0);
		}

#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

		{
			typedef boost::unordered_multimap<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 20);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef boost::unordered_multimap<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 20);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef yggr::unordered_multimap<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 20);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef yggr::unordered_multimap<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 20);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef yggr::safe_container::safe_unordered_multimap<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 20);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef yggr::safe_container::safe_unordered_multimap<int, int> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(*i, *i * 10);
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 20);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef 
				yggr::safe_container::safe_unordered_multimap
				<
					int, int,
					boost::mutex,
					hasher_type,
					key_equal_type,
					alloc_type,
					yggr::unordered_multimap
				> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei(alloc);
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(std::make_pair(*i, *i * 10));
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 20);
			yggr_test_assert(cont_zwei.size() == 0);
		}

		{
			typedef 
				yggr::safe_container::safe_unordered_multimap
				<
					int, int,
					boost::mutex,
					hasher_type,
					key_equal_type,
					alloc_type,
					yggr::unordered_multimap
				> now_map_type;

			map_type cont_eins(alloc);
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			now_map_type cont_zwei(alloc);
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(*i, *i * 10);
				}
			}

			yggr_test_assert(cont_eins.size() == 10);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 20);
			yggr_test_assert(cont_zwei.size() == 0);
		}
	}

	// erase_if
	{
		int arr[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
		map_type cont(alloc);
		{
			for(int *i = arr, *isize = arr + 10; i != isize; ++i)
			{
				cont.insert(*i, *i * 10);
			}
		}

		yggr_test_assert(cont.size() == 10);

		//struct less_pair
		//{
		//public:
		//	typedef typename map_type::key_type key_type;
		//	typedef typename map_type::value_type value_type;
		//public:
		//	less_pair(const key_type& key)
		//		: key_(key)
		//	{
		//	}

		//public:
		//	inline bool operator()(const value_type& val) const
		//	{
		//		return val.first < key_;
		//	}

		//public:
		//	key_type key_;
		//};

		typedef less_pair<map_type> less_pair_type;

		less_pair_type cmper(3);

		std::size_t rm_size = yggr::safe_container::erase_if(cont, cmper);
		yggr_test_assert(rm_size == 4);
		yggr_test_assert(!cont.contains(1));
		yggr_test_assert(!cont.contains(2));
		yggr_test_assert(cont.contains(3));
		yggr_test_assert(cont.contains(4));
		yggr_test_assert(cont.contains(5));
		yggr_test_assert(cont.size() == 6);
	}

	std::cout << "---------------test_modifiers end-----------------" << std::endl;
}

template<typename Cont>
bool ins_node_handler(typename Cont::base_type& base,
						const std::pair<typename Cont::iterator, bool>& pair)
{
	return pair.second && pair.first != base.end();
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
	typedef Cont map_type;
	typedef SegCont seg_cont_type;

	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::hasher hasher_type;
	typedef typename map_type::key_equal key_equal_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;
	typedef typename map_type::value_type val_type;

	typedef iter_type (base_map_type::* begin_end_foo_type)(void);
	typedef citer_type (base_map_type::* cbegin_end_foo_type)(void) const;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	// extract insert
	{
		int arr[10] = {1, 1, 2, 3, 3};
		{
			map_type ins_cont(alloc);
			for(int *i = arr, *isize = arr + 5; i != isize; ++i)
			{
				ins_cont.insert(*i, *i * 10);
			}
			
			typename map_type::node_type node = ins_cont.extract(3);
			yggr_test_assert(node.key() == 3);
			yggr_test_assert(node.mapped() == 30);
			yggr_test_assert(ins_cont.count(3) == 1);

			bool iret = ins_cont.insert(boost::move(node));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret);

			yggr_test_assert(ins_cont.count(3) == 2);
		}

		{
			map_type ins_cont(alloc);
			for(int *i = arr, *isize = arr + 5; i != isize; ++i)
			{
				ins_cont.insert(*i, *i * 10);
			}
			typename map_type::node_type node = 
				ins_cont.extract(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1));
			yggr_test_assert(!node.empty());
			yggr_test_assert(ins_cont.size() == 4);

			bool iret = 
				ins_cont.insert(
					boost::move(node),
					boost::bind(&ins_node_handler<map_type>, _1, _2));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret);

			yggr_test_assert(ins_cont.size() == 5);
		}

		{
			map_type ins_cont(alloc);
			for(int *i = arr, *isize = arr + 5; i != isize; ++i)
			{
				ins_cont.insert(*i, *i * 10);
			}
			typename map_type::node_type node = ins_cont.extract(3);
			yggr_test_assert(node.key() == 3);
			yggr_test_assert(node.mapped() == 30);
			yggr_test_assert(ins_cont.count(3) == 1);

			bool iret = 
				ins_cont.insert(
					boost::bind(static_cast<begin_end_foo_type>(&base_map_type::end), _1),
					boost::move(node));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret);

			yggr_test_assert(ins_cont.count(3) == 2);
		}

		{
			map_type ins_cont(alloc);
			for(int *i = arr, *isize = arr + 5; i != isize; ++i)
			{
				ins_cont.insert(*i, *i * 10);
			}
			typename map_type::node_type node = ins_cont.extract(3);
			yggr_test_assert(node.key() == 3);
			yggr_test_assert(node.mapped() == 30);
			yggr_test_assert(ins_cont.count(3) == 1);

			bool iret = 
				ins_cont.insert(
					boost::bind(static_cast<begin_end_foo_type>(&base_map_type::end), _1),
					boost::move(node),
					boost::bind(&ins_hint_node_handler<map_type>, _1, _2));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret);

			yggr_test_assert(ins_cont.count(3) == 2);
		}
	}

	std::cout << "---------------test_extract_insert end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_buckets(SegCont& seg_cont)
{
	typedef Cont map_type;
	typedef SegCont seg_cont_type;

	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::hasher hasher_type;
	typedef typename map_type::key_equal key_equal_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;
	typedef typename map_type::value_type val_type;

	typedef iter_type (base_map_type::* begin_end_foo_type)(void);
	typedef citer_type (base_map_type::* cbegin_end_foo_type)(void) const;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	base_map_type init_cont(0, hasher_type(), key_equal_type(), alloc);
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));
	init_cont.insert(typename map_type::value_type(0, 0));

	map_type cont(init_cont.begin(), init_cont.end(), alloc);
	base_map_type base_cont(init_cont.begin(), init_cont.end(), 
								boost::unordered::detail::default_bucket_count, 
								hasher_type(), key_equal_type(), alloc);

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
	typedef Cont map_type;
	typedef SegCont seg_cont_type;

	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::hasher hasher_type;
	typedef typename map_type::key_equal key_equal_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;
	typedef typename map_type::value_type val_type;

	typedef iter_type (base_map_type::* begin_end_foo_type)(void);
	typedef citer_type (base_map_type::* cbegin_end_foo_type)(void) const;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	base_map_type init_cont(0, hasher_type(), key_equal_type(), alloc);
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));
	init_cont.insert(typename map_type::value_type(0, 0));

	map_type cont(init_cont.begin(), init_cont.end(), alloc);
	base_map_type base_cont(init_cont.begin(), init_cont.end(), 
							boost::unordered::detail::default_bucket_count, 
							hasher_type(), key_equal_type(), alloc);

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

	cont.reserve(1000);
	base_cont.reserve(1000);

	yggr_test_assert(cont.load_factor() == base_cont.load_factor());
	yggr_test_assert(cont.max_load_factor() == base_cont.max_load_factor());

	std::cout << "---------------test_hash_policy end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_safe_other(SegCont& seg_cont)
{
	typedef Cont map_type;
	typedef SegCont seg_cont_type;

	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::hasher hasher_type;
	typedef typename map_type::key_equal key_equal_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;
	typedef typename map_type::value_type val_type;

	typedef iter_type (base_map_type::* begin_end_foo_type)(void);
	typedef citer_type (base_map_type::* cbegin_end_foo_type)(void) const;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	base_map_type init_cont(0, hasher_type(), key_equal_type(), alloc);
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));
	init_cont.insert(typename map_type::value_type(0, 0));

	map_type cont(init_cont.begin(), init_cont.end(), alloc);
	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(cont.is_exists(1, cont.hash_function(), cont.key_eq()));
	yggr_test_assert(cont.is_exists(0));
	yggr_test_assert(cont.is_exists(0, cont.hash_function(), cont.key_eq()));
	yggr_test_assert(!cont.is_exists(100));
	yggr_test_assert(!cont.is_exists(100, cont.hash_function(), cont.key_eq()));

	map_type empty_map(alloc);

	std::pair<typename map_type::key_type, typename map_type::mapped_type> beg_val;
	typename map_type::key_type beg_key = 0;
	typename map_type::mapped_type beg_mapped = 0;

	yggr_test_assert(!empty_map.get_begin_value(beg_val));
	yggr_test_assert(!empty_map.get_begin_key(beg_key));
	yggr_test_assert(!empty_map.get_begin_mapped(beg_mapped));

	try
	{
		empty_map.begin_value();
		yggr_test_assert(false);
	}
	catch(const typename map_type::error_type&)
	{
	}

	try
	{
		empty_map.begin_key();
		yggr_test_assert(false);
	}
	catch(const typename map_type::error_type&)
	{
	}

	try
	{
		empty_map.begin_mapped();
		yggr_test_assert(false);
	}
	catch(const typename map_type::error_type&)
	{
	}

	cont.get_begin_value(beg_val);
	yggr_test_assert(beg_val == cont.begin_value());

	cont.get_begin_key(beg_key);
	yggr_test_assert(beg_key == cont.begin_key());

	cont.get_begin_mapped(beg_mapped);
	yggr_test_assert(beg_mapped == cont.begin_mapped());

	yggr_test_assert(cont.is_exists(beg_key));

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void test_swap(SegCont& seg_cont)
{
	typedef Cont map_type;
	typedef SegCont seg_cont_type;

	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::hasher hasher_type;
	typedef typename map_type::key_equal key_equal_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;
	typedef typename map_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	base_map_type init_cont1(0, hasher_type(), key_equal_type(), alloc);
	init_cont1.insert(typename map_type::value_type(1, 1));
	init_cont1.insert(typename map_type::value_type(2, 2));
	init_cont1.insert(typename map_type::value_type(3, 3));
	init_cont1.insert(typename map_type::value_type(4, 4));
	init_cont1.insert(typename map_type::value_type(5, 5));

	base_map_type init_cont2(0, hasher_type(), key_equal_type(), alloc);
	init_cont2.insert(typename map_type::value_type(6, 6));
	init_cont2.insert(typename map_type::value_type(7, 7));
	init_cont2.insert(typename map_type::value_type(8, 8));
	init_cont2.insert(typename map_type::value_type(9, 9));
	init_cont2.insert(typename map_type::value_type(0, 0));

	map_type cont1(init_cont1.begin(), init_cont1.end(), alloc);
	map_type cont2(init_cont2.begin(), init_cont2.end(), alloc);

	base_map_type base_cont1(init_cont1.begin(), init_cont1.end(), 0, hasher_type(), key_equal_type(), alloc);
	base_map_type base_cont2(init_cont2.begin(), init_cont2.end(), 0, hasher_type(), key_equal_type(), alloc);
	yggr_test_assert(cont1.size() == base_cont1.size());

	base_map_type base_cont3(base_cont1);
	base_map_type base_cont4(base_cont2);

	// ------------------ boost::swap test------------------------
	// base_type&& this_type&
	boost::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	boost::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	boost::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	boost::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	boost::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));

	boost::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	boost::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	boost::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));

	// ------------------ std::swap test------------------------
	// base_type&& this_type&
	std::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	std::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	std::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	std::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	std::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));

	std::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	std::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	std::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));


	// ------------------ yggr::swap test------------------------
	// base_type&& this_type&
	yggr::swap(boost::move(base_cont4), cont1);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont4.size() == base_cont1.size());

	// base_type& this_type&
	yggr::swap(base_cont3, cont1);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont3.size() == base_cont2.size());

	// this_type& base_type&&
	yggr::swap(cont1, boost::move(base_cont3));
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	yggr_test_assert(base_cont3.size() == base_cont1.size());

	// this_type& base_type&
	yggr::swap(cont1, boost::move(base_cont4));
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	yggr_test_assert(base_cont4.size() == base_cont2.size());

	// this_type& this_type&
	yggr::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));

	yggr::swap(cont1, cont2);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));

	// this_type&& this_type&
	yggr::swap(boost::move(cont1), cont2);
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));

	// this_type& this_type&&
	yggr::swap(cont1, boost::move(cont2));
	cont1.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont1)));
	cont2.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));

	std::cout << "---------------test_swap end-----------------" << std::endl;
}

template<typename Cont, typename Iter>
bool check_element(const typename Cont::base_type& base, 
					Iter i, int chk_val)
{
	return (i != base.end()) && (i->second == chk_val);
}

template<typename Cont, typename SegCont>
void test_find(SegCont& seg_cont)
{
	typedef Cont map_type;
	typedef SegCont seg_cont_type;

	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::hasher hasher_type;
	typedef typename map_type::key_equal key_equal_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;
	typedef typename map_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	base_map_type init_cont(0, hasher_type(), key_equal_type(), alloc);
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));

	map_type cont(init_cont);
	map_type& ref_cont = cont;
	const map_type& cref_cont = cont;

	yggr_test_assert(ref_cont.find(1, boost::bind(&check_element<map_type, typename map_type::iterator>, _1, _2, 1)));
	yggr_test_assert(cref_cont.find(2, boost::bind(&check_element<map_type, typename map_type::const_iterator>, _1, _2, 2)));

	yggr_test_assert(!ref_cont.find(100, boost::bind(&check_element<map_type, typename map_type::iterator>, _1, _2, 100)));
	yggr_test_assert(!cref_cont.find(100, boost::bind(&check_element<map_type, typename map_type::const_iterator>, _1, _2, 100)));

	yggr_test_assert(ref_cont.find(1, cont.hash_function(), cont.key_eq(), 
									boost::bind(&check_element<map_type, typename map_type::iterator>, _1, _2, 1)));
	yggr_test_assert(cref_cont.find(2, cont.hash_function(), cont.key_eq(), 
									boost::bind(&check_element<map_type, typename map_type::const_iterator>, _1, _2, 2)));

	yggr_test_assert(!ref_cont.find(100, cont.hash_function(), cont.key_eq(), 
									boost::bind(&check_element<map_type, typename map_type::iterator>, _1, _2, 100)));
	yggr_test_assert(!cref_cont.find(100, cont.hash_function(), cont.key_eq(), 
									boost::bind(&check_element<map_type, typename map_type::const_iterator>, _1, _2, 100)));

	std::cout << "---------------test_find end-----------------" << std::endl;

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

template<typename Iter, typename Cont>
void ref_all_check_handler(Iter iter, const typename Cont::base_type& r)
{
	typedef Cont map_type;
	typename map_type::const_iterator i = r.find(iter->first);
	yggr_test_assert(i != r.end());
}

template<typename Iter, typename Cont>
void cref_all_check_handler(Iter iter, const typename Cont::base_type& r)
{
	typedef Cont map_type;
	typename map_type::const_iterator i = r.find(iter->first);
	yggr_test_assert(i != r.end());
}

template<typename Cont>
void check_handler_eins(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
	//typename map_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
	{
		yggr_test_assert(r.find(i->first) != r.end());
	}
}

template<typename Cont, typename Handler>
void check_handler_zwei(const typename Cont::base_type& l, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
	//typename map_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
	{
		yggr_test_assert(r.find(i->first) != r.end());
	}
	ret_handler();
}

template<typename Cont>
void set_handler_eins(typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
	if(!l.empty())
	{
		yggr_test_assert(l.size() == r.size());
		for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
		{
			yggr_test_assert(r.find(i->first) == r.end());
		}
	}
	l = r;
	{
		for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
		{
			yggr_test_assert(r.find(i->first) != r.end());
		}
	}
}

template<typename Cont, typename Handler>
void set_handler_zwei(typename Cont::base_type& l, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
	if(!l.empty())
	{
		yggr_test_assert(l.size() == r.size());
		for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
		{
			yggr_test_assert(r.find(i->first) == r.end());
		}
	}
	l = r;
	{
		for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
		{
			yggr_test_assert(r.find(i->first) != r.end());
		}
	}

	ret_handler();
}

template<typename Cont, typename Handler>
void return_handler_call_helper(Cont& cont, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont map_type;
	typedef Handler ret_handler_type;
	cont.using_handler(boost::bind(&set_handler_zwei<map_type, ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Cont, typename Handler>
void return_handler_call_helper(const Cont& cont, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont map_type;
	typedef Handler ret_handler_type;
	cont.using_handler(boost::bind(&check_handler_zwei<map_type, ret_handler_type>, _1, boost::cref(r), _2), ret_handler);
}

template<typename Cont, typename SegCont>
void test_using_handler(SegCont& seg_cont)
{
	typedef Cont map_type;
	typedef SegCont seg_cont_type;

	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::hasher hasher_type;
	typedef typename map_type::key_equal key_equal_type;
	typedef typename map_type::allocator_type alloc_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;
	typedef typename map_type::value_type val_type;

	alloc_type alloc(seg_cont.segment().get_segment_manager());

	base_map_type init_cont1(0, hasher_type(), key_equal_type(), alloc);
	init_cont1.insert(typename map_type::value_type(1, 1));
	init_cont1.insert(typename map_type::value_type(2, 2));
	init_cont1.insert(typename map_type::value_type(3, 3));
	init_cont1.insert(typename map_type::value_type(4, 4));
	init_cont1.insert(typename map_type::value_type(5, 5));

	base_map_type init_cont2(0, hasher_type(), key_equal_type(), alloc);
	init_cont2.insert(typename map_type::value_type(6, 6));
	init_cont2.insert(typename map_type::value_type(7, 7));
	init_cont2.insert(typename map_type::value_type(8, 8));
	init_cont2.insert(typename map_type::value_type(9, 9));
	init_cont2.insert(typename map_type::value_type(10, 10));

	base_map_type base_cont1(init_cont1.begin(), init_cont1.end(), 0, hasher_type(), key_equal_type(), alloc);
	base_map_type base_cont2(init_cont2.begin(), init_cont2.end(), 0, hasher_type(), key_equal_type(), alloc);

	map_type cont(alloc);
	map_type& ref_cont = cont;
	const map_type& cref_cont = cont;

	ref_cont.using_handler(boost::bind(&set_handler_eins<map_type>, _1, boost::cref(base_cont1)));
	cref_cont.using_handler(boost::bind(&check_handler_eins<map_type>, _1, boost::cref(base_cont1)));

	int ref_count = 0;
	return_handler_call_helper(ref_cont, base_cont2, boost::bind(&return_handler, boost::ref(ref_count)));
	yggr_test_assert(ref_count == 1);

	int cref_count = 0;
	return_handler_call_helper(cref_cont, base_cont2, boost::bind(&return_handler, boost::ref(cref_count)));
	yggr_test_assert(cref_count == 1);

	ref_cont.for_each_iterator(boost::bind(&ref_all_check_handler<typename map_type::iterator, map_type>,
											_1, boost::cref(base_cont2)));
	cref_cont.for_each_iterator(boost::bind(&cref_all_check_handler<typename map_type::const_iterator, map_type>,
												_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.for_each_iterator(boost::bind(&ref_all_check_handler<typename map_type::iterator, map_type>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<typename map_type::iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(ref_count)));
	yggr_test_assert(ref_count == 2);

	cref_count = 0;
	cref_cont.for_each_iterator(boost::bind(&cref_all_check_handler<typename map_type::const_iterator, map_type>,
											_1, boost::cref(base_cont2)),
								boost::bind(&cdt_handler<typename map_type::const_iterator, std::less<int> >,
												_1, 8, std::less<int>(), boost::ref(cref_count)));
	yggr_test_assert(cref_count == 2);


	std::cout << "---------------test_use_handler end-----------------" << std::endl;
}

template<typename Cont, typename SegCont>
void debug_test(SegCont& seg_cont)
{
	typedef Cont map_type;

	test_constructor<map_type>(seg_cont);
	test_capacity<map_type>(seg_cont);
	test_modifiers<map_type>(seg_cont);
	test_extract_insert<map_type>(seg_cont);
	test_buckets<map_type>(seg_cont);
	test_hash_policy<map_type>(seg_cont);
	test_safe_other<map_type>(seg_cont);
	test_swap<map_type>(seg_cont);
	test_find<map_type>(seg_cont);
	test_using_handler<map_type>(seg_cont);
}


int main(int argc, char* argv[])
{
	// managed_shared_memory
	{
		typedef yggr::safe_container::safe_unordered_multimap<int, int>::value_type map_value_type;
		typedef boost::interprocess::managed_shared_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<map_value_type, seg_type::segment_manager> alloc_type;
		typedef 
			yggr::safe_container::safe_unordered_multimap
			<
				int, int,
				boost::interprocess::interprocess_mutex, 
				boost::hash<int>,
				std::equal_to<int>,
				alloc_type
			> map_type;
		
		seg_cont_type seg_cont("test_managed_shared_memory_safe_unordered_multimap",
								yggr::segment::segment_op::create_only(), 
								65536,
								boost::bind(
									&delete_segment<seg_type>, 
									"test_managed_shared_memory_safe_unordered_multimap") );

		yggr_test_assert(!seg_cont.empty());

		debug_test<map_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	// test_mamaged_mapped_file
	{
		typedef yggr::safe_container::safe_unordered_multimap<int, int>::value_type map_value_type;
		typedef boost::interprocess::managed_mapped_file seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<map_value_type, seg_type::segment_manager> alloc_type;
		typedef 
			yggr::safe_container::safe_unordered_multimap
			<
				int, int,
				boost::interprocess::interprocess_mutex, 
				boost::hash<int>,
				std::equal_to<int>,
				alloc_type
			> map_type;
		
		seg_cont_type seg_cont("../test_out/segment/test_mamaged_mapped_file_safe_unordered_multimap",
								yggr::segment::segment_op::create_only(), 
								65536,
								boost::bind(
									&delete_segment<seg_type>,
									"../test_out/segment/test_mamaged_mapped_file_safe_unordered_multimap"));

		yggr_test_assert(!seg_cont.empty());

		debug_test<map_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	// test_managed_heap_memory
	{
		typedef yggr::safe_container::safe_unordered_multimap<int, int>::value_type map_value_type;
		typedef boost::interprocess::managed_heap_memory seg_type;
		typedef yggr::segment::segment_container<seg_type> seg_cont_type;
		typedef boost::interprocess::allocator<map_value_type, seg_type::segment_manager> alloc_type;
		typedef 
			yggr::safe_container::safe_unordered_multimap
			<
				int, int,
				boost::interprocess::interprocess_mutex, 
				boost::hash<int>,
				std::equal_to<int>,
				alloc_type
			> map_type;
		
		seg_cont_type seg_cont(65536);

		yggr_test_assert(!seg_cont.empty());

		debug_test<map_type>(seg_cont);

		seg_cont.free();
		yggr_test_assert(!seg_cont.is_named_enable_object());
		yggr_test_assert(seg_cont.empty());
	}

	wait_any_key(argc, argv);
	return 0;
}
