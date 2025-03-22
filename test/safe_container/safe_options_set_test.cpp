// safe_set_test.cpp

// #define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>
#include <functional>

#include <boost/bind.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/safe_container/safe_set.hpp>
#include <yggr/safe_container/safe_options_set.hpp>

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
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		yggr_test_assert(*i == *j);
	}
}

template<typename Cont>
void test_constructor(void)
{
	typedef Cont set_type;

	set_type cont1;
	yggr_test_assert(cont1.empty());

	set_type cont2(cont1.key_comp());
	yggr_test_assert(cont2.empty());

	set_type cont3(cont1.get_allocator());
	yggr_test_assert(cont3.empty());

	set_type cont4(cont3.key_comp(), cont2.get_allocator());
	yggr_test_assert(cont4.empty());

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont5(arr + 0, arr + 10);
	yggr_test_assert(cont5.size() == 6);

	set_type cont6(arr + 0, arr + 10, cont5.key_comp());
	yggr_test_assert(cont6.size() == 6);

	set_type cont7(arr + 0, arr + 10, cont5.get_allocator());
	yggr_test_assert(cont7.size() == 6);

	set_type cont8(arr + 0, arr + 10, cont5.key_comp(), cont5.get_allocator());
	yggr_test_assert(cont8.size() == 6);

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

		set_type cont12(cont11);

		// safe_copy_safe
		yggr_test_assert(cont11 == cont12);
		yggr_test_assert(!(cont11 != cont12));
		yggr_test_assert(cont11 <= cont12);
		yggr_test_assert(!(cont11 < cont12));
		yggr_test_assert(cont11 >= cont12);
		yggr_test_assert(!(cont11 > cont12));
	}

	{
		typename set_type::base_type base_cont13(arr + 0, arr + 10);
		set_type cont13(boost::move(base_cont13), base_cont13.get_allocator());
		yggr_test_assert(cont13.size() == 6);
		set_type cont14(cont13);
		yggr_test_assert(cont14.size() == 6);
		set_type cont15(boost::move(cont14));
		yggr_test_assert(cont15.size() == 6);
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
void check_last_element(typename Cont::base_type& base,
							const std::pair<typename Cont::iterator, bool>& iter_pair,
							int check)
{
	yggr_test_assert(iter_pair.first != base.end());
	yggr_test_assert(iter_pair.second);
	yggr_test_assert(*(iter_pair.first) == check);
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
void test_modifiers(void)
{
	typedef Cont set_type;
	typedef typename set_type::base_type base_set_type;
	typedef typename set_type::iterator iter_type;
	typedef typename set_type::const_iterator citer_type;

	typedef iter_type (base_set_type::* begin_end_foo_type)(void);
	typedef citer_type (base_set_type::* cbegin_end_foo_type)(void) const;

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont;
	typename set_type::base_type base_cont;

	cont.insert(100);
	base_cont.insert(100);

	int n = 90;
	cont.insert(boost::move(n));
	base_cont.insert(90);

	cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont)));

	cont.insert(200, boost::bind(&check_last_element<set_type>, _1, _2,  200));
	base_cont.insert(200);

	int m = 300;
	cont.insert(boost::move(m), boost::bind(&check_last_element<set_type>, _1, _2, 300));
	base_cont.insert(300);

	yggr_test_assert(cont.size() == base_cont.size());

	cont.erase(200);
	base_cont.erase(200);

	yggr_test_assert(cont.size() == base_cont.size());

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

	{
		yggr_test_assert(cont.emplace(yggr::non_handler, 555));
		yggr_test_assert(cont.emplace_hint(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1),
									yggr::non_handler, 666));
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

	// extract insert
	{
		set_type ins_cont;
		ins_cont.insert(arr + 0, arr + 10);
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
		ins_cont.insert(arr + 0, arr + 10);
		typename set_type::node_type node =
			ins_cont.extract(boost::bind(static_cast<begin_end_foo_type>(&base_set_type::begin), _1));
		yggr_test_assert(node.value() == 0);
		yggr_test_assert(!ins_cont.find(0));

		typename set_type::insert_return_type iret =
			ins_cont.insert(
				boost::move(node),
				boost::bind(&ins_node_handler<set_type>, _1, _2));
		yggr_test_assert(node.empty());
		yggr_test_assert(iret.node.empty());
		yggr_test_assert(iret.inserted);

		yggr_test_assert(ins_cont.find(0));
	}

	{
		set_type ins_cont;
		ins_cont.insert(arr + 0, arr + 10);
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
		ins_cont.insert(arr + 0, arr + 10);
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

	// merge
	{
		int arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
		int arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};
		// set
		{
			typedef std::set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef std::set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef boost::container::set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef boost::container::set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::options_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::options_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::safe_container::safe_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::safe_container::safe_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::safe_container::safe_options_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::safe_container::safe_options_set<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		// multiset
		{
			typedef std::multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef std::multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef boost::container::multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef boost::container::multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::options_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::options_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::safe_container::safe_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
//			std::size_t n = cont_zwei.size();
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::safe_container::safe_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::safe_container::safe_options_multiset<int> now_set_type;

			set_type cont_eins(arr1 + 0, arr1 + 10);
			now_set_type cont_zwei(arr2 + 0, arr2 + 10);

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
//			std::size_t n = cont_zwei.size();
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::safe_container::safe_options_multiset<int> now_set_type;

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

template<typename Cont, typename Iter>
void check_equal_range(const typename Cont::base_type&,
						const std::pair<Iter, Iter>& iter_pair,
						typename Cont::value_type now,
						typename Cont::value_type next)
{
	Iter s = iter_pair.first;
	++s;
	yggr_test_assert(s == iter_pair.second);
	yggr_test_assert(*iter_pair.first == now);
	yggr_test_assert(*iter_pair.second == next);
}

template<typename Cont>
void test_operations(void)
{
	typedef Cont set_type;

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr + 0, arr + 10);
	yggr_test_assert(cont.count(0) == 1);

	set_type& cont_ref = cont;
	const set_type& cont_cref = cont;

	cont_ref.equal_range(3, boost::bind(&check_equal_range<set_type, typename set_type::iterator>,
											_1, _2, 3, 4));

	cont_cref.equal_range(3, boost::bind(&check_equal_range<set_type, typename set_type::const_iterator>,
											_1, _2, 3, 4));

	cont_ref.rebalance();

	std::cout << "---------------test_operations end-----------------" << std::endl;
}

template<typename Cont>
void test_safe_other(void)
{
	typedef Cont set_type;

	int arr[10] = {1, 2, 3, 4, 5};
	set_type cont(arr + 0, arr + 10);
	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(cont.is_exists(0));
	yggr_test_assert(!cont.is_exists(100));

	typename set_type::base_type base_cont1 = cont.load();
	cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont1)));

	typename set_type::base_type base_cont2 = cont;
	cont.using_handler(boost::bind(&check_range_equal<set_type>, _1, boost::cref(base_cont2)));

	set_type empty_set;

	int beg_val = 0, rbeg_val = 0;

	yggr_test_assert(!empty_set.get_begin_value(beg_val));
	yggr_test_assert(!empty_set.get_rbegin_value(rbeg_val));

	try
	{
		empty_set.begin_value();
		yggr_test_assert(false);
	}
	catch(const typename set_type::error_type&)
	{
	}

	try
	{
		empty_set.rbegin_value();
		yggr_test_assert(false);
	}
	catch(const typename set_type::error_type&)
	{
	}

	cont.get_begin_value(beg_val);
	cont.get_rbegin_value(rbeg_val);

	yggr_test_assert(beg_val == cont.begin_value());
	yggr_test_assert(rbeg_val == cont.rbegin_value());

	yggr_test_assert(beg_val == 0);
	yggr_test_assert(rbeg_val == 5);

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
void ref_all_check_iter_handler(Iter iter, const typename Cont::base_type& r)
{
	typedef Cont set_type;
	typename set_type::const_iterator i = std::find(r.begin(), r.end(), *iter);
	yggr_test_assert(i != r.end());
}

