//c_mongo_index_model.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONOG_INDEX_MODEL_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONOG_INDEX_MODEL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/mongoc_index_model_native_ex.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/and.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_index_model
	: protected yggr_mongoc_index_model_t
{
public:
	typedef yggr_mongoc_index_model_t base_type;
	typedef mongoc_index_model_t org_type;

	typedef c_bson bson_type;

private:
	typedef c_mongo_index_model this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			bson_type,
			bson_t,
			const bson_type&
		> bson_member_modify_helper_type;

public:
	c_mongo_index_model(void);

	c_mongo_index_model(BOOST_RV_REF(bson_t) k)
	{
		this_type::assign(boost::move(k));
	}

	c_mongo_index_model(const bson_t& k);

	c_mongo_index_model(BOOST_RV_REF(c_bson) k)
	{
		this_type::assign(boost::move(k));
	}

	c_mongo_index_model(const c_bson& k);

	template<typename Bson1, typename Bson2>
	c_mongo_index_model(BOOST_RV_REF(Bson1) k, BOOST_RV_REF(Bson2) o,
						typename 
							boost::enable_if
							<
								boost::mpl::and_
								<
									boost::is_base_of<bson_t, Bson1>,
									boost::is_base_of<bson_t, Bson2>
								>, 
								mplex::sfinae_type
							>::type sfinae = 0)
	{
		this_type::assign(boost::move(k), boost::move(o));
	}

	template<typename Bson1, typename Bson2>
	c_mongo_index_model(BOOST_RV_REF(Bson1) k, const Bson2& o,
						typename 
							boost::enable_if
							<
								boost::mpl::and_
								<
									boost::is_base_of<bson_t, Bson1>,
									boost::is_base_of<bson_t, Bson2>
								>, 
								mplex::sfinae_type
							>::type sfinae = 0)
	{
		this_type::assign(boost::move(k), o);
	}

	template<typename Bson1, typename Bson2>
	c_mongo_index_model(const Bson1& k, BOOST_RV_REF(Bson2) o,
						typename 
							boost::enable_if
							<
								boost::mpl::and_
								<
									boost::is_base_of<bson_t, Bson1>,
									boost::is_base_of<bson_t, Bson2>
								>, 
								mplex::sfinae_type
							>::type sfinae = 0)
	{
		this_type::assign(k, boost::move(o));
	}

	template<typename Bson1, typename Bson2>
	c_mongo_index_model(const Bson1& k, const Bson2& o,
						typename 
							boost::enable_if
							<
								boost::mpl::and_
								<
									boost::is_base_of<bson_t, Bson1>,
									boost::is_base_of<bson_t, Bson2>
								>, 
								mplex::sfinae_type
							>::type sfinae = 0)
	{
		this_type::assign(k, o);
	}

	c_mongo_index_model(const org_type* right);

	c_mongo_index_model(BOOST_RV_REF(this_type) right)
	{
		this_type::assign(boost::move(right));
	}

	c_mongo_index_model(const this_type& right);
	~c_mongo_index_model(void);

public:
	this_type& operator=(const org_type* right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type::assign(boost::move(right));
		return *this;
	}

	this_type& operator=(const this_type& right);

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right);

public:
	inline org_type* org_pointer(void)
	{
		base_type& base_ref = *this;
		return reinterpret_cast<org_type*>(boost::addressof(base_ref));
	}

	inline const org_type* org_pointer(void) const
	{
		const base_type& base_cref = *this;
		return reinterpret_cast<const org_type*>(boost::addressof(base_cref));
	}

public:
	inline operator org_type*(void)
	{
		return this_type::org_pointer();
	}

	inline operator const org_type*(void) const
	{
		return this_type::org_pointer();
	}

