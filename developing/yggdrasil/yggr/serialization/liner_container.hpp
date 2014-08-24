// serialization liner_container.hpp

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

#ifndef __YGGR_SERIALIZATION_LINER_CONTAINER_HPP__
#define __YGGR_SERIALIZATION_LINER_CONTAINER_HPP__

#ifdef BOOST_SERIALIZATION_VECTOR_HPP
#error	"use this file can't include <boost/serialization/vector.hpp> ."
#endif // BOOST_SERIALIZATION_VECTOR_HPP

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/type_traits/is_arithmetic.hpp>

#include <yggr/serialization/detail/type_traits.hpp>
#include <yggr/serialization/collections_save_imp.hpp>
#include <yggr/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>
#include <yggr/serialization/array.hpp>
#include <boost/serialization/detail/get_data.hpp>
#include <boost/mpl/bool.hpp>

// default is being compatible with version 1.34.1 files, not 1.35 files
#ifndef BOOST_SERIALIZATION_VECTOR_VERSIONED
#define BOOST_SERIALIZATION_VECTOR_VERSIONED(V) (V==4 || V==5)
#endif // BOOST_SERIALIZATION_VECTOR_VERSIONED

#include <sstream>
#include <yggr/serialization/nvp.hpp>

#define YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD( __type__ ) \
	template<typename Archive, typename Val, typename Alloc> \
	inline void save(Archive& ar, \
					const __type__ < Val, Alloc > &t, \
					const unsigned int, \
					boost::mpl::false_) { \
        yggr::serialization::container::save_collection<Archive, __type__ <Val, Alloc> > \
        (ar, t); } \
		\
	template<typename Archive, typename Val, typename Alloc> \
	inline void load(Archive & ar, \
						__type__<Val, Alloc> &t, \
						const unsigned int, \
						mpl::false_) { \
		yggr::serialization::container::load_collection< \
			Archive, \
			__type__ <Val, Alloc>, \
			boost::serialization::stl::archive_input_seq< \
				Archive, __type__ <Val, Alloc> >, \
			typename boost::mpl::if_<typename yggr::serialization::detail::is_vector<__type__ <Val, Alloc> >::type, \
										boost::serialization::stl::reserve_imp<__type__ <Val, Alloc> >, \
										boost::serialization::stl::no_reserve_imp<__type__ <Val, Alloc> > >::type> \
		(ar, t); } \
		\
	template<typename Archive, typename Val, typename Alloc> \
	inline void save(Archive & ar, \
						const __type__<Val, Alloc> &t, \
						const unsigned int, \
						mpl::true_) { \
		const yggr::serialization::collection_size_type count(t.size()); \
		ar << BOOST_SERIALIZATION_NVP(count); \
		if (!t.empty()) ar << make_array(detail::get_data(t),t.size()); } \
		\
	template<typename Archive, typename Val, typename Alloc> \
	inline void load(Archive & ar, \
						__type__<Val, Alloc> &t, \
						const unsigned int, \
						mpl::true_) { \
		yggr::serialization::collection_size_type count(t.size()); \
		ar >> BOOST_SERIALIZATION_NVP(count); \
		t.resize(count); \
		unsigned int item_version = 0; \
		if(BOOST_SERIALIZATION_VECTOR_VERSIONED(ar.get_library_version())) { \
			ar >> BOOST_SERIALIZATION_NVP(item_version); } \
		if (!t.empty()) ar >> make_array(detail::get_data(t),t.size()); } \
		\
	template<typename Archive, typename Val, typename Alloc> \
	inline void save(Archive & ar, \
						const __type__<Val, Alloc> &t, \
						const unsigned int file_version) { \
		typedef typename yggr::serialization::detail::is_vector< __type__<Val, Alloc> >::type is_vector_type; \
		typedef BOOST_DEDUCED_TYPENAME \
		boost::serialization::use_array_optimization<Archive>::template apply< \
			BOOST_DEDUCED_TYPENAME remove_const<Val>::type > \
				::type is_use_optimized; \
		typedef typename boost::mpl::if_<is_vector_type, \
											is_use_optimized, \
											boost::mpl::bool_<false> >::type use_optimized; \
		save(ar,t,file_version, use_optimized()); } \
		\
	template<typename Archive, typename Val, typename Alloc> \
	inline void load( Archive & ar, \
						__type__<Val, Alloc> &t, \
						const unsigned int file_version) { \
		typedef typename yggr::serialization::detail::is_vector< __type__<Val, Alloc> >::type is_vector_type; \
		typedef BOOST_DEDUCED_TYPENAME \
		boost::serialization::use_array_optimization<Archive>::template apply< \
			BOOST_DEDUCED_TYPENAME remove_const<Val>::type > \
				::type is_use_optimized; \
		typedef typename boost::mpl::if_<is_vector_type, \
											is_use_optimized, \
											boost::mpl::bool_<false> >::type use_optimized; \
		load(ar,t,file_version, use_optimized()); } \
		\
	template<typename Archive, typename Val, typename Alloc> \
	inline void serialize(Archive & ar, \
							__type__<Val, Alloc> & t, \
							const unsigned int file_version) { \
		boost::serialization::split_free(ar, t, file_version); }


	#define YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD( __type__ ) \
	template<class Archive, class Alloc> \
	inline void save(Archive & ar, \
						const __type__<bool, Alloc> &t, \
						const unsigned int) { \
		yggr::serialization::collection_size_type count(t.size()); \
		ar << BOOST_SERIALIZATION_NVP(count); \
		typename __type__<bool, Alloc>::const_iterator it = t.begin(); \
		while(count-- > 0) { \
			bool tb = *it++; \
			ar << boost::serialization::make_nvp("item", tb); } } \
		\
	template<typename Archive, typename Alloc> \
	inline void load(Archive & ar, \
						__type__<bool, Alloc> &t, \
						const unsigned int) { \
		yggr::serialization::collection_size_type count; \
		ar >> BOOST_SERIALIZATION_NVP(count); \
		t.clear(); \
		while(count-- > 0) { \
			bool i; \
			ar >> boost::serialization::make_nvp("item", i); \
			t.push_back(i); } } \
		\
	template<typename Archive, typename Alloc> \
	inline void serialize(Archive & ar, \
							__type__<bool, Alloc> & t, \
							const unsigned int file_version) { \
		boost::serialization::split_free(ar, t, file_version); }

#endif // __YGGR_SERIALIZATION_LINER_CONTAINER_HPP__
