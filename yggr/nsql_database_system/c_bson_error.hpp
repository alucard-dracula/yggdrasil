//c_bson_error.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_ERROR_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_ERROR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/support/operator_t.hpp>
#include <yggr/move/move.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/charset/string_view.hpp>
#include <yggr/nsql_database_system/bson_error_native_ex.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace yggr
{
namespace nsql_database_system
{

//BSON_ALIGNED_BEGIN (8)
//typedef struct _bson_error_t
//{
//   uint32_t domain;
//   uint32_t code;
//   char     message[504];
//} bson_error_t
//BSON_ALIGNED_END (8);

class c_bson_error
	: public bson_error_t // must use public extends
{
public:
	typedef bson_error_t base_type;
	typedef base_type org_type;

private:
	typedef c_bson_error this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:

	c_bson_error(void);

	c_bson_error(u32 domain, u32 code, const char* format, ...);

	c_bson_error(BOOST_RV_REF(base_type) right)
	{
		base_type& base = *this;
		bson_error_native_ex::s_bson_error_init(&base);

		base_type& right_ref = right;
		bson_error_native_ex::s_bson_error_copy(this, &right_ref);
	}

	c_bson_error(const base_type& right);

	c_bson_error(BOOST_RV_REF(this_type) right)
	{
		base_type& base = *this;
		bson_error_native_ex::s_bson_error_init(&base);

		this_type& right_ref = right;
		bson_error_native_ex::s_bson_error_copy(this, &right_ref);
	}

	c_bson_error(const this_type& right);
	~c_bson_error(void);

protected:
	this_type& pro_operator_set(const base_type& right);

public:
	template<typename BsonErr> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonErr>, this_type&>::type
		operator=(BOOST_RV_REF(BsonErr) right)
	{
		BsonErr& right_ref = right;
		return this_type::pro_operator_set(static_cast<base_type&>(right_ref));
	}

	template<typename BsonErr> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonErr>, this_type&>::type
		operator=(const BsonErr& right)
	{
		return this_type::pro_operator_set(static_cast<const base_type&>(right));
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		return this_type::pro_operator_set(static_cast<base_type&>(right_ref));
	}

	this_type& operator=(const this_type& right);

protected:
	void pro_swap(base_type& right);

public:
	template<typename BsonErr> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonErr>, void>::type
		swap(BOOST_RV_REF(BsonErr) right)
	{
		BsonErr& right_ref = right;
		this_type::pro_swap(static_cast<base_type&>(right_ref));
	}

	template<typename BsonErr> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonErr>, void>::type
		swap(BsonErr& right)
	{
		this_type::pro_swap(static_cast<base_type&>(right));
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::pro_swap(static_cast<base_type&>(right_ref));
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void)
	{
		return static_cast<org_type*>(this);
	}

	inline const org_type* org_pointer(void) const
	{
		return static_cast<const org_type*>(this);
	}

public:
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return
			base_type::domain
			|| base_type::code
			|| (base_type::message)[0];
	}

public:
	inline u32 domain(void) const
	{
		return base_type::domain;
	}

	inline u32 code(void) const
	{
		return base_type::code;
	}

	inline const char* what(void) const
	{
		return
			(base_type::domain == 0  && base_type::code == 0)?
				(base_type::message[0]? base_type::message : "Success")
				: (base_type::message[0]? base_type::message : "Unknow error");
	}

	inline const char* message(void) const
	{
		return this_type::what();
	}
};

namespace detail
{

template<>
struct bson_operator_outter<c_bson_error>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_bson_error& right,
					bson_json_mode_def::mode_type mode) const
	{
		typedef c_bson_error::org_type now_org_type;
		typedef bson_operator_outter<now_org_type> outter_type;

		outter_type outter;
		return outter(os, static_cast<const now_org_type&>(right), mode);
	}
};

} // namespace detail

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_error& right)
{
	typedef detail::bson_operator_outter<c_bson_error> outter_type;

	outter_type outter;
	return outter(os, right, 0);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_bson_error) right)
//{
//	const c_bson_error& right_cref = right;
//	return yggr::nsql_database_system::operator<<(os, right_cref);
//}

// now don't use this foo style, but don't delete it
//template<typename Char, typename Traits,
//			template<typename _Char, typename _Alloc> class Basic_OStream >
//Basic_OStream<Char, Traits>&
//	operator<<(Basic_OStream<Char, Traits>& os, const c_bson_error& right);

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_error& l, bson_error_t& r)
{
	l.swap(r);
}

#else

template<typename BsonErr> inline
typename boost::enable_if<boost::is_base_of<bson_error_t, BsonErr>, void>::type
	swap(c_bson_error& l, BsonErr& r)
{
	l.swap(r);
}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_error& l, c_bson_error& r)
{
	l.swap(r);
}

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

#endif //__YGGR_NSQL_DATABASE_SYSTEM_C_BSON_ERROR_HPP__
