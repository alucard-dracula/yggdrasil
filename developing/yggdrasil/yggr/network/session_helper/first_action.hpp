//first_action.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HELPER_FIRST_ACTION_HPP__
#define __YGGR_NETWORK_SESSION_HELPER_FIRST_ACTION_HPP__

namespace yggr
{
namespace network
{
namespace session_helper
{

class first_action
{
public:
	template<typename Session>
	void operator()(Session& s) const
	{
		typedef Session session_type;
		typedef typename session_type::conn_ptr_type conn_ptr_type;

		{
			conn_ptr_type ptr = s.get_conn_ptr();
			if(!ptr)
			{
				s.handler_close_level_error();
				return;
			}
			ptr->next_send();
		}

		s.prv_recv_packet_helper(
									boost::make_tuple(
														boost::bind(
																		&session_type::handler_recv, 
																		s.shared_from_this(), _1, _2
																	), 
														boost::bind(
																		&session_type::handler_close_level_error, 
																		s.shared_from_this()
																	)
													)
								);
	}
};

} // namespace session_helper
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_SESSION_HELPER_FIRST_ACTION_HPP__