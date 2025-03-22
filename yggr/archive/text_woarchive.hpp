// text_woarchive.hpp

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

#ifndef __YGGR_ARCHIVE_TEXT_WOARCHIVE_HPP__
#define __YGGR_ARCHIVE_TEXT_WOARCHIVE_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/archive/detail/auto_link_warchive.hpp> // if condecvt_null at this befro link unsymbol
#include <boost/archive/text_woarchive.hpp>

#include <yggr/archive/impl/basic_text_oprimitive.hpp>
#include <yggr/archive/impl/basic_text_oarchive.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>
#include <yggr/archive/impl/text_woarchive_impl.hpp>
#include <yggr/archive/impl/codecvt_null.hpp>
#include <yggr/archive/impl/utf8_codecvt_facet.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>

#include <yggr/serialization/collection_size_type.hpp>

#include <yggr/charset/default_charset_config.hpp>
#include <yggr/charset/charset_converter.hpp>

#include <yggr/ppex/friend.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr 
{ 
namespace archive 
{

template<typename Archive>
class text_woarchive_impl 
	: public boost::archive::text_woarchive_impl<Archive>
{
public:
	typedef boost::archive::text_woarchive_impl<Archive> base_type;
private:
	typedef text_woarchive_impl this_type;
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_oarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_text_oarchive<Archive>);
	friend class boost::archive::basic_text_oprimitive<std::wostream>;
	friend class boost::archive::save_access;
protected:
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
	
	text_woarchive_impl(std::wostream & os, unsigned int flags)
		: base_type(os, flags)
	{
	}

	~text_woarchive_impl(void)
	{
	}

public:
	using base_type::save;

	void save(const char* s)
	{
		// note: superfluous local variable fixes borland warning
		const ser_size_type size = static_cast<ser_size_type>(std::strlen(s));
		*base_type::This() << size;
		base_type::This()->newtoken();
		while(*s != '\0')
			base_type::os.put(base_type::os.widen(*s++));
	}

	template<typename Char, typename Traits, typename Alloc>
	typename boost::enable_if<boost::is_same<Char, char>, void>::type
		save(const std::basic_string<Char, Traits, Alloc>& s)
	{
		const ser_size_type size = static_cast<ser_size_type>(s.size());
		*base_type::This() << size;
		base_type::This()->newtoken();
		const char * cptr = s.data();
		for(std::size_t i = size; i-- > 0;)
			base_type::os.put(base_type::os.widen(*cptr++));
	}

	template<typename Char, typename Traits, typename Alloc>
	typename boost::enable_if<boost::is_same<Char, char>, void>::type
		save(const boost::container::basic_string<Char, Traits, Alloc>& s)
	{
		const ser_size_type size = static_cast<ser_size_type>(s.size());
		*base_type::This() << size;
		base_type::This()->newtoken();
		const char * cptr = s.data();
		for(std::size_t i = size; i-- > 0;)
			base_type::os.put(base_type::os.widen(*cptr++));
	}

// os is wchar_t type, so using parent-child preprocess define
#if !(defined(BOOST_NO_CWCHAR) && defined(BOOST_NO_INTRINSIC_WCHAR_T))

protected:
	template<typename Char> inline
	void pro_save_const_wchar_t_pointer(const Char* ws)
	{
		const ser_size_type size = static_cast<ser_size_type>(std::wostream::traits_type::length(ws));
		*base_type::This() << size;
		base_type::This()->newtoken();
		base_type::os.write(reinterpret_cast<const wchar_t*>(ws), size);
	}

	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	void pro_save_wchar_t_string(const BasicString<Char, Traits, Alloc>& ws)
	{
		const ser_size_type size = static_cast<ser_size_type>(ws.length());
		*base_type::This() << size;
		base_type::This()->newtoken();
		base_type::os.write(reinterpret_cast<const wchar_t*>(ws.data()), size);
	}

public:
	inline void save(const wchar_t* ws)
	{
		return pro_save_const_wchar_t_pointer(ws);
	}

#	if !defined(BOOST_NO_STD_WSTRING)

	template<typename Char, typename Traits, typename Alloc> inline
	typename boost::enable_if<boost::is_same<Char, wchar_t>, void>::type
		save(const std::basic_string<Char, Traits, Alloc>& ws)
	{
		return pro_save_wchar_t_string(ws);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename boost::enable_if<boost::is_same<Char, wchar_t>, void>::type
		save(const boost::container::basic_string<Char, Traits, Alloc>& ws)
	{
		return pro_save_wchar_t_string(ws);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename 
		boost::disable_if
		<
			boost::mpl::or_
			<
				boost::is_same<Char, char>,
				boost::is_same<Char, wchar_t>
			>,
			void
		>::type
		save(const std::basic_string<Char, Traits, Alloc>& s)
	{
		typedef Char now_char_type;

		YGGR_CPP23_IF_CONSTEXPR(
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type) == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t)
			&& sizeof(now_char_type) == sizeof(wchar_t))
		{
			return pro_save_wchar_t_string(s);
		}
		else
		{
			charset::charset_converter conv(YGGR_STRING_DEFAULT_CHARSET_NAME(now_char_type),
											YGGR_STRING_DEFAULT_CHARSET_NAME(wchar_t));

			std::wstring ws;
			if(!conv(s, ws))
			{
				boost::serialization::throw_exception(
					boost::archive::archive_exception(boost::archive::archive_exception::output_stream_error));
			}

			return pro_save_wchar_t_string(ws);
		}
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename 
		boost::disable_if
		<
			boost::mpl::or_
			<
				boost::is_same<Char, char>,
				boost::is_same<Char, wchar_t>
			>,
			void
		>::type
		save(const boost::container::basic_string<Char, Traits, Alloc>& s)
	{
		typedef Char now_char_type;

		YGGR_CPP23_IF_CONSTEXPR(
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type) == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t)
			&& sizeof(now_char_type) == sizeof(wchar_t))
		{
			return pro_save_wchar_t_string(s);
		}
		else
		{
			charset::charset_converter conv(YGGR_STRING_DEFAULT_CHARSET_NAME(now_char_type),
											YGGR_STRING_DEFAULT_CHARSET_NAME(wchar_t));

			std::wstring ws;
			if(!conv(s, ws))
			{
				boost::serialization::throw_exception(
					boost::archive::archive_exception(boost::archive::archive_exception::output_stream_error));
			}

			return pro_save_wchar_t_string(ws);
		}
	}

#	if !defined(YGGR_NO_CHAR8_T)

	inline void save(const c8& t)
	{
		base_type::save( static_cast<s16>(t));
	}

	inline void save(const c8* s)
	{
		charset::charset_converter conv(YGGR_STRING_DEFAULT_CHARSET_NAME(c8),
										YGGR_STRING_DEFAULT_CHARSET_NAME(wchar_t));

		std::wstring ws;
		if(!conv(s, ws))
		{
			boost::serialization::throw_exception(
				boost::archive::archive_exception(boost::archive::archive_exception::output_stream_error));
		}

		return pro_save_wchar_t_string(ws);
	}
#	endif // YGGR_NO_CHAR8_T

#	if !defined(YGGR_NO_CHAR16_T)
	
	inline void save(const c16& t)
	{
		base_type::save(static_cast<s32>(t));
	}

	inline void save(const c16* s)
	{
		
		YGGR_CPP23_IF_CONSTEXPR(
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(c16) == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t)
			&& sizeof(c16) == sizeof(wchar_t))
		{
			return pro_save_const_wchar_t_pointer(s);
		}
		else
		{
			charset::charset_converter conv(YGGR_STRING_DEFAULT_CHARSET_NAME(c16),
											YGGR_STRING_DEFAULT_CHARSET_NAME(wchar_t));

			std::wstring ws;
			if(!conv(s, ws))
			{
				boost::serialization::throw_exception(
					boost::archive::archive_exception(boost::archive::archive_exception::output_stream_error));
			}
		}
	}

