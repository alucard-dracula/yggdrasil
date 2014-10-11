// bson_impl_reinclue.hpp

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


//#ifndef __YGGR_SERIALIZATION_DETAIL_BSON_IMPL_REINCLUDE_HPP__
//#define __YGGR_SERIALIZATION_DETAIL_BSON_IMPL_REINCLUDE_HPP__

#if	(defined(YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE) \
	 || defined(YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE))
#	include <yggr/serialization/array.hpp>
#endif // YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP_IARCHIVE
#	include <yggr/serialization/array_bson_load_impl.hpp>
#endif // YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_ARRAY_BSON_SAVE_IMPL_HPP_OARCHIVE
#	include <yggr/serialization/array_bson_save_impl.hpp>
#endif // YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP_OARCHIVE

#ifdef YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP_BSON_ARCHIVE
#	include <yggr/serialization/collection_size_type.hpp>
#endif // YGGR_SERIALIZATION_COLLECTION_SIZE_TYPE_BSON_IMPL_HPP_BSON_ARCHIVE

#ifdef YGGR_SERIALIZATION_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
#	include <yggr/serialization/collections_load_imp.hpp>
#endif // YGGR_SERIALIZATION_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_COLLECTIONS_SAVE_BSON_IMPL_HPP_OARCHIVE
#	include <yggr/serialization/collections_save_imp.hpp>
#endif // YGGR_SERIALIZATION_COLLECTIONS_SAVE_BSON_IMPL_HPP_OARCHIVE

#ifdef YGGR_SERIALIZATION_LINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE
#	include <yggr/serialization/liner_container_bson_impl.hpp>
#endif // YGGR_SERIALIZATION_LINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_DEQUE_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/deque.hpp>
#endif // YGGR_SERIALIZATION_DEQUE_BSON_IMPL_HPP_ARCHIVE

#ifdef YGGR_SERIALIZATION_LIST_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/list.hpp>
#endif // YGGR_SERIALIZATION_DEQUE_BSON_IMPL_HPP_ARCHIVE

#ifdef YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/vector.hpp>
#endif // YGGR_SERIALIZATION_VECTOR_BSON_IMPL_HPP_ARCHIVE

#if	(defined(YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE) \
	 || defined(YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE))
#	include <yggr/serialization/utility.hpp>
#endif // YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_STRING_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/string.hpp>
#endif // YGGR_SERIALIZATION_STRING_BSON_IMPL_HPP_ARCHIVE

#ifdef YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/utf8_string.hpp>
#endif //YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_ARCHIVE

#ifdef YGGR_SERIALIZATION_UNLINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE
#	include <yggr/serialization/unliner_container_bson_impl.hpp>
#endif //YGGR_SERIALIZATION_UNLINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_SET_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/set.hpp>
#endif // YGGR_SERIALIZATION_SET_BSON_IMPL_HPP_ARCHIVE

#ifdef YGGR_SERIALIZATION_KEY_VALUE_CONTAINER_BSON_IMPL_HPP_IARCHIVE
#	include <yggr/serialization/key_value_container_bson_impl.hpp>
#endif // YGGR_SERIALIZATION_KEY_VALUE_CONTAINER_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_MAP_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/map.hpp>
#endif // YGGR_SERIALIZATION_MAP_BSON_IMPL_HPP_ARCHIVE

#ifdef YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE
#	include <yggr/serialization/unordered_collections_load_bson_impl.hpp>
#endif // YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_LOAD_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP_OARCHIVE
#	include <yggr/serialization/unordered_collections_save_bson_impl.hpp>
#endif // YGGR_SERIALIZATION_UNORDERED_COLLECTIONS_SAVE_BSON_IMPL_HPP_OARCHIVE

#ifdef YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE
#	include <yggr/serialization/unordered_unliner_container_bson_impl.hpp>
#endif //YGGR_SERIALIZATION_UNORDERED_UNLINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTAINER_BSON_IMPL_HPP_IARCHIVE
#	include <yggr/serialization/unordered_key_value_container_bson_impl.hpp>
#endif // YGGR_SERIALIZATION_UNORDERED_KEY_VALUE_CONTAINER_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_UNORDERED_SET_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/unordered_set.hpp>
#endif // YGGR_SERIALIZATION_UNORDERED_SET_BSON_IMPL_HPP_ARCHIVE

#ifdef YGGR_SERIALIZATION_UNORDERED_MAP_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/unordered_map.hpp>
#endif // YGGR_SERIALIZATION_UNORDERED_MAP_BSON_IMPL_HPP_ARCHIVE

#ifdef YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP_IARCHIVE
#	include <yggr/serialization/multi_index_container_t_bson_impl.hpp>
#endif // YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/multi_index_container.hpp>
#endif // YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_BSON_IMPL_HPP_ARCHIVE

#ifdef YGGR_SERIALIZATION_MULTI_INDEX_COLLECTION_LOAD_BSON_IMPL_HPP_IARCHIVE
#	include <yggr/serialization/multi_index_collections_load_bson_impl.hpp>
#endif // YGGR_SERIALIZATION_MULTI_INDEX_COLLECTION_LOAD_BSON_IMPL_HPP_IARCHIVE

#ifdef YGGR_SERIALIZATION_MULTI_INDEX_COLLECTIONS_SAVE_BSON_IMPL_OARCHIVE
#	include <yggr/serialization/multi_index_collections_save_bson_impl.hpp>
#endif // YGGR_SERIALIZATION_MULTI_INDEX_COLLECTIONS_SAVE_BSON_IMPL_OARCHIVE

#ifdef YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP_ARCHIVE
#	include <yggr/serialization/tuple.hpp>
#endif // YGGR_SERIALIZATION_TUPLE_BSON_IMPL_HPP_ARCHIVE

//#endif // __YGGR_SERIALIZATION_DETAIL_BSON_IMPL_REINCLUDE_HPP__
