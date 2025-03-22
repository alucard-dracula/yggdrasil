// c_mongo_gridfs_file_opt.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_GRIDFS_FILE_OPT_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_GRIDFS_FILE_OPT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/swap_this_def.hpp>
#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/member_var_modify_helper.hpp>

#include <yggr/charset/utf8_string.hpp>

#include <yggr/nsql_database_system/mongodb_config.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <yggr/nsql_database_system/detail/string_member_var_get.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

namespace yggr
{
namespace nsql_database_system
{

//struct _mongoc_gridfs_file_opt_t
//{
//   const char   *md5;
//   const char   *filename;
//   const char   *content_type;
//   const bson_t *aliases;
//   const bson_t *metadata;
//   uint32_t      chunk_size;
//};

class c_mongo_gridfs_file_opt
	: public mongoc_gridfs_file_opt_t
{
public:
	typedef mongoc_gridfs_file_opt_t base_type;
	typedef base_type org_type;
	typedef yggr::utf8_string inner_string_type;
	typedef yggr::utf8_string_view inner_string_view_type;
	typedef c_bson bson_type;

private:
	typedef c_mongo_gridfs_file_opt this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	typedef
		utility::member_var_modify_helper
		<
			this_type,
			inner_string_type,
			inner_string_type,
			const inner_string_type&
		> inner_string_member_modify_helper_type;

	typedef 
		utility::member_var_modify_helper
		<
			this_type,
			bson_type,
			bson_t,
			const bson_type&
		> bson_member_modify_helper_type;

public:
	c_mongo_gridfs_file_opt(void);

	c_mongo_gridfs_file_opt(const char* fname);

#ifndef YGGR_NO_CWCHAR
	c_mongo_gridfs_file_opt(const wchar_t* fname);
#endif // YGGR_NO_CWCHAR

#ifndef YGGR_NO_CHAR8_T
	c_mongo_gridfs_file_opt(const c8* fname);
#endif // YGGR_NO_CHAR8_T

#ifndef YGGR_NO_CHAR16_T
	c_mongo_gridfs_file_opt(const c16* fname);
#endif // YGGR_NO_CHAR16_T

#ifndef YGGR_NO_CHAR32_T
	c_mongo_gridfs_file_opt(const c32* fname);
#endif // YGGR_NO_CHAR32_T
	
	template<typename String>
	c_mongo_gridfs_file_opt(const String& fname,
							typename
								boost::enable_if
								<
									charset::utf8_string_constructable<String>,
									sfinae_type
								>::type sfinae = 0)
		: _fname(fname)
	{
		base_type& base = *this;
		memset(&base, 0, sizeof(base_type));
		base_type::filename = _fname.empty()? 0 : _fname.data();
	}

	template<typename String1, typename String2, typename String3,
				typename Bson1, typename Bson2 >
	c_mongo_gridfs_file_opt(const String1& str_md5,
							const String2& fname,
							const String3& content,
							const Bson1& aliases, 
							const Bson2& metadata,
							u32 chunk_size,
							typename
								boost::enable_if
								<
									boost::mpl::and_
									<
										boost::mpl::and_
										<
											charset::utf8_string_constructable< String1 >,
											charset::utf8_string_constructable< String2 >,
											charset::utf8_string_constructable< String3 >
										>,
										boost::mpl::and_
										<
											boost::is_base_of< bson_t, Bson1>,
											boost::is_base_of< bson_t, Bson2>
										>
									>,
									sfinae_type
								>::type sfinae = 0)
		: _md5(str_md5), _fname(fname), _content(content), _aliases(aliases), _metadata(metadata)
	{
		base_type& base = *this;
		memset(&base, 0, sizeof(base_type));

		base_type::md5 = _md5.empty()? 0 : _md5.data();
		base_type::filename = _fname.empty()? 0 : _fname.data();
		base_type::content_type = _content.empty()? 0 : _content.data();
		base_type::aliases = _aliases.empty()? 0 : &_aliases;
		base_type::metadata = _metadata.empty()? 0 : &_metadata;

		base_type::chunk_size = chunk_size;
	}

	c_mongo_gridfs_file_opt(const base_type& right)
		: _md5(right.md5? right.md5 : "" ),
			_fname(right.filename? right.filename : "" ),
			_content(right.content_type? right.content_type : "")
	{
		base_type& base = *this;
		memset(&base, 0, sizeof(base_type));

		if(right.aliases)
		{
			_aliases = *right.aliases;
		}

		if(right.metadata)
		{
			_metadata = *right.metadata;
		}

		base_type::md5 = _md5.empty()? 0 : _md5.data();
		base_type::filename = _fname.empty()? 0 : _fname.data();
		base_type::content_type = _content.empty()? 0 : _content.data();
		base_type::aliases = _aliases.empty()? 0 : &_aliases;
		base_type::metadata = _metadata.empty()? 0 : &_metadata;

		base_type::chunk_size = right.chunk_size;
	}

	c_mongo_gridfs_file_opt(BOOST_RV_REF(this_type) right)
		: _md5(boost::move(right._md5)),
			_fname(boost::move(right._fname)),
			_content(boost::move(right._content)),
			_aliases(boost::move(right._aliases)),
			_metadata(boost::move(right._metadata))
	{
		base_type::md5 = _md5.empty()? 0 : _md5.data();
		base_type::filename = _fname.empty()? 0 : _fname.data();
		base_type::content_type = _content.empty()? 0 : _content.data();
		base_type::aliases = _aliases.empty()? 0 : &_aliases;
		base_type::metadata = _metadata.empty()? 0 : &_metadata;

		base_type::chunk_size = right.chunk_size;
	}

	c_mongo_gridfs_file_opt(const this_type& right);
	~c_mongo_gridfs_file_opt(void);

public:
	this_type& operator=(const base_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_md5, boost::move(right._md5));
		copy_or_move_or_swap(_fname, boost::move(right._fname));
		copy_or_move_or_swap(_content, boost::move(right._content));
		copy_or_move_or_swap(_aliases, boost::move(right._aliases));
		copy_or_move_or_swap(_metadata, boost::move(right._metadata));

		base_type::md5 = _md5.empty()? 0 : _md5.data();
		base_type::filename = _fname.empty()? 0 : _fname.data();
		base_type::content_type = _content.empty()? 0 : _content.data();
		base_type::aliases = _aliases.empty()? 0 : &_aliases;
		base_type::metadata = _metadata.empty()? 0 : &_metadata;

		base_type::chunk_size = boost::move(right.chunk_size);

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
		return static_cast<org_type*>(this);
	}

