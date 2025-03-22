//default_charset_config.hpp

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

#ifndef __YGGR_CHARSET_DEFAULT_CHARSET_CONFIG_HPP__
#define __YGGR_CHARSET_DEFAULT_CHARSET_CONFIG_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/nonable/nonable.hpp>
#include <yggr/type_traits/number_t.hpp>
#include <yggr/container/vector.hpp>

#include <yggr/charset/assert_char.hpp>
#include <yggr/charset/convertible_traits.hpp>
#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/string_decl.hpp>
#include <yggr/charset/string_view_decl.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <iconv.h>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

#ifdef TAG_YGGR_INNER_STRING_CHARSET_NAME
#	error "inner_string_charset_name must use utf-8"
#endif // TAG_YGGR_INNER_STRING_CHARSET_NAME

#define TAG_YGGR_INNER_STRING_CHARSET_NAME() "utf-8"
#define TAG_YGGR_UTF8_STRING_CHARSET_NAME TAG_YGGR_INNER_STRING_CHARSET_NAME

#if !defined (YGGR_SYS_DEFAULT_STRING_CHARSET_NAME_CFG)
#	if defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)
#		define YGGR_SYS_DEFAULT_STRING_CHARSET_NAME_CFG TAG_YGGR_INNER_STRING_CHARSET_NAME()
#	else
#		define YGGR_SYS_DEFAULT_STRING_CHARSET_NAME_CFG "char"
#	endif  // defined(YGGR_AT_LINUX) || defined(YGGR_AT_DARWIN)
#endif // YGGR_SYS_DEFAULT_STRING_CHARSET_NAME

#if !defined(YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME_CFG)
#	define YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME_CFG "wchar_t"
#endif // YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME_CFG

#if !defined (YGGR_SYS_DEFAULT_WSTRING_2_CHARSET_NAME_CFG)
#	define YGGR_SYS_DEFAULT_WSTRING_2_CHARSET_NAME_CFG "ucs-2-internal"
#endif // YGGR_SYS_DEFAULT_WSTRING_2_CHARSET_NAME

#if !defined (YGGR_SYS_DEFAULT_WSTRING_4_CHARSET_NAME_CFG)
#	define YGGR_SYS_DEFAULT_WSTRING_4_CHARSET_NAME_CFG "ucs-4-internal"
#endif // YGGR_SYS_DEFAULT_WSTRING_2_CHARSET_NAME

// char8_t
#if !defined (YGGR_SYS_DEFAULT_CHAR8_STRING_CHARSET_NAME_CFG)
#	define YGGR_SYS_DEFAULT_CHAR8_STRING_CHARSET_NAME_CFG TAG_YGGR_INNER_STRING_CHARSET_NAME() // "utf-8"
#endif // YGGR_SYS_DEFAULT_CHAR8_STRING_CHARSET_NAME_CFG

// char_16_t
#if !defined (YGGR_SYS_DEFAULT_CHAR16_STRING_CHARSET_NAME_CFG)
#	if defined(YGGR_LITTLE_ENDIAN)
#		define YGGR_SYS_DEFAULT_CHAR16_STRING_CHARSET_NAME_CFG "utf-16le"
#	elif defined(YGGR_BIG_ENDIAN)
#		define YGGR_SYS_DEFAULT_CHAR16_STRING_CHARSET_NAME_CFG "utf-16be"
#	else
#		define YGGR_SYS_DEFAULT_CHAR16_STRING_CHARSET_NAME_CFG "utf-16"
#	endif // YGGR_LITTLE_ENDIAN
#endif // YGGR_SYS_DEFAULT_CHAR16_STRING_CHARSET_NAME_CFG

