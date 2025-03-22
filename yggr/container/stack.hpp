// stack.hpp

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

#ifndef __YGGR_CONTAINER_STACK_HPP__
#define __YGGR_CONTAINER_STACK_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/compatibility/stl_switch.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/container/deque.hpp>

#include <yggr/container/detail/swap_def.hpp>
#include <yggr/container/detail/range_size_def.hpp>
#include <yggr/container/detail/wrapper_emplace_helper.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <stack>

namespace yggr
{
namespace container
{

//emplace
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename C, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< std::stack<T, C> >,
		void
	>::type
	emplace(std::stack<T, C>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename C, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< std::stack<T, C> >,
		void
	>::type
	emplace(std::stack<T, C>& c, BOOST_FWD_REF(Args)... args)
{
	c.push(T(boost::forward<Args>(args)...));
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< std::stack<T, C> >, void>::type \
			emplace(std::stack<T, C>& c \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< std::stack<T, C> >, void>::type \
			emplace(std::stack<T, C>& c \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace container
} // namespace yggr

namespace yggr
{
namespace container
{

template< typename Val,
			typename Container = container::deque<Val> > 
class stack
{
public:
	typedef Container container_type;
	typedef typename container_type::value_type value_type;
	typedef typename container_type::size_type size_type;
	typedef typename container_type::reference reference;
	typedef typename container_type::const_reference const_reference;
	typedef typename container_type::allocator_type allocator_type;

private:
	typedef stack this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
#if defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940)
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	BOOST_MPL_ASSERT_NOT((::yggr::is_same_tpl<std::deque<Val>, container_type>));
#endif // #if defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940)


public:
	stack(void)
		: c()
	{
	}

	template<typename Alloc>
	explicit stack(const Alloc& alloc)
		: c(alloc)
	{
	}

	explicit stack(BOOST_RV_REF(container_type) cont)
		: c(boost::move(cont))
	{
	}

	explicit stack(const container_type& cont)
		: c(cont)
	{
	}

	template<typename Alloc>
	stack(BOOST_RV_REF(container_type) cont, const Alloc& alloc)
		: c(alloc)
	{
		c = boost::move(cont); // compatibility not cpp11
	}

	template<typename Alloc>
	stack(const container_type& cont, const Alloc& alloc)
		: c(alloc)
	{
		c = cont;
	}

	template<typename Iter>
	stack(Iter s, Iter e)
		: c(s, e)
	{
	}

	template<typename Iter, typename Alloc>
	stack(Iter s, Iter e, const Alloc& alloc)
		: c(s, e, alloc)
	{
	}

	stack(BOOST_RV_REF(this_type) right)
		: c(boost::move(right.c))
	{
	}

	template<typename Alloc>
	stack(BOOST_RV_REF(this_type) right, const Alloc& alloc)
		: c(alloc)
	{
		c = boost::move(right.c);
	}

	stack(const this_type& right)
		: c(right.c)
	{
	}

	template<typename Alloc>
	stack(const this_type& right, const Alloc& alloc)
		: c(alloc)
	{
		c = right.c;
	}

	~stack(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(container_type) right)
	{
		container_type& right_ref = right;
		if(boost::addressof(c) ==  boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(c, boost::move(right_ref));
		return *this;
	}

	this_type& operator=(const container_type& right)
	{
		if(boost::addressof(c) ==  boost::addressof(right))
		{
			return *this;
		}

		c = right;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(c, boost::move(right.c));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}

		c = right.c;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		::yggr::swap(c, right.c);
	}

public:
	inline operator std::stack<value_type, container_type>(void) const
	{
		typedef std::stack<value_type, container_type> ret_type;
		return ret_type(c);
	}

	template<typename OVal, typename OContainer> inline
	operator std::stack<OVal, OContainer>(void) const
	{
		typedef std::stack<OVal, OContainer> ret_type;
		return ret_type(OContainer(c.begin(), c.end()));
	}

	inline bool empty(void) const
	{
		return c.empty();
	}

	inline size_type size(void) const
	{
		return c.size();
	}

	inline reference top(void)
	{
		return c.back();
	}

	inline const_reference top(void) const
	{
		return c.back();
	}

	inline void push(BOOST_RV_REF(value_type) val)
	{
		c.push_back(boost::move(val));
	}

	inline void push(const value_type& val)
	{
		c.push_back(val);
	}

	inline void pop(void)
	{
		c.pop_back();
	}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename ...Args> inline
	void emplace(BOOST_FWD_REF(Args)... args)
	{
		typedef detail::wrapper_emplace_helper<container_type> h_type;
		h_type h;
		h(c, boost::forward<Args>(args)...);
	}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		void emplace( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef detail::wrapper_emplace_helper<container_type> h_type; \
			h_type h; \
			h(c YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline allocator_type get_allocator(void) const
	{
		return c.get_allocator();
	}

	inline container_type& container(void)
	{
		return c;
	}

	inline const container_type& container(void) const
	{
		return c;
	}

protected:
	container_type c; // reference from msvc and gcc name
};

// stack TEMPLATE FUNCTIONS
template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator==(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	// test for stack equality
	return l.container() == r.container();
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator!=(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	// test for stack inequality
	return (!(l == r));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator<(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	// test if l < r for stacks
	return l.container() < r.container();
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator>(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	// test if l > r for stacks
	return (r < l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator<=(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	// test if l <= r for stacks
	return (!(r < l));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator>=(const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(stack, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	// test if l >= r for stacks
	return (!(l < r));
}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename C, typename ...Args> inline
void emplace(stack<T, C>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace(boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		void emplace(stack<T, C>& c \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

} // namespace container

using container::stack;

} // namespace yggr

namespace yggr
{
namespace container
{
namespace swap_support
{
	YGGR_PP_CONTAINER_SWAP(2, ::yggr::container::stack, inline)
} // namespace swap_support

using swap_support::swap;

} // namespace container
} // namespace yggr

namespace std
{
	using ::yggr::container::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::container::swap_support::swap;
} // namespace boost

// boost::size support
namespace std
{
	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, std::stack)
} // namespace std

YGGR_PP_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(2, std::stack)

namespace yggr
{
namespace container
{
	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, yggr::container::stack)
} // namespace container
} // namespace yggr

YGGR_PP_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(2,  yggr::container::stack)

#endif // __YGGR_CONTAINER_STACK_HPP__
