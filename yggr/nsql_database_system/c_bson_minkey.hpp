//c_bson_minkey.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_MINKEY_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_MINKEY_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/friend.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/nsql_database_system/bson_typeid.hpp>
#include <yggr/nsql_database_system/bson_inner_data_accesser_decl.hpp>
#include <yggr/nsql_database_system/bson_native_ex.hpp>
#include <yggr/nsql_database_system/bson_value_native_ex.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <cassert>

namespace yggr
{
namespace nsql_database_system
{

class c_bson_minkey;

} // namespace nsql_database_system
} // namespace yggr

YGGR_PP_BSON_TYPEID_IMPL(c_bson_minkey, bson_typeid_def::E_BSON_TYPE_MINKEY)

namespace yggr
{
namespace nsql_database_system
{

class c_bson_minkey
{
private:
	typedef c_bson_minkey this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_bson_minkey(void)
	{
	}

	c_bson_minkey(const bson_value_t& val)
	{
		this_type::pro_assign_bson_value_t(val);
	}

	c_bson_minkey(BOOST_RV_REF(this_type))
	{
	}

	c_bson_minkey(const this_type&)
	{
	}

	~c_bson_minkey(void)
	{
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, this_type&>::type
		operator=(const BsonValue& val)
	{
		this_type::pro_assign_bson_value_t(val);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type))
	{
		return *this;
	}

	inline this_type& operator=(const this_type&)
	{
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type))
	{
	}

	inline void swap(this_type&)
	{
	}

protected:
	void pro_assign_bson_value_t(const bson_value_t& val)
	{
		bool bsuccess = bson_value_native_ex::s_value_to_minkey(&val);
		assert(bsuccess);
	}

public:
	YGGR_CONSTEXPR_OR_INLINE static u32 value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_MINKEY;
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 s_value_typeid(void)
	{
		return bson_typeid_def::E_BSON_TYPE_MINKEY;
	}

public:
	template<typename BsonValue> inline
	typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
		compare_eq(const BsonValue& val) const
	{
		return bson_value_native_ex::s_value_cmp_eq_minkey(boost::addressof(val));
	}

	inline bool compare_eq(const this_type& right) const
	{
		return true;
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
		return bson_append_minkey(&bs, str_name.data(), str_name.size());
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, bool>::type
		save_value(Bson& bs) const
	{
		return bson_native_ex::s_bson_save_value_of_minkey(&bs);
	}

	template<typename BsonIter, typename String> inline
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
		// don't need load
	}

	template<typename BsonIter> inline
	typename boost::enable_if<boost::is_base_of<bson_iter_t, BsonIter>, void>::type
		load_value(const BsonIter& iter)
	{
		//don't need load
	}
};

inline bool operator==(const c_bson_minkey&, const c_bson_minkey&)
{
	return true;
}

inline bool operator!=(const c_bson_minkey&, const c_bson_minkey&)
{
	return false;
}

// ==
//inline bool operator==(const bson_value_t& l, const c_bson_minkey& r)
//{
//	return bson_value_native_ex::s_value_cmp_eq_minkey(&l);
//}
//
//inline bool operator==(const c_bson_minkey& l, const bson_value_t& r)
//{
//	return bson_value_native_ex::s_value_cmp_eq_minkey(&r);
//}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const BsonValue& l, const c_bson_minkey& r)
{
	return r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator==(const c_bson_minkey& l, const BsonValue& r)
{
	return l.compare_eq(r);
}

// !=
//inline bool operator!=(const bson_value_t& l, const c_bson_minkey& r)
//{
//	return !bson_value_native_ex::s_value_cmp_eq_minkey(&l);
//}
//
//inline bool operator!=(const c_bson_minkey& l, const bson_value_t& r)
//{
//	return !bson_value_native_ex::s_value_cmp_eq_minkey(&r);
//}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const BsonValue& l, const c_bson_minkey& r)
{
	return !r.compare_eq(l);
}

template<typename BsonValue> inline
typename boost::enable_if<boost::is_base_of<bson_value_t, BsonValue>, bool>::type
	operator!=(const c_bson_minkey& l, const BsonValue& r)
{
	return !l.compare_eq(r);
}

namespace detail
{

template<>
struct bson_operator_outter<c_bson_minkey>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					c_bson_minkey, 
					bson_json_mode_def::mode_type) const
	{
		os << "{ \"$minKey\" : 1 }";
		return os;
	}
};

} // namespace detail

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, c_bson_minkey)
{
	typedef detail::bson_operator_outter<c_bson_minkey> outter_type;

	c_bson_minkey val;
	outter_type outter;
	return outter(os, val, 0);
}

} // namespace nsql_database_system
} // namesapce yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

inline void swap(c_bson_minkey& l, c_bson_minkey& r) 
{
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_MINKEY_HPP__
