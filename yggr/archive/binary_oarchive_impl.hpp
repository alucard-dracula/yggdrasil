//binary_oarchive_impl.hpp

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

#ifndef __YGGR_ARCHIVE_BINARY_OARCHIVE_IMPL_HPP__
#define __YGGR_ARCHIVE_BINARY_OARCHIVE_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/serialization/pfto.hpp>

#include <boost/archive/basic_binary_oprimitive.hpp>
#include <yggr/archive/impl/basic_binary_oprimitive.hpp>

#include <boost/archive/basic_binary_oarchive.hpp>
#include <yggr/archive/impl/basic_binary_oarchive.hpp>

#include <yggr/serialization/collection_size_type.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/archive/detail/is_sizeof_char_1_byte.hpp>

#include <boost/container/string.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/type_traits/is_same.hpp>

#include <ostream>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr
{
namespace archive
{

template<typename Archive, typename Elem, typename Tr>
class binary_oarchive_impl
	: public boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>,
		public boost::archive::basic_binary_oarchive<Archive>
{
public:
	typedef boost::archive::basic_binary_oprimitive<Archive, Elem, Tr> base_type;

private:
	typedef binary_oarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_oarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_binary_oarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_binary_oprimitive<
								Archive YGGR_PP_SYMBOL_COMMA() Elem YGGR_PP_SYMBOL_COMMA() Tr>);
	friend class boost::archive::save_access;
protected:
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

	// note: the following should not needed - but one compiler (vc 7.1)
	// fails to compile one test (test_shared_ptr) without it !!!
	// make this protected so it can be called from a derived archive
	template<class T> inline
	void save_override(T& t YGGR_PFTO_TYPE(int))
	{
		this->boost::archive::basic_binary_oarchive<Archive>::save_override(t YGGR_PFTO_VAR(0L));
	}

	inline void init(unsigned int flags)
	{
		if(0 != (flags & boost::archive::no_header))
			return;
#	if ! defined(__MWERKS__)
		this->boost::archive::basic_binary_oarchive<Archive>::init();
		this->boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>::init();
#	else
		boost::archive::basic_binary_oarchive<Archive>::init();
		boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>::init();
#	endif
	}

	binary_oarchive_impl(
		std::basic_streambuf<Elem, Tr> & bsb,
		unsigned int flags
	) :
		boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>(
			bsb,
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_oarchive<Archive>(flags)
	{
		init(flags);
	}

	binary_oarchive_impl(
		std::basic_ostream<Elem, Tr> & os,
		unsigned int flags
	) :
		boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>(
			* os.rdbuf(),
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_oarchive<Archive>(flags)
	{
		init(flags);
	}

public:
	using base_type::save;

	// !!!why using pro_load_string_char_x, because in msvc2022 not serialized correctly when using sfinae and wstringstream!!!
protected:
	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	void pro_save_string_char_1(const BasicString<Char, Traits, Alloc>& s)
	{
		ser_size_type l = static_cast<ser_size_type>(s.size());
		base_type::This()->save(l);
		base_type::save_binary(s.data(), l);
	}

	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	void pro_save_string_char_n(const BasicString<Char, Traits, Alloc>& ws)
	{
		typedef Char now_char_type;
		ser_size_type l = static_cast<ser_size_type>(ws.size());
		base_type::This()->save(l);
		base_type::save_binary(ws.data(), l * sizeof(now_char_type) /*/ sizeof(char)*/);
	}

public:
	inline void save(const char* s)
	{
		ser_size_type l = static_cast<ser_size_type>(std::strlen(s));
		base_type::This()->save(l);
		base_type::save_binary(s, l);
	}

	template<typename Traits, typename Alloc> inline
	void save(const std::basic_string<char, Traits, Alloc>& s)
	{
		return pro_save_string_char_1(s);
	}

	template<typename Traits, typename Alloc> inline
	void save(const boost::container::basic_string<char, Traits, Alloc>& s)
	{
		return pro_save_string_char_1(s);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::enable_if
		<
			detail::is_sizeof_char_1_byte<Char>,
			void
		>::type
		save(const std::basic_string<Char, Traits, Alloc>& s)
	{
		return pro_save_string_char_1(s);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::enable_if
		<
			detail::is_sizeof_char_1_byte<Char>,
			void
		>::type
		save(const boost::container::basic_string<Char, Traits, Alloc>& s)
	{
		return pro_save_string_char_1(s);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			detail::is_sizeof_char_1_byte<Char>,
			void
		>::type
		save(const std::basic_string<Char, Traits, Alloc>& s)
	{
		return pro_save_string_char_n(s);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			detail::is_sizeof_char_1_byte<Char>,
			void
		>::type
		save(const boost::container::basic_string<Char, Traits, Alloc>& s)
	{
		return pro_save_string_char_n(s);
	}


#ifndef BOOST_NO_CWCHAR
	inline void save(const wchar_t* ws)
	{
		ser_size_type l = static_cast<ser_size_type>(std::wcslen(ws));
		base_type::This()->save(l);
		base_type::save_binary(ws, l * sizeof(wchar_t)/* / sizeof(char)*/);
	}
#endif // BOOST_NO_CWCHAR

#ifndef BOOST_NO_STD_WSTRING

	template<typename Traits, typename Alloc> inline
	void save(const std::basic_string<wchar_t, Traits, Alloc>& ws)
	{
		return pro_save_string_char_n(ws);
	}

	template<typename Traits, typename Alloc> inline
	void save(const boost::container::basic_string<wchar_t, Traits, Alloc>& ws)
	{
		return pro_save_string_char_n(ws);
	}

#endif // BOOST_NO_STD_WSTRING

#if !defined(YGGR_NO_CHAR8_T)
	inline void save(const c8* s)
	{
		ser_size_type l = static_cast<ser_size_type>(std::strlen(reinterpret_cast<const char*>(s)));
		base_type::This()->save(l);
		base_type::save_binary(s, l);
	}
#endif //YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_U8STRING_T)
	template<typename Traits, typename Alloc> inline
	void save(const std::basic_string<c8, Traits, Alloc>& s)
	{
		return pro_save_string_char_1(s);
	}

	template<typename Traits, typename Alloc> inline
	void save(const boost::container::basic_string<c8, Traits, Alloc>& s)
	{
		return pro_save_string_char_1(s);
	}

#endif // YGGR_NO_U8STRING_T

#if !defined(YGGR_NO_CHAR16_T)
	inline void save(const c16* s)
	{
		typedef std::char_traits<c16> now_char_traits_type;

		ser_size_type l = static_cast<ser_size_type>(now_char_traits_type::length(s));
		base_type::This()->save(l);
		base_type::save_binary(s, l * sizeof(c16)/* / sizeof(char)*/);
	}
#endif //YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_U16STRING_T)
	template<typename Traits, typename Alloc> inline
	void save(const std::basic_string<c16, Traits, Alloc>& s)
	{
		return pro_save_string_char_n(s);
	}

	template<typename Traits, typename Alloc> inline
	void save(const boost::container::basic_string<c16, Traits, Alloc>& s)
	{
		return pro_save_string_char_n(s);
	}

#endif // YGGR_NO_U16STRING_T

#if !defined(YGGR_NO_CHAR32_T)
	inline void save(const c32* s)
	{
		typedef std::char_traits<c32> now_char_traits_type;

		ser_size_type l = static_cast<ser_size_type>(now_char_traits_type::length(s));
		base_type::This()->save(l);
		base_type::save_binary(s, l * sizeof(c32)/* / sizeof(char)*/);
	}
#endif //YGGR_NO_CHAR32_T

#if !defined(YGGR_NO_U32STRING_T)
	template<typename Traits, typename Alloc> inline
	void save(const std::basic_string<c32, Traits, Alloc>& s)
	{
		return pro_save_string_char_n(s);
	}

	template<typename Traits, typename Alloc> inline
	void save(const boost::container::basic_string<c32, Traits, Alloc>& s)
	{
		return pro_save_string_char_n(s);
	}

#endif // YGGR_NO_U32STRING_T

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

} // namespace archive
} // namespace boost

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

#endif // __YGGR_ARCHIVE_BINARY_OARCHIVE_IMPL_HPP__