#	endif // YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)

	inline void save(const c32& t)
	{
		base_type::save(static_cast<s32>(t));
	}

	inline void save(const c32* s)
	{
		YGGR_CPP23_IF_CONSTEXPR(
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(c32) == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t)
			&& sizeof(c32) == sizeof(wchar_t))
		{
			return pro_save_const_wchar_t_pointer(s);
		}
		else
		{
			charset::charset_converter conv(YGGR_STRING_DEFAULT_CHARSET_NAME(c32),
											YGGR_STRING_DEFAULT_CHARSET_NAME(wchar_t));

			std::wstring ws;
			if(!conv(s, ws))
			{
				boost::serialization::throw_exception(
					boost::archive::archive_exception(boost::archive::archive_exception::output_stream_error));
			}

			return pro_save_wchar_t_string(ws);
		}
	}
#	endif // YGGR_NO_CHAR16_T

#	endif // BOOST_NO_STD_WSTRING
#endif // BOOST_NO_INTRINSIC_WCHAR_T

	inline void save(const boost::serialization::collection_size_type& t)
	{
		const std::size_t tmp1 = t;
		ser_size_type tmp = static_cast<const ser_size_type>(tmp1);
		base_type::save(tmp);
	}

	inline void save(const yggr::serialization::collection_size_type& t)
	{
		const ser_size_type tmp = t;
		base_type::save(tmp);
	}
};

class text_woarchive 
	: public yggr::archive::text_woarchive_impl<yggr::archive::text_woarchive>
{
public:
	typedef std::wostream::char_type char_type;
	typedef std::wostream::traits_type traits_type;

	typedef yggr::archive::text_woarchive_impl<yggr::archive::text_woarchive> base_type;
private:
	typedef text_woarchive this_type;
public:
	text_woarchive(std::wostream & os, unsigned int flags = 0)
		: base_type(os, flags)
	{
	}
	
	~text_woarchive()
	{
	}
};

typedef text_woarchive naked_text_woarchive;

} // namespace archive
} // namespace yggr

BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::text_woarchive)

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

// boost::archive::detail::save_non_pointer_type patch
#include <yggr/archive/detail/oserializer_patch.hpp>

#define YGGR_PP_ARCHIVE_TYPE() ::yggr::archive::text_woarchive
	YGGR_PP_BOOST_OSERIALIZER_PATCH(YGGR_PP_ARCHIVE_TYPE)
#undef YGGR_PP_ARCHIVE_TYPE

#endif // __YGGR_ARCHIVE_TEXT_WOARCHIVE_HPP__
