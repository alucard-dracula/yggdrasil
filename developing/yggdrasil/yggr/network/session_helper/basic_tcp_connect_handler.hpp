//basic_tcp_connect_handler.hpp

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

#ifndef __YGGR_NETEORK_SESSION_HELPER_BASIC_TCP_CONNECT_HANDLER_HPP__
#define __YGGR_NETEORK_SESSION_HELPER_BASIC_TCP_CONNECT_HANDLER_HPP__

#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include <boost/bind.hpp>

#include <yggr/base/exception.hpp>
#include <yggr/base/system_code.hpp>
#include <yggr/base/interface_ptr.hpp>

#include <yggr/nonable/nonable.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>

#include <yggr/ids/base_ids_def.hpp>
#include <yggr/ids/id_generator.hpp>

#include <yggr/safe_container/safe_backup_device.hpp>


namespace yggr
{
namespace network
{
namespace session_helper
{

template<typename ConnPtr>
struct basic_tcp_connect_handler
	: public yggr::safe_container::safe_backup_device
				<
					yggr::ids::id64_type,
					ConnPtr,
					yggr::ids::pointer_id_generator<yggr::ids::id64_type>
				>,
		private nonable::noncopyable,
		private nonable::nonmoveable
{


protected:

	typedef ConnPtr conn_ptr_type;
	typedef typename mplex::pointer_to_value_t<conn_ptr_type>::type conn_type;
	typedef typename conn_type::endpoint_type endpoint_type;
	typedef typename conn_type::protocol_type protocol_type;

	typedef typename protocol_type::resolver resolver_type;
	typedef typename resolver_type::iterator resolver_iter_type;
	typedef typename resolver_type::query resolver_query_type;

private:
	typedef yggr::safe_container::safe_backup_device
				<
					yggr::ids::id64_type,
					conn_ptr_type,
					ids::pointer_id_generator<yggr::ids::id64_type>
				> base_type;

protected:
	typedef typename base_type::key_type id_type;
	typedef typename base_type::container_type container_type;

private:
	typedef basic_tcp_connect_handler this_type;

protected:
	basic_tcp_connect_handler(void)
	{
	}

	~basic_tcp_connect_handler(void)
	{
	}

	template<	typename Address,
				typename Port,
				typename Handler>
	static void s_connect(conn_ptr_type pconn,
							const Address& addr,
							const Port& port,
							const Handler& handler)
	{
		typedef Address address_type;
		typedef Port port_type;
		typedef Handler handler_type;

		if(!pconn)
		{
			return;
		}

		id_type id = base_type::backup(pconn);
		if(!id)
		{
			pconn->close();
			return;
		}

		resolver_type rslvr(pconn->get_service());
		resolver_query_type query(addr, port);
		resolver_iter_type edpt_iter = rslvr.resolve(query);
		endpoint_type edpt = *edpt_iter;

		pconn->connect(
				boost::make_tuple(
									edpt,
									boost::bind(
													&this_type::s_prv_handler_connect
																<
																	address_type,
																	port_type,
																	handler_type
																>,
													boost::asio::placeholders::error,
													++edpt_iter,
													id,
													addr,
													port,
													boost::any(handler)
												)
									)
						);
	}

private:

	template<	typename Address,
				typename Port,
				typename Handler>
	static void s_prv_handler_connect(const boost::system::error_code& e,
										resolver_iter_type edpt_iter,
										const id_type& id,
										const Address& addr,
										const Port& port,
										const boost::any& any)
	{
		typedef Address address_type;
		typedef Port port_type;
		typedef Handler handler_type;

		conn_ptr_type pconn = base_type::drop_backup(id);
		if(!pconn)
		{
			return;
		}

		const handler_type* phandler = boost::any_cast<handler_type>(&any);

		if(!phandler)
		{

			pconn->close();
			return;
		}

		bool bcontinue = (*phandler)(e, pconn);

		if(!e)
		{
			if(bcontinue)
			{
				conn_ptr_type new_pconn(new conn_type(pconn->get_init_object()));

				this_type::s_connect(
										new_pconn,
										addr,
										port,
										(*phandler)
									);
			}

			return;
		}

		if(!bcontinue)
		{
			pconn->close();
			return;
		}

		if(edpt_iter != resolver_iter_type())
		{
			endpoint_type edpt = *edpt_iter;
			pconn->connect(
							boost::make_tuple(
												edpt,
												boost::bind(
																&this_type::s_prv_handler_connect
																			<
																				address_type,
																				port_type,
																				handler_type
																			>,
																boost::asio::placeholders::error,
																++edpt_iter,
																id,
																addr,
																port,
																any
															)
											)
							);
			return;
		}

		//std::cout << "reconnect" << std::endl;
		conn_ptr_type new_pconn(new conn_type(pconn->get_init_object()));
		pconn->close();

		this_type::s_connect(
								new_pconn,
								addr,
								port,
								(*phandler)
							);


		exception::exception::throw_error(e);

	}
};

} // namespace session_helper
} // namespace network
} // namespace yggr

#endif //__YGGR_NETEORK_SESSION_HELPER_BASIC_TCP_CONNECT_HANDLER_HPP__
