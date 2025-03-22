//p2p_peer_boot_info.hpp

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

#ifndef __YGGR_P2P_PROTOCOL_P2P_PEER_BOOT_INFO_HPP__
#define __YGGR_P2P_PROTOCOL_P2P_PEER_BOOT_INFO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/network/address_helper.hpp>
#include <yggr/p2p/p2p_peer_boot_info_generator.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/access.hpp>

#include <utility>

namespace yggr
{
namespace p2p
{

template<typename Owner_ID, typename Socket_Info>
class p2p_peer_boot_info
{
public:
	typedef Owner_ID owner_id_type;
	typedef Socket_Info socket_info_type;

private:
	typedef p2p_peer_boot_info this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	p2p_peer_boot_info(void)
		: _owner_id(), 
			_src_nat_socket_info(),
			_src_local_socket_info(),
			_dst_nat_socket_info(), 
			_dst_local_socket_info()
	{
	}

	p2p_peer_boot_info(BOOST_RV_REF(owner_id_type) owner_id,
					BOOST_RV_REF(socket_info_type) src_nat_socket_info,
					BOOST_RV_REF(socket_info_type) src_local_socket_info,
					BOOST_RV_REF(socket_info_type) dst_nat_socket_info,
					BOOST_RV_REF(socket_info_type) dst_local_socket_info )
		: _owner_id(boost::move(owner_id)),
			_src_nat_socket_info(boost::move(src_nat_socket_info)),
			_src_local_socket_info(boost::move(src_local_socket_info)),
			_dst_nat_socket_info(boost::move(dst_nat_socket_info)),
			_dst_local_socket_info(boost::move(dst_local_socket_info))
			
	{
	}

	p2p_peer_boot_info(const owner_id_type& owner_id,
						const socket_info_type& src_nat_socket_info,
						const socket_info_type& src_local_socket_info,
						const socket_info_type& dst_nat_socket_info,
						const socket_info_type& dst_local_socket_info)
		: _owner_id(owner_id),
			_src_nat_socket_info(src_nat_socket_info),
			_src_local_socket_info(src_local_socket_info),
			_dst_nat_socket_info(dst_nat_socket_info),
			_dst_local_socket_info(dst_local_socket_info)
	{
	}

	p2p_peer_boot_info(BOOST_RV_REF(this_type) right)
		: _owner_id(boost::move(right._owner_id)),
			_src_nat_socket_info(boost::move(right._src_nat_socket_info)),
			_src_local_socket_info(boost::move(right._src_local_socket_info)),
			_dst_nat_socket_info(boost::move(right._dst_nat_socket_info)),
			_dst_local_socket_info(boost::move(right._dst_local_socket_info))
			
	{
	}

	p2p_peer_boot_info(const this_type& right)
		: _owner_id(right._owner_id),
			_src_nat_socket_info(right._src_nat_socket_info),
			_src_local_socket_info(right._src_local_socket_info),
			_dst_nat_socket_info(right._dst_nat_socket_info),
			_dst_local_socket_info(right._dst_local_socket_info)
	{
	}

	~p2p_peer_boot_info(void)
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

		copy_or_move_or_swap(_owner_id, boost::move(right._owner_id));
		copy_or_move_or_swap(_src_nat_socket_info, boost::move(right._src_nat_socket_info));
		copy_or_move_or_swap(_src_local_socket_info, boost::move(right._src_local_socket_info));
		copy_or_move_or_swap(_dst_nat_socket_info, boost::move(right._dst_nat_socket_info));
		copy_or_move_or_swap(_dst_local_socket_info, boost::move(right._dst_local_socket_info));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_owner_id = right._owner_id;
		_src_nat_socket_info = right._src_nat_socket_info;
		_src_local_socket_info = right._src_local_socket_info;
		_dst_nat_socket_info = right._dst_nat_socket_info;
		_dst_local_socket_info = right._dst_local_socket_info;

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

		yggr::swap(_owner_id, right._owner_id);
		yggr::swap(_src_nat_socket_info, right._src_nat_socket_info);
		yggr::swap(_src_local_socket_info, right._src_local_socket_info);
		yggr::swap(_dst_nat_socket_info, right._dst_nat_socket_info);
		yggr::swap(_dst_local_socket_info, right._dst_local_socket_info);
	}

public:
	inline owner_id_type& owner_id(void)
	{
		return _owner_id;
	}