	inline const org_type* org_pointer(void) const
	{
		return static_cast<const org_type*>(this);
	}

protected:
	inline const inner_string_type& pro_get_var_md5(void) const
	{
		return _md5;
	}

	inline void pro_set_var_md5(const inner_string_type& val)
	{
		_md5 = val;
		base_type::md5 = _md5.empty()? 0 : _md5.data();
	}

public:
	inline inner_string_member_modify_helper_type var_md5(void)
	{
		return 
			inner_string_member_modify_helper_type(
				*this, _md5,
				&this_type::pro_set_var_md5,
				&this_type::pro_get_var_md5);
	}

	// inline const string_type& var_md5(void) const;
	
	// template<typename ...Args> inline 
	// string_type& var_md5(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String var_md5(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, pro_get_var_md5(), var_md5, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, pro_get_var_md5(), var_md5, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, pro_get_var_md5(), var_md5)


protected:
	inline const inner_string_type& pro_get_var_file_name(void) const
	{
		return _fname;
	}

	inline void pro_set_var_file_name(const inner_string_type& val)
	{
		_fname = val;
		base_type::filename = _fname.empty()? 0 : _fname.data();
	}

public:
	inline inner_string_member_modify_helper_type var_file_name(void)
	{
		return 
			inner_string_member_modify_helper_type(
				*this, _fname,
				&this_type::pro_set_var_file_name,
				&this_type::pro_get_var_file_name);
	}

	// inline const string_type& var_file_name(void) const;
	
	// template<typename ...Args> inline 
	// string_type& var_file_name(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String var_file_name(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, pro_get_var_file_name(), var_file_name, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, pro_get_var_file_name(), var_file_name, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, pro_get_var_file_name(), var_file_name)

protected:
	inline const inner_string_type& pro_get_var_content(void) const
	{
		return _content;
	}

