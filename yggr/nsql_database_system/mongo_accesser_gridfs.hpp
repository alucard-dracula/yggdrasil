//mongo_accesser_gridfs.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_GRIDFS_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_GRIDFS_HPP__

#include <yggr/nsql_database_system/basic_mongo_accesser_gridfs.hpp>
#include <yggr/nsql_database_system/detail/to_pointer.hpp>

namespace yggr
{
namespace nsql_database_system
{

class mongo_accesser_gridfs
	: public basic_mongo_accesser_gridfs
{
public:
	typedef basic_mongo_accesser_gridfs base_type;

	typedef base_type::bson_type bson_type;
	typedef base_type::org_bson_type org_bson_type;

	typedef base_type::bson_value_type bson_value_type;
	typedef base_type::org_bson_value_type org_bson_value_type;

	typedef base_type::bson_error_type bson_error_type;
	typedef base_type::org_bson_error_type org_bson_error_type;

	typedef base_type::write_concern_type write_concern_type;
	typedef base_type::org_write_concern_type org_write_concern_type;

	typedef base_type::read_prefs_type read_prefs_type;
	typedef base_type::org_read_prefs_type org_read_prefs_type;

	typedef base_type::index_model_type index_model_type;
	typedef base_type::org_index_model_type org_index_model_type;

	typedef base_type::find_and_modify_opts_type find_and_modify_opts_type;
	typedef base_type::org_find_and_modify_opts_type org_find_and_modify_opts_type;

	typedef base_type::read_concern_type read_concern_type;
	typedef base_type::org_read_concern_type org_read_concern_type;

	typedef base_type::mongoc_error_domain_type mongoc_error_domain_type;
	typedef base_type::mongoc_error_code_type mongoc_error_code_type;

public:
	typedef base_type::gridfs_file_opt_type gridfs_file_opt_type;
	typedef base_type::org_gridfs_file_opt_type org_gridfs_file_opt_type;

	typedef base_type::gridfs_file_type gridfs_file_type;
	typedef base_type::org_gridfs_file_type org_gridfs_file_type;

private:
	typedef mongo_accesser_gridfs this_type;

public:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// don't coding get_param or set_params function, because that is call time used only,
	// example get_read_concern(...)
	// if you want to use that, please call foo "using_handler" execute you custom native operations
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//------------------------gridfs s----------------------------

	// save
private:
	template<typename Connection_T,
				typename String1, typename String2, typename String3> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_save_file_wrap(Connection_T& conn,
								const String1& str_db,
								const String2& str_gridfs,
								const String3& str_fpath,
								s32 flag, s32 mode,
								org_gridfs_file_opt_type* popt,
								org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));
		utf8_string2_type utf8_str_fpath((charset::string_charset_helper_data(str_fpath)));

		return
			base_type::s_org_gridfs_save_file(
				conn,
				utf8_str_db.data(),
				utf8_str_gridfs.data(),
#		if  defined(YGGR_AT_WINDOWS)
				utf8_str_fpath.template str<inner_string_type::base_type>().data(),
#		else
				utf8_str_fpath.data(),
#		endif // YGGR_AT_WINDOWS
				flag, mode,
				popt, perr );
	}

public:
	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_save_file(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const String3& str_fpath,
							s32 flag, s32 mode,
							Gridfs_File_Opt& opt,
							org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_save_file_wrap(
				conn, str_db, str_gridfs, str_fpath, flag, mode,
				reinterpret_cast<org_gridfs_file_opt_type*>(&opt), &err);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_save_file(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const String3& str_fpath,
							s32 flag, s32 mode,
							Gridfs_File_Opt* opt,
							org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_save_file_wrap(
				conn, str_db, str_gridfs, str_fpath, flag, mode,
				reinterpret_cast<org_gridfs_file_opt_type*>(opt), &err);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3 > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_save_file(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const String3& str_fpath,
							s32 flag, s32 mode,
							org_gridfs_file_opt_type* opt,
							org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_save_file_wrap(
				conn, str_db, str_gridfs, str_fpath, flag, mode, opt, &err);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_save_file(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const String3& str_fpath,
							s32 flag, s32 mode,
							Gridfs_File_Opt& opt,
							org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_save_file_wrap(
				conn, str_db, str_gridfs, str_fpath, flag, mode,
				reinterpret_cast<org_gridfs_file_opt_type*>(&opt), err);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_save_file(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const String3& str_fpath,
							s32 flag, s32 mode,
							Gridfs_File_Opt* opt,
							org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_save_file_wrap(
				conn, str_db, str_gridfs, str_fpath, flag, mode,
				reinterpret_cast<org_gridfs_file_opt_type*>(opt), err);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3 > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_save_file(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const String3& str_fpath,
							s32 flag, s32 mode,
							org_gridfs_file_opt_type* opt,
							org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_save_file_wrap(
				conn, str_db, str_gridfs, str_fpath, flag, mode, opt, err);
	}

	// save_buffer
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer_wrap(Connection_T& conn,
											const String1& str_db,
											const String2& str_gridfs,
											u32 tmout_msec,
											const u8* buffer, u64 len,
											org_gridfs_file_opt_type* popt,
											org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));

		return
			base_type::s_org_gridfs_save_file_of_buffer(
				conn, utf8_str_db.data(), utf8_str_gridfs.data(),
				tmout_msec, buffer, len, popt, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const void* buffer, u64 len,
										Gridfs_File_Opt& opt,
										org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				reinterpret_cast<const u8*>(buffer), len,
				reinterpret_cast<org_gridfs_file_opt_type*>(&opt),
				&err);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const void* buffer, u64 len,
										Gridfs_File_Opt* opt,
										org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				reinterpret_cast<const u8*>(buffer), len,
				reinterpret_cast<org_gridfs_file_opt_type*>(opt),
				&err);
	}

	template<typename Connection_T,
				typename String1, typename String2 > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const void* buffer, u64 len,
										org_gridfs_file_opt_type* opt,
										org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				reinterpret_cast<const u8*>(buffer), len,
				opt, &err);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const void* buffer, u64 len,
										Gridfs_File_Opt& opt,
										org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				reinterpret_cast<const u8*>(buffer), len,
				reinterpret_cast<org_gridfs_file_opt_type*>(&opt),
				err);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const void* buffer, u64 len,
										Gridfs_File_Opt* opt,
										org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				reinterpret_cast<const u8*>(buffer), len,
				reinterpret_cast<org_gridfs_file_opt_type*>(opt),
				err);
	}

	template<typename Connection_T,
				typename String1, typename String2 > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const void* buffer, u64 len,
										org_gridfs_file_opt_type* opt,
										org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				reinterpret_cast<const u8*>(buffer), len,
				opt, err);
	}

