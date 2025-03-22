//packet_info_macro.hpp

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

#ifndef __YGGR_NETWORK_PACKET_INFO_VMACRO_HPP__
#define __YGGR_NETWORK_PACKET_INFO_VMACRO_HPP__

#ifndef __YGGR_NETWORK_PACKET_INFO_HPP__
#	error "please include packet_info.hpp"
#endif // __YGGR_NETWORK_PACKET_INFO_HPP__

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	error "this file is using for no cxx11 variadic temples, please check environment."
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

#ifndef YGGR_PP_PACKET_INFO_PARAMS_LEN
#	define YGGR_PP_PACKET_INFO_PARAMS_LEN YGGR_TUPLE_LIMIT_LENGTH
#endif // YGGR_PP_PACKET_INFO_PARAMS_LEN

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

#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() ::yggr::tuples::null_type
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename Val ) >
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
class packet_info
	: public
		mplex::conv_to_yggr_tuple
		<
			typename
				boost::mpl::erase
				<
					boost::mpl::vector
					<
						YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), Val)
					>,
					typename
						boost::mpl::find
						<
							boost::mpl::vector
							<
								YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), Val)
							>,
							yggr::tuples::null_type
						>::type,
					typename
						boost::mpl::end
						<
							boost::mpl::vector
							<
								YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), Val)
							>
						>::type
				>::type
		>::type
{
private:
	typedef yggr::tuples::null_type null_type;
	typedef
		boost::mpl::vector
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), Val)
		> tmp_vt_t_type;
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

		BOOST_MPL_ASSERT((boost::mpl::bool_< (N < E_length) >));
	};

	template<typename T, yggr::size_type idx = static_cast<yggr::size_type>(-1), typename Nil_T = void>
	struct t_arg
		: public arg< idx >
	{
		YGGR_STATIC_CONSTANT(size_type, index = idx);
	};

	template<typename T, typename Nil_T>
	struct t_arg<T, static_cast<yggr::size_type>(-1), Nil_T>
		: public arg< static_cast<size_type>(boost::mpl::find<vt_t_type, T>::type::pos::value) >
	{
		YGGR_STATIC_CONSTANT(size_type, index = (boost::mpl::find<vt_t_type, T>::type::pos::value));
	};

private:

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T) >
	friend class packet_info;

	typedef packet_info this_type;

	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:

#	define BOOST_PP_LOCAL_MACRO(__n__) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		packet_info( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: base_type( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ) { \
				BOOST_MPL_ASSERT((boost::mpl::less_equal< boost::mpl::long_< __n__ >, boost::mpl::long_<this_type::E_length> >)); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_PACKET_INFO_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME


	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T) >
	packet_info(BOOST_RV_REF(boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)>) right)
		: base_type(right)
	{
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T) >
	packet_info(const boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)>& right)
		: base_type(right)
	{
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T) >
	packet_info(BOOST_RV_REF(yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>) right)
		: base_type(right)
	{
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T) >
	packet_info(const yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>& right)
		: base_type(right)
	{
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T) >
	packet_info(BOOST_RV_REF(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)>) right)
		: base_type(
			boost::move(
				static_cast
				<
					typename
						packet_info
						<
							YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)
						>::base_type&
				>(right)) )
	{
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T) >
	packet_info(const packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)>& right)
		: base_type(
				static_cast
				<
					const
						typename
							packet_info
							<
								YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)
							>::base_type&
				>(right))
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
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T) > inline
	this_type& operator=(BOOST_RV_REF(boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)>) right)
	{
		base_type& base = *this;
		copy_or_move_or_swap(base, boost::move(right));
		return *this;
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T) > inline
	this_type& operator=(const boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)>& right)
	{
		base_type::operator=(right);
		return *this;
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T) > inline
	this_type& operator=(BOOST_RV_REF(yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>) right)
	{
		base_type& base = *this;
		copy_or_move_or_swap(base, boost::move(right));
		return *this;
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T) > inline
	this_type& operator=(const yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>& right)
	{
		base_type::operator=(right);
		return *this;
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T) > inline
	this_type& operator=(BOOST_RV_REF(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)>) right)
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)> other_type;
		typedef typename other_type::base_type other_base_type;

		base_type& base = *this;
		other_base_type& other_base = right;

		copy_or_move_or_swap(base, boost::move(other_base));
		return *this;
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T) > inline
	this_type& operator=(const packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)>& right)
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)> other_type;
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
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T) > inline
	void swap(BOOST_RV_REF(boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)>) right)
	{
		base_type::swap(right);
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T) > inline
	void swap(boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)>& right)
	{
		base_type::swap(right);
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T) > inline
	void swap(BOOST_RV_REF(yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>) right)
	{
		base_type::swap(right);
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T) > inline
	void swap(yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>& right)
	{
		base_type::swap(right);
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T) > inline
	void swap(BOOST_RV_REF(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)>) right)
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)> right_type;

		right_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T) > inline
	void swap(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)>& right)
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)> other_type;
		typedef typename other_type::base_type other_base_type;

		other_base_type& right_base = right;
		base_type::swap(right_base);
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
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T) > inline
	operator ::boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)>(void) const
	{
		typedef ::boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T)> ret_type;

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
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), typename T) > inline \
	bool __foo_name__(const boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), T) >& right) const { \
		const base_type& l = *this; \
		return l == right; } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), typename T) > inline \
	bool __foo_name__(const yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_TUPLE_LIMIT_LENGTH(), T)>& right) const { \
		const base_type& l = *this; \
		return l == right; } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T) > inline \
	bool __foo_name__(const packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)>& right) const { \
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)> right_type; \
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

	inline static this_type make_packet_info(void)
	{
		return this_type();
	}

#define BOOST_PP_LOCAL_MACRO(__n__) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	static this_type make_packet_info( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
		BOOST_MPL_ASSERT((boost::mpl::less_equal< boost::mpl::long_< __n__ >, boost::mpl::long_<this_type::E_length> >)); \
		return this_type( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_PACKET_INFO_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 ver)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"packet_info_data",
				boost::serialization::base_object< base_type >(*this));
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

#define YGGR_PP_CAST_TO_PACKET_INFO_IMPL( __n__ ) \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T), \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename _T)> class Src > \
	struct cast_to_packet_info< Src< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > > { \
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T)> type; };


#define BOOST_PP_LOCAL_MACRO( __n__ ) \
			YGGR_PP_CAST_TO_PACKET_INFO_IMPL( __n__ )

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#include BOOST_PP_LOCAL_ITERATE( )


#undef YGGR_PP_CAST_TO_PACKET_INFO_IMPL

} // namespace packet
} // naemspace yggr

namespace yggr
{
namespace packet
{

//_YGGR_TMP_PP_PACKET_OPERATOR_COMPARE(YGGR_PP_PACKET_INFO_PARAMS_LEN())

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename T1 ),
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T2 ) > inline
bool operator==(const boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), T1 ) >& l,
					const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T2 ) >& r)
{
	return r.compare_eq(l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename T1 ),
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T2 ) > inline
bool operator==(const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), T1 ) >& l,
					const boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T2 ) >& r)
{
	return l.compare_eq(r);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T1 ),
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T2 ) > inline
bool operator==(const yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T1 ) >& l,
					const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T2 ) >& r)
{
	return r.compare_eq(l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T1 ),
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T2 ) > inline
bool operator==(const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T1 ) >& l,
					const yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T2 ) >& r)
{
	return l.compare_eq(r);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T1 ),
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T2 ) > inline
bool operator==(const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T1 ) >& l,
					const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T2 ) >& r)
{
	return l.compare_eq(r);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T ) > inline
bool operator==(const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T ) >& l,
					const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T ) >& r)
{
	return l.compare_eq(r);
}


