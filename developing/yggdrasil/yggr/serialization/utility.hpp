//utility.hpp

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

#ifndef __YGGR_SERIALIZATION_UTILITY_HPP__
#define __YGGR_SERIALIZATION_UTILITY_HPP__

#include <utility>
#include <boost/config.hpp>

#include <boost/type_traits/remove_const.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/mpl/and.hpp>

namespace boost 
{ 
namespace serialization 
{

template<typename Archive, typename Key, typename Val>
inline void save(Archive & ar,
					const std::pair<Key, Val> &t,
					const unsigned int /* file_version */)
{
    typedef BOOST_DEDUCED_TYPENAME boost::remove_const<Key>::type typef;
    ar << boost::serialization::make_nvp("first", const_cast<typef &>(t.first));
    ar << boost::serialization::make_nvp("second", t.second);
}

template<typename Archive, typename Key, typename Val>
inline void load(Archive & ar,
					std::pair<Key, Val> &t,
					const unsigned int /* file_version */)
{
   typedef BOOST_DEDUCED_TYPENAME boost::remove_const<Key>::type typef;
    ar >> boost::serialization::make_nvp("first", const_cast<typef &>(t.first));
    ar >> boost::serialization::make_nvp("second", t.second);
}

// pair
template<typename Archive, typename Key, typename Val>
inline void serialize(Archive & ar,
						std::pair<Key, Val>& t,
						const unsigned int file_version)
{
    boost::serialization::split_free<Archive, 
										std::pair<Key, Val>
								>(ar, t, file_version);
}

} // serialization
} // namespace boost

#include <boost/serialization/collection_traits.hpp>
namespace boost
{
namespace serialization
{

template<typename Key, typename Val>
struct implementation_level< std::pair <Key, Val> >
{
    typedef mpl::integral_c_tag tag;
    typedef mpl::int_<object_serializable> type;
    BOOST_STATIC_CONSTANT(int, value = object_serializable);
};

} // namespace serialization
} // namespace boost

#endif // __YGGR_SERIALIZATION_UTILITY_HPP__

#include <yggr/serialization/utility_bson_impl.hpp>