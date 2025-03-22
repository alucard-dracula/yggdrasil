//timer_object_container.hpp

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

#ifndef __YGGR_TIME_DETAIL_TIMER_OBJECT_CONTAINER_HPP__
#define __YGGR_TIME_DETAIL_TIMER_OBJECT_CONTAINER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/args_holder_nv.hpp>

#include <yggr/container/list.hpp>

namespace yggr
{
namespace time
{
namespace detail
{

template<typename Val>
class timer_object_container
{
public:
	typedef Val value_type;
	typedef ::yggr::list<value_type> list_type;

	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

private:
	typedef timer_object_container this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	timer_object_container(void)
	{
	}

	timer_object_container(BOOST_RV_REF(this_type) right)
		: _list(boost::move(right._list))
	{
	}

	timer_object_container(const this_type& right)
		: _list(right._list)
	{
	}

	~timer_object_container(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		copy_or_move_or_swap(_list, boost::move(right._list));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_list = right._list;
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

		_list.swap(right._list);
	}

public:
	inline void splice_to(this_type& right)
	{
		_list.splice(_list.end(), right._list);
		_list.swap(right._list);
	}

	inline void insert(BOOST_RV_REF(value_type) timer_task)
	{
		if(timer_task)
		{
			_list.push_back(boost::move(timer_task));
		}
	}

	inline void insert(const value_type& timer_task)
	{
		if(timer_task)
		{
			_list.push_back(timer_task);
		}
	}

	template<typename Cmp> inline
	size_type remove(const Cmp& cmp)
	{
		size_type old_size = _list.size();
		_list.remove_if(cmp);
		size_type new_size = _list.size();

		return old_size - new_size;
	}

	template<typename Handler> inline
	void get(const Handler& handler)
	{
		handler(_list);
	}

	template<typename Handler> inline
	void get_of_any_handler(const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;

		const handler_type* phandler = utility::args_holder_ref_wrap_static_cast<handler_type const>(any_handler);
		assert(phandler);

		if(phandler)
		{
			(*phandler)(_list);
		}
	}

	inline yggr::size_type size(void) const
	{
		return _list.size();
	}

	inline void clear(void)
	{
		_list.clear();
	}

	inline bool empty(void) const
	{
		return _list.empty();
	}

private:
	list_type _list;
};

} // namespace detail
} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_DETAIL_TIMER_OBJECT_CONTAINER_HPP__
