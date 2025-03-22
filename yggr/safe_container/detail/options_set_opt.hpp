//options_set_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_OPTIONS_SET_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_OPTIONS_SET_OPT_HPP__

#include <yggr/container/options_set.hpp>
#include <yggr/safe_container/detail/boost_set_opt.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename T>
struct cast_to_options_set_opt
{
	typedef T type;
};

template<typename T>
struct cast_to_options_multiset_opt
{
	typedef T type;
};

template<typename K, typename C, typename A, typename O>
struct cast_to_options_set_opt< yggr::container::options_set<K, C, A, O> >
{
private:
	typedef ::yggr::container::options_set<K, C, A, O> now_set_type;

public:
	typedef typename cast_to_set_opt<typename now_set_type::base_type>::type type;
};

template<typename K, typename C, typename A, typename O>
struct cast_to_options_multiset_opt< yggr::container::options_multiset<K, C, A, O> >
{
private:
	typedef ::yggr::container::options_multiset<K, C, A, O> now_set_type;

public:
	typedef typename cast_to_multiset_opt<typename now_set_type::base_type>::type type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_OPTIONS_SET_OPT_HPP__