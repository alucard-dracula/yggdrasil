//xml_wiarchive.hpp

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

#ifndef __YGGR_ARCHIVE_XML_WIARCHIVE_HPP__
#define __YGGR_ARCHIVE_XML_WIARCHIVE_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/archive/detail/auto_link_warchive.hpp> // if condecvt_null at this befro link unsymbol
#include <boost/archive/xml_wiarchive.hpp>
#include <boost/archive/impl/basic_xml_grammar.hpp>

#include <yggr/archive/impl/basic_text_iprimitive.hpp>
#include <yggr/archive/impl/basic_xml_iarchive.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>
#include <yggr/archive/impl/xml_wiarchive_impl.hpp>
#include <yggr/archive/impl/codecvt_null.hpp>
#include <yggr/archive/impl/utf8_codecvt_facet.hpp>

#include <yggr/serialization/collection_size_type.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>

#include <yggr/charset/default_charset_config.hpp>
#include <yggr/charset/charset_converter.hpp>

#include <yggr/ppex/friend.hpp>

#include <yggr/archive/detail/pfto_compatible.hpp>

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
class xml_wiarchive_impl 
	: public boost::archive::xml_wiarchive_impl<Archive>
{
public:
	typedef boost::archive::xml_wiarchive_impl<Archive> base_type;

private:
	typedef xml_wiarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_iarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_xml_iarchive<Archive>);
	friend class boost::archive::basic_text_iprimitive<std::wistream>;
	friend class boost::archive::load_access;
protected:
#endif
	
	xml_wiarchive_impl(std::wistream & is, unsigned int flags)
		: base_type(is, flags)
	{
	}
	
	~xml_wiarchive_impl(void)
	{
	}

public:
	using base_type::load;

	//void load(char * s); // using base_type::save
	//void load(std::string & s); // using base_type::save

protected:
	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	void pro_load_char_string(BasicString<Char, Traits, Alloc>& s)
	{
		std::wstring ws;
		bool result = base_type::gimpl->parse_string(base_type::is, ws);
		if(! result)
			boost::serialization::throw_exception(
				boost::archive::xml_archive_exception(
					boost::archive::xml_archive_exception::xml_archive_parsing_error)
			);
#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != s.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			s.resize(0);
		s.reserve(ws.size());
		std::copy(
			boost::archive::iterators::mb_from_wchar<std::wstring::iterator>(
				BOOST_MAKE_PFTO_WRAPPER(ws.begin())
			), 
			boost::archive::iterators::mb_from_wchar<std::wstring::iterator>(
				BOOST_MAKE_PFTO_WRAPPER(ws.end())
			), 
			std::back_inserter(s)
		);
	}

public:

//	inline void load(std::string& s)
//	{
//		std::wstring ws;
//		bool result = base_type::gimpl->parse_string(base_type::is, ws);
//		if(! result)
//			boost::serialization::throw_exception(
//				boost::archive::xml_archive_exception(
//					boost::archive::xml_archive_exception::xml_archive_parsing_error)
//			);
//#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
//		if(NULL != s.data())
//#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
//			s.resize(0);
//		s.reserve(ws.size());
//		std::copy(
//			boost::archive::iterators::mb_from_wchar<std::wstring::iterator>(
//				BOOST_MAKE_PFTO_WRAPPER(ws.begin())
//			), 
//			boost::archive::iterators::mb_from_wchar<std::wstring::iterator>(
//				BOOST_MAKE_PFTO_WRAPPER(ws.end())
//			), 
//			std::back_inserter(s)
//		);
//	}

	template<typename Traits, typename Alloc> inline
	void load(std::basic_string<char, Traits, Alloc>& s)
	{
		return pro_load_char_string(s);
	}

	template<typename Traits, typename Alloc> inline
	void load(boost::container::basic_string<char, Traits, Alloc>& s)
	{
		return pro_load_char_string(s);
	}

