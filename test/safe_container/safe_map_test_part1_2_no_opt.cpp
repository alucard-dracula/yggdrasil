//safe_map_test.cpp

#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/utility/pair_compare.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/mplex/null_t.hpp>

#include <yggr/safe_container/safe_map.hpp>
#include <yggr/safe_container/safe_options_map.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <cassert>
#include <functional>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(base)

#define THREAD_TEST_USING_MANUAL() 0

#define THREAD_TEST_THREAD_SIZE() 10
#define THREAD_TEST_THREAD_LOOP_SIZE() 10

//-------------------------test base-------------------------

//# if(YGGR_CPP_VERSION < YGGR_CPP_VER_11)
//
//template<typename K1, typename V1, typename K2, typename V2> inline
//bool operator==(const std::pair<K1, V1>& l, const std::pair<K2, V2>& r)
//{
//	return l.first == r.first && l.second == r.second;
//}
//
//#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename Cont>
void check_range_equal(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
	//typename map_type::size_type size = l.size();

	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		yggr_test_assert(i->first == i->first && i->second == i->second);
	}
}

template<typename Cont>
void test_constructor(void)
{
	typedef Cont map_type;

	map_type cont1;
	yggr_test_assert(cont1.empty());

	map_type cont2(cont1.key_comp());
	yggr_test_assert(cont2.empty());

	map_type cont3(cont1.get_allocator());
	yggr_test_assert(cont3.empty());

	map_type cont4(cont3.key_comp(), cont2.get_allocator());
	yggr_test_assert(cont4.empty());

	typename map_type::base_type base_cont;
	base_cont.insert(typename map_type::value_type(1, 1));
	base_cont.insert(typename map_type::value_type(2, 2));
	base_cont.insert(typename map_type::value_type(3, 3));
	base_cont.insert(typename map_type::value_type(4, 4));
	base_cont.insert(typename map_type::value_type(5, 5));
	base_cont.insert(typename map_type::value_type(0, 0));
	map_type cont5(base_cont.begin(), base_cont.end());
	yggr_test_assert(cont5.size() == 6);

	map_type cont6(base_cont.begin(), base_cont.end(), cont5.key_comp());
	yggr_test_assert(cont6.size() == 6);

	map_type cont7(base_cont.begin(), base_cont.end(), cont5.get_allocator());
	yggr_test_assert(cont7.size() == 6);

	map_type cont8(base_cont.begin(), base_cont.end(), cont5.key_comp(), cont5.get_allocator());
	yggr_test_assert(cont8.size() == 6);

	typename map_type::base_type base_cont9(base_cont.begin(), base_cont.end());
	map_type cont9(boost::move(base_cont9));
	yggr_test_assert(cont9.size() == 6);

	typename map_type::base_type base_cont10(base_cont.begin(), base_cont.end());
	map_type cont10(base_cont10);
	yggr_test_assert(cont10.size() == 6 && base_cont10.size() == 6);
	cont10.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont10)));

	typename map_type::base_type base_cont11(base_cont.begin(), base_cont.end());
	map_type cont11(boost::move(base_cont11), base_cont11.get_allocator());
	yggr_test_assert(cont11.size() == 6);

	typename map_type::base_type base_cont12(base_cont.begin(), base_cont.end());
	map_type cont12(base_cont12, base_cont12.get_allocator());
	yggr_test_assert(cont12.size() == 6 && base_cont12.size() == 6);
	cont12.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont12)));

	{
		typename map_type::base_type base_cont13(base_cont.begin(), base_cont.end());
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

	{
		map_type cont11(base_cont);
		typename map_type::base_type base_cont11 = cont11;

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

		map_type cont12(cont11);

		// safe_copy_safe
		yggr_test_assert(cont11 == cont12);
		yggr_test_assert(!(cont11 != cont12));
		yggr_test_assert(cont11 <= cont12);
		yggr_test_assert(!(cont11 < cont12));
		yggr_test_assert(cont11 >= cont12);
		yggr_test_assert(!(cont11 > cont12));
	}

	std::cout << "---------------test_constructor end-----------------" << std::endl;
}

template<typename Cont>
void test_capacity(void)
{
	typedef Cont map_type;

	typename map_type::base_type base_cont;
	base_cont.insert(typename map_type::value_type(1, 1));
	base_cont.insert(typename map_type::value_type(2, 2));
	base_cont.insert(typename map_type::value_type(3, 3));
	base_cont.insert(typename map_type::value_type(4, 4));
	base_cont.insert(typename map_type::value_type(5, 5));
	base_cont.insert(typename map_type::value_type(0, 0));

	map_type cont1(base_cont.begin(), base_cont.end());
	typename map_type::base_type base_cont1(base_cont.begin(), base_cont.end());

	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.max_size() == base_cont1.max_size());

	yggr_test_assert(cont1.empty() == base_cont1.empty());

	std::cout << "---------------test_capacity end-----------------" << std::endl;
}

template<typename Cont>
void test_element_access(void)
{
	typedef Cont map_type;

	typename map_type::base_type base_cont;
	base_cont.insert(typename map_type::value_type(1, 1));
	base_cont.insert(typename map_type::value_type(2, 2));
	base_cont.insert(typename map_type::value_type(3, 3));
	base_cont.insert(typename map_type::value_type(4, 4));
	base_cont.insert(typename map_type::value_type(5, 5));
	base_cont.insert(typename map_type::value_type(0, 0));

	map_type cont(base_cont.begin(), base_cont.end());
	yggr_test_assert(cont.set_value(5, 100));
	yggr_test_assert(cont.set_value(200, 200));


	cont[6] = 60;
	yggr_test_assert(cont[6] == 60);
	try
	{
		cont.at(7) = 70;
		yggr_test_assert(false);
	}
	catch(const typename map_type::error_type&) {}

	try
	{
		cont[7] = 70;
	}
	catch(const typename map_type::error_type&)
	{
		yggr_test_assert(false);
	}

	yggr_test_assert(cont.at(7) == 70);

	std::cout << "---------------test_element_access end-----------------" << std::endl;
}

template<typename Cont>
void check_last_element_pair(typename Cont::base_type&,
								const std::pair<typename Cont::iterator, bool>& ret,
								int check)
{
	yggr_test_assert(ret.first->second == check);
}

