//ctrl_center_types.hpp

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

#ifndef __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CENTER_TYPES_HPP__
#define __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CENTER_TYPES_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/any_val/any.hpp>
#include <yggr/ids/inner_process_id.hpp>
#include <yggr/utility/args_holder.hpp>

#include <typeinfo>

namespace yggr
{
namespace system_controller
{
namespace detail
{

struct ctrl_center_types_def
{
	typedef u32 code_type;
	typedef u64 ctrl_id_type;

	typedef yggr::ids::inner_process_id modules_id_type;
	typedef const std::type_info* handler_id_type;

	typedef utility::basic_args_holder param_type;

	typedef ::yggr::any handler_wrap_type;

	typedef utility::bad_args_holder_cast bad_cast_type;

};

} // namespace detail
} // namespace system_contrller
} // namespace yggr

#endif // __YGGR_SYSTEM_CONTROLLER_DETAIL_CTRL_CENTER_TYPES_HPP__
