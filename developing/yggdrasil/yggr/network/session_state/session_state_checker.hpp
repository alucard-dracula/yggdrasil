//session_state_checker.hpp

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

#ifndef __YGGR_NETWORK_SESSION_STATE_SESSION_STATE_CHECKER_HPP__
#define __YGGR_NETWORK_SESSION_STATE_SESSION_STATE_CHECKER_HPP__

#include <yggr/network/session_state/session_state_def.hpp>
#include <yggr/network/session_state/default_checker.hpp>

namespace yggr
{
namespace network
{
namespace session_state
{

template<typename Def, typename Checker>
class session_state_checker
{
public:
	typedef Def def_type;
	typedef Checker checker_type;

private:
	typedef session_state_checker this_type;

public:
	session_state_checker(void)
	{
	}

	~session_state_checker(void)
	{
	}

	template<typename Session_Container>
	void operator()(Session_Container& cont) const
	{
		checker_type checker;
		checker(cont);
	}

	//template<typename Session_Container, typename Handler>
	//void operator()(Session_Container& cont, const Handler& handler) const
	//{
	//	checker_type checker;
	//	checker(cont, handler);
	//}
};

typedef session_state_checker
		<
			session_state_def<tag_def::E_session_state_online>, 
			default_checker
		> default_session_state_checker_type;

} // session_state
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SESSION_STATE_SESSION_STATE_CHECKER_HPP__