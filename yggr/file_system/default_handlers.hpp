//default_handlers.hpp

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

#ifndef __YGGR_FILE_SYSTEM_DEFAULT_HANDLERS_HPP__
#define __YGGR_FILE_SYSTEM_DEFAULT_HANDLERS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/file_system/rename_handlers.hpp>
#include <yggr/file_system/recursive_handlers.hpp>

namespace yggr
{
namespace file_system
{

template<typename HRename, 
			typename HRecursive_Copy_Notify = recursive_copy_empty_notifier,
			typename HRecursive_Move_Notify = recursive_move_empty_notifier,
			typename HRecursive_Remove_Notify = recursive_remove_empty_notifier >
struct basic_default_handlers
{
	typedef native::path path_type;

	typedef HRename rename_handler_type;
	typedef HRecursive_Copy_Notify recursive_copy_notify_handler_type;
	typedef HRecursive_Move_Notify recursive_move_notify_handler_type;
	typedef HRecursive_Remove_Notify recursive_remove_notify_handler_type;
};	

typedef basic_default_handlers< rename_of_count<rename_ext_bak> > default_handlers;

} // namespace file_system
} // namespace yggr

#endif // __YGGR_FILE_SYSTEM_DEFAULT_HANDLERS_HPP__
