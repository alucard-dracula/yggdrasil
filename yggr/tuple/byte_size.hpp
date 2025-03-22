// byte_size.hpp

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

#ifndef __YGGR_TUPLE_BYTE_SIZE_HPP__
#define __YGGR_TUPLE_BYTE_SIZE_HPP__

#include <yggr/bytes/byte_size.hpp>
#include <yggr/tuple/tuple.hpp>
#include <yggr/bytes/byte_size_sp_tpl.hpp>

#include <boost/mpl/or.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/type_traits/remove_cv.hpp>


namespace yggr
{
namespace bytes
{
namespace detail
{

#define YGGR_TMP_PP_T_PARAMS_TITLE() T

// cons
#define YGGR_TMP_PP_TUPLE_CONS_BYTE_SIZE( __cons__ ) \
	template<typename TH, typename TT > \
	struct byte_size_tpl_helper< __cons__<TH, TT> > \
		: public \
			boost::mpl::or_< \
				byte_size_helper<typename boost::remove_cv<TH>::type>, \
				byte_size_helper<typename boost::remove_cv<TT>::type> >::type { \
	public: \
		typedef __cons__<TH, TT> value_type; \
	public: \
		inline u64 operator()(const value_type& val) const { \
			typedef typename value_type::head_type cv_head_type; \
			typedef typename boost::remove_cv<cv_head_type>::type head_type; \
			typedef typename value_type::tail_type cv_tail_type; \
			typedef typename boost::remove_cv<cv_tail_type>::type tail_type; \
			typedef byte_size_helper<head_type> h1_type; \
			typedef byte_size_helper<tail_type> h2_type; \
			h1_type h1; \
			h2_type h2; \
			return h1(yggr::tuples::get_head(val)) \
					+ h2(yggr::tuples::get_tail(val)); } \
		\
		inline u64 operator()(void) const { \
			typedef typename value_type::head_type cv_head_type; \
			typedef typename boost::remove_cv<cv_head_type>::type head_type; \
			typedef typename value_type::tail_type cv_tail_type; \
			typedef typename boost::remove_cv<cv_tail_type>::type tail_type; \
			typedef byte_size_helper<head_type> h1_type; \
			typedef byte_size_helper<tail_type> h2_type; \
			h1_type h1; \
			h2_type h2; \
			return h1() + h2(); } }; \
	\
	template<typename TH> \
	struct byte_size_tpl_helper< __cons__<TH, yggr::tuples::null_type> > \
		: public byte_size_helper<TH>::type { \
	public: \
		typedef __cons__<TH, yggr::tuples::null_type> value_type; \
	public: \
		inline u64 operator()(const value_type& val) const { \
			typedef typename value_type::head_type cv_head_type; \
			typedef typename boost::remove_cv<cv_head_type>::type head_type; \
			typedef byte_size_helper<head_type> h1_type; \
			h1_type h1; \
			return h1(yggr::tuples::get_head(val)); } \
		\
		inline u64 operator()(void) const { \
			typedef typename value_type::head_type cv_head_type; \
			typedef typename boost::remove_cv<cv_head_type>::type head_type; \
			typedef byte_size_helper<head_type> h1_type; \
			h1_type h1; \
			return h1(); } }; \
	\
	template<> \
	struct byte_size_tpl_helper< __cons__<yggr::tuples::null_type, yggr::tuples::null_type> > \
		: public boost::mpl::false_ { \
		inline u64 operator()(const value_type& val) const { return 0; } \
		inline u64 operator()(void) const { return 0; } };

// tuple

template<typename T, std::size_t I = 0, std::size_t ISize = (::yggr::tuple_size<T>::value), std::size_t Diss = ISize - I >
struct byte_size_tuple_helper
	: public
		boost::mpl::or_
		<
			byte_size_helper< typename boost::remove_cv< typename ::yggr::tuple_element<I, T>::type >::type  >,
			byte_size_tuple_helper<T, I + 1, ISize>
		>::type
{
	typedef T tuple_type;
	typedef typename ::yggr::tuple_element<I, tuple_type>::type i_type;
	typedef byte_size_tuple_helper<T, I + 1, ISize> next_h_type;
	typedef byte_size_helper<i_type> ih_type;

	inline u64 operator()(const tuple_type& t) const
	{
		ih_type h;
		next_h_type next_h;
		return h(yggr::get<I>(t)) + next_h(t);
	}

	inline u64 operator()(void) const
	{
		ih_type h;
		next_h_type next_h;
		return h() + next_h();
	}
};

template<typename T, std::size_t I, std::size_t ISize>
struct byte_size_tuple_helper<T, I, ISize, 1>
	: public byte_size_helper< typename boost::remove_cv< typename ::yggr::tuple_element<I, T>::type >::type  >
{
	typedef T tuple_type;
	typedef typename ::yggr::tuple_element<I, tuple_type>::type i_type;
	typedef byte_size_helper<i_type> ih_type;

	inline u64 operator()(const tuple_type& t) const
	{
		ih_type h;
		return h(yggr::get<I>(t));
	}

	inline u64 operator()(void) const
	{
		ih_type h;
		return h();
	}
};

template<typename T, std::size_t ISize>
struct byte_size_tuple_helper<T, ISize, ISize, 0>
	: public boost::mpl::false_
{
	typedef T tuple_type;
	typedef yggr::tuples::null_type i_type;
	typedef yggr::tuples::null_type ih_type;

	inline u64 operator()(const tuple_type&) const
	{
		return 0;
	}

	inline u64 operator()(void) const
	{
		return 0;
	}
};

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#	define YGGR_TMP_PP_TUPLE_BYTE_SIZE( __tuple__ ) \
		template< typename ...Args > \
		struct byte_size_tpl_helper < __tuple__< Args... > > \
			: public \
				byte_size_tuple_helper< __tuple__< Args... > > { \
		public: \
			typedef __tuple__< Args... > value_type; \
		private: \
			typedef byte_size_tuple_helper< __tuple__< Args... > > h1_type; \
		public: \
			inline u64 operator()(const value_type& val) const { \
				h1_type h1; \
				return h1(val); } \
			\
			inline u64 operator()(void) const { \
				h1_type h1; \
				return h1(); } };

#else

#	define YGGR_TMP_PP_TUPLE_BYTE_SIZE( __tuple__ ) \
		template< YGGR_PP_TEMPLATE_PARAMS_TYPES( BOOST_TUPLE_LIMIT_LENGTH(), \
													typename YGGR_TMP_PP_T_PARAMS_TITLE() ) > \
		struct byte_size_tpl_helper < \
					__tuple__< \
						YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), \
														YGGR_TMP_PP_T_PARAMS_TITLE()) > > \
			: public \
				byte_size_tpl_helper< \
					typename \
						__tuple__< \
							YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), \
															YGGR_TMP_PP_T_PARAMS_TITLE()) >::inherited >::type { \
		public: \
			typedef \
				__tuple__< \
					YGGR_PP_TEMPLATE_PARAMS_TYPES(BOOST_TUPLE_LIMIT_LENGTH(), \
													YGGR_TMP_PP_T_PARAMS_TITLE()) > value_type; \
		public: \
			inline u64 operator()(const value_type& val) const { \
				typedef typename value_type::inherited cv_cons_type; \
				typedef typename boost::remove_cv<cv_cons_type>::type cons_type; \
				typedef byte_size_tpl_helper<cons_type> h1_type; \
				h1_type h1; \
				return h1(static_cast<const cons_type&>(val)); } \
			\
			inline u64 operator()(void) const { \
				typedef typename value_type::inherited cv_cons_type; \
				typedef typename boost::remove_cv<cv_cons_type>::type cons_type; \
				typedef byte_size_tpl_helper<cons_type> h1_type; \
				h1_type h1; \
				return h1(); } }; 

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

