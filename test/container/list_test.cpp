// list_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/list.hpp>


#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_construct(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::list<int> list_type;
	typedef yggr::list<list_type::value_type, list_type::allocator_type> isomeric_same_lst_type;

#if !(BOOST_VERSION < 107000)
	typedef list_type::base_type base_lst_type;
	typedef isomeric_same_lst_type::base_type isomeric_same_base_lst_type;
#else
	typedef list_type base_lst_type;
	typedef isomeric_same_lst_type isomeric_same_base_lst_type;
#endif // #if !(BOOST_VERSION < 107000)

	list_type lst0(arr + 0, arr + 5);
	yggr_test_assert(lst0.size() == 5);

	{
		list_type lst;
		yggr_test_assert(lst.empty());
	}

	{
		list_type lst(lst0.get_allocator());
		yggr_test_assert(lst.empty());
	}

	{
		list_type lst(10);
		yggr_test_assert(lst.size() == 10);
	}

#if !(BOOST_VERSION < 107000)
	{
		list_type lst(10, lst0.get_allocator());
		yggr_test_assert(lst.size() == 10);
	}
#endif // #if !(BOOST_VERSION < 107000)

	{
		list_type lst(10, 100);
		yggr_test_assert(lst.size() == 10);
		yggr_test_assert(lst.front() == 100);
		yggr_test_assert(lst.back() == 100);
	}

	{
		list_type lst(10, 100, lst0.get_allocator());
		yggr_test_assert(lst.size() == 10);
		yggr_test_assert(lst.front() == 100);
		yggr_test_assert(lst.back() == 100);
	}

	{
		list_type lst(arr + 0, arr + 5);
		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 3);
	}

	{
		list_type lst(arr + 0, arr + 5, lst0.get_allocator());
		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 3);
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

#if !(BOOST_VERSION < 105700)
	{
		list_type lst({1, 2, 3, 4, 5});
		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 5);
	}

	{
		list_type lst({1, 2, 3, 4, 5}, lst0.get_allocator());
		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 5);
	}

#endif // #if !(BOOST_VERSION < 105700)

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	{
		base_lst_type tmp(arr + 0, arr + 5);
		list_type lst(tmp);

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst == tmp);
		yggr_test_assert(tmp == lst);
	}

	{
		list_type tmp(arr + 0, arr + 5);
		list_type lst(tmp);

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst == tmp);
		yggr_test_assert(tmp == lst);
	}

	{
		isomeric_same_base_lst_type tmp(arr + 0, arr + 5);
		list_type lst(tmp);

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst == tmp);
		yggr_test_assert(tmp == lst);
	}

	{
		isomeric_same_lst_type tmp(arr + 0, arr + 5);
		list_type lst(tmp);

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst == tmp);
		yggr_test_assert(tmp == lst);
	}

	{
		base_lst_type tmp(arr + 0, arr + 5);
		list_type lst(boost::move(tmp));

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 3);
	}


	{
		list_type tmp(arr + 0, arr + 5);
		list_type lst(boost::move(tmp));

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 3);
	}

	{
		isomeric_same_base_lst_type tmp(arr + 0, arr + 5);
		list_type lst(boost::move(tmp));

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 3);
	}

	{
		isomeric_same_lst_type tmp(arr + 0, arr + 5);
		list_type lst(boost::move(tmp));

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 3);
	}

	{
		base_lst_type tmp(arr + 0, arr + 5);
		list_type lst(tmp, tmp.get_allocator());

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst == tmp);
		yggr_test_assert(tmp == lst);
	}

	{
		list_type tmp(arr + 0, arr + 5);
		list_type lst(tmp, tmp.get_allocator());

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst == tmp);
		yggr_test_assert(tmp == lst);
	}

	{
		isomeric_same_base_lst_type tmp(arr + 0, arr + 5);
		list_type lst(tmp, tmp.get_allocator());

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst == tmp);
		yggr_test_assert(tmp == lst);
	}

	{
		isomeric_same_lst_type tmp(arr + 0, arr + 5);
		list_type lst(tmp, tmp.get_allocator());

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst == tmp);
		yggr_test_assert(tmp == lst);
	}

	{
		base_lst_type tmp(arr + 0, arr + 5);
		list_type lst(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 3);
	}


	{
		list_type tmp(arr + 0, arr + 5);
		list_type lst(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 3);
	}

	{
		isomeric_same_base_lst_type tmp(arr + 0, arr + 5);
		list_type lst(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 3);
	}

	{
		isomeric_same_lst_type tmp(arr + 0, arr + 5);
		list_type lst(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(lst.size() == 5);
		yggr_test_assert(lst.front() == 1);
		yggr_test_assert(lst.back() == 3);
	}

	std::cout << "------------test_construct end---------------" << std::endl;
}

