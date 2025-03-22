//xml_oarchive.hpp

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

#ifndef __YGGR_ARCHIVE_XML_OARCHIVE_HPP__
#define __YGGR_ARCHIVE_XML_OARCHIVE_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/archive/xml_oarchive.hpp>

#include <yggr/archive/impl/basic_text_oprimitive.hpp>
#include <yggr/archive/impl/basic_xml_oarchive.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>
#include <yggr/archive/impl/xml_oarchive_impl.hpp>

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

template<class Archive>
class xml_oarchive_impl
	: public boost::archive::xml_oarchive_impl<Archive>
{
public:
	typedef boost::archive::xml_oarchive_impl<Archive> base_type;

private:
	typedef xml_oarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_oarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_xml_oarchive<Archive>);
	friend class boost::archive::basic_text_oprimitive<std::ostream>;
	friend class boost::archive::save_access;
protected:
#endif

	xml_oarchive_impl(std::ostream & os, unsigned int flags)
		: base_type(os, flags)
	{
	}

	~xml_oarchive_impl(void)
	{
	}

public:
	using base_type::save;

protected:
	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	void pro_save_char_string(const BasicString<Char, Traits, Alloc>& s)
	{
		//  at least one library doesn't typedef value_type for strings
		//  so rather than using string directly make a pointer iterator out of it
		typedef 
			boost::archive::iterators::xml_escape
			<
				const char * 
			> xml_escape_translator;
		std::copy(
			xml_escape_translator(BOOST_MAKE_PFTO_WRAPPER(s.data())),
			xml_escape_translator(BOOST_MAKE_PFTO_WRAPPER(s.data()+ s.size())), 
			boost::archive::iterators::ostream_iterator<char>(base_type::os)
		);
	}

public:

	//void save(const char * s); // using base_type::save
	//void save(const std::string & s); // using base_type::save

	template<typename Traits, typename Alloc> inline
	void save(const std::basic_string<char, Traits, Alloc>& s)
	{
		return pro_save_char_string(s);
	}

	template<typename Traits, typename Alloc> inline
	void save(const boost::container::basic_string<char, Traits, Alloc>& s)
	{
		return pro_save_char_string(s);
	}

#ifndef BOOST_NO_CWCHAR

//#ifndef BOOST_NO_INTRINSIC_WCHAR_T
//	void save(const wchar_t * ws); // using base_type::save
//#endif // BOOST_NO_INTRINSIC_WCHAR_T

#ifndef BOOST_NO_STD_WSTRING
	
protected:
	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
					typename Char, typename Traits, typename Alloc> inline
	void pro_save_wchar_t_string(const BasicString<Char, Traits, Alloc>& ws)
	{
		//  at least one library doesn't typedef value_type for strings
		//  so rather than using string directly make a pointer iterator out of it
		//    save_iterator(os, ws.data(), ws.data() + std::wcslen(ws.data()));

		// this using ws.size(), not std::wcslen(ws.data())
		// see boost/archive/impl/xml_oarchive_impl.ipp
		boost::archive::save_iterator(
			base_type::os,
			reinterpret_cast<const wchar_t*>(ws.data()),
			reinterpret_cast<const wchar_t*>(ws.data()) + ws.size()); 
	}
public:

	//void save(const std::wstring & ws); // using base_type::save

	template<typename Traits, typename Alloc> inline
	void save(const std::basic_string<wchar_t, Traits, Alloc>& ws)
	{
		return pro_save_wchar_t_string(ws);
	}

	template<typename Traits, typename Alloc> inline
	void save(const boost::container::basic_string<wchar_t, Traits, Alloc>& ws)
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

	inline void save(const c8 t)
	{
		save(static_cast<s16>(t));
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
	inline void save(const c16 t)
	{
		save(static_cast<s32>(t));
	}

	inline void save(const c16* s)
	{
		YGGR_CPP23_IF_CONSTEXPR(
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(c16) == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t)
			&& sizeof(c32) == sizeof(wchar_t))
		{
			base_type::save(reinterpret_cast<const wchar_t*>(s));
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

			return pro_save_wchar_t_string(ws);
		}
	}
#	endif // YGGR_NO_CHAR16_T

#	if !defined(YGGR_NO_CHAR32_T)
	inline void save(const c32 t)
	{
		save(static_cast<s32>(t));
	}

	inline void save(const c32* s)
	{
		YGGR_CPP23_IF_CONSTEXPR(
			YGGR_STR_STRING_DEFAULT_CHARSET_NAME(c32) == YGGR_STR_STRING_DEFAULT_CHARSET_NAME(wchar_t)
			&& sizeof(c32) == sizeof(wchar_t))
		{
			base_type::save(reinterpret_cast<const wchar_t*>(s));
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
#	endif // YGGR_NO_CHAR32_T

#endif // BOOST_NO_STD_WSTRING
#endif // BOOST_NO_CWCHAR

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


class xml_oarchive 
	: public yggr::archive::xml_oarchive_impl<yggr::archive::xml_oarchive>
{
public:
	typedef std::ostream::char_type char_type;
	typedef std::ostream::traits_type traits_type;

	typedef yggr::archive::xml_oarchive_impl<yggr::archive::xml_oarchive> base_type;
private:
	typedef xml_oarchive this_type;

public:
	xml_oarchive(std::ostream & os, unsigned int flags = 0)
		: base_type(os, flags)
	{
	}

	~xml_oarchive(void)
	{
	}
};

typedef xml_oarchive naked_xml_oarchive;

} // namespace archive
} // namespace yggr

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::xml_oarchive)

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

// boost::archive::detail::save_non_pointer_type patch
#include <yggr/archive/detail/oserializer_patch.hpp>

#define YGGR_PP_ARCHIVE_TYPE() ::yggr::archive::xml_oarchive
	YGGR_PP_BOOST_OSERIALIZER_PATCH(YGGR_PP_ARCHIVE_TYPE)
#undef YGGR_PP_ARCHIVE_TYPE

#endif // __YGGR_ARCHIVE_XML_OARCHIVE_HPP__
