//safe_set_fwd.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_SET_FWD_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_SET_FWD_HPP__

namespace yggr
{
namespace safe_container
{

template<typename Val,
			typename Mutex,
			typename Cmp,
			typename Alloc,
			template<typename _Val, typename _Cmp, typename _Alloc> class Set>
class safe_set;

template<typename Val,
			typename Mutex,
			typename Cmp,
			typename Alloc,
			template<typename _Val, typename _Cmp, typename _Alloc> class MultiSet>
class safe_multiset;

} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{

template<typename Val,
			typename Mutex,
			typename Cmp,
			typename Alloc,
			typename Options, 
			template<typename _Val, typename _Cmp, typename _Alloc, typename _Options> 
			class Set
		>
class safe_options_set;

template<typename Val,
			typename Mutex,
			typename Cmp,
			typename Alloc,
			typename Options, 
			template<typename _Val, typename _Cmp, typename _Alloc, typename _Options> 
			class Set
		>
class safe_options_multiset;

} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_SAFE_SET_FWD_HPP__