template<typename Cont>
void check_last_element_iter(typename Cont::base_type& base,
								const std::pair<typename Cont::iterator, bool>& pos_pair,
								int check)
{
	yggr_test_assert(pos_pair.second);
	yggr_test_assert(pos_pair.first != base.end());
	yggr_test_assert(pos_pair.first->second == check);
}

//template<typename Cont>
//bool check_error(Cont& map, const typename Cont::value_type& val)
//{
//	return true;
//}

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
bool ins_handler_zwei(typename Cont::base_type& base,
						const std::pair<typename Cont::iterator, bool>& pair,
						const typename Cont::value_type& chk_val)
{
	yggr_test_assert(chk_val == *(pair.first));
	return pair.second && (pair.first != base.end());
}

template<typename T> inline
T make_rvalue(const T& t)
{
	return t;
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

template<typename Cont>
void test_modifiers(void)
{
	typedef Cont map_type;
	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;

	typedef iter_type (base_map_type::* begin_end_foo_type)(void);
	typedef citer_type (base_map_type::* cbegin_end_foo_type)(void) const;

	map_type cont;
	typename map_type::base_type base_cont;

	{
		cont.insert(100, 100);
		base_cont.insert(typename map_type::value_type(100, 100));

		int kn = 90, n = 90;
		cont.insert(kn, boost::move(n));
		base_cont.insert(typename map_type::value_type(kn, 90));

		cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));

		cont.insert(200, 200, boost::bind(&check_last_element_pair<map_type>, _1, _2, 200));
		cont.insert(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::end), _1),
					typename map_type::value_type(200, 200),
					boost::bind(&check_last_element_iter<map_type>, _1, _2, 200));

		base_cont.insert(typename map_type::value_type(200, 200));

		int km = 300, m = 300;
		cont.insert(km, boost::move(m), boost::bind(&check_last_element_pair<map_type>, _1, _2, 300));
		base_cont.insert(typename map_type::value_type(km, 300));

		yggr_test_assert(cont.size() == base_cont.size());
	}

	{
		cont.insert(typename map_type::value_type(1000, 1000));
		base_cont.insert(typename map_type::value_type(1000, 1000));

		int kn = 900, n = 900;
		typename map_type::value_type v(kn, n);
		cont.insert(boost::move(v));
		base_cont.insert(typename map_type::value_type(kn, 900));

		cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));

		cont.insert(typename map_type::value_type(2000, 2000), boost::bind(&check_last_element_pair<map_type>, _1, _2, 2000));
		base_cont.insert(typename map_type::value_type(2000, 2000));

		int km = 3000, m = 3000;
		typename map_type::value_type v2(km, m);
		cont.insert(boost::move(v2), boost::bind(&check_last_element_pair<map_type>, _1, _2, 3000));
		base_cont.insert(typename map_type::value_type(km, 3000));

		yggr_test_assert(cont.size() == base_cont.size());
	}

	cont.set_value(200, 5000);
	int sn = 6000;
	cont.set_value(200, boost::move(sn));

	cont.set_value(typename map_type::value_type(200, 200));
	cont.set_value(typename map_type::value_type(9000, 9000));
	base_cont.insert(typename map_type::value_type(9000, 9000));

	yggr_test_assert(cont.size() == base_cont.size());

	cont.erase(200);
	base_cont.erase(200);

	yggr_test_assert(cont.size() == base_cont.size());

	yggr_test_assert(!cont.replace(200, 500));
	yggr_test_assert(cont.replace(100, 1000));
	yggr_test_assert(cont.replace(typename map_type::value_type(100, 100)));

	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));
	cont.swap(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));

	typename map_type::base_type base_cont2(base_cont);

	cont.swap(boost::move(base_cont2));
	yggr_test_assert(!cont.empty() && !base_cont2.empty());
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));

	map_type cont3(base_cont);

	cont.swap(cont3);
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));
	cont3.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));

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
		bool bchk = false;

		map_type ins_cont;
		bchk = ins_cont.emplace(yggr::non_handler);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
		yggr_test_assert(ins_cont.contains(0));

		bchk = ins_cont.try_emplace(yggr::non_handler, 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
		yggr_test_assert(ins_cont.contains(0));

		bchk = ins_cont.try_emplace(yggr::non_handler, 1);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.find(1));
		yggr_test_assert(ins_cont[1] == 0);
		yggr_test_assert(ins_cont.contains(1));
	}

	{
		bool bchk = false;

		map_type ins_cont;
		bchk = ins_cont.emplace(yggr::non_handler, 10, 10);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));

		bchk = ins_cont.try_emplace(yggr::non_handler, 10, 10);
		yggr_test_assert(!bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
		yggr_test_assert(ins_cont.contains(10));

		bchk = ins_cont.try_emplace(yggr::non_handler, 11, 11);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.find(11));
		yggr_test_assert(ins_cont[11] == 11);
		yggr_test_assert(ins_cont.contains(11));
	}

	{
		bool bchk = false;

		map_type ins_cont;
		yggr_test_assert(ins_cont.emplace(boost::bind(&ins_handler<map_type>, _1, _2, std::make_pair(0, 0))));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));

		bchk = ins_cont.try_emplace(boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(0, 0)), 0);
		yggr_test_assert(!bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
		yggr_test_assert(ins_cont.contains(0));

		bchk = ins_cont.try_emplace(boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(1, 0)), 1);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.find(1));
		yggr_test_assert(ins_cont[1] == 0);
		yggr_test_assert(ins_cont.contains(1));
	}

	{
		bool bchk = false;

		map_type ins_cont;
		yggr_test_assert(ins_cont.emplace(boost::bind(&ins_handler<map_type>, _1, _2, std::make_pair(10, 10)), 10, 10));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));

		bchk = ins_cont.try_emplace(boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(10, 10)), 10, 10);
		yggr_test_assert(!bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
		yggr_test_assert(ins_cont.contains(10));

		bchk = ins_cont.try_emplace(boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(11, 11)), 11, 11);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.find(11));
		yggr_test_assert(ins_cont[11] == 11);
		yggr_test_assert(ins_cont.contains(11));
	}

	{
		bool bchk = false;

		map_type ins_cont;
		yggr_test_assert(ins_cont.emplace(boost::bind(&ins_handler<map_type>, _1, _2, std::make_pair(10, 10)), make_rvalue(10), make_rvalue(10)));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));

		bchk = ins_cont.try_emplace(boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(10, 10)), make_rvalue(10), make_rvalue(10));
		yggr_test_assert(!bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
		yggr_test_assert(ins_cont.contains(10));

		bchk = ins_cont.try_emplace(boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(11, 11)), make_rvalue(11), make_rvalue(11));
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.find(11));
		yggr_test_assert(ins_cont[11] == 11);
		yggr_test_assert(ins_cont.contains(11));
	}

	{
		bool bchk = false;

		map_type ins_cont;
		yggr_test_assert(ins_cont.emplace_hint(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1), yggr::non_handler));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));

		bchk = ins_cont.try_emplace(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				yggr::non_handler,
				0);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
		yggr_test_assert(ins_cont.contains(0));

		bchk = ins_cont.try_emplace(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				yggr::non_handler,
				1);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.find(1));
		yggr_test_assert(ins_cont[1] == 0);
		yggr_test_assert(ins_cont.contains(1));
	}

	{
		bool bchk = false;

		map_type ins_cont;
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
						yggr::non_handler, std::make_pair(10, 10)));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));

		bchk = ins_cont.try_emplace(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				yggr::non_handler,
				10, 10);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
		yggr_test_assert(ins_cont.contains(10));

		bchk = ins_cont.try_emplace(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				yggr::non_handler,
				11, 11);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.find(11));
		yggr_test_assert(ins_cont[11] == 11);
		yggr_test_assert(ins_cont.contains(11));
	}

	{
		bool bchk = false;

		map_type ins_cont;
		yggr_test_assert(ins_cont.empty());
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
						boost::bind(&ins_handler<map_type>, _1, _2, std::make_pair(0, 0))));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));

		bchk = ins_cont.try_emplace(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(0, 0)),
				0);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));
		yggr_test_assert(ins_cont.contains(0));

		bchk = ins_cont.try_emplace(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(1, 0)),
				1);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.find(1));
		yggr_test_assert(ins_cont[1] == 0);
		yggr_test_assert(ins_cont.contains(1));
	}

	{
		bool bchk = false;

		map_type ins_cont;
		yggr_test_assert(ins_cont.emplace_hint(
						boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
						boost::bind(&ins_handler<map_type>, _1, _2, std::make_pair(10, 10)), std::make_pair(10, 10)));
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));

		bchk = ins_cont.try_emplace(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(10, 10)),
				10, 10);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(10));
		yggr_test_assert(ins_cont.contains(10));

		bchk = ins_cont.try_emplace(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(11, 11)),
				11, 11);
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont.find(11));
		yggr_test_assert(ins_cont[11] == 11);
		yggr_test_assert(ins_cont.contains(11));
	}

	//insert_or_assign
	{
		bool bchk = false;

		map_type ins_cont;

		bchk = ins_cont.insert_or_assign(make_rvalue(10), make_rvalue(10));
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont[10] == 10);

		bchk = ins_cont.insert_or_assign(10, make_rvalue(100));
		yggr_test_assert(!bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont[10] == 100);

		bchk = ins_cont.insert_or_assign(
				make_rvalue(20), make_rvalue(20),
				boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(20, 20)));
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont[20] == 20);

		bchk = ins_cont.insert_or_assign(
				20, make_rvalue(200),
				boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(20, 200)));
		yggr_test_assert(!bchk);
		yggr_test_assert(ins_cont.size() == 2);
		yggr_test_assert(ins_cont[20] == 200);

		bchk = ins_cont.insert_or_assign(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				make_rvalue(30), make_rvalue(30));
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 3);
		yggr_test_assert(ins_cont[30] == 30);

		bchk = ins_cont.insert_or_assign(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				30, make_rvalue(300));
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 3);
		yggr_test_assert(ins_cont[30] == 300);

		bchk = ins_cont.insert_or_assign(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				make_rvalue(40), make_rvalue(40),
				boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(40, 40)));
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 4);
		yggr_test_assert(ins_cont[40] == 40);

		bchk = ins_cont.insert_or_assign(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1),
				40, make_rvalue(400),
				boost::bind(&ins_handler_zwei<map_type>, _1, _2, std::make_pair(40, 400)));
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 4);
		yggr_test_assert(ins_cont[40] == 400);
	}

	cont.clear();
	yggr_test_assert(cont.empty());

	// merge
	{
		int arr1[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};
		int arr2[10] = {5, 5, 6, 6, 7, 7, 8, 8, 9, 9};
		// map
		{
			typedef std::map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef std::map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef boost::container::map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef boost::container::map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::options_map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::options_map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::safe_container::safe_map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::safe_container::safe_map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::safe_container::safe_options_map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::safe_container::safe_options_map<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			map_type cont_eins;
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(*i, *i * 10);
				}
			}

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			map_type cont_eins;
			{
				for(int *i = arr1, *isize = arr1 + 10; i != isize; ++i)
				{
					cont_eins.insert(*i, *i * 10);
				}
			}

			map_type cont_zwei;
			{
				for(int *i = arr2, *isize = arr2 + 10; i != isize; ++i)
				{
					cont_zwei.insert(*i, *i * 10);
				}
			}

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		// multiset
		{
			typedef std::multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef std::multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef boost::container::multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef boost::container::multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::options_multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::options_multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::safe_container::safe_multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::safe_container::safe_multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::safe_container::safe_options_multimap<int, int> now_map_type;

			map_type cont_eins;
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

			yggr_test_assert(cont_eins.size() == 5);
			yggr_test_assert(cont_zwei.size() == 10);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 6);
		}

		{
			typedef yggr::safe_container::safe_options_multimap<int, int> now_map_type;

			map_type cont_eins;
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
		map_type cont;
		{
			for(int *i = arr, *isize = arr + 5; i != isize; ++i)
			{
				cont.insert(*i, *i * 10);
			}
		}

		yggr_test_assert(cont.size() == 5);

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
typename
	boost::enable_if
	<
		boost::is_base_of<yggr::mplex::null_type, typename Cont::insert_return_type>,
		typename Cont::insert_return_type
	>::type
	ins_node_handler(typename Cont::base_type& base,
						typename Cont::base_insert_return_type& ins_ret)
{
	typedef Cont map_type;
	return typename map_type::insert_return_type();
}

template<typename Cont>
typename
	boost::disable_if
	<
		boost::is_base_of<yggr::mplex::null_type, typename Cont::insert_return_type>,
		typename Cont::insert_return_type
	>::type
	ins_node_handler(typename Cont::base_type& base,
						typename Cont::base_insert_return_type& ins_ret)
{
	typedef Cont map_type;
	return typename map_type::insert_return_type(boost::move(ins_ret));
}

template<typename Cont>
typename
	boost::enable_if
	<
		boost::is_base_of<yggr::mplex::null_type, typename Cont::insert_return_type>,
		bool
	>::type
	ins_hint_node_handler(typename Cont::base_type& base,
							const std::pair<typename Cont::iterator, bool>& pair)
{
	return false;
}

template<typename Cont>
typename
	boost::disable_if
	<
		boost::is_base_of<yggr::mplex::null_type, typename Cont::insert_return_type>,
		bool
	>::type
	ins_hint_node_handler(typename Cont::base_type& base,
							const std::pair<typename Cont::iterator, bool>& pair)
{
	return pair.second && pair.first != base.end();
}

template<typename Cont>
typename
	boost::enable_if
	<
		boost::is_base_of<yggr::mplex::null_type, typename Cont::insert_return_type>,
		void
	>::type
	test_extract_insert(void)
{
#if 0 // 1 test boost_mpl_assert_msg
	typedef Cont map_type;
	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;

	typedef iter_type (base_map_type::* begin_end_foo_type)(void);
	typedef citer_type (base_map_type::* cbegin_end_foo_type)(void) const;

	int arr[10] = {1, 2, 3, 4, 5};
	{
		map_type ins_cont;
		for(int *i = arr, *isize = arr + 5; i != isize; ++i)
		{
			ins_cont.insert(*i, *i * 10);
		}

		typename map_type::node_type node = ins_cont.extract(3);
		typename map_type::insert_return_type iret = ins_cont.insert(boost::move(node));
	}

	{
		map_type ins_cont;
		for(int *i = arr, *isize = arr + 5; i != isize; ++i)
		{
			ins_cont.insert(*i, *i * 10);
		}
		typename map_type::node_type node =
			ins_cont.extract(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1));

		typename map_type::insert_return_type iret =
			ins_cont.insert(
				boost::move(node),
				boost::bind(&ins_node_handler<map_type>, _1, _2));
	}

	{
		map_type ins_cont;
		for(int *i = arr, *isize = arr + 5; i != isize; ++i)
		{
			ins_cont.insert(*i, *i * 10);
		}
		typename map_type::node_type node = ins_cont.extract(3);

		bool iret =
			ins_cont.insert(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::end), _1),
				boost::move(node));
	}

	{
		map_type ins_cont;
		for(int *i = arr, *isize = arr + 5; i != isize; ++i)
		{
			ins_cont.insert(*i, *i * 10);
		}
		typename map_type::node_type node = ins_cont.extract(3);

		bool iret =
			ins_cont.insert(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::end), _1),
				boost::move(node),
				boost::bind(&ins_hint_node_handler<map_type>, _1, _2));
	}
