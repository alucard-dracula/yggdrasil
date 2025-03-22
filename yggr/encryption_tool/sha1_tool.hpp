//sha1_tool.hpp

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

#ifndef __YGGR_ENCRYPTION_TOOL_SHA1_TOOL_HPP__
#define __YGGR_ENCRYPTION_TOOL_SHA1_TOOL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/base/random.h>

#include <yggr/bind/bind.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/ppex/swap_this_def.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/container/array.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/hex_basic_converter.hpp>
#include <yggr/charset/string_converter.hpp>

#include <yggr/smart_ptr_ex/native_array_deleter.hpp>
#include <yggr/encryption_tool/tool_buf_size.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#include <memory>
#include <sstream>
#include <iomanip>
#include <algorithm>

#include <sha.h>

namespace yggr
{
namespace encryption_tool
{

class sha1_tool
{
public:
	YGGR_STATIC_CONSTANT(u32, E_SHA1_SIZE = sha1_size::buf_size);
	YGGR_STATIC_CONSTANT(u32, E_SHA1_STRING_BUF_SIZE = sha1_size::buf_size << 1); // sha1_size::buf_size * 2
	YGGR_STATIC_CONSTANT(u32, E_SHA1_STRING_LEAST_SIZE = E_SHA1_STRING_BUF_SIZE + 1);
	
	typedef u8 value_type;
	typedef ::yggr::shared_ptr<value_type> key_type;

	typedef ::yggr::array<value_type, E_SHA1_SIZE> sig_type;
	typedef sig_type check_type;

private:
	typedef sha1_tool this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	sha1_tool(void)
	{
	}

	template<typename Key>
	sha1_tool(const Key& key)
	{
	}

	sha1_tool(BOOST_RV_REF(this_type) right)
	{
	}

	sha1_tool(const this_type& right)
	{
	}

	~sha1_tool(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
	}

	inline void swap(this_type& right) 
	{
	}

public:
	inline void init(void) {};

	template<typename Oth_Key> inline
	void init(const Oth_Key& oth_key)
	{
	}

	inline std::pair<key_type, u32> get_key(void) const
	{
		return std::pair<key_type, u32>(key_type(), 0);
	}

	template<typename Buffer> inline
	sig_type encrypt(const Buffer& buf) const
	{
		typedef Buffer buf_type;
		typedef typename buf_type::value_type buf_val_type;

		SHA1_DATA sd1;
		sig_type sig = {0};

		static const buf_val_type empty_buf[] = { buf_val_type() };
		const buf_val_type* pbuf = buf.size()? &buf[0] : &empty_buf[0];

		SHA1(&sd1, pbuf, buf.size());
		SHA1_Copy(reinterpret_cast<u8*>(&sig[0]), &sd1);

		return sig;
	}

public:
	YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC size_type sha_size(void)
	{
		return E_SHA1_SIZE;
	}

	YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC size_type check_size(void)
	{
		return E_SHA1_SIZE;
	}

	YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC size_type sig_size(void)
	{
		return E_SHA1_SIZE;
	}

	YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC size_type string_buffer_size(void)
	{
		return E_SHA1_STRING_BUF_SIZE;
	}

	YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC size_type string_buffer_least(void)
	{
		return E_SHA1_STRING_LEAST_SIZE;
	}

	// sig_to_code
	inline static yggr::string sig_to_code(const sig_type& sig, 
											u32 mode = charset::hex_basic_converter::E_Mode_Default, 
											const yggr::string& dst_charset_name = yggr::string())
	{
		yggr::string tcode(E_SHA1_STRING_BUF_SIZE, 0);
		return charset::hex_basic_converter::s_hex_to_string(&tcode[0], &sig[0], E_SHA1_SIZE, mode)?
				tcode : yggr::string();
	}

