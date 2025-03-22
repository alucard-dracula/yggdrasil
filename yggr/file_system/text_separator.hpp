// text_separator.hpp

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

#ifndef __YGGR_FILE_SYSTEM_TEXT_SEPARATOR_HPP__
#define __YGGR_FILE_SYSTEM_TEXT_SEPARATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/const_strings.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/container/clear.hpp>

#include <yggr/type_traits/native_t.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_const.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>

#include <iostream>


namespace yggr
{
namespace file_system
{

// meta
struct tag_text_separator
{
};

namespace detail
{

template<typename Container>
struct _has_fsys_text_separator
{
private:
	typedef Container cont_type;

	typedef u8 true_type;
	typedef u32 false_type;

	template<typename T>
	static true_type chk_foo(typename ::yggr::native_t<typename T::fsys_text_separator>::type*);

	template<typename T>
	static false_type chk_foo(...);

public:
	typedef boost::mpl::bool_<(sizeof(true_type) == sizeof(chk_foo<cont_type>(0)))> type;
};

} // namespace detail

template<typename Container>
struct is_fsys_text_separator
	: public detail::_has_fsys_text_separator<Container>::type
{
};

// data
namespace detail
{

struct default_text_separator
{
public:

	//symbol
	template<typename Char> inline
	static Char symbol_space(void)
	{
		YGGR_PP_CONST_CHAR_LOCAL_DEF(symbol_space, ' ');
		return YGGR_PP_CONST_CHAR_LOCAL_GET(symbol_space, Char);
	}

	template<typename Char> inline
	static Char symbol_lf(void)
	{
		YGGR_PP_CONST_CHAR_LOCAL_DEF(symbol_lf, '\r');
		return YGGR_PP_CONST_CHAR_LOCAL_GET(symbol_lf, Char);
	}

	template<typename Char> inline
	static Char symbol_cr(void)
	{
		YGGR_PP_CONST_CHAR_LOCAL_DEF(symbol_cr, '\n');
		return YGGR_PP_CONST_CHAR_LOCAL_GET(symbol_cr, Char);
	}

	template<typename Char> inline
	static Char symbol_comma(void)
	{
		YGGR_PP_CONST_CHAR_LOCAL_DEF(symbol_comma, ',');
		return YGGR_PP_CONST_CHAR_LOCAL_GET(symbol_comma, Char);
	}

	// symbol_spr
	template<typename Char> inline
	static const Char* symbol_spr_empty(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_spr, "");
		return YGGR_PP_CONST_STRING_LOCAL_GET(symbol_spr, Char);
	}

	template<typename Char> inline
	static const Char* symbol_spr_space(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_spr, " ");
		return YGGR_PP_CONST_STRING_LOCAL_GET(symbol_spr, Char);
	}

	template<typename Char> inline
	static const Char* symbol_spr_lf(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_spr, "\r");
		return YGGR_PP_CONST_STRING_LOCAL_GET(symbol_spr, Char);
	}

	template<typename Char> inline
	static const Char* symbol_spr_cr(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_spr, "\n");
		return YGGR_PP_CONST_STRING_LOCAL_GET(symbol_spr, Char);
	}

	template<typename Char> inline
	static const Char* symbol_spr_comma(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_spr, ",");
		return YGGR_PP_CONST_STRING_LOCAL_GET(symbol_spr, Char);
	}

	template<typename Char> inline
	static const Char* symbol_spr_newline(void)
	{
		YGGR_PP_CONST_STRING_LOCAL_DEF(symbol_spr, "\r\n");
		return YGGR_PP_CONST_STRING_LOCAL_GET(symbol_spr, Char);
	}
};

} // namespace detail

// ""
struct text_separator_empty
{
	typedef tag_text_separator fsys_text_separator;
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::is_const< Basic_String<Char, Traits, Alloc> >
			>,
			charset::is_basic_string_t< Basic_String<Char, Traits, Alloc> >
		>,
		Basic_String<Char, Traits, Alloc>&
	>::type
	to_text_separator_string(Basic_String<Char, Traits, Alloc>& str, const text_separator_empty&)
{
	container::clear(str);
	return str;
}

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const text_separator_empty&)
{
	return os;
}

