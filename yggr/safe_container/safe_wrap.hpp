//safe_wrap.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_WARP_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_WARP_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/thread/boost_thread.hpp>
#include <yggr/thread/mutex_def_helper.hpp>

#include <yggr/support/operator_t.hpp>
#include <yggr/move/move.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/mplex/null_t.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/safe_container/safe_container_error.hpp>

#include <boost/ref.hpp>
#include <boost/thread/mutex.hpp>

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

template<	typename Value,
			typename Mutex = void,
			typename SharedPtr = smart_ptr_ex::shared_ptr<Value>
		>
class safe_wrap
{
public:
	typedef Value value_type;
	typedef SharedPtr value_ptr_type;
	typedef typename smart_ptr_ex::shared_ptr_element_t<value_ptr_type>::type element_type;
	typedef value_ptr_type element_ptr_type;
	typedef value_ptr_type base_type;

	typedef safe_container_error::error_type error_type;

private:
	typedef safe_container_error::error_maker_type error_maker_type;

private:
	typedef thread::mutex_def_helper<Mutex> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::mutex_type mutex_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

private:
	typedef safe_wrap this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// opeator_set_helper don't outside
	template<typename DD, typename AA = mplex::null_type>
	struct opeator_set_helper
	{
	public:
		typedef DD deleter_type;
		typedef AA allocator_type;
		typedef safe_wrap outside_type;
		typedef boost::reference_wrapper<outside_type const> reference_type;

	private:
		friend outside_type;

	private:
		typedef opeator_set_helper this_type;

	public:
		opeator_set_helper(const outside_type& ncref, deleter_type nd, allocator_type na)
			: _cref(boost::cref(ncref)), _d(nd), _a(na)
		{
		}

		opeator_set_helper(const this_type& right)
			: _cref(right._cref), _d(right._d), _a(right._a)
		{
		}

		~opeator_set_helper(void)
		{
		}

	private:
		reference_type _cref;
		deleter_type _d;
		allocator_type _a;
	};


public:
	safe_wrap(void)
		: _ptr()
	{
	}

	safe_wrap(value_type* pval)
		: _ptr(construct_shared<value_type>(pval))
	{
	}

	template<typename D>
	safe_wrap(value_type* pval, D d)
		: _ptr(construct_shared<value_type>(pval, d))
	{
	}

	template<typename D, typename A>
	safe_wrap(value_type* pval, D d, A a)
		: _ptr(pval, d, a)
	{
	}

	safe_wrap(BOOST_RV_REF(value_type) val)
		: _ptr(construct_shared<value_type>(yggr_nothrow_new value_type(boost::move(val))))
	{
	}

	template<typename D>
	safe_wrap(BOOST_RV_REF(value_type) val, D d)
		: _ptr(construct_shared<value_type>(yggr_nothrow_new value_type(boost::move(val)), d))
	{
	}

	template<typename D, typename A>
	safe_wrap(BOOST_RV_REF(value_type) val, D d, A a)
		: _ptr(construct_shared<value_type>(yggr_nothrow_new value_type(boost::move(val)), d, a))
	{
	}

	safe_wrap(const value_type& val)
		: _ptr(construct_shared<value_type>(yggr_nothrow_new value_type(val)))
	{
	}

	template<typename D>
	safe_wrap(const value_type& val, D d)
		: _ptr(construct_shared<value_type>(yggr_nothrow_new value_type(val), d))
	{
	}

	template<typename D, typename A>
	safe_wrap(const value_type& val, D d, A a)
		: _ptr(construct_shared<value_type>(yggr_nothrow_new value_type(val), d, a))
	{
	}

	// base_type is value_ptr_type
	safe_wrap(const base_type& base)
		: _ptr(base)
	{
	}

	safe_wrap(BOOST_RV_REF(this_type) right)
		: _ptr()
	{
		this_type& right_ref = right;
		right_ref.swap(_ptr);
	}

	template<typename D>
	safe_wrap(BOOST_RV_REF(this_type) right, D d)
		: _ptr()
	{
		this_type& right_ref = right;

		base_type right_ptr;
		right_ref.swap(right_ptr);

		if(!right_ptr)
		{
			return;
		}

		base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(), d);
		if(tmp)
		{
			boost::swap(*tmp, *right_ptr);
		}
		_ptr.swap(tmp);
	}

	template<typename D, typename A>
	safe_wrap(BOOST_RV_REF(this_type) right, D d, A a)
		: _ptr()
	{
		this_type& right_ref = right;

		base_type right_ptr;
		right_ref.swap(right_ptr);

		if(!right_ptr)
		{
			return;
		}

		base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(), d, a);
		if(tmp)
		{
			boost::swap(*tmp, *right_ptr);
		}
		_ptr.swap(tmp);
	}

	safe_wrap(const this_type& right)
		: _ptr()
	{
		try
		{
			base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(right.load()));
			_ptr.swap(tmp);
		}
		catch(const error_type&)
		{
			return;
		}
	}

	template<typename D>
	safe_wrap(const this_type& right, D d)
		: _ptr()
	{
		try
		{
			base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(right.load()), d);
			_ptr.swap(tmp);
		}
		catch(const error_type&)
		{
			return;
		}
	}

	template<typename D, typename A>
	safe_wrap(const this_type& right, D d, A a)
		: _ptr()
	{
		try
		{
			base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(right.load()), d, a);
			_ptr.swap(tmp);
		}
		catch(const error_type&)
		{
			return;
		}
	}

	~safe_wrap(void)
	{
	}

