//iterator_adp.hpp

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

#ifndef __YGGR_REGULAR_PARSE_ITERATOR_ADP_HPP__
#define __YGGR_REGULAR_PARSE_ITERATOR_ADP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/regular_parse/detail/conv_iterator_helper.hpp>

namespace yggr
{
namespace regular_parse
{

template<typename Iter> inline
Iter unwrap_iterator(Iter i)
{
	return i;
}

template<typename BaseIter, typename T> inline
BaseIter unwrap_iterator(boost::u32_to_u16_iterator<BaseIter, T> i)
{
	return i.base();
}

template<typename BaseIter, typename T> inline
BaseIter unwrap_iterator(boost::u16_to_u32_iterator<BaseIter, T> i)
{
	return i.base();
}

template<typename BaseIter, typename T> inline
BaseIter unwrap_iterator(boost::u32_to_u8_iterator<BaseIter, T> i)
{
	return i.base();
}

template<typename BaseIter, typename T> inline
BaseIter unwrap_iterator(boost::u8_to_u32_iterator<BaseIter, T> i)
{
	return i.base();
}

template<typename BaseIter, typename T> inline
BaseIter unwrap_iterator(detail::u16_to_u8_iterator<BaseIter, T> i)
{
	return i.base();
}

template<typename BaseIter, typename T> inline
BaseIter unwrap_iterator(detail::u8_to_u16_iterator<BaseIter, T> i)
{
	return i.base();
}

template<typename BaseIter> inline
BaseIter unwrap_iterator(boost::utf16_output_iterator<BaseIter> i)
{
	return i.base();
}


template<typename BaseIter> inline
BaseIter unwrap_iterator(boost::utf8_output_iterator<BaseIter> i)
{
	return i.base();
}

} // namespace regular_parse
} // namespace yggr

#endif // __YGGR_REGULAR_PARSE_ITERATOR_ADP_HPP__
