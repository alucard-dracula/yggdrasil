//udp_async_strand_io.hpp

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

#ifndef __YGGR_NETWORK_IO_UDP_ASYNC_STRAND_IO_HPP__
#define __YGGR_NETWORK_IO_UDP_ASYNC_STRAND_IO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/friend.hpp>
#include <yggr/move/move.hpp>
#include <yggr/container/vector.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/udp_packet_wrap.hpp>
#include <yggr/network/packets_support/packets_queue.hpp>

#include <boost/ref.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace network
{
namespace io
{

template<typename Linker,
			typename OPacket,
			typename IPacket>
class udp_async_strand_io
	: public Linker
{
public:

	typedef Linker linker_type;
	typedef OPacket opacket_type;
	typedef IPacket ipacket_type;

	typedef typename linker_type::protocol_tag_type protocol_tag_type;
	typedef typename linker_type::service_type service_type;
	typedef typename linker_type::protocol_type protocol_type;
	typedef typename linker_type::socket_type socket_type;
	typedef typename linker_type::endpoint_type endpoint_type;
	typedef typename linker_type::link_address_type link_address_type;
	typedef typename linker_type::deadline_timer_type deadline_timer_type;

	typedef typename linker_type::link_init_type link_init_type;
	typedef typename linker_type::link_type link_type;

	typedef opacket_type send_data_type;
	typedef ipacket_type recv_data_type;

private:
	BOOST_MPL_ASSERT((boost::is_same<socket_type, link_init_type>));

	typedef yggr::network::type_traits::tag_pak_static tag_static_type;
	typedef yggr::network::type_traits::tag_pak_dynamic tag_dynamic_type;

private:
	typedef typename service_type::strand strand_type;
	typedef ::yggr::shared_ptr<recv_data_type> recv_data_ptr_type;
	typedef typename send_data_type::buf_type send_data_buf_type;
	typedef typename recv_data_type::buf_type recv_data_buf_type;

	BOOST_MPL_ASSERT((boost::is_same<send_data_buf_type, recv_data_buf_type>));

public:
	typedef send_data_buf_type data_buf_type;

private:
	typedef udp_packet_wrap<data_buf_type> send_wrap_type;

private:
	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("udp_async_strand_io")
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
	typedef udp_async_strand_io this_type;

private:
	typedef yggr::network::packets_support::opackets_queue<this_type> out_queue_type;
	
	YGGR_PP_FRIEND_TYPENAME(out_queue_type);

protected:

	explicit udp_async_strand_io(link_init_type& link_init)
		: linker_type(link_init), 
			_strand(
#if (BOOST_ASIO_VERSION < 101400) 
				link_init.get_io_service()
#else
				static_cast<service_type&>(link_init.get_executor().context())
#endif //BOOST_ASIO_VERSION < 101400) 
			)
	{
	}
	
	template<typename DHandler>
	udp_async_strand_io(link_init_type& link_init, const DHandler& d)
		: linker_type(link_init, d),
			_strand(
#if (BOOST_ASIO_VERSION < 101400) 
				link_init.get_io_service()
#else
				static_cast<service_type&>(link_init.get_executor().context())
#endif //BOOST_ASIO_VERSION < 101400) 
				)
	{
	}

	udp_async_strand_io(link_init_type& link_init, const endpoint_type& ep)
		: linker_type(link_init, ep), 
			_strand(
#if (BOOST_ASIO_VERSION < 101400) 
				link_init.get_io_service()
#else
				static_cast<service_type&>(link_init.get_executor().context())
#endif //BOOST_ASIO_VERSION < 101400) 
				)
	{
	}

	template<typename DHandler>
	udp_async_strand_io(link_init_type& link_init, const endpoint_type& ep, const DHandler& d)
		: linker_type(link_init, ep, d),
			_strand(
#if (BOOST_ASIO_VERSION < 101400) 
				link_init.get_io_service()
#else
				static_cast<service_type&>(link_init.get_executor().context())
#endif //BOOST_ASIO_VERSION < 101400) 
				)
	{
	}

	~udp_async_strand_io(void)
	{
	}

protected:

	template<typename Pak, typename Handler> inline
	void send_packet(Pak& pak, const Handler& handler)
	{
		_out_queue.send_packet(*this, pak, handler);
	}

	inline void next_send(void)
	{
		_out_queue.next_send(*this);
	}

	template<typename Pak, typename Tuple_Handler> inline
	void recv_packet(const Tuple_Handler& handler)
	{
	}

	inline void close(void)
	{
		_out_queue.clear();
		//linker_type::close(); //udp don't use this
	}

private:

	template<typename Pak, typename Handler>
	void exec_send_packet(Pak& pak, const Handler& handler)
	{
		typedef ::yggr::vector<data_buf_type> data_bufs_type;
		typedef ::yggr::vector<boost::asio::const_buffer> bufs_type;

		send_wrap_type wrap(pak);
		size_type isize = wrap.count();
		data_bufs_type data_bufs(isize);
		bufs_type bufs(isize);

		for(size_type i = 0; i != isize; ++i)
		{
			wrap.get_data_buf(i, data_bufs[i]);
			if(data_bufs[i].empty())
			{
			    {
			        data_bufs_type tmp;
                    data_bufs.swap(tmp);
			    }
			    {
			        bufs_type tmp;
                    bufs.swap(tmp);
			    }
				break;
			}
			bufs[i] = boost::asio::buffer(boost::addressof(data_bufs[i][0]), data_bufs[i].size());
		}

		linker_type::get_link()
			.socket()
			.async_send_to(bufs, linker_type::get_link().remote_endpoint(), _strand.wrap(handler));
	}


private:
	strand_type _strand;
	out_queue_type _out_queue;
};

} // namespace io
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_IO_UDP_ASYNC_STRAND_IO_HPP__
