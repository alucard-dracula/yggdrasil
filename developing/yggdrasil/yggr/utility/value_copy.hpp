//value_copy.hpp

#ifndef __YGGR_UTILITY_VALUE_COPY_HPP__
#define __YGGR_UTILITY_VALUE_COPY_HPP__

#include <algorithm>
#include <vector>
#include <boost/container/vector.hpp>

namespace yggr
{
namespace utility
{

template<typename T>
void value_copy(T& l, const T& r)
{
	l = r;
}

template<typename T, typename Ax>
void value_copy(std::vector<T, Ax>& l, const std::vector<T, Ax>& r)
{
	assert(l.size() == r.size());
	if(l.size() != r.size() || l.empty())
	{
		return;
	}

	std::copy(r.begin(), r.end(), l.begin());
}

template<typename T, typename Ax>
void value_copy(boost::container::vector<T, Ax>& l, const boost::container::vector<T, Ax>& r)
{
	assert(l.size() == r.size());
	if(l.empty())
	{
		return;
	}

	std::copy(r.begin(), r.end(), l.begin());
}

} // namespace utility
} // namespace yggr

#endif //__YGGR_UTILITY_VALUE_COPY_HPP__