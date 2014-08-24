// yggr serialization unordered_key_value_container.hpp

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

#ifndef __YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTAINER_HPP__
#define __YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTAINER_HPP__

#include <boost/config.hpp>

#include <yggr/serialization/utility.hpp>
#include <yggr/serialization/unordered_collections_save_imp.hpp>
#include <yggr/serialization/unordered_collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/detail/stack_constructor.hpp>

namespace yggr 
{ 
namespace serialization 
{
namespace container 
{

// map input
template<typename Archive, typename Container>
struct archive_input_unordered_key_value
{
    inline void operator()(Archive &ar, Container &s, const unsigned int v)
	{
        typedef BOOST_DEDUCED_TYPENAME Container::value_type type;
        boost::serialization::detail::stack_construct<Archive, type> t(ar, v);
        // borland fails silently w/o full namespace
        ar >> boost::serialization::make_nvp("item", t.reference());
        std::pair<BOOST_DEDUCED_TYPENAME Container::const_iterator, bool> result = 
            s.insert(t.reference());
        // note: the following presumes that the map::value_type was NOT tracked
        // in the archive.  This is the usual case, but here there is no way
        // to determine that.  
        if(result.second)
		{
            ar.reset_object_address(
                & (result.first->second),
                & t.reference().second
            );
        }
    }
};

// multimap input
template<typename Archive, typename Container>
struct archive_input_unordered_multi_key_value
{
    inline void operator()(Archive &ar, Container &s, const unsigned int v)
	{
        typedef BOOST_DEDUCED_TYPENAME Container::value_type type;
        boost::serialization::detail::stack_construct<Archive, type> t(ar, v);
        // borland fails silently w/o full namespace
        ar >> boost::serialization::make_nvp("item", t.reference());
        BOOST_DEDUCED_TYPENAME Container::const_iterator result 
            = s.insert(t.reference());
        // note: the following presumes that the map::value_type was NOT tracked
        // in the archive.  This is the usual case, but here there is no way
        // to determine that.  
        ar.reset_object_address(
            & result->second,
            & t.reference()
        );
    }
};

} // namespace container
} // namespace serialization
} // namespace yggr

#define YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTINER_SAVE_LOAD( __type__ ) \
	template<typename Archive, \
				typename Key, typename Val, typename HashFoo, typename Cmp, typename Alloc> \
	inline void save(Archive & ar, \
						const __type__<Key, Val, HashFoo, Cmp, Alloc> &t, \
						const unsigned int file_version) { \
		yggr::serialization::container::save_unordered_collection< \
			Archive, \
			__type__<Key, Val, HashFoo, Cmp, Alloc> >(ar, t); } \
	\
	template<typename Archive, \
				typename Key, typename Val, typename HashFoo, typename Cmp, typename Alloc> \
	inline void load(Archive & ar, \
						__type__<Key, Val, HashFoo, Cmp, Alloc> &t, \
						const unsigned int file_version) { \
		yggr::serialization::container::load_unordered_collection< \
			Archive, \
			__type__<Key, Val, HashFoo, Cmp, Alloc>, \
			typename boost::mpl::if_< \
				typename yggr::serialization::detail::is_multi_container< \
							__type__<Key, Val, HashFoo, Cmp, Alloc> >::type, \
						yggr::serialization::container::archive_input_unordered_multi_key_value< \
							Archive, \
							__type__<Key, Val, HashFoo, Cmp, Alloc> >, \
						yggr::serialization::container::archive_input_unordered_key_value< \
							Archive, \
							__type__<Key, Val, HashFoo, Cmp, Alloc> > >::type >(ar, t); } \
	\
	template<typename Archive, \
				typename Key, typename Val, typename HashFoo, typename Cmp, typename Alloc> \
	inline void serialize(Archive & ar, \
							__type__<Key, Val, HashFoo, Cmp, Alloc> &t, \
							const unsigned int file_version) { \
		boost::serialization::split_free(ar, t, file_version); }

#endif // __YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTAINER_HPP__