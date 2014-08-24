//client_config_template.hpp

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

#ifndef __YGGR_CLIENT_CLIENT_CONFIG_CLIENT_CONFIG_TEMPLATE_HPP__
#define __YGGR_CLIENT_CLIENT_CONFIG_CLIENT_CONFIG_TEMPLATE_HPP__

#include <yggr/base/exception.hpp>
#include <yggr/base/exception_sort.hpp>
#include <yggr/base/error_make.hpp>

namespace yggr
{
namespace client
{
namespace client_config
{

template<typename Tag>
struct client_config_template
{
	typedef Tag tag_type;

	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("client_handler")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_3)
			ERROR_CODE_DEF(E_invalid_access)
			ERROR_CODE_DEF(E_invalid_host_and_port)
			ERROR_CODE_DEF(E_empty_conn)
			ERROR_CODE_DEF(E_invalid_conn_alloc)
			ERROR_CODE_DEF(E_invalid_client)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_3)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_access, "invalid access")
			ERROR_CODE_MSG(E_invalid_host_and_port, "invalid host and port")
			ERROR_CODE_MSG(E_empty_conn, "empty conn")
			ERROR_CODE_MSG(E_invalid_conn_alloc, "connect alloc out of memory")
			ERROR_CODE_MSG(E_invalid_client, "invalid client")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------
};

} // namespace client_config
} // namespace client
} // namespace yggr

#endif // __YGGR_CLIENT_CLIENT_CONFIG_CLIENT_CONFIG_TEMPLATE_HPP__