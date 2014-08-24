//proxy_mode_tactics.hpp

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

	u32 operator()(u32 old_mode, u32 new_mode) const
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
		};

		return 0; //return 0 refuse, return same accept, return diff change
	}
};

} // namespace proxy_mode
} // namespace proxy
} // namespace yggr

#endif //__YGGR_PROXY_PROXY_MODE_PROXY_MODE_TACTICS_HPP__