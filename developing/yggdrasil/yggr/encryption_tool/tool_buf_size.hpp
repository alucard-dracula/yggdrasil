//tool_buf_size.hpp

#ifndef __YGGR_ENCRYPTION_TOOL_TOOL_BUF_SIZE_HPP__
#define __YGGR_ENCRYPTION_TOOL_TOOL_BUF_SIZE_HPP__

namespace yggr
{
namespace encryption_tool
{

template<typename Tool>
struct tool_buf_size
{
	enum
	{
		size = 0,
		E_compile_u32 =0xffffffff
	};
};

} // naemspace encryption_tool
} // namespace yggr

#define YGGR_ENCRYPTION_TOOL_TOOL_BUF_SIZE(__type__, __size__) \
	namespace yggr { namespace encryption_tool { \
		template<> struct tool_buf_size< __type__ > { \
			enum { size = __size__ }; }; }}

#define YGGR_ENCRYPTION_TOOL_DEFED_TOOL_BUF_SIZE(__type__, __def__) \
	namespace yggr { namespace encryption_tool { \
		template<> struct tool_buf_size< __type__ > { \
			enum { size = __type__::__def__ }; }; }}

#endif // __YGGR_ENCRYPTION_TOOL_TOOL_BUF_SIZE_HPP__