//list.hpp

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

#ifndef __YGGR_CONTAINER_LIST_HPP__
#define __YGGR_CONTAINER_LIST_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/container/container_selector.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/tag_sfinae.hpp>

#include <yggr/container/basic_function_adapter.hpp>
#include <yggr/container/detail/is_xxx_container.hpp>

#include <yggr/container/detail/call_erase_helper.hpp>
#include <yggr/container/detail/call_insert_helper.hpp>

#include <yggr/container/detail/swap_def.hpp>
#include <yggr/container/detail/comparer_def.hpp>
#include <yggr/container/detail/range_size_def.hpp>

#include <yggr/container/detail/has_emplace.hpp>
#include <yggr/container/is_isomeric_same_list.hpp>

#include <boost/container/list.hpp>
#include <boost/utility/enable_if.hpp>

#include <list>

YGGR_PP_IS_XXX_CONTAINER_DEF(2, std_list, std::list);

YGGR_PP_IS_XXX_CONTAINER_DEF(2, boost_list, boost::container::list);

// compatibility caller
namespace yggr
{
namespace container
{

//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< std::list<T, A> >,
		typename std::list<T, A>::iterator
	>::type
	emplace(std::list<T, A>& c,
			typename std::list<T, A>::const_iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< std::list<T, A> >,
		typename std::list<T, A>::iterator
	>::type
	emplace(std::list<T, A>& c,
			typename std::list<T, A>::iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return insert(c, pos, T(boost::forward<Args>(args)...));
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_front< std::list<T, A> >,
		typename std::list<T, A>::reference
	>::type
	emplace_front(std::list<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_front(boost::forward<Args>(args)...);
	return c.front();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_front< std::list<T, A> >,
		typename std::list<T, A>::reference
	>::type
	emplace_front(std::list<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_front(T(boost::forward<Args>(args)...));
	return c.front();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_back< std::list<T, A> >,
		typename std::list<T, A>::reference
	>::type
	emplace_back(std::list<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_back< std::list<T, A> >,
		typename std::list<T, A>::reference
	>::type
	emplace_back(std::list<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_back(T(boost::forward<Args>(args)...));
	return c.back();
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< detail::has_any_emplace< std::list<T, A> >, \
									typename std::list<T, A>::iterator >::type \
			emplace(std::list<T, A>& c, \
						typename std::list<T, A>::const_iterator pos \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< detail::has_any_emplace< std::list<T, A> >, \
									typename std::list<T, A>::iterator >::type \
			emplace(std::list<T, A>& c, \
						typename std::list<T, A>::iterator pos \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::list<T, A>::reference \
			emplace_front(std::list<T, A>& c \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_front(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.front(); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename std::list<T, A>::reference \
			emplace_back(std::list<T, A>& c \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
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

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< boost::container::list<T, A> >,
		typename boost::container::list<T, A>::iterator
	>::type
	emplace(boost::container::list<T, A>& c,
			typename boost::container::list<T, A>::const_iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< boost::container::list<T, A> >,
		typename boost::container::list<T, A>::iterator
	>::type
	emplace(boost::container::list<T, A>& c,
			typename boost::container::list<T, A>::iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return insert(c, pos, T(boost::forward<Args>(args)...));
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_front< boost::container::list<T, A> >,
		typename boost::container::list<T, A>::reference
	>::type
	emplace_front(boost::container::list<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_front(boost::forward<Args>(args)...);
	return c.front();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_front< boost::container::list<T, A> >,
		typename boost::container::list<T, A>::reference
	>::type
	emplace_front(boost::container::list<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_front(T(boost::forward<Args>(args)...));
	return c.front();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace_back< boost::container::list<T, A> >,
		typename boost::container::list<T, A>::reference
	>::type
	emplace_back(boost::container::list<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace_back(boost::forward<Args>(args)...);
	return c.back();
}

template<typename T, typename A, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace_back< boost::container::list<T, A> >,
		typename boost::container::list<T, A>::reference
	>::type
	emplace_back(boost::container::list<T, A>& c, BOOST_FWD_REF(Args)... args)
{
	c.push_back(T(boost::forward<Args>(args)...));
	return c.back();
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< detail::has_any_emplace< boost::container::list<T, A> >, \
									typename boost::container::list<T, A>::iterator >::type \
			emplace(boost::container::list<T, A>& c, \
						typename boost::container::list<T, A>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< detail::has_any_emplace< boost::container::list<T, A> >, \
									typename boost::container::list<T, A>::iterator >::type \
			emplace(boost::container::list<T, A>& c, \
						typename boost::container::list<T, A>::iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return insert(c, pos, T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_front< boost::container::list<T, A> >, \
			typename boost::container::list<T, A>::reference>::type \
			emplace_front(boost::container::list<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_front(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.front(); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_front< boost::container::list<T, A> >, \
			typename boost::container::list<T, A>::reference >::type \
			emplace_front(boost::container::list<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push_front(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return c.front(); } \
		\
		template<typename T, typename A	\
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if< \
			detail::has_any_emplace_back< boost::container::list<T, A> >, \
			typename boost::container::list<T, A>::reference >::type \
			emplace_back(boost::container::list<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace_back(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			return c.back(); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if< \
			detail::has_any_emplace_back< boost::container::list<T, A> >, \
			typename boost::container::list<T, A>::reference >::type \
			emplace_back(boost::container::list<T, A>& c \
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

#endif // defined(BOOST_CONTAINER_PERFECT_FORWARDING) || defined(BOOST_CONTAINER_DOXYGEN_INVOKED)

// erase
template<typename T, typename A, typename Val> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename std::list<T, A>::iterator>,
				boost::is_same<Val, typename std::list<T, A>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename std::list<T, A>::reverse_iterator>,
				boost::is_same<Val, typename std::list<T, A>::const_reverse_iterator>
			>
		>,
		typename boost::container::list<T, A>::size_type
	>::type
	erase(std::list<T, A>& c, const Val& value)
{
	typedef std::list<T, A> list_type;
	typedef typename list_type::size_type list_size_type;

	list_size_type olds = c.size();
	c.remove(value);
	return olds - c.size();
}

// erase_if
template<typename T, typename A, typename Pred> inline
typename std::list<T, A>::size_type
	erase_if(std::list<T, A>& c, Pred pred)
{
	typedef std::list<T, A> list_type;
	typedef typename list_type::size_type list_size_type;

	list_size_type olds = c.size();
	c.remove_if(pred);
	return olds - c.size();
}

// erase
template<typename T, typename A, typename Val> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename boost::container::list<T, A>::iterator>,
				boost::is_same<Val, typename boost::container::list<T, A>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename boost::container::list<T, A>::reverse_iterator>,
				boost::is_same<Val, typename boost::container::list<T, A>::const_reverse_iterator>
			>
		>,
		typename boost::container::list<T, A>::size_type
	>::type
	erase(boost::container::list<T, A>& c, const Val& value)
{
	typedef boost::container::list<T, A> list_type;
	typedef typename list_type::size_type list_size_type;

	list_size_type olds = c.size();
	c.remove(value);
	return olds - c.size();
}

// erase_if
template<typename T, typename A, typename Pred> inline
typename boost::container::list<T, A>::size_type
	erase_if(boost::container::list<T, A>& c, Pred pred)
{
	typedef boost::container::list<T, A> list_type;
	typedef typename list_type::size_type list_size_type;

	list_size_type olds = c.size();
	c.remove_if(pred);
	return olds - c.size();
}

// remove
template<typename T, typename A> inline
typename boost::container::list<T, A>::size_type
	remove(boost::container::list<T, A>& c, const T& val)
{
	typedef boost::container::list<T, A> list_type;
	typedef typename list_type::size_type list_size_type;

	list_size_type olds = c.size();
	c.remove(val);
	return olds - c.size();
}

// remove_if
template<typename T, typename A, typename Pred> inline
typename boost::container::list<T, A>::size_type
	remove_if(boost::container::list<T, A>& c, Pred pred)
{
	typedef boost::container::list<T, A> list_type;
	typedef typename list_type::size_type list_size_type;

	list_size_type olds = c.size();
	c.remove_if(pred);
	return olds - c.size();
}

// unique
template<typename T, typename A> inline
typename boost::container::list<T, A>::size_type
	unique(boost::container::list<T, A>& c)
{
	typedef boost::container::list<T, A> list_type;
	typedef typename list_type::size_type list_size_type;

	list_size_type olds = c.size();
	c.unique();
	return olds - c.size();
}

template<typename T, typename A, typename Pred> inline
typename boost::container::list<T, A>::size_type
	unique(boost::container::list<T, A>& c, Pred pred)
{
	typedef boost::container::list<T, A> list_type;
	typedef typename list_type::size_type list_size_type;

	list_size_type olds = c.size();
	c.unique(pred);
	return olds - c.size();
}

} // namespace container
} // namespace yggr


#if !(BOOST_VERSION < 107000)

namespace boost
{
namespace container
{

	YGGR_PP_CONTAINER_BASE_ISOMERIC_SAME_COMPARER_FULL(
		2, ::boost::container::list, ::yggr::container::is_isomeric_same_list, inline)

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
		2, ::boost::container::list, ::yggr::container::is_isomeric_same_list, inline)

} // namespace swap_support
} // namespace detail

using detail::swap_support::swap;

} // namespace container
} // namespace boost

namespace std
{
	using ::boost::container::detail::swap_support::swap;
} // namespace std

namespace boost
{
	using container::detail::swap_support::swap;
} // namespace boost

#endif // !(BOOST_VERSION < 107000)

// boost::size support
namespace std
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, std::list)

} // namespace std

namespace boost
{
namespace container
{

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, boost::container::list)

} // namespace container
} // namespace boost


#if !(BOOST_VERSION < 107000)

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T, typename A>
class list;

} // namespace detail
} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{
namespace detail
{

template<typename T,
			typename A =
				typename mplex::typename_expand_get<boost::container::list<T>, 1>::type >
class list
	: public boost::container::list<T, A>
{

public:
	typedef A tpl_arg_allocator_type;

public:
	typedef boost::container::list<T, A> base_type;

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
	friend class list;

private:
	typedef list this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	list(void) YGGR_NOEXCEPT_IF(boost::container::dtl::is_nothrow_default_constructible<allocator_type>::value)
		: base_type()
	{
	}

	explicit list(const allocator_type& a) YGGR_NOEXCEPT_OR_NOTHROW
		: base_type(a)
	{
	}

	explicit list(size_type n)
		: base_type(n)
	{
	}

	explicit list(size_type n, const allocator_type& a)
		: base_type(n, value_type(), a)
	{
	}

	list(size_type n, const value_type& v)
		: base_type(n, v)
	{
	}

	list(size_type n, const value_type& v, const allocator_type& a)
		: base_type(n, v, a)
	{
	}

	template<typename InIt>
	list(InIt first, InIt last)
		: base_type(first, last)
	{
	}

	template <typename InIt>
	list(InIt first, InIt last, const allocator_type& a )
		: base_type(first, last, a)
	{
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
	list(std::initializer_list<value_type> il, const allocator_type& a = allocator_type())
		: base_type(il, a)
	{
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	list(BOOST_RV_REF(base_type) right) YGGR_NOEXCEPT_OR_NOTHROW
		: base_type(boost::move(right))
	{
	}

	list(const base_type& right)
		: base_type(right)
	{
	}

	list(BOOST_RV_REF(this_type) right) YGGR_NOEXCEPT_OR_NOTHROW
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	list(const this_type& right)
		: base_type(static_cast<const base_type&>(right))
	{
	}

	template<typename A2>
	list(BOOST_RV_REF_BEG
				boost::container::list<value_type, A2>
			BOOST_RV_REF_END right,
			typename
				boost::enable_if
				<
					is_isomeric_same_list<base_type, boost::container::list<value_type, A2> >,
					mplex::sfinae_type
				>::type sfinae = 0 )
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::list<value_type, A2>&>(right))))
	{
	}

	template<typename A2>
	list(const boost::container::list<value_type, A2>& right,
			typename
				boost::enable_if
				<
					is_isomeric_same_list<base_type, boost::container::list<value_type, A2> >,
					mplex::sfinae_type
				>::type sfinae = 0 )
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	template<typename A2>
	list(BOOST_RV_REF_BEG
				list<value_type, A2>
			BOOST_RV_REF_END right,
			typename
				boost::enable_if
				<
					is_isomeric_same_list<base_type, typename list<value_type, A2>::base_type >,
					mplex::sfinae_type
				>::type sfinae = 0 )
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<typename list<value_type, A2>::base_type&>(right))))
	{
	}

	template<typename A2>
	list(const list<value_type, A2>& right,
			typename
				boost::enable_if
				<
					is_isomeric_same_list<base_type, typename list<value_type, A2>::base_type >,
					mplex::sfinae_type
				>::type sfinae = 0 )
		: base_type(reinterpret_cast<const base_type&>(right))
	{
	}

	list(BOOST_RV_REF(base_type) right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	list(const base_type& right, const allocator_type& a)
		: base_type(right, a)
	{
	}

	template<typename A2>
	list(BOOST_RV_REF_BEG
				boost::container::list<value_type, A2>
			BOOST_RV_REF_END right,
			const allocator_type& a,
			typename
				boost::enable_if
				<
					is_isomeric_same_list<base_type, boost::container::list<value_type, A2> >,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(
					static_cast<boost::container::list<value_type, A2>&>(right))), a)
	{
	}

	template<typename A2>
	list(const boost::container::list<value_type, A2>& right,
			const allocator_type& a,
			typename
				boost::enable_if
				<
					is_isomeric_same_list<base_type, boost::container::list<value_type, A2> >,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

	list(BOOST_RV_REF(this_type) right, const allocator_type& a)
		: base_type(boost::move(static_cast<base_type&>(right)), a)
	{
	}

	list(const this_type& right, const allocator_type& a)
		: base_type(static_cast<const base_type&>(right), a)
	{
	}

	template<typename A2>
	list(BOOST_RV_REF_BEG
				list<value_type, A2>
			BOOST_RV_REF_END right,
			const allocator_type& a,
			typename
				boost::enable_if
				<
					is_isomeric_same_list<base_type, typename list<value_type, A2>::base_type >,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(
			boost::move(
				reinterpret_cast<base_type&>(static_cast< list<value_type, A2>& >(right))), a)
	{
	}

	template<typename A2>
	list(const list<value_type, A2>& right, const allocator_type& a,
			typename
				boost::enable_if
				<
					is_isomeric_same_list<base_type, typename list<value_type, A2>::base_type >,
					mplex::sfinae_type
				>::type sfinae = 0)
		: base_type(reinterpret_cast<const base_type&>(right), a)
	{
	}

	~list(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
	}

public:
	//inline this_type& operator=(BOOST_COPY_ASSIGN_REF(base_type) right)
	//{
	//	base_type::operator=(right);
	//	return *this;
	//}

//#if defined(YGGR_NO_CXX11_RVALUE_REFERENCES) && !defined(__GNUC__)
//
//	inline this_type& operator=(BOOST_COPY_ASSIGN_REF(this_type) right)
//	{
//		base_type::operator=(static_cast<BOOST_COPY_ASSIGN_REF(base_type)>(right));
//		return *this;
//	}
//
//#endif // #if defined(YGGR_NO_CXX11_RVALUE_REFERENCES) && !defined(__GNUC__)

	inline this_type& operator=(const base_type& right)
	{
		base_type::operator=(right);
		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		base_type::operator=(static_cast<const base_type&>(right));
		return *this;
	}

#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)

	inline this_type& operator=(std::initializer_list<value_type> il)
	{
		base_type::assign(il.begin(), il.end());
		return *this;
	}
#endif // BOOST_NO_CXX11_HDR_INITIALIZER_LIST

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
		BOOST_NOEXCEPT_IF(
			allocator_traits_type::propagate_on_container_move_assignment::value
			|| allocator_traits_type::is_always_equal::value)
	{
		base_type::operator=(boost::move(right));
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
		BOOST_NOEXCEPT_IF(
			allocator_traits_type::propagate_on_container_move_assignment::value
			|| allocator_traits_type::is_always_equal::value)
	{
		base_type::operator=(boost::move(static_cast<base_type&>(right)));
		return *this;
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_list<base_type, boost::container::list<value_type, A2> >,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						boost::container::list<value_type, A2>
					BOOST_RV_REF_END right)
	{
		typedef boost::container::list<value_type, A2> right_vt_type;

		base_type::operator=(
			boost::move(reinterpret_cast<base_type&>(static_cast<right_vt_type&>(right))));
		return *this;
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_list<base_type, boost::container::list<value_type, A2> >,
			this_type&
		>::type
		operator=(const boost::container::list<value_type, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_list<base_type, typename list<value_type, A2>::base_type >,
			this_type&
		>::type
		operator=(BOOST_RV_REF_BEG
						list<value_type, A2>
					BOOST_RV_REF_END right)
	{
		typedef list<value_type, A2> right_vt_type;
		typedef typename right_vt_type::base_type right_base_vt_type;

		base_type::operator=(
			boost::move(reinterpret_cast<base_type&>(static_cast<right_base_vt_type&>(right))));
		return *this;
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_list<base_type, typename list<value_type, A2>::base_type >,
			this_type&
		>::type
		operator=(const list<value_type, A2>& right)
	{
		base_type::operator=(reinterpret_cast<const base_type&>(right));
		return *this;
	}

public:
	using base_type::swap;

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_list<base_type, boost::container::list<value_type, A2> >,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					boost::container::list<value_type, A2>
				BOOST_RV_REF_END right)
	{
		typedef boost::container::list<value_type, A2> right_vt_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_vt_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_list<base_type, boost::container::list<value_type, A2> >,
			void
		>::type
		swap(boost::container::list<value_type, A2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_list<base_type, typename list<value_type, A2>::base_type >,
			void
		>::type
		swap(BOOST_RV_REF_BEG
					list<value_type, A2>
				BOOST_RV_REF_END right)
	{
		typedef list<value_type, A2> right_vt_type;
		typedef typename right_vt_type::base_type right_base_vt_type;
		base_type& right_ref = reinterpret_cast<base_type&>(static_cast<right_base_vt_type&>(right));
		base_type::swap(right_ref);
	}

	template<typename A2> inline
	typename
		boost::enable_if
		<
			is_isomeric_same_list<base_type, typename list<value_type, A2>::base_type >,
			void
		>::type
		swap(list<value_type, A2>& right)
	{
		base_type& right_ref = reinterpret_cast<base_type&>(right);
		base_type::swap(right_ref);
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		base_type::swap(right);
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

	using base_type::front;
	using base_type::back;

	using base_type::emplace;
	using base_type::emplace_front;
	using base_type::emplace_back;

	using base_type::push_front;
	using base_type::pop_front;

	using base_type::push_back;
	using base_type::pop_back;

	using base_type::insert;

	using base_type::erase;
	using base_type::clear;

	using base_type::splice;

	//using base_type::remove;
	inline size_type remove(const value_type& val)
	{
		size_type old_size = base_type::size();
		base_type::remove(val);
		return old_size - base_type::size();
	}

	template<typename UnaryPredicate> inline
	size_type remove_if(UnaryPredicate pred)
	{
		size_type old_size = base_type::size();
		base_type::remove_if(pred);
		return old_size - base_type::size();
	}

	//using base_type::unique;
	inline size_type unique(void)
	{
		size_type old_size = base_type::size();
		base_type::unique();
		return old_size - base_type::size();
	}

	template<typename BinaryPredicate> inline
	size_type unique( BinaryPredicate pred)
	{
		size_type old_size = base_type::size();
		base_type::unique(pred);
		return old_size - base_type::size();
	}

	using base_type::merge;

	using base_type::sort;
	using base_type::reverse;

};

// don't using YGGR_PP_CONTAINER_COMPARER_FULL
//YGGR_PP_CONTAINER_COMPARER_FULL(2, list, inline)

YGGR_PP_CONTAINER_ISOMERIC_SAME_COMPARER_FULL(
	2,
	::boost::container::list,
	list,
	is_isomeric_same_list,
	inline)

YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, list)

} // namespace detail

//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

// emplace
template<typename T, typename A, typename ...Args> inline
typename detail::list<T, A>::iterator
	emplace(detail::list<T, A>& c,
			typename detail::list<T, A>::iterator pos,
			BOOST_FWD_REF(Args)... args)
{
	return c.emplace(pos, boost::forward<Args>(args)...);
}

// emplace_front
template<typename T, typename A, typename ...Args> inline
typename detail::list<T, A>::reference
	emplace_front(detail::list<T, A>& c,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_front(boost::forward<Args>(args)...);
}

// emplace_back
template<typename T, typename A, typename ...Args> inline
typename detail::list<T, A>::reference
	emplace_back(detail::list<T, A>& c,
					BOOST_FWD_REF(Args)... args)
{
	return c.emplace_back(boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::list<T, A>::iterator \
			emplace(detail::list<T, A>& c, \
						typename detail::list<T, A>::const_iterator pos \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace(pos YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::list<T, A>::reference \
			emplace_front(detail::list<T, A>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			return c.emplace_front(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename detail::list<T, A>::reference \
			emplace_back(detail::list<T, A>& c \
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
template<typename T, typename A, typename Val> inline
typename
	boost::disable_if
	<
		boost::mpl::or_
		<
			boost::mpl::or_
			<
				boost::is_same<Val, typename detail::list<T, A>::iterator>,
				boost::is_same<Val, typename detail::list<T, A>::const_iterator>
			>,
			boost::mpl::or_
			<
				boost::is_same<Val, typename detail::list<T, A>::reverse_iterator>,
				boost::is_same<Val, typename detail::list<T, A>::const_reverse_iterator>
			>
		>,
		typename detail::list<T, A>::size_type
	>::type
	erase(detail::list<T, A>& c, const Val& value)
{
	typedef detail::list<T, A> vec_type;
	typedef typename vec_type::iterator vec_iter_type;
	typedef typename vec_type::size_type vec_size_type;

	vec_iter_type it = std::remove(c.begin(), c.end(), value);
	vec_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// erase_if
template<typename T, typename A, typename Pred> inline
typename detail::list<T, A>::size_type
	erase_if(detail::list<T, A>& c, Pred pred)
{
	typedef detail::list<T, A> vec_type;
	typedef typename vec_type::iterator vec_iter_type;
	typedef typename vec_type::size_type vec_size_type;

	vec_iter_type it = std::remove_if(c.begin(), c.end(), pred);
	vec_size_type ret = std::distance(it, c.end());
	c.erase(it, c.end());
	return ret;
}

// remove
template<typename T, typename A> inline
typename detail::list<T, A>::size_type
	remove(detail::list<T, A>& c, const T& val)
{
	return c.remove(val);
}

// remove_if
template<typename T, typename A, typename Pred> inline
typename detail::list<T, A>::size_type
	remove_if(detail::list<T, A>& c, Pred pred)
{
	return c.remove_if(pred);
}

// unique
template<typename T, typename A> inline
typename detail::list<T, A>::size_type
	unique(detail::list<T, A>& c)
{
	return c.unique();
}

template<typename T, typename A, typename Pred> inline
typename detail::list<T, A>::size_type
	unique(detail::list<T, A>& c, Pred pred)
{
	return c.unique(pred);
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

	YGGR_PP_CONTAINER_SWAP(2, ::yggr::container::detail::list, inline)

	YGGR_PP_CONTAINER_ISOMERIC_SAME_SWAP(
		2,
		::boost::container::list,
		::yggr::container::detail::list,
		::yggr::container::is_isomeric_same_list,
		inline)

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

#endif // (BOOST_VERSION < 107000)

namespace yggr
{
namespace container
{

#if defined YGGR_USE_BOOST_CONTAINER
#	if (BOOST_VERSION < 107000)
		using boost::container::list;
#	else
		using detail::list;
#	endif // BOOST_VERSION < 107000
#elif defined YGGR_USE_STL_CONTAINER
	using std::list;
#else
#	error "use list type unknown error. please check source"
#endif //YGGR_USE_BOOST_CONTAINER


} // namespace container

using container::list;

} // namespace yggr

#endif // __YGGR_CONTAINER_LIST_HPP__
