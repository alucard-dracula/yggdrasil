//dtl_error.hpp

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

#ifndef __YGGR_NSQL_DATABASE_DTL_ERROR_HPP__
#define __YGGR_NSQL_DATABASE_DTL_ERROR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/base/error.hpp>
#include <yggr/move/move.hpp>
#include <yggr/exception/exception_sort.hpp>

#include <yggr/database_system/database_string_helper.hpp>

namespace yggr
{
namespace database_system
{

struct dtl_error_code_def
{
	YGGR_STATIC_CONSTANT(u32, E_DTL_DB_ERR_CODE = exception::exception_sort::E_code_dtl_db_error);
	YGGR_STATIC_CONSTANT(u32, E_DTL_ROOT_ERR_CODE = exception::exception_sort::E_code_dtl_root_error);
};

typedef yggr::error dtl_error;

} // namespace database_system
} // namespace yggr

#endif // __YGGR_NSQL_DATABASE_DTL_ERROR_HPP__