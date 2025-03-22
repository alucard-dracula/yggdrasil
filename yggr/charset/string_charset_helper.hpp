//string_charset_helper.hpp

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

#ifndef __YGGR_CHARSET_STRING_CHARSET_HELPER_HPP__
#define __YGGR_CHARSET_STRING_CHARSET_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/nonable.hpp>
#include <yggr/typeof/typeof.hpp>

#include <yggr/charset/string_decl.hpp>
#include <yggr/charset/default_charset_config.hpp>
#include <yggr/charset/charset_type_traits.hpp>

#include <yggr/charset/detail/utf8_string_t_getter.hpp>
#include <yggr/charset/detail/utf8_string_or_string_view_t_getter.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>

#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace yggr
{
namespace charset
{

template<typename T1, typename T2>
class string_charset_helper;

template<typename T>
struct is_string_charset_helper
	: public boost::mpl::false_
{
};

template<typename T1, typename T2>
struct is_string_charset_helper< string_charset_helper<T1, T2> >
	: public boost::mpl::true_
{
};


template<typename T>
struct is_string_charset_helper_ex
	: public
		is_string_charset_helper
		<
			typename
				boost::remove_cv
				<
					typename boost::remove_reference<T>::type
				>::type
		>::type
{
};

template<typename T1, typename T2>
struct is_convertible_utf8_string< string_charset_helper<T1, T2> >
	: public boost::mpl::true_
{
};

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

template<typename T1, typename T2 = const yggr::string&>
class string_charset_helper
	: private nonable::nonmoveable
{
public:
	typedef T1 first_type;
	typedef T2 second_type;

	typedef typename
		boost::remove_cv
		<
			typename boost::remove_reference<first_type>::type
		>::type native_first_type;

	typedef typename
		boost::remove_cv
		<
			typename boost::remove_reference<second_type>::type
		>::type native_second_type;

private:
	typedef string_charset_helper this_type;

public:
	string_charset_helper(first_type t1, second_type t2)
		: first(t1), second(t2)
	{
	}

	string_charset_helper(const this_type& right)
		: first(right.first), second(right.second)
	{
	}

public:
	first_type first;
	second_type second;
};

} // namespace charset
} // namespace yggr

YGGR_TYPEOF_REGISTER_TEMPLATE(yggr::charset::string_charset_helper, 2)