//	template<typename Char, typename Traits, typename Alloc> inline
//	typename
//		boost::enable_if
//		<
//			boost::is_same<boost::mpl::size_t<sizeof(Char)>, boost::mpl::size_t<1> >,
//			void
//		>::type
//		load(std::basic_string<Char, Traits, Alloc>& s)
//	{
//		std::wstring ws;
//		bool result = base_type::gimpl->parse_string(base_type::is, ws);
//		if(! result)
//			boost::serialization::throw_exception(
//				boost::archive::xml_archive_exception(
//					boost::archive::xml_archive_exception::xml_archive_parsing_error)
//			);
//#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
//		if(NULL != s.data())
//#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
//			s.resize(0);
//		s.reserve(ws.size());
//		std::copy(
//			boost::archive::iterators::mb_from_wchar<std::wstring::iterator>(
//				BOOST_MAKE_PFTO_WRAPPER(ws.begin())
//			), 
//			boost::archive::iterators::mb_from_wchar<std::wstring::iterator>(
//				BOOST_MAKE_PFTO_WRAPPER(ws.end())
//			), 
//			std::back_inserter(s)
//		);
//	}
//
//	template<typename Char, typename Traits, typename Alloc> inline
//	typename
//		boost::enable_if
//		<
//			boost::is_same<boost::mpl::size_t<sizeof(Char)>, boost::mpl::size_t<1> >,
//			void
//		>::type
//		load(boost::container::basic_string<Char, Traits, Alloc>& s)
//	{
//		std::wstring ws;
//		bool result = base_type::gimpl->parse_string(base_type::is, ws);
//		if(! result)
//			boost::serialization::throw_exception(
//				boost::archive::xml_archive_exception(
//					boost::archive::xml_archive_exception::xml_archive_parsing_error)
//			);
//#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
//		if(NULL != s.data())
//#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
//			s.resize(0);
//		s.reserve(ws.size());
//		std::copy(
//			boost::archive::iterators::mb_from_wchar<std::wstring::iterator>(
//				BOOST_MAKE_PFTO_WRAPPER(ws.begin())
//			), 
//			boost::archive::iterators::mb_from_wchar<std::wstring::iterator>(
//				BOOST_MAKE_PFTO_WRAPPER(ws.end())
//			), 
//			std::back_inserter(s)
//		);
//	}

#ifndef BOOST_NO_STD_WSTRING

protected:
	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	void pro_load_wchar_t_stl_string(BasicString<Char, Traits, Alloc>& ws)
	{
		typedef Char now_char_type;

		std::wstring tstring;
		bool result = base_type::gimpl->parse_string(base_type::is, tstring);
		if(! result)
			boost::serialization::throw_exception(
				boost::archive::xml_archive_exception(
					boost::archive::xml_archive_exception::xml_archive_parsing_error)
			);

		ws.swap(tstring);
	}

	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	void pro_load_wchar_t_string(BasicString<Char, Traits, Alloc>& ws)
	{
		typedef Char now_char_type;

		std::wstring tstring;
		bool result = base_type::gimpl->parse_string(base_type::is, tstring);
		if(! result)
			boost::serialization::throw_exception(
				boost::archive::xml_archive_exception(
					boost::archive::xml_archive_exception::xml_archive_parsing_error)
			);

		ws.assign(
			reinterpret_cast<const now_char_type*>(tstring.data()), 
			reinterpret_cast<const now_char_type*>(tstring.data()) + tstring.size());
	}

