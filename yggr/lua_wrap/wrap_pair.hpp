//wrap_pair.hpp

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

#ifndef __YGGR_LUA_WRAP_WRAP_PAIR_HPP__
#define __YGGR_LUA_WRAP_WRAP_PAIR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/pair.hpp>

#include <yggr/type_traits/is_pair.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/lua_wrap/wrap_base_t_maker.hpp>
#include <yggr/lua_wrap/detail/luabind_function_noexcept.hpp>

#include <boost/ref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_class.hpp>

#include <ostream>

namespace yggr
{
namespace lua_wrap
{

template<typename First, typename Second,
			template<typename _F, typename _S> class TPair>
class wrap_pair;

} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{
namespace detail
{

template<typename T>
struct is_wrap_pair
	: public boost::mpl::false_
{
};

template<typename First, typename Second,
			template<typename _F, typename _S> class TPair>
struct is_wrap_pair< wrap_pair<First, Second, TPair> >
	: public boost::mpl::true_
{
};

} // namespace detail
} // namespace lua_wrap
} // namespace yggr

namespace yggr
{
namespace lua_wrap
{

template<typename First, typename Second,
			template<typename _F, typename _S> class TPair = std::pair>
class wrap_pair : public TPair<First, Second>
{
public:
	typedef TPair<First, Second> base_type;
	typedef typename base_type::first_type first_type;
	typedef typename base_type::second_type second_type;

private:
	BOOST_MPL_ASSERT((is_pair<base_type>));

private:
	typedef wrap_pair this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CXX14_CONSTEXPR wrap_pair(void)
	{
		base_type::first = first_type();
		base_type::second = second_type();
	}

	template<typename U1, typename U2> YGGR_CXX14_CONSTEXPR
	wrap_pair(BOOST_RV_REF(U1) nf, BOOST_RV_REF(U2) ns)
	{
		copy_or_move_or_swap(base_type::first, boost::move(nf));
		copy_or_move_or_swap(base_type::second, boost::move(ns));
	}

	template<typename U1, typename U2> YGGR_CXX14_CONSTEXPR
	wrap_pair(const U1& nf, const U2& ns)
	{
		base_type::first = nf;
		base_type::second = ns;
	}

	YGGR_CXX14_CONSTEXPR wrap_pair(BOOST_RV_REF(first_type) nf,
								BOOST_RV_REF(second_type) ns)
	{
		copy_or_move_or_swap(base_type::first, boost::move(nf));
		copy_or_move_or_swap(base_type::second, boost::move(ns));
	}

	YGGR_CXX14_CONSTEXPR wrap_pair(const first_type& nf, const second_type& ns)
	{
		base_type::first = nf;
		base_type::second = ns;
	}

	// gcc14 and later ambiguous UPair<U1, U2> and real_tpl_pair<U1, U2>
	template<typename UPair> YGGR_CXX14_CONSTEXPR
	wrap_pair(BOOST_RV_REF(UPair)right,
				typename boost::enable_if<is_pair<UPair>, mplex::sfinae_type>::type sfinae = 0)
	{
		copy_or_move_or_swap(base_type::first, boost::move(right.first));
		copy_or_move_or_swap(base_type::second, boost::move(right.second));
	}

	template<typename UPair> YGGR_CXX14_CONSTEXPR
	wrap_pair(const UPair& right,
				typename boost::enable_if<is_pair<UPair>, mplex::sfinae_type>::type sfinae = 0)
	{
		base_type::first = right.first;
		base_type::second = right.second;
	}

	YGGR_CXX14_CONSTEXPR wrap_pair(BOOST_RV_REF(base_type) right)
	{
		copy_or_move_or_swap(base_type::first, boost::move(right.first));
		copy_or_move_or_swap(base_type::second, boost::move(right.second));
	}

	YGGR_CXX14_CONSTEXPR wrap_pair(const base_type& right)
	{
		base_type::first = right.first;
		base_type::second = right.second;
	}

	template<typename U1, typename U2,
				template<typename _T1, typename _T2> class UPair> YGGR_CXX14_CONSTEXPR
	wrap_pair(BOOST_RV_REF_BEG wrap_pair<U1, U2, UPair> BOOST_RV_REF_END right)
	{
		copy_or_move_or_swap(base_type::first, boost::move(right.first));
		copy_or_move_or_swap(base_type::second, boost::move(right.second));
	}

