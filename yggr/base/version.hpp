//version.hpp

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

#ifndef __YGGR_VERSION_HPP__
#define __YGGR_VERSION_HPP__

#ifndef __YGGR_YGGR_DEF_H__
#	error "please include <yggr/base/yggrdef.h> !!!";
#endif // __YGGR_YGGR_CONFIG_H__

#include <boost/version.hpp>

#if (BOOST_VERSION < 105300)
#	error "yggdrasil not support boost 1.52 and older"
#endif // (BOOST_VERSION < 105300)

#define __version_to_string( __x__ ) #__x__
#define __version_expand( __x__ ) __x__

#define YGGR_MAJOR_VERSION 0
#define YGGR_MINOR_VERSION 8
#define YGGR_MICRO_VERSION 0

#define YGGR_MAKE_VERSION_HEX( __major__, __minor__, __micro__ ) \
	( (__major__ << 24) | (__minor__ << 16) | (__micro__ << 8) )

#define YGGR_MAKE_VERSION_DOT_DEC( __major__, __minor__, __micro__ ) \
	__major__.__minor__.__micro__

#define YGGR_MAKE_VERSION_DOT_DEC_STRING( __major__, __minor__, __micro__ ) \
	__version_to_string(__major__.__minor__.__micro__)

#define YGGR_VERSION YGGR_MAKE_VERSION_DOT_DEC(YGGR_MAJOR_VERSION, YGGR_MINOR_VERSION, YGGR_MICRO_VERSION)
#define YGGR_VERSION_S YGGR_MAKE_VERSION_DOT_DEC_STRING(YGGR_MAJOR_VERSION, YGGR_MINOR_VERSION, YGGR_MICRO_VERSION)
#define YGGR_VERSION_HEX YGGR_MAKE_VERSION_HEX(YGGR_MAJOR_VERSION, YGGR_MINOR_VERSION, YGGR_MICRO_VERSION)


#endif // __YGGR_VERSION_HPP__
