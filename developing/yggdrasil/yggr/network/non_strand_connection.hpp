//non_strand_connection.hpp

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

#ifndef __YGGR_NETWORK_NON_STRAND_CONNECTION_HPP__
#define __YGGR_NETWORK_NON_STRAND_CONNECTION_HPP__

#ifndef YGGR_CONNECT_INCLUDE
#	error "error include non_strand_connection.hpp please include connection.hpp"
#endif // YGGR_CONNECT_INCLUDE

#include <cassert>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <yggr/base/exception.hpp>
#include <yggr/base/system_code.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <yggr/safe_container/safe_buffered_object.hpp>
#include <yggr/base/error_make.hpp>

namespace yggr
{
namespace network
{

template<typename Linker, typename OPacket, typename IPacket>
class connection <Linker, OPacket, IPacket, false, false> :  public boost::enable_shared_from_this<connection<Linker, OPacket, IPacket, false, false> >,
															public Linker//, private nonable::noncopyable
{
public:

	typedef Linker linker_type;
	typedef OPacket opacket_type;
	typedef IPacket ipacket_type;

	typedef typename linker_type::service_type service_type;
	typedef typename linker_type::socket_type socket_type;
	typedef typename linker_type::link_init_type link_init_type;
	typedef typename linker_type::link_type link_type;
	typedef typename linker_type::link_address_type link_address_type;
	typedef typename linker_type::endpoint_type endpoint_type;
	typedef typename linker_type::protocol_type protocol_type;

	typedef opacket_type send_data_type;
	typedef ipacket_type recv_data_type;

	typedef typename send_data_type::base_type send_base_type;

private:

	typedef typename recv_data_type::buf_type recv_buf_type;

	class wrap_recv_buf : public recv_buf_type
	{
	public:
		void splice_to(const wrap_recv_buf& right)
		{
		}

		operator recv_buf_type&(void)
		{
			return *this;
		}

		operator const recv_buf_type&(void) const
		{
			return *this;
		}
	};

	typedef wrap_recv_buf wrap_recv_buf_type;
	typedef safe_container::safe_buffered_object<wrap_recv_buf_type> recv_buf_queue_type;

	typedef boost::shared_ptr<recv_data_type> recv_data_ptr_type;
	typedef typename send_data_type::buf_type send_data_buf_type;
	typedef typename recv_data_type::buf_type recv_data_buf_type;


public:
	typedef send_data_buf_type data_buf_type;

private:

	//class error S----------------------------------------------------------
	ERROR_MAKER_BEGIN("non_strand_connection")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_17)
			ERROR_CODE_DEF(E_invalid_recv_head)
			ERROR_CODE_DEF(E_invalid_recv_data)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_17)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_recv_head, "invalid recv head")
			ERROR_CODE_MSG(E_invalid_recv_data, "invalid recv data")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------

private:
	typedef connection this_type;

public:
	explicit connection(link_init_type& link_init)
		: linker_type(link_init)
	{
		assert(typeid(send_data_buf_type).name() == typeid(recv_data_buf_type).name());
	}

	~connection(void)
	{
	}

	//-----------------send s----------------------------------------

	template<typename User_ID, typename Data, typename Handler>
	void send_packet(const User_ID& uid, const Data& data, const Handler& handler)
	{
		send_data_type pak(data, uid, Data::s_data_id(), Data::s_data_ver(), false);
		prv_send_packet(pak, handler);
	}

	template<typename Handler>
	void send_packet(const send_base_type& base_pak, const Handler& handler)
	{
		send_data_type pak(base_pak);
		prv_send_packet(pak, handler);
	}


	template<typename Handler>
	void send_packet(const send_data_type& pak, const Handler& handler)
	{
		prv_secnd_packet(pak, handler);
	}

	template<typename Pak, typename Handler>
	void send_other_packet(const Pak& pak, const Handler& handler)
	{
		prv_send_packet(pak, handler);
	}

	//------------------send e---------------------------------------
	//------------------recv s---------------------------------------

template<typename Pak, typename Tuple_Handler>
	void recv_other_packet(const Tuple_Handler& handler)
	{
		typedef Pak pak_type;
		prv_recv_packet<pak_type>(handler);
	}

	template<typename Tuple_Handler>
	void recv_packet(const Tuple_Handler& handler)
	{
		typedef recv_data_type pak_type;
		prv_recv_packet<pak_type>(handler);
	}
