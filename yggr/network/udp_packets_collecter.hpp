//udp_packets_collecter.hpp

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

#ifndef __YGGR_NETWORK_UDP_PACKETS_COLLECTER_HPP__
#define __YGGR_NETWORK_UDP_PACKETS_COLLECTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/tuple/tuple.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>
#include <yggr/safe_container/safe_buffered_object.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <yggr/network/udp_pak_info.hpp>

namespace yggr
{
namespace network
{

template<typename Packet_Wrap>
class udp_packets_collecter
	: private nonable::noncopyable
{
public:
	typedef Packet_Wrap basic_packet_wrap_type;
	typedef typename basic_packet_wrap_type::data_buf_type data_buf_type;
	typedef typename basic_packet_wrap_type::id_type id_type;

private:
	typedef basic_packet_wrap_type packet_wrap;

public:
	typedef packet_wrap packet_wrap_type;

private:
	typedef safe_container::safe_buffered_object<packet_wrap_type> packet_wrap_buf_queue_type;

public:
	typedef typename packet_wrap_buf_queue_type::obj_ptr_type packet_wrap_ptr_type;

private:
	typedef safe_container::safe_wrap<packet_wrap_type> safe_packet_wrap_type;
	typedef safe_container::safe_buffered_object<safe_packet_wrap_type> safe_packet_wrap_buf_queue_type;
	typedef typename safe_packet_wrap_buf_queue_type::obj_ptr_type safe_packet_wrap_ptr_type;

private:
	typedef safe_container::safe_unordered_map<id_type, safe_packet_wrap_ptr_type> wrap_map_type;
	typedef typename wrap_map_type::iterator wrap_map_iter_type;
	typedef typename wrap_map_type::const_iterator wrap_map_citer_type;
	typedef typename wrap_map_type::base_type unsafe_wrap_map_type;

private:
	typedef udp_packets_collecter this_type;

#define _packet_wrap_buf_queue _packet_wrap_buf_queue()
#define _safe_packet_wrap_buf_queue _safe_packet_wrap_buf_queue()

public:
	udp_packets_collecter(void)
	{
	}

	~udp_packets_collecter(void)
	{
	}

public:
	inline packet_wrap_ptr_type collect(BOOST_RV_REF(data_buf_type) buf)
	{
		typedef network::udp_pak_id udp_pak_id_type;

		const data_buf_type& buf_cref = buf;
		udp_pak_id_type id;

		return
			id.from_buffer(buf_cref)?
				this_type::collect(id, boost::move(buf))
				: packet_wrap_ptr_type();
	}

	inline packet_wrap_ptr_type collect(const data_buf_type& buf)
	{
		typedef network::udp_pak_id udp_pak_id_type;

		udp_pak_id_type id;

		return
			id.from_buffer(buf)?
				this_type::collect(id, buf)
				: packet_wrap_ptr_type();
	}

	inline packet_wrap_ptr_type collect(const id_type& id, BOOST_RV_REF(data_buf_type) buf)
	{
		packet_wrap_ptr_type ppak_wrap = _packet_wrap_buf_queue.drop();
		assert(ppak_wrap);
		if(!ppak_wrap)
		{
			return packet_wrap_ptr_type();
		}

		assert(ppak_wrap->empty());
		(*ppak_wrap) << boost::move(buf);

		return ppak_wrap->is_completed()? ppak_wrap : this_type::prv_collect_of_wrap(id, ppak_wrap);
	}

	inline packet_wrap_ptr_type collect(const id_type& id, const data_buf_type& buf)
	{
		packet_wrap_ptr_type ppak_wrap = _packet_wrap_buf_queue.drop();
		if(!ppak_wrap)
		{
			return packet_wrap_ptr_type();
		}

		assert(ppak_wrap->empty());
		(*ppak_wrap) << buf;

		return ppak_wrap->is_completed()? ppak_wrap : this_type::prv_collect_of_wrap(id, ppak_wrap);
	}

public:
	void check_state(void)
	{
		typename wrap_map_type::base_type tmp = _wrap_map.load();

		for(typename wrap_map_type::iterator i = tmp.begin(), isize = tmp.end();
				i != isize; ++i)
		{
			try
			{
				if((!i->second) || (i->second->empty())
					|| (i->second->call_function(&basic_packet_wrap_type::check_state)
						== basic_packet_wrap_type::E_state_die))
				{
					_wrap_map.erase(i->first);
				}
			}
			catch(const typename safe_packet_wrap_type::error_type&)
			{
				_wrap_map.erase(i->first);
			}
		}
	}

	inline void back_buffer(packet_wrap_ptr_type pbuf)
	{
		pbuf->clear();
		_packet_wrap_buf_queue.back(pbuf);
	}

	inline void clear(void)
	{
		_wrap_map.clear();
	}

private:
	inline void prv_back_safe_packet_wrap(safe_packet_wrap_ptr_type psafe_pak_wrap)
	{
		if(!psafe_pak_wrap)
		{
			return;
		}

		psafe_pak_wrap->clear();
		_safe_packet_wrap_buf_queue.back(psafe_pak_wrap);
	}

private:
	packet_wrap_ptr_type prv_collect_of_wrap(const id_type& id, packet_wrap_ptr_type ppak_wrap)
	{
		assert(ppak_wrap);
		assert(!ppak_wrap->is_completed());

		safe_packet_wrap_ptr_type pins_safe_pak_wrap = _safe_packet_wrap_buf_queue.drop();
		assert(pins_safe_pak_wrap);

		if(!pins_safe_pak_wrap)
		{
			return packet_wrap_ptr_type();
		}

		pins_safe_pak_wrap->store(ppak_wrap);
		safe_packet_wrap_ptr_type psafe_pak_wrap;

		try
		{
			psafe_pak_wrap =
				_wrap_map.insert(
					id, psafe_pak_wrap,
					boost::bind(&this_type::prv_s_handler_insert_buffer, _1, _2));
		}
		catch(const ::yggr::stl_exception&)
		{
			this_type::prv_back_safe_packet_wrap(pins_safe_pak_wrap);
			return packet_wrap_ptr_type();
		}

		if(!psafe_pak_wrap)
		{
			return packet_wrap_ptr_type();
		}
		else
		{
			this_type::prv_back_safe_packet_wrap(pins_safe_pak_wrap);
			packet_wrap_ptr_type pret;
			try
			{
				pret =
					psafe_pak_wrap->using_handler(
						boost::bind(&this_type::prv_s_handler_collect, _1,  ppak_wrap));
			}
			catch(const typename safe_packet_wrap_type::error_type&)
			{
				_wrap_map.erase(id);
				this_type::prv_back_safe_packet_wrap(psafe_pak_wrap);
				return packet_wrap_ptr_type();
			}

			if(pret)
			{
				_wrap_map.erase(id);
				psafe_pak_wrap->clear();
				this_type::prv_back_safe_packet_wrap(psafe_pak_wrap);
			}
			else
			{
				this_type::back_buffer(ppak_wrap);
			}

			return pret;
		}
	}

	static packet_wrap_ptr_type
		prv_s_handler_collect(packet_wrap_ptr_type& pbase, packet_wrap_ptr_type ppak_wrap)
	{
		assert(pbase && ppak_wrap);
		(*pbase) << boost::move(static_cast<basic_packet_wrap_type&>(*ppak_wrap));
		return pbase->is_completed()? pbase : packet_wrap_ptr_type();
	}

	inline static safe_packet_wrap_ptr_type
		prv_s_handler_insert_buffer(typename wrap_map_type::base_type& base,
									const std::pair<typename wrap_map_type::iterator, bool> rst)
	{
		return rst.second? safe_packet_wrap_ptr_type() : rst.first->second;
	}

#undef _packet_wrap_buf_queue
#undef _safe_packet_wrap_buf_queue

private:
	wrap_map_type _wrap_map;

	inline static packet_wrap_buf_queue_type& _packet_wrap_buf_queue(void) // inline static var in cpp03
	{
		static packet_wrap_buf_queue_type queue;
		return queue;
	}

	inline static safe_packet_wrap_buf_queue_type& _safe_packet_wrap_buf_queue(void) // inline static var in cpp03
	{
		static safe_packet_wrap_buf_queue_type queue;
		return queue;
	}

};

} // namespace network
} // nemspace yggr

#endif // __YGGR_NETWORK_UDP_PACKETS_COLLECTER_HPP__
