//boost_umap_ha_opt.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETIAL_BOOST_UMAP_HA_OPT_HPP__
#define __YGGR_SAFE_CONTAINER_DETIAL_BOOST_UMAP_HA_OPT_HPP__

#include <yggr/base/yggrdef.h>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#include <yggr/safe_container/detail/boost_container_opt_path.hpp>
#include <boost/unordered_map.hpp>

namespace yggr
{
namespace safe_container
{
namespace detail
{

template< typename K, typename T, 
			typename H, typename P, typename A >
class boost_umap_ha_opt;

template<typename T>
struct cast_to_umap_ha_opt
{
	typedef T type;
};

template< typename K, typename T, typename H, typename P, typename A>
struct cast_to_umap_ha_opt< boost::unordered_map<K, T, H, P, A> >
{
	typedef boost_umap_ha_opt<K, T, H, P, A> type;
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#include YGGR_PP_BOOST_CONTAINER_OPT_LATEST_VERSION_PATH(boost_umap_ha_opt)

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION
#endif // __YGGR_SAFE_CONTAINER_DETIAL_BOOST_UMAP_HA_OPT_HPP__
