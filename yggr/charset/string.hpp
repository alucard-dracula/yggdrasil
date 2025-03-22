// string.hpp

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

#ifndef __YGGR_CHARSET_STRING_HPP__
#define __YGGR_CHARSET_STRING_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/typeof/typeof.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/typename_expand.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>
#include <yggr/iterator_ex/is_reverse_iterator.hpp>

#include <yggr/container/detail/has_shrink_to_fit.hpp>

#include <yggr/charset/assert_char.hpp>
#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/string_decl.hpp>
#include <yggr/charset/string_hash.hpp>

#include <yggr/charset/charset_foo.hpp>
#include <yggr/charset/string_byte_size.hpp>
#include <yggr/charset/other_char_chr_wchr.hpp>
#include <yggr/charset/other_string_str_wstr.hpp>
#include <yggr/charset/convertible_traits.hpp>
#include <yggr/charset/is_isomeric_same_string.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <iostream>
#include <algorithm>

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! fix boost::container::string's bugs															!!!
!!! the boost::container::string' getline is defined											!!!
!!!																								!!!
!!! template <class CharT, class Traits, class Allocator>										!!!
!!! std::basic_istream<CharT, Traits>&															!!!
!!!		getline(std::istream& is, basic_string<CharT,Traits,Allocator>& s,CharT delim);			!!!
!!!																								!!!
!!! and																							!!!
!!!																								!!!
!!! template <class CharT, class Traits, class Allocator>										!!!
!!! inline std::basic_istream<CharT, Traits>&													!!!
!!!	getline(std::basic_istream<CharT, Traits>& is, basic_string<CharT,Traits,Allocator>& s);	!!!
!!!																								!!!
!!! see the first params!																		!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

namespace boost
{
namespace container
{

template <typename CharT_OS, typename Traits_OS, 
			typename CharT, typename Traits, typename Allocator> inline
std::basic_ostream<CharT_OS, Traits_OS>&
	operator<<(std::basic_ostream<CharT_OS, Traits_OS>& os, 
				const basic_string<CharT, Traits, Allocator>& s)
{
	typedef basic_string<CharT, Traits, Allocator> input_string_type;
	typedef basic_string<CharT_OS, Traits_OS> target_string_type;

	target_string_type tmp;
	tmp.reserve(s.size());
	
	return 
		os << ::yggr::charset::charset_foo::s_xchg(
				tmp, s, 
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(CharT),
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(CharT_OS));
}


template <typename CharT_IS, typename Traits_IS, 
			typename CharT, typename Traits, typename Allocator> inline
std::basic_istream<CharT_IS, Traits_IS>&
	operator>>(std::basic_istream<CharT_IS, Traits_IS>& is, 
				basic_string<CharT, Traits, Allocator>& s)
{
	typedef basic_string<CharT, Traits, Allocator> input_string_type;
	typedef basic_string<CharT_IS, Traits_IS> target_string_type;

	target_string_type tmp;
	is >> tmp;
	
	::yggr::charset::charset_foo::s_xchg(
		s, tmp,
		YGGR_STR_STRING_DEFAULT_CHARSET_NAME(CharT), 
		YGGR_STR_STRING_DEFAULT_CHARSET_NAME(CharT_IS));
	return is;
}

} // namespace container
} // namespace boost

#if !(BOOST_VERSION < 107000)

