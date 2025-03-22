//hn_conv.hpp

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

#ifndef __YGGR_NETWORK_HN_CONV_HPP__
#define __YGGR_NETWORK_HN_CONV_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/utility/swap.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/has_member_check.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/asio.hpp>
#include <boost/mpl/bool.hpp>

#define HN_CONV_8( __type__ ) \
	inline __type__ hton( __type__ n) { return n; } \
	inline __type__ ntoh( __type__ n) { return n; }

#define HN_CONV_NATIVE HN_CONV_8

#define HN_CONV_16_ORG( __type__ ) \
	inline __type__ hton( __type__ n) { return htons(n); } \
	inline __type__ ntoh( __type__ n) { return ntohs(n); }

#define HN_CONV_16(__type__) \
	inline __type__ hton( __type__ n ) { \
		u16* p = reinterpret_cast<u16*>(&n); \
		p[0] = (htons(p[0])); \
		return n; } \
		\
	inline __type__ ntoh( __type__ n ) { \
		u16* p = reinterpret_cast<u16*>(&n); \
		p[0] = (ntohs(p[0])); \
		return n; }

#define HN_CONV_32_ORG( __type__ ) \
	inline __type__ hton( __type__ n) { return htonl(n); } \
	inline __type__ ntoh( __type__ n) { return ntohl(n); }

#define HN_CONV_32(__type__) \
	inline __type__ hton( __type__ n ) { \
		u32* p = reinterpret_cast<u32*>(&n); \
		p[0] = (htonl(p[0])); \
		return n; } \
		\
	inline __type__ ntoh( __type__ n ) { \
		u32* p = reinterpret_cast<u32*>(&n); \
		p[0] = (ntohl(p[0])); \
		return n; }

#define HN_CONV_64(__type__) \
	inline __type__ hton( __type__ n ) { \
	if(is_little_endian()) { \
		u32* p = reinterpret_cast<u32*>(&n); \
		p[0] = (htonl(p[0])); \
		p[1] = (htonl(p[1])); \
		yggr::swap(p[0], p[1]); } \
	return n; } \
	\
	inline __type__ ntoh( __type__ n ) { \
		if(is_little_endian()) { \
			u32* p = reinterpret_cast<u32*>(&n); \
			yggr::swap(p[0], p[1]); \
			p[0] = (ntohl(p[0])); \
			p[1] = (ntohl(p[1])); } \
		return n; }

namespace yggr
{
namespace network
{
namespace detail
{

