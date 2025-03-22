// file_system_c_str.hpp

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

#ifndef __YGGR_FILE_SYSTEM_FILE_SYSTEM_C_STR_HPP__
#define __YGGR_FILE_SYSTEM_FILE_SYSTEM_C_STR_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/version.hpp>
#include <boost/filesystem/fstream.hpp>

#if (BOOST_VERSION < 107700)

#	define YGGR_FILESYSTEM_C_STR( __var_path__ ) __var_path__.BOOST_FILESYSTEM_C_STR

#else

#	define YGGR_FILESYSTEM_C_STR( __var_path__ ) BOOST_FILESYSTEM_C_STR( __var_path__ )

#endif // (BOOST_VERSION < 107700)


namespace yggr
{
namespace file_system
{
} // namespace file_system
} // namespace yggr

#endif // __YGGR_FILE_SYSTEM_FILE_SYSTEM_C_STR_HPP__
