// functional_args_size_test.cpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

#include <yggr/mplex/null_t.hpp>
#include <yggr/func/functional_args_size.hpp>

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
	BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::functional_args_size<yggr::mplex::null_type>::value == 0)>));

	BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::functional_args_size<non_func_t>::value == 0)>));

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::functional_args_size<u_func_t>::value == 1)>));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(yggr::functional_args_size<b_func_t>::value == 2)>));
#endif // (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	wait_any_key(argc, argv);
	return 0;
};