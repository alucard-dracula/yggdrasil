// recursive_handlers.hpp

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

#ifndef __YGGR_FILE_SYSTEM_RECURSIVE_HANDLERS_HPP__
#define __YGGR_FILE_SYSTEM_RECURSIVE_HANDLERS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/file_system/local_fsys_native_operators.hpp>

#include <sstream>

namespace yggr
{
namespace file_system
{

struct recursive_copy_empty_notifier
{
	typedef native::path path_type;
	typedef native::error_code error_code_type;

	inline void operator()(const path_type& from,
							const path_type& to,
							const path_type& current_from,
							const path_type& current_to,
							bool current_result,
							boost::uintmax_t current_count,
							error_code_type* current_ec,
							bool continueable) const
	{
		return;
	}
};

typedef recursive_copy_empty_notifier recursive_move_empty_notifier;


struct recursive_remove_empty_notifier
{
	typedef native::path path_type;
	typedef native::error_code error_code_type;

	inline void operator()(const path_type& path,
							const path_type& current_path,
							bool current_result,
							boost::uintmax_t current_count,
							error_code_type* current_ec,
							bool continueable) const
	{
		return;
	}
};

} // namespace file_system
} // namesapce yggr

#endif // __YGGR_FILE_SYSTEM_RECURSIVE_HANDLERS_HPP__
