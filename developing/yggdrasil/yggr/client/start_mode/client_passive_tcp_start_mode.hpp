//client_passive_tcp_start_mode.hpp

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

#ifndef __YGGR_CLIENT_START_MODE_CLIENT_PASSIVE_TCP_START_MODE_HPP__
#define __YGGR_CLIENT_START_MODE_CLIENT_PASSIVE_TCP_START_MODE_HPP__

#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/mplex/pointer_to_value_t.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/hn_conv.hpp>

namespace yggr
{
namespace client
{
namespace start_mode
{

template<
			typename Recv_Packet,
			typename Start_Data_Generator,
			typename Session_Creator
		>
struct client_passive_tcp_start_mode
{
public:
	typedef Recv_Packet recv_packet_type;
	typedef Start_Data_Generator start_data_generator;
	typedef Session_Creator session_creator_type;

private:
	typedef typename start_data_generator::start_data_type start_data_type;

private:
	typedef client_passive_tcp_start_mode this_type;

public:
	template<typename ConnPtr, typename Session_Mgr>
	void operator()(ConnPtr pconn, Session_Mgr& smgr) const
	{
		this_type::prv_create_helper(pconn, smgr.shared_from_this());
	}

private:
	template<typename ConnPtr, typename Session_Mgr_Ptr>
	void prv_create_helper(ConnPtr pconn, Session_Mgr_Ptr psmgr) const
	{
		typedef ConnPtr conn_ptr_type;
		typedef Session_Mgr_Ptr session_mgr_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_mgr_ptr_type>::type session_mgr_type;
		typedef typename session_mgr_type::tag_type session_mgr_tag_type;

		BOOST_MPL_ASSERT((
							boost::is_same
							<
								session_mgr_tag_type,
								yggr::network::type_traits::tag_client
							>
						));

		if(!pconn)
		{
			return;
		}

		pconn->template recv_packet<recv_packet_type>(
								boost::make_tuple(
														boost::bind(
																		&this_type::s_handler_recv_eins
																					<
																						conn_ptr_type,
																						session_mgr_ptr_type,
																						recv_packet_type
																					>,
																		_1, _2, pconn, psmgr
																	),
														boost::bind(
																		&this_type::s_handler_recv_eins_error
																					<
																						conn_ptr_type
																					>,
																		pconn
																	)
													)
											);
	}

	template<typename ConnPtr, typename Session_Mgr_Ptr, typename Packet>
	static void s_handler_recv_eins(const boost::system::error_code& e, Packet& pak,
										ConnPtr pconn, Session_Mgr_Ptr psmgr)
	{
		typedef Session_Mgr_Ptr session_mgr_ptr_type;
		typedef typename mplex::pointer_to_value_t<session_mgr_ptr_type>::type session_mgr_type;
		typedef typename session_mgr_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;
		if(!(pconn && psmgr))
		{
			return;
		}

		if(pak.owner_id())
		{
			pconn->close();
			return;
		}

		start_data_generator gener;
		start_data_type start_data(gener(pak));
		pak.owner_info().set_now_owner_id(start_data.id());

		psmgr->handler_create_and_fix_start_packet(e, session_creator_type(pconn, start_data), pak);
	}

	template<typename ConnPtr>
	static void s_handler_recv_eins_error(ConnPtr pconn)
	{
		if(pconn)
		{
			pconn->close();
		}
	}
};

} // namespce start_mode
} // namespace client
} // namespace yggr

#endif //__YGGR_CLIENT_START_MODE_CLIENT_PASSIVE_TCP_START_MODE_HPP__
