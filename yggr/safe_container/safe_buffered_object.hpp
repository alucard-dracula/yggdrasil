//safe_buffered_object.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_BUFFERED_OBJECT_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_BUFFERED_OBJECT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/thread/mutex_def_helper.hpp>
#include <yggr/ppex/has_member_check.hpp>

#include <yggr/mplex/limits.hpp>

#include <yggr/safe_container/safe_deque.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/ref.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_same.hpp>


YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_BEGIN()

YGGR_PP_HAS_ANY_MEMBER_FOO_DEF(splice_to, splice_to)

YGGR_PP_HAS_MEMBER_FOO_NAMESPACE_END()

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Queue, bool has_splice_to>
struct safe_buffered_object_splice_to_op_impl
{
public:
	typedef Queue queue_type;
	typedef typename queue_type::value_type obj_ptr_type;

private:
	typedef safe_buffered_object_splice_to_op_impl this_type;

public:
	inline void operator()(queue_type&, obj_ptr_type) const
	{
	}
};

template<typename Queue>
struct safe_buffered_object_splice_to_op_impl<Queue, true>
{
public:
	typedef Queue queue_type;
	typedef typename queue_type::value_type obj_ptr_type;

private:
	typedef safe_buffered_object_splice_to_op_impl this_type;

public:
	inline void operator()(queue_type& queue, obj_ptr_type pobj) const
	{
		queue.using_handler(
			boost::bind(
				&this_type::prv_s_handler_splice_to,
				_1, pobj));
	}

private:
	inline static void prv_s_handler_splice_to(typename queue_type::base_type& base, obj_ptr_type ptr)
	{
		assert(base.size() && base.front());

		if(!(ptr && base.size() && base.front()))
		{
			return;
		}

		ptr->splice_to(*base.front());
	}
};

template<typename Queue>
struct safe_buffered_object_splice_to_op
	: public
		safe_buffered_object_splice_to_op_impl
		<
			Queue,
			YGGR_PP_HAS_ANY_MEMBER_FOO_TYPENAME_VALUE(splice_to, Queue)
		>
{
};

// safe_buffered_object_fake_obj_init
struct safe_buffered_object_fake_obj_init
{
	safe_buffered_object_fake_obj_init(void)
	{
	}

	template<typename T>
	safe_buffered_object_fake_obj_init(BOOST_RV_REF(T))
	{
	}

	template<typename T>
	safe_buffered_object_fake_obj_init(const T&)
	{
	}
};

// safe_buffered_object_obj_creator
template<typename Object, typename Param,
			template<typename _T> class Alloc>
struct safe_buffered_object_obj_creator
{
	typedef Object obj_type;
	typedef Param param_type;
	typedef Alloc<obj_type> obj_alloc_type;
	typedef ::yggr::shared_ptr<obj_type> obj_ptr_type;

	inline obj_ptr_type operator()(const param_type& arg) const
	{
		obj_alloc_type alloc;
		return ::yggr::allocate_shared<obj_type>(alloc, arg);
	}
};

template<typename Object, template<typename _T> class Alloc>
struct safe_buffered_object_obj_creator<Object, safe_buffered_object_fake_obj_init, Alloc>
{
	typedef Object obj_type;
	typedef safe_buffered_object_fake_obj_init param_type;
	typedef Alloc<obj_type> obj_alloc_type;
	typedef ::yggr::shared_ptr<obj_type> obj_ptr_type;

