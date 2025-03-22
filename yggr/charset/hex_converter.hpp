//hex_converter.hpp

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

#ifndef __YGGR_CHARSET_HEX_CONVERTER_HPP__
#define __YGGR_CHARSET_HEX_CONVERTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/move/move.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/container_ex/is_container.hpp>
#include <yggr/container_ex/is_allocator.hpp>

#include <yggr/bytes/byte_size_sp_basic.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/resize.hpp>
#include <yggr/container/clear.hpp>
#include <yggr/container/get_allocator.hpp>

#include <yggr/charset/hex_basic_converter.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <cassert>

namespace yggr
{
namespace charset
{
namespace detail
{

// string_to_hex_base_helper
template<typename HexBuf>
struct string_to_hex_base_helper
{
protected:
	typedef HexBuf hex_buf_type;
	typedef typename boost::range_value<hex_buf_type>::type hex_buf_value_type;

protected:
	//template<typename IHexBuf, typename Char> inline
	//bool exec_conv(IHexBuf& buf, const Char* p, size_type n) const
	//{
	//	typedef typename boost::range_value<IHexBuf>::type chk_val_type;
	//	BOOST_MPL_ASSERT((boost::is_same<hex_buf_value_type, chk_val_type>));

	//	// needed byte_space = upper((n / 2) / sizeof(hex_buf_value_type))
	//	// -> upper( n / (2 * sizeof(hex_buf_value_type)) )
	//	// -> upper( n / (sizeof(hex_buf_value_type) << 1) ) // opt * 2
	//	// -> (n + (sizeof(hex_buf_value_type) << 1) - 1) / (sizeof(hex_buf_value_type) << 1) // opt upper

	//	yggr::resize(
	//		buf, 
	//		(n + (sizeof(hex_buf_value_type) << 1) - 1) 
	//			/ (sizeof(hex_buf_value_type) << 1), 
	//		hex_buf_value_type());
	//		
	//	return 
	//		hex_basic_converter::s_string_to_hex(
	//			reinterpret_cast<u8*>(&(*boost::begin(buf))), p, n);
	//}

