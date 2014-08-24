//proxy_fix_state_def.hpp

#ifndef __YGGR_PROXY_PROXY_FIX_STATE_DEF_HPP__
#define __YGGR_PROXY_PROXY_FIX_STATE_DEF_HPP__

namespace yggr
{
namespace proxy
{

struct proxy_fix_state_def
{
	enum
	{
		E_accepted = 0,
		E_changed,
		E_refused,
		E_existsed,
		E_failed,

		E_compile_u32 = 0xffffffff
	};
};

} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_FIX_STATE_DEF_HPP__