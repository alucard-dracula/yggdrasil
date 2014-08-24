//data_range.hpp

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

#ifndef __YGGR_CONTAINER_DATA_RANGE_HPP__
#define __YGGR_CONTAINER_DATA_RANGE_HPP__

#include <cassert>

#include <boost/atomic.hpp>

#include <yggr/base/error_make.hpp>

namespace yggr
{
namespace container
{

template<typename T>
class data_range
{
public:
	typedef T value_type;

private:
	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("data_range")
		ERROR_CODE_DEF_NON_CODE_BEGIN()
			ERROR_CODE_DEF(E_left_limited)
			ERROR_CODE_DEF(E_right_limited)
		ERROR_CODE_DEF_NON_CODE_END()

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_left_limited, "left limited")
			ERROR_CODE_MSG(E_right_limited, "right limited")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------

private:
	typedef boost::atomic<value_type> mid_val_type;
	typedef data_range this_type;

public:
	data_range(const value_type& start, const value_type& last)
		: _start(start), _last(last), _now_val(start)
	{
		assert(_start < _last);
	}

	data_range(const value_type& start, const value_type& last, const value_type& now)
		: _start(start), _last(last), _now_val(now < start? start : now > _last? _last : now)
	{
		assert(_start < _last);
	}

	data_range(const this_type& right)
		: _start(right._start), _last(right._last), _now_val(right._now_val)
	{
	}

	~data_range(void)
	{
	}

	bool is_in(const value_type& val) const
	{
		return !(val < _start  || val >= _last);
	}

	bool is_not_in(const value_type& val) const
	{
		return val < _start  || val >= _last;
	}

	const value_type& left_limit(void) const
	{
		return _start;
	}

	const value_type& right_limit(void) const
	{
		return _last;
	}

	value_type now(void) const
	{
		return _now_val.load();
	}

	void reset(void)
	{
		value_type now(value_type());
		value_type next(_start);
		while(1)
		{
			now = _now_val.load();
			if(_now_val.compare_exchange_strong(now, next))
			{
				break;
			}
		}
	}

	value_type next(void)
	{
		value_type now(0);
		value_type next(0);
		while(1)
		{
			now = _now_val.load();
			next = now + 1;
			if(next < _last)
			{
				if(_now_val.compare_exchange_strong(now, next))
				{
					return now;
				}
			}
			else
			{
				throw error_maker_type::make_error(error_maker_type::E_right_limited);
			}
		}
	}

	value_type before(void)
	{
		value_type now(value_type());
		value_type next(value_type());

		while(1)
		{
			now = _now_val.load();
			next = now - 1;

			if(next >= _start)
			{
				if(_now_val.compare_exchange_strong(now, next))
				{
					return now;
				}
			}
			else
			{
				return error_maker_type::make_error(error_maker_type::E_left_limited);
			}
		}
	}

private:
	const value_type _start;
	const value_type _last;
	mid_val_type _now_val;
};

} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DATA_RANGE_HPP__
