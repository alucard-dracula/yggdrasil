//segment_config.hpp

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

#ifndef __YGGR_SEGMENT_SEGMENT_CONFIG_HPP__
#define __YGGR_SEGMENT_SEGMENT_CONFIG_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/type_traits/aligned_storage.hpp>

///*
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!! fix old version boost( <= 1.53) interprocess						!!!
//!!! win32_api.hpp get_wmi_class_attribute IWbemClassObject_BIPC::Get	!!!
//!!! wchar_variant size													!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//*/
//
//// this macro defined in third_party_issues_config.hpp
//
//#ifndef YGGR_BOOST_OLD_VERSION_COMPATIBILITY_FIXED
//#	define YGGR_BOOST_OLD_VERSION_COMPATIBILITY_FIXED
//#endif // YGGR_BOOST_OLD_VERSION_COMPATIBILITY_FIXED

#endif // __YGGR_SEGMENT_SEGMENT_CONFIG_HPP__
