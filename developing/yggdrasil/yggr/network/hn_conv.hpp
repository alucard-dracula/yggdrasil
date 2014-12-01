//hn_conv.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#include <boost/asio.hpp>
#include <yggr/base/yggrdef.h>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>

#define HN_CONV_8( __type__ ) \
	inline __type__ hton( __type__ n) { return n; } \
	inline __type__ ntoh( __type__ n) { return n; }

#define HN_CONV_16( __type__ ) \
	inline __type__ hton( __type__ n) { return (htons(n)); } \
	inline __type__ ntoh( __type__ n) { return (ntohs(n)); }


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
	u32* p = reinterpret_cast<u32*>(&n); \
	p[0] = (htonl(p[0])); \
	p[1] = (htonl(p[1])); \
	std::swap(p[0], p[1]); \
	return n; } \
	\
	inline __type__ ntoh( __type__ n ) { \
		u32* p = reinterpret_cast<u32*>(&n); \
		std::swap(p[0], p[1]); \
		p[0] = (ntohl(p[0])); \
		p[1] = (ntohl(p[1])); \
		return n; }

namespace yggr
{
namespace network
{

// 1byte see template foo

HN_CONV_8(char)
HN_CONV_8(s8)
HN_CONV_8(u8)
HN_CONV_16(wchar_t)
HN_CONV_16(s16)
HN_CONV_16(u16)
HN_CONV_32(s32)
HN_CONV_32(u32)
    
#if defined(__APPLE__) || defined(__APPLE_CC__)
    inline u32 hton( const long n )
    {
		return htonl(n);
    }
#endif // __APPLE__
    
#ifdef __ANDROID__
	HN_CONV_32(long int)
	HN_CONV_32(unsigned long int)
#endif // __ANDROID__
 
HN_CONV_64(s64)
HN_CONV_64(u64)

HN_CONV_32(f32)
HN_CONV_64(f64)

template<typename T>
T hton(const T& t)
{
	return t.hton();
}

template<typename T>
T ntoh(const T& t)
{
	return t.ntoh();
}

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String>
const Basic_String<char, Traits, Alloc>&
	hton(const Basic_String<char, Traits, Alloc>& val)
{
	return val;
}

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String>
const Basic_String<char, Traits, Alloc>&
	ntoh(const Basic_String<char, Traits, Alloc>& val)
{
	return val;
}

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String>
const Basic_String<wchar_t, Traits, Alloc>&
	hton(const Basic_String<wchar_t, Traits, Alloc>& val)
{
	typedef Basic_String<wchar_t, Traits, Alloc> string_type;
	typedef typename string_type::iterator iter_type;
	string_type tmp(val);
	for(iter_type i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
	{
		*i = hton<u16>(*i);
	}
	return tmp;
}

template<typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
                class Basic_String>
const Basic_String<wchar_t, Traits, Alloc>&
	ntoh(const Basic_String<wchar_t, Traits, Alloc>& val)
{
	typedef Basic_String<wchar_t, Traits, Alloc> string_type;
	typedef typename string_type::iterator iter_type;
	string_type tmp(val);
	for(iter_type i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
	{
		*i = ntoh<u16>(*i);
	}
	return tmp;
}

//f32 hton(f32 val)
//{
//	return val;
//}
//
//f32 ntoh(f32 val)
//{
//	return val;
//}
//
//f64 hton(f64 val)
//{
//	return val;
//}
//
//f64 ntoh(f64 val)
//{
//	return val;
//}

} // namespace network
} // namespace yggr

#undef HN_CONV_8
#undef HN_CONV_16
#undef HN_CONV_32
#undef HN_CONV_64

#endif // __YGGR_NETWORK_HN_CONV_HPP__
