//p2p_encryption_peer_boot_info.hpp

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

#ifndef __YGGR_P2P_PROTOCOL_P2P_ENCRYPTION_PEER_BOOT_INFO_HPP__
#define __YGGR_P2P_PROTOCOL_P2P_ENCRYPTION_PEER_BOOT_INFO_HPP__

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/p2p/protocol/p2p_peer_boot_info.hpp>
#include <yggr/p2p/p2p_ekeys_generator.hpp>

namespace yggr
{
namespace p2p
{

template<typename Owner_ID, 
			typename Socket_Info,
			typename EKeys >
class p2p_encryption_peer_boot_info
	: public p2p_peer_boot_info<Owner_ID, Socket_Info>
{
public:
	typedef p2p_peer_boot_info<Owner_ID, Socket_Info> base_type;
	typedef Owner_ID owner_id_type;
	typedef Socket_Info socket_info_type;
	typedef EKeys ekeys_type;

private:
	typedef p2p_encryption_peer_boot_info this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	p2p_encryption_peer_boot_info(void)
		: _ekeys()
	{
	}

	p2p_encryption_peer_boot_info(BOOST_RV_REF(owner_id_type) owner_id,
									BOOST_RV_REF(socket_info_type) src_nat_socket_info,
									BOOST_RV_REF(socket_info_type) src_local_socket_info,
									BOOST_RV_REF(socket_info_type) dst_nat_socket_info,
									BOOST_RV_REF(socket_info_type) dst_local_socket_info,
									BOOST_RV_REF(ekeys_type) ekeys)
		: base_type(boost::move(owner_id),
						boost::move(src_nat_socket_info),
						boost::move(src_local_socket_info),
						boost::move(dst_nat_socket_info),
						boost::move(dst_local_socket_info) ),
			_ekeys(boost::move(ekeys))
	{
	}

	p2p_encryption_peer_boot_info(const owner_id_type& owner_id,
									const socket_info_type& src_nat_socket_info,
									const socket_info_type& src_local_socket_info,
									const socket_info_type& dst_nat_socket_info,
									const socket_info_type& dst_local_socket_info,
									const ekeys_type& ekeys )
		: base_type(owner_id, 
						src_nat_socket_info,
						src_local_socket_info, 
						dst_nat_socket_info, 
						dst_local_socket_info),
			_ekeys(ekeys)
	{
	}

	p2p_encryption_peer_boot_info(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right))),
			_ekeys(boost::move(right._ekeys))
	{
	}

	p2p_encryption_peer_boot_info(const this_type& right)
		: base_type(right), _ekeys(right._ekeys)
	{
	}

	~p2p_encryption_peer_boot_info(void)
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

		base_type& base = *this;
		copy_or_move_or_swap(base, boost::move(static_cast<base_type&>(right)));
		copy_or_move_or_swap(_ekeys, boost::move(right._ekeys));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type::operator=(right);
		_ekeys = right._ekeys;
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

		base_type::swap(right);
		yggr::swap(_ekeys, right._ekeys);
	}

public:
	using base_type::owner_id;
	using base_type::src_nat_socket_info;
	using base_type::src_local_socket_info;
	using base_type::dst_nat_socket_info;
	using base_type::dst_local_socket_info;
	
	inline ekeys_type& encryption_keys(void)
	{
		return _ekeys;
	}

	inline const ekeys_type& encryption_keys(void) const
	{
		return _ekeys;
	}

	using base_type::swap_src_dst;
	using base_type::is_try_reduce_link_enable;
	using base_type::empty;

	inline std::size_t hash(void) const
	{
		std::size_t seed = base_type::hash();
		boost::hash_combine(seed, _ekeys);
		return seed;
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP("peer_info", 
										boost::serialization::base_object< base_type >(*this));
		ar & YGGR_SERIALIZATION_NVP(_ekeys);
	}

private:
	ekeys_type _ekeys;
};

} // namespace p2p
} // namespace yggr

namespace yggr
{
namespace p2p
{

template< typename Owner_ID, typename Socket_Info, typename EKeys >
class peer_boot_info_generator< p2p_encryption_peer_boot_info<Owner_ID, Socket_Info, EKeys> >
{
public:
	typedef Owner_ID owner_id_type;
	typedef Socket_Info socket_info_type;
	typedef EKeys ekeys_type;
	typedef p2p_ekeys_generator<ekeys_type> ekeys_gen_type;

