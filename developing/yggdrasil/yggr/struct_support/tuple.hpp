// tuple.hpp

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

#ifndef __YGGR_STRUCT_SUPPORT_TUPLE_HPP__
#define __YGGR_STRUCT_SUPPORT_TUPLE_HPP__

#include <sstream>

#include <yggr/move/move.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/functional/hash/hash.hpp>

// tuple_support foo maroc detail

// _YGGR_TMP_PP_TEP_SUB_OP(__n__, __left__, __right__)
// _YGGR_TMP_PP_TEP_LEFT ()
// _YGGR_TMP_PP_TEP_RIGHT ()
#define _YGGR_TMP_PP_TUPLE_ARGS_OP( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
						_YGGR_TMP_PP_TEP_SUB_OP( __n__, \
													YGGR_PP_OPEN( _YGGR_TMP_PP_TEP_LEFT  ), \
                                                YGGR_PP_OPEN( _YGGR_TMP_PP_TEP_RIGHT ) )

// _YGGR_TMP_PP_TEP_SUB_GET(__n__, __val__)
// _YGGR_TMP_PP_TEP_VAL ()
#define _YGGR_TMP_PP_TUPLE_ARGS_GET( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
						_YGGR_TMP_PP_TEP_SUB_GET( __n__, \
													YGGR_PP_OPEN( _YGGR_TMP_PP_TEP_VAL  ) )

namespace yggr
{
namespace struct_support
{

class tuple_support
{
private:
	typedef tuple_support this_type;

    // gcc must use T, because gcc not support in class template use all params special case
	template<bool len_same, typename T = this_type>
	class tuple_equal_run;

	template<typename T>
	class tuple_equal_run<true, T>
	{
	public:
		typedef tuple_support parent_type;
		template<typename T1, typename T2>
		bool operator()(const T1& t1, const T2& t2) const
		{
			return parent_type::prv_tuple_equal(t1, t2);
		}
    };

	template<typename T>
	class tuple_equal_run<false, T>
	{
	public:
		typedef tuple_support parent_type;
		template<typename T1, typename T2>
		bool operator()(const T1& t1, const T2& t2) const
		{
			return false;
		}
	};

	template<bool len_same, typename T>
	friend class tuple_equal_run;

public:

	inline static bool tuple_equal(const ::boost::tuple<>& left, const ::boost::tuple<>& right)
	{
		return true;
	}

	inline static bool tuple_not_equal(const ::boost::tuple<>& left, const ::boost::tuple<>& right)
	{
		return false;
	}

	template<typename Tuple>
	static bool tuple_equal(const Tuple& left, const Tuple& right)
	{
		return this_type::prv_tuple_equal(left, right);
	}

	template<typename Tuple>
	static bool tuple_not_equal(const Tuple& left, const Tuple& right)
	{
		return !this_type::prv_tuple_equal(left, right);
	}

	template<typename Tuple1, typename Tuple2>
	static bool tuple_equal(const Tuple1& left, const Tuple2& right)
	{
		typedef tuple_equal_run< (boost::tuples::length<Tuple1>::value
									== boost::tuples::length<Tuple2>::value) >
				run_type;
		run_type run;
		return run(left, right);
	}

	template<typename Tuple1, typename Tuple2>
	static bool tuple_not_equal(const Tuple1& left, const Tuple2& right)
	{
		typedef tuple_equal_run< (boost::tuples::length<Tuple1>::value
									== boost::tuples::length<Tuple2>::value) >
				run_type;
		run_type run;
		return !run(left, right);
	}

	// tuple swap
	inline static void tuple_swap(::boost::tuple<>& left, ::boost::tuple<>& right)
	{
		return;
	}

	inline static void tuple_swap(BOOST_RV_REF(::boost::tuple<>) left, ::boost::tuple<>& right)
	{
		return;
	}

	inline static void tuple_swap(::boost::tuple<>& left, BOOST_RV_REF(::boost::tuple<>) right)
	{
		return;
	}

	template<typename Tuple>
	static void tuple_swap(Tuple& left, Tuple& right)
	{
		this_type::prv_tuple_swap(left, right);
	}

	template<typename Tuple>
	static void tuple_swap(BOOST_RV_REF(Tuple) left, Tuple& right)
	{
		this_type::prv_tuple_swap(boost::forward<Tuple>(left), right);
	}

