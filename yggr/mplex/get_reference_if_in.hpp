//get_reference_if_in.hpp

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

#ifndef __YGGR_MPLEX_GET_REFERENCE_IF_IN_HPP__
#define __YGGR_MPLEX_GET_REFERENCE_IF_IN_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/find.hpp>

namespace yggr
{
namespace mplex
{

template<typename Src, typename Default, 
			typename RefableCont,
			typename Pred = 
				boost::is_same
				<
					boost::mpl::_1, 
					typename 
						boost::remove_cv
						<
							typename boost::remove_reference<Src>::type
						>::type
				>
		>
struct get_reference_if_in
{
public:
	typedef Default default_type;
	typedef Src cvr_src_type;
	typedef RefableCont refable_cont_type;
	typedef Pred pred_type;

	typedef typename 
		boost::remove_cv
		<
			typename boost::remove_reference<cvr_src_type>::type
		>::type src_type;

	typedef typename boost::mpl::end<refable_cont_type>::type refable_end_iter_type;
	typedef typename boost::mpl::find_if<refable_cont_type, pred_type>::type refable_finded_iter_type;

public:
	typedef typename
		boost::mpl::if_
		<
			boost::is_same<refable_end_iter_type, refable_finded_iter_type>,
			default_type,
			src_type&
		>::type nctype;

	typedef typename
		boost::mpl::if_
		<
			boost::is_same<refable_end_iter_type, refable_finded_iter_type>,
			default_type,
			const src_type&
		>::type ctype;
};

} // namespace mplex
} // namespace yggr

#endif // __YGGR_MPLEX_GET_REFERENCE_IF_IN_HPP__
