// options_vector_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>
#include <cassert>

#include <yggr/container/options_vector.hpp>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#define TEST_COMPILE_ERROR 0

void test_construct(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::options_vector<int> vt_type;
	typedef yggr::options_vector<vt_type::value_type, vt_type::allocator_type> isomeric_same_vt_type;

	vt_type vt0(arr + 0, arr + 5);
	yggr_test_assert(vt0.size() == 5);

	{
		vt_type vt;
		yggr_test_assert(vt.empty());
	}

	{
		vt_type vt(vt0.get_allocator());
		yggr_test_assert(vt.empty());
	}

	{
		vt_type vt(10);
		yggr_test_assert(vt.size() == 10);
	}

	{
		vt_type vt(10, vt0.get_allocator());
		yggr_test_assert(vt.size() == 10);
	}

	{
		vt_type vt(10, 100);
		yggr_test_assert(vt.size() == 10);
		yggr_test_assert(vt.front() == 100);
		yggr_test_assert(vt.back() == 100);
	}

	{
		vt_type vt(10, 100, vt0.get_allocator());
		yggr_test_assert(vt.size() == 10);
		yggr_test_assert(vt.front() == 100);
		yggr_test_assert(vt.back() == 100);
	}

	{
		vt_type vt(arr + 0, arr + 5);
		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 3);
	}

	{
		vt_type vt(arr + 0, arr + 5, vt0.get_allocator());
		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 3);
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

#	if !(BOOST_VERSION < 105700)

	{
		vt_type vt({1, 2, 3, 4, 5});
		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 5);
	}

#	endif // #	if !(BOOST_VERSION < 105700)

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	{
		vt_type::base_type tmp(arr + 0, arr + 5);
		vt_type vt(tmp);

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt == tmp);
		yggr_test_assert(tmp == vt);
	}

	{
		vt_type tmp(arr + 0, arr + 5);
		vt_type vt(tmp);

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt == tmp);
		yggr_test_assert(tmp == vt);
	}

	{
		isomeric_same_vt_type::base_type tmp(arr + 0, arr + 5);
		vt_type vt(tmp);

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt == tmp);
		yggr_test_assert(tmp == vt);
	}

	{
		isomeric_same_vt_type tmp(arr + 0, arr + 5);
		vt_type vt(tmp);

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt == tmp);
		yggr_test_assert(tmp == vt);
	}

	{
		vt_type::base_type tmp(arr + 0, arr + 5);
		vt_type vt(boost::move(tmp));

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 3);
	}

	{
		vt_type tmp(arr + 0, arr + 5);
		vt_type vt(boost::move(tmp));

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 3);
	}

	{
		isomeric_same_vt_type::base_type tmp(arr + 0, arr + 5);
		vt_type vt(boost::move(tmp));

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 3);
	}

	{
		isomeric_same_vt_type tmp(arr + 0, arr + 5);
		vt_type vt(boost::move(tmp));

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 3);
	}

	{
		vt_type::base_type tmp(arr + 0, arr + 5);
		vt_type vt(tmp, tmp.get_allocator());

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt == tmp);
		yggr_test_assert(tmp == vt);
	}

	{
		vt_type tmp(arr + 0, arr + 5);
		vt_type vt(tmp, tmp.get_allocator());

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt == tmp);
		yggr_test_assert(tmp == vt);
	}

	{
		isomeric_same_vt_type::base_type tmp(arr + 0, arr + 5);
		vt_type vt(tmp, tmp.get_allocator());

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt == tmp);
		yggr_test_assert(tmp == vt);
	}

	{
		isomeric_same_vt_type tmp(arr + 0, arr + 5);
		vt_type vt(tmp, tmp.get_allocator());

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt == tmp);
		yggr_test_assert(tmp == vt);
	}

	{
		vt_type::base_type tmp(arr + 0, arr + 5);
		vt_type vt(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 3);
	}

	{
		vt_type tmp(arr + 0, arr + 5);
		vt_type vt(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 3);
	}

	{
		isomeric_same_vt_type::base_type tmp(arr + 0, arr + 5);
		vt_type vt(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 3);
	}

	{
		isomeric_same_vt_type tmp(arr + 0, arr + 5);
		vt_type vt(boost::move(tmp), tmp.get_allocator());

		yggr_test_assert(vt.size() == 5);
		yggr_test_assert(vt.front() == 1);
		yggr_test_assert(vt.back() == 3);
	}

	std::cout << "------------test_construct end---------------" << std::endl;
}

