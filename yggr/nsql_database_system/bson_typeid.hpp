//bson_typeid.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPEID_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPEID_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nsql_database_system/bson_typeid_def.hpp>

#include <boost/mpl/integral_c.hpp>

namespace yggr
{
namespace nsql_database_system
{

template<typename T>
struct bson_typeid;

template<typename T>
struct bson_overlap_typeid;

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<typename InnerHelper, bool is_splice >
struct bson_overlap_typeid_overlap_helper;

template<typename InnerHelper>
struct bson_overlap_typeid_overlap_helper<InnerHelper, true >
{
	YGGR_STATIC_CONSTANT(
		bson_typeid_def::bson_typeid_type, 
		value = bson_typeid_def::E_BSON_TYPE_DOCUMENT);

	template<typename T> inline
	u8 operator()(const T&) const
	{
		return static_cast<u8>(value);
	}
};

template<typename InnerHelper>
struct bson_overlap_typeid_overlap_helper<InnerHelper, false >
{
	typedef InnerHelper inner_type;

	YGGR_STATIC_CONSTANT(u32, value = inner_type::value);
	
	template<typename T> inline
	u8 operator()(const T& obj) const
	{
		inner_type h;
		return h(obj);
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{

template<typename T>
struct bson_typeid
{
	typedef T type;
	typedef T wrap_type;

	YGGR_STATIC_CONSTANT(
		bson_typeid_def::bson_typeid_type, 
		value = bson_typeid_def::E_BSON_TYPE_EOD);

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const
	{
		return static_cast<u8>(value);
	}

	YGGR_CONSTEXPR_OR_INLINE u8 operator()(const type&) const
	{
		return static_cast<u8>(value);
	}
};

template<typename T>
struct bson_overlap_typeid
{
private:
	typedef bson_typeid<T> inner_type;

public:
	typedef typename inner_type::type type;
	typedef typename inner_type::wrap_type wrap_type;

	typedef 
		detail::bson_overlap_typeid_overlap_helper
		<
			inner_type,
			(static_cast<u8>(inner_type::value)
				== static_cast<u8>(bson_typeid_def::E_BSON_TYPE_SPLICE_OBJECT))
		> h_type;

public:
	YGGR_STATIC_CONSTANT(u32, value = h_type::value);

	inline u8 operator()(void) const
	{
		return static_cast<u8>(value);
	}

	inline u8 operator()(const type& obj) const
	{
		h_type h;
		return h(obj);
	}
};

template<typename T> inline
u8 overlap_bson_typeid(void)
{
	typedef T cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	typedef bson_overlap_typeid<value_type> bson_typeid_type;

	bson_typeid_type h;
	return h();
}

template<typename T> inline
u8 overlap_bson_typeid(const T& obj)
{
	typedef T cv_value_type;
	typedef typename boost::remove_cv<cv_value_type>::type value_type;
	typedef bson_overlap_typeid<value_type> bson_typeid_type;

	bson_typeid_type h;
	return h(obj);
}

} // naemspace nsql_database_system
} // namespace yggr

#define YGGR_PP_BSON_TYPEID_IMPL(__type__, __id__) \
	namespace yggr { namespace nsql_database_system { \
		template<> \
		struct bson_typeid< __type__ > { \
			typedef __type__ type; \
			typedef __type__ wrap_type; \
			YGGR_STATIC_CONSTANT( u32, value = ( __id__ ) ); \
			YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const { return static_cast<u8>(value); } \
			YGGR_CONSTEXPR_OR_INLINE u8 operator()(const type&) const { return static_cast<u8>(value); } }; }}

#define YGGR_PP_BSON_WRAP_TYPEID_IMPL(__type__, __wrap_type__, __id__) \
	namespace yggr { namespace nsql_database_system { \
		template<> \
		struct bson_typeid< __type__ > { \
			typedef __type__ type; \
			typedef __wrap_type__ wrap_type; \
			YGGR_STATIC_CONSTANT( u32, value = ( __id__ ) ); \
			YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const { return static_cast<u8>(value); } \
			YGGR_CONSTEXPR_OR_INLINE u8 operator()(const type&) const { return static_cast<u8>(value); } }; }}

#define YGGR_PP_CONTAINER_BSON_TYPEID_IMPL( __template_n__, __type__, __id__ ) \
	namespace yggr { namespace nsql_database_system { \
		template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__template_n__, typename T)> \
		struct bson_typeid< __type__ < YGGR_PP_TEMPLATE_PARAMS_TYPES(__template_n__, T) > > { \
			typedef  __type__ < YGGR_PP_TEMPLATE_PARAMS_TYPES(__template_n__, T) > type; \
			typedef  __type__ < YGGR_PP_TEMPLATE_PARAMS_TYPES(__template_n__, T) > wrap_type; \
			YGGR_STATIC_CONSTANT( u32, value = ( __id__ ) ); \
			YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const { return static_cast<u8>(value); } \
			YGGR_CONSTEXPR_OR_INLINE u8 operator()(const type&) const { return static_cast<u8>(value); } }; }}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

#define YGGR_PP_VARIADIC_CONTAINER_BSON_TYPEID_IMPL( __template_n__, __type__, __id__ ) \
	namespace yggr { namespace nsql_database_system { \
		template<typename ...Args> \
		struct bson_typeid< __type__ < Args... > > { \
			typedef  __type__ < Args... > type; \
			typedef  __type__ < Args... > wrap_type; \
			YGGR_STATIC_CONSTANT( u32, value = ( __id__ ) ); \
			YGGR_CONSTEXPR_OR_INLINE u8 operator()(void) const { return static_cast<u8>(value); } \
			YGGR_CONSTEXPR_OR_INLINE u8 operator()(const type&) const { return static_cast<u8>(value); } }; }}

#else

#	define YGGR_PP_VARIADIC_CONTAINER_BSON_TYPEID_IMPL YGGR_PP_CONTAINER_BSON_TYPEID_IMPL

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_TYPEID_HPP__