// " "
struct text_separator_space
{
	typedef tag_text_separator fsys_text_separator;
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::is_const< Basic_String<Char, Traits, Alloc> >
			>,
			charset::is_basic_string_t< Basic_String<Char, Traits, Alloc> >
		>,
		Basic_String<Char, Traits, Alloc>&
	>::type
	to_text_separator_string(Basic_String<Char, Traits, Alloc>& str, const text_separator_space&)
{
	str = detail::default_text_separator::symbol_spr_space<Char>();
	return str;
}

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const text_separator_space&)
{
	os << detail::default_text_separator::symbol_spr_space<Char>();
	return os;
}

// "\r\n"
struct text_separator_newline
{
	typedef tag_text_separator fsys_text_separator;
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::is_const< Basic_String<Char, Traits, Alloc> >
			>,
			charset::is_basic_string_t< Basic_String<Char, Traits, Alloc> >
		>,
		Basic_String<Char, Traits, Alloc>&
	>::type
	to_text_separator_string(Basic_String<Char, Traits, Alloc>& str, const text_separator_newline&)
{
	str = detail::default_text_separator::symbol_spr_newline<Char>();
	return str;
}

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const text_separator_newline&)
{
	os << detail::default_text_separator::symbol_spr_newline<Char>();
	return os;
}

// "\r"
struct text_separator_lf
{
	typedef tag_text_separator fsys_text_separator;
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::is_const< Basic_String<Char, Traits, Alloc> >
			>,
			charset::is_basic_string_t< Basic_String<Char, Traits, Alloc> >
		>,
		Basic_String<Char, Traits, Alloc>&
	>::type
	to_text_separator_string(Basic_String<Char, Traits, Alloc>& str, const text_separator_lf&)
{
	str = detail::default_text_separator::symbol_spr_lf<Char>();
	return str;
}

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const text_separator_lf&)
{
	os << detail::default_text_separator::symbol_spr_lf<Char>();
	return os;
}

// "\n"
struct text_separator_cr
{
	typedef tag_text_separator fsys_text_separator;
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::is_const< Basic_String<Char, Traits, Alloc> >
			>,
			charset::is_basic_string_t< Basic_String<Char, Traits, Alloc> >
		>,
		Basic_String<Char, Traits, Alloc>&
	>::type
	to_text_separator_string(Basic_String<Char, Traits, Alloc>& str, const text_separator_cr&)
{
	str = detail::default_text_separator::symbol_spr_cr<Char>();
	return str;
}

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const text_separator_cr&)
{
	os << detail::default_text_separator::symbol_spr_cr<Char>();
	return os;
}

// ","
struct text_separator_comma
{
	typedef tag_text_separator fsys_text_separator;
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::is_const< Basic_String<Char, Traits, Alloc> >
			>,
			charset::is_basic_string_t< Basic_String<Char, Traits, Alloc> >
		>,
		Basic_String<Char, Traits, Alloc>&
	>::type
	to_text_separator_string(Basic_String<Char, Traits, Alloc>& str, const text_separator_comma&)
{
	str = detail::default_text_separator::symbol_spr_comma<Char>();
	return str;
}

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const text_separator_comma&)
{
	os << detail::default_text_separator::symbol_spr_comma<Char>();
	return os;
}

// other
template<typename Spr>
struct text_separator
{
public:
	typedef tag_text_separator fsys_text_separator;
	typedef Spr spr_type;

public:
	text_separator(const spr_type& spr)
		: _spr(spr)
	{
	}

	~text_separator(void)
	{
	}

public:
	inline const spr_type& operator()(void) const
	{
		return _spr;
	}

private:
	const spr_type& _spr;
};

template<typename Char, typename Traits, typename Alloc, typename Spr,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::not_
			<
				boost::is_const< Basic_String<Char, Traits, Alloc> >
			>,
			charset::is_basic_string_t< Basic_String<Char, Traits, Alloc> >
		>,
		Basic_String<Char, Traits, Alloc>&
	>::type
	to_text_separator_string(Basic_String<Char, Traits, Alloc>& str, const text_separator<Spr>& spr)
{
	std::basic_stringstream<Char> ss;
	ss << spr();
	ss >> str;
	return str;
}

template<typename Char, typename Traits, typename Spr> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const text_separator<Spr>& spr)
{
	os << spr();
	return os;
}

} // namesapce file_system
} // namespace yggr

#endif // __YGGR_FILE_SYSTEM_TEXT_SEPARATOR_HPP__
