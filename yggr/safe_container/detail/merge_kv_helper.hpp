//merge_kv_helper.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_MERGE_KV_HELPER_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_MERGE_KV_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/move/move.hpp>

//#include <yggr/container/map.hpp>
//#include <yggr/container/options_map.hpp>
//#include <yggr/container/set.hpp>
//#include <yggr/container/options_set.hpp>
//#include <yggr/container/unordered_set.hpp>
//#include <yggr/container/unordered_map.hpp>

namespace yggr
{
namespace container
{

template<typename L, typename R> inline
void merage(L&, R&)
{
	YGGR_MPLEX_FXIED_ASSERT_FALSE();
}

} // namespace container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename LCont, typename RCont>
struct merge_kv_ref_helper
{
public:
	typedef LCont lcont_type;
	typedef RCont rcont_type;

public:
	merge_kv_ref_helper(lcont_type& l)
		: _l(l)
	{
	}

	~merge_kv_ref_helper(void)
	{
	}
public:
	inline void operator()(rcont_type& r) const
	{
		using ::yggr::container::merge; // msvc cpp20 must using "using" fix adl
		merge(_l, r); 
	}

public:
	lcont_type& _l;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_MERGE_KV_HELPER_HPP__
