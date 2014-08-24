//proxy_mode_def.hpp

#ifndef __YGGR_PROXY_PROXY_MODE_PROXY_MODE_DEF_HPP__
#define __YGGR_PROXY_PROXY_MODE_PROXY_MODE_DEF_HPP__

#include <boost/mpl/vector_c.hpp>
#include <yggr/base/yggrdef.h>


namespace yggr
{
namespace proxy
{
namespace proxy_mode
{

struct proxy_mode_def
{
	enum
	{
		E_proxy_mode_failed = 0,
		E_proxy_mode_monopolize,
		E_proxy_mode_source_hash,
		E_proxy_mode_blanace,
		E_proxy_mode_all,

		E_proxy_mode_extern,

		E_compile_u32 = 0xffffffff
	};
};

} // namespace proxy_mode
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_MODE_PROXY_MODE_DEF_HPP__