public:
	this_type& operator=(BOOST_RV_REF(value_type) val)
	{
		this_type::store(boost::move(val));
		return *this;
	}

	this_type& operator=(const value_type& val)
	{
		this_type::store(val);
		return *this;
	}

	this_type& operator=(const base_type& ptr)
	{
		this_type::store(ptr);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type::swap(right);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		write_lock_type lk(_mutex);

		try
		{
			base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(right.load()));
			_ptr.swap(tmp);
		}
		catch(const error_type&)
		{
			return *this;
		}

		return *this;
	}

	template<typename D>
	this_type& operator=(const opeator_set_helper<D, mplex::null_type>& right_val)
	{
		const this_type& right = right_val._cref;

		if(this == &right)
		{
			return *this;
		}

		write_lock_type lk(_mutex);

		try
		{
			base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(right.load()), right_val._d);
			_ptr.swap(tmp);
		}
		catch(const error_type&)
		{
			return *this;
		}

		return *this;
	}

	template<typename D, typename A>
	this_type& operator=(const opeator_set_helper<D, A>& right_val)
	{
		const this_type& right = right_val._cref;

		if(this == &right)
		{
			return *this;
		}

		write_lock_type lk(_mutex);

		try
		{
			base_type tmp = 
				construct_shared<value_type>(
					yggr_nothrow_new value_type(right.load()),
					right_val._d, 
					right_val._a);
			_ptr.swap(tmp);
		}
		catch(const error_type&)
		{
			return *this;
		}

		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(value_type) right)
	{
		value_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(value_type& val)
	{
		{
			write_lock_type lk(_mutex);
			if(_ptr)
			{
				boost::swap(*_ptr, val);
			}
			else
			{
				base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(boost::move(val)));
				_ptr.swap(tmp);
			}
		}

		value_type tmp_val;
		boost::swap(tmp_val, val);
	}

	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(base_type& base)
	{
		write_lock_type lk(_mutex);
		if(_ptr == base)
		{
			return;
		}

		_ptr.swap(base);
	}

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

		write_lock_type lk(_mutex);
		right.swap(_ptr);
	}

