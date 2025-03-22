//anonymous_shared_memory.cpp

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

#include <yggr/segment/anonymous_shared_memory.hpp>
#include <boost/interprocess/detail/config_begin.hpp>

#ifdef _MSC_VER
#   include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace segment
{

anonymous_shared_memory::anonymous_shared_memory(void)
{
}

anonymous_shared_memory::anonymous_shared_memory(std::size_t size, void *address)
{
	base_type& base = *this;
	boost::interprocess::anonymous_shared_memory(size, address).swap(base);
}

anonymous_shared_memory::~anonymous_shared_memory(void)
{
}

void anonymous_shared_memory::swap(base_type &base)
{
	base_type::swap(base);
}

void anonymous_shared_memory::swap(this_type& right)
{
	base_type::swap(right);
}

} // namespace segment
} // namespace yggr

#include <boost/interprocess/detail/config_end.hpp>
