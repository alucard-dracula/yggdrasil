//system_code.hpp

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

#ifndef __YGGR_SYSTEM_CONTROLLER_SYSTEM_CODE_HPP__
#define __YGGR_SYSTEM_CONTROLLER_SYSTEM_CODE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
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
	YGGR_STATIC_CONSTANT(u32, E_SYSTEM_CODE = 0x00006000);

	YGGR_STATIC_CONSTANT(u32, E_IO_SERVICE_POOL = E_SYSTEM_CODE + 1);

	YGGR_STATIC_CONSTANT(u32, E_TCP_SERVER_SYSTEM = E_SYSTEM_CODE + 2);
	YGGR_STATIC_CONSTANT(u32, E_UDP_SERVER_SYSTEM = E_SYSTEM_CODE + 3);

	YGGR_STATIC_CONSTANT(u32, E_TCP_CLIENT_SYSTEM = E_SYSTEM_CODE + 4);
	YGGR_STATIC_CONSTANT(u32, E_UDP_CLIENT_SYSTEM = E_SYSTEM_CODE + 5);

	YGGR_STATIC_CONSTANT(u32, E_TCP_BASE_NETWORK_SYSTEM = E_SYSTEM_CODE + 6);
	YGGR_STATIC_CONSTANT(u32, E_UDP_BASE_NETWORK_SYSTEM = E_SYSTEM_CODE + 7);

	YGGR_STATIC_CONSTANT(u32, E_SESSION_SYSTEM = E_SYSTEM_CODE + 8);
	YGGR_STATIC_CONSTANT(u32, E_PROXY_SYSTEM = E_SYSTEM_CODE + 9);
	YGGR_STATIC_CONSTANT(u32, E_CONNECT_GROUP_SYSTEM = E_SYSTEM_CODE + 10);
	YGGR_STATIC_CONSTANT(u32, E_FILE_SYSTEM = E_SYSTEM_CODE + 11);
	YGGR_STATIC_CONSTANT(u32, E_DATABASE_SYSTEM = E_SYSTEM_CODE + 12);
	YGGR_STATIC_CONSTANT(u32, E_NSQL_DATABASE_SYSTEM = E_SYSTEM_CODE + 13);
	YGGR_STATIC_CONSTANT(u32, E_SCRIPT_SYSTEM = E_SYSTEM_CODE + 14);
	YGGR_STATIC_CONSTANT(u32, E_ARCHIVE_SYSTEM = E_SYSTEM_CODE + 15);

	YGGR_STATIC_CONSTANT(u32, E_SYS_CODE_END = E_SYSTEM_CODE + 0x00001000 - 1); //0x00006fff
	YGGR_STATIC_CONSTANT(u32, E_SYS_CODE_USER = E_SYSTEM_CODE + 0x00001000); //0x00007000

};

} // namespace system_controller
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif  //_MSC_VER

#endif // __YGGR_SYSTEM_CONTROLLER_SYSTEM_CODE_HPP__