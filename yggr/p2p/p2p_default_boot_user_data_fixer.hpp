//p2p_default_boot_user_data_fixer.hpp

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

#ifndef __YGGR_P2P_P2P_DEFAULT_BOOT_USER_DATA_FIXER_HPP__
#define __YGGR_P2P_P2P_DEFAULT_BOOT_USER_DATA_FIXER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/container_ex/container_mapped_value_t.hpp>
#include <yggr/container/unordered_map.hpp>

namespace yggr
{
namespace p2p
{

template<typename Owner_Container, 
			typename P2P_Boot_Request,
			typename P2P_Boot_Request_Back >
class p2p_default_boot_user_data_fixer
{
public:
	typedef Owner_Container owner_info_container_type;
	typedef P2P_Boot_Request p2p_boot_request_type;
	typedef P2P_Boot_Request_Back p2p_boot_request_back_type;

	typedef typename container_mapped_value_t<owner_info_container_type>::type owner_type;
	typedef typename owner_type::owner_id_type owner_id_type;

	typedef yggr::unordered_map<owner_type, p2p_boot_request_back_type> p2p_boot_request_back_map_type;

private:
	typedef p2p_default_boot_user_data_fixer this_type;

public:
	p2p_default_boot_user_data_fixer(void)
	{
	}

	~p2p_default_boot_user_data_fixer(void)
	{
	}

public:
	inline bool fix_boot_request_enable(const owner_type& owner, const p2p_boot_request_type& boot_request)
	{
		return true;
	}

	inline void fix_boot_request(const owner_type& owner, 
									const p2p_boot_request_type& boot_request,
									owner_info_container_type& owners_self,
									p2p_boot_request_back_type& back_self, 
									p2p_boot_request_back_map_type& back_others )
	{
		return;
	}

	inline void remove_info(const owner_type& owner)
	{
		return;
	}

};

} // namespace p2p
} // namespace yggr

#endif // __YGGR_P2P_P2P_DEFAULT_BOOT_USER_DATA_FIXER_HPP__
