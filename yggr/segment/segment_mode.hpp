//segment_mode

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

#ifndef __YGGR_SEGMENT_SEGMENT_MODE_HPP__
#define __YGGR_SEGMENT_SEGMENT_MODE_HPP__

#include <yggr/segment/segment_config.hpp> // it must be written in the first sentence include
#include <yggr/base/static_constant.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <boost/interprocess/detail/os_file_functions.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace segment
{

class segment_mode 
	: private nonable::noncreateable
{
public:
	YGGR_STATIC_CONSTANT(u32, E_read_only = boost::interprocess::read_only);
	YGGR_STATIC_CONSTANT(u32, E_read_write = boost::interprocess::read_write);
	YGGR_STATIC_CONSTANT(u32, E_copy_on_write = boost::interprocess::copy_on_write);
	YGGR_STATIC_CONSTANT(u32, E_read_private = boost::interprocess::read_private);
	YGGR_STATIC_CONSTANT(u32, E_invalid_mode = boost::interprocess::invalid_mode);
};

} // namespace segment
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif // _MSC_VER

#endif // __YGGR_SEGMENT_SEGMENT_MODE_HPP__
