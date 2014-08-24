//array_bson_load_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP__
//#define __YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP__

#ifdef YGGR_HAS_BSON_IARCHIVE
#	define __YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP__

#include <sstream>

#include <boost/archive/detail/iserializer.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/collection_size_type.hpp>

//#include <yggr/serialization/detail/bson_archive_compile_def.hpp>
//#include <yggr/archive/bson_iarchive.hpp>

namespace boost
{
namespace archive
{
namespace detail
{

template<bool skip_auto_id>
struct load_array_type<yggr::archive::bson_iarchive<skip_auto_id> >
{
	template<typename T>
	static void invoke(yggr::archive::bson_iarchive<skip_auto_id> &ar, T &t)
	{
		typedef BOOST_DEDUCED_TYPENAME remove_extent< T >::type value_type;
        
		// convert integers to correct enum to load
		// determine number of elements in the array. Consider the
		// fact that some machines will align elements on boundries
		// other than characters.
		std::size_t current_count = sizeof(t) / (
			static_cast<char *>(static_cast<void *>(&t[1])) 
			- static_cast<char *>(static_cast<void *>(&t[0]))
		);

		yggr::serialization::collection_size_type count;
		yggr::u64 idx = 0;
		{
			std::stringstream ss;
			ss << idx;
			ar >> YGGR_SERIALIZE_NAME_NVP(ss.str().c_str(), count);
			++idx;
		}

		if(static_cast<std::size_t>(count) > current_count)
			boost::serialization::throw_exception(
				archive::archive_exception(
					boost::archive::archive_exception::array_size_too_short
				)
			);

		for(std::size_t i = 0, isize = count; i != isize; ++i, ++idx)
		{
			std::stringstream ss;
			ss << idx;
			ar >> YGGR_SERIALIZE_NAME_NVP(ss.str().c_str(), t[i]);
		}
	}
};

} // namespace detail
} // namespace archive
} // namespace boost

#	ifdef YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP_IARCHIVE
#		undef YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP_IARCHIVE
#	endif // __YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP__IARCHIVE
#else
#	define YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP_IARCHIVE
#endif //YGGR_HAS_BSON_IARCHIVE

#endif // __YGGR_SERIALIZATION_ARRAY_BSON_LOAD_IMPL_HPP__