namespace yggr
{
namespace charset
{

namespace detail
{

// string_charset_helper_first_t_helper
template<typename T>
struct string_charset_helper_first_t_helper
{
	typedef T type;
};

template<typename T1, typename T2>
struct string_charset_helper_first_t_helper< string_charset_helper<T1, T2> >
	: public
		boost::remove_cv
		<
			typename boost::remove_reference<T1>::type
		>
{
};

//string_charset_helper_second_t_helper
template<typename T>
struct string_charset_helper_second_t_helper
{
	typedef const string& type;
};

template<typename T1, typename T2>
struct string_charset_helper_second_t_helper< string_charset_helper<T1, T2> >
{
	typedef T2 type;
};

} // namespace detail

template<typename T>
struct string_charset_helper_first_t
	: public detail::string_charset_helper_first_t_helper<T>
{
};

template<typename T>
struct string_charset_helper_second_t
	: public detail::string_charset_helper_second_t_helper<T>
{
};

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

// helper
// make_string_charset_helper

// char
template<typename Char> inline
typename
	boost::enable_if
	<
		is_can_be_conv_into_char<Char>,
		string_charset_helper<YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char), const string&>
	>::type
	make_string_charset_helper(Char src)
{
	typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(static_cast<first_type>(src), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename Char> inline
typename
	boost::enable_if
	<
		is_can_be_conv_into_char<Char>,
		string_charset_helper<YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char), const string&>
	>::type
	make_string_charset_helper(Char src, const string& src_charset_name)
{
	typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(static_cast<first_type>(src), src_charset_name);
}

// const Char*
template<typename Char> inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		string_charset_helper<const Char*, const string&>
	>::type
	make_string_charset_helper(const Char* src)
{
	typedef const Char* first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename Char> inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		string_charset_helper<const Char*, const string&>
	>::type
	make_string_charset_helper(const Char* src, const string& src_charset_name)
{
	typedef const Char* first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, src_charset_name);
}

// Char*
template<typename Char> inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		string_charset_helper<Char*, const string&>
	>::type
	make_string_charset_helper(Char* src)
{
	typedef Char* first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
}

template<typename Char> inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		string_charset_helper<Char*, const string&>
	>::type
	make_string_charset_helper(Char* src, const string& src_charset_name)
{
	typedef Char* first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, src_charset_name);
}

// Char[] // don't decl conflict const Char*

//template<typename Char, std::size_t N> inline
//typename
//	boost::enable_if
//	<
//		is_native_char<Char>,
//		string_charset_helper<Char*, const string&>
//	>::type
//	make_string_charset_helper(Char (&src)[N])
//{
//	typedef Char* first_type;
//	typedef const string& second_type;
//	typedef string_charset_helper<first_type, second_type> ret_type;
//
//	return ret_type(boost::addressof(src[0]), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
//}
//
//template<typename Char, std::size_t N> inline
//typename
//	boost::enable_if
//	<
//		is_native_char<Char>,
//		string_charset_helper<Char*, const string&>
//	>::type
//	make_string_charset_helper(Char (&src)[N], const string& src_charset_name)
//{
//	typedef Char* first_type;
//	typedef const string& second_type;
//	typedef string_charset_helper<first_type, second_type> ret_type;
//
//	return ret_type(&src[0], src_charset_name);
//}
//
//template<typename Char, std::size_t N> inline
//typename
//	boost::enable_if
//	<
//		is_native_char<Char>,
//		string_charset_helper<const Char*, const string&>
//	>::type
//	make_string_charset_helper(const Char (&src)[N])
//{
//	typedef const Char* first_type;
//	typedef const string& second_type;
//	typedef string_charset_helper<first_type, second_type> ret_type;
//
//	return ret_type(boost::addressof(src[0]), YGGR_STR_STRING_DEFAULT_CHARSET_NAME(Char));
//}
//
//template<typename Char, std::size_t N> inline
//typename
//	boost::enable_if
//	<
//		is_native_char<Char>,
//		string_charset_helper<const Char*, const string&>
//	>::type
//	make_string_charset_helper(const Char (&src)[N], const string& src_charset_name)
//{
//	typedef const Char* first_type;
//	typedef const string& second_type;
//	typedef string_charset_helper<first_type, second_type> ret_type;
//
//	return ret_type(&src[0], src_charset_name);
//}

// !!! why use 2 versions, because MSVC will generate a C1001 error !!!

// basic_string& or utf8_string&
template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		string_charset_helper<Basic_String<Char, Traits, Alloc>&, const string&>
	>::type
	make_string_charset_helper(Basic_String<Char, Traits, Alloc>& src)
{
	typedef Basic_String<Char, Traits, Alloc> first_value_type;
	typedef first_value_type& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(first_value_type));
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		string_charset_helper<Basic_String<Char, Traits, Alloc>&, const string&>
	>::type
	make_string_charset_helper(Basic_String<Char, Traits, Alloc>& src, const string& src_charset_name)
{
	typedef Basic_String<Char, Traits, Alloc>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, src_charset_name);
}

// const basic_string& or const utf8_string&

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		string_charset_helper<const Basic_String<Char, Traits, Alloc>&, const string&>
	>::type
	make_string_charset_helper(const Basic_String<Char, Traits, Alloc>& src)
{
	typedef Basic_String<Char, Traits, Alloc> first_value_type;
	typedef const first_value_type& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(first_value_type));
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		string_charset_helper<const Basic_String<Char, Traits, Alloc>&, const string&>
	>::type
	make_string_charset_helper(const Basic_String<Char, Traits, Alloc>& src,
								const string& src_charset_name)
{
	typedef const Basic_String<Char, Traits, Alloc>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, src_charset_name);
}

