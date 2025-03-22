//vector.hpp

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

#ifndef __YGGR_CONTAINER_VECTOR_HPP__
#define __YGGR_CONTAINER_VECTOR_HPP__

#include <yggr/container/container_selector.hpp>
#include <yggr/move/move.hpp>
#include <yggr/typeof/typeof.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/mplex/typename_expand.hpp>

#include <yggr/algorithm/lexicographical_c_compare.hpp>

#include <yggr/container/align_vector.hpp>

#include <yggr/container/is_isomeric_same_vector.hpp>
#if !(BOOST_VERSION < 106700)
#	include <yggr/container/is_isomeric_same_options_vector.hpp>
#endif // !(BOOST_VERSION < 106700)

#include <yggr/container/basic_function_adapter.hpp>
#include <yggr/container/detail/is_xxx_container.hpp>

#include <yggr/container/detail/call_erase_helper.hpp>
#include <yggr/container/detail/call_insert_helper.hpp>
#include <yggr/container/detail/swap_def.hpp>
#include <yggr/container/detail/comparer_def.hpp>
#include <yggr/container/detail/range_size_def.hpp>

#include <yggr/container/detail/has_data.hpp>
#include <yggr/container/detail/has_emplace.hpp>
#include <yggr/container/detail/has_shrink_to_fit.hpp>

#include <boost/ref.hpp>
#include <boost/container/vector.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <vector>


YGGR_PP_IS_XXX_CONTAINER_DEF(2, std_vector, std::vector);

#if BOOST_VERSION < 106700
	YGGR_PP_IS_XXX_CONTAINER_DEF(2, boost_vector, boost::container::vector);
#else
	YGGR_PP_IS_XXX_CONTAINER_DEF(3, boost_vector, boost::container::vector);
#endif // BOOST_VERSION < 106700