template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename T1 ),
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T2 ) > inline
bool operator!=(const boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), T1 ) >& l,
					const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T2 ) >& r)
{
	return !r.compare_eq(l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), typename T1 ),
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T2 ) > inline
bool operator!=(const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), T1 ) >& l,
					const boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T2 ) >& r)
{
	return !l.compare_eq(r);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T1 ),
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T2 ) > inline
bool operator!=(const yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T1 ) >& l,
					const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T2 ) >& r)
{
	return !r.compare_eq(l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T1 ),
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T2 ) > inline
bool operator!=(const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T1 ) >& l,
					const yggr::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T2 ) >& r)
{
	return !l.compare_eq(r);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T1 ),
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T2 ) > inline
bool operator!=(const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T1 ) >& l,
					const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T2 ) >& r)
{
	return !l.compare_eq(r);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), typename T ) > inline
bool operator!=(const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_TUPLE_LIMIT_LENGTH(), T ) >& l,
					const packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T ) >& r)
{
	return !l.compare_eq(r);
}

// io
// now don't use this foo style, but don't delete it
//#define _YGGR_TMP_PP_PACKET_INFO_OUT(__n__) \
//	template<typename Char, typename Traits \
//				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
//				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename Val) \
//				, template<typename _Char, typename _Traits> class Basic_OStream > \
//	Basic_OStream<Char, Traits>& \
//		operator<<(Basic_OStream<Char, Traits>& os, \
//					const yggr::packet::packet_info< \
//								YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, Val)>& right);

template< typename Char, typename Traits
			YGGR_PP_SYMBOL_IF(YGGR_PP_PACKET_INFO_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
			YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T) >
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const yggr::packet::packet_info<
						YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T) >& right)
{
	typedef yggr::packet::packet_info<
							YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)> info_type;
	typedef typename info_type::value_type info_value_type;
	const info_value_type& cref_val = right;

	os << "packet_info" << cref_val;
	return os;
}

template< typename Char, typename Traits
			YGGR_PP_SYMBOL_IF(YGGR_PP_PACKET_INFO_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
			YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T) >
std::basic_istream<Char, Traits>&
	operator>>(std::basic_istream<Char, Traits>& is,
					yggr::packet::packet_info<
						YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T) >& right)
{
	typedef
		yggr::packet::packet_info
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)
		> info_type;
	typedef typename info_type::value_type info_value_type;

	info_value_type& ref_val = right;

	is >> ref_val;

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

#define YGGR_TMP_PP_T_PARAMS_TITLE() T
#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() yggr::tuples::null_type

template<
	YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE(
		YGGR_TUPLE_LIMIT_LENGTH(), typename YGGR_TMP_PP_T_PARAMS_TITLE() ) >

#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
struct make_packet_info_mapper
{
#define YGGR_PP_TEMPLATE_CUSTOM_TYPE( __n__ ) \
	typename make_tuple_traits< BOOST_PP_CAT(YGGR_TMP_PP_T_PARAMS_TITLE(), __n__) >::type

	typedef
		packet_info
		<
			YGGR_PP_TEMPLATE_PARAMS_CUSTOM_TYPES(
				YGGR_PP_PACKET_INFO_PARAMS_LEN(), YGGR_PP_TEMPLATE_PARAMS_TYPES_MAKER )
		> type;

#undef YGGR_PP_TEMPLATE_CUSTOM_TYPE
};

#undef YGGR_TMP_PP_T_PARAMS_TITLE

} // namespace detail

inline packet_info<> make_packet_info(void)
{
	return packet_info<>();
}

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename detail::make_packet_info_mapper<YGGR_PP_FOO_TYPES_GROUP( __n__ )>::type \
		make_packet_info(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_CREF_PARAMS)) { \
			typedef typename detail::make_packet_info_mapper< YGGR_PP_FOO_TYPES_GROUP( __n__ ) >::type ret_type; \
			return ret_type(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_TUPLE_LIMIT_LENGTH() )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

} // namespace packet
} // namespace yggr


