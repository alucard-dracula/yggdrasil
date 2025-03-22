// multi_index_container_inserter.hpp

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

#ifndef __YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_INSERTER_HPP__
#define __YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_INSERTER_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

namespace yggr
{
namespace serialization
{
namespace container
{

template<typename Index_Container>
struct back_inserter
{
	typedef Index_Container cont_type;
	typedef typename cont_type::const_iterator citer_type;

	template<typename T> inline
	std::pair<citer_type, bool>
		operator()(cont_type& s, T& val) const
	{
		return s.insert(val);
	}
};

template<typename SuperMeta, typename TagList>
struct back_inserter< boost::multi_index::detail::sequenced_index<SuperMeta, TagList> >
{
	typedef boost::multi_index::detail::sequenced_index<SuperMeta, TagList> cont_type;
	typedef typename cont_type::const_iterator citer_type;

	template<typename T> inline
	std::pair<citer_type, bool>
		operator()(cont_type& s, T& val) const
	{
		return s.push_back(val);
	}
};

template<typename SuperMeta, typename TagList>
struct back_inserter< boost::multi_index::detail::random_access_index<SuperMeta, TagList> >
{
	typedef boost::multi_index::detail::random_access_index<SuperMeta, TagList> cont_type;
	typedef typename cont_type::const_iterator citer_type;

	template<typename T> inline
	std::pair<citer_type, bool>
		operator()(cont_type& s, T& val) const
	{
		return s.push_back(val);
	}
};

template<typename V, typename I, typename A> inline
std::pair<typename boost::multi_index::multi_index_container<V, I, A>::const_iterator, bool>
	back_insert(boost::multi_index::multi_index_container<V, I, A>& midx, V& v)
{
	typedef boost::multi_index::multi_index_container<V, I, A> midx_type;
	typedef typename boost::multi_index::nth_index<midx_type, 0>::type idx_cont_type;
	typedef back_inserter<idx_cont_type> inserter_type;

	inserter_type inserter;
	return inserter(midx.template get<0>(), v);
}


} // namespace container
} // namespace serialization
} // namespace yggr

#endif // __YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_INSERTER_HPP__
