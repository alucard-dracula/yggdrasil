//session_tcp_curer.hpp

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

#ifndef __YGGR_NETWORK_SESSION_HELPER_SESSION_TCP_CURER_HPP__
#define __YGGR_NETWORK_SESSION_HELPER_SESSION_TCP_CURER_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/mplex/pointer_to_value_t.hpp>
#include <yggr/network/session_helper/basic_tcp_connect_handler.hpp>

namespace yggr
{
namespace network
{
namespace session_helper
{

template<typename Session>
struct session_tcp_curer
	: public basic_tcp_connect_handler<typename Session::conn_ptr_type>
{

private:
	typedef Session session_type;
	typedef typename session_type::conn_ptr_type conn_ptr_type;
	typedef typename session_type::conn_type conn_type;
	typedef basic_tcp_connect_handler<conn_ptr_type> base_type;

private:
	typedef session_tcp_curer this_type;

public:

	//insert session after
	template<typename Key, typename Session_Mgr>
	void operator()(const Key& key, Session_Mgr& smgrt) const
	{
		this_type::prv_operator_helper(key, smgrt.shared_from_this());
	}

	//insert session befor
	template<typename Session_Mgr>
	void operator()(conn_ptr_type pconn, Session_Mgr& smgrt) const
	{
		this_type::prv_operator_helper(pconn, smgrt.shared_from_this());
	}


private:

	template<typename Key, typename Session_Mgr_Ptr >
	void prv_operator_helper(const Key& key, Session_Mgr_Ptr psmgr) const
	{
		typedef Session_Mgr_Ptr session_mgr_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_mgr_ptr_type>::type session_mgr_type;
		typedef typename session_mgr_type::session_ptr_type session_ptr_type;
		typedef typename session_mgr_type::session_type now_session_type;

		BOOST_MPL_ASSERT((boost::is_same<now_session_type, session_type>));

		session_ptr_type sptr = psmgr->drop(key);

		if(!sptr)
		{
			return;
		}

		{
			conn_ptr_type pconn = sptr->get_conn_ptr();
			this_type::prv_operator_helper(pconn, psmgr);
		}

		sptr->close();
	}

	template<typename Session_Mgr_Ptr >
	void prv_operator_helper(const conn_ptr_type& pconn, Session_Mgr_Ptr psmgr) const
	{
		typedef Session_Mgr_Ptr session_mgr_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_mgr_ptr_type>::type session_mgr_type;
		typedef typename session_mgr_type::session_ptr_type session_ptr_type;
		typedef typename session_mgr_type::session_type now_session_type;

		BOOST_MPL_ASSERT((boost::is_same<now_session_type, session_type>));

		if(!pconn)
		{
			return;
		}

		conn_ptr_type new_pconn(new conn_type(pconn->get_init_object()));

		if(!new_pconn)
		{
			return;
		}

		base_type::s_connect(
								new_pconn,
								pconn->str_remote_address(),
								pconn->str_remote_port(),
								boost::bind(&this_type::s_new_handler_connect
														<
															session_mgr_ptr_type
														>,
												_1, _2, psmgr)
							);
	}

	template<typename SessionMgrPtr>
	static bool s_new_handler_connect(const boost::system::error_code& e,
										conn_ptr_type pconn,
										SessionMgrPtr psmgr
										)
	{
		if(!psmgr)
		{
			if(pconn)
			{
				pconn->close();
			}
			return false;
		}

		if(!pconn)
		{
			return false;
		}

		if(!e)
		{
			psmgr->insert(pconn);
			return false;
		}

		return true;
	}
};

} // namespace exeception_fixer
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_SESSION_HELPER_SESSION_TCP_CURER_HPP__
