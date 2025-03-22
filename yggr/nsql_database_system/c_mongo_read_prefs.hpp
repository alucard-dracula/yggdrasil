//c_mongo_read_prefs.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_READ_PREFS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_READ_PREFS_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/move/move.hpp>

#include <yggr/nsql_database_system/mongoc_read_prefs_native_ex.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/bson_visit_t.hpp>

#include <boost/type_traits/is_base_of.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_read_prefs_t {
//   mongoc_read_mode_t mode;
//   bson_t tags;
//   int64_t max_staleness_seconds;
//   bson_t hedge;
//};

class c_mongo_read_prefs
	: protected yggr_mongoc_read_prefs
{
public:
	typedef yggr_mongoc_read_prefs base_type;
	typedef mongoc_read_prefs_t org_type;

	typedef c_bson bson_type;

	typedef mongoc_read_prefs_native_ex::mode_type mode_type;

	typedef s64 max_staleness_seconds_type;

public:
	YGGR_STATIC_CONSTANT(
		s64, E_MONGOC_NO_MAX_STALENESS = mongoc_read_prefs_native_ex::E_MONGOC_NO_MAX_STALENESS);
	YGGR_STATIC_CONSTANT(
		s64, 
		E_MONGOC_SMALLEST_MAX_STALENESS_SECONDS = mongoc_read_prefs_native_ex::E_MONGOC_SMALLEST_MAX_STALENESS_SECONDS);

public:
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_INVALIDATE = mongoc_read_prefs_native_ex::E_MONGOC_READ_INVALIDATE);
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_PRIMARY = mongoc_read_prefs_native_ex::E_MONGOC_READ_PRIMARY);
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_SECONDARY = mongoc_read_prefs_native_ex::E_MONGOC_READ_SECONDARY);
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_PRIMARY_PREFERRED = mongoc_read_prefs_native_ex::E_MONGOC_READ_PRIMARY_PREFERRED);
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_SECONDARY_PREFERRED = mongoc_read_prefs_native_ex::E_MONGOC_READ_SECONDARY_PREFERRED);
	YGGR_STATIC_CONSTANT(mode_type, E_MONGOC_READ_NEAREST = mongoc_read_prefs_native_ex::E_MONGOC_READ_NEAREST);

private:
	typedef c_mongo_read_prefs this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_read_prefs(void);
	explicit c_mongo_read_prefs(mode_type mode);

	explicit c_mongo_read_prefs(const org_type* read_prefs);
	c_mongo_read_prefs(const org_type& right);

	c_mongo_read_prefs(BOOST_RV_REF(this_type) right)
	{
		mongoc_read_prefs_native_ex::s_mongoc_read_prefs_init(this);
		this_type& right_ref = right;
		mongoc_read_prefs_native_ex::s_mongoc_read_prefs_move(this, &right_ref);
	}

	c_mongo_read_prefs(const this_type& right);
	~c_mongo_read_prefs(void);

public:
	this_type& operator=(const org_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		mongoc_read_prefs_native_ex::s_mongoc_read_prefs_move(this, &right_ref);
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	void swap(org_type&);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void)
	{
		base_type& base = *this;
		return reinterpret_cast<org_type*>(boost::addressof(base));
	}

	inline const org_type* org_pointer(void) const
	{
		const base_type& base = *this;
		return reinterpret_cast<const org_type*>(boost::addressof(base));
	}

public:
	inline operator org_type&(void)
	{
		return *(this_type::org_pointer());
	}

	inline operator const org_type&(void) const
	{
		return *(this_type::org_pointer());
	}

	inline operator org_type*(void)
	{
		return this_type::org_pointer();
	}

	inline operator const org_type*(void) const
	{
		return this_type::org_pointer();
	}

public:
	inline bool validate(void) const
	{
		return mongoc_read_prefs_native_ex::s_mongoc_read_prefs_is_valid(this);
	}

public:
	// mode
	inline mode_type& var_mode(void)
	{
		return base_type::mode;
	}

	inline mode_type var_mode(void) const
	{
		return base_type::mode;
	}

	// tag
	inline bson_type& var_tags(void)
	{
		return *(reinterpret_cast<bson_type*>(boost::addressof(base_type::tags)));
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, Bson&>::type
		var_tags(void)
	{
		typedef Bson now_bson_type;

		now_bson_type* pbs = reinterpret_cast<now_bson_type*>(boost::addressof(base_type::tags));
		assert(pbs);
		return *pbs;
	}

	inline const bson_type& var_tags(void) const
	{
		return *(reinterpret_cast<const bson_type*>(boost::addressof(base_type::tags)));
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		var_tags(void) const
	{
		typedef Bson now_bson_type;

		const now_bson_type* pbs = reinterpret_cast<const now_bson_type*>(boost::addressof(base_type::tags));
		assert(pbs);
		return *pbs;
	}

	// add_tag
	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, void>::type
		add_tag(const Bson& tag)
	{
		mongoc_read_prefs_add_tag(this_type::org_pointer(), boost::addressof(tag));
	}

	// max_staleness_seconds
	inline max_staleness_seconds_type& var_max_staleness_seconds(void)
	{
		return base_type::max_staleness_seconds;
	}

	inline max_staleness_seconds_type var_max_staleness_seconds(void) const
	{
		return base_type::max_staleness_seconds;
	}

	// hedge
	inline bson_type& var_hedge(void)
	{
		return *(reinterpret_cast<bson_type*>(boost::addressof(base_type::hedge)));
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, Bson&>::type
		var_hedge(void)
	{
		typedef Bson now_bson_type;

		now_bson_type* pbs = reinterpret_cast<now_bson_type*>(boost::addressof(base_type::hedge));
		assert(pbs);
		return *pbs;
	}

	inline const bson_type& var_hedge(void) const
	{
		return *(reinterpret_cast<const bson_type*>(boost::addressof(base_type::hedge)));
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		var_hedge(void) const
	{
		typedef Bson now_bson_type;

		const now_bson_type* pbs = reinterpret_cast<const now_bson_type*>(boost::addressof(base_type::hedge));
		assert(pbs);
		return *pbs;
	}
};

namespace detail
{

template<>
struct bson_operator_outter<c_mongo_read_prefs>
{
	template<typename Char, typename Traits> inline
	std::basic_ostream<Char, Traits>&
		operator()(std::basic_ostream<Char, Traits>& os, 
					const c_mongo_read_prefs& val, 
					bson_json_mode_def::mode_type) const
	{
		os << "{ \"mode\" : " << val.var_mode() 
			<< ", \"tags\" : " << val.var_tags() 
			<< ", \"max_staleness_seconds\" : " << val.var_max_staleness_seconds() 
			<< ", \"hedge\" : " << val.var_hedge()
			<< " }";
		return os;
	}
};


} // namespace detail

// non-member function
// io

template<typename Char, typename Traits> inline
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os, const c_mongo_read_prefs& val)
{
	typedef detail::bson_operator_outter<c_mongo_read_prefs> outter_type;

	outter_type outter;
	return outter(os, val, 0);
}

// need test, if ok, remove it
//template<typename Char, typename Traits>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os, BOOST_RV_REF(c_mongo_read_prefs) val)
//{
//	const c_mongo_read_prefs& right_cref = val;
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
	
inline void swap(c_mongo_read_prefs& l, c_mongo_read_prefs& r) 
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

#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_READ_PREFS_HPP__
