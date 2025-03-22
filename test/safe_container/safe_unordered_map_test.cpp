//safe_unordered_map_test.cpp

//#define YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/utility/pair_compare.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <cassert>
#include <functional>
#include <utility> // test std::pair operator==

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
//#endif // # if(YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename Cont>
void check_range_equal(const typename Cont::base_type& l, const typename Cont::base_type& r)
{
	typedef Cont map_type;
	typedef typename map_type::const_iterator iter_type;
	//typename map_type::size_type size = l.size();
	yggr_test_assert(l.size() == r.size());
	bool bchk = false;
	for(iter_type i = l.begin(), isize = l.end(); i != isize; ++i)
	{
		bchk = (r.find(i->first) != r.end());
		yggr_test_assert(bchk);
	}
}

template<typename Cont>
void test_constructor(void)
{
	typedef Cont map_type;

	map_type cont1;
	yggr_test_assert(cont1.empty());

	map_type cont2(cont1.get_allocator());
	yggr_test_assert(cont2.empty());

	map_type cont3(cont1.key_eq(), cont1.get_allocator());
	yggr_test_assert(cont3.empty());

	map_type cont4(cont3.hash_function(), cont3.key_eq(), cont2.get_allocator());
	yggr_test_assert(cont4.empty());

	map_type cont4_2(5, cont3.hash_function(), cont3.key_eq(), cont2.get_allocator());
	yggr_test_assert(cont4_2.empty());

	typename map_type::base_type init_cont;
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));
	init_cont.insert(typename map_type::value_type(0, 0));

	map_type cont5(init_cont.begin(), init_cont.end());
	yggr_test_assert(cont5.size() == 6);

	map_type cont6(init_cont.begin(), init_cont.end(), cont5.get_allocator());
	yggr_test_assert(cont6.size() == 6);

	map_type cont7(init_cont.begin(), init_cont.end(), cont5.key_eq(), cont5.get_allocator());
	yggr_test_assert(cont7.size() == 6);

	map_type cont8(init_cont.begin(), init_cont.end(), cont5.hash_function(), cont5.key_eq(), cont5.get_allocator());
	yggr_test_assert(cont8.size() == 6);

	map_type cont8_2(init_cont.begin(), init_cont.end(), 5, cont5.hash_function(), cont5.key_eq(), cont5.get_allocator());
	yggr_test_assert(cont8_2.size() == 6);

	typename map_type::base_type base_cont9(init_cont.begin(), init_cont.end());
	map_type cont9(boost::move(base_cont9));
	yggr_test_assert(cont9.size() == 6);

	typename map_type::base_type base_cont10(init_cont.begin(), init_cont.end());
	map_type cont10(base_cont10);
	yggr_test_assert(cont10.size() == 6 && base_cont10.size() == 6);
	cont10.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont10)));

	typename map_type::base_type base_cont11(init_cont.begin(), init_cont.end());
	map_type cont11(boost::move(base_cont11), base_cont11.get_allocator());
	yggr_test_assert(cont11.size() == 6);

	typename map_type::base_type base_cont12(init_cont.begin(), init_cont.end());
	map_type cont12(base_cont12, base_cont12.get_allocator());
	yggr_test_assert(cont12.size() == 6 && base_cont12.size() == 6);
	cont12.using_handler(boost::bind(&check_range_equal<map_type>, _1, boost::cref(base_cont12)));

	{
		map_type cont11(init_cont.begin(), init_cont.end());
		typename map_type::base_type base_cont11 = cont11;

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
		typename map_type::base_type base_cont13(init_cont.begin(), init_cont.end());
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

template<typename Cont>
void test_capacity(void)
{
	typedef Cont map_type;

	typename map_type::base_type init_cont;
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));
	init_cont.insert(typename map_type::value_type(0, 0));

	map_type cont1(init_cont.begin(), init_cont.end());
	typename map_type::base_type base_cont1(init_cont.begin(), init_cont.end());

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
	base_cont.insert(typename map_type::value_type(0, 0));
	base_cont.insert(typename map_type::value_type(1, 1));
	base_cont.insert(typename map_type::value_type(2, 2));
	base_cont.insert(typename map_type::value_type(3, 3));
	base_cont.insert(typename map_type::value_type(4, 4));
	base_cont.insert(typename map_type::value_type(5, 5));

	map_type cont;
	cont.insert(base_cont.begin(), base_cont.end());

	yggr_test_assert(cont.set_value(5, 100));
	yggr_test_assert(cont.set_value(200, 200));

	{
		int val = 0;
		bool b = cont.get_value(5, val);
		yggr_test_assert(val == 100);
		yggr_test_assert(b);

		b = cont.get_value(10000, val);
		yggr_test_assert(!b);
	}

	cont[6] = 60;
	yggr_test_assert(cont[6] == 60);
	cont[7] = 60;
	cont.at(7) = 70;
	yggr_test_assert(cont.at(7) == 70);

	std::cout << "---------------test_element_access end-----------------" << std::endl;
}

