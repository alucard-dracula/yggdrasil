//packet_info_vtpl.hpp

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

#ifndef __YGGR_NETWORK_PACKET_INFO_VTPL_HPP__
#define __YGGR_NETWORK_PACKET_INFO_VTPL_HPP__

#ifndef __YGGR_NETWORK_PACKET_INFO_HPP__
#	error "please include packet_info.hpp"
#endif // __YGGR_NETWORK_PACKET_INFO_HPP__

#ifdef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	error "this file is using for cxx11 variadic temples, please check environment."
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/ppex/foo_params.hpp>

#include <yggr/func/foo_t_maker.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/conv_to_tuple.hpp>

#include <yggr/tuple/tuple.hpp>
#include <yggr/tuple_ex/tuple.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/tuple.hpp>

#include <boost/functional/hash/hash.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/long.hpp>

#include <boost/type_traits/is_same.hpp>

#include <sstream>
#include <utility>

namespace yggr
{
namespace packet
{

class packet_info_op;

} // namespace packet
} // namespace yggr

namespace yggr
{
namespace packet
{

template<typename ...Val>
class packet_info
	: public
		mplex::conv_to_yggr_tuple
		<
			typename
				boost::mpl::erase
				<
					boost::mpl::vector <Val...>,
					typename
						boost::mpl::find
						<
							boost::mpl::vector <Val...>,
							yggr::tuples::null_type
						>::type,
					typename
						boost::mpl::end
						<
							boost::mpl::vector <Val...>
						>::type
				>::type
		>::type
{
private:
	typedef yggr::tuples::null_type null_type;
	typedef boost::mpl::vector <Val...> tmp_vt_t_type;
	typedef typename boost::mpl::find<tmp_vt_t_type, null_type>::type i;
	typedef typename
		boost::mpl::erase
		<
			tmp_vt_t_type,
			i,
			typename boost::mpl::end<tmp_vt_t_type>::type
		>::type vt_t_type;
public:
	YGGR_STATIC_CONSTANT(size_type, E_length = boost::mpl::size<vt_t_type>::value);

public:
	typedef vt_t_type params_type;
	typedef typename mplex::conv_to_yggr_tuple<params_type, null_type>::type value_type;
	typedef value_type base_type;

	template<size_type N>
	struct arg
		: public mplex::get_arg<params_type, N, null_type>
	{
		YGGR_STATIC_CONSTANT(size_type, index = N);

		BOOST_MPL_ASSERT((boost::mpl::bool_<(N <E_length)>));
	};

	template<typename T, yggr::size_type idx = static_cast<yggr::size_type>(-1), typename Nil_T = void>
	struct t_arg
		: public arg<idx>
	{
		YGGR_STATIC_CONSTANT(size_type, index = idx);
	};

	template<typename T, typename Nil_T>
	struct t_arg<T, static_cast<yggr::size_type>(-1), Nil_T>
		: public arg<static_cast<size_type>(boost::mpl::find<vt_t_type, T>::type::pos::value)>
	{
		YGGR_STATIC_CONSTANT(size_type, index = (boost::mpl::find<vt_t_type, T>::type::pos::value));
	};


private:

	template<typename ...T>
	friend class packet_info;

	typedef packet_info this_type;

	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:

	template<typename ...T>
	packet_info(BOOST_FWD_REF(T)... args)
		: base_type(boost::forward<T>(args)...)
	{
		BOOST_MPL_ASSERT((
			boost::mpl::less_equal<boost::mpl::long_<sizeof...(T)>,
			boost::mpl::long_<this_type::E_length> >));
	}

	template<typename ...T>
	packet_info(BOOST_RV_REF(boost::tuple<T...>) right)
		: base_type(right)
	{
	}

	template<typename ...T>
	packet_info(const boost::tuple<T...>& right)
		: base_type(right)
	{
	}

	template<typename ...T>
	packet_info(BOOST_RV_REF(yggr::tuple<T...>) right)
		: base_type(right)
	{
	}

	template<typename ...T>
	packet_info(const yggr::tuple<T...>& right)
		: base_type(right)
	{
	}

	template<typename ...T>
	packet_info(BOOST_RV_REF(packet_info<T...>) right)
		: base_type(
			boost::move(
				static_cast<typename packet_info <T...>::base_type&>(right)) )
	{
	}

	template<typename ...T>
	packet_info(const packet_info<T...>& right)
		: base_type(static_cast<const typename packet_info <T...>::base_type&>(right))
	{
	}

	packet_info( BOOST_RV_REF( this_type ) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	packet_info(const this_type& right)
		: base_type(right)
	{
	}

	~packet_info(void)
	{
	}

public:
	template<typename ...T> inline
	this_type& operator=(BOOST_RV_REF(boost::tuple<T...>) right)
	{
		base_type& base = *this;
		copy_or_move_or_swap(base, boost::move(right));
		return *this;
	}

	template<typename ...T> inline
	this_type& operator=(const boost::tuple<T...>& right)
	{
		base_type::operator=(right);
		return *this;
	}

	template<typename ...T> inline
	this_type& operator=(BOOST_RV_REF(yggr::tuple<T...>) right)
	{
		base_type& base = *this;
		copy_or_move_or_swap(base, boost::move(right));
		return *this;
	}

	template<typename ...T> inline
	this_type& operator=(const yggr::tuple<T...>& right)
	{
		base_type::operator=(right);
		return *this;
	}

	template<typename ...T> inline
	this_type& operator=(BOOST_RV_REF(packet_info<T...>) right)
	{
		typedef packet_info<T...> other_type;
		typedef typename other_type::base_type other_base_type;

		base_type& base = *this;
		other_base_type& other_base = right;

		copy_or_move_or_swap(base, boost::move(other_base));
		return *this;
	}

	template<typename ...T> inline
	this_type& operator=(const packet_info<T...>& right)
	{
		typedef packet_info<T...> other_type;
		typedef typename other_type::base_type other_base_type;

		const other_base_type& other_base = right;
		base_type::operator=(other_base);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type& base = *this;
		base_type& rbase = right;

		copy_or_move_or_swap(base, boost::move(rbase));

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type::operator=(right);
		return *this;
	}

public:
	template<typename ...T> inline
	void swap(BOOST_RV_REF(boost::tuple<T...>) right)
	{
		base_type::swap(right);
	}

	template<typename ...T> inline
	void swap(boost::tuple<T...>& right)
	{
		base_type::swap(right);
	}

	template<typename ...T> inline
	void swap(BOOST_RV_REF(yggr::tuple<T...>) right)
	{
		base_type::swap(right);
	}

	template<typename ...T> inline
	void swap(yggr::tuple<T...>& right)
	{
		base_type::swap(right);
	}

	template<typename ...T> inline
	void swap(BOOST_RV_REF(packet_info<T...>) right)
	{
		typedef packet_info<T...> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename ...T> inline
	void swap(packet_info<T...>& right)
	{
		typedef packet_info<T...> other_type;
		typedef typename other_type::base_type other_base_type;

		base_type::swap(static_cast<other_base_type&>(right));
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		base_type::swap(right);
	}

public:
	template<typename ...T> inline
	operator ::boost::tuple<T...>(void) const
	{
		typedef ::boost::tuple<T...> ret_type;

		const base_type& base = *this;
		return ret_type(base);
	}

public:
	inline void clear(void)
	{
		base_type& base = *this;
		value_type().swap(base);
	}

	inline static size_type element_size(void)
	{
		return this_type::E_length;
	}

	template<size_type N>inline
	typename arg<N>::type& get(void)
	{
		base_type& base = *this;
		return yggr::get<N>(base);
	}

	template<size_type N>inline
	const typename arg<N>::type& get(void) const
	{
		const base_type& base = *this;
		return yggr::get<N>(base);
	}

	template<typename T>inline
	typename t_arg<T>::type& get(void)
	{
		typedef t_arg<T> t_arg_type;

		base_type& base = *this;
		return yggr::get<t_arg_type::index>(base);
	}

	template<typename T> inline
	const typename t_arg<T>::type& get(void) const
	{
		typedef t_arg<T> t_arg_type;

		const base_type& base = *this;
		return yggr::get<t_arg_type::index>(base);
	}

	template<typename Arg> inline
	typename Arg::type& arg_get(void)
	{
		typedef Arg arg_type;

		base_type& base = *this;
		return yggr::get<arg_type::index>(base);
	}

	template<typename Arg> inline
	const typename Arg::type& arg_get(void) const
	{
		typedef Arg arg_type;

		const base_type& base = *this;
		return yggr::get<arg_type::index>(base);
	}

	template<typename T_Arg> inline
	typename T_Arg::type& t_arg_get(void)
	{
		typedef T_Arg t_arg_type;

		base_type& base = *this;
		return yggr::get<t_arg_type::index>(base);
	}

	template<typename T_Arg> inline
	const typename T_Arg::type& t_arg_get(void) const
	{
		typedef T_Arg t_arg_type;

		const base_type& base = *this;
		return yggr::get<t_arg_type::index>(base);
	}

public:
	// compare, compare_eq
#define __YGGR_PP_TMP_PACKET_INFO_COMPARE_FOOS( __foo_name__ ) \
	template<typename ...T> inline \
	bool __foo_name__(const boost::tuple<T...>& right) const { \
		const base_type& l = *this; \
		return l == right; } \
	\
	template<typename ...T> inline \
	bool __foo_name__(const yggr::tuple<T...>& right) const { \
		const base_type& l = *this; \
		return l == right; } \
	\
	template<typename ...T> inline \
	bool __foo_name__(const packet_info<T...>& right) const { \
		typedef packet_info<T...> right_type; \
		typedef typename right_type::base_type right_base_type; \
		const base_type& l = *this; \
		const right_base_type& r = right; \
		return l == r; } \
	\
	inline bool __foo_name__(const this_type& right) const { \
		const base_type& l = *this; \
		const base_type& r = right; \
		return l == r; }

	__YGGR_PP_TMP_PACKET_INFO_COMPARE_FOOS(compare_eq)
	__YGGR_PP_TMP_PACKET_INFO_COMPARE_FOOS(compare)

#undef __YGGR_PP_TMP_PACKET_INFO_COMPARE_FOOS

	template<typename ...Args> inline
	static this_type make_packet_info(BOOST_FWD_REF(Args)... args)
	{
		BOOST_MPL_ASSERT((
			boost::mpl::less_equal<boost::mpl::long_<sizeof...(Args)>,
			boost::mpl::long_<this_type::E_length> >));
		return this_type(boost::forward<Args>(args)...);
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 ver)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"packet_info_data",
				boost::serialization::base_object<base_type>(*this));
	}

};

} // namespace packet
} // namespace yggr