	template<typename IHexBuf, typename Char> inline
	typename
		boost::enable_if
		<
			boost::is_same<hex_buf_value_type, typename boost::range_value<IHexBuf>::type>,
			bool
		>::type
		exec_conv(IHexBuf& buf, const Char* p, size_type n) const
	{
		// needed buf_space = upper((n / 2) / sizeof(hex_buf_value_type))
		// -> upper( n / (2 * sizeof(hex_buf_value_type)) )
		// -> upper( n / (sizeof(hex_buf_value_type) << 1) ) // opt * 2
		// -> (n + (sizeof(hex_buf_value_type) << 1) - 1) / (sizeof(hex_buf_value_type) << 1) // opt upper

		yggr::resize(
			buf, 
			(n + (sizeof(hex_buf_value_type) << 1) - 1) 
				/ (sizeof(hex_buf_value_type) << 1), 
			hex_buf_value_type());
			
		return 
			hex_basic_converter::s_string_to_hex(
				reinterpret_cast<u8*>(&(*boost::begin(buf))), p, n);
	}
};

// string_to_hex_helper
template<typename HexBuf, bool is_rnd>
struct string_to_hex_helper;

template<typename HexBuf>
struct string_to_hex_helper<HexBuf, false>
	: protected string_to_hex_base_helper<HexBuf>
{
protected:
	typedef string_to_hex_base_helper<HexBuf> base_type;

public:
	typedef typename base_type::hex_buf_type hex_buf_type;
	typedef typename base_type::hex_buf_value_type hex_buf_value_type;
	typedef yggr::vector<hex_buf_value_type> tmp_hex_buf_type;

	template<typename Char> inline
	bool operator()(hex_buf_type& buf,
						const Char* p, size_type n,
						const string& src_charset_name) const
	{
		assert((p) && math::is_even_number(n));
		yggr::clear(buf);

		if(!(p && math::is_even_number(n)))
		{
			return false;
		}
		else if(n == 0)
		{
			return true;
		}
		else
		{
			tmp_hex_buf_type tmp;
			if(src_charset_name == charset_name_t<Char>())
			{
				if(base_type::exec_conv(tmp, p, n))
				{
					std::copy(tmp.begin(), tmp.end(), iterator_ex::inserter(buf, boost::end(buf)));
					return true;
				}
			}
			else
			{
				// why using this conv
				// because
				// if the current src_charset_name != defaulet_charset_name,
				//	then it is likely that the storage space will be different
				yggr::utf8_string str(p, n, src_charset_name);

				if((str.size() == str.length() && str.size() == n)
						&& base_type::exec_conv(tmp, str.data(), n))
				{
					std::copy(tmp.begin(), tmp.end(), iterator_ex::inserter(buf, boost::end(buf)));
					return true;
				}
			}

			return false;
		}
	}
};

template<typename HexBuf>
struct string_to_hex_helper<HexBuf, true>
	: protected string_to_hex_base_helper<HexBuf>
{
protected:
	typedef string_to_hex_base_helper<HexBuf> base_type;

public:
	typedef typename base_type::hex_buf_type hex_buf_type;
	typedef typename base_type::hex_buf_value_type hex_buf_value_type;

	template<typename Char> inline
	bool operator()(hex_buf_type& buf, 
						const Char* p, size_type n,
						const string& src_charset_name) const
	{
		assert((p) && math::is_even_number(n));
		yggr::clear(buf);

		if(!(p && math::is_even_number(n)))
		{
			return false;
		}
		else if(n == 0)
		{
			return true;
		}
		else
		{
			if(src_charset_name == charset_name_t<Char>())
			{
				return base_type::exec_conv(buf, p, n);
			}
			else
			{
				yggr::utf8_string str(p, n, src_charset_name);

				return (str.size() == str.length() && str.size() == n)
						&& base_type::exec_conv(buf, str.data(), n);
			}
		}
	}
};

// hex_to_string_helper
template<typename HexBufIter, bool is_rnd>
struct hex_to_string_helper;

template<typename HexBufIter>
struct hex_to_string_helper<HexBufIter, false>
{
public:
	typedef HexBufIter hex_buf_iter_type;
	typedef typename boost::iterator_value<hex_buf_iter_type>::type hex_buf_value_type;

	YGGR_STATIC_CONSTANT(u32, E_char_step = (sizeof(hex_buf_value_type) << 1));
	BOOST_MPL_ASSERT((boost::mpl::bool_<(E_char_step > 0)>));

private:
	typedef hex_to_string_helper this_type;

public:
	template<typename String>
	String& operator()(String& str, hex_buf_iter_type s, hex_buf_iter_type e, u32 mode,
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
				yggr::resize(str, byte_size << 1, 0);
				string_val_type* str_i = &(*boost::begin(str));

				for(; s != e; ++s, str_i += E_char_step)
				{
					hex_basic_converter::s_hex_to_string(str_i, reinterpret_cast<const u8*>(&(*s)), 
												sizeof(hex_buf_value_type), mode);
				}
			}
			else
			{
				yggr::utf8_string tstr;
				tstr.resize(byte_size << 1);
				assert(tstr.size() == tstr.length());
				yggr::utf8_string::value_type* str_i = &(tstr.org_str()[0]);

				for(; s != e; ++s, str_i += E_char_step)
				{
					hex_basic_converter::s_hex_to_string(str_i, reinterpret_cast<const u8*>(&(*s)), 
															sizeof(hex_buf_value_type), mode);
				}

				str = tstr.str<string_type>(yggr::get_allocator(str), dst_charset_name);
			}
		}

		return str;
	}