namespace yggr
{
namespace packet
{
namespace swap_support
{

// boost::tuple
template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename LT )
			YGGR_PP_SYMBOL_IF(YGGR_PP_PACKET_INFO_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename RT ) > inline
void swap(::boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), LT )>& l,
			packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), RT )>& r)
{
	r.swap(l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename LT )
			YGGR_PP_SYMBOL_IF(YGGR_PP_PACKET_INFO_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename RT ) > inline
void swap(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), LT )>& l,
			::boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), RT )>& r)
{
	l.swap(r);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T ) > inline
void swap(::boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T )>& l,
			packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T )>& r)
{
	r.swap(l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T ) > inline
void swap(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T )>& l,
			::boost::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T )>& r)
{
	l.swap(r);
}

// ::yggr::tuple
template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename LT )
			YGGR_PP_SYMBOL_IF(YGGR_PP_PACKET_INFO_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename RT ) > inline
void swap(::yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), LT )>& l,
			packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), RT )>& r)
{
	r.swap(l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename LT )
			YGGR_PP_SYMBOL_IF(YGGR_PP_PACKET_INFO_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename RT ) > inline
void swap(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), LT )>& l,
			::yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), RT )>& r)
{
	l.swap(r);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T ) > inline
void swap(::yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T )>& l,
			packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T )>& r)
{
	r.swap(l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T ) > inline
void swap(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T )>& l,
			::yggr::tuple<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T )>& r)
{
	l.swap(r);
}

// packet_info
template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename LT )
			YGGR_PP_SYMBOL_IF(YGGR_PP_PACKET_INFO_PARAMS_LEN(), YGGR_PP_SYMBOL_COMMA)
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename RT ) > inline
void swap(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), LT )>& l,
			packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), RT )>& r)
{
	l.swap(r);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T ) > inline
void swap(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T )>& l,
			packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), T )>& r)
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

template<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T )> inline
std::size_t hash_value(
	const ::yggr::packet::packet_info <
				YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)>& info)
{
	typedef
		yggr::packet::packet_info
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)
		> packet_info_type;
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

template<YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_PACKET_INFO_PARAMS_LEN(), typename T )>
struct hash< ::yggr::packet::packet_info<
					YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)> >
{
	typedef
		::yggr::packet::packet_info<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_PARAMS_LEN(), T)> type;

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

// get_sub_info_type
// YGGR_PP_PACKET_INFO_TYPE_NAME () info_type
// YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() idx_name

#define YGGR_PP_GET_PACKET_SUB_INFO_TYPE( __n__, __type__ ) \
	typename __type__::template arg< __n__ >::type

#define YGGR_PP_GET_PACKET_SUB_INFO_TYPES_LOOP( __z__, __n__, __op__ ) \
        YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
		YGGR_PP_GET_PACKET_SUB_INFO_TYPE( __n__, \
				YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_TYPE_NAME ) )

#define YGGR_PP_GET_PACKET_SUB_INFO_TYPES_OF_IDX_NAME_LOOP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
		YGGR_PP_GET_PACKET_SUB_INFO_TYPE( \
			BOOST_PP_CAT( YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_TYPE_IDX_NAME ), __n__ ), \
			YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_TYPE_NAME ) )

// get_sub_info
// YGGR_PP_PACKET_INFO_NAME() obj_name
// YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() idx_name

#define YGGR_PP_GET_PACKET_SUB_INFO( __n__, __obj__ ) \
		( __obj__.template get< __n__ >( ) )

#define YGGR_PP_GET_PACKET_SUB_INFOS_LOOP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
		YGGR_PP_GET_PACKET_SUB_INFO( \
			__n__, YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_NAME ) )

#define YGGR_PP_GET_PACKET_SUB_INFOS_OF_IDX_NAME_LOOP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
		YGGR_PP_GET_PACKET_SUB_INFO( \
			BOOST_PP_CAT( YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_TYPE_IDX_NAME ), __n__), \
			YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_NAME ) )


