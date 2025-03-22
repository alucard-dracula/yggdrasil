//basic_tcp_connect_handler.hpp

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

#ifndef __YGGR_NETEORK_SESSION_HELPER_BASIC_TCP_CONNECT_HANDLER_HPP__
#define __YGGR_NETEORK_SESSION_HELPER_BASIC_TCP_CONNECT_HANDLER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/bind/bind.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/time/time.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/system_controller/system_code.hpp>
#include <yggr/smart_ptr_ex/interface_ptr.hpp>

#include <yggr/nonable/nonable.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>
#include <yggr/utility/args_holder_nv.hpp>

#include <yggr/ids/base_ids_def.hpp>

#include <yggr/safe_container/safe_backup_device.hpp>

#include <boost/asio.hpp>

namespace yggr
{
namespace network
{
namespace session_helper
{

template<typename ConnPtr>
struct basic_tcp_connect_handler
	: public 
		yggr::safe_container::safe_backup_device
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
	typedef 
		yggr::safe_container::safe_backup_device
		<
			yggr::ids::id64_type,
			conn_ptr_type,
			ids::pointer_id_generator<yggr::ids::id64_type>
		> base_type;

protected:
	typedef typename base_type::key_type id_type;
	typedef typename base_type::container_type container_type;

private:
	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

private:
	typedef basic_tcp_connect_handler this_type;

protected:
	basic_tcp_connect_handler(void)
	{
	}

	~basic_tcp_connect_handler(void)
	{
	}

protected:
	template<	typename Address,
				typename Port,
				typename Handler> inline
	static void s_connect(conn_ptr_type pconn,
							const Address& addr,
							const Port& port,
							const Handler& handler)
	{
		typedef Address address_type;
		typedef Port port_type;
		typedef Handler handler_type;
		typedef utility::args_holder_nv<handler_type> holder_handler_type;

		if(!pconn)
		{
			return;
		}

		std::pair<id_type, bool> id_pair = base_type::backup(pconn);
	
		if(!id_pair.second)
		{
			pconn->close();
			return;
		}

		resolver_type rslvr(pconn->get_service());
		resolver_query_type query(addr, port);
		resolver_iter_type edpt_iter = rslvr.resolve(query);
		endpoint_type edpt = *edpt_iter;

		holder_handler_type any_handler(handler);

		pconn->connect(
			yggr::make_tuple(
				edpt,
				boost::bind(
					&this_type::prv_s_handler_connect_v1
					<
						address_type,
						port_type,
						handler_type
					>,
					boost::asio::placeholders::error,
					++edpt_iter,
					id_pair.first,
					addr,
					port,
					any_handler ) ) );
	}

	template<	typename Address,
				typename Port,
				typename Handler,
				typename Time> inline
	static void s_connect(conn_ptr_type pconn,
							const Address& addr,
							const Port& port,
							const Handler& handler,
							const Time& tm_duration)
	{
		typedef Address address_type;
		typedef Port port_type;
		typedef Handler handler_type;
		typedef Time time_type;
		typedef utility::args_holder_nv<handler_type> holder_handler_type;

		if(!pconn)
		{
			return;
		}

		std::pair<id_type, bool> id_pair = base_type::backup(pconn);
	
		if(!id_pair.second)
		{
			pconn->close();
			return;
		}

		resolver_type rslvr(pconn->get_service());
		resolver_query_type query(addr, port);
		resolver_iter_type edpt_iter = rslvr.resolve(query);
		endpoint_type edpt = *edpt_iter;

		holder_handler_type any_handler(handler);

		pconn->connect(
			yggr::make_tuple(
				edpt,
				boost::bind(
					&this_type::prv_s_handler_connect_v2
					<
						address_type,
						port_type,
						handler_type,
						time_type
					>,
					boost::asio::placeholders::error,
					++edpt_iter,
					id_pair.first,
					addr,
					port,
					any_handler,
					tm_duration) ) );

		this_type::prv_s_setup_timedout_handler(
				id_pair.first,
				tm_duration,
				boost::bind(
					&this_type::prv_s_handler_connect_v2
					<
						address_type,
						port_type,
						handler_type,
						time_type
					>,
					boost::asio::placeholders::error,
					edpt_iter,
					id_pair.first,
					addr,
					port,
					any_handler,
					tm_duration) );
	}

private:

	template<	typename Address,
				typename Port,
				typename Handler>
	static void prv_s_handler_connect_v1(const boost::system::error_code& e,
											resolver_iter_type edpt_iter,
											const id_type& id,
											const Address& addr,
											const Port& port,
											const basic_args_holder_nv_type& any_handler)
	{
		typedef Address address_type;
		typedef Port port_type;
		typedef Handler handler_type;
		
		typedef utility::args_holder_nv<handler_type> holder_handler_type;

		conn_ptr_type pconn = base_type::drop_backup(id);
		if(!pconn)
		{
			return;
		}

		const handler_type* phandler = utility::args_holder_static_cast<handler_type>(any_handler);
		assert(phandler);

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
				conn_ptr_type new_pconn;
				try
				{
					new_pconn = conn_ptr_type(yggr_nothrow_new conn_type(pconn->get_init_object()));
				}
				catch(...)
				{
					new_pconn = conn_ptr_type();
				}

				this_type::s_connect(new_pconn, addr, port, (*phandler) );
			}

			return;
		}

		if(!bcontinue)
		{
			pconn->close();
			return;
		}