	template<typename String> inline
	String operator()(hex_buf_iter_type s, hex_buf_iter_type e, u32 mode,
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
					const Alloc& alloc,
					u32 mode,
					const string& dst_charset_name) const
	{
		typedef String string_type;
		string_type ret(alloc);
		// must using this->
		return this->operator()(ret, s, e, mode, dst_charset_name);
	}
};

template<typename HexBufIter>
struct hex_to_string_helper<HexBufIter, true>
{
public:
	typedef HexBufIter hex_buf_iter_type;
	typedef typename boost::iterator_value<hex_buf_iter_type>::type hex_buf_value_type;

private:
	typedef hex_to_string_helper this_type;

public:
	template<typename String>
	String& operator()(String& str,
						hex_buf_iter_type s, hex_buf_iter_type e, u32 mode,
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
				yggr::resize(str, byte_size << 1, 0);
				string_val_type* str_i = &(*boost::begin(str));

				hex_basic_converter::s_hex_to_string(str_i, reinterpret_cast<const u8*>(&(*s)), byte_size, mode);
			}
			else
			{
				yggr::utf8_string tstr;
				tstr.resize(byte_size << 1);
				assert(tstr.size() == tstr.length());
					
				yggr::utf8_string::value_type* str_i = &(tstr.org_str()[0]);

				hex_basic_converter::s_hex_to_string(str_i, reinterpret_cast<const u8*>(&(*s)), byte_size, mode);
				str = tstr.str<string_type>(yggr::get_allocator(str), dst_charset_name);
			}
		}

		return str;
	}

	template<typename String> inline
	String operator()(hex_buf_iter_type s, hex_buf_iter_type e, u32 mode,
						const string& dst_charset_name) const
	{
		typedef String string_type;
		typedef typename boost::range_value<string_type>::type string_val_type;

		string_type ret;
		// must using this->
		return this->operator()(ret, s, e, mode, dst_charset_name);
	}

	template<typename String, typename Alloc> inline
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		operator()(hex_buf_iter_type s, hex_buf_iter_type e, 
					const Alloc& alloc,
					u32 mode,
					const string& dst_charset_name) const
	{
		typedef String string_type;

		string_type ret(alloc);
		// must using this->
		return this->operator()(ret, s, e, mode, dst_charset_name);
	}
};


} // namespace detail
} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

