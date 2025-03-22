// tuple.hpp

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

#ifndef __YGGR_TUPLE_EX_TUPLE_HPP__
#define __YGGR_TUPLE_EX_TUPLE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/tuple/tuple.hpp>
#include <yggr/tuple/tuple_comparison.hpp>
#include <yggr/tuple/tuple_io.hpp>

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	include <yggr/type_traits/integer_sequence.hpp>
#	include <yggr/support/return_arg.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <boost/utility/enable_if.hpp>
#include <boost/functional/hash/hash.hpp>

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
#	include <tuple>
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace yggr
{
namespace tuples
{
namespace detail
{

struct tuple_cmper_failed_true
{
	template<typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE
	bool operator()(const T1&, const T2&) const
	{
		return true;
	}
};

struct tuple_cmper_failed_false
{
	template<typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE
	bool operator()(const T1&, const T2&) const
	{
		return false;
	}
};

struct tuple_cmper_failed
{
	template<typename T1, typename T2> YGGR_CONSTEXPR_OR_INLINE
	bool operator()(const T1&, const T2&) const
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return false;
	}
};

// eq
template<bool is_same_len>
struct tuple_eq;

template<>
struct tuple_eq< true >
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		return yggr::tuples::get_inherited(l) == yggr::tuples::get_inherited(r);
	}
};

template<>
struct tuple_eq< false >
	: public tuple_cmper_failed_false
{
public:
	typedef tuple_cmper_failed_false base_type;

public:
	using base_type::operator();
};

// neq
template<bool is_same_len>
struct tuple_neq;

template<>
struct tuple_neq< true >
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		return yggr::tuples::get_inherited(l) != yggr::tuples::get_inherited(r);
	}
};

template<>
struct tuple_neq< false >
	: public tuple_cmper_failed_true
{
public:
	typedef tuple_cmper_failed_true base_type;

public:
	using base_type::operator();
};

// lt
template<bool is_same_len>
struct tuple_lt;

template<>
struct tuple_lt< true >
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		return yggr::tuples::get_inherited(l) < yggr::tuples::get_inherited(r);
	}
};

template<>
struct tuple_lt< false >
	: public tuple_cmper_failed
{
public:
	typedef tuple_cmper_failed base_type;

public:
	using base_type::operator();
};

// lte
template<bool is_same_len>
struct tuple_lte;

template<>
struct tuple_lte< true >
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		return yggr::tuples::get_inherited(l) <= yggr::tuples::get_inherited(r);
	}
};

template<>
struct tuple_lte< false >
	: public tuple_cmper_failed
{
public:
	typedef tuple_cmper_failed base_type;

public:
	using base_type::operator();
};

// gt
template<bool is_same_len>
struct tuple_gt;

template<>
struct tuple_gt< true >
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		return yggr::tuples::get_inherited(l) > yggr::tuples::get_inherited(r);
	}
};

template<>
struct tuple_gt< false >
	: public tuple_cmper_failed
{
public:
	typedef tuple_cmper_failed base_type;

public:
	using base_type::operator();
};

// gte
template<bool is_same_len>
struct tuple_gte;

template<>
struct tuple_gte< true >
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		return yggr::tuples::get_inherited(l) >= yggr::tuples::get_inherited(r);
	}
};

template<>
struct tuple_gte< false >
	: public tuple_cmper_failed
{
public:
	typedef tuple_cmper_failed base_type;

public:
	using base_type::operator();
};

} // namespace detail
} // namespace tuples
} // namespace yggr

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

