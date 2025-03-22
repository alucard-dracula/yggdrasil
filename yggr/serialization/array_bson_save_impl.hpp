//array_bson_save_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_ARRAY_BSON_SAVE_IMPL_HPP__
//#define __YGGR_SERIALIZATION_ARRAY_BSON_SAVE_IMPL_HPP__

#ifdef YGGR_HAS_BSON_OARCHIVE
#	define __YGGR_SERIALIZATION_ARRAY_BSON_SAVE_IMPL_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/collection_size_type.hpp>

#include <boost/archive/detail/oserializer.hpp>

#include <sstream>

namespace boost
{
namespace archive
{
namespace detail
{

template<>
struct save_array_type< yggr::archive::bson_oarchive<false> >
{
	template<class T>
	static void invoke(yggr::archive::bson_oarchive<false>& ar, const T& t)
	{
		typedef typename boost::remove_extent< T >::type value_type;

		save_access::end_preamble(ar);
		// consider alignment
		std::size_t c = sizeof(t) / (
			static_cast<const char *>(static_cast<const void *>(&t[1]))
			- static_cast<const char *>(static_cast<const void *>(&t[0]))
		);
		yggr::serialization::collection_size_type count(c);
		yggr::u64 idx = 0;
		{
			std::stringstream ss;
			ss << idx;
			ar << YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), count);
			++idx;
		}

		for(std::size_t i = 0, isize = count; i != isize; ++i, ++idx)
		{
			std::stringstream ss;
			ss << idx;
			ar << YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), t[i]);
		}
	}
};

template<>
struct save_array_type< yggr::archive::bson_oarchive<true> >
{
	template<class T>
	static void invoke( yggr::archive::bson_oarchive<true>& ar, const T& t)
	{
		typedef typename boost::remove_extent< T >::type value_type;

		save_access::end_preamble(ar);
		// consider alignment
		std::size_t c = sizeof(t) / (
			static_cast<const char *>(static_cast<const void *>(&t[1]))
			- static_cast<const char *>(static_cast<const void *>(&t[0]))
		);
		yggr::serialization::collection_size_type count(c);
		yggr::u64 idx = 0;

		for(std::size_t i = 0, isize = count; i != isize; ++i, ++idx)
		{
			std::stringstream ss;
			ss << idx;
			ar << YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), t[i]);
		}
	}
};

} // namespace detail
} // namespace archive
} // namespace boost
#	ifdef YGGR_SERIALIZATION_ARRAY_BSON_SAVE_IMPL_HPP_OARCHIVE
#		undef YGGR_SERIALIZATION_ARRAY_BSON_SAVE_IMPL_HPP_OARCHIVE
#	endif // __YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP__IARCHIVE
#else
#	define YGGR_SERIALIZATION_ARRAY_BSON_SAVE_IMPL_HPP_OARCHIVE
#endif // YGGR_HAS_BSON_IARCHIVE

#endif // __YGGR_SERIALIZATION_ARRAY_BSON_SAVE_IMPL_HPP__