#endif // 0, 1
	std::cout << "---------------not test_extract_insert end-----------------" << std::endl;
}

template<typename Cont>
typename
	boost::disable_if
	<
		boost::is_base_of<yggr::mplex::null_type, typename Cont::insert_return_type>,
		void
	>::type
	test_extract_insert(void)
{
	typedef Cont map_type;
	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;

	typedef iter_type (base_map_type::* begin_end_foo_type)(void);
	typedef citer_type (base_map_type::* cbegin_end_foo_type)(void) const;

	// extract insert

	int arr[10] = {1, 2, 3, 4, 5};
	{
		map_type ins_cont;
		for(int *i = arr, *isize = arr + 5; i != isize; ++i)
		{
			ins_cont.insert(*i, *i * 10);
		}

		typename map_type::node_type node = ins_cont.extract(3);
		yggr_test_assert(node.key() == 3);
		yggr_test_assert(node.mapped() == 30);
		yggr_test_assert(!ins_cont.find(3));
		yggr_test_assert(!ins_cont.contains(3));

		typename map_type::insert_return_type iret = ins_cont.insert(boost::move(node));
		yggr_test_assert(node.empty());
		yggr_test_assert(iret.node.empty());
		yggr_test_assert(iret.inserted);

		yggr_test_assert(ins_cont.find(3));
	}

	{
		map_type ins_cont;
		for(int *i = arr, *isize = arr + 5; i != isize; ++i)
		{
			ins_cont.insert(*i, *i * 10);
		}
		typename map_type::node_type node =
			ins_cont.extract(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1));
		yggr_test_assert(node.key() == 1);
		yggr_test_assert(node.mapped() == 10);
		yggr_test_assert(!ins_cont.find(1));

		typename map_type::insert_return_type iret =
			ins_cont.insert(
				boost::move(node),
				boost::bind(&ins_node_handler<map_type>, _1, _2));
		yggr_test_assert(node.empty());
		yggr_test_assert(iret.node.empty());
		yggr_test_assert(iret.inserted);

		yggr_test_assert(ins_cont.find(1));
	}

	{
		map_type ins_cont;
		for(int *i = arr, *isize = arr + 5; i != isize; ++i)
		{
			ins_cont.insert(*i, *i * 10);
		}
		typename map_type::node_type node = ins_cont.extract(3);
		yggr_test_assert(node.key() == 3);
		yggr_test_assert(node.mapped() == 30);
		yggr_test_assert(!ins_cont.find(3));

		bool iret =
			ins_cont.insert(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::end), _1),
				boost::move(node));
		yggr_test_assert(node.empty());
		yggr_test_assert(iret);

		yggr_test_assert(ins_cont.find(3));
	}

	{
		map_type ins_cont;
		for(int *i = arr, *isize = arr + 5; i != isize; ++i)
		{
			ins_cont.insert(*i, *i * 10);
		}
		typename map_type::node_type node = ins_cont.extract(3);
		yggr_test_assert(node.key() == 3);
		yggr_test_assert(node.mapped() == 30);
		yggr_test_assert(!ins_cont.find(3));

		bool iret =
			ins_cont.insert(
				boost::bind(static_cast<begin_end_foo_type>(&base_map_type::end), _1),
				boost::move(node),
				boost::bind(&ins_hint_node_handler<map_type>, _1, _2));
		yggr_test_assert(node.empty());
		yggr_test_assert(iret);

		yggr_test_assert(ins_cont.find(3));
	}

	std::cout << "---------------test_extract_insert end-----------------" << std::endl;
}

