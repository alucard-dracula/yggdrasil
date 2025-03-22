//c_bson_timestamp.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_TIMESTAMP_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_TIMESTAMP_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/move/move.hpp>

#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/ppex/friend.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/ref.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <cassert>

namespace yggr
{
namespace nsql_database_system
{

struct bson_timestamp_t;
class c_bson_timestamp;

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_WRAP_TYPEID_IMPL(bson_timestamp_t, c_bson_timestamp, bson_typeid_def::E_BSON_TYPE_TIMESTAMP)
YGGR_PP_BSON_TYPEID_IMPL(c_bson_timestamp, bson_typeid_def::E_BSON_TYPE_TIMESTAMP)

namespace yggr
{
namespace nsql_database_system
{

struct bson_timestamp_t
{
	union
	{
		struct
		{
			u32 t;
			u32 i;
		};
		u64 tms;
	};
};

class c_bson_timestamp
	: public bson_timestamp_t
{
public:
	typedef bson_timestamp_t base_type;

private:
	typedef c_bson_timestamp this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_bson_timestamp(void);
	c_bson_timestamp(u32 time, u32 inc);
	c_bson_timestamp(const bson_value_t& val);

	c_bson_timestamp(BOOST_RV_REF(base_type) right)
	{
		base_type::t = right.t;
		base_type::i = right.i;
	}

	c_bson_timestamp(const base_type& right);

	c_bson_timestamp(BOOST_RV_REF(this_type) right)
	{
		base_type::tms = right.tms;
	}

	c_bson_timestamp(const this_type& right);
	~c_bson_timestamp(void);

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	template<typename BsonTimestamp> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimestamp>, this_type&>::type
		operator=(BOOST_RV_REF(BsonTimestamp) right)
	{
		BsonTimestamp& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::tms = right.tms;
		return *this;
	}

	template<typename BsonTimestamp> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimestamp>, this_type&>::type
		operator=(const BsonTimestamp& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		base_type::tms = right.tms;
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		base_type::tms = right_ref.tms;
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	template<typename BsonTimestamp> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimestamp>, void>::type
		swap(BOOST_RV_REF(BsonTimestamp) right)
	{
		BsonTimestamp& right_ref = right;

		if(this == boost::addressof(right_ref))
		{
			return;
		}

		yggr::swap(base_type::tms, right_ref.tms);
	}

	template<typename BsonTimestamp> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimestamp>, void>::type
		swap(BsonTimestamp& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		yggr::swap(base_type::tms, right.tms);
	}

protected:
	void pro_assign_bson_value_t(const bson_value_t& val);

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& right) const
	{
		return bson_value_native_ex::s_value_cmp_eq_timestamp(boost::addressof(right), base_type::t, base_type::i);
	}

	template<typename BsonTimestamp> inline
	typename boost::enable_if<boost::is_base_of<base_type, BsonTimestamp>, bool>::type
		compare_eq(const BsonTimestamp& right) const
	{
		return base_type::tms == right.tms;
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_TIMESTAMP;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_TIMESTAMP;
	}

public:
	inline this_type& grab(void)
	{
		++base_type::i;
		return *this;
	}

	inline u32& time(void)
	{
		return base_type::t;
	}

	inline u32 time(void) const
	{
		return base_type::t;
	}

	inline u32& inc(void)
	{
		return base_type::i;
	}

	inline u32 inc(void) const
	{
		return base_type::i;
	}

	inline u64& timestamp(void)
	{
		return base_type::tms;
	}

	inline u64 timestamp(void) const
	{
		return base_type::tms;
	}

private:
	YGGR_PP_FRIEND_CLASS( bson_inner_data_accesser<true> );

	//save and load def
	template<typename Bson, typename String > inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson>,
				charset::utf8_string_constructable< String >
			>,
			bool
		>::type
		save(Bson& bs, const String& name) const
	{
		typedef String now_string_type;
		YGGR_TYPEDEF_CONST_UTF8_STRING_OR_STRING_VIEW_TYPE_TPL(now_string_type, now_inner_string_type);

		if(bson_native_ex::s_bson_is_nil(&bs))
		{
			bson_init(&bs);
		}

		now_inner_string_type str_name((charset::string_charset_helper_data(name)));
		return
			bson_append_timestamp(
				&bs, str_name.data(), str_name.size(),
				base_type::t, base_type::i);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		BOOST_MPL_ASSERT((boost::mpl::bool_<sizeof(base_type) == sizeof(u64)>));
		return bson_native_ex::s_bson_save_value_of_timestamp(&bs, base_type::tms);
	}

	//template<typename BsonIter, typename Char> inline
	//typename
	//	boost::enable_if
	//	<
	//		boost::mpl::and_
	//		<
	//			boost::is_base_of<bson_iter_t, BsonIter>,
	//			is_native_char<Char>
	//		>,
	//		void
	//	>::type
	//	load(const BsonIter& iter, const Char* name)
	//{
	//	assert(iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name));
	//	if(!iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name))
	//	{
	//		return;
	//	}

	//	u32 tmp_t = 0;
	//	u32 tmp_i = 0;
	//	bson_iter_timestamp(&iter, &tmp_t, &tmp_i);
	//	base_type::t = tmp_t;
	//	base_type::i = tmp_i;
	//}

	//template<typename BsonIter, typename Char, typename Traits, typename Alloc,
	//			template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	//typename
	//	boost::enable_if
	//	<
	//		boost::mpl::and_
	//		<
	//			boost::is_base_of<bson_iter_t, BsonIter>,
	//			charset::is_string_t< Basic_String<Char, Traits, Alloc> >
	//		>,
	//		void
	//	>::type
	//	load(const BsonIter& iter, const Basic_String<Char, Traits, Alloc>& name)
	//{
	//	assert(iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name));
	//	if(!iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name))
	//	{
	//		return;
	//	}

	//	u32 tmp_t = 0;
	//	u32 tmp_i = 0;
	//	bson_iter_timestamp(&iter, &tmp_t, &tmp_i);
	//	base_type::t = tmp_t;
	//	base_type::i = tmp_i;
	//}

	template<typename BsonIter, typename String > inline
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_iter_t, BsonIter>,
				charset::utf8_string_constructable< String >
			>,
			void
		>::type
		load(const BsonIter& iter, const String& name)
	{
		assert(iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name));
		if(!iter.load_check(bson_typeid< boost::remove_const<this_type>::type >::value, name))
		{
			return;
		}

		u32 tmp_t = 0;
		u32 tmp_i = 0;
		bson_iter_timestamp(&iter, &tmp_t, &tmp_i);
		base_type::t = tmp_t;
		base_type::i = tmp_i;
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		u32 tmp_i = 0;
		u32 tmp_t = 0;
		bson_iter_timestamp(&iter, &tmp_t, &tmp_i);
		base_type::i = tmp_i;
		base_type::t = tmp_t;
	}
};

// non member operator foo
//==
template<typename BsonTimestamp> inline
typename boost::enable_if<boost::is_base_of<bson_timestamp_t, BsonTimestamp>, bool>::type
	operator==(const bson_timestamp_t& l, const BsonTimestamp& r)
{
	return l.tms == r.tms;
}

template<typename BsonTimestamp> inline
typename boost::enable_if<boost::is_base_of<bson_timestamp_t, BsonTimestamp>, bool>::type
	operator==(const c_bson_timestamp& l, const BsonTimestamp& r)
{
	return l.compare_eq(r);
}

// !=
template<typename BsonTimestamp> inline
typename boost::enable_if<boost::is_base_of<bson_timestamp_t, BsonTimestamp>, bool>::type
	operator!=(const bson_timestamp_t& l, const BsonTimestamp& r)
{
	return !(l.tms == r.tms);
}

template<typename BsonTimestamp> inline
typename boost::enable_if<boost::is_base_of<bson_timestamp_t, BsonTimestamp>, bool>::type
	operator!=(const c_bson_timestamp& l, const BsonTimestamp& r)
{
	return !l.compare_eq(r);
}

// BsonValue
// ==
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const bson_timestamp_t& l, const BsonValue& r)
{
	return bson_value_native_ex::s_value_cmp_eq_timestamp(boost::addressof(r), l.t, l.i);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const bson_timestamp_t& r)
{
	return bson_value_native_ex::s_value_cmp_eq_timestamp(boost::addressof(l), r.t, r.i);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_timestamp& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_timestamp& r)
{
	return r.compare_eq(l);
}

// !=
template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const bson_timestamp_t& l, const BsonValue& r)
{
	return !bson_value_native_ex::s_value_cmp_eq_timestamp(boost::addressof(r), l.t, l.i);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const bson_timestamp_t& r)
{
	return !bson_value_native_ex::s_value_cmp_eq_timestamp(boost::addressof(l), r.t, r.i);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_timestamp& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_timestamp& r)
{
	return !r.compare_eq(l);
}

namespace detail
{

template<>
struct bson_operator_outter<bson_timestamp_t>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const bson_timestamp_t& val,
					bson_json_mode_def::mode_type) const
	{
		os << "{ \"$timestamp\" : { \"t\" : "
			<< val.t
			<< ", \"i\" : " << val.i
			<< " } }";

		return os;
	}
};