public:
	YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		read_lock_type lk(_mutex);
		return !!_ptr;
	}

	value_ptr_type get_shared_ptr(void) const
	{
		read_lock_type lk(_mutex);
		return _ptr;
	}

	void store(BOOST_RV_REF(value_type) val)
	{
		base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(boost::move(val)));
		{
			write_lock_type lk(_mutex);
			_ptr.swap(tmp);
		}
	}

	void store(const value_type& val)
	{
		base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(val));
		{
			write_lock_type lk(_mutex);
			_ptr.swap(tmp);
		}
	}

	void store(const value_ptr_type& ptr)
	{
		value_ptr_type tmp(ptr);
		{
			write_lock_type lk(_mutex);
			if(_ptr != ptr)
			{
				_ptr.swap(tmp);
			}
		}
	}

	value_type exchange(BOOST_RV_REF(value_type) val)
	{
		base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(boost::move(val)));
		{
			write_lock_type lk(_mutex);
			_ptr.swap(tmp);
		}
		return tmp? *tmp : value_type();
	}

	value_type exchange(const value_type& val)
	{
		base_type tmp = construct_shared<value_type>(yggr_nothrow_new value_type(val));
		{
			write_lock_type lk(_mutex);
			_ptr.swap(tmp);
		}

		return tmp? *tmp : value_type();
	}

	value_ptr_type exchange(const value_ptr_type& ptr)
	{
		value_ptr_type tmp(ptr);

		{
			write_lock_type lk(_mutex);
			if(_ptr != ptr)
			{
				_ptr.swap(tmp);
			}
		}

        return tmp;
	}

	// discard get_value foo, same foo load, plase use load
	//value_type get_value(void) const;

	value_type load(void) const
	{
		read_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return *_ptr;
	}

	// discard save foo, same foo store, please use store
	//void save(const value_type& val);

	template<typename T>
	T get_inner_address(void) const
	{
		read_lock_type lk(_mutex);
		return reinterpret_cast<T>(_ptr.get());
	}

	bool empty(void) const
	{
		read_lock_type lk(_mutex);
		return !_ptr;
	}

	void clear(void)
	{
		value_ptr_type tmp;
		{
			write_lock_type lk(_mutex);
			tmp.swap(_ptr);
		}
	}

	template<typename Src, typename Dst>
	bool compare_exchange_weak(const Src& src, const Dst& dst)
	{
		write_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return (*_ptr == src) && (*_ptr = dst, true);
	}

	template<typename Src, typename Dst>
	bool store_or_compare_exchange_weak(const Src& src, const Dst& dst)
	{
		value_ptr_type tmp;
		if(!_ptr)
		{
			tmp = construct_shared<value_type>(yggr_nothrow_new value_type(dst));
		}

		write_lock_type lk(_mutex);
		if(!_ptr)
		{
			if(!tmp)
			{
				tmp = construct_shared<value_type>(yggr_nothrow_new value_type(dst));
			}
			tmp.swap(_ptr);
			return !!_ptr;
		}
		else
		{
			return (*_ptr == src) && (*_ptr = dst, true);
		}
	}

	bool compare_exchange_weak(const value_ptr_type& src, const value_ptr_type& dst)
	{
		write_lock_type lk(_mutex);
		return (_ptr == src) && (_ptr = dst, true);
	}

	template<typename Src, typename Dst> inline
	bool compare_exchange_strong(const Src& src, const Dst& dst)
	{
		return this_type::compare_exchange_weak(src, dst);
	}

	inline bool compare_exchange_strong(const value_ptr_type& src, const value_ptr_type& dst)
	{
		return this_type::compare_exchange_weak(src, dst);
	}

	template<typename Src, typename Dst> inline
	bool store_or_compare_exchange_strong(const Src& src, const Dst& dst)
	{
		return this_type::store_or_compare_exchange_weak(src, dst);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Foo, typename ...Args>
	typename func::foo_t_info<Foo>::result_type 
		call_function(Foo foo, BOOST_FWD_REF(Args)... args)
	{
		write_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return ((_ptr.get())->*foo)(boost::forward<Args>(args)...);
	}

	template<typename Foo, typename ...Args>
	typename func::foo_t_info<Foo>::result_type 
		call_function(Foo foo, BOOST_FWD_REF(Args)... args) const
	{
		read_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}

		return ((_ptr.get())->*foo)(boost::forward<Args>(args)...);
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename Foo \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	typename func::foo_t_info<Foo>::result_type \
		call_function(Foo foo YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
		write_lock_type lk(_mutex); \
		if(!_ptr) { throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return ((_ptr.get())->*foo)(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template<typename Foo \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	typename func::foo_t_info<Foo>::result_type \
		call_function(Foo foo YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		read_lock_type lk(_mutex); \
		if(!_ptr) { throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
		return ((_ptr.get())->*foo)(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}
		return handler(_ptr);
	}

	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_element_not_exists);
		}
		return handler(_ptr);
	}

	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		locked_unsafe_using_handler(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		return handler(_ptr);
	}

	template<typename Handler>
	typename func::foo_t_info<Handler>::result_type
		locked_unsafe_using_handler(const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		return handler(_ptr);
	}

public:
	template<typename D> inline
	static opeator_set_helper<D> 
		make_operator_set_helper(const this_type& data, D d)
	{
		typedef opeator_set_helper<D> ret_type;
		typedef typename ret_type::allocator_type now_allocator_type;
		return ret_type(data, d, now_allocator_type());
	}

	template<typename D, typename A> inline
	static opeator_set_helper<D, A> 
		make_operator_set_helper(const this_type& data, D d, A a)
	{
		typedef opeator_set_helper<D, A> ret_type;
		return ret_type(data, d, a);
	}


private:
	mutable mutex_type _mutex;
	value_ptr_type _ptr;
};

} // namesapce safe_container
} // namespace yggr


//--------------------------------------------------support swap-------------------------------------
namespace yggr
{
namespace safe_container
{
namespace swap_support
{

template<typename Val, typename Mutex, typename SharedPtr> inline 
void swap(safe_wrap<Val, Mutex, SharedPtr>& left, Val& right) 
{ 
	left.swap(right);
} 
	
template<typename Val, typename Mutex, typename SharedPtr> inline 
void swap(safe_wrap<Val, Mutex, SharedPtr>& left, SharedPtr& right ) 
{ 
	left.swap(right); 
} 
	
template<typename Val, typename Mutex, typename SharedPtr> inline 
void swap(Val& left, safe_wrap<Val, Mutex, SharedPtr>& right) 
{ 
	right.swap(left); 
} 
	
template<typename Val, typename Mutex, typename SharedPtr> inline 
void swap(SharedPtr& left, safe_wrap<Val, Mutex, SharedPtr>& right)
{ 
	right.swap(left);
} 
	
template<typename Val, typename Mutex, typename SharedPtr> inline 
void swap(safe_wrap<Val, Mutex, SharedPtr>& left, safe_wrap<Val, Mutex, SharedPtr>& right) 
{ 
	left.swap(right);
}

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

#endif // __YGGR_SAFE_CONTAINER_SAFE_WARP_HPP__
