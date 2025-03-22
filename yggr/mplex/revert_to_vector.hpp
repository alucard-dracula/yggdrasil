// revert_to_vector.hpp

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

#ifndef __YGGR_MPLEX_REVERT_TO_VECTOR_HPP__
#define __YGGR_MPLEX_REVERT_TO_VECTOR_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	include <yggr/type_traits/integer_sequence.hpp>
#else
#include <yggr/ppex/typedef.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>

namespace yggr
{
namespace mplex
{
namespace detail
{

template<typename T, std::size_t N>
struct detail_revert_to_vector;

} // namespace detail
} // namespace mplex
} // namespace yggr

namespace yggr
{
namespace mplex
{
namespace detail
{

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename T, typename SeqIdx>
struct revert_to_vector_conv;

template<typename T, std::size_t ...I>
struct revert_to_vector_conv<T, index_sequence<I...> >
	: public boost::mpl::vector<typename boost::mpl::at_c<T, I>::type...>
{
};

template<typename T, std::size_t N>
struct detail_revert_to_vector
	: public revert_to_vector_conv<T, make_index_sequence<N> >
{
};

#else

#	define YGGR_TMP_PP_TEMPLATE_PARAMS_AT_C_TRANS_IMPL( __z__, __n__, __class_name__ ) \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				typename boost::mpl::at_c< __class_name__, __n__ >::type

#	define YGGR_TMP_PP_TEMPLATE_PARAMS_AT_C_TRANS( __count__, __class_name__ ) \
				BOOST_PP_REPEAT( __count__, YGGR_TMP_PP_TEMPLATE_PARAMS_AT_C_TRANS_IMPL, __class_name__ )

#	define YGGR_PP_MPLEX_DETAIL_DETAIL_REVERT_TO_VECTOR_IMPL( __n__ ) \
		template<typename T> \
		struct detail_revert_to_vector< T, __n__ > \
			: public boost::mpl::vector< YGGR_TMP_PP_TEMPLATE_PARAMS_AT_C_TRANS( __n__, T ) > {};


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_PP_MPLEX_DETAIL_DETAIL_REVERT_TO_VECTOR_IMPL( __n__ )

#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_TEMPLATE_PARAMS_LEN() )
#	include BOOST_PP_LOCAL_ITERATE( )


#	undef YGGR_PP_MPLEX_DETAIL_DETAIL_REVERT_TO_VECTOR_IMPL

#	undef YGGR_TMP_PP_TEMPLATE_PARAMS_AT_C_TRANS
#	undef YGGR_TMP_PP_TEMPLATE_PARAMS_AT_C_TRANS_IMPL

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename T>
struct detail_revert_to_vector<T, 0>
	: public boost::mpl::vector<>
{
};

} // namespace detail

template<typename T>
struct revert_to_vector
	: public detail::detail_revert_to_vector<T, boost::mpl::size<T>::value> 
{
};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_REVERT_TO_VECTOR_HPP__