#if !defined (YGGR_SYS_DEFAULT_CHAR32_STRING_CHARSET_NAME_CFG)
#	if defined(YGGR_LITTLE_ENDIAN)
#		define YGGR_SYS_DEFAULT_CHAR32_STRING_CHARSET_NAME_CFG "utf-32le"
#	elif defined(YGGR_BIG_ENDIAN)
#		define YGGR_SYS_DEFAULT_CHAR32_STRING_CHARSET_NAME_CFG "utf-32be"
#	else
#		define YGGR_SYS_DEFAULT_CHAR32_STRING_CHARSET_NAME_CFG "utf-32"
#	endif // YGGR_LITTLE_ENDIAN
#endif // YGGR_SYS_DEFAULT_CHAR32_STRING_CHARSET_NAME_CFG

#define YGGR_SYS_DEFAULT_STRING_CHARSET_NAME() YGGR_SYS_DEFAULT_STRING_CHARSET_NAME_CFG
#define YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME() YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME_CFG

#define YGGR_SYS_DEFAULT_WSTRING_2_CHARSET_NAME() YGGR_SYS_DEFAULT_WSTRING_2_CHARSET_NAME_CFG
#define YGGR_SYS_DEFAULT_WSTRING_4_CHARSET_NAME() YGGR_SYS_DEFAULT_WSTRING_4_CHARSET_NAME_CFG

#define YGGR_SYS_DEFAULT_CHAR8_STRING_CHARSET_NAME() YGGR_SYS_DEFAULT_CHAR8_STRING_CHARSET_NAME_CFG

#define YGGR_SYS_DEFAULT_CHAR16_STRING_CHARSET_NAME() YGGR_SYS_DEFAULT_CHAR16_STRING_CHARSET_NAME_CFG
#define YGGR_SYS_DEFAULT_CHAR16_STRING_LE_CHARSET_NAME() YGGR_SYS_DEFAULT_CHAR16_STRING_LE_CHARSET_NAME_CFG
#define YGGR_SYS_DEFAULT_CHAR16_STRING_BE_CHARSET_NAME() YGGR_SYS_DEFAULT_CHAR16_STRING_BE_CHARSET_NAME_CFG

#define YGGR_SYS_DEFAULT_CHAR32_STRING_CHARSET_NAME() YGGR_SYS_DEFAULT_CHAR32_STRING_CHARSET_NAME_CFG
#define YGGR_SYS_DEFAULT_CHAR32_STRING_LE_CHARSET_NAME() YGGR_SYS_DEFAULT_CHAR32_STRING_LE_CHARSET_NAME_CFG
#define YGGR_SYS_DEFAULT_CHAR32_STRING_BE_CHARSET_NAME() YGGR_SYS_DEFAULT_CHAR32_STRING_BE_CHARSET_NAME_CFG

#define YGGR_PP_MAKE_DEFAULT_CHARSET_NAME_T_BASIC(__class_name__, __name__) \
	struct BOOST_PP_CAT(defalut_charset_name_t_, __class_name__) { \
		YGGR_CXX23_CONSTEXPR_OR_INLINE static const char* s_charset_name(void) { \
			return s_str_charset_name().data(); } \
		\
		YGGR_CXX23_CONSTEXPR_OR_INLINE static const string& s_str_charset_name(void) { \
			YGGR_CXX23_CONSTEXPR_OR_STATIC_CONST string str_charset(__name__()); \
			return str_charset; } }

#define YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC( __class_name__ ) BOOST_PP_CAT(defalut_charset_name_t_, __class_name__)

namespace yggr
{
namespace charset
{

/*
warning:
const char* str1 = "string";
const char* str2 = "string";
assert(str1 == str2); // msvc is false, gcc is true
so can't write to this style
*/

namespace detail
{

// struct defalut_charset_name_t_default_string
YGGR_PP_MAKE_DEFAULT_CHARSET_NAME_T_BASIC(default_string, YGGR_SYS_DEFAULT_STRING_CHARSET_NAME);

// struct defalut_charset_name_t_default_wstring
YGGR_PP_MAKE_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring, YGGR_SYS_DEFAULT_WSTRING_CHARSET_NAME);

// struct defalut_charset_name_t_default_wstring_2
YGGR_PP_MAKE_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring_2, YGGR_SYS_DEFAULT_WSTRING_2_CHARSET_NAME);

// struct defalut_charset_name_t_default_wstring_4
YGGR_PP_MAKE_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring_4, YGGR_SYS_DEFAULT_WSTRING_4_CHARSET_NAME);

