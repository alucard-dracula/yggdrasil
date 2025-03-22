//cast_midx_base_type.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETIAL_CAST_MIDX_BASE_TYPE_HPP__
#define __YGGR_SAFE_CONTAINER_DETIAL_CAST_MIDX_BASE_TYPE_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/base/static_constant.hpp>

#include <yggr/safe_container/detail/boost_midx_base_type_opt.hpp>
#include <yggr/safe_container/detail/cast_midx_indexed_by.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

// nth_layer_dewrap
template<typename Nth_Layer>
struct nth_layer_dewrap;

// multi_index_base_type_dewrap
template<typename MIBT>
struct multi_index_base_type_dewrap;

// nth_layer_cast
/*
 * the nth_layer_cast is rebuild nth_layer
 * , so index_base not need cast
*/
template<typename Nth_Layer>
struct nth_layer_org_cast;

template<typename Nth_Layer>
struct nth_layer_opt_cast;

// multi_index_base_type_cast
template<typename MIBT>
struct multi_index_base_type_org_cast;

template<typename MIBT>
struct multi_index_base_type_opt_cast;

} // namespace detail
} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace detail
{

// nth_layer_dewrap
#define YGGR_PP_NTH_LAYER_DEWRAP_DEF( __layer__ ) \
	template<int N, typename V, typename I, typename A> \
	struct nth_layer_dewrap< __layer__<N, V, I, A> > { \
		YGGR_STATIC_CONSTANT(std::size_t, size = N); \
		typedef V value_type; \
		typedef I list_type; \
		typedef A allocator_type; }; 

YGGR_PP_NTH_LAYER_DEWRAP_DEF(nth_layer_opt);
YGGR_PP_NTH_LAYER_DEWRAP_DEF(boost::multi_index::detail::nth_layer);

#undef YGGR_PP_NTH_LAYER_DEWRAP_DEF

#define YGGR_PP_MULTI_INDEX_BASE_TYPE_DEWRAP_DEF( __midx_base_type__ ) \
	template<typename V, typename I, typename A> \
	struct multi_index_base_type_dewrap< \
			__midx_base_type__<V, I, A> > { \
		typedef V value_type; \
		typedef I list_type; \
		typedef A allocator_type; };


YGGR_PP_MULTI_INDEX_BASE_TYPE_DEWRAP_DEF(multi_index_base_opt_type);
YGGR_PP_MULTI_INDEX_BASE_TYPE_DEWRAP_DEF(boost::multi_index::detail::multi_index_base_type);

#undef YGGR_PP_MULTI_INDEX_BASE_TYPE_DEWRAP_DEF

// nth_layer_org_cast
#define YGGR_PP_NTH_LAYER_CAST_DEF(__name__, __dst__, __list_cast__) \
	template<typename Nth_Layer> \
	struct __name__ { \
	private: \
		typedef nth_layer_dewrap<Nth_Layer> dewrap_type; \
	public: \
		typedef __dst__< \
				static_cast<int>(dewrap_type::size), \
				typename dewrap_type::value_type, \
				typename __list_cast__ < \
					typename dewrap_type::list_type >::type, \
				typename dewrap_type::allocator_type > type; };

YGGR_PP_NTH_LAYER_CAST_DEF(nth_layer_org_cast, 
							boost::multi_index::detail::nth_layer, 
							indexed_by_org_cast);

YGGR_PP_NTH_LAYER_CAST_DEF(nth_layer_opt_cast, 
							nth_layer_opt, 
							indexed_by_opt_cast);

#undef YGGR_PP_NTH_LAYER_CAST_DEF

// multi_index_base_type_org_cast
#define YGGR_PP_MULTI_INDEX_BASE_TYPE_CAST_DEF(__name__, __dst__, __list_cast__) \
	template<typename MIBT> \
	struct __name__ { \
	private: \
		typedef multi_index_base_type_dewrap<MIBT> dewrap_type; \
	public: \
		typedef __dst__ < \
				typename dewrap_type::value_type, \
				typename __list_cast__ < \
					typename dewrap_type::list_type >::type, \
				typename dewrap_type::allocator_type > type; };

YGGR_PP_MULTI_INDEX_BASE_TYPE_CAST_DEF(multi_index_base_type_org_cast,
										boost::multi_index::detail::multi_index_base_type,
										indexed_by_org_cast);

YGGR_PP_MULTI_INDEX_BASE_TYPE_CAST_DEF(multi_index_base_type_opt_cast,
										multi_index_base_opt_type,
										indexed_by_opt_cast);

#undef YGGR_PP_MULTI_INDEX_BASE_TYPE_CAST_DEF


} // namespace detail
} // namespace safe_container
} // namespace yggr


#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETIAL_CAST_MIDX_BASE_TYPE_HPP__