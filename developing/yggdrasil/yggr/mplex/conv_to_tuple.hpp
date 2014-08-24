// conv_to_tuple.hpp

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

#ifndef __YGGR_MPLEX_CONV_TO_TUPLE_HPP__
#define __YGGR_MPLEX_CONV_TO_TUPLE_HPP__

#include <boost/tuple/tuple.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/at.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/typedef.hpp>
#include <yggr/ppex/symbol_if.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/foo_params.hpp>

// _YGGR_TMP_PP_VECTOR_SUB_GET(__n__, __val__)
// _YGGR_TMP_PP_VECTOR_VAL ()
#define _YGGR_TMP_PP_VECTOR_ARGS_GET( __z__, __n__, __op__ ) \
		YGGR_PP_SYMBOL_IF( __n__, __op__ ) \
						_YGGR_TMP_PP_VECTOR_SUB_GET( __n__, \
													YGGR_PP_OPEN( _YGGR_TMP_PP_VECTOR_VAL  ) )

#define _YGGR_TMP_PP_CONV_TO_TUPLE_IMPL_DEF(__n__) \
	template<typename _YGGR_TMP_PP_VECTOR_VAL() > \
	struct conv_to_tuple_impl< _YGGR_TMP_PP_VECTOR_VAL(), __n__ > { \
		public: \
			typedef boost::tuple<YGGR_PP_FOO_PARAMS_CUSTOM_OP_AFTER(__n__, YGGR_PP_SYMBOL_COMMA)> type; };


namespace yggr
{
namespace mplex
{
namespace detail
{

template<typename Vector, std::size_t n>
struct conv_to_tuple_impl;

template<typename Vector>
struct conv_to_tuple_impl<Vector, 0>
{
public:
	typedef boost::tuple<> type;
};

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	_YGGR_TMP_PP_CONV_TO_TUPLE_IMPL_DEF( __n__ )


#define _YGGR_TMP_PP_VECTOR_SUB_GET( __n__, __val__ ) \
	typename boost::mpl::at_c< __val__, __n__ >::type

#define _YGGR_TMP_PP_VECTOR_VAL() Vector
#define YGGR_PP_FOO_CUSTOM_OP() _YGGR_TMP_PP_VECTOR_ARGS_GET

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )

#undef YGGR_PP_FOO_CUSTOM_OP
#undef _YGGR_TMP_PP_VECTOR_VAL
#undef _YGGR_TMP_PP_VECTOR_SUB_GET


} // namespace detail
} // namespace mplex
} // namespace yggr

namespace yggr
{
namespace mplex
{

template<typename Vector, typename Null_T = boost::tuples::null_type>
struct conv_to_tuple
{
private:
	typedef Vector vt_type;
	typedef Null_T null_type;
	typedef typename boost::mpl::find<vt_type, null_type>::type i; 
	BOOST_MPL_ASSERT((
						boost::mpl::if_c <
							(i::pos::value == boost::mpl::size<vt_type>::value),
							boost::mpl::true_,
							boost::mpl::false_ > ));
public:
	typedef typename yggr::mplex::detail::conv_to_tuple_impl<vt_type, boost::mpl::size<vt_type>::value>::type type;
};

} // namespace mplex
} // namespace yggr

#undef _YGGR_TMP_PP_CONV_TO_TUPLE_IMPL_DEF

#endif // __YGGR_MPLEX_CONV_TO_TUPLE_HPP__
