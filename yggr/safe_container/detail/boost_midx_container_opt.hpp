// boost_midx_container_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_CONTAINER_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_CONTAINER_OPT_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/safe_container/detail/boost_container_opt_path.hpp>
#include <yggr/safe_container/detail/boost_midx_container_opt_decl.hpp>

//#	include YGGR_PP_BOOST_CONTAINER_OPT_LATEST_VERSION_PATH(boost_midx_container_h_opt)

//#if !defined(YGGR_SAFE_CONTAINER_NOT_USING_ALLOCATOR_OPT)
//#	include YGGR_PP_BOOST_CONTAINER_OPT_LATEST_VERSION_PATH(boost_midx_container_ha_opt)
//#endif // YGGR_SAFE_CONTAINER_NOT_USING_ALLOCATOR_OPT

#include YGGR_PP_BOOST_CONTAINER_OPT_LATEST_VERSION_PATH(boost_midx_container_ha_opt)

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETAIL_BOOST_MIDX_CONTAINER_OPT_HPP__