void test_nth_and_index_of(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::options_vector<int> vector_type;

	vector_type vt(arr + 0, arr + 5);

	{
		vector_type::iterator nth = vt.nth(0);
		yggr_test_assert(nth == vt.begin());
		yggr_test_assert(*nth == 1);
		std::size_t idx_of = vt.index_of(nth);
		yggr_test_assert(idx_of == 0);
	}

	{
		vector_type::iterator nth = vt.nth(4);
		yggr_test_assert(nth != vt.end());
		yggr_test_assert(*nth == 3);
		std::size_t idx_of = vt.index_of(nth);
		yggr_test_assert(idx_of == 4);
	}

	{
		vector_type::iterator nth = vt.nth(5);
		yggr_test_assert(nth == vt.end());
		std::size_t idx_of = vt.index_of(nth);
		yggr_test_assert(idx_of == 5);
	}

#if TEST_COMPILE_ERROR

	{
		vector_type::iterator nth = vt.nth(5);
		++nth;
		yggr_test_assert(nth != vt.end());
		std::size_t idx_of = vt.index_of(nth);// assert false
	}

#endif // TEST_COMPILE_ERROR

	{
		vector_type::const_iterator nth = const_cast<const vector_type&>(vt).nth(0);
		yggr_test_assert(nth == vt.begin());
		yggr_test_assert(*nth == 1);
		std::size_t idx_of = vt.index_of(nth);
		yggr_test_assert(idx_of == 0);
	}

	{
		vector_type::const_iterator nth = const_cast<const vector_type&>(vt).nth(4);
		yggr_test_assert(nth != vt.end());
		yggr_test_assert(*nth == 3);
		std::size_t idx_of = vt.index_of(nth);
		yggr_test_assert(idx_of == 4);
	}

	{
		vector_type::const_iterator nth = const_cast<const vector_type&>(vt).nth(5);
		yggr_test_assert(nth == vt.end());
		std::size_t idx_of = vt.index_of(nth);
		yggr_test_assert(idx_of == 5);
	}

#if TEST_COMPILE_ERROR

	{
		vector_type::const_iterator nth = const_cast<const vector_type&>(vt).nth(5);
		++nth;
		yggr_test_assert(nth != vt.end());
		std::size_t idx_of = vt.index_of(nth);// assert false
	}

#endif // TEST_COMPILE_ERROR

	std::cout << "------------test_nth_and_index_of end---------------" << std::endl;
}

