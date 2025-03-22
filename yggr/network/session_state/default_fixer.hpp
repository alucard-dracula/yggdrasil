//default_fixer.hpp

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

#ifndef __YGGR_NETWORK_SESSION_STATE_DEFAULT_FIXER_HPP__
#define __YGGR_NETWORK_SESSION_STATE_DEFAULT_FIXER_HPP__

#include <yggr/network/session_state/session_state_def.hpp>

namespace yggr
{
namespace network
{
namespace session_state
{

struct default_fixer
{
public:
	// if there is no constructor, error C4101 may occur
	default_fixer(void)
	{
	}

	~default_fixer(void)
	{
	}

public:
	template<typename Def, typename Packer, typename SessionPtr> inline
	static std::pair<u32, bool> check_packet(const Packer& packet, SessionPtr sptr)
	{
		assert(sptr);
		return std::pair<u32, bool>(Def::s_init(), true);
	}

	template<typename Def, typename State, typename SessionPtr> inline
	static void check(State& state, SessionPtr sptr)
	{
		return;
	}
};

} // namespace session_state
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_SESSION_STATE_DEFAULT_FIXER_HPP__