	inline const owner_id_type& owner_id(void) const
	{
		return _owner_id;
	}

	inline socket_info_type& dst_nat_socket_info(void)
	{
		return _dst_nat_socket_info;
	}

	inline const socket_info_type& dst_nat_socket_info(void) const
	{
		return _dst_nat_socket_info;
	}

	inline socket_info_type& dst_local_socket_info(void)
	{
		return _dst_local_socket_info;
	}

	inline const socket_info_type& dst_local_socket_info(void) const
	{
		return _dst_local_socket_info;
	}

	inline socket_info_type& src_nat_socket_info(void)
	{
		return _src_nat_socket_info;
	}

	inline const socket_info_type& src_nat_socket_info(void) const
	{
		return _src_nat_socket_info;
	}

	inline socket_info_type& src_local_socket_info(void)
	{
		return _src_local_socket_info;
	}

	inline const socket_info_type& src_local_socket_info(void) const
	{
		return _src_local_socket_info;
	}

	inline void swap_src_dst(void)
	{
		yggr::swap(_src_nat_socket_info, _dst_nat_socket_info);
		yggr::swap(_src_local_socket_info, _dst_local_socket_info);
	}

	// first: is_reduce_link_enable, second: is_try_local_host
	inline bool is_try_reduce_link_enable(void) const
	{
		typedef network::address_helper address_helper_type;
		typedef address_helper_type::address_type address_type;

		address_type src_nat_address(address_helper_type::address_from_string(_src_nat_socket_info.host()));
		address_type src_local_address(address_helper_type::address_from_string(_src_local_socket_info.host()));
		address_type dst_nat_address(address_helper_type::address_from_string(_dst_nat_socket_info.host()));
		address_type dst_local_address(address_helper_type::address_from_string(_dst_local_socket_info.host()));
		
		return 
			(dst_nat_address != dst_local_address)	// has nat 
			&& (dst_nat_address == src_nat_address)	// same nat
			&& (!((src_local_address == dst_local_address) // loopback communication occurs if local endpoint are equal 
					&& (_src_local_socket_info.port() == _dst_local_socket_info.port())));
	}

	inline bool empty(void) const
	{
		return 
			_owner_id == owner_id_type()
			|| _src_nat_socket_info == socket_info_type()
			|| _src_local_socket_info == socket_info_type()
			|| _dst_nat_socket_info == socket_info_type()
			|| _dst_local_socket_info == socket_info_type();
	}

	inline std::size_t hash(void) const
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, _owner_id);
		boost::hash_combine(seed, _src_nat_socket_info);
		boost::hash_combine(seed, _src_local_socket_info);
		boost::hash_combine(seed, _dst_nat_socket_info);
		boost::hash_combine(seed, _dst_local_socket_info);
		return seed;
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_owner_id);

		ar & YGGR_SERIALIZATION_NVP(_src_nat_socket_info);
		ar & YGGR_SERIALIZATION_NVP(_src_local_socket_info);
		ar & YGGR_SERIALIZATION_NVP(_dst_nat_socket_info);
		ar & YGGR_SERIALIZATION_NVP(_dst_local_socket_info);
	}

private:
	owner_id_type _owner_id;
	socket_info_type _src_nat_socket_info;
	socket_info_type _src_local_socket_info;
	socket_info_type _dst_nat_socket_info;
	socket_info_type _dst_local_socket_info;
};

} // namespace p2p
} // namespace yggr