private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer_wrap(Connection_T& conn,
											const String1& str_db,
											const String2& str_gridfs,
											u32 tmout_msec,
											const Buffer& buffer,
											org_gridfs_file_opt_type* popt,
											org_bson_error_type* perr)
	{
		typedef Buffer buffer_type;
		typedef typename boost::range_value<buffer_type>::type buffer_value_type;
		typedef typename boost::range_iterator<buffer_type>::type buffer_iter_type;

		BOOST_MPL_ASSERT_NOT((boost::is_class<buffer_value_type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<buffer_iter_type>));

		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));

		u64 len = bytes::byte_size(buffer);

		if(!len)
		{
			return false;
		}

		const u8* pbuf = reinterpret_cast<const u8*>(boost::addressof(*boost::begin(buffer)));

		return
			base_type::s_org_gridfs_save_file_of_buffer(
				conn, utf8_str_db.data(), utf8_str_gridfs.data(),
				tmout_msec, pbuf, len, popt, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const Buffer& buffer,
										Gridfs_File_Opt& opt,
										org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				buffer,
				reinterpret_cast<org_gridfs_file_opt_type*>(&opt),
				&err);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const Buffer& buffer,
										Gridfs_File_Opt* opt,
										org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				buffer,
				reinterpret_cast<org_gridfs_file_opt_type*>(opt),
				&err);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const Buffer& buffer,
										org_gridfs_file_opt_type* opt,
										org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				buffer,
				opt, &err);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer,
				typename Gridfs_File_Opt> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const Buffer& buffer,
										Gridfs_File_Opt& opt,
										org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				buffer,
				reinterpret_cast<org_gridfs_file_opt_type*>(&opt),
				err);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer,
				typename Gridfs_File_Opt > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const Buffer& buffer,
										Gridfs_File_Opt* opt,
										org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				buffer,
				reinterpret_cast<org_gridfs_file_opt_type*>(opt),
				err);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_save_file_of_buffer(Connection_T& conn,
										const String1& str_db,
										const String2& str_gridfs,
										u32 tmout_msec,
										const Buffer& buffer,
										org_gridfs_file_opt_type* opt,
										org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_save_file_of_buffer_wrap(
				conn, str_db, str_gridfs,
				tmout_msec,
				buffer,
				opt, err);
	}

	// load
private:
	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename Buffer> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			u64
		>::type
		s_gridfs_load_file_wrap(Connection_T& conn,
								const String1& str_db,
								const String2& str_gridfs,
								const String3& str_fname,
								s32 tmout_msec,
								Buffer& out_buf,
								org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));
		utf8_string2_type utf8_str_fname((charset::string_charset_helper_data(str_fname)));

		return
			base_type::s_org_gridfs_load_file(
				conn,
				utf8_str_db.data(),
				utf8_str_gridfs.data(),
				utf8_str_fname.data(),
				tmout_msec, out_buf, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename Buffer> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			u64
		>::type
		s_gridfs_load_file(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const String3& str_fname,
							s32 tmout_msec,
							Buffer& out_buf,
							org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_load_file_wrap(
				conn, str_db, str_gridfs, str_fname,
				tmout_msec, out_buf, err);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename Buffer> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			u64
		>::type
		s_gridfs_load_file(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const String3& str_fname,
							s32 tmout_msec,
							Buffer& out_buf,
							org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_load_file_wrap(
				conn, str_db, str_gridfs, str_fname,
				tmout_msec, out_buf, boost::addressof(err));
	}


	// gridfs_find
private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_find_wrap(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const org_bson_type* pquery,
							org_bson_error_type* perr,
							const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));

		return
			base_type::s_org_gridfs_find(
				conn, utf8_str_db.data(), utf8_str_gridfs.data(),
				pquery, perr, handler_collector);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Bson,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_find(Connection_T& conn,
						const String1& str_db,
						const String2& str_gridfs,
						const Bson& query,
						org_bson_error_type& err,
						const HandlerCollector& handler_collector)
	{
		const org_bson_type* pquery = detail::to_const_pointer<org_bson_type>(query);

		return
			this_type::s_gridfs_find_wrap(
				conn, str_db, str_gridfs,
				pquery, &err, handler_collector);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Bson,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_find(Connection_T& conn,
						const String1& str_db,
						const String2& str_gridfs,
						const Bson& query,
						org_bson_error_type* err,
						const HandlerCollector& handler_collector)
	{
		const org_bson_type* pquery = detail::to_const_pointer<org_bson_type>(query);

		return
			this_type::s_gridfs_find_wrap(
				conn, str_db, str_gridfs,
				pquery, err, handler_collector);
	}

	// gridfs_find_one
private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_find_one_wrap(Connection_T& conn,
								const String1& str_db,
								const String2& str_gridfs,
								const org_bson_type* pquery,
								org_bson_error_type* perr,
								const Handler& handler)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));

		return
			base_type::s_org_gridfs_find_one(
				conn, utf8_str_db.data(), utf8_str_gridfs.data(),
				pquery, perr, handler);
	}

	//gridfs_find_one_by_filename
