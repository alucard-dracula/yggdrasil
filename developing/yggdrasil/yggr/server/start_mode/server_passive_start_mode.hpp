//server_passive_start_mode.hpp

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

#ifndef __YGGR_SERVER_START_MODE_SERVER_PASSIVE_START_MODE_HPP__
#define __YGGR_SERVER_START_MODE_SERVER_PASSIVE_START_MODE_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <yggr/network/type_traits/tags.hpp>

namespace yggr
{
namespace server
{
namespace start_mode
{

struct server_passive_start_mode
{
	template<typename Session>
	void operator()(Session& s) const
	{
		typedef Session session_type;
		prv_start_helper(s,
							boost::make_tuple(
												boost::bind(
																&session_type::handler_recv, 
																s.shared_from_this(), 
																_1, _2),
												boost::bind(
																&session_type::handler_close_level_error, 
																s.shared_from_this()
															)
											)
						);
	}

private:
	template<typename Session, typename Handler>
	void prv_start_helper(Session& s, const Handler& handler) const
	{
		typedef Session session_type;
		typedef Handler handler_type;

		s.handler_start(boost::asio::placeholders::error, handler)
	}
};

} // namespace start_mode
} // namespace server
} // namespace yggr

#endif // __YGGR_SERVER_START_MODE_SERVER_PASSIVE_START_MODE_HPP__