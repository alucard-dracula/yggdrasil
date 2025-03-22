//multi_index_tag.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEX_TAG_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEX_TAG_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/distance.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/multi_index/detail/has_tag.hpp>

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{

template<typename Index>
struct get_tag
{
private:
	typedef Index index_type;
	typedef typename index_type::tag_list tag_list_type;

public:
	typedef typename
		boost::mpl::if_c
		<
			boost::mpl::size<tag_list_type>::value,
			typename boost::mpl::at_c<tag_list_type, 0>::type,
			boost::mpl::na
		>::type type;
};

template<typename Index_Type_List, typename Tag>
struct get_N
	: public
		boost::mpl::if_
		<
			boost::is_same
			<
				typename boost::mpl::end<typename Index_Type_List::type>::type,
				typename
					boost::mpl::find_if
					<
						Index_Type_List,
						boost::multi_index::detail::has_tag<Tag>
					>::type
			>,
			boost::mpl::int_<-1>,
			boost::mpl::int_
			<
				static_cast<int>
				(
					boost::mpl::distance
					<
						typename boost::mpl::begin<typename Index_Type_List::type>::type,
						typename
							boost::mpl::find_if
							<
								Index_Type_List,
								boost::multi_index::detail::has_tag<Tag>
							>::type

					>::value
				)
			>
		>::type
{
};

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEX_TAG_HPP__
