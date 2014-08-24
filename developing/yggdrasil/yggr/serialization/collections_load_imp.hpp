// collections_load_imp.hpp

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

#ifndef __YGGR_SERIALIZATION_COLLECTIONS_LOAD_IMP_HPP__
#define __YGGR_SERIALIZATION_COLLECTIONS_LOAD_IMP_HPP__

#include <boost/serialization/collections_load_imp.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/serialization/collection_size_type.hpp>

namespace yggr
{
namespace serialization 
{
namespace container 
{

template<typename Archive, typename Container, typename InputFunction, typename R>
struct collections_loader_switch
{
	inline void operator()(Archive& ar, Container& s) const
	{
		s.clear();
		collection_size_type count;
		const boost::archive::library_version_type library_version(
			ar.get_library_version());
		// retrieve number of elements
		boost::serialization::item_version_type item_version(0);
		ar >> BOOST_SERIALIZATION_NVP(count);
		if(boost::archive::library_version_type(3) < library_version)
		{
			ar >> BOOST_SERIALIZATION_NVP(item_version);
		}

		R rx;
		rx(s, count);
	
		InputFunction ifunc;
		BOOST_DEDUCED_TYPENAME Container::iterator hint;
		hint = s.begin();
		while(count-- > 0)
		{
			hint = ifunc(ar, s, item_version, hint);
		}
	}
};

template<typename Archive, typename Container, typename InputFunction, typename R>
inline void load_collection(Archive & ar, Container &s)
{
	collections_loader_switch<Archive, Container, InputFunction, R> loader;
	loader(ar, s);
}

} // namespace container 
} // namespace serialization
} // namespace yggr

#endif //__YGGR_SERIALIZATION_COLLECTIONS_LOAD_IMP_HPP__

#include <yggr/serialization/collections_load_bson_impl.hpp>