//compare detail

/*
* after def
* YGGR_PP_PACKET_INFO_TYPE_IDX_NAME // size_name
* YGGR_PP_PACKET_INFO_NAME // pak pak_obj_name
*/

#	define _YGGR_TMP_PP_PACKET_INFO_FOO_COMPARE( __n__ ) \
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, size_type YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() ) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					typename PakInfo, YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		static bool compare(const PakInfo& YGGR_PP_PACKET_INFO_NAME() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return (yggr::make_tuple(YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER(__n__, YGGR_PP_SYMBOL_COMMA ) ) \
						== yggr::make_tuple(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ) ); } \
		\
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, size_type YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() ) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					typename PakInfo, YGGR_PP_FOO_TYPES_DEF(__n__), \
					typename Handler > inline \
		static typename boost::enable_if< \
			typename func::foo_t_info<Handler>::is_callable_type, \
			typename func::foo_t_info<Handler>::result_type >::type \
			compare(const Handler& cmper, \
					const PakInfo& YGGR_PP_PACKET_INFO_NAME() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return cmper(yggr::make_tuple(YGGR_PP_FOO_PARAMS_CUSTOM_OP_LATER(__n__, YGGR_PP_SYMBOL_COMMA ) ), \
							yggr::make_tuple(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ) ); }

// gets detail
/*
* after def
* YGGR_PP_PACKET_INFO_TYPE_IDX_NAME // size_name
* YGGR_PP_PACKET_INFO_NAME // pak_obj_name
* YGGR_PP_PACKET_INFO_TYPE_NAME // pak_type_name
*/

#	define _YGGR_TMP_PP_PACKET_INFO_FOO_GET_SUB_INFOS( __n__ ) \
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, size_type YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() ) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					typename YGGR_PP_PACKET_INFO_TYPE_NAME() > inline \
		static ::yggr::tuple< YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
								__n__, YGGR_PP_GET_PACKET_SUB_INFO_TYPES_OF_IDX_NAME_LOOP, YGGR_PP_SYMBOL_COMMA ) > \
			get_sub_infos( const YGGR_PP_PACKET_INFO_TYPE_NAME()& YGGR_PP_PACKET_INFO_NAME() ) { \
			return yggr::make_tuple(YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
										__n__, YGGR_PP_GET_PACKET_SUB_INFOS_OF_IDX_NAME_LOOP, YGGR_PP_SYMBOL_COMMA ) ); }

/*
* after def
* YGGR_PP_PACKET_INFO_TYPE_IDX_NAME // size_name
* YGGR_PP_PACKET_INFO_NAME // pak_obj_name
* YGGR_PP_PACKET_INFO_TYPE_NAME // pak_type_name
*/

#	define _YGGR_TMP_PP_PACKET_INFO_FOO_GET_SUB_PAK_INFO( __n__ ) \
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, size_type YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() ) \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					typename YGGR_PP_PACKET_INFO_TYPE_NAME() > inline \
		static ::yggr::packet::packet_info< YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
											__n__, YGGR_PP_GET_PACKET_SUB_INFO_TYPES_OF_IDX_NAME_LOOP, YGGR_PP_SYMBOL_COMMA ) > \
			get_sub_packet_info( const YGGR_PP_PACKET_INFO_TYPE_NAME()& YGGR_PP_PACKET_INFO_NAME() ) { \
			typedef yggr::packet::packet_info< YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
												__n__, YGGR_PP_GET_PACKET_SUB_INFO_TYPES_OF_IDX_NAME_LOOP, YGGR_PP_SYMBOL_COMMA ) > result_type; \
			return result_type::make_packet_info( \
						YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
						__n__, YGGR_PP_GET_PACKET_SUB_INFOS_OF_IDX_NAME_LOOP, YGGR_PP_SYMBOL_COMMA ) ); }


