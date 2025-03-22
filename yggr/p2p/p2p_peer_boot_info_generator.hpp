//p2p_peer_boot_info_generator.hpp

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

#ifndef __YGGR_P2P_P2P_PEER_BOOT_INFO_GENERATOR_HPP__
#define __YGGR_P2P_P2P_PEER_BOOT_INFO_GENERATOR_HPP__

namespace yggr
{
namespace p2p
{

template<typename Peer_Boot_Info>
class peer_boot_info_generator;

//example:
//template< >
//class peer_boot_info_generator< you_boot_info<Owner_ID, Socket_Info[, ...]> >
//{
//public:
//	typedef Owner_ID owner_id_type;
//	typedef Socket_Info socket_info_type;
//	typedef you_boot_info<owner_id_type, socket_info_type> now_p2p_peer_boot_info_type;
//	typedef std::pair<now_p2p_peer_boot_info_type, now_p2p_peer_boot_info_type> result_type;
//
//public:
//	result_type operator()(const owner_id_type& owner_id, 
//							const socket_info_type& src_nat_socket_info,
//							const socket_info_type& src_local_socket_info,
//							const socket_info_type& dst_nat_socket_info,
//							const socket_info_type& dst_local_socket_info) const
//	{
//		// you_boot_info_gen
//	}
//
//	result_type operator()(BOOST_RV_REF(owner_id_type) owner_id, 
//							BOOST_RV_REF(socket_info_type) src_nat_socket_info,
//							BOOST_RV_REF(socket_info_type) src_local_socket_info,
//							BOOST_RV_REF(socket_info_type) dst_nat_socket_info,
//							BOOST_RV_REF(socket_info_type) dst_local_socket_info) const
//	{
//		// you_boot_info_gen
//	}
//};

} // namespace p2p
} // namespace yggr

#endif // __YGGR_P2P_P2P_PEER_BOOT_INFO_GENERATOR_HPP__