// compatibility caller
namespace yggr
{
namespace container
{

// shrink_to_fit
template<typename T, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_shrink_to_fit< std::vector<T, A> >,
		void
	>::type
	shrink_to_fit(std::vector<T, A>& c)
{
	c.shrink_to_fit();
}

template<typename T, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_shrink_to_fit< std::vector<T, A> >,
		void
	>::type
	shrink_to_fit(std::vector<T, A>& c)
{
	std::vector<T, A> tmp(c.begin(), c.end(), c.get_allocator());
	c.swap(tmp);
}

#if BOOST_VERSION < 106700

template<typename T, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_shrink_to_fit< boost::container::vector<T, A> >,
		void
	>::type
	shrink_to_fit(boost::container::vector<T, A>& c)
{
	c.shrink_to_fit();
}

template<typename T, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_shrink_to_fit<  boost::container::vector<T, A> >,
		void
	>::type
	shrink_to_fit(boost::container::vector<T, A>& c)
{
	boost::container::vector<T, A> tmp(c.begin(), c.end(), c.get_allocator());
	c.swap(tmp);
}

#else

template<typename T, typename A, typename O> inline
void shrink_to_fit(boost::container::vector<T, A, O>& c)
{
	c.shrink_to_fit();
}

#endif // BOOST_VERSION < 106700

// data
template<typename T, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_data< std::vector<T, A> >,
		T*
	>::type
	data(std::vector<T, A>& c)
{
	return c.data();
}

template<typename T, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_data< std::vector<T, A> >,
		T*
	>::type
	data(std::vector<T, A>& c)
{
	return c.empty()? 0 : &(c[0]);
}

template<typename T, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_data< std::vector<T, A> >,
		const T*
	>::type
	data(const std::vector<T, A>& c)
{
	return c.data();
}

template<typename T, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_data< std::vector<T, A> >,
		const T*
	>::type
	data(const std::vector<T, A>& c)
{
	return c.empty()? 0 : &(c[0]);
}

#if BOOST_VERSION < 106700

template<typename T, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_data< boost::container::vector<T, A> >,
		T*
	>::type
	data(boost::container::vector<T, A>& c)
{
	return c.data();
}

template<typename T, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_data< boost::container::vector<T, A> >,
		T*
	>::type
	data(boost::container::vector<T, A>& c)
{
	return c.empty()? 0 : &(c[0]);
}

template<typename T, typename A> inline
typename
	boost::enable_if
	<
		detail::has_any_data< boost::container::vector<T, A> >,
		const T*
	>::type
	data(const boost::container::vector<T, A>& c)
{
	return c.data();
}

template<typename T, typename A> inline
typename
	boost::disable_if
	<
		detail::has_any_data< boost::container::vector<T, A> >,
		const T*
	>::type
	data(const boost::container::vector<T, A>& c)
{
	return c.empty()? 0 : &(c[0]);
}

#else

template<typename T, typename A, typename O> inline
T* data(boost::container::vector<T, A, O>& c)
{
	return c.data();
}

template<typename T, typename A, typename O> inline
const T* data(const boost::container::vector<T, A, O>& c)
{
	return c.data();
}

#endif // #if BOOST_VERSION < 106700

//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

// emplace
template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< std::vector<T, A> >,
		typename std::vector<T, A>::iterator
	>::type
	emplace(std::vector<T, A>& c,
			typename std::vector<T, A>::iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< std::vector<T, A> >,
		typename std::vector<T, A>::iterator
	>::type
	emplace(std::vector<T, A>& c,
			typename std::vector<T, A>::iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return insert(c, pos, T(boost::forward<Args>(args)...));
}

// emplace_back
template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_back< std::vector<T, A> >,
		typename std::vector<T, A>::reference
	>::type
	emplace_back(std::vector<T, A>& c,
					BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_back< std::vector<T, A> >,
		typename std::vector<T, A>::reference
	>::type
	emplace_back(std::vector<T, A>& c,
					BOOST_FWD_REF(Args)... args)
{
	c.push_back(T(boost::forward<Args>(args)...));
	return c.back();
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< std::vector<T, A> >, \
									typename std::vector<T, A>::iterator>::type \
			emplace(std::vector<T, A>& c, \
						typename std::vector<T, A>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< std::vector<T, A> >, \
									typename std::vector<T, A>::iterator>::type \
			emplace(std::vector<T, A>& c, \
						typename std::vector<T, A>::iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::vector<T, A>::reference \
			emplace_back(std::vector<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_back(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.back(); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


#if defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

#	if BOOST_VERSION < 106700
// emplace
template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< boost::container::vector<T, A> >,
		typename boost::container::vector<T, A>::iterator
	>::type
	emplace(boost::container::vector<T, A>& c,
			typename boost::container::vector<T, A>::const_iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< boost::container::vector<T, A> >,
		typename boost::container::vector<T, A>::iterator
	>::type
	emplace(boost::container::vector<T, A>& c,
			typename boost::container::vector<T, A>::iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return insert(c, pos, T(boost::forward<Args>(args)...));
}

// emplace_back
template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_back< boost::container::vector<T, A> >,
		typename boost::container::vector<T, A>::reference
	>::type
	emplace_back(boost::container::vector<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_back< boost::container::vector<T, A> >,
		typename boost::container::vector<T, A>::reference
	>::type
	emplace_back(boost::container::vector<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_back(T(boost::forward<Args>(args)...));
	return c.back();
}

#	else

// emplace
template<typename T, typename A, typename O, typename ...Args> inline
typename boost::container::vector<T, A, O>::iterator
	emplace(boost::container::vector<T, A, O>& c,
			typename boost::container::vector<T, A, O>::const_iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

// emplace_back
template<typename T, typename A, typename O, typename ...Args> inline
typename boost::container::vector<T, A, O>::reference
	emplace_back(boost::container::vector<T, A, O>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

#	endif // #if BOOST_VERSION < 106700

#else

#	if BOOST_VERSION < 106700



#	if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#		define YGGR_TMP_PP_EMPLACE_DEF_HAS_EMPLACE(__n__) \
			template<typename T, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::enable_if<detail::has_any_emplace< boost::container::vector<T, A> >, \
										typename boost::container::vector<T, A>::iterator>::type \
				emplace(boost::container::vector<T, A>& c, \
							typename boost::container::vector<T, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename T, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::enable_if< \
				detail::has_any_emplace_back< boost::container::vector<T, A> >, \
				typename boost::container::vector<T, A>::reference>::type \
				emplace_back(boost::container::vector<T, A>& c YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				c.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
				return c.back(); }

#	else

#		define YGGR_TMP_PP_EMPLACE_DEF_HAS_EMPLACE(__n__) \
			template<typename T, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::enable_if<detail::has_any_emplace< boost::container::vector<T, A> >, \
										typename boost::container::vector<T, A>::iterator>::type \
				emplace(boost::container::vector<T, A>& c, \
							typename boost::container::vector<T, A>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
			\
			template<typename T, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::enable_if< \
				detail::has_any_emplace_back< boost::container::vector<T, A> >, \
				typename boost::container::vector<T, A>::reference>::type \
				emplace_back(boost::container::vector<T, A>& c YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				c.push_back(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
				return c.back(); }


#	endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#		define YGGR_TMP_PP_EMPLACE_DEF_NOT_HAS_EMPLACE(__n__) \
			template<typename T, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::disable_if<detail::has_any_emplace< boost::container::vector<T, A> >, \
										typename boost::container::vector<T, A>::iterator>::type \
				emplace(boost::container::vector<T, A>& c, \
							typename boost::container::vector<T, A>::iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
			\
			template<typename T, typename A \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::disable_if< \
				detail::has_any_emplace_back< boost::container::vector<T, A> >, \
				typename boost::container::vector<T, A>::reference>::type \
				emplace_back(boost::container::vector<T, A>& c YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				c.push_back(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
				return c.back(); }

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			YGGR_TMP_PP_EMPLACE_DEF_HAS_EMPLACE(__n__) \
			YGGR_TMP_PP_EMPLACE_DEF_NOT_HAS_EMPLACE(__n__)

#	else

#		define YGGR_TMP_PP_EMPLACE_DEF_HAS_EMPLACE(__n__)
#		define YGGR_TMP_PP_EMPLACE_DEF_NOT_HAS_EMPLACE(__n__)

#		define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
			template<typename T, typename A, typename O \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::container::vector<T, A, O>::iterator \
				emplace(boost::container::vector<T, A, O>& c, \
							typename boost::container::vector<T, A, O>::const_iterator pos \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
			\
			template<typename T, typename A, typename O \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
			typename boost::container::vector<T, A, O>::reference \
				emplace_back(boost::container::vector<T, A, O>& c YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
				c.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
				return c.back(); }

#	endif // BOOST_VERSION < 106700

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
				YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg

#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF
#	undef YGGR_TMP_PP_EMPLACE_DEF_NOT_HAS_EMPLACE
#	undef YGGR_TMP_PP_EMPLACE_DEF_HAS_EMPLACE

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

// erase
template<typename T, typename A, typename Val> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename std::vector<T, A>::iterator>,
				boost::is_same<Val, typename std::vector<T, A>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename std::vector<T, A>::reverse_iterator>,
				boost::is_same<Val, typename std::vector<T, A>::const_reverse_iterator>
			>
		>,
		typename std::vector<T, A>::size_type
	>::type
	erase(std::vector<T, A>& c, const Val& value)
{
	typedef std::vector<T, A> vec_type;
	typedef typename vec_type::iterator vec_iter_type;
	typedef typename vec_type::size_type vec_size_type;

	vec_iter_type it = std::remove(c.begin(), c.end(), value);
	vec_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename Pred> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename std::vector<T, A>::size_type
	erase_if(std::vector<T, A>& c, Pred pred)
{
	typedef std::vector<T, A> vec_type;
	typedef typename vec_type::iterator vec_iter_type;
	typedef typename vec_type::size_type vec_size_type;

	vec_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	vec_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

#if BOOST_VERSION < 106700

// erase
template<typename T, typename A, typename Val> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename boost::container::vector<T, A>::iterator>,
				boost::is_same<Val, typename boost::container::vector<T, A>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename boost::container::vector<T, A>::reverse_iterator>,
				boost::is_same<Val, typename boost::container::vector<T, A>::const_reverse_iterator>
			>
		>,
		typename boost::container::vector<T, A>::size_type
	>::type
	erase(boost::container::vector<T, A>& c, const Val& value)
{
	typedef boost::container::vector<T, A> vec_type;
	typedef typename vec_type::iterator vec_iter_type;
	typedef typename vec_type::size_type vec_size_type;

	vec_iter_type it = std::remove(c.begin(), c.end(), value);
	vec_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename Pred> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename boost::container::vector<T, A>::size_type
	erase_if(boost::container::vector<T, A>& c, Pred pred)
{
	typedef boost::container::vector<T, A> vec_type;
	typedef typename vec_type::iterator vec_iter_type;
	typedef typename vec_type::size_type vec_size_type;

	vec_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	vec_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

#else

// erase
template<typename T, typename A, typename O, typename Val> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename boost::container::vector<T, A, O>::iterator>,
				boost::is_same<Val, typename boost::container::vector<T, A, O>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename boost::container::vector<T, A, O>::reverse_iterator>,
				boost::is_same<Val, typename boost::container::vector<T, A, O>::const_reverse_iterator>
			>
		>,
		typename boost::container::vector<T, A, O>::size_type
	>::type
	erase(boost::container::vector<T, A, O>& c, const Val& value)
{
	typedef boost::container::vector<T, A, O> vec_type;
	typedef typename vec_type::iterator vec_iter_type;
	typedef typename vec_type::size_type vec_size_type;

	vec_iter_type it = std::remove(c.begin(), c.end(), value);
	vec_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename O, typename Pred> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename boost::container::vector<T, A, O>::size_type
	erase_if(boost::container::vector<T, A, O>& c, Pred pred)
{
	typedef boost::container::vector<T, A, O> vec_type;
	typedef typename vec_type::iterator vec_iter_type;
	typedef typename vec_type::size_type vec_size_type;

	vec_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	vec_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

#endif // BOOST_VERSION < 106700

} // namespace container
} // namespace yggr

#if BOOST_VERSION < 105700

#include <functional>

namespace boost
{
namespace container
{

template<typename T, typename A> inline
bool operator>(const boost::container::vector<T, A>& l,
				const boost::container::vector<T, A>& r)
{
	return (boost::addressof(l) != boost::addressof(r))
			&& lexicographical_compare(
				l.begin(), l.end(),
				r.begin(), r.end(),
				std::greater<T>());
}

template<typename T, typename A> inline
bool operator>=(const boost::container::vector<T, A>& l,
				const boost::container::vector<T, A>& r)
{
	return !(l < r);
}

template<typename T, typename A> inline
bool operator<=(const boost::container::vector<T, A>& l,
				const boost::container::vector<T, A>& r)
{
	return !(l > r);
}

} // namespace container
} // namespace boost

#endif // BOOST_VERSION < 105700

#if !(BOOST_VERSION < 106700)

#include <yggr/container/is_isomeric_same_options_vector.hpp>

namespace boost
{
namespace container
{

YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_COMPARER_FULL(
	3,
	::boost::container::vector,
	::yggr::container::is_isomeric_same_options_vector,
	YGGR_CXX14_CONSTEXPR_OR_INLINE)

} // namespace container
} // namespace boost

namespace boost
{
namespace container
{
namespace detail
{
namespace swap_support
{

	YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_SWAP(
		3,
		::boost::container::vector,
		::yggr::container::is_isomeric_same_options_vector,
		YGGR_CXX14_CONSTEXPR_OR_INLINE )

} // namespace swap_support
} // namespace detail

using detail::swap_support::swap;

} // namespace container
} // namespace boost

namespace std
{
	using ::boost::container::detail::swap_support::swap;
}// namespace std

namespace boost
{
	using container::detail::swap_support::swap;
} // namespace boost

#endif // !(BOOST_VERSION < 106700)

// boost::size support
namespace std
{

	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, std::vector)

} // namespace std

namespace boost
{
namespace container
{

#if BOOST_VERSION < 106700

	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, boost::container::vector)

#else

	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, boost::container::vector)

#endif // BOOST_VERSION < 106700

} // namespace container
} // namespace boost


#if !(BOOST_VERSION < 106700)

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T, typename A>
class vector;

} // namespace detail
} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{

// boost 106700 append options
// boost 106900 allocator = void
template<typename T,
			typename A =
				typename mplex::typename_expand_get<boost::container::vector<T>, 1>::type >
class vector
	: public boost::container::vector<T, A>
{
public:
	typedef A tpl_arg_allocator_type;

public:
	typedef boost::container::vector<T, A> base_type;

protected:
	typedef typename
		boost::container::get_vector_opt
		<
			void,
			typename base_type::size_type
		>::type options_type;

public:
	typedef typename base_type::value_type value_type;
	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::difference_type difference_type;
	typedef typename base_type::allocator_type allocator_type;
	typedef typename base_type::stored_allocator_type stored_allocator_type;
	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;
	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;

protected:
	typedef boost::container::allocator_traits<allocator_type> allocator_traits_type;

	template <typename U, typename UAllocator>
	friend class vector;

private:
	typedef vector this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(void) YGGR_NOEXCEPT_IF(boost::container::dtl::is_nothrow_default_constructible<allocator_type>::value)
		: base_type()
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	vector(const allocator_type& a) YGGR_NOEXCEPT_OR_NOTHROW
		: base_type(a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	vector(size_type n)
		: base_type(n)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE explicit
	vector(size_type n, const allocator_type& a)
		: base_type(n, value_type(), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(size_type n, const value_type& v)
		: base_type(n, v)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(size_type n, const value_type& v, const allocator_type& a)
		: base_type(n, v, a)
	{
	}

	template<typename InIt> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(InIt first, InIt last
			BOOST_CONTAINER_DOCIGN(
				BOOST_MOVE_I
				typename
					boost::container::dtl::disable_if_c
					<
						boost::container::dtl::is_convertible<InIt BOOST_MOVE_I size_type>::value
						BOOST_MOVE_I
						boost::container::dtl::nat
					>::type * = 0) )
		: base_type(first, last)
	{
	}

	template <typename InIt> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(InIt first, InIt last, const allocator_type& a
			BOOST_CONTAINER_DOCIGN(
				BOOST_MOVE_I
				typename
					boost::container::dtl::disable_if_c
					<
						boost::container::dtl::is_convertible<InIt BOOST_MOVE_I size_type>::value
						BOOST_MOVE_I
						boost::container::dtl::nat
					>::type * = 0) )
		: base_type(first, last, a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(std::initializer_list<value_type> il, const allocator_type& a = allocator_type())
		: base_type(il, a)
	{
	}

#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(BOOST_RV_REF(base_type) right) YGGR_NOEXCEPT_OR_NOTHROW
		: base_type(boost::move(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(const base_type& right)
		: base_type(right)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(const this_type& right)
		: base_type(static_cast<const base_type&>(right))
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(BOOST_RV_REF(base_type) right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(const base_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(const this_type& right, const allocator_type& a)
		: base_type(static_cast<const base_type&>(right), a)
	{
	}

#if !defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(BOOST_RV_REF_BEG
				boost::container::vector<value_type, A2, O2>
			BOOST_RV_REF_END right,
			typename
				boost::enable_if_c
				<
					boost::container::dtl::is_version<A2, 0>::value
						&& !is_isomeric_same_options_vector<base_type, boost::container::vector<value_type, A2, O2> >::value,
					mplex::sfinae_type
				>::type sfinae = 0 )
		: base_type(boost::move(right))
	{
	}

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(BOOST_RV_REF_BEG
				vector<value_type, A2>
			BOOST_RV_REF_END right,
			typename
				boost::enable_if_c
				<
					(boost::container::dtl::is_version<A2, 0>::value
						&& !is_isomeric_same_options_vector<base_type, typename vector<value_type, A2>::base_type >::value),
					mplex::sfinae_type
				>::type sfinae = 0 )
		: base_type(boost::move(static_cast<typename vector<value_type, A2>::base_type&>(right)))
	{
	}

#endif   //!defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(BOOST_RV_REF_BEG
				boost::container::vector<value_type, A2, O2>
			BOOST_RV_REF_END right,
			typename
				boost::enable_if
				<
					is_isomeric_same_options_vector<base_type, boost::container::vector<value_type, A2, O2> >,
					mplex::sfinae_type
				>::type sfinae = 0 )
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::vector<value_type, A2, O2>&>(right))))
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(const boost::container::vector<value_type, A2, O2>& right,
			typename
				boost::enable_if
				<
					is_isomeric_same_options_vector<base_type, boost::container::vector<value_type, A2, O2> >,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(BOOST_RV_REF_BEG
				vector<value_type, A2>
			BOOST_RV_REF_END right,
			typename
				boost::enable_if
				<
					is_isomeric_same_options_vector<base_type, typename vector<value_type, A2>::base_type >,
					mplex::sfinae_type
				>::type sfinae = 0 )
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename vector<value_type, A2>::base_type&>(right))))
	{
	}

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(const vector<value_type, A2>& right,
			typename
				boost::enable_if
				<
					is_isomeric_same_options_vector<base_type, typename vector<value_type, A2>::base_type >,
					mplex::sfinae_type
				>::type sfinae = 0 )
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(BOOST_RV_REF_BEG
				boost::container::vector<value_type, A2, O2>
			BOOST_RV_REF_END right,
			const allocator_type& a,
			typename
				boost::enable_if
				<
					is_isomeric_same_options_vector<base_type, boost::container::vector<value_type, A2, O2> >,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::vector<value_type, A2>&>(right))), a)
	{
	}

	template<typename A2, typename O2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(const boost::container::vector<value_type, A2, O2>& right,
			const allocator_type& a,
			typename
				boost::enable_if
				<
					is_isomeric_same_options_vector<base_type, boost::container::vector<value_type, A2, O2> >,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(BOOST_RV_REF_BEG
				vector<value_type, A2>
			BOOST_RV_REF_END right,
			const allocator_type& a,
			typename
				boost::enable_if
				<
					is_isomeric_same_options_vector<base_type, typename vector<value_type, A2>::base_type >,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(static_cast< vector<value_type, A2>& >(right))), a)
	{
	}

	template<typename A2> YGGR_CONTAINER_CONSTRUCT_FORCEINLINE
	vector(const vector<value_type, A2>& right, const allocator_type& a,
			typename
				boost::enable_if
				<
					is_isomeric_same_options_vector<base_type, typename vector<value_type, A2>::base_type >,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

	~vector(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
	}

public:
    YGGR_CXX14_CONSTEXPR_OR_INLINE
	this_type& operator=(const base_type& right)
	{
		base_type::operator=(right);
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	this_type& operator=(const this_type& right)
	{
		base_type::operator=(static_cast<const base_type&>(right));
		return *this;
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	this_type& operator=(std::initializer_list<value_type> il)
	{
		base_type::assign(il.begin(), il.end());
		return *this;
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	this_type& operator=(BOOST_RV_REF(base_type) right)
		BOOST_NOEXCEPT_IF(
			allocator_traits_type::propagate_on_container_move_assignment::value
			|| allocator_traits_type::is_always_equal::value)
	{
		base_type::operator=(boost::move(right));
		return *this;
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE
	this_type& operator=(BOOST_RV_REF(this_type) right)
		BOOST_NOEXCEPT_IF(
			allocator_traits_type::propagate_on_container_move_assignment::value
			|| allocator_traits_type::is_always_equal::value)
	{
		base_type::operator=(boost::move(static_cast<base_type&>(right)));
		return *this;
	}

#if !defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::container::dtl::enable_if_and
		<
			this_type&,
			boost::container::dtl::is_version<A2, 0>,
			boost::container::dtl::is_different<A2, allocator_type>
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::vector<value_type, A2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::vector<value_type, A2> right_vt_type;

		base_type::operator=(boost::move(static_cast<right_vt_type&>(right)));
		return *this;
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::container::dtl::enable_if_and
		<
			this_type&,
			boost::container::dtl::is_version<A2, 0>,
			boost::container::dtl::is_different<A2, allocator_type>
		>::type
		operator=(const boost::container::vector<value_type, A2>& right)
	{
		typedef boost::container::vector<value_type, A2> right_vt_type;

		base_type::operator=(static_cast<const right_vt_type&>(right));
		return *this;
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::container::dtl::enable_if_and
		<
			this_type&,
			boost::container::dtl::is_version<A2, 0>,
			boost::container::dtl::is_different<A2, allocator_type>
		>::type
		operator=(BOOST_RV_REF_BEG
						vector<value_type, A2>
					BOOST_RV_REF_END right)
	{
		typedef vector<value_type, A2> right_vt_type;
		typedef typename right_vt_type::base_type right_base_vt_type;

		base_type::operator=(boost::move(static_cast<right_base_vt_type&>(right)));
		return *this;
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::container::dtl::enable_if_and
		<
			this_type&,
			boost::container::dtl::is_version<A2, 0>,
			boost::container::dtl::is_different<A2, allocator_type>
		>::type
		operator=(const vector<value_type, A2>& right)
	{
		typedef vector<value_type, A2> right_vt_type;
		typedef typename right_vt_type::base_type right_base_vt_type;

		base_type::operator=(static_cast<const right_base_vt_type&>(right));
		return *this;
	}

#endif // BOOST_CONTAINER_DOXYGEN_INVOKED

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_isomeric_same_options_vector<base_type, boost::container::vector<value_type, A2, O2> >,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::vector<value_type, A2, O2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::vector<value_type, A2, O2> right_vt_type;

		base_type::operator=(
			boost::move(reinterpret_cast<base_type&>(static_cast<right_vt_type&>(right))));
		return *this;
	}

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_isomeric_same_options_vector<base_type, boost::container::vector<value_type, A2, O2> >,
			this_type&
		>::type
		operator=(const boost::container::vector<value_type, A2, O2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_isomeric_same_options_vector<base_type, typename vector<value_type, A2>::base_type >,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						vector<value_type, A2>
					BOOST_RV_REF_END right)
	{
		typedef vector<value_type, A2> right_vt_type;
		typedef typename right_vt_type::base_type right_base_vt_type;

		base_type::operator=(
			boost::move(reinterpret_cast<base_type&>(static_cast<right_base_vt_type&>(right))));
		return *this;
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_isomeric_same_options_vector<base_type, typename vector<value_type, A2>::base_type >,
			this_type&
		>::type
		operator=(const vector<value_type, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

public:
	using base_type::swap;

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	YGGR_CXX14_CONSTEXPR_OR_INLINE void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		base_type::swap(right);
	}

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_isomeric_same_options_vector<base_type, boost::container::vector<value_type, A2, O2> >,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::vector<value_type, A2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::vector<value_type, A2> right_vt_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_vt_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename A2, typename O2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_isomeric_same_options_vector<base_type, boost::container::vector<value_type, A2, O2> >,
			void
		>::type
		swap(boost::container::vector<value_type, A2, O2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_isomeric_same_options_vector<base_type, typename vector<value_type, A2>::base_type >,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					vector<value_type, A2>
				BOOST_RV_REF_END right)
	{
		typedef vector<value_type, A2> right_vt_type;
		typedef typename right_vt_type::base_type right_base_vt_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_base_vt_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename A2> YGGR_CXX14_CONSTEXPR_OR_INLINE
	typename
		boost::enable_if
		<
			is_isomeric_same_options_vector<base_type, typename vector<value_type, A2>::base_type >,
			void
		>::type
		swap(vector<value_type, A2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

public:
	using base_type::assign;

	using base_type::get_allocator;

	using base_type::begin;
	using base_type::cbegin;

	using base_type::end;
	using base_type::cend;

	using base_type::rbegin;
	using base_type::crbegin;

	using base_type::rend;
	using base_type::crend;

	using base_type::empty;

	using base_type::size;
	using base_type::max_size;

	using base_type::resize;

	using base_type::capacity;
	using base_type::reserve;
	using base_type::shrink_to_fit;

	using base_type::front;
	using base_type::back;

	using base_type::operator[];
	using base_type::at;

	using base_type::data;

	using base_type::emplace;
	using base_type::emplace_back;

	using base_type::push_back;
	using base_type::pop_back;

	using base_type::insert;

	using base_type::erase;
	using base_type::clear;

};

// don't need defined YGGR_PP_CONTAINER_COMPARER_FULL, using boost::container::vector
//#if (BOOST_VERSION < 106700)
//	YGGR_PP_CONTAINER_COMPARER_FULL(2, vector, YGGR_CXX14_CONSTEXPR_OR_INLINE)
//#endif // BOOST_VERSION < 106700

YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL_MN(
	3, 2,
	::boost::container::vector,
	::yggr::container::detail::vector,
	::yggr::container::is_isomeric_same_options_vector,
	YGGR_CXX14_CONSTEXPR_OR_INLINE)

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, vector)

} // namespace detail

// shrink_to_fit
template<typename T, typename A> YGGR_CXX14_CONSTEXPR_OR_INLINE
void shrink_to_fit(detail::vector<T, A>& c)
{
	c.shrink_to_fit();
}

// data
template<typename T, typename A> YGGR_CXX14_CONSTEXPR_OR_INLINE
T* data(detail::vector<T, A>& c)
{
	return c.data();
}

template<typename T, typename A> YGGR_CXX14_CONSTEXPR_OR_INLINE
const T* data(const detail::vector<T, A>& c)
{
	return c.data();
}

//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

// emplace
template<typename T, typename A, typename ...Args> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename detail::vector<T, A>::iterator
	emplace(detail::vector<T, A>& c,
			typename detail::vector<T, A>::iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

// emplace_back
template<typename T, typename A, typename ...Args> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename detail::vector<T, A>::reference
	emplace_back(detail::vector<T, A>& c,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_back(boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > YGGR_CXX14_CONSTEXPR_OR_INLINE \
		typename detail::vector<T, A>::iterator \
			emplace(detail::vector<T, A>& c, \
						typename detail::vector<T, A>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > YGGR_CXX14_CONSTEXPR_OR_INLINE \
		typename detail::vector<T, A>::reference \
			emplace_back(detail::vector<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

// erase
template<typename T, typename A, typename Val> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename detail::vector<T, A>::iterator>,
				boost::is_same<Val, typename detail::vector<T, A>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename detail::vector<T, A>::reverse_iterator>,
				boost::is_same<Val, typename detail::vector<T, A>::const_reverse_iterator>
			>
		>,
		typename detail::vector<T, A>::size_type
	>::type
	erase(detail::vector<T, A>& c, const Val& value)
{
	typedef detail::vector<T, A> vec_type;
	typedef typename vec_type::iterator vec_iter_type;
	typedef typename vec_type::size_type vec_size_type;

	vec_iter_type it = std::remove(c.begin(), c.end(), value);
	vec_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename Pred> YGGR_CXX14_CONSTEXPR_OR_INLINE
typename detail::vector<T, A>::size_type
	erase_if(detail::vector<T, A>& c, Pred pred)
{
	typedef detail::vector<T, A> vec_type;
	typedef typename vec_type::iterator vec_iter_type;
	typedef typename vec_type::size_type vec_size_type;

	vec_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	vec_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{
namespace swap_support
{

	YGGR_PP_CONTAINER_SWAP(2, ::yggr::container::detail::vector, YGGR_CXX14_CONSTEXPR_OR_INLINE)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP_MN(
		3, 2,
		::boost::container::vector,
		::yggr::container::detail::vector,
		::yggr::container::is_isomeric_same_options_vector,
		YGGR_CXX14_CONSTEXPR_OR_INLINE)

} // namespace swap_support

using swap_support::swap;

} // namespace detail
} // namespace container
} // namespace yggr

namespace std
{
	using ::yggr::container::detail::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::container::detail::swap_support::swap;
} // namespace boost

#endif // !(BOOST_VERSION < 106700)

namespace yggr
{
namespace container
{

#if defined YGGR_USE_BOOST_CONTAINER
#	if (BOOST_VERSION < 106700)
		using boost::container::vector;
#	else
		using detail::vector;
#	endif // BOOST_VERSION < 106700
#elif defined YGGR_USE_STL_CONTAINER
	using std::vector;
#else
#	error "use vector type unknown error. please check source"
#endif //YGGR_USE_BOOST_CONTAINER


} // namespace container

using container::vector;

} // namespace yggr

#endif // __YGGR_CONTAINER_VECTOR_HPP__
