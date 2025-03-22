// endian.hpp

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

#ifndef __YGGR_BASE_ENDIAN_HPP__
#define __YGGR_BASE_ENDIAN_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

#if (BOOST_VERSION < 106900)

#	include <boost/detail/endian.hpp>

#	if defined(BOOST_BIG_ENDIAN)
#		if !defined(YGGR_BIG_ENDIAN)
#			define YGGR_BIG_ENDIAN BOOST_BIG_ENDIAN
#		endif // YGGR_BIG_ENDIAN
#	endif // BOOST_BIG_ENDIAN

#	if defined(BOOST_LITTLE_ENDIAN)
#		if !defined(YGGR_LITTLE_ENDIAN)
#			define YGGR_LITTLE_ENDIAN BOOST_LITTLE_ENDIAN
#		endif // YGGR_LITTLE_ENDIAN
#	endif // BOOST_LITTLE_ENDIAN

#	if defined(BOOST_PDP_ENDIAN)
#		if !defined(YGGR_PDP_ENDIAN)
#			define YGGR_PDP_ENDIAN BOOST_PDP_ENDIAN
#		endif // YGGR_PDP_ENDIAN
#	endif // BOOST_PDP_ENDIAN

#	if !defined(YGGR_BYTE_ORDER)
#		if defined(YGGR_PDP_ENDIAN)
#			if !defined(BOOST_BYTE_ORDER)
#				define YGGR_BYTE_ORDER 2143
#			else
#				define YGGR_BYTE_ORDER BOOST_BYTE_ORDER
#			endif // !defined(BOOST_BYTE_ORDER)
#		else
#			define YGGR_BYTE_ORDER BOOST_BYTE_ORDER
#		endif // YGGR_PDP_ENDIAN
#	endif // YGGR_LITTLE_ENDIAN

#else

#	include <boost/predef/other/endian.h>

#	if BOOST_ENDIAN_BIG_BYTE
#		define YGGR_BIG_ENDIAN
#		define YGGR_BYTE_ORDER 4321
#	endif // BOOST_ENDIAN_BIG_BYTE

#	if BOOST_ENDIAN_LITTLE_BYTE
#		define YGGR_LITTLE_ENDIAN
#		define YGGR_BYTE_ORDER 1234
#	endif // BOOST_ENDIAN_LITTLE_BYTE

#	if BOOST_ENDIAN_LITTLE_WORD
#		define YGGR_PDP_ENDIAN
#		define YGGR_BYTE_ORDER 2134
#	endif // BOOST_ENDIAN_LITTLE_WORD

#endif // (BOOST_VERSION < 107200)

#endif // __YGGR_BASE_ENDIAN_HPP__
