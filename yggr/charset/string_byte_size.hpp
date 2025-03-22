//string_byte_size.hpp

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

#ifndef __YGGR_CHARSET_STRING_BYTE_SIZE_HPP__
#define __YGGR_CHARSET_STRING_BYTE_SIZE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/charset/assert_char.hpp>

#include <boost/container/string.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/or.hpp>
#include <boost/range/functions.hpp>

#include <cstring>
#include <string>

namespace yggr
{
namespace charset
{
namespace detail
{
struct string_byte_size_helper
{
private:
	typedef string_byte_size_helper this_type;

public:
	inline static size_type string_byte_size(const char* str)
	{
		return std::strlen(str);
	}

	inline static size_type char_buffer_byte_size(const char* str)
	{
		return std::strlen(str) + sizeof(char);
	}

#ifndef YGGR_NO_CWCHAR 
	inline static size_type string_byte_size(const wchar_t* str)
	{
		return std::wcslen(str) * sizeof(wchar_t);
	}

	inline static size_type char_buffer_byte_size(const wchar_t* str)
	{
		return std::wcslen(str) * sizeof(wchar_t) + sizeof(wchar_t);
	}
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
	inline static size_type string_byte_size(const ::yggr::c8* str)
	{
		return std::strlen(reinterpret_cast<const char*>(str));
	}

	inline static size_type char_buffer_byte_size(const ::yggr::c8* str)
	{
		return std::strlen(reinterpret_cast<const char*>(str)) + sizeof(::yggr::c8);
	}
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
	inline static size_type string_byte_size(const ::yggr::c16* str)
	{
		typedef std::char_traits< ::yggr::c16> traits_type;
		return traits_type::length(str) * sizeof(::yggr::c16);
	}

	inline static size_type char_buffer_byte_size(const ::yggr::c16* str)
	{
		typedef std::char_traits< ::yggr::c16> traits_type;
		return traits_type::length(str) * sizeof(::yggr::c16) + sizeof(::yggr::c16);
	}
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
	inline static size_type string_byte_size(const ::yggr::c32* str)
	{
		typedef std::char_traits< ::yggr::c32> traits_type;
		return traits_type::length(str) * sizeof(::yggr::c32);
	}

	inline static size_type char_buffer_byte_size(const ::yggr::c32* str)
	{
		typedef std::char_traits< ::yggr::c32> traits_type;
		return traits_type::length(str) * sizeof(::yggr::c32) + sizeof(::yggr::c32);
	}
#endif // YGGR_NO_CHAR32_T

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static typename boost::enable_if< is_native_char<Char>, size_type >::type
		string_byte_size(const Basic_String<Char, Traits, Alloc>& str)
	{
		return str.size() * sizeof(Char);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	static typename boost::enable_if< is_native_char<Char>, size_type >::type
		char_buffer_byte_size(const Basic_String<Char, Traits, Alloc>& str)
	{
		return str.size() * sizeof(Char) + sizeof(Char);
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView> inline
	static typename boost::enable_if< is_native_char<Char>, size_type >::type
		string_byte_size(const BasicStringView<Char, Traits>& str_view)
	{
		return str_view.size() * sizeof(Char);
	}

	template<typename Char, typename Traits,
				template<typename _Char, typename _Traits> class BasicStringView> inline
	static typename boost::enable_if< is_native_char<Char>, size_type >::type
		char_buffer_byte_size(const BasicStringView<Char, Traits>& str_view)
	{
		return str_view.size() * sizeof(Char) + sizeof(Char);
	}
};

} // namespace detail
} // namespace charset
} // namespace detail

// ----------------------string_byte_size_t------------------------

namespace yggr
{
namespace charset
{
namespace detail
{

template<typename String>
struct inner_string_byte_size_t
{
	typedef String cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef typename type::value_type cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef const type& param_type;

	inline size_type operator()(param_type str) const
	{
		return boost::size(str) * sizeof(value_type);
	}
};

template<typename Char>
struct inner_string_byte_size_t<Char*>
{
	typedef Char* cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef Char cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef const value_type* param_type;

	inline u64 operator()(param_type str) const
	{
		return string_byte_size_helper::string_byte_size(str);
	}
};

template<typename Char, std::size_t N>
struct inner_string_byte_size_t<Char[N]>
{
	typedef Char cv_type[N];
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef Char cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef const type& param_type;

	inline u64 operator()(param_type str) const
	{
		return string_byte_size_helper::string_byte_size(str);
	}
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
struct inner_string_byte_size_t< Basic_String<Char, Traits, Alloc> >
{
	typedef Basic_String<Char, Traits, Alloc> cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef typename type::value_type cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef const type& param_type;