namespace boost
{
namespace container
{

template<typename Char, typename Traits, typename Alloc1, typename Alloc2> inline
typename
	boost::enable_if
	<
		::yggr::charset::is_isomeric_same_string
		<
			boost::container::basic_string<Char, Traits, Alloc1>,
			boost::container::basic_string<Char, Traits, Alloc2>
		>,
		bool
	>::type
	operator==(const boost::container::basic_string<Char, Traits, Alloc1>& l,
				const boost::container::basic_string<Char, Traits, Alloc2>& r)
{
	typedef boost::container::basic_string<Char, Traits, Alloc1> now_string_type;

	return l == reinterpret_cast<const now_string_type&>(r);
}

template<typename Char, typename Traits, typename Alloc1, typename Alloc2> inline
typename
	boost::enable_if
	<
		::yggr::charset::is_isomeric_same_string
		<
			boost::container::basic_string<Char, Traits, Alloc1>,
			boost::container::basic_string<Char, Traits, Alloc2>
		>,
		bool
	>::type
	operator!=(const boost::container::basic_string<Char, Traits, Alloc1>& l,
				const boost::container::basic_string<Char, Traits, Alloc2>& r)
{
	typedef boost::container::basic_string<Char, Traits, Alloc1> now_string_type;

	return l != reinterpret_cast<const now_string_type&>(r);
}

template<typename Char, typename Traits, typename Alloc1, typename Alloc2> inline
typename
	boost::enable_if
	<
		::yggr::charset::is_isomeric_same_string
		<
			boost::container::basic_string<Char, Traits, Alloc1>,
			boost::container::basic_string<Char, Traits, Alloc2>
		>,
		bool
	>::type
	operator<(const boost::container::basic_string<Char, Traits, Alloc1>& l,
				const boost::container::basic_string<Char, Traits, Alloc2>& r)
{
	typedef boost::container::basic_string<Char, Traits, Alloc1> now_string_type;

	return l < reinterpret_cast<const now_string_type&>(r);
}

template<typename Char, typename Traits, typename Alloc1, typename Alloc2> inline
typename
	boost::enable_if
	<
		::yggr::charset::is_isomeric_same_string
		<
			boost::container::basic_string<Char, Traits, Alloc1>,
			boost::container::basic_string<Char, Traits, Alloc2>
		>,
		bool
	>::type
	operator<=(const boost::container::basic_string<Char, Traits, Alloc1>& l,
				const boost::container::basic_string<Char, Traits, Alloc2>& r)
{
	typedef boost::container::basic_string<Char, Traits, Alloc1> now_string_type;

	return l <= reinterpret_cast<const now_string_type&>(r);
}

template<typename Char, typename Traits, typename Alloc1, typename Alloc2> inline
typename
	boost::enable_if
	<
		::yggr::charset::is_isomeric_same_string
		<
			boost::container::basic_string<Char, Traits, Alloc1>,
			boost::container::basic_string<Char, Traits, Alloc2>
		>,
		bool
	>::type
	operator>(const boost::container::basic_string<Char, Traits, Alloc1>& l,
				const boost::container::basic_string<Char, Traits, Alloc2>& r)
{
	typedef boost::container::basic_string<Char, Traits, Alloc1> now_string_type;

	return l > reinterpret_cast<const now_string_type&>(r);
}

template<typename Char, typename Traits, typename Alloc1, typename Alloc2> inline
typename
	boost::enable_if
	<
		::yggr::charset::is_isomeric_same_string
		<
			boost::container::basic_string<Char, Traits, Alloc1>,
			boost::container::basic_string<Char, Traits, Alloc2>
		>,
		bool
	>::type
	operator>=(const boost::container::basic_string<Char, Traits, Alloc1>& l,
				const boost::container::basic_string<Char, Traits, Alloc2>& r)
{
	typedef boost::container::basic_string<Char, Traits, Alloc1> now_string_type;

	return l >= reinterpret_cast<const now_string_type&>(r);
}

} // namespace container
} // namespace boost

namespace boost
{
namespace container
{
namespace detail
{
namespace swap_support
{
	
template<typename Char, typename Traits, typename Alloc1, typename Alloc2> inline
typename
	boost::enable_if
	<
		::yggr::charset::is_isomeric_same_string
		<
			boost::container::basic_string<Char, Traits, Alloc1>,
			boost::container::basic_string<Char, Traits, Alloc2>
		>,
		void
	>::type
	swap(boost::container::basic_string<Char, Traits, Alloc1>& l,
			boost::container::basic_string<Char, Traits, Alloc2>& r)
{
	typedef boost::container::basic_string<Char, Traits, Alloc1> now_string1_type;
	typedef boost::container::basic_string<Char, Traits, Alloc2> now_string2_type;

	now_string1_type str_l(r.begin(), r.end(), l.get_allocator());
	now_string2_type str_r(l.begin(), l.end(), r.get_allocator());

	l.swap(str_l);
	r.swap(str_r);
}


} // namespace swap_support
} // namespace detail

using detail::swap_support::swap;

} // namespace container
} // namespace boost

namespace std
{
	using ::boost::container::detail::swap_support::swap;
}// namespace std