//------------------recv e---------------------------------------

private:

	template<typename Pak, typename Handler>
	void prv_send_packet(const Pak& pak, const Handler& handler)
	{
		pak.hton();

		if(pak.header_size())
		{
			boost::array<boost::asio::const_buffer, 2> bufs;
			bufs[0] = boost::asio::const_buffer(reinterpret_cast<const void*>(pak.header_buf()), pak.header_size());
			bufs[1] = boost::asio::const_buffer(reinterpret_cast<const void*>(pak.buf()), static_cast<size_t>(pak.buf_size()));
			boost::asio::async_write(linker_type::get_link(), bufs, handler);
		}
		else
		{
			boost::array<boost::asio::const_buffer, 2> bufs;
			bufs[0] = boost::asio::const_buffer(reinterpret_cast<const void*>(pak.buf()), static_cast<size_t>(pak.buf_size()));
			boost::asio::async_write(linker_type::get_link(), bufs, handler);
		}
	}

	template<typename Pak, typename Tuple_Handler>
	void prv_recv_packet(const Tuple_Handler& handler)
	{
		typedef Pak pak_type;
		typedef Tuple_Handler handler_type;

		typedef typename pak_type::buf_type pak_buf_type;
		typedef typename recv_buf_queue_type::obj_ptr_type recv_buf_ptr_type;

		recv_buf_ptr_type precv_buf = _s_recv_buf_queue.drop();

		if(!precv_buf)
		{
			boost::system::error_code err(boost::asio::error::no_buffer_space);
			pak_type pak;
			boost::get<0>(handler)(err, boost::ref(pak));
			return;
		}

		if(!(pak_type::s_header_size()))
		{
			size_type buf_size = pak_type::s_buf_size();

			if(!buf_size)
			{
				this_type::_s_recv_buf_queue.back(precv_buf);
				boost::system::error_code err(boost::asio::error::no_buffer_space);
				pak_type pak;
				boost::get<0>(handler)(err, boost::ref(pak));
				return;
			}

			precv_buf->resize(buf_size);
			boost::array<boost::asio::mutable_buffer, 1> bufs;
			bufs[0] = boost::asio::mutable_buffer(reinterpret_cast<void*>(&((*precv_buf)[0])), buf_size);

			boost::asio::async_read(linker_type::get_link(), bufs,
										boost::bind(&this_type::template handler_recv_data<
																							pak_type,
																							recv_buf_ptr_type,
																							handler_type
																						>,
														this_type::this_type::shared_from_this(),
														boost::asio::placeholders::error, buf_size,
														boost::asio::placeholders::bytes_transferred, precv_buf,
														handler
													)
									);
			return;
		}

		size_type header_buf_size = pak_type::s_header_size();
		precv_buf->resize(header_buf_size);

		boost::array<boost::asio::mutable_buffer, 1> bufs;

		bufs[0] = boost::asio::mutable_buffer(reinterpret_cast<void*>(&((*precv_buf)[0])), header_buf_size);

		boost::asio::async_read(linker_type::get_link(), bufs,
									boost::bind(&this_type::template handler_recv_header<
																							pak_type,
																							recv_buf_ptr_type,
																							handler_type
																						>,
													this_type::shared_from_this(),
													boost::asio::placeholders::error,
													header_buf_size,
													boost::asio::placeholders::bytes_transferred,
													precv_buf,
													handler
												)
								);

	}

	template<typename Pak, typename Recv_Buf_Ptr, typename Tuple_Handler>
	void handler_recv_header(const boost::system::error_code& e, std::size_t need_bytes_trans, std::size_t real_bytes_trans,
								Recv_Buf_Ptr precv_buf, const Tuple_Handler& handler)
	{
		typedef Pak pak_type;
		typedef typename pak_type::header_type header_type;
		typedef Recv_Buf_Ptr recv_buf_ptr_type;

		typedef Tuple_Handler handler_type;

		if(!precv_buf)
		{
			boost::system::error_code err(boost::asio::error::no_buffer_space);
			pak_type pak;
			boost::get<0>(handler)(err, boost::ref(pak));
		}

		if(!e)
		{
			if(need_bytes_trans != real_bytes_trans)
			{
				boost::array<boost::asio::mutable_buffer, 1> bufs;
				bufs[0] = boost::asio::mutable_buffer(reinterpret_cast<void*>(&((*precv_buf)[0])), need_bytes_trans);
				boost::asio::async_read(linker_type::get_link(), bufs,
											boost::bind(&this_type::template handler_recv_header<
																									pak_type,
																									recv_buf_ptr_type,
																									handler_type
																								>,
															this_type::shared_from_this(),
															boost::asio::placeholders::error,
															need_bytes_trans,
															boost::asio::placeholders::bytes_transferred,
															precv_buf,
															handler
														)
								);
				return;
			}

			header_type data_buf_size = pak_type::s_serialize_header(*precv_buf);

			if(data_buf_size > pak_type::s_limit_buf_size())
			{
				boost::system::error_code err(boost::asio::error::message_size);
				pak_type pak;
				boost::get<0>(handler)(err, boost::ref(pak));
				return;
			}

			precv_buf->resize(data_buf_size);

			if(!data_buf_size || data_buf_size != precv_buf->size())
			{
				this_type::_s_recv_buf_queue.back(precv_buf);
				boost::system::error_code err(boost::asio::error::no_buffer_space);
				pak_type pak;
				boost::get<0>(handler)(err, boost::ref(pak));
				return;
			}

			boost::array<boost::asio::mutable_buffer, 1> bufs;
			bufs[0] = boost::asio::mutable_buffer(reinterpret_cast<void*>(&((*precv_buf)[0])), data_buf_size);

			boost::asio::async_read(linker_type::get_link(), bufs,
										boost::bind(&this_type::template handler_recv_data<
																							pak_type,
																							recv_buf_ptr_type,
																							handler_type
																						>,
														this_type::shared_from_this(),
														boost::asio::placeholders::error,
														data_buf_size,
														boost::asio::placeholders::bytes_transferred,
														precv_buf,
														handler
													)
									);

			return;
		}

		this_type::_s_recv_buf_queue.back(precv_buf);
		pak_type pak;
		boost::get<0>(handler)(e, boost::ref(pak));
	}

	template<typename Pak, typename Recv_Buf_Ptr, typename Tuple_Handler>
	void handler_recv_data(const boost::system::error_code& e, std::size_t need_bytes_trans, std::size_t real_bytes_trans,
							Recv_Buf_Ptr precv_buf, const Tuple_Handler& handler)
	{
		typedef Pak pak_type;
		typedef Recv_Buf_Ptr recv_buf_ptr_type;
		typedef Tuple_Handler handler_type;

		if(!precv_buf)
		{
			boost::system::error_code err(boost::asio::error::no_buffer_space);
			pak_type pak;
			boost::get<0>(handler)(err, boost::ref(pak));
			return;
		}

		if(need_bytes_trans != real_bytes_trans)
		{
			if(!e)
			{
				boost::array<boost::asio::mutable_buffer, 1> bufs;
				bufs[0] = boost::asio::mutable_buffer(reinterpret_cast<void*>(&((*precv_buf)[0])), need_bytes_trans);

				boost::asio::async_read(linker_type::get_link(), bufs,
										boost::bind(&this_type::template handler_recv_data<
																							pak_type,
																							recv_buf_ptr_type,
																							handler_type
																						>,
														this_type::shared_from_this(),
														boost::asio::placeholders::error,
														need_bytes_trans,
														boost::asio::placeholders::bytes_transferred,
														precv_buf,
														handler
													)
										);
			}
			else
			{
				this_type::_s_recv_buf_queue.back(precv_buf);
				exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_recv_data));
				boost::get<1>(handler)();
			}
			return;
		}

		pak_type pak(*precv_buf);
		//pak_type pak;
		//pak.copy_from_buf(*precv_buf);
		this_type::_s_recv_buf_queue.back(precv_buf);
		boost::get<0>(handler)(e, boost::ref(pak));
	}

private:
	static recv_buf_queue_type _s_recv_buf_queue;
};

template<typename Linker, typename OPacket, typename IPacket>
    typename connection<Linker, OPacket, IPacket, false, false>::recv_buf_queue_type
        connection<Linker, OPacket, IPacket, false, false>::_s_recv_buf_queue;

} //network
} //yggr
#endif //__YGGR_NETWORK_NON_STRAND_CONNECTION_HPP__
