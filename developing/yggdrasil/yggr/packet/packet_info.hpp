//packet_info.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_NETWORK_PACKET_INFO_HPP__
#define __YGGR_NETWORK_PACKET_INFO_HPP__

#include <sstream>
#include <utility>

#include <boost/tuple/tuple.hpp>
#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>
#include <boost/functional/hash/hash.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/long.hpp>

#include <boost/type_traits/is_same.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/mplex/foo_type_info.hpp>
#include <yggr/mplex/conv_to_tuple.hpp>

#include <yggr/struct_support/tuple.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/tuple.hpp>

#ifndef YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN
#	define YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN
#endif // YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN

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

#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() boost::tuples::null_type
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename Val ) >
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
class packet_info
{
private:
	typedef boost::tuples::null_type null_type;
	typedef boost::mpl::vector
			<
				YGGR_PP_TEMPLATE_PARAMS_TYPES(
					YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN,
					Val)
			> tmp_vt_t_type;
	typedef typename boost::mpl::find<tmp_vt_t_type, null_type>::type i;
	typedef typename boost::mpl::erase<tmp_vt_t_type,
										i,
										typename boost::mpl::end<tmp_vt_t_type>::type>::type vt_t_type;
public:
	enum
	{
		E_length = boost::mpl::size<vt_t_type>::value,
		E_compile = 0xffffffff
	};

public:
	typedef vt_t_type params_type;
	typedef typename mplex::conv_to_tuple<params_type, null_type>::type value_type;
	template<size_type N>
	struct arg
	{
		enum
		{
			index = N,
			E_compile_u32 = 0xffffff
		};
		BOOST_MPL_ASSERT((boost::mpl::if_c< (N < E_length), boost::mpl::true_, boost::mpl::false_>));
		typedef typename mplex::get_arg<params_type, N>::type type;
	};

	template<typename T, size_type Mode = 0> // now Mode must be 0
	struct t_arg
	{
		enum
		{
			index = boost::mpl::find<vt_t_type, T>::type::pos::value,
			E_compile_u32 = 0xffffffff
		};
		typedef typename arg< index >::type type;
	};

private:

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename T) >
	friend class packet_info;

	typedef packet_info this_type;

	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	packet_info(void)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	packet_info(BOOST_RV_REF(value_type) val)
		: _info(boost::forward<value_type>(val))
	{
	}
#else
	packet_info(BOOST_RV_REF(value_type) val)
	{
		value_type& val_ref = val;
		boost::swap(_info, val_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	packet_info(const value_type& val)
		: _info(val)
	{
	}

#define BOOST_PP_LOCAL_MACRO(__n__) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	packet_info( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_REAL_CREF_PARAMS ) ) \
		: _info( boost::make_tuple( YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ) ) { \
			BOOST_MPL_ASSERT((boost::mpl::less_equal< boost::mpl::long_<__n__>, boost::mpl::long_<this_type::E_length> >)); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename T) >
	packet_info(BOOST_RV_REF(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)>) right)
		: _info(boost::forward<value_type>(right._info))
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)> other_type;
		typedef typename other_type::value_type other_value_type;

		BOOST_MPL_ASSERT((boost::is_same<value_type, other_value_type>));
	}
#else
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename T) >
	packet_info(BOOST_RV_REF(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)>) right)
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)> other_type;
		typedef typename other_type::value_type other_value_type;

		BOOST_MPL_ASSERT((boost::is_same<value_type, other_value_type>));
		other_type& other_ref = right;
		boost::swap(_info, other_ref._info);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename T) >
	packet_info(const packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)>& right)
		: _info(right._info)
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)> other_type;
		typedef typename other_type::value_type other_value_type;

		BOOST_MPL_ASSERT((boost::is_same<value_type, other_value_type>));
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	packet_info( BOOST_RV_REF( this_type ) right)
		: _info(boost::forward<value_type>(right._info))
	{
	}