template<typename Cont>
void test_safe_element_reference(void)
{
	typedef Cont map_type;

	std::pair<int, int> arr1[10] = {std::make_pair(10, 10),
									std::make_pair(11, 11),
									std::make_pair(12, 12)};

	std::pair<int, int> arr2[10] = {std::make_pair(20, 20),
									std::make_pair(21, 21),
									std::make_pair(22, 22)};

	map_type cont1(arr1 + 0, arr1 + 10);
	map_type cont2(arr2 + 0, arr2 + 10);

#define TEST_OPERATOR( __op__ ) \
	a = cont1[10] __op__ cont1[10]; \
	yggr_test_assert(a == (arr1[0].second __op__ arr1[0].second)); \
	a = cont1[10] __op__ cont2[20]; \
	yggr_test_assert(a == (arr1[0].second __op__ arr2[0].second)); \
	\
	a = cont1.at(10) __op__ cont1.at(10); \
	yggr_test_assert(a == (arr1[0].second __op__ arr1[0].second)); \
	a = cont1.at(10) __op__ cont2.at(20); \
	yggr_test_assert(a == (arr1[0].second __op__ arr2[0].second));


	{
		int a = 0;
		{
			a = cont1[10] + cont1[10];
			yggr_test_assert(a == arr1[0].second + arr1[0].second);

			a = cont1[10] + cont2[20];
			yggr_test_assert(a == arr1[0].second + arr2[0].second);

			a = cont1.at(10) + cont1.at(10);
			yggr_test_assert(a == (arr1[0].second + arr1[0].second));

			a = cont1.at(10) + cont2.at(20);
			yggr_test_assert(a == (arr1[0].second + arr2[0].second));
		}

		{
			a = cont1[10] - cont1[10];
			yggr_test_assert(a == arr1[0].second - arr1[0].second);

			a = cont1[10] - cont2[20];
			yggr_test_assert(a == arr1[0].second - arr2[0].second);

			a = cont1.at(10) - cont1.at(10);
			yggr_test_assert(a == (arr1[0].second - arr1[0].second));

			a = cont1.at(10) - cont2.at(20);
			yggr_test_assert(a == (arr1[0].second - arr2[0].second));
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
		cont1[5] __op__ cont1[10]; \
		yggr_test_assert(cont1[5] == (a __op__ cont1[10])); \
		yggr_test_assert(cont1[5] == (b __op__ arr1[0].second)); \
		cont1[5] = 0; \
		a = 0; b = 0; \
		\
		cont1[5] __op__ cont2[20]; \
		yggr_test_assert(cont1[5] == (a __op__ cont2[20])); \
		yggr_test_assert(cont1[5] == (b __op__ arr2[0].second)); \
		cont1[5] = 0; \
		a = 0; b = 0; \
		\
		cont1.at(5) __op__ cont1.at(10); \
		yggr_test_assert(cont1.at(5) == (a __op__ cont1.at(10))); \
		yggr_test_assert(cont1.at(5) == (b __op__ arr1[0].second)); \
		cont1[5] = 0; \
		a = 0; b = 0; \
		\
		cont1.at(5) __op__ cont2.at(20); \
		yggr_test_assert(cont1.at(5) == (a __op__ cont2.at(20))); \
		yggr_test_assert(cont1.at(5) == (b __op__ arr2[0].second)); \
		cont1[5] = 0; \
		a = 0; b = 0;

	{
		int a = 0;
		int b = 0;

		cont1[5] = cont1[10];
		yggr_test_assert(cont1[5] == cont1[10]);
		yggr_test_assert(cont1[5] == arr1[0].second);
		cont1[5] = 0;

		cont1[5] = cont2[20];
		yggr_test_assert(cont1[5] == cont2[20]);
		yggr_test_assert(cont1[5] == arr2[0].second);
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
		cont1[5] = +cont1[10];
		yggr_test_assert(cont1[5] == cont1[10]);

		cont1[5] = +cont2[20];
		yggr_test_assert(cont1[5] == cont2[20]);

		// -
		cont1[5] = -cont1[10];
		yggr_test_assert(cont1[5] == -cont1[10]);

		cont1[5] = -cont2[20];
		yggr_test_assert(cont1[5] == -cont2[20]);

		// ++
		cont1[5] = ++cont1[11];
		yggr_test_assert(cont1[5] == cont1[11]);

		cont1[5] = ++cont2[21];
		yggr_test_assert(cont1[5] == cont2[21]);

		cont1[5] = cont1[11]++;
		yggr_test_assert(cont1[5] == cont1[11] - 1);

		cont1[5] = cont2[21]++;
		yggr_test_assert(cont1[5] == cont2[21] - 1);

		// --
		cont1[5] = --cont1[11];
		yggr_test_assert(cont1[5] == cont1[11]);

		cont1[5] = --cont2[21];
		yggr_test_assert(cont1[5] == cont2[21]);

		cont1[5] = cont1[11]--;
		yggr_test_assert(cont1[5] == cont1[11] + 1);

		cont1[5] = cont2[21]--;
		yggr_test_assert(cont1[5] == cont2[21] + 1);

		// ~
		cont1[5] = ~cont1[10];
		yggr_test_assert(cont1[5] == ~cont1[10]);

		cont1[5] = ~cont2[20];
		yggr_test_assert(cont1[5] == ~cont2[20]);
	}

	{
		yggr_test_assert(cont1[10] && cont1[11]);
		yggr_test_assert(cont1[10] && cont2[21]);

		yggr_test_assert(cont1[10] || cont1[8]);
		yggr_test_assert(cont1[10] || cont2[8]);

		yggr_test_assert(!(cont1[8] || cont1[8]));
		yggr_test_assert(!(cont1[8] || cont2[8]));

		yggr_test_assert(!cont1[8]);
		yggr_test_assert(cont1[10]);
	}

	{
		yggr_test_assert(cont1[10] < cont1[11]);
		yggr_test_assert(cont1[10] < cont2[21]);

		yggr_test_assert(cont1[10] <= cont1[10]);
		yggr_test_assert(cont1[10] <= cont2[20]);

		yggr_test_assert(cont1[10] > cont1[8]);
		yggr_test_assert(cont1[10] > cont2[8]);

		yggr_test_assert(cont1[10] >= cont1[10]);
		yggr_test_assert(cont1[10] >= cont2[8]);

		yggr_test_assert(cont1[10] == cont1[10]);
		yggr_test_assert(cont1[10] != cont2[8]);
	}

	// mix
	{
		{
			int num = (cont1[5] = cont1[8] + cont2[20] + cont2[21] * cont1[13]);
			yggr_test_assert(num == (arr1[8].second + arr2[0].second + arr2[1].second * arr1[3].second));
		}

		{
			int num = (cont1[5] = 1 + cont1[10] + cont2[21] * 3);
			yggr_test_assert(num == (1 + arr1[0].second + arr2[1].second * 3));
		}

		{
			int num = cont1[10] + cont1.at(11);
			yggr_test_assert(num == arr1[0].second + arr1[1].second);
		}

		{
			int num = cont1[10] + cont2.at(20);
			yggr_test_assert(num == arr1[0].second + arr2[0].second);
		}

		{
			int num = cont1.at(11) + cont1[10];
			yggr_test_assert(num == arr1[0].second + arr1[1].second);
		}

		{
			int num = cont2.at(20) + cont1[10];
			yggr_test_assert(num == arr1[0].second + arr2[0].second);
		}
	}

	{

		std::pair<float, float> arr1[10] = {std::make_pair(10, 10),
											std::make_pair(11, 11),
											std::make_pair(12, 12)};

		typedef yggr::safe_container::safe_map<float, float> fmap_type;

		fmap_type cont3(arr1 + 0, arr1 + 10);
		cont1[5] = cont3[10];

		cont1[5] += cont3[10];
		yggr_test_assert(cont1[5] == cont3[10] * 2);

		float f = yggr::safe_container::safe_element_reference_unary_operator(
					boost::bind(&yggr::any_val::operator_wrap::negate<float>, _1),
					cont1[5]);
		yggr_test_assert(f == -cont1[5]);

		float f2 = yggr::safe_container::safe_element_reference_binary_operator(
					boost::bind(&yggr::any_val::operator_wrap::plus<float, float>, _1, _2),
					cont1[5], cont1[5]);
		yggr_test_assert(f2 == cont1[5] + cont1[5]);
	}

	{
		typedef yggr::safe_container::safe_map<int, yggr::shared_ptr<int> > ptr_vt_type;

		ptr_vt_type cont3;
		cont3.insert(1, yggr::shared_ptr<int>(yggr_nothrow_new int(1)));

		yggr_test_assert(*cont3[1] == 1);
		yggr::shared_ptr<int> ptr = cont3[1];
		*ptr = 100;
		yggr_test_assert(*cont3.at(1) == 100);

		// error left operand must be l-value, because operator* is_read_only
		//yggr_test_assert(*cont3.at(1) = 100);

	}

	std::cout << "---------------test_safe_element_reference end-----------------" << std::endl;
}

template<typename Cont>
void test_safe_other(void)
{
	typedef Cont map_type;

	typename map_type::base_type base_cont;
	base_cont.insert(typename map_type::value_type(1, 1));
	base_cont.insert(typename map_type::value_type(2, 2));
	base_cont.insert(typename map_type::value_type(3, 3));
	base_cont.insert(typename map_type::value_type(4, 4));
	base_cont.insert(typename map_type::value_type(5, 5));
	base_cont.insert(typename map_type::value_type(0, 0));

	map_type cont(base_cont.begin(), base_cont.end());
	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(cont.is_exists(0));
	yggr_test_assert(!cont.is_exists(100));

	typename map_type::base_type base_cont2 = cont.load();
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont2)));

	typename map_type::base_type base_cont3 = cont;
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont3)));

	cont.store(base_cont);
	cont.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont)));

	map_type empty_map;

	std::pair<typename map_type::key_type, typename map_type::mapped_type> beg_val, rbeg_val;
	typename map_type::key_type beg_key = 0, rbeg_key = 0;
	typename map_type::mapped_type beg_mapped = 0, rbeg_mapped = 0;

	yggr_test_assert(!empty_map.get_begin_value(beg_val));
	yggr_test_assert(!empty_map.get_rbegin_value(rbeg_val));

	yggr_test_assert(!empty_map.get_begin_key(beg_key));
	yggr_test_assert(!empty_map.get_rbegin_key(rbeg_key));

	yggr_test_assert(!empty_map.get_begin_mapped(beg_mapped));
	yggr_test_assert(!empty_map.get_rbegin_mapped(rbeg_mapped));

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
		empty_map.rbegin_value();
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
		empty_map.rbegin_key();
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

	try
	{
		empty_map.rbegin_mapped();
		yggr_test_assert(false);
	}
	catch(const typename map_type::error_type&)
	{
	}

	cont.get_begin_value(beg_val);
	cont.get_rbegin_value(rbeg_val);

	yggr_test_assert(beg_val == cont.begin_value());
	yggr_test_assert(rbeg_val == cont.rbegin_value());

	cont.get_begin_key(beg_key);
	cont.get_rbegin_key(rbeg_key);

	yggr_test_assert(beg_key == cont.begin_key());
	yggr_test_assert(rbeg_key == cont.rbegin_key());

	cont.get_begin_mapped(beg_mapped);
	cont.get_rbegin_mapped(rbeg_mapped);

	yggr_test_assert(beg_mapped == cont.begin_mapped());
	yggr_test_assert(rbeg_mapped == cont.rbegin_mapped());

	yggr_test_assert(beg_key == 0 && beg_mapped == 0);
	yggr_test_assert(rbeg_key == 5 && rbeg_mapped == 5);

	std::cout << "---------------test_safe_other end-----------------" << std::endl;
}