	inline u64 operator()(param_type str) const
	{
		return string_byte_size_helper::string_byte_size(str);
	}
};

template<typename Char, typename Traits,
			template<typename _Char, typename _Traits> class BasicStringView >
struct inner_string_byte_size_t< BasicStringView<Char, Traits> >
{
	typedef BasicStringView<Char, Traits> cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef typename type::value_type cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef const type& param_type;

	inline u64 operator()(param_type str) const
	{
		return string_byte_size_helper::string_byte_size(str);
	}
};


template<typename T>
struct string_byte_size_t
{
private:
	typedef typename  boost::remove_reference
			<
				typename boost::remove_cv<T>::type
			>::type inner_param_type;
	typedef inner_string_byte_size_t<inner_param_type> inner_type;

public:
	typedef typename inner_type::type type;
	typedef typename inner_type::value_type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef typename inner_type::param_type param_type;

	inline u64 operator()(param_type str) const
	{
		inner_type inner;
		return inner(str);
	}
};

} // namespace detail

template<typename Char> inline
u64 string_byte_size(const Char* str)
{
	typedef Char* cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef detail::string_byte_size_t<type> h_type;
	h_type h;
	return h(str);
}

template<typename Char, std::size_t N> inline
u64 string_byte_size(const Char str[N])
{
	typedef Char cv_type[N];
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef detail::string_byte_size_t<type> h_type;
	h_type h;
	return h(str);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
u64 string_byte_size(const Basic_String<Char, Traits, Alloc>& str)
{
	typedef Basic_String<Char, Traits, Alloc> cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef detail::string_byte_size_t<type> h_type;
	h_type h;
	return h(str);
}

template<typename Char, typename Traits,
			template<typename _Char, typename _Traits> class BasicStringView> inline
u64 string_byte_size(const BasicStringView<Char, Traits>& str)
{
	typedef BasicStringView<Char, Traits> cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef detail::string_byte_size_t<type> h_type;
	h_type h;
	return h(str);
}

} // namespace charset
} // namespace yggr

// ------------------string_buffer_byte_size_t---------------------

namespace yggr
{
namespace charset
{
namespace detail
{
template<typename String>
struct inner_char_buffer_byte_size_t
{
	typedef String cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef typename type::value_type cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef const type& param_type;

	inline u64 operator()(param_type str) const
	{
		return str.size() * sizeof(value_type) + sizeof(value_type);
	}
};

template<typename Char>
struct inner_char_buffer_byte_size_t<Char*>
{
	typedef Char* cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef Char cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef const value_type* param_type;

	inline u64 operator()(param_type str) const
	{
		return string_byte_size_helper::char_buffer_byte_size(str);
	}
};

template<typename Char, std::size_t N>
struct inner_char_buffer_byte_size_t<Char[N]>
{
	typedef Char cv_type[N];
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef Char cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef const type& param_type;

	inline u64 operator()(param_type str) const
	{
		return string_byte_size_helper::char_buffer_byte_size(str);
	}
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String >
struct inner_char_buffer_byte_size_t< Basic_String<Char, Traits, Alloc> >
{
	typedef Basic_String<Char, Traits, Alloc> cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef typename type::value_type cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef const type& param_type;

	inline u64 operator()(param_type str) const
	{
		return string_byte_size_helper::char_buffer_byte_size(str);
	}
};

template<typename T>
struct char_buffer_byte_size_t
{
private:
	typedef typename boost::remove_reference
			<
				typename boost::remove_cv<T>::type
			>::type inner_param_type;
	typedef inner_char_buffer_byte_size_t<inner_param_type> inner_type;

public:
	typedef typename inner_type::type type;
	typedef typename inner_type::value_type value_type;
	YGGR_ASSERT_CHAR_TYPE(value_type);
	typedef typename inner_type::param_type param_type;

	inline u64 operator()(param_type str) const
	{
		inner_type inner;
		return inner(str);
	}
};

} // namespace detail

template<typename Char> inline
u64 char_buffer_byte_size(const Char* str)
{
	typedef Char* cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef detail::char_buffer_byte_size_t<type> h_type;
	h_type h;
	return h(str);
}

template<typename Char, std::size_t N> inline
u64 char_buffer_byte_size(const Char str[N])
{
	typedef Char cv_type[N];
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef detail::char_buffer_byte_size_t<type> h_type;
	h_type h;
	return h(str);
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
u64 char_buffer_byte_size(const Basic_String<Char, Traits, Alloc>& str)
{
	typedef Basic_String<Char, Traits, Alloc> cv_type;
	typedef typename boost::remove_cv<cv_type>::type type;
	typedef detail::char_buffer_byte_size_t<type> h_type;
	h_type h;
	return h(str);
}

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_STRING_BYTE_SIZE_HPP__