class hex_converter
	: public hex_basic_converter
{

public:
	typedef hex_basic_converter base_type;

private:
	typedef hex_converter this_type;

public:
	// char_to_hex

	using base_type::s_char_to_hex;

	template<typename Char, typename Traits, typename Alloc> inline
	static u8 s_char_to_hex(const utf8_char_impl<Char, Traits, Alloc>& ch)
	{
		return ch.size() == 1? base_type::s_char_to_hex(*(ch.data())) : u8(-1);
	}

	template<typename Char, typename Traits> inline
	static u8 s_char_to_hex(const basic_utf8_char_view<Char, Traits>& ch)
	{
		return ch.size() == 1? base_type::s_char_to_hex(*(ch.data())) : u8(-1);
	}

	template<typename Utf8String> inline
	static u8 s_char_to_hex(const basic_utf8_char_reference<Utf8String>& ch)
	{
		return this_type::s_char_to_hex(ch.view());
	}

	template<typename Utf8String> inline
	static u8 s_char_to_hex(const basic_utf8_char_const_reference<Utf8String>& ch)
	{
		return this_type::s_char_to_hex(ch.view());
	}

	// s_hex_to_char // don't need write wrap, because support from s_hex_to_string
	using base_type::s_hex_to_char;


public:
	// s_string_to_hex
	using base_type::s_string_to_hex;

	template<typename HexBuf, typename Char> inline
	static bool s_string_to_hex(HexBuf& buf, const Char* p, const string& src_charset_name = charset_name_t<Char>())
	{
		return this_type::s_string_to_hex(buf, p, charset_base_foo::strlen(p), src_charset_name);
	}

	template<typename HexBuf, typename Char> inline
	static bool s_string_to_hex(HexBuf& buf, const Char* p, size_type n,
									const string& src_charset_name = charset_name_t<Char>())
	{
		typedef HexBuf hex_buf_type;
		typedef typename boost::range_iterator<hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::string_to_hex_helper
			<
				hex_buf_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
		return h(buf, p, n, src_charset_name);
	}

	// msvc is parse failed, so modify it
	//template<typename HexBuf, typename Char, typename Traits, typename Alloc,
	//			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	//static
	//typename
	//	boost::enable_if
	//	<
	//		is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
	//		bool
	//	>::type
	//	s_string_to_hex(HexBuf& buf, const Basic_String<Char, Traits, Alloc>& src,
	//						const string& src_charset_name = charset_name_t< Char >());

	template<typename HexBuf, 
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String,
				typename Char, typename Traits, typename Alloc > inline
	static
	typename
		boost::enable_if
		<
			is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
			bool
		>::type
		s_string_to_hex(HexBuf& buf, const Basic_String<Char, Traits, Alloc>& src,
							const string& src_charset_name = charset_name_t< Char >())
	{
		typedef HexBuf hex_buf_type;
		typedef typename boost::range_iterator<hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::string_to_hex_helper
			<
				hex_buf_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
		return h(buf, src.data(), src.size(), src_charset_name);
	}

	template<typename HexBuf, typename Char, typename Traits, typename Alloc> inline
	static bool s_string_to_hex(HexBuf& buf, const utf8_string_impl<Char, Traits, Alloc>& src,
								const string& src_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME())
	{
		typedef HexBuf hex_buf_type;
		typedef typename boost::range_iterator<hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::string_to_hex_helper
			<
				hex_buf_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		assert(src_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		if(src.size() != src.length())
		{
			yggr::clear(buf);
			return false;
		}
		else
		{
			h_type h;
			return h(buf, src.data(), src.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
		}
	}

	template<typename HexBuf, 
				template<typename _Char, typename _Traits> class BasicStringView,
				typename Char, typename Traits> inline
	static
	typename
		boost::enable_if
		<
			is_basic_string_view_t< BasicStringView<Char, Traits> >,
			bool
		>::type
		s_string_to_hex(HexBuf& buf, const BasicStringView<Char, Traits>& src,
							const string& src_charset_name = charset_name_t< Char >())
	{
		typedef HexBuf hex_buf_type;
		typedef typename boost::range_iterator<hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::string_to_hex_helper
			<
				hex_buf_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
		return h(buf, src.data(), src.size(), src_charset_name);
	}

	template<typename HexBuf, typename Char, typename Traits> inline
	static bool s_string_to_hex(HexBuf& buf, const basic_utf8_string_view<Char, Traits>& src,
								const string& src_charset_name = YGGR_STR_UTF8_STRING_CHARSET_NAME())
	{
		typedef HexBuf hex_buf_type;
		typedef typename boost::range_iterator<hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::string_to_hex_helper
			<
				hex_buf_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		assert(src_charset_name == YGGR_STR_UTF8_STRING_CHARSET_NAME());

		if(src.size() != src.length())
		{
			yggr::clear(buf);
			return false;
		}
		else
		{
			h_type h;
			return h(buf, src.data(), src.size(), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
		}
	}

public:
	// s_hex_to_string
	using base_type::s_hex_to_string;

	template<typename String, typename HexBuf> inline
	static 
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_class<String>,
				boost::mpl::not_< boost::is_const<String> >,
				container_ex::is_container<HexBuf>
			>,
			String&
		>::type
		s_hex_to_string(String& str,
							const HexBuf& hex_buf, u32 mode = base_type::E_Mode_Default,
							const string& dst_charset_name = charset_name_t<String>())
	{
		typedef HexBuf cv_hex_buf_type;
		typedef typename boost::remove_cv<cv_hex_buf_type>::type hex_buf_type;
		typedef typename boost::range_iterator<const hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::hex_to_string_helper
			<
				hex_buf_iter_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
		return h(str, boost::begin(hex_buf), boost::end(hex_buf), mode, dst_charset_name);
	}

	template<typename String, typename HexBuf> inline
	static String s_hex_to_string(const HexBuf& hex_buf, u32 mode = base_type::E_Mode_Default,
									const string& dst_charset_name = charset_name_t<String>())
	{
		typedef HexBuf cv_hex_buf_type;
		typedef typename boost::remove_cv<cv_hex_buf_type>::type hex_buf_type;
		typedef typename boost::range_iterator<const hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::hex_to_string_helper
			<
				hex_buf_iter_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
#if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
		return h.operator()<String>(boost::begin(hex_buf), boost::end(hex_buf), mode, dst_charset_name);
#else
		return h.template operator()<String>(boost::begin(hex_buf), boost::end(hex_buf), mode, dst_charset_name);
#endif // #if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
	}

	template<typename String, typename HexBuf, typename Alloc> inline
	static
	typename boost::enable_if<container_ex::is_allocator<Alloc>, String>::type
		s_hex_to_string(const HexBuf& hex_buf, 
						const Alloc& alloc,
						u32 mode = base_type::E_Mode_Default,
						const string& dst_charset_name = charset_name_t<String>())
	{
		typedef HexBuf cv_hex_buf_type;
		typedef typename boost::remove_cv<cv_hex_buf_type>::type hex_buf_type;
		typedef typename boost::range_iterator<const hex_buf_type>::type hex_buf_iter_type;
		typedef 
			detail::hex_to_string_helper
			<
				hex_buf_iter_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
#if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
		return h.operator()<String>(boost::begin(hex_buf), boost::end(hex_buf), alloc, mode, dst_charset_name);
#else
		return h.template operator()<String>(boost::begin(hex_buf), boost::end(hex_buf), alloc, mode, dst_charset_name);
#endif // #if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
	}

	template<typename String, typename HexBufIter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<String> >,
				is_iterator<HexBufIter>
			>,
			String&
		>::type
		s_hex_to_string(String& str,
							HexBufIter s, HexBufIter e, 
							u32 mode = base_type::E_Mode_Default,
							const string& dst_charset_name = charset_name_t<String>())
	{
		typedef HexBufIter hex_buf_iter_type;
		typedef 
			detail::hex_to_string_helper
			<
				hex_buf_iter_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
		return h(str, s, e, mode, dst_charset_name);
	}

	template<typename String, typename HexBufIter> inline
	static
	typename
		boost::enable_if
		<
			is_iterator<HexBufIter>,
			String
		>::type
		s_hex_to_string(HexBufIter s, HexBufIter e, 
							u32 mode = base_type::E_Mode_Default,
							const string& dst_charset_name = charset_name_t<String>())
	{
		typedef HexBufIter hex_buf_iter_type;
		typedef 
			detail::hex_to_string_helper
			<
				hex_buf_iter_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
#if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
		return h.operator()<String>(s, e, mode, dst_charset_name);
#else
		return h.template operator()<String>(s, e, mode, dst_charset_name);
#endif // #if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
	}

	template<typename String, typename HexBufIter, typename Alloc> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<HexBufIter>,
				container_ex::is_allocator<Alloc>
			>,
			String
		>::type
		s_hex_to_string(HexBufIter s, HexBufIter e,
						const Alloc& alloc,
						u32 mode = base_type::E_Mode_Default,
						const string& dst_charset_name = charset_name_t<String>())
	{
		typedef HexBufIter hex_buf_iter_type;
		typedef 
			detail::hex_to_string_helper
			<
				hex_buf_iter_type, 
				is_random_access_iterator<hex_buf_iter_type>::value
			> h_type;

		h_type h;
#if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
		return h.operator()<String>(s, e, alloc, mode, dst_charset_name);
#else
		return h.template operator()<String>(s, e, alloc, mode, dst_charset_name);
#endif // #if defined(_MSC_VER) && (YGGR_CPP_VERSION < YGGR_CPP_VER_20)
	}
};

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_HEX_CONVERTER_HPP__