#else
	packet_info( BOOST_RV_REF( this_type ) right)
	{
		this_type& right_ref = right;
		boost::swap(_info, right_ref._info);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	packet_info(const this_type& right)
		: _info(right._info)
	{
	}

	~packet_info(void)
	{
	}

	operator value_type&(void)
	{
		return _info;
	}

	operator const value_type&(void) const
	{
		return _info;
	}

	void clear(void)
	{
		value_type().swap(_info);
	}

	std::size_t element_size(void) const
	{
		return this_type::E_length;
	}

	static std::size_t s_element_size(void)
	{
		return this_type::E_length;
	}

	template<size_type N>
	typename arg<N>::type& get(void)
	{
		return boost::get<N>(_info);
	}

	template<size_type N>
	const typename arg<N>::type& get(void) const
	{
		return boost::get<N>(_info);
	}

	template<typename T>
	typename t_arg<T>::type& t_get(void)
	{
		typedef t_arg<T> t_arg_type;
		return boost::get<t_arg_type::index>(_info);
	}

	template<typename T>
	const typename t_arg<T>::type& t_get(void) const
	{
		typedef t_arg<T> t_arg_type;
		return boost::get<t_arg_type::index>(_info);
	}

	this_type& operator=(BOOST_RV_REF(value_type) right)
	{
		boost::swap(_info, boost::forward<value_type>(right));
		return *this;
	}

	this_type& operator=(const value_type& right)
	{
		_info = right;
		return *this;
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename T) >
	this_type& operator=(BOOST_RV_REF(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)>) right)
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)> other_type;
		typedef typename other_type::value_type other_value_type;

		BOOST_MPL_ASSERT((boost::is_same<value_type, other_value_type>));

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_info = boost::forward<value_type>(right._info);
#else
		other_type& other_ref = right;
		boost::swap(_info, other_ref._info);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename T) >
	this_type& operator=(const packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)>& right)
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)> other_type;
		typedef typename other_type::value_type other_value_type;

		BOOST_MPL_ASSERT((boost::is_same<value_type, other_value_type>));

		_info = right._info;
		return *this;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_info = boost::forward<value_type>(right._info);
#else
		this_type& right_ref = right;
		boost::swap(_info,right_ref._info);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_info = right._info;
		return *this;
	}

	bool operator==(const value_type& right) const
	{
		return ::operator==(_info, right);
	}

	bool operator==(const this_type& right) const
	{
		return ::operator==(_info, right._info);
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename T) >
	bool operator==(const packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)>& right)
	{
		return ::operator==(_info, right._info);
	}

	bool operator!=(const value_type& right) const
	{
		return ::operator!=(_info, right);
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename T) >
	bool operator!=(const packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)>& right)
	{
		return ::operator!=(_info, right._info);
	}

	bool operator!=(const this_type& right) const
	{
		return ::operator!=(_info, right._info);
	}

	void swap(BOOST_RV_REF(value_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		boost::swap(_info, boost::forward<value_type>(right));
#else
		value_type& val_ref = right;
		boost::swap( _info, val_ref );
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(value_type& right)
	{
		boost::swap(_info, right);
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename T) >
	void swap(BOOST_RV_REF(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)>) right)
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)> other_type;
		typedef typename other_type::value_type other_value_type;

		BOOST_MPL_ASSERT((boost::is_same<value_type, other_value_type>));

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		boost::swap(_info, boost::forward<value_type>(right._info));
#else
		other_type& other_ref = right;
		boost::swap(_info, other_ref._info);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, typename T) >
	void swap(packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)>& right)
	{
		typedef packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN, T)> other_type;
		typedef typename other_type::value_type other_value_type;

		BOOST_MPL_ASSERT((boost::is_same<value_type, other_value_type>));

		boost::swap(_info, right._info);
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		boost::swap(_info, boost::forward<value_type>(right._info));
#else
		this_type& right_ref = right;
		boost::swap(_info, right_ref._info);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		boost::swap(_info, right._info);
	}