template<typename Char, typename Traits,
			template<typename _Char, typename _Traits> class Basic_String_View > inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		string_charset_helper<const Basic_String_View<Char, Traits>&, const string&>
	>::type
	make_string_charset_helper(const Basic_String_View<Char, Traits>& src)
{
	typedef Basic_String_View<Char, Traits> first_value_type;
	typedef const first_value_type& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, YGGR_STR_STRING_DEFAULT_CHARSET_NAME(first_value_type));
}

template<typename Char, typename Traits,
			template<typename _Char, typename _Traits> class Basic_String_View > inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		string_charset_helper<const Basic_String_View<Char, Traits>&, const string&>
	>::type
	make_string_charset_helper(const Basic_String_View<Char, Traits>& src,
								const string& src_charset_name)
{
	typedef const Basic_String_View<Char, Traits>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, src_charset_name);
}

template<typename T1, typename T2> inline
const string_charset_helper<T1, T2>&
	make_string_charset_helper(const string_charset_helper<T1, T2>& src)
{
	return src;
}

template<typename T1, typename T2, typename T3> inline
const string_charset_helper<T1, T2>&
	make_string_charset_helper(const string_charset_helper<T1, T2>& src, const T3&)
{
	return src;
}


//--------------- string_charset_helper_data-----------------

template<typename Char> inline
typename
	boost::enable_if
	<
		is_can_be_conv_into_char<Char>,
		string_charset_helper<YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char), const string&>
	>::type
	string_charset_helper_data(Char src,
								const string& src_charset_name = charset_name_t<Char>())
{
	typedef YGGR_PP_FORMAT_CHAR_TYPE_TPL(Char) first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, src_charset_name);
}

template<typename Char, typename Charset_Name> inline
typename
	boost::enable_if
	<
		is_can_be_conv_into_char<Char>,
		const string_charset_helper<Char, Charset_Name>&
	>::type
	string_charset_helper_data(const string_charset_helper<Char, Charset_Name>& src)
{
	return src;
}

template<typename Char> inline
typename
	boost::enable_if
	<
		is_can_be_conv_into_char<Char>,
		string_charset_helper<const Char*, const string&>
	>::type
	string_charset_helper_data(const Char* src, const string& src_charset_name = charset_name_t<Char>())
{
	typedef const Char* first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;
	return ret_type(src, src_charset_name);
}

template<typename Char, typename Charset_Name> inline
typename
	boost::enable_if
	<
		is_can_be_conv_into_char<typename boost::remove_const<Char>::type>,
		const string_charset_helper<Char*, Charset_Name>&
	>::type
	string_charset_helper_data(const string_charset_helper<Char*, Charset_Name>& src)
{
	return src;
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename
	boost::enable_if
	<
		is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
		string_charset_helper<const Basic_String<Char, Traits, Alloc>&, const string&>
	>::type
	string_charset_helper_data(const Basic_String<Char, Traits, Alloc>& src,
								const string& src_charset_name = charset_name_t< Basic_String<Char, Traits, Alloc> >())
{
	typedef const Basic_String<Char, Traits, Alloc>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, src_charset_name);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
typename
	boost::enable_if
	<
		is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
		string_charset_helper<Basic_String<Char, Traits, Alloc>&, const string&>
	>::type
	string_charset_helper_data(Basic_String<Char, Traits, Alloc>& src,
								const string& src_charset_name = charset_name_t< Basic_String<Char, Traits, Alloc> >())
{
	typedef Basic_String<Char, Traits, Alloc>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, src_charset_name);
}

template<typename Basic_String_T, typename Charset_Name> inline
typename
	boost::enable_if
	<
		is_basic_string_t< typename boost::remove_const<Basic_String_T>::type >,
		const string_charset_helper<Basic_String_T&, Charset_Name>&
	>::type
	string_charset_helper_data(const string_charset_helper<Basic_String_T&, Charset_Name>& src)
{
	return src;
}