public:

	//void load(std::wstring& ws); // using base_type::save

	inline void load(std::wstring& ws)
	{
		return pro_load_wchar_t_stl_string(ws);
	}

	template<typename Traits, typename Alloc> inline
	void load(std::basic_string<wchar_t, Traits, Alloc>& ws)
	{
		return pro_load_wchar_t_string(ws);
	}

	template<typename Traits, typename Alloc> inline
	void load(boost::container::basic_string<wchar_t, Traits, Alloc>& ws)
	{
		return pro_load_wchar_t_string(ws);
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
		load(std::basic_string<Char, Traits, Alloc>& s)
	{
		typedef Char now_char_type;

		YGGR_CPP23_IF_CONSTEXPR(
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type) == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t)
			&& sizeof(now_char_type) == sizeof(wchar_t))
		{
			return pro_load_wchar_t_string(s);
		}
		else
		{
			std::wstring ws;
			pro_load_wchar_t_stl_string(ws);

			if(ws.empty())
			{
				s.clear();
				return;
			}

			charset::charset_converter conv(YGGR_STRING_DEFAULT_CHARSET_NAME(wchar_t),
											YGGR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
			if(!conv(ws, s))
			{
				boost::serialization::throw_exception(
					boost::archive::archive_exception(boost::archive::archive_exception::input_stream_error));
			}
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
		load(boost::container::basic_string<Char, Traits, Alloc>& s)
	{
		typedef Char now_char_type;

		YGGR_CPP23_IF_CONSTEXPR(
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(now_char_type) == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t)
			&& sizeof(now_char_type) == sizeof(wchar_t))
		{
			return pro_load_wchar_t_string(s);
		}
		else
		{
			std::wstring ws;
			pro_load_wchar_t_stl_string(ws);

			if(ws.empty())
			{
				s.clear();
				return;
			}

			charset::charset_converter conv(YGGR_STRING_DEFAULT_CHARSET_NAME(wchar_t),
											YGGR_STRING_DEFAULT_CHARSET_NAME(now_char_type));
			if(!conv(ws, s))
			{
				boost::serialization::throw_exception(
					boost::archive::archive_exception(boost::archive::archive_exception::input_stream_error));
			}
		}
	}

#	if !defined(YGGR_NO_CHAR8_T)
	inline void load(c8& t)
    {
        s16 i;
        base_type::load(i);
        t = i;
    }

	void load(c8* s)
	{
		typedef yggr::basic_string<c8> out_string_type;
		std::wstring ws;
		out_string_type outs;
		pro_load_wchar_t_stl_string(ws);

		charset::charset_converter conv(YGGR_STRING_DEFAULT_CHARSET_NAME(wchar_t),
										YGGR_STRING_DEFAULT_CHARSET_NAME(c8));

		if(!conv(ws, outs))
		{
			boost::serialization::throw_exception(
				boost::archive::archive_exception(boost::archive::archive_exception::input_stream_error));
		}
		
		if(outs.size()) memcpy(s, outs.data(), outs.size()/* * sizeof(c8)*/);
		s[outs.size()] = u8'\0';
	}
#	endif //YGGR_NO_CHAR8_T

#	if !defined(YGGR_NO_CHAR16_T)
	inline void load(c16& t)
    {
        s32 i;
        load(i);
        t = i;
    }

	inline void load(c16* s)
	{
		typedef yggr::basic_string<c16> out_string_type;

		YGGR_CPP23_IF_CONSTEXPR(
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(c16) == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t)
			&& sizeof(c16) == sizeof(wchar_t))
		{
			return base_type::load(reinterpret_cast<wchar_t*>(s));
		}
		else
		{
			std::wstring ws;
			out_string_type outs;
			pro_load_wchar_t_stl_string(ws);

			charset::charset_converter conv(YGGR_STRING_DEFAULT_CHARSET_NAME(wchar_t),
											YGGR_STRING_DEFAULT_CHARSET_NAME(c16));

			if(!conv(ws, outs))
			{
				boost::serialization::throw_exception(
					boost::archive::archive_exception(boost::archive::archive_exception::input_stream_error));
			}
			
			if(outs.size()) memcpy(s, outs.data(), outs.size()/* * sizeof(c8)*/);
			s[outs.size()] = u'\0';
		}
	}

#	endif //YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)
	inline void load(c32& t)
	{
        s32 i;
        load(i);
        t = i;
    }

	inline void load(c32* s)
	{
		typedef yggr::basic_string<c32> out_string_type;

		YGGR_CPP23_IF_CONSTEXPR(
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(c32) == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t)
			&& sizeof(c32) == sizeof(wchar_t))
		{
			return base_type::load(reinterpret_cast<wchar_t*>(s));
		}
		else
		{
			std::wstring ws;
			out_string_type outs;
			pro_load_wchar_t_stl_string(ws);

			charset::charset_converter conv(YGGR_STRING_DEFAULT_CHARSET_NAME(wchar_t),
											YGGR_STRING_DEFAULT_CHARSET_NAME(c32));

			if(!conv(ws, outs))
			{
				boost::serialization::throw_exception(
					boost::archive::archive_exception(boost::archive::archive_exception::input_stream_error));
			}
			
			if(outs.size()) memcpy(s, outs.data(), outs.size() * sizeof(c32));
			s[outs.size()] = U'\0';
		}
	}
