//binary_iarchive_impl.hpp

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

#ifndef __YGGR_BOOST_ARCHIVE_BINARY_IARCHIVE_IMPL_HPP__
#define __YGGR_BOOST_ARCHIVE_BINARY_IARCHIVE_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/archive/basic_binary_iprimitive.hpp>
#include <yggr/archive/impl/basic_binary_iprimitive.hpp>

#include <boost/archive/basic_binary_iarchive.hpp>
#include <yggr/archive/impl/basic_binary_iarchive.hpp>

#include <yggr/serialization/pfto.hpp>
#include <yggr/serialization/collection_size_type.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/charset/string.hpp>

#include <yggr/archive/detail/is_sizeof_char_1_byte.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/type_traits/is_same.hpp>

#include <istream>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr
{
namespace archive
{

template<typename Archive, typename Elem, typename Tr>
class binary_iarchive_impl
	: public boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>,
		public boost::archive::basic_binary_iarchive<Archive>
{
public:
	typedef boost::archive::basic_binary_iprimitive<Archive, Elem, Tr> base_type;
	typedef boost::archive::basic_binary_iarchive<Archive> base2_type;
private:
	typedef binary_iarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_iarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_binary_iarchive<Archive>);
	YGGR_PP_FRIEND_TYPENAME(boost::archive::basic_binary_iprimitive<
								Archive YGGR_PP_SYMBOL_COMMA() Elem YGGR_PP_SYMBOL_COMMA() Tr>);
	friend class boost::archive::load_access;
protected:
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

	// note: the following should not needed - but one compiler (vc 7.1)
	// fails to compile one test (test_shared_ptr) without it !!!
	// make this protected so it can be called from a derived archive

	template<class T> inline
	void load_override(T& t YGGR_PFTO_TYPE(int))
	{
		base2_type::load_override(t YGGR_PFTO_VAR(0L));
	}

	inline void init(unsigned int flags)
	{
		if(0 != (flags & boost::archive::no_header))
			return;
#	if !defined(__MWERKS__)
		this->boost::archive::basic_binary_iarchive<Archive>::init();
		this->boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>::init();
#	else
		boost::archive::basic_binary_iarchive<Archive>::init();
		boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>::init();
#	endif // __MWERKS__
	}

	binary_iarchive_impl(
		std::basic_streambuf<Elem, Tr> & bsb,
		unsigned int flags
	) :
		boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>(
			bsb,
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_iarchive<Archive>(flags)
	{
		init(flags);
	}
	binary_iarchive_impl(
		std::basic_istream<Elem, Tr> & is,
		unsigned int flags
	) :
		boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>(
			* is.rdbuf(),
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_iarchive<Archive>(flags)
	{
		init(flags);
	}

public:
	using base_type::load;

	// why using
	// boost::is_same<boost::mpl::size_t<sizeof(Char)>, boost::mpl::size_t<1> >
	// !!!because enable_if_c<sizeof(Char) == 1, void> msvc is failed !!!

	// !!!why using pro_load_string_char_x, because in msvc2022 not serialized correctly when using sfinae and wstringstream!!!
	// !!! means "Schrodinger's template instantiation" !!!
protected:
	template<template<typename _Char, typename _Traits, typename _Alloc> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	void pro_load_string_char_1(BasicString<Char, Traits, Alloc>& s)
	{
		ser_size_type l = 0;
		base_type::This()->load(l);
		// borland de-allocator fixup
#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != s.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			s.resize(l);
		// note breaking a rule here - could be a problem on some platform
		if(0 < l)
			base_type::load_binary(&(*s.begin()), l);
	}

	template<template<typename _Char, typename _Tratis, typename _Alloc> class BasicString,
				typename Char, typename Traits, typename Alloc> inline
	void pro_load_string_char_n(BasicString<Char, Traits, Alloc>& ws)
	{
		typedef Char now_char_type;
		ser_size_type l = 0;
		base_type::This()->load(l);
		// borland de-allocator fixup
#	if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
		if(NULL != ws.data())
#	endif // BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
			ws.resize(l);
		// note breaking a rule here - is could be a problem on some platform
		base_type::load_binary(const_cast<now_char_type *>(ws.data()), l * sizeof(now_char_type)/* / sizeof(char)*/);
	}

public:
	inline void load(char* s)
	{
		ser_size_type l = 0;
		base_type::This()->load(l);
		base_type::load_binary(s, l);
		s[l] = '\0';
	}

	// !!!don't remove args of basic_string<char, Traits, Alloc> function, because using this foo fix "Schrodinger's template instantiation" !!!
	template<typename Traits, typename Alloc> inline
	void load(std::basic_string<char, Traits, Alloc>& s)
	{
		return pro_load_string_char_1(s);
	}

	template<typename Traits, typename Alloc> inline
	void load(boost::container::basic_string<char, Traits, Alloc>& s)
	{
		return pro_load_string_char_1(s);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::enable_if
		<
			detail::is_sizeof_char_1_byte<Char>,
			void
		>::type
		load(std::basic_string<Char, Traits, Alloc>& s)
	{
		return pro_load_string_char_1(s);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::enable_if
		<
			detail::is_sizeof_char_1_byte<Char>,
			void
		>::type
		load(boost::container::basic_string<Char, Traits, Alloc>& s)
	{
		return pro_load_string_char_1(s);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			detail::is_sizeof_char_1_byte<Char>,
			void
		>::type
		load(std::basic_string<Char, Traits, Alloc>& s)
	{
		return pro_load_string_char_n(s);
	}

	template<typename Char, typename Traits, typename Alloc> inline
	typename
		boost::disable_if
		<
			detail::is_sizeof_char_1_byte<Char>,
			void
		>::type
		load(boost::container::basic_string<Char, Traits, Alloc>& s)
	{
		return pro_load_string_char_n(s);
	}

#ifndef BOOST_NO_CWCHAR
	inline void load(wchar_t* ws)
	{
		ser_size_type l = 0; // number of wchar_t !!!
		base_type::This()->load(l);
		base_type::load_binary(ws, l * sizeof(wchar_t)/* / sizeof(char)*/);
		ws[l] = L'\0';
	}
#endif // BOOST_NO_CWCHAR

#ifndef BOOST_NO_STD_WSTRING
	
	template<typename Traits, typename Alloc> inline
	void load(std::basic_string<wchar_t, Traits, Alloc>& ws)
	{
		return pro_load_string_char_n(ws);
	}

	template<typename Traits, typename Alloc> inline
	void load(boost::container::basic_string<wchar_t, Traits, Alloc>& ws)
	{
		return pro_load_string_char_n(ws);
	}

#endif // BOOST_NO_STD_WSTRING

#if !defined(YGGR_NO_CHAR8_T)
	inline void load(c8* s)
	{
		ser_size_type l = 0;
		base_type::This()->load(l);
		base_type::load_binary(s, l);
		s[l] = (u8'\0');
	}
#endif //YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_U8STRING_T)
	template<typename Traits, typename Alloc> inline
	void load(std::basic_string<c8, Traits, Alloc>& s)
	{
		return pro_load_string_char_1(s);
	}

	template<typename Traits, typename Alloc> inline
	void load(boost::container::basic_string<c8, Traits, Alloc>& s)
	{
		return pro_load_string_char_1(s);
	}
#endif // YGGR_NO_U8STRING_T

#if !defined(YGGR_NO_CHAR16_T)
	inline void load(c16* s)
	{
		ser_size_type l = 0; // number of wchar_t !!!
		base_type::This()->load(l);
		base_type::load_binary(s, l * sizeof(c16)/* / sizeof(char)*/);
		s[l] = u'\0';
	}
#endif //YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_U16STRING_T)
	template<typename Traits, typename Alloc> inline
	void load(std::basic_string<c16, Traits, Alloc>& s)
	{
		return pro_load_string_char_n(s);
	}

	template<typename Traits, typename Alloc> inline
	void load(boost::container::basic_string<c16, Traits, Alloc>& s)
	{
		return pro_load_string_char_n(s);
	}
#endif // YGGR_NO_U16STRING_T

#if !defined(YGGR_NO_CHAR32_T)
	inline void load(c32* s)
	{
		ser_size_type l = 0; // number of wchar_t !!!
		base_type::This()->load(l);
		base_type::load_binary(s, l * sizeof(c32)/* / sizeof(char)*/);
		s[l] = U'\0';
	}
#endif //YGGR_NO_CHAR32_T

#if !defined(YGGR_NO_U32STRING_T)
	template<typename Traits, typename Alloc> inline
	void load(std::basic_string<c32, Traits, Alloc>& s)
	{
		return pro_load_string_char_n(s);
	}

	template<typename Traits, typename Alloc> inline
	void load(boost::container::basic_string<c32, Traits, Alloc>& s)
	{
		return pro_load_string_char_n(s);
	}
#endif // YGGR_NO_U32STRING_T

	inline void load(boost::serialization::collection_size_type& t)
	{
		//std::size_t tmp = 0;
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

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

#endif // __YGGR_BOOST_ARCHIVE_BINARY_IARCHIVE_IMPL_HPP__
