//unordered_collections_load_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP__

#ifdef YGGR_HAS_BSON_IARCHIVE
#	define __YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP__

#include <sstream>

#include <yggr/serialization/nvp.hpp>
//#include <yggr/serialization/detail/bson_archive_compile_def.hpp>
#include <yggr/serialization/array_bson_load_impl.hpp>

namespace yggr
{
namespace serialization
{
namespace container
{

template<bool skip_auto_id, typename Container, typename InputFunction>
struct unordered_collections_loader_switch<yggr::archive::bson_iarchive<skip_auto_id>,
											Container, InputFunction>
{
	inline void operator()(yggr::archive::bson_iarchive<skip_auto_id>& ar, Container& s) const
	{
		s.clear();
		collection_size_type count;
		collection_size_type bucket_count;
		boost::serialization::item_version_type item_version(0);
		boost::archive::library_version_type library_version(
			ar.get_library_version()
		);

		yggr::u64 idx = 0;
		// retrieve number of elements
		if(boost::archive::library_version_type(6) != library_version)
		{
			{
				std::stringstream ss;
				ss << idx;
				ar >> YGGR_SERIALIZE_NAME_NVP(ss.str().c_str(), count);
				++idx;
			}
			{
				std::stringstream ss;
				ss << idx;
				ar >> YGGR_SERIALIZE_NAME_NVP(ss.str().c_str(), bucket_count);
				++idx;
			}
		}
		else
		{
			// note: fixup for error in version 6.  collection size was
			// changed to size_t BUT for hashed collections it was implemented
			// as an unsigned int.  This should be a problem only on win64 machines
			// but I'll leave it for everyone just in case.
			unsigned int c;
			unsigned int bc;
			{
				std::stringstream ss;
				ss << idx;
				ar >> YGGR_SERIALIZE_NAME_NVP(ss.str().c_str(), c);
				count = c;
				++idx;
			}
			{
				std::stringstream ss;
				ss << idx;
				ar >> YGGR_SERIALIZE_NAME_NVP(ss.str().c_str(), bc);
				bucket_count = bc;
				++idx;
			}
		}

		if(boost::archive::library_version_type(3) < library_version)
		{
			std::stringstream ss;
			ss << idx;
			ar >> YGGR_SERIALIZE_NAME_NVP(ss.str().c_str(), item_version);
			++idx;
		}

#		if ! defined(__MWERKS__)
		s.reserve(bucket_count);
#		endif // __MWERKS__
		InputFunction ifunc;
		for(;count-- > 0; ++idx)
		{
			ifunc(ar, s, item_version, idx);
		}
	}
};

} // namespace container
} // namespace serialization
} // namespace yggr

#	ifdef YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
#		undef YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
#	endif // YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
#else
#	define YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
#endif // YGGR_HAS_BSON_IARCHIVE

#endif // __YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP__
