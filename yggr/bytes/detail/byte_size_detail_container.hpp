//byte_size_detail_container.hpp

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

#ifndef __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_CONTAINER_HPP__
#define __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_CONTAINER_HPP__

#include <yggr/bytes/detail/byte_size_detail_basic.hpp>
#include <yggr/container/unwrap_container.hpp>

namespace yggr
{
namespace bytes
{
namespace detail
{

template<typename T, bool is_class> 
struct byte_size_tpl_container_inner_helper; 
		
template<typename T> 
struct byte_size_tpl_container_inner_helper<T, true> 
{ 
	typedef T value_type;
	typedef ::yggr::container::unwrap_container<value_type> unwrap_type; 
	typedef typename unwrap_type::type now_cont_type; 
	typedef typename boost::range_value<now_cont_type>::type cv_sub_val_type; 
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type; 
	typedef byte_size_helper<sub_val_type> h_type; 

	inline u64 operator()(const value_type& cont) const 
	{ 
		
		h_type h; 
		unwrap_type unwrap; 
		const now_cont_type& cont_cref = unwrap(cont); 
		return std::accumulate(boost::begin(cont_cref), boost::end(cont_cref), 0, 
								boost::bind(&acc_op::foo<sub_val_type, h_type>, 
												_1, _2, boost::cref(h))); 
	} 
}; 
		
template<typename T> 
struct byte_size_tpl_container_inner_helper<T, false> 
{ 
	typedef T value_type;

	typedef ::yggr::container::unwrap_container<value_type> unwrap_type; 
	typedef typename unwrap_type::type now_cont_type; 
	typedef typename boost::range_value<now_cont_type>::type cv_sub_val_type; 
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type; 
	typedef byte_size_helper<sub_val_type> h_type; 

	inline u64 operator()(const value_type& cont) const 
	{
		h_type h; 
		unwrap_type unwrap; 
		return boost::size(unwrap(cont)) * h(); 
	} 
}; 
		

template<typename T>
struct byte_size_tpl_container_helper
	: public boost::mpl::true_
{
public: 
	typedef T value_type; 

private:
	typedef typename value_type::value_type cv_sub_val_type; 
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type; 
	typedef boost::is_class<sub_val_type> is_class_type; 
	typedef byte_size_helper<sub_val_type> sub_value_helper_type; 
	typedef 
		byte_size_tpl_container_inner_helper
		<
			value_type, 
			(is_class_type::value && sub_value_helper_type::value)
		> ih_type; 
		
public: 
	inline u64 operator()(const value_type& cont) const
	{
		ih_type ih; 
		return ih(cont); 
	}
};

} // namespace detail
} // namespace bytes
} // namespace yggr

//#define YGGR_PP_MAKE_CONTAINER_BYTE_SIZE_HELPER( __n__ ) \
//	namespace yggr { namespace bytes { namespace detail { \
//		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) \
//					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
//				template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename _T ) > class C > \
//	struct byte_size_helper< C < YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > > \
//		: public byte_size_tpl_container_helper< C< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) > > {}; }}}
//
//#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
//		YGGR_PP_MAKE_CONTAINER_BYTE_SIZE_HELPER( __n__ )
//
//#	define BOOST_PP_LOCAL_LIMITS ( 2, 10 )
//#	include BOOST_PP_LOCAL_ITERATE(  )



namespace yggr
{
namespace bytes
{
namespace detail
{

template<typename T, bool is_class> 
struct byte_size_tplic_container_inner_helper; 
			
template<typename T> 
struct byte_size_tplic_container_inner_helper<T, true> 
{
	typedef T value_type;
	typedef typename value_type::base_type now_type; 
	typedef byte_size_helper<now_type> h_type; 

	typedef byte_size_tplic_container_inner_helper this_type;

	inline u64 handler_accmulate(const now_type& cont) const 
	{
		h_type h; 
		return h(cont); 
	} 
				
	inline u64 operator()(const value_type& cont) const 
	{ 
		return cont.using_handler(boost::bind(&this_type::handler_accmulate, this, _1)); 
	} 
}; 
				
template<typename T> 
struct byte_size_tplic_container_inner_helper<T, false> 
{ 
	typedef T value_type;
	typedef typename value_type::value_type cv_sub_val_type; 
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type; 
	typedef byte_size_helper<sub_val_type> h_type; 

	inline u64 operator()(const value_type& cont) const 
	{
		h_type h; 
		return cont.size() * h(); 
	} 
}; 

template<typename T>
struct byte_size_tplic_container_helper
	: public boost::mpl::true_
{ 
public: 
	typedef T value_type;

private:
	typedef typename value_type::value_type cv_sub_val_type; 
	typedef typename boost::remove_cv<cv_sub_val_type>::type sub_val_type; 
	typedef boost::is_class<sub_val_type> is_class_type; 
	typedef byte_size_helper<sub_val_type> sub_value_helper_type; 
	typedef 
		byte_size_tplic_container_inner_helper
		<
			value_type,
			(is_class_type::value && sub_value_helper_type::value)
		> ih_type; 
			
public: 
	inline u64 operator()(const value_type& cont) const 
	{
		ih_type ih; 
		return ih(cont); 
	}
};

} // namespace detail
} // namespace bytes
} // namespace yggr

namespace yggr
{
namespace bytes
{
namespace detail
{

#ifdef _MSC_VER

//	think you for UnitUniverse (QQ1020676476) fix This macro problem in the MSVC
#	define YGGR_PP_MAKE_SAFE_CONTAINER_BYTE_SIZE_HELPER_HEADER( __n__ ) \
	template< template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename _T ) \
								YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
								template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __n__, 1 ), typename __T ) > class __IC > class C \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __n__, 1 ), typename _T ) > class IC \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) >
#else

#	define YGGR_PP_MAKE_SAFE_CONTAINER_BYTE_SIZE_HELPER_HEADER( __n__ ) \
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __n__, 1 ), typename _T ) > class IC \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename _T ) \
								YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
								template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_PP_SUB( __n__, 1 ), typename __T ) > class __IC > class C >

#endif // _MSC_VER

#	define YGGR_PP_MAKE_SAFE_CONTAINER_BYTE_SIZE_HELPER( __n__ ) \
		YGGR_PP_MAKE_SAFE_CONTAINER_BYTE_SIZE_HELPER_HEADER( __n__ ) \
		struct byte_size_helper< C< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) \
										YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
										IC> > \
			: public byte_size_tplic_container_helper< \
						C< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ) \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
							IC > > {};

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_MAKE_SAFE_CONTAINER_BYTE_SIZE_HELPER( __n__ )

#	define BOOST_PP_LOCAL_LIMITS ( 3, 10 )
#	include BOOST_PP_LOCAL_ITERATE(  )

} // namespace detail
} // namespace bytes
} // namespace yggr

#endif // __YGGR_BYTES_DETAIL_BYTE_SIZE_DETAIL_CONTAINER_HPP__
