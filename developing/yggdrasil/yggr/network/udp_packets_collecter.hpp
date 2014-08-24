//udp_packets_collecter.hpp

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

#ifndef __YGGR_NETWORK_UDP_PACKETS_COLLECTER_HPP__
#define __YGGR_NETWORK_UDP_PACKETS_COLLECTER_HPP__

#include <boost/tuple/tuple.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>
#include <yggr/safe_container/safe_buffered_object.hpp>

#include <yggr/network/udp_pak_info.hpp>

namespace yggr
{
namespace network
{

template<typename Wrap>
class udp_packets_collecter 
	: private nonable::noncopyable
{
public:
	typedef Wrap wrap_type;
	typedef typename wrap_type::data_buf_type data_buf_type;
	typedef typename wrap_type::id_type id_type;
	
private:
	class wrap_obj 
		: public wrap_type
	{
	public:
		inline void splice_to(const wrap_type& right)
		{
		}

		inline operator wrap_type&(void)
		{
			return *this;
		}

		inline operator const wrap_type&(void) const
		{
			return *this;
		}
	};

public:
	typedef wrap_obj wrap_obj_type;

private:
	typedef safe_container::safe_buffered_object<wrap_obj_type> wrap_obj_buf_queue_type;

public:
	typedef typename wrap_obj_buf_queue_type::obj_ptr_type wrap_obj_ptr_type;

private:
	typedef safe_container::safe_unordered_map<id_type, wrap_obj_ptr_type> wrap_map_type;
	typedef typename wrap_map_type::iterator wrap_map_iter_type;
	typedef typename wrap_map_type::const_iterator wrap_map_citer_type;

private:
	typedef udp_packets_collecter this_type;

public:
	udp_packets_collecter(void)
	{
	}

	~udp_packets_collecter(void)
	{
	}

	wrap_obj_ptr_type collect(const data_buf_type& buf)
	{
		typedef network::udp_pak_id udp_pak_id_type;

		udp_pak_id_type id;
		id.from_buffer(buf);

		return this_type::collect(id, buf);
	}

	wrap_obj_ptr_type collect(const id_type& id, const data_buf_type& buf)
	{
		return _wrap_map.use_handler(boost::bind(&this_type::handler_insert_buffer, this, 
														_1, boost::cref(id), boost::cref(buf)));
	}

	void check_state(void)
	{
	}

	void back_buffer(wrap_obj_ptr_type pbuf)
	{
		pbuf->clear();
		_wrap_obj_buf_queue.back(pbuf);
	}

	void clear(void)
	{
		_wrap_map.clear();
	}

private:

	void handler_check_state(typename wrap_map_type::base_type& base)
	{
		typedef typename wrap_map_type::base_type cont_type;
		typedef typename cont_type::iterator iter_type;

		for(iter_type i = base.begin(), isize = base.end(); i != isize;)
		{
			if(!i->second || i->second->check_state() == wrap_type::E_state_die)
			{
				i = base.erase(i);
				isize = base.end();
				continue;
			}

			++i;
		}
	}

	wrap_obj_ptr_type handler_insert_buffer(typename wrap_map_type::base_type& base,
												const id_type& id,
												const data_buf_type& buf)
	{
		typedef typename wrap_map_type::base_type cont_type;
		typedef typename wrap_map_type::value_type ins_val_type;
		typedef typename cont_type::iterator iter_type;

		iter_type iter = base.find(id);

		wrap_obj_ptr_type ptr;
		if(iter != base.end())
		{
			if(!iter->second)
			{
				base.erase(iter);
			}
			else
			{
				 ptr = iter->second;
				(*ptr) << buf;
			}
		}

		if(ptr)
		{
			if(ptr->is_complete())
			{
				base.erase(iter);
				return ptr;
			}
			else
			{
				return wrap_obj_ptr_type();
			}
		}

		ptr = _wrap_obj_buf_queue.drop();
		if(!ptr)
		{
			return wrap_obj_ptr_type();
		}

		assert(ptr->empty());
		(*ptr) << buf;

		if(ptr->is_complete())
		{
			return ptr;
		}

		base.insert(ins_val_type(id, ptr));
		return wrap_obj_ptr_type();
	}

private:
	wrap_map_type _wrap_map;
	static wrap_obj_buf_queue_type _wrap_obj_buf_queue;
};

template<typename Wrap>
typename udp_packets_collecter<Wrap>::wrap_obj_buf_queue_type
	udp_packets_collecter<Wrap>::_wrap_obj_buf_queue;

} // namespace network
} // nemspace yggr

#endif // __YGGR_NETWORK_UDP_PACKETS_COLLECTER_HPP__