	template<typename Tuple>
	static void tuple_swap(Tuple& left, BOOST_RV_REF(Tuple) right)
	{
		this_type::prv_tuple_swap(left, boost::forward<Tuple>(right));
	}

private:
	// tuple_equal detail
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T) > \
	static bool prv_tuple_equal( const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_LEFT(), \
							 const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_RIGHT() ) { \
		return ( YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_LOGIC_AND ) ); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T1),  YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T2) > \
	static bool prv_tuple_equal( const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T1 ) >& _YGGR_TMP_PP_TEP_LEFT(), \
							 const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T2 ) >& _YGGR_TMP_PP_TEP_RIGHT() ) { \
		return ( YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_LOGIC_AND ) ); }

#define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __left__, __right__ ) \
		( boost::get< __n__ >( __left__ ) == boost::get< __n__ >( __right__ ) )

#define _YGGR_TMP_PP_TEP_LEFT() left
#define _YGGR_TMP_PP_TEP_RIGHT() right
#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_TUPLE_ARGS_OP

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_CUSTOM_OP
#undef _YGGR_TMP_PP_TEP_LEFT
#undef _YGGR_TMP_PP_TEP_RIGHT
#undef _YGGR_TMP_PP_TEP_SUB_OP

// tuple_swap def
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T) > \
	static void prv_tuple_swap( ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_LEFT(), \
								::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_RIGHT() ) { \
			( YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T) > \
	static void prv_tuple_swap( BOOST_RV_REF(::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >) _YGGR_TMP_PP_TEP_LEFT(), \
								::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_RIGHT() ) { \
			( YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T) > \
	static void prv_tuple_swap( ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_LEFT(), \
								BOOST_RV_REF(::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >) _YGGR_TMP_PP_TEP_RIGHT() ) { \
			( YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_COMMA ) ); }
#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T) > \
	static void prv_tuple_swap( ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_LEFT(), \
								::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_RIGHT() ) { \
			( YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T) > \
	static void prv_tuple_swap( BOOST_RV_REF(::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >) l, \
								::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_RIGHT() ) { \
			typedef ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > now_tuple_type; \
			now_tuple_type& _YGGR_TMP_PP_TEP_LEFT() = l; \
			( YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T) > \
	static void prv_tuple_swap( ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& _YGGR_TMP_PP_TEP_LEFT(), \
								BOOST_RV_REF(::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >) r ) { \
			typedef ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > now_tuple_type; \
			now_tuple_type& _YGGR_TMP_PP_TEP_RIGHT() = r; \
			( YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

#define _YGGR_TMP_PP_TEP_SUB_OP( __n__, __left__, __right__ ) \
		( boost::swap( boost::get< __n__ >( __left__ ), boost::get< __n__ >( __right__ ) ) )

#define _YGGR_TMP_PP_TEP_LEFT() left
#define _YGGR_TMP_PP_TEP_RIGHT() right
#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_TUPLE_ARGS_OP

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_CUSTOM_OP
#undef _YGGR_TMP_PP_TEP_LEFT
#undef _YGGR_TMP_PP_TEP_RIGHT
#undef _YGGR_TMP_PP_TEP_SUB_OP
};

} // namespace struct_support
} // namespace yggr

//#undef _YGGR_TMP_PP_TUPLE_ARGS_OP

// operator == !=

#define _YGGR_TMP_PP_TUPLE_COMPARE0() \
inline bool operator!=(const ::boost::tuple<>& left, const ::boost::tuple<>& right) { \
	return false; } \
\
inline bool operator==(const ::boost::tuple<>& left, const ::boost::tuple<>& right) { \
	return true; }

#define _YGGR_TMP_PP_TUPLE_COMPARE( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T) > \
	bool operator==( const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& left, \
							 const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& right ) { \
		return yggr::struct_support::tuple_support::tuple_equal(left, right); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T1),  YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T2) > \
	bool operator==( const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T1 ) >& left, \
							 const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T2 ) >& right ) { \
		return yggr::struct_support::tuple_support::tuple_equal(left, right); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T) > \
	bool operator!=( const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& left, \
							 const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& right ) { \
		return yggr::struct_support::tuple_support::tuple_not_equal(left, right); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T1),  YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T2) > \
	bool operator!=( const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T1 ) >& left, \
							 const ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T2 ) >& right ) { \
		return yggr::struct_support::tuple_support::tuple_not_equal(left, right); }

