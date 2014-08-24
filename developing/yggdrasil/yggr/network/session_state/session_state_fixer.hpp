//session_state_fixer.hpp

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

#ifndef __YGGR_NETWORK_SESSION_STATE_FIXER_HPP__
#define __YGGR_NETWORK_SESSION_STATE_FIXER_HPP__

#include <boost/atomic.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/network/session_state/session_state_def.hpp>
#include <yggr/network/session_state/default_fixer.hpp>

namespace yggr
{
namespace network
{
namespace session_state
{

template<typename Def, typename Fixer>
class session_state_fixer
{
public:
	typedef Fixer fixer_type;
	typedef Def def_type;

private:
	typedef boost::atomic<u32> state_type;
	typedef session_state_fixer this_type;

public:
	session_state_fixer(void)
		: _state(def_type::s_init())
	{
	}

	~session_state_fixer(void)
	{
	}

	template<typename Packet, typename SessionPtr>
	bool fix_packet(const Packet& pak, SessionPtr sptr)
	{
		typedef std::pair<u32, bool> ret_type;

		if(!sptr)
		{
			return false;
		}

		fixer_type fixer;
		ret_type ret = fixer.template check_packet<def_type>(pak, sptr);

		u32 old_sat = 0;
		u32 new_sat = ret.first;
		for(;;)
		{
			old_sat = _state.load();
			if(_state.compare_exchange_strong(old_sat, new_sat))
			{
				break;
			}
		}

		return ret.second;
	}

	template<typename SessionPtr>
	void check(SessionPtr sptr)
	{
		fixer_type fixer;
		fixer.template check<def_type>(_state, sptr);
	}

	//template<typename SessionPtr, typename Handler>
	//void check(SessionPtr sptr, const Handler& handler)
	//{
	//	fixer_type fixer;
	//	fixer.check<def_type>(_state, sptr, handler);
	//}

	u32 now_state(void) const
	{
		return _state.load();
	}

private:
	state_type _state;
};

typedef session_state_fixer
		<
			session_state_def<tag_def::E_session_state_online>,
			default_fixer
		> default_session_state_fixer_type;

} // namespace session_state
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SESSION_STATE_FIXER_HPP__
