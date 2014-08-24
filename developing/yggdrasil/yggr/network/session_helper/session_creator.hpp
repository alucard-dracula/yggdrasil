//session_creator.hpp

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

#ifndef __YGGR_NETWOR_SESSION_HELPER_SESSION_CREATOR_HPP__
#define __YGGR_NETWOR_SESSION_HELPER_SESSION_CREATOR_HPP__

#include <yggr/mplex/pointer_to_value_t.hpp>
#include <boost/shared_ptr.hpp>

namespace yggr
{
namespace network
{
namespace session_helper
{

template<typename Start_Data, typename Conn,
			template<typename _Start_Data>
				class Checker_Creator,
			template <typename _Start_Data>
				class Crypher_Creator>
class session_creator
{
public:
	typedef Start_Data start_data_type;
	typedef Conn conn_type;
	typedef boost::shared_ptr<conn_type> conn_ptr_type;
	typedef Checker_Creator<start_data_type> checker_creator_type;
	typedef Crypher_Creator<start_data_type> crypher_creator_type;

private:
	typedef session_creator this_type;

public:
	session_creator(const conn_ptr_type& pconn, const start_data_type& start_data)
		: _pconn(pconn), _start_data(start_data)
	{
	}

	~session_creator(void)
	{
	}

	template<typename SessionPtr, typename AdapterMgrPtr>
	SessionPtr create(const AdapterMgrPtr& padp_mgr) const
	{
		typedef SessionPtr session_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_ptr_type>::type session_type;
		typedef typename session_type::conn_ptr_type now_conn_ptr_type;

		BOOST_MPL_ASSERT((boost::is_same<conn_ptr_type, now_conn_ptr_type>));

		if(!_pconn)
		{
			return session_ptr_type();
		}

		if(!_start_data.id())
		{
			_pconn->close();
			return session_ptr_type();
		}

		checker_creator_type chc;
		crypher_creator_type crc;

		try
		{
			return session_ptr_type(
										new session_type(
															_start_data.id(), _pconn, padp_mgr,
															chc(_start_data), crc(_start_data),
															_pconn->local_endpoint(),
															_pconn->remote_endpoint()
														)
										);
		}
		catch(const boost::system::error_code&)
		{
		}

		_pconn->close();
		return session_ptr_type();
	}

	void cancel(void) const
	{
		if(!_pconn)
		{
			return ;
		}

		_pconn->close();
	}

	conn_ptr_type get(void) const
	{
		return _pconn;
	}

private:
	mutable conn_ptr_type _pconn;
	start_data_type _start_data;
	
};

} // namespace session_helper
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWOR_SESSION_HELPER_SESSION_CREATOR_HPP__