#define BOOST_PP_LOCAL_MACRO(__n__) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	static this_type make_packet_info( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		BOOST_MPL_ASSERT((boost::mpl::less_equal< boost::mpl::long_<__n__>, boost::mpl::long_<this_type::E_length> >)); \
		return this_type( YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 ver)
	{
		ar & YGGR_SERIALIZE_NVP(_info);
	}

private:
	value_type _info;
};

} // namespace packet
} // namespace yggr

//operator == detail
#define _YGGR_TMP_PP_PACKET_OPERATOR_COMPARE(__n__) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename Val ) > \
	bool operator==(const typename yggr::packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val ) >::value_type& left, \
						const yggr::packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val ) >& right) { \
		return right.operator==(left); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename Val ) > \
	bool operator!=(const typename yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )> \
						::value_type& left, \
						const yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )>& right) { \
		return right.operator!=(left); }


//swap detail

#define _YGGR_TMP_PP_PACKET_INFO_SWAP( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename LVal ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename RVal ) > inline \
	void swap(yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, LVal )>& left, \
				yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, RVal )>& right) { \
		left.swap(right); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename LVal ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename RVal ) > inline \
	void swap(BOOST_RV_REF( yggr::packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, LVal )> ) left, \
							yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, RVal )>& right) { \
		right.swap(left); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename LVal ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename RVal ) > inline \
	void swap(yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, LVal )>& left, \
				BOOST_RV_REF(yggr::packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, RVal )>) right) { \
		left.swap(right); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename Val ) > inline \
	void swap(yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )>& left, \
				typename yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )> \
					::value_type& right) { \
		left.swap(right); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename Val ) > inline \
	void swap(yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )>& left, \
				BOOST_RV_REF(typename yggr::packet::packet_info< \
											YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )>::value_type) right) { \
		left.swap(right); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename Val ) > inline \
	void swap(BOOST_RV_REF(yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )>) left, \
				typename yggr::packet::packet_info< \
											YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )>::value_type& right) { \
		boost::swap(right, left); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename Val ) > inline \
	void swap(typename yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )> \
				::value_type& left, \
				yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )>& right) { \
		left.swap(right); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename Val ) > inline \
	void swap(BOOST_RV_REF(typename yggr::packet::packet_info< \
										YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )>::value_type) left, \
				yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )>& right) { \
		left.swap(right); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename Val ) > inline \
	void swap(typename yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )> \
				::value_type& left, \
				BOOST_RV_REF(yggr::packet::packet_info<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, Val )>) right) { \
		boost::swap(left, right); }



#define _YGGR_TMP_PP_PACKET_INFO_OUT(__n__) \
	template<typename Char, typename Traits \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename Val)> \
	std::basic_ostream<Char, Traits>& \
		operator<<(std::basic_ostream<Char, Traits>& os, \
					const yggr::packet::packet_info< \
								YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, Val)>& right) { \
		typedef yggr::packet::packet_info< \
								YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, Val)> info_type; \
		typedef typename info_type::value_type info_value_type; \
		const info_value_type& cref_val = right; \
		os << "packet_info_" << info_type::s_element_size() << " : "<< cref_val; \
		return os; }

	_YGGR_TMP_PP_PACKET_OPERATOR_COMPARE(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN)
	_YGGR_TMP_PP_PACKET_INFO_OUT(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN)

namespace std
{
	_YGGR_TMP_PP_PACKET_INFO_SWAP(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN)
} // namespace std

namespace boost
{

	_YGGR_TMP_PP_PACKET_INFO_SWAP(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN)

} // namespace boost

#undef _YGGR_TMP_PP_PACKET_INFO_SWAP
#undef _YGGR_TMP_PP_PACKET_OPERATOR_COMPARE
#undef _YGGR_TMP_PP_PACKET_INFO_OUT