template<typename Cont>
void test_swap(void)
{
	typedef Cont map_type;

	typename map_type::base_type init_cont1;
	init_cont1.insert(typename map_type::value_type(1, 1));
	init_cont1.insert(typename map_type::value_type(2, 2));
	init_cont1.insert(typename map_type::value_type(3, 3));
	init_cont1.insert(typename map_type::value_type(4, 4));
	init_cont1.insert(typename map_type::value_type(5, 5));

	typename map_type::base_type init_cont2;
	init_cont2.insert(typename map_type::value_type(6, 6));
	init_cont2.insert(typename map_type::value_type(7, 7));
	init_cont2.insert(typename map_type::value_type(8, 8));
	init_cont2.insert(typename map_type::value_type(9, 9));
	init_cont2.insert(typename map_type::value_type(0, 0));

	map_type cont1(init_cont1.begin(), init_cont1.end());
	map_type cont2(init_cont2.begin(), init_cont2.end());

	typename map_type::base_type base_cont1(init_cont1.begin(), init_cont1.end());
	typename map_type::base_type base_cont2(init_cont2.begin(), init_cont2.end());
	yggr_test_assert(cont1.size() == base_cont1.size());
	yggr_test_assert(cont1.size() == base_cont2.size());


	typename map_type::base_type base_cont3(base_cont1);
	typename map_type::base_type base_cont4(base_cont2);

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
	iter->second = i->second;
}

