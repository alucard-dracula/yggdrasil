//unordered_collections_save_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP__

#ifdef YGGR_HAS_BSON_OARCHIVE
#	define __YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array_bson_save_impl.hpp>

#include <boost/ref.hpp>
#include <sstream>

namespace yggr
{
namespace serialization
{
namespace container 
{

template<typename Container>
struct unordered_collections_saver_switch<yggr::archive::bson_oarchive<false>, Container>
{
	void operator()(yggr::archive::bson_oarchive<false>& ar, const Container& s) const
	{
		collection_size_type count(s.size());
		const collection_size_type bucket_count(s.bucket_count());
		const boost::serialization::item_version_type item_version(
			boost::serialization::version<typename Container::value_type>::value
		);

		yggr::u64 idx = 0;

		{
			std::stringstream ss;
			ss << idx;
			ar << YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), count);
			++idx;
		}
		{
			std::stringstream ss;
			ss << idx;
			ar << YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), bucket_count);
			++idx;
		}
		{
			std::stringstream ss;
			ss << idx;
			ar << YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), item_version);
			++idx;
		}

		typename Container::const_iterator it = s.begin();
		for(;count-- > 0; ++idx)
		{
			// note borland emits a no-op without the explicit namespace
			boost::serialization::save_construct_data_adl(
				ar, 
				boost::addressof(*it), 
				boost::serialization::version<
					typename Container::value_type
				>::value
			);
			
			std::stringstream ss;
			ss << idx;
			ar << YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), *it++);
		}
	}
};

template<typename Container>
struct unordered_collections_saver_switch<yggr::archive::bson_oarchive<true>, Container>
{
	void operator()(yggr::archive::bson_oarchive<true>& ar, const Container& s) const
	{
		collection_size_type count(s.size());
		const collection_size_type bucket_count(s.bucket_count());
		const boost::serialization::item_version_type item_version(
			boost::serialization::version<typename Container::value_type>::value
		);

		yggr::u64 idx = 0;

		typename Container::const_iterator it = s.begin();
		for(;count-- > 0; ++idx)
		{
			// note borland emits a no-op without the explicit namespace
			boost::serialization::save_construct_data_adl(
				ar, 
				boost::addressof(*it), 
				boost::serialization::version<
					typename Container::value_type
				>::value
			);
			
			std::stringstream ss;
			ss << idx;
			ar << YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), *it++);
		}
	}
};

} // namespace container
} // namespace serialization
} // namespace yggr

#	ifdef YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP_OARCHIVE
#		undef YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP_OARCHIVE
#	endif // YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP_OARCHIVE
#else
#	define YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP_OARCHIVE
#endif // YGGR_HAS_BSON_OARCHIVE

#endif // __YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP__