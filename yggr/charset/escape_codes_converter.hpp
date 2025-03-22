// escape_codes_converter.hpp

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

#ifndef __YGGR_CHARSET_ESCAPE_CODES_CONVERTER_HPP__
#define __YGGR_CHARSET_ESCAPE_CODES_CONVERTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/move/move.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/container_ex/is_container.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/bytes/byte_size_sp_basic.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/resize.hpp>
#include <yggr/container/clear.hpp>
#include <yggr/container/get_allocator.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/escape_codes_basic_converter.hpp>

#include <boost/range/functions.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <cassert>

namespace yggr
{
namespace charset
{
namespace detail
{

// hex_to_string_helper
template<typename HexBufIter, bool is_rnd>
struct escape_bytes_helper;

template<typename HexBufIter>
struct escape_bytes_helper<HexBufIter, false>
{
public:
	typedef HexBufIter hex_buf_iter_type;
	typedef typename boost::iterator_value<hex_buf_iter_type>::type hex_buf_value_type;

	YGGR_STATIC_CONSTANT(u32, E_char_step = (sizeof(hex_buf_value_type) * 3));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(E_char_step > 0)>));

private:
	typedef escape_bytes_helper this_type;

public:
	template<typename String>
	String& operator()(String& str, hex_buf_iter_type s, hex_buf_iter_type e, 
						typename range_ex::range_value_ex<String>::type sep, u32 mode,
						const string& dst_charset_name) const
	{
		typedef String string_type;
		typedef typename boost::range_value<string_type>::type string_val_type;

		size_type byte_size = bytes_sp::byte_size(s, e);
		assert(byte_size == static_cast<size_type>(std::distance(s, e)) * sizeof(hex_buf_value_type));
		yggr::clear(str);

		if(byte_size)
		{
			if(dst_charset_name == charset_name_t< string_type >())
			{
				yggr::resize(str, byte_size * 3, 0);
				string_val_type* str_i = &(*boost::begin(str));

				for(; s != e; ++s, str_i += E_char_step)
				{
					escape_codes_basic_converter::s_escape_bytes(
						str_i, reinterpret_cast<const u8*>(&(*s)), 
						sizeof(hex_buf_value_type), sep, mode);
				}
			}
			else
			{
				assert(static_cast<u32>(sep) <= static_cast<u32>(0x000000ff));
				yggr::utf8_string tstr;
				tstr.resize(byte_size * 3);
				assert(tstr.size() == tstr.length());
				yggr::utf8_string::value_type* str_i = &(tstr.org_str()[0]);

				for(; s != e; ++s, str_i += E_char_step)
				{
					escape_codes_basic_converter::s_escape_bytes(
						str_i, reinterpret_cast<const u8*>(&(*s)), 
						sizeof(hex_buf_value_type), static_cast<char>(sep), mode);
				}

				str = tstr.str<string_type>(yggr::get_allocator(str), dst_charset_name);
			}
		}

		return str;
	}

