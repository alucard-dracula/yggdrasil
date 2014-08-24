//session_state_def.hpp

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

#ifndef __YGGR_NETWORK_SESSION_STATE_SESSION_STATE_DEF_HPP__
#define __YGGR_NETWORK_SESSION_STATE_SESSION_STATE_DEF_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

#include <yggr/base/yggrdef.h>

namespace yggr
{
namespace network
{
namespace session_state
{

struct tag_def
{
	enum
	{
		E_session_state_offline = 0,
		E_session_state_need_check,
		E_session_state_online,

		E_session_state_end,
		
		E_compile_u32 = 0xffffffff
	};
};

template<u32 init>
struct session_state_def
{
	enum
	{
		E_session_state_offline = tag_def::E_session_state_offline,
		E_session_state_need_check = tag_def::E_session_state_need_check,
		E_session_state_online = tag_def::E_session_state_online,

		E_session_state_end = tag_def::E_session_state_end,
		
		E_compile_u32 = 0xffffffff
	};

	BOOST_MPL_ASSERT((
						boost::mpl::bool_
						<
							(
								init >= E_session_state_offline
								&& init < E_session_state_end
							)
						>
					));

	inline static u32 s_init(void)
	{
		return init;
	}

	inline static u32 s_online(void)
	{
		return E_session_state_online;
	}

	inline static u32 s_need_check(void)
	{
		return E_session_state_need_check;
	}

	inline static u32 s_offline(void)
	{
		return E_session_state_offline;
	}
};

} // namespace session_state
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SESSION_STATE_SESSION_STATE_DEF_HPP__