namespace yggr
{
namespace packet
{

template<typename T>
struct cast_to_packet_info;

template<template<typename ..._T> class Src, typename ...T>
struct cast_to_packet_info< Src<T...> >
{
	typedef packet_info<T...> type;
};

} // namespace packet
} // naemspace yggr

namespace yggr
{
namespace packet
{

// operator ==
template<typename ...T1, typename ...T2 > inline
bool operator==(const boost::tuple<T1...>& l, const packet_info<T2...>& r)
{
	return r.compare_eq(l);
}

template<typename ...T1, typename ...T2 > inline
bool operator==(const packet_info<T1...>& l, const boost::tuple<T2...>& r)
{
	return l.compare_eq(r);
}

template<typename ...T1, typename ...T2> inline
bool operator==(const yggr::tuple<T1...>& l, const packet_info<T2...>& r)
{
	return r.compare_eq(l);
}

template<typename ...T1, typename ...T2> inline
bool operator==(const packet_info<T1...>& l, const yggr::tuple<T2...>& r)
{
	return l.compare_eq(r);
}

template<typename ...T1, typename ...T2> inline
bool operator==(const packet_info<T1...>& l, const packet_info<T2...>& r)
{
	return l.compare_eq(r);
}

template<typename ...T> inline
bool operator==(const packet_info<T...>& l, const packet_info<T...>& r)
{
	return l.compare_eq(r);
}

// operator!=

template<typename ...T1, typename ...T2 > inline
bool operator!=(const boost::tuple<T1...>& l, const packet_info<T2...>& r)
{
	return !r.compare_eq(l);
}

template<typename ...T1, typename ...T2 > inline
bool operator!=(const packet_info<T1...>& l, const boost::tuple<T2...>& r)
{
	return !l.compare_eq(r);
}

template<typename ...T1, typename ...T2> inline
bool operator!=(const yggr::tuple<T1...>& l, const packet_info<T2...>& r)
{
	return !r.compare_eq(l);
}

template<typename ...T1, typename ...T2> inline
bool operator!=(const packet_info<T1...>& l, const yggr::tuple<T2...>& r)
{
	return !l.compare_eq(r);
}

template<typename ...T1, typename ...T2> inline
bool operator!=(const packet_info<T1...>& l, const packet_info<T2...>& r)
{
	return !l.compare_eq(r);
}

template<typename ...T> inline
bool operator!=(const packet_info<T...>& l, const packet_info<T...>& r)
{
	return !l.compare_eq(r);
}

// io

template<typename Char, typename Traits, typename ...T>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const yggr::packet::packet_info<T...>& right)
{
	typedef yggr::packet::packet_info<T...> info_type;
	typedef typename info_type::value_type info_value_type;
	const info_value_type& cref_val = right;

	os <<"packet_info" <<cref_val;
	return os;
}


template<typename Char, typename Traits, typename ...T>
std::basic_istream<Char, Traits>&
	operator>>(std::basic_istream<Char, Traits>& is, yggr::packet::packet_info<T...>& right)
{
	typedef yggr::packet::packet_info <T...> info_type;
	typedef typename info_type::value_type info_value_type;

	info_value_type& ref_val = right;

	is>> ref_val;

	return is;
}

} // namespace packet
} // namespace yggr

