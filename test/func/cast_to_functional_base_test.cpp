// cast_to_functional_base.hpp

#include <test/wait_any_key/wait_any_key.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>
#include <yggr/func/cast_to_functional_base.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

struct non_func_t
{
};

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

typedef std::unary_function <int, int> base_u_func_t;

struct u_func_t : public std::unary_function <int, int>
{
	int operator()(int) const
	{
		return 0;
	}
};

typedef std::binary_function <int, int, int> base_b_func_t;
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
	BOOST_MPL_ASSERT((boost::is_same<yggr::func::cast_to_functional_base<non_func_t>::type, yggr::mplex::null_type>));

#if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)
	BOOST_MPL_ASSERT((boost::is_same<yggr::func::cast_to_functional_base<base_u_func_t>::type, base_u_func_t>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::func::cast_to_functional_base<u_func_t>::type, base_u_func_t>));

	BOOST_MPL_ASSERT((boost::is_same<yggr::func::cast_to_functional_base<base_b_func_t>::type, base_b_func_t>));
	BOOST_MPL_ASSERT((boost::is_same<yggr::func::cast_to_functional_base<u_func_t>::type, base_u_func_t>));
#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_17)

	wait_any_key(argc, argv);
	return 0;
};