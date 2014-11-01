//value_swap.hpp

#ifndef __YGGR_UTILITY_VALUE_SWAP_HPP__
#define __YGGR_UTILITY_VALUE_SWAP_HPP__

#include <algorithm>
#include <vector>
#include <boost/container/vector.hpp>
#include <yggr/utility/swap.hpp>

namespace yggr
{
namespace utility
{

template<typename T>
void value_swap(T& l, T& r)
{
	boost::swap(l, r);
}

template<typename T, typename Ax>
void value_swap(std::vector<T, Ax>& l, std::vector<T, Ax>& r)
{
	assert(l.size() == r.size());
	if(l.size() != r.size() || l.empty())
	{
		return;
	}

	std::swap_ranges(l.begin(), l.end(), r.begin());
}

template<typename T, typename Ax>
void value_swap(boost::container::vector<T, Ax>& l, boost::container::vector<T, Ax>& r)
{
	assert(l.size() == r.size());
	if(l.size() != r.size() || l.empty())
	{
		return;
	}

	std::swap_ranges(l.begin(), l.end(), r.begin());
}

} // namespace utility
} // namespace yggr

#endif //__YGGR_UTILITY_VALUE_SWAP_HPP__