namespace boost
{
	using container::detail::swap_support::swap;
} // namespace boost

#endif // !(BOOST_VERSION < 107000)

namespace yggr
{
namespace charset
{

using std::getline;

// declear
template<typename CharT, typename Traits, typename Allocator>
std::basic_istream<CharT, Traits>&
	getline(BOOST_RV_REF_BEG std::basic_istream<CharT, Traits> BOOST_RV_REF_END is,
				boost::container::basic_string<CharT, Traits, Allocator>& s, CharT delim);

template<typename CharT, typename Traits, typename Allocator>
std::basic_istream<CharT, Traits>&
	getline(std::basic_istream<CharT, Traits>& is,
				boost::container::basic_string<CharT, Traits, Allocator>& s, CharT delim);

template<typename CharT, typename Traits, typename Allocator>
std::basic_istream<CharT, Traits>&
	getline(BOOST_RV_REF_BEG std::basic_istream<CharT, Traits> BOOST_RV_REF_END is,
				boost::container::basic_string<CharT,Traits,Allocator>& s);

template<typename CharT, typename Traits, typename Allocator>
std::basic_istream<CharT, Traits>&
	getline(std::basic_istream<CharT, Traits>& is,
				boost::container::basic_string<CharT,Traits,Allocator>& s);


// impl
template<typename CharT, typename Traits, typename Allocator> inline
std::basic_istream<CharT, Traits>&
	getline(BOOST_RV_REF_BEG std::basic_istream<CharT, Traits> BOOST_RV_REF_END is,
				boost::container::basic_string<CharT, Traits, Allocator>& s, CharT delim)
{
	typedef std::basic_istream<CharT, Traits> stream_type;
	return ::yggr::charset::getline(static_cast<stream_type&>(is), s, delim);
}

template<typename CharT, typename Traits, typename Allocator>
std::basic_istream<CharT, Traits>&
	getline(std::basic_istream<CharT, Traits>& is,
				boost::container::basic_string<CharT, Traits, Allocator>& s, CharT delim)
{
	typename boost::container::basic_string<CharT, Traits, Allocator>::size_type nread = 0;
	typename std::basic_istream<CharT, Traits>::sentry sentry(is, true);
	if(sentry)
	{
		std::basic_streambuf<CharT, Traits>* buf = is.rdbuf();
		s.clear();

		while(nread < s.max_size())
		{
			int c1 = buf->sbumpc();
			if(Traits::eq_int_type(c1, Traits::eof()))
			{
				is.setstate(std::ios_base::eofbit);
				break;
			}
			else
			{
				++nread;
				CharT c = Traits::to_char_type(c1);
				if(!Traits::eq(c, delim))
				{
					s.push_back(c);
				}
				else
				{
					break;              // Character is extracted but not appended.
				}
			}
		}
	}

	if(nread == 0 || nread >= s.max_size())
	{
		is.setstate(std::ios_base::failbit);
	}

	return is;
}

template<typename CharT, typename Traits, typename Allocator> inline
std::basic_istream<CharT, Traits>&
	getline(BOOST_RV_REF_BEG std::basic_istream<CharT, Traits> BOOST_RV_REF_END is,
				boost::container::basic_string<CharT,Traits,Allocator>& s)
{
	typedef std::basic_istream<CharT, Traits> stream_type;
	return ::yggr::charset::getline(static_cast<stream_type&>(is), s, is.widen('\n'));
}

template<typename CharT, typename Traits, typename Allocator> inline
std::basic_istream<CharT, Traits>&
	getline(std::basic_istream<CharT, Traits>& is,
				boost::container::basic_string<CharT,Traits,Allocator>& s)
{
	return ::yggr::charset::getline(is, s, is.widen('\n'));
}

} // namespace charset
} // namespace yggr


namespace yggr
{
namespace charset
{

namespace detail
{

template<typename Now_String, typename Tar_String>
struct string_t_getter
{
private:
	typedef Now_String cvr_now_string_type;
	typedef typename boost::remove_cv<
						typename boost::remove_reference<
							cvr_now_string_type>::type>::type now_string_type;

