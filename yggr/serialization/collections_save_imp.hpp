//collections_save_imp.hpp

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

#ifndef __YGGR_SERIALIZATION_COLLECTIONS_SAVE_IMP_HPP__
#define __YGGR_SERIALIZATION_COLLECTIONS_SAVE_IMP_HPP__

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/collection_size_type.hpp>

#include <boost/ref.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/item_version_type.hpp>

namespace yggr
{
namespace serialization
{
namespace container
{

template<typename Archive, typename Container>
struct collections_saver_switch
{
	void operator()(Archive& ar, const Container& s) const
	{
		// record number of elements
		collection_size_type count(s.size());
		const boost::serialization::item_version_type item_version(
			boost::serialization::version<typename Container::value_type>::value
		);
		ar << YGGR_SERIALIZATION_NVP(count);
		ar << YGGR_SERIALIZATION_NVP(item_version);
		typename Container::const_iterator it = s.begin();
		while(count-- > 0){
			// note borland emits a no-op without the explicit namespace
			boost::serialization::save_construct_data_adl(
				ar,
				boost::addressof(*it),
				item_version
			);
			ar << YGGR_SERIALIZATION_NAME_NVP("item", *it++);
		}
	}
};

template<typename Archive, typename Container> inline
void save_collection(Archive& ar, const Container &s)
{
    collections_saver_switch<Archive, Container> saver;
	saver(ar, s);
}

} // namespace container
} // namespace serialization
} // namespace boost

#endif //__YGGR_SERIALIZATION_COLLECTIONS_SAVE_IMP_HPP__

#include <yggr/serialization/collections_save_bson_impl.hpp>