// safe_stack.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_STACK_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_STACK_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/mutex_def_helper.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/func/foo_t_info.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/container/stack.hpp>

#include <yggr/safe_container/detail/swap_def.hpp>
#include <yggr/safe_container/detail/comparer_def.hpp>

#include <yggr/safe_container/detail/safe_element_reference.hpp>
#include <yggr/safe_container/detail/operator_set_impl.hpp>
#include <yggr/safe_container/detail/swap_impl.hpp>
#include <yggr/safe_container/detail/emplace_impl.hpp>
#include <yggr/safe_container/detail/push_pop_impl.hpp>
#include <yggr/safe_container/detail/safe_other_impl.hpp>
#include <yggr/safe_container/detail/using_handler_impl.hpp>
#include <yggr/safe_container/detail/range_size_def.hpp>
#include <yggr/safe_container/safe_container_error.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/thread/mutex.hpp>

#include <algorithm>

/* 
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! All safe containers can not guarantee the creation and destruction of multi-threaded safe,		!!!
!!! in different threads to create and destroy the safe container, please use swap to deal with,	!!!
!!! and																								!!! 
!!!  it is forbidden to pass any "iterator reference pointer" inside the safe_container				!!!
!!!	 through the callback function to the outside of the safe_container and save or use it!			!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

namespace yggr
{
namespace safe_container
{

template<	typename Val,
			typename Mutex = void,
			typename Container = container::deque<Val>,
			template <typename _Val, typename _Container> class Stack = container::stack
		>
class safe_stack
{
public:
	typedef typename Container::allocator_type tpl_arg_allocator_type;

public:
	typedef Stack<Val, Container> base_type;

private:
	typedef container::stack<Val, Container> chk_type;
	BOOST_MPL_ASSERT((boost::is_same<base_type, chk_type>));

public:
	typedef typename base_type::value_type value_type;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;

	typedef Container container_type;
	typedef typename container_type::allocator_type allocator_type;

	typedef safe_container_error::error_type error_type;

private:
	typedef safe_container_error::error_maker_type error_maker_type;

private:
	typedef thread::mutex_def_helper<Mutex> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

private:
	typedef safe_stack this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
#if defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940)
	// see test/compiler_bugs/vs2022/msvc_2022_17.10.1_deque_test_shrink_to_fit_bug.cpp
	BOOST_MPL_ASSERT_NOT((::yggr::is_same_tpl<std::deque<Val>, container_type>));
#endif // #if defined(_MSC_VER) && (1930 <= _MSC_VER) && (_MSC_VER <= 1940)

public:
	safe_stack(void)
	{
	}

	template<typename Alloc>
	explicit safe_stack(const Alloc& alloc)
		: _base(alloc)
	{
	}

	explicit safe_stack(const container_type& cont)
		: _base(cont)
	{
	}

	template<typename Alloc>
	safe_stack(const container_type& cont, const Alloc& alloc)
		: _base(cont, alloc)
	{
	}

	template<typename Iter>
	safe_stack(Iter s, Iter e)
		: _base(s, e)
	{
	}

	template<typename Iter, typename Alloc>
	safe_stack(Iter s, Iter e, const Alloc& alloc)
		: _base(s, e, alloc)
	{
	}

	safe_stack(BOOST_RV_REF(base_type) right)
		: _base(boost::move(right))
	{
	}

	template<typename Alloc>
	safe_stack(BOOST_RV_REF(base_type) right, const Alloc& alloc)
		: _base(boost::move(right), alloc)
	{
	}

	safe_stack(const base_type& right)
		: _base(right)
	{
	}

	template<typename Alloc>
	safe_stack(const base_type& right, const Alloc& alloc)
		: _base(right, alloc)
	{
	}

	safe_stack(BOOST_RV_REF(this_type) right)
		: _base(right.get_allocator())
	{
		this_type& right_ref = right;
		right_ref.swap(_base);
	}

	template<typename Alloc>
	safe_stack(BOOST_RV_REF(this_type) right, const Alloc& alloc)
		: _base(alloc)
	{
		this_type& right_ref = right;
		right_ref.swap(_base);
	}

	safe_stack(const this_type& right)
		: _base(right.get_allocator())
	{
		right.copy_to_base(_base);
	}

	template<typename Alloc>
	safe_stack(const this_type& right, const Alloc& alloc)
		: _base(alloc)
	{
		right.copy_to_base(_base);
	}

	~safe_stack(void)
	{
	}

public:
	//inline this_type& operator=(BOOST_RV_REF(base_type) right);
	//this_type& operator=(const base_type& right);

	//inline this_type& operator=(BOOST_RV_REF(this_type) right);
	//this_type& operator=(const this_type& right);

	YGGR_PP_SAFE_CONTAINER_OPERATOR_SET_IMPL()

public:
	//inline void swap(BOOST_RV_REF(base_type) right);
	//void swap(base_type& right);

	//inline void swap(BOOST_RV_REF(this_type) right);
	//void swap(this_type& right);

	YGGR_PP_SAFE_CONTAINER_SWAP_IMPL()

public:
	//operator base_type(void) const;

	YGGR_PP_SAFE_CONTAINER_OPERATOR_BASE_IMPL()

public:
	bool empty(void) const
	{
		read_lock_type lk(_mutex);
		return _base.empty();
	}

	size_type size(void) const
	{
		read_lock_type lk(_mutex);
		return _base.size();
	}

private:
	// top()
	inline static reference unlock_top(base_type& base)
	{
		if(base.empty())
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return base.top();
	}

	value_type unsafe_get_top(void) const
	{
		read_lock_type lk(_mutex);
		if(_base.empty())
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return _base.top();
	}

	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		unsafe_set_top(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		if(_base.empty())
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return handler(_base.top());
	}

	class bridge_top;

	friend class bridge_top;

	class bridge_top
		: public detail::basic_bridge<safe_stack>
	{
	private:
		typedef detail::basic_bridge<safe_stack> bridge_base_type;

	public:
		typedef typename bridge_base_type::container_type container_type;
		typedef typename bridge_base_type::base_container_type base_container_type;
		typedef typename bridge_base_type::value_type value_type;

	private:
		typedef bridge_top this_type;

	public:
		bridge_top(container_type& cont)
			: bridge_base_type(cont)
		{
		}

		bridge_top(const bridge_top& right)
			: bridge_base_type(right)
		{
		}

		~bridge_top(void)
		{
		}

	public:
		inline bool operator==(const bridge_top& right) const
		{
			return bridge_base_type::compare_eq(right);
		}

		template<typename OtherBridge> inline
		bool operator==(const OtherBridge&) const
		{
			return false;
		}

		template<typename OtherBridge> inline
		bool operator!=(const OtherBridge& right) const
		{
			return !this->operator==(right);
		}

	public:
		inline value_type operator()(void) const
		{
			const container_type& cont = bridge_base_type::_wrap_cont;
			return cont.unsafe_get_top();
		}

		template<typename Handler> inline
		typename func::foo_t_info<Handler>::result_type
			operator()(const Handler& handler) const
		{
			container_type& cont = bridge_base_type::_wrap_cont;
			return cont.unsafe_set_top(handler);
		}

	public:
		inline boost::function1<value_type&, base_container_type&>
			native_handler(void) const
		{
			return boost::bind(&container_type::unlock_top, _1);
		}

		template<typename Op_Handler, typename OtherBridge> inline
		boost::function1
		<
			typename func::foo_t_info<Op_Handler>::result_type, 
			base_container_type&
		>
			link(const Op_Handler& op_handler, const OtherBridge& other_bridge) const
		{
			return 
				bridge_base_type::pro_s_link_handler(
					op_handler, 
					this_type::native_handler(), 
					other_bridge.native_handler());
		}

		template<typename Op_Handler, typename Handler> inline
		boost::function1
		<
			typename func::foo_t_info<Op_Handler>::result_type, 
			base_container_type&
		>
			link_handler(const Op_Handler& op_handler, const Handler& handler) const
		{
			return bridge_base_type::pro_s_link_handler(op_handler, this_type::native_handler(), handler);
		}
	};

public:

	inline detail::safe_element_reference<bridge_top> top(void)
	{
		return detail::safe_element_reference<bridge_top>(bridge_top(*this));
	}

	inline value_type top(void) const
	{
		return this_type::unsafe_get_top();
	}

	bool get_top(value_type& val) const
	{
		read_lock_type lk(_mutex);
		return (!_base.empty()) && (val = _base.top(), true);
	}

	bool set_top(BOOST_RV_REF(value_type) val)
	{
		write_lock_type lk(_mutex);
		return 
			(!_base.empty()) 
			&& (copy_or_move_or_swap(_base.top(), boost::move(val)), true);
	}

	bool set_top(const value_type& val)
	{
		value_type tmp(val);

		write_lock_type lk(_mutex);
		return 
			(!_base.empty())
			&& (copy_or_move_or_swap(_base.top(), boost::move(tmp)), true);
	}

	// push
	//bool push(BOOST_RV_REF(value_type) val);
	//bool push(const value_type& val);

	//template<typename Handler> 
	//typename Handler::result_type
	//	push(BOOST_RV_REF(value_type) val, const Handler& handler);
	
	//template<typename Handler> 
	//typename Handler::result_type
	//	push(const value_type& val, const Handler& handler);

	YGGR_PP_SAFE_SEQ_WRAP_PUSH_IMPL()

	//pop
	//bool pop(void);
	//bool pop(value_type& val);
	
	//template<typename Handler> 
	//typename Handler::result_type
	//	pop(const Handler& handler);
	
	//template<typename Handler> 
	//typename Handler::result_type
	//	pop(value_type& val, const Handler& handler);

	YGGR_PP_SAFE_SEQ_WRAP_POP_IMPL(top)

	////emplace
	//template<typename ...Args>
	//void emplace(Args&&... args);

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	YGGR_PP_SAFE_SEQ_EMPLACE_VALUE_IMPL( _ )

#else

	// compatibility msvc100 deque emplace_back
	inline bool emplace(void)
	{
		write_lock_type lk(_mutex);
		try 
		{ 
			_base.push(value_type());
			return true;
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())); 
			return false; 
		} 
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_SAFE_SEQ_EMPLACE_VALUE_IMPL( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() arg

#	define BOOST_PP_LOCAL_LIMITS (1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES
	
	void clear(void)
	{
		base_type tmp(this_type::get_allocator());
		{
			write_lock_type lk(_mutex);
			_base.swap(tmp);
		}
	}

	// use handler:
	//template<typename Handler>
	//typename Handler::result_type using_handler(const Handler& handler);

	//template<typename Handler>
	//typename Handler::result_type using_handler(const Handler& handler) const;

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type 
	//	using_handler(const Handler& handler, const Return_Handler& ret_handler);

	//template<typename Handler, typename Return_Handler>
	//typename Handler::result_type 
	//	using_handler(const Handler& handler, const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_CONTAINER_USING_HANDLER_IMPL()

	// using_hook_handler
	//template<typename Handler> inline
	//typename Handler::result_type 
	//	using_hook_handler(const Handler& handler);
	//
	//template<typename Handler> inline
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler) const;
	//
	//template<typename Handler, typename Return_Handler> inline
	//typename Handler::result_type 
	//	using_hook_handler(const Handler& handler, 
	//						const Return_Handler& ret_handler);
	//
	//template<typename Handler, typename Return_Handler> inline 
	//typename Handler::result_type
	//	using_hook_handler(const Handler& handler,
	//						const Return_Handler& ret_handler) const;

	YGGR_PP_SAFE_CONTAINER_USING_HOOK_HANDLER_IMPL()

	//safe_other

	//base_type& copy_to_base(base_type& out) const;

	YGGR_PP_SAFE_CONTAINER_COPY_TO_BASE_IMPL()

	//base_type load(void) const;
	//base_type load(const allocator_type& alloc) const;

	YGGR_PP_SAFE_CONTAINER_LOAD_IMPL()

	//void store(BOOST_RV_REF(base_type) base);
	//void store(const base_type& base);

	YGGR_PP_SAFE_CONTAINER_STONE_IMPL()

	allocator_type get_allocator(void) const
	{
		read_lock_type lk(_mutex);
		return _base.get_allocator();
	}

	container_type container(void) const
	{
		read_lock_type lk(_mutex);
		return _base.container();
	}

private:
	mutable mutex_type _mutex;
	base_type _base;
};

YGGR_PP_SAFE_CONTAINER_COMPARER_FULL(3, safe_stack)

YGGR_PP_SAFE_CONTAINER_RANGE_SIZE_SUPPORT(3, safe_stack)

} // namespace safe_container
} // namespace yggr

YGGR_PP_SAFE_WRAP_CONTAINER_RANGE_SIZE_CHECK_PASS_SUPPORT(3, yggr::safe_container::safe_stack)

//--------------------------------------------------support swap-------------------------------------
namespace yggr
{
namespace safe_container
{
namespace swap_support
{
	YGGR_PP_SAFE_CONTAINER_SWAP(3, safe_stack)
} // namespace swap_support

using swap_support::swap;

} // namespace safe_container
} // namespace yggr

//--------------------------------------------------support std::swap-------------------------------------
namespace std
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace std

//-------------------------------------------------support boost::swap-----------------------------------
namespace boost
{
	using ::yggr::safe_container::swap_support::swap;
} // namespace boost

#endif //__YGGR_SAFE_CONTAINER_SAFE_STACK_HPP__
