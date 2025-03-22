//timer.hpp

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

#ifndef __YGGR_TIME_TIMER_HPP__
#define __YGGR_TIME_TIMER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

namespace yggr
{
namespace time
{

template<typename Time>
class timer
{
public:
	typedef Time time_type;

private:
	typedef timer this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	timer(void)
		: _start_time()
	{
	}

	timer(BOOST_RV_REF(this_type) right)
		: _start_time(boost::move(right._start_time))
	{
	}

	timer(const this_type& right)
		: _start_time(right._start_time)
	{
	}

	~timer(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_start_time, boost::move(right._start_time));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}
		_start_time = right._start_time;
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
		yggr::swap(_start_time, right._start_time);
	}

public:
	inline void restart(void)
	{
		_start_time = time_type();
	}

	inline time_type elapsed(void) const
	{
		return time_type(time_type() - _start_time);
	}

	inline const time_type& start_time(void) const
	{
		return _start_time;
	}

private:
	time_type _start_time;
};

} // namespace time
} // namespace yggr

namespace yggr
{
namespace time
{
namespace swap_support
{

template<typename Time> inline 
void swap(timer<Time>& l, timer<Time>& r)
{
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace time
} // namespace yggr

namespace std
{
	using ::yggr::time::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::time::swap_support::swap;
} // namespace boost

#endif //__YGGR_TIME_TIMER_HPP__