// basic_string_view
template<typename Char, typename Traits,
			template<typename _Char, typename _Traits> class Basic_StringView > inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< Basic_StringView<Char, Traits> >,
		string_charset_helper<const Basic_StringView<Char, Traits>&, const string&>
	>::type
	string_charset_helper_data(const Basic_StringView<Char, Traits>& src,
								const string& src_charset_name = charset_name_t< Basic_StringView<Char, Traits> >())
{
	typedef const Basic_StringView<Char, Traits>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, src_charset_name);
}

template<typename Char, typename Traits,
			template<typename _Char, typename _Traits> class Basic_StringView > inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< Basic_StringView<Char, Traits> >,
		string_charset_helper<const Basic_StringView<Char, Traits>&, const string&>
	>::type
	string_charset_helper_data(Basic_StringView<Char, Traits>& src,
								const string& src_charset_name = charset_name_t< Basic_StringView<Char, Traits> >())
{
	typedef Basic_StringView<Char, Traits>& first_type;
	typedef const string& second_type;
	typedef string_charset_helper<first_type, second_type> ret_type;

	return ret_type(src, src_charset_name);
}

template<typename Basic_StringView_T, typename Charset_Name> inline
typename
	boost::enable_if
	<
		is_basic_string_view_t< typename boost::remove_const<Basic_StringView_T>::type >,
		const string_charset_helper<Basic_StringView_T&, Charset_Name>&
	>::type
	string_charset_helper_data(const string_charset_helper<Basic_StringView_T&, Charset_Name>& src)
{
	return src;
}

//string_charset_helper_first
template<typename String> inline
const String& string_charset_helper_first(const String& src)
{
	return src;
}

template<typename String> inline
String& string_charset_helper_first(String& src)
{
	return src;
}

template<typename String, typename Charset_Name> inline
const String& string_charset_helper_first(const String& src, const Charset_Name&)
{
	return src;
}

template<typename String, typename Charset_Name> inline
String& string_charset_helper_first(String& src, const Charset_Name&)
{
	return src;
}

template<typename T1, typename T2> inline
T1 string_charset_helper_first(const string_charset_helper<T1, T2>& src)
{
	return src.first;
}

template<typename T1, typename T2> inline
T1 string_charset_helper_first(string_charset_helper<T1, T2>& src)
{
	return src.first;
}

//string_charset_helper_second
template<typename String> inline
const string& string_charset_helper_second(const String&)
{
	return charset_name_t<String>();
}

template<typename String> inline
const string& string_charset_helper_second(String&)
{
	return charset_name_t<String>();
}

template<typename String, typename Charset_Name> inline
const Charset_Name& string_charset_helper_second(const String&, const Charset_Name& charset_name)
{
	return charset_name;
}

template<typename String, typename Charset_Name> inline
const Charset_Name& string_charset_helper_second(String&, const Charset_Name& charset_name)
{
	return charset_name;
}

template<typename T1, typename T2> inline
T2 string_charset_helper_second(const string_charset_helper<T1, T2>& src)
{
	return src.second;
}

template<typename T1, typename T2> inline
T2 string_charset_helper_second(string_charset_helper<T1, T2>& src)
{
	return src.second;
}

} // namespace charset
} // namespace yggr

// utf8_string_t_getter support
namespace yggr
{
namespace charset
{
namespace detail
{
template<typename T1, typename T2, typename Utf8String>
struct utf8_string_t_getter< string_charset_helper<T1, T2>, Utf8String >
	: public
		utf8_string_t_getter
		<
			typename
				boost::remove_cv
				<
					typename boost::remove_reference<T1>::type
				>::type,
			Utf8String
		>
{
protected:
	typedef
		utf8_string_t_getter
		<
			typename
				boost::remove_cv
				<
					typename boost::remove_reference<T1>::type
				>::type,
			Utf8String
		> base_type;
public:
	typedef typename base_type::nctype nctype;
	typedef typename base_type::ctype ctype;
private:
	BOOST_MPL_ASSERT((charset::is_utf8_string_t<Utf8String>));
};

} // namespace detail
} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_STRING_CHARSET_HELPER_HPP__