public:
	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, void>::type
		assign(BOOST_RV_REF(Bson) k)
	{
		_keys = boost::move(k);
		base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
		base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, void>::type
		assign(const Bson& k)
	{
		_keys = k;
		base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
		base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
	}

	template<typename Bson1, typename Bson2> inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson1>,
				boost::is_base_of<bson_t, Bson2>
			>, 
			void
		>::type
		assign(BOOST_RV_REF(Bson1) k, BOOST_RV_REF(Bson2) o)
	{
		_keys = boost::move(k);
		_opts = boost::move(o);
		base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
		base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
	}

	template<typename Bson1, typename Bson2> inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson1>,
				boost::is_base_of<bson_t, Bson2>
			>, 
			void
		>::type
		assign(BOOST_RV_REF(Bson1) k, const Bson2& o)
	{
		_keys = boost::move(k);
		_opts = o;
		base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
		base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
	}

	template<typename Bson1, typename Bson2> inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson1>,
				boost::is_base_of<bson_t, Bson2>
			>, 
			void
		>::type
		assign(const Bson1& k, BOOST_RV_REF(Bson2) o)
	{
		_keys = k;
		_opts = boost::move(o);
		base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
		base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
	}

	template<typename Bson1, typename Bson2> inline
	typename 
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::is_base_of<bson_t, Bson1>,
				boost::is_base_of<bson_t, Bson2>
			>, 
			void
		>::type
		assign(const Bson1& k, const Bson2& o)
	{
		_keys = k;
		_opts = o;
		base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
		base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
	}

	void assign(const org_type* right);

	inline void assign(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return;
		}

		_keys = boost::move(right._keys);
		_opts = boost::move(right._opts);
		base_type::keys = _keys.empty()? 0 : boost::addressof(_keys);
		base_type::opts = _opts.empty()? 0 : boost::addressof(_opts);
	}

	void assign(const this_type& right);

protected:
	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		pro_get_var_keys(void) const
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<const now_bson_type&>(_keys);
	}

	void pro_set_var_keys(const bson_t& k);

public:
	inline bson_member_modify_helper_type var_keys(void)
	{
		return 
			bson_member_modify_helper_type(
				*this, _keys,
				&this_type::pro_set_var_keys,
				&this_type::pro_get_var_keys<bson_type>);
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>, 
			utility::member_var_modify_helper
			<
				this_type,
				bson_type,
				bson_t,
				const Bson&
			>
		>::type
		var_keys(void)
	{
		typedef Bson now_bson_type;
		typedef 
			utility::member_var_modify_helper
			<
				this_type,
				bson_type,
				bson_t,
				const now_bson_type&
			> now_member_modify_helper_type;

		return 
			now_member_modify_helper_type(
				*this, _keys,
				&this_type::pro_set_var_keys,
				&this_type::pro_get_var_keys<now_bson_type>);
	}

	inline const bson_type& var_keys(void) const
	{
		return _keys;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		var_keys(void) const
	{
		typedef Bson now_bson_type;
		return this_type::pro_get_var_keys<now_bson_type>();
	}

protected:
	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		pro_get_var_opts(void) const
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<const now_bson_type&>(_opts);
	}

	void pro_set_var_opts(const bson_t& o);

public:
	inline bson_member_modify_helper_type var_opts(void)
	{
		return 
			bson_member_modify_helper_type(
				*this, _opts,
				&this_type::pro_set_var_opts,
				&this_type::pro_get_var_opts<bson_type>);
	}

	template<typename Bson> inline
	typename
		boost::enable_if
		<
			boost::is_base_of<bson_t, Bson>, 
			utility::member_var_modify_helper
			<
				this_type,
				bson_type,
				bson_t,
				const Bson&
			>
		>::type
		var_opts(void)
	{
		typedef Bson now_bson_type;

		typedef 
			utility::member_var_modify_helper
			<
				this_type,
				bson_type,
				bson_t,
				const now_bson_type&
			> now_member_modify_helper_type;

		return 
			now_member_modify_helper_type(
				*this, _opts,
				&this_type::pro_set_var_opts,
				&this_type::pro_get_var_opts<now_bson_type>);
	}

	inline const bson_type& var_opts(void) const
	{
		return _opts;
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		var_opts(void) const
	{
		typedef Bson now_bson_type;
		return this_type::pro_get_var_opts<now_bson_type>();
	}

public:
	inline bool empty(void) const
	{
		return !(base_type::keys);
	}

protected:
	void pro_rebuild_base(void);

private:
	bson_type _keys;
	bson_type _opts;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{

	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_index_model)

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

#if defined(BOOST_MOVE_TT_CXX11_IS_COPY_CONSTRUCTIBLE)
namespace boost
{
namespace move_detail
{

template<>
struct is_copy_constructible<yggr::nsql_database_system::c_mongo_index_model>
{
	static const bool value = true;
};

} // namespace modetail
} // namespace boost
#endif // BOOST_MOVE_TT_CXX11_IS_COPY_CONSTRUCTIBLE


#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONOG_INDEX_MODEL_HPP__