#	endif //YGGR_NO_CHAR32_T

	//template<typename Char, typename Traits, typename Alloc> inline
	//typename
	//	boost::disable_if
	//	<
	//		boost::is_same<boost::mpl::size_t<sizeof(Char)>, boost::mpl::size_t<1> >,
	//		void
	//	>::type
	//	load(std::basic_string<Char, Traits, Alloc>& ws)
	//{
	//	std::wstring tstring;
	//	bool result = base_type::gimpl->parse_string(base_type::is, tstring);
	//	if(! result)
	//		boost::serialization::throw_exception(
	//			boost::archive::xml_archive_exception(
	//				boost::archive::xml_archive_exception::xml_archive_parsing_error)
	//		);

	//	ws.assign(tstring.begin(), tstring.end());
	//}

	//template<typename Char, typename Traits, typename Alloc> inline
	//typename
	//	boost::disable_if
	//	<
	//		boost::is_same<boost::mpl::size_t<sizeof(Char)>, boost::mpl::size_t<1> >,
	//		void
	//	>::type
	//	load(boost::container::basic_string<Char, Traits, Alloc>& ws)
	//{
	//	std::wstring tstring;
	//	bool result = base_type::gimpl->parse_string(base_type::is, tstring);
	//	if(! result)
	//		boost::serialization::throw_exception(
	//			boost::archive::xml_archive_exception(
	//				boost::archive::xml_archive_exception::xml_archive_parsing_error)
	//		);

	//	ws.assign(tstring.begin(), tstring.end());
	//}

#endif // BOOST_NO_STD_WSTRING

//#ifndef BOOST_NO_INTRINSIC_WCHAR_T
//	void load(wchar_t* ws); // using base_type::save
//#endif // BOOST_NO_INTRINSIC_WCHAR_T

	inline void load(boost::serialization::collection_size_type& t)
	{
		ser_size_type tmp = 0;
		base_type::load(tmp);
		t = boost::serialization::collection_size_type(tmp);
	}

	inline void load(yggr::serialization::collection_size_type& t)
	{
		ser_size_type tmp = 0;
		base_type::load(tmp);
		t = yggr::serialization::collection_size_type(tmp);
	}
};

class naked_xml_wiarchive 
	: public yggr::archive::xml_wiarchive_impl<yggr::archive::naked_xml_wiarchive>
{
public:
	typedef std::wistream::char_type char_type;
	typedef std::wistream::traits_type traits_type;

	typedef yggr::archive::xml_wiarchive_impl<yggr::archive::naked_xml_wiarchive> base_type;
private:
	typedef naked_xml_wiarchive this_type;

public:
	naked_xml_wiarchive(std::wistream & is, unsigned int flags = 0)
		: base_type(is, flags)
	{
	}

	~naked_xml_wiarchive(void)
	{
	}
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#  pragma warning(pop) 
#endif // BOOST_MSVC

#if BOOST_VERSION < 105600
#	include <boost/archive/shared_ptr_helper.hpp>
#endif // BOOST_VERSION < 105600

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr 
{ 
namespace archive 
{

class xml_wiarchive
	: public yggr::archive::xml_wiarchive_impl<yggr::archive::xml_wiarchive>
#if BOOST_VERSION < 105600
		, public boost::archive::detail::shared_ptr_helper
#endif // BOOST_VERSION < 105600
{
public:
	typedef std::wistream::char_type char_type;
	typedef std::wistream::traits_type traits_type;

	typedef yggr::archive::xml_wiarchive_impl<yggr::archive::xml_wiarchive> base_type;
private:
	typedef xml_wiarchive this_type;
	
public:
	xml_wiarchive(std::wistream & is, unsigned int flags = 0) 
		: base_type(is, flags)
	{
	}

	~xml_wiarchive(void)
	{
	}
};

} // namespace archive
} // namespace yggr

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::xml_wiarchive)

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

// boost::archive::detail::load_non_pointer_type patch
#include <yggr/archive/detail/iserializer_patch.hpp>

#define YGGR_PP_ARCHIVE_TYPE() ::yggr::archive::naked_xml_wiarchive
	YGGR_PP_BOOST_ISERIALIZER_PATCH(YGGR_PP_ARCHIVE_TYPE)
#undef YGGR_PP_ARCHIVE_TYPE

#define YGGR_PP_ARCHIVE_TYPE() ::yggr::archive::xml_wiarchive
	YGGR_PP_BOOST_ISERIALIZER_PATCH(YGGR_PP_ARCHIVE_TYPE)
#undef YGGR_PP_ARCHIVE_TYPE

#endif // __YGGR_ARCHIVE_XML_WIARCHIVE_HPP__
