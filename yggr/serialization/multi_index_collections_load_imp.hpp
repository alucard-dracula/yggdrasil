//multi_index_collection_load_imp.hpp

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

#ifndef __YGGR_SERIALIZATION_MULTI_INDEX_COLLECTIONS_LOAD_IMP_HPP__
#define __YGGR_SERIALIZATION_MULTI_INDEX_COLLECTIONS_LOAD_IMP_HPP__

#include <yggr/base/yggrdef.h>

// helper function templates for serialization of hashed collections
#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/collection_size_type.hpp>

#include <boost/archive/detail/basic_iarchive.hpp>
#include <boost/serialization/item_version_type.hpp>

namespace yggr
{
namespace serialization 
{
namespace container 
{

template<typename Archive, typename Container, typename InputFunction>
struct multi_index_collection_loader_switch
{
	void operator()(Archive& ar, Container& s) const
	{
		s.clear();
		collection_size_type count;
		boost::serialization::item_version_type item_version(0);
		boost::archive::library_version_type library_version(
			ar.get_library_version()
		);
		// retrieve number of elements
		if(boost::archive::library_version_type(6) != library_version)
		{
			ar >> YGGR_SERIALIZATION_NVP(count);
		}
		else
		{
			// note: fixup for error in version 6.  collection size was
			// changed to size_t BUT for hashed collections it was implemented
			// as an unsigned int.  This should be a problem only on win64 machines
			// but I'll leave it for everyone just in case.
			unsigned int c;
			ar >> YGGR_SERIALIZATION_NVP(c);
			count = c;
		}
		if(boost::archive::library_version_type(3) < library_version)
		{
			ar >> YGGR_SERIALIZATION_NVP(item_version);
		}

		InputFunction ifunc;
		while(count-- > 0)
		{
			ifunc(ar, s, item_version);
		}
	}
};

template<typename Archive, typename Container, typename InputFunction> inline
void load_multi_index_collection(Archive& ar, Container& s)
{
	multi_index_collection_loader_switch<Archive, Container, InputFunction> loader;
	loader(ar, s);
}

} // namespace container 
} // namespace serialization
} // namespace yggr

#endif //__YGGR_SERIALIZATION_MULTI_INDEX_COLLECTIONS_LOAD_IMP_HPP__

#include <yggr/serialization/multi_index_collections_load_bson_impl.hpp>