template<>
struct bson_operator_outter<c_bson_timestamp>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os,
					const c_bson_timestamp& val,
					bson_json_mode_def::mode_type mode) const
	{
		typedef c_bson_timestamp::base_type now_base_type;
		typedef bson_operator_outter<now_base_type> outter_type;

		outter_type outter;
		return outter(os, static_cast<const now_base_type&>(val), mode);
	}
};


} // namespace detail

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const bson_timestamp_t& val)
{
	typedef detail::bson_operator_outter<bson_timestamp_t> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}


template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_bson_timestamp& val)
{
	typedef detail::bson_operator_outter<c_bson_timestamp> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}


// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_bson_timestamp) val)
//{
//	const c_bson_timestamp& right_cref = val;
//	return yggr::nsql_database_system::operator<<(os, right_cref);
//}

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

inline void swap(bson_timestamp_t& l, bson_timestamp_t& r)
{
	yggr::swap(l.tms, r.tms);
}

template<typename BsonTimestamp> inline
typename boost::enable_if<boost::is_base_of<bson_timestamp_t, BsonTimestamp>, void>::type
	swap(bson_timestamp_t& l, BsonTimestamp& r)
{
	yggr::swap(l.tms, r.tms);
}

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

template<typename BsonTimestamp> inline
typename boost::enable_if<boost::is_base_of<bson_timestamp_t, BsonTimestamp>, void>::type
	swap(bson_timestamp_t& l, BOOST_RV_REF(BsonTimestamp) r)
{
	yggr::swap(l.tms, r.tms);
}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

#if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_timestamp& l, bson_timestamp_t& r)
{
	l.swap(r);
}

#else

template<typename BsonTimestamp> inline
typename boost::enable_if<boost::is_base_of<bson_timestamp_t, BsonTimestamp>, void>::type
	swap(c_bson_timestamp& l, BsonTimestamp& r)
{
	l.swap(r);
}

#endif // #if defined(__GNUC__) && (YGGR_CPP_VERSION < YGGR_CPP_VER_11)

inline void swap(c_bson_timestamp& l, c_bson_timestamp& r)
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_TIMESTAMP_HPP__
