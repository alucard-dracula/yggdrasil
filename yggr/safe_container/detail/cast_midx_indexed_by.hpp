//cast_midx_indexed_by.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETIAL_CAST_MIDX_INDEXED_BY_HPP__
#define __YGGR_SAFE_CONTAINER_DETIAL_CAST_MIDX_INDEXED_BY_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/mplex/static_assert.hpp>
#include <yggr/safe_container/detail/cast_midx_index.hpp>

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
#	include <yggr/type_traits/integer_sequence.hpp>
#else
#	include <yggr/ppex/typedef.hpp>
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/multi_index/indexed_by.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename IndexedBy>
struct indexed_by_opt_cast;

template<typename IndexedBy>
struct indexed_by_org_cast;

} // namespace detail
} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace detail
{

// indexed_by_opt_cast

template<int N, typename T>
struct cast_to_indexed_by_detail;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename T, typename Seq>
struct cast_to_indexed_by_detail_helper;
	
template<typename T, std::size_t ...I>
struct cast_to_indexed_by_detail_helper<T, ::yggr::index_sequence<I...> >
{
	typedef
		boost::multi_index::indexed_by
		<
		typename boost::mpl::at_c<T, I>::type...
		> type;
};

template<int N, typename T>
struct cast_to_indexed_by_detail
	: public 
		cast_to_indexed_by_detail_helper
		<
			T, 
			yggr::make_index_sequence<static_cast<std::size_t>(N)> 
		>
{
};

#else

//indexed_by_opt_cast
#	if BOOST_MULTI_INDEX_LIMIT_INDEXED_BY_SIZE < BOOST_MPL_LIMIT_VECTOR_SIZE
#		define YGGR_MULTI_INDEX_INDEXED_BY_SIZE BOOST_MULTI_INDEX_LIMIT_INDEXED_BY_SIZE
#	else
#		define YGGR_MULTI_INDEX_INDEXED_BY_SIZE BOOST_MPL_LIMIT_VECTOR_SIZE
#	endif // BOOST_MULTI_INDEX_LIMIT_INDEXED_BY_SIZE < BOOST_MPL_LIMIT_VECTOR_SIZE


#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename T> \
		struct cast_to_indexed_by_detail<__n__, T> { \
			typedef \
				boost::multi_index::indexed_by< \
					BOOST_PP_ENUM(__n__, YGGR_MULTI_INDEX_GET_INDEX, T ) > type; };

#	define YGGR_MULTI_INDEX_GET_INDEX(__z__, __n__, __var__) \
		typename boost::mpl::at_c<__var__, __n__>::type

#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_MULTI_INDEX_INDEXED_BY_SIZE )
#	include BOOST_PP_LOCAL_ITERATE( )
#	undef YGGR_MULTI_INDEX_CAST_INDEXED_BY_OPT_CAST

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename T>
struct cast_to_inexed_by
	: public
		cast_to_indexed_by_detail
		<
			boost::mpl::size<T>::value, 
			T
		>
{
private:
	BOOST_MPL_ASSERT((boost::mpl::bool_<(boost::mpl::size<T>::value > 0)>));
};

// indexed_by_opt_cast
template<typename IndexedBy>
struct indexed_by_opt_cast
	: public 
		indexed_by_opt_cast
		<
			typename cast_to_inexed_by<IndexedBy>::type
		>
{
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename ...T>
struct indexed_by_opt_cast< boost::multi_index::indexed_by<T...> >
{
	typedef 
		boost::multi_index::indexed_by
		<
			typename index_wrap_opt_cast< T >::type...
		> type;
};

#else

#	define YGGR_MULTI_INDEX_INDEXED_BY_OPT_CAST(__z__, __n__, __var__) \
		typename index_wrap_opt_cast< BOOST_PP_CAT(__var__, __n__) >::type

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_MULTI_INDEX_INDEXED_BY_SIZE, typename T)>
struct indexed_by_opt_cast
		< 
			boost::multi_index::indexed_by
			<
				YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_MULTI_INDEX_INDEXED_BY_SIZE, T)
			> 
		>
{
	typedef 
		boost::multi_index::indexed_by
		<
			BOOST_PP_ENUM(
				YGGR_MULTI_INDEX_INDEXED_BY_SIZE,
				YGGR_MULTI_INDEX_INDEXED_BY_OPT_CAST, T )
		> type;
};

#	undef YGGR_MULTI_INDEX_INDEXED_BY_OPT_CAST

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


// indexed_by_org_cast
template<typename IndexedBy>
struct indexed_by_org_cast
	: public 
		indexed_by_org_cast
		<
			typename cast_to_inexed_by<IndexedBy>::type
		>
{
};

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename ...T>
struct indexed_by_org_cast< boost::multi_index::indexed_by<T...> >
{
	typedef 
		boost::multi_index::indexed_by
		<
			typename index_wrap_org_cast< T >::type...
		> type;
};


#else

#define YGGR_MULTI_INDEX_INDEXED_BY_ORG_CAST(__z__, __n__, __var__) \
	typename index_wrap_org_cast< BOOST_PP_CAT(__var__, __n__) >::type

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_MULTI_INDEX_INDEXED_BY_SIZE, typename T)>
struct indexed_by_org_cast
		< 
			boost::multi_index::indexed_by
			<
				YGGR_PP_TEMPLATE_PARAMS_TYPES(YGGR_MULTI_INDEX_INDEXED_BY_SIZE, T)
			> 
		>
{
	typedef 
		boost::multi_index::indexed_by
		<
			BOOST_PP_ENUM(
				YGGR_MULTI_INDEX_INDEXED_BY_SIZE,
				YGGR_MULTI_INDEX_INDEXED_BY_ORG_CAST, T )
		> type;
};

#undef YGGR_MULTI_INDEX_INDEXED_BY_ORG_CAST
#undef YGGR_MULTI_INDEX_INDEXED_BY_SIZE

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace detail
} // namespace safe_container
} // namespace yggr


#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETIAL_CAST_MIDX_INDEXED_BY_HPP__