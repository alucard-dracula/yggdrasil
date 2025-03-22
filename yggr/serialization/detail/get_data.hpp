// get_data.hpp

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
#ifndef YGGR_SERIALIZATION_DETAIL_GET_DATA_HPP
#define YGGR_SERIALIZATION_DETAIL_GET_DATA_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif // _MSC_VER

#if defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)
#	define STD _STLP_STD
#else
#	define STD std
#endif //  defined(__SGI_STL_PORT) || defined(_STLPORT_VERSION)

#include <boost/ref.hpp>

#include <vector>
#include <valarray>

namespace yggr 
{
namespace serialization 
{ 
namespace detail 
{

template <typename T, typename Allocator> inline
T* get_data(STD::vector<T, Allocator>& v)
{
	return v.size()? boost::addressof(v[0]) : 0;
}

template <typename T, typename Allocator> inline
T* get_data(STD::vector<T, Allocator> const& v)
{
	return get_data(const_cast<STD::vector<T, Allocator>&>(v));
}

template <typename T> inline
T* get_data(STD::valarray<T>& v)
{
	return v.size()? boost::addressof(v[0]) : 0;
}

template <typename T> inline
const T* get_data(STD::valarray<T> const& v)
{
	return get_data(const_cast<STD::valarray<T>&>(v));
}

} // detail
} // serialization
} // yggr

#undef STD

#endif // YGGR_SERIALIZATION_DETAIL_GET_DATA_HPP