template<typename Cont>
bool check_last_element(typename Cont::base_type& base,
							const std::pair<typename Cont::iterator, bool>& iter_pair,
							int check)
{
	yggr_test_assert(iter_pair.first != base.end());
	yggr_test_assert(iter_pair.first->second == check);
	return iter_pair.second;
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
bool ins_handler_zwei(typename Cont::base_type& base,
						const std::pair<typename Cont::iterator, bool>& pair,
						const typename Cont::value_type& chk_val)
{
	yggr_test_assert(chk_val == *pair.first);
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

	yggr_test_assert(cont.replace(300, 3000));
	yggr_test_assert(cont.replace(300, 300));
	yggr_test_assert(!cont.replace(9000, 8000));

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
		bool bchk = false;

		map_type ins_cont;
		bchk = ins_cont.emplace(yggr::non_handler );
		yggr_test_assert(bchk);
		yggr_test_assert(ins_cont.size() == 1);
		yggr_test_assert(ins_cont.find(0));

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
						boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1), yggr::non_handler, 10, 10));
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
						boost::bind(&ins_handler<map_type>, _1, _2, std::make_pair(10, 10)), 10, 10));
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

		// unordered_map
#ifndef YGGR_NO_CXX11_HDR_UNORDERED_MAP
		{
			typedef std::unordered_map<int, int> now_map_type;

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
			typedef std::unordered_map<int, int> now_map_type;

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

#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

		{
			typedef boost::unordered_map<int, int> now_map_type;

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
			typedef boost::unordered_map<int, int> now_map_type;

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
			typedef yggr::unordered_map<int, int> now_map_type;

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
			typedef yggr::unordered_map<int, int> now_map_type;

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
			typedef yggr::safe_container::safe_unordered_map<int, int> now_map_type;
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
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(cont_zwei);
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		{
			typedef yggr::safe_container::safe_unordered_map<int, int> now_map_type;
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
			yggr_test_assert(cont_zwei.size() == 5);
			cont_eins.merge(boost::move(cont_zwei));
			yggr_test_assert(cont_eins.size() == 9);
			yggr_test_assert(cont_zwei.size() == 1);
		}

		// unordered_multimap
#ifndef YGGR_NO_CXX11_HDR_UNORDERED_MAP
		{
			typedef std::unordered_multimap<int, int> now_map_type;

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
			typedef std::unordered_multimap<int, int> now_map_type;

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
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

		{
			typedef boost::unordered_multimap<int, int> now_map_type;

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
			typedef boost::unordered_multimap<int, int> now_map_type;

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
			typedef yggr::unordered_multimap<int, int> now_map_type;

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
			typedef yggr::unordered_multimap<int, int> now_map_type;

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
			typedef yggr::safe_container::safe_unordered_multimap<int, int> now_map_type;

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
			typedef yggr::safe_container::safe_unordered_multimap<int, int> now_map_type;

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
		//	typedef map_type::key_type key_type;
		//	typedef map_type::value_type value_type;
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
typename Cont::insert_return_type
	ins_node_handler(typename Cont::base_type& base,
						typename Cont::base_insert_return_type& ins_ret)
{
	typedef Cont map_type;
	return typename map_type::insert_return_type(boost::move(ins_ret));
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

	typedef Cont map_type;
	typedef typename map_type::base_type base_map_type;
	typedef typename map_type::iterator iter_type;
	typedef typename map_type::const_iterator citer_type;

	typedef iter_type (base_map_type::* begin_end_foo_type)(void);
	typedef citer_type (base_map_type::* cbegin_end_foo_type)(void) const;

	// extract insert
	{
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
			int tmp_key = 0;
			map_type ins_cont;
			for(int *i = arr, *isize = arr + 5; i != isize; ++i)
			{
				ins_cont.insert(*i, *i * 10);
			}
			typename map_type::node_type node =
				ins_cont.extract(boost::bind(static_cast<begin_end_foo_type>(&base_map_type::begin), _1));
			yggr_test_assert(!!node);
			yggr_test_assert(!ins_cont.find(node.key()));
			tmp_key = node.key();

			typename map_type::insert_return_type iret =
				ins_cont.insert(
					boost::move(node),
					boost::bind(&ins_node_handler<map_type>, _1, _2));
			yggr_test_assert(node.empty());
			yggr_test_assert(iret.node.empty());
			yggr_test_assert(iret.inserted);

			yggr_test_assert(ins_cont.find(tmp_key));
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
	}

	std::cout << "---------------test_extract_insert end-----------------" << std::endl;
}

template<typename Cont>
void test_buckets(void)
{
	typedef Cont map_type;

	typename map_type::base_type init_cont;
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));
	init_cont.insert(typename map_type::value_type(0, 0));

	map_type cont(init_cont.begin(), init_cont.end());
	typename map_type::base_type base_cont(init_cont.begin(), init_cont.end());

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
	typedef Cont map_type;

	typename map_type::base_type init_cont;
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));
	init_cont.insert(typename map_type::value_type(0, 0));

	map_type cont(init_cont.begin(), init_cont.end());
	typename map_type::base_type base_cont(init_cont.begin(), init_cont.end());

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

		typedef yggr::safe_container::safe_unordered_map<float, float> fmap_type;

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
		typedef yggr::safe_container::safe_unordered_map<int, yggr::shared_ptr<int> > ptr_vt_type;

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

	typename map_type::base_type init_cont;
	init_cont.insert(typename map_type::value_type(1, 1));
	init_cont.insert(typename map_type::value_type(2, 2));
	init_cont.insert(typename map_type::value_type(3, 3));
	init_cont.insert(typename map_type::value_type(4, 4));
	init_cont.insert(typename map_type::value_type(5, 5));
	init_cont.insert(typename map_type::value_type(0, 0));

	map_type cont(init_cont.begin(), init_cont.end());
	yggr_test_assert(cont.is_exists(1));
	yggr_test_assert(cont.is_exists(1, cont.hash_function(), cont.key_eq()));
	yggr_test_assert(cont.is_exists(0));
	yggr_test_assert(cont.is_exists(0, cont.hash_function(), cont.key_eq()));
	yggr_test_assert(!cont.is_exists(100));
	yggr_test_assert(!cont.is_exists(100, cont.hash_function(), cont.key_eq()));

	map_type empty_map;

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

template<typename Cont, typename Iter>
bool check_element(const typename Cont::base_type& base, Iter i, int chk_val)
{
	return (i != base.end()) && (i->second == chk_val);
}

template<typename Cont>
void test_find(void)
{
	typedef Cont map_type;

	typename map_type::base_type init_cont;
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
	test_buckets<map_type>();
	test_hash_policy<map_type>();
	test_safe_element_reference<map_type>();
	test_safe_other<map_type>();
	test_swap<map_type>();
	test_find<map_type>();
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
	test_buckets<map_type>();
	test_hash_policy<map_type>();
	test_safe_element_reference<map_type>();
	test_safe_other<map_type>();
	test_swap<map_type>();
	test_find<map_type>();
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
bool push_handler(typename Container::base_type&,
					const std::pair<typename Container::iterator, bool>&,
					int& count)
{
	++count;
	return true;
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
	return std::pair<int, val_type>(std::rand() % E_max_elem, val_type(new elem_type(std::rand() % 1000)));
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
			cont.is_exists(std::rand() % E_max_elem);
			cont.is_exists(std::rand() % E_max_elem);
		}
		break;
	case 7: // using_handler const
		hvt.push_back(7 + E_read);
		cont.using_handler(boost::bind(&cref_handler<container_type>, _1));
		break;
	case 8:
		hvt.push_back(8 + E_read);
		cont.count(std::rand() % E_max_elem);
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
		{
		    typename container_type::value_type val(create_val<val_type>());
            cont.insert(boost::move(val));
		}
		break;
	case 4: // insert(val&&, handler)
		{
			hvt.push_back(4 + E_write);
			int count = 0;

			typename container_type::value_type val(create_val<val_type>());
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
			cont.erase(std::rand() % E_max_elem);
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
            //typename container_type::value_type val(create_val<val_type>());
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

	{
		for(int i = 0; i != trd_size; ++i)
		{
			trds.create_thread(boost::bind(thread_foo_of_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt)));
		}
		trds.join_all();
	}

	std::cout << "test_thread_safe_of_mutex end" << std::endl;
}

template<typename Container>
void thread_foo_of_shared_mutex(Container& cont, int size, hvt_type& hvt, bool mode)
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

	trds.create_thread(boost::bind(&thread_foo_of_shared_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), true));

	{
		for(int i = 0, isize = trd_size - 1; i != isize; ++i)
		{
			trds.create_thread(boost::bind(thread_foo_of_shared_mutex<container_type>, boost::ref(cont), loop_size, boost::ref(hvt), false));
		}
		trds.join_all();
	}

	std::cout << "test_thread_safe_of_shared_mutex end" << std::endl;
}

