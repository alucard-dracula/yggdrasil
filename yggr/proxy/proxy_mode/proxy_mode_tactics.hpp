//proxy_mode_tactics.hpp

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

#ifndef __YGGR_PROXY_PROXY_MODE_PROXY_MODE_TACTICS_HPP__
#define __YGGR_PROXY_PROXY_MODE_PROXY_MODE_TACTICS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/proxy/proxy_mode/proxy_mode_def.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_mode
{

struct proxy_mode_tactics
{
	typedef proxy_mode_def mode_def_type;

	inline u32 operator()(u32 old_mode, u32 new_mode) const
	{
		switch(old_mode)
		{
		case mode_def_type::E_proxy_mode_monopolize:
			switch(new_mode)
			{
			case mode_def_type::E_proxy_mode_monopolize:
			case mode_def_type::E_proxy_mode_blanace:
				return mode_def_type::E_proxy_mode_blanace;
			case mode_def_type::E_proxy_mode_source_hash:
				return mode_def_type::E_proxy_mode_source_hash;
			case mode_def_type::E_proxy_mode_all:
				break;
			}
			break;
		case mode_def_type::E_proxy_mode_source_hash:
			switch(new_mode)
			{
			case mode_def_type::E_proxy_mode_monopolize:
			case mode_def_type::E_proxy_mode_source_hash:
				return mode_def_type::E_proxy_mode_source_hash;
			case mode_def_type::E_proxy_mode_blanace:
				return mode_def_type::E_proxy_mode_blanace;
			case mode_def_type::E_proxy_mode_all:
				break;
			}
			break;
		case mode_def_type::E_proxy_mode_blanace:
			switch(new_mode)
			{
			case mode_def_type::E_proxy_mode_monopolize:
			case mode_def_type::E_proxy_mode_blanace:
				return mode_def_type::E_proxy_mode_blanace;
			case mode_def_type::E_proxy_mode_source_hash:
				return mode_def_type::E_proxy_mode_source_hash;
			case mode_def_type::E_proxy_mode_all:
				break;
			}
			break;
		case mode_def_type::E_proxy_mode_all:
			switch(new_mode)
			{
			case mode_def_type::E_proxy_mode_all:
				return mode_def_type::E_proxy_mode_all;
			}
			break;
		default:
			return old_mode == new_mode? old_mode : 0;
		};

		return 0; //return 0 refuse, return same accept, return diff change
	}
};

} // namespace proxy_mode
} // namespace proxy
} // namespace yggr

#endif //__YGGR_PROXY_PROXY_MODE_PROXY_MODE_TACTICS_HPP__