	typedef Tar_String cvr_tar_string_type;
	typedef typename boost::remove_cv<
						typename boost::remove_reference<
							cvr_tar_string_type>::type>::type tar_string_type;
public:
	typedef typename boost::mpl::if_<
						typename boost::is_same<now_string_type, tar_string_type>::type,
						const now_string_type&,
						tar_string_type
					>::type ctype;

	typedef typename boost::mpl::if_<
						typename boost::is_same<now_string_type, tar_string_type>::type,
						now_string_type&,
						tar_string_type
					>::type nctype;
};

template<typename Now_Char, typename Tar_Char>
struct string_t_getter<Now_Char*, Tar_Char*>
{
	YGGR_ASSERT_CHAR_TYPE(Now_Char);
	YGGR_ASSERT_CHAR_TYPE(Tar_Char);
	typedef charset::basic_string<Tar_Char> ctype;
	typedef charset::basic_string<Tar_Char> nctype;
};

} // namespace detail

} // namespace charset

#define YGGR_TYPEDEF_STRING_TYPE(__now_string_t__, __tar_string_t__, __tar_string_t_name___) \
	typedef typename yggr::charset::detail \
						::string_t_getter< __now_string_t__, __tar_string_t__ > \
						::nctype __tar_string_t_name___

#define YGGR_TYPEDEF_CONST_STRING_TYPE(__now_string_t__, __tar_string_t__, __tar_string_t_name___) \
	typedef typename yggr::charset::detail \
						::string_t_getter< __now_string_t__, __tar_string_t__ > \
						::ctype __tar_string_t_name___

//using charset::string;
//using charset::wstring;

} // namesapce yggr

namespace yggr
{
namespace charset
{

// shrink_to_fit
template<typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		container::detail::has_any_shrink_to_fit
		< 
			std::basic_string<Char, Traits, Alloc> 
		>,
		void
	>::type
	shrink_to_fit(std::basic_string<Char, Traits, Alloc>& c)
{
	c.shrink_to_fit();
}

template<typename Char, typename Traits, typename Alloc> inline
typename 
	boost::disable_if
	<
		container::detail::has_any_shrink_to_fit
		< 
			std::basic_string<Char, Traits, Alloc> 
		>,
		void
	>::type
	shrink_to_fit(std::basic_string<Char, Traits, Alloc>& c)
{
	std::basic_string<Char, Traits, Alloc> tmp(c.begin(), c.end());
	c.swap(tmp);
}

template<typename Char, typename Traits, typename Alloc> inline
typename 
	boost::enable_if
	<
		container::detail::has_any_shrink_to_fit
		< 
			boost::container::basic_string<Char, Traits, Alloc> 
		>,
		void
	>::type
	shrink_to_fit(boost::container::basic_string<Char, Traits, Alloc>& c)
{
	c.shrink_to_fit();
}

template<typename Char, typename Traits, typename Alloc> inline
typename 
	boost::disable_if
	<
		container::detail::has_any_shrink_to_fit
		< 
			boost::container::basic_string<Char, Traits, Alloc> 
		>,
		void
	>::type
	shrink_to_fit(boost::container::basic_string<Char, Traits, Alloc>& c)
{
	boost::container::basic_string<Char, Traits, Alloc> tmp(c.begin(), c.end());
	c.swap(tmp);
}

} // namespace charset
} // namespace yggr

// basic_string getter
namespace yggr
{
namespace charset
{

// the struct can only use the custom template special

template<typename T>
struct basic_string_getter;

template<typename Char, typename Traits, typename Alloc>
struct basic_string_getter< std::basic_string<Char, Traits, Alloc> >
{
public:
	typedef std::basic_string<Char, Traits, Alloc> type;

public:
	inline type& operator()(type& src) const
	{
		return src;
	}

	inline const type& operator()(const type& src) const
	{
		return src;
	}
};

template<typename Char, typename Traits, typename Alloc>
struct basic_string_getter< boost::container::basic_string<Char, Traits, Alloc> >
{
public:
	typedef boost::container::basic_string<Char, Traits, Alloc> type;

public:
	inline type& operator()(type& src) const
	{
		return src;
	}

