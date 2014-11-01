//const_var.hpp

#ifndef __YGGR_MATH_CONST_VAR_HPP__
#define __YGGR_MATH_CONST_VAR_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

#define YGGR_PP_MATH_PI 3.1415925

namespace yggr
{
namespace math
{

struct const_var
{
	template<typename T>
	inline static T pi(void)
	{
		BOOST_MPL_ASSERT((boost::is_floating_point<T>));
		return T(YGGR_PP_MATH_PI);
	}
};

} // namespace math
} // namespace yggr

#undef YGGR_PP_MATH_PI

#endif //__YGGR_MATH_CONST_VAR_HPP__