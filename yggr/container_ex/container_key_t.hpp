// container_key_t.hpp

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

#ifndef __YGGR_CONTAINER_EX_CONTAINER_KEY_T_HPP__
#define __YGGR_CONTAINER_EX_CONTAINER_KEY_T_HPP__

#include <yggr/container_ex/has_key_t.hpp>
#include <yggr/container_ex/container_mapped_value_t.hpp>

namespace yggr
{
namespace container_ex
{
namespace detail
{

template<typename Container, bool b_has_key_t = has_key_t<Container>::value >
struct container_key_t_detail
	: yggr::container_ex::container_mapped_key_t<Container>
{
};

template<typename Container>
struct container_key_t_detail<Container, true>
{
	typedef typename Container::key_type type;
};

} // namespace detail

template<typename Container>
struct container_key_t
	: public 
		detail::container_key_t_detail<Container>
		
{
public:
	typedef typename boost::range_value<Container>::type value_type;
};

} // namespace container_ex

using container_ex::container_key_t;

} // namespace yggr

#endif // __YGGR_CONTAINER_EX_CONTAINER_KEY_T_HPP__

