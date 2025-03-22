//p2p_boot_request.hpp

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

#ifndef __YGGR_P2P_PROTOCOL_P2P_BOOT_REQUEST_HPP__
#define __YGGR_P2P_PROTOCOL_P2P_BOOT_REQUEST_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/serialization/access.hpp>

namespace yggr
{
namespace p2p
{

template<typename Socket_Info, typename User_Data = u32>
class p2p_boot_request
{
public:
	typedef Socket_Info socket_info_type;
	typedef User_Data user_data_type;

private:
	typedef p2p_boot_request this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	p2p_boot_request(void)
		: _local_socket_info(), _user_data()
	{
	}

	p2p_boot_request(const socket_info_type& socket_info)
		: _local_socket_info(socket_info), _user_data()
	{
	}

	p2p_boot_request(const socket_info_type& socket_info,
						const user_data_type& user_data)
		: _local_socket_info(socket_info),
			_user_data(user_data)
	{
	}

	p2p_boot_request(BOOST_RV_REF(socket_info_type) socket_info)
		: _local_socket_info(boost::move(socket_info)),
			_user_data()
	{
	}

	p2p_boot_request(BOOST_RV_REF(socket_info_type) socket_info,
						BOOST_RV_REF(user_data_type) user_data)
		: _local_socket_info(boost::move(socket_info)),
			_user_data(boost::move(user_data))
	{
	}

	p2p_boot_request(BOOST_RV_REF(this_type) right)
		: _local_socket_info(boost::move(right._local_socket_info)),
			_user_data(boost::move(right._user_data))
	{
	}

	p2p_boot_request(const this_type& right)
		: _local_socket_info(right._local_socket_info),
			_user_data(right._user_data)
	{
	}

	~p2p_boot_request(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_local_socket_info, boost::move(right._local_socket_info));
		copy_or_move_or_swap(_user_data, boost::move(right._user_data));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_local_socket_info = right._local_socket_info;
		_user_data = right._user_data;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		yggr::swap(_local_socket_info, right._local_socket_info);
		yggr::swap(_user_data, right._user_data);
	}

public:
	inline socket_info_type& local_socket_info(void)
	{
		return _local_socket_info;
	}

	inline const socket_info_type& local_socket_info(void) const
	{
		return _local_socket_info;
	}

	inline user_data_type& user_data(void)
	{
		return _user_data;
	}

	inline const user_data_type& user_data(void) const
	{
		return _user_data;
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_local_socket_info);
		ar & YGGR_SERIALIZATION_NVP(_user_data);
	}

private:
	socket_info_type _local_socket_info; // fix local_network peers
	user_data_type _user_data;
};

} // namespace p2p
} // namespace yggr

namespace yggr
{
namespace p2p
{
namespace swap_support
{

template<typename Socket_Info, typename UserData> inline 
void swap(p2p_boot_request<Socket_Info, UserData>& l, 
			p2p_boot_request<Socket_Info, UserData>& r)
{ 
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace p2p
} // namespace yggr

namespace std
{
	using ::yggr::p2p::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::p2p::swap_support::swap;
} // namespace boost

#endif // __YGGR_P2P_PROTOCOL_P2P_BOOT_REQUEST_HPP__
