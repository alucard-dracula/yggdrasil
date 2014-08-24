//packets_queue.hpp

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

#ifndef __YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_QUEUE_HPP__
#define __YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_QUEUE_HPP__

#include <cassert>

#include <yggr/move/move.hpp>
#include <boost/atomic.hpp>

#include <yggr/base/interface_ptr.hpp>
#include <yggr/safe_container/safe_queue.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>


namespace yggr
{
namespace network
{
namespace packets_support
{

template<typename Conn>
class opackets_queue
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Conn conn_type;
	typedef boost::atomic<bool> gate_type;

private:

	class base_opacket_container
	{
	public:
		virtual ~base_opacket_container(void)
		{
		}

		virtual bool empty(void) const = 0;
		virtual void send_packet(conn_type& conn) = 0;
	};

	template<typename Pak, typename Handler>
	class opacket_container
		: public base_opacket_container
	{
	public:

		typedef Pak pak_type;
		typedef Handler handler_type;
	public:
		opacket_container(pak_type& pak, const handler_type& handler)
			: _pak(boost::move(pak)), _handler(handler)
		{
		}

		virtual ~opacket_container(void)
		{
		}

		virtual bool empty(void) const
		{
			return _pak.empty();
		}

		virtual void send_packet(conn_type& conn)
		{
			conn.exec_send_packet(_pak, _handler);
		}

	private:
		pak_type _pak;
		handler_type _handler;
	};

	typedef interface_ptr<base_opacket_container> base_opacket_container_ptr_type;
	typedef safe_container::safe_queue<base_opacket_container_ptr_type> queue_type;

private:
	typedef opackets_queue this_type;

public:

	opackets_queue(void)
		:_gate(true)
	{
	}

	~opackets_queue(void)
	{
	}

	void next_send(conn_type& conn)
	{
		base_opacket_container_ptr_type ptr(_queue.use_handler(
													boost::bind(&this_type::handler_next_send,
													this, _1)));

		if(ptr)
		{
			assert(!ptr->empty());
			ptr->send_packet(conn);
		}
	}

	template<typename Pak, typename Handler>
	void send_packet(conn_type& conn, Pak& pak, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef Pak pak_type;
		typedef opacket_container<pak_type, handler_type> now_container_type;

		if(_queue.use_handler(
					boost::bind(
									&this_type::handler_send_packet<pak_type, handler_type>,
									this, _1, boost::ref(pak), boost::cref(handler)
								)
							))
		{
			conn.exec_send_packet(pak, handler);
		}
	}

	void clear(void)
	{
		_queue.use_handler(boost::bind(&this_type::handler_clear, this, _1));
	}

private: 
	template<typename Pak, typename Handler>
	bool handler_send_packet(typename queue_type::base_type& base, 
								Pak& pak, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef Pak pak_type;
		typedef opacket_container<pak_type, handler_type> now_container_type;

		bool bgate = true;

		if(_gate.compare_exchange_strong(bgate, false))
		{
			assert(base.empty());
			return true;
		}
		else
		{
			base.push(base_opacket_container_ptr_type(new now_container_type(pak, handler)));
			assert(!base.empty());
			return false;
		}
	}

	base_opacket_container_ptr_type handler_next_send(typename queue_type::base_type& base)
	{
		if(base.empty())
		{
			bool bgate = false;
			//assert(_gate.load() == bgate);
			_gate.compare_exchange_strong(bgate, true); //gate state is close or open open_gate false->true;
			return base_opacket_container_ptr_type();
		}

		base_opacket_container_ptr_type ptr(base.front());
		base.pop();
		return ptr;
	}

	void handler_clear(typename queue_type::base_type& base)
	{
		for(;!base.empty();)
		{
			base.pop();
		}
		_gate.store(true);
	}

private:
	gate_type _gate;
	queue_type _queue;

};

} // namespace packets_support
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_QUEUE_HPP__