private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename String3,
				typename Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_find_one_by_filename_wrap(Connection_T& conn,
											const String1& str_db,
											const String2& str_gridfs,
											const String3& str_fname,
											org_bson_error_type* perr,
											const Handler& handler)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));
		utf8_string2_type utf8_str_fname((charset::string_charset_helper_data(str_fname)));

		return
			base_type::s_org_gridfs_find_one_by_filename(
				conn, utf8_str_db.data(), utf8_str_gridfs.data(),
				utf8_str_fname.data(), perr, handler);
	}

	// gridfs_find_one_helper
public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonQuery,
				typename Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				boost::is_base_of
				<
					org_bson_type,
					typename native_t<BsonQuery>::type
				>
			>,
			bool
		>::type
		s_gridfs_find_one(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const BsonQuery& query,
							org_bson_error_type* err,
							const Handler& handler)
	{
		return
			this_type::s_gridfs_find_one_wrap(
				conn,
				str_db,
				str_gridfs,
				detail::to_const_pointer<org_bson_type>(query),
				err,
				handler);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonQuery,
				typename Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				boost::is_base_of
				<
					org_bson_type,
					typename native_t<BsonQuery>::type
				>
			>,
			bool
		>::type
		s_gridfs_find_one(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const BsonQuery& query,
							org_bson_error_type& err,
							const Handler& handler)
	{
		return
			this_type::s_gridfs_find_one_wrap(
				conn,
				str_db,
				str_gridfs,
				detail::to_const_pointer<org_bson_type>(query),
				boost::addressof(err),
				handler);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename StrFileName,
				typename Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<StrFileName>
			>,
			bool
		>::type
		s_gridfs_find_one(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const StrFileName& str_fname,
							org_bson_error_type* err,
							const Handler& handler)
	{
		return
			this_type::s_gridfs_find_one_by_filename_wrap(
				conn,
				str_db,
				str_gridfs,
				str_fname,
				err,
				handler);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename StrFileName,
				typename Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<StrFileName>
			>,
			bool
		>::type
		s_gridfs_find_one(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							const StrFileName& str_fname,
							org_bson_error_type& err,
							const Handler& handler)
	{
		return
			this_type::s_gridfs_find_one_by_filename_wrap(
				conn,
				str_db,
				str_gridfs,
				str_fname,
				boost::addressof(err),
				handler);
	}

	// gridfs_chunks_find
private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_chunks_find_wrap(Connection_T& conn,
									const String1& str_db,
									const String2& str_gridfs,
									u32 flags, u32 skip, u32 limit, u32 batch_size,
									const org_bson_type* pquery,
									const org_bson_type* pfields,
									const org_read_prefs_type* pread_prefs,
									org_bson_error_type* perr,
									const HandlerCollector& handler)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));

		return
			base_type::s_org_gridfs_chunks_find(
				conn, utf8_str_db.data(), utf8_str_gridfs.data(),
				flags, skip, limit, batch_size,
				pquery, pfields, pread_prefs, perr, handler);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Bson1, typename Bson2,
				typename ReadPrefs,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_chunks_find(Connection_T& conn,
								const String1& str_db,
								const String2& str_gridfs,
								u32 flags, u32 skip, u32 limit, u32 batch_size,
								const Bson1& query,
								const Bson2& fields,
								const ReadPrefs& read_prefs,
								org_bson_error_type& err,
								const HandlerCollector& handler)
	{
		const org_bson_type* pquery = detail::to_const_pointer<org_bson_type>(query);
		const org_bson_type* pfields = detail::to_const_pointer<org_bson_type>(fields);
		const org_read_prefs_type* pread_prefs = detail::to_const_pointer<org_read_prefs_type>(read_prefs);

		return
			this_type::s_gridfs_chunks_find_wrap(
				conn, str_db.data(), str_gridfs.data(),
				flags, skip, limit, batch_size,
				pquery, pfields, pread_prefs, &err, handler);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Bson1, typename Bson2,
				typename ReadPrefs,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_chunks_find(Connection_T& conn,
								const String1& str_db,
								const String2& str_gridfs,
								u32 flags, u32 skip, u32 limit, u32 batch_size,
								const Bson1& query,
								const Bson2& fields,
								const ReadPrefs& read_prefs,
								org_bson_error_type* err,
								const HandlerCollector& handler)
	{
		const org_bson_type* pquery = detail::to_const_pointer<org_bson_type>(query);
		const org_bson_type* pfields = detail::to_const_pointer<org_bson_type>(fields);
		const org_read_prefs_type* pread_prefs = detail::to_const_pointer<org_read_prefs_type>(read_prefs);

		return
			this_type::s_gridfs_chunks_find_wrap(
				conn, str_db.data(), str_gridfs.data(),
				flags, skip, limit, batch_size,
				pquery, pfields, pread_prefs, err, handler);
	}

	// gridfs_files_find
private:

	template<typename Connection_T,
				typename String1, typename String2,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_files_find_wrap(Connection_T& conn,
									const String1& str_db,
									const String2& str_gridfs,
									u32 flags, u32 skip, u32 limit, u32 batch_size,
									const org_bson_type* pquery,
									const org_bson_type* pfields,
									const org_read_prefs_type* pread_prefs,
									org_bson_error_type* perr,
									const HandlerCollector& handler)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));

		return
			base_type::s_org_gridfs_files_find(
				conn, utf8_str_db.data(), utf8_str_gridfs.data(),
				flags, skip, limit, batch_size,
				pquery, pfields, pread_prefs, perr, handler);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Bson1, typename Bson2,
				typename ReadPrefs,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_files_find(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							u32 flags, u32 skip, u32 limit, u32 batch_size,
							const Bson1& query,
							const Bson2& fields,
							const ReadPrefs& read_prefs,
							org_bson_error_type& err,
							const HandlerCollector& handler)
	{
		const org_bson_type* pquery = detail::to_const_pointer<org_bson_type>(query);
		const org_bson_type* pfields = detail::to_const_pointer<org_bson_type>(fields);
		const org_read_prefs_type* pread_prefs = detail::to_const_pointer<org_read_prefs_type>(read_prefs);

		return
			this_type::s_gridfs_files_find_wrap(
				conn, str_db, str_gridfs,
				flags, skip, limit, batch_size,
				pquery, pfields, pread_prefs, &err, handler);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Bson1, typename Bson2,
				typename ReadPrefs,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_files_find(Connection_T& conn,
							const String1& str_db,
							const String2& str_gridfs,
							u32 flags, u32 skip, u32 limit, u32 batch_size,
							const Bson1& query,
							const Bson2& fields,
							const ReadPrefs* read_prefs,
							org_bson_error_type* err,
							const HandlerCollector& handler)
	{
		const org_bson_type* pquery = detail::to_const_pointer<org_bson_type>(query);
		const org_bson_type* pfields = detail::to_const_pointer<org_bson_type>(fields);
		const org_read_prefs_type* pread_prefs = detail::to_const_pointer<org_read_prefs_type>(read_prefs);

		return
			this_type::s_gridfs_files_find_wrap(
				conn, str_db, str_gridfs,
				flags, skip, limit, batch_size,
				pquery, pfields, pread_prefs, err, handler);
	}

private:
	template<typename Connection_T,
				typename String1, typename String2, typename String3 > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_remove_file_wrap(Connection_T& conn,
									const String1& str_db,
									const String2& str_gridfs,
									const String3& str_fname,
									org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));
		utf8_string2_type utf8_str_fname((charset::string_charset_helper_data(str_fname)));


		return
			base_type::s_org_gridfs_remove_file(
				conn, utf8_str_db.data(), utf8_str_gridfs.data(),
				utf8_str_fname.data(), perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename String3 > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_remove_file(Connection_T& conn,
								const String1& str_db,
								const String2& str_gridfs,
								const String3& str_fname,
								org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_remove_file_wrap(
				conn, str_db, str_gridfs, str_fname, &err);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename String3 > inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_remove_file(Connection_T& conn,
								const String1& str_db,
								const String2& str_gridfs,
								const String3& str_fname,
								org_bson_error_type* err)
	{
		return
			this_type::s_gridfs_remove_file_wrap(
				conn, str_db, str_gridfs, str_fname, err);
	}

	// gridfs_files_begin
private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_files_begin_wrap(Connection_T& conn,
									const String1& str_db,
									const String2& str_gridfs,
									const org_bson_type* pquery,
									org_bson_error_type* perr,
									const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_gridfs((charset::string_charset_helper_data(str_gridfs)));

		return
			base_type::s_org_gridfs_files_begin(
				conn, utf8_str_db.data(), utf8_str_gridfs.data(),
				pquery, perr, handler_collector);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Bson,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_files_begin(Connection_T& conn,
								const String1& str_db,
								const String2& str_gridfs,
								const Bson& query,
								org_bson_error_type& err,
								const HandlerCollector& handler_collector)
	{
		const org_bson_type* pquery = detail::to_const_pointer<org_bson_type>(query);

		return
			this_type::s_gridfs_files_begin_wrap(
				conn, str_db, str_gridfs, pquery, &err, handler_collector);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Bson,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_files_begin(Connection_T& conn,
								const String1& str_db,
								const String2& str_gridfs,
								const Bson& query,
								org_bson_error_type* err,
								const HandlerCollector& handler_collector)
	{
		const org_bson_type* pquery = detail::to_const_pointer<org_bson_type>(query);

		return
			this_type::s_gridfs_files_begin_wrap(
				conn, str_db, str_gridfs, pquery, err, handler_collector);
	}

	// gridfs_files_end
public:
	template<typename Connection_T, typename Files_Container > inline
	static bool s_gridfs_files_end(Connection_T& conn,
										Files_Container& files_cont,
										org_bson_error_type& err)
	{
		return base_type::s_org_gridfs_files_end(conn, files_cont, &err);
	}

	template<typename Connection_T, typename Files_Container > inline
	static bool s_gridfs_files_end(Connection_T& conn,
										Files_Container& files_cont,
										org_bson_error_type* perr = 0)
	{
		return base_type::s_org_gridfs_files_end(conn, files_cont, perr);
	}

	template<typename Connection_T, typename Files_Container, typename Gridfs_File_Getter > inline
	static bool s_gridfs_files_end(Connection_T& conn,
										Files_Container& files_cont,
										org_bson_error_type& err,
										const Gridfs_File_Getter& file_getter)
	{
		return base_type::s_org_gridfs_files_end(conn, files_cont, &err, file_getter);
	}

	template<typename Connection_T, typename Files_Container, typename Gridfs_File_Getter > inline
	static bool s_gridfs_files_end(Connection_T& conn,
										Files_Container& files_cont,
										org_bson_error_type* err,
										const Gridfs_File_Getter& file_getter)
	{
		return base_type::s_org_gridfs_files_end(conn, files_cont, err, file_getter);
	}

	//------------------------gridfs e----------------------------

	//------------------------gridfs_bucket s----------------------------

	// save
private:
	template<typename Connection_T,
				typename String1, typename String2, typename String3> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_wrap(Connection_T& conn,
										const String1& str_db,
										const String2& str_fname,
										const String3& str_fpath,
										s32 flag, s32 mode,
										const org_bson_type* pbucket_opts,
										const org_read_prefs_type* pbucket_read_prefs,
										const org_bson_type* pstream_opts,
										org_bson_value_type* pfile_id,
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_fname((charset::string_charset_helper_data(str_fname)));
		utf8_string2_type utf8_str_fpath(charset::string_charset_helper_data(str_fpath));

		return
			base_type::s_org_gridfs_bucket_save_file(
				conn,
				utf8_str_db.data(),
				utf8_str_fname.data(),
#		if  defined(YGGR_AT_WINDOWS)
				utf8_str_fpath.template str<inner_string_type::base_type>().data(),
#		else
				utf8_str_fpath.data(),
#		endif // YGGR_AT_WINDOWS
				flag, mode,
				pbucket_opts, pbucket_read_prefs,
				pstream_opts, pfile_id, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file(Connection_T& conn,
									const String1& str_db,
									const String2& str_fname,
									const String3& str_fpath,
									s32 flag, s32 mode,
									const BsonBucketOpts& bucket_opts,
									const ReadPrefs& bucket_read_prefs,
									const BsonStreamOpts& stream_opts,
									org_bson_value_type* pfile_id,
									org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_save_file_wrap(
				conn, str_db, str_fname, str_fpath,
				flag, mode,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				pfile_id,
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file(Connection_T& conn,
									const String1& str_db,
									const String2& str_fname,
									const String3& str_fpath,
									s32 flag, s32 mode,
									const BsonBucketOpts& bucket_opts,
									const ReadPrefs& bucket_read_prefs,
									const BsonStreamOpts& stream_opts,
									BsonValueFileID& file_id,
									org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_save_file_wrap(
				conn, str_db, str_fname, str_fpath,
				flag, mode,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				boost::addressof(file_id),
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file(Connection_T& conn,
									const String1& str_db,
									const String2& str_fname,
									const String3& str_fpath,
									s32 flag, s32 mode,
									const BsonBucketOpts& bucket_opts,
									const ReadPrefs& bucket_read_prefs,
									const BsonStreamOpts& stream_opts,
									org_bson_value_type* pfile_id,
									org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_save_file_wrap(
				conn, str_db, str_fname, str_fpath,
				flag, mode,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				pfile_id,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file(Connection_T& conn,
									const String1& str_db,
									const String2& str_fname,
									const String3& str_fpath,
									s32 flag, s32 mode,
									const BsonBucketOpts& bucket_opts,
									const ReadPrefs& bucket_read_prefs,
									const BsonStreamOpts& stream_opts,
									BsonValueFileID& file_id,
									org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_save_file_wrap(
				conn, str_db, str_fname, str_fpath,
				flag, mode,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				boost::addressof(file_id),
				boost::addressof(err));
	}

// save with_id
private:
	template<typename Connection_T,
				typename String1, typename String2, typename String3> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_with_id_wrap(Connection_T& conn,
												const String1& str_db,
												const String2& str_fname,
												const String3& str_fpath,
												s32 flag, s32 mode,
												const org_bson_type* pbucket_opts,
												const org_read_prefs_type* pbucket_read_prefs,
												const org_bson_type* pstream_opts,
												const org_bson_value_type* pfile_id,
												org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;
		typedef typename native_t<String3>::type string3_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string3_type, utf8_string3_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_fname((charset::string_charset_helper_data(str_fname)));
		utf8_string2_type utf8_str_fpath(charset::string_charset_helper_data(str_fpath));

		return
			base_type::s_org_gridfs_bucket_save_file_with_id(
				conn,
				utf8_str_db.data(),
				utf8_str_fname.data(),
#		if  defined(YGGR_AT_WINDOWS)
				utf8_str_fpath.template str<inner_string_type::base_type>().data(),
#		else
				utf8_str_fpath.data(),
#		endif // YGGR_AT_WINDOWS
				flag, mode,
				pbucket_opts, pbucket_read_prefs,
				pstream_opts, pfile_id, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_with_id(Connection_T& conn,
											const String1& str_db,
											const String2& str_fname,
											const String3& str_fpath,
											s32 flag, s32 mode,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonStreamOpts& stream_opts,
											const BsonValueFileID& file_id,
											org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_save_file_with_id_wrap(
				conn, str_db, str_fname, str_fpath,
				flag, mode,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				detail::to_const_pointer<org_bson_value_type>(file_id),
				perr);
	}


	template<typename Connection_T,
				typename String1, typename String2, typename String3,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>,
				charset::utf8_string_constructable<String3>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_with_id(Connection_T& conn,
											const String1& str_db,
											const String2& str_fname,
											const String3& str_fpath,
											s32 flag, s32 mode,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonStreamOpts& stream_opts,
											const BsonValueFileID& file_id,
											org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_save_file_with_id_wrap(
				conn, str_db, str_fname, str_fpath,
				flag, mode,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				detail::to_const_pointer<org_bson_value_type>(file_id),
				boost::addressof(err));
	}

	// save_buffer
private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer_wrap(Connection_T& conn,
													const String1& str_db,
													const String2& str_fname,
													u32 tmout_msec, const u8* buffer, u64 len,
													const org_bson_type* pbucket_opts,
													const org_read_prefs_type* pbucket_read_prefs,
													const org_bson_type* pstream_opts,
													org_bson_value_type* pfile_id,
													org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_fname((charset::string_charset_helper_data(str_fname)));

		return
			base_type::s_org_gridfs_bucket_save_file_of_buffer(
				conn,
				utf8_str_db.data(),
				utf8_str_fname.data(),
				tmout_msec, buffer, len,
				pbucket_opts, pbucket_read_prefs,
				pstream_opts, pfile_id, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer(Connection_T& conn,
											const String1& str_db,
											const String2& str_fname,
											u32 tmout_msec, const void* buffer, u64 len,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonStreamOpts& stream_opts,
											org_bson_value_type* pfile_id,
											org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_wrap(
				conn, str_db, str_fname,
				tmout_msec, reinterpret_cast<const u8*>(buffer), len,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				pfile_id,
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer(Connection_T& conn,
											const String1& str_db,
											const String2& str_fname,
											u32 tmout_msec, const void* buffer, u64 len,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonStreamOpts& stream_opts,
											BsonValueFileID& file_id,
											org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_wrap(
				conn, str_db, str_fname,
				tmout_msec, reinterpret_cast<const u8*>(buffer), len,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				boost::addressof(file_id),
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer(Connection_T& conn,
											const String1& str_db,
											const String2& str_fname,
											u32 tmout_msec, const void* buffer, u64 len,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonStreamOpts& stream_opts,
											org_bson_value_type* pfile_id,
											org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_wrap(
				conn, str_db, str_fname,
				tmout_msec, reinterpret_cast<const u8*>(buffer), len,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				pfile_id,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer(Connection_T& conn,
											const String1& str_db,
											const String2& str_fname,
											u32 tmout_msec, const void* buffer, u64 len,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonStreamOpts& stream_opts,
											BsonValueFileID& file_id,
											org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_wrap(
				conn, str_db, str_fname,
				tmout_msec, reinterpret_cast<const u8*>(buffer), len,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				boost::addressof(file_id),
				boost::addressof(err));
	}

private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer_wrap(Connection_T& conn,
													const String1& str_db,
													const String2& str_fname,
													u32 tmout_msec,
													const Buffer& buffer,
													const org_bson_type* pbucket_opts,
													const org_read_prefs_type* pbucket_read_prefs,
													const org_bson_type* pstream_opts,
													org_bson_value_type* pfile_id,
													org_bson_error_type* perr)
	{
		typedef Buffer buffer_type;
		typedef typename boost::range_value<buffer_type>::type buffer_value_type;
		typedef typename boost::range_iterator<buffer_type>::type buffer_iter_type;

		BOOST_MPL_ASSERT_NOT((boost::is_class<buffer_value_type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<buffer_iter_type>));

		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_fname((charset::string_charset_helper_data(str_fname)));

		u64 len = bytes::byte_size(buffer);

		if(!len)
		{
			return false;
		}

		const u8* pbuf = reinterpret_cast<const u8*>(boost::addressof(*boost::begin(buffer)));

		return
			base_type::s_org_gridfs_bucket_save_file_of_buffer(
				conn, utf8_str_db.data(), utf8_str_fname.data(),
				tmout_msec, pbuf, len,
				pbucket_opts, pbucket_read_prefs,
				pstream_opts, pfile_id, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer(Connection_T& conn,
											const String1& str_db,
											const String2& str_fname,
											u32 tmout_msec,
											const Buffer& buffer,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonStreamOpts& stream_opts,
											org_bson_value_type* pfile_id,
											org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_wrap(
				conn, str_db, str_fname,
				tmout_msec, buffer,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				pfile_id,
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer(Connection_T& conn,
											const String1& str_db,
											const String2& str_fname,
											u32 tmout_msec, const Buffer& buffer,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonStreamOpts& stream_opts,
											BsonValueFileID& file_id,
											org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_wrap(
				conn, str_db, str_fname,
				tmout_msec, buffer,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				boost::addressof(file_id),
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer(Connection_T& conn,
											const String1& str_db,
											const String2& str_fname,
											u32 tmout_msec, const Buffer& buffer,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonStreamOpts& stream_opts,
											org_bson_value_type* pfile_id,
											org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_wrap(
				conn, str_db, str_fname,
				tmout_msec, buffer,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				pfile_id,
				boost::addressof(err));
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer(Connection_T& conn,
											const String1& str_db,
											const String2& str_fname,
											u32 tmout_msec, const Buffer& buffer,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonStreamOpts& stream_opts,
											BsonValueFileID& file_id,
											org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_wrap(
				conn, str_db, str_fname,
				tmout_msec, buffer,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				boost::addressof(file_id),
				boost::addressof(err));
	}

private:
	template<typename Connection_T,
				typename String1, typename String2> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer_with_id_wrap(Connection_T& conn,
															const String1& str_db,
															const String2& str_fname,
															u32 tmout_msec, const u8* buffer, u64 len,
															const org_bson_type* pbucket_opts,
															const org_read_prefs_type* pbucket_read_prefs,
															const org_bson_type* pstream_opts,
															const org_bson_value_type* pfile_id,
															org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_fname((charset::string_charset_helper_data(str_fname)));

		return
			base_type::s_org_gridfs_bucket_save_file_of_buffer_with_id(
				conn,
				utf8_str_db.data(),
				utf8_str_fname.data(),
				tmout_msec, buffer, len,
				pbucket_opts, pbucket_read_prefs,
				pstream_opts, pfile_id, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer_with_id(Connection_T& conn,
													const String1& str_db,
													const String2& str_fname,
													u32 tmout_msec, const void* buffer, u64 len,
													const BsonBucketOpts& bucket_opts,
													const ReadPrefs& bucket_read_prefs,
													const BsonStreamOpts& stream_opts,
													const BsonValueFileID& file_id,
													org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_with_id_wrap(
				conn, str_db, str_fname,
				tmout_msec, reinterpret_cast<const u8*>(buffer), len,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				detail::to_const_pointer<org_bson_value_type>(file_id),
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer_with_id(Connection_T& conn,
													const String1& str_db,
													const String2& str_fname,
													u32 tmout_msec, const void* buffer, u64 len,
													const BsonBucketOpts& bucket_opts,
													const ReadPrefs& bucket_read_prefs,
													const BsonStreamOpts& stream_opts,
													const BsonValueFileID& file_id,
													org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_with_id_wrap(
				conn, str_db, str_fname,
				tmout_msec, reinterpret_cast<const u8*>(buffer), len,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				detail::to_const_pointer<org_bson_value_type>(file_id),
				boost::addressof(err));
	}

private:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer_with_id_wrap(Connection_T& conn,
															const String1& str_db,
															const String2& str_fname,
															u32 tmout_msec, const Buffer& buffer,
															const org_bson_type* pbucket_opts,
															const org_read_prefs_type* pbucket_read_prefs,
															const org_bson_type* pstream_opts,
															const org_bson_value_type* pfile_id,
															org_bson_error_type* perr)
	{
		typedef Buffer buffer_type;
		typedef typename boost::range_value<buffer_type>::type buffer_value_type;
		typedef typename boost::range_iterator<buffer_type>::type buffer_iter_type;

		BOOST_MPL_ASSERT_NOT((boost::is_class<buffer_value_type>));
		BOOST_MPL_ASSERT((is_random_access_iterator<buffer_iter_type>));

		typedef typename native_t<String1>::type string1_type;
		typedef typename native_t<String2>::type string2_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);
		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string2_type, utf8_string2_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));
		utf8_string2_type utf8_str_fname((charset::string_charset_helper_data(str_fname)));

		u64 len = bytes::byte_size(buffer);

		if(!len)
		{
			return false;
		}

		const u8* pbuf = reinterpret_cast<const u8*>(boost::addressof(*boost::begin(buffer)));

		return
			base_type::s_org_gridfs_bucket_save_file_of_buffer_with_id(
				conn, utf8_str_db.data(), utf8_str_fname.data(),
				tmout_msec, pbuf, len,
				pbucket_opts, pbucket_read_prefs,
				pstream_opts, pfile_id, perr);
	}

public:
	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer_with_id(Connection_T& conn,
													const String1& str_db,
													const String2& str_fname,
													u32 tmout_msec, const Buffer& buffer,
													const BsonBucketOpts& bucket_opts,
													const ReadPrefs& bucket_read_prefs,
													const BsonStreamOpts& stream_opts,
													const BsonValueFileID& file_id,
													org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_with_id_wrap(
				conn, str_db, str_fname,
				tmout_msec, buffer,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				detail::to_const_pointer<org_bson_value_type>(file_id),
				perr);
	}

	template<typename Connection_T,
				typename String1, typename String2,
				typename Buffer,
				typename BsonBucketOpts, typename ReadPrefs, typename BsonStreamOpts,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				charset::utf8_string_constructable<String1>,
				charset::utf8_string_constructable<String2>
			>,
			bool
		>::type
		s_gridfs_bucket_save_file_of_buffer_with_id(Connection_T& conn,
													const String1& str_db,
													const String2& str_fname,
													u32 tmout_msec, const Buffer& buffer,
													const BsonBucketOpts& bucket_opts,
													const ReadPrefs& bucket_read_prefs,
													const BsonStreamOpts& stream_opts,
													const BsonValueFileID& file_id,
													org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_save_file_of_buffer_with_id_wrap(
				conn, str_db, str_fname,
				tmout_msec, buffer,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(stream_opts),
				detail::to_const_pointer<org_bson_value_type>(file_id),
				boost::addressof(err));
	}


	// load
private:
	template<typename Connection_T, typename String1, typename Buffer> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			u64
		>::type
		s_gridfs_bucket_load_file_wrap(Connection_T& conn,
										const String1& str_db,
										const org_bson_type* pbucket_opts,
										const org_read_prefs_type* pbucket_read_prefs,
										const org_bson_value_type *pfile_id,
										s32 tmout_msec,
										Buffer& out_buf,
										org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_gridfs_bucket_load_file(
				conn, utf8_str_db.data(),
				pbucket_opts, pbucket_read_prefs,
				pfile_id, tmout_msec,
				out_buf, perr);
	}

public:
	template<typename Connection_T, typename String1,
				typename BsonBucketOpts, typename ReadPrefs,
				typename BsonValueFileID,
				typename Buffer> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			u64
		>::type
		s_gridfs_bucket_load_file(Connection_T& conn,
									const String1& str_db,
									const BsonBucketOpts& bucket_opts,
									const ReadPrefs& bucket_read_prefs,
									const BsonValueFileID& file_id,
									s32 tmout_msec,
									Buffer& out_buf,
									org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_load_file_wrap(
				conn, str_db,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_value_type>(file_id),
				tmout_msec,
				out_buf,
				perr);
	}

	template<typename Connection_T, typename String1,
				typename BsonBucketOpts, typename ReadPrefs,
				typename BsonValueFileID,
				typename Buffer> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			u64
		>::type
		s_gridfs_bucket_load_file(Connection_T& conn,
									const String1& str_db,
									const BsonBucketOpts& bucket_opts,
									const ReadPrefs& bucket_read_prefs,
									const BsonValueFileID& file_id,
									s32 tmout_msec,
									Buffer& out_buf,
									org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_load_file_wrap(
				conn, str_db,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_value_type>(file_id),
				tmout_msec,
				out_buf,
				boost::addressof(err));
	}

	// gridfs_bucket_find
private:
	template<typename Connection_T,
				typename String1,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_gridfs_bucket_find_wrap(Connection_T& conn,
									const String1& str_db,
									const org_bson_type* pbucket_opts,
									const org_read_prefs_type* pbucket_read_prefs,
									const org_bson_type* pfilter,
									const org_bson_type* popts,
									org_bson_error_type* perr,
									const HandlerCollector& handler_collector)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_gridfs_bucket_find(
				conn, utf8_str_db.data(),
				pbucket_opts, pbucket_read_prefs,
				pfilter, popts,
				perr, handler_collector);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonBucketOpts, typename ReadPrefs,
				typename BsonFilter, typename BsonOpts,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_gridfs_bucket_find(Connection_T& conn,
								const String1& str_db,
								const BsonBucketOpts& bucket_opts,
								const ReadPrefs& bucket_read_prefs,
								const BsonFilter& filter,
								const BsonOpts& opts,
								org_bson_error_type* perr,
								const HandlerCollector& handler_collector)
	{
		return
			this_type::s_gridfs_bucket_find_wrap(
				conn, str_db,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(filter),
				detail::to_const_pointer<org_bson_type>(opts),
				perr,
				handler_collector);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonBucketOpts, typename ReadPrefs,
				typename BsonFilter, typename BsonOpts,
				typename HandlerCollector> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			bool
		>::type
		s_gridfs_bucket_find(Connection_T& conn,
								const String1& str_db,
								const BsonBucketOpts& bucket_opts,
								const ReadPrefs& bucket_read_prefs,
								const BsonFilter& filter,
								const BsonOpts& opts,
								org_bson_error_type& err,
								const HandlerCollector& handler_collector)
	{
		return
			this_type::s_gridfs_bucket_find_wrap(
				conn, str_db,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(filter),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err),
				handler_collector);
	}

private:
	template<typename Connection_T,
				typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			s64
		>::type
		s_gridfs_bucket_remove_file_by_id_wrap(Connection_T& conn,
												const String1& str_db,
												const org_bson_type* pbucket_opts,
												const org_read_prefs_type* pbucket_read_prefs,
												const org_bson_value_type* pfile_id,
												org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_gridfs_bucket_remove_file_by_id(
				conn, utf8_str_db.data(),
				pbucket_opts, pbucket_read_prefs,
				pfile_id, perr);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonBucketOpts, typename ReadPrefs,
				typename BsonValueFileID > inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			s64
		>::type
		s_gridfs_bucket_remove_file_by_id(Connection_T& conn,
											const String1& str_db,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonValueFileID& file_id,
											org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_remove_file_by_id_wrap(
				conn, str_db,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_value_type>(file_id),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonBucketOpts, typename ReadPrefs,
				typename BsonValueFileID> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			s64
		>::type
		s_gridfs_bucket_remove_file_by_id(Connection_T& conn,
											const String1& str_db,
											const BsonBucketOpts& bucket_opts,
											const ReadPrefs& bucket_read_prefs,
											const BsonValueFileID& file_id,
											org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_remove_file_by_id_wrap(
				conn, str_db,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_value_type>(file_id),
				boost::addressof(err));
	}

private:
	template<typename Connection_T,
				typename String1> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			s64
		>::type
		s_gridfs_bucket_remove_files_wrap(Connection_T& conn,
												const String1& str_db,
												const org_bson_type* pbucket_opts,
												const org_read_prefs_type* pbucket_read_prefs,
												const org_bson_type* pfilter,
												const org_bson_type* popts,
												org_bson_error_type* perr)
	{
		typedef typename native_t<String1>::type string1_type;

		YGGR_TYPEDEF_CONST_UTF8_STRING_TYPE_TPL(string1_type, utf8_string1_type);

		utf8_string1_type utf8_str_db((charset::string_charset_helper_data(str_db)));

		return
			base_type::s_org_gridfs_bucket_remove_files(
				conn, utf8_str_db.data(),
				pbucket_opts, pbucket_read_prefs,
				pfilter, popts, perr);
	}

public:
	template<typename Connection_T,
				typename String1,
				typename BsonBucketOpts, typename ReadPrefs,
				typename BsonFilter, typename BsonOpts > inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			s64
		>::type
		s_gridfs_bucket_remove_files(Connection_T& conn,
										const String1& str_db,
										const BsonBucketOpts& bucket_opts,
										const ReadPrefs& bucket_read_prefs,
										const BsonFilter& filter,
										const BsonOpts& opts,
										org_bson_error_type* perr = 0)
	{
		return
			this_type::s_gridfs_bucket_remove_files_wrap(
				conn, str_db,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(filter),
				detail::to_const_pointer<org_bson_type>(opts),
				perr);
	}

	template<typename Connection_T,
				typename String1,
				typename BsonBucketOpts, typename ReadPrefs,
				typename BsonFilter, typename BsonOpts> inline
	static
	typename
		boost::enable_if
		<
			charset::utf8_string_constructable<String1>,
			s64
		>::type
		s_gridfs_bucket_remove_files(Connection_T& conn,
										const String1& str_db,
										const BsonBucketOpts& bucket_opts,
										const ReadPrefs& bucket_read_prefs,
										const BsonFilter& filter,
										const BsonOpts& opts,
										org_bson_error_type& err)
	{
		return
			this_type::s_gridfs_bucket_remove_files_wrap(
				conn, str_db,
				detail::to_const_pointer<org_bson_type>(bucket_opts),
				detail::to_const_pointer<org_read_prefs_type>(bucket_read_prefs),
				detail::to_const_pointer<org_bson_type>(filter),
				detail::to_const_pointer<org_bson_type>(opts),
				boost::addressof(err));
	}

	//------------------------gridfs_bucket e----------------------------
};

} // namespace nsql_database_system
} // namespace yggr

#endif //__YGGR_NSQL_DATABASE_SYSTEM_MONGO_ACCESSER_GRIDFS_HPP__
