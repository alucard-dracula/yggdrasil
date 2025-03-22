//safe_list.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_LIST_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_LIST_HPP__

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! All safe containers can not guarantee the creation and destruction of multi-threaded safe,		!!!
!!! in different threads to create and destroy the safe container, please use swap to deal with,	!!!
!!! and																								!!!
!!!  it is forbidden to pass any "iterator reference pointer" inside the safe_container				!!!
!!!	 through the callback function to the outside of the safe_container and save or use it!			!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

#include <yggr/safe_container/optimization_config.hpp>
#include <yggr/safe_container/list/safe_list_org.hpp>

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	include <yggr/safe_container/list/safe_list_opt.hpp>

#	if !(BOOST_VERSION < 107000)
#		include <yggr/safe_container/list/safe_list_opt_boost.hpp>
#	endif // !(BOOST_VERSION < 107000)

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

//-------------------------------------------------non-member_foo cpp20-------------------------------------
namespace yggr
{
namespace safe_container
{

// erase
template<template<typename _T, typename _A> class Lst,
			typename T, typename M, typename A, typename Val> inline
typename safe_list<T, M, A, Lst>::size_type
	erase(safe_list<T, M, A, Lst>& c, const Val& val)
{
	return c.remove(val);
}

// erase_if
template<template<typename _T, typename _A> class Lst,
			typename T, typename M, typename A, typename Pred> inline
typename safe_list<T, M, A, Lst>::size_type
	erase_if(safe_list<T, M, A, Lst>& c, Pred pred)
{
	return c.remove_if(pred);
}

} // namespace safe_container
} // namespace yggr

#endif //__YGGR_SAFE_CONTAINER_SAFE_LIST_HPP__
