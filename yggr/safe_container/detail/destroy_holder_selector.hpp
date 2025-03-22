//erase_confilict_fix_helper.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_ERASE_CONFILICT_FIX_HELPER_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_ERASE_CONFILICT_FIX_HELPER_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/base/yggr_msg_macro.h>
#include <yggr/compatibility/stl_exception.hpp>

// ----------------------opt version---------------------------------------
#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

namespace yggr
{
namespace safe_container
{
namespace detail
{

template<typename DH, typename MDH, typename Iterator>
struct destroy_holder_selector
{
	typedef DH type;

	template<typename Container> inline
	void operator()(Container&, type&) const
	{
	}
};

template<typename DH, typename MDH, typename EI, typename ZI>
struct destroy_holder_selector<DH, MDH, std::pair<EI, ZI> >
{
	typedef MDH type;

	template<typename Container> inline
	void operator()(Container& cont, type& dh) const
	{
		typedef typename Container::size_type size_type;
		size_type holder_block = cont.size() / type::static_max_size();
		holder_block = static_cast<size_type>((!!holder_block));
		assert(holder_block < 2);
		dh.reserve((type::static_max_size() << holder_block));
	}
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_DETAIL_ERASE_CONFILICT_FIX_HELPER_HPP__
