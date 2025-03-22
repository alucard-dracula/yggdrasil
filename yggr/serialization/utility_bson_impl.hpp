//utility_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP__

#if defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HASH_BSON_IARCHVIE)

#include <yggr/base/yggrdef.h>
#include <yggr/nsql_database_system/bson_type_traits.hpp>
#include <yggr/serialization/nvp.hpp>

#include <sstream>

#endif // defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HASH_BSON_IARCHVIE)

namespace yggr
{
namespace nsql_database_system
{

#if defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HASH_BSON_IARCHVIE)
#	ifndef __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__
#	define __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__

template<typename Key, typename Val>
struct bson_typeid< std::pair<Key, Val> >
{
	typedef std::pair<Key, Val> type;
	typedef std::pair<Key, Val> wrap_type;

	YGGR_STATIC_CONSTANT(
		bson_typeid_def::bson_typeid_type, 
		value = bson_typeid_def::E_BSON_TYPE_ARRAY);

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const 
	{
		return static_cast<u8>(value); 
	}

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(const type&) const
	{
		return static_cast<u8>(value);
	}
};

#	endif // __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__
#endif // defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HASH_BSON_IARCHVIE)

} // namespace nsql_database_system
} // namespace yggr

namespace boost
{
namespace serialization
{

#ifdef YGGR_HAS_BSON_OARCHIVE
#	ifndef __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE__
#	define __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE__

template<bool nld, typename Key, typename Val> inline
void save( yggr::archive::bson_oarchive<nld>& ar,
					const std::pair<Key, Val>& t,
					const unsigned int file_version)
{
	typedef typename boost::remove_const<Key>::type typef;
    ar << YGGR_SERIALIZATION_NAME_NVP("0", const_cast<typef &>(t.first));
    ar << YGGR_SERIALIZATION_NAME_NVP("1", t.second);
}

#		ifdef YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE
#			undef YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE
#		endif // YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE
#	endif // __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE__
#else
#	define YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE
#endif // YGGR_HAS_BSON_OARCHIVE


#ifdef YGGR_HAS_BSON_IARCHIVE
#	ifndef __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE__
#	define __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE__
template<bool skip_auto_id, typename Key, typename Val> inline
void load( yggr::archive::bson_iarchive<skip_auto_id>& ar,
					std::pair<Key, Val>& t,
					const unsigned int file_version)
{
	typedef typename boost::remove_const<Key>::type key_type;
	ar >> YGGR_SERIALIZATION_NAME_NVP("0", const_cast<key_type&>(t.first));
	ar >> YGGR_SERIALIZATION_NAME_NVP("1", t.second);
}

#		ifdef YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE
#			undef YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE
#		endif // YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE
#	endif // __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE__
#else
#	define YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE
#endif // YGGR_HAS_BSON_IARCHIVE

} // namespace serialization
} // namespace boost

#if (defined( __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__ ) \
		&& defined( __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE__ ) \
		&& defined( __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE__ ) )

#	define __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP__
#	undef __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__
#	undef __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_OARCHIVE__
#	undef __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_IARCHIVE__

#endif // defined( __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__ )

#endif // __YGGR_SERIALIZATION_UTILITY_BSON_IMPL_HPP__
