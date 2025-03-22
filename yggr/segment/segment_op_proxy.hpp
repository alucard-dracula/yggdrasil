//segment_op_proxy.hpp

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

#ifndef __YGGR_SEGMENT_SEGMENT_OP_PROXY_HPP__
#define __YGGR_SEGMENT_SEGMENT_OP_PROXY_HPP__

namespace yggr
{
namespace segment
{

template<typename Seg>
class segment_op_proxy;

//member
/*
*	create
*	remove
*	is_empty
*	size
*/

} // namespace segment
} // namespace yggr

#define YGGR_SEGMENT_OPERATOR_INCLUDE

//named_enable
#include <yggr/segment/shared_memory_object_op_proxy.hpp>
#include <yggr/segment/managed_shared_memory_op_proxy.hpp>
#include <yggr/segment/file_mapping_op_proxy.hpp>
#include <yggr/segment/managed_mapped_file_op_proxy.hpp>
#include <yggr/segment/message_queue_op_proxy.hpp>

//named_disable
#include <yggr/segment/managed_heap_memory_op_proxy.hpp>
#include <yggr/segment/managed_external_buffer_op_proxy.hpp>
#include <yggr/segment/anonymous_shared_memory_op_proxy.hpp>

#endif // __YGGR_SEGMENT_SEGMENT_OP_PROXY_HPP__
