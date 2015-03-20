//uuid.cpp

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

#include <memory>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <sstream>
#include <boost/functional/hash/hash.hpp>

#include <yggr/ids/uuid.hpp>


#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace ids
{

uuid::uuid(void)
{
}

uuid::uuid(const this_type& right)
	: base_type(right)
{
}

uuid::~uuid(void)
{
}

u32 uuid::variant(void) const
{
    // variant is stored in octet 7
    // which is index 8, since indexes count backwards
    yggr::u8 octet7 = _data[8]; // octet 7 is array index 8
    if ( (octet7 & 0x80) == 0x00 ) { // 0b0xxxxxxx
        return variant_ncs;
    } else if ( (octet7 & 0xC0) == 0x80 ) { // 0b10xxxxxx
        return variant_rfc_4122;
    } else if ( (octet7 & 0xE0) == 0xC0 ) { // 0b110xxxxx
        return variant_microsoft;
    } else {
        //assert( (octet7 & 0xE0) == 0xE0 ) // 0b111xxxx
        return variant_future;
    }
}

u32 uuid::version(void) const
{
    //version is stored in octet 9
    // which is index 6, since indexes count backwards
    yggr::u8 octet9 = _data[6];
    if ( (octet9 & 0xF0) == 0x10 ) {
        return version_time_based;
    } else if ( (octet9 & 0xF0) == 0x20 ) {
        return version_dce_security;
    } else if ( (octet9 & 0xF0) == 0x30 ) {
        return version_name_based_md5;
    } else if ( (octet9 & 0xF0) == 0x40 ) {
        return version_random_number_based;
    } else if ( (octet9 & 0xF0) == 0x50 ) {
        return version_name_based_sha1;
    } else {
        return version_unknown;
    }
}

uuid::this_type& uuid::operator=(const this_type& right)
{
	if(this == &right) {return *this;}
	base_type::operator=(right);
	return *this;
}


void uuid::swap(this_type& right)
{
	base_type::swap(right);
}

std::size_t uuid::hash(void) const
{
	std::size_t seed = 0;
	for(const_iterator i = base_type::begin(), isize = base_type::end(); i != isize; ++i)
	{
		seed ^= static_cast<std::size_t>(*i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	return seed;
}

///*static*/ bool uuid::s_from_code(this_type& id, const std::string& code)
//{
//	yggr::u64 pos = code.find_first_not_of("0123456789abcdef-");
//	if(pos != std::string::npos)
//	{
//		return false;
//	}
//	std::string right_code = code;
//	std::string::iterator new_end = std::remove(right_code.begin(), right_code.end(), '-');
//	if(std::distance(right_code.begin(), new_end) != 32)
//	{
//		return false;
//	}
//
//	yggr::u32 idx = 0;
//	for(std::string::const_iterator i = right_code.begin(), isize = right_code.end();
//			i != isize; std::advance(i, 2), ++idx)
//	{
//		std::stringstream ss;
//		ss << std::hex << (*i) << (*(i + 1));
//		ss >> id[idx];
//	}
//}

} // namespace ids

} // namespace yggr

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace ids
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

std::size_t hash_value(const yggr::ids::uuid& id)
{
	return id.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace ids
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

#define _YGGR_TMP_PP_UUID_SWAP_IMPL() \
	void swap(yggr::ids::uuid& left, yggr::ids::uuid& right) { \
		left.swap(right); }

namespace std
{
	_YGGR_TMP_PP_UUID_SWAP_IMPL()
} // namespace std

namespace boost
{
	_YGGR_TMP_PP_UUID_SWAP_IMPL()
} // namespace boost

#undef _YGGR_TMP_PP_UUID_SWAP_IMPL


