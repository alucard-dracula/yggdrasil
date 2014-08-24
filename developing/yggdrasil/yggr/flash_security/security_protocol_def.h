//security_protocol_def.h

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

#ifndef __YGGR_FLASH_SECURITY_SECURITY_PROTOCOL_DEF_H__
#define __YGGR_FLASH_SECURITY_SECURITY_PROTOCOL_DEF_H__

#include <yggr/nonable/noncreateable.hpp>
#include <yggr/base/yggrdef.h>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace flash_security
{

class security_protocol_def 
	: private yggr::nonable::noncreateable
{
public:
	enum
	{
		E_Request_Length = 23,
		E_link_port = 843,
		E_compile_u32 = 0xffffffff
	};
	
public:
	static inline u32 request_length(void)
	{
		return E_Request_Length;
	}

	static inline u16 link_port(void)
	{
		return E_link_port;
	}
};

} // namespace flash_security
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER


#endif //__YGGR_FLASH_SECURITY_SECURITY_PROTOCOL_DEF_H__