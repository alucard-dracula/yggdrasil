//archive_data_wrap_traits.hpp

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

#ifndef __YGGR_ARCHIVE_ARCHIVE_DATA_WRAP_TRAITS_HPP__
#define __YGGR_ARCHIVE_ARCHIVE_DATA_WRAP_TRAITS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>

#include <boost/mpl/bool.hpp>

namespace yggr
{
namespace archive
{

// how different ARCHIVE storage scheme // see class id_n
template<typename Archive, typename T>
struct binary_data_wrap_traits
	: public boost::mpl::false_
{
	typedef T wrap_type;
};

} // namespace archve
} // namespace 

#define YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_DEF( __archive__, __val__, __wrap__ ) \
	namespace yggr { namespace archive { \
	template<> struct binary_data_wrap_traits< __archive__, __val__ > \
		: public boost::mpl::true_ { typedef __wrap__ type; }; }}

#define YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS( __archive__, __val__ ) \
	yggr::archive::binary_data_wrap_traits< __archive__, __val__ >

#define YGGR_PP_ARCHIVE_BINARY_DATA_WRAP_TRAITS_WRAP( __archive__, __val__ ) \
	yggr::archive::binary_data_wrap_traits< __archive__, __val__ >::wrap_type

#define YGGR_PP_ARCHIVE_NEED_BINARY_DATA_WRAP_TRAITS_T( __archive__, __val__ ) \
	yggr::archive::binary_data_wrap_traits< __archive__, __val__ >::type

#define YGGR_PP_ARCHIVE_NEED_BINARY_DATA_WRAP_TRAITS_V( __archive__, __val__ ) \
	yggr::archive::binary_data_wrap_traits< __archive__, __val__ >::value

#endif //__YGGR_ARCHIVE_ARCHIVE_DATA_WRAP_TRAITS_HPP__