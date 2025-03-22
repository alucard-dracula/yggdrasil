//queue.hpp

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

#ifndef __YGGR_CONTAINER_QUEUE_HPP__
#define __YGGR_CONTAINER_QUEUE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/compatibility/stl_switch.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/container/deque.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/container/detail/swap_def.hpp>
#include <yggr/container/detail/range_size_def.hpp>
#include <yggr/container/detail/wrapper_emplace_helper.hpp>

#include <boost/ref.hpp>

#include <queue>

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
		detail::has_any_emplace< std::queue<T, C> >,
		void
	>::type
	emplace(std::queue<T, C>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename C, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< std::queue<T, C> >,
		void
	>::type
	emplace(std::queue<T, C>& c, BOOST_FWD_REF(Args)... args)
{
	c.push(T(boost::forward<Args>(args)...));
}

template<typename T, typename C, typename Cmp, typename ...Args> inline
typename
	boost::enable_if
	<
		detail::has_any_emplace< std::priority_queue<T, C, Cmp> >,
		void
	>::type
	emplace(std::priority_queue<T, C, Cmp>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace(boost::forward<Args>(args)...);
}

template<typename T, typename C, typename Cmp, typename ...Args> inline
typename
	boost::disable_if
	<
		detail::has_any_emplace< std::priority_queue<T, C, Cmp> >,
		void
	>::type
	emplace(std::priority_queue<T, C, Cmp>& c, BOOST_FWD_REF(Args)... args)
{
	c.push(T(boost::forward<Args>(args)...));
}


#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< std::queue<T, C> >, void>::type \
			emplace(std::queue<T, C>& c \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< std::queue<T, C> >, void>::type \
			emplace(std::queue<T, C>& c \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.push(T(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); } \
		\
		template<typename T, typename C, typename Cmp \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::enable_if<detail::has_any_emplace< std::priority_queue<T, C, Cmp> >, void>::type \
			emplace(std::priority_queue<T, C, Cmp>& c \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			c.emplace(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template<typename T, typename C, typename Cmp \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		typename boost::disable_if<detail::has_any_emplace< std::priority_queue<T, C, Cmp> >, void>::type \
			emplace(std::priority_queue<T, C, Cmp>& c \
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

template<	typename Val,
			typename Container = container::deque<Val >
		>
class queue
{
public:
	typedef Container container_type;
	typedef typename container_type::value_type value_type;
	typedef typename container_type::size_type size_type;
	
	typedef typename container_type::reference reference;
	typedef typename container_type::const_reference const_reference;
	typedef typename container_type::allocator_type allocator_type;
	
private:
	typedef queue this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
#if defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940)
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	BOOST_MPL_ASSERT_NOT((::yggr::is_same_tpl<std::deque<Val>, container_type>));
#endif // #if defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940)

public:
	queue(void)
		: c()
	{
	}

	template<typename Alloc>
	explicit queue(const Alloc& alloc)
		: c(alloc)
	{
	}

	explicit queue(BOOST_RV_REF(container_type) cont)
		: c(boost::move(cont))
	{
	}

	explicit queue(const container_type& cont)
		: c(cont)
	{
	}

	template<typename Alloc>
	queue(BOOST_RV_REF(container_type) cont, const Alloc& alloc)
		: c(boost::move(cont), alloc)
	{
	}

	template<typename Alloc>
	queue(const container_type& cont, const Alloc& alloc)
		: c(cont, alloc)
	{
	}

	template<typename Iter>
	queue(Iter s, Iter e)
		: c(s, e)
	{
	}

	template<typename Iter, typename Alloc>
	queue(Iter s, Iter e, const Alloc& alloc)
		: c(s, e, alloc)
	{
	}

	queue(BOOST_RV_REF(this_type) right)
		: c(boost::move(right.c))
	{
	}

	queue(const this_type& right)
		: c(right.c)
	{
	}

	template<typename Alloc>
	queue(BOOST_RV_REF(this_type) right, const Alloc& alloc)
		: c(alloc)
	{
		c = boost::move(right.c);
	}

	template<typename Alloc>
	queue(const this_type& right, const Alloc& alloc)
		: c(alloc)
	{
		c = right.c;
	}

	~queue(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(container_type) right)
	{
		container_type& right_ref = right;
		if(boost::addressof(c) == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(c, boost::move(right_ref));
		return *this;
	}

	this_type& operator=(const container_type& right)
	{
		if(boost::addressof(c) == boost::addressof(right))
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
	inline operator std::queue<value_type, container_type>(void) const
	{
		typedef std::queue<value_type, container_type> ret_type;
		return ret_type(c);
	}

	template<typename OVal, typename OContainer> inline
	operator std::queue<OVal, OContainer>(void) const
	{
		typedef std::queue<OVal, OContainer> ret_type;
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

	inline reference front(void)
	{
		return c.front();
	}

	inline const_reference front(void) const
	{
		return c.front();
	}

	inline reference back(void)
	{
		return c.back();
	}

	inline const_reference back(void) const
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
		c.pop_front();
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
			h(c \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
					YGGR_TMP_PP_EMPLACE_DEF( __n__ )

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
	container_type c;
};

// queue TEMPLATE FUNCTIONS
template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator==(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	
	// test for queue equality
	return l.container() == r.container();
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator!=(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	
	// test for queue inequality
	return (!(l == r));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator<(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	
	// test if l < r for queues
	return l.container() < r.container();
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator>(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	
	// test if l > r for queues
	return (r < l);
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator<=(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
					const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	
	// test if l <= r for queues
	return (!(r < l));
}

template< YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename T) > inline
bool operator>=(const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& l,
				const YGGR_PP_TEMPLATE_TYPE(queue, YGGR_PP_TEMPLATE_PARAMS_TYPES(2, T) )& r)
{	
	// test if l >= r for queues
	return (!(l < r));
}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename C, typename ...Args> inline
void emplace(queue<T, C>& c, BOOST_FWD_REF(Args)... args)
{
	c.emplace(boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		void emplace(queue<T, C>& c \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
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

using container::queue;

} // namespace yggr

namespace yggr
{
namespace container
{
namespace swap_support
{
	YGGR_PP_CONTAINER_SWAP(2, ::yggr::container::queue, inline)
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
	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, std::queue)
} // namespace std

YGGR_PP_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(2, std::queue)

namespace yggr
{
namespace container
{
	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(2, queue)
} // namespace container
} // namespace yggr

YGGR_PP_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(2, yggr::container::queue)

//----------------------priority_queue----------------------------------

namespace yggr
{
namespace container
{
template<	typename Val,
			typename Container = container::vector<Val>,
			typename Cmp = std::less<typename Container::value_type>
		>
class priority_queue
{
public:
	typedef typename mplex::typename_expand_get<Container, 1>::type tpl_arg_allocator_type;

public:
	typedef Container container_type;
	typedef typename container_type::value_type value_type;
	typedef typename container_type::size_type size_type;

	typedef typename container_type::reference reference;
	typedef typename container_type::const_reference const_reference;
	typedef typename container_type::allocator_type allocator_type;

	typedef Cmp value_compare;
	
private:
	typedef priority_queue this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	priority_queue(void)
		: c(), comp()
	{
	}

	explicit priority_queue(const value_compare& cmp)
		: c(), comp(cmp)
	{
	}

	template<typename Alloc> 
	explicit priority_queue(const Alloc& alloc)
		: c(alloc), comp()
	{
	}

	template<typename Alloc>
	priority_queue(const value_compare& cmp, const Alloc& alloc)
		: c(alloc), comp(cmp)
	{
	}

	explicit priority_queue(BOOST_RV_REF(container_type) cont)
		: c(boost::move(cont)), comp()
	{
	}

	explicit priority_queue(const container_type& cont)
		: c(cont), comp()
	{
	}

	priority_queue(const value_compare& cmp, BOOST_RV_REF(container_type) cont)
		: c(boost::move(cont)), comp(cmp)
	{
	}

	priority_queue(const value_compare& cmp, const container_type& cont)
		: c(cont), comp(cmp)
	{
	}

	template<typename Alloc>
	priority_queue(const value_compare& cmp, BOOST_RV_REF(container_type) cont,
					const Alloc& alloc)
		: c(alloc), comp(cmp)
	{
		c = boost::move(cont);
	}

	template<typename Alloc>
	priority_queue(const value_compare& cmp, const container_type& cont,
					const Alloc& alloc)
		: c(alloc), comp(cmp)
	{
		c = cont;
	}

	template<typename InIter>
	priority_queue(InIter start, InIter last)
		: c(start, last), comp()
	{
		std::make_heap(c.begin(), c.end(), comp);
	}

	template<typename InIter, typename Alloc>
	priority_queue(InIter start, InIter last, const Alloc& alloc)
		: c(start, last, alloc), comp()
	{
		std::make_heap(c.begin(), c.end());
	}

	template<typename InIter>
	priority_queue(InIter start, InIter last, const value_compare& cmp)
		: c(start, last), comp(cmp)
	{
		std::make_heap(c.begin(), c.end());
	}

	template<typename InIter, typename Alloc>
	priority_queue(InIter start, InIter last, const value_compare& cmp, const Alloc& alloc)
		: c(start, last, alloc), comp(cmp)
	{
		std::make_heap(c.begin(), c.end());
	}

	template<typename InIter>
	priority_queue(InIter start, InIter last, const value_compare& cmp, BOOST_RV_REF(container_type) cont)
		: c(boost::move(cont)), comp(cmp)
	{
		c.insert(c.end(), start, last);
		std::make_heap(c.begin(), c.end(), cmp);
	}

	template<typename InIter>
	priority_queue(InIter start, InIter last, const value_compare& cmp, const container_type& cont)
		: c(cont), comp(cmp)
	{
		c.insert(c.end(), start, last);
		std::make_heap(c.begin(), c.end());
	}

	template<typename InIter, typename Alloc>
	priority_queue(InIter start, InIter last,
					const value_compare& cmp, BOOST_RV_REF(container_type) cont,
					const Alloc& alloc)
		: c(alloc), comp(cmp)
	{
		c = boost::move(cont);
		c.insert(c.end(), start, last);
		std::make_heap(c.begin(), c.end(), cmp);
	}

	template<typename InIter, typename Alloc>
	priority_queue(InIter start, InIter last,
					const value_compare& cmp, const container_type& cont,
					const Alloc& alloc)
		: c(alloc), comp(cmp)
	{
		c = cont;
		c.insert(c.end(), start, last);
		std::make_heap(c.begin(), c.end());
	}

	priority_queue(BOOST_RV_REF(this_type) right)
		: c(boost::move(right.c)), comp(right.comp)
	{
	}

	priority_queue(const this_type& right)
		: c(right.c), comp(right.comp)
	{
	}

	template<typename Alloc>
	priority_queue(BOOST_RV_REF(this_type) right, const Alloc& alloc)
		: c(alloc), comp(right.comp)
	{
		c = boost::move(right.c);
	}

	template<typename Alloc>
	priority_queue(const this_type& right, const Alloc& alloc)
		: c(alloc), comp(right.comp)
	{
		c = right.c;
	}

	~priority_queue(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(container_type) right)
	{
		container_type& right_ref = right;
		if(boost::addressof(c) == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(c, boost::move(right_ref));
		std::make_heap(c.begin(), c.end(), comp);
		return *this;
	}

	this_type& operator=(const container_type& right)
	{
		if(boost::addressof(c) == boost::addressof(right))
		{
			return *this;
		}

		c = right;
		std::make_heap(c.begin(), c.end(), comp);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		copy_or_move_or_swap(c, right.c);
		copy_or_move_or_swap(comp, right.comp);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		c = right.c;
		comp = right.comp;
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

		::yggr::swap(this_type::container(), right.container());
		::yggr::swap(this_type::value_comp(), right.value_comp());
	}

public:
	inline operator std::priority_queue<value_type, container_type, value_compare>(void) const
	{
		typedef std::priority_queue<value_type, container_type, value_compare> ret_type;
		return ret_type(c, comp);
	}

	template<typename OVal, typename OContainer, typename OCmp> inline
	operator std::priority_queue<OVal, OContainer, OCmp>(void) const
	{
		typedef std::priority_queue<OVal, OContainer, OCmp> ret_type;
		return ret_type(c.begin(), c.end());
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
		return c.front();
	}

	inline const_reference top(void) const
	{
		return c.front();
	}

	inline void push(BOOST_RV_REF(value_type) val)
	{
		c.push_back(boost::move(val));
		std::push_heap(c.begin(), c.end(), comp);
	}

	inline void push(const value_type& val)
	{
		c.push_back(val);
		std::push_heap(c.begin(), c.end(), comp);
	}

	inline void pop(void)
	{
		std::pop_heap(c.begin(), c.end(), comp);
		c.pop_back();
	}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename ...Args> inline
	void emplace(BOOST_FWD_REF(Args)... args)
	{
		typedef detail::wrapper_emplace_helper<container_type> h_type;
		h_type h;
		h(c, boost::forward<Args>(args)...);
		std::push_heap(c.begin(), c.end());
	}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		void emplace( YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			typedef detail::wrapper_emplace_helper<container_type> h_type; \
			h_type h; \
			h(c \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); \
			std::push_heap(c.begin(), c.end()); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
					YGGR_TMP_PP_EMPLACE_DEF( __n__ )

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

	inline value_compare& value_comp(void)
	{
		return comp;
	}

	inline const value_compare& value_comp(void) const
	{
		return comp;
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
	container_type c;
	value_compare comp;
};


#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

template<typename T, typename C, typename Cmp, typename ...Args> inline
void emplace(priority_queue<T, C, Cmp>& c,
					BOOST_FWD_REF(Args)... args)
{
	c.emplace(boost::forward<Args>(args)...);
}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template<typename T, typename C, typename Cmp \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		void emplace(priority_queue<T, C, Cmp>& c \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
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

} // container

// priority_queue not support compare see stl document!!!

using container::priority_queue;

} // namespace yggr

namespace yggr
{
namespace container
{
namespace swap_support
{
	YGGR_PP_CONTAINER_SWAP(3, ::yggr::container::priority_queue, inline)
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
	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, std::priority_queue)
} // namespace std

YGGR_PP_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(3, std::priority_queue)

namespace yggr
{
namespace container
{
	YGGR_PP_CONTAINER_RANGE_SIZE_SUPPORT(3, yggr::container::priority_queue)
} // namespace container
} // namespace yggr

YGGR_PP_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(3, yggr::container::priority_queue)

#endif //__YGGR_CONTAINER_QUEUE_HPP__