template<typename Iter, typename Cont>
void cref_all_check_handler(Iter iter, const typename Cont::base_type& r)
{
	typedef Cont map_type;
	typename map_type::const_iterator i = r.find(iter->first);
	yggr_test_assert(i != r.end());
}

template<typename Cont>
void check_handler_of_val_eins(const typename Cont::base_type& base,
								const typename Cont::const_iterator& iter,
								const typename Cont::mapped_type& val)
{
	yggr_test_assert(iter != base.end());
	yggr_test_assert(iter->second == val);
}

template<typename Cont>
void check_handler_eins(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
	//typename map_type::size_type size = l.size();

	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		yggr_test_assert(*i == *j);
	}
}

template<typename Cont, typename Handler>
void check_handler_zwei(const typename Cont::base_type& l, const typename Cont::base_type& r, const Handler& ret_handler)
{
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
	//typename map_type::size_type size = l.size();

	yggr_test_assert(l.size() == r.size());
	for(iter_type i = l.begin(), isize = l.end(), j = r.begin(), jsize = r.end(); i != isize; ++i, ++j)
	{
		yggr_test_assert(*i == *j);
	}
	ret_handler();
}

template<typename Cont>
void set_handler_of_val_eins(typename Cont::base_type& base,
								const typename Cont::iterator& iter,
								const typename Cont::mapped_type& val)
{
	yggr_test_assert(iter != base.end());
	yggr_test_assert(iter->second != val);
	iter->second = val;
}

