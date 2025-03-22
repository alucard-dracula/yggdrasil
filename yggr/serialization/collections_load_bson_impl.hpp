// collections_load_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_COLLECTIONS_LOAD_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_COLLECTIONS_LOAD_BSON_IMPL_HPP__

#ifdef YGGR_HAS_BSON_IARCHIVE
#	define __YGGR_SERIALIZATION_COLLECTIONS_LOAD_BSON_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array_bson_load_impl.hpp>

#include <sstream>

namespace yggr
{
namespace serialization 
{
namespace container 
{

template<bool skip_auto_id, typename Container, typename InputFunction, typename R>
struct collections_loader_switch<yggr::archive::bson_iarchive<skip_auto_id>, Container, InputFunction, R>
{
	void operator()(yggr::archive::bson_iarchive<skip_auto_id>& ar, Container& s) const
	{
		s.clear();
		collection_size_type count;
		const boost::archive::library_version_type library_version(
			ar.get_library_version());
		// retrieve number of elements
		boost::serialization::item_version_type item_version(0);
		u64 idx = 0;
		{
			std::stringstream ss;
			ss << idx;
			ar >> YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), count);
			++idx;
		}

		if(boost::archive::library_version_type(3) < library_version)
		{
			std::stringstream ss;
			ss << idx;
			ar >> YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), item_version);
			++idx;
		}

		R rx;
		rx(s, count);
	
		InputFunction ifunc;
		typename Container::iterator hint;
		hint = s.begin();
		for(;count-- > 0; ++idx)
		{
			hint = ifunc(ar, s, item_version, hint, idx);
		}
	}
};

} // namespace container 
} // namespace serialization
} // namespace yggr

#	ifdef YGGR_SERIALIZATION_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
#		undef YGGR_SERIALIZATION_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
#	endif // YGGR_SERIALIZATION_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
#else
#	define YGGR_SERIALIZATION_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
#endif // YGGR_HAS_BSON_IARCHIVE

#endif //__YGGR_SERIALIZATION_COLLECTIONS_LOAD_BSON_IMPL_HPP__