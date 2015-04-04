//safe_wrap.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/exception.hpp>
#include <yggr/base/exception_sort.hpp>
#include <yggr/base/error_make.hpp>
#include <yggr/helper/mutex_def_helper.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
//#include <yggr/ppex/foo_params.hpp>


namespace yggr
{
namespace safe_container
{

struct safe_wrap_error
{
	ERROR_MAKER_BEGIN("safe_wrap")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_20)
			ERROR_CODE_DEF(E_empty_wrap)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_20)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_empty_wrap, "empty wrap")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()
};

template<	typename Value,
			typename Mutex = boost::mutex,
			typename SharedPtr = boost::shared_ptr<Value>
		>
class safe_wrap
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Value value_type;
	typedef value_type element_type;
	typedef SharedPtr value_ptr_type;
	typedef value_ptr_type base_type;

	typedef safe_wrap_error::error_maker_type error_maker_type;
	typedef safe_wrap_error::error_type error_type;

private:
	typedef Mutex mutex_type;
	typedef helper::mutex_def_helper<mutex_type> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

	typedef safe_wrap this_type;

public:
	safe_wrap(void)
		: _ptr()
	{
	}

	safe_wrap(value_type* pval)
		: _ptr(pval)
	{
	}

/*//#define BOOST_PP_LOCAL_MACRO(__n__) \
//	template< YGGR_PP_FOO_TYPES_DEF(__n__) > \
//	safe_wrap(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS )) \
//		: _ptr(new value_type(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA))) { }
//
//#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
//#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
//#include BOOST_PP_LOCAL_ITERATE(  )
//#undef YGGR_PP_FOO_ARG_NAME*/

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	safe_wrap(BOOST_RV_REF(value_type) val)
		: _ptr(new value_type(boost::forward<value_type>(val)))
	{
	}
#else
	safe_wrap(BOOST_RV_REF(value_type) val)
		: _ptr(new value_type(boost::forward<value_type>(val)))
	{
		const value_type& val_cref = val;
		base_type tmp(new value_type(val_cref));
		_ptr.swap(tmp);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	safe_wrap(const value_type& val)
		: _ptr(new value_type(val))
	{
	}

	safe_wrap(const base_type& base)
		: _ptr(base)
	{
	}

	~safe_wrap(void)
	{
	}

	operator bool(void) const
	{
		read_lock_type lk(_mutex);
		return !_ptr;
	}

	value_ptr_type get_shared_ptr(void) const
	{
		read_lock_type lk(_mutex);
		return _ptr;
	}

	value_type get_value(void) const
	{
		read_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_wrap);
		}

		return *_ptr;
	}

	value_type load(void) const
	{
		read_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_wrap);
		}

		return *_ptr;
	}

	this_type& operator=(BOOST_RV_REF(value_type) val)
	{
		write_lock_type lk(_mutex);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type tmp(new value_type(boost::forward<value_type>(val)));
#else
		const value_type& val_cref = val;
		base_type tmp(new value_type(val_cref));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		_ptr.swap(tmp);
		return *this;
	}

	this_type& operator=(const value_type& val)
	{
		write_lock_type lk(_mutex);
		base_type tmp(new value_type(val));
		_ptr.swap(tmp);
		return *this;
	}

	this_type& operator=(const value_ptr_type& ptr)
	{
		write_lock_type lk(_mutex);
		value_ptr_type tmp(ptr);
		_ptr.swap(tmp);
		return *this;
	}

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
		write_lock_type lk(_mutex);
		value_ptr_type().swap(_ptr);
	}

	void swap(value_type& val)
	{
		write_lock_type lk(_mutex);
		if(_ptr)
		{
			boost::swap(*_ptr, val);
			return;
		}

		base_type tmp(new value_type(val));

		value_type tmp_val;
		boost::swap(tmp_val, val);

		_ptr.swap(tmp);
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	void swap(BOOST_RV_REF(value_type) val)
	{
		write_lock_type lk(_mutex);
		if(_ptr)
		{
			boost::swap(*_ptr, val);
			return;
		}
		base_type tmp(new value_type(boost::forward<value_type>(val)));
		value_type val_tmp;
		boost::swap(val_tmp, val);
		_ptr.swap(tmp);
	}
#else
	void swap(BOOST_RV_REF(value_type) val)
	{
		write_lock_type lk(_mutex);
		if(_ptr)
		{
			value_type val_ref = val;
			boost::swap(*_ptr, val_ref);
			return;
		}

		value_type& val_ref = val;
		base_type tmp(new value_type(boost::forward<value_type>(val)));
		value_type val_tmp;
		boost::swap(val_tmp, val_ref);
		_ptr.swap(tmp);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES


	void swap(base_type& base)
	{
		write_lock_type lk(_mutex);
		_ptr.swap(base);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		write_lock_type lk(_mutex);
		base_type& base = _ptr;
		right.swap(base);
	}

	template<typename Src, typename Dst>
	bool compare_exchange_strong(const Src& src, const Dst& dst)
	{
		write_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_wrap);
		}

		if(*_ptr == src)
		{
			*_ptr = dst;
			return true;
		}

		return false;
	}

	template<typename Handler>
	typename Handler::result_type
		use_handler(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_wrap);
		}
		return handler(_ptr);
	}

	template<typename Handler>
	typename Handler::result_type
		use_handler(const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		if(!_ptr)
		{
			throw error_maker_type::make_error(error_maker_type::E_empty_wrap);
		}
		return handler(_ptr);
	}

	template<typename Handler>
	typename Handler::result_type
		locked_unsafe_use_handler(const Handler& handler)
	{
		write_lock_type lk(_mutex);
		return handler(_ptr);
	}

	template<typename Handler>
	typename Handler::result_type
		locked_unsafe_use_handler(const Handler& handler) const
	{
		read_lock_type lk(_mutex);
		return handler(_ptr);
	}

