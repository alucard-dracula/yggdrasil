//lzma_foo.hpp

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

#ifndef __YGGR_COMPACTION_TOOL_LZMA_FOO_HPP__
#define __YGGR_COMPACTION_TOOL_LZMA_FOO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/compaction_tool/lzma_include_helper.hpp>
#include <yggr/compaction_tool/lzma_props.hpp>

#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace compaction_tool
{

struct lzma_foo
{
	template<typename BufferDest, typename BufferSrc> inline
	static u32 compress(BufferDest& dest, std::size_t& destlen, const BufferSrc& src, std::size_t srclen,
								lzma_props& out_props, std::size_t& out_props_size, s32 level = 5,
								std::size_t dict_size = 1 << 24, s32 lc = 3, s32 lp = 0, s32 pb = 2,
								s32 fb = 32, s32 num_thread = 2)
	{
		return 
			LzmaCompress(reinterpret_cast<u8*>(&dest[0]), &destlen, 
							reinterpret_cast<const u8*>(&src[0]), srclen, 
							&out_props[0], &out_props_size,
							level, dict_size, lc, lp, pb, fb, num_thread);
	}

	template<typename BufferDest, typename BufferSrc> inline
	static u32 uncompress(BufferDest& dest, std::size_t& destlen, const BufferSrc& src, std::size_t& srclen,
							const lzma_props& props, std::size_t props_size = lzma_props::E_length)
	{
		return 
			LzmaUncompress(reinterpret_cast<u8*>(&dest[0]), &destlen, 
							reinterpret_cast<const u8*>(&src[0]), &srclen,
							&props[0], props_size);
	}
};

} // namespace compaction_tool
} // namespace yggr

#endif // __YGGR_COMPACTION_TOOL_LZMA_FOO_HPP__