template<typename Val, typename Cont>
void ref_all_check_value_handler(const Val& val, const typename Cont::base_type& r)
{
	typedef Cont set_type;
	typename set_type::const_iterator i = std::find(r.begin(), r.end(), val);
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
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		yggr_test_assert(*i == *j);
	}
}

template<typename Cont, typename Handler>
void check_handler_zwei(const typename Cont::base_type& l, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont set_type;
	typedef typename set_type::const_iterator iter_type;
	//typename set_type::size_type size = l.size();
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
	typedef Cont set_type;
	typedef typename set_type::const_iterator iter_type;
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
	typedef Cont set_type;
	typedef typename set_type::const_iterator iter_type;
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

	ref_cont.for_each_iterator(boost::bind(&ref_all_check_iter_handler<typename set_type::iterator, set_type>,
											_1, boost::cref(base_cont2)));

	ref_cont.for_each(boost::bind(&ref_all_check_value_handler<typename set_type::value_type, set_type>,
										_1, boost::cref(base_cont2)));

	ref_cont.for_each_reverse(boost::bind(&ref_all_check_value_handler<typename set_type::value_type, set_type>,
											_1, boost::cref(base_cont2)));

	cref_cont.for_each_iterator(boost::bind(&cref_all_check_handler<typename set_type::const_iterator, set_type>,
												_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.for_each_iterator(boost::bind(&ref_all_check_iter_handler<typename set_type::iterator, set_type>,
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

	ref_cont.for_each_iterator_reverse(boost::bind(&ref_all_check_iter_handler<typename set_type::reverse_iterator, set_type>,
													_1, boost::cref(base_cont2)));
	cref_cont.for_each_iterator_reverse(boost::bind(&cref_all_check_handler<typename set_type::const_reverse_iterator, set_type>,
													_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.for_each_iterator_reverse(boost::bind(&ref_all_check_iter_handler<typename set_type::reverse_iterator, set_type>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<typename set_type::reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(ref_count)));
	yggr_test_assert(ref_count == 2);

	cref_count = 0;
	cref_cont.for_each_iterator_reverse(boost::bind(&cref_all_check_handler<typename set_type::const_reverse_iterator, set_type>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<typename set_type::const_reverse_iterator, std::less<int> >,
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
	test_operations<set_type>();
	test_safe_other<set_type>();
	test_swap<set_type>();
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
					const std::pair<typename Container::iterator, bool>& iter_pair, int& count)
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
	int op = std::rand() % 8;
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
	case 7: // using_handler const
		hvt.push_back(7 + E_read);
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
	typedef typename container_base_type::iterator iterator_type;
	typedef typename container_base_type::const_iterator const_iterator_type;

	typedef iterator_type (container_base_type::* begin_end_foo_type)(void);
	typedef const_iterator_type (container_base_type::* cbegin_end_foo_type)(void) const;

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
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
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
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
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
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES
		break;
	case 4: // insert(val&&, handler)
		{
			hvt.push_back(4 + E_write);
			int count = 0;
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			bool bret = cont.insert(boost::move(create_val<val_type>()),
							boost::bind(&push_handler<container_type>, _1, _2, boost::ref(count)));
#else
            val_type val(create_val<val_type>());
            bool bret = cont.insert(boost::move(val),
							boost::bind(&push_handler<container_type>, _1, _2, boost::ref(count)));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
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
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
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
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
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
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
		}
		break;
	case 10: //clear
		hvt.push_back(10 + E_write);
		cont.clear();
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
		break;
	case 11: // use handler
		hvt.push_back(11 + E_write);
		cont.using_handler(boost::bind(&ref_handler<container_type>, _1));
		cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
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
		typedef yggr::safe_container::safe_options_set<int> set_type;
		debug_test<set_type>();
	}

	{
		typedef
			yggr::safe_container::safe_options_set
			<
				int,
				boost::mutex,
				std::less<int>,
				std::allocator<int>
			> set_type;
		debug_test<set_type>();
	}

#if !(BOOST_VERSION < 105800)
	{
		typedef
			yggr::safe_container::safe_options_set
			<
				int,
				boost::mutex,
				std::less<int>,
				boost::container::new_allocator<int>
			> set_type;
		debug_test<set_type>();
	}
#endif // !(BOOST_VERSION < 105800)

	std::srand(std::time(0));

	// mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_options_set<val_type> set_type;
		test_thread_safe_of_mutex<set_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_options_set
			<
				val_type,
				boost::mutex,
				std::less<val_type>,
				std::allocator<val_type>
			> set_type;
		test_thread_safe_of_mutex<set_type>();
	}

#if !(BOOST_VERSION < 105800)
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_options_set
			<
				val_type,
				boost::mutex,
				std::less<val_type>,
				boost::container::new_allocator<val_type>
			> set_type;
		test_thread_safe_of_mutex<set_type>();
	}
#endif // !(BOOST_VERSION < 105800)

	// shared_mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_options_set<val_type, boost::shared_mutex> set_type;
		test_thread_safe_of_shared_mutex<set_type>();
	}

	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_options_set
			<
				val_type,
				boost::shared_mutex,
				std::less<val_type>,
				std::allocator<val_type>
			> set_type;
		test_thread_safe_of_shared_mutex<set_type>();
	}

#if !(BOOST_VERSION < 105800)
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_options_set
			<
				val_type,
				boost::shared_mutex,
				std::less<val_type>,
				boost::container::new_allocator<val_type>
			> set_type;
		test_thread_safe_of_shared_mutex<set_type>();
	}
#endif // !(BOOST_VERSION < 105800)

	wait_any_key(argc, argv);
	return 0;
}
