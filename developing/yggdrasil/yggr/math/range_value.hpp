//range_value.hpp

#ifndef __YGGR_MATH_RANGE_VALUE_HPP__
#define __YGGR_MATH_RANGE_VALUE_HPP__

namespace yggr
{
namespace math
{

template<typename T>
inline T range_value(const T& val, const T& min, const T& max)
{
	return val < min? min : (val > max? max : val);
}

} // namespace math
} // namespace yggr

#endif // __YGGR_MATH_RANGE_VALUE_HPP__