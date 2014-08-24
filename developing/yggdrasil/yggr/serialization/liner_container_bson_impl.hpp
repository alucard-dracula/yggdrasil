//liner_container_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_LINER_CONTAINER_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_LINER_CONTAINER_BSON_IMPL_HPP__

//#include <yggr/serialization/detail/bson_archive_compile_def.hpp>

#ifdef YGGR_HAS_BSON_IARCHIVE
#	define __YGGR_SERIALIZATION_LINER_CONTAINER_BSON_IMPL_HPP__

namespace boost
{
namespace serialization
{
namespace stl
{

template<bool skip_auto_id, typename Container>
struct archive_input_seq<yggr::archive::bson_iarchive<skip_auto_id>, Container>
{
	typedef yggr::archive::bson_iarchive<skip_auto_id> archive_type;
    inline BOOST_DEDUCED_TYPENAME Container::iterator
    operator()(
        archive_type &ar, 
        Container &s, 
        const unsigned int v,
        BOOST_DEDUCED_TYPENAME Container::iterator hint,
		yggr::u64 idx)
	{
        typedef BOOST_DEDUCED_TYPENAME Container::value_type type;
        detail::stack_construct<archive_type, type> t(ar, v);
        // borland fails silently w/o full namespace
		std::stringstream ss;
		ss << idx;
        ar >> boost::serialization::make_nvp(ss.str().c_str(), t.reference());
        s.push_back(t.reference());
        ar.reset_object_address(& s.back() , & t.reference());
        return hint;
    }
};

} // naespace stl
} // namespace serialization
} // namespace boost

#	ifdef YGGR_SERIALIZATION_LINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE
#		undef YGGR_SERIALIZATION_LINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE
#	endif //YGGR_SERIALIZATION_LINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE
#else
#	define YGGR_SERIALIZATION_LINER_CONTAINER_BSON_IMPL_HPP_IARCHIVE
#endif // YGGR_HAS_BSON_IARCHIVE

#endif // __YGGR_SERIALIZATION_LINER_CONTAINER_BSON_IMPL_HPP__