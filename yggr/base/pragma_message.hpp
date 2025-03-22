// pragma_message.hpp

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

#ifndef __YGGR_BASE_PRAGMA_MESSAGE_HPP__
#define __YGGR_BASE_PRAGMA_MESSAGE_HPP__

#include <boost/version.hpp>

#if defined(BOOST_DISABLE_PRAGMA_MESSAGE)
#	define YGGR_DISABLE_PRAGMA_MESSAGE
#endif // BOOST_DISABLE_PRAGMA_MESSAGE

#if defined(YGGR_DISABLE_PRAGMA_MESSAGE)
#	define BOOST_DISABLE_PRAGMA_MESSAGE
#endif // YGGR_DISABLE_PRAGMA_MESSAGE

#if (BOOST_VERSION < 106700)

#	define YGGT_TMP_DO_STRINGIZE(X) #X
#	define YGGR_TMP_STRINGIZE(X) YGGT_TMP_DO_STRINGIZE(X)


#	if defined(YGGR_DISABLE_PRAGMA_MESSAGE)
#		define YGGR_PRAGMA_MESSAGE(x)
#	elif defined(__INTEL_COMPILER)
#		define YGGR_PRAGMA_MESSAGE(x) __pragma(message(__FILE__ "(" YGGR_TMP_STRINGIZE(__LINE__) "): note: " x))
#	elif defined(__GNUC__)
#		define YGGR_PRAGMA_MESSAGE(x) _Pragma(YGGR_TMP_STRINGIZE(message(x)))
#	elif defined(_MSC_VER)
#		define YGGR_PRAGMA_MESSAGE(x) __pragma(message(__FILE__ "(" YGGR_TMP_STRINGIZE(__LINE__) "): note: " x))
#	else
#		define YGGR_PRAGMA_MESSAGE(x)
#endif // BOOST_DISABLE_PRAGMA_MESSAGE

#else

#	include <boost/config/pragma_message.hpp>

#	define YGGR_PRAGMA_MESSAGE BOOST_PRAGMA_MESSAGE

#endif // (BOOST_VERSION < 106700)

#endif // __YGGR_BASE_PRAGMA_MESSAGE_HPP__