	template<typename U1, typename U2,
				template<typename _T1, typename _T2> class UPair> YGGR_CXX14_CONSTEXPR
	wrap_pair(const wrap_pair<U1, U2, UPair>& right)
	{
		base_type::first = right.first;
		base_type::second = right.second;
	}

	YGGR_CXX14_CONSTEXPR wrap_pair(BOOST_RV_REF(this_type) right)
	{
		copy_or_move_or_swap(base_type::first, boost::move(right.first));
		copy_or_move_or_swap(base_type::second, boost::move(right.second));
	}

	YGGR_CXX14_CONSTEXPR wrap_pair(const this_type& right)
	{
		base_type::first = right.first;
		base_type::second = right.second;
	}

	~wrap_pair(void)
	{
	}

public:
	//template<typename U1, typename U2,
	//			template<typename _T1, typename _T2> class UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
	//this_type& operator=(BOOST_RV_REF_BEG UPair<U1, U2> BOOST_RV_REF_END right)
	//{
	//	copy_or_move_or_swap(base_type::first, boost::move(right.first));
	//	copy_or_move_or_swap(base_type::second, boost::move(right.second));
	//	return *this;
	//}

	//template<typename U1, typename U2,
	//			template<typename _T1, typename _T2> class UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
	//this_type& operator=(const UPair<U1, U2>& right)
	//{
	//	base_type::first = right.first;
	//	base_type::second = right.second;
	//	return *this;
	//}

	template<typename UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_pair<UPair>, this_type&>::type
		operator=(BOOST_RV_REF(UPair) right)
	{
		copy_or_move_or_swap(base_type::first, boost::move(right.first));
		copy_or_move_or_swap(base_type::second, boost::move(right.second));
		return *this;
	}

