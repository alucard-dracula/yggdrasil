//lzma_tool.hpp

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

#ifndef __YGGR_COMPACTION_TOOL_LZMA_TOOL_HPP__
#define __YGGR_COMPACTION_TOOL_LZMA_TOOL_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/compaction_tool/lzma_foo.hpp>
#include <yggr/compaction_tool/lzma_data.hpp>

namespace yggr
{
namespace compaction_tool
{

template<typename Lzma_Data = lzma_data<>,
			yggr::u64 blocksize = 4096,
			s32 level = 5, 
			std::size_t dict_size = (1 << 24),
			s32 lc = 3, 
			s32 lp = 0, 
			s32 pb = 2,
			s32 fb = 32, 
			s32 num_thread = 2
			>
class lzma_tool
{
public:

	typedef Lzma_Data lzma_data_type;
	typedef typename lzma_data_type::size_type size_type;
	typedef typename lzma_data_type::lzma_props_type lzma_props_type;
	typedef typename lzma_data_type::lzma_buf_type lzma_buf_type;

	BOOST_MPL_ASSERT((boost::mpl::bool_<(blocksize >= 1024)>));

private:
	typedef lzma_tool this_type;


//public:
//	inline static u64 s_block_size(void)
//	{
//		return block_size;
//	}

public:
	lzma_tool(void)
	{
	}

	~lzma_tool(void)
	{
	}

	template<typename Packet, typename Buffer>
	bool compress(Buffer& buf) const
	{
		typedef Packet packet_type;
		typedef Buffer buf_type;
		typedef typename buf_type::value_type buf_val_type;
		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(buf_val_type) == 1)>));

		if(buf.empty())
		{
			return false;
		}

		std::size_t src_size = buf.size();
		std::size_t dst_size = ((src_size + blocksize - 1) / blocksize) * blocksize;
		
		buf_type dst_buf(dst_size, buf_val_type());
		lzma_props_type props;
		std::size_t props_size = 5;
		if(SZ_OK != lzma_foo::compress(dst_buf, dst_size, buf, src_size, props, props_size,
										level, dict_size, lc, lp, pb, fb, num_thread))
		{
			return false;
		}

		assert(props_size == 5);
		dst_buf.erase(dst_buf.begin() + dst_size, dst_buf.end());
		
		lzma_data_type lzma_data(src_size, props, yggr::move::move_helper::forced_move(dst_buf));

		packet_type pak;
		if(!pak.save(lzma_data))
		{
			return false;
		}

		buf.swap(pak.org_buf());

		return true;
	}

	template<typename Packet, typename Buffer>
	bool uncompress(Buffer& buf) const
	{
		typedef Packet packet_type;
		typedef Buffer buf_type;
		typedef typename buf_type::value_type buf_val_type;
		BOOST_MPL_ASSERT((boost::mpl::bool_<(sizeof(buf_val_type) == 1)>));

		if(buf.empty())
		{
			return false;
		}

		packet_type pak(yggr::move::move_helper::forced_move(buf));
		lzma_data_type lzma_data;
		
		if(!pak.load(lzma_data))
		{
			return false;
		}

		std::size_t src_size = lzma_data.buf().size();
		std::size_t dst_size = lzma_data.org_size();
		buf_type dst_buf(dst_size, buf_val_type());

		if(SZ_OK != lzma_foo::uncompress(dst_buf, dst_size, lzma_data.buf(), src_size, lzma_data.props()))
		{
			return false;
		}

		buf.swap(dst_buf);

		return true;
	}
	//处理到这里 
};

} // namespace compaction_tool
} // namespace yggr

#endif // __YGGR_COMPACTION_TOOL_LZMA_TOOL_HPP__