private:
	mutable mutex_type _mutex;
	value_ptr_type _ptr;
};

} // namesapce safe_container
} // namespace yggr

#define _YGGR_TMP_PP_SAFE_WRAP_SWAP() \
	template<typename Val, typename Mutex, typename SharedPtr> \
	void swap(yggr::safe_container::safe_wrap<Val, Mutex, SharedPtr>& left, \
				BOOST_RV_REF(Val) right) { \
		left.swap(right); } \
	\
	template<typename Val, typename Mutex, typename SharedPtr> \
	void swap(yggr::safe_container::safe_wrap<Val, Mutex, SharedPtr>& left, \
				Val& right) { \
		left.swap(right); } \
	\
	template<typename Val, typename Mutex, typename SharedPtr> \
	void swap(yggr::safe_container::safe_wrap<Val, Mutex, SharedPtr>& left, \
				SharedPtr& right ) { \
		left.swap(right); } \
	\
	template<typename Val, typename Mutex, typename SharedPtr> \
	void swap(BOOST_RV_REF(Val) left, \
				yggr::safe_container::safe_wrap<Val, Mutex, SharedPtr>& right) { \
		right.swap(left); } \
	\
	template<typename Val, typename Mutex, typename SharedPtr> \
	void swap(Val& left, \
				yggr::safe_container::safe_wrap<Val, Mutex, SharedPtr>& right) { \
		right.swap(left); } \
	\
	template<typename Val, typename Mutex, typename SharedPtr> \
	void swap(SharedPtr& left, \
				yggr::safe_container::safe_wrap<Val, Mutex, SharedPtr>& right) { \
		right.swap(left); }

namespace std
{
	_YGGR_TMP_PP_SAFE_WRAP_SWAP()
} // namespace std

namespace boost
{
	_YGGR_TMP_PP_SAFE_WRAP_SWAP()
} // namespace boost

#undef _YGGR_TMP_PP_SAFE_WRAP_SWAP


#endif // __YGGR_SAFE_CONTAINER_SAFE_WARP_HPP__
