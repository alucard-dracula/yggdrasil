// config.hpp

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

#ifndef __YGGR_MOVE_CONFIG_HPP__
#define __YGGR_MOVE_CONFIG_HPP__

#if !defined(YGGR_USE_CXX11)
#   if defined(_MSC_VER)
#	    if (_MSC_VER < 1600)
#		    define BOOST_NO_CXX11_RVALUE_REFERENCES
#		    define YGGR_NO_CXX11_RVALUE_REFERENCES
#	    endif // _MSC_VER < 1600
#   elif defined(__GNUC__)
#	    if !defined(__GXX_EXPERIMENTAL_CXX0X__) || (__GXX_EXPERIMENTAL_CXX0X__ == 0)
#	    	define BOOST_NO_CXX11_RVALUE_REFERENCES
#	    	define YGGR_NO_CXX11_RVALUE_REFERENCES
#	    endif // YGGR_USE_CXX11
#   else
#       error "please append check code !!!"
#   endif // _MSC_VER
#endif // YGGR_USE_CXX11


#endif // __YGGR_MOVE_CONFIG_HPP__