		for(;edpt_iter != resolver_iter_type();)
		{
			std::pair<id_type, bool> id_pair = base_type::backup(pconn);
			if(!id_pair.second)
			{
				break;
			}

			endpoint_type edpt = *edpt_iter;
			pconn->connect(
				yggr::make_tuple(
					edpt,
					boost::bind(
						&this_type::prv_s_handler_connect_v1
						<
							address_type,
							port_type,
							handler_type
						>,
						boost::asio::placeholders::error,
						++edpt_iter,
						id_pair.first,
						addr,
						port,
						reinterpret_cast<const holder_handler_type&>(any_handler) ) ) );
			return;
		}

		conn_ptr_type new_pconn;
		try
		{
			new_pconn = conn_ptr_type(yggr_nothrow_new conn_type(pconn->get_init_object()));
		}
		catch(...)
		{
			new_pconn = conn_ptr_type();
		}
		pconn->close();

		this_type::s_connect(new_pconn, addr, port, (*phandler) );

		exception::exception::throw_error(e, yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
	}

	template<	typename Address,
				typename Port,
				typename Handler,
				typename Time>
	static void prv_s_handler_connect_v2(const boost::system::error_code& e,
											resolver_iter_type edpt_iter,
											const id_type& id,
											const Address& addr,
											const Port& port,
											const basic_args_holder_nv_type& any_handler,
											const Time& tm_duration)
	{
		typedef Address address_type;
		typedef Port port_type;
		typedef Handler handler_type;
		typedef Time time_type;
		typedef utility::args_holder_nv<handler_type> holder_handler_type;

		conn_ptr_type pconn = base_type::drop_backup(id);
		if(!pconn)
		{
			return;
		}

		pconn->timer_finish();

		const handler_type* phandler = utility::args_holder_static_cast<handler_type>(any_handler);
		assert(phandler);

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
				conn_ptr_type new_pconn;
				try
				{
					new_pconn = conn_ptr_type(yggr_nothrow_new conn_type(pconn->get_init_object()));
				}
				catch(...)
				{
					new_pconn = conn_ptr_type();
				}

				this_type::s_connect(new_pconn, addr, port, (*phandler), tm_duration);
			}

			return;
		}

		if(!bcontinue)
		{
			pconn->close();
			return;
		}

		for(;edpt_iter != resolver_iter_type();)
		{
			std::pair<id_type, bool> id_pair = base_type::backup(pconn);
			if(!id_pair.second)
			{
				break;
			}

			endpoint_type edpt = *edpt_iter;
			pconn->connect(
				yggr::make_tuple(
					edpt,
					boost::bind(
						&this_type::prv_s_handler_connect_v2
						<
							address_type,
							port_type,
							handler_type,
							time_type
						>,
						boost::asio::placeholders::error,
						++edpt_iter,
						id_pair.first,
						addr,
						port,
						reinterpret_cast<const holder_handler_type&>(any_handler),
						tm_duration ) ) );

			if(!this_type::prv_s_setup_timedout_handler(
					id_pair.first,
					tm_duration,
					boost::bind(
						&this_type::prv_s_handler_connect_v2
						<
							address_type,
							port_type,
							handler_type,
							time_type
						>,
						boost::asio::placeholders::error,
						edpt_iter,
						id_pair.first,
						addr,
						port,
						any_handler,
						tm_duration) ))
			{
				break;
			}

			return;
		}

		conn_ptr_type new_pconn;
		try
		{
			new_pconn = conn_ptr_type(yggr_nothrow_new conn_type(pconn->get_init_object()));
		}
		catch(...)
		{
			new_pconn = conn_ptr_type();
		}
		pconn->close();

		this_type::s_connect(new_pconn, addr, port, (*phandler), tm_duration);

		exception::exception::throw_error(e, yggr::system_controller::system_code::E_TCP_BASE_NETWORK_SYSTEM);
	}

	template<typename Handler, typename Time> inline
	static bool prv_s_setup_timedout_handler(const id_type& id, const Time& tm, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv<handler_type> holder_handler_type;

		conn_ptr_type pconn = base_type::get(id);

		if(!pconn)
		{
			return false;
		}
		
		holder_handler_type any_handler(handler);

		pconn->expires_from_now_timer_wait(
			tm,
			boost::bind(
				&this_type::prv_s_handler_timedout<handler_type>,
				id, any_handler));

		return true;
	}

	template<typename Handler> inline
	static void prv_s_handler_timedout(const id_type& id, const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv<handler_type> holder_handler_type;
		typedef typename conn_type::deadline_timer_type deadline_timer_type;
		typedef typename deadline_timer_type::traits_type deadline_timer_traits_type;

		conn_ptr_type pconn = base_type::get(id);

		if(!pconn)
		{
			return;
		}

		if(pconn->deadline_timer().expires_at() <= deadline_timer_traits_type::now())
		{
			const handler_type* phandler = utility::args_holder_static_cast<handler_type>(any_handler);
			assert(phandler);

			if(!phandler)
			{
				return;
			}

			pconn->shut_down_all();
			pconn->timer_finish();
			
			boost::system::error_code ec(boost::asio::error::timed_out,
											boost::asio::error::get_system_category());
			(*phandler)(ec);
		}
	}

};

} // namespace session_helper
} // namespace network
} // namespace yggr

#endif //__YGGR_NETEORK_SESSION_HELPER_BASIC_TCP_CONNECT_HANDLER_HPP__
