// safe_packet_tool.hpp

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

#ifndef __YGGR_ENCRYPTION_TOOL_SAFE_PACKET_TOOL_HPP__
#define __YGGR_ENCRYPTION_TOOL_SAFE_PACKET_TOOL_HPP__

#include <yggr/base/yggrdef.h>
#include <cassert>
#include <yggr/encryption_tool/tool_type_check.hpp>


namespace yggr
{
namespace encryption_tool
{

template<typename Encryption_Tool, typename Check_Tool>
class safe_packet_tool
{
private:
	typedef Encryption_Tool encryption_tool_type;
	BOOST_MPL_ASSERT((is_reduction_tool<encryption_tool_type>));
	typedef Check_Tool check_tool_type;
	typedef typename check_tool_type::check_type check_type;
	typedef typename check_type::value_type check_val_type;

	typedef safe_packet_tool this_type;

public:
	typedef typename encryption_tool_type::key_type key_type;
public:
	safe_packet_tool(void)
	{
	}

	safe_packet_tool(const this_type& right)
		: _encrypt_tool(right._encrypt_tool)
	{
	}

	~safe_packet_tool(void)
	{
	}

	void init(void)
	{
		_encrypt_tool.init();
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void init(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		_encrypt_tool.init( YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME


	std::pair<key_type, u32> get_key(void) const
	{
		return _encrypt_tool.get_key();
	}

	template<typename Buffer>
	bool encrypt(Buffer& buf) const
	{
		return prv_encrypt<Buffer>(buf);
	}

	template<typename Buffer>
	bool decrypt(Buffer& buf) const
	{
		return prv_decrypt<Buffer>(buf);
	}

protected:
	template<typename Buffer>
	bool prv_encrypt(Buffer& buf) const
	{
		typedef Buffer buf_type;
		typedef typename buf_type::value_type buf_val_type;

		if(buf.empty())
		{
			return false;
		}

		check_tool_type ct;
		check_type chk = ct.encrypt(buf);

		size_type chk_byte_size = chk.size() * sizeof(check_val_type);

		size_type buf_size = buf.size();
		size_type buf_byte_size = buf_size * sizeof(buf_val_type);
		

		assert(chk_byte_size == ct.check_size());
		if(chk_byte_size != ct.check_size())
		{
			return false;
		}

		size_type byte_size = chk_byte_size + buf_byte_size;
		size_type new_buf_size = (byte_size + sizeof(buf_val_type) - 1) / sizeof(buf_val_type);

		buf_type src_buf(new_buf_size, 0);
		memcpy((u8*)&src_buf[0], &chk[0], chk_byte_size);
		memcpy(((u8*)&src_buf[0]) + chk_byte_size, (u8*)&buf[0], buf_byte_size);

		if(!_encrypt_tool.encrypt(src_buf))
		{
			return false;
		}

		buf.swap(src_buf);

		return true;
	}

	template<typename Buffer>
	bool prv_decrypt(Buffer& buf) const
	{
		typedef Buffer buf_type;
		typedef typename buf_type::value_type buf_val_type;

		if(buf.empty())
		{
			return false;
		}

		if(!_encrypt_tool.decrypt(buf))
		{
			return false;
		}

		size_type buf_size = buf.size();
		size_type buf_byte_size = buf_size * sizeof(buf_val_type);

		size_type chk_byte_size = check_tool_type::s_check_size();

		if(buf_byte_size <= chk_byte_size)
		{
			return false;
		}

		size_type data_byte_size = ((buf_byte_size - chk_byte_size) / sizeof(buf_val_type)) * sizeof(buf_val_type);

		check_type chk_buf;
		memcpy(&chk_buf[0], (u8*)&buf[0], chk_byte_size);
		buf_type data_buf((buf_val_type*)((u8*)&buf[0] + chk_byte_size), (buf_val_type*)((u8*)&buf[0] + chk_byte_size + data_byte_size));

		check_tool_type ctr;
		check_type chk = ctr.encrypt(data_buf);

		if(chk_buf != chk)
		{
			return false;
		}

		buf.swap(data_buf);
		return true;
	}

private:
	encryption_tool_type _encrypt_tool;
};

template<typename Encryption_Tool, typename Check_Tool>
struct is_reduction_tool< safe_packet_tool<Encryption_Tool, Check_Tool> >
{
	typedef typename is_reduction_tool<Encryption_Tool>::type type;
};

} // namespace encryption_tool
} // namespace yggr

#endif //__YGGR_ENCRYPTION_TOOL_SAFE_PACKET_TOOL_HPP__