	template<typename Code> inline
	static Code sig_to_code(const sig_type& sig, u32 mode = charset::hex_basic_converter::E_Mode_Default,
								const yggr::string& dst_charset_name = charset::charset_name_t<Code>())
	{
		typedef Code code_type;

		if(dst_charset_name == charset::charset_name_t<code_type>())
		{
			code_type tcode;
			yggr::resize(tcode, E_SHA1_STRING_BUF_SIZE, 0);
			return charset::hex_basic_converter::s_hex_to_string(&tcode[0], &sig[0], E_SHA1_SIZE, mode)?
					tcode : code_type();
		}
		else
		{
			return charset::string_converter::s_conv<code_type>(
					this_type::sig_to_code(sig, mode, dst_charset_name/*the dst_charset_name not used*/), 
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::string), 
					dst_charset_name);
		}
	}

	template<typename Code, typename Alloc> inline
	static
	typename 
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			Code
		>::type
		sig_to_code(const sig_type& sig, const Alloc& alloc,
						u32 mode = charset::hex_basic_converter::E_Mode_Default,
						const yggr::string& dst_charset_name = charset::charset_name_t<Code>())
	{
		typedef Code code_type;

		if(dst_charset_name == charset::charset_name_t<code_type>())
		{
			code_type tcode(alloc);
			yggr::resize(tcode, E_SHA1_STRING_BUF_SIZE, 0);
			return charset::hex_basic_converter::s_hex_to_string(&tcode[0], &sig[0], E_SHA1_SIZE, mode)?
					tcode : code_type(alloc);
		}
		else
		{
			return charset::string_converter::s_conv<code_type>(
					this_type::sig_to_code(sig, mode, dst_charset_name/*the dst_charset_name not used*/), 
					YGGR_STR_STRING_DEFAULT_CHARSET_NAME(yggr::string), 
					dst_charset_name);
		}
	}

	// code_to_sig
	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static typename boost::enable_if<is_native_char<Char>, bool>::type
		code_to_sig(sig_type& sig, const Basic_String<Char, Traits, Alloc>& code,
						const yggr::string& src_charset_name 
							= charset::charset_name_t< Basic_String<Char, Traits, Alloc> >())
	{
		typedef Basic_String<Char, Traits, Alloc> code_type;	

		if(src_charset_name == charset::charset_name_t< code_type >())
		{
			return
				(!(code.size() < E_SHA1_STRING_BUF_SIZE))
				&& (!!(charset::hex_basic_converter::s_string_to_hex(
						&sig[0], code.data(), E_SHA1_STRING_BUF_SIZE)));
		}
		else
		{
			yggr::string tcode;
			charset::string_converter::s_conv(tcode, code, src_charset_name);

			return 
				(!(tcode.size() < E_SHA1_STRING_BUF_SIZE))
				&& (!!(charset::hex_basic_converter::s_string_to_hex(
						&sig[0], tcode.data(), E_SHA1_STRING_BUF_SIZE)));
		}
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static typename boost::enable_if<is_native_char<Char>, std::pair<sig_type, bool> >::type
		code_to_sig(const Basic_String<Char, Traits, Alloc>& code,
					const yggr::string& src_charset_name 
						= charset::charset_name_t< Basic_String<Char, Traits, Alloc> >())
	{
		std::pair<sig_type, bool> ret;
		ret.second = this_type::code_to_sig(ret.first, code, src_charset_name);
		return ret;
	}

};

} // namespace encryption_tool
} // namespace yggr

namespace yggr
{
namespace encryption_tool
{
namespace swap_support
{

	YGGR_PP_SWAP_THIS_REF_DEF(sha1_tool)

} // namespace swap_support

using swap_support::swap;

} // namespace encryption_tool
} // namesapce yggr

namespace std
{
	using ::yggr::encryption_tool::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::encryption_tool::swap_support::swap;
} // namespace boost

#endif //__YGGR_ENCRYPTION_TOOL_SHA1_TOOL_HPP__
