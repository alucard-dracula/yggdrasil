//array_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP__

#if defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)
#include <yggr/nsql_database_system/bson_type_traits.hpp>
#include <yggr/serialization/nvp.hpp>

//#include <yggr/serialization/detail/bson_archive_compile_def.hpp>
#endif // defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)

namespace boost
{
namespace serialization
{

#ifdef YGGR_HAS_BSON_OARCHIVE 
#	ifndef __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE__
#		define __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE__
template <typename T, std::size_t N>
void serialize(yggr::archive::bson_oarchive<false>& ar, boost::array<T,N>& a, const unsigned int /* version */)
{
	ar << YGGR_SERIALIZE_NAME_NVP("elems",a.elems);
}

template <typename T, std::size_t N>
void serialize(yggr::archive::bson_oarchive<true>& ar, boost::array<T,N>& a, const unsigned int /* version */)
{
	for(std::size_t i = 0; i != N; ++i)
	{
		std::stringstream ss;
		ss << i;
		ar << YGGR_SERIALIZE_NAME_NVP(ss.str().c_str(),a[i]);
	}
}
#		ifdef YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE
#			undef YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE
#		endif // YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE
#	endif // __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE__
#else
#	define YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE
#endif //YGGR_HAS_BSON_OARCHIVE

#ifdef YGGR_HAS_BSON_IARCHIVE
#	ifndef __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE__
#	define __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE__
template <bool skip_auto_id, typename T, std::size_t N>
void serialize(yggr::archive::bson_iarchive<skip_auto_id>& ar, boost::array<T,N>& a, const unsigned int /* version */)
{
	ar >> YGGR_SERIALIZE_NAME_NVP("elems",a.elems);
}
#		ifdef YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE
#			undef YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE
#		endif // YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE
#	endif // __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE__
#else
#	define YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE
#endif // YGGR_HAS_BSON_IARCHIVE

} // namespace serialization
} // namespace boost



namespace yggr
{
namespace nsql_database_system
{

#if defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)
#	ifndef __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__
#	define __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__
template<typename Val, int N>
struct bson_value_typeid< Val[N] >
{
	enum
	{
		value = bson_value_type::E_BSON_TYPE_ARRAY
	};
};
#	endif //__YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__
#endif // defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)

} // namespace nsql_database_system
} // namespace yggr

#if (defined( __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE__ ) \
		&& defined( __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE__ ) \
		&& defined( __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__ ) )

#	define __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP__
#	undef __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE__
#	undef __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_IARCHIVE__
#	undef __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_BSON_VALUE_TYPEID__

#endif // defined( __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE__ )

#endif // __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP__