	inline void pro_set_var_content(const inner_string_type& val)
	{
		_content = val;
		base_type::content_type = _content.empty()? 0 : _content.data();
	}

public:
	inline inner_string_member_modify_helper_type var_content(void)
	{
		return 
			inner_string_member_modify_helper_type(
				*this, _content,
				&this_type::pro_set_var_content,
				&this_type::pro_get_var_content);
	}

	// inline const string_type& var_content(void) const;
	
	// template<typename ...Args> inline 
	// string_type& var_content(const Args&& ...args) const;
	
	// template<typename String, typename ...Args> inline
	// String var_content(const Args&& ...args) const;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, pro_get_var_content(), var_content, __n__ )

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		YGGR_PP_C_MONGO_STRING_MEMBER_GET_SAME(inner_string_type, pro_get_var_content(), var_content, __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, 3 )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	YGGR_PP_C_MONGO_STRING_MEMBER_GET_DIFF(inner_string_type, pro_get_var_content(), var_content)


protected:
	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		pro_get_var_aliases(void) const
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<const now_bson_type&>(_aliases);
	}

	inline void pro_set_var_aliases(const bson_t& val)
	{
		_aliases = val;
		base_type::aliases = _aliases.empty()? 0 : boost::addressof(_aliases);
	}

public:
	inline bson_member_modify_helper_type var_aliases(void)
	{
		return 
			bson_member_modify_helper_type(
				*this, _aliases,
				&this_type::pro_set_var_aliases,
				&this_type::pro_get_var_aliases<bson_type>);
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
		var_aliases(void)
	{
		typedef Bson now_bson_type;
		typedef 
			utility::member_var_modify_helper
			<
				this_type,
				bson_type,
				bson_t,
				const now_bson_type&
			> now_bson_member_modify_helper_type;

		return 
			now_bson_member_modify_helper_type(
				*this, _aliases,
				&this_type::pro_set_var_aliases,
				&this_type::pro_get_var_aliases<now_bson_type>);
	}

	inline const bson_type& var_aliases(void) const
	{
		return this_type::pro_get_var_aliases<bson_type>();
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		var_aliases(void) const
	{
		typedef Bson now_bson_type;
		return this_type::pro_get_var_aliases<now_bson_type>();
	}

protected:
	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		pro_get_var_metadata(void) const
	{
		typedef Bson now_bson_type;
		return reinterpret_cast<const now_bson_type&>(_metadata);
	}

	inline void pro_set_var_metadata(const bson_t& val)
	{
		_metadata = val;
		base_type::metadata = _metadata.empty()? 0 : boost::addressof(_metadata);
	}

public:
	inline bson_member_modify_helper_type var_metadata(void)
	{
		return 
			bson_member_modify_helper_type(
				*this, _metadata,
				&this_type::pro_set_var_metadata,
				&this_type::pro_get_var_metadata<bson_type>);
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
		var_metadata(void)
	{
		typedef Bson now_bson_type;
		typedef 
			utility::member_var_modify_helper
			<
				this_type,
				bson_type,
				bson_t,
				const now_bson_type&
			> now_bson_member_modify_helper_type;

		return 
			now_bson_member_modify_helper_type(
				*this, _metadata,
				&this_type::pro_set_var_metadata,
				&this_type::pro_get_var_metadata<now_bson_type>);
	}

	inline const bson_type& var_metadata(void) const
	{
		return this_type::pro_get_var_metadata<bson_type>();
	}

	template<typename Bson> inline
	typename boost::enable_if<boost::is_base_of<bson_t, Bson>, const Bson&>::type
		var_metadata(void) const
	{
		typedef Bson now_bson_type;
		return this_type::pro_get_var_metadata<now_bson_type>();
	}

	// var_chunk_size
	inline uint32_t& var_chunk_size(void)
	{
		return base_type::chunk_size;
	}

	inline uint32_t var_chunk_size(void) const
	{
		return base_type::chunk_size;
	}

private:
	inner_string_type _md5;
	inner_string_type _fname;
	inner_string_type _content;
	bson_type _aliases;
	bson_type _metadata;
};

} // namespace nsql_database_system
} // namespace yggr

namespace yggr
{
namespace nsql_database_system
{
namespace swap_support
{
	YGGR_PP_INLINE_SWAP_THIS_REF_DEF_IMPL(c_mongo_gridfs_file_opt)
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


#endif // __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_GRIDFS_FILE_OPT_HPP__
