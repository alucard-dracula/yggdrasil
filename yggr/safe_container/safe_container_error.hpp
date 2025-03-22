// safe_wrap_error.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_WRAP_ERROR_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_WRAP_ERROR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/error_make.hpp>
#include <yggr/exception/exception_sort.hpp>

namespace yggr
{
namespace safe_container
{

struct safe_container_error
{
	ERROR_MAKER_BEGIN("safe_container")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_20)
			ERROR_CODE_DEF(E_element_not_exists)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_20)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_element_not_exists, "element_not_exists")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()
};

} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_SAFE_WRAP_ERROR_HPP__