	template<typename UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename boost::enable_if<is_pair<UPair>, this_type&>::type
		operator=(const UPair& right)
	{
		base_type::first = right.first;
		base_type::second = right.second;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(BOOST_RV_REF(base_type) right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		base_type& right_ref = right;
		if(static_cast<base_type*>(this) == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(base_type::first, boost::move(right.first));
		copy_or_move_or_swap(base_type::second, boost::move(right.second));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(const base_type& right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		if(static_cast<base_type*>(this) == boost::addressof(right))
		{
			return *this;
		}

		base_type::first = right.first;
		base_type::second = right.second;
		return *this;
	}

	template<typename U1, typename U2,
				template<typename _T1, typename _T2> class UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
	this_type& operator=(BOOST_RV_REF_BEG wrap_pair<U1, U2, UPair> BOOST_RV_REF_END right)
	{
		copy_or_move_or_swap(base_type::first, boost::move(right.first));
		copy_or_move_or_swap(base_type::second, boost::move(right.second));
		return *this;
	}

	template<typename U1, typename U2,
				template<typename _T1, typename _T2> class UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
	this_type& operator=(const wrap_pair<U1, U2, UPair>& right)
	{
		base_type::first = right.first;
		base_type::second = right.second;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		base_type::first = right.first;
		base_type::second = right.second;
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type& operator=(const this_type& right) YGGR_NOEXCEPT_OR_NOTHROW
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		base_type::first = right.first;
		base_type::second = right.second;
		return *this;
	}

public:
	void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(base_type& right)
	{
		if(static_cast<base_type*>(this) == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(base_type::first, right.first);
		::yggr::swap(base_type::second, right.second);
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(base_type::first, right.first);
		::yggr::swap(base_type::second, right.second);
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE base_type& unwrap(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return static_cast<base_type&>(*this);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE const base_type& unwrap(void) const YGGR_NOEXCEPT_OR_NOTHROW
	{
		return static_cast<const base_type&>(*this);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE this_type copy(void) const
	{
		return *this;
	}

public:
	YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::mpl::if_
		<
			boost::is_class<first_type>,
			const first_type&,
			first_type
		>::type
		get_first(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return base_type::first;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::mpl::if_
		<
			boost::is_class<first_type>,
			const second_type&,
			second_type
		>::type
		get_second(void) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return base_type::second;
	}

	template<typename U1> YGGR_CXX14_CONSTEXPR_OR_INLINE
	this_type& set_first(const U1& val)
	{
		base_type::first = val;
		return *this;
	}

	template<typename U2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	this_type& set_second(const U2& val)
	{
		base_type::second = val;
		return *this;
	}

public:
	// compare_eq
	template<typename UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			::yggr::is_pair<UPair>,
			bool
		>::type
		compare_eq(const UPair& right) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return base_type::first == right.first && base_type::second == right.second;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE bool compare_eq(const base_type& right) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return
			static_cast<const base_type*>(this) == boost::addressof(right)
			|| (base_type::first == right.first
				&& base_type::second == right.second);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE bool compare_eq(const this_type& right) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare_eq(right.unwrap());
	}

	// compare
	template<typename UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			::yggr::is_pair<UPair>,
			s32
		>::type
		compare(const UPair& right) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return
			base_type::first == right.first?
				base_type::second == right.second?
					0
					: base_type::second < right.second? -1 : 1
				: base_type::first < right.first? -1 : 1;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE s32 compare(const base_type& right) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return
			static_cast<const base_type*>(this) == boost::addressof(right)?
			0
			: base_type::first == right.first?
				base_type::second == right.second?
					0
					: base_type::second < right.second? -1 : 1
				: base_type::first < right.first? -1 : 1;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE s32 compare(const this_type& right) const YGGR_LUABIND_FUNC_NOEXCEPT_OR_NOTHROW
	{
		return this_type::compare(right.unwrap());
	}
};

// non-member_foo

//==
template<template<typename _U1, typename _U2> class UPair,
			typename TPair, typename U1, typename U2 > YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< TPair >,
		bool
	>::type
	operator==(const TPair& l, const wrap_pair<U1, U2, UPair>& r)
{
	return r.compare_eq(l);
}

template<template<typename _T1, typename _T2> class TPair, 
			typename T1, typename T2, typename UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< UPair >,
		bool
	>::type
	operator==(const wrap_pair<T1, T2, TPair>& l, const UPair& r)
{
	return l.compare_eq(r);
}

template<typename T1, typename T2, template<typename _T1, typename _T2> class TPair,
			typename U1, typename U2, template<typename _U1, typename _U2> class UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator==(const wrap_pair<T1, T2, TPair>& l, const wrap_pair<U1, U2, UPair>& r)
{
	return l.compare_eq(r);
}

// !=
template<template<typename _U1, typename _U2> class UPair,
			typename TPair, typename U1, typename U2 > YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< TPair >,
		bool
	>::type
	operator!=(const TPair& l, const wrap_pair<U1, U2, UPair>& r)
{
	return !r.compare_eq(l);
}

template<template<typename _T1, typename _T2> class TPair, 
			typename T1, typename T2, typename UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< UPair >,
		bool
	>::type
	operator!=(const wrap_pair<T1, T2, TPair>& l, const UPair& r)
{
	return !l.compare_eq(r);
}

template<typename T1, typename T2, template<typename _T1, typename _T2> class TPair,
			typename U1, typename U2, template<typename _U1, typename _U2> class UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
bool operator!=(const wrap_pair<T1, T2, TPair>& l, const wrap_pair<U1, U2, UPair>& r)
{
	return !l.compare_eq(r);
}

// <
template<template<typename _U1, typename _U2> class UPair,
			typename TPair, typename U1, typename U2 > YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< TPair >,
		bool
	>::type
	operator<(const TPair& l, const wrap_pair<U1, U2, UPair>& r)
{
	return 0 < r.compare(l);
}

template<template<typename _T1, typename _T2> class TPair, 
			typename T1, typename T2, typename UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< UPair >,
		bool
	>::type
	operator<(const wrap_pair<T1, T2, TPair>& l, const UPair& r)
{
	return l.compare(r) < 0;
}

template<typename T1, typename T2, template<typename _T1, typename _T2> class TPair,
			typename U1, typename U2, template<typename _U1, typename _U2> class UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
s32 operator<(const wrap_pair<T1, T2, TPair>& l, const wrap_pair<U1, U2, UPair>& r)
{
	return l.compare(r) < 0;
}

// <=
template<template<typename _U1, typename _U2> class UPair,
			typename TPair, typename U1, typename U2 > YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< TPair >,
		bool
	>::type
	operator<=(const TPair& l, const wrap_pair<U1, U2, UPair>& r)
{
	return 0 <= r.compare(l);
}

template<template<typename _T1, typename _T2> class TPair, 
			typename T1, typename T2, typename UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< UPair >,
		bool
	>::type
	operator<=(const wrap_pair<T1, T2, TPair>& l, const UPair& r)
{
	return l.compare(r) <= 0;
}


template<typename T1, typename T2, template<typename _T1, typename _T2> class TPair,
			typename U1, typename U2, template<typename _U1, typename _U2> class UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
s32 operator<=(const wrap_pair<T1, T2, TPair>& l, const wrap_pair<U1, U2, UPair>& r)
{
	return l.compare(r) <= 0;
}

// >
template<template<typename _U1, typename _U2> class UPair,
			typename TPair, typename U1, typename U2 > YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< TPair >,
		bool
	>::type
	operator>(const TPair& l, const wrap_pair<U1, U2, UPair>& r)
{
	return r.compare(l) < 0;
}

template<template<typename _T1, typename _T2> class TPair, 
			typename T1, typename T2, typename UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< UPair >,
		bool
	>::type
	operator>(const wrap_pair<T1, T2, TPair>& l, const UPair& r)
{
	return l.compare(r) > 0;
}

template<typename T1, typename T2, template<typename _T1, typename _T2> class TPair,
			typename U1, typename U2, template<typename _U1, typename _U2> class UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
s32 operator>(const wrap_pair<T1, T2, TPair>& l, const wrap_pair<U1, U2, UPair>& r)
{
	return l.compare(r) > 0;
}

// >=
template<template<typename _U1, typename _U2> class UPair,
			typename TPair, typename U1, typename U2 > YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< TPair >,
		bool
	>::type
	operator>=(const TPair& l, const wrap_pair<U1, U2, UPair>& r)
{
	return r.compare(l) <= 0;
}

template<template<typename _T1, typename _T2> class TPair, 
			typename T1, typename T2, typename UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::enable_if
	<
		is_pair< UPair >,
		bool
	>::type
	operator>=(const wrap_pair<T1, T2, TPair>& l, const UPair& r)
{
	return l.compare(r) >= 0;
}

template<typename T1, typename T2, template<typename _T1, typename _T2> class TPair,
			typename U1, typename U2, template<typename _U1, typename _U2> class UPair> YGGR_CXX14_CONSTEXPR_OR_INLINE
s32 operator>=(const wrap_pair<T1, T2, TPair>& l, const wrap_pair<U1, U2, UPair>& r)
{
	return l.compare(r) >= 0;
}


// <<
template<typename Char, typename Traits, typename T1, typename T2> YGGR_CXX14_CONSTEXPR_OR_INLINE
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const yggr::lua_wrap::wrap_pair<T1, T2>& right)
{
	os << "[ " << (right.first) << ", " << (right.second) << " ]";
	return os;
}

template<template<typename _T1, typename _T2> class TPair, typename T1, typename T2> YGGR_CXX14_CONSTEXPR_OR_INLINE
yggr::lua_wrap::wrap_pair<T1, T2, TPair>
	make_pair(const T1& f, const T2& s)
{
	typedef yggr::lua_wrap::wrap_pair<T1, T2, TPair> now_pair_type;
	return now_pair_type(f, s);
}

} // namespace lua_wrap
} // namespace yggr


// swap
namespace yggr
{
namespace lua_wrap
{
namespace swap_support
{

template<template<typename _T1, typename _T2> class TPair,
			typename T1, typename T2 > YGGR_CXX14_CONSTEXPR_OR_INLINE
void swap(typename wrap_pair<T1, T2, TPair>::base_type& l, wrap_pair<T1, T2, TPair>& r)
{
	r.swap(l);
}

template<template<typename _T1, typename _T2> class TPair,
			typename T1, typename T2 > YGGR_CXX14_CONSTEXPR_OR_INLINE
void swap(wrap_pair<T1, T2, TPair>& l, typename wrap_pair<T1, T2, TPair>::base_type& r)
{
	l.swap(r);
}

template<template<typename _T1, typename _T2> class TPair,
			typename T1, typename T2 > YGGR_CXX14_CONSTEXPR_OR_INLINE
void swap(wrap_pair<T1, T2, TPair>& l, wrap_pair<T1, T2, TPair>& r)
{
	l.swap(r);
}

} // namwspace swap_support

using swap_support::swap;

} // namespace lua_wrap
} // namespace yggr

namespace std
{
	using ::yggr::lua_wrap::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::lua_wrap::swap_support::swap;
} // namespace boost


#endif //__YGGR_LUA_WRAP_WRAP_PAIR_HPP__
