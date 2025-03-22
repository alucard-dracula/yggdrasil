// options_deque_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/options_deque.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_construct(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::options_deque<int> deque_type;

	deque_type dq0(arr + 0, arr + 5);
	yggr_test_assert(dq0.size() == 5);

	{
		deque_type dq;
		yggr_test_assert(dq.empty());
	}

	{
		deque_type dq(dq0.get_allocator());
		yggr_test_assert(dq.empty());
	}

	{
		deque_type dq(10);
		yggr_test_assert(dq.size() == 10);
	}

	{
		deque_type dq(10, dq0.get_allocator());
		yggr_test_assert(dq.size() == 10);
	}

	{
		deque_type dq(10, 100);
		yggr_test_assert(dq.size() == 10);
		yggr_test_assert(dq.front() == 100);
		yggr_test_assert(dq.back() == 100);
	}

	{
		deque_type dq(10, 100, dq0.get_allocator());
		yggr_test_assert(dq.size() == 10);
		yggr_test_assert(dq.front() == 100);
		yggr_test_assert(dq.back() == 100);
	}

	{
		deque_type dq(arr + 0, arr + 5);
		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		deque_type dq(arr + 0, arr + 5, dq0.get_allocator());
		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

#	if !(BOOST_VERSION < 105700)
	{
		deque_type dq({1, 2, 3, 4, 5});
		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 5);
	}

#	endif //#	if !(BOOST_VERSION < 105700)

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	{
		deque_type::base_type tmp(arr + 0, arr + 5);
		deque_type dq(tmp);

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		deque_type tmp(arr + 0, arr + 5);
		deque_type dq(tmp);

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		deque_type::base_type tmp(arr + 0, arr + 5);
		deque_type dq(boost::move(tmp));

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		deque_type tmp(arr + 0, arr + 5);
		deque_type dq(boost::move(tmp));

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		deque_type::base_type tmp(arr + 0, arr + 5);
		deque_type dq(tmp, tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		deque_type tmp(arr + 0, arr + 5);
		deque_type dq(tmp, tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		deque_type::base_type tmp(arr + 0, arr + 5);
		deque_type dq(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		deque_type tmp(arr + 0, arr + 5);
		deque_type dq(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	std::cout << "------------test_construct end---------------" << std::endl;
}

void test_compare(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::options_deque<int> dq_type;
	typedef yggr::options_deque<dq_type::value_type, dq_type::allocator_type> isomeric_same_dq_type;

	typedef dq_type::base_type base_dq_type;
	typedef isomeric_same_dq_type isomeric_same_base_dq_type;

	dq_type dq(arr + 0, arr + 5);

	dq_type dq_equal(arr + 0, arr + 5);
	isomeric_same_dq_type dq_same_equal(arr + 0, arr + 5);

	dq_type dq_less(arr + 0, arr + 5);
	isomeric_same_dq_type dq_same_less(arr + 0, arr + 5);
	dq_less[4] -= 1;
	dq_same_less[4] -= 1;

	dq_type dq_greater(arr + 0, arr + 5);
	isomeric_same_dq_type dq_same_greater(arr + 0, arr + 5);
	dq_greater[4] += 1;
	dq_same_greater[4] += 1;

	{
		// ==
		yggr_test_assert(static_cast<const base_dq_type&>(dq) == dq);
		yggr_test_assert(dq == static_cast<const base_dq_type&>(dq));
		yggr_test_assert(dq == dq);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) == dq_same_equal);
		yggr_test_assert(dq == static_cast<const isomeric_same_base_dq_type&>(dq_same_equal));
		yggr_test_assert(dq == dq_same_equal);

		// != 
		yggr_test_assert(static_cast<const base_dq_type&>(dq) != dq_less);
		yggr_test_assert(dq != static_cast<const base_dq_type&>(dq_less));
		yggr_test_assert(dq != dq_less);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) != dq_same_less);
		yggr_test_assert(dq != static_cast<const isomeric_same_base_dq_type&>(dq_same_less));
		yggr_test_assert(dq != dq_same_less);

		// <
		yggr_test_assert(static_cast<const base_dq_type&>(dq) < dq_greater);
		yggr_test_assert(dq < static_cast<const base_dq_type&>(dq_greater));
		yggr_test_assert(dq < dq_greater);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) < dq_same_greater);
		yggr_test_assert(dq < static_cast<const isomeric_same_base_dq_type&>(dq_same_greater));
		yggr_test_assert(dq < dq_same_greater);

		// <=
		yggr_test_assert(static_cast<const base_dq_type&>(dq) <= dq);
		yggr_test_assert(dq <= static_cast<const base_dq_type&>(dq));
		yggr_test_assert(dq <= dq);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) <= dq_equal);
		yggr_test_assert(dq <= static_cast<const base_dq_type&>(dq_equal));
		yggr_test_assert(dq <= dq_equal);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) <= dq_same_equal);
		yggr_test_assert(dq <= static_cast<const isomeric_same_base_dq_type&>(dq_same_equal));
		yggr_test_assert(dq <= dq_same_equal);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) <= dq_greater);
		yggr_test_assert(dq <= static_cast<const base_dq_type&>(dq_greater));
		yggr_test_assert(dq <= dq_greater);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) <= dq_same_greater);
		yggr_test_assert(dq <= static_cast<const isomeric_same_base_dq_type&>(dq_same_greater));
		yggr_test_assert(dq <= dq_same_greater);

		// >
		yggr_test_assert(static_cast<const base_dq_type&>(dq) > dq_less);
		yggr_test_assert(dq > static_cast<const base_dq_type&>(dq_less));
		yggr_test_assert(dq > dq_less);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) > dq_same_less);
		yggr_test_assert(dq > static_cast<const isomeric_same_base_dq_type&>(dq_same_less));
		yggr_test_assert(dq > dq_same_less);

		// >=
		yggr_test_assert(static_cast<const base_dq_type&>(dq) >= dq);
		yggr_test_assert(dq >= static_cast<const base_dq_type&>(dq));
		yggr_test_assert(dq >= dq);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) >= dq_equal);
		yggr_test_assert(dq >= static_cast<const base_dq_type&>(dq_equal));
		yggr_test_assert(dq >= dq_equal);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) >= dq_same_equal);
		yggr_test_assert(dq >= static_cast<const isomeric_same_base_dq_type&>(dq_same_equal));
		yggr_test_assert(dq >= dq_same_equal);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) >= dq_less);
		yggr_test_assert(dq >= static_cast<const base_dq_type&>(dq_less));
		yggr_test_assert(dq >= dq_less);

		yggr_test_assert(static_cast<const base_dq_type&>(dq) >= dq_same_less);
		yggr_test_assert(dq >= static_cast<const isomeric_same_base_dq_type&>(dq_same_less));
		yggr_test_assert(dq >= dq_same_less);

	}

	std::cout << "------------test_compare end---------------" << std::endl;
}

