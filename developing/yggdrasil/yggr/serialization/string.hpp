//yggr serialization string.hpp

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

#ifndef __YGGR_SERIALIZATION_STRING_HPP__
#define __YGGR_SERIALIZATION_STRING_HPP__

#include <yggr/charset/string.hpp>

#include <boost/archive/detail/oserializer.hpp>
#include <boost/archive/detail/iserializer.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>

namespace boost
{
namespace serialization
{

template<typename Archive, typename Char, typename Traits, typename Alloc>
inline void save(Archive & ar,
					const boost::container::basic_string<Char, Traits, Alloc> &t,
					const unsigned int ver)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> string_type;
	typedef std::basic_string<Char, Traits> store_type;

	store_type tmp(t.begin(), t.end());
	archive::save_access::save_primitive(ar, tmp);
}

template<typename Archive, typename Char, typename Traits, typename Alloc>
inline void load(Archive & ar,
					boost::container::basic_string<Char, Traits, Alloc> &t,
					const unsigned int ver)
{
    typedef boost::container::basic_string<Char, Traits, Alloc> string_type;
	typedef std::basic_string<Char, Traits> load_type;

	load_type tmp;
	archive::load_access::load_primitive(ar, tmp);
	t.assign(tmp.begin(), tmp.end());
}

template<typename Archive, typename Char, typename Traits, typename Alloc>
inline void serialize(Archive & ar,
						boost::container::basic_string<Char, Traits, Alloc> & t,
						const unsigned int ver)
{
    boost::serialization::split_free(ar, t, ver);
}

} // serialization
} // namespace boost

#include <boost/serialization/collection_traits.hpp>

BOOST_SERIALIZATION_COLLECTION_TRAITS(boost::container::basic_string)

#endif // __YGGR_SERIALIZATION_STRING_HPP__

#include <yggr/serialization/string_bson_impl.hpp>