	template<typename String> inline
	String operator()(hex_buf_iter_type s, hex_buf_iter_type e, 
						typename range_ex::range_value_ex<String>::type sep, u32 mode,
						const string& dst_charset_name) const
	{
		typedef String string_type;

		string_type ret;
		// must using this->
		return this->operator()(ret, s, e, mode, dst_charset_name);
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		operator()(hex_buf_iter_type s, hex_buf_iter_type e,
					typename range_ex::range_value_ex<String>::type sep, 
					const Alloc& alloc,
					u32 mode,
					const string& dst_charset_name = charset_name_t<String>()) const
	{
		typedef String string_type;
		string_type ret(alloc);
		// must using this->
		return this->operator()(ret, s, e, mode, dst_charset_name);
	}
};

template<typename HexBufIter>
struct escape_bytes_helper<HexBufIter, true>
{
public:
	typedef HexBufIter hex_buf_iter_type;
	typedef typename boost::iterator_value<hex_buf_iter_type>::type hex_buf_value_type;

private:
	typedef escape_bytes_helper this_type;

public:
	template<typename String>
	String& operator()(String& str,
						hex_buf_iter_type s, hex_buf_iter_type e, 
						typename range_ex::range_value_ex<String>::type sep, u32 mode,
						const string& dst_charset_name = charset_name_t<String>()) const
	{
		typedef String string_type;
		typedef typename boost::range_value<string_type>::type string_val_type;

		size_type byte_size = bytes_sp::byte_size(s, e);
		assert(byte_size == static_cast<size_type>(std::distance(s, e)) * sizeof(hex_buf_value_type));
		yggr::clear(str);

		if(byte_size)
		{
			if(dst_charset_name == charset_name_t< string_type >())
			{
				yggr::resize(str, byte_size * 3, 0);
				string_val_type* str_i = &(*boost::begin(str));

				escape_codes_basic_converter::s_escape_bytes(
					str_i, reinterpret_cast<const u8*>(&(*s)), byte_size, sep, mode);
			}
			else
			{
				assert(static_cast<u32>(sep) <= static_cast<u32>(0x000000ff));
				yggr::utf8_string tstr;
				tstr.resize(byte_size * 3);
				assert(tstr.size() == tstr.length());
					
				yggr::utf8_string::value_type* str_i = &(tstr.org_str()[0]);

				escape_codes_basic_converter::s_escape_bytes(
					str_i, reinterpret_cast<const u8*>(&(*s)), byte_size, static_cast<char>(sep), mode);
				str = tstr.str<string_type>(yggr::get_allocator(str), dst_charset_name);
			}
		}

		return str;
	}

