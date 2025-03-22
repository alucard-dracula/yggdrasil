//packets_queue.hpp

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

#ifndef __YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_QUEUE_HPP__
#define __YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_QUEUE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/args_holder_nv.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/safe_container/safe_deque.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <boost/atomic.hpp>

#include <cassert>

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
	private:
		typedef base_opacket_container this_type;

	public:
		virtual ~base_opacket_container(void)
		{
		}

	public:
		virtual bool empty(void) const = 0;
		virtual void send_packet(conn_type& conn) const = 0;

	};

	template<typename Pak, typename Handler>
	class opacket_container
		: public base_opacket_container
	{
	public:
		typedef Pak pak_type;
		typedef Handler handler_type;

	private:
		typedef base_opacket_container base_type;
		typedef opacket_container this_type;
	

	public:
		opacket_container(pak_type& pak, const handler_type& handler)
			: _pak(boost::move(pak)), _handler(handler)
		{
		}

		~opacket_container(void)
		{
		}

	public:
		virtual bool empty(void) const
		{
			return _pak.empty();
		}

		virtual void send_packet(conn_type& conn) const
		{
			conn.exec_send_packet(_pak, _handler);
		}

	private:
		mutable pak_type _pak;
		handler_type _handler;
	};

	typedef shared_ptr<base_opacket_container> base_opacket_container_ptr_type;
	typedef safe_container::safe_deque<base_opacket_container_ptr_type> queue_type;

private:
	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

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

public:
	template<typename Pak, typename Handler> inline
	void send_packet(conn_type& conn, Pak& pak, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef Pak pak_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;

		handler_holder_type holder(handler);

		if(_queue.using_handler(
				boost::bind(
					&this_type::prv_handler_send_packet<pak_type, handler_type>,
					this, _1, boost::ref(pak), boost::cref<basic_args_holder_nv_type>(holder) ) ) )
		{
			conn.exec_send_packet(pak, handler);
		}
	}

	inline void next_send(conn_type& conn)
	{
		base_opacket_container_ptr_type ptr(_queue.using_handler(
												boost::bind(
													&this_type::prv_handler_next_send,
													this, _1)));

		if(ptr)
		{
			assert(!ptr->empty());
			ptr->send_packet(conn);
		}
	}

	inline void clear(void)
	{
		typedef typename queue_type::base_type base_queue_type;
		base_queue_type tmp;
		_queue.using_handler(boost::bind(&this_type::prv_handler_clear, this, _1, boost::ref(tmp)));
	}

private: 
	template<typename Pak, typename Handler> inline
	bool prv_handler_send_packet(typename queue_type::base_type& base, 
									Pak& pak, 
									const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;
		typedef Pak pak_type;
		typedef opacket_container<pak_type, handler_type> now_container_type;

		const handler_type* phandler = 
			utility::args_holder_ref_wrap_static_cast<handler_type const>(any_handler);
		assert(phandler);

		if(!phandler)
		{
			return false;
		}

		bool bgate_now = true;
		bool bgate_next = false;

		if(_gate.compare_exchange_strong(bgate_now, bgate_next))
		{
			assert(base.empty());
			return true;
		}
		else
		{
			base_opacket_container_ptr_type ptr = 
				construct_shared<base_opacket_container>(
					yggr_nothrow_new now_container_type(pak, *phandler) );

			if(!ptr)
			{
				return false;
			}

			base.push_back(ptr);
			assert(!base.empty());
			return false;
		}
	}

	inline base_opacket_container_ptr_type prv_handler_next_send(typename queue_type::base_type& base)
	{
		if(base.empty())
		{
			bool bgate_now = false;
			bool bgate_next = true;
			_gate.compare_exchange_strong(bgate_now, bgate_next); //gate state is close or open open_gate false->true;
			return base_opacket_container_ptr_type();
		}
		else
		{
			base_opacket_container_ptr_type ptr(boost::move(base.front()));
			base.pop_front();
			return ptr;
		}
	}

	inline void prv_handler_clear(typename queue_type::base_type& base, 
									typename queue_type::base_type& out)
	{
		base.swap(out);
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