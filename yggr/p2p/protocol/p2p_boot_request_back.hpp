//p2p_boot_request_back.hpp

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

#ifndef __YGGR_P2P_PROTOCOL_P2P_BOOT_BACK_HPP__
#define __YGGR_P2P_PROTOCOL_P2P_BOOT_BACK_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/container_ex/container_mapped_value_t.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

namespace yggr
{
namespace p2p
{

template<typename Peer_Boot_Info_Container, typename User_Data = u32>
class p2p_boot_request_back
{
public:
	typedef Peer_Boot_Info_Container peer_boot_info_cont_type;
	typedef typename container_mapped_value_t<peer_boot_info_cont_type>::type peer_boot_info_type;
	typedef User_Data user_data_type;

private:
	typedef p2p_boot_request_back this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	p2p_boot_request_back(void)
		: _user_data()
	{
	}

	template<typename InputIter>
	p2p_boot_request_back(InputIter s, InputIter e, const user_data_type& user_data)
		: _peer_boot_infos(s, e), _user_data(user_data)
	{
	}

	p2p_boot_request_back(const peer_boot_info_cont_type& peer_infos, const user_data_type& user_data)
		: _peer_boot_infos(peer_infos), _user_data(user_data)
	{
	}

	p2p_boot_request_back(BOOST_RV_REF(peer_boot_info_cont_type) peer_infos, 
							BOOST_RV_REF(user_data_type) user_data)
		: _peer_boot_infos(boost::move(peer_infos)), 
			_user_data(boost::move(user_data))
	{
	}

	p2p_boot_request_back(BOOST_RV_REF(this_type) right)
		: _peer_boot_infos(boost::move(right._peer_boot_infos)),
			_user_data(boost::move(right._user_data))
	{
	}

	p2p_boot_request_back(const this_type& right)
		: _peer_boot_infos(right._peer_boot_infos), _user_data(right._user_data)
	{
	}

	~p2p_boot_request_back(void)
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

		copy_or_move_or_swap(_peer_boot_infos, boost::move(right._peer_boot_infos));
		copy_or_move_or_swap(_user_data, boost::move(right._user_data));

		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_peer_boot_infos = right._peer_boot_infos;
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

		yggr::swap(_peer_boot_infos, right._peer_boot_infos);
		yggr::swap(_user_data, right._user_data);
	}

public:
	inline bool empty(void) const
	{
		return boost::empty(_peer_boot_infos);
	}

	inline peer_boot_info_cont_type& peer_boot_infos(void)
	{
		return _peer_boot_infos;
	}

	inline const peer_boot_info_cont_type& peer_boot_infos(void) const
	{
		return _peer_boot_infos;
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
		ar & YGGR_SERIALIZATION_NVP(_peer_boot_infos);
		ar & YGGR_SERIALIZATION_NVP(_user_data);
	}

private:
	peer_boot_info_cont_type _peer_boot_infos;
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

template<typename Peer_Boot_Info_Container, typename User_Data> inline 
void swap(p2p_boot_request_back<Peer_Boot_Info_Container, User_Data>& l, 
			p2p_boot_request_back<Peer_Boot_Info_Container, User_Data>& r) 
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

#endif // __YGGR_P2P_PROTOCOL_P2P_BOOT_BACK_HPP__