namespace yggr
{
namespace p2p
{

template< typename Owner_ID, typename Socket_Info >
class peer_boot_info_generator< p2p_peer_boot_info<Owner_ID, Socket_Info> >
{
public:
	typedef Owner_ID owner_id_type;
	typedef Socket_Info socket_info_type;
	typedef p2p_peer_boot_info<owner_id_type, socket_info_type> now_p2p_peer_boot_info_type;
	// first src, second dst
	typedef std::pair<now_p2p_peer_boot_info_type, now_p2p_peer_boot_info_type> result_type;

public:
	// the foo less pair_pair.first.second!!! what the fuck
	//inline result_type operator()(BOOST_RV_REF(owner_id_type) owner_id, 
	//								BOOST_RV_REF(socket_info_type) src_nat_socket_info,
	//								BOOST_RV_REF(socket_info_type) src_local_socket_info,
	//								BOOST_RV_REF(socket_info_type) dst_nat_socket_info,
	//								BOOST_RV_REF(socket_info_type) dst_local_socket_info) const
	//{
	//	now_p2p_peer_boot_info_type info(boost::move(owner_id), 
	//										boost::move(src_nat_socket_info), 
	//										boost::move(src_local_socket_info),
	//										boost::move(dst_nat_socket_info), 
	//										boost::move(dst_local_socket_info));

	//	result_type ret;
	//	ret.first = info;

	//	info.swap_src_dst();
	//	ret.second = boost::move(info);

	//	return ret;
	//}

	// the foo less pair_pair.first.second!!! what the fuck
	//inline result_type operator()(const owner_id_type& owner_id, 
	//								const socket_info_type& src_nat_socket_info,
	//								const socket_info_type& src_local_socket_info,
	//								const socket_info_type& dst_nat_socket_info,
	//								const socket_info_type& dst_local_socket_info) const
	//{
	//	now_p2p_peer_boot_info_type info(owner_id, 
	//									src_nat_socket_info, 
	//									src_local_socket_info, 
	//									dst_nat_socket_info,
	//									dst_local_socket_info);

	//	result_type ret;
	//	ret.first = info;

	//	info.swap_src_dst();
	//	ret.second = boost::move(info);

	//	return ret;
	//}

	inline result_type& operator()(result_type& ret,
									BOOST_RV_REF(owner_id_type) owner_id, 
									BOOST_RV_REF(socket_info_type) src_nat_socket_info,
									BOOST_RV_REF(socket_info_type) src_local_socket_info,
									BOOST_RV_REF(socket_info_type) dst_nat_socket_info,
									BOOST_RV_REF(socket_info_type) dst_local_socket_info) const
	{
		now_p2p_peer_boot_info_type info(boost::move(owner_id), 
											boost::move(src_nat_socket_info), 
											boost::move(src_local_socket_info),
											boost::move(dst_nat_socket_info), 
											boost::move(dst_local_socket_info));

		ret.first = info;

		info.swap_src_dst();
		ret.second = boost::move(info);

		return ret;
	}

	inline result_type& operator()(result_type& ret,
									const owner_id_type& owner_id, 
									const socket_info_type& src_nat_socket_info,
									const socket_info_type& src_local_socket_info,
									const socket_info_type& dst_nat_socket_info,
									const socket_info_type& dst_local_socket_info) const
	{
		now_p2p_peer_boot_info_type info(owner_id, 
										src_nat_socket_info, 
										src_local_socket_info, 
										dst_nat_socket_info,
										dst_local_socket_info);

		ret.first = info;

		info.swap_src_dst();
		ret.second = boost::move(info);

		return ret;
	}
};

} // namespace p2p
} // namespace yggr

namespace yggr
{
namespace p2p
{
namespace swap_support
{

template<typename Socket_Info, typename Owner_ID> inline 
void swap(p2p_peer_boot_info<Socket_Info, Owner_ID>& l, 
			p2p_peer_boot_info<Socket_Info, Owner_ID>& r)
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

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace network
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename Socket_Info, typename Owner_ID> inline
std::size_t hash_value(const yggr::p2p::p2p_peer_boot_info<Socket_Info, Owner_ID>& val)
{
	return val.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} // namespace boost
#else
} // namespace network
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#if defined(YGGR_HAS_CXX11_STD_HASH)

namespace std
{

template<typename Socket_Info, typename Owner_ID>
struct hash<yggr::p2p::p2p_peer_boot_info<Socket_Info, Owner_ID> >
{
	typedef yggr::p2p::p2p_peer_boot_info<Socket_Info, Owner_ID> type;

	inline std::size_t operator()(const type& val) const
	{
		return val.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#endif // __YGGR_P2P_PROTOCOL_P2P_PEER_BOOT_INFO_HPP__
