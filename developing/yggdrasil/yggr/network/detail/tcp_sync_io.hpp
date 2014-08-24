//tcp_sync_io.hpp

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

/*
* now sessoin framework not support tcp_sync_io
*/

#ifndef __YGGR_NETWORK_DETAIL_TCP_SYNC_IO_HPP__
#define __YGGR_NETWORK_DETAIL_TCP_SYNC_IO_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <yggr/move/move.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/base/exception.hpp>
#include <yggr/base/system_code.hpp>
#include <yggr/base/error_make.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/safe_container/safe_buffered_object.hpp>

#include <yggr/network/packets_support/packets_queue.hpp>


namespace yggr
{
namespace network
{
namespace detail
{

template<typename Linker, 
			typename OPacket, 
			typename IPacket>
class tcp_sync_io
	: public Linker
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
	typedef typename linker_type::protocol_tag_type protocol_tag_type;

	typedef opacket_type send_data_type;
	typedef ipacket_type recv_data_type;

private:
	BOOST_MPL_ASSERT((boost::is_same<service_type, link_init_type>));

	typedef yggr::network::type_traits::tag_pak_static tag_static_type;
	typedef yggr::network::type_traits::tag_pak_dynamic tag_dynamic_type;

private:

	typedef typename recv_data_type::buf_type recv_buf_type;

	class wrap_recv_buf : public recv_buf_type
	{
	public:
		inline void splice_to(const wrap_recv_buf& right)
		{
		}

		inline operator recv_buf_type&(void)
		{
			return *this;
		}

		inline operator const recv_buf_type&(void) const
		{
			return *this;
		}
	};

	typedef wrap_recv_buf wrap_recv_buf_type;
	typedef safe_container::safe_buffered_object<wrap_recv_buf_type> recv_buf_queue_type;

	typedef boost::shared_ptr<recv_data_type> recv_data_ptr_type;
	typedef typename send_data_type::buf_type send_data_buf_type;
	typedef typename recv_data_type::buf_type recv_data_buf_type;

	BOOST_MPL_ASSERT((boost::is_same<send_data_buf_type, recv_data_buf_type>));

public:
	typedef send_data_buf_type data_buf_type;

private:
	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("tcp_sync_io")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_17)
			ERROR_CODE_DEF(E_invalid_recv_size)
			ERROR_CODE_DEF(E_invalid_recv_head)
			ERROR_CODE_DEF(E_invalid_recv_data)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_17)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_recv_size, "invalid recv size")
			ERROR_CODE_MSG(E_invalid_recv_head, "invalid recv head")
			ERROR_CODE_MSG(E_invalid_recv_data, "invalid recv data")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------

private:
	typedef tcp_sync_io this_type;

	typedef yggr::network::packets_support::opackets_queue<this_type> out_queue_type;
	friend class out_queue_type;

protected:
	explicit tcp_sync_io(link_init_type& link_init)
		: linker_type(link_init)//, _strand(link_init)
	{
		assert(typeid(send_data_buf_type).name() == typeid(recv_data_buf_type).name());
	}

	~tcp_sync_io(void)
	{
	}

protected:

	template<typename Pak, typename Handler>
	void send_packet(Pak& pak, const Handler& handler)
	{
		_out_queue.send_packet(*this, pak, handler);
	}

	void next_send(void)
	{
		_out_queue.next_send(*this);
	}

	template<typename Pak, typename Tuple_Handler>
	void recv_packet(const Tuple_Handler& handler)
	{
		typedef Pak pak_type;
		typedef typename pak_type::packet_tag_type tag_type;
		recv_packet<pak_type>(handler, (tag_type*)0);
	}

	void close(void)
	{
		_out_queue.clear();
		linker_type::close();
	}

