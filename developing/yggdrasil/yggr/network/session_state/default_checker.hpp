//default_checker.hpp

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

#ifndef __YGGR_NETWORK_SESSION_STATE_DEFAULT_CHECKER_HPP__
#define __YGGR_NETWORK_SESSION_STATE_DEFAULT_CHECKER_HPP__

#include <yggr/network/session_state/session_state_def.hpp>

namespace yggr
{
namespace network
{
namespace session_state
{

struct default_checker
{
	template<typename Session_Container>
	inline void operator()(Session_Container& cont) const
	{
		assert(false); //default dont't need call this function
		return;
	}
};

} // namespace session_state
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_SESSION_STATE_DEFAULT_CHECKER_HPP__