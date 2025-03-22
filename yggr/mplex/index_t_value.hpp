// index_t_value.hpp

/****************************************************************************
Copyright (c) 2014-2022 yggdrasil

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

#ifndef __YGGR_MPLEX_INDEX_T_VALUE_HPP__
#define __YGGR_MPLEX_INDEX_T_VALUE_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/mpl/integral_c.hpp>

namespace yggr
{
namespace mplex
{

namespace detail
{

template<typename T>
struct index_t_value_detail
	: public boost::mpl::integral_c<yggr::s64, static_cast<yggr::s64>(T::value)>
{
};

} // namespace detail

template<typename T>
struct index_t_value
	: public detail::index_t_value_detail<T>::type
{
};

} // namespace mplex
} // namespace yggr


#define YGGR_INDEX_T_VALUE_DEF( __type__, __value__ ) \
	namespace yggr { namespace mplex { namespace detail { \
	template<> struct index_t_value_detail< __type__ > \
		: public boost::mpl::integral_c< yggr::s64, static_cast<yggr::s64>(__value__) > {}; }}}

#define YGGR_INDEX_T_VALUE_GET( __type__ ) yggr::mplex::index_t_value< __type__ >::value

#define YGGR_INDEX_T_VALUE_GET_CAST( __cast_type__, __type__ ) \
			static_cast< __cast_type__ >(yggr::mplex::index_t_value< __type__ >::value)

#endif // __YGGR_MPLEX_INDEX_T_VALUE_HPP__