private:

	template<typename Pak, typename Handler>
	void exec_send_packet(Pak& pak, const Handler& handler)
	{
		typedef Pak pak_type;
		typedef typename pak_type::packet_tag_type tag_type;
		send_packet(pak, handler, (tag_type*)0);
	}

	template<typename Pak, typename Handler>
	void send_packet(Pak& pak, const Handler& handler, const tag_static_type*)
	{
		
		boost::array<boost::asio::const_buffer, 1> bufs;
		
		if(pak.data_buf())
		{
			bufs[0] = boost::asio::const_buffer(reinterpret_cast<const void*>(pak.data_buf()),
												static_cast<size_t>(pak.size()));
		}

		boost::system::error_code e;
		boost::asio::wirte(linker_type::get_link(), bufs, boost::asio::transfer_all(), e);
		handler(e);
	}

	template<typename Pak, typename Handler>
	void send_packet(Pak& pak, const Handler& handler, const tag_dynamic_type*)
	{
		typedef Pak pak_type;
		typedef typename pak_type::size_container_type size_container_type;
		typedef typename pak_type::net_info_type net_info_type;

		size_container_type size_cont = yggr::network::hton(size_container_type(pak.net_info_size(), pak.data_size()));
		const net_info_type& net_info = pak.net_info();

		boost::array<boost::asio::const_buffer, 3> bufs;

		if(!(size_cont.buf() && net_info.buf() && pak.data_buf()))
		{
			bufs[0] = boost::asio::const_buffer(reinterpret_cast<const void*>(size_cont.buf()), 
												static_cast<std::size_t>(pak_type::s_size_container_size()));

			bufs[1] = boost::asio::const_buffer(reinterpret_cast<const void*>(net_info.buf()), 
													static_cast<std::size_t>(net_info.buf_size()));

			bufs[2] = boost::asio::const_buffer(reinterpret_cast<const void*>(pak.data_buf()), 
													static_cast<std::size_t>(pak.data_size()));
		}

		boost::system::error_code e;
		boost::asio::write(linker_type::get_link(), bufs, boost::asio::transfer_all(), e);
		handler(e);
	}

	template<typename Pak, typename Tuple_Handler>
	void recv_packet(const Tuple_Handler& handler, const tag_static_type*)
	{
		typedef Pak pak_type;
		typedef typename pak_type::size_type size_type;
		typedef Tuple_Handler handler_type;

		typedef typename recv_buf_queue_type::obj_ptr_type recv_buf_ptr_type;

		recv_buf_ptr_type precv_buf = _s_recv_buf_queue.drop();

		if(!precv_buf)
		{
			boost::system::error_code err(boost::asio::error::no_buffer_space);
			pak_type pak;
			boost::get<0>(handler)(err, boost::ref(pak));
			return;
		}

		assert(!pak_type::s_size_size());
		std::size_t buf_size = pak_type::s_buf_size();
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

		boost::system::error_code e;
		std::size_t read_size = boost::asio::read(linker_type::get_link(), bufs, boost::asio::transfer_all(), e);
		handler_recv_data_of_static<pak_type>(e, buf_size, read_size, precv_buf, handler);
	}

	template<typename Pak, typename Tuple_Handler>
	void recv_packet(const Tuple_Handler& handler, const tag_dynamic_type*)
	{
		typedef Pak pak_type;
		typedef typename pak_type::size_type size_type;


		typedef Tuple_Handler handler_type;

		typedef typename recv_buf_queue_type::obj_ptr_type recv_buf_ptr_type;

		recv_buf_ptr_type precv_buf = _s_recv_buf_queue.drop();

		if(!precv_buf)
		{
			boost::system::error_code err(boost::asio::error::no_buffer_space);
			pak_type pak;
			boost::get<0>(handler)(err, boost::ref(pak));
			return;
		}

		assert(pak_type::s_size_container_size());
		if(!pak_type::s_size_container_size())
		{
			this_type::_s_recv_buf_queue.back(precv_buf);
			boost::system::error_code err(boost::asio::error::invalid_argument);
			pak_type pak;
			boost::get<0>(handler)(err, boost::ref(pak));
			return;
		}

		size_type size_cont_buf_size = pak_type::s_size_container_size();
		precv_buf->resize(size_cont_buf_size);

		boost::array<boost::asio::mutable_buffer, 1> bufs;

		bufs[0] = boost::asio::mutable_buffer(reinterpret_cast<void*>(&((*precv_buf)[0])), size_cont_buf_size);

		boost::system::error_code e;
		std::size_t read_size = boost::asio::read(linker_type::get_link(), bufs, boost::asio::transfer_all(), e);
		handler_recv_size<pak_type>(e, size_cont_buf_size, read_size, precv_buf, handler);
	}

	template<typename Pak, typename Recv_Buf_Ptr, typename Tuple_Handler>
	void handler_recv_size(const boost::system::error_code& e, 
								std::size_t need_bytes_trans, 
								std::size_t real_bytes_trans,
								Recv_Buf_Ptr precv_buf,
								const Tuple_Handler& handler)
	{
		typedef Pak pak_type;
		typedef typename pak_type::size_type size_type;
		typedef typename pak_type::size_container_type size_container_type;
		typedef Recv_Buf_Ptr recv_buf_ptr_type;

		typedef Tuple_Handler handler_type;

		if(!precv_buf)
		{
			boost::system::error_code err(boost::asio::error::no_buffer_space);
			pak_type pak;
			boost::get<0>(handler)(err, boost::ref(pak));
			return;
		}

		if(!e)
		{
			if(need_bytes_trans != real_bytes_trans)
			{
				this_type::_s_recv_buf_queue.back(precv_buf);
				exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_recv_size));
				boost::get<1>(handler)();
				return;
			}

			
			size_container_type size_cont(yggr::network::ntoh(size_container_type(*precv_buf)));
			if(!size_cont.effective())
			{
				boost::system::error_code err(boost::asio::error::invalid_argument);
				pak_type pak;
				boost::get<0>(handler)(err, boost::ref(pak));
				return;
			}

			std::size_t count_buf_size = size_cont.count_size();
			std::size_t header_buf_size = size_cont.info_size();
			std::size_t data_buf_size = size_cont.data_size();

			if(data_buf_size > pak_type::s_limit_buf_size())
			{
				boost::system::error_code err(boost::asio::error::message_size);
				pak_type pak;
				boost::get<0>(handler)(err, boost::ref(pak));
				return;
			}

			precv_buf->resize(count_buf_size);

			if(!data_buf_size || count_buf_size != precv_buf->size())
			{
				this_type::_s_recv_buf_queue.back(precv_buf);
				boost::system::error_code err(boost::asio::error::no_buffer_space);
				pak_type pak;
				boost::get<0>(handler)(err, boost::ref(pak));
				return;
			}

			boost::array<boost::asio::mutable_buffer, 2> bufs;

			bufs[0] = boost::asio::mutable_buffer(reinterpret_cast<void*>(&((*precv_buf)[0])), 
													header_buf_size);

			bufs[1] = boost::asio::mutable_buffer(reinterpret_cast<void*>((&((*precv_buf)[0])) + header_buf_size), 
													data_buf_size);

			boost::system::error_code e;
			std::size_t read_size = boost::asio::read(linker_type::get_link(), bufs, boost::asio::transfer_all(), e);
			handler_recv_data_of_dynamic<pak_type>(e, count_buf_size, read_size, size_cont, precv_buf, handler);

			return;
		}

		pak_type pak;
		boost::get<0>(handler)(e, boost::ref(pak));
	}

	template<typename Pak, typename Recv_Buf_Ptr, typename Tuple_Handler>
	void handler_recv_data_of_dynamic(const boost::system::error_code& e,
										std::size_t need_bytes_trans, 
										std::size_t real_bytes_trans,
										const typename Pak::size_container_type& size_cont,
										Recv_Buf_Ptr precv_buf, 
										const Tuple_Handler& handler)
	{
		typedef Pak pak_type;
		typedef typename pak_type::size_type size_type;
		typedef typename pak_type::net_info_type net_info_type;
		typedef Recv_Buf_Ptr recv_buf_ptr_type;
		typedef Tuple_Handler handler_type;

		if(!precv_buf)
		{
			boost::system::error_code err(boost::asio::error::no_buffer_space);
			pak_type pak;
			boost::get<0>(handler)(err, boost::ref(pak));
			return;
		}

		std::size_t info_buf_size = size_cont.info_size();
		std::size_t data_buf_size = size_cont.data_size();

		if(need_bytes_trans != real_bytes_trans)
		{
			this_type::_s_recv_buf_queue.back(precv_buf);
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_recv_data, "dynamic_packet"));
			boost::get<1>(handler)();
			return;
		}

		net_info_type net_info;
		net_info.assign(&(*precv_buf)[0], &(*precv_buf)[0] + info_buf_size);
		pak_type pak(boost::move::move_helper::forced_move(net_info), 
						&(*precv_buf)[0] + info_buf_size, 
						&(*precv_buf)[0] + need_bytes_trans);
		this_type::_s_recv_buf_queue.back(precv_buf);
		boost::get<0>(handler)(e, boost::ref(pak));
	}

	template<typename Pak, typename Recv_Buf_Ptr, typename Tuple_Handler>
	void handler_recv_data_of_static(const boost::system::error_code& e,
										std::size_t need_bytes_trans, 
										std::size_t real_bytes_trans,
										Recv_Buf_Ptr precv_buf, 
										const Tuple_Handler& handler)
	{
		typedef Pak pak_type;
		typedef typename pak_type::size_type size_type;
		typedef typename pak_type::net_info_type net_info_type;
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
			this_type::_s_recv_buf_queue.back(precv_buf);
			exception::exception::throw_error(error_type(error_type::E_invalid_recv_data, "static_packet"));
			boost::get<1>(handler)();
			return;
		}

		
		pak_type pak(*precv_buf);
		this_type::_s_recv_buf_queue.back(precv_buf);
		boost::get<0>(handler)(e, boost::ref(pak));
	}


private:
	out_queue_type _out_queue;
	static recv_buf_queue_type _s_recv_buf_queue;
};

template<typename Linker, typename OPacket, typename IPacket>
    typename tcp_sync_io<Linker, OPacket, IPacket>::recv_buf_queue_type
        tcp_sync_io<Linker, OPacket, IPacket>::_s_recv_buf_queue;

} // namespcae detail
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_DETAIL_TCP_SYNC_IO_HPP__