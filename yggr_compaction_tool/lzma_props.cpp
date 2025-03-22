//lzma_props.cpp

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

#include <yggr/compaction_tool/lzma_props.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace compaction_tool
{

lzma_props::lzma_props(void)
{
	base_type& base = *this;
	memset(&base[0], 0, E_byte_length);
}

lzma_props::lzma_props(const this_type& right)
	: base_type(right)
{
}

lzma_props::~lzma_props(void)
{
}

lzma_props::this_type& lzma_props::operator=(const this_type& right)
{
	if(this == &right) {return *this;}
	base_type& base = *this;
	base = right;
	return *this;
}

void lzma_props::swap(this_type& right)
{
	if(this == &right) 
	{
		return;
	}

	base_type::swap(right);
}

} // namespace compaction_tool
} // namespace yggr