namespace yggr
{
namespace tuple_ex
{
namespace detail
{

//eq
template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs,
			std::size_t ...I> inline
bool tuple_eq_foo_helper(const TupleL<LArgs...>& l, const TupleR<RArgs...>& r, ::yggr::index_sequence<I...>)
{
	bool bchk = true;
	return support::return_first_arg((bchk = bchk && (::yggr::get<I>(l) == ::yggr::get<I>(r)))...);
}

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> inline
typename
	boost::enable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_eq_foo(const TupleL<LArgs...>& l, const TupleR<RArgs...>& r)
{
	typedef TupleL<LArgs...> now_tuple_type;
	return tuple_eq_foo_helper(l, r, yggr::make_index_sequence< ::yggr::tuple_size<now_tuple_type>::value>());
}

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::disable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_eq_foo(const TupleL<LArgs...>&, const TupleR<RArgs...>&)
{
	return false;
}

// neq
template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> inline
typename
	boost::enable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_neq_foo(const TupleL<LArgs...>& l, const TupleR<RArgs...>& r)
{
	typedef TupleL<LArgs...> now_tuple_type;
	return !tuple_eq_foo_helper(l, r, yggr::make_index_sequence< ::yggr::tuple_size<now_tuple_type>::value>());
}

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::disable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_neq_foo(const TupleL<LArgs...>&, const TupleR<RArgs...>&)
{
	return true;
}

//lt
template<std::size_t I, std::size_t ISize>
struct tuple_lt_foo_helper
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		typedef tuple_lt_foo_helper<I + 1, ISize> next_helper_type;
		next_helper_type next_helper;
		return
			(yggr::get<I>(l) < yggr::get<I>(r))
				|| (!(yggr::get<I>(r) < yggr::get<I>(l))
						&& (next_helper(l, r)) );
	}
};

template<std::size_t ISize>
struct tuple_lt_foo_helper<ISize, ISize>
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		return false;
	}
};

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> inline
typename
	boost::enable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_lt_foo(const TupleL<LArgs...>& l, const TupleR<RArgs...>& r)
{
	typedef TupleL<LArgs...> l_type;

	tuple_lt_foo_helper<0, ::yggr::tuple_size< l_type >::value> helper;
	return helper(l, r);
}

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::disable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_lt_foo(const TupleL<LArgs...>&, const TupleR<RArgs...>&)
{
	YGGR_MPLEX_FXIED_ASSERT_FALSE();
	return false;
}

//lte
template<std::size_t I, std::size_t ISize>
struct tuple_lte_foo_helper
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		typedef tuple_lte_foo_helper<I + 1, ISize> next_helper_type;
		next_helper_type next_helper;
		return
			(yggr::get<I>(l) <= yggr::get<I>(r))
				|| (!(yggr::get<I>(r) <= yggr::get<I>(l))
						&& (next_helper(l, r)) );
	}
};

template<std::size_t ISize>
struct tuple_lte_foo_helper<ISize, ISize>
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		return false;
	}
};

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> inline
typename
	boost::enable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_lte_foo(const TupleL<LArgs...>& l, const TupleR<RArgs...>& r)
{
	typedef TupleL<LArgs...> l_type;

	tuple_lte_foo_helper<0, ::yggr::tuple_size< l_type >::value> helper;
	return helper(l, r);
}

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::disable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_lte_foo(const TupleL<LArgs...>&, const TupleR<RArgs...>&)
{
	YGGR_MPLEX_FXIED_ASSERT_FALSE();
	return false;
}

//gt
template<std::size_t I, std::size_t ISize>
struct tuple_gt_foo_helper
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		typedef tuple_gt_foo_helper<I + 1, ISize> next_helper_type;
		next_helper_type next_helper;
		return
			(yggr::get<I>(l) > yggr::get<I>(r))
				|| (!(yggr::get<I>(r) > yggr::get<I>(l))
						&& (next_helper(l, r)) );
	}
};

template<std::size_t ISize>
struct tuple_gt_foo_helper<ISize, ISize>
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		return false;
	}
};

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> inline
typename
	boost::enable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_gt_foo(const TupleL<LArgs...>& l, const TupleR<RArgs...>& r)
{
	typedef TupleL<LArgs...> l_type;

	tuple_gt_foo_helper<0, ::yggr::tuple_size< l_type >::value> helper;
	return helper(l, r);
}

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::disable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_gt_foo(const TupleL<LArgs...>&, const TupleR<RArgs...>&)
{
	YGGR_MPLEX_FXIED_ASSERT_FALSE();
	return false;
}

