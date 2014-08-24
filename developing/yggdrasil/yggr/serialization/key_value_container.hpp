//key_value_container.hpp

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

#ifndef __YGGR_SERIALIZATION_KEY_VALUE_CONTAINER_HPP__
#define __YGGR_SERIALIZATION_KEY_VALUE_CONTAINER_HPP__

#include <boost/config.hpp>

#include <yggr/serialization/utility.hpp>
#include <yggr/serialization/collections_save_imp.hpp>
#include <yggr/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>

#define YGGR_SERIALIZATION_KEY_VALUE_CONTINER_SAVE_LOAD( __type__ ) \
template<typename Archive, typename Key, typename Val, typename Cmp, typename Alloc> \
inline void save(Archive & ar, \
					const __type__<Key, Val, Cmp, Alloc> &t, \
					const unsigned int /* file_version */) { \
    yggr::serialization::container::save_collection< \
        Archive, __type__<Key, Val, Cmp, Alloc> >(ar, t); } \
		\
template<typename Archive, typename Key, typename Val, typename Cmp, typename Alloc> \
inline void load(Archive & ar, \
					__type__<Key, Val, Cmp, Alloc> &t, \
					const unsigned int /* file_version */){ \
    yggr::serialization::container::load_collection< \
        Archive, \
        __type__<Key, Val, Cmp, Alloc>, \
        boost::serialization::stl::archive_input_map< \
            Archive, __type__<Key, Val, Cmp, Alloc> >, \
            boost::serialization::stl::no_reserve_imp< __type__ < \
                Key, Val, Cmp, Alloc > > >(ar, t); } \
		\
template<typename Archive, typename Key, typename Val, typename Cmp, typename Alloc> \
inline void serialize(Archive & ar, \
						__type__<Key, Val, Cmp, Alloc> &t, \
						const unsigned int file_version) { \
    boost::serialization::split_free(ar, t, file_version); }

#endif // __YGGR_SERIALIZATION_KEY_VALUE_CONTAINER_HPP__