void test_compare(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::list<int> list_type;
	typedef yggr::list<list_type::value_type, list_type::allocator_type> isomeric_same_lst_type;

#if !(BOOST_VERSION < 107000)
	typedef list_type::base_type base_lst_type;
	typedef isomeric_same_lst_type::base_type isomeric_same_base_lst_type;
#else
	typedef list_type base_lst_type;
	typedef isomeric_same_lst_type isomeric_same_base_lst_type;
#endif // #if !(BOOST_VERSION < 107000)

	list_type lst(arr + 0, arr + 5);

	list_type lst_equal(arr + 0, arr + 5);
	isomeric_same_lst_type lst_same_equal(arr + 0, arr + 5);

	list_type lst_less(arr + 0, arr + 5);
	isomeric_same_lst_type lst_same_less(arr + 0, arr + 5);
	lst_less.back() -= 1;
	lst_same_less.back() -= 1;

	list_type lst_greater(arr + 0, arr + 5);
	isomeric_same_lst_type lst_same_greater(arr + 0, arr + 5);
	lst_greater.back() += 1;
	lst_same_greater.back() += 1;

	{
		// ==
		yggr_test_assert(static_cast<const base_lst_type&>(lst) == lst);
		yggr_test_assert(lst == static_cast<const base_lst_type&>(lst));
		yggr_test_assert(lst == lst);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) == lst_same_equal);
		yggr_test_assert(lst == static_cast<const isomeric_same_base_lst_type&>(lst_same_equal));
		yggr_test_assert(lst == lst_same_equal);

		// != 
		yggr_test_assert(static_cast<const base_lst_type&>(lst) != lst_less);
		yggr_test_assert(lst != static_cast<const base_lst_type&>(lst_less));
		yggr_test_assert(lst != lst_less);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) != lst_same_less);
		yggr_test_assert(lst != static_cast<const isomeric_same_base_lst_type&>(lst_same_less));
		yggr_test_assert(lst != lst_same_less);

		// <
		yggr_test_assert(static_cast<const base_lst_type&>(lst) < lst_greater);
		yggr_test_assert(lst < static_cast<const base_lst_type&>(lst_greater));
		yggr_test_assert(lst < lst_greater);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) < lst_same_greater);
		yggr_test_assert(lst < static_cast<const isomeric_same_base_lst_type&>(lst_same_greater));
		yggr_test_assert(lst < lst_same_greater);

		// <=
		yggr_test_assert(static_cast<const base_lst_type&>(lst) <= lst);
		yggr_test_assert(lst <= static_cast<const base_lst_type&>(lst));
		yggr_test_assert(lst <= lst);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) <= lst_equal);
		yggr_test_assert(lst <= static_cast<const base_lst_type&>(lst_equal));
		yggr_test_assert(lst <= lst_equal);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) <= lst_same_equal);
		yggr_test_assert(lst <= static_cast<const isomeric_same_base_lst_type&>(lst_same_equal));
		yggr_test_assert(lst <= lst_same_equal);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) <= lst_greater);
		yggr_test_assert(lst <= static_cast<const base_lst_type&>(lst_greater));
		yggr_test_assert(lst <= lst_greater);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) <= lst_same_greater);
		yggr_test_assert(lst <= static_cast<const isomeric_same_base_lst_type&>(lst_same_greater));
		yggr_test_assert(lst <= lst_same_greater);

		// >
		yggr_test_assert(static_cast<const base_lst_type&>(lst) > lst_less);
		yggr_test_assert(lst > static_cast<const base_lst_type&>(lst_less));
		yggr_test_assert(lst > lst_less);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) > lst_same_less);
		yggr_test_assert(lst > static_cast<const isomeric_same_base_lst_type&>(lst_same_less));
		yggr_test_assert(lst > lst_same_less);

		// >=
		yggr_test_assert(static_cast<const base_lst_type&>(lst) >= lst);
		yggr_test_assert(lst >= static_cast<const base_lst_type&>(lst));
		yggr_test_assert(lst >= lst);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) >= lst_equal);
		yggr_test_assert(lst >= static_cast<const base_lst_type&>(lst_equal));
		yggr_test_assert(lst >= lst_equal);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) >= lst_same_equal);
		yggr_test_assert(lst >= static_cast<const isomeric_same_base_lst_type&>(lst_same_equal));
		yggr_test_assert(lst >= lst_same_equal);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) >= lst_less);
		yggr_test_assert(lst >= static_cast<const base_lst_type&>(lst_less));
		yggr_test_assert(lst >= lst_less);

		yggr_test_assert(static_cast<const base_lst_type&>(lst) >= lst_same_less);
		yggr_test_assert(lst >= static_cast<const isomeric_same_base_lst_type&>(lst_same_less));
		yggr_test_assert(lst >= lst_same_less);

	}

	std::cout << "------------test_compare end---------------" << std::endl;
}



void test_erase(void)
{
	int arr[5] = {1, 1, 2, 2, 3};

	{
		typedef std::list<int> list_type;
		list_type lst(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase(lst, 1);
		yggr_test_assert(s == 2);

		s = yggr::container::erase(lst, 6);
		yggr_test_assert(s == 0);
	}

	{
		typedef boost::container::list<int> list_type;
		list_type lst(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase(lst, 1);
		yggr_test_assert(s == 2);

		s = yggr::container::erase(lst, 6);
		yggr_test_assert(s == 0);
	}

	{
		typedef yggr::list<int> list_type;
		list_type lst(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase(lst, 1);
		yggr_test_assert(s == 2);

		s = yggr::container::erase(lst, 6);
		yggr_test_assert(s == 0);
	}

	std::cout << "------------test_erase end---------------" << std::endl;
}

template<std::size_t V>
struct test_pred_t
{
	inline bool operator()(int n) const
	{
		return n == V;
	}
};

void test_erase_if(void)
{
	int arr[5] = {1, 1, 2, 2, 3};

	{
		typedef std::list<int> list_type;

		list_type lst(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase_if(lst, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::container::erase_if(lst, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	{
		typedef boost::container::list<int> list_type;

		list_type lst(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase_if(lst, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::container::erase_if(lst, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	{
		typedef yggr::list<int> list_type;

		list_type lst(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase_if(lst, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::container::erase_if(lst, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	std::cout << "------------test_erase_if end---------------" << std::endl;
}

int main(int argc, char* argv[])
{

	test_construct();
	test_compare();

	test_erase();
	test_erase_if();

	wait_any_key(argc, argv);
	return 0;
}