	inline obj_ptr_type operator()(const param_type&) const
	{
		obj_alloc_type alloc;
		return ::yggr::allocate_shared<obj_type>(alloc);
	}
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{

template<typename Object, typename Mutex = void,
			typename Obj_Init = void,
			template<typename _T> class Alloc = std::allocator>
class safe_buffered_object
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
private:
	typedef detail::safe_buffered_object_fake_obj_init fake_obj_init_type;

public:
	YGGR_STATIC_CONSTANT(::yggr::size_type, E_min_limit_size = 2);

	typedef Object obj_type;

	typedef typename
		boost::mpl::if_
		<
			boost::is_same<Obj_Init, void>,
			fake_obj_init_type,
			Obj_Init
		>::type obj_init_type;

	typedef ::yggr::shared_ptr<obj_type> obj_ptr_type;

private:
	typedef Mutex tpl_arg_mutex_type;
	typedef Alloc<obj_ptr_type> allocator_type;

	typedef safe_container::safe_deque<obj_ptr_type, tpl_arg_mutex_type, allocator_type> obj_ptr_queue_type;
	typedef typename obj_ptr_queue_type::base_type unsafe_obj_ptr_queue_type;
	typedef typename obj_ptr_queue_type::iterator obj_ptr_queue_iter_type;
	typedef typename obj_ptr_queue_type::const_iterator obj_ptr_queue_citer_type;

	typedef
		detail::safe_buffered_object_obj_creator
		<
			obj_type,
			obj_init_type,
			Alloc
		> obj_creator_type;

	typedef detail::safe_buffered_object_splice_to_op<obj_ptr_queue_type> splice_to_op_type;


private:
	typedef safe_buffered_object this_type;

public:

	safe_buffered_object(void)
		: _init(),
			_limit_size(mplex::numeric_limits< ::yggr::size_type>::max_type::value)
	{
		this_type::prv_nodes_init();
		assert(_queue.size() == 1);
	}

	template<typename T>
	safe_buffered_object(BOOST_RV_REF(T) init,
							yggr::size_type limit_size
								= mplex::numeric_limits< ::yggr::size_type>::max_type::value)
		: _init(obj_init_type(init)),
			_limit_size((limit_size < this_type::E_min_limit_size? this_type::E_min_limit_size : limit_size))
	{
		this_type::prv_nodes_init();
		assert(_queue.size() == 1);
	}

	template<typename T>
	safe_buffered_object(const T& init,
							yggr::size_type limit_size
								= mplex::numeric_limits< ::yggr::size_type>::max_type::value )
		: _init(obj_init_type(init)),
			_limit_size((limit_size < this_type::E_min_limit_size? this_type::E_min_limit_size : limit_size))
	{
		this_type::prv_nodes_init();
		assert(_queue.size() == 1);
	}

	~safe_buffered_object(void)
	{
	}

public:
	void clear(void)
	{
		prv_nodes_init();
		unsafe_obj_ptr_queue_type tmp;

		obj_ptr_type ptr;

		do
		{
			ptr = this_type::prv_create_node();
			assert(ptr);
		} while (!ptr);

		for(;tmp.size() != 1;)
		{
			try
			{
				tmp.push_back(ptr);
				assert(tmp.size() == 1);
				break;
			}
			catch(...)
			{
				unsafe_obj_ptr_queue_type clear_tmp;
				yggr::swap(clear_tmp, tmp);
			}
		}

		_queue.swap(tmp);
	}

	template<typename Value> inline
	bool insert(BOOST_RV_REF(Value) val)
	{
		typedef Value now_val_type;

		now_val_type tmp(boost::move(val));

		return
			_queue.using_handler(
				boost::bind(
					&this_type::prv_s_handler_value_insert<now_val_type>,
					_1, boost::ref(tmp)));
	}

	template<typename Value> inline
	bool insert(const Value& val)
	{
		typedef Value now_val_type;

		now_val_type tmp(val);

		return
			_queue.using_handler(
				boost::bind(
					&this_type::prv_s_handler_value_insert<now_val_type>,
					_1, boost::ref(tmp)));
	}

	template<typename Cmp> inline
	yggr::size_type remove(const Cmp& cmp)
	{
		typedef Cmp cmp_type;

		return
			_queue.using_handler(
				boost::bind(
					&this_type::prv_s_handler_value_remove<cmp_type>,
					_1, boost::cref(cmp)));
	}

	inline obj_ptr_type drop(void)
	{
		obj_ptr_type ptr;
		obj_ptr_type ptr_new;

		if(_queue.size() == 1)
		{
			if(!(ptr_new = this_type::prv_create_node()))
			{
				return obj_ptr_type();
			}
		}

		ptr =
			_queue.using_handler(
				boost::bind(
					&this_type::prv_handler_drop, this, _1, ptr_new));

		return ptr;
	}

	inline void back(const obj_ptr_type& ptr)
	{
		if(!(ptr && _queue.size() < _limit_size))
		{
			return;
		}

		_queue.using_handler(
			boost::bind(&this_type::prv_handler_node_push_back, this, _1, ptr));
	}

public:

	template<typename Handler>
	bool get(const Handler& handler) const
	{
		obj_ptr_type ptr = this_type::prv_get_drop();

		if(!ptr)
		{
			return false;
		}

		handler(boost::ref(*ptr));

		if(!ptr->empty())
		{
			splice_to_op_type op;
			op(_queue, ptr);
			ptr->clear();
		}

		this_type::prv_get_back(ptr);
		return true;
	}

	template<typename Handler, typename Return_Handler>
	bool get(const Handler& handler, const Return_Handler& ret_handler) const
	{
		obj_ptr_type ptr = this_type::prv_get_drop();

		if(!ptr)
		{
			return false;
		}

		handler(boost::ref(*ptr), ret_handler);

		if(!ptr->empty())
		{
			splice_to_op_type op;
			op(_queue, ptr);
			ptr->clear();
		}

		this_type::prv_get_back(ptr);
		return true;
	}

	inline static ::yggr::size_type min_size(void)
	{
		return this_type::E_min_limit_size;
	}

	inline ::yggr::size_type limit_size(void) const
	{
		return _limit_size;
	}

private:
	obj_ptr_type prv_get_drop(void) const
	{
		obj_ptr_type ptr;
		obj_ptr_type ptr_new;

		if(_queue.size() == 1)
		{
			if(!(ptr_new = this_type::prv_create_node()))
			{
				return obj_ptr_type();
			}
		}

		return
			_queue.using_handler(
				boost::bind(
					&this_type::prv_handler_drop_non_empty, this, _1, ptr_new));
	}

	inline void prv_get_back(const obj_ptr_type& ptr) const
	{
		if(ptr && _queue.size() < _limit_size)
		{
			_queue.using_handler(
				boost::bind(&this_type::prv_handler_node_push_back, this, _1, ptr));
		}
	}

	template<typename Cmp>
	static ::yggr::size_type prv_s_handler_value_remove(unsafe_obj_ptr_queue_type& base, const Cmp cmp)
	{
		::yggr::size_type s = 0;
		for(obj_ptr_queue_iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
		{
			assert(*i);
			s += (*i)? (*i)->remove(cmp) : 0;
		}

		return s;
	}

	template<typename T> inline
	static bool prv_s_handler_value_insert(unsafe_obj_ptr_queue_type& base, T& val)
	{
		assert(base.size() && base.front());
		return
			base.size()
			&& base.front()
			&& (base.front()->insert(boost::move(val)), true);
	}

	obj_ptr_type prv_handler_drop(unsafe_obj_ptr_queue_type& base, obj_ptr_type ptr_new)
	{
		assert((base.size() && base.front()));

		if(!(base.size() && base.front()))
		{
			return obj_ptr_type();
		}

		if((base.size() == 1))
		{
			if(!ptr_new)
			{
				if(!(ptr_new = this_type::prv_create_node()))
				{
					return obj_ptr_type();
				}
			}

			assert((ptr_new && ptr_new != base.front()));
			::yggr::swap(base.front(), ptr_new);

			return ptr_new;
		}
		else
		{
			obj_ptr_type tmp = base.front();
			base.pop_front();
			return tmp;
		}
	}

	obj_ptr_type prv_handler_drop_non_empty(unsafe_obj_ptr_queue_type& base, obj_ptr_type ptr_new) const
	{
		assert((base.size() && base.front()));
		if(!(base.size() && base.front()))
		{
			return obj_ptr_type();
		}

		if((base.size() == 1))
		{
			if(!ptr_new)
			{
				if(!(ptr_new = this_type::prv_create_node()))
				{
					return obj_ptr_type();
				}
			}

			if(base.front()->empty())
			{
				base.push_back(ptr_new);
				return obj_ptr_type();
			}

			assert(ptr_new != base.front());
			::yggr::swap(base.front(), ptr_new);

			return ptr_new;
		}
		else
		{
			obj_ptr_type tmp = base.front()->empty()? obj_ptr_type() : base.front();
			if(tmp)
			{
				base.pop_front();
			}

			return tmp;
		}
	}

	void prv_nodes_init(void)
	{
		unsafe_obj_ptr_queue_type tmp;
		tmp.resize(this_type::E_min_limit_size);
		tmp.clear();

		obj_ptr_type ptr;

		for(;tmp.size() < 1;)
		{
			ptr = this_type::prv_create_node();
			assert(ptr);
			if(!ptr)
			{
				continue;
			}

			try
			{
				tmp.push_back(ptr);
			}
			catch(...)
			{
				unsafe_obj_ptr_queue_type tmp_clear;
				tmp_clear.resize(this_type::E_min_limit_size);
				tmp_clear.clear();
				::yggr::swap(tmp, tmp_clear);
			}
		}

		assert(tmp.size() == 1);

		_queue.swap(tmp);
	}

	inline obj_ptr_type prv_create_node(void) const
	{
		static obj_creator_type creator;
		return creator(_init);
	}

private:
	inline bool prv_handler_node_push_back(unsafe_obj_ptr_queue_type& base, obj_ptr_type ptr) const
	{
		try
		{
			yggr::size_type old_size = base.size();
			return
				ptr
				&& (old_size < _limit_size)
				&& (base.push_back(ptr), old_size < base.size());
		}
		catch(...)
		{
			return false;
		}
	}

private:
	const obj_init_type _init;
	const yggr::size_type _limit_size;
	mutable obj_ptr_queue_type _queue;
};

} // namespace safe_container
} // namespace yggr

#endif //__YGGR_SAFE_CONTAINER_SAFE_BUFFERED_OBJECT_HPP__