	template<typename String> inline
	String operator()(hex_buf_iter_type s, hex_buf_iter_type e, 
						typename range_ex::range_value_ex<String>::type sep, u32 mode,
						const string& dst_charset_name = charset_name_t<String>()) const
	{
		typedef String string_type;
		typedef typename boost::range_value<string_type>::type string_val_type;

		string_type ret;
		// must using this->
		return this->operator()(ret, s, e, sep, mode, dst_charset_name);
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		operator()(hex_buf_iter_type s, hex_buf_iter_type e, 
					typename range_ex::range_value_ex<String>::type sep,
					const Alloc& alloc,
					u32 mode,
					const string& dst_charset_name = charset_name_t<String>()) const
	{
		typedef String string_type;

		string_type ret(alloc);
		// must using this->
		return this->operator()(ret, s, e, sep, mode, dst_charset_name);
	}
};

// string_to_hex_base_helper
template<typename HexBuf>
struct unescape_bytes_base_helper
{
protected:
	typedef HexBuf hex_buf_type;
	typedef typename boost::range_value<hex_buf_type>::type hex_buf_value_type;

protected:
	// return a byte_size
	template<typename IHexBuf, typename Char> inline
	typename
		boost::enable_if
		<
			boost::is_same<hex_buf_value_type, typename boost::range_value<IHexBuf>::type>,
			std::size_t
		>::type
		exec_conv(IHexBuf& buf, const Char* p, size_type n, Char sep) const
	{
		// needed buf_space = upper(n * sizeof(Char) / sizeof(hex_buf_value_type)))
		// -> (n * sizeof(Char) + sizeof(hex_buf_value_type) - 1) / (sizeof(hex_buf_value_type)) // opt upper

		typedef Char char_type;

		yggr::resize(
			buf, 
			(n * sizeof(char_type) + sizeof(hex_buf_value_type) - 1) / sizeof(hex_buf_value_type), 
			hex_buf_value_type());

		std::size_t conv_byte_size = 
			escape_codes_basic_converter::s_unescape_bytes(
				reinterpret_cast<u8*>(&(*boost::begin(buf))), p, n, sep);

		std::size_t buf_demand_size = (conv_byte_size + sizeof(hex_buf_value_type) - 1) / sizeof(hex_buf_value_type);
		std::size_t buf_size = ::yggr::size(buf);

		assert(buf_demand_size <= buf_size);
		if(buf_demand_size < buf_size)
		{
			yggr::resize(buf, buf_demand_size);
		}

		return conv_byte_size;
	}
};

// string_to_hex_helper
template<typename HexBuf, bool is_rnd>
struct unescape_bytes_helper;

template<typename HexBuf>
struct unescape_bytes_helper<HexBuf, false>
	: protected unescape_bytes_base_helper<HexBuf>
{
protected:
	typedef unescape_bytes_base_helper<HexBuf> base_type;

public:
	typedef typename base_type::hex_buf_type hex_buf_type;
	typedef typename base_type::hex_buf_value_type hex_buf_value_type;
	typedef yggr::vector<hex_buf_value_type> tmp_hex_buf_type;

	template<typename Char> inline
	std::size_t operator()(hex_buf_type& buf,
							const Char* p, size_type n, Char sep,
							const string& src_charset_name) const
	{
		assert(p);
		yggr::clear(buf);

		if(!(p && n))
		{
			return 0;
		}
		else
		{
			tmp_hex_buf_type tmp;
			std::size_t conv_size = 0;
			if(src_charset_name == charset_name_t<Char>())
			{
				conv_size = base_type::exec_conv(tmp, p, n, sep);
			}
			else
			{
				// why using this conv
				// because
				// if the current src_charset_name != defaulet_charset_name,
				//	then it is likely that the storage space will be different
				assert(static_cast<u32>(sep) <= static_cast<u32>(0x000000ff));
				yggr::utf8_string str(p, n, src_charset_name);

				conv_size = base_type::exec_conv(tmp, str.data(), str.size(), static_cast<char>(sep));
			}

			if(conv_size)
			{
				std::copy(tmp.begin(), tmp.end(), iterator_ex::inserter(buf, boost::end(buf)));
			}

			return conv_size;
		}
	}
};

template<typename HexBuf>
struct unescape_bytes_helper<HexBuf, true>
	: protected unescape_bytes_base_helper<HexBuf>
{
protected:
	typedef unescape_bytes_base_helper<HexBuf> base_type;

public:
	typedef typename base_type::hex_buf_type hex_buf_type;
	typedef typename base_type::hex_buf_value_type hex_buf_value_type;

	template<typename Char> inline
	std::size_t operator()(hex_buf_type& buf, 
							const Char* p, size_type n, Char sep,
							const string& src_charset_name) const
	{
		assert(p);
		yggr::clear(buf);

		if(!(p && n))
		{
			return 0;
		}
		else
		{
			if(src_charset_name == charset_name_t<Char>())
			{
				return base_type::exec_conv(buf, p, n, sep);
			}
			else
			{
				assert(static_cast<u32>(sep) <= static_cast<u32>(0x000000ff));
				yggr::utf8_string str(p, n, src_charset_name);

				return base_type::exec_conv(buf, str.data(), str.size(), static_cast<char>(sep));
			}
		}
	}
};

} // namespace detail
} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

class escape_codes_converter
	: public escape_codes_basic_converter
{
public:
	typedef escape_codes_basic_converter base_type;

private:
	typedef escape_codes_converter this_type;

public:
	using base_type::SEPUrl;
	using base_type::SEPStringLiteral;

public:
	// s_escape_byte
	using base_type::s_escape_byte;

	template<typename String> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<String> >,
				boost::is_class< String >
			>,
			String&
		>::type
		s_escape_byte(String& str, u8 val, typename range_ex::range_value_ex<String>::type sep, 
						u32 mode = base_type::E_Mode_Default,
						const string& dst_charset_name = charset_name_t<String>())
	{
		typedef String ret_type;
		typedef typename range_ex::range_value_ex<ret_type>::type ret_value_type;

		yggr::clear(str);

		if(dst_charset_name == charset_name_t< ret_type >())
		{
			container::resize(str, 3, 0);
			ret_value_type* str_i = &(*boost::begin(str));
			base_type::s_escape_byte(str_i, val, sep, mode);
		}
		else
		{
			assert(static_cast<u32>(sep) <= static_cast<u32>(0x000000ff));
			yggr::utf8_string tstr;
			tstr.resize(3);
			assert(tstr.size() == tstr.length());
			yggr::utf8_string::value_type* str_i = &(tstr.org_str()[0]);

			base_type::s_escape_byte(str_i, val, static_cast<char>(sep), mode);
			str = tstr.str<ret_type>(yggr::get_allocator(str), dst_charset_name);
		}

		return str;
	}

	template<typename String> inline
	static 
	String s_escape_byte(u8 val, typename range_ex::range_value_ex<String>::type sep, 
							u32 mode = base_type::E_Mode_Default,
							const string& dst_charset_name = charset_name_t<String>())
	{
		String tmp;
		this_type::s_escape_byte(tmp, val, sep, mode, dst_charset_name);
		return tmp;
	}

	template<typename String, typename Alloc> inline
	static 
	typename 
		boost::enable_if
		<
			container_ex::is_allocator<Alloc>,
			String
		>::type
		s_escape_byte(u8 val, typename range_ex::range_value_ex<String>::type sep, 
						const Alloc& alloc,
						u32 mode = base_type::E_Mode_Default,
						const string& dst_charset_name = charset_name_t<String>())
	{
		String tmp(alloc);
		this_type::s_escape_byte(tmp, val, sep, mode, dst_charset_name);
		return tmp;
	}

public:
	using base_type::s_escape_bytes;

	template<typename String, typename HexBuf> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<String> >,
				container_ex::is_container<HexBuf>
			>,
			String&
		>::type
		s_escape_bytes(String& str,
						const HexBuf& hex_buf, 
						typename range_ex::range_value_ex<String>::type sep,
						u32 mode = base_type::E_Mode_Default,
						const string& dst_charset_name = charset_name_t<String>())
	{
		typedef HexBuf cv_hex_buf_type;
		typedef typename boost::remove_cv<cv_hex_buf_type>::type hex_buf_type;
		typedef typename boost::range_iterator<const hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::escape_bytes_helper
			<
				hex_buf_iter_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
		return h(str, boost::begin(hex_buf), boost::end(hex_buf), sep, mode, dst_charset_name);
	}

	template<typename String, typename HexBuf> inline
	static 
	typename 
		boost::enable_if
		<
			container_ex::is_container<HexBuf>,
			String
		>::type
		s_escape_bytes(const HexBuf& hex_buf, 
						typename range_ex::range_value_ex<String>::type sep,
						u32 mode = base_type::E_Mode_Default,
						const string& dst_charset_name = charset_name_t<String>())
	{
		typedef HexBuf cv_hex_buf_type;
		typedef typename boost::remove_cv<cv_hex_buf_type>::type hex_buf_type;
		typedef typename boost::range_iterator<const hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::escape_bytes_helper
			<
				hex_buf_iter_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
#if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
		return h.operator()<String>(boost::begin(hex_buf), boost::end(hex_buf), sep, mode, dst_charset_name);
#else
		return h.template operator()<String>(boost::begin(hex_buf), boost::end(hex_buf), sep, mode, dst_charset_name);
#endif // #if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
	}

	template<typename String, typename HexBuf, typename Alloc> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<HexBuf>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		s_escape_bytes(const HexBuf& hex_buf, 
						typename range_ex::range_value_ex<String>::type sep,
						const Alloc& alloc,
						u32 mode = base_type::E_Mode_Default,
						const string& dst_charset_name = charset_name_t<String>())
	{
		typedef HexBuf cv_hex_buf_type;
		typedef typename boost::remove_cv<cv_hex_buf_type>::type hex_buf_type;
		typedef typename boost::range_iterator<const hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::escape_bytes_helper
			<
				hex_buf_iter_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
#if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
		return h.operator()<String>(boost::begin(hex_buf), boost::end(hex_buf), sep, alloc, mode, dst_charset_name);
#else
		return h.template operator()<String>(boost::begin(hex_buf), boost::end(hex_buf), sep, alloc, mode, dst_charset_name);
#endif // #if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
	}

public:
	using base_type::s_unescape_byte;

	template<typename String> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				charset::is_string_t<String>,
				charset::is_string_view_t<String>
			>,
			u8
		>::type
		s_unescape_byte(const String& str)
	{
		return base_type::s_unescape_byte(str.data());
	}

public:
	using base_type::s_unescape_bytes;

	template<typename HexBuf, typename Char> inline
	static std::size_t s_unescape_bytes(HexBuf& buf, const Char* p, size_type n, Char sep,
									const string& src_charset_name = charset_name_t<Char>())
	{
		typedef HexBuf hex_buf_type;
		typedef typename boost::range_iterator<hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::unescape_bytes_helper
			<
				hex_buf_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
		return h(buf, p, n, sep, src_charset_name);
	}

	template<typename HexBuf, typename Char> inline
	static std::size_t s_unescape_bytes(HexBuf& buf, const Char* p, Char sep,
									const string& src_charset_name = charset_name_t<Char>())
	{
		return this_type::s_unescape_bytes(buf, p, charset_base_foo::strlen(p), sep, src_charset_name);
	}

	template<typename HexBuf, 
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String,
				typename Char, typename Traits, typename Alloc > inline
	static
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			std::size_t
		>::type
		s_unescape_bytes(HexBuf& buf, const Basic_String<Char, Traits, Alloc>& src, Char sep,
							const string& src_charset_name = charset_name_t< Char >())
	{
		typedef HexBuf hex_buf_type;
		typedef typename boost::range_iterator<hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::unescape_bytes_helper
			<
				hex_buf_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
		return h(buf, src.data(), src.size(), sep, src_charset_name);
	}

	template<typename HexBuf, typename Char, typename Traits, typename Alloc> inline
	static std::size_t s_unescape_bytes(HexBuf& buf, const utf8_string_impl<Char, Traits, Alloc>& src, Char sep,
										const string& src_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME())
	{
		typedef HexBuf hex_buf_type;
		typedef typename boost::range_iterator<hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::unescape_bytes_helper
			<
				hex_buf_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		assert(src_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		h_type h;
		return h(buf, src.data(), src.size(), sep, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}

	template<typename HexBuf, 
				template<typename _Char, typename _Traits> class BasicStringView,
				typename Char, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			is_basic_string_view_t< BasicStringView<Char, Traits> >,
			std::size_t
		>::type
		s_unescape_bytes(HexBuf& buf, const BasicStringView<Char, Traits>& src, Char sep,
							const string& src_charset_name = charset_name_t< Char >())
	{
		typedef HexBuf hex_buf_type;
		typedef typename boost::range_iterator<hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::unescape_bytes_helper
			<
				hex_buf_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
		return h(buf, src.data(), src.size(), sep, src_charset_name);
	}

	template<typename HexBuf, typename Char, typename Traits> inline
	static std::size_t s_unescape_bytes(HexBuf& buf, const basic_utf8_string_view<Char, Traits>& src, Char sep,
											const string& src_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME())
	{
		typedef HexBuf hex_buf_type;
		typedef typename boost::range_iterator<hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::unescape_bytes_helper
			<
				hex_buf_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		assert(src_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		h_type h;
		return h(buf, src.data(), src.size(), sep, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
	}
};

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_ESCAPE_CODES_CONVERTER_HPP__