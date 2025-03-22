// time.hpp

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

#ifndef __YGGR_SERIALIZATION_TIME_HPP__
#define __YGGR_SERIALIZATION_TIME_HPP__

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/time/time.hpp>

#include <boost/type_traits/remove_const.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/is_bitwise_serializable.hpp>
#include <boost/mpl/and.hpp>

namespace boost 
{ 
namespace serialization 
{

template<typename Archive> inline
void save(Archive& ar,
			const ::yggr::time::time& t,
			const unsigned int /* file_version */)
{
	typedef ::yggr::time::time now_tm_type;

	yggr::s64 tsec = static_cast<yggr::s64>(t.sec);
	yggr::s64 tnsec = static_cast<yggr::s64>(t.nsec);

	ar << YGGR_SERIALIZATION_NAME_NVP("sec", tsec);
    ar << YGGR_SERIALIZATION_NAME_NVP("nsec", tnsec);
}

template<typename Archive> inline
void load(Archive& ar,
			::yggr::time::time& t,
			const unsigned int /* file_version */)
{
	typedef ::yggr::time::time now_tm_type;

	yggr::s64 tsec = 0;
	yggr::s64 tnsec = 0;

    ar >> YGGR_SERIALIZATION_NAME_NVP("sec", tsec);
    ar >> YGGR_SERIALIZATION_NAME_NVP("nsec", tnsec);

	t.sec = static_cast<now_tm_type::xtime_sec_t>(tsec);
	t.nsec = static_cast<now_tm_type::xtime_nsec_t>(tnsec);
}

template<typename Archive> inline
void serialize(Archive& ar,
				::yggr::time::time& t,
				const unsigned int file_version)
{
    boost::serialization::split_free< Archive, ::yggr::time::time >(ar, t, file_version);
}

} // namespace serialization
} // namespace boost

#include <boost/serialization/collection_traits.hpp>
namespace boost
{
namespace serialization
{

template<>
struct implementation_level< ::yggr::time::time >
{
    typedef mpl::integral_c_tag tag;
    typedef mpl::int_<object_serializable> type;
    YGGR_STATIC_CONSTANT(int, value = object_serializable);
};

} // namespace serialization
} // namespace boost

#endif // __YGGR_SERIALIZATION_TIME_HPP__

#include <yggr/serialization/time_bson_impl.hpp>
