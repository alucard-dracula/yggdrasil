//pc_ibm_icu.hpp

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

#ifndef __YGGR_COMPILE_LINK_PC_IBM_ICU_HPP__
#define __YGGR_COMPILE_LINK_PC_IBM_ICU_HPP__

#include <yggr/compile_link/msvc_cfg.hpp>

#ifdef _MSC_VER

#if defined(BOOST_HAS_ICU) || defined(YGGR_HAS_ICU)

#if !defined(YGGR_ICU_USING_NATIVE_NAME)

#	pragma comment YGGR_COMPILE_LINK_LIB_NO_FIRST_LIB_TAG(icuuc)
#	pragma comment YGGR_COMPILE_LINK_LIB_NO_FIRST_LIB_TAG(icuin)
#	pragma comment YGGR_COMPILE_LINK_LIB_NO_FIRST_LIB_TAG_AND_DEBUG_TAG(icudt)

#else

#if defined(_DEBUG)
#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(icuucd)
#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(icuind)
#else
#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(icuuc)
#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(icuin)
#endif // _DEBUG

#	pragma comment YGGR_COMPILE_LINK_NRULE_LIB(icudt)

#endif // YGGR_ICU_USING_NATIVE_NAME

#endif // BOOST_HAS_ICU

#endif // _MSC_VER

#endif // __YGGR_COMPILE_LINK_PC_IBM_ICU_HPP__
