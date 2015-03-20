//lzma_props.cpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

//std::size_t lzma_props::_prop_size = 5;

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

bool lzma_props::operator==(const this_type& right) const
{
	const base_type& l = *this;
	const base_type& r = *this;
	return 0 == memcmp(&l[0], &r[0], E_byte_length);
}

bool lzma_props::operator!=(const this_type& right) const
{
	const base_type& l = *this;
	const base_type& r = *this;
	return 0 != memcmp(&l[0], &r[0], E_byte_length);
}

void lzma_props::swap(this_type& right)
{
	base_type::swap(right);
}

const std::size_t lzma_props::prop_size(void) const
{
	return std::size_t(5);
}

//std::size_t& lzma_props::prop_size(void)
//{
//	assert(this_type::_prop_size == 5);
//	return this_type::_prop_size;
//}

///*static*/ const std::size_t& lzma_props::s_c_prop_size(void)
//{
//	assert(this_type::_prop_size == 5);
//	return this_type::_prop_size;
//}

/*static*/ const std::size_t lzma_props::s_prop_size(void)
{
	return std::size_t(5);
}

} // namespace compaction_tool
} // namespace yggr

#define YGGR_COMPACTION_TOOL_LZMA_PROPS_SWAP_IMPL() \
	void swap(yggr::compaction_tool::lzma_props& l, \
			yggr::compaction_tool::lzma_props& r) { \
	l.swap(r); }

namespace std
{
	YGGR_COMPACTION_TOOL_LZMA_PROPS_SWAP_IMPL()
} // namespace std

namespace boost
{
	YGGR_COMPACTION_TOOL_LZMA_PROPS_SWAP_IMPL()
} // namespace boost

#undef YGGR_COMPACTION_TOOL_LZMA_PROPS_SWAP_IMPL