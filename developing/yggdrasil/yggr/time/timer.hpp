//timer.hpp

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

#ifndef __YGGR_TIME_TIMER_HPP__
#define __YGGR_TIME_TIMER_HPP__



#include <yggr/move/move.hpp>

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
		: _start_time(boost::forward<time_type>(right._start_time))
	{
	}

	timer(const this_type& right)
		: _start_time(right._start_time)
	{
	}

	~timer(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		_start_time = boost::forward<time_type>(right._start_time);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_start_time = right._start_time;
		return *this;
	}

	void restart(void)
	{
		_start_time = time_type();
	}

	const time_type elapsed(void) const
	{
		return  time_type(time_type() - _start_time);
	}

	const time_type elapsed_max(void) const
	{
		return time_type::max_time() - _start_time;
	}

	const time_type elapsed_min(void) const
	{
		return time_type::min_time();
	}

	const time_type& start_time(void) const
	{
		return _start_time;
	}

//	void swap(BOOST_RV_REF(this_type) right)
//	{
//		std::swap(_start_time, boost::forward<time_type>(right._start_time));
//	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(_start_time, right._start_time);
	}

private:
	time_type _start_time;
};

} // namespace time
} // namespace timer

#define _YGGR_TMP_PP_TIMER_SWAP_DEF() \
	template<typename Time> \
	void swap(yggr::time::timer<Time>& left, yggr::time::timer<Time>& right) { \
		left.swap(right); } \
	\
	template<typename Time> \
	void swap(BOOST_RV_REF(yggr::time::timer<Time>) left, yggr::time::timer<Time>& right) { \
		right.swap(left); } \
	\
	template<typename Time> \
	void swap(yggr::time::timer<Time>& left, BOOST_RV_REF(yggr::time::timer<Time>) right) { \
		left.swap(right); }

namespace std
{
	_YGGR_TMP_PP_TIMER_SWAP_DEF()
} // namespace std

namespace boost
{
	_YGGR_TMP_PP_TIMER_SWAP_DEF()
} // namespace boost

#undef _YGGR_TMP_PP_TIMER_SWAP_DEF

#endif //__YGGR_TIME_TIMER_HPP__
