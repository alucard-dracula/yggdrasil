// bind_av_list_cast.hpp

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

#ifndef __YGGR_FUNC_BIND_AV_LIST_CAST_HPP__
#define __YGGR_FUNC_BIND_AV_LIST_CAST_HPP__

#include <yggr/mplex/cast_to_t_container.hpp>

namespace yggr
{
namespace func
{

template<typename T>
struct bind_av_list_cast
	: public mplex::cast_to_vector<T>
{
};

template<typename R, typename F, typename L>
struct bind_av_list_cast<boost::_bi::bind_t<R, F, L> >
	: public mplex::cast_to_vector<L>
{
};

} // namespace func
} // namespace yggr

#endif // __YGGR_FUNC_BIND_AV_LIST_CAST_HPP__