void test_nth_and_index_of(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::options_deque<int> deque_type;

	deque_type dq(arr + 0, arr + 5);

	{
		deque_type::iterator nth = dq.nth(0);
		yggr_test_assert(nth == dq.begin());
		yggr_test_assert(*nth == 1);
		std::size_t idx_of = dq.index_of(nth);
		yggr_test_assert(idx_of == 0);
	}

	{
		deque_type::iterator nth = dq.nth(4);
		yggr_test_assert(nth != dq.end());
		yggr_test_assert(*nth == 3);
		std::size_t idx_of = dq.index_of(nth);
		yggr_test_assert(idx_of == 4);
	}

	{
		deque_type::iterator nth = dq.nth(5);
		yggr_test_assert(nth == dq.end());
		std::size_t idx_of = dq.index_of(nth);
		yggr_test_assert(idx_of == 5);
	}

	//{
	//	deque_type::iterator nth = dq.nth(5);
	//	++nth;
	//	yggr_test_assert(nth != dq.end());
	//	std::size_t idx_of = dq.index_of(nth);// assert false
	//}

	{
		deque_type::const_iterator nth = const_cast<const deque_type&>(dq).nth(0);
		yggr_test_assert(nth == dq.begin());
		yggr_test_assert(*nth == 1);
		std::size_t idx_of = dq.index_of(nth);
		yggr_test_assert(idx_of == 0);
	}

	{
		deque_type::const_iterator nth = const_cast<const deque_type&>(dq).nth(4);
		yggr_test_assert(nth != dq.end());
		yggr_test_assert(*nth == 3);
		std::size_t idx_of = dq.index_of(nth);
		yggr_test_assert(idx_of == 4);
	}

	{
		deque_type::const_iterator nth = const_cast<const deque_type&>(dq).nth(5);
		yggr_test_assert(nth == dq.end());
		std::size_t idx_of = dq.index_of(nth);
		yggr_test_assert(idx_of == 5);
	}

	//{
	//	deque_type::const_iterator nth = const_cast<const deque_type&>(dq).nth(5);
	//	++nth;
	//	yggr_test_assert(nth != dq.end());
	//	std::size_t idx_of = dq.index_of(nth);// assert false
	//}

	std::cout << "------------test_nth_and_index_of end---------------" << std::endl;
}

void test_erase(void)
{
	int arr[5] = {1, 1, 2, 2, 3};

	{
		typedef yggr::options_deque<int> deque_type;
		deque_type dq(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase(dq, 1);
		yggr_test_assert(s == 2);

		s = yggr::container::erase(dq, 6);
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
		typedef yggr::options_deque<int> deque_type;

		deque_type dq(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase_if(dq, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::container::erase_if(dq, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	std::cout << "------------test_erase_if end---------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_compare();
	test_nth_and_index_of();

	test_erase();
	test_erase_if();

	wait_any_key(argc, argv);
	return 0;
}