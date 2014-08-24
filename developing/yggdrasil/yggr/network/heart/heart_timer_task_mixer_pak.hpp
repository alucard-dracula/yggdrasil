//heart_timer_task_mixer_pak.hpp

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

#ifndef __YGGR_NETWORK_HEART_HEART_TIMER_TASK_MIXER_PAK_HPP__
#define __YGGR_NETWORK_HEART_HEART_TIMER_TASK_MIXER_PAK_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/time/time.hpp>
#include <yggr/time/timer.hpp>
#include <yggr/time/timer_task_state.hpp>

namespace yggr
{
namespace network
{
namespace heart
{

template< typename ServerPtr,
			typename Handler_Tag,
			typename Protocol_Tag>
class heart_timer_task_mixer_pak
{

public:
	typedef ServerPtr server_ptr_type;
	typedef Handler_Tag handler_tag_type;
	typedef Protocol_Tag protocol_tag_type;

private:
	typedef yggr::time::timer_task_state state_type;
	typedef yggr::time::time time_type;
	typedef yggr::time::timer<time_type> timer_type;

private:
	typedef heart_timer_task_mixer_pak this_type;

public:
	heart_timer_task_mixer_pak(server_ptr_type psvr,
							const time_type& step)
		: _psvr(psvr), _step(step)
	{
		assert(_psvr);
	}

	~heart_timer_task_mixer_pak(void)
	{
	}

	u32 operator()(void)
	{
		if(_step < _timer.elapsed())
		{
			_psvr->template check_state<handler_tag_type, protocol_tag_type>();
			_timer.restart();
			return yggr::time::timer_task_state::E_timer_task_ran;
		}

		return yggr::time::timer_task_state::E_timer_task_not_ran;
	}

private:
	server_ptr_type _psvr;
	time_type _step;
	timer_type _timer;

};

} // namespace heart
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_HEART_HEART_TIMER_TASK_MIXER_PAK_HPP__
