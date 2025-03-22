// multi_index_container_t_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP__

#ifdef YGGR_HAS_BSON_IARCHIVE
#	define __YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/serialization/nvp.hpp>

#include <boost/ref.hpp>

#include <sstream>

namespace yggr 
{ 
namespace serialization 
{
namespace container 
{

// multi_index input
template<bool skip_auto_id, typename Container>
struct archive_input_multi_index<yggr::archive::bson_iarchive<skip_auto_id>, Container>
{
    inline void operator()(yggr::archive::bson_iarchive<skip_auto_id>& ar, Container &s, 
							const unsigned int v, u64 idx) const
	{
		typedef yggr::archive::bson_iarchive<skip_auto_id> archive_type;
        typedef typename Container::value_type type;
        boost::serialization::detail::stack_construct<archive_type, type> t(ar, v);
        // borland fails silently w/o full namespace
		std::stringstream ss;
		ss << idx;
        ar >> YGGR_SERIALIZATION_NAME_NVP(ss.str().data(), t.reference());
		std::pair<typename Container::const_iterator, bool> result = 
			back_insert(s, t.reference());
        if(result.second)
            ar.reset_object_address(
				boost::addressof(*result.first), boost::addressof(t.reference()));
    }
};

} // namespace container
} // namespace serialization
} // namespace yggr

#	ifdef YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP_IARCHIVE
#		undef YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP_IARCHIVE
#	endif // YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP_IARCHIVE
#else
#	define YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP_IARCHIVE
#endif // __YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP__

#endif // __YGGR_SERIALIZATION_MULTI_INDEX_CONTAINER_T_BSON_IMPL_HPP__