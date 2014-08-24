//unordered_collections_save_imp.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_IMP_HPP__
#define __YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_IMP_HPP__

#include <boost/config.hpp>
#include <yggr/serialization/nvp.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>
#include <yggr/serialization/collection_size_type.hpp>
#include <boost/serialization/item_version_type.hpp>

namespace yggr
{
namespace serialization
{
namespace container 
{

template<typename Archive, typename Container>
struct unordered_collections_saver_switch
{
	inline void operator()(Archive& ar, const Container& s) const
	{
		collection_size_type count(s.size());
		const collection_size_type bucket_count(s.bucket_count());
		const boost::serialization::item_version_type item_version(
			boost::serialization::version<BOOST_DEDUCED_TYPENAME Container::value_type>::value
		);

		ar << BOOST_SERIALIZATION_NVP(count);
		ar << BOOST_SERIALIZATION_NVP(bucket_count);
		ar << BOOST_SERIALIZATION_NVP(item_version);

		BOOST_DEDUCED_TYPENAME Container::const_iterator it = s.begin();
		while(count-- > 0){
			// note borland emits a no-op without the explicit namespace
			boost::serialization::save_construct_data_adl(
				ar, 
				&(*it), 
				boost::serialization::version<
					BOOST_DEDUCED_TYPENAME Container::value_type
				>::value
			);
			ar << boost::serialization::make_nvp("item", *it++);
		}
	}
};

template<typename Archive, typename Container>
inline void save_unordered_collection(Archive& ar, const Container& s)
{
	unordered_collections_saver_switch<Archive, Container> saver;
	saver(ar, s);
}

} // namespace container 
} // namespace serialization
} // namespace yggr

#endif //__YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_IMP_HPP__

#include <yggr/serialization/unordered_collections_save_bson_impl.hpp>