//gte
template<std::size_t I, std::size_t ISize>
struct tuple_gte_foo_helper
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		typedef tuple_gte_foo_helper<I + 1, ISize> next_helper_type;
		next_helper_type next_helper;
		return
			(yggr::get<I>(l) >= yggr::get<I>(r))
				|| (!(yggr::get<I>(r) >= yggr::get<I>(l))
						&& (next_helper(l, r)) );
	}
};

template<std::size_t ISize>
struct tuple_gte_foo_helper<ISize, ISize>
{
	template<typename T1, typename T2> inline
	bool operator()(const T1& l, const T2& r) const
	{
		return false;
	}
};

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> inline
typename
	boost::enable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_gte_foo(const TupleL<LArgs...>& l, const TupleR<RArgs...>& r)
{
	typedef TupleL<LArgs...> l_type;

	tuple_gte_foo_helper<0, ::yggr::tuple_size< l_type >::value> helper;
	return helper(l, r);
}

template<template<typename ..._Args> class TupleL,
			template<typename ..._Args> class TupleR,
			typename ...LArgs, typename ...RArgs> YGGR_CONSTEXPR_OR_INLINE
typename
	boost::disable_if_c
	<
		::yggr::tuple_size< TupleL<LArgs...> >::value == ::yggr::tuple_size< TupleR<RArgs...> >::value,
		bool
	>::type
	tuple_gte_foo(const TupleL<LArgs...>&, const TupleR<RArgs...>&)
{
	YGGR_MPLEX_FXIED_ASSERT_FALSE();
	return false;
}

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
} // namespace tuples

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace std
{
// ==
template< typename ...LArgs, typename ...RArgs > inline
bool operator==(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_eq_foo(l, r);
}

template< typename ...LArgs, typename ...RArgs > inline
bool operator==(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_eq_foo(l, r);
}

// !=
template< typename ...LArgs, typename ...RArgs > inline
bool operator!=(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_neq_foo(l, r);
}

template< typename ...LArgs, typename ...RArgs > inline
bool operator!=(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_neq_foo(l, r);
}

// <
template< typename ...LArgs, typename ...RArgs > inline
bool operator<(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_lt_foo(l, r);
}

template< typename ...LArgs, typename ...RArgs > inline
bool operator<(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_lt_foo(l, r);
}

// <=
template< typename ...LArgs, typename ...RArgs > inline
bool operator<=(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_lte_foo(l, r);
}

template< typename ...LArgs, typename ...RArgs > inline
bool operator<=(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_lte_foo(l, r);
}

// >
template< typename ...LArgs, typename ...RArgs > inline
bool operator>(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_gt_foo(l, r);
}

template< typename ...LArgs, typename ...RArgs > inline
bool operator>(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_gt_foo(l, r);
}

// >=
template< typename ...LArgs, typename ...RArgs > inline
bool operator>=(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_gte_foo(l, r);
}

template< typename ...LArgs, typename ...RArgs > inline
bool operator>=(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_gte_foo(l, r);
}

} // namespace std

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace boost
{
namespace tuples
{

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

// std::tuple ==
template< typename ...LArgs, typename ...RArgs > inline
bool operator==(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_eq_foo(l, r);
}

// std::tuple !=
template< typename ...LArgs, typename ...RArgs > inline
bool operator!=(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_neq_foo(l, r);
}

// std::tuple <
template< typename ...LArgs, typename ...RArgs > inline
bool operator<(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_lt_foo(l, r);
}

// std::tuple <=
template< typename ...LArgs, typename ...RArgs > inline
bool operator<=(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_lte_foo(l, r);
}

// std::tuple >
template< typename ...LArgs, typename ...RArgs > inline
bool operator>(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_gt_foo(l, r);
}

// std::tuple >=
template< typename ...LArgs, typename ...RArgs > inline
bool operator>=(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_gte_foo(l, r);
}

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

// boost::tuple operator==
template< typename ...LArgs, typename ...RArgs > inline
bool operator==(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_eq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmp;
	return cmp(l, r);
}

// boost::tuple operator!=
template< typename ...LArgs, typename ...RArgs > inline
bool operator!=(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_neq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// boost::tuple operator<
template< typename ...LArgs, typename ...RArgs > inline
bool operator<(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_lt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// boost::tuple operator<=
template< typename ...LArgs, typename ...RArgs > inline
bool operator<=(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_lte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// boost::tuple operator>
template< typename ...LArgs, typename ...RArgs > inline
bool operator>(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_gt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// boost::tuple operator>=
template< typename ...LArgs, typename ...RArgs > inline
bool operator>=(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_gte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator==
template< typename ...LArgs, typename ...RArgs > inline
bool operator==(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef yggr::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_eq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmp;
	return cmp(l, r);
}

// yggr::tuple operator!=
template< typename ...LArgs, typename ...RArgs > inline
bool operator!=(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef yggr::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_neq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator<
template< typename ...LArgs, typename ...RArgs > inline
bool operator<(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef yggr::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_lt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator<=
template< typename ...LArgs, typename ...RArgs > inline
bool operator<=(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef yggr::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_lte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator>
template< typename ...LArgs, typename ...RArgs > inline
bool operator>(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef yggr::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_gt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator>=
template< typename ...LArgs, typename ...RArgs > inline
bool operator>=(const tuple<LArgs...>& l, const yggr::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef yggr::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_gte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// std::pair operator cmp
//==
template< typename T1, typename T2, typename ...Args> inline
bool operator==(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) == r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator==(const std::pair<T1, T2>& l, const tuple<Args...>& r)
{
	return l == yggr::tuples::get_inherited(r);
}

//!=
template< typename T1, typename T2, typename ...Args> inline
bool operator!=(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) != r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator!=(const std::pair<T1, T2>& l, const tuple<Args...>& r)
{
	return l != yggr::tuples::get_inherited(r);
}

// <
template< typename T1, typename T2, typename ...Args> inline
bool operator<(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) < r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator<(const std::pair<T1, T2>& l, const tuple<Args...>& r)
{
	return l < yggr::tuples::get_inherited(r);
}

// <=
template< typename T1, typename T2, typename ...Args> inline
bool operator<=(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) <= r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator<=(const std::pair<T1, T2>& l, const tuple<Args...>& r)
{
	return l <= yggr::tuples::get_inherited(r);
}

// >
template< typename T1, typename T2, typename ...Args> inline
bool operator>(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) > r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator>(const std::pair<T1, T2>& l, const tuple<Args...>& r)
{
	return l > yggr::tuples::get_inherited(r);
}

// >=
template< typename T1, typename T2, typename ...Args> inline
bool operator>=(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) >= r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator>=(const std::pair<T1,T2>& l, const tuple<Args...>& r)
{
	return l >= yggr::tuples::get_inherited(r);
}

} // namespace tuples
} // namespace boost


namespace yggr
{
namespace tuples
{

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

// std::tuple ==
template< typename ...LArgs, typename ...RArgs > inline
bool operator==(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_eq_foo(l, r);
}

// std::tuple !=
template< typename ...LArgs, typename ...RArgs > inline
bool operator!=(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_neq_foo(l, r);
}

// std::tuple <
template< typename ...LArgs, typename ...RArgs > inline
bool operator<(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_lt_foo(l, r);
}

// std::tuple <=
template< typename ...LArgs, typename ...RArgs > inline
bool operator<=(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_lte_foo(l, r);
}

// std::tuple >
template< typename ...LArgs, typename ...RArgs > inline
bool operator>(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_gt_foo(l, r);
}

// std::tuple >=
template< typename ...LArgs, typename ...RArgs > inline
bool operator>=(const tuple<LArgs...>& l, const std::tuple<RArgs...>& r)
{
	return yggr::tuple_ex::detail::tuple_gte_foo(l, r);
}

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)


// boost::tuple operator==
template< typename ...LArgs, typename ...RArgs > inline
bool operator==(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_eq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmp;
	return cmp(l, r);
}

// boost::tuple operator!=
template< typename ...LArgs, typename ...RArgs > inline
bool operator!=(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_neq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// boost::tuple operator<
template< typename ...LArgs, typename ...RArgs > inline
bool operator<(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_lt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// boost::tuple operator<=
template< typename ...LArgs, typename ...RArgs > inline
bool operator<=(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_lte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// boost::tuple operator>
template< typename ...LArgs, typename ...RArgs > inline
bool operator>(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_gt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// boost::tuple operator>=
template< typename ...LArgs, typename ...RArgs > inline
bool operator>=(const tuple<LArgs...>& l, const tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_gte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator==
template< typename ...LArgs, typename ...RArgs > inline
bool operator==(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef boost::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_eq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmp;
	return cmp(l, r);
}

// yggr::tuple operator!=
template< typename ...LArgs, typename ...RArgs > inline
bool operator!=(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef boost::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_neq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator<
template< typename ...LArgs, typename ...RArgs > inline
bool operator<(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef boost::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_lt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator<=
template< typename ...LArgs, typename ...RArgs > inline
bool operator<=(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef boost::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_lte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator>
template< typename ...LArgs, typename ...RArgs > inline
bool operator>(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef boost::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_gt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator>=
template< typename ...LArgs, typename ...RArgs > inline
bool operator>=(const tuple<LArgs...>& l, const boost::tuples::tuple<RArgs...>& r)
{
	typedef tuple<LArgs...> l_type;
	typedef boost::tuples::tuple<RArgs...> r_type;

	typedef ::yggr::tuples::detail::tuple_gte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// std::pair operator cmp
//==
template< typename T1, typename T2, typename ...Args> inline
bool operator==(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) == r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator==(const std::pair<T1, T2>& l, const tuple<Args...>& r)
{
	return l == yggr::tuples::get_inherited(r);
}

//!=
template< typename T1, typename T2, typename ...Args> inline
bool operator!=(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) != r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator!=(const std::pair<T1, T2>& l, const tuple<Args...>& r)
{
	return l != yggr::tuples::get_inherited(r);
}

// <
template< typename T1, typename T2, typename ...Args> inline
bool operator<(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) < r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator<(const std::pair<T1, T2>& l, const tuple<Args...>& r)
{
	return l < yggr::tuples::get_inherited(r);
}

// <=
template< typename T1, typename T2, typename ...Args> inline
bool operator<=(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) <= r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator<=(const std::pair<T1, T2>& l, const tuple<Args...>& r)
{
	return l <= yggr::tuples::get_inherited(r);
}

// >
template< typename T1, typename T2, typename ...Args> inline
bool operator>(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) > r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator>(const std::pair<T1, T2>& l, const tuple<Args...>& r)
{
	return l > yggr::tuples::get_inherited(r);
}

// >=
template< typename T1, typename T2, typename ...Args> inline
bool operator>=(const tuple<Args...>& l, const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) >= r;
}

template< typename T1, typename T2, typename ...Args> inline
bool operator>=(const std::pair<T1,T2>& l,const tuple<Args...>& r)
{
	return l >= yggr::tuples::get_inherited(r);
}

} // namespace tuples
} // namespace yggr

#else

namespace boost
{
namespace tuples
{

// operator cmp boost::tuple
template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator==(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_eq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator!=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_neq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator<(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_lt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator<=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_lte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator>(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_gt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator>=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_gte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator cmp

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator==(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_eq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator!=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_neq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator<(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_lt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator<=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_lte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator>(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_gt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator>=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_gte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// std::pair operator cmp
//==
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator==(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) == r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator==(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l == yggr::tuples::get_inherited(r);
}

//!=
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator!=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) != r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator!=(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l != yggr::tuples::get_inherited(r);
}

//<
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator<(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) < r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator<(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l < yggr::tuples::get_inherited(r);
}

//<=
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator<=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) <= r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator<=(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l <= yggr::tuples::get_inherited(r);
}

//>
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator>(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) > r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator>(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l > yggr::tuples::get_inherited(r);
}

//>=
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator>=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) >= r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator>=(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l >= yggr::tuples::get_inherited(r);
}

} // namespace tuples
} // namespace boost

namespace yggr
{
namespace tuples
{

// operator cmp boost::tuple
template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator==(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_eq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator!=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_neq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator<(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_lt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator<=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_lte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator>(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_gt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator>=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_gte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// yggr::tuple operator cmp

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator==(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_eq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator!=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_neq<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator<(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_lt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator<=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_lte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator>(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_gt<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

template
	<
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T1),
		YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T2)
	> inline
bool operator>=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)>& l,
				const boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)>& r)
{
	typedef tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T1)> l_type;
	typedef boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T2)> r_type;

	typedef ::yggr::tuples::detail::tuple_gte<(length<l_type>::value == length<r_type>::value)> cmper_type;

	cmper_type cmper;
	return cmper(l, r);
}

// std::pair operator cmp
//==
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator==(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) == r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator==(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l == yggr::tuples::get_inherited(r);
}

//!=
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator!=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) != r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator!=(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l != yggr::tuples::get_inherited(r);
}

//<
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator<(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) < r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator<(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l < yggr::tuples::get_inherited(r);
}

//<=
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator<=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) <= r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator<=(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l <= yggr::tuples::get_inherited(r);
}

//>
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator>(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) > r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator>(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l > yggr::tuples::get_inherited(r);
}

//>=
template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator>=(const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& l,
				const std::pair<T1, T2>& r)
{
	return yggr::tuples::get_inherited(l) >= r;
}

template< typename T1, typename T2,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T3)> inline
bool operator>=(const std::pair<T1, T2>& l,
				const tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T3)>& r)
{
	return l >= yggr::tuples::get_inherited(r);
}

} // namespace tuples
} // namespace yggr

#endif // #ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

namespace yggr
{
namespace tuples
{

template<typename T>
struct cons_hash_helper;


template<typename HH, typename TT,
			template<typename _HH, typename _TT> class Cons>
struct cons_hash_helper< Cons< HH, TT> >
{
	typedef Cons< HH, TT > value_type;

	inline void operator()(std::size_t& seed, const value_type& val) const
	{
		typedef cons_hash_helper<typename value_type::tail_type> next_helper_type;

		boost::hash_combine(seed, val.get_head());
		next_helper_type h;
		h(seed, val.get_tail());
	}
};

template<typename HH,
			template<typename _HH, typename _TT> class Cons>
struct cons_hash_helper< Cons<HH, null_type> >
{
	typedef Cons<HH, null_type> value_type;

	inline void operator()(std::size_t& seed, const value_type& val) const
	{
		boost::hash_combine(seed, val.get_head());
	}
};

template<>
struct cons_hash_helper< null_type >
{
	typedef null_type value_type;

	inline void operator()(std::size_t& seed, const value_type& ) const
	{
		boost::hash<std::string> hasher;
		seed = hasher(std::string());
	}
};

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

namespace detail
{

template<std::size_t I, std::size_t ISize>
struct std_tuple_hash_helper
{
	template<typename Tuple> inline
	void operator()(std::size_t& seed, const Tuple& tuple) const
	{
		typedef std_tuple_hash_helper<I + 1, ISize> next_helper_type;

		boost::hash_combine(seed, yggr::get<I>(tuple));
		next_helper_type h;
		h(seed, tuple);
	}
};

template<std::size_t ISize>
struct std_tuple_hash_helper<ISize, ISize>
{
	template<typename Tuple> inline
	void operator()(std::size_t& seed, const Tuple& tuple) const
	{
	}
};

} // namespace detail

#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

} // namespace tuples
} // namespace yggr

namespace boost
{

// boost tuple hash
template<typename HH, typename TT>
struct hash< tuples::cons<HH, TT> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< tuples::cons<HH, TT>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef tuples::cons<HH, TT> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef yggr::tuples::cons_hash_helper<value_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename ...Args>
struct hash< tuples::tuple<Args...> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< tuples::tuple<Args...>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef tuples::tuple<Args...> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)


template<typename ...Args>
struct hash< std::tuple<Args...> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< std::tuple<Args...>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
public:
	typedef std::tuple<Args...> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

public:
	inline std::size_t operator()(const value_type& val) const
	{
		typedef
			::yggr::tuples::detail::std_tuple_hash_helper
			<
				0,
				::yggr::tuple_size<value_type>::value
			> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

#endif // YGGR_NO_CXX11_HDR_TUPLE

#else

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T)>
struct hash< tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> value_type;
	typedef const value_type& argument_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<>
struct hash< tuples::null_type >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< tuples::null_type, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef tuples::null_type value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef yggr::tuples::cons_hash_helper<value_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

// yggr tuple hash
template<typename HH, typename TT>
struct hash< yggr::tuples::cons<HH, TT> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< yggr::tuples::cons<HH, TT>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef yggr::tuples::cons<HH, TT> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef yggr::tuples::cons_hash_helper<value_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename ...Args>
struct hash< yggr::tuples::tuple<Args...> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< yggr::tuples::tuple<Args...>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef yggr::tuples::tuple<Args...> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#else

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T)>
struct hash< yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>, std::size_t>
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)> value_type;
	typedef const value_type& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace boost

#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)

#	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
template<typename ...Args>
struct hash< std::tuple<Args...> >
{
public:
	typedef std::tuple<Args...> value_type;

public:
	inline std::size_t operator()(const value_type& val) const
	{
		typedef
			::yggr::tuples::detail::std_tuple_hash_helper
			<
				0,
				::yggr::tuple_size<value_type>::value
			> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

#	endif // #	if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#endif // YGGR_NO_CXX11_HDR_TUPLE

// boost tuple hash
template<typename HH, typename TT>
struct hash< ::boost::tuples::cons<HH, TT> >
{
	typedef ::boost::tuples::cons<HH, TT> type;

	inline std::size_t operator()(const type& val) const
	{
		boost::hash<type> hasher;
		return hasher(val);
	}
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename ...Args>
struct hash< ::boost::tuples::tuple<Args...> >
{
	typedef ::boost::tuples::tuple<Args...> value_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#else

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T)>
struct hash< ::boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> >
{
	typedef ::boost::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> value_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<>
struct hash< ::boost::tuples::null_type >
{
	typedef ::boost::tuples::null_type value_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef yggr::tuples::cons_hash_helper<value_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, val);

		return seed;
	}
};

// yggr tuple hash
template<typename HH, typename TT>
struct hash< ::yggr::tuples::cons<HH, TT> >
{
	typedef ::yggr::tuples::cons<HH, TT> type;

	inline std::size_t operator()(const type& val) const
	{
		boost::hash<type> hasher;
		return hasher(val);
	}
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename ...Args>
struct hash< yggr::tuples::tuple<Args...> >
{
	typedef yggr::tuples::tuple<Args...> value_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#else

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T)>
struct hash< yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)> >
{
	typedef yggr::tuples::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)> value_type;

	inline std::size_t operator()(const value_type& val) const
	{
		typedef typename value_type::inherited cons_type;

		typedef yggr::tuples::cons_hash_helper<cons_type> h_type;

		std::size_t seed = 0;
		h_type h;

		h(seed, static_cast<const cons_type&>(val));

		return seed;
	}
};

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

// swap

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

#endif // __YGGR_TUPLE_EX_TUPLE_HPP__
