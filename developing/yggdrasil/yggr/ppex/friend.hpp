//friend.hpp

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


#ifndef __YGGR_PPEX_FRIEND_HPP__
#define __YGGR_PPEX_FRIEND_HPP__

#ifndef _MSC_VER
#	include <yggr/type_traits/traits.hpp>
#endif // _MSC_VER

#define YGGR_PP_FRIEND_ClASS( __type__ ) friend class __type__

#ifdef _MSC_VER
//#	define YGGR_PP_FRIEND_TYPEDEF( __type__ ) friend __type__
#	define YGGR_PP_FRIEND_TYPENAME( __type__ ) friend typename __type__
#	define YGGR_PP_FRIEND_DEDUCED_TYPENAME( __type__ ) friend typename __type__
#elif defined(__APPLE__) || defined (__APPLE_CC__)
//#	define YGGR_PP_FRIEND_TYPEDEF( __type__ ) friend class yggr::traits< __type__ >::type
#	define YGGR_PP_FRIEND_TYPENAME( __type__ ) friend typename yggr::traits< __type__ >::type
#	define YGGR_PP_FRIEND_DEDUCED_TYPENAME( __type__ ) friend typename yggr::traits< typename __type__ >::type
#else
//#	define YGGR_PP_FRIEND_TYPEDEF( __type__ ) friend class yggr::traits< __type__ >::type
#	define YGGR_PP_FRIEND_TYPENAME( __type__ ) friend class yggr::traits< __type__ >::type
#	define YGGR_PP_FRIEND_DEDUCED_TYPENAME( __type__ ) friend class yggr::traits< typename __type__ >::type
#endif // _MSC_VER

#define YGGR_PP_FRIEND_CLASS( __type__ ) friend class __type__


namespace yggr
{
namespace ppex
{
} // namespace ppex
} // namespace yggr

#endif // __YGGR_PPEX_FRIEND_HPP__