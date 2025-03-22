// is_functional_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>

#include <yggr/mplex/null_t.hpp>
#include <yggr/func/is_functional.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

struct non_func_t
{
};

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

struct u_func_t : public std::unary_function <int, int>
{
	int operator()(int) const
	{
		return 0;
	}
};

struct b_func_t : public std::binary_function <int, int, int>
{
	int operator()(int, int) const
	{
		return 0;
	}
};

#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

int main(int argc, char* argv[])
{
	BOOST_MPL_ASSERT_NOT((yggr::is_base_of_unary_function<yggr::mplex::null_type>));
	BOOST_MPL_ASSERT_NOT((yggr::is_base_of_binary_function<non_func_t>));
	BOOST_MPL_ASSERT_NOT((yggr::is_functional<non_func_t>));

	BOOST_MPL_ASSERT_NOT((yggr::is_base_of_unary_function<non_func_t>));
	BOOST_MPL_ASSERT_NOT((yggr::is_base_of_binary_function<non_func_t>));
	BOOST_MPL_ASSERT_NOT((yggr::is_functional<non_func_t>));

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	BOOST_MPL_ASSERT((yggr::is_base_of_unary_function<u_func_t>));
	BOOST_MPL_ASSERT_NOT((yggr::is_base_of_binary_function<u_func_t>));
	BOOST_MPL_ASSERT((yggr::is_functional<u_func_t>));

	BOOST_MPL_ASSERT_NOT((yggr::is_base_of_unary_function<b_func_t>));
	BOOST_MPL_ASSERT((yggr::is_base_of_binary_function<b_func_t>));
	BOOST_MPL_ASSERT((yggr::is_functional<b_func_t>));

#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	wait_any_key(argc, argv);
	return 0;
};