#define _YGGR_TMP_PP_PACKET_INFO_HASH_FOO(__n__) \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename Val)> \
	std::size_t hash_value( \
		const yggr::packet::packet_info < \
						YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, Val)>& info) { \
		typedef yggr::packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, Val) > packet_info_type; \
		typedef typename packet_info_type::value_type val_type; \
		const val_type& val_cref = info; \
		boost::hash<val_type> hasher; \
		return hasher(val_cref); }

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace packet
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

	_YGGR_TMP_PP_PACKET_INFO_HASH_FOO(YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN)

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace packet
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#undef _YGGR_TMP_PP_PACKET_INFO_HASH_FOO

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

// get_sub_info_type
// YGGR_PP_PACKET_INFO_TYPE_NAME () info_type
// YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() idx_name

#define YGGR_PP_GET_PACKET_SUB_INFO_TYPE( __n__, __type__ ) \
	typename __type__::template arg<__n__>::type

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
						YGGR_PP_GET_PACKET_SUB_INFO( __n__, \
								YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_NAME ) )

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

#define _YGGR_TMP_PP_PACKET_INFO_FOO_COMPARE( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, size_type YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				typename PakInfo, YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	static bool compare(const PakInfo& YGGR_PP_PACKET_INFO_NAME() YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		return (boost::make_tuple(YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER(__n__, YGGR_PP_SYMBOL_COMMA ) ) \
					== boost::make_tuple(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ) ); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, size_type YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				typename PakInfo, YGGR_PP_FOO_TYPES_DEF(__n__), \
				typename Handler > \
	static bool compare(const PakInfo& YGGR_PP_PACKET_INFO_NAME(), \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							const Handler& handler) { \
		return handler(boost::make_tuple(YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER(__n__, YGGR_PP_SYMBOL_COMMA ) ), \
						boost::make_tuple(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ) ); }

// gets detail
/*
* after def
* YGGR_PP_PACKET_INFO_TYPE_IDX_NAME // size_name
* YGGR_PP_PACKET_INFO_NAME // pak_obj_name
* YGGR_PP_PACKET_INFO_TYPE_NAME // pak_type_name
*/

#define _YGGR_TMP_PP_PACKET_INFO_FOO_GET_SUB_INFOS( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, size_type YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				typename YGGR_PP_PACKET_INFO_TYPE_NAME() > \
	static boost::tuple< YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
							__n__, YGGR_PP_GET_PACKET_SUB_INFO_TYPES_OF_IDX_NAME_LOOP, YGGR_PP_SYMBOL_COMMA ) > \
		get_sub_infos( const YGGR_PP_PACKET_INFO_TYPE_NAME()& YGGR_PP_PACKET_INFO_NAME() ) { \
		return boost::make_tuple(YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
									__n__, YGGR_PP_GET_PACKET_SUB_INFOS_OF_IDX_NAME_LOOP, YGGR_PP_SYMBOL_COMMA ) ); }

#define _YGGR_TMP_PP_PACKET_INFO_FOO_GET_SUB_INFO( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, size_type YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				typename YGGR_PP_PACKET_INFO_TYPE_NAME() > \
	static yggr::packet::packet_info< YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
										__n__, YGGR_PP_GET_PACKET_SUB_INFO_TYPES_OF_IDX_NAME_LOOP, YGGR_PP_SYMBOL_COMMA ) > \
		get_sub_info( const YGGR_PP_PACKET_INFO_TYPE_NAME()& YGGR_PP_PACKET_INFO_NAME() ) { \
		typedef yggr::packet::packet_info< YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
											__n__, YGGR_PP_GET_PACKET_SUB_INFO_TYPES_OF_IDX_NAME_LOOP, YGGR_PP_SYMBOL_COMMA ) > result_type; \
		return result_type(YGGR_PP_FOO_PARAMS_CUSTOM_OP_NOW( \
									__n__, YGGR_PP_GET_PACKET_SUB_INFOS_OF_IDX_NAME_LOOP, YGGR_PP_SYMBOL_COMMA ) ); }

namespace yggr
{
namespace packet
{

class packet_info_op
	: private nonable::noncreateable
{
public:

	//bool compare(pak_info)
	//  this foo is mark only, don't use it, if compile error stop this please check your code
	template<typename PakInfo>
	static bool compare(const PakInfo& pak_info);

	//template<typename PakInfo>
	//static bool compare(const PakInfo& pak_info)
	//{
	//	//BOOST_MPL_ASSERT((boost::mpl::false_));
	//	assert(false);
	//	return false;
	//}

	template<typename PakInfo, typename Handler>
	static bool compare(const PakInfo& pak_info, const Handler& handler)
	{
		return handler(pak_info);
	}

// bool compare(pak_info) detail
#define BOOST_PP_LOCAL_MACRO(__n__) \
	_YGGR_TMP_PP_PACKET_INFO_FOO_COMPARE(__n__)

#define YGGR_PP_PACKET_INFO_NAME() pak_info
#define YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() N
#define YGGR_PP_FOO_ARG_NAME() arg
#define YGGR_PP_FOO_CUSTOM_OP() YGGR_PP_GET_PACKET_SUB_INFOS_OF_IDX_NAME_LOOP

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_CUSTOM_OP
#undef YGGR_PP_FOO_ARG_NAME
#undef YGGR_PP_PACKET_INFO_NAME
#undef YGGR_PP_PACKET_INFO_TYPE_IDX_NAME

// boost::tuple<...> get_sub_infos(pak_info) dtail
#define BOOST_PP_LOCAL_MACRO(__n__) \
	_YGGR_TMP_PP_PACKET_INFO_FOO_GET_SUB_INFOS(__n__)

#define YGGR_PP_PACKET_INFO_TYPE_NAME() PakInfo
#define YGGR_PP_PACKET_INFO_NAME() pak_info
#define YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() N
#define YGGR_PP_FOO_ARG_NAME() arg

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_ARG_NAME
#undef YGGR_PP_PACKET_INFO_TYPE_IDX_NAME
#undef YGGR_PP_PACKET_INFO_NAME
#undef YGGR_PP_PACKET_INFO_TYPE_NAME

// yggr::packet::packet_info_n<...> get_sub_info(pak_info) dtail
#define BOOST_PP_LOCAL_MACRO(__n__) \
	_YGGR_TMP_PP_PACKET_INFO_FOO_GET_SUB_INFO(__n__)

#define YGGR_PP_PACKET_INFO_TYPE_NAME() PakInfo
#define YGGR_PP_PACKET_INFO_NAME() pak_info
#define YGGR_PP_PACKET_INFO_TYPE_IDX_NAME() N
#define YGGR_PP_FOO_ARG_NAME() arg

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_PACKET_INFO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_ARG_NAME
#undef YGGR_PP_PACKET_INFO_TYPE_IDX_NAME
#undef YGGR_PP_PACKET_INFO_NAME
#undef YGGR_PP_PACKET_INFO_TYPE_NAME

};

} // namespace packet
} // namespace yggr

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
						YGGR_PP_PACKET_INFO_SUB_OP( __n__, \
													YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_VALUE  ) )

#define YGGR_PP_PACKET_INFO_SUB_OP(__n__, __val__) \
			( __val__.get<__n__>() )


#define YGGR_PP_PACKET_INFO_TO_PARAMS() YGGR_PP_PACKET_INFO_TO_FOO_PARAMS_OP


//typename type version
#define YGGR_PP_TYPENAME_PACKET_INFO_TO_FOO_PARAMS_OP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
						YGGR_PP_TYPENAME_PACKET_INFO_SUB_OP( __n__, \
													YGGR_PP_OPEN( YGGR_PP_PACKET_INFO_VALUE  ) )

#define YGGR_PP_TYPENAME_PACKET_INFO_SUB_OP(__n__, __val__) \
			( __val__.template get<__n__>() )


#define YGGR_PP_TYPENAME_PACKET_INFO_TO_PARAMS() YGGR_PP_TYPENAME_PACKET_INFO_TO_FOO_PARAMS_OP

#endif // __YGGR_NETWORK_PACKET_INFO_HPP__