	YGGR_PP_HAS_MEMBER_FOO_DEF(hton, T, T, hton, 1, void)
	YGGR_PP_HAS_MEMBER_FOO_DEF(ntoh, T, T, ntoh, 1, void)

} // namesapce detail
} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{

YGGR_CONSTEXPR_OR_STATIC_CONST union { u32 mylong; char c[4]; } endian_test = { 0x623f3f6c };

#if !defined(YGGR_NO_CONSTEXPR)

inline static bool is_little_endian(void) YGGR_NOEXCEPT_OR_NOTHROW
{ 
	static const bool ret = (endian_test.c[0] == 'l');
	return ret;
}

inline static bool is_big_endian(void) YGGR_NOEXCEPT_OR_NOTHROW
{
	static const bool ret = (endian_test.c[0] == 'b');
	return ret;
}
#else

YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC bool is_little_endian(void) 
{ 
	return (endian_test.c[0] == 'l');
}

YGGR_STATIC_CONSTEXPR_OR_INLINE_STATIC bool is_big_endian(void) 
{
	return (endian_test.c[0] == 'b');
}

#endif // #if (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

// 1byte see template foo

namespace detail
{
template<typename T, std::size_t size = sizeof(T)>
struct wchar_t_hn_conv_helper;

template<typename T>
struct wchar_t_hn_conv_helper<T, 2>
{
	inline static T hton(T src)
	{
		return htons(*(reinterpret_cast<u16*>(&src)));
	}

	inline static T ntoh(T src)
	{
		return ntohs(*(reinterpret_cast<u16*>(&src)));
	}
};

template<typename T>
struct wchar_t_hn_conv_helper<T, 4>
{
	inline static T hton(T src)
	{
		return htonl(*(reinterpret_cast<u32*>(&src)));
	}

	inline static T ntoh(T src)
	{
		return ntohl(*(reinterpret_cast<u32*>(&src)));
	}
};

} // namespace detail

HN_CONV_8(char)
HN_CONV_8(s8)
HN_CONV_8(u8)

#ifndef YGGR_NO_CWCHAR

inline wchar_t hton( wchar_t n) 
{ 
	typedef detail::wchar_t_hn_conv_helper<wchar_t> h_type;
	return h_type::hton(n);
}

inline wchar_t ntoh( wchar_t n) 
{ 
	typedef detail::wchar_t_hn_conv_helper<wchar_t> h_type;
	return h_type::ntoh(n);
}

#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T

HN_CONV_8(c8)

#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T

HN_CONV_16(c16)

#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T

HN_CONV_32(c32)

#endif // YGGR_NO_CHAR32_T

HN_CONV_16(s16)
HN_CONV_16_ORG(u16)
HN_CONV_32(s32)
HN_CONV_32_ORG(u32)
    
#if defined(YGGR_AT_DARWIN)
    inline u32 hton( const long n )
    {
		return htonl(*(reinterpret_cast<const u32*>(&n)));
    }
#endif // YGGR_AT_DARWIN
    
#if ((defined(YGGR_AT_ANDROID)) && (defined(_ILP32) && (_ILP32)))
	HN_CONV_32(long int)
	HN_CONV_32_ORG(unsigned long int)
#endif // #if ((defined(YGGR_AT_ANDROID)) && (defined(_ILP32) && (_ILP32)))
 
HN_CONV_64(s64)
HN_CONV_64(u64)

#ifdef YGGR_NETWORK_FLOAT_POINT_HN_CONV_ENABLE

HN_CONV_32(f32)
HN_CONV_64(f64)

#else

HN_CONV_NATIVE(f32)
HN_CONV_NATIVE(f64)

#endif // YGGR_NETWORK_FLOAT_POINT_HN_CONV_ENABLE

template<typename T> inline
typename 
	boost::enable_if
	<
		detail::has_hton<T>,
		T
	>::type
	hton(const T& t)
{
	return t.hton();
}

template<typename T> inline
typename 
	boost::disable_if
	<
		detail::has_hton<T>,
		const T&
	>::type
	hton(const T& t)
{
	return t;
}

template<typename T> inline
typename 
	boost::enable_if
	<
		detail::has_ntoh<T>,
		T
	>::type
	ntoh(const T& t)
{
	return t.ntoh();
}

template<typename T> inline
typename 
	boost::disable_if
	<
		detail::has_ntoh<T>,
		const T&
	>::type
	ntoh(const T& t)
{
	return t;
}

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String> inline 
const Basic_String<char, Traits, Alloc>&
	hton(const Basic_String<char, Traits, Alloc>& val)
{
	return val;
}

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String> inline 
const Basic_String<char, Traits, Alloc>&
	ntoh(const Basic_String<char, Traits, Alloc>& val)
{
	return val;
}

#ifndef YGGR_NO_CWCHAR

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String>
Basic_String<wchar_t, Traits, Alloc>&
	hton(Basic_String<wchar_t, Traits, Alloc>& val)
{
	typedef detail::wchar_t_hn_conv_helper<wchar_t> h_type;
	typedef Basic_String<wchar_t, Traits, Alloc> string_type;
	typedef typename string_type::iterator iter_type;

	string_type tmp(val);
	for(iter_type i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
	{
		*i = h_type::hton(*i);
	}
	return tmp;
}

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String>
Basic_String<wchar_t, Traits, Alloc>&
	ntoh(Basic_String<wchar_t, Traits, Alloc>& val)
{
	typedef detail::wchar_t_hn_conv_helper<wchar_t> h_type;
	typedef Basic_String<wchar_t, Traits, Alloc> string_type;
	typedef typename string_type::iterator iter_type;

	string_type tmp(val);
	for(iter_type i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
	{
		*i = h_type::ntoh(*i);
	}
	return tmp;
}

#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String> inline 
const Basic_String<yggr::c8, Traits, Alloc>&
	hton(const Basic_String<yggr::c8, Traits, Alloc>& val)
{
	return val;
}

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String> inline 
const Basic_String<yggr::c8, Traits, Alloc>&
	ntoh(const Basic_String<yggr::c8, Traits, Alloc>& val)
{
	return val;
}

#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String>
Basic_String<yggr::c16, Traits, Alloc>&
	hton(Basic_String<yggr::c16, Traits, Alloc>& val)
{
	typedef detail::wchar_t_hn_conv_helper<yggr::c16> h_type;
	typedef Basic_String<yggr::c16, Traits, Alloc> string_type;
	typedef typename string_type::iterator iter_type;

	string_type tmp(val);
	for(iter_type i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
	{
		*i = h_type::hton(*i);
	}
	return tmp;
}

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String>
Basic_String<yggr::c16, Traits, Alloc>&
	ntoh(Basic_String<yggr::c16, Traits, Alloc>& val)
{
	typedef detail::wchar_t_hn_conv_helper<yggr::c16> h_type;
	typedef Basic_String<yggr::c16, Traits, Alloc> string_type;
	typedef typename string_type::iterator iter_type;

	string_type tmp(val);
	for(iter_type i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
	{
		*i = h_type::ntoh(*i);
	}
	return tmp;
}

#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String>
Basic_String<yggr::c32, Traits, Alloc>&
	hton(Basic_String<yggr::c32, Traits, Alloc>& val)
{
	typedef detail::wchar_t_hn_conv_helper<yggr::c32> h_type;
	typedef Basic_String<yggr::c32, Traits, Alloc> string_type;
	typedef typename string_type::iterator iter_type;

	string_type tmp(val);
	for(iter_type i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
	{
		*i = h_type::hton(*i);
	}
	return tmp;
}

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String>
Basic_String<yggr::c32, Traits, Alloc>&
	ntoh(Basic_String<yggr::c32, Traits, Alloc>& val)
{
	typedef detail::wchar_t_hn_conv_helper<yggr::c32> h_type;
	typedef Basic_String<yggr::c32, Traits, Alloc> string_type;
	typedef typename string_type::iterator iter_type;

	string_type tmp(val);
	for(iter_type i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
	{
		*i = h_type::ntoh(*i);
	}
	return tmp;
}

#endif // YGGR_NO_CHAR32_T

} // namespace network
} // namespace yggr

#undef HN_CONV_64
#undef HN_CONV_32
#undef HN_CONV_32_ORG
#undef HN_CONV_16
#undef HN_CONV_16_ORG
#undef HN_CONV_NATIVE
#undef HN_CONV_8

#endif // __YGGR_NETWORK_HN_CONV_HPP__