	inline const type& operator()(const type& src) const
	{
		return src;
	}
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
struct basic_string_getter< Basic_String<Char, Traits, Alloc> >
{
private:
	typedef Basic_String<Char, Traits, Alloc> now_string_type;
	typedef std::basic_string<Char, Traits, Alloc> stl_string_type;
	typedef boost::container::basic_string<Char, Traits, Alloc> boost_string_type;

public:
	typedef typename
		boost::mpl::if_
		<
			boost::is_base_of<stl_string_type, now_string_type>,
			stl_string_type,
			boost_string_type
		>::type type;

public:
	inline type& operator()(now_string_type& src) const
	{
		return static_cast<type&>(src);
	}

	inline const type& operator()(const now_string_type& src) const
	{
		return static_cast<const type&>(src);
	}
};


template<typename T> inline
typename basic_string_getter<T>::type&
	get_basic_string(T& src)
{
	typedef basic_string_getter<T> getter_type;
	getter_type getter;
	return getter(src);
}

template<typename T> inline
const typename basic_string_getter<T>::type&
	get_basic_string(const T& src)
{
	typedef basic_string_getter<T> getter_type;
	getter_type getter;
	return getter(src);
}

} // namespace charset
} // namespace yggr

namespace yggr
{
namespace charset
{

// is_validate_char
template<typename Char> inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		bool
	>::type
	is_validate_char(Char)
{
	return is_native_char<Char>::value;
}

// is_validate_string
template<typename Char> inline
typename
	boost::enable_if
	<
		is_native_char<Char>,
		bool
	>::type
	is_validate_string(const Char*)
{
	return is_native_char<Char>::value;
}

template<typename Char, typename Traits, typename Alloc > inline
bool is_validate_string(const std::basic_string<Char, Traits, Alloc>& str)
{
	return charset_base_foo::strlen(str.data()) == str.size();
}

template<typename Char, typename Traits, typename Alloc > inline
bool is_validate_string(const boost::container::basic_string<Char, Traits, Alloc>& str)
{
	return charset_base_foo::strlen(str.data()) == str.size();
}

} // namespace charset
} // namespace yggr

// string traits
namespace yggr
{
namespace charset
{

typedef boost::mpl::true_ tag_inner_string_traits;
typedef boost::mpl::false_ tag_not_inner_string_traits;

namespace detail
{

template<typename T>
struct is_inner_string_traits
	: public boost::mpl::false_
{
};

template<typename T>
struct is_inner_string_traits<T*>
	: public
		is_convertible_char
		<
			typename boost::remove_cv<T>::type
		>::type
{
};

template<typename T, std::size_t N>
struct is_inner_string_traits<T[N]>
	: public
		is_convertible_char
		<
			typename boost::remove_cv<T>::type
		>::type
{
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
struct is_inner_string_traits< Basic_String<Char, Traits, Alloc> >
	: public is_basic_string_t< Basic_String<Char, Traits, Alloc> >::type
{
};

template<typename T, typename IsInnerDecl>
struct basic_string_traits;

template<typename T>
struct basic_string_traits<T*, tag_inner_string_traits>
{
	typedef T char_type;
	typedef typename
		mplex::typename_expand_get
		<
			basic_string<char_type>, 
			2
		>::type tpl_arg_allocator_type;

	typedef char_type* string_type;
	typedef char_type value_type;
	//typedef std::char_traits<char_type> traits_type;
	//typedef std::allocator<char_type> allocator_type;
	typedef typename basic_string<char_type>::traits_type traits_type;
	typedef typename basic_string<char_type>::allocator_type allocator_type;
	typedef char_type& reference;
	typedef const char_type& const_reference;
	typedef char_type* pointer;
	typedef const char_type* const_pointer;
	typedef pointer iterator;
	typedef const_pointer const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef std::ptrdiff_t difference_type;
	typedef std::size_t size_type;

	typedef charset::basic_string<char_type> base_class_string_type;
	typedef base_class_string_type class_string_type;

	typedef tag_inner_string_traits is_inner_decl_type;
};

template<typename T, std::size_t N>
struct basic_string_traits<T[N], tag_inner_string_traits>
	: public basic_string_traits<T*, tag_inner_string_traits>
{
};

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String>
struct basic_string_traits<Basic_String<Char, Traits, Alloc>, tag_inner_string_traits>
{
	typedef Char char_type;
	typedef Alloc tpl_arg_allocator_type;
	typedef Basic_String<Char, Traits, Alloc> string_type;
	typedef typename string_type::value_type value_type;
	typedef typename string_type::traits_type traits_type;
	typedef typename string_type::allocator_type allocator_type;
	typedef typename string_type::reference reference;
	typedef typename string_type::const_reference const_reference;
	typedef typename string_type::pointer pointer;
	typedef typename string_type::const_pointer const_pointer;
	typedef typename string_type::iterator iterator;
	typedef typename string_type::const_iterator const_iterator;
	typedef typename string_type::reverse_iterator reverse_iterator;
	typedef typename string_type::const_reverse_iterator const_reverse_iterator;
	typedef typename string_type::difference_type difference_type;
	typedef typename string_type::size_type size_type;

	typedef string_type base_class_string_type;
	typedef string_type class_string_type;

	typedef tag_inner_string_traits is_inner_decl_type;
};

} // namespace detail

template<typename T>
struct string_traits
	: public
		detail::basic_string_traits
		<
			typename boost::remove_cv<T>::type,
			detail::is_inner_string_traits< typename boost::remove_cv<T>::type >
		>
{
};

} // namespace charset
} // namespace yggr

// adapter function
namespace yggr
{
namespace charset
{

// org_str
template<typename Char> inline
typename
	boost::enable_if
	<
		is_convertible_char<typename boost::remove_cv<Char>::type>,
		Char*
	>::type
	org_str(Char* src)
{
	return src;
}

template<typename Char, std::size_t N> inline
typename
	boost::enable_if
	<
		is_convertible_char<typename boost::remove_cv<Char>::type>,
		Char(&)[N]
	>::type
	org_str(Char(&src)[N])
{
	return src;
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
		Basic_String<Char, Traits, Alloc>&
	>::type
	org_str(Basic_String<Char, Traits, Alloc>& src)
{
	return src;
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
		const Basic_String<Char, Traits, Alloc>&
	>::type
	org_str(const Basic_String<Char, Traits, Alloc>& src)
{
	return src;
}

// recount_length

template<typename Char> inline
typename
	boost::enable_if
	<
		is_convertible_char<typename boost::remove_cv<Char>::type>,
		size_type
	>::type
	recount_length(Char* src)
{
	return charset_base_foo::strlen(src);
}

template<typename Char, std::size_t N> inline
typename
	boost::enable_if
	<
		is_convertible_char<typename boost::remove_cv<Char>::type>,
		size_type
	>::type
	org_str(Char(&src)[N])
{
    typedef Char value_type;
	return src[N - 1]?
			static_cast<std::size_t>(
				std::distance(
					boost::begin(src),
					std::find(boost::begin(src), boost::end(src), value_type()) ) )
			: charset::charset_base_foo::strlen(boost::begin(src));
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
		typename Basic_String<Char, Traits, Alloc>::size_type
	>::type
	recount_length(Basic_String<Char, Traits, Alloc>& src)
{
	return src.length();
}

// affix_length

template<typename Char> inline
typename
	boost::enable_if
	<
		is_convertible_char<typename boost::remove_cv<Char>::type>,
		void
	>::type
	affix_length(Char* src, size_type n)
{
	assert(n == charset_base_foo::strlen(src));
}

template<typename Char, std::size_t N> inline
typename
	boost::enable_if
	<
		is_convertible_char<typename boost::remove_cv<Char>::type>,
		Char(&)[N]
	>::type
	affix_length(Char(&src)[N], size_type n)
{
    typedef Char value_type;
	assert(( n ==
		(src[N - 1]?
			static_cast<std::size_t>(
				std::distance(
					boost::begin(src),
					std::find(boost::begin(src), boost::end(src), value_type()) ) )
			: charset::charset_base_foo::strlen(boost::begin(src)) ) ));
}

template<typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
typename
	boost::enable_if
	<
		is_basic_string_t< Basic_String<Char, Traits, Alloc> >,
		void
	>::type
	affix_length(Basic_String<Char, Traits, Alloc>& src,
					typename Basic_String<Char, Traits, Alloc>::size_type n)
{
	assert(n == src.length());
}

} // namespace charset
} // namespace yggr

#endif // __YGGR_CHARSET_STRING_HPP__