// foo make_packet_info
namespace yggr
{
namespace packet
{
namespace detail
{

using boost::tuples::make_tuple_traits;

template<typename ...T>
struct make_packet_info_mapper
{
	typedef packet_info<typename make_tuple_traits<T>::type ...> type;
};

} // namespace detail

template<typename ...T> inline
typename detail::make_packet_info_mapper<T...>::type
	make_packet_info(const T& ...args)
{
	typedef typename detail::make_packet_info_mapper<T...>::type ret_type;
	return ret_type(args...);
}

} // namespace packet
} // namespace yggr


namespace yggr
{
namespace packet
{
namespace swap_support
{

// boost::tuple
template<typename ...LT, typename ...RT> inline
void swap(::boost::tuple<LT...>& l, packet_info<RT...>& r)
{
	r.swap(l);
}

template<typename ...LT, typename ...RT> inline
void swap(packet_info<LT...>& l, ::boost::tuple<RT...>& r)
{
	l.swap(r);
}

template<typename ...T> inline
void swap(::boost::tuple<T...>& l,
			packet_info<T...>& r)
{
	r.swap(l);
}

template<typename ...T> inline
void swap(packet_info<T...>& l,
			::boost::tuple<T...>& r)
{
	l.swap(r);
}

// ::yggr::tuple
template<typename ...LT, typename ...RT> inline
void swap(::yggr::tuple<LT...>& l, packet_info<RT...>& r)
{
	r.swap(l);
}

template<typename ...LT, typename ...RT> inline
void swap(packet_info<LT...>& l, ::yggr::tuple<RT...>& r)
{
	l.swap(r);
}

template<typename ...T> inline
void swap(::yggr::tuple<T...>& l, packet_info<T...>& r)
{
	r.swap(l);
}

template<typename ...T> inline
void swap(packet_info<T...>& l, ::yggr::tuple<T...>& r)
{
	l.swap(r);
}

// packet_info
template<typename ...LT, typename ...RT> inline
void swap(packet_info<LT...>& l, packet_info<RT...>& r)
{
	l.swap(r);
}

template<typename ...T> inline
void swap(packet_info<T...>& l, packet_info<T...>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace packet
} // namespace yggr

namespace std
{
	using ::yggr::packet::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::packet::swap_support::swap;
} // namespace boost

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace packet
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename ...T> inline
std::size_t hash_value(const ::yggr::packet::packet_info<T...>& info)
{
	typedef yggr::packet::packet_info<T...> packet_info_type;
	typedef typename packet_info_type::value_type val_type;

	const val_type& val_cref = info;
	boost::hash<val_type> hasher;
	return hasher(val_cref);
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace packet
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

template<typename ...T>
struct hash< ::yggr::packet::packet_info<T...> >
{
	typedef ::yggr::packet::packet_info<T...> type;

	inline std::size_t operator()(const type& info) const
	{
		typedef typename type::value_type val_type;

		const val_type& val_cref = info;
		::boost::hash<val_type> hasher;
		return hasher(val_cref);
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace packet
{

class packet_info_op
	: private nonable::noncreateable
{

public:
	// compare

	template<size_type ...Idx, typename PakInfo, typename ...Args> inline
	static
	typename
		boost::enable_if_c
		<
			((sizeof...(Idx)) && (sizeof...(Idx) == sizeof...(Args))),
			bool
		>::type
		compare(const PakInfo& pak_info, const Args&... args)
	{
		return yggr::make_tuple((pak_info.template get<Idx>())...) == yggr::make_tuple(args...);
	}

	template<size_type ...Idx, typename Handler, typename PakInfo, typename ...Args> inline
	static
	typename
		boost::enable_if_c
		<
			((sizeof...(Idx))
				&& (sizeof...(Idx) == sizeof...(Args))
				&& func::foo_t_info<Handler>::is_callable_type::value ),
			typename func::foo_t_info<Handler>::result_type
		>::type
		compare(const Handler& handler, const PakInfo& pak_info, const Args&... args)
	{
		return handler(yggr::make_tuple((pak_info.template get<Idx>())...), yggr::make_tuple(args...));
	}

	//get_sub_infos
	template<size_type ...Idx, typename PakInfo> inline
	static ::yggr::tuple<typename PakInfo::template arg<Idx>::type ...>
		get_sub_infos(const PakInfo& info)
	{
		return yggr::make_tuple(info.template get<Idx>()...);
	}

	//get_sub_info
	template<size_type ...Idx, typename PakInfo> inline
	static ::yggr::packet::packet_info< typename PakInfo::template arg<Idx>::type ...>
		get_sub_packet_info(const PakInfo& info)
	{
		typedef ::yggr::packet::packet_info<typename PakInfo::template arg<Idx>::type ...> result_type;
		return yggr::packet::make_packet_info(info.template get<Idx>()...);
	}

};

} // namespace packet
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

#endif // __YGGR_NETWORK_PACKET_INFO_VTPL_HPP__
