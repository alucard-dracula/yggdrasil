//swap_this_def.hpp

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

#ifndef __YGGR_PPEX_SWAP_THIS_DEF_HPP__
#define __YGGR_PPEX_SWAP_THIS_DEF_HPP__

#include <yggr/base/yggrdef.h>

#define YGGR_PP_SWAP_THIS_REF_DEF( __type__ ) \
	void swap(__type__& l, __type__& r);

#define YGGR_PP_SWAP_THIS_REF_DEF_IMPL( __type__ ) \
	void swap(__type__& l, __type__& r) { \
		l.swap(r); }

#define YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL( __type__ ) \
	inline YGGR_PP_SWAP_THIS_REF_DEF_IMPL( __type__ )

#define YGGR_PP_SWAP_THIS_SUPPORT_REF_DEF( __type__, __support_type__ ) \
	void swap(__type__& l, __type__::__support_type__& r); \
	void swap(__type__::__support_type__& l, __type__& r);

#define YGGR_PP_SWAP_THIS_SUPPORT_REF_DEF_IMPL( __type__, __support_type__ ) \
	void swap(__type__& l, __type__::__support_type__& r) { l.swap(r); } \
	void swap(__type__::__support_type__& l, __type__& r) { r.swap(l); }

#define YGGR_PP_INLINE_SWAP_THIS_SUPPORT_REF_DEF_IMPL( __type__, __support_type__ ) \
	inline void swap(__type__& l, __type__::__support_type__& r) { l.swap(r); } \
	inline void swap(__type__::__support_type__& l, __type__& r) { r.swap(l); }

namespace yggr
{
namespace ppex
{
} // namespace ppex
} // namespace yggr

#endif // __YGGR_PPEX_SWAP_THIS_DEF_HPP__