int main(int argc, char* argv[])
{
	{
		typedef yggr::safe_container::safe_unordered_map<int, int> map_type;
		debug_test<map_type>();
	}

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	{
		typedef
			yggr::safe_container::safe_unordered_map
			<
				int, int,
				boost::mutex,
#   if defined(__GNUC__)
#       if (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
				// in GCC CPP17 and later stl, hashes are split into fast and non-fast,
                //  fast hash and non-fast hash do not support merge operations
                std::hash<int>,
#       else
                boost::hash<int>,
#       endif // __MINGW32__ __linux
#   else
				std::hash<int>,
#   endif // __GNUC__
				std::equal_to<int>,
				std::allocator<std::pair<const int, int> >,
				std::unordered_map
			> map_type;
		debug_test_std<map_type>();
	}
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

	std::srand(std::time(0));

	// mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_unordered_map<int, val_type> map_type;
		test_thread_safe_of_mutex<map_type>();
	}

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_unordered_map
			<
				int, val_type,
				boost::mutex,
#   if defined(__GNUC__)
#       if (YGGR_CPP_VER_17 <= YGGR_CPP_VERSION)
				// in GCC CPP17 and later stl, hashes are split into fast and non-fast,
                //  fast hash and non-fast hash do not support merge operations
                std::hash<int>,
#       else
                boost::hash<int>,
#       endif // __MINGW32__ __linux
#   else
				std::hash<int>,
#   endif // __GNUC__
				std::equal_to<int>,
				std::allocator<std::pair<const int, val_type> >,
				std::unordered_map
			> map_type;
		test_thread_safe_of_mutex<map_type>();
	}
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

	// shared_mutex
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef yggr::safe_container::safe_unordered_map<int, val_type, boost::shared_mutex> map_type;
		test_thread_safe_of_shared_mutex<map_type>();
	}

#if !defined(YGGR_NO_CXX11_HDR_UNORDERED_MAP)
	{
		typedef yggr::shared_ptr<int> val_type;
		typedef
			yggr::safe_container::safe_unordered_map
			<
				int, val_type,
				boost::shared_mutex,
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
				std::allocator<std::pair<const int, val_type> >,
				std::unordered_map
			> map_type;
		test_thread_safe_of_shared_mutex<map_type>();
	}
#endif // YGGR_NO_CXX11_HDR_UNORDERED_MAP

	wait_any_key(argc, argv);
	return 0;
}
