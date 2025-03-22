// collections_load_imp_compatible.hpp

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

#ifndef __YGGR_SERIALIZATION_STL_COLLECTIONS_LOAD_IMP_COMPATIBLE_HPP__
#define __YGGR_SERIALIZATION_STL_COLLECTIONS_LOAD_IMP_COMPATIBLE_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/config.hpp>
#include <boost/ref.hpp>
#include <boost/serialization/collections_load_imp.hpp>

namespace yggr
{
namespace serialization
{
namespace stl
{

#if (BOOST_VERSION < 105900)

using boost::serialization::stl::archive_input_seq;
using boost::serialization::stl::archive_input_map;
using boost::serialization::stl::archive_input_set;
	
using boost::serialization::stl::reserve_imp;
using boost::serialization::stl::no_reserve_imp;

#else
	// sequential container input
template<class Archive, class Container>
struct archive_input_seq
{
	inline typename Container::iterator
		operator()(
			Archive &ar, 
			Container &s, 
			const unsigned int v,
			typename Container::iterator hint)
	{
		typedef typename Container::value_type type;
		boost::serialization::detail::stack_construct<Archive, type> t(ar, v);
		// borland fails silently w/o full namespace
		ar >> boost::serialization::make_nvp("item", t.reference());
		s.push_back(t.reference());
		ar.reset_object_address(boost::addressof(s.back()), boost::addressof(t.reference()));
		return hint;
	}
};

// map input
template<class Archive, class Container>
struct archive_input_map
{
	inline typename Container::iterator
		operator()(
			Archive &ar, 
			Container &s, 
			const unsigned int v,
			typename Container::iterator hint)
	{
		typedef typename Container::value_type type;
		boost::serialization::detail::stack_construct<Archive, type> t(ar, v);
		// borland fails silently w/o full namespace
		ar >> boost::serialization::make_nvp("item", t.reference());
		typename Container::iterator result = s.insert(hint, t.reference());
		// note: the following presumes that the map::value_type was NOT tracked
		// in the archive.  This is the usual case, but here there is no way
		// to determine that.  
		ar.reset_object_address(
			boost::addressof(result->second),
			boost::addressof(t.reference().second) );
		return result;
	}
};

// set input
template<class Archive, class Container>
struct archive_input_set
{
	inline typename Container::iterator
		operator()(
			Archive &ar, 
			Container &s, 
			const unsigned int v,
			typename Container::iterator hint)
	{
		typedef typename Container::value_type type;
		boost::serialization::detail::stack_construct<Archive, type> t(ar, v);
		// borland fails silently w/o full namespace
		ar >> boost::serialization::make_nvp("item", t.reference());

		typename Container::iterator result = s.insert(hint, t.reference());
		
		ar.reset_object_address(
			boost::addressof(*result), 
			boost::addressof(t.reference()) );

		return result;
	}
};

template<class Container>
class reserve_imp
{
public:
	inline void operator()(Container &s, std::size_t count) const 
	{
		s.reserve(count);
	}
};

template<class Container>
class no_reserve_imp
{
public:
	inline void operator()(Container & /* s */, std::size_t /* count */) const
	{
	}
};


#endif // (BOOST_VERSION < 105900)

} // namespace stl
} // namespace serialization
} // namespace yggr

#endif // __YGGR_SERIALIZATION_STL_COLLECTIONS_LOAD_IMP_COMPATIBLE_HPP__