YGGR_TMP_PP_TUPLE_CONS_BYTE_SIZE(boost::tuples::cons)
YGGR_TMP_PP_TUPLE_CONS_BYTE_SIZE(yggr::tuples::cons)

YGGR_TMP_PP_TUPLE_BYTE_SIZE(boost::tuple)
YGGR_TMP_PP_TUPLE_BYTE_SIZE(yggr::tuple)

template<>
struct byte_size_tpl_helper< yggr::tuples::null_type >
	: public boost::mpl::false_
{
	typedef yggr::tuples::null_type value_type;

	inline u64 operator()(const value_type& val) const 
	{
		return 0; 
	}

	inline u64 operator()(void) const 
	{
		return 0; 
	} 
};

#if !defined(YGGR_NO_CXX11_HDR_TUPLE)
YGGR_TMP_PP_TUPLE_BYTE_SIZE(std::tuple)
#endif // #if !defined(YGGR_NO_CXX11_HDR_TUPLE)

#undef YGGR_TMP_PP_T_PARAMS_TITLE
#undef YGGR_TMP_PP_TUPLE_CONS_BYTE_SIZE
#undef YGGR_TMP_PP_TUPLE_BYTE_SIZE

} // namespace detail
} // namespace bytes
} // namespace yggr

#endif // __YGGR_TUPLE_BYTE_SIZE_HPP__
