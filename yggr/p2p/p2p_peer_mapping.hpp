//p2p_peer_mapping.hpp

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

#ifndef __YGGR_P2P_P2P_PEER_MAPPING_HPP__
#define __YGGR_P2P_P2P_PEER_MAPPING_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mapping/unique_mapping.hpp>
#include <utility>

namespace yggr
{
namespace p2p
{

template<typename Owner, typename Socket_Info>
class p2p_peer_mapping
	: public yggr::mapping::unique_mapping
				<
					typename Owner::owner_id_type,
					Owner,
					std::pair< Socket_Info, Socket_Info >
				>
{
public:
	typedef Owner owner_type;
	typedef typename owner_type::owner_id_type owner_id_type;
	typedef Socket_Info socket_info_type;
	typedef std::pair<socket_info_type, socket_info_type> socket_info_pair_type;

	typedef yggr::mapping::unique_mapping
			<
				owner_id_type,
				owner_type,
				socket_info_pair_type
			> base_type;

	typedef base_type mapping_type;

	typedef std::pair< owner_type, bool > mapping_owner_result_type;
	typedef std::pair< owner_id_type, bool > mapping_owner_id_result_type;
	typedef std::pair< socket_info_pair_type, bool > mapping_socket_info_result_type;

private:
	typedef p2p_peer_mapping this_type;

public:
	p2p_peer_mapping(void)
	{
	}

	~p2p_peer_mapping(void)
	{
	}

public:
	inline bool append(const owner_type& owner,
						const socket_info_type& nat_socket_info,
						const socket_info_type& local_socket_info )
	{
		return
			base_type::append(
				owner.owner_id(),
				owner,
				socket_info_pair_type(nat_socket_info, local_socket_info));
	}

	inline bool compulsory_append(const owner_type& owner,
									const socket_info_type& nat_socket_info,
									const socket_info_type& local_socket_info )
	{
		return
			base_type::compulsory_append(
				owner.owner_id(),
				owner,
				socket_info_pair_type(nat_socket_info, local_socket_info));
	}

	inline mapping_owner_result_type get_owner(const owner_id_type& oid) const
	{
		return base_type::template mapping_value<0, 1>(oid);
	}

	inline mapping_socket_info_result_type get_socket_info(const owner_id_type& oid) const
	{
		return base_type::template mapping_value<0, 2>(oid);
	}

	inline mapping_socket_info_result_type get_socket_info(const owner_type& owner) const
	{
		return base_type::template mapping_value<0, 2>(owner.owner_id());
	}

	template<typename Container> inline
	Container& mapping_other_owners(const owner_type& owner, Container& out) const
	{
		return base_type::template mapping_other_values<0, 1>(owner.owner_id(), out);
	}

	template<typename Container> inline
	Container& mapping_other_owners(const owner_id_type& oid, Container& out) const
	{
		return base_type::template mapping_other_values<0, 1>(oid, out);
	}

	template<typename Container> inline
	Container& mapping_other_socket_infos(const owner_type& owner, Container& out) const
	{
		return base_type::template mapping_other_values<0, 2>(owner.owner_id(), out);
	}

	template<typename Container> inline
	Container& mapping_other_socket_infos(const owner_id_type& oid, Container& out) const
	{
		return base_type::template mapping_other_values<0, 2>(oid, out);
	}

	inline bool remove(const owner_type& owner)
	{
		return base_type::template remove<0>(owner.owner_id());
	}

	inline bool remove(const owner_id_type& oid)
	{
		return base_type::template remove<0>(oid);
	}
};

} // namespace p2p
} // namespace yggr

#endif // __YGGR_P2P_P2P_PEER_MAPPING_HPP__
