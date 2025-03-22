// gen_callable_object.hpp

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

#ifndef __YGGR_CALLER_GEN_CALLABLE_OBJECT_HPP__
#define __YGGR_CALLER_GEN_CALLABLE_OBJECT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/is_callable.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>

#include <cassert>

namespace yggr
{
namespace caller
{

template<typename F> inline
typename 
	boost::enable_if
	< 
		boost::mpl::and_
		<
			is_callable<F>,
			boost::is_pointer<F>
		>,
		typename
			boost::add_reference
			<
				typename 
					boost::add_const
					<
						typename boost::remove_pointer<F>::type
					>::type
			>::type
	>::type
	gen_callable_object(const F& f)
{
	return *f;
}

template<typename F> inline
typename 
	boost::enable_if
	< 
		boost::mpl::and_
		<
			is_callable<F>,
			boost::is_pointer<F>
		>,
		typename
			boost::add_reference
			<
				typename boost::remove_pointer<F>
			>::type
	>::type
	gen_callable_object(F& f)
{
	return *f;
}

template<typename F> inline
typename 
	boost::enable_if
	< 
		boost::mpl::and_
		<
			is_callable<F>,
			boost::mpl::not_< boost::is_pointer<F> >
		>,
		const F&
	>::type
	gen_callable_object(const F& f)
{
	return f;
}

template<typename F> inline
typename 
	boost::enable_if
	< 
		boost::mpl::and_
		<
			is_callable<F>,
			boost::mpl::not_< boost::is_pointer<F> >
		>,
		F&
	>::type
	gen_callable_object(F& f)
{
	return f;
}

} // namespace caller
} // namespace yggr

#endif // __YGGR_CALLER_GEN_CALLABLE_OBJECT_HPP__
