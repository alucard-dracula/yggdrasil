// yggr serialization vector.hpp

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

#ifndef __YGGR_SERIALIZATION_VECTOR_HPP__
#define __YGGR_SERIALIZATION_VECTOR_HPP__

#include <yggr/container/vector.hpp>
#include <yggr/serialization/liner_container.hpp>

#include <yggr/serialization/detail/type_traits.hpp>

namespace yggr
{
namespace serialization
{
namespace detail
{
template<typename Val, typename Alloc>
struct is_vector< std::vector<Val, Alloc> >
	: public boost::mpl::true_
{
};

#if (BOOST_VERSION < 106700)

template<typename Val, typename Alloc>
struct is_vector< boost::container::vector<Val, Alloc> >
	: public boost::mpl::true_
{
};

#else

template<typename Val, typename Alloc, typename Options>
struct is_vector< boost::container::vector<Val, Alloc, Options> >
	: public boost::mpl::true_
{
};

template<typename Val, typename Alloc>
struct is_vector< yggr::container::detail::vector<Val, Alloc> >
	: public boost::mpl::true_
{
};

#endif // (BOOST_VERSION < 106700)

} // namespace detail
} // namespace serialization
} // namespace yggr

namespace boost
{
namespace serialization
{

	// debug code don't delete
	//template<typename Archive, typename Val, typename Alloc> 
	//inline void save(Archive& ar, 
	//				const std::vector < Val, Alloc >& t, 
	//				const unsigned int, 
	//				boost::mpl::false_) 
	//{ 
 //       yggr::serialization::container::save_collection<Archive, std::vector<Val, Alloc> > 
 //       (ar, t); 
	//} 
	//	
	//template<typename Archive, typename Val, typename Alloc> 
	//inline void load(Archive& ar, 
	//					std::vector<Val, Alloc>& t, 
	//					const unsigned int, 
	//					mpl::false_) 
	//{ 
	//	yggr::serialization::container::load_collection< 
	//		Archive, 
	//		std::vector <Val, Alloc>, 
	//		boost::serialization::stl::archive_input_seq< 
	//			Archive, std::vector <Val, Alloc> >, 
	//		typename boost::mpl::if_<typename yggr::serialization::detail::is_vector<std::vector <Val, Alloc> >::type, 
	//									boost::serialization::stl::reserve_imp<std::vector <Val, Alloc> >, 
	//									boost::serialization::stl::no_reserve_imp<std::vector <Val, Alloc> > >::type> 
	//	(ar, t); 
	//} 
	//	
	//template<typename Archive, typename Val, typename Alloc> 
	//inline void save(Archive& ar, 
	//					const std::vector<Val, Alloc>& t, 
	//					const unsigned int, 
	//					mpl::true_)
	//{ 
	//	const yggr::serialization::collection_size_type count(t.size()); 
	//	ar << YGGR_SERIALIZATION_NVP(count); 
	//	if(!t.empty()) ar << make_array(detail::get_data(t),t.size()); 
	//} 
	//	
	//template<typename Archive, typename Val, typename Alloc> 
	//inline void load(Archive& ar, 
	//					std::vector<Val, Alloc>& t, 
	//					const unsigned int, 
	//					mpl::true_) 
	//{ 
	//	yggr::serialization::collection_size_type count(t.size()); 
	//	ar >> YGGR_SERIALIZATION_NVP(count); 
	//	t.resize(count); 
	//	unsigned int item_version = 0; 
	//	if(BOOST_SERIALIZATION_VECTOR_VERSIONED(ar.get_library_version())) 
	//	{ 
	//		ar >> YGGR_SERIALIZATION_NVP(item_version); 
	//	} 
	//	if(!t.empty()) ar >> make_array(detail::get_data(t),t.size()); 
	//} 
	//	
	//template<typename Archive, typename Val, typename Alloc> 
	//inline void save(Archive& ar, 
	//					const std::vector<Val, Alloc>& t, 
	//					const unsigned int file_version)
	//{ 
	//	typedef typename yggr::serialization::detail::is_vector< std::vector<Val, Alloc> >::type is_vector_type; 
	//	typedef typename 
	//	boost::serialization::use_array_optimization<Archive>::template apply< 
	//		typename remove_const<Val>::type > 
	//			::type is_use_optimized; 
	//	typedef typename boost::mpl::if_<is_vector_type, 
	//										is_use_optimized, 
	//										boost::mpl::bool_<false> >::type use_optimized; 
	//	save(ar,t,file_version, use_optimized()); 
	//} 
	//	
	//template<typename Archive, typename Val, typename Alloc> 
	//inline void load( Archive& ar, 
	//					std::vector<Val, Alloc>& t, 
	//					const unsigned int file_version) 
	//{ 
	//	typedef typename yggr::serialization::detail::is_vector< std::vector<Val, Alloc> >::type is_vector_type; 
	//	typedef typename 
	//	boost::serialization::use_array_optimization<Archive>::template apply< 
	//		typename remove_const<Val>::type > 
	//			::type is_use_optimized; 
	//	typedef typename boost::mpl::if_<is_vector_type, 
	//										is_use_optimized, 
	//										boost::mpl::bool_<false> >::type use_optimized; 
	//	load(ar,t,file_version, use_optimized()); 
	//} 
	//	
	//template<typename Archive, typename Val, typename Alloc> 
	//inline void serialize(Archive& ar, 
	//						std::vector<Val, Alloc> & t, 
	//						const unsigned int file_version) 
	//{ 
	//	boost::serialization::split_free(ar, t, file_version); 
	//}

	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, std::vector)

#if (BOOST_VERSION < 106700)
	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, boost::container::vector)
#else
	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(3, boost::container::vector)
	YGGR_SERIALIZATION_LINER_CONTINER_SAVE_LOAD(2, yggr::container::detail::vector)
#endif // (BOOST_VERSION < 106700)

#if ! BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
	YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, std::vector)

#	if (BOOST_VERSION < 106700)
		YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, boost::container::vector)
#	else
		YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(3, boost::container::vector)
		YGGR_SERIALIZATION_BOOL_LINER_CONTINER_SAVE_LOAD(2, yggr::container::detail::vector)
#	endif // (BOOST_VERSION < 106700)
#endif // BOOST_WORKAROUND

} // namespace serialization
} // namespace boost

#include <yggr/serialization/detail/container_implementation_level_def.hpp>

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, std::vector, object_serializable)

#if (BOOST_VERSION < 106700)
	YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, boost::container::vector, object_serializable)
#else
	YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(3, boost::container::vector, object_serializable)
	YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(2, yggr::container::detail::vector, object_serializable)
#endif // (BOOST_VERSION < 106700)

#endif // __YGGR_SERIALIZATION_VECTOR_HPP__

#include <yggr/serialization/vector_bson_impl.hpp>

