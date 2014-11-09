//pair.hpp

#ifndef __YGGR_STRUCT_SUPPORT_PAIR_HPP__
#define __YGGR_STRUCT_SUPPORT_PAIR_HPP__

#include <utility>
#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace struct_support
{
namespace detail
{

template<typename T1, typename T2, std::size_t idx>
struct pair_getter;

template<typename T1, typename T2>
struct pair_getter<T1, T2, 0>
{
	typedef std::pair<T1, T2> pair_type;
	typedef typename pair_type::first_type result_type;

	inline result_type& operator()(pair_type& val) const
	{
		return val.first;
	}

	inline const result_type& operator()(const pair_type& val) const
	{
		return val.first;
	}
};

template<typename T1, typename T2>
struct pair_getter<T1, T2, 1>
{
	typedef std::pair<T1, T2> pair_type;
	typedef typename pair_type::second_type result_type;

	inline result_type& operator()(pair_type& val) const
	{
		return val.second;
	}

	inline const result_type& operator()(const pair_type& val) const
	{
		return val.second;
	}
};

} // namespace detail
} // namespace struct_support

template<yggr::size_type idx, typename T1, typename T2>
typename struct_support::detail::pair_getter<T1, T2, idx>::result_type& 
	get(std::pair<T1, T2>& val)
{ 
	typedef struct_support::detail::pair_getter<T1, T2, idx> getter_type;
	getter_type getter;
	return getter(val);
}

template<yggr::size_type idx, typename T1, typename T2>
const typename struct_support::detail::pair_getter<T1, T2, idx>::result_type&
	get(const std::pair<T1, T2>& val)
{ 
	typedef struct_support::detail::pair_getter<T1, T2, idx> getter_type;
	getter_type getter;
	return getter(val);
}

} // namespace yggr

//namespace boost_swap_impl
//{
//	template<typename T1, typename T2>
//	void swap_impl(std::pair<T1, T2>& l, std::pair<T1, T2>& r)
//	{
//		::boost_swap_impl::swap_impl(l.first, r.first);
//		::boost_swap_impl::swap_impl(l.second, r.second);
//	}
//}

//namespace boost
//{
//	template<typename T1, typename T2>
//	void swap_impl(std::pair<T1, T2>& l, std::pair<T1, T2>& r)
//	{
//		boost::swap(l.first, r.first);
//		boost::swap(l.second, r.second);
//	}
//} // namespace boost

#endif // __YGGR_STRUCT_SUPPORT_PAIR_HPP__
