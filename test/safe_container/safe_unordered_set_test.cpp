//safe_unordered_set_test.cpp

#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <functional>

#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/safe_container/safe_unordered_set.hpp>

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
void check_range_equal(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont set_type;
	typedef typename set_type::const_iterator iter_type;
	//typename set_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	bool bchk = false;
	for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
	{
		bchk = (r.find(*i) != r.end());
		yggr_test_assert(bchk);
	}
}

template<typename Cont>
void test_constructor(void)
{
	typedef Cont set_type;

	set_type cont1;
	yggr_test_assert(cont1.empty());

	set_type cont2(cont1.get_allocator());
	yggr_test_assert(cont2.empty());

	set_type cont3(cont1.key_eq(), cont1.get_allocator());
	yggr_test_assert(cont3.empty());

	set_type cont4(cont3.hash_function(), cont3.key_eq(), cont2.get_allocator());
	yggr_test_assert(cont4.empty());

	set_type cont4_2(5, cont3.hash_function(), cont3.key_eq(), cont2.get_allocator());
	yggr_test_assert(cont4_2.empty());

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont5(arr + 0, arr + 10);
	yggr_test_assert(cont5.size() == 6);
	yggr_test_assert(cont5.count(1) == 1);

	set_type cont6(arr + 0, arr + 10, cont5.get_allocator());
	yggr_test_assert(cont6.size() == 6);

	set_type cont7(arr + 0, arr + 10, cont5.key_eq(), cont5.get_allocator());
	yggr_test_assert(cont7.size() == 6);

	set_type cont8(arr + 0, arr + 10, cont5.hash_function(), cont5.key_eq(), cont5.get_allocator());
	yggr_test_assert(cont8.size() == 6);

	set_type cont8_2(arr + 0, arr + 10, 5, cont5.hash_function(), cont5.key_eq(), cont5.get_allocator());
	yggr_test_assert(cont8_2.size() == 6);

	typename set_type::base_type base_cont9(arr + 0, arr + 10);
	set_type cont9(boost::move(base_cont9));
	yggr_test_assert(cont9.size() == 6);

	typename set_type::base_type base_cont10(arr + 0, arr + 10);
	set_type cont10(base_cont10);
	yggr_test_assert(cont10.size() == 6 && base_cont10.size() == 6);
	cont10.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont10)));

	typename set_type::base_type base_cont11(arr + 0, arr + 10);
	set_type cont11(boost::move(base_cont11), base_cont11.get_allocator());
	yggr_test_assert(cont11.size() == 6);

	typename set_type::base_type base_cont12(arr + 0, arr + 10);
	set_type cont12(base_cont12, base_cont12.get_allocator());
	yggr_test_assert(cont12.size() == 6 && base_cont12.size() == 6);
	cont12.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont12)));

	{
		set_type cont11(arr + 0, arr + 10);
		typename set_type::base_type base_cont11 = cont11;

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
		typename set_type::base_type base_cont13(arr + 0, arr + 10);
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

template<typename Cont>
void test_capacity(void)
{
	typedef Cont set_type;

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont1(arr + 0, arr + 10);
	typename set_type::base_type base_cont1(arr + 0, arr + 10);

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

template<typename Cont>
void test_modifiers(void)
{
	typedef Cont set_type;
	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;

	typedef iter_type (base_set_type::* begin_end_foo_type)(void);
	typedef citer_type (base_set_type::* cbegin_end_foo_type)(void) const;

	set_type cont;
	typename set_type::base_type base_cont;

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

	typename set_type::base_type base_cont2(base_cont);

	cont.swap(boost::move(base_cont2));
	yggr_test_assert(!cont.empty() && !base_cont2.empty());
	cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));

	set_type cont3(base_cont);

	cont.swap(cont3);
	cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));
	cont3.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));

	//emplace
	{
		set_type ins_cont;
		ins_cont.emplace(yggr::non_handler );
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
		yggr_test_assert(ins_cont.find(0, ins_cont.hash_function(), ins_cont.key_eq()));
	}

	{
		set_type ins_cont;
		ins_cont.emplace(yggr::non_handler, 10);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
	}

	{
		set_type ins_cont;
		yggr_test_assert(ins_cont.emplace(boost::bind(&ins_handler<set_type>, _1, _2, 0)));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
	}

	{
		set_type ins_cont;
		yggr_test_assert(ins_cont.emplace(boost::bind(&ins_handler<set_type>, _1, _2, 10), 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
	}

	{
		set_type ins_cont;
		ins_cont.emplace_hint(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1), yggr::non_handler);
		yggr_test_assert(ins_cont.emplace_hint(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1), yggr::non_handler));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
	}

	{
		set_type ins_cont;
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1), yggr::non_handler, 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
	}

	{
		set_type ins_cont;
		yggr_test_assert(ins_cont.empty());
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1),
						boost::bind(&ins_handler<set_type>, _1, _2, 0)));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
	}

	{
		set_type ins_cont;
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
#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET
		{
			typedef std::unordered_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef std::unordered_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

		{
			typedef boost::unordered_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef boost::unordered_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::unordered_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::unordered_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			set_type cont_eins(arr1 + 0, arr1 + 10);
			set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			set_type cont_eins(arr1 + 0, arr1 + 10);
			set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		// unordered_multiset
#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET
		{
			typedef std::unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef std::unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}
#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

		{
			typedef boost::unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef boost::unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::safe_container::safe_unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::safe_container::safe_unordered_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

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

		set_type cont(arr + 0, arr + 5);

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

template<typename Cont>
void test_extract_insert(void)
{
	typedef Cont set_type;
	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;

	typedef iter_type (base_set_type::* begin_end_foo_type)(void);
	typedef citer_type (base_set_type::* cbegin_end_foo_type)(void) const;

	// extract insert
	{
		int arr[10] = {1, 2, 3, 4, 5};
		{
			set_type ins_cont;
			ins_cont.insert(arr + 0, arr + 5);
			typename set_type::node_type node = ins_cont.extract(666);
			yggr_test_assert(node.empty());

			typename set_type::insert_return_type iret = ins_cont.insert(boost::move(node));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret.node.empty());
			yggr_test_assert(!iret.inserted);
		}

		{
			set_type ins_cont;
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
			set_type ins_cont;
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
			set_type ins_cont;
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
			set_type ins_cont;
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

template<typename Cont>
void test_element_lookup(void)
{
	typedef Cont set_type;

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr + 0, arr + 10);
	yggr_test_assert(cont.count(0) == 1);

	set_type& cont_ref = cont;
	const set_type& cont_cref = cont;

	cont_ref.equal_range(3, boost::bind(&check_equal_range<set_type, typename set_type::iterator>,
											_1, _2, 3));

	cont_cref.equal_range(3, boost::bind(&check_equal_range<set_type, typename set_type::const_iterator>,
											_1, _2, 3));

	std::cout << "---------------test_element_lookup end-----------------" << std::endl;
}

template<typename Cont>
void test_buckets(void)
{
	typedef Cont set_type;

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr + 0, arr + 10);
	typename set_type::base_type base_cont(arr + 0, arr + 10);

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

template<typename Cont>
void test_hash_policy(void)
{
	typedef Cont set_type;

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr + 0, arr + 10);
	typename set_type::base_type base_cont(arr + 0, arr + 10);

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

template<typename Cont>
void test_safe_other(void)
{
	typedef Cont set_type;

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr + 0, arr + 10);
	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(cont.is_exists(1, cont.hash_function(), cont.key_eq()));
	yggr_test_assert(cont.is_exists(0));
	yggr_test_assert(cont.is_exists(0, cont.hash_function(), cont.key_eq()));
	yggr_test_assert(!cont.is_exists(100));
	yggr_test_assert(!cont.is_exists(100, cont.hash_function(), cont.key_eq()));

	set_type empty_set;

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

template<typename Cont>
void test_swap(void)
{
	typedef Cont set_type;

	int arr1[10] = {1, 2, 3, 4, 5};
	int arr2[10] = {6, 7, 8, 9, 0};

	set_type cont1(arr1 + 0, arr1 + 10);
	set_type cont2(arr2 + 0, arr2 + 5);

	typename set_type::base_type base_cont1(arr1 + 0, arr1 + 10);
	typename set_type::base_type base_cont2(arr2 + 0, arr2 + 5);
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() != base_cont2.size());

	typename set_type::base_type base_cont3(base_cont1);
	typename set_type::base_type base_cont4(base_cont2);

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

template<typename Cont>
void test_find(void)
{
	typedef Cont set_type;

	int arr[5] = {1, 2, 3, 4, 5};

	set_type cont(arr + 0, arr + 5);
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

template<typename Cont>
void test_using_handler(void)
{
	typedef Cont set_type;

	int arr1[5] = {1, 2, 3, 4, 5};
	int arr2[5] = {6, 7, 8, 9, 10};

	typename set_type::base_type base_cont1(arr1 + 0, arr1 + 5);
	typename set_type::base_type base_cont2(arr2 + 0, arr2 + 5);

	set_type cont;
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

template<typename Cont>
void debug_test(void)
{
	typedef Cont set_type;

	test_constructor<set_type>();
	test_capacity<set_type>();
	test_modifiers<set_type>();
	test_extract_insert<set_type>();
	test_element_lookup<set_type>();
	test_buckets<set_type>();
	test_hash_policy<set_type>();
	test_safe_other<set_type>();
	test_swap<set_type>();
	test_find<set_type>();
	test_using_handler<set_type>();
}

template<typename Cont>
void debug_test_std(void)
{
	typedef Cont set_type;

	test_constructor<set_type>();
	test_capacity<set_type>();
	test_modifiers<set_type>();
#if !(YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	test_extract_insert<set_type>();
#endif // !(YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	test_element_lookup<set_type>();
	test_buckets<set_type>();
	test_hash_policy<set_type>();
	test_safe_other<set_type>();
	test_swap<set_type>();
	test_find<set_type>();
	test_using_handler<set_type>();
}

//-----------------------test safe---------------------------------------------

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <cstdlib>
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
bool push_handler(typename Container::base_type&,
					const std::pair<typename Container::iterator, bool>& iter_pair, 
					int& count)
{
	++count;
	return iter_pair.second;
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
	std::cout << n <<std::endl;
}

template<typename Val>
Val create_val(void)
{
	typedef Val val_type;
	typedef typename val_type::element_type elem_type;
	return val_type(new elem_type(std::rand() % 1000));
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
	case 3: // key_eq
		hvt.push_back(3 + E_read);
		cont.key_eq();
		break;
	case 4: // hash_function
		hvt.push_back(4 + E_read);
		cont.hash_function();
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
	case 7: // using_handler const
		hvt.push_back(7 + E_read);
		cont.using_handler(boost::bind(&cref_handler<container_type>, _1));
		break;
	case 8:
		hvt.push_back(8 + E_read);
		cont.count(create_val<val_type>());
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
		{
		    val_type val(create_val<val_type>());
            cont.insert(boost::move(val));
		}
		break;
	case 4: // insert(val&&, handler)
		{
			hvt.push_back(4 + E_write);
			int count = 0;
			val_type val(create_val<val_type>());
			bool bret = cont.insert(boost::move(val),
							boost::bind(&push_handler<container_type>, _1, _2, boost::ref(count)));
			yggr_test_assert((count == 1 && bret == true) || (count == 0 && bret == false));
		}
		break;
	case 5: // // push_back(const val&, handler)
		{
			hvt.push_back(5 + E_write);
			int count = 0;
			bool bret = cont.insert(create_val<val_type>(),
							boost::bind(&push_handler<container_type>, _1, _2, boost::ref(count)));
			yggr_test_assert((count == 1 && bret == true) || (count == 0 && bret == false));
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

	if(!(loop_size && trd_size))
	{
		return;
	}

	container_type cont;
	hvt_type hvt;

	boost::thread_group trds;

	trds.create_thread(boost::bind(&thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), true));

	for(int i = 0, isize = trd_size - 1; i != isize; ++i)
	{
		trds.create_thread(boost::bind(thread_foo_of_shard_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), false));
	}
	trds.join_all();

	std::cout << "test_thread_safe_of_shared_mutex end" << std::endl;
}

int main(int argc, char* argv[])
{
	{
		typedef yggr::safe_container::safe_unordered_set<int> set_type;
		debug_test<set_type>();
	}

#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET
	{
		typedef 
			yggr::safe_container::safe_unordered_set
			<
				int, 
				boost::mutex,
#   if defined(__GNUC__)
#       if (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
				// in GCC CPP17 and later stl, hashes are split into fast and non-fast,
                //  fast hash and non-fast hash do not support merge operations
                std::hash<int>,
#       else
                boost::hash<int>,
#       endif // (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
#   else
				std::hash<int>,
#   endif // __GNUC__
				std::equal_to<int>,
				std::allocator<int>,
				std::unordered_set
			> set_type;

		debug_test_std<set_type>();
	}
#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

	std::srand(std::time(0));
	
	// mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_unordered_set<val_type> set_type;
		test_thread_safe_of_mutex<set_type>();
	}

#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef 
			yggr::safe_container::safe_unordered_set
			<
				val_type, 
				boost::mutex,
#   if defined(__GNUC__)
#       if (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
				// in GCC CPP17 and later stl, hashes are split into fast and non-fast,
                //  fast hash and non-fast hash do not support merge operations
                std::hash<val_type>,
#       else
                boost::hash<val_type>,
#       endif // __MINGW32__ __linux
#   else
				std::hash<val_type>,
#   endif // __GNUC__
				std::equal_to<val_type>,
				std::allocator<val_type>,
				std::unordered_set
			> set_type;
		test_thread_safe_of_mutex<set_type>();
	}
#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

	// shared_mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_unordered_set<val_type, boost::shared_mutex> set_type;
		test_thread_safe_of_shared_mutex<set_type>();
	}

#ifndef YGGR_NO_CXX11_HDR_UNORDERED_SET
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef 
			yggr::safe_container::safe_unordered_set
			<
				val_type, 
				boost::shared_mutex,
#   if defined(__GNUC__)
#       if (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
				// in GCC CPP17 and later stl, hashes are split into fast and non-fast,
                //  fast hash and non-fast hash do not support merge operations
                std::hash<val_type>,
#       else
                boost::hash<val_type>,
#       endif // (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
#   else
				std::hash<val_type>,
#   endif // __GNUC__
				std::equal_to<val_type>,
				std::allocator<val_type>,
				std::unordered_set
			> set_type;
		test_thread_safe_of_shared_mutex<set_type>();
	}
#endif // YGGR_NO_CXX11_HDR_UNORDERED_SET

	wait_any_key(argc, argv);
	return 0;
}