// struct defalut_charset_name_t_default_char8_string
YGGR_PP_MAKE_DEFAULT_CHARSET_NAME_T_BASIC(default_char8_string, YGGR_SYS_DEFAULT_CHAR8_STRING_CHARSET_NAME);

// struct defalut_charset_name_t_default_char16_string
YGGR_PP_MAKE_DEFAULT_CHARSET_NAME_T_BASIC(default_char16_string, YGGR_SYS_DEFAULT_CHAR16_STRING_CHARSET_NAME);

// struct defalut_charset_name_t_default_char32_string
YGGR_PP_MAKE_DEFAULT_CHARSET_NAME_T_BASIC(default_char32_string, YGGR_SYS_DEFAULT_CHAR32_STRING_CHARSET_NAME);

template<typename Char,
			std::size_t Char_Size = sizeof(Char),
#	if !defined YGGR_NO_CWCHAR
			std::size_t WChar_Size = sizeof(wchar_t)
#	else
#		if defined(YGGR_AT_WINDOWS)
			std::size_t WChar_Size = sizeof(s16)
#		else
			std::size_t WChar_Size = sizeof(s32)
#		endif // YGGR_AT_WINDOWS
#	endif // YGGR_NO_CWCHAR
			>
struct defalut_charset_name_t;

template<typename Char, std::size_t WChar_Size>
struct defalut_charset_name_t<Char, 1, WChar_Size>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_string)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_string) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#if defined(YGGR_NO_CHAR16_T)
template<typename Char, std::size_t WChar_Size>
struct defalut_charset_name_t<Char, 2, WChar_Size>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring_2)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring_2) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#else

template<typename Char, std::size_t WChar_Size>
struct defalut_charset_name_t<Char, 2, WChar_Size>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char16_string)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char16_string) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#endif // #if defined(YGGR_NO_CHAR16_T)

#if defined(YGGR_NO_CHAR32_T)

template<typename Char, std::size_t WChar_Size>
struct defalut_charset_name_t<Char, 4, WChar_Size>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring_4)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring_4) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#else

template<typename Char, std::size_t WChar_Size>
struct defalut_charset_name_t<Char, 4, WChar_Size>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char32_string)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char32_string) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#endif // YGGR_NO_CHAR32_T

#if !defined(YGGR_NO_CWCHAR)

#	if !defined(YGGR_NO_CHAR16_T)

template<>
struct defalut_charset_name_t<wchar_t, 2, 2>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char16_string)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char16_string) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

template<>
struct defalut_charset_name_t<wchar_t, 2, 4>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char16_string)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char16_string) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#	else

template<>
struct defalut_charset_name_t<wchar_t, 2, 2>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

template<>
struct defalut_charset_name_t<wchar_t, 2, 4>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring_2)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring_2) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#	endif // #	if !defined(YGGR_NO_CHAR16_T)

#	if !defined(YGGR_NO_CHAR32_T)

template<>
struct defalut_charset_name_t<wchar_t, 4, 2>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char32_string)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char32_string) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

template<>
struct defalut_charset_name_t<wchar_t, 4, 4>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char32_string)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char32_string) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#	else

template<>
struct defalut_charset_name_t<wchar_t, 4, 2>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring_4)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring_4) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

template<>
struct defalut_charset_name_t<wchar_t, 4, 4>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_wstring) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#endif // #	if !defined(YGGR_NO_CHAR32_T)

#endif // YGGR_NO_CWCHAR

#if !defined(YGGR_NO_CHAR8_T)

template<std::size_t WChar_Size>
struct defalut_charset_name_t< ::yggr::c8, 1, WChar_Size>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char8_string)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char8_string) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#endif // YGGR_NO_CHAR8_T

#if !defined(YGGR_NO_CHAR16_T)

template<std::size_t WChar_Size>
struct defalut_charset_name_t< ::yggr::c16, 2, WChar_Size>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char16_string)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char16_string) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#endif // YGGR_NO_CHAR16_T

