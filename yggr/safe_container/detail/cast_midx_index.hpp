//cast_midx_index.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETIAL_CAST_MIDX_INDEX_HPP__
#define __YGGR_SAFE_CONTAINER_DETIAL_CAST_MIDX_INDEX_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename Index>
struct index_opt_cast;

template<typename Index>
struct index_org_cast;

template<typename Index>
struct index_wrap_opt_cast;

template<typename Index>
struct index_wrap_org_cast;

} // namespace detail
} // namespace safe_container
} // namespace yggr

namespace yggr
{
namespace safe_container
{
namespace detail
{

// index_opt_cast
template<typename Index>
struct index_opt_cast
{
	typedef Index type;
};

// index_org_cast
template<typename Index>
struct index_org_cast
{
	typedef Index type;
};

// index_wrap_opt_cast
template<typename Index>
struct index_wrap_opt_cast
{
	typedef Index type;
};

// index_org_cast
template<typename Index>
struct index_wrap_org_cast
{
	typedef Index type;
};

// cast to foo

// cast to opt
template<typename Index> inline 
typename index_opt_cast<Index>::type&
	cast_to_index_opt(Index& src)
{
	typedef typename index_opt_cast<Index>::type dst_type;
	return reinterpret_cast<dst_type&>(src);
}

template<typename Index> inline
const typename index_opt_cast<Index>::type&
	cast_to_index_opt(const Index& src)
{
	typedef typename index_opt_cast<Index>::type dst_type;
	return reinterpret_cast<const dst_type&>(src);
}

// cast to org
template<typename Index> inline 
typename index_org_cast<Index>::type&
	cast_to_index_org(Index& src)
{
	typedef typename index_org_cast<Index>::type dst_type;
	return reinterpret_cast<dst_type&>(src);
}

template<typename Index> inline 
const typename index_org_cast<Index>::type&
	cast_to_index_org(const Index& src)
{
	typedef typename index_org_cast<Index>::type dst_type;
	return reinterpret_cast<const dst_type&>(src);
}

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETIAL_CAST_MIDX_INDEX_HPP__