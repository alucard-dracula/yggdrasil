//array_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP__

#if defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)
#include <yggr/nsql_database_system/bson_type_traits.hpp>
#include <yggr/serialization/nvp.hpp>

#endif // defined(YGGR_HAS_BSON_OARCHIVE) || defined(YGGR_HAS_BSON_IARCHIVE)

namespace boost
{
namespace serialization
{

#ifdef YGGR_HAS_BSON_OARCHIVE 
#	ifndef __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE__
#		define __YGGR_SERIALIZATION_ARRAY_BSON_IMPL_HPP_OARCHIVE__

#ifndef YGGR_NO_CXX11_HDR_ARRAY

template<typename T, std::size_t N> inline
void serialize(yggr::archive::bson_oarchive<false>& ar, std::array<T,N>& a, const unsigned int /* version */)
{
	ar << YGGR_SERIALIZATION_NAME_NVP("elems", yggr::container::cast_to_native_array_reference(a));
}

template<typename T, std::size_t N> 
void serialize(yggr::archive::bson_oarchive<true>& ar, std::array<T,N>& a, const unsigned int /* version */)
{
	for(std::size_t i = 0; i != N; ++i)
	{
		std::stringstream ss;
		ss << i;
		ar << YGGR_SERIALIZATION_NAME_NVP(ss.str().data(),a[i]);
	}
}

#endif // YGGR_NO_CXX11_HDR_ARRAY

template<typename T, std::size_t N> inline
void serialize(yggr::archive::bson_oarchive<false>& ar, boost::array<T,N>& a, const unsigned int /* version */)
{
	ar << YGGR_SERIALIZATION_NAME_NVP("elems", yggr::container::cast_to_native_array_reference(a));
}

template<typename T, std::size_t N>
void serialize(yggr::archive::bson_oarchive<true>& ar, boost::array<T,N>& a, const unsigned int /* version */)
{
	for(std::size_t i = 0; i != N; ++i)
	{
		std::stringstream ss;
		ss << i;
		ar << YGGR_SERIALIZATION_NAME_NVP(ss.str().data(),a[i]);
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

#ifndef YGGR_NO_CXX11_HDR_ARRAY

template <bool skip_auto_id, typename T, std::size_t N> inline
void serialize(yggr::archive::bson_iarchive<skip_auto_id>& ar, std::array<T,N>& a, const unsigned int /* version */)
{
	ar >> YGGR_SERIALIZATION_NAME_NVP("elems", yggr::container::cast_to_native_array_reference(a));
}

#endif // YGGR_NO_CXX11_HDR_ARRAY

template <bool skip_auto_id, typename T, std::size_t N> inline
void serialize(yggr::archive::bson_iarchive<skip_auto_id>& ar, boost::array<T,N>& a, const unsigned int /* version */)
{
	ar >> YGGR_SERIALIZATION_NAME_NVP("elems", yggr::container::cast_to_native_array_reference(a));
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

template<typename Val, std::size_t N>
struct bson_typeid< Val[N] >
{
	typedef Val type[N];
	typedef Val wrap_type[N];

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