namespace yggr
{
namespace packet
{

class packet_info_op
	: private nonable::noncreateable
{
public:

	// compare
	// bool compare(pak_info) detail
#	define BOOST_PP_LOCAL_MACRO(__n__) \
		_YGGR_TMP_PP_PACKET_INFO_FOO_COMPARE(__n__)

#	define YGGR_PP_PACKET_INFO_NAME() pak_info
#	define YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() N
#	define YGGR_PP_FOO_ARG_NAME() arg
#	define YGGR_PP_FOO_CUSTOM_OP() YGGR_PP_GET_PACKET_SUB_INFOS_OF_IDX_NAME_LOOP

#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_PACKET_INFO_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_CUSTOM_OP
#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_PP_PACKET_INFO_NAME
#	undef YGGR_PP_PACKET_INFO_TYPE_IDX_NAME


// get_sub_infos

// yggr::tuple<...> get_sub_infos(pak_info) dtail
#	define BOOST_PP_LOCAL_MACRO(__n__) \
		_YGGR_TMP_PP_PACKET_INFO_FOO_GET_SUB_INFOS(__n__)

#	define YGGR_PP_PACKET_INFO_TYPE_NAME() PakInfo
#	define YGGR_PP_PACKET_INFO_NAME() pak_info
#	define YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() N
#	define YGGR_PP_FOO_ARG_NAME() arg

#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_PACKET_INFO_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_PP_PACKET_INFO_TYPE_IDX_NAME
#	undef YGGR_PP_PACKET_INFO_NAME
#	undef YGGR_PP_PACKET_INFO_TYPE_NAME

// yggr::packet::packet_info_n<...> get_sub_packet_info(pak_info) dtail
#	define BOOST_PP_LOCAL_MACRO(__n__) \
		_YGGR_TMP_PP_PACKET_INFO_FOO_GET_SUB_PAK_INFO(__n__)

#	define YGGR_PP_PACKET_INFO_TYPE_NAME() PakInfo
#	define YGGR_PP_PACKET_INFO_NAME() pak_info
#	define YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() N
#	define YGGR_PP_FOO_ARG_NAME() arg

#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_PACKET_INFO_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_PP_PACKET_INFO_TYPE_IDX_NAME
#	undef YGGR_PP_PACKET_INFO_NAME
#	undef YGGR_PP_PACKET_INFO_TYPE_NAME

};

} // namespace packet
} // namespace yggr

#undef _YGGR_TMP_PP_PACKET_INFO_FOO_GET_SUB_PAK_INFO
#undef _YGGR_TMP_PP_PACKET_INFO_FOO_GET_SUB_INFOS
#undef _YGGR_TMP_PP_PACKET_INFO_FOO_COMPARE

#ifdef _MSC_VER
#	pragma warning (pop)
#endif //_MSC_VER

// YGGR_PP_PACKET_INFO_SUB_OP(__n__, __val__)
// YGGR_PP_PACKET_INFO_VALUE ()

// real type version
#define YGGR_PP_PACKET_INFO_TO_FOO_PARAMS_OP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
		YGGR_PP_PACKET_INFO_SUB_OP( __n__,  YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_VALUE  ) )

#define YGGR_PP_PACKET_INFO_SUB_OP(__n__, __val__) \
			( __val__.get< __n__ >() )


#define YGGR_PP_PACKET_INFO_TO_PARAMS() YGGR_PP_PACKET_INFO_TO_FOO_PARAMS_OP


//typename type version
#define YGGR_PP_TYPENAME_PACKET_INFO_TO_FOO_PARAMS_OP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
		YGGR_PP_TYPENAME_PACKET_INFO_SUB_OP( __n__, YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_VALUE  ) )

#define YGGR_PP_TYPENAME_PACKET_INFO_SUB_OP(__n__, __val__) \
			( __val__.template get< __n__ >() )


#define YGGR_PP_TYPENAME_PACKET_INFO_TO_PARAMS() YGGR_PP_TYPENAME_PACKET_INFO_TO_FOO_PARAMS_OP

#endif // __YGGR_NETWORK_PACKET_INFO_VMACRO_HPP__