void test_compare(void)
{
	int arr[5] = {1, 1, 2, 2, 3};
	typedef yggr::options_vector<int> vt_type;
	typedef yggr::options_vector<vt_type::value_type, vt_type::allocator_type> isomeric_same_vt_type;

	typedef vt_type::base_type base_vt_type;
	typedef isomeric_same_vt_type isomeric_same_base_vt_type;

	vt_type vt(arr + 0, arr + 5);

	vt_type vt_equal(arr + 0, arr + 5);
	isomeric_same_vt_type vt_same_equal(arr + 0, arr + 5);

	vt_type vt_less(arr + 0, arr + 5);
	isomeric_same_vt_type vt_same_less(arr + 0, arr + 5);
	vt_less[4] -= 1;
	vt_same_less[4] -= 1;

	vt_type vt_greater(arr + 0, arr + 5);
	isomeric_same_vt_type vt_same_greater(arr + 0, arr + 5);
	vt_greater[4] += 1;
	vt_same_greater[4] += 1;

	{
		// ==
		yggr_test_assert(static_cast<const base_vt_type&>(vt) == vt);
		yggr_test_assert(vt == static_cast<const base_vt_type&>(vt));
		yggr_test_assert(vt == vt);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) == vt_same_equal);
		yggr_test_assert(vt == static_cast<const isomeric_same_base_vt_type&>(vt_same_equal));
		yggr_test_assert(vt == vt_same_equal);

		// != 
		yggr_test_assert(static_cast<const base_vt_type&>(vt) != vt_less);
		yggr_test_assert(vt != static_cast<const base_vt_type&>(vt_less));
		yggr_test_assert(vt != vt_less);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) != vt_same_less);
		yggr_test_assert(vt != static_cast<const isomeric_same_base_vt_type&>(vt_same_less));
		yggr_test_assert(vt != vt_same_less);

		// <
		yggr_test_assert(static_cast<const base_vt_type&>(vt) < vt_greater);
		yggr_test_assert(vt < static_cast<const base_vt_type&>(vt_greater));
		yggr_test_assert(vt < vt_greater);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) < vt_same_greater);
		yggr_test_assert(vt < static_cast<const isomeric_same_base_vt_type&>(vt_same_greater));
		yggr_test_assert(vt < vt_same_greater);

		// <=
		yggr_test_assert(static_cast<const base_vt_type&>(vt) <= vt);
		yggr_test_assert(vt <= static_cast<const base_vt_type&>(vt));
		yggr_test_assert(vt <= vt);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) <= vt_equal);
		yggr_test_assert(vt <= static_cast<const base_vt_type&>(vt_equal));
		yggr_test_assert(vt <= vt_equal);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) <= vt_same_equal);
		yggr_test_assert(vt <= static_cast<const isomeric_same_base_vt_type&>(vt_same_equal));
		yggr_test_assert(vt <= vt_same_equal);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) <= vt_greater);
		yggr_test_assert(vt <= static_cast<const base_vt_type&>(vt_greater));
		yggr_test_assert(vt <= vt_greater);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) <= vt_same_greater);
		yggr_test_assert(vt <= static_cast<const isomeric_same_base_vt_type&>(vt_same_greater));
		yggr_test_assert(vt <= vt_same_greater);

		// >
		yggr_test_assert(static_cast<const base_vt_type&>(vt) > vt_less);
		yggr_test_assert(vt > static_cast<const base_vt_type&>(vt_less));
		yggr_test_assert(vt > vt_less);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) > vt_same_less);
		yggr_test_assert(vt > static_cast<const isomeric_same_base_vt_type&>(vt_same_less));
		yggr_test_assert(vt > vt_same_less);

		// >=
		yggr_test_assert(static_cast<const base_vt_type&>(vt) >= vt);
		yggr_test_assert(vt >= static_cast<const base_vt_type&>(vt));
		yggr_test_assert(vt >= vt);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) >= vt_equal);
		yggr_test_assert(vt >= static_cast<const base_vt_type&>(vt_equal));
		yggr_test_assert(vt >= vt_equal);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) >= vt_same_equal);
		yggr_test_assert(vt >= static_cast<const isomeric_same_base_vt_type&>(vt_same_equal));
		yggr_test_assert(vt >= vt_same_equal);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) >= vt_less);
		yggr_test_assert(vt >= static_cast<const base_vt_type&>(vt_less));
		yggr_test_assert(vt >= vt_less);

		yggr_test_assert(static_cast<const base_vt_type&>(vt) >= vt_same_less);
		yggr_test_assert(vt >= static_cast<const isomeric_same_base_vt_type&>(vt_same_less));
		yggr_test_assert(vt >= vt_same_less);

	}

	std::cout << "------------test_compare end---------------" << std::endl;
}

void test_erase(void)
{
	int arr[5] = {1, 1, 2, 2, 3};

	{
		typedef yggr::options_vector<int> vt_type;
		vt_type vt(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase(vt, 1);
		yggr_test_assert(s == 2);

		s = yggr::container::erase(vt, 6);
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
		typedef yggr::options_vector<int> vt_type;

		vt_type vt(arr + 0, arr + 5);

		std::size_t s = 0;

		s = yggr::container::erase_if(vt, test_pred_t<1>());
		yggr_test_assert(s == 2);

		s = yggr::container::erase_if(vt, test_pred_t<6>());
		yggr_test_assert(s == 0);
	}

	std::cout << "------------test_erase_if end---------------" << std::endl;
}

int main(int argc, char* argv[])
{
	test_construct();
	test_nth_and_index_of();

	test_compare();

	test_erase();
	test_erase_if();

	wait_any_key(argc, argv);
	return 0;
}