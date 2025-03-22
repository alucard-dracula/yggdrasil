//network_sender.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_SENDER_HPP__
#define __YGGR_NETWORK_NETWORK_SENDER_HPP__

#include <yggr/base/ptr_single.hpp>

namespace yggr
{
namespace network
{

// network_sender
template<typename NetEntity, 
			typename Mutex = boost::shared_mutex,
			yggr::u32 version = 0,
			template<typename _T, typename _Mutex, yggr::u32 _version>
				class Getter = ptr_single>
class network_sender
{
public:
	typedef NetEntity net_entity_type;
	typedef Getter<net_entity_type, Mutex, version> getter_type;
	typedef typename getter_type::obj_ptr_type net_entity_ptr_type;

private:
	typedef network_sender this_type;

public:
	network_sender(void)
	{
	}

	~network_sender(void)
	{
	}

public:
	template<typename Key, typename NetInfo, typename Data, typename Handler> inline
	bool operator()(const Key& key, const NetInfo& netinfo, const Data& data, const Handler& handler) const
	{
		return this_type::send_packet(key, netinfo, data, handler);
	}

	template<typename Key, typename Pak, typename Handler> inline
	bool operator()(const Key& key, Pak& pak, const Handler& handler) const
	{
		return this_type::send_packet(key, pak, handler);
	}

	template<typename Key, typename Pak> inline
	bool operator()(const Key& key, Pak& pak) const
	{
		return this_type::send_packet(key, pak);
	}

public:

	template<typename Key, typename NetInfo, typename Data, typename Handler> inline
	static bool send_packet(const Key& key, const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		net_entity_ptr_type ptr(getter_type::get_ins());
		return ptr && ptr->send_packet(key, netinfo, data, handler);
	}

	template<typename Key, typename Pak, typename Handler> inline
	static bool send_packet(const Key& key, Pak& pak, const Handler& handler)
	{
		net_entity_ptr_type ptr(getter_type::get_ins());
		return ptr && ptr->send_packet(key, pak, handler);
	}

	template<typename Key, typename Pak> inline
	static bool send_packet(const Key& key, Pak& pak)
	{
		net_entity_ptr_type ptr(getter_type::get_ins());
		return ptr && ptr->send_packet(key, pak);
	}

	inline static void non_handler_send(const boost::system::error_code&) {}
};

} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_NETWORK_SENDER_HPP__