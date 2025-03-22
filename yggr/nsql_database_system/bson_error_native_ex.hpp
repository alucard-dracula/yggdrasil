//bson_error_native_ex.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_NATIVE_EX_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_NATIVE_EX_HPP__

#include <yggr/base/yggrdef.h>

#include <yggr/nsql_database_system/bson_config.hpp>
#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>
#include <yggr/nsql_database_system/native_ex_decl_bson.hpp>

#include <yggr/nsql_database_system/bson_error_code_ex_def.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_base_of.hpp>

#define YGGR_ASSERT_BSON_ERROR_TYPE( __type__ ) \
	BOOST_MPL_ASSERT((boost::is_base_of<bson_error_t, __type__>))

#define YGGR_ASSERT_BSON_ERROR_TYPE_NOT( __type__ ) \
	BOOST_MPL_ASSERT_NOT((boost::is_base_of<bson_error_t, __type__>))

namespace yggr
{
namespace nsql_database_system
{

class bson_error_native_ex
{
private:
	typedef bson_error_native_ex this_type;

public:
	static bson_error_t* s_bson_error_init_non_check(bson_error_t* pberr);

	inline static bson_error_t* s_bson_error_init(bson_error_t* pberr)
	{
		assert(pberr);
		return this_type::s_bson_error_init_non_check(pberr);
	}

	static int s_bson_error_init_va_list(bson_error_t* pberr, u32 domain, u32 code, const char* format, va_list ap);
	static bson_error_t* s_bson_error_init(bson_error_t* pberr, u32 domain, u32 code, const char* format, ...);
	static bson_error_t* s_bson_error_copy(bson_error_t* pdst, const bson_error_t* psrc);
	static void s_bson_error_swap(bson_error_t* pl, bson_error_t* pr);
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace detail
{

template<>
struct bson_operator_outter<bson_error_t>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const bson_error_t& right,
					bson_json_mode_def::mode_type) const
	{
		os << "{ "
		<< "\"domain\" : " << right.domain << ", "
		<< "\"code\" : " << right.code << ", "
		<< "\"msg\" : \"" << right.message << "\""
		<< " }";
		return os;
	}
};

} // namespace detail
} // namespace nsql_database_system
} // namespace yggr

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const bson_error_t& right)
{
	typedef yggr::nsql_database_system::detail::bson_operator_outter<bson_error_t> outter_type;

	outter_type outter;
	return outter(os, right, 0);
}

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

void swap(bson_error_t& l, bson_error_t& r);

template<typename BsonErr> inline
typename boost::enable_if<boost::is_base_of<bson_error_t, BsonErr>, void>::type
	swap(bson_error_t& l, BsonErr& r)
{
	bson_error_native_ex::s_bson_error_swap(boost::addressof(l), boost::addressof(r));
}

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename BsonErr> inline
typename boost::enable_if<boost::is_base_of<bson_error_t, BsonErr>, void>::type
	swap(bson_error_t& l, BOOST_RV_REF(BsonErr) r)
{
	BsonErr& right_ref = r;
	bson_error_native_ex::s_bson_error_swap(boost::addressof(l), boost::addressof(right_ref));
}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

} // namespace swap_support

using swap_support::swap;

} // namespace nsql_database_system
} // namespace yggr

namespace std
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::nsql_database_system::swap_support::swap;
} // namespace boost


#endif // __YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_NATIVE_EX_HPP__
