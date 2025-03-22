//p2p_default_peer_user_data_fixer.hpp

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
#include <yggr/utility/swap.hpp>

namespace yggr
{
namespace p2p
{

template<typename Owner, 
			typename P2P_Boot_Request_Back,
			typename P2P_Boot_Start >
class p2p_default_peer_user_data_fixer
{
public:
	typedef Owner Owner_type;
	typedef P2P_Boot_Request_Back p2p_boot_request_back_type;
	typedef P2P_Boot_Start p2p_boot_start_type;

	typedef typename owner_type::owner_id_type owner_id_type;
	typedef typename p2p_boot_request_back_type::peer_boot_info_cont_type peer_boot_info_cont_type;
	typedef typename p2p_boot_request_back_type::peer_boot_info_type peer_boot_info_type;

private:
	typedef p2p_default_peer_user_data_fixer this_type;

public:
	p2p_default_peer_user_data_fixer(void)
	{
	}

	~p2p_default_peer_user_data_fixer(void)
	{
	}

public:
	inline bool fix_boot_request_back_enable(const owner_type& owner, 
												const p2p_boot_request_back_type& boot_request_back)
	{
		return !boot_request_back.empty();
	}

	inline void fix_boot_request_back(const owner_type& owner,
										peer_boot_info_cont_type& out_peer_info_cont, 
										p2p_boot_start_type& out_boot_start )
	{
	}

	inline void remove_info(const owner_type& owner)
	{
		return;
	}

};

} // namespace p2p
} // namespace yggr

#endif // __YGGR_P2P_P2P_DEFAULT_BOOT_USER_DATA_FIXER_HPP__
