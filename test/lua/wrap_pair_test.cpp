//lua_wrap_pair_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <yggr/lua_wrap/wrap_pair.hpp>

#include <cassert>
#include <typeinfo>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

template<typename T>
void test_construct(void)
{
	typedef T wrap_pair_type;
	typedef typename wrap_pair_type::base_type pair_type;
	typedef typename wrap_pair_type::first_type first_type;
	typedef typename wrap_pair_type::second_type second_type;
	
	{
		wrap_pair_type wpair;
		yggr_test_assert(wpair == wrap_pair_type(0, 0));
	}

	{
		yggr::u64 f = 1;
		yggr::f32 s = 1;
		wrap_pair_type wpair(boost::move(f), boost::move(s));
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		wrap_pair_type wpair(yggr::s32(1), yggr::f32(1));
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		first_type f = 1;
		second_type s = 1;
		wrap_pair_type wpair(boost::move(f), boost::move(s));
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		wrap_pair_type wpair(1, 1);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		std::pair<yggr::s32, yggr::f32> other_pair(1, 1);
		wrap_pair_type wpair(boost::move(other_pair));
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		std::pair<yggr::s32, yggr::f32> other_pair(1, 1);
		wrap_pair_type wpair(other_pair);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		pair_type other_pair(1, 1);
		wrap_pair_type wpair(boost::move(other_pair));
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		pair_type other_pair(1, 1);
		wrap_pair_type wpair(other_pair);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		yggr::lua_wrap::wrap_pair<yggr::s32, yggr::f32> other_pair(1, 1);
		wrap_pair_type wpair(boost::move(other_pair));
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		yggr::lua_wrap::wrap_pair<yggr::s32, yggr::f32> other_pair(1, 1);
		wrap_pair_type wpair(other_pair);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		wrap_pair_type other_pair(1, 1);
		wrap_pair_type wpair(boost::move(other_pair));
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		wrap_pair_type other_pair(1, 1);
		wrap_pair_type wpair(other_pair);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	std::cout << "------test_construct [" << typeid(wrap_pair_type).name() << "] end------" << std::endl;
}


template<typename T>
void test_operator_set(void)
{
	typedef T wrap_pair_type;
	typedef typename wrap_pair_type::base_type pair_type;
	typedef typename wrap_pair_type::first_type first_type;
	typedef typename wrap_pair_type::second_type second_type;

	{
		std::pair<yggr::s32, yggr::f32> other_pair(1, 1);
		wrap_pair_type wpair;
		wpair = boost::move(other_pair);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		std::pair<yggr::s32, yggr::f32> other_pair(1, 1);
		wrap_pair_type wpair;
		wpair = other_pair;
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		pair_type other_pair(1, 1);
		wrap_pair_type wpair;
		wpair = boost::move(other_pair);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		pair_type other_pair(1, 1);
		wrap_pair_type wpair;
		wpair = other_pair;
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		yggr::lua_wrap::wrap_pair<yggr::s32, yggr::f32> other_pair(1, 1);
		wrap_pair_type wpair;
		wpair = boost::move(other_pair);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		yggr::lua_wrap::wrap_pair<yggr::s32, yggr::f32> other_pair(1, 1);
		wrap_pair_type wpair;
		wpair = other_pair;
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		wrap_pair_type other_pair(1, 1);
		wrap_pair_type wpair;
		wpair = boost::move(other_pair);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	{
		wrap_pair_type other_pair(1, 1);
		wrap_pair_type wpair;
		wpair = other_pair;
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
	}

	std::cout << "------test_operator_set [" << typeid(wrap_pair_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_swap(void)
{
	typedef T wrap_pair_type;
	typedef typename wrap_pair_type::base_type pair_type;
	typedef typename wrap_pair_type::first_type first_type;
	typedef typename wrap_pair_type::second_type second_type;

	{
		pair_type other_pair(1, 1);
		wrap_pair_type wpair;
		wpair.swap(boost::move(other_pair));
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
		yggr_test_assert(other_pair == wrap_pair_type(0, 0));
	}

	{
		pair_type other_pair(1, 1);
		wrap_pair_type wpair;
		wpair.swap(other_pair);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
		yggr_test_assert(other_pair == wrap_pair_type(0, 0));
	}

	{
		wrap_pair_type other_pair(1, 1);
		wrap_pair_type wpair;
		wpair.swap(boost::move(other_pair));
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
		yggr_test_assert(other_pair == wrap_pair_type(0, 0));
	}

	{
		wrap_pair_type other_pair(1, 1);
		wrap_pair_type wpair;
		wpair.swap(other_pair);
		yggr_test_assert(wpair == wrap_pair_type(1, 1));
		yggr_test_assert(other_pair == wrap_pair_type(0, 0));
	}

	std::cout << "------test_swap [" << typeid(wrap_pair_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_other(void)
{
	typedef T wrap_pair_type;
	typedef typename wrap_pair_type::base_type pair_type;
	typedef typename wrap_pair_type::first_type first_type;
	typedef typename wrap_pair_type::second_type second_type;

	{
		wrap_pair_type wpair(1, 2);
		wrap_pair_type& wpair_ref = wpair;
		const wrap_pair_type& wpair_cref = wpair;

		yggr_test_assert(wpair_ref.get_first() == 1);
		yggr_test_assert(wpair_ref.get_second() == 2);

		wpair_ref.set_first(10);
		wpair_ref.set_second(20);

		yggr_test_assert(wpair_cref.get_first() == 10);
		yggr_test_assert(wpair_cref.get_second() == 20);
	}

	std::cout << "------test_other [" << typeid(wrap_pair_type).name() << "] end------" << std::endl;
}

template<typename T>
void test_compare(void)
{
	typedef T wrap_pair_type;
	typedef typename wrap_pair_type::base_type pair_type;
	typedef typename wrap_pair_type::first_type first_type;
	typedef typename wrap_pair_type::second_type second_type;

	{
		yggr_test_assert(wrap_pair_type(1, 2).compare_eq(wrap_pair_type(1, 2).unwrap()));
		yggr_test_assert(wrap_pair_type(1, 2).compare_eq(wrap_pair_type(1, 2)));

		yggr_test_assert(!wrap_pair_type(1, 2).compare_eq(wrap_pair_type(1, 3).unwrap()));
		yggr_test_assert(!wrap_pair_type(1, 2).compare_eq(wrap_pair_type(1, 3)));
	}

	{
		yggr_test_assert(0 == wrap_pair_type(1, 2).compare(wrap_pair_type(1, 2).unwrap()));
		yggr_test_assert(0 == wrap_pair_type(1, 2).compare(wrap_pair_type(1, 2)));

		yggr_test_assert(-1 == wrap_pair_type(1, 2).compare(wrap_pair_type(1, 3).unwrap()));
		yggr_test_assert(-1 == wrap_pair_type(1, 2).compare(wrap_pair_type(1, 3)));

		yggr_test_assert(1 == wrap_pair_type(1, 3).compare(wrap_pair_type(1, 2).unwrap()));
		yggr_test_assert(1 == wrap_pair_type(1, 3).compare(wrap_pair_type(1, 2)));
	}

	{
		yggr_test_assert(wrap_pair_type(1, 2).unwrap() == wrap_pair_type(1, 2));
		yggr_test_assert(wrap_pair_type(1, 2) == wrap_pair_type(1, 2).unwrap());
		yggr_test_assert(wrap_pair_type(1, 2) == wrap_pair_type(1, 2));
	}

	{
		yggr_test_assert(wrap_pair_type(1, 2).unwrap() != wrap_pair_type(1, 3));
		yggr_test_assert(wrap_pair_type(1, 2) != wrap_pair_type(1, 3).unwrap());
		yggr_test_assert(wrap_pair_type(1, 2) != wrap_pair_type(1, 3));
	}

	{
		yggr_test_assert(wrap_pair_type(1, 2).unwrap() < wrap_pair_type(1, 3));
		yggr_test_assert(wrap_pair_type(1, 2) < wrap_pair_type(1, 3).unwrap());
		yggr_test_assert(wrap_pair_type(1, 2) < wrap_pair_type(1, 3));
	}

	{
		yggr_test_assert(wrap_pair_type(1, 2).unwrap() <= wrap_pair_type(1, 2));
		yggr_test_assert(wrap_pair_type(1, 2) <= wrap_pair_type(1, 2).unwrap());
		yggr_test_assert(wrap_pair_type(1, 2) <= wrap_pair_type(1, 2));

		yggr_test_assert(wrap_pair_type(1, 2).unwrap() <= wrap_pair_type(1, 3));
		yggr_test_assert(wrap_pair_type(1, 2) <= wrap_pair_type(1, 3).unwrap());
		yggr_test_assert(wrap_pair_type(1, 2) <= wrap_pair_type(1, 3));
	}

	{
		yggr_test_assert(wrap_pair_type(1, 3).unwrap() > wrap_pair_type(1, 2));
		yggr_test_assert(wrap_pair_type(1, 3) > wrap_pair_type(1, 2).unwrap());
		yggr_test_assert(wrap_pair_type(1, 3) > wrap_pair_type(1, 2));

		yggr_test_assert(wrap_pair_type(1, 3).unwrap() >= wrap_pair_type(1, 2));
		yggr_test_assert(wrap_pair_type(1, 3) >= wrap_pair_type(1, 2).unwrap());
		yggr_test_assert(wrap_pair_type(1, 3) >= wrap_pair_type(1, 2));
	}

	std::cout << "------test_compare [" << typeid(wrap_pair_type).name() << "] end------" << std::endl;
}

template<typename T1, typename T2>
void test_compare_twice(void)
{
	typedef T1 l_wrap_pair_type;
	typedef typename l_wrap_pair_type::base_type l_pair_type;
	typedef typename l_wrap_pair_type::first_type l_first_type;
	typedef typename l_wrap_pair_type::second_type l_second_type;

	typedef T2 r_wrap_pair_type;
	typedef typename r_wrap_pair_type::base_type r_pair_type;
	typedef typename r_wrap_pair_type::first_type r_first_type;
	typedef typename r_wrap_pair_type::second_type r_second_type;

	{
		yggr_test_assert(l_wrap_pair_type(1, 2).compare_eq(r_wrap_pair_type(1, 2).unwrap()));
		yggr_test_assert(l_wrap_pair_type(1, 2).compare_eq(r_wrap_pair_type(1, 2)));

		yggr_test_assert(!l_wrap_pair_type(1, 2).compare_eq(r_wrap_pair_type(1, 3).unwrap()));
		yggr_test_assert(!l_wrap_pair_type(1, 2).compare_eq(r_wrap_pair_type(1, 3)));
	}

	{
		yggr_test_assert(0 == l_wrap_pair_type(1, 2).compare(r_wrap_pair_type(1, 2).unwrap()));
		yggr_test_assert(0 == l_wrap_pair_type(1, 2).compare(r_wrap_pair_type(1, 2)));

		yggr_test_assert(-1 == l_wrap_pair_type(1, 2).compare(r_wrap_pair_type(1, 3).unwrap()));
		yggr_test_assert(-1 == l_wrap_pair_type(1, 2).compare(r_wrap_pair_type(1, 3)));

		yggr_test_assert(1 == l_wrap_pair_type(1, 3).compare(r_wrap_pair_type(1, 2).unwrap()));
		yggr_test_assert(1 == l_wrap_pair_type(1, 3).compare(r_wrap_pair_type(1, 2)));
	}

	{
		yggr_test_assert(l_wrap_pair_type(1, 2).unwrap() == r_wrap_pair_type(1, 2));
		yggr_test_assert(l_wrap_pair_type(1, 2) == r_wrap_pair_type(1, 2).unwrap());
		yggr_test_assert(l_wrap_pair_type(1, 2) == r_wrap_pair_type(1, 2));
	}

	{
		yggr_test_assert(l_wrap_pair_type(1, 2).unwrap() != r_wrap_pair_type(1, 3));
		yggr_test_assert(l_wrap_pair_type(1, 2) != r_wrap_pair_type(1, 3).unwrap());
		yggr_test_assert(l_wrap_pair_type(1, 2) != r_wrap_pair_type(1, 3));
	}

	{
		yggr_test_assert(l_wrap_pair_type(1, 2).unwrap() < r_wrap_pair_type(1, 3));
		yggr_test_assert(l_wrap_pair_type(1, 2) < r_wrap_pair_type(1, 3).unwrap());
		yggr_test_assert(l_wrap_pair_type(1, 2) < r_wrap_pair_type(1, 3));
	}

	{
		yggr_test_assert(l_wrap_pair_type(1, 2).unwrap() <= r_wrap_pair_type(1, 2));
		yggr_test_assert(l_wrap_pair_type(1, 2) <= r_wrap_pair_type(1, 2).unwrap());
		yggr_test_assert(l_wrap_pair_type(1, 2) <= r_wrap_pair_type(1, 2));

		yggr_test_assert(l_wrap_pair_type(1, 2).unwrap() <= r_wrap_pair_type(1, 3));
		yggr_test_assert(l_wrap_pair_type(1, 2) <= r_wrap_pair_type(1, 3).unwrap());
		yggr_test_assert(l_wrap_pair_type(1, 2) <= r_wrap_pair_type(1, 3));
	}

	{
		yggr_test_assert(l_wrap_pair_type(1, 3).unwrap() > r_wrap_pair_type(1, 2));
		yggr_test_assert(l_wrap_pair_type(1, 3) > r_wrap_pair_type(1, 2).unwrap());
		yggr_test_assert(l_wrap_pair_type(1, 3) > r_wrap_pair_type(1, 2));

		yggr_test_assert(l_wrap_pair_type(1, 3).unwrap() >= r_wrap_pair_type(1, 2));
		yggr_test_assert(l_wrap_pair_type(1, 3) >= r_wrap_pair_type(1, 2).unwrap());
		yggr_test_assert(l_wrap_pair_type(1, 3) >= r_wrap_pair_type(1, 2));
	}
	
	std::cout << "------test_compare_twice [" << typeid(r_wrap_pair_type).name() << "] end------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct< yggr::lua_wrap::wrap_pair<yggr::s64, yggr::f64> >();
	test_operator_set< yggr::lua_wrap::wrap_pair<yggr::s64, yggr::f64> >();
	test_swap< yggr::lua_wrap::wrap_pair<yggr::s64, yggr::f64> >();
	test_other< yggr::lua_wrap::wrap_pair<yggr::s64, yggr::f64> >();
	test_compare< yggr::lua_wrap::wrap_pair<yggr::s64, yggr::f64> >();

	test_compare_twice
	<
		yggr::lua_wrap::wrap_pair<yggr::s64, yggr::f64>,
		yggr::lua_wrap::wrap_pair<yggr::s32, yggr::f32>
	>();


	wait_any_key(argc, argv);
	return 0;
}