_YGGR_TMP_PP_TUPLE_COMPARE0()

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	_YGGR_TMP_PP_TUPLE_COMPARE( __n__ )

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#if defined(__APPLE__) || defined(__APPLE_CC__)
namespace boost
{
namespace tuples
{
#else
namespace std
{
#endif //__APPLE__
    
_YGGR_TMP_PP_TUPLE_COMPARE0()
    
#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	_YGGR_TMP_PP_TUPLE_COMPARE( __n__ )

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()
  
#if defined(__APPLE__) || defined(__APPLE_CC__)
} // namespace tuples
} // namespace std
#else
}
#endif // __APPLE__

#undef _YGGR_TMP_PP_TUPLE_COMPARE0
#undef _YGGR_TMP_PP_TUPLE_COMPARE


// swap
#define _YGGR_TMP_PP_TUPLE_DEFAULT_SWAP() \
	inline void swap(::boost::tuple<>& left, ::boost::tuple<>& right) { } \
	inline void swap(BOOST_RV_REF(::boost::tuple<>) left, ::boost::tuple<>& right) { } \
	inline void swap(::boost::tuple<>& left, BOOST_RV_REF(::boost::tuple<>) right) { }

#define _YGGR_TMP_PP_TUPLE_DETAIL_SWAP( __n__ ) \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T)> \
	void swap(::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& left, \
				::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& right) { \
		yggr::struct_support::tuple_support::tuple_swap(left, right); } \
	\
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T)> \
	void swap(BOOST_RV_REF( ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > ) left, \
				::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& right) { \
		typedef ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > this_tuple_type; \
		yggr::struct_support::tuple_support::tuple_swap(boost::forward<this_tuple_type>(left), right); } \
	\
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T)> \
	void swap( ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >& left, \
				BOOST_RV_REF(::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) >) right) { \
		typedef ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > this_tuple_type; \
		yggr::struct_support::tuple_support::tuple_swap(left, boost::forward<this_tuple_type>(right)); }

namespace std
{
	_YGGR_TMP_PP_TUPLE_DEFAULT_SWAP()

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	_YGGR_TMP_PP_TUPLE_DETAIL_SWAP( __n__ )

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

} // namespace std

namespace boost
{
	_YGGR_TMP_PP_TUPLE_DEFAULT_SWAP()

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	_YGGR_TMP_PP_TUPLE_DETAIL_SWAP( __n__ )

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

} // namespace boost


#undef _YGGR_TMP_PP_TUPLE_DEFAULT_SWAP
#undef _YGGR_TMP_PP_TUPLE_DETAIL_SWAP

#define _YGGR_TMP_TUPLE_HASH_FOO( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T) > \
	struct hash< ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T) > > \
		: public std::unary_function< ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T) >, std::size_t> { \
		typedef ::boost::tuple< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T) > value_type; \
		std::size_t operator()( const value_type& YGGR_PP_OPEN(_YGGR_TMP_PP_TEP_VAL) ) const { \
			std::size_t YGGR_PP_OPEN(_YGGR_TMP_PP_SEED_NAME) = 0; \
			(YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return YGGR_PP_OPEN(_YGGR_TMP_PP_SEED_NAME); } };


namespace boost
{

template<>
struct hash< ::boost::tuple<> >
	: public std::unary_function< ::boost::tuple<>, std::size_t>
{
	typedef ::boost::tuple<> value_type;
	std::size_t operator()(const value_type& val) const
	{
		boost::hash<std::string> hasher;
		return hasher(std::string());
	}
};


#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	_YGGR_TMP_TUPLE_HASH_FOO( __n__ )

#define _YGGR_TMP_PP_TEP_SUB_GET( __n__, __val__ ) \
		( boost::hash_combine(YGGR_PP_OPEN(_YGGR_TMP_PP_SEED_NAME) YGGR_PP_SYMBOL_COMMA() boost::get< __n__ >( __val__ ) ) )

#define _YGGR_TMP_PP_SEED_NAME() nseed
#define _YGGR_TMP_PP_TEP_VAL() val
#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_TUPLE_ARGS_GET

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE()

#undef YGGR_PP_FOO_CUSTOM_OP
#undef _YGGR_TMP_PP_SEED_NAME
#undef _YGGR_TMP_PP_TEP_VAL
#undef _YGGR_TMP_PP_TEP_SUB_GET

} // namespace boost


#undef _YGGR_TMP_PP_TUPLE_ARGS_OP
#undef _YGGR_TMP_PP_TUPLE_ARGS_GET

#endif // __YGGR_STRUCT_SUPPORT_TUPLE_HPP__

//#include <boost/functional/hash/extensions.hpp>