	typedef p2p_encryption_peer_boot_info<owner_id_type, socket_info_type, ekeys_type> now_p2p_peer_boot_info_type;
	typedef std::pair<now_p2p_peer_boot_info_type, now_p2p_peer_boot_info_type> result_type;

public:
	//inline result_type operator()(BOOST_RV_REF(owner_id_type) owner_id, 
	//								BOOST_RV_REF(socket_info_type) src_nat_socket_info,
	//								BOOST_RV_REF(socket_info_type) src_local_socket_info,
	//								BOOST_RV_REF(socket_info_type) dst_nat_socket_info,
	//								BOOST_RV_REF(socket_info_type) dst_local_socket_info) const
	//{
	//	ekeys_gen_type gen;
	//	ekeys_type src_key, dst_key;
	//	gen(src_key, dst_key);

	//	now_p2p_peer_boot_info_type info(boost::move(owner_id), 
	//										boost::move(src_nat_socket_info), 
	//										boost::move(src_local_socket_info),
	//										boost::move(dst_nat_socket_info), 
	//										boost::move(dst_local_socket_info),
	//										boost::move(src_key));

	//	result_type ret;
	//	ret.first = info;

	//	info.swap_src_dst();
	//	info.encryption_keys() = dst_key;
	//	ret.second = boost::move(info);

	//	return ret;
	//}

	//inline result_type operator()(const owner_id_type& owner_id, 
	//								const socket_info_type& src_nat_socket_info,
	//								const socket_info_type& src_local_socket_info,
	//								const socket_info_type& dst_nat_socket_info,
	//								const socket_info_type& dst_local_socket_info) const
	//{
	//	ekeys_gen_type gen;
	//	ekeys_type src_key, dst_key;
	//	gen(src_key, dst_key);

	//	now_p2p_peer_boot_info_type info(owner_id, 
	//										src_nat_socket_info, 
	//										src_local_socket_info, 
	//										dst_nat_socket_info,
	//										dst_local_socket_info, 
	//										src_key);
	//	result_type ret;
	//	ret.first = info;

	//	info.swap_src_dst();
	//	info.encryption_keys() = dst_key;
	//	ret.second = boost::move(info);

	//	return ret;
	//	
	//}

	inline result_type& operator()(result_type& ret,
									BOOST_RV_REF(owner_id_type) owner_id, 
									BOOST_RV_REF(socket_info_type) src_nat_socket_info,
									BOOST_RV_REF(socket_info_type) src_local_socket_info,
									BOOST_RV_REF(socket_info_type) dst_nat_socket_info,
									BOOST_RV_REF(socket_info_type) dst_local_socket_info) const
	{
		ekeys_gen_type gen;
		ekeys_type src_key, dst_key;
		gen(src_key, dst_key);

		now_p2p_peer_boot_info_type info(boost::move(owner_id), 
											boost::move(src_nat_socket_info), 
											boost::move(src_local_socket_info),
											boost::move(dst_nat_socket_info), 
											boost::move(dst_local_socket_info),
											boost::move(src_key));

		ret.first = info;

		info.swap_src_dst();
		info.encryption_keys() = dst_key;
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
		ekeys_gen_type gen;
		ekeys_type src_key, dst_key;
		gen(src_key, dst_key);

		now_p2p_peer_boot_info_type info(owner_id, 
											src_nat_socket_info, 
											src_local_socket_info, 
											dst_nat_socket_info,
											dst_local_socket_info, 
											src_key);

		ret.first = info;

		info.swap_src_dst();
		info.encryption_keys() = dst_key;
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

template<typename Owner_ID, typename Socket_Info, typename EKeys> inline 
void swap(p2p_encryption_peer_boot_info<Owner_ID, Socket_Info, EKeys>& l, 
			p2p_encryption_peer_boot_info<Owner_ID, Socket_Info, EKeys>& r) 
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

template<typename Socket_Info, typename Owner_ID, typename EKeys> inline
std::size_t hash_value(const 
						::yggr::p2p::p2p_encryption_peer_boot_info
						<
							Socket_Info, 
							Owner_ID, 
							EKeys 
						>& val)
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

template<typename Socket_Info, typename Owner_ID, typename EKeys>
struct hash< ::yggr::p2p::p2p_encryption_peer_boot_info<
					Socket_Info, Owner_ID, EKeys > >
{
	typedef 
		::yggr::p2p::p2p_encryption_peer_boot_info
		<
			Socket_Info, 
			Owner_ID, 
			EKeys 
		> type;

	inline std::size_t operator()(const type& val) const
	{
		return val.hash();
	}
};

} // namespace std

#endif // YGGR_HAS_CXX11_STD_HASH

#endif // __YGGR_P2P_PROTOCOL_P2P_ENCRYPTION_PEER_BOOT_INFO_HPP__