//template<typename Handler>
//void set_handler_of_val_zwei(map_type::iterator& iter, const map_type::mapped_type& val, const Handler& ret_handler)
//{
//	yggr_test_assert(iter->second != val);
//	iter->second = val;
//	ret_handler();
//}

template<typename Cont>
void set_handler_eins(typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
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
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
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

template<typename Cont>
void test_using_handler(void)
{
	typedef Cont map_type;

	typename map_type::base_type init_cont1;
	init_cont1.insert(typename map_type::value_type(1, 1));
	init_cont1.insert(typename map_type::value_type(2, 2));
	init_cont1.insert(typename map_type::value_type(3, 3));
	init_cont1.insert(typename map_type::value_type(4, 4));
	init_cont1.insert(typename map_type::value_type(5, 5));

	typename map_type::base_type init_cont2;
	init_cont2.insert(typename map_type::value_type(6, 6));
	init_cont2.insert(typename map_type::value_type(7, 7));
	init_cont2.insert(typename map_type::value_type(8, 8));
	init_cont2.insert(typename map_type::value_type(9, 9));
	init_cont2.insert(typename map_type::value_type(10, 10));

	typename map_type::base_type base_cont1(init_cont1.begin(), init_cont1.end());
	typename map_type::base_type base_cont2(init_cont2.begin(), init_cont2.end());

	map_type cont;
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

	ref_cont.for_each_iterator_reverse(boost::bind(&ref_all_check_handler<typename map_type::reverse_iterator, map_type>,
													_1, boost::cref(base_cont2)));
	cref_cont.for_each_iterator_reverse(boost::bind(&cref_all_check_handler<typename map_type::const_reverse_iterator, map_type>,
													_1, boost::cref(base_cont2)));

	ref_count = 0;
	ref_cont.for_each_iterator_reverse(boost::bind(&ref_all_check_handler<typename map_type::reverse_iterator, map_type>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<typename map_type::reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(ref_count)));
	yggr_test_assert(ref_count == 2);

	cref_count = 0;
	cref_cont.for_each_iterator_reverse(boost::bind(&cref_all_check_handler<typename map_type::const_reverse_iterator, map_type>,
													_1, boost::cref(base_cont2)),
										boost::bind(&cdt_handler<typename map_type::const_reverse_iterator, std::less<int> >,
														_1, 8, std::less<int>(), boost::ref(cref_count)));
	yggr_test_assert(cref_count == 2);

	ref_cont.find(7, boost::bind(&set_handler_of_val_eins<map_type>, _1, _2, 2000));
	cref_cont.find(7, boost::bind(&check_handler_of_val_eins<map_type>, _1, _2, 2000));

	std::cout << "---------------test_use_handler end-----------------" << std::endl;
}

template<typename Cont>
void debug_test(void)
{
	typedef Cont map_type;

	test_constructor<map_type>();
	test_capacity<map_type>();
	test_element_access<map_type>();
	test_modifiers<map_type>();
	test_extract_insert<map_type>();
	test_safe_element_reference<map_type>();
	test_safe_other<map_type>();
	test_swap<map_type>();
	test_using_handler<map_type>();
}

template<typename Cont>
void debug_test_std(void)
{
	typedef Cont map_type;

	test_constructor<map_type>();
	test_capacity<map_type>();
	test_element_access<map_type>();
	test_modifiers<map_type>();
#if !(YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	test_extract_insert<map_type>();
#endif // !(YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	test_safe_element_reference<map_type>();
	test_safe_other<map_type>();
	test_swap<map_type>();
	test_using_handler<map_type>();
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
bool push_handler(typename Container::base_type& base,
					const std::pair<typename Container::iterator, bool>& pair,
					int& count)
{
	if(pair.second)
	{
		++count;
	}

	return pair.second;
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
std::pair<int, Val> create_val(void)
{
	typedef Val val_type;
	typedef typename val_type::element_type elem_type;
	return std::pair<int, val_type>(std::rand() % 100, val_type(new elem_type(std::rand() % 100)));
}

template<typename Container>
void read_op(const Container& cont, hvt_type& hvt)
{
	typedef Container container_type;
	typedef typename container_type::base_type container_base_type;
	typedef typename container_type::mapped_type val_type;

	if(E_max_elem < hvt.size())
	{
		hvt.clear();
	}
	int op = std::rand() %  10;
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
	case 7: // using_handler const
		hvt.push_back(7 + E_read);
		cont.using_handler(boost::bind(&cref_handler<container_type>, _1));
		break;
	case 8: // get_value
		{
			hvt.push_back(8+ E_read);
			val_type p;
			cont.get_value(std::rand() % 100, p);
		}
		break;
	case 9:
		hvt.push_back(9 + E_read);
		cont.count(std::rand() % 100);
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
	typedef typename container_type::mapped_type val_type;
	typedef typename container_base_type::iterator iterator_type;
	typedef typename container_base_type::const_iterator const_iterator_type;

	typedef iterator_type (container_base_type::* begin_end_foo_type)(void);
	typedef const_iterator_type (container_base_type::* cbegin_end_foo_type)(void) const;

	if(E_max_elem < hvt.size())
	{
		hvt.clear();
	}

	int op = std::rand() % 15;
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

			typename container_type::value_type v1(create_val<val_type>());
			typename container_type::value_type v2(create_val<val_type>());
			typename container_type::value_type v3(create_val<val_type>());

			tmp.insert(v1);
			tmp.insert(v2);
			tmp.insert(v3);

			try
			{
				val_type val = cont[200];
				if(val)
				{
					*val += *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
				}
				else
				{
					typename val_type::element_type m = *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
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

			typename container_type::value_type v1(create_val<val_type>());
			typename container_type::value_type v2(create_val<val_type>());
			typename container_type::value_type v3(create_val<val_type>());

			tmp.insert(v1);
			tmp.insert(v2);
			tmp.insert(v3);

			try
			{
				val_type val = cont[200];
				if(val)
				{
					*val += *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
				}
				else
				{
					typename val_type::element_type m = *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
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
							boost::bind(&push_handler<container_type>, _1, _2, boost::ref(count)));
#else
            typename container_type::value_type val(create_val<val_type>());
            bool bret = cont.insert(boost::move(val),
							boost::bind(&push_handler<container_type>, _1, _2, boost::ref(count)));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
			yggr_test_assert((count == 1 && bret == true) || (count == 0 && bret == false));
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));
		}
		break;
	case 5: // // push_back(const val&, handler)
		{
			hvt.push_back(5 + E_write);
			int count = 0;
			bool bret =  cont.insert(create_val<val_type>(),
							boost::bind(&push_handler<container_type>, _1, _2, boost::ref(count)));
			yggr_test_assert((count == 1 && bret == true) || (count == 0 && bret == false));
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
			cont.erase(boost::bind(static_cast<begin_end_foo_type>(&container_base_type::begin), _1));

			typename container_type::value_type v1(create_val<val_type>());
			typename container_type::value_type v2(create_val<val_type>());
			typename container_type::value_type v3(create_val<val_type>());

			tmp.insert(v1);
			tmp.insert(v2);
			tmp.insert(v3);

			try
			{
				val_type val = cont[200];
				if(val)
				{
					*val += *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
				}
				else
				{
					typename val_type::element_type m = *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
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

			typename container_type::value_type v1(create_val<val_type>());
			typename container_type::value_type v2(create_val<val_type>());
			typename container_type::value_type v3(create_val<val_type>());

			tmp.insert(v1);
			tmp.insert(v2);
			tmp.insert(v3);

			try
			{
				val_type val = cont[200];
				if(val)
				{
					*val += *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
				}
				else
				{
					typename val_type::element_type m = *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
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

			typename container_type::value_type v1(create_val<val_type>());
			typename container_type::value_type v2(create_val<val_type>());
			typename container_type::value_type v3(create_val<val_type>());

			tmp.insert(v1);
			tmp.insert(v2);
			tmp.insert(v3);

			try
			{
				val_type val = cont[200];
				if(val)
				{
					*val += *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
				}
				else
				{
					typename val_type::element_type m = *cont.at(v1.first) + (*cont.at(v2.first)) * (*cont.at(v3.first));
					std::cout << m << std::endl;
				}
			}
			catch(const typename container_type::error_type&) {}
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
		break;
	case 12: //set_value(const &)
		hvt.push_back(12 + E_write);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		cont.set_value(boost::move(create_val<val_type>()));
#else
        {
            std::pair<int, val_type> val(create_val<val_type>());
            cont.set_value(boost::move(val));
        }
#endif  //YGGR_NO_CXX11_RVALUE_REFERENCES
	case 13: //set_value(const &)
		hvt.push_back(13 + E_write);
		cont.set_value(create_val<val_type>());
		break;
	case 14: // replace
		hvt.push_back(14 + E_write);
		cont.replace(create_val<val_type>());
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
		if(std::rand() %  2)
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

int main(int argc, char* argv[])
{
	//{
	//	typedef yggr::safe_container::safe_map<int, int> map_type;
	//	debug_test<map_type>();
	//}

	//{
	//	typedef
	//		yggr::safe_container::safe_map
	//		<
	//			int, int,
	//            boost::mutex,
	//            std::less<int>,
	//            std::allocator<std::pair<const int, int> >,
	//            std::map
	//		> map_type;
	//	debug_test_std<map_type>();
	//}

#if BOOST_VERSION < 105600
	{
		typedef
			yggr::safe_container::safe_map
			<
				int, int,
				boost::mutex,
				std::less<int>,
				std::allocator< std::pair<const int, int> >,
				boost::container::map
			> map_type;
		debug_test<map_type>();
	}
#endif // BOOST_VERSION < 105600

	{
		typedef
			yggr::safe_container::safe_map
			<
				int, int,
				boost::mutex,
				std::less<int>,
				std::allocator< std::pair<const int, int> >,
				yggr::map
			> map_type;
		debug_test<map_type>();
	}

#if !(BOOST_VERSION < 105800)
	{
		typedef
			yggr::safe_container::safe_map
			<
				int, int,
				boost::mutex,
				std::less<int>,
				boost::container::new_allocator< std::pair<const int, int> >,
				yggr::map
			> map_type;
		debug_test<map_type>();
	}

#endif // !(BOOST_VERSION < 105800)

	wait_any_key(argc, argv);
	return 0;
}
