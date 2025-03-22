//reference_trans.hpp

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

#ifndef __YGGR_TYPE_TRAITS_REFERENCE_TRANS_HPP__
#define __YGGR_TYPE_TRAITS_REFERENCE_TRANS_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/ref.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/mpl/if.hpp>

namespace yggr
{

template<typename T>
struct reference_trans
	: public 
		boost::mpl::if_
		<
			boost::is_reference<T>,
			boost::reference_wrapper<typename boost::remove_reference<T>::type>,
			T
		>
{
};

} // namespace yggr

#endif // __YGGR_TYPE_TRAITS_REFERENCE_TRANS_HPP__