#if !defined(YGGR_NO_CHAR32_T)

template<std::size_t WChar_Size>
struct defalut_charset_name_t< ::yggr::c32, 4, WChar_Size>
	: public YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char32_string)
{
public:
	typedef YGGR_PP_GET_DEFAULT_CHARSET_NAME_T_BASIC(default_char32_string) base_type;
public:
	using base_type::s_charset_name;
	using base_type::s_str_charset_name;
};

#endif // YGGR_NO_CHAR32_T

} // namespace detail

struct default_charset_config
{
private:
	typedef default_charset_config this_type;

public:
	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			const char*
		>::type
		get_default_string_charset_name(void)
	{
		typedef Char char_type;
		typedef detail::defalut_charset_name_t<char_type> name_type;

		return name_type::s_charset_name();
	}

	template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
	static
	typename
		boost::enable_if
		<
			is_can_be_conv_into_char<Char>,
			const string&
		>::type
		get_str_default_string_charset_name(void)
	{
		typedef Char char_type;
		typedef detail::defalut_charset_name_t<char_type> name_type;

		return name_type::s_str_charset_name();
	}

	YGGR_CXX23_CONSTEXPR_OR_INLINE static const char* get_inner_string_charset_name(void)
	{
		return this_type::get_str_inner_string_charset_name().data();
	}

	YGGR_CXX23_CONSTEXPR_OR_INLINE static const string& get_str_inner_string_charset_name(void)
	{
		static const string str_charset(TAG_YGGR_INNER_STRING_CHARSET_NAME());
		return str_charset;
	}

	YGGR_CXX23_CONSTEXPR_OR_INLINE static const char* get_utf8_string_charset_name(void)
	{
		return this_type::get_inner_string_charset_name();
	}

	YGGR_CXX23_CONSTEXPR_OR_INLINE static const string& get_str_utf8_string_charset_name(void)
	{
		return this_type::get_str_inner_string_charset_name();
	}
};

#define YGGR_INNER_STRING_CHARSET_NAME() \
	::yggr::charset::default_charset_config::get_inner_string_charset_name()

#define YGGR_STR_INNER_STRING_CHARSET_NAME() \
	::yggr::charset::default_charset_config::get_str_inner_string_charset_name()

#define YGGR_UTF8_STRING_CHARSET_NAME() \
	::yggr::charset::default_charset_config::get_utf8_string_charset_name()

#define YGGR_STR_UTF8_STRING_CHARSET_NAME() \
	::yggr::charset::default_charset_config::get_str_utf8_string_charset_name()


template<typename Char>
struct charset_name_t
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Char char_type;
	typedef typename boost::remove_cv<char_type>::type native_char_type;

	typedef typename
		boost::mpl::if_
		<
			is_native_char<native_char_type>,
			basic_string<native_char_type>,
			vector<native_char_type>
		>::type string_type;

	typedef basic_string_view<native_char_type> string_view_type;

public:
	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const char*(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_default_string_charset_name<native_char_type>();
	}

	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const string&(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_str_default_string_charset_name<native_char_type>();
	}
};

template<typename Char>
struct charset_name_t<Char*>
	: public charset_name_t<Char>
{
};

template<typename Char, std::size_t N>
struct charset_name_t<Char[N]>
	: public charset_name_t<Char>
{
};

namespace detail
{

template<typename String>
struct charset_name_t_string_impl
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef String string_type;
	typedef typename string_type::value_type char_type;
	typedef typename string_type::traits_type traits_type;
	typedef typename boost::remove_cv<char_type>::type native_char_type;
	typedef basic_string_view<native_char_type, traits_type> string_view_type;

	BOOST_MPL_ASSERT((is_basic_string_t<string_type>));

public:
	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const char*(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_default_string_charset_name<native_char_type>();
	}

	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const string&(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_str_default_string_charset_name<native_char_type>();
	}
};

