// deque_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/deque.hpp>


#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_construct(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::deque<int> dq_type;
	typedef yggr::deque<dq_type::value_type, dq_type::allocator_type> isomeric_same_dq_type;

#if !(BOOST_VERSION < 107100)
	typedef dq_type::base_type base_dq_type;
	typedef isomeric_same_dq_type::base_type isomeric_same_base_dq_type;
#else
	typedef dq_type base_dq_type;
	typedef isomeric_same_dq_type isomeric_same_base_dq_type;
#endif // #if !(BOOST_VERSION < 107100)

	dq_type dq0(arr + 0, arr + 5);
	yggr_test_assert(dq0.size() == 5);

	{
		dq_type dq;
		yggr_test_assert(dq.empty());
	}

	{
		dq_type dq(dq0.get_allocator());
		yggr_test_assert(dq.empty());
	}

	{
		dq_type dq(10);
		yggr_test_assert(dq.size() == 10);
	}

#if !(BOOST_VERSION < 107100)
	{
		dq_type dq(10, dq0.get_allocator());
		yggr_test_assert(dq.size() == 10);
	}
#endif // #if !(BOOST_VERSION < 107100)

	{
		dq_type dq(10, 100);
		yggr_test_assert(dq.size() == 10);
		yggr_test_assert(dq.front() == 100);
		yggr_test_assert(dq.back() == 100);
	}

	{
		dq_type dq(10, 100, dq0.get_allocator());
		yggr_test_assert(dq.size() == 10);
		yggr_test_assert(dq.front() == 100);
		yggr_test_assert(dq.back() == 100);
	}

	{
		dq_type dq(arr + 0, arr + 5);
		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		dq_type dq(arr + 0, arr + 5, dq0.get_allocator());
		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

#	if !(BOOST_VERSION < 105700)
	{
		dq_type dq({1, 2, 3, 4, 5});
		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 5);
	}

	{
		dq_type dq({1, 2, 3, 4, 5}, dq0.get_allocator());
		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 5);
	}

#	endif // #	if !(BOOST_VERSION < 105700)

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	{
		base_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(tmp);

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		dq_type tmp(arr + 0, arr + 5);
		dq_type dq(tmp);

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		isomeric_same_base_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(tmp);

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		isomeric_same_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(tmp);

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}


	{
		base_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(boost::move(tmp));

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		dq_type tmp(arr + 0, arr + 5);
		dq_type dq(boost::move(tmp));

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		isomeric_same_base_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(boost::move(tmp));

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		isomeric_same_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(boost::move(tmp));

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}


	{
		base_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(tmp, tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		dq_type tmp(arr + 0, arr + 5);
		dq_type dq(tmp, tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		isomeric_same_base_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(tmp, tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		isomeric_same_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(tmp, tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq == tmp);
		yggr_test_assert(tmp == dq);
	}

	{
		base_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		dq_type tmp(arr + 0, arr + 5);
		dq_type dq(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		isomeric_same_base_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	{
		isomeric_same_dq_type tmp(arr + 0, arr + 5);
		dq_type dq(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(dq.size() == 5);
		yggr_test_assert(dq.front() == 1);
		yggr_test_assert(dq.back() == 3);
	}

	std::cout << "------------test_construct end---------------" << std::endl;
}

void test_compare(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::deque<int> dq_type;
	typedef yggr::deque<dq_type::value_type, dq_type::allocator_type> isomeric_same_dq_type;

#if !(BOOST_VERSION < 107100)
	typedef dq_type::base_type base_dq_type;
	typedef isomeric_same_dq_type::base_type isomeric_same_base_dq_type;
#else
	typedef dq_type base_dq_type;
	typedef isomeric_same_dq_type isomeric_same_base_dq_type;
#endif // #if !(BOOST_VERSION < 107100)

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

void test_erase(void)
{
	int arr[5] = {1, 1, 2, 2, 3};

	{
		typedef std::deque<int> deque_type;
		deque_type dq(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase(dq, 1);
		yggr_test_assert(s == 2);

		s = yggr::container::erase(dq, 6);
		yggr_test_assert(s == 0);
	}

	{
		typedef boost::container::deque<int> deque_type;
		deque_type dq(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase(dq, 1);
		yggr_test_assert(s == 2);

		s = yggr::container::erase(dq, 6);
		yggr_test_assert(s == 0);
	}

	{
		typedef yggr::deque<int> deque_type;
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
		typedef std::deque<int> deque_type;

		deque_type dq(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase_if(dq, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::container::erase_if(dq, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	{
		typedef boost::container::deque<int> deque_type;

		deque_type dq(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase_if(dq, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::container::erase_if(dq, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	{
		typedef yggr::deque<int> deque_type;

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

	test_erase();
	test_erase_if();

	wait_any_key(argc, argv);
	return 0;
}