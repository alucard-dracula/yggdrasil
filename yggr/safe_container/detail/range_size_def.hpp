//range_size_def.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_RANGE_SIZE_DEF_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_RANGE_SIZE_DEF_HPP__

#include <yggr/ppex/foo_params.hpp>

#include <boost/preprocessor/arithmetic/sub.hpp>

#include <boost/range/iterator.hpp>
#include <boost/range/size.hpp>

#include <boost/type_traits/remove_reference.hpp>

#define YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_SUPPORT( __type_count__, __type__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
			  template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > class Base > inline \
	typename YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::size_type \
		range_calculate_size(const YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )& c ) { \
		return c.size(); }

// boost::size() meta check support
#if defined(BOOST_RANGE_ENABLE_CONCEPT_ASSERT) && (BOOST_RANGE_ENABLE_CONCEPT_ASSERT)

#	define YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT_DETAIL( __type__ ) \
		private: \
			typedef __type__ c_type; \
		public: \
			typedef typename remove_reference<c_type>::type Rng; \
			typedef typename range_iterator<Rng const>::type const_iterator; \
			typedef typename range_iterator<Rng>::type iterator; \
			BOOST_CONCEPT_USAGE(SinglePassRangeConcept){}

#	define YGGR_PP_SAFE_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT_DETAIL( __type__ ) \
		private: \
			typedef __type__ c_type; \
		public: \
			typedef typename remove_reference<c_type>::type wrap_Rng; \
			typedef typename wrap_Rng::container_type Rng; \
			typedef typename range_iterator<Rng const>::type const_iterator; \
			typedef typename range_iterator<Rng>::type iterator; \
			BOOST_CONCEPT_USAGE(SinglePassRangeConcept){}

#else

#	define YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT_DETAIL( __type__ )

#	define YGGR_PP_SAFE_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT_DETAIL( __type__ )

#endif // BOOST_RANGE_ENABLE_CONCEPT_ASSERT

#if (defined(BOOST_RANGE_ENABLE_CONCEPT_ASSERT)) && (BOOST_RANGE_ENABLE_CONCEPT_ASSERT == 1)

#	define YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(__type_count__, __type__) \
		namespace boost { \
			template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
						template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > \
						class Base > \
			struct SinglePassRangeConcept< \
				YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base ) > { \
				YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT_DETAIL( \
					YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base ) ) }; }

#	define YGGR_PP_SAFE_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT_BASE(__type_count__, __type__) \
		namespace boost { \
			template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
						template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > \
						class Base > \
			struct SinglePassRangeConcept< \
				YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base ) > { \
				YGGR_PP_SAFE_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT_DETAIL( \
					YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base ) ) }; }

#else

#	define YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(__type_count__, __type__)

#	define YGGR_PP_SAFE_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT_BASE(__type_count__, __type__)

#endif // BOOST_RANGE_ENABLE_CONCEPT_ASSERT

#define YGGR_PP_SAFE_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(__type_count__, __type__) \
	YGGR_PP_SAFE_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT_BASE(__type_count__, __type__) \
	\
	namespace boost { \
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
						template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > \
						class Base > \
			struct range_size< \
				YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base ) > { \
				typedef typename \
					YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::size_type type; }; \
		\
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, typename T ), \
						template < YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __type_count__, 1 ), typename _T ) > \
						class Base > \
			struct range_size< \
				const YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base ) > { \
				typedef typename \
					YGGR_PP_TEMPLATE_TYPE2( __type__, YGGR_PP_TEMPLATE_PARAMS_TYPES( __type_count__, T ), Base )::size_type type; }; }

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namesapce safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_RANGE_SIZE_DEF_HPP__
