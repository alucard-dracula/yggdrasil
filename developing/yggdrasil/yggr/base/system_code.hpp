//system_code.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_SYSTEM_CONTROLLER_SYSTEM_CODE_HPP__
#define __YGGR_SYSTEM_CONTROLLER_SYSTEM_CODE_HPP__

#include <yggr/nonable/noncreateable.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace system_controller
{

class system_code 
	: private nonable::noncreateable
{
public:
	enum
	{
		E_SYSTEM_CODE = 0x00006000,
		E_IO_SERVICE_POOL,
		E_TCP_SERVER_SYSTEM,
		E_UDP_SERVER_SYSTEM,
		E_TCP_CLIENT_SYSTEM,
		E_UDP_CLIENT_SYSTEM,
		E_TCP_BASE_NETWORK_SYSTEM,
		E_UDP_BASE_NETWORK_SYSTEM,
		E_SESSION_SYSTEM,
		E_PROXY_SYSTEM,
		E_CONNECT_GROUP_SYSTEM,
		E_FILE_SYSTEM,
		E_DATABASE_SYSTEM,
		E_NSQL_DATABASE_SYSTEM,
		E_SCRIPT_SYSTEM,
		E_ARCHIVE_SYSTEM,

		E_SYS_CODE_END,
		E_SYS_CODE_USER = E_SYS_CODE_END + 2,

		E_compile_u32 = 0xffffffff
	};
};

} // namespace system_controller
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif  //_MSC_VER

#endif // __YGGR_SYSTEM_CONTROLLER_SYSTEM_CODE_HPP__