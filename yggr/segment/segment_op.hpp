//segment_op.hpp

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

#ifndef __YGGR_SEGMENT_SEGMENT_OP_HPP__
#define __YGGR_SEGMENT_SEGMENT_OP_HPP__

#include <yggr/segment/segment_config.hpp> // it must be written in the first sentence include
#include <yggr/nonable/noncreateable.hpp>

#include <boost/interprocess/creation_tags.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace segment
{

class segment_op
    : private nonable::noncreateable
{
public:
	typedef boost::interprocess::create_only_t create_only_type;
	typedef boost::interprocess::open_only_t open_only_type;
	typedef boost::interprocess::open_read_only_t open_read_only_type;
	typedef boost::interprocess::open_read_private_t open_read_private_type;
	typedef boost::interprocess::open_copy_on_write_t open_copy_on_write_type;
	typedef boost::interprocess::open_or_create_t open_or_create_type;

	inline static const create_only_type create_only(void)
	{
		return create_only_type();
	}

	inline static const open_only_type open_only(void)
	{
		return open_only_type();
	}
	
	inline static const open_read_only_type open_read_only(void)
	{
		return open_read_only_type();
	}
	
	inline static const open_or_create_type open_or_create(void)
	{
		return open_or_create_type();
	}

	inline static const open_copy_on_write_type open_copy_on_write(void)
	{
		return open_copy_on_write_type();
	}
};

} // namespace segment
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif //_MSC_VER

#endif // __YGGR_SEGMENT_SEGMENT_OP_HPP__
