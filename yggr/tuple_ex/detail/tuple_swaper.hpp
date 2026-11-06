// tuple_swaper.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_TUPLE_EX_DETAIL_TUPLE_SWAPER_HPP__
#define __YGGR_TUPLE_EX_DETAIL_TUPLE_SWAPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/tuple/tuple.hpp>
#include <yggr/tuple/tuple_comparison.hpp>
#include <yggr/tuple/tuple_io.hpp>

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
#	include <tuple>
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace yggr
{
namespace tuple_ex
{
namespace detail
{
// swap detail

template<std::size_t I, std::size_t ISize>
struct tuple_swap_helper
{
	template<typename T1, typename T2> inline
	void operator()(T1& l, T2& r) const
	{
		typedef tuple_swap_helper<I + 1, ISize> next_swap_helper_type;

		::yggr::swap(::yggr::get<I>(l), ::yggr::get<I>(r));

		next_swap_helper_type h;
		h(l, r);
	}
};

template<std::size_t ISize>
struct tuple_swap_helper<ISize, ISize>
{
	template<typename T1, typename T2> inline
	void operator()(T1& l, T2& r) const
	{
	}
};

} // namespace detail
} // namespace tuple_ex
} // namespace yggr

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace yggr
{
namespace tuple_ex
{
namespace swap_support
{

// std::tuple swap std::tuple // declar in std
// std::tuple swap boost::tuple
template< typename ...LArgs, typename ...RArgs > inline
typename
	boost::enable_if_c
	<
		std::tuple_size< std::tuple<LArgs...> >::value == boost::tuples::length< boost::tuple<RArgs...> >::value,
		void
	>::type
	swap(std::tuple<LArgs...>& l, boost::tuple<RArgs...>& r)
{
	typedef std::tuple<LArgs...> l_type;
	typedef boost::tuple<RArgs...> r_type;

	typedef ::yggr::tuple_ex::detail::tuple_swap_helper<0, std::tuple_size<l_type>::value> helper_type;

	helper_type h;
	h(l, r);
}

template< typename ...LArgs, typename ...RArgs > inline
typename
	boost::disable_if_c
	<
		std::tuple_size< std::tuple<LArgs...> >::value == boost::tuples::length< boost::tuple<RArgs...> >::value,
		void
	>::type
	swap(std::tuple<LArgs...>&, boost::tuple<RArgs...>&)
{
	YGGR_MPLEX_FXIED_ASSERT_FALSE();
}

// std::tuple swap yggr::tuple
template< typename ...LArgs, typename ...RArgs > inline
typename
	boost::enable_if_c
	<
		std::tuple_size< std::tuple<LArgs...> >::value == yggr::tuples::length< yggr::tuple<RArgs...> >::value,
		void
	>::type
	swap(std::tuple<LArgs...>& l, yggr::tuple<RArgs...>& r)
{
	typedef std::tuple<LArgs...> l_type;
	typedef yggr::tuple<RArgs...> r_type;

	typedef ::yggr::tuple_ex::detail::tuple_swap_helper<0, std::tuple_size<l_type>::value> helper_type;

	helper_type h;
	h(l, r);
}

template< typename ...LArgs, typename ...RArgs > inline
typename
	boost::disable_if_c
	<
		std::tuple_size< std::tuple<LArgs...> >::value == yggr::tuples::length< yggr::tuple<RArgs...> >::value,
		void
	>::type
	swap(std::tuple<LArgs...>&, yggr::tuple<RArgs...>&)
{
	YGGR_MPLEX_FXIED_ASSERT_FALSE();
}

//boost::tuple swap std::tuple
template< typename ...LArgs, typename ...RArgs > inline
typename
	boost::enable_if_c
	<
		boost::tuples::length< boost::tuple<LArgs...> >::value == std::tuple_size< std::tuple<RArgs...> >::value,
		void
	>::type
	swap(boost::tuple<LArgs...>& l, std::tuple<RArgs...>& r)
{
	typedef boost::tuple<LArgs...> l_type;
	typedef std::tuple<RArgs...> r_type;

	typedef ::yggr::tuple_ex::detail::tuple_swap_helper<0, boost::tuples::length<l_type>::value> helper_type;

	helper_type h;
	h(l, r);
}

template< typename ...LArgs, typename ...RArgs > inline
typename
	boost::disable_if_c
	<
		boost::tuples::length< boost::tuple<LArgs...> >::value == std::tuple_size< std::tuple<RArgs...> >::value,
		void
	>::type
	swap(boost::tuple<LArgs...>& l, std::tuple<RArgs...>& r)
{
	YGGR_MPLEX_FXIED_ASSERT_FALSE();
}

//boost::tuple swap boost::tuple // declar in boost
//boost::tuple swap yggr::tuple // declar in yggr

//yggr::tuple swap std::tuple
template< typename ...LArgs, typename ...RArgs > inline
typename
	boost::enable_if_c
	<
		yggr::tuples::length< boost::tuple<LArgs...> >::value == std::tuple_size< std::tuple<RArgs...> >::value,
		void
	>::type
	swap(yggr::tuple<LArgs...>& l, std::tuple<RArgs...>& r)
{
	typedef yggr::tuple<LArgs...> l_type;
	typedef std::tuple<RArgs...> r_type;

	typedef ::yggr::tuple_ex::detail::tuple_swap_helper<0, yggr::tuples::length<l_type>::value> helper_type;

	helper_type h;
	h(l, r);
}

template< typename ...LArgs, typename ...RArgs > inline
typename
	boost::disable_if_c
	<
		yggr::tuples::length< boost::tuple<LArgs...> >::value == std::tuple_size< std::tuple<RArgs...> >::value,
		void
	>::type
	swap(yggr::tuple<LArgs...>& l, std::tuple<RArgs...>& r)
{
	YGGR_MPLEX_FXIED_ASSERT_FALSE();
}

//yggr::tuple swap boost::tuple // declar in boost
//yggr::tuple swap yggr::tuple // declar in yggr

} // namespace swap_support
} // namespace tuple_ex
} // namespace yggr

namespace yggr
{
namespace tuples
{

using yggr::tuple_ex::swap_support::swap;

} // namesapce tuples
} // namespace yggr

namespace std
{

using yggr::tuple_ex::swap_support::swap;

} // namespace std

namespace boost
{
	using yggr::tuple_ex::swap_support::swap;
} // namespace boost

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

#endif // __YGGR_TUPLE_EX_DETAIL_TUPLE_SWAPER_HPP__