template<typename StringView>
struct charset_name_t_string_view_impl
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef StringView string_view_type;
	typedef typename string_view_type::value_type char_type;
	typedef typename string_view_type::traits_type traits_type;
	typedef typename boost::remove_cv<char_type>::type native_char_type;
	typedef basic_string<native_char_type, traits_type> string_type;

	BOOST_MPL_ASSERT((is_basic_string_t<string_type>));

public:
	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const char*(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_default_string_charset_name<native_char_type>();
	}

	YGGR_CXX23_CONSTEXPR_OR_INLINE operator const string&(void) const YGGR_CXX11_NOEXCEPT_OR_NOTHROW
	{
		return default_charset_config::get_str_default_string_charset_name<native_char_type>();
	}
};

}// namespace detail


template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
struct charset_name_t< Basic_String<Char, Traits, Alloc> >
	: public detail::charset_name_t_string_impl< Basic_String<Char, Traits, Alloc> >
{
};

template<typename Char, typename Traits,
			template<typename _Char, typename _Traits> class Basic_String_View>
struct charset_name_t< Basic_String_View<Char, Traits> >
	: public detail::charset_name_t_string_view_impl< Basic_String_View<Char, Traits> >
{
};


template<typename CharL, typename CharR> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator==(const charset_name_t<CharL>& l, const charset_name_t<CharR>& r)
{
	return static_cast<const string&>(l) == static_cast<const string&>(r);
}

template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator==(const charset_name_t<Char>& l, const charset_name_t<Char>& r)
{
	return static_cast<const string&>(l) == static_cast<const string&>(r);
}

template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator==(const string& l, const charset_name_t<Char>& r)
{
	return l == static_cast<const string&>(r);
}

template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator==(const charset_name_t<Char>& l, const string& r)
{
	return static_cast<const string&>(l) == r;
}

template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator==(const char* l, const charset_name_t<Char>& r)
{
	return l == static_cast<const char*>(r);
}

template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator==(const charset_name_t<Char>& l, const char* r)
{
	return static_cast<const char*>(l) == r;
}

template<typename CharL, typename CharR> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator!=(const charset_name_t<CharL>& l, const charset_name_t<CharR>& r)
{
	return !(static_cast<const string&>(l) == static_cast<const string&>(r));
}

template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator!=(const charset_name_t<Char>& l, const charset_name_t<Char>& r)
{
	return !(static_cast<const string&>(l) == static_cast<const string&>(r));
}

template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator!=(const string& l, const charset_name_t<Char>& r)
{
	return !(l == static_cast<const string&>(r));
}

template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator!=(const charset_name_t<Char>& l, const string& r)
{
	return !(static_cast<const string&>(l) == r);
}

template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator!=(const char* l, const charset_name_t<Char>& r)
{
	return !(l == static_cast<const char*>(r));
}

template<typename Char> YGGR_CXX23_CONSTEXPR_OR_INLINE
bool operator!=(const charset_name_t<Char>& l, const char* r)
{
	return !(static_cast<const char*>(l) == r);
}

#define YGGR_STRING_DEFAULT_CHARSET_NAME( __char_t__ ) \
	static_cast<const char*>(::yggr::charset::charset_name_t< __char_t__ >())

#define YGGR_STR_STRING_DEFAULT_CHARSET_NAME( __char_t__ ) \
	static_cast<const yggr::string&>(::yggr::charset::charset_name_t< __char_t__ >())

#define YGGR_DEDUCED_DEFAULT_CHARSET_NAME( __char_t__ ) \
	static_cast<const char*>(::yggr::charset::charset_name_t< typename __char_t__::type >())

#define YGGR_DEDUCED_STR_DEFAULT_CHARSET_NAME( __char_t__ ) \
	static_cast<const yggr::string&>(::yggr::charset::charset_name_t< typename __char_t__::type >())

#define YGGR_CHARSET_MAKE_STRING(__basic_string__, __char__, __traits__, __alloc__ ) \
	__basic_string__< __char__, __traits__, __alloc__ >


} // namespace charset
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_CHARSET_DEFAULT_CHARSET_CONFIG_HPP__
