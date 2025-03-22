//local_fsys_basic_operators.hpp

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

#ifndef __YGGR_FILE_SYSTEM_LOCAL_FSYS_BASIC_OPERATORS_HPP__
#define __YGGR_FILE_SYSTEM_LOCAL_FSYS_BASIC_OPERATORS_HPP__

#include <yggr/base/yggrdef.h>

#ifdef YGGR_USE_OLD_BOOST_FILESYSTEM
#	define BOOST_FILESYSTEM_VERSION 2
#endif // YGGR_USE_OLD_BOOST_FILESYSTEM

// gcc not need this, but it needs to be some debug
//#ifdef __GNUC__
//#   define BOOST_NO_CXX11_SCOPED_ENUMS
//#endif // __GNUC__

#include <yggr/compatibility/stl_exception.hpp>

#include <yggr/base/bit_options.hpp>
#include <yggr/move/move.hpp>

#include <yggr/bytes/byte_size.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/func/foo_t_info.hpp>
#include <yggr/mplex/basic_t_sizeof_log2.hpp>
#include <yggr/mplex/typename_replaceor.hpp>

#include <yggr/container_ex/container_category_check.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/resize.hpp>
#include <yggr/container/clear.hpp>
#include <yggr/container/unordered_set.hpp>

#include <yggr/file_system/detail/path_opt.hpp>
#include <yggr/file_system/text_separator.hpp>
#include <yggr/file_system/path_object_maker.hpp>
#include <yggr/file_system/local_fsys_operator_options.hpp>
#include <yggr/file_system/file_system_c_str.hpp>
#include <yggr/file_system/rename_handlers.hpp>

#include <boost/ref.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <iostream>
#include <numeric>
#include <sstream>
#include <memory>
#include <algorithm>


#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace file_system
{

template<typename File_Stream_Config>
class local_fsys_basic_operators
	: public path_object_maker
{

private:
	typedef File_Stream_Config fstream_cfg_type;
	typedef local_fsys_basic_operators this_type;

public:
	typedef yggr::size_type file_size_type;
	typedef boost::uintmax_t size_type;

	typedef typename fstream_cfg_type::ifstream_type ifstream_type;
	typedef typename fstream_cfg_type::ofstream_type ofstream_type;
	typedef typename fstream_cfg_type::fbuf_type fbuf_type;
	typedef typename fstream_cfg_type::char_type char_type;
	typedef typename fstream_cfg_type::default_handlers_type default_handlers_type;

	typedef text_separator_cr default_text_spr_type;

	typedef char_type fstream_char_type;

	typedef native::path path_type;
	typedef path_type::value_type path_char_type;
	typedef path_type::string_type path_string_type;

	typedef native::file_status file_status_type;
	typedef native::space_info space_info_type;

	typedef native::error_code error_code_type;
	typedef native::filesystem_error exception_type;

	typedef native::directory_entry directory_entry_type;
	typedef native::directory_iterator directory_iterator_type;
	typedef native::recursive_directory_iterator recursive_directory_iterator_type;

	typedef path_object_maker base_type;

	typedef local_fsys_operator_opts options_type;
	typedef local_fsys_file_types_opts file_types_options_type;

	typedef native::symlink_option symlink_option_type;
	typedef native::directory_options directory_options_type;
	typedef native::copy_options copy_option_type;

protected:
	typedef BOOST_SCOPED_ENUM(symlink_option_type) symlink_option_enum_type;
	typedef BOOST_SCOPED_ENUM(directory_options_type) directory_options_enum_type;
	typedef BOOST_SCOPED_ENUM(copy_option_type) copy_option_enum_type;

protected:
	typedef not_rename<path_type> empty_unique_handler_type;

private:
	BOOST_MPL_ASSERT((boost::is_same<char_type, typename ifstream_type::char_type >));
	BOOST_MPL_ASSERT((boost::is_same<char_type, typename ofstream_type::char_type >));
	BOOST_MPL_ASSERT((boost::is_same<char_type, typename fbuf_type::char_type >));

public:
	YGGR_STATIC_CONSTANT(file_size_type, npos = static_cast<file_size_type>(file_size_type(-1)));

protected:
	inline static bool pro_s_has_opts(u32 opts, u32 chk)
	{
		return has_bit_options<u32>(opts, chk);
	}

	inline static u32 pro_s_check_opts(u32 opts, u32 chk)
	{
		return check_bit_options<u32>(opts, chk);
	}

	inline static u32 pro_s_erase_opts(u32 opts, u32 rm)
	{
		return erase_bit_options<u32>(opts, rm);
	}

	inline static u32 pro_s_append_opts(u32 opts, u32 add)
	{
		return append_bit_options<u32>(opts, add);
	}

	//inline static BOOST_SCOPED_ENUM(boost::filesystem::symlink_option)
	//	pro_s_conv_to_symlink_option(u32 opts)
	//{
	//	return (opts & static_cast<u32>(options_type::symlink_recurse))?
	//			boost::filesystem::symlink_option::recurse
	//			: boost::filesystem::symlink_option::no_recurse;
	//}

protected:
	inline static bool pro_s_ec_chk(const error_code_type* ec)
	{
		return (!ec) || (!(*ec));
	}

	inline static bool pro_s_ec_chk_equal_to(const error_code_type* ec, int code)
	{
		return (!ec) || (!(*ec)) || ec->value() == code;
	}

	inline static bool pro_s_ec_chk_not_equal_to(const error_code_type* ec, int code)
	{
		return (!ec) || (!(*ec)) || ec->value() != code;
	}

	inline static void pro_s_ec_clear(error_code_type* ec)
	{
		if(ec)
		{
			ec->clear();
		}
	}

	//-----------------path_object-------------------

protected:
	inline static path_type pro_s_current_path(error_code_type* ec)
	{
		path_type ret = native::call_current_path(ec);
		return this_type::pro_s_ec_chk(ec)? ret : path_type();
	}

	inline static bool pro_s_current_path(const path_type& path, error_code_type* ec)
	{
		return (native::call_current_path(path, ec), this_type::pro_s_ec_chk(ec));
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static path_type current_path(void)
	{
		return this_type::pro_s_current_path(0);
	}

	// non-throw
	inline static path_type current_path(error_code_type& ec)
	{
		return this_type::pro_s_current_path(boost::addressof(ec));
	}

	// throw exception_type, compatibility::stl_exception
	inline static bool current_path(const path_type& path)
	{
		return this_type::pro_s_current_path(path, 0);
	}

	// non-throw
	inline static bool current_path(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_current_path(path, boost::addressof(ec));
	}

public:
	inline static path_type parent_path(const path_type& path)
	{
		if(path.empty())
		{
			return path_type();
		}
		else
		{
			const path_string_type& str_path = get_path_string(path);

			if(base_type::is_slants(str_path[str_path.size() - 1]))
			{
				path_type tmp = path;
				path_string_type& str_tmp = get_path_string(tmp);
				str_tmp.erase(str_tmp.size() - 1);
				return tmp.parent_path();
			}
			else
			{
				return path.parent_path();
			}
		}
	}

public:
	using base_type::make_path_object;

public:
	inline static const path_type& relative_current_path(void)
	{
		static const path_type path = this_type::make_path_object("./");
		return path;
	}

	inline static const path_type& relative_parent_path(void)
	{
		static const path_type path = this_type::make_path_object("../");
		return path;
	}

#if defined(YGGR_AT_WINDOWS)
	inline static path_type root_path(void)
	{
		error_code_type ec;
		path_type path = this_type::current_path(ec).root_path();
		return !ec? path : path_type();
	}
#else
	inline static const path_type& root_path(void)
	{
		static const path_type path =
			this_type::make_path_object(
				base_type::pro_s_slants_begin<path_char_type>(),
				base_type::pro_s_slants_begin<path_char_type>() + 1);
		return path;
	}
#endif // YGGR_AT_WINDOWS

protected:
	inline static path_type pro_s_root_path_no_fmt(const path_type& path)
	{
		assert(path == native::system_complete(path));
		return path.root_path();
	}

	inline static path_type pro_s_root_path(const path_type& path, error_code_type* ec)
	{
		path_type sc_path;
		return (sc_path = native::call_system_complete(path, ec), this_type::pro_s_ec_chk(ec))?
				this_type::pro_s_root_path_no_fmt(sc_path) : path_type();
	}

public:

	// root_path
	inline static path_type root_path(const path_type& path)
	{
		return this_type::pro_s_root_path(path, 0);
	}

	// non-throw
	inline static path_type root_path(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_root_path(path, boost::addressof(ec));
	}

	// non throw
	inline static path_type absolute(const path_type& path,
										const path_type& base = this_type::current_path())
	{
		return native::absolute(path, base);
	}

protected:
	inline static path_type pro_s_canonical(const path_type& path, const path_type& base, error_code_type* ec)
	{
		path_type ret = native::call_canonical(path, base, ec);
		return this_type::pro_s_ec_chk(ec)? ret : path_type();
	}
public:
	// canonical
	// Converts p, which must exist, to an absolute path that has no symbolic link, "dot", or "dot-dot" elements.

	// throw exception_type, compatibility::stl_exception
	inline static path_type canonical(const path_type& path, const path_type& base = this_type::current_path())
	{
		return this_type::pro_s_canonical(path, base, 0);
	}

	// non-throw
	inline static path_type canonical(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_canonical(path, this_type::current_path(), boost::addressof(ec));
	}

	// non-throw
	inline static path_type canonical(const path_type& path, const path_type& base, error_code_type& ec)
	{
		return this_type::pro_s_canonical(path, base, boost::addressof(ec));
	}

public:
	inline static path_type system_complete(const path_type& path)
	{
		return native::system_complete(path);
	}

	inline static path_type system_complete(const path_type& path, error_code_type& ec)
	{
		return native::system_complete(path, ec);
	}

	// -------------------all type function--------------------

protected:
	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!!! warning:															!!!
	!!! if is windows symlink boost::filesystem::is_exists return false,	!!!
	!!! so rewrite it.														!!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/

	inline static bool pro_s_is_exists(const path_type& path, error_code_type* ec)
	{
		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		switch(sat_path.type())
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return false;
		default:
			return true;
		}
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static bool is_exists(const path_type& path)
	{
		return this_type::pro_s_is_exists(path, 0);
	}

	// non-throw
	inline static bool is_exists(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_is_exists(path, boost::addressof(ec));
	}

protected:
	inline static bool pro_s_is_in_same_volume(const path_type& p1, const path_type& p2, error_code_type* ec)
	{
		return native::call_is_in_same_volume(p1, p2, ec);
	}

public:
	inline static bool is_in_same_volume(const path_type& p1, const path_type& p2)
	{
		return native::call_is_in_same_volume(p1, p2, 0);
	}

	inline static bool is_in_same_volume(const path_type& p1, const path_type& p2, error_code_type& ec)
	{
		return native::call_is_in_same_volume(p1, p2, boost::addressof(ec));
	}

protected:
	inline static bool pro_s_symlink_is_in_same_volume(const path_type& p1, const path_type& p2, error_code_type* ec)
	{
		return native::call_symlink_is_in_same_volume(p1, p2, ec);
	}

public:
	inline static bool symlink_is_in_same_volume(const path_type& p1, const path_type& p2)
	{
		return native::call_symlink_is_in_same_volume(p1, p2, 0);
	}

	inline static bool symlink_is_in_same_volume(const path_type& p1, const path_type& p2, error_code_type& ec)
	{
		return native::call_symlink_is_in_same_volume(p1, p2, boost::addressof(ec));
	}

private:
	inline static bool prv_s_equivalent_no_ec_chk(const path_type& path1, const file_status_type& sat1,
														const path_type& path2, const file_status_type& sat2,
														error_code_type* ec)
	{
		assert(sat1 == this_type::pro_s_path_symlink_status(path1, ec));
		assert(sat2 == this_type::pro_s_path_symlink_status(path2, ec));

		return (sat1.type() == sat2.type())
				&& native::call_equivalent(path1, path2, ec);
	}

protected:
	// boost::filesystem::equivalent not support symlink
	inline static bool pro_s_equivalent_no_ec_chk(const path_type& path1, const file_status_type& sat1,
													const path_type& path2, const file_status_type& sat2,
													error_code_type* ec)
	{
		return this_type::prv_s_equivalent_no_ec_chk(path1, sat1, path2, sat2, ec);
	}

	inline static bool pro_s_equivalent(const path_type& path1, const file_status_type& sat1,
										const path_type& path2, const file_status_type& sat2,
										error_code_type* ec)
	{
		return this_type::prv_s_equivalent_no_ec_chk(path1, sat1, path2, sat2, ec)
				&& this_type::pro_s_ec_chk(ec);
	}

	inline static bool pro_s_equivalent_no_ec_chk(const path_type& path1, const path_type& path2, error_code_type* ec)
	{
		file_status_type sat1 = this_type::pro_s_path_symlink_status(path1, ec);
		file_status_type sat2 = this_type::pro_s_path_symlink_status(path2, ec);

		return this_type::prv_s_equivalent_no_ec_chk(path1, sat1, path2, sat2, ec);
	}

	inline static bool pro_s_equivalent(const path_type& path1, const path_type& path2, error_code_type* ec)
	{
		file_status_type sat1 = this_type::pro_s_path_symlink_status(path1, ec);
		file_status_type sat2 = this_type::pro_s_path_symlink_status(path2, ec);

		return this_type::prv_s_equivalent_no_ec_chk(path1, sat1, path2, sat2, ec)
				&& this_type::pro_s_ec_chk(ec);
	}

public:
	//equivalent
	// warning: "equivalent" not support symlink,
	//			if you want to equivalent symlink, please using "symlink_equivalent"

	// throw exception_type, compatibility::stl_exception
	inline static bool equivalent(const path_type& path1, const path_type& path2)
	{
		return this_type::pro_s_equivalent(path1, path2, 0);
	}

	// non-throw
	inline static bool equivalent(const path_type& path1, const path_type& path2, error_code_type& ec)
	{
		return this_type::pro_s_equivalent(path1, path2, boost::addressof(ec));
	}

protected:
	inline static std::size_t pro_s_equivalent_hash_value_no_ec_chk(const path_type& path, error_code_type* ec)
	{
		return native::call_equivalent_hash_value(path, ec);
	}

	inline static std::size_t pro_s_equivalent_hash_value(const path_type& path, error_code_type* ec)
	{
		std::size_t val = native::call_equivalent_hash_value(path, ec);
		return this_type::pro_s_ec_chk(ec)? val : 0;
	}

public:
	// equivalent_hash_value
	// warning: "equivalent_hash_value" not support symlink,
	//			if you want to equivalent symlink, please "symlink_equivalent_hash_value"

	// throw exception_type, compatibility::stl_exception
	inline static std::size_t equivalent_hash_value(const path_type& path)
	{
		return this_type::pro_s_equivalent_hash_value(path, 0);
	}

	// non-throw
	inline static std::size_t equivalent_hash_value(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_equivalent_hash_value(path, boost::addressof(ec));
	}

protected:
	inline static std::size_t
		pro_s_equivalent_hash_value_if_has_hard_link_no_ec_chk(const path_type& path, error_code_type* ec)
	{
		return native::call_equivalent_hash_value_if_has_hard_link(path, ec);
	}

	inline static std::size_t
		pro_s_equivalent_hash_value_if_has_hard_link(const path_type& path, error_code_type* ec)
	{
		std::size_t val = native::call_equivalent_hash_value_if_has_hard_link(path, ec);
		return this_type::pro_s_ec_chk(ec)? val : 0;
	}

public:
	// equivalent_hash_value
	// warning: "equivalent_hash_value_if_has_hard_link" not support symlink,
	//			if you want to equivalent symlink, please "symlink_equivalent_hash_value_if_has_hard_link"

	// throw exception_type, compatibility::stl_exception
	inline static std::size_t equivalent_hash_value_if_has_hard_link(const path_type& path)
	{
		return this_type::pro_s_equivalent_hash_value_if_has_hard_link(path, 0);
	}

	// non-throw
	inline static std::size_t equivalent_hash_value_if_has_hard_link(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_equivalent_hash_value_if_has_hard_link(path, boost::addressof(ec));
	}

private:
	inline static bool prv_s_symlink_equivalent_no_ec_chk(const path_type& path1, const file_status_type& sat1,
															const path_type& path2, const file_status_type& sat2,
															error_code_type* ec)
	{
		assert(sat1 == this_type::pro_s_path_symlink_status(path1, ec));
		assert(sat2 == this_type::pro_s_path_symlink_status(path2, ec));

		return sat1.type() == sat2.type()
				&& native::call_symlink_equivalent(path1, path2, ec);

	}

protected:
	inline static bool pro_s_symlink_equivalent_no_ec_chk(const path_type& path1, const file_status_type& sat1,
															const path_type& path2, const file_status_type& sat2,
															error_code_type* ec)
	{
		return this_type::prv_s_symlink_equivalent_no_ec_chk(path1, sat1, path2, sat2, ec);
	}

	inline static bool pro_s_symlink_equivalent(const path_type& path1, const file_status_type& sat1,
													const path_type& path2, const file_status_type& sat2,
													error_code_type* ec)
	{
		return this_type::prv_s_symlink_equivalent_no_ec_chk(path1, sat1, path2, sat2, ec)
				&& this_type::pro_s_ec_chk(ec);
	}

	inline static bool pro_s_symlink_equivalent_no_ec_chk(const path_type& path1, const path_type& path2, error_code_type* ec)
	{
		file_status_type sat1 = this_type::pro_s_path_symlink_status(path1, ec);
		file_status_type sat2 = this_type::pro_s_path_symlink_status(path2, ec);

		return this_type::prv_s_symlink_equivalent_no_ec_chk(path1, sat1, path2, sat2, ec);
	}

	inline static bool pro_s_symlink_equivalent(const path_type& path1, const path_type& path2, error_code_type* ec)
	{
		file_status_type sat1 = this_type::pro_s_path_symlink_status(path1, ec);
		file_status_type sat2 = this_type::pro_s_path_symlink_status(path2, ec);

		return this_type::prv_s_symlink_equivalent_no_ec_chk(path1, sat1, path2, sat2, ec)
				&& this_type::pro_s_ec_chk(ec);
	}

public:
	//symlink_equivalent
	// warning: same to "equivalent" and support symlink

	// throw exception_type, compatibility::stl_exception
	inline static bool symlink_equivalent(const path_type& path1, const path_type& path2)
	{
		return this_type::pro_s_symlink_equivalent(path1, path2, 0);
	}

	// non-throw
	inline static bool symlink_equivalent(const path_type& path1, const path_type& path2, error_code_type& ec)
	{
		return this_type::pro_s_symlink_equivalent(path1, path2, boost::addressof(ec));
	}

protected:
	inline static std::size_t pro_s_symlink_equivalent_hash_value_no_ec_chk(const path_type& path, error_code_type* ec)
	{
		return native::call_symlink_equivalent_hash_value(path, ec);
	}

	inline static std::size_t pro_s_symlink_equivalent_hash_value(const path_type& path, error_code_type* ec)
	{
		std::size_t val = native::call_symlink_equivalent_hash_value(path, ec);
		return this_type::pro_s_ec_chk(ec)? val : 0;
	}

public:
	// symlink_equivalent_hash_value
	// warning: same to "equivalent_hash_value" and support symlink

	// throw exception_type, compatibility::stl_exception
	inline static std::size_t symlink_equivalent_hash_value(const path_type& path)
	{
		return this_type::pro_s_symlink_equivalent_hash_value(path, 0);
	}

	// non-throw
	inline static std::size_t symlink_equivalent_hash_value(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_symlink_equivalent_hash_value(path, boost::addressof(ec));
	}

protected:
	inline static std::size_t
		pro_s_symlink_equivalent_hash_value_if_hash_hard_link_no_ec_chk(const path_type& path, error_code_type* ec)
	{
		return native::call_symlink_equivalent_hash_value_if_has_hard_link(path, ec);
	}

	inline static std::size_t
		pro_s_symlink_equivalent_hash_value_if_has_hard_link(const path_type& path, error_code_type* ec)
	{
		std::size_t val = native::call_symlink_equivalent_hash_value_if_has_hard_link(path, ec);
		return this_type::pro_s_ec_chk(ec)? val : 0;
	}

public:
	// symlink_equivalent_hash_value
	// warning: same to "equivalent_hash_value_if_has_hard_link" and support symlink

	// throw exception_type, compatibility::stl_exception
	inline static std::size_t
		symlink_equivalent_hash_value_if_has_hard_link(const path_type& path)
	{
		return this_type::pro_s_symlink_equivalent_hash_value_if_has_hard_link(path, 0);
	}

	// non-throw
	inline static std::size_t
		symlink_equivalent_hash_value_if_has_hard_link(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_symlink_equivalent_hash_value_if_has_hard_link(path, boost::addressof(ec));
	}

protected:
	inline static native::perms pro_s_permissions(const path_type& path, error_code_type* ec)
	{
		return this_type::pro_s_path_symlink_status(path, ec).permissions();
	}

	inline static bool pro_s_permissions(const path_type& path, native::perms prms, error_code_type* ec)
	{
		return (native::call_permissions(path, prms, ec), this_type::pro_s_ec_chk(ec));
	}

public:

	// throw exception_type, compatibility::stl_exception
	inline static native::perms permissions(const path_type& path)
	{
		return this_type::pro_s_permissions(path, static_cast<error_code_type*>(0));
	}

	// non-throw
	inline static native::perms permissions(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_permissions(path, boost::addressof(ec));
	}

	// throw exception_type, compatibility::stl_exception
	inline static bool permissions(const path_type& path, native::perms prms)
	{
		return this_type::pro_s_permissions(path, prms, 0);
	}

	// non-throw
	inline static bool permissions(const path_type& path, native::perms prms, error_code_type& ec)
	{
		return this_type::pro_s_permissions(path, prms, boost::addressof(ec));
	}

protected:
	inline static file_status_type pro_s_path_status(const path_type& path, error_code_type* ec)
	{
		return native::call_status_ex(path, ec);
	}

public:
	// path status

	// throw exception_type, compatibility::stl_exception
	inline static file_status_type path_status(const path_type& path)
	{
		return this_type::pro_s_path_status(path, 0);
	}

	// non-throw
	inline static file_status_type path_status(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_path_status(path, boost::addressof(ec));
	}

protected:
	inline static file_status_type pro_s_path_symlink_status(const path_type& path, error_code_type* ec)
	{
		return native::call_symlink_status_ex(path, ec);
	}

public:
	// path status

	// throw exception_type, compatibility::stl_exception
	inline static file_status_type path_symlink_status(const path_type& path)
	{
		return this_type::pro_s_path_symlink_status(path, 0);
	}

	// non-throw
	inline static file_status_type path_symlink_status(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_path_symlink_status(path, boost::addressof(ec));
	}

public:
	// non-throw
	inline static bool status_known(const file_status_type& s)
	{
		return native::status_known(s);
	}

protected:
	static bool pro_s_expand_path(path_type& dst_path, const path_type& src_path, error_code_type* ec)
	{
		dst_path.clear();

		if(src_path.empty())
		{
			return false;
		}

		file_status_type sat_src = this_type::pro_s_path_symlink_status(src_path, ec);
		u32 ft_src = sat_src.type();

		switch(ft_src)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return false;
		case boost::filesystem::symlink_file:
			break;
		default:
			return (dst_path = src_path, true);
		}

		path_type tmp_path;

		try
		{
			if(!(tmp_path = native::call_read_symlink(src_path, ec), this_type::pro_s_ec_chk(ec)))
			{
				return false;
			}
		}
		catch(const exception_type&)
		{
			return false;
		}

		if(tmp_path.is_relative())
		{
			tmp_path = src_path.parent_path() / tmp_path;
		}

		try
		{
			sat_src = this_type::pro_s_path_symlink_status(tmp_path, ec);
			if(!this_type::pro_s_ec_chk(ec))
			{
				sat_src = file_status_type();
			}
		}
		catch(const exception_type&)
		{
			sat_src = file_status_type();
		}

		ft_src = sat_src.type();
		switch(ft_src)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return false;
		default:
			dst_path.swap(tmp_path);
			return true;
		}
	}

	inline static path_type pro_s_expand_path(const path_type& src_path, error_code_type* ec)
	{
		path_type tmp;
#ifdef _DEBUG
		if(!this_type::pro_s_expand_path(tmp, src_path, ec))
		{
			assert(tmp.empty());
		}
#else
		this_type::pro_s_expand_path(tmp, src_path, ec);
#endif // _DEBUG
		return tmp;
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static bool expand_path(path_type& dst_path, const path_type& src_path)
	{
		return this_type::pro_s_expand_path(dst_path, src_path, 0);
	}

	// non-throw
	inline static bool expand_path(path_type& dst_path, const path_type& src_path, error_code_type& ec)
	{
		return this_type::pro_s_expand_path(dst_path, src_path, boost::addressof(ec));
	}

	// throw exception_type, compatibility::stl_exception
	inline static path_type expand_path(const path_type& src_path)
	{
		return this_type::pro_s_expand_path(src_path, 0);
	}

	// non-throw
	inline static path_type expand_path(const path_type& src_path, error_code_type& ec)
	{
		return this_type::pro_s_expand_path(src_path, boost::addressof(ec));
	}

protected:
	static bool pro_s_recursive_expand_path(path_type& dst_path, const path_type& src_path, error_code_type* ec)
	{
		dst_path.clear();

		if(src_path.empty())
		{
			return false;
		}

		file_status_type sat_src = this_type::pro_s_path_symlink_status(src_path, ec);
		u32 ft_src = sat_src.type();

		switch(ft_src)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return false;
		case boost::filesystem::symlink_file:
			break;
		default:
			return (dst_path = src_path, true);
		}

		path_type tmp_path_now = src_path;
		path_type tmp_path_next;

		for(;;)
		{
			try
			{
				if(!(tmp_path_next = native::call_read_symlink(tmp_path_now, ec), this_type::pro_s_ec_chk(ec)))
				{
					return false;
				}
			}
			catch(const exception_type&)
			{
				return false;
			}

			if(tmp_path_next.is_relative())
			{
				tmp_path_next = tmp_path_now.parent_path() / tmp_path_next;
			}

			try
			{
				sat_src = this_type::pro_s_path_symlink_status(tmp_path_next, ec);
				if(!this_type::pro_s_ec_chk(ec))
				{
					sat_src = file_status_type();
				}
			}
			catch(const exception_type&)
			{
				sat_src = file_status_type();
			}

			ft_src = sat_src.type();
			switch(ft_src)
			{
			case boost::filesystem::status_error:
			case boost::filesystem::file_not_found:
				return false;
			case boost::filesystem::symlink_file:
				tmp_path_now.swap(tmp_path_next);
				break;
			default:
				dst_path.swap(tmp_path_next);
				return true;
			}
		}
	}

	inline static path_type pro_s_recursive_expand_path(const path_type& src_path, error_code_type* ec)
	{
		path_type tmp;
#ifdef _DEBUG
		if(!this_type::pro_s_recursive_expand_path(tmp, src_path, ec))
		{
			assert(tmp.empty());
		}
#else
		this_type::pro_s_recursive_expand_path(tmp, src_path, ec);
#endif // _DEBUG
		return tmp;
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static bool recursive_expand_path(path_type& dst_path, const path_type& src_path)
	{
		return this_type::pro_s_recursive_expand_path(dst_path, src_path, 0);
	}

	// non-throw
	inline static bool recursive_expand_path(path_type& dst_path, const path_type& src_path, error_code_type& ec)
	{
		return this_type::pro_s_recursive_expand_path(dst_path, src_path, boost::addressof(ec));
	}

	// throw exception_type, compatibility::stl_exception
	inline static path_type recursive_expand_path(const path_type& src_path)
	{
		return this_type::pro_s_recursive_expand_path(src_path, 0);
	}

	// non-throw
	inline static path_type recursive_expand_path(const path_type& src_path, error_code_type& ec)
	{
		return this_type::pro_s_recursive_expand_path(src_path, boost::addressof(ec));
	}

public:
	// is_empty

	// throw exception_type, compatibility::stl_exception
	inline static bool is_empty(const path_type& path)
	{
		return native::is_empty(path);
	}

	// non-throw
	inline static bool is_empty(const path_type& path, error_code_type& ec)
	{
		return native::is_empty(path, ec);
	}

public:
	// is_other

	// non-throw
	inline static bool is_other(const file_status_type& s)
	{
		return native::is_other(s);
	}

	// throw exception_type, compatibility::stl_exception
	inline static bool is_other(const path_type& path)
	{
		return native::is_other(path);
	}

	// non-throw
	inline static bool is_other(const path_type& path, error_code_type& ec)
	{
		return native::is_other(path, ec);
	}

public:
	// get_last_write_time

	// throw exception_type, compatibility::stl_exception
	inline static std::time_t last_write_time(const path_type& path)
	{
		return native::last_write_time(path);
	}

	// non-throw
	inline static std::time_t last_write_time(const path_type& path, error_code_type& ec)
	{
		return native::last_write_time(path, ec);
	}

protected:
	// need check ec
	inline static std::time_t
		pro_s_set_last_write_time(const path_type& path, std::time_t tm, error_code_type* ec)
	{
		std::time_t oldtm = native::call_last_write_time(path, ec);
		if(this_type::pro_s_ec_chk(ec))
		{
			native::call_last_write_time(path, tm, ec);
		}

		return oldtm;
	}

public:
	// set_last_write_time
	inline static std::time_t last_write_time(const path_type& path, std::time_t tm)
	{
		return this_type::pro_s_set_last_write_time(path, tm, 0);
	}

	inline static std::time_t last_write_time(const path_type& path, std::time_t tm, error_code_type& ec)
	{
		return this_type::pro_s_set_last_write_time(path, tm, boost::addressof(ec));
	}

protected:
	inline static space_info_type pro_s_space(const path_type& path, error_code_type* ec)
	{
		static const space_info_type faild_ret =
            {static_cast<boost::uintmax_t>(-1), static_cast<boost::uintmax_t>(-1), static_cast<boost::uintmax_t>(-1)};
		space_info_type ret = native::call_space(path, ec);

		return this_type::pro_s_ec_chk(ec)? ret : faild_ret;
	}

public:

	inline static space_info_type space(const path_type& path)
	{
		return this_type::pro_s_space(path, 0);
	}

	inline static space_info_type space(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_space(path, boost::addressof(ec));
	}

	//----------------------------------directorys----------------------------------

public:
	inline static directory_entry_type
		make_directory_entry_object(const path_type& path,
									file_status_type st = file_status_type(),
									file_status_type symlink_st = file_status_type())
	{
		return directory_entry_type(path, st, symlink_st);
	}

protected:
	inline static directory_iterator_type
		pro_s_make_directory_iterator_object(const path_type& path, error_code_type* ec)
	{
		directory_iterator_type iter =
			native::emplace_directory_iterator(path, ec);
		return this_type::pro_s_ec_chk(ec)? iter : directory_iterator_type();
	}

public:

	//make_directory_iterator_object
	// throw exception_type, compatibility::stl_exception
	inline static directory_iterator_type
		make_directory_iterator_object(const path_type& path)
	{
		return this_type::pro_s_make_directory_iterator_object(path, 0);
	}

	// non-throw
	inline static directory_iterator_type
		make_directory_iterator_object(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_make_directory_iterator_object(path, boost::addressof(ec));
	}

protected:
	inline static recursive_directory_iterator_type
		pro_s_make_recursive_directory_iterator_object(const path_type& path,
														symlink_option_enum_type opt,
														error_code_type* ec)
	{
		recursive_directory_iterator_type iter =
			native::emplace_recursive_directory_iterator(path, opt, ec);
		return this_type::pro_s_ec_chk(ec)? iter : recursive_directory_iterator_type();
	}

	inline static recursive_directory_iterator_type
		pro_s_make_recursive_directory_iterator_object(const path_type& path,
														directory_options_enum_type opt,
														error_code_type* ec)
	{
		recursive_directory_iterator_type iter =
			native::emplace_recursive_directory_iterator(path, opt, ec);
		return this_type::pro_s_ec_chk(ec)? iter : recursive_directory_iterator_type();
	}

public:
	//make_recursive_directory_iterator_object
	// throw exception_type, compatibility::stl_exception

#if BOOST_VERSION < 107200
	inline static recursive_directory_iterator_type
		make_recursive_directory_iterator_object(const path_type& path,
													symlink_option_enum_type opt = symlink_option_type::none)
	{
		return this_type::pro_s_make_recursive_directory_iterator_object(path, opt, 0);
	}

	inline static recursive_directory_iterator_type
		make_recursive_directory_iterator_object(const path_type& path, directory_options_enum_type opt)
	{
		return this_type::pro_s_make_recursive_directory_iterator_object(path, opt, 0);
	}

#else

	inline static recursive_directory_iterator_type
		make_recursive_directory_iterator_object(const path_type& path,
													directory_options_enum_type opt = directory_options_type::none)
	{
		return this_type::pro_s_make_recursive_directory_iterator_object(path, opt, 0);
	}

	inline static recursive_directory_iterator_type
		make_recursive_directory_iterator_object(const path_type& path, symlink_option_enum_type opt)
	{
		return this_type::pro_s_make_recursive_directory_iterator_object(path, opt, 0);
	}
#endif // #if BOOST_VERSION < 107200

	// non-throw
	inline static recursive_directory_iterator_type
		make_recursive_directory_iterator_object(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_make_recursive_directory_iterator_object(
					path, symlink_option_type::none, boost::addressof(ec));
	}

	inline static recursive_directory_iterator_type
		make_recursive_directory_iterator_object(const path_type& path,
													symlink_option_enum_type opt,
													error_code_type& ec)
	{
		return this_type::pro_s_make_recursive_directory_iterator_object(path, opt, boost::addressof(ec));
	}

	inline static recursive_directory_iterator_type
		make_recursive_directory_iterator_object(const path_type& path,
													directory_options_enum_type opt,
													error_code_type& ec)
	{
		return this_type::pro_s_make_recursive_directory_iterator_object(path, opt, boost::addressof(ec));
	}

protected:
	static bool pro_s_check_is_included_in_directory_no_fmt(const path_type& root_path, const path_type& sub_path, error_code_type* ec)
	{
		typedef typename ifstream_type::pos_type pos_type;
		typedef typename mplex::basic_t_sizeof_log2<path_char_type>::type path_char_sizeof_log2_type;
		BOOST_MPL_ASSERT_NOT((boost::is_same<path_char_sizeof_log2_type, mplex::basic_t_sizeof_log2_failed>));

		assert(root_path == native::system_complete(root_path));
		assert(sub_path == native::system_complete(sub_path));

		size_type root_path_size = root_path.native().size();
		size_type sub_path_size = sub_path.native().size();

		if(!(root_path_size && sub_path_size))
		{
			if(ec)
			{
				ec->assign(YGGR_FSYS_ERROR_PATH_NAME_FAILED(), boost::system::system_category());
			}
			else
			{
				 BOOST_FILESYSTEM_THROW(
					exception_type("yggr::file_system::local_fsys::check_is_included_in_directory",
									root_path, sub_path,
									error_code_type(
										YGGR_FSYS_ERROR_PATH_NAME_FAILED(),
										boost::system::system_category())));
			}
			return false;
		}
		else if(!(root_path_size < sub_path_size))
		{
			return false;
		}
		else
		{
			const path_char_type* root_path_str = root_path.c_str();
			const path_char_type* sub_path_str = sub_path.c_str();

			return base_type::pro_s_is_slants(root_path_str[root_path_size - 1])?
					0 == memcmp(root_path_str, sub_path_str,
								root_path_size << path_char_sizeof_log2_type::value)
					: (sub_path_size - root_path_size > 1)
						&& base_type::pro_s_is_slants(sub_path_str[root_path_size])
						&& 0 == memcmp(root_path_str, sub_path_str,
										root_path_size << path_char_sizeof_log2_type::value);
		}
	}

	inline static bool pro_s_check_is_included_in_directory(const path_type& root_path, const path_type& sub_path, error_code_type* ec)
	{
		path_type sc_root_path;
		path_type sc_sub_path;

		return (sc_root_path = native::call_system_complete(root_path, ec), this_type::pro_s_ec_chk(ec))
					&& (sc_sub_path = native::call_system_complete(sub_path, ec), this_type::pro_s_ec_chk(ec))
					&& this_type::pro_s_check_is_included_in_directory_no_fmt(sc_root_path, sc_sub_path, ec);
	}

public:
	// check_is_included_in_directory
	// throw exception_type, compatibility::stl_exception
	inline static bool check_is_included_in_directory(const path_type& root_path, const path_type& sub_path)
	{
		return this_type::pro_s_check_is_included_in_directory(root_path, sub_path, 0);
	}

	// non-throw
	inline static bool check_is_included_in_directory(const path_type& root_path, const path_type& sub_path, error_code_type& ec)
	{
		return this_type::pro_s_check_is_included_in_directory(root_path, sub_path, boost::addressof(ec));
	}

protected:
	inline static bool pro_s_is_included_in_directory(const path_type& root_path, const path_type& sub_path,
														error_code_type* ec)
	{
		file_status_type sat_root = this_type::pro_s_path_symlink_status(root_path, ec);

		switch(sat_root.type())
		{
		case boost::filesystem::directory_file:
			break;
		default:
			return false;
		}

		file_status_type sat_sub = this_type::pro_s_path_symlink_status(sub_path, ec);

		switch(sat_sub.type())
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return false;
		default:
			return this_type::pro_s_check_is_included_in_directory(root_path, sub_path, ec);
		}
	}

public:
	// is_include_in_directory
	// throw exception_type, compatibility::stl_exception
	inline static bool is_included_in_directory(const path_type& root_path, const path_type& sub_path)
	{
		return this_type::pro_s_is_included_in_directory(root_path, sub_path, 0);
	}

	// non-throw
	inline static bool is_included_in_directory(const path_type& root_path, const path_type& sub_path,
												error_code_type& ec)
	{
		return this_type::pro_s_is_included_in_directory(root_path, sub_path, boost::addressof(ec));
	}

protected:
	static bool pro_s_check_is_child_no_fmt(const path_type& parent, const path_type& child, error_code_type* ec)
	{
		assert(parent == native::system_complete(parent));
		assert(child == native::system_complete(child));

		size_type parent_size = parent.native().size();
		size_type child_size = child.native().size();

		if(!(parent_size && child_size))
		{
			if(ec)
			{
				ec->assign(YGGR_FSYS_ERROR_PATH_NAME_FAILED(), boost::system::system_category());
			}
			else
			{
				 BOOST_FILESYSTEM_THROW(
					exception_type("yggr::file_system::local_fsys::check_is_child_directory",
									parent, child,
									error_code_type(
										YGGR_FSYS_ERROR_PATH_NAME_FAILED(),
										boost::system::system_category())));
			}
			return false;
		}
		else if(!(parent_size < child_size))
		{
			return false;
		}
		else
		{
			const path_char_type* parent_str = parent.c_str();
			const path_char_type* child_str = child.c_str();

			const path_char_type* chk_iter = 0;
			const path_char_type* chk_iter_end = child_str + child_size;

			if(base_type::pro_s_is_slants(parent_str[parent_size - 1])
				&& (0 == memcmp(parent_str, child_str, parent_size)))
			{
				chk_iter =
					std::find(child_str + parent_size, chk_iter_end,
								base_type::pro_s_system_slant<path_char_type>());

				assert(chk_iter <= chk_iter_end);
				return std::distance(chk_iter, chk_iter_end) < 2;
			}
			else if(base_type::pro_s_is_slants(child_str[parent_size])
					&& (0 == memcmp(parent_str, child_str, parent_size - 1)))
			{
				chk_iter =
					std::find(child_str + (parent_size + 1), chk_iter_end,
								base_type::pro_s_system_slant<path_char_type>());

				assert(chk_iter <= chk_iter_end);
				return std::distance(chk_iter, chk_iter_end) < 2;
			}
			else
			{
				return false;
			}
		}
	}

	inline static bool pro_s_check_is_child(const path_type& parent, const path_type& child, error_code_type* ec)
	{
		path_type sc_parent;
		path_type sc_child;

		return (sc_parent = native::call_system_complete(parent, ec), this_type::pro_s_ec_chk(ec))
					&& (sc_child = native::call_system_complete(child, ec), this_type::pro_s_ec_chk(ec))
					&& this_type::pro_s_check_is_child_no_fmt(sc_parent, sc_child, ec);
	}

public:
	// check_is_child
	// throw exception_type, compatibility::stl_exception
	inline static bool check_is_child(const path_type& parent, const path_type& child)
	{
		return this_type::pro_s_check_is_child(parent, child, 0);
	}

	// non-throw
	inline static bool check_is_child(const path_type& parent, const path_type& child, error_code_type& ec)
	{
		return this_type::pro_s_check_is_child(parent, child, boost::addressof(ec));
	}

protected:
	inline static bool pro_s_is_child(const path_type& parent, const path_type& child, error_code_type* ec)
	{
		file_status_type sat_parent = this_type::pro_s_path_symlink_status(parent, ec);

		switch(sat_parent.type())
		{
		case boost::filesystem::directory_file:
			break;
		default:
			return false;
		}

		file_status_type sat_child = this_type::pro_s_path_symlink_status(child, ec);

		switch(sat_child.type())
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return false;
		default:
			return this_type::pro_s_symlink_equivalent(parent, this_type::parent_path(child), ec);
		}
	}

public:
	inline static bool is_child(const path_type& parent, const path_type& child)
	{
		return this_type::pro_s_is_child(parent, child, 0);
	}

	inline static bool is_child(const path_type& parent, const path_type& child,
											error_code_type& ec)
	{
		return this_type::pro_s_is_child(parent, child, boost::addressof(ec));
	}

public:
	// is_directory
	// non-throw
	inline static bool is_directory(const file_status_type& s)
	{
		return native::is_directory(s);
	}

	// throw exception_type, compatibility::stl_exception
	inline static bool is_directory(const path_type& path)
	{
		return native::is_directory(path);
	}

	// non-throw
	inline static bool is_directory(const path_type& path, error_code_type& ec)
	{
		return native::is_directory(path, ec);
	}

protected:
	inline static size_type
		pro_s_count_children_directory(const path_type& path, error_code_type* ec)
	{
		return this_type::pro_s_count_children(path, ec, file_types_options_type::directory_file);
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static size_type count_children_directory(const path_type& path)
	{
		return this_type::pro_s_count_children(path, 0, file_types_options_type::directory_file);
	}

	// non-throw
	inline static size_type count_children_directory(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_count_children(path, boost::addressof(ec), file_types_options_type::directory_file);
	}

protected:
	inline static size_type
		pro_s_recursive_count_children_directory(const path_type& path, error_code_type* ec)
	{
		return this_type::pro_s_recursive_count_children(path, ec, file_types_options_type::directory_file);
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static size_type recursive_count_children_directory(const path_type& path)
	{
		return this_type::pro_s_recursive_count_children(path, 0, file_types_options_type::directory_file);
	}

	// non-throw
	inline static size_type
		recursive_count_children_directory(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_recursive_count_children(
				path, boost::addressof(ec), file_types_options_type::directory_file);
	}

protected:
	//create_directories ops check
	// directory only operator don't support replace
	inline static bool pro_s_opts_chk_create_directories(u32 opts)
	{
		u32 chk_opts =
			check_bit_options<u32>(
				opts,
				(static_cast<u32>(options_type::dir_success_if_exists)
					| static_cast<u32>(options_type::dir_merge_if_exists)
					| static_cast<u32>(options_type::dir_replace_if_exists_not_dir)
					| static_cast<u32>(options_type::dir_replace_if_exists)
					| static_cast<u32>(options_type::dir_rename_if_exists)));

		switch(chk_opts)
		{
		case 0:
		case options_type::dir_success_if_exists:
		case options_type::dir_merge_if_exists:
		case options_type::dir_replace_if_exists_not_dir:
		case options_type::dir_replace_if_exists:
		case options_type::dir_rename_if_exists:
		case options_type::dir_success_if_exists | options_type::dir_merge_if_exists:
			return true;
		default:
			return false;
		}
	}

	inline static u32 pro_s_opts_format_create_directories(u32 opts)
	{
		return replace_bit_options_if_exists<u32>(
				opts,
				static_cast<u32>(options_type::dir_merge_if_exists),
				static_cast<u32>(options_type::dir_success_if_exists));
	}

	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_create_directories(const path_type& path, const Unique_Handler& unique_handler,
									error_code_type* ec, u32 opts)
	{
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;
		typedef std::pair<file_size_type, bool> chk_op_ret_type;

		if(path.empty())
		{
			return false;
		}

		if(!this_type::pro_s_opts_chk_create_directories(opts))
		{
			assert(false); // options failed
			return false;
		}

		opts = this_type::pro_s_opts_format_create_directories(opts);

		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();

		switch(ft_path)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			return (native::call_create_directories(path, ec), this_type::pro_s_ec_chk(ec));
		case boost::filesystem::directory_file:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_replace_if_exists_not_dir)))
			{
				return false;
			}
			else
			{
				if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return true;
				}
			}
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_rename_if_exists)))
			{
				return (native::call_create_directories(unique_handler(), ec), this_type::pro_s_ec_chk(ec));
			}
			else if(has_bit_options_in<u32>(
						opts,
						static_cast<u32>(options_type::dir_replace_if_exists)
						| static_cast<u32>(options_type::dir_replace_if_exists_not_dir)) )
			{
				return (native::call_remove_all(path, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_create_directories(path, ec), this_type::pro_s_ec_chk(ec));

			}
			else
			{
				return false;
			}
		}
	}

public:
	// create_directories
	// throw exception_type, compatibility::stl_exception
	// validate options: dir_success_if_exists || dir_rename_if_exists

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		create_directories(const path_type& path, const Unique_Handler& unique_handler,
							u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		return this_type::pro_s_create_directories(path, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		create_directories(const path_type& path, const Unique_Handler& unique_handler,
							error_code_type& ec,
							u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		return this_type::pro_s_create_directories(path, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	inline static bool create_directories(const path_type& path,
											u32 opts = static_cast<u32>(options_type::dir_success_if_exists) )
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(path);
		return this_type::pro_s_create_directories(path, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool create_directories(const path_type& path, error_code_type& ec,
											u32 opts = static_cast<u32>(options_type::dir_success_if_exists) )
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(path);
		return this_type::pro_s_create_directories(path, unique_handler, boost::addressof(ec), opts);
	}

protected:
	inline static bool pro_s_create_parent_directories(const path_type& path, error_code_type* ec)
	{
		return (native::call_create_directories(this_type::parent_path(path)), this_type::pro_s_ec_chk(ec));
	}

public:
	//create_parent_directories

	// throw exception_type, compatibility::stl_exception
	inline static bool create_parent_directories(const path_type& path)
	{
		return this_type::pro_s_create_parent_directories(path, 0);
	}

	// non-throw
	inline static bool create_parent_directories(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_create_parent_directories(path, boost::addressof(ec));
	}

	//create_directory opts check
protected:
	// !!! same to pro_s_opts_chk_create_directories !!!
	//inline static bool pro_s_opts_chk_create_directory(u32 opts);

	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_create_directory(const path_type& path,
								const Unique_Handler& unique_handler,
								error_code_type* ec, u32 opts)
	{
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;
		typedef std::pair<file_size_type, bool> chk_op_ret_type;

		if(path.empty())
		{
			return false;
		}

		if(!this_type::pro_s_opts_chk_create_directories(opts))
		{
			assert(false); // options failed
			return false;
		}

		opts = this_type::pro_s_opts_format_create_directories(opts);

		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();

		switch(ft_path)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				return (native::call_create_directories(path, ec), this_type::pro_s_ec_chk(ec));
			}
			break;
		case boost::filesystem::directory_file:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_replace_if_exists_not_dir)))
			{
				return false;
			}
			else
			{
				if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return true;
				}
			}
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_rename_if_exists)))
			{
				return (native::call_create_directory(unique_handler(), ec), this_type::pro_s_ec_chk(ec));
			}
			else if(has_bit_options_in<u32>(
						opts,
						static_cast<u32>(options_type::dir_replace_if_exists)
						| static_cast<u32>(options_type::dir_replace_if_exists_not_dir)) )
			{
				return (native::call_remove_all(path, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_create_directory(path, ec), this_type::pro_s_ec_chk(ec));

			}
			else
			{
				return false;
			}
		}

		path_type prnt_path = this_type::parent_path(path);
		file_status_type sat_parent = this_type::pro_s_path_symlink_status(prnt_path, ec);

		switch(sat_parent.type())
		{
		case boost::filesystem::directory_file:
			return native::call_create_directory(path, ec) && this_type::pro_s_ec_chk(ec);
		default:
			return false;
		}
	}

public:

	//create_directory
	// throw exception_type, compatibility::stl_exception
	// validate options:
	//		dir_success_if_exists || dir_rename_if_exists,
	//		dir_create_if_not_exists

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		create_directory(const path_type& path, const Unique_Handler& unique_handler,
							u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		return this_type::pro_s_create_directory(path, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		create_directory(const path_type& path, const Unique_Handler& unique_handler,
							error_code_type& ec,
							u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		return this_type::pro_s_create_directory(path, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	inline static bool create_directory(const path_type& path,
											u32 opts = static_cast<u32>(options_type::dir_success_if_exists) )
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(path);
		return this_type::pro_s_create_directory(path, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool create_directory(const path_type& path, error_code_type& ec,
											u32 opts = static_cast<u32>(options_type::dir_success_if_exists) )
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(path);
		return this_type::pro_s_create_directory(path, unique_handler, boost::addressof(ec), opts);
	}

protected:
	inline static bool pro_s_opts_chk_rename_directory(u32 opts)
	{
		return this_type::pro_s_opts_chk_create_directories(opts);
	}

	inline static u32 pro_s_opts_format_rename_directory(u32 opts)
	{
		return this_type::pro_s_opts_format_create_directories(opts);
	}

	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_rename_directory(const path_type& old_path, const path_type& new_path,
								const Unique_Handler& unique_handler,
								error_code_type* ec,  u32 opts)
	{
		if(old_path.empty() || new_path.empty())
		{
			return false;
		}

		if(!pro_s_opts_chk_rename_directory(opts))
		{
			assert(false);
			return false;
		}

		opts = pro_s_opts_format_rename_directory(opts);

		file_status_type sat_old = this_type::pro_s_path_symlink_status(old_path, ec);
		u32 ft_old = sat_old.type();

		switch(ft_old)
		{
		case boost::filesystem::directory_file:
			break;
		default:
			return false;
		}

		if(!(this_type::pro_s_symlink_equivalent(
				this_type::parent_path(old_path), this_type::parent_path(new_path), ec)))
		{
			return false;
		}
		
		file_status_type sat_new = this_type::pro_s_path_symlink_status(new_path, ec);
		u32 ft_new = sat_new.type();

		switch(ft_new)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			return (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
		case boost::filesystem::directory_file:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_replace_if_exists_not_dir)))
			{
				return false;
			}
			else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_success_if_exists))
					|| (this_type::pro_s_symlink_equivalent_no_ec_chk(old_path, sat_old, new_path, sat_new, ec)
						&& this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_replace_if_exists))) )
			{
				return true;
			}
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_rename_if_exists)))
			{
				return (native::call_rename(old_path, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
			}
			else if(has_bit_options_in<u32>(
						opts,
						static_cast<u32>(options_type::dir_replace_if_exists)
						| static_cast<u32>(options_type::dir_replace_if_exists_not_dir)) )
			{
				return (native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
			}
			else
			{
				return false;
			}
		}

	}
public:

	// rename_directory
	// throw exception_type, compatibility::stl_exception
	// validate options: dir_rename_if_exists || dir_success_if_exists


	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		rename_directory(const path_type& old_path, const path_type& new_path,
							const Unique_Handler& unique_handler,
							u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		return this_type::pro_s_rename_directory(old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		rename_directory(const path_type& old_path, const path_type& new_path,
							const Unique_Handler& unique_handler, error_code_type& ec,
							u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		return this_type::pro_s_rename_directory(old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	inline static bool rename_directory(const path_type& old_path, const path_type& new_path, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_rename_directory(old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool rename_directory(const path_type& old_path, const path_type& new_path,
										error_code_type& ec, u32 opts = 0 )
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_rename_directory(old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

private:
	// why need _no_fmt, because this foo called pro_s_root_path_no_fmt
	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		prv_s_move_directory_no_fmt(const path_type& old_path, const file_status_type& sat_old,
									const path_type& new_path, const file_status_type& sat_new,
									const Unique_Handler& unique_handler,
									error_code_type* ec, u32 opts)
	{
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;
		typedef std::pair<file_size_type, bool> chk_op_ret_type;

		assert(old_path == native::system_complete(old_path));
		assert(new_path == native::system_complete(new_path));

		if(!pro_s_opts_chk_rename_directory(opts))
		{
			assert(false);
			return false;
		}

		opts = pro_s_opts_format_rename_directory(opts);

		//file_status_type sat_old = this_type::pro_s_path_symlink_status(old_path, ec);
		u32 ft_old = sat_old.type();

		switch(ft_old)
		{
		case boost::filesystem::directory_file:
			break;
		default:
			return false;
		}

		//file_status_type sat_new = this_type::pro_s_path_symlink_status(new_path, ec);
		u32 ft_new = sat_new.type();

		switch(ft_new)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists))
				&& native::call_is_empty(old_path, ec) && this_type::pro_s_ec_chk(ec))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(new_path.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return false;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return false;
				}
			}

			//if(this_type::pro_s_symlink_equivalent( // win old version
			//	this_type::pro_s_root_path_no_fmt(old_path),
			//	this_type::pro_s_root_path_no_fmt(new_path), ec))
			if(this_type::pro_s_symlink_is_in_same_volume(
				old_path,
				this_type::parent_path(new_path), ec))
			{
				return (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
			}
			else if(native::call_is_empty(old_path, ec) && this_type::pro_s_ec_chk(ec))
			{
#		if BOOST_VERSION < 107400
				return (native::call_copy_directory(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
						&& (native::call_remove(old_path, ec), this_type::pro_s_ec_chk(ec));
#		else
				return (native::call_copy_directory(old_path, new_path, ec) && this_type::pro_s_ec_chk(ec))
						&& (native::call_remove(old_path, ec), this_type::pro_s_ec_chk(ec));
#		endif // BOOST_VERSION < 107400
			}
			else
			{
				return false;
			}
		case boost::filesystem::directory_file:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_replace_if_exists_not_dir)))
			{
				return false;
			}
			else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_success_if_exists))
					|| (this_type::pro_s_symlink_equivalent_no_ec_chk(old_path, sat_old, new_path, sat_new, ec)
						&& this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_replace_if_exists))) )
			{
				return true;
			}
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_rename_if_exists)))
			{
				// boost::filesystem::rename not support diff vol directory

				//if(this_type::pro_s_symlink_equivalent(
				//		this_type::pro_s_root_path_no_fmt(old_path),
				//		this_type::pro_s_root_path_no_fmt(new_path), ec))
				if(this_type::pro_s_symlink_is_in_same_volume(
					old_path,
					this_type::parent_path(new_path), ec))
				{
					return (native::call_rename(old_path, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
				}
				else if(native::call_is_empty(old_path, ec) && this_type::pro_s_ec_chk(ec))
				{
#		if BOOST_VERSION < 107400
					return (native::call_copy_directory(old_path, unique_handler(), ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_remove(old_path, ec), this_type::pro_s_ec_chk(ec));
#		else
					return (native::call_copy_directory(old_path, unique_handler(), ec) &&  this_type::pro_s_ec_chk(ec))
							&& (native::call_remove(old_path, ec), this_type::pro_s_ec_chk(ec));
#		endif // BOOST_VERSION < 107400
				}
				else
				{
					return false;
				}
			}
			else if(has_bit_options_in<u32>(
						opts,
						static_cast<u32>(options_type::dir_replace_if_exists)
						| static_cast<u32>(options_type::dir_replace_if_exists_not_dir)) )
			{
				if(!(native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec)))
				{
					return false;
				}

				//if(this_type::pro_s_symlink_equivalent(
				//		this_type::pro_s_root_path_no_fmt(old_path),
				//		this_type::pro_s_root_path_no_fmt(new_path), ec))
				if(this_type::pro_s_symlink_is_in_same_volume(
					old_path,
					this_type::parent_path(new_path), ec))
				{
					return (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
				}
				else if(native::call_is_empty(old_path, ec) && this_type::pro_s_ec_chk(ec))
				{
#		if BOOST_VERSION < 107400
					return (native::call_copy_directory(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_remove(old_path, ec), this_type::pro_s_ec_chk(ec));
#		else
					return (native::call_copy_directory(old_path, new_path, ec) && this_type::pro_s_ec_chk(ec))
							&& (native::call_remove(old_path, ec), this_type::pro_s_ec_chk(ec));
#		endif //  BOOST_VERSION < 107400
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		prv_s_move_directory(const path_type& old_path, const file_status_type& sat_old,
								const path_type& new_path, const file_status_type& sat_new,
								const Unique_Handler& unique_handler,
								error_code_type* ec, u32 opts)
	{
		path_type sc_old_path;
		path_type sc_new_path;

		assert(!(old_path.empty() || new_path.empty()));

		return (sc_old_path = native::call_system_complete(old_path, ec), this_type::pro_s_ec_chk(ec))
				&& (sc_new_path = native::call_system_complete(new_path, ec), this_type::pro_s_ec_chk(ec))
				&& this_type::prv_s_move_directory_no_fmt(
					sc_old_path, sat_old, sc_new_path, sat_new, unique_handler, ec, opts);
	}

protected:

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_move_directory_no_fmt(const path_type& old_path, const path_type& new_path,
										const Unique_Handler& unique_handler,
										error_code_type* ec, u32 opts)
	{
		assert(old_path == native::system_complete(old_path));
		assert(new_path == native::system_complete(new_path));

		file_status_type sat_old = this_type::pro_s_path_symlink_status(old_path, ec);
		file_status_type sat_new = this_type::pro_s_path_symlink_status(new_path, ec);

		return this_type::prv_s_move_directory_no_fmt(
				old_path, sat_old, new_path, sat_new, unique_handler, ec, opts);
	}

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_move_directory(const path_type& old_path, const path_type& new_path,
								const Unique_Handler& unique_handler,
								error_code_type* ec, u32 opts)
	{
		path_type sc_old_path;
		path_type sc_new_path;

		if(old_path.empty() || new_path.empty())
		{
			return false;
		}

		return (sc_old_path = native::call_system_complete(old_path, ec), this_type::pro_s_ec_chk(ec))
				&& (sc_new_path = native::call_system_complete(new_path, ec), this_type::pro_s_ec_chk(ec))
				&& this_type::pro_s_move_directory_no_fmt(sc_old_path, sc_new_path, unique_handler, ec, opts);
	}

public:
	// move_directory
	// throw exception_type, compatibility::stl_exception
	// validate options: dir_rename_if_exists
	//					dir_success_if_exists
	//					dir_create_if_not_exists

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		move_directory(const path_type& old_path, const path_type& new_path,
							const Unique_Handler& unique_handler,
							u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		return this_type::pro_s_move_directory(old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		move_directory(const path_type& old_path, const path_type& new_path,
							const Unique_Handler& unique_handler, error_code_type& ec,
							u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		return this_type::pro_s_move_directory(old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	inline static bool move_directory(const path_type& old_path, const path_type& new_path, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_move_directory(old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool move_directory(const path_type& old_path, const path_type& new_path,
										error_code_type& ec, u32 opts = 0 )
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_move_directory(old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

protected:
	//copy_directory ops check
	inline static bool pro_s_opts_chk_copy_directory(u32 opts)
	{
		return this_type::pro_s_opts_chk_create_directories(opts);
	}

	inline static u32 pro_s_opts_format_copy_directory(u32 opts)
	{
		return this_type::pro_s_opts_format_create_directories(opts);
	}

private:
	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		prv_s_copy_directory(const path_type& from, const file_status_type& sat_from,
								const path_type& to, const file_status_type& sat_to,
								const Unique_Handler& unique_handler,
								error_code_type* ec, u32 opts)
	{
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;
		typedef std::pair<file_size_type, bool> chk_op_ret_type;

		assert(!from.empty() || to.empty());

		if(!pro_s_opts_chk_copy_directory(opts))
		{
			assert(false);
			return false;
		}

		opts = pro_s_opts_format_copy_directory(opts);

		u32 ft_from = sat_from.type();

		switch(ft_from)
		{
		case boost::filesystem::directory_file:
			break;
		default:
			return false;
		}

		u32 ft_to = sat_to.type();

		switch(ft_to)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(to.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return false;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return false;
				}
			}
#		if BOOST_VERSION < 107400
			return (native::call_copy_directory(from, to, ec), this_type::pro_s_ec_chk(ec));
#		else
			return (native::call_copy_directory(from, to, ec) && this_type::pro_s_ec_chk(ec));
#		endif // BOOST_VERSION < 107400
		case boost::filesystem::directory_file:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_replace_if_exists_not_dir)))
			{
				return false;
			}
			else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_success_if_exists))
					|| (this_type::pro_s_symlink_equivalent_no_ec_chk(from, sat_from, to, sat_to, ec)
						&& this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_replace_if_exists))) )
			{
				return true;
			}
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_rename_if_exists)))
			{
#		if BOOST_VERSION < 107400
				return (native::call_copy_directory(from, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
#		else
				return (native::call_copy_directory(from, unique_handler(), ec) && this_type::pro_s_ec_chk(ec));
#		endif // BOOST_VERSION < 107400
			}
			else if(has_bit_options_in<u32>(
						opts,
						static_cast<u32>(options_type::dir_replace_if_exists)
						| static_cast<u32>(options_type::dir_replace_if_exists_not_dir)) )
			{
#		if BOOST_VERSION < 107400
				return (native::call_remove_all(to, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_copy_directory(from, to, ec), this_type::pro_s_ec_chk(ec));
#		else
				return (native::call_remove_all(to, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_copy_directory(from, to, ec) && this_type::pro_s_ec_chk(ec));
#		endif //  BOOST_VERSION < 107400
			}
			else
			{
				return false;
			}
		}
	}

protected:
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_copy_directory(const path_type& from, const path_type& to,
								const Unique_Handler& unique_handler,
								error_code_type* ec, u32 opts)
	{
		if(from.empty() || to.empty())
		{
			return false;
		}

		file_status_type sat_from = this_type::pro_s_path_symlink_status(from, ec);
		file_status_type sat_to = this_type::pro_s_path_symlink_status(to, ec);

		return this_type::prv_s_copy_directory(from, sat_from, to, sat_to, unique_handler, ec, opts);
	}

public:

	// copy_directory
	// throw exception_type, compatibility::stl_exception
	// validate options:
	//		dir_success_if_exists
	//		|| dir_rename_if_exists,
	//		|| dir_merge_if_exists
	//		dir_create_if_not_exists
	//		dir_merge_if_exists is same to dir_success_if_exists

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		copy_directory(const path_type& from, const path_type& to,
						const Unique_Handler& unique_handler,
						u32 opts = static_cast<u32>(options_type::dir_rename_if_exists))
	{
		return this_type::pro_s_copy_directory(from, to, unique_handler, 0, opts);
	}

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		copy_directory(const path_type& from, const path_type& to,
						const Unique_Handler& unique_handler,
						error_code_type& ec,
						u32 opts = static_cast<u32>(options_type::dir_rename_if_exists))
	{
		return this_type::pro_s_copy_directory(from, to, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	inline static bool copy_directory(const path_type& from, const path_type& to, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(to);
		return this_type::pro_s_copy_directory(from, to, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool copy_directory(const path_type& from, const path_type& to,
								error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(to);
		return this_type::pro_s_copy_directory(from, to, unique_handler, boost::addressof(ec), opts);
	}

protected:
	inline static u32 pro_s_opts_format_recursive_copy_directories(u32 opts)
	{
		return replace_bit_options_if_exists<u32>(
				opts,
				options_type::dir_success_if_exists,
				options_type::dir_merge_if_exists);
	}

protected:
	template<typename Unique_Handler, typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			size_type
		>::type
		pro_s_recursive_copy_directories(const path_type& from, const path_type& to,
											const Unique_Handler& unique_handler,
											const Notify_Handler& notify_handler,
											error_code_type* ec, u32 opts)
	{
		opts = pro_s_opts_format_recursive_copy_directories(opts);

		return this_type::pro_s_recursive_copy(
				from, to, unique_handler, notify_handler, ec,
				opts, file_types_options_type::general_directory);
	}

public:

	//recursive_copy_directories
	//
	// throw exception_type, compatibility::stl_exception
	// validate options:
	//		dir_create_if_not_exists,
	//		dir_success_if_exists
	//		|| dir_merge_if_exists
	//		|| dir_rename_if_exists
	//		recursive_operator_ignore_if_failed

	template<typename Unique_Handler, typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			size_type
		>::type
		recursive_copy_directories(const path_type& from, const path_type& to,
									const Unique_Handler& unique_handler,
									const Notify_Handler& notify_handler,
									u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		return this_type::pro_s_recursive_copy_directories(
					from, to, unique_handler, notify_handler, 0, opts);
	}

	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				boost::mpl::bool_<!(func::foo_t_info<Unique_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_copy_directories(const path_type& from, const path_type& to,
									const Unique_Handler& unique_handler,
									u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		typedef typename default_handlers_type::recursive_copy_notify_handler_type notify_handler_type;

		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_copy_directories(
				from, to, unique_handler, notify_handler, 0, opts);
	}

	template<typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Notify_Handler>,
				boost::mpl::bool_<!!(func::foo_t_info<Notify_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_copy_directories(const path_type& from, const path_type& to,
									const Notify_Handler& notify_handler,
									u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(to);
		return this_type::pro_s_recursive_copy_directories(
				from, to, unique_handler, notify_handler, 0, opts);
	}

	template<typename Unique_Handler, typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			size_type
		>::type
		recursive_copy_directories(const path_type& from, const path_type& to,
									const Unique_Handler& unique_handler,
									const Notify_Handler& notify_handler,
									error_code_type& ec,
									u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		return this_type::pro_s_recursive_copy_directories(
					from, to, unique_handler, notify_handler,
					boost::addressof(ec), opts);
	}

	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				boost::mpl::bool_<!(func::foo_t_info<Unique_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_copy_directories(const path_type& from, const path_type& to,
									const Unique_Handler& unique_handler,
									error_code_type& ec,
									u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		typedef typename default_handlers_type::recursive_copy_notify_handler_type notify_handler_type;

		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_copy_directories(
					from, to, unique_handler, notify_handler,
					boost::addressof(ec), opts);
	}


	template<typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Notify_Handler>,
				boost::mpl::bool_<!!(func::foo_t_info<Notify_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_copy_directories(const path_type& from, const path_type& to,
									const Notify_Handler& notify_handler,
									error_code_type& ec,
									u32 opts = static_cast<u32>(options_type::dir_rename_if_exists) )
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(to);
		return this_type::pro_s_recursive_copy_directories(
					from, to, unique_handler, notify_handler,
					boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	inline static size_type recursive_copy_directories(const path_type& from, const path_type& to, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;
		typedef typename default_handlers_type::recursive_copy_notify_handler_type notify_handler_type;

		rename_handler_type unique_handler(to);
		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_copy_directories(
				from, to, unique_handler, notify_handler,
				0, opts);
	}

	// non-throw
	inline static size_type recursive_copy_directories(const path_type& from, const path_type& to,
														error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;
		typedef typename default_handlers_type::recursive_copy_notify_handler_type notify_handler_type;

		rename_handler_type unique_handler(to);
		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_copy_directories(
				from, to, unique_handler, notify_handler,
				boost::addressof(ec), opts);
	}

protected:
	inline static bool pro_s_remove_directory(const path_type& path, error_code_type* ec, u32 opts)
	{
		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		switch(sat_path.type())
		{
		case boost::filesystem::file_not_found:
			return this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::remove_success_if_not_exists));
		case boost::filesystem::directory_file:
			return native::call_remove(path, ec) && this_type::pro_s_ec_chk(ec);
		default:
			return false;
		}
	}

public:
	// if path is directory and path is empty means the path is removeable

	// validate options: remove_success_if_not_exists // if dir is not exists return true;

	// throw exception_type, compatibility::stl_exception
	inline static bool remove_directory(const path_type& path, u32 opts = 0)
	{
		return this_type::pro_s_remove_directory(path, 0, opts);
	}

	// non-throw
	inline static bool remove_directory(const path_type& path, error_code_type& ec, u32 opts = 0)
	{
		return this_type::pro_s_remove_directory(path, boost::addressof(ec), opts);
	}

protected:
	template<typename Notify_Handler> inline
	static typename boost::enable_if<is_callable<Notify_Handler>, size_type>::type
		pro_s_recursive_remove_directories(const path_type& path,
											const Notify_Handler& notify_handler,
											error_code_type* ec,
											u32 opts)
	{
		return this_type::pro_s_recursive_remove(
				path, notify_handler, 0, opts,
				file_types_options_type::general_directory);
	}

public:

	// recursive_remove_directories
	// throw exception_type, compatibility::stl_exception
	// validate options: remove_success_if_not_exists // if dir is not exists return true
	//						recursive_operator_ignore_if_failed // if recursive operator exception occurred, ignore it
	template<typename Notify_Handler> inline
	static typename boost::enable_if<is_callable<Notify_Handler>, size_type>::type
		recursive_remove_directories(const path_type& path, const Notify_Handler& notify_handler, u32 opts = 0)
	{
		return this_type::pro_s_recursive_remove_directories(path, notify_handler, 0, opts);
	}

	// non-throw
	template<typename Notify_Handler> inline
	static typename boost::enable_if<is_callable<Notify_Handler>, size_type>::type
		recursive_remove_directories(const path_type& path,
										const Notify_Handler& notify_handler,
										error_code_type& ec, u32 opts = 0)
	{
		return this_type::pro_s_recursive_remove_directories(path, notify_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	inline static size_type
		recursive_remove_directories(const path_type& path, u32 opts = 0)
	{
		typedef typename default_handlers_type::recursive_remove_notify_handler_type notify_handler_type;

		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_remove_directories(path, notify_handler, 0, opts);
	}

	// non-throw
	inline static size_type
		recursive_remove_directories(const path_type& path, error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::recursive_remove_notify_handler_type notify_handler_type;

		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_remove_directories(path, notify_handler, boost::addressof(ec), opts);
	}

public:

	// throw exception_type, compatibility::stl_exception
	inline static path_type temp_directory_path(void)
	{
		return native::temp_directory_path();
	}

	// non-throw
	inline static path_type temp_directory_path(error_code_type& ec)
	{
		return native::temp_directory_path(ec);
	}

	//----------------------files-------------------
public:

	// non-throw
	inline static bool is_regular_file(const file_status_type& s)
	{
		return native::is_regular_file(s);
	}

	inline static bool is_regular_file(const path_type& path)
	{
		return native::is_regular_file(path);
	}

	inline static bool is_regular_file(const path_type& path, error_code_type& ec)
	{
		return native::is_regular_file(path, ec);
	}

protected:
	static file_size_type pro_s_file_size(ifstream_type& ifs)
	{
		typedef typename ifstream_type::pos_type pos_type;
		typedef typename mplex::basic_t_sizeof_log2<char_type>::type char_sizeof_log2_type;
		BOOST_MPL_ASSERT_NOT((boost::is_same<char_sizeof_log2_type, mplex::basic_t_sizeof_log2_failed>));

		assert((!!ifs && ifs.is_open()));

		file_size_type size = 0;

		pos_type now_pos = ifs.tellg();
		ifs.seekg(0, std::ios_base::end);
		size = ifs.tellg();
		ifs.seekg(now_pos, std::ios_base::beg);

		return size << char_sizeof_log2_type::value;
	}

	static file_size_type pro_s_file_size_of_begin_to_current(ifstream_type& ifs)
	{
		typedef typename ifstream_type::pos_type pos_type;
		typedef typename mplex::basic_t_sizeof_log2<char_type>::type char_sizeof_log2_type;
		BOOST_MPL_ASSERT_NOT((boost::is_same<char_sizeof_log2_type, mplex::basic_t_sizeof_log2_failed>));

		assert((!!ifs && ifs.is_open()));
		file_size_type size = static_cast<file_size_type>(ifs.tellg());
		return size << char_sizeof_log2_type::value;
	}

	static file_size_type pro_s_file_size_of_current_to_end(ifstream_type& ifs)
	{
		typedef typename ifstream_type::pos_type pos_type;
		typedef typename mplex::basic_t_sizeof_log2<char_type>::type char_sizeof_log2_type;
		BOOST_MPL_ASSERT_NOT((boost::is_same<char_sizeof_log2_type, mplex::basic_t_sizeof_log2_failed>));

		assert((!!ifs && ifs.is_open()));

		file_size_type size = 0;

		pos_type now_pos = ifs.tellg();
		ifs.seekg(0, std::ios_base::end);
		size = static_cast<file_size_type>(ifs.tellg());
		ifs.seekg(now_pos, std::ios_base::beg);

		return (size - static_cast<file_size_type>(now_pos)) << char_sizeof_log2_type::value;
	}

	inline static file_size_type pro_s_file_size(const path_type& path, error_code_type* ec)
	{
		file_size_type size = native::call_file_size(path, ec);
		return this_type::pro_s_ec_chk(ec)? size : this_type::npos;
	}

public:

	// non-throw
	inline static file_size_type file_size(ifstream_type& ifs)
	{
		return !!ifs && ifs.is_open()? this_type::pro_s_file_size(ifs) : this_type::npos;
	}

	// non-throw
	inline static file_size_type file_size_of_begin_to_current(ifstream_type& ifs)
	{
		return !!ifs && ifs.is_open()?
				this_type::pro_s_file_size_of_begin_to_current(ifs)
				: this_type::npos;
	}

	// non-throw
	inline static file_size_type file_size_of_current_to_end(ifstream_type& ifs)
	{
		return !!ifs && ifs.is_open()?
				this_type::pro_s_file_size_of_current_to_end(ifs)
				: this_type::npos;
	}

	// throw exception_type, compatibility::stl_exception
	inline static file_size_type file_size(const path_type& path)
	{
		return this_type::pro_s_file_size(path, 0);
	}

	// non-throw
	inline static file_size_type file_size(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_file_size(path, boost::addressof(ec));
	}

protected:
	static file_size_type pro_s_recursive_file_size(const path_type& path, error_code_type* ec)
	{
		typedef container::unordered_set<std::size_t> eq_set_type;
		typedef yggr::tuple<boost::uintmax_t, boost::uintmax_t, std::size_t> eq_info_type;

		file_size_type fsize = 0;
		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();
		eq_set_type eq_set;

		directory_entry_type entry;
		switch(ft_path)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return this_type::npos;
		case boost::filesystem::directory_file:
			break;
		case boost::filesystem::regular_file:
			fsize = native::call_file_size(path, ec);
			return this_type::pro_s_ec_chk(ec)? fsize : this_type::npos;
		default:
			return this_type::npos;
		}

#	if BOOST_VERSION < 107200
		symlink_option_enum_type symopt = symlink_option_type::no_recurse;
#	else
		directory_options_enum_type symopt = directory_options_type::none;
#	endif //BOOST_VERSION < 107200

		recursive_directory_iterator_type dir_iter;

		try
		{
			dir_iter = this_type::pro_s_make_recursive_directory_iterator_object(path, symopt, ec);
			if(!this_type::pro_s_ec_chk(ec))
			{
				return this_type::npos;
			}
		}
		catch(const exception_type&)
		{
			return this_type::npos;
		}

		eq_info_type eq_info;
		file_size_type chk_size = 0;
		std::size_t hash_val = 0;
		for(recursive_directory_iterator_type isize; dir_iter != isize;)
		{
			try
			{
				eq_info = native::call_hard_link_count_and_size_and_eqhash(dir_iter->path(), ec);
				if(!this_type::pro_s_ec_chk(ec))
				{
					eq_info = yggr::make_tuple(
								static_cast<boost::uintmax_t>(0),
								static_cast<boost::uintmax_t>(-1),
								0);
				}
			}
			catch(const exception_type&)
			{
				eq_info = yggr::make_tuple(
								static_cast<boost::uintmax_t>(0),
								static_cast<boost::uintmax_t>(-1),
								0);
			}

			chk_size = yggr::get<1>(eq_info);

			if(chk_size != this_type::npos)
			{
				if(yggr::get<0>(eq_info) > 1)
				{
					hash_val = yggr::get<2>(eq_info);
					if(eq_set.find(hash_val) == eq_set.end())
					{
						eq_set.insert(hash_val);
						fsize += yggr::get<1>(eq_info);
					}
				}
				else
				{
					fsize += chk_size;
				}
			}

			if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
			{
				break;
			}
		}

		return fsize;
	}

public:
	// recursive_file_size

	// throw exception_type, compatibility::stl_exception
	inline static file_size_type recursive_file_size(const path_type& path)
	{
		return this_type::pro_s_recursive_file_size(path, 0);
	}

	// non-throw
	inline static file_size_type recursive_file_size(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_recursive_file_size(path, boost::addressof(ec));
	}

protected:
	inline static bool pro_s_opts_chk_create_file(u32 opts)
	{
		switch(check_bit_options(
				opts,
				static_cast<u32>(options_type::file_success_if_exists)
				| static_cast<u32>(options_type::file_replace_if_exists_not_dir)
				| static_cast<u32>(options_type::file_replace_if_exists)
				| static_cast<u32>(options_type::file_rename_if_exists)) )
		{
		case 0:
		case static_cast<u32>(options_type::file_success_if_exists):
		case static_cast<u32>(options_type::file_replace_if_exists_not_dir):
		case static_cast<u32>(options_type::file_replace_if_exists):
		case static_cast<u32>(options_type::file_rename_if_exists):
			return true;
		default:
			return false;
		}
	}

	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, file_size_type>::type
		pro_s_create_file_of_binary(const path_type& path, file_size_type size,
										const Unique_Handler& unique_handler,
										error_code_type* ec,
										u32 opts)
	{
		typedef typename
			mplex::typename_replaceor
			<
				fbuf_type,
				char,
				typename
					mplex::typename_replaceor
					<
						typename fbuf_type::traits_type,
						char
					>::type
			>::type now_fbuf_type;

		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;
		typedef std::pair<file_size_type, bool> chk_op_ret_type;

		if(path.empty())
		{
			return this_type::npos;
		}

		if(!this_type::pro_s_opts_chk_create_file(opts))
		{
			assert(false); // options error
			return this_type::npos;
		}

		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();

		now_fbuf_type fbuf;
		now_fbuf_type* pbuf = 0;

		switch(ft_path)
		{
		case boost::filesystem::status_error:
			return this_type::npos;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_file_path_and_name(path.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return this_type::npos;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return this_type::npos;
				}
			}
			pbuf = fbuf.open(YGGR_FILESYSTEM_C_STR(path),
								std::ios_base::in
								| std::ios_base::out
								| std::ios_base::trunc
								| std::ios_base::binary);
			break;
		default:
			if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists_not_dir))
					&& ft_path != boost::filesystem::directory_file)
				|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists)))
			{
				if(!(native::call_remove_all(path, ec), this_type::pro_s_ec_chk(ec)))
				{
					return this_type::npos;
				}

				pbuf = fbuf.open(YGGR_FILESYSTEM_C_STR(path),
									std::ios_base::in
									| std::ios_base::out
									| std::ios_base::trunc
									| std::ios_base::binary);
			}
			else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_success_if_exists)))
			{
				return ft_path == boost::filesystem::regular_file?
						this_type::pro_s_file_size(path, ec) : this_type::npos;
			}
			else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_rename_if_exists)))
			{
				pbuf = fbuf.open(YGGR_FILESYSTEM_C_STR(unique_handler()),
									std::ios_base::in
									| std::ios_base::out
									| std::ios_base::trunc
									| std::ios_base::binary);
			}
			else
			{
				return this_type::npos;
			}
		}

		if(!pbuf)
		{
			return this_type::npos;
		}

		if(size)
		{
			fbuf.pubseekoff(size - 1, std::ios_base::beg);
			fbuf.sputc(char(0));
		}
		fbuf.close();

		return size;
	}

	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, file_size_type>::type
		pro_s_create_file_of_text(const path_type& path,
									file_size_type size,
									const Unique_Handler& unique_handler,
									error_code_type* ec,
									u32 opts)
	{
		typedef typename mplex::basic_t_sizeof_log2<char_type>::type char_sizeof_log2_type;
		BOOST_MPL_ASSERT_NOT((boost::is_same<char_sizeof_log2_type, mplex::basic_t_sizeof_log2_failed>));

		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;
		typedef std::pair<file_size_type, bool> chk_op_ret_type;

		if(path.empty())
		{
			return this_type::npos;
		}

		if(!this_type::pro_s_opts_chk_create_file(opts))
		{
			assert(false); // options error
			return this_type::npos;
		}

		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();

		fbuf_type fbuf;
		fbuf_type* pbuf = 0;

		switch(ft_path)
		{
		case boost::filesystem::status_error:
			return this_type::npos;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_file_path_and_name(path.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return this_type::npos;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return this_type::npos;
				}
			}

			pbuf = fbuf.open(YGGR_FILESYSTEM_C_STR(path),
								std::ios_base::in
								| std::ios_base::out
								| std::ios_base::trunc
								| std::ios_base::binary);
			break;
		default:
			if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists_not_dir))
					&& ft_path != boost::filesystem::directory_file)
				|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists)))
			{
				if(!(native::call_remove_all(path, ec), this_type::pro_s_ec_chk(ec)))
				{
					return this_type::npos;
				}

				pbuf = fbuf.open(YGGR_FILESYSTEM_C_STR(path),
									std::ios_base::in
									| std::ios_base::out
									| std::ios_base::trunc
									| std::ios_base::binary);
			}
			else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_success_if_exists)))
			{
				return ft_path == boost::filesystem::regular_file?
						this_type::pro_s_file_size(path, ec) : this_type::npos;
			}
			else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_rename_if_exists)))
			{
				pbuf = fbuf.open(YGGR_FILESYSTEM_C_STR(unique_handler()),
									std::ios_base::in
									| std::ios_base::out
									| std::ios_base::trunc
									| std::ios_base::binary);
			}
			else
			{
				return this_type::npos;
			}
		}

		if(!pbuf)
		{
			return this_type::npos;
		}

		if(size)
		{
			fbuf.pubseekoff(size - 1, std::ios_base::beg);
			fbuf.sputc(char_type(0));
		}

		fbuf.close();
		return size << char_sizeof_log2_type::value;
	}

public:

	// create_file_of_binary
	// validate options:
	//		dir_create_if_not_exists, // if the file's parent path not exists create it
	//		file_success_if_exists // if the file exists, return the file's size, else create the file
	//		|| file_replace_if_exists_not_dir // if the file exists, create a new file with a unique rule file name
	//		|| file_replace_if_exists //if the file exists, create a new file with a unique rule file name
	//		|| file_rename_if_exists  // if the file is exists create a new file.
	//
	// warning: the parameter size in create_file_of_binary is byte_size
	// example: create_file_of_binary(path, 1024); the file size is 1024 byte

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, file_size_type>::type
		create_file_of_binary(const path_type& path, file_size_type size,
								const Unique_Handler& unique_handler,
								u32 opts = static_cast<u32>(options_type::file_rename_if_exists))
	{
		return this_type::pro_s_create_file_of_binary(path, size, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, file_size_type>::type
		create_file_of_binary(const path_type& path, file_size_type size,
								const Unique_Handler& unique_handler,
								error_code_type& ec,
								u32 opts = static_cast<u32>(options_type::file_rename_if_exists))
	{
		return this_type::pro_s_create_file_of_binary(path, size, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	inline static file_size_type
		create_file_of_binary(const path_type& path, file_size_type size, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type unique_handler_type;

		unique_handler_type unique_handler(path);
		return this_type::pro_s_create_file_of_binary(path, size, unique_handler, 0, opts);
	}

	// non-throw
	inline static file_size_type
		create_file_of_binary(const path_type& path, file_size_type size, error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type unique_handler_type;

		unique_handler_type unique_handler(path);
		return this_type::pro_s_create_file_of_binary(path, size, unique_handler, boost::addressof(ec), opts);
	}

public:
	// create_file_of_text
	// throw exception_type, compatibility::stl_exception
	// validate options:
	//		dir_create_if_not_exists, // if the file's parent path not exists create it
	//		file_success_if_exists // if the file exists, return the file's size, else create the file
	//		|| file_replace_if_exists_not_dir // if the file exists, create a new file with a unique rule file name
	//		|| file_replace_if_exists //if the file exists, create a new file with a unique rule file name
	//		|| file_rename_if_exists  // if the file is exists create a new file.
	//
	// warning: the parameter "size" in create_file_of_text is "sizeof(char_type) * size" is not byte_size,
	//			the return value is byte_size
	// example: create_file_of_text(path, 1024); the file size is sizeof(char_type) * 1024 byte,
	//			char_type see fstream_cfg_type (assert(sizeof(char_type) * 1024 == create_file_of_text(path, 1024)));

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, file_size_type>::type
		create_file_of_text(const path_type& path, file_size_type size,
							const Unique_Handler& unique_handler,
							u32 opts = static_cast<u32>(options_type::file_rename_if_exists))
	{
		return this_type::pro_s_create_file_of_text(path, size, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, file_size_type>::type
		create_file_of_text(const path_type& path, file_size_type size,
								const Unique_Handler& unique_handler,
								error_code_type& ec,
								u32 opts = static_cast<u32>(options_type::file_rename_if_exists))
	{
		return this_type::pro_s_create_file_of_text(path, size, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	inline static file_size_type
		create_file_of_text(const path_type& path, file_size_type size, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type unique_handler_type;

		unique_handler_type unique_handler(path);
		return this_type::pro_s_create_file_of_text(path, size, unique_handler, 0, opts);
	}

	// non-throw
	inline static file_size_type
		create_file_of_text(const path_type& path, file_size_type size, error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type unique_handler_type;

		unique_handler_type unique_handler(path);
		return this_type::pro_s_create_file_of_text(path, size, unique_handler, boost::addressof(ec), opts);
	}

protected:
	// pro_s_write_file_of_binary
	static file_size_type
		pro_s_write_file_of_binary(ofstream_type& ofs, const void* buffer, file_size_type size)
	{
		assert(buffer);
		if(!size)
		{
			return 0;
		}
		if(!(buffer && !!ofs && ofs.is_open()))
		{
			return this_type::npos;
		}

		ofs.write(reinterpret_cast<const char*>(buffer), size);
		return !ofs? this_type::npos : size;
	}

private:
	static bool prv_s_write_file_path_check_and_fix(const path_type& path, error_code_type* ec, u32 opts)
	{
		typedef typename default_handlers_type::rename_handler_type unique_handler_type;
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;

		if(path.empty())
		{
			return false;
		}

		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();

		switch(ft_path)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::directory_file:
		case boost::filesystem::symlink_file:
		case boost::filesystem::type_unknown:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(path.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return false;
				}

				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return false;
				}
			}
		default:
			return true;
		}
	}

	static file_size_type
		prv_s_write_file_of_binary_detail(const path_type& path,
											std::ios_base::openmode mode,
											const void* buffer,
											file_size_type size,
											error_code_type* ec,
											u32 opts)
	{
		assert(buffer);
		if(!size)
		{
			return 0;
		}

		if(!(buffer && this_type::prv_s_write_file_path_check_and_fix(path, ec, opts) ))
		{
			return this_type::npos;
		}

		ofstream_type ofs;
		ofs.open(YGGR_FILESYSTEM_C_STR(path),
					replace_bit_options<std::ios_base::openmode>(
						mode, std::ios::in, std::ios::out | std::ios::binary));

		if(!ofs)
		{
			if(ofs.is_open())
			{
				ofs.close();
			}
			return this_type::npos;
		}
		else
		{
			file_size_type ret_size = this_type::pro_s_write_file_of_binary(ofs, buffer, size);
			ofs.close();
			return ret_size;
		}
	}

protected:
	inline static file_size_type
		pro_s_write_file_of_binary(const path_type& path,
									const void* buffer,
									file_size_type size,
									error_code_type* ec,
									u32 opts)
	{
		return this_type::prv_s_write_file_of_binary_detail(
					path, std::ios::out | std::ios::binary, buffer, size, ec, opts);
	}

public:
	// write_file_of_binary
	// write_file_xxx ofstream version not call flush,
	// if you want to flush, please call ofs.flush() to your self

	// non-throw
	inline static file_size_type
		write_file_of_binary(ofstream_type& ofs, const void* buffer, file_size_type size)
	{
		return this_type::pro_s_write_file_of_binary(ofs, buffer, size);
	}

	// container_version
	// non-throw
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			is_random_access_container<Container>,
			file_size_type
		>::type
		write_file_of_binary(ofstream_type& ofs, const Container& cont)
	{
		file_size_type size = bytes::byte_size(cont);

		return size? this_type::pro_s_write_file_of_binary(ofs, &(*boost::begin(cont)), size) : 0;
	}

	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					is_random_access_container<Container>
				>
			>,
			file_size_type
		>::type
		write_file_of_binary(ofstream_type& ofs, const Container& cont)
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(boost::begin(cont), boost::end(cont));
		file_size_type size = bytes::byte_size(vt);

		return size? this_type::pro_s_write_file_of_binary(ofs, &vt[0], size) : 0;
	}

	// iterator version
	// non-throw
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_reverse_iterator<Iter> >,
				is_random_access_iterator<Iter>
			>,
			file_size_type
		>::type
		write_file_of_binary(ofstream_type& ofs, Iter s, Iter e)
	{
		file_size_type size = bytes::byte_size(s, e);

		return size? this_type::pro_s_write_file_of_binary(ofs, &(*s), size) :0;
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				is_reverse_iterator<Iter>,
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_random_access_iterator<Iter> >
				>
			>,
			file_size_type
		>::type
		write_file_of_binary(ofstream_type& ofs, Iter s, Iter e)
	{
		typedef Iter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(s, e);
		file_size_type size = bytes::byte_size(vt);

		return size? this_type::pro_s_write_file_of_binary(ofs, &vt[0], size) : 0;
	}

	// validate options: dir_create_if_not_exists, // if the file's parent path not exists create it
	// throw exception_type, compatibility::stl_exception
	inline static file_size_type
		write_file_of_binary(const path_type& path, const void* buffer,
								file_size_type size, u32 opts = 0)
	{
		return this_type::pro_s_write_file_of_binary(
					path, buffer, size, static_cast<error_code_type*>(0), opts);
	}

	// non-throw
	inline static file_size_type
		write_file_of_binary(const path_type& path, const void* buffer,
								file_size_type size, error_code_type& ec, u32 opts = 0)
	{
		return this_type::pro_s_write_file_of_binary(path, buffer, size, boost::addressof(ec), opts);
	}

	// container version
	// throw exception_type, compatibility::stl_exception
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			is_random_access_container<Container>,
			file_size_type
		>::type
		write_file_of_binary(const path_type& path, const Container& cont, u32 opts = 0)
	{
		file_size_type size = bytes::byte_size(cont);

		return size?
				this_type::pro_s_write_file_of_binary(
					path, &(*boost::begin(cont)), size, static_cast<error_code_type*>(0), opts) : 0;
	}

	// non-throw
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			is_random_access_container<Container>,
			file_size_type
		>::type
		write_file_of_binary(const path_type& path, const Container& cont, error_code_type& ec, u32 opts = 0)
	{
		file_size_type size = bytes::byte_size(cont);

		return size?
				this_type::pro_s_write_file_of_binary(
					path, &(*boost::begin(cont)), size, boost::addressof(ec), opts) : 0;
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					is_random_access_container<Container>
				>
			>,
			file_size_type
		>::type
		write_file_of_binary(const path_type& path, const Container& cont, u32 opts = 0)
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(boost::begin(cont), boost::end(cont));
		file_size_type size = bytes::byte_size(vt);

		return size?
				this_type::pro_s_write_file_of_binary(
					path, &vt[0], size, static_cast<error_code_type*>(0), opts) : 0;
	}

	// non-throw
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					is_random_access_container<Container>
				>
			>,
			file_size_type
		>::type
		write_file_of_binary(const path_type& path, const Container& cont, error_code_type& ec, u32 opts = 0)
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(boost::begin(cont), boost::end(cont));
		file_size_type size = bytes::byte_size(vt);

		return size?
				this_type::pro_s_write_file_of_binary(
					path, &vt[0], size, boost::addressof(ec), opts) : 0;
	}

	// iterator_version
	// throw exception_type, compatibility::stl_exception
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_reverse_iterator<Iter> >,
				is_random_access_iterator<Iter>
			>,
			file_size_type
		>::type
		write_file_of_binary(const path_type& path, Iter s, Iter e, u32 opts = 0)
	{
		file_size_type size = bytes::byte_size(s, e);

		return size?
				this_type::pro_s_write_file_of_binary(
					path, &(*s), size, static_cast<error_code_type*>(0), opts) : 0;
	}

	// non-throw
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_reverse_iterator<Iter> >,
				is_random_access_iterator<Iter>
			>,
			file_size_type
		>::type
		write_file_of_binary(const path_type& path, Iter s, Iter e, error_code_type& ec, u32 opts = 0)
	{
		file_size_type size = bytes::byte_size(s, e);

		return size?
				this_type::pro_s_write_file_of_binary(
					path, &(*s), size, boost::addressof(ec), opts) : 0;
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				is_reverse_iterator<Iter>,
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_random_access_iterator<Iter> >
				>
			>,
			file_size_type
		>::type
		write_file_of_binary(const path_type& path, Iter s, Iter e, u32 opts = 0)
	{
		typedef Iter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(s, e);
		file_size_type size = bytes::byte_size(vt);

		return size?
				this_type::pro_s_write_file_of_binary(
					path, &vt[0], size, static_cast<error_code_type*>(0), opts) : 0;
	}

	// non-throw
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				is_reverse_iterator<Iter>,
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_random_access_iterator<Iter> >
				>
			>,
			file_size_type
		>::type
		write_file_of_binary(const path_type& path, Iter s, Iter e, error_code_type& ec, u32 opts = 0)
	{
		typedef Iter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(s, e);
		file_size_type size = bytes::byte_size(vt);

		return size? this_type::pro_s_write_file_of_binary(
					path, &vt[0], size, boost::addressof(ec), opts) : 0;
	}

protected:
	// pro_s_write_file_of_text
	template<typename T, typename Spr> inline
	static typename boost::enable_if< is_fsys_text_separator<Spr>, bool>::type
		pro_s_write_file_of_text(ofstream_type& ofs, const T& val, const Spr& spr)
	{
		if(!(!!ofs && ofs.is_open()))
		{
			return false;
		}
		else
		{
			ofs << val << spr;
			return !!ofs;
		}
	}

	template<typename Iter,  typename Spr>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		pro_s_write_file_of_text(ofstream_type& ofs, Iter val_s, Iter val_e, const Spr& spr)
	{
		if(!(!!ofs && ofs.is_open()))
		{
			return val_s;
		}
		else
		{
			for(; (val_s != val_e) && !!ofs; ++val_s)
			{
				ofs << *val_s << spr;
			}

			return val_s;
		}
	}

private:
	template<typename T, typename Spr>
	static typename boost::enable_if< is_fsys_text_separator<Spr>, bool>::type
		prv_s_write_file_of_text_detail(const path_type& path,
											std::ios_base::openmode mode,
											const T& val,
											const Spr& spr,
											error_code_type* ec,
											u32 opts)
	{
		if(!this_type::prv_s_write_file_path_check_and_fix(path, ec, opts))
		{
			return false;
		}

		ofstream_type ofs;
		ofs.open(YGGR_FILESYSTEM_C_STR(path),
					replace_bit_options_if_in<std::ios_base::openmode>(
						mode, std::ios::in | std::ios::binary, std::ios::out));

		if(!ofs)
		{
			if(ofs.is_open())
			{
				ofs.close();
			}
			return false;
		}
		else
		{
			bool bret = this_type::pro_s_write_file_of_text(ofs, val, spr);
			ofs.close();
			return bret;
		}
	}

	template<typename Iter, typename Spr>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		prv_s_write_file_of_text_detail(const path_type& path,
										std::ios_base::openmode mode,
										Iter val_s, Iter val_e,
										const Spr& spr,
										error_code_type* ec,
										u32 opts)
	{
		if(!this_type::prv_s_write_file_path_check_and_fix(path, ec, opts))
		{
			return val_s;
		}

		ofstream_type ofs;
		ofs.open(YGGR_FILESYSTEM_C_STR(path),
					replace_bit_options_if_in<std::ios_base::openmode>(
						mode, std::ios::in | std::ios::binary, std::ios::out));

		if(!ofs)
		{
			if(ofs.is_open())
			{
				ofs.close();
			}
			return val_s;
		}
		else
		{
			Iter ret = this_type::pro_s_write_file_of_text(ofs, val_s, val_e, spr);
			ofs.close();
			return ret;
		}
	}

protected:
	template<typename T, typename Spr> inline
	static typename boost::enable_if<is_fsys_text_separator<Spr>, bool>::type
		pro_s_write_file_of_text(const path_type& path, const T& val,
									const Spr& spr, error_code_type* ec,
									u32 opts)
	{
		return this_type::prv_s_write_file_of_text_detail(
					path, std::ios::out, val, spr, ec, opts);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		pro_s_write_file_of_text(const path_type& path,
									Iter val_s, Iter val_e,
									const Spr& spr,
									error_code_type* ec,
									u32 opts)
	{
		return this_type::prv_s_write_file_of_text_detail(
				path, std::ios::out, val_s, val_e, spr, ec, opts);
	}

public:
	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!!!	warning:																!!!
	!!!		"write_file_of_text" and "read_file_of_text" is not serialzation!	!!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/

	// write_file_of_text
	// non-throw
	template<typename T> inline
	static bool write_file_of_text(ofstream_type& ofs, const T& val)
	{
		default_text_spr_type spr;
		return this_type::pro_s_write_file_of_text(ofs, val, spr);
	}

	template<typename T, typename Spr> inline
	static typename boost::enable_if< is_fsys_text_separator<Spr>, bool >::type
		write_file_of_text(ofstream_type& ofs, const T& val, const Spr& spr)
	{
		return this_type::pro_s_write_file_of_text(ofs, val, spr);
	}

	// iterator version
	template<typename Iter> inline
	static typename boost::enable_if<is_iterator<Iter>, Iter>::type
		write_file_of_text(ofstream_type& ofs, Iter val_s, Iter val_e)
	{
		default_text_spr_type spr;
		return this_type::pro_s_write_file_of_text(ofs, val_s, val_e, spr);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		write_file_of_text(ofstream_type& ofs, Iter val_s, Iter val_e, const Spr& spr)
	{
		return this_type::pro_s_write_file_of_text(ofs, val_s, val_e, spr);
	}

	// validate options: dir_create_if_not_exists, // if the file's parent path not exists create it
	// throw exception_type, compatibility::stl_exception
	template<typename T> inline
	static bool write_file_of_text(const path_type& path, const T& val, u32 opts = 0)
	{
		default_text_spr_type spr;
		return this_type::pro_s_write_file_of_text(
				path, val, spr, static_cast<error_code_type*>(0), opts);
	}

	// non-throw
	template<typename T> inline
	static bool write_file_of_text(const path_type& path, const T& val, error_code_type& ec, u32 opts = 0)
	{
		default_text_spr_type spr;
		return this_type::pro_s_write_file_of_text(
				path, val, spr, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename T, typename Spr> inline
	static typename boost::enable_if<is_fsys_text_separator<Spr>, bool >::type
		write_file_of_text(const path_type& path, const T& val, const Spr& spr, u32 opts = 0)
	{
		return this_type::pro_s_write_file_of_text(
				path, val, spr, static_cast<error_code_type*>(0), opts);
	}

	// non-throw
	template<typename T, typename Spr> inline
	static typename boost::enable_if<is_fsys_text_separator<Spr>, bool >::type
		write_file_of_text(const path_type& path, const T& val, const Spr& spr,
							error_code_type& ec, u32 opts = 0)
	{
		return this_type::pro_s_write_file_of_text(
				path, val, spr, boost::addressof(ec), opts);
	}

	// iterator version
	// throw exception_type, compatibility::stl_exception
	template<typename Iter> inline
	static typename boost::enable_if<is_iterator<Iter>, Iter>::type
		write_file_of_text(const path_type& path, Iter val_s, Iter val_e, u32 opts = 0)
	{
		default_text_spr_type spr;
		return this_type::pro_s_write_file_of_text(
				path, val_s, val_e, spr, static_cast<error_code_type*>(0), opts);
	}

	template<typename Iter> inline
	static typename boost::enable_if<is_iterator<Iter>, Iter>::type
		write_file_of_text(const path_type& path, Iter val_s, Iter val_e,
							error_code_type& ec, u32 opts = 0)
	{
		default_text_spr_type spr;
		return this_type::pro_s_write_file_of_text(
				path, val_s, val_e, spr, boost::addressof(ec), opts);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		write_file_of_text(const path_type& path, Iter val_s, Iter val_e,
							const Spr& spr, u32 opts = 0)
	{
		return this_type::pro_s_write_file_of_text(
				path, val_s, val_e, spr, static_cast<error_code_type*>(0), opts);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		write_file_of_text(const path_type& path, Iter val_s, Iter val_e,
							const Spr& spr, error_code_type& ec, u32 opts = 0)
	{
		return this_type::pro_s_write_file_of_text(
				path, val_s, val_e, spr, boost::addressof(ec), opts);
	}

protected:
	// pro_s_append_file_of_binary
	inline static file_size_type
		pro_s_append_file_of_binary(ofstream_type& ofs, const void* buffer, file_size_type size)
	{
		return this_type::pro_s_write_file_of_binary(ofs, buffer, size);
	}

	inline static file_size_type
		pro_s_append_file_of_binary(const path_type& path,
									const void* buffer,
									file_size_type size,
									error_code_type* ec,
									u32 opts)
	{
		return this_type::prv_s_write_file_of_binary_detail(
					path, std::ios::out | std::ios::binary | std::ios::app,
					buffer, size, ec, opts);
	}

	// pro_s_append_file_of_text
	template<typename T, typename Spr> inline
	static typename boost::enable_if< is_fsys_text_separator<Spr>, bool>::type
		pro_s_append_file_of_text(ofstream_type& ofs, const T& val, const Spr& spr)
	{
		return this_type::pro_s_write_file_of_text(ofs, val, spr);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		pro_s_append_file_of_text(ofstream_type& ofs, Iter val_s, Iter val_e, const Spr& spr)
	{
		return this_type::pro_s_write_file_of_text(ofs, val_s, val_e, spr);
	}

	template<typename T, typename Spr> inline
	static typename boost::enable_if< is_fsys_text_separator<Spr>, bool >::type
		pro_s_append_file_of_text(const path_type& path, const T& val,
									const Spr& spr, error_code_type* ec, u32 opts)
	{
		return this_type::prv_s_write_file_of_text_detail(
				path, std::ios::out | std::ios::app, val, spr, ec, opts);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		pro_s_append_file_of_text(const path_type& path, Iter val_s, Iter val_e,
									const Spr& spr, error_code_type* ec,
									u32 opts)
	{
		return this_type::prv_s_write_file_of_text_detail(
					path, std::ios::out | std::ios::app,
					val_s, val_e, spr, ec, opts);
	}

public:
	// append_file_of_binary
	// append_file_xxx ofstream version not call flush,
	// if you want to flush, please call ofs.flush() to your self

	// non-throw
	inline static file_size_type
		append_file_of_binary(ofstream_type& ofs, const void* buffer, file_size_type size)
	{
		return this_type::pro_s_append_file_of_binary(ofs, buffer, size);
	}

	// container_version
	// non-throw
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			is_random_access_container<Container>,
			file_size_type
		>::type
		append_file_of_binary(ofstream_type& ofs, const Container& cont)
	{
		file_size_type size = bytes::byte_size(cont);

		return size?
				this_type::pro_s_append_file_of_binary(ofs, &(*boost::begin(cont)), size) : 0;
	}

	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					is_random_access_container<Container>
				>
			>,
			file_size_type
		>::type
		append_file_of_binary(ofstream_type& ofs, const Container& cont)
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(boost::begin(cont), boost::end(cont));
		file_size_type size = bytes::byte_size(vt);

		return size? this_type::pro_s_append_file_of_binary(ofs, &vt[0], size) : 0;
	}

	// iterator version
	// non-throw
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_reverse_iterator<Iter> >,
				is_random_access_iterator<Iter>
			>,
			file_size_type
		>::type
		append_file_of_binary(ofstream_type& ofs, Iter s, Iter e)
	{
		file_size_type size = bytes::byte_size(s, e);

		return size? this_type::pro_s_append_file_of_binary(ofs, &(*s), size) : 0;
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				is_reverse_iterator<Iter>,
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_random_access_iterator<Iter> >
				>
			>,
			file_size_type
		>::type
		append_file_of_binary(ofstream_type& ofs, Iter s, Iter e)
	{
		typedef Iter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(s, e);
		file_size_type size = bytes::byte_size(vt);

		return size? this_type::pro_s_append_file_of_binary(ofs, &vt[0], size) : 0;
	}

	// validate options: dir_create_if_not_exists, // if the file's parent path not exists create it
	// throw exception_type, compatibility::stl_exception
	inline static file_size_type
		append_file_of_binary(const path_type& path, const void* buffer,
								file_size_type size, u32 opts = 0)
	{
		return this_type::pro_s_append_file_of_binary(
					path, buffer, size, static_cast<error_code_type*>(0), opts);
	}

	// non-throw
	inline static file_size_type
		append_file_of_binary(const path_type& path, const void* buffer,
								file_size_type size, error_code_type& ec, u32 opts = 0)
	{
		return this_type::pro_s_append_file_of_binary(path, buffer, size, boost::addressof(ec), opts);
	}

	// container version
	// throw exception_type, compatibility::stl_exception
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			is_random_access_container<Container>,
			file_size_type
		>::type
		append_file_of_binary(const path_type& path, const Container& cont, u32 opts = 0)
	{
		file_size_type size = bytes::byte_size(cont);

		return size?
				this_type::pro_s_append_file_of_binary(
					path, &(*boost::begin(cont)), size, static_cast<error_code_type*>(0), opts) : 0;
	}

	// non-throw
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			is_random_access_container<Container>,
			file_size_type
		>::type
		append_file_of_binary(const path_type& path, const Container& cont, error_code_type& ec, u32 opts = 0)
	{
		file_size_type size = bytes::byte_size(cont);

		return size?
				this_type::pro_s_append_file_of_binary(
					path, &(*boost::begin(cont)), size, boost::addressof(ec), opts) : 0;
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					is_random_access_container<Container>
				>
			>,
			file_size_type
		>::type
		append_file_of_binary(const path_type& path, const Container& cont, u32 opts = 0)
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(boost::begin(cont), boost::end(cont));
		file_size_type size = bytes::byte_size(vt);

		return size?
				this_type::pro_s_append_file_of_binary(
					path, &vt[0], size, static_cast<error_code_type*>(0), opts) : 0;
	}

	// non-throw
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					is_random_access_container<Container>
				>
			>,
			file_size_type
		>::type
		append_file_of_binary(const path_type& path, const Container& cont, error_code_type& ec, u32 opts = 0)
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(boost::begin(cont), boost::end(cont));
		file_size_type size = bytes::byte_size(vt);

		return size?
				this_type::pro_s_append_file_of_binary(
					path, &vt[0], size, boost::addressof(ec), opts) : 0;
	}

	// iterator_version
	// throw exception_type, compatibility::stl_exception
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_reverse_iterator<Iter> >,
				is_random_access_iterator<Iter>
			>,
			file_size_type
		>::type
		append_file_of_binary(const path_type& path, Iter s, Iter e, u32 opts = 0)
	{
		file_size_type size = bytes::byte_size(s, e);

		return size?
				this_type::pro_s_append_file_of_binary(
					path, &(*s), size, static_cast<error_code_type*>(0), opts) : 0;
	}

	// non-throw
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_reverse_iterator<Iter> >,
				is_random_access_iterator<Iter>
			>,
			file_size_type
		>::type
		append_file_of_binary(const path_type& path, Iter s, Iter e, error_code_type& ec, u32 opts = 0)
	{
		file_size_type size = bytes::byte_size(s, e);

		return size?
				this_type::pro_s_append_file_of_binary(
					path, &(*s), size, boost::addressof(ec), opts) : 0;
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				is_reverse_iterator<Iter>,
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_random_access_iterator<Iter> >
				>
			>,
			file_size_type
		>::type
		append_file_of_binary(const path_type& path, Iter s, Iter e, u32 opts = 0)
	{
		typedef Iter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(s, e);
		file_size_type size = bytes::byte_size(vt);

		return size?
				this_type::pro_s_append_file_of_binary(
					path, &vt[0], size, static_cast<error_code_type*>(0), opts) : 0;
	}

	// non-throw
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::or_
			<
				is_reverse_iterator<Iter>,
				boost::mpl::and_
				<
					is_iterator<Iter>,
					boost::mpl::not_< is_random_access_iterator<Iter> >
				>
			>,
			file_size_type
		>::type
		append_file_of_binary(const path_type& path, Iter s, Iter e, error_code_type& ec, u32 opts = 0)
	{
		typedef Iter iter_type;
		typedef typename boost::iterator_value<iter_type>::type val_type;
		typedef container::vector<val_type> vt_type;

		vt_type vt(s, e);
		file_size_type size = bytes::byte_size(vt);

		return size?
				this_type::pro_s_append_file_of_binary(
					path, &vt[0], size, boost::addressof(ec), opts) : 0;
	}

public:
	// write_file_of_text
	// non-throw
	template<typename T> inline
	static bool append_file_of_text(ofstream_type& ofs, const T& val)
	{
		default_text_spr_type spr;
		return this_type::pro_s_append_file_of_text(ofs, val, spr, 0);
	}

	template<typename T, typename Spr> inline
	static typename boost::enable_if< is_fsys_text_separator<Spr>, bool>::type
		append_file_of_text(ofstream_type& ofs, const T& val, const Spr& spr)
	{
		return this_type::pro_s_append_file_of_text(ofs, val, spr);
	}

	// iterator version
	template<typename Iter> inline
	static typename boost::enable_if<is_iterator<Iter>, Iter>::type
		append_file_of_text(ofstream_type& ofs, Iter val_s, Iter val_e)
	{
		default_text_spr_type spr;
		return this_type::pro_s_append_file_of_text(
				ofs, val_s, val_e, spr, 0);
	}

	template<typename Iter, typename Spr> inline
	static typename boost::enable_if<is_iterator<Iter>, Iter>::type
		append_file_of_text(ofstream_type& ofs, Iter val_s, Iter val_e, const Spr& spr)
	{
		return this_type::pro_s_append_file_of_text(ofs, val_s, val_e, spr);
	}

	// validate options: dir_create_if_not_exists, // if the file's parent path not exists create it
	// throw exception_type, compatibility::stl_exception
	template<typename T> inline
	static bool append_file_of_text(const path_type& path, const T& val, u32 opts = 0)
	{
		default_text_spr_type spr;
		return this_type::pro_s_append_file_of_text(
				path, val, spr, static_cast<error_code_type*>(0), opts);
	}

	// non-throw
	template<typename T> inline
	static bool append_file_of_text(const path_type& path, const T& val, error_code_type& ec, u32 opts = 0)
	{
		default_text_spr_type spr;
		return this_type::pro_s_append_file_of_text(
				path, val, spr, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename T, typename Spr> inline
	static typename boost::enable_if<is_fsys_text_separator<Spr>, bool >::type
		append_file_of_text(const path_type& path, const T& val, const Spr& spr, u32 opts = 0)
	{
		return this_type::pro_s_append_file_of_text(
				path, val, spr, static_cast<error_code_type*>(0), opts);
	}

	// non-throw
	template<typename T, typename Spr> inline
	static typename boost::enable_if<is_fsys_text_separator<Spr>, bool >::type
		append_file_of_text(const path_type& path, const T& val, const Spr& spr,
							error_code_type& ec, u32 opts = 0)
	{
		return this_type::pro_s_append_file_of_text(
				path, val, spr,
				boost::addressof(ec), opts);
	}

	// iterator version
	// throw exception_type, compatibility::stl_exception
	template<typename Iter> inline
	static typename boost::enable_if<is_iterator<Iter>, Iter>::type
		append_file_of_text(const path_type& path, Iter val_s, Iter val_e, u32 opts = 0)
	{
		default_text_spr_type spr;
		return this_type::pro_s_append_file_of_text(
				path, val_s, val_e, spr,
				static_cast<error_code_type*>(0), opts);
	}

	template<typename Iter> inline
	static typename boost::enable_if<is_iterator<Iter>, Iter>::type
		append_file_of_text(const path_type& path, Iter val_s, Iter val_e,
							error_code_type& ec, u32 opts = 0)
	{
		default_text_spr_type spr;
		return this_type::pro_s_append_file_of_text(
				path, val_s, val_e, spr,
				boost::addressof(ec), opts);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		append_file_of_text(const path_type& path, Iter val_s, Iter val_e,
							const Spr& spr, u32 opts = 0)
	{
		return this_type::pro_s_append_file_of_text(
				path, val_s, val_e, spr, static_cast<error_code_type*>(0), opts);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		append_file_of_text(const path_type& path, Iter val_s, Iter val_e,
							const Spr& spr, error_code_type& ec, u32 opts = 0)
	{
		return this_type::pro_s_append_file_of_text(
				path, val_s, val_e, spr, boost::addressof(ec), opts);
	}

protected:
	// pro_s_write_file_of_binary
	static file_size_type
		pro_s_read_file_of_binary(ifstream_type& ifs, void* buffer, file_size_type size)
	{
		typedef charset::basic_string<char_type> buf_type;
		typedef typename mplex::basic_t_sizeof_log2<char_type>::type char_sizeof_log2_type;
		BOOST_MPL_ASSERT_NOT((boost::is_same<char_sizeof_log2_type, mplex::basic_t_sizeof_log2_failed>));

		assert(buffer);
		file_size_type readable_size = this_type::pro_s_file_size_of_current_to_end(ifs);
		size = (std::min)(size, readable_size);

		if(!size)
		{
			return 0;
		}

		if(!(buffer && !!ifs && ifs.is_open()))
		{
			return this_type::npos;
		}
		else
		{
			// "size >> char_sizeof_log2_type::value" <=> "size / sizeof(char_type)"
			file_size_type read_size = size >> char_sizeof_log2_type::value;
			ifs.read(reinterpret_cast<char_type*>(buffer), read_size);

			return read_size << char_sizeof_log2_type::value;
		}
	}

	template<typename Container>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_random_access_container<Container>,
				boost::mpl::not_<boost::is_const<Container> >
			>,
			file_size_type
		>::type
		pro_s_read_file_of_binary(ifstream_type& ifs, Container& cont, file_size_type size)
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type cont_val_type;

		if(!size)
		{
			return 0;
		}

		file_size_type readable_size = this_type::pro_s_file_size_of_current_to_end(ifs);
		size = (std::min)(size, readable_size);

		size_type cont_size = (size + sizeof(cont_val_type) - 1) / sizeof(cont_val_type);

		if(!cont_size)
		{
			return 0;
		}
		else
		{
			yggr::resize(cont, cont_size);
			return this_type::pro_s_read_file_of_binary(ifs, &(*boost::begin(cont)), size);
		}
	}

	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_< boost::is_const<Container> >,
				boost::mpl::not_< is_random_access_container<Container> >
			>,
			file_size_type
		>::type
		pro_s_read_file_of_binary(ifstream_type& ifs, Container& cont, file_size_type size)
	{
		typedef Container cont_type;
		typedef typename boost::range_value<cont_type>::type cont_val_type;
		typedef container::vector<cont_val_type> rnd_cont_type;

		rnd_cont_type rnd_cont;
		file_size_type read_size =
			this_type::pro_s_read_file_of_binary(ifs, rnd_cont, size);

		if(read_size)
		{
			yggr::clear(cont);
			std::copy(rnd_cont.begin(), rnd_cont.end(), iterator_ex::inserter(cont, boost::end(cont)));
		}

		return read_size;
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< is_reverse_iterator<Iter> >,
				is_random_access_iterator<Iter>,
				boost::mpl::not_< is_const_iterator<Iter> >
			>,
			file_size_type
		>::type
		pro_s_read_file_of_binary(ifstream_type& ifs, Iter s, Iter e)
	{
		typedef Iter iter_type;
		typedef typename boost::iterator_value<iter_type>::type cont_val_type;

		std::ptrdiff_t cont_size = std::distance(s, e);
		assert(!(cont_size < 0));
		if(cont_size < 0)
		{
			return this_type::npos;
		}

		return cont_size == 0? 0 : this_type::pro_s_read_file_of_binary(ifs, &(*s), cont_size * sizeof(cont_val_type));
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::or_
				<
					is_reverse_iterator<Iter>,
					boost::mpl::and_
					<
						is_iterator<Iter>,
						boost::mpl::not_< is_random_access_iterator<Iter> >
					>
				>,
				boost::mpl::not_< is_const_iterator<Iter> >
			>,
			file_size_type
		>::type
		pro_s_read_file_of_binary(ifstream_type& ifs, Iter s, Iter e)
	{
		typedef Iter iter_type;
		typedef typename boost::iterator_value<iter_type>::type cont_val_type;
		typedef container::vector<cont_val_type> cont_type;

		std::ptrdiff_t cont_size = std::distance(s, e);
		assert(!(cont_size < 0));

		if(cont_size < 0)
		{
			return this_type::npos;
		}

		cont_type tmp_cont(s, e);

		file_size_type ret_size =
			cont_size == 0? 0 : this_type::pro_s_read_file_of_binary(ifs, &tmp_cont[0], cont_size * sizeof(cont_val_type));

		std::copy(tmp_cont.begin(), tmp_cont.end(), s);

		return ret_size;
	}

private:
	static file_size_type
		prv_s_read_file_of_binary_detail(const path_type& path,
											std::ios_base::openmode mode,
											void* buffer,
											file_size_type size)
	{
		assert(buffer);

		ifstream_type ifs;
		ifs.open(YGGR_FILESYSTEM_C_STR(path),
					replace_bit_options<std::ios_base::openmode>(
						mode, std::ios::out, std::ios::in | std::ios::binary));

		if(!ifs)
		{
			if(ifs.is_open())
			{
				ifs.close();
			}
			return this_type::npos;
		}
		else
		{
			file_size_type ret_size = this_type::pro_s_read_file_of_binary(ifs, buffer, size);
			ifs.close();
			return ret_size;
		}
	}

	template<typename Container>
	static typename boost::enable_if<container_ex::is_container<Container>, file_size_type>::type
		prv_s_read_file_of_binary_detail(const path_type& path,
											std::ios_base::openmode mode,
											Container& cont,
											file_size_type size)
	{
		ifstream_type ifs;
		ifs.open(YGGR_FILESYSTEM_C_STR(path),
					replace_bit_options<std::ios_base::openmode>(
						mode, std::ios::out, std::ios::in | std::ios::binary));

		if(!ifs)
		{
			if(ifs.is_open())
			{
				ifs.close();
			}
			return this_type::npos;
		}
		else
		{
			file_size_type ret_size = this_type::pro_s_read_file_of_binary(ifs, cont, size);
			ifs.close();
			return ret_size;
		}
	}

	template<typename Iter>
	static typename boost::enable_if<is_iterator<Iter>, file_size_type>::type
		prv_s_read_file_of_binary_detail(const path_type& path,
											std::ios_base::openmode mode,
											Iter s, Iter e,
											file_size_type size)
	{
		ifstream_type ifs;
		ifs.open(YGGR_FILESYSTEM_C_STR(path),
					replace_bit_options<std::ios_base::openmode>(
						mode, std::ios::out, std::ios::in | std::ios::binary));

		if(!ifs)
		{
			if(ifs.is_open())
			{
				ifs.close();
			}
			return this_type::npos;
		}
		else
		{
			file_size_type ret_size = this_type::pro_s_read_file_of_binary(ifs, s, e);
			ifs.close();
			return ret_size;
		}
	}

protected:
	inline static file_size_type
		pro_s_read_file_of_binary(const path_type& path,
									void* buffer,
									file_size_type size)
	{
		assert(buffer);
		return buffer?
				this_type::prv_s_read_file_of_binary_detail(
					path, std::ios::in | std::ios::binary, buffer, size)
				: this_type::npos;
	}

	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					boost::is_const<Container>
				>
			>,
			file_size_type
		>::type
		pro_s_read_file_of_binary(const path_type& path, Container& cont, file_size_type size)
	{
		return size?
				this_type::prv_s_read_file_of_binary_detail(
						path, std::ios::in | std::ios::binary,
						cont, size) : 0;
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_
				<
					is_const_iterator<Iter>
				>
			>,
			file_size_type
		>::type
		pro_s_read_file_of_binary(const path_type& path, Iter s, Iter e/*, error_code_type* ec*/)
	{
		typedef Iter iter_type;
		typedef typename boost::iterator_value<iter_type>::type cont_val_type;

		std::ptrdiff_t cont_size = std::distance(s, e);
		assert(!(cont_size < 0));
		if(cont_size < 0)
		{
			return this_type::npos;
		}

		return cont_size == 0?
			0 : this_type::prv_s_read_file_of_binary_detail(
					path, std::ios::in | std::ios::binary,
					s, e, cont_size * sizeof(cont_val_type));
	}

public:
	// read_file_of_binary
	// non-throw
	inline static file_size_type
		read_file_of_binary(ifstream_type& ifs, void* buffer, file_size_type size = -1)
	{
		return this_type::pro_s_read_file_of_binary(ifs, buffer, size);
	}

	// container_version
	// non-throw
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					boost::is_const<Container>
				>
			>,
			file_size_type
		>::type
		read_file_of_binary(ifstream_type& ifs, Container& cont, file_size_type size = -1)
	{
		return this_type::pro_s_read_file_of_binary(ifs, cont, size/*, 0*/);
	}

	// iterator version
	// non-throw
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_
				<
					is_const_iterator<Iter>
				>
			>,
			file_size_type
		>::type
		read_file_of_binary(ifstream_type& ifs, Iter s, Iter e)
	{
		return this_type::pro_s_read_file_of_binary(ifs, s, e);
	}

	// non-throw
	inline static file_size_type
		read_file_of_binary(const path_type& path, void* buffer, file_size_type size = -1)
	{
		return this_type::pro_s_read_file_of_binary(path, buffer, size);
	}

	// non-throw
	template<typename Container> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				container_ex::is_container<Container>,
				boost::mpl::not_
				<
					boost::is_const<Container>
				>
			>,
			file_size_type
		>::type
		read_file_of_binary(const path_type& path, Container& cont, file_size_type size = -1)
	{
		return this_type::pro_s_read_file_of_binary(path, cont, size);
	}

	// non-throw
	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_
				<
					is_const_iterator<Iter>
				>
			>,
			file_size_type
		>::type
		read_file_of_binary(const path_type& path, Iter s, Iter e)
	{
		return this_type::pro_s_read_file_of_binary(path, s, e);
	}

private:
	template<typename T>
	static bool prv_s_read_file_of_text_detail(ifstream_type& ifs, T& val,
												charset::basic_string<char_type>& tmp,
												char_type dim)
	{
		typedef std::basic_stringstream<char_type> now_ss_type;
		assert((!!ifs && ifs.is_open()));
		assert(!ifs.eof());

		now_ss_type ss;
		charset::getline(ifs, tmp, dim);
		ss.write(tmp.data(), tmp.size());

		return !!ifs? ((ss >> val), true) : false;

	}

	template<typename T>
	static bool prv_s_read_file_of_text_detail(ifstream_type& ifs, T& val,
													charset::basic_string<char_type>& tmp,
													char_type dim,
													const char_type* str_spr,
													file_size_type spr_size,
													file_size_type spr_cmp_size)
	{
		typedef charset::basic_string<char_type> now_string_type;
		typedef std::basic_stringstream<char_type> now_ss_type;

		typedef typename mplex::basic_t_sizeof_log2<char_type>::type char_sizeof_log2_type;
		BOOST_MPL_ASSERT_NOT((boost::is_same<char_sizeof_log2_type, mplex::basic_t_sizeof_log2_failed>));

		assert((!!ifs && ifs.is_open()));
		assert(!ifs.eof());
		assert(str_spr);
		assert(spr_cmp_size);
		assert(spr_cmp_size < spr_size);
		assert(dim);

		tmp.clear();

		now_ss_type ss;
		file_size_type read_pos;

		bool bloop = true;
		for(;!!ifs && !ifs.eof() && bloop;)
		{
			charset::getline(ifs, tmp, dim);
			ss.write(tmp.data(), tmp.size());
			tmp.clear();

			for(;!!ifs && !ifs.eof();)
			{
				read_pos = ifs.tellg();
				charset::getline(ifs, tmp, str_spr[0]);
				if(ifs.eof() || tmp.size() < spr_size - 1)
				{
					ss.write(tmp.data(), tmp.size());
					break;
				}
				else if(0 == memcmp(&tmp[0], &str_spr[1],
									(spr_size - 1) << char_sizeof_log2_type::value))
				{
					bloop = false;
					ifs.seekg(read_pos + spr_cmp_size, std::ios_base::beg);
					break;
				}
				else
				{
					ss.write(&dim, 1);
					ss.write(tmp.data(), tmp.size());
				}
			}
		}

		return !!ifs? ((ss >> val), true) : false;
	}

protected:
	// pro_s_write_file_of_text

	template<typename T, typename Spr>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<T> >,
				is_fsys_text_separator<Spr>
			>,
			bool
		>::type
		pro_s_read_file_of_text(ifstream_type& ifs, T& val, const Spr& spr)
	{
		typedef charset::basic_string<char_type> now_string_type;
		typedef std::basic_stringstream<char_type> now_ss_type;

		if(!(!!ifs && ifs.is_open() && !ifs.eof()))
		{
			return false;
		}
		else
		{
			now_string_type str_spr;
			to_text_separator_string(str_spr, spr);

			size_type spr_size = 0;
			size_type spr_cmp_size = 0;
			char_type dim = 0;

			if(str_spr.length())
			{
				spr_size = str_spr.size();
				spr_cmp_size = spr_size? spr_size - 1 : spr_size;
				dim = str_spr[0];
			}

			now_string_type tmp;

			return spr_cmp_size?
				this_type::prv_s_read_file_of_text_detail(
					ifs, val, tmp, dim, str_spr.data(), spr_size, spr_cmp_size)
				: this_type::prv_s_read_file_of_text_detail(ifs, val, tmp, dim);
		}
	}

	template<typename Iter, typename Spr>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_const_iterator<Iter> >,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		pro_s_read_file_of_text(ifstream_type& ifs, Iter val_s, Iter val_e, const Spr& spr)
	{
		typedef charset::basic_string<char_type> now_string_type;
		typedef std::basic_stringstream<char_type> now_ss_type;

		if(!(!!ifs && ifs.is_open() && !ifs.eof()))
		{
			return val_s;
		}
		else
		{
			now_string_type str_spr;
			to_text_separator_string(str_spr, spr);

			size_type spr_size = 0;
			size_type spr_cmp_size = 0;
			char_type dim = 0;

			if(str_spr.length())
			{
				spr_size = str_spr.size();
				spr_cmp_size = spr_size? spr_size - 1 : spr_size;
				dim = str_spr[0];
			}

			now_string_type tmp;

			if(spr_cmp_size)
			{
				for(; (val_s != val_e) && (!ifs.eof())
						&& this_type::prv_s_read_file_of_text_detail(
								ifs, *val_s, tmp, dim, str_spr.data(), spr_size, spr_cmp_size);
						++val_s);
			}
			else
			{
				for(; (val_s != val_e) && (!ifs.eof())
						&& this_type::prv_s_read_file_of_text_detail(ifs, *val_s, tmp, dim);
						++val_s);
			}

			return val_s;
		}
	}

private:
	template<typename T, typename Spr>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<T> >,
				is_fsys_text_separator<Spr>
			>,
			bool
		>::type
		prv_s_read_file_of_text_detail(const path_type& path,
										std::ios_base::openmode mode,
										T& val,
										const Spr& spr)
	{
		ifstream_type ifs;
		ifs.open(YGGR_FILESYSTEM_C_STR(path),
					replace_bit_options_if_in<std::ios_base::openmode>(
						mode, std::ios::out | std::ios::binary, std::ios::in));

		if(!ifs)
		{
			if(ifs.is_open())
			{
				ifs.close();
			}
			return false;
		}
		else
		{
			bool bret = this_type::pro_s_read_file_of_text(ifs, val, spr);
			ifs.close();
			return bret;
		}
	}

	template<typename Iter, typename Spr>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_const_iterator<Iter> >,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		prv_s_read_file_of_text_detail(const path_type& path,
										std::ios_base::openmode mode,
										Iter val_s, Iter val_e,
										const Spr& spr)
	{
		ifstream_type ifs;
		ifs.open(YGGR_FILESYSTEM_C_STR(path),
					replace_bit_options_if_in<std::ios_base::openmode>(
						mode, std::ios::out | std::ios::binary, std::ios::in));

		if(!ifs)
		{
			if(ifs.is_open())
			{
				ifs.close();
			}
			return val_s;
		}
		else
		{
			Iter ret = this_type::pro_s_read_file_of_text(ifs, val_s, val_e, spr);
			ifs.close();
			return ret;
		}
	}

protected:
	template<typename T, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<T> >,
				is_fsys_text_separator<Spr>
			>,
			bool
		>::type
		pro_s_read_file_of_text(const path_type& path, T& val, const Spr& spr)
	{
		return this_type::prv_s_read_file_of_text_detail(path, std::ios::out, val, spr);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_const_iterator<Iter> >,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		pro_s_read_file_of_text(const path_type& path,
								Iter val_s, Iter val_e,
								const Spr& spr)
	{
		return this_type::prv_s_read_file_of_text_detail(
				path, std::ios::out, val_s, val_e, spr);
	}


public:

	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!!!	warning:																!!!
	!!!		"write_file_of_text" and "read_file_of_text" is not serialzation!	!!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/

	template<typename T> inline
	static typename boost::enable_if< boost::mpl::not_< boost::is_const<T> >, bool>::type
		read_file_of_text(ifstream_type& ifs, T& val)
	{
		default_text_spr_type spr;
		return this_type::pro_s_read_file_of_text(ifs, val, spr);
	}

	template<typename T, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<T> >,
				is_fsys_text_separator<Spr>
			>,
			bool
		>::type
		read_file_of_text(ifstream_type& ifs, T& val, const Spr& spr)
	{
		return this_type::pro_s_read_file_of_text(ifs, val, spr);
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_
				<
					is_const_iterator<Iter>
				>
			>,
			Iter
		>::type
		read_file_of_text(ifstream_type& ifs, Iter s, Iter e)
	{
		default_text_spr_type spr;
		return this_type::pro_s_read_file_of_text(ifs, s, e, spr);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_const_iterator<Iter> >,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		read_file_of_text(ifstream_type& path, Iter s, Iter e, const Spr& spr)
	{
		return this_type::pro_s_read_file_of_text(path, s, e, spr);
	}

	template<typename T> inline
	static bool read_file_of_text(const path_type& path, T& val)
	{
		default_text_spr_type spr;
		return this_type::pro_s_read_file_of_text(path, val, spr);
	}

	template<typename T, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				boost::mpl::not_< boost::is_const<T> >,
				is_fsys_text_separator<Spr>
			>,
			bool
		>::type
		read_file_of_text(const path_type& path, T& val, const Spr& spr)
	{
		return this_type::pro_s_read_file_of_text(path, val, spr);
	}

	template<typename Iter> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_const_iterator<Iter> >
			>,
			Iter
		>::type
		read_file_of_text(const path_type& path, Iter s, Iter e)
	{
		default_text_spr_type spr;
		return this_type::pro_s_read_file_of_text(path, s, e, spr);
	}

	template<typename Iter, typename Spr> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_iterator<Iter>,
				boost::mpl::not_< is_const_iterator<Iter> >,
				is_fsys_text_separator<Spr>
			>,
			Iter
		>::type
		read_file_of_text(const path_type& path, Iter s, Iter e, const Spr& spr)
	{
		return this_type::pro_s_read_file_of_text(path, s, e, spr);
	}


protected:
	inline static size_type
		pro_s_count_children_file(const path_type& path, error_code_type* ec)
	{
		return this_type::pro_s_count_children(path, ec, file_types_options_type::all_file);
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static size_type count_children_file(const path_type& path)
	{
		return this_type::pro_s_count_children(path, 0, file_types_options_type::all_file);
	}

	// non-throw
	inline static size_type count_children_file(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_count_children(path, boost::addressof(ec), file_types_options_type::all_file);
	}

protected:
	inline static size_type
		pro_s_recursive_count_children_file(const path_type& path, error_code_type* ec)
	{
		return this_type::pro_s_recursive_count_children(path, ec, file_types_options_type::all_file);
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static size_type recursive_count_children_file(const path_type& path)
	{
		return this_type::pro_s_recursive_count_children(path, 0, file_types_options_type::all_file);
	}

	// non-throw
	inline static size_type
		recursive_count_children_file(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_recursive_count_children(
				path, boost::addressof(ec), file_types_options_type::all_file);
	}

protected:
	inline static bool pro_s_opts_chk_copy_file(u32 opts)
	{
		return this_type::pro_s_opts_chk_create_file(opts);
	}

private:
	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		prv_s_copy_file(const path_type& from, const file_status_type& sat_from,
						const path_type& to, const file_status_type& sat_to,
						const Unique_Handler& unique_handler,
						error_code_type* ec, u32 opts)
	{
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;

		assert(!(from.empty() || to.empty()));

		if(!this_type::pro_s_opts_chk_copy_file(opts))
		{
			assert(false);
			return false;
		}

		u32 ft_from = sat_from.type();

		switch(ft_from)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
		case boost::filesystem::directory_file:
		case boost::filesystem::symlink_file:
		case boost::filesystem::type_unknown:
			return false;
		default:
			break;
		}

		copy_option_enum_type cp_opts = copy_option_type::none; // deprecate fail_if_exists
		u32 ft_to = sat_to.type();

		switch(ft_to)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_file_path_and_name(to.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return false;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return false;
				}
			}
#if (BOOST_VERSION < 107400)
			return (native::call_copy_file(from, to, cp_opts, ec), this_type::pro_s_ec_chk(ec));
#else
			return native::call_copy_file(from, to, cp_opts, ec);
#endif // (BOOST_VERSION < 107400)
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_rename_if_exists)))
			{
#if (BOOST_VERSION < 107400)
				return (native::call_copy_file(from, unique_handler(), cp_opts, ec), this_type::pro_s_ec_chk(ec));
#else
				return native::call_copy_file(from, unique_handler(), cp_opts, ec);
#endif //(BOOST_VERSION < 107400)
			}
			else if(this_type::pro_s_symlink_equivalent_no_ec_chk(from, sat_from, to, sat_to, ec))
			{
				return has_bit_options_in<u32>(
						opts, static_cast<u32>(options_type::file_success_if_exists)
								| static_cast<u32>(options_type::file_replace_if_exists_not_dir)
								| static_cast<u32>(options_type::file_replace_if_exists) );
			}
			else
			{
				if(!this_type::pro_s_ec_chk(ec))
				{
					return false;
				}

				if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists_not_dir))
						&& ft_to != boost::filesystem::directory_file)
					|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists)))
				{
					cp_opts = copy_option_type::overwrite_existing; // deprecate overwrite_if_exists
					return (native::call_remove_all(to, ec), this_type::pro_s_ec_chk(ec))
#if (BOOST_VERSION < 107400)
								&& (native::call_copy_file(from, to, cp_opts, ec), this_type::pro_s_ec_chk(ec));
#else
								&& native::call_copy_file(from, to, cp_opts, ec);
#endif //(BOOST_VERSION < 107400)

				}
				else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_success_if_exists)))
				{
					return ft_from == ft_to;
				}
				else
				{
					return false;
				}
			}
		}
	}

protected:

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_copy_file(const path_type& from, const path_type& to,
						const Unique_Handler& unique_handler,
						error_code_type* ec, u32 opts)
	{
		if(from.empty() || to.empty())
		{
			return false;
		}

		file_status_type sat_from = this_type::pro_s_path_symlink_status(from, ec);
		file_status_type sat_to = this_type::pro_s_path_symlink_status(to, ec);

		return this_type::prv_s_copy_file(
				from, sat_from, to, sat_to, unique_handler, ec, opts);
	}

public:
	// copy_file

	// validate options: dir_create_if_not_exists // if the file's parent path not exists create it
	//					file_success_if_exists // if path is exists don't copy, return true
	//					file_replace_if_exists_not_dir // if file is exists and dst is not directory replace it
	//					file_replace_if_exists // if file is exists replace it
	//					file_rename_if_exists // if path is exists rename it

	// throw exception_type, compatibility::stl_exception
	inline static bool copy_file(const path_type& from, const path_type& to, u32 opts = 0)
	{
		typedef typename
			default_handlers_type::rename_handler_type unique_handler_type;

		unique_handler_type unique_handler(to);
		return this_type::pro_s_copy_file(from, to, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool copy_file(const path_type& from, const path_type& to,
									error_code_type& ec, u32 opts = 0)
	{
		typedef typename
			default_handlers_type::rename_handler_type unique_handler_type;

		unique_handler_type unique_handler(to);
		return this_type::pro_s_copy_file(
				from, to, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		copy_file(const path_type& from, const path_type& to,
					const Unique_Handler& unique_handler,
					u32 opts = options_type::file_rename_if_exists)
	{
		return this_type::pro_s_copy_file(from, to, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		copy_file(const path_type& from, const path_type& to,
					const Unique_Handler& unique_handler,
					error_code_type& ec,
					u32 opts = options_type::file_rename_if_exists)
	{
		return this_type::pro_s_copy_file(
				from, to, unique_handler, boost::addressof(ec), opts);
	}

protected:
	inline static bool pro_s_opts_chk_rename_file(u32 opts)
	{
		return this_type::pro_s_opts_chk_create_file(opts);
	}

	template<typename Unique_Handler>
	static bool pro_s_rename_file(const path_type& old_path, const path_type& new_path,
									const Unique_Handler& unique_handler,
									error_code_type* ec, u32 opts)
	{
		if(old_path.empty() || new_path.empty())
		{
			return false;
		}

		if(!this_type::pro_s_opts_chk_rename_file(opts))
		{
			assert(false); // options failed
			return false;
		}

		file_status_type sat_old = this_type::pro_s_path_symlink_status(old_path, ec);
		u32 ft_old = sat_old.type();

		switch(ft_old)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
		case boost::filesystem::directory_file:
		case boost::filesystem::symlink_file:
		case boost::filesystem::type_unknown:
			return false;
		default:
			break;
		}

		if(!(this_type::pro_s_symlink_equivalent(
				this_type::parent_path(old_path),
				this_type::parent_path(new_path), ec)))
		{
			return false;
		}

		file_status_type sat_new = this_type::pro_s_path_symlink_status(new_path, ec);
		u32 ft_new = sat_new.type();

		switch(ft_new)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			return (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_rename_if_exists)))
			{
				return (native::call_rename(old_path, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
			}
			else if(this_type::pro_s_symlink_equivalent_no_ec_chk(old_path, sat_old, new_path, sat_new, ec))
			{
				return has_bit_options_in<u32>(
						opts, static_cast<u32>(options_type::file_success_if_exists)
								| static_cast<u32>(options_type::file_replace_if_exists_not_dir)
								| static_cast<u32>(options_type::file_replace_if_exists) );
			}
			else
			{
				if(!this_type::pro_s_ec_chk(ec))
				{
					return false;
				}

				if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists_not_dir))
						&& ft_new != boost::filesystem::directory_file)
					|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists)))
				{
					return (native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
				}
				else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_success_if_exists)))
				{
					return ft_new == ft_old;
				}
				else
				{
					return false;
				}
			}
		}
	}

public:
	// rename_file

	// validate options: file_success_if_exists // if path is exists don't rename, return true
	//					file_replace_if_exists_not_dir // if file is exists and dst is not directory replace it
	//					file_replace_if_exists // if file is exists replace it
	//					file_rename_if_exists // if path is exists rename it

	// throw exception_type, compatibility::stl_exception
	inline static bool rename_file(const path_type& old_path, const path_type& new_path, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_rename_file(old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool rename_file(const path_type& old_path, const path_type& new_path,
									error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_rename_file(
				old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			is_callable<Unique_Handler>,
			bool
		>::type
		rename_file(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						u32 opts = static_cast<u32>(options_type::file_rename_if_exists))
	{
		return this_type::pro_s_rename_file(
				old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			is_callable<Unique_Handler>,
			bool
		>::type
		rename_file(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						error_code_type& ec,
						u32 opts = static_cast<u32>(options_type::file_rename_if_exists))
	{
		return this_type::pro_s_rename_file(
				old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

protected:

	inline static bool pro_s_opts_chk_move_file(u32 opts)
	{
		return this_type::pro_s_opts_chk_create_file(opts);
	}

private:
	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		prv_s_move_file(const path_type& old_path, const file_status_type& sat_old,
						const path_type& new_path, const file_status_type& sat_new,
						const Unique_Handler& unique_handler,
						error_code_type* ec, u32 opts)
	{
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;

		assert(!(old_path.empty() || new_path.empty()));

		if(!pro_s_opts_chk_move_file(opts))
		{
			assert(false);
			return false;
		}

		u32 ft_old = sat_old.type();

		switch(ft_old)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
		case boost::filesystem::directory_file:
		case boost::filesystem::symlink_file:
		case boost::filesystem::type_unknown:
			return false;
		default:
			break;
		}

		u32 ft_new = sat_new.type();

		switch(ft_new)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_file_path_and_name(new_path.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return false;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return false;
				}
			}

#if defined(YGGR_AT_WINDOWS)
			return (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
#else
            //  {
            //     error_code_type ec_tmp;
            //     return 
            //         this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(new_path), ec)?
            //             this_type::pro_s_ec_chk(ec) && (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
            //           :   this_type::pro_s_ec_chk(ec) 
            //                 && native::call_copy_file(old_path, new_path, ec) && this_type::pro_s_ec_chk(ec)
            //                 && ((native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec))?
            //                     true : (native::call_remove(new_path, boost::addressof(ec_tmp)), false));
            //  }
			// !!! Why not roll back new_path, because it's safer to do so !!!
			return 
				this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(new_path), ec)?
					this_type::pro_s_ec_chk(ec) && (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
					:   this_type::pro_s_ec_chk(ec) 
						&& native::call_copy_file(old_path, new_path, ec) && this_type::pro_s_ec_chk(ec)
						&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec);
#endif // YGGR_AT_WINDOWS
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_rename_if_exists)))
			{
#if defined(YGGR_AT_WINDOWS)
				return (native::call_rename(old_path, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
#else
				path_type unique_new_path = unique_handler();
				return 
					this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(unique_new_path), ec)?
						this_type::pro_s_ec_chk(ec)
						&& (native::call_rename(old_path, unique_new_path, ec), this_type::pro_s_ec_chk(ec))
						:   this_type::pro_s_ec_chk(ec) 
							&& native::call_copy_file(old_path, unique_new_path, ec) && this_type::pro_s_ec_chk(ec)
							&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec);
#endif // YGGR_AT_WINDOWS
			}
			else if(this_type::pro_s_symlink_equivalent_no_ec_chk(old_path, sat_old, new_path, sat_new, ec))
			{
				return has_bit_options_in<u32>(
						opts, static_cast<u32>(options_type::file_success_if_exists)
								| static_cast<u32>(options_type::file_replace_if_exists_not_dir)
								| static_cast<u32>(options_type::file_replace_if_exists) );
			}
			else
			{
				if(!this_type::pro_s_ec_chk(ec))
				{
					return false;
				}

				if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists_not_dir))
						&& ft_new != boost::filesystem::directory_file)
					|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists)))
				{
#if defined(YGGR_AT_WINDOWS)
					return (native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
#else
					return 
						(native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
						&& (this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(new_path), ec)?
								this_type::pro_s_ec_chk(ec)
								&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
								:   this_type::pro_s_ec_chk(ec) 
									&& native::call_copy_file(old_path, new_path, ec) && this_type::pro_s_ec_chk(ec)
									&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec));
#endif // YGGR_AT_WINDOWS
				}
				else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_success_if_exists)))
				{
					return ft_new == ft_old;
				}
				else
				{
					return false;
				}
			}
		}
	}

protected:
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_move_file(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						error_code_type* ec, u32 opts)
	{
		if(old_path.empty() || new_path.empty())
		{
			return false;
		}

		file_status_type sat_old = this_type::pro_s_path_symlink_status(old_path, ec);
		file_status_type sat_new = this_type::pro_s_path_symlink_status(new_path, ec);

		return this_type::prv_s_move_file(
				old_path, sat_old, new_path, sat_new, unique_handler, ec, opts);
	}


public:
	// move_file
	// validate options: dir_create_if_not_exists // if the file's parent path not exists create it
	//					file_success_if_exists // if path is exists don't move, return true
	//					file_replace_if_exists_not_dir // if file is exists and dst is not directory replace it
	//					file_replace_if_exists // if file is exists replace it
	//					file_rename_if_exists // if path is exists rename it

	// throw exception_type, compatibility::stl_exception
	inline static bool move_file(const path_type& old_path, const path_type& new_path, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_move_file(old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool move_file(const path_type& old_path, const path_type& new_path,
									error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_move_file(
				old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		move_file(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						u32 opts = static_cast<u32>(options_type::file_rename_if_exists))
	{
		return this_type::pro_s_move_file(
				old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		move_file(const path_type& old_path, const path_type& new_path,
					const Unique_Handler& unique_handler,
					error_code_type& ec,
					u32 opts = static_cast<u32>(options_type::file_rename_if_exists))
	{
		return this_type::pro_s_move_file(
				old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

protected:
	inline static bool pro_s_remove_file(const path_type& path, error_code_type* ec, u32 opts)
	{
		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);

		switch(sat_path.type())
		{
		case boost::filesystem::status_error:
		case boost::filesystem::directory_file:
		case boost::filesystem::symlink_file:
			return false;
		case boost::filesystem::file_not_found:
			return this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::remove_success_if_not_exists));
		default:
			return native::call_remove(path, ec) && this_type::pro_s_ec_chk(ec);
		}
	}

public:
	// remove_file
	// validate options: remove_success_if_not_exists // if file is not exists return true;

	// throw exception_type, compatibility::stl_exception
	inline static bool remove_file(const path_type& path, u32 opts = 0)
	{
		return this_type::pro_s_remove_file(path, 0, opts);
	}

	// non-throw
	inline static bool remove_file(const path_type& path, error_code_type& ec, u32 opts = 0)
	{
		return this_type::pro_s_remove_file(path, boost::addressof(ec), opts);
	}

protected:
	inline static bool pro_s_resize_file(const path_type& path, size_type new_size, error_code_type* ec)
	{
		return (native::call_resize_file(path, new_size, ec), this_type::pro_s_ec_chk(ec));
	}

public:

	// throw exception_type, compatibility::stl_exception
	inline static bool resize_file(const path_type& path, size_type new_size)
	{
		return this_type::pro_s_resize_file(path, new_size, 0);
	}

	// non-throw
	inline static bool resize_file(const path_type& path, size_type new_size, error_code_type& ec)
	{
		return this_type::pro_s_resize_file(path, new_size, boost::addressof(ec));
	}

	//---------------------symlink-----------------------------

public:
	// is_symlink
	inline static bool is_symlink(const file_status_type& s)
	{
		return native::is_symlink(s);
	}

	// throw exception_type, compatibility::stl_exception
	inline static bool is_symlink(const path_type& path)
	{
		return native::is_symlink(path);
	}

	// non-throw
	inline static bool is_symlink(const path_type& path, error_code_type& ec)
	{
		return native::is_symlink(path, ec);
	}

protected:
	inline static size_type pro_s_count_children_symlink(const path_type& path, error_code_type* ec)
	{
		return this_type::pro_s_count_children(
				path, ec, file_types_options_type::general_link);
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static size_type count_children_symlink(const path_type& path)
	{
		return this_type::pro_s_count_children(
				path, 0, file_types_options_type::general_link);
	}

	// non-throw
	inline static size_type count_children_symlink(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_count_children(
				path, boost::addressof(ec), file_types_options_type::general_link);
	}

protected:
	inline static size_type
		pro_s_recursive_count_children_symlink(const path_type& path, error_code_type* ec)
	{
		return this_type::pro_s_recursive_count_children(
				path, ec, file_types_options_type::general_link);
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static size_type recursive_count_children_symlink(const path_type& path)
	{
		return this_type::pro_s_recursive_count_children(
				path, 0, file_types_options_type::general_link);
	}

	// non-throw
	inline static size_type
		recursive_count_children_symlink(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_recursive_count_children(
				path, boost::addressof(ec), file_types_options_type::general_link);
	}

protected:
	inline static bool pro_s_opts_chk_create_symlink(u32 opts)
	{
		switch(check_bit_options(
				opts,
				static_cast<u32>(options_type::symlink_success_if_exists)
				| static_cast<u32>(options_type::symlink_replace_if_exists_not_dir)
				| static_cast<u32>(options_type::symlink_replace_if_exists)
				| static_cast<u32>(options_type::symlink_rename_if_exists)) )
		{
		case 0:
		case static_cast<u32>(options_type::symlink_success_if_exists):
		case static_cast<u32>(options_type::symlink_replace_if_exists_not_dir):
		case static_cast<u32>(options_type::symlink_replace_if_exists):
		case static_cast<u32>(options_type::symlink_rename_if_exists):
			return true;
		default:
			return false;
		}
	}

	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_create_symlink(const path_type& to, const path_type& new_symlink,
								const Unique_Handler& unique_handler,
								error_code_type* ec, u32 opts)
	{
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;
		typedef std::pair<bool, bool> chk_op_ret_type;

		if(to.empty() || new_symlink.empty())
		{
			return false;
		}

		if(!this_type::pro_s_opts_chk_create_symlink(opts))
		{
			assert(false); // options error
			return false;
		}

		file_status_type sat_new = this_type::pro_s_path_symlink_status(new_symlink, ec);
		u32 ft_new = sat_new.type();

		switch(ft_new)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(new_symlink.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return false;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return false;
				}
			}
			return (native::call_create_symlink(to, new_symlink, ec), this_type::pro_s_ec_chk(ec));
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_rename_if_exists)))
			{
				return (native::call_create_symlink(to, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
			}
			if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists_not_dir))
					&& ft_new != boost::filesystem::directory_file)
				|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists)))
			{
				if(this_type::pro_s_symlink_equivalent_no_ec_chk(to, new_symlink, ec)
					|| !this_type::pro_s_ec_chk(ec) )
				{
					return false;
				}

				return (native::call_remove_all(new_symlink, ec), this_type::pro_s_ec_chk(ec))
						&& (native::call_create_symlink(to, new_symlink, ec), this_type::pro_s_ec_chk(ec));
			}
			else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_success_if_exists)))
			{
				return ft_new == boost::filesystem::symlink_file;
			}
			else
			{
				return false;
			}
		}
	}

public:
	// create_symlink
	// validate options:
	//		dir_create_if_not_exists, // if the symlink's parent path not exists create it
	//		symlink_success_if_exists // if the symlink exists, return the true, else create the symlink
	//		|| symlink_replace_if_exists_not_dir // if the symlink exists, create a new file with a unique rule file name
	//		|| symlink_replace_if_exists // if the symlink exists, create a new file with a unique rule file name
	//		|| symlink_rename_if_exists  // if the symlink is exists, create a new file with a unique rule file name

	// throw exception_type, compatibility::stl_exception
	inline static bool create_symlink(const path_type& to, const path_type& new_symlink, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_symlink);
		return this_type::pro_s_create_symlink(to, new_symlink, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool create_symlink(const path_type& to, const path_type& new_symlink,
										error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_symlink);
		return this_type::pro_s_create_symlink(to, new_symlink, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		create_symlink(const path_type& to, const path_type& new_symlink,
						const Unique_Handler& unique_hanlder,
						u32 opts = static_cast<u32>(options_type::symlink_rename_if_exists))
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_symlink);
		return this_type::pro_s_create_symlink(to, new_symlink, unique_handler, 0, opts);
	}

	// non-thow
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		create_symlink(const path_type& to, const path_type& new_symlink,
						const Unique_Handler& unique_hanlder,
						error_code_type& ec,
						u32 opts = static_cast<u32>(options_type::symlink_rename_if_exists))
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_symlink);
		return this_type::pro_s_create_symlink(to, new_symlink, unique_handler, boost::addressof(ec), opts);
	}

protected:
	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_create_directory_symlink(const path_type& to, const path_type& new_symlink,
										const Unique_Handler& unique_handler,
										error_code_type* ec, u32 opts)
	{
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;
		typedef std::pair<bool, bool> chk_op_ret_type;

		if(to.empty() || new_symlink.empty())
		{
			return false;
		}

		if(!this_type::pro_s_opts_chk_create_symlink(opts))
		{
			assert(false); // options error
			return false;
		}

		file_status_type sat_new = this_type::pro_s_path_symlink_status(new_symlink, ec);
		u32 ft_new = sat_new.type();

		switch(ft_new)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(new_symlink.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return false;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return false;
				}
			}
			return (native::call_create_directory_symlink(to, new_symlink, ec), this_type::pro_s_ec_chk(ec));
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_rename_if_exists)))
			{
				return (native::call_create_directory_symlink(to, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
			}
			if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists_not_dir))
					&& ft_new != boost::filesystem::directory_file)
				|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists)))
			{
				if(this_type::pro_s_symlink_equivalent_no_ec_chk(to, new_symlink, ec)
					|| !this_type::pro_s_ec_chk(ec) )
				{
					return false;
				}

				return (native::call_remove_all(new_symlink, ec), this_type::pro_s_ec_chk(ec))
						&& (native::call_create_directory_symlink(to, new_symlink, ec), this_type::pro_s_ec_chk(ec));
			}
			else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_success_if_exists)))
			{
				return ft_new == boost::filesystem::symlink_file;
			}
			else
			{
				return false;
			}
		}
	}

public:
	// create_directory_symlink
	// validate options:
	//		dir_create_if_not_exists, // if the symlink's parent path not exists create it
	//		symlink_success_if_exists // if the symlink exists, return the true, else create the symlink
	//		|| symlink_replace_if_exists_not_dir // if the symlink exists, create a new file with a unique rule file name
	//		|| symlink_replace_if_exists // if the symlink exists, create a new file with a unique rule file name
	//		|| symlink_rename_if_exists  // if the symlink is exists, create a new file with a unique rule file name

	// throw exception_type, compatibility::stl_exception
	inline static bool create_directory_symlink(const path_type& to, const path_type& new_symlink, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_symlink);
		return this_type::pro_s_create_directory_symlink(to, new_symlink, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool create_directory_symlink(const path_type& to, const path_type& new_symlink,
										error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_symlink);
		return this_type::pro_s_create_directory_symlink(to, new_symlink, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		create_directory_symlink(const path_type& to, const path_type& new_symlink,
									const Unique_Handler& unique_hanlder,
									u32 opts = static_cast<u32>(options_type::symlink_rename_if_exists))
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_symlink);
		return this_type::pro_s_create_directory_symlink(to, new_symlink, unique_handler, 0, opts);
	}

	// non-thow
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		create_directory_symlink(const path_type& to, const path_type& new_symlink,
									const Unique_Handler& unique_hanlder,
									error_code_type& ec,
									u32 opts = static_cast<u32>(options_type::symlink_rename_if_exists))
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_symlink);
		return this_type::pro_s_create_directory_symlink(to, new_symlink, unique_handler, boost::addressof(ec), opts);
	}

protected:
	inline static bool pro_s_opts_chk_copy_symlink(u32 opts)
	{
		return this_type::pro_s_opts_chk_create_symlink(opts);
	}

private:
	template<typename Unique_Handler>
	static bool prv_s_copy_symlink(const path_type& from, const file_status_type& sat_from,
									const path_type& to, const file_status_type& sat_to,
									const Unique_Handler& unique_handler,
									error_code_type* ec, u32 opts)
	{
		typedef std::pair<bool, bool> chk_ret_type;

		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;

		assert(!(from.empty() || to.empty()));

		if(!this_type::pro_s_opts_chk_copy_symlink(opts))
		{
			assert(false); // options failed
			return false;
		}

		u32 ft_from = sat_from.type();

		switch(ft_from)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
		case boost::filesystem::directory_file:
			return false;
		case boost::filesystem::symlink_file:
			break;
		default:
			return false;
		}

		u32 ft_to = sat_to.type();

		switch(ft_to)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_file_path_and_name(to.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return false;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return false;
				}
			}
			return (native::call_copy_symlink(from, to, ec), this_type::pro_s_ec_chk(ec));
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_rename_if_exists)))
			{
				return (native::call_copy_symlink(from, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
			}
			else if(this_type::pro_s_symlink_equivalent_no_ec_chk(from, sat_from, to, sat_to, ec))
			{
				return has_bit_options_in<u32>(
						opts, static_cast<u32>(options_type::symlink_success_if_exists)
								| static_cast<u32>(options_type::symlink_replace_if_exists_not_dir)
								| static_cast<u32>(options_type::symlink_replace_if_exists) );
			}
			else
			{
				if(!this_type::pro_s_ec_chk(ec))
				{
					return false;
				}

				if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists_not_dir))
						&& ft_to != boost::filesystem::directory_file)
					|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists)))
				{
					return (native::call_remove_all(to, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_copy_symlink(from, to, ec), this_type::pro_s_ec_chk(ec));
				}
				else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_success_if_exists)))
				{
					return ft_to == boost::filesystem::symlink_file;
				}
				else
				{
					return false;
				}
			}
		}
	}

protected:
	template<typename Unique_Handler> inline
	static bool pro_s_copy_symlink(const path_type& from, const path_type& to,
									const Unique_Handler& unique_handler,
									error_code_type* ec, u32 opts)
	{
		if(from.empty() || to.empty())
		{
			return false;
		}

		file_status_type sat_from = this_type::pro_s_path_symlink_status(from, ec);
		file_status_type sat_to = this_type::pro_s_path_symlink_status(to, ec);

		return this_type::prv_s_copy_symlink(
				from, sat_from, to, sat_to, unique_handler, ec, opts);
	}


public:
	// copy_symlink
	// validate options: dir_create_if_not_exists // if the symlink's parent path not exists create it
	//					file_success_if_exists // if path is exists don't copy, return true
	//					file_replace_if_exists_not_dir // if symlink is exists and dst is not directory replace it
	//					file_replace_if_exists // if symlink is exists replace it
	//					file_rename_if_exists // if path is exists rename it

	// throw exception_type, compatibility::stl_exception
	inline static bool copy_symlink(const path_type& from, const path_type& to, u32 opts = 0)
	{
		typedef typename
			default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(to);
		return this_type::pro_s_copy_symlink(from, to, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool copy_symlink(const path_type& from, const path_type& to,
										error_code_type& ec, u32 opts = 0)
	{
		typedef typename
			default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(to);
		return this_type::pro_s_copy_symlink(from, to, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		copy_symlink(const path_type& from, const path_type& to,
						const Unique_Handler& unique_handler,
						u32 opts = options_type::symlink_rename_if_exists)
	{
		return this_type::pro_s_copy_symlink(from, to, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		copy_symlink(const path_type& from, const path_type& to,
						const Unique_Handler& unique_handler,
						error_code_type& ec,
						u32 opts = options_type::symlink_rename_if_exists)
	{
		return this_type::pro_s_copy_symlink(from, to, unique_handler, boost::addressof(ec), opts);
	}


protected:
	inline static bool pro_s_opts_chk_rename_symlink(u32 opts)
	{
		return this_type::pro_s_opts_chk_create_symlink(opts);
	}

	template<typename Unique_Handler>
	static bool pro_s_rename_symlink(const path_type& old_path, const path_type& new_path,
										const Unique_Handler& unique_handler,
										error_code_type* ec, u32 opts)
	{
		if(old_path.empty() || new_path.empty())
		{
			return false;
		}

		if(!this_type::pro_s_opts_chk_rename_symlink(opts))
		{
			assert(false); // options failed
			return false;
		}

		file_status_type sat_old = this_type::pro_s_path_symlink_status(old_path, ec);
		u32 ft_old = sat_old.type();

		switch(ft_old)
		{
		case boost::filesystem::symlink_file:
			break;
		default:
			return false;
		}

		if(!(this_type::pro_s_symlink_equivalent(
				this_type::parent_path(old_path),
				this_type::parent_path(new_path), ec)))
		{
			return false;
		}


		file_status_type sat_new = this_type::pro_s_path_symlink_status(new_path, ec);
		u32 ft_new = sat_new.type();

		switch(ft_new)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			return (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_rename_if_exists)))
			{
				return (native::call_rename(old_path, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
			}
			else if(this_type::pro_s_symlink_equivalent_no_ec_chk(old_path, sat_old, new_path, sat_new, ec))
			{
				return has_bit_options_in<u32>(
						opts, static_cast<u32>(options_type::symlink_success_if_exists)
								| static_cast<u32>(options_type::symlink_replace_if_exists_not_dir)
								| static_cast<u32>(options_type::symlink_replace_if_exists) );
			}
			else
			{
				if(!this_type::pro_s_ec_chk(ec))
				{
					return false;
				}

				if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists_not_dir))
							&& ft_new != boost::filesystem::directory_file)
						|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists)))
				{
					return (native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
				}
				else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_success_if_exists)))
				{
					return ft_new == boost::filesystem::symlink_file;
				}
				else
				{
					return false;
				}
			}
		}
	}

public:
	// rename_symlink
	// validate options: symlink_rename_if_exists

	// throw exception_type, compatibility::stl_exception
	inline static bool rename_symlink(const path_type& old_path, const path_type& new_path, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_rename_symlink(old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool rename_symlink(const path_type& old_path, const path_type& new_path,
										error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_rename_symlink(
				old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			is_callable<Unique_Handler>,
			bool
		>::type
		rename_symlink(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						u32 opts = static_cast<u32>(options_type::symlink_rename_if_exists))
	{
		return this_type::pro_s_rename_symlink(
				old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			is_callable<Unique_Handler>,
			bool
		>::type
		rename_symlink(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						error_code_type& ec,
						u32 opts = static_cast<u32>(options_type::symlink_rename_if_exists))
	{
		return this_type::pro_s_rename_symlink(
				old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

protected:

	inline static bool pro_s_opts_chk_move_symlink(u32 opts)
	{
		return this_type::pro_s_opts_chk_create_symlink(opts);
	}

private:
	template<typename Unique_Handler>
	static bool prv_s_move_symlink(const path_type& old_path, const file_status_type& sat_old,
									const path_type& new_path, const file_status_type& sat_new,
									const Unique_Handler& unique_handler,
									error_code_type* ec, u32 opts)
	{
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;

		assert(!(old_path.empty() || new_path.empty()));

		if(!pro_s_opts_chk_move_symlink(opts))
		{
			assert(false);
			return false;
		}

		u32 ft_old = sat_old.type();

		switch(ft_old)
		{
		case boost::filesystem::symlink_file:
			break;
		default:
			return false;
		}

		u32 ft_new = sat_new.type();

		switch(ft_new)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_file_path_and_name(new_path.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return false;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return false;
				}
			}
#if defined(YGGR_AT_WINDOWS)
			return (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
#else
            //  {
            //     error_code_type ec_tmp;
            //     return 
            //         this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(new_path), ec)?
            //             this_type::pro_s_ec_chk(ec) && (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
            //           :   this_type::pro_s_ec_chk(ec) 
            //                 && (native::call_copy_symlink(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
            //                 && ((native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec))?
            //                     true : (native::call_remove(new_path, boost::addressof(ec_tmp)), false));
            //  }
			// !!! Why not roll back new_path, because it's safer to do so !!!
			return 
				this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(new_path), ec)?
					this_type::pro_s_ec_chk(ec) && (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
					:   this_type::pro_s_ec_chk(ec) 
						&& (native::call_copy_symlink(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
						&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec);
#endif // YGGR_AT_WINDOWS
		default:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_rename_if_exists)))
			{
#if defined(YGGR_AT_WINDOWS)
				return (native::call_rename(old_path, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
#else
				path_type unique_new_path = unique_handler();
				return 
					this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(unique_new_path), ec)?
						this_type::pro_s_ec_chk(ec)
						&& (native::call_rename(old_path, unique_new_path, ec), this_type::pro_s_ec_chk(ec))
						:   this_type::pro_s_ec_chk(ec) 
							&& (native::call_copy_symlink(old_path, unique_new_path, ec), this_type::pro_s_ec_chk(ec))
							&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec);
#endif // YGGR_AT_WINDOWS
			}
			else if(this_type::pro_s_symlink_equivalent_no_ec_chk(old_path, sat_old, new_path, sat_new, ec))
			{
				return has_bit_options_in<u32>(
						opts, static_cast<u32>(options_type::symlink_success_if_exists)
								| static_cast<u32>(options_type::symlink_replace_if_exists_not_dir)
								| static_cast<u32>(options_type::symlink_replace_if_exists) );
			}

			if(!this_type::pro_s_ec_chk(ec))
			{
				return false;
			}

			if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists_not_dir))
						&& ft_new != boost::filesystem::directory_file)
					|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists)))
			{
#if defined(YGGR_AT_WINDOWS)
				return (native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
						&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec));
#else
				return 
					(native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
					&& (this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(new_path), ec)?
							this_type::pro_s_ec_chk(ec) && (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
							:   this_type::pro_s_ec_chk(ec) 
								&& (native::call_copy_symlink(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
								&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec));

#endif // YGGR_AT_WINDOWS
			}
			else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_success_if_exists)))
			{
				return ft_new == boost::filesystem::symlink_file;
			}
			else
			{
				return false;
			}
		}
	}

protected:

	template<typename Unique_Handler> inline
	static bool pro_s_move_symlink(const path_type& old_path, const path_type& new_path,
									const Unique_Handler& unique_handler,
									error_code_type* ec, u32 opts)
	{
		if(old_path.empty() || new_path.empty())
		{
			return false;
		}

		file_status_type sat_old = this_type::pro_s_path_symlink_status(old_path, ec);
		file_status_type sat_new = this_type::pro_s_path_symlink_status(new_path, ec);

		return this_type::prv_s_move_symlink(
				old_path, sat_old, new_path, sat_new, unique_handler, ec, opts);
	}

public:
	// move_symlink
	// validate options: symlink_rename_if_exists

	// throw exception_type, compatibility::stl_exception
	inline static bool move_symlink(const path_type& old_path, const path_type& new_path, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_move_symlink(old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool move_symlink(const path_type& old_path, const path_type& new_path,
									error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_move_symlink(
				old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			is_callable<Unique_Handler>,
			bool
		>::type
		move_symlink(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						u32 opts = static_cast<u32>(options_type::symlink_rename_if_exists))
	{
		return this_type::pro_s_move_symlink(old_path, new_path, unique_handler, 0, opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			is_callable<Unique_Handler>,
			bool
		>::type
		move_symlink(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						error_code_type& ec,
						u32 opts = static_cast<u32>(options_type::symlink_rename_if_exists))
	{
		return this_type::pro_s_move_symlink(
				old_path, new_path, unique_handler, boost::addressof(ec), opts);
	}

protected:
	inline static bool pro_s_remove_symlink(const path_type& path, error_code_type* ec, u32 opts)
	{
		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);

		switch(sat_path.type())
		{
		case boost::filesystem::file_not_found:
			return this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::remove_success_if_not_exists));
		case boost::filesystem::symlink_file:
			return native::call_remove(path, ec) && this_type::pro_s_ec_chk(ec);
		default:
			return false;
		}
	}

public:
	// remove_symlink
	// validate options: remove_success_if_not_exists // if symlink is not exists return true;

	// throw exception_type, compatibility::stl_exception
	inline static bool remove_symlink(const path_type& path, u32 opts = 0)
	{
		return this_type::pro_s_remove_symlink(path, 0, opts);
	}

	// non-throw
	inline static bool remove_symlink(const path_type& path, error_code_type& ec, u32 opts = 0)
	{
		return this_type::pro_s_remove_symlink(path, boost::addressof(ec), opts);
	}

public:
	// read_symlink
	// throw exception_type, compatibility::stl_exception
	inline static path_type read_symlink(const path_type& path)
	{
		return native::read_symlink(path);
	}

	// non-throw
	inline static path_type read_symlink(const path_type& path, error_code_type& ec)
	{
		return native::read_symlink(path, ec);
	}

protected:
	static file_status_type
		pro_s_recursive_read_symlink(path_type& dst_path, const path_type& src_path, error_code_type* ec)
	{
		dst_path.clear();

		if(src_path.empty())
		{
			return file_status_type();
		}

		file_status_type sat_src = this_type::pro_s_path_symlink_status(src_path, ec);
		u32 ft_src = sat_src.type();

		switch(ft_src)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return sat_src;
		case boost::filesystem::symlink_file:
			break;
		default:
			return file_status_type();
		}

		path_type tmp_path_now = src_path;
		path_type tmp_path_next;
		//path_type tmp_path = src_path;

		for(;;)
		{
			try
			{
				if(!(tmp_path_next = native::call_read_symlink(tmp_path_now, ec), this_type::pro_s_ec_chk(ec)))
				{
					return file_status_type();
				}
			}
			catch(const exception_type&)
			{
				return file_status_type();
			}

			if(tmp_path_next.is_relative())
			{
				tmp_path_next = tmp_path_now.parent_path() / tmp_path_next;
			}

			try
			{
				sat_src = this_type::pro_s_path_symlink_status(tmp_path_next, ec);
				if(!this_type::pro_s_ec_chk(ec))
				{
					sat_src = file_status_type();
				}
			}
			catch(const exception_type&)
			{
				sat_src = file_status_type();
			}
			
			ft_src = sat_src.type();

			switch(ft_src)
			{
			case boost::filesystem::status_error:
				return sat_src;
			case boost::filesystem::symlink_file:
				tmp_path_now.swap(tmp_path_next);
				break;
			default:
				dst_path.swap(tmp_path_next);
				return sat_src;
			}
		}
	}

	inline static std::pair<path_type, file_status_type>
		pro_s_recursive_read_symlink(const path_type& src_path, error_code_type* ec)
	{
		typedef std::pair<path_type, file_status_type> ret_type;

		ret_type ret;
		ret.second = this_type::pro_s_recursive_read_symlink(ret.first, src_path, ec);
		return ret;
	}


public:
	// recursive_read_symlink

	// throw exception_type, compatibility::stl_exception
	inline static file_status_type
		recursive_read_symlink(path_type& dst_path, const path_type& src_path)
	{
		return this_type::pro_s_recursive_read_symlink(dst_path, src_path, 0);
	}

	// non-throw
	inline static file_status_type
		recursive_read_symlink(path_type& dst_path, const path_type& src_path, error_code_type& ec)
	{
		return this_type::pro_s_recursive_read_symlink(dst_path, src_path, boost::addressof(ec));
	}

	// throw exception_type, compatibility::stl_exception
	inline static std::pair<path_type, file_status_type>
		recursive_read_symlink(const path_type& path)
	{
		return this_type::pro_s_recursive_read_symlink(path, 0);
	}

	// non-throw
	inline static std::pair<path_type, file_status_type>
		recursive_read_symlink(const path_type& path, error_code_type& ec)
	{
		return this_type::pro_s_recursive_read_symlink(path, boost::addressof(ec));
	}

public:
	// hard_link_count
	// warning: "hard_link_count" not_support symlink,
	// if you want to count symlink, please using "symlink_hard_link_count"

	// throw exception_type, compatibility::stl_exception
	inline static size_type hard_link_count(const path_type& path)
	{
		return native::hard_link_count(path);
	}

	// non-throw
	inline static size_type hard_link_count(const path_type& path, error_code_type& ec)
	{
		return native::hard_link_count(path, ec);
	}

public:
	// symlink_hard_link_count
	// warning: same to "hard_link_count" and support symlink

	// throw exception_type, compatibility::stl_exception
	inline static size_type symlink_hard_link_count(const path_type& path)
	{
		return native::symlink_hard_link_count(path);
	}

	// non-throw
	inline static size_type symlink_hard_link_count(const path_type& path, error_code_type& ec)
	{
		return native::symlink_hard_link_count(path, ec);
	}

protected:
	inline static bool pro_s_opts_chk_create_hard_link(u32 opts)
	{
		u32 chk_opt = 0;

		chk_opt =
			check_bit_options<u32>(
				opts,
				(static_cast<u32>(options_type::hard_link_success_if_exists)
					| static_cast<u32>(options_type::hard_link_success_if_equivalent)
					| static_cast<u32>(options_type::hard_link_replace_if_exists_not_dir)
					| static_cast<u32>(options_type::hard_link_replace_if_exists)
					| static_cast<u32>(options_type::hard_link_rename_if_exists) ));


		switch(chk_opt)
		{
		case 0:
		case options_type::hard_link_success_if_exists:
		case options_type::hard_link_success_if_equivalent:
		case options_type::hard_link_replace_if_exists_not_dir:
		case options_type::hard_link_replace_if_exists:
		case options_type::hard_link_rename_if_exists:
			break;
		default:
			return false;
		}

		return true;
	}

	template<typename Unique_Handler>
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_create_hard_link(const path_type& to, const path_type& new_hlink,
								const Unique_Handler& unique_handler,
								error_code_type* ec, u32 opts)
	{
		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;
		typedef std::pair<bool, bool> chk_op_ret_type;

		if(to.empty() || new_hlink.empty())
		{
			return false;
		}

		if(!this_type::pro_s_opts_chk_create_hard_link(opts))
		{
			assert(false); // options error
			return false;
		}

		file_status_type sat_to = this_type::pro_s_path_symlink_status(to, ec);
		u32 ft_to = sat_to.type();

		switch(ft_to)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
#if !defined(YGGR_FSYS_DIRECTORY_HARD_LINK_SUPPORT)
		case boost::filesystem::directory_file:
#endif // YGGR_FSYS_DIRECTORY_HARD_LINK_SUPPORT
			return false;
		default:
			break;
		}

		file_status_type sat_new_hlink = this_type::pro_s_path_symlink_status(new_hlink, ec);
		u32 ft_new_hlink = sat_new_hlink.type();

		switch(ft_new_hlink)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
			{
				chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(new_hlink.native());
				if(!yggr::get<2>(chk_name_ret))
				{
					return false;
				}
				const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
				empty_unique_handler_type empty_unique_handler;
				if(!this_type::pro_s_create_directories(
						this_type::make_path_object(iter_pair.first, iter_pair.second),
						empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
						static_cast<u32>(options_type::dir_success_if_exists)))
				{
					return false;
				}
			}
			return (native::call_create_hard_link(to, new_hlink, ec), this_type::pro_s_ec_chk(ec));
		default:
			break;
		}

		if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::hard_link_success_if_exists)))
		{
			return ft_new_hlink == ft_to;
		}
		else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::hard_link_rename_if_exists)))
		{
			return (native::call_create_hard_link(to, unique_handler(), ec), this_type::pro_s_ec_chk(ec));
		}
		else
		{
			if(this_type::pro_s_symlink_equivalent_no_ec_chk(to, sat_to, new_hlink, sat_new_hlink, ec))
			{
				return this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::hard_link_success_if_equivalent));
			}
			else
			{
				return this_type::pro_s_ec_chk(ec)
						&& ((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::hard_link_replace_if_exists_not_dir))
								&& ft_new_hlink != boost::filesystem::directory_file)
							|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::hard_link_replace_if_exists)))
						&& (native::call_remove_all(new_hlink, ec), this_type::pro_s_ec_chk(ec))
						&& (native::call_create_hard_link(to, new_hlink, ec), this_type::pro_s_ec_chk(ec));
			}
		}
	}

public:
	// create_hard_link
	// validate options:
	//		dir_create_if_not_exists, // if the file's parent path not exists create it.
	//		hard_link_success_if_exists // if the same dir_entry exists, return true, else return false.
	//		|| hard_link_success_if_equivalent // if the equivalent dir_entry exists, return true, else return false.
	//		|| hard_link_replace_if_exists_not_dir // if the dir_entry exists and is not directory, remove it and create a new hlink.
	//		|| hard_link_replace_if_exists // if the dir_entry exists, remove it and create a new hlink
	//		|| hard_link_rename_if_exists // if the dir_entry is exists, create a new hlink with a unique rule path name.

	// throw exception_type, compatibility::stl_exception
	inline static bool create_hard_link(const path_type& to, const path_type& new_hlink, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_hlink);
		return this_type::pro_s_create_hard_link(to, new_hlink, unique_handler, 0, opts);
	}

	// non-throw
	inline static bool create_hard_link(const path_type& to, const path_type& new_hlink,
										error_code_type& ec, u32 opts = 0)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_hlink);
		return this_type::pro_s_create_hard_link(to, new_hlink, unique_handler, boost::addressof(ec), opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		create_hard_link(const path_type& to, const path_type& new_hlink,
							const Unique_Handler& unique_hanlder,
							u32 opts = static_cast<u32>(options_type::file_rename_if_exists))
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_hlink);
		return this_type::pro_s_create_hard_link(to, new_hlink, unique_handler, 0, opts);
	}

	// non-thow
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		create_hard_link(const path_type& to, const path_type& new_hlink,
							const Unique_Handler& unique_hanlder,
							error_code_type& ec,
							u32 opts = static_cast<u32>(options_type::file_rename_if_exists))
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_hlink);
		return this_type::pro_s_create_hard_link(
				to, new_hlink, unique_handler, boost::addressof(ec), opts);
	}


	//------------------all_type and recursive---------------------------

protected:
	inline static bool pro_s_opts_chk_copy(u32 opts)
	{
		u32 chk_opt = 0;

		chk_opt =
			check_bit_options<u32>(
				opts,
				(static_cast<u32>(options_type::dir_merge_if_exists)
					| static_cast<u32>(options_type::dir_replace_if_exists_not_dir)
					| static_cast<u32>(options_type::dir_replace_if_exists)
					| static_cast<u32>(options_type::dir_rename_if_exists) ));

		switch(chk_opt)
		{
		case 0:
		case options_type::dir_merge_if_exists:
		case options_type::dir_replace_if_exists_not_dir:
		case options_type::dir_replace_if_exists:
		case options_type::dir_rename_if_exists:
			break;
		default:
			return false;
		}

		chk_opt =
			check_bit_options<u32>(
				opts,
				(static_cast<u32>(options_type::file_replace_if_exists_not_dir)
					| static_cast<u32>(options_type::file_replace_if_exists)
					| static_cast<u32>(options_type::file_rename_if_exists) ));


		switch(chk_opt)
		{
		case 0:
		case options_type::file_replace_if_exists_not_dir:
		case options_type::file_replace_if_exists:
		case options_type::file_rename_if_exists:
			break;
		default:
			return false;
		}

		chk_opt =
			check_bit_options<u32>(
				opts,
				(static_cast<u32>(options_type::symlink_replace_if_exists_not_dir)
					| static_cast<u32>(options_type::symlink_replace_if_exists)
					| static_cast<u32>(options_type::symlink_rename_if_exists) ));

		switch(chk_opt)
		{
		case 0:
		case options_type::symlink_replace_if_exists_not_dir:
		case options_type::symlink_replace_if_exists:
		case options_type::symlink_rename_if_exists:
			break;
		default:
			return false;
		}

		return true;
	}

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_copy(const path_type& from, const path_type& to,
					const Unique_Handler& unique_handler,
					error_code_type* ec, u32 opts, u32 ft_opts)
	{
		if(from.empty() || to.empty())
		{
			return false;
		}

		file_status_type sat_from = this_type::pro_s_path_symlink_status(from, ec);
		file_status_type sat_to = this_type::pro_s_path_symlink_status(to, ec);

		u32 ft_from = sat_from.type();

		if(!has_bit_options<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(ft_from)))
		{
			return false;
		}

		switch(ft_from)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return false;
		case boost::filesystem::directory_file:
			return this_type::prv_s_copy_directory(from, sat_from, to, sat_to, unique_handler, ec, opts);
		case boost::filesystem::symlink_file:
			return this_type::prv_s_copy_symlink(from, sat_from, to, sat_to, unique_handler, ec, opts);
		default:
			return this_type::prv_s_copy_file(from, sat_from, to, sat_to, unique_handler, ec, opts);
		}
	}

public:
	inline static bool copy(const path_type& from, const path_type& to,
							u32 opts = 0,
							u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(to);
		return this_type::pro_s_copy(from, to, unique_handler, 0, opts, ft_opts);
	}

	inline static bool copy(const path_type& from, const path_type& to,
							error_code_type& ec,
							u32 opts = 0,
							u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(to);
		return this_type::pro_s_copy(from, to, unique_handler, boost::addressof(ec), opts, ft_opts);
	}

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		copy(const path_type& from, const path_type& to,
				const Unique_Handler& unique_handler,
				u32 opts = static_cast<u32>(options_type::all_rename_if_exists),
				u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_copy(from, to, unique_handler, 0, opts, ft_opts);
	}

	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		copy(const path_type& from, const path_type& to,
				const Unique_Handler& unique_handler,
				error_code_type& ec,
				u32 opts = static_cast<u32>(options_type::all_rename_if_exists),
				u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_copy(from, to, unique_handler, boost::addressof(ec), opts, ft_opts);
	}

private:
	template<typename Unique_Handler, typename Notify_Handler>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			bool // false: failed, true: success
		>::type
		prv_s_r_recursive_copy_no_fmt(size_type& count,
										const path_type& root_from, const path_type& root_to,
										const path_type& from, const path_type& to,
										const Unique_Handler& unique_handler,
										const Notify_Handler& notify_handler,
										error_code_type* ec,
										u32 opts, u32 ft_opts,
										bool create_dir_chk)
	{
		typedef Unique_Handler unique_handler_type;

		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;

		assert(!(root_from.empty() || root_to.empty()));
		assert(!(from.empty() || to.empty()));

		assert(root_from == native::system_complete(root_from));
		assert(root_to == native::system_complete(root_to));

		assert(from == native::system_complete(from));
		assert(to == native::system_complete(to));

		path_type next_to = to;

		file_status_type sat_from, sat_to;
		u32 ft_from = 0, ft_to = 0;

		bool b_igonre_if_failed =
			this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::recursive_operator_ignore_if_failed));

		try
		{
			sat_from = this_type::pro_s_path_symlink_status(from, ec);
			sat_to = this_type::pro_s_path_symlink_status(to, ec);
		}
		catch(const exception_type&)
		{
			sat_from = file_status_type();
			sat_to = file_status_type();
		}

		ft_from = sat_from.type();
		ft_to = sat_to.type();

		bool bcpable = has_bit_options<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(ft_from));

		switch(ft_from)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
			return false;
		// copy directory
		case boost::filesystem::directory_file:
			try
			{
				if(this_type::pro_s_check_is_included_in_directory_no_fmt(from, to, ec)
					|| !this_type::pro_s_ec_chk(ec))
				{
					notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
					return false;
				}
			}
			catch(const exception_type&)
			{
				notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
				return false;
			}

			if(!bcpable)
			{
				break;
			}

			switch(ft_to)
			{
			case boost::filesystem::status_error:
				notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
				return false;
			case boost::filesystem::file_not_found:
				if((create_dir_chk? count == 0 : true)
					&& this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
				{
					chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(to.native());
					if(!yggr::get<2>(chk_name_ret))
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
					const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
					try
					{
						empty_unique_handler_type empty_unique_handler;
						if(!this_type::pro_s_create_directories(
								this_type::make_path_object(iter_pair.first, iter_pair.second),
								empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
								static_cast<u32>(options_type::dir_success_if_exists)))
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}

				try
				{
#			if BOOST_VERSION < 107400
					if(!(native::call_copy_directory(from, to, ec), this_type::pro_s_ec_chk(ec)))
#			else
					if(!(native::call_copy_directory(from, to, ec) && this_type::pro_s_ec_chk(ec)))
#			endif // BOOST_VERSION < 107400
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				catch(const exception_type&)
				{
					notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
					return false;
				}
				++count;
				notify_handler(root_from, root_to, from, to, true, count, ec, true);
				break;
			case boost::filesystem::directory_file:
				if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_replace_if_exists_not_dir)))
				{
					notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
					return false;
				}

				{
					bool is_equivalent = false;
					try
					{
						is_equivalent = this_type::pro_s_symlink_equivalent_no_ec_chk(from, sat_from, to, sat_to, ec);
						if(!this_type::pro_s_ec_chk(ec))
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}

					if(is_equivalent
						&& has_bit_options_in<u32>(
								opts,
								static_cast<u32>(options_type::dir_success_if_exists)
								| static_cast<u32>(options_type::dir_merge_if_exists)
								| static_cast<u32>(options_type::dir_replace_if_exists)))
					{
						++count;
						notify_handler(root_from, root_to, from, to, true, count, ec, true);

						if(has_bit_options_in<u32>(
							opts,
							static_cast<u32>(options_type::file_rename_if_exists)
							| static_cast<u32>(options_type::symlink_rename_if_exists)) )
						{
							break;
						}
						else
						{
							count += this_type::pro_s_recursive_count_children(to, ec, ft_opts);
							notify_handler(root_from, root_to, from, to, true, count, ec, true);
							return true;
						}
					}
				}

				if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_merge_if_exists)))
				{
					++count;
					notify_handler(root_from, root_to, from, to, true, count, ec, true);
					break;
				}
				else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_success_if_exists)))
				{
					count += this_type::pro_s_recursive_count_children(to, ec, ft_opts) + 1;
					notify_handler(root_from, root_to, from, to, true, count, ec, true);
					return true;
				}
			default:
				if(has_bit_options_in<u32>(
						opts,
						static_cast<u32>(options_type::dir_replace_if_exists)
						| static_cast<u32>(options_type::dir_replace_if_exists_not_dir)) )
				{
					try
					{
#				if BOOST_VERSION < 107400
						if(!((native::call_remove_all(to, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_copy_directory(from, to, ec), this_type::pro_s_ec_chk(ec))))
#				else
						if(!((native::call_remove_all(to, ec), this_type::pro_s_ec_chk(ec))
							&& (native::call_copy_directory(from, to, ec) && this_type::pro_s_ec_chk(ec))))
#				endif // BOOST_VERSION < 107400
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}

						++count;
						notify_handler(root_from, root_to, from, to, true, count, ec, true);
						break;
					}
					catch(const exception_type&)
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_rename_if_exists)))
				{
					next_to = unique_handler();
					try
					{
#				if BOOST_VERSION < 107400
						if(!(native::call_copy_directory(from, next_to, ec), this_type::pro_s_ec_chk(ec)))
#				else
						if(!(native::call_copy_directory(from, next_to, ec) && this_type::pro_s_ec_chk(ec)))
#				endif // BOOST_VERSION < 107400
						{
							notify_handler(root_from, root_to, from, next_to, false, count, ec, b_igonre_if_failed);
							return false;
						}

						++count;
						notify_handler(root_from, root_to, from, next_to, true, count, ec, true);
						break;
					}
					catch(const exception_type&)
					{
						notify_handler(root_from, root_to, from, next_to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				else
				{
					notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
					return false;
				}
			}
			break;

		// copy symlink
		case boost::filesystem::symlink_file:
			if(!bcpable)
			{
				return true;
			}

			switch(ft_to)
			{
			case boost::filesystem::status_error:
				notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
				return false;
			case boost::filesystem::file_not_found:
				if((create_dir_chk? count == 0 : true)
					&& this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
				{
					chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(to.native());
					if(!yggr::get<2>(chk_name_ret))
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
					const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);

					try
					{
						empty_unique_handler_type empty_unique_handler;
						if(!this_type::pro_s_create_directories(
								this_type::make_path_object(iter_pair.first, iter_pair.second),
								empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
								static_cast<u32>(options_type::dir_success_if_exists)))
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}

				try
				{
					if((native::call_copy_symlink(from, to, ec), this_type::pro_s_ec_chk(ec)))
					{
						++count;
						notify_handler(root_from, root_to, from, to, true, count, ec, true);
					}
					else
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				catch(const exception_type&)
				{
					notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
					return false;
				}
				return true;
			default:
				if(!bcpable)
				{
					return true;
				}

				if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_rename_if_exists)))
				{
					try
					{
						if((native::call_copy_symlink(from, unique_handler(), ec), this_type::pro_s_ec_chk(ec)))
						{
							++count;
							notify_handler(root_from, root_to, from, to, true, count, ec, true);
						}
						else
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				else
				{
					bool bequivalent = false;

					try
					{
						bequivalent = this_type::pro_s_symlink_equivalent_no_ec_chk(from, sat_from, to, sat_to, ec);
						if(!this_type::pro_s_ec_chk(ec))
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}

					if(bequivalent)
					{
						if(has_bit_options_in<u32>(
								opts, static_cast<u32>(options_type::symlink_success_if_exists)
										| static_cast<u32>(options_type::symlink_replace_if_exists_not_dir)
										| static_cast<u32>(options_type::symlink_replace_if_exists)))
						{
							++count;
							notify_handler(root_from, root_to, from, to, true, count, ec, true);
						}
						else
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists_not_dir))
								&& ft_to != boost::filesystem::directory_file)
							|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists)))
					{
						try
						{
							if(!((native::call_remove_all(to, ec), this_type::pro_s_ec_chk(ec))
									&& (native::call_copy_symlink(from, to, ec), this_type::pro_s_ec_chk(ec))))
							{
								notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
								return false;
							}
							else
							{
								++count;
								notify_handler(root_from, root_to, from, to, true, count, ec, true);
							}
						}
						catch(const exception_type&)
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_success_if_exists))
								&& ft_to == boost::filesystem::symlink_file)
					{
						++count;
						notify_handler(root_from, root_to, from, to, true, count, ec, true);
					}
					else
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
			}
			return true;

		// copy_file
		default:
			if(!bcpable)
			{
				return true;
			}

			switch(ft_to)
			{
			case boost::filesystem::status_error:
				notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
				return false;
			case boost::filesystem::file_not_found:
				if((create_dir_chk? count == 0 : true)
					&& this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
				{
					chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(to.native());
					if(!yggr::get<2>(chk_name_ret))
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
					const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);

					try
					{
						empty_unique_handler_type empty_unique_handler;
						if(!this_type::pro_s_create_directories(
								this_type::make_path_object(iter_pair.first, iter_pair.second),
								empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
								static_cast<u32>(options_type::dir_success_if_exists)))
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}

				try
				{
#if (BOOST_VERSION < 107400)
					if((native::call_copy_file(from, to, ec), this_type::pro_s_ec_chk(ec)))
#else
					if(native::call_copy_file(from, to, ec))
#endif // (BOOST_VERSION < 107400)
					{
						++count;
						notify_handler(root_from, root_to, from, to, true, count, ec, true);
					}
					else
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				catch(const exception_type&)
				{
					notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
					return false;
				}
				return true;
			default:
				if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_rename_if_exists)))
				{
					try
					{
#if (BOOST_VERSION < 107400)
						if((native::call_copy_file(from, unique_handler(), ec), this_type::pro_s_ec_chk(ec)))
#else
						if(native::call_copy_file(from, unique_handler(), ec))
#endif // (BOOST_VERSION < 107400)
						{
							++count;
							notify_handler(root_from, root_to, from, to, true, count, ec, true);
						}
						else
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				else
				{
					bool bequivalent = false;
					try
					{
						bequivalent = this_type::pro_s_symlink_equivalent_no_ec_chk(from, sat_from, to, sat_to, ec);
						if(!this_type::pro_s_ec_chk(ec))
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}

					if(bequivalent)
					{
						if(has_bit_options_in<u32>(
							opts, static_cast<u32>(options_type::file_success_if_exists)
									| static_cast<u32>(options_type::file_replace_if_exists_not_dir)
									| static_cast<u32>(options_type::file_replace_if_exists) ))
						{
							++count;
							notify_handler(root_from, root_to, from, to, true, count, ec, true);
						}
						else
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists_not_dir))
								&& ft_to != boost::filesystem::directory_file)
							|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists)))
					{
						try
						{
							if(!((native::call_remove_all(to, ec), this_type::pro_s_ec_chk(ec))
#if (BOOST_VERSION < 107400)
									&& (native::call_copy_file(from, to, ec), this_type::pro_s_ec_chk(ec))))
#else
									&& native::call_copy_file(from, to, ec) ))
#endif // (BOOST_VERSION < 107400)
							{
								notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
								return false;
							}
							else
							{
								++count;
								notify_handler(root_from, root_to, from, to, true, count, ec, true);
							}
						}
						catch(const exception_type&)
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_success_if_exists)))
					{
						if(ft_to == ft_from)
						{
							++count;
							notify_handler(root_from, root_to, from, to, true, count, ec, true);
						}
						else
						{
							notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else
					{
						notify_handler(root_from, root_to, from, to, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
			}
			return true;
		}

		assert(ft_from == boost::filesystem::directory_file);
		directory_iterator_type dir_iter, dir_iter_end;

		try
		{
			dir_iter = this_type::pro_s_make_directory_iterator_object(from, ec);
		}
		catch(const exception_type&)
		{
			return false;
		}

		path_type tmp_path;
		const path_string_type& from_str = get_path_string(from);
		path_string_type& tmp_str = get_path_string(tmp_path);

		container::reserve(tmp_str, YGGR_FSYS_PATH_BUF_RECOMMEND_SIZE());
		tmp_path = next_to;

		if(!base_type::pro_s_is_slants(*tmp_str.rbegin()))
		{
			tmp_str += base_type::pro_s_slant<path_char_type>();
		}

		size_type from_size =
			from_str.size() + static_cast<std::size_t>(!base_type::pro_s_is_slants(*from_str.rbegin()));

		size_type tmp_size = tmp_str.size();

		if(b_igonre_if_failed)
		{
			for(;dir_iter != dir_iter_end;)
			{
				charset::basic_string_replace(tmp_str, tmp_size, -1, dir_iter->path().native(), from_size, -1);
				unique_handler_type now_unqiue_handler(tmp_path);
				this_type::prv_s_r_recursive_copy_no_fmt(
					count, root_from, root_to,
					dir_iter->path(), tmp_path,
					now_unqiue_handler, notify_handler,
					ec, opts, ft_opts, create_dir_chk);

				this_type::pro_s_ec_clear(ec);

				// the error is not passedable, so don't use try-catch
				if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
				{
					return false;
				}
			}
		}
		else
		{
			bool bcontinue = false;
			for(;dir_iter != dir_iter_end;)
			{
				charset::basic_string_replace(tmp_str, tmp_size, -1, dir_iter->path().native(), from_size, -1);
				unique_handler_type now_unqiue_handler(tmp_path);
				bcontinue =
					this_type::prv_s_r_recursive_copy_no_fmt(
						count, root_from, root_to,
						dir_iter->path(), tmp_path,
						now_unqiue_handler, notify_handler,
						ec, opts, ft_opts, create_dir_chk);

				if(!(bcontinue && this_type::pro_s_ec_chk(ec)))
				{
					return false;
				}

				if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
				{
					return false;
				}
			}
		}

		return true;
	}

protected:

	//template<typename Unique_Handler, typename Notify_Handler> inline
	//static
	//typename
	//	boost::enable_if
	//	<
	//		boost::mpl::and_
	//		<
	//			is_callable<Unique_Handler>,
	//			is_callable<Notify_Handler>
	//		>,
	//		size_type
	//	>::type
	//	pro_s_recursive_copy_no_fmt(const path_type& from, const path_type& to,
	//								const Unique_Handler& unique_handler,
	//								const Notify_Handler& notify_handler,
	//								error_code_type* ec,
	//								u32 opts, u32 ft_opts)
	//{
	//	assert(!(from.empty() || to.empty()));
	//	assert(from == native::system_complete(from));
	//	assert(to == native::system_complete(to));
	//	assert(unique_handler.src_path() == to);

	//	size_type count = 0;
	//	bool create_dir_chk =
	//		this_type::pro_s_has_opts(
	//			opts,
	//			static_cast<u32>(options_type::dir_create_if_not_exists))
	//		&& this_type::pro_s_has_opts(
	//				ft_opts,
	//				static_cast<u32>(file_types_options_type::directory_file));

	//	this_type::prv_s_r_recursive_copy_no_fmt(
	//		count, from, to, from, to,
	//		unique_handler, notify_handler, ec, opts, ft_opts, create_dir_chk);

	//	return count;
	//}

	template<typename Unique_Handler, typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			size_type
		>::type
		pro_s_recursive_copy(const path_type& from, const path_type& to,
								const Unique_Handler& unique_handler,
								const Notify_Handler& notify_handler,
								error_code_type* ec,
								u32 opts, u32 ft_opts)
	{
		path_type sc_from;
		path_type sc_to;

		size_type count = 0;
		bool create_dir_chk =
			this_type::pro_s_has_opts(
				opts,
				static_cast<u32>(options_type::dir_create_if_not_exists))
			&& this_type::pro_s_has_opts(
					ft_opts,
					static_cast<u32>(file_types_options_type::directory_file));

		if(!from.empty() && !to.empty()
			&& (sc_from = native::call_system_complete(from), this_type::pro_s_ec_chk(ec))
			&& (sc_to = native::call_system_complete(to), this_type::pro_s_ec_chk(ec)))
		{
			Unique_Handler sc_unque_handler(sc_to);
			this_type::prv_s_r_recursive_copy_no_fmt(
				count, sc_from, sc_to, sc_from, sc_to,
				sc_unque_handler, notify_handler, ec, opts, ft_opts, create_dir_chk);
		}

		return count;
	}

public:

	// throw exception_type, compatibility::stl_exception
	inline static size_type
		recursive_copy(const path_type& from, const path_type& to,
						u32 opts = 0,
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::rename_handler_type unique_handler_type;

		typedef typename
			default_handlers_type::recursive_copy_notify_handler_type notify_handler_type;

		unique_handler_type unique_handler(to);
		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_copy(
				from, to,
				unique_handler, notify_handler,
				0, opts, ft_opts);
	}

	// non-throw
	inline static size_type
		recursive_copy(const path_type& from, const path_type& to,
						error_code_type& ec,
						u32 opts = 0,
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::rename_handler_type unique_handler_type;

		typedef typename
			default_handlers_type::recursive_copy_notify_handler_type notify_handler_type;

		unique_handler_type unique_handler(to);
		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_copy(
				from, to, unique_handler, notify_handler,
				boost::addressof(ec), opts, ft_opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Notify_Handler>,
				boost::mpl::bool_<!!(func::foo_t_info<Notify_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_copy(const path_type& from, const path_type& to,
						const Notify_Handler& notify_handler,
						u32 opts = 0,
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::rename_handler_type unique_handler_type;

		unique_handler_type unique_handler(to);
		return this_type::pro_s_recursive_copy(
				from, to, unique_handler, notify_handler,
				0, opts, ft_opts);
	}

	// non-throw
	template<typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Notify_Handler>,
				boost::mpl::bool_<!!(func::foo_t_info<Notify_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_copy(const path_type& from, const path_type& to,
						const Notify_Handler& notify_handler,
						error_code_type& ec,
						u32 opts = 0,
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::rename_handler_type unique_handler_type;

		unique_handler_type unique_handler(to);
		return this_type::pro_s_recursive_copy(
				from, to, unique_handler, notify_handler,
				boost::addressof(ec), opts, ft_opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				boost::mpl::bool_<!(func::foo_t_info<Unique_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_copy(const path_type& from, const path_type& to,
						const Unique_Handler& unique_handler,
						u32 opts = static_cast<u32>(options_type::all_rename_if_exists),
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::recursive_copy_notify_handler_type notify_handler_type;

		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_copy(
				from, to, unique_handler, notify_handler, 0, opts, ft_opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				boost::mpl::bool_<!(func::foo_t_info<Unique_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_copy(const path_type& from, const path_type& to,
						const Unique_Handler& unique_handler,
						error_code_type& ec,
						u32 opts = static_cast<u32>(options_type::all_rename_if_exists),
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::recursive_copy_notify_handler_type notify_handler_type;

		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_copy(
				from, to, unique_handler, notify_handler,
				boost::addressof(ec), opts, ft_opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler, typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			size_type
		>::type
		recursive_copy(const path_type& from, const path_type& to,
						const Unique_Handler& unique_handler,
						const Notify_Handler& notify_handler,
						u32 opts = static_cast<u32>(options_type::all_rename_if_exists),
						u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_recursive_copy(
				from, to, unique_handler, notify_handler,
				0, opts, ft_opts);
	}

	// non-throw
	template<typename Unique_Handler, typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			size_type
		>::type
		recursive_copy(const path_type& from, const path_type& to,
						const Unique_Handler& unique_handler,
						const Notify_Handler& notify_handler,
						error_code_type& ec,
						u32 opts = static_cast<u32>(options_type::all_rename_if_exists),
						u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_recursive_copy(
				from, to, unique_handler, notify_handler,
				boost::addressof(ec), opts, ft_opts);
	}


protected:
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		pro_s_move(const path_type& old_path, const path_type& new_path,
					const Unique_Handler& unique_handler,
					error_code_type* ec,
					u32 opts, u32 ft_opts)
	{
		if(old_path.empty() || new_path.empty())
		{
			return false;
		}

		file_status_type sat_old = this_type::pro_s_path_symlink_status(old_path, ec);
		file_status_type sat_new = this_type::pro_s_path_symlink_status(new_path, ec);

		u32 ft_old = sat_old.type();

		if(!has_bit_options<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(ft_old)))
		{
			return false;
		}

		switch(ft_old)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return false;
		case boost::filesystem::directory_file:
			return this_type::prv_s_move_directory(old_path, sat_old, new_path, sat_new, unique_handler, ec, opts);
		case boost::filesystem::symlink_file:
			return this_type::prv_s_move_symlink(old_path, sat_old, new_path, sat_new, unique_handler, ec, opts);
		default:
			return this_type::prv_s_move_file(old_path, sat_old, new_path, sat_new, unique_handler, ec, opts);
		}
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static bool move(const path_type& old_path, const path_type& new_path,
							u32 opts = 0,
							u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_move(old_path, new_path, unique_handler, 0, opts, ft_opts);
	}

	// non-throw
	inline static bool move(const path_type& old_path, const path_type& new_path,
							error_code_type& ec,
							u32 opts = 0,
							u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename default_handlers_type::rename_handler_type rename_handler_type;

		rename_handler_type unique_handler(new_path);
		return this_type::pro_s_move(old_path, new_path, unique_handler, boost::addressof(ec), opts, ft_opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		move(const path_type& old_path, const path_type& new_path,
				const Unique_Handler& unique_handler,
				u32 opts = static_cast<u32>(options_type::all_rename_if_exists),
				u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_move(old_path, new_path, unique_handler, 0, opts, ft_opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static typename boost::enable_if<is_callable<Unique_Handler>, bool>::type
		move(const path_type& old_path, const path_type& new_path,
				const Unique_Handler& unique_handler,
				error_code_type& ec,
				u32 opts = static_cast<u32>(options_type::all_rename_if_exists),
				u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_move(old_path, new_path, unique_handler, boost::addressof(ec), opts, ft_opts);
	}

private:
	template<typename Unique_Handler, typename Notify_Handler>
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			bool
		>::type
		prv_s_r_recursive_move_no_fmt(size_type& count,
										const path_type& root_old_path, const path_type& root_new_path,
										const path_type& old_path, const path_type& new_path,
										const Unique_Handler& unique_handler,
										const Notify_Handler& notify_handler,
										error_code_type* ec,
										u32 opts, u32 ft_opts,
										bool create_dir_chk)
	{
		typedef Unique_Handler unique_handler_type;

		typedef path_string_type::const_iterator path_string_iter_type;
		typedef std::pair<path_string_iter_type, path_string_iter_type> chk_data_type;
		typedef yggr::tuple<chk_data_type, chk_data_type, bool> chk_name_ret_type;

		assert(!(root_old_path.empty() || root_new_path.empty()));
		assert(!(old_path.empty() || new_path.empty()));

		assert(root_old_path == native::system_complete(root_old_path));
		assert(root_new_path == native::system_complete(root_new_path));

		assert(old_path == native::system_complete(old_path));
		assert(new_path == native::system_complete(new_path));

		//size_type count = 0;
		path_type next_new = new_path;

		file_status_type sat_old, sat_new;
		u32 ft_old = 0, ft_new = 0;

		bool bneed_remove_dir = false;

		bool b_igonre_if_failed =
			this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::recursive_operator_ignore_if_failed));

		try
		{
			sat_old = this_type::pro_s_path_symlink_status(old_path, ec);
			sat_new = this_type::pro_s_path_symlink_status(new_path, ec);
		}
		catch(const exception_type&)
		{
			sat_old = file_status_type();
			sat_new = file_status_type();
		}

		ft_old = sat_old.type();
		ft_new = sat_new.type();

		bool bmoveable = has_bit_options<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(ft_old));

		switch(ft_old)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
			return false;
		// move directory
		case boost::filesystem::directory_file:
			try
			{
				if(this_type::pro_s_check_is_included_in_directory_no_fmt(old_path, new_path, ec)
					|| !this_type::pro_s_ec_chk(ec))
				{
					notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
					return false;
				}
			}
			catch(const exception_type&)
			{
				notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
				return false;
			}

			if(!bmoveable)
			{
				break;
			}

			switch(ft_new)
			{
			case boost::filesystem::status_error:
				notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
				return false;
			case boost::filesystem::file_not_found:
				if((create_dir_chk? 0 == count : true)
					&& this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
				{
					chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(new_path.native());
					if(!yggr::get<2>(chk_name_ret))
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
					const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
					try
					{
						empty_unique_handler_type empty_unique_handler;
						if(!this_type::pro_s_create_directories(
								this_type::make_path_object(iter_pair.first, iter_pair.second),
								empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
								static_cast<u32>(options_type::dir_success_if_exists)))
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}

				try
				{
					bool bequal_vol =
						native::call_symlink_is_in_same_volume(
							old_path, this_type::parent_path(new_path), ec);

					if(!this_type::pro_s_ec_chk(ec))
					{
						assert(ec);
						return false;
					}

					if(bequal_vol && ft_opts == static_cast<u32>(file_types_options_type::types_all))
					{
						if(!(native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec)))
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
						else
						{
							count += this_type::pro_s_recursive_count_children(new_path, ec, ft_opts) + 1;
							notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
							return true;
						}
					}
					else
					{
#				if BOOST_VERSION < 107400
						if(!(native::call_copy_directory(old_path, new_path, ec), this_type::pro_s_ec_chk(ec)))
#				else
						if(!(native::call_copy_directory(old_path, new_path, ec) && this_type::pro_s_ec_chk(ec)))
#				endif // BOOST_VERSION < 107400
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}

						bneed_remove_dir = true;
						++count;
						notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
					}
				}
				catch(const exception_type&)
				{
					notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
					return false;
				}
				break;
			case boost::filesystem::directory_file:
				if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_replace_if_exists_not_dir)))
				{
					notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
					return false;
				}

				{
					bool is_equivalent = false;
					try
					{
						is_equivalent = this_type::pro_s_symlink_equivalent_no_ec_chk(old_path, sat_old, new_path, sat_new, ec);
						if(!this_type::pro_s_ec_chk(ec))
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}

					if(is_equivalent
						&& has_bit_options_in<u32>(
								opts,
								static_cast<u32>(options_type::dir_success_if_exists)
								| static_cast<u32>(options_type::dir_merge_if_exists)
								| static_cast<u32>(options_type::dir_replace_if_exists)))
					{
						++count;
						notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
						count += this_type::pro_s_recursive_count_children(new_path, ec, ft_opts);
						notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
						return true;
					}
				}

				if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_merge_if_exists)))
				{
					bneed_remove_dir = true;
					++count;
					notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
					break;
				}
				else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_success_if_exists)))
				{
					++count;
					notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
					return true;
				}
			default:
				if(has_bit_options_in<u32>(
						opts,
						static_cast<u32>(options_type::dir_replace_if_exists)
						| static_cast<u32>(options_type::dir_replace_if_exists_not_dir)) )
				{
					try
					{
						if(!(native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec)))
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}

						bool bequal_vol = native::call_symlink_is_in_same_volume(old_path, new_path, ec);

						if(!this_type::pro_s_ec_chk(ec))
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}

						if(bequal_vol && ft_opts == static_cast<u32>(file_types_options_type::types_all))
						{
							if(!(native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec)))
							{
								notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
								return false;
							}
							else
							{
								count += this_type::pro_s_recursive_count_children(new_path, ec, ft_opts) + 1;
								notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
								return true;
							}
						}
						else
						{
#				if BOOST_VERSION < 107400
							if(!(native::call_copy_directory(old_path, new_path, ec), this_type::pro_s_ec_chk(ec)))
#				else
							if(!(native::call_copy_directory(old_path, new_path, ec) && this_type::pro_s_ec_chk(ec)))
#				endif // BOOST_VERSION < 107400
							{
								notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
								return false;
							}
							bneed_remove_dir = true;
							++count;
							notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
							break;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_rename_if_exists)))
				{
					next_new = unique_handler();
					try
					{
						bool bequal_vol = native::call_symlink_is_in_same_volume(old_path, new_path, ec);

						if(!this_type::pro_s_ec_chk(ec))
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}

						if(bequal_vol && ft_opts == static_cast<u32>(file_types_options_type::types_all))
						{
							if(!(native::call_rename(old_path, next_new, ec), this_type::pro_s_ec_chk(ec)))
							{
								notify_handler(root_old_path, root_new_path, old_path, next_new, false, count, ec, b_igonre_if_failed);
								return false;
							}
							else
							{
								count += this_type::pro_s_recursive_count_children(next_new, ec, ft_opts) + 1;
								notify_handler(root_old_path, root_new_path, old_path, next_new, true, count, ec, true);
								return true;
							}
						}
						else
						{
							if(!(native::call_rename(old_path, next_new, ec), this_type::pro_s_ec_chk(ec)))
							{
								notify_handler(root_old_path, root_new_path, old_path, next_new, false, count, ec, b_igonre_if_failed);
								return false;
							}

							bneed_remove_dir = true;
							++count;
							notify_handler(root_old_path, root_new_path, old_path, next_new, true, count, ec, true);
						}

					}
					catch(const exception_type&)
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				else
				{
					notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
					return false;
				}
			}
			break;

		// move symlink
		case boost::filesystem::symlink_file:
			if(!bmoveable)
			{
				return true;
			}

			switch(ft_new)
			{
			case boost::filesystem::status_error:
				notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
				return false;
			case boost::filesystem::file_not_found:
				if((create_dir_chk? 0 == count : true)
					&& this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
				{
					chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(new_path.native());
					if(!yggr::get<2>(chk_name_ret))
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
					const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);

					try
					{
						empty_unique_handler_type empty_unique_handler;
						if(!this_type::pro_s_create_directories(
								this_type::make_path_object(iter_pair.first, iter_pair.second),
								empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
								static_cast<u32>(options_type::dir_success_if_exists)))
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}

				try
				{
#if defined(YGGR_AT_WINDOWS)
					if((native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec)))
#else
					if( this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(new_path), ec)?
							this_type::pro_s_ec_chk(ec) 
							&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
						: this_type::pro_s_ec_chk(ec) 
							&& (native::call_copy_symlink(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
							&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec) )
#endif // YGGR_AT_WINDOWS
					{
						++count;
						notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
					}
				}
				catch(const exception_type&)
				{
					notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
					return false;
				}
				return true;
			default:
				if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_rename_if_exists)))
				{
					try
					{
#if defined(YGGR_AT_WINDOWS)
						if((native::call_rename(old_path, unique_handler(), ec), this_type::pro_s_ec_chk(ec)))
#else
						path_type unique_new_path = unique_handler();
						if( this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(unique_new_path), ec)?
								this_type::pro_s_ec_chk(ec) 
								&& (native::call_rename(old_path, unique_new_path, ec), this_type::pro_s_ec_chk(ec))
								: this_type::pro_s_ec_chk(ec) 
									&& (native::call_copy_symlink(old_path, unique_new_path, ec), this_type::pro_s_ec_chk(ec))
									&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec) )
#endif // YGGR_AT_WINDOWS
						{
							++count;
							notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
						}
						else
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				else
				{
					bool bequivalent = false;
					try
					{
						bequivalent = this_type::pro_s_symlink_equivalent_no_ec_chk(old_path, sat_old, new_path, sat_new, ec);
						if(!this_type::pro_s_ec_chk(ec))
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}

					if(bequivalent)
					{
						if(has_bit_options_in<u32>(
								opts, static_cast<u32>(options_type::symlink_success_if_exists)
										| static_cast<u32>(options_type::symlink_replace_if_exists_not_dir)
										| static_cast<u32>(options_type::symlink_replace_if_exists)))
						{
							++count;
							notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
						}
						else
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists_not_dir))
								&& ft_new != boost::filesystem::directory_file)
							|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_replace_if_exists)))
					{
						try
						{
#if defined(YGGR_AT_WINDOWS)
							if(!((native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
									&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))))
#else
							if(!( (native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
									&& (this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(new_path), ec)?
											this_type::pro_s_ec_chk(ec) 
											&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
										: this_type::pro_s_ec_chk(ec) 
											&& (native::call_copy_symlink(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
											&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec)) ))
#endif // YGGR_AT_WINDOWS
							{
								notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
								return false;
							}
							else
							{
								++count;
								notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
							}
						}
						catch(const exception_type&)
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::symlink_success_if_exists)))
					{
						if(ft_new == boost::filesystem::symlink_file)
						{
							++count;
							notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
						}
						else
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
			}
			return true;

		// move_file
		default:
			if(!bmoveable)
			{
				return true;
			}

			switch(ft_new)
			{
			case boost::filesystem::status_error:
				notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
				return false;
			case boost::filesystem::file_not_found:
				if((create_dir_chk? 0 == count : true)
					&& this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::dir_create_if_not_exists)))
				{
					chk_name_ret_type chk_name_ret = base_type::parsing_dir_path_and_name(new_path.native());
					if(!yggr::get<2>(chk_name_ret))
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}

					const chk_data_type& iter_pair = yggr::get<0>(chk_name_ret);
					try
					{
						empty_unique_handler_type empty_unique_handler;
						if(!this_type::pro_s_create_directories(
								this_type::make_path_object(iter_pair.first, iter_pair.second),
								empty_unique_handler, ec, // in pro_s_create_directories the unique_handler not using it
								static_cast<u32>(options_type::dir_success_if_exists)))
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}

				try
				{
#if defined(YGGR_AT_WINDOWS)
					if((native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec)))
#else
					if( this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(new_path), ec)?
							this_type::pro_s_ec_chk(ec) 
							&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
							: this_type::pro_s_ec_chk(ec) 
								&& native::call_copy_file(old_path, new_path, ec) && this_type::pro_s_ec_chk(ec)
								&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec) )
#endif // YGGR_AT_WINDOWS
					{
						++count;
						notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
					}
					else
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				catch(const exception_type&)
				{
					notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
					return false;
				}
				return true;
			default:
				if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_rename_if_exists)))
				{
					try
					{
#if defined(YGGR_AT_WINDOWS)
						if((native::call_rename(old_path, unique_handler(), ec), this_type::pro_s_ec_chk(ec)))
#else
						path_type unique_new_path = unique_handler();

						if( this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(unique_new_path), ec)?
								this_type::pro_s_ec_chk(ec) 
								&& (native::call_rename(old_path, unique_new_path, ec), this_type::pro_s_ec_chk(ec))
								: this_type::pro_s_ec_chk(ec) 
									&& native::call_copy_file(old_path, unique_new_path, ec) && this_type::pro_s_ec_chk(ec)
									&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec) )
#endif // YGGR_AT_WINDOWS
						{
							++count;
							notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
						}
						else
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				else
				{
					bool bequivalent = false;
					try
					{
						bequivalent = this_type::pro_s_symlink_equivalent_no_ec_chk(old_path, sat_old, new_path, sat_new, ec);
						if(!this_type::pro_s_ec_chk(ec))
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					catch(const exception_type&)
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}

					if(bequivalent)
					{
						if(has_bit_options_in<u32>(
								opts, static_cast<u32>(options_type::file_success_if_exists)
										| static_cast<u32>(options_type::file_replace_if_exists_not_dir)
										| static_cast<u32>(options_type::file_replace_if_exists)))
						{
							++count;
							notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
						}
						else
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else if((this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists_not_dir))
								&& ft_new != boost::filesystem::directory_file)
							|| this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_replace_if_exists)))
					{

						try
						{
#if defined(YGGR_AT_WINDOWS)
							if(!((native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
									&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))))
#else
							if(!( (native::call_remove_all(new_path, ec), this_type::pro_s_ec_chk(ec))
									&& (this_type::pro_s_is_in_same_volume(old_path, this_type::parent_path(new_path), ec)?
										this_type::pro_s_ec_chk(ec) 
										&& (native::call_rename(old_path, new_path, ec), this_type::pro_s_ec_chk(ec))
										: this_type::pro_s_ec_chk(ec) 
											&& native::call_copy_file(old_path, new_path, ec) && this_type::pro_s_ec_chk(ec)
											&& native::call_remove(old_path, ec) && this_type::pro_s_ec_chk(ec)) ))
#endif // YGGR_AT_WINDOWS
							{
								notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
								return false;
							}
							else
							{
								++count;
								notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
							}
						}
						catch(const exception_type&)
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::file_success_if_exists)))
					{
						if(ft_new == ft_old)
						{
							++count;
							notify_handler(root_old_path, root_new_path, old_path, new_path, true, count, ec, true);
						}
						else
						{
							notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
							return false;
						}
					}
					else
					{
						notify_handler(root_old_path, root_new_path, old_path, new_path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
			}
			return true;
		}

		assert(ft_old == boost::filesystem::directory_file);
		directory_iterator_type dir_iter, dir_iter_end;

		try
		{
			dir_iter = this_type::pro_s_make_directory_iterator_object(old_path, ec);
		}
		catch(const exception_type&)
		{
			return false;
		}

		path_type tmp_path;
		const path_string_type& old_str = get_path_string(old_path);
		path_string_type& tmp_str = get_path_string(tmp_path);

		container::reserve(tmp_str, YGGR_FSYS_PATH_BUF_RECOMMEND_SIZE());
		tmp_path = next_new;

		if(!base_type::pro_s_is_slants(*tmp_str.rbegin()))
		{
			tmp_str += base_type::pro_s_slant<path_char_type>();
		}

		size_type old_size =
			old_str.size() + static_cast<std::size_t>(!base_type::pro_s_is_slants(*old_str.rbegin()));

		size_type tmp_size = tmp_str.size();

		bool bret = true;
		if(b_igonre_if_failed)
		{
			for(;dir_iter != dir_iter_end; /*++dir_iter*/)
			{
				charset::basic_string_replace(tmp_str, tmp_size, -1, dir_iter->path().native(), old_size, -1);
				unique_handler_type now_unqiue_handler(tmp_path);

				this_type::prv_s_r_recursive_move_no_fmt(
					count, root_old_path, root_new_path,
					dir_iter->path(), tmp_path,
					now_unqiue_handler, notify_handler,
					ec, opts, ft_opts, create_dir_chk);

				this_type::pro_s_ec_clear(ec);

				if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
				{
					bret = false;
					break;
				}
			}
		}
		else
		{
			bool bcontinue = false;
			for(;dir_iter != dir_iter_end; /*++dir_iter*/)
			{
				charset::basic_string_replace(tmp_str, tmp_size, -1, dir_iter->path().native(), old_size, -1);
				unique_handler_type now_unqiue_handler(tmp_path);

				bcontinue =
					this_type::prv_s_r_recursive_move_no_fmt(
						count, root_old_path, root_new_path,
						dir_iter->path(), tmp_path,
						now_unqiue_handler, notify_handler,
						ec, opts, ft_opts, create_dir_chk);


				if(!(bcontinue && this_type::pro_s_ec_chk(ec)))
				{
					bret = false;
					break;
				}

				if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
				{
					bret = false;
					break;
				}
			}
		}

		if(bneed_remove_dir)
		{
			try
			{
				native::call_remove(old_path, ec);
			}
			catch(const exception_type&){}
		}

		return bret;
	}

protected:

	template<typename Unique_Handler, typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			size_type
		>::type
		pro_s_recursive_move_no_fmt(const path_type& old_path, const path_type& new_path,
									const Unique_Handler& unique_handler,
									const Notify_Handler& notify_handler,
									error_code_type* ec,
									u32 opts, u32 ft_opts)
	{
		assert(!(old_path.empty() || new_path.empty()));
		assert(old_path == native::system_complete(old_path));
		assert(new_path == native::system_complete(new_path));

		size_type count = 0;
		bool create_dir_chk =
			this_type::pro_s_has_opts(
				opts,
				static_cast<u32>(options_type::dir_create_if_not_exists))
			&& this_type::pro_s_has_opts(
					ft_opts,
					static_cast<u32>(file_types_options_type::directory_file));

		this_type::prv_s_r_recursive_move_no_fmt(
			count, old_path, new_path, old_path, new_path,
			unique_handler, notify_handler,
			ec, opts, ft_opts, create_dir_chk);
		return count;
	}

	template<typename Unique_Handler, typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			size_type
		>::type
		pro_s_recursive_move(const path_type& old_path, const path_type& new_path,
								const Unique_Handler& unique_handler,
								const Notify_Handler& notify_handler,
								error_code_type* ec,
								u32 opts, u32 ft_opts)
	{
		path_type sc_old_path;
		path_type sc_new_path;

		size_type count = 0;
		bool create_dir_chk =
			this_type::pro_s_has_opts(
				opts,
				static_cast<u32>(options_type::dir_create_if_not_exists))
			&& this_type::pro_s_has_opts(
					ft_opts,
					static_cast<u32>(file_types_options_type::directory_file));

		if(!old_path.empty() && !new_path.empty()
			&& (sc_old_path = native::call_system_complete(old_path), this_type::pro_s_ec_chk(ec))
			&& (sc_new_path = native::call_system_complete(new_path), this_type::pro_s_ec_chk(ec)))
		{
			Unique_Handler now_unique_handler(sc_new_path);
			this_type::prv_s_r_recursive_move_no_fmt(
				count, sc_old_path, sc_new_path,
				sc_old_path, sc_new_path,
				now_unique_handler, notify_handler,
				ec, opts, ft_opts, create_dir_chk);
		}

		return count;
	}

public:
	// throw exception_type, compatibility::stl_exception
	inline static size_type
		recursive_move(const path_type& old_path, const path_type& new_path,
						u32 opts = 0,
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::rename_handler_type unique_handler_type;

		typedef typename
			default_handlers_type::recursive_move_notify_handler_type notify_handler_type;

		unique_handler_type unique_handler(new_path);
		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_move(
				old_path, new_path,
				unique_handler, notify_handler,
				0, opts, ft_opts);
	}

	// non-throw
	inline static size_type
		recursive_move(const path_type& old_path, const path_type& new_path,
						error_code_type& ec,
						u32 opts = 0,
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::rename_handler_type unique_handler_type;

		typedef typename
			default_handlers_type::recursive_move_notify_handler_type notify_handler_type;

		unique_handler_type unique_handler(new_path);
		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_move(
				old_path, new_path,
				unique_handler, notify_handler,
				boost::addressof(ec), opts, ft_opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Notify_Handler>,
				boost::mpl::bool_<!!(func::foo_t_info<Notify_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_move(const path_type& old_path, const path_type& new_path,
						const Notify_Handler& notify_handler,
						u32 opts = 0,
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::rename_handler_type unique_handler_type;

		unique_handler_type unique_handler(new_path);
		return this_type::pro_s_recursive_move(
				old_path, new_path,
				unique_handler, notify_handler,
				0, opts, ft_opts);
	}

	// non-throw
	template<typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Notify_Handler>,
				boost::mpl::bool_<!!(func::foo_t_info<Notify_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_move(const path_type& old_path, const path_type& new_path,
						const Notify_Handler& notify_handler,
						error_code_type& ec,
						u32 opts = 0,
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::rename_handler_type unique_handler_type;

		unique_handler_type unique_handler(new_path);
		return this_type::pro_s_recursive_move(
				old_path, new_path,
				unique_handler, notify_handler,
				boost::addressof(ec), opts, ft_opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				boost::mpl::bool_<!(func::foo_t_info<Unique_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_move(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						u32 opts =
							static_cast<u32>(options_type::dir_rename_if_exists)
							| static_cast<u32>(options_type::file_rename_if_exists)
							| static_cast<u32>(options_type::symlink_rename_if_exists),
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::recursive_move_notify_handler_type notify_handler_type;

		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_move(
				old_path, new_path,
				unique_handler, notify_handler,
				0, opts, ft_opts);
	}

	// non-throw
	template<typename Unique_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				boost::mpl::bool_<!(func::foo_t_info<Unique_Handler>::arg_holder_list_size)>
			>,
			size_type
		>::type
		recursive_move(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						error_code_type& ec,
						u32 opts =
							static_cast<u32>(options_type::dir_rename_if_exists)
							| static_cast<u32>(options_type::file_rename_if_exists)
							| static_cast<u32>(options_type::symlink_rename_if_exists),
						u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::recursive_move_notify_handler_type notify_handler_type;

		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_move(
				old_path, new_path,
				unique_handler, notify_handler,
				boost::addressof(ec), opts, ft_opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Unique_Handler, typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			size_type
		>::type
		recursive_move(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						const Notify_Handler& notify_handler,
						u32 opts =
							static_cast<u32>(options_type::dir_rename_if_exists)
							| static_cast<u32>(options_type::file_rename_if_exists)
							| static_cast<u32>(options_type::symlink_rename_if_exists),
						u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_recursive_move(
				old_path, new_path,
				unique_handler, notify_handler,
				0, opts, ft_opts);
	}

	// non-throw
	template<typename Unique_Handler, typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Unique_Handler>,
				is_callable<Notify_Handler>
			>,
			size_type
		>::type
		recursive_move(const path_type& old_path, const path_type& new_path,
						const Unique_Handler& unique_handler,
						const Notify_Handler& notify_handler,
						error_code_type& ec,
						u32 opts =
							static_cast<u32>(options_type::dir_rename_if_exists)
							| static_cast<u32>(options_type::file_rename_if_exists)
							| static_cast<u32>(options_type::symlink_rename_if_exists),
						u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_recursive_move(
				old_path, new_path,
				unique_handler, notify_handler,
				boost::addressof(ec), opts, ft_opts);
	}

protected:
	inline static bool pro_s_remove(const path_type& path, error_code_type* ec, u32 opts, u32 ft_opts)
	{
		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();

		switch(ft_path)
		{
		case boost::filesystem::status_error:
			return false;
		case boost::filesystem::file_not_found:
			return this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::remove_success_if_not_exists));
		case boost::filesystem::directory_file:
			return has_bit_options<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(ft_path))
					&& native::call_is_empty(path, ec) && this_type::pro_s_ec_chk(ec)
					&& native::call_remove(path, ec) && this_type::pro_s_ec_chk(ec);
		default:
			return has_bit_options<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(ft_path))
					&& native::call_remove(path, ec) && this_type::pro_s_ec_chk(ec);
		}
	}

public:
	//remove
	// validate options: remove_success_if_not_exists // if item is not exists return true;

	// throw exception_type, compatibility::stl_exception
	inline static bool remove(const path_type& path, u32 opts = 0,
								u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_remove(path, 0, opts, ft_opts);
	}

	//non-throw
	inline static bool remove(const path_type& path, error_code_type& ec,
								u32 opts = 0, u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_remove(path, boost::addressof(ec), opts, ft_opts);
	}

private:
	template<typename Notify_Handler>
	static
	typename
		boost::enable_if
		<
			is_callable<Notify_Handler>,
			bool // is_failed
		>::type
		prv_s_r_recursive_remove_no_fmt(size_type& count,
										const path_type& root_path,
										const path_type& path,
										const Notify_Handler& notify_handler,
										error_code_type* ec,
										u32 opts,
										u32 ft_opts)
	{
		assert(!path.empty());
		assert(path == native::system_complete(path));

		file_status_type sat_path;
		u32 ft_path = 0;

		bool bneed_remove_dir = false;
		bool b_igonre_if_failed =
				this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::recursive_operator_ignore_if_failed));

		try
		{
			sat_path = this_type::pro_s_path_symlink_status(path, ec);
		}
		catch(const exception_type&)
		{
			sat_path = file_status_type();
		}

		ft_path = sat_path.type();
		bool brmable = has_bit_options<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(ft_path));

		switch(ft_path)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			if(this_type::pro_s_has_opts(opts, static_cast<u32>(options_type::remove_success_if_not_exists)))
			{
				++count;
				notify_handler(root_path, path, true, count, ec, true);
				return true;
			}
			else
			{
				notify_handler(root_path, path, false, count, ec, b_igonre_if_failed);
				return false;
			}
		// remove directory
		case boost::filesystem::directory_file:
			bneed_remove_dir = brmable;
			break;
		// remove symlink and file
		default:
			if(brmable)
			{
				try
				{
					bool bremoved = native::call_remove(path, ec);
					if(!this_type::pro_s_ec_chk(ec))
					{
						notify_handler(root_path, path, false, count, ec, b_igonre_if_failed);
						return false;
					}

					if(bremoved)
					{
						++count;
						notify_handler(root_path, path, true, count, ec, true);
						return true;
					}
					else
					{
						notify_handler(root_path, path, false, count, ec, b_igonre_if_failed);
						return false;
					}
				}
				catch(const exception_type&)
				{
					notify_handler(root_path, path, false, count, ec, b_igonre_if_failed);
					return false;
				}
			}
			return true;
		}

		assert(ft_path == boost::filesystem::directory_file);
		directory_iterator_type dir_iter, dir_iter_end;

		try
		{
			dir_iter = this_type::pro_s_make_directory_iterator_object(path, ec);
		}
		catch(const exception_type&)
		{
			notify_handler(root_path, path, false, count, ec, b_igonre_if_failed);
			return false;
		}

		bool bret = true;
		if(b_igonre_if_failed)
		{
			for(;dir_iter != dir_iter_end; )
			{
				this_type::prv_s_r_recursive_remove_no_fmt(
					count, root_path, dir_iter->path(),
					notify_handler, ec, opts, ft_opts);

				if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
				{
					bret = false;
					break;
				}
			}
		}
		else
		{
			bool bcontinue = false;
			for(;dir_iter != dir_iter_end; /*++dir_iter*/)
			{
				bcontinue =
					this_type::prv_s_r_recursive_remove_no_fmt(
						count, root_path, dir_iter->path(),
						notify_handler, ec, opts, ft_opts);

				if(!(bcontinue && this_type::pro_s_ec_chk(ec)))
				{
					bret = false;
					break;
				}

				if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
				{
					bret = false;
					break;
				}
			}
		}

		//fix_count;
		if(bneed_remove_dir)
		{
			try
			{
				if(native::call_is_empty(path, ec) && this_type::pro_s_ec_chk(ec)
					&& native::call_remove(path, ec) && this_type::pro_s_ec_chk(ec))
				{
					++count;
					notify_handler(root_path, path, true, count, ec, true);
					return true;
				}
				else
				{
					notify_handler(root_path, path, false, count, ec, b_igonre_if_failed);
					return false;
				}
			}
			catch(const exception_type&)
			{
				notify_handler(root_path, path, false, count, ec, b_igonre_if_failed);
				return false;
			}
		}

		return bret;
	}

protected:

	template<typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			is_callable<Notify_Handler>,
			size_type
		>::type
		pro_s_recursive_remove_no_fmt(const path_type& path,
										error_code_type* ec,
										const Notify_Handler& notify_handler,
										u32 opts, u32 ft_opts)
	{
		assert(!path.empty());
		assert(path == native::system_complete(path));

		size_type count = 0;

		this_type::prv_s_r_recursive_remove_no_fmt(
			count, path, path, ec, notify_handler, opts, ft_opts);
		return count;
	}

	template<typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			is_callable<Notify_Handler>,
			size_type
		>::type
		pro_s_recursive_remove(const path_type& path,
								const Notify_Handler& notify_handler,
								error_code_type* ec,
								u32 opts, u32 ft_opts)
	{
		path_type sc_path;
		size_type count = 0;

		if(!path.empty()
			&& (sc_path = native::call_system_complete(path), this_type::pro_s_ec_chk(ec)))
		{
			this_type::prv_s_r_recursive_remove_no_fmt(
					count, sc_path, sc_path, notify_handler, ec, opts, ft_opts);
		}

		return count;
	}

public:
	//recursive_remove

	// validate options: remove_success_if_not_exists // if dir is not exists return true;

	// throw exception_type, compatibility::stl_exception
	template<typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			is_callable<Notify_Handler>,
			size_type
		>::type
		recursive_remove(const path_type& path,
							const Notify_Handler& notify_handler,
							u32 opts = options_type::recursive_operator_ignore_if_failed,
							u32 ft_opts = file_types_options_type::types_all)
	{

		return this_type::pro_s_recursive_remove(
				path, notify_handler, 0, opts, ft_opts);
	}

	// non-throw
	template<typename Notify_Handler> inline
	static
	typename
		boost::enable_if
		<
			is_callable<Notify_Handler>,
			size_type
		>::type
		recursive_remove(const path_type& path,
							const Notify_Handler& notify_handler,
							error_code_type& ec,
							u32 opts = options_type::recursive_operator_ignore_if_failed,
							u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_recursive_remove(
				path, notify_handler, boost::addressof(ec), opts, ft_opts);
	}

	// throw exception_type, compatibility::stl_exception
	inline static size_type
		recursive_remove(const path_type& path,
							u32 opts = 0,
							u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::recursive_remove_notify_handler_type notify_handler_type;

		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_remove(
				path, notify_handler, 0, opts, ft_opts);
	}

	// non-throw
	inline static size_type
		recursive_remove(const path_type& path,
							error_code_type& ec,
							u32 opts = 0,
							u32 ft_opts = file_types_options_type::types_all)
	{
		typedef typename
			default_handlers_type::recursive_remove_notify_handler_type notify_handler_type;

		notify_handler_type notify_handler;
		return this_type::pro_s_recursive_remove(
				path, notify_handler, boost::addressof(ec), opts, ft_opts);
	}

public:

	// throw exception_type, compatibility::stl_exception
	inline static size_type remove_all(const path_type& path)
	{
		return native::remove_all(path);
	}

	// non-throw
	inline static size_type remove_all(const path_type& path, error_code_type& ec)
	{
		return native::remove_all(path, ec);
	}

protected:
	inline static file_status_type pro_directory_entry_symlink_status(const directory_entry_type& de,
																		error_code_type* ec = 0)
	{
		file_status_type sat_iter;

		try
		{
			sat_iter = native::directory_entry_symlink_status(de, ec);
			if(!this_type::pro_s_ec_chk(ec))
			{
				sat_iter = file_status_type();
			}
		}
		catch(const exception_type&)
		{
			sat_iter = file_status_type();
		}
		catch(...)
		{
			sat_iter = file_status_type();
		}

		return sat_iter;
	}

protected:
	template<typename Handler>
	static typename boost::enable_if<is_callable<Handler>, size_type>::type
		pro_s_for_each(const path_type& path, const Handler& handler,
						error_code_type* ec, u32 ft_opts)
	{
		size_type count = 0;
		file_status_type sat_path;
		try
		{
			sat_path = this_type::pro_s_path_symlink_status(path, ec);
		}
		catch(const exception_type&)
		{
			sat_path = file_status_type();
		}

		u32 ft_path = sat_path.type();
		directory_entry_type entry;

		switch(ft_path)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return count;
		default:
			if(has_bit_options_in<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(ft_path)))
			{
				entry = directory_entry_type(path, this_type::pro_s_path_symlink_status(path, ec), sat_path);
				if(!this_type::pro_s_ec_chk(ec))
				{
					return count;
				}
				else
				{
					++count;
				}
				handler(entry, count, ec);
			}

			if(boost::filesystem::directory_file == ft_path)
			{
				break;
			}
			else
			{
				return count;
			}
		}

		directory_iterator_type dir_iter;

		try
		{
			dir_iter = this_type::pro_s_make_directory_iterator_object(path, ec);
			if(!this_type::pro_s_ec_chk(ec))
			{
				return count;
			}
		}
		catch(const exception_type&)
		{
			return count;
		}

		file_status_type sat_iter;
		for(directory_iterator_type isize; dir_iter != isize;)
		{
			sat_iter = this_type::pro_directory_entry_symlink_status(*dir_iter, ec);


			if(has_bit_options_in<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(sat_iter.type())))
			{
				++count;
				handler(*dir_iter, count, ec);
			}

			if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
			{
				break;
			}
		}

		return count;
	}

public:

	// throw exception_type, compatibility::stl_exception
	template<typename Handler> inline
	static typename boost::enable_if<is_callable<Handler>, size_type>::type
		for_each(const path_type& path, const Handler& handler,
					u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_for_each(path, handler, 0, ft_opts);
	}

	// non-throw
	template<typename Handler> inline
	static typename boost::enable_if<is_callable<Handler>, size_type>::type
		for_each(const path_type& path, const Handler& handler,
					error_code_type& ec,
					u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_for_each(path, handler, boost::addressof(ec), ft_opts);
	}

protected:
	template<typename Handler>
	static typename boost::enable_if<is_callable<Handler>, size_type>::type
		pro_s_recursive_for_each(const path_type& path, const Handler& handler,
									error_code_type* ec, u32 ft_opts)
	{
		size_type count = 0;
		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();

		directory_entry_type entry;
		switch(ft_path)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return count;
		default:
			if(has_bit_options_in<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(ft_path)))
			{
				entry = directory_entry_type(path, this_type::pro_s_path_symlink_status(path, ec), sat_path);
				if(!this_type::pro_s_ec_chk(ec))
				{
					return count;
				}
				else
				{
					++count;
					handler(entry, count, ec);
				}
			}

			if(boost::filesystem::directory_file == ft_path)
			{
				break;
			}
			else
			{
				return count;
			}
		}

#	if BOOST_VERSION < 107200
		symlink_option_enum_type symopt = symlink_option_type::no_recurse;
#	else
		directory_options_enum_type symopt = directory_options_type::none;
#	endif //BOOST_VERSION < 107200

		recursive_directory_iterator_type dir_iter;

		try
		{
			dir_iter = this_type::pro_s_make_recursive_directory_iterator_object(path, symopt, ec);
			if(!this_type::pro_s_ec_chk(ec))
			{
				return count;
			}
		}
		catch(const exception_type&)
		{
			return count;
		}

		file_status_type sat_iter;
		for(recursive_directory_iterator_type isize; dir_iter != isize;)
		{
			sat_iter = this_type::pro_directory_entry_symlink_status(*dir_iter, ec);

			if(has_bit_options_in<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(sat_iter.type())))
			{
				++count;
				handler(*dir_iter, count, ec);
			}

			if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
			{
				break;
			}
		}

		return count;
	}

	template<typename Handler, typename NoPushFilter>
	static typename boost::enable_if<is_callable<Handler>, size_type>::type
		pro_s_recursive_for_each_no_push_filter(const path_type& path,
												const Handler& handler,
												const NoPushFilter& no_push_filter,
												error_code_type* ec,
												u32 ft_opts)
	{
		size_type count = 0;
		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();

		directory_entry_type entry;
		switch(ft_path)
		{
		case boost::filesystem::status_error:
		case boost::filesystem::file_not_found:
			return count;
		default:
			if(has_bit_options_in<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(ft_path)))
			{
				entry = directory_entry_type(path, this_type::pro_s_path_symlink_status(path, ec), sat_path);
				if(!this_type::pro_s_ec_chk(ec))
				{
					return count;
				}
				else
				{
					++count;
					handler(entry, count, ec);
					if(no_push_filter(recursive_directory_iterator_type(), entry, count, ec))
					{
						return count;
					}
				}
			}

			if(boost::filesystem::directory_file == ft_path)
			{
				break;
			}
			else
			{
				return count;
			}
		}

#	if BOOST_VERSION < 107200
		symlink_option_enum_type symopt = symlink_option_type::no_recurse;
#	else
		directory_options_enum_type symopt = directory_options_type::none;
#	endif //BOOST_VERSION < 107200

		recursive_directory_iterator_type dir_iter;

		try
		{
			dir_iter = this_type::pro_s_make_recursive_directory_iterator_object(path, symopt, ec);
			if(!this_type::pro_s_ec_chk(ec))
			{
				return count;
			}
		}
		catch(const exception_type&)
		{
			return count;
		}

		file_status_type sat_iter;
		for(recursive_directory_iterator_type isize; dir_iter != isize;)
		{
			sat_iter = this_type::pro_directory_entry_symlink_status(*dir_iter, ec);

			if(has_bit_options_in<u32>(ft_opts, file_types_options_type::conv_to_file_type_opts(sat_iter.type())))
			{
				++count;
				handler(*dir_iter, count, ec);
			}

			if(boost::filesystem::directory_file == ft_path)
			{
				if(no_push_filter(dir_iter, *dir_iter, count, ec))
				{

#	if (BOOST_VERSION < 106000)
					dir_iter.no_push(true);
#	else
					dir_iter.disable_recursion_pending(true);
#	endif // (BOOST_VERSION < 106000)
				}
			}

			if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
			{
				break;
			}
		}

		return count;
	}

public:

	// throw exception_type, compatibility::stl_exception
	template<typename Handler> inline
	static typename boost::enable_if<is_callable<Handler>, size_type>::type
		recursive_for_each(const path_type& path, const Handler& handler,
							u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_recursive_for_each(path, handler, 0, ft_opts);
	}

	// non-throw
	template<typename Handler> inline
	static typename boost::enable_if<is_callable<Handler>, size_type>::type
		recursive_for_each(const path_type& path, const Handler& handler,
							error_code_type& ec,
							u32 ft_opts = file_types_options_type::types_all)
	{
		return
			this_type::pro_s_recursive_for_each(
				path, handler, boost::addressof(ec), ft_opts);
	}

	// throw exception_type, compatibility::stl_exception
	template<typename Handler, typename NoPushFilter> inline
	static typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				is_callable<NoPushFilter>
			>,
			size_type
		>::type
		recursive_for_each(const path_type& path,
							const Handler& handler,
							const NoPushFilter& no_push_filter,
							u32 ft_opts = file_types_options_type::types_all)
	{
		return
			this_type::pro_s_recursive_for_each_no_push_filter(
				path, handler, no_push_filter, static_cast<error_code_type*>(0), ft_opts);
	}

	// non-throw
	template<typename Handler, typename NoPushFilter> inline
	static typename
		boost::enable_if
		<
			boost::mpl::and_
			<
				is_callable<Handler>,
				is_callable<NoPushFilter>
			>,
			size_type
		>::type
		recursive_for_each(const path_type& path,
							const Handler& handler,
							const NoPushFilter& no_push_filter,
							error_code_type& ec,
							u32 ft_opts = file_types_options_type::types_all)
	{
		return
			this_type::pro_s_recursive_for_each_no_push_filter(
				path, handler, no_push_filter, boost::addressof(ec), ft_opts);
	}


protected:
	inline static u32 pro_s_count_children_file_opts_fmt(u32 ft_opts)
	{
		static const u32 chk = static_cast<u32>(file_types_options_type::types_all);
		return append_bit_options_if_not_in<u32>(ft_opts, chk);
	}

	template<std::size_t N>
	static void pro_s_count_children_array_init(u32 ft_opts, size_type(&arr)[N])
	{
#ifdef _DEBUG
		{
			size_type arr_chk[N] = {0};
			assert(0 == memcmp(&arr_chk[0], &arr[0], sizeof(size_type) * N));
		}
#endif // _DEBUG
		u32 chk_opts = check_bit_options<u32>(ft_opts, file_types_options_type::failed);
		arr[file_types_options_type::conv_to_file_type(chk_opts)] =
			static_cast<size_type>(!!chk_opts);

		for(u32 i = file_types_options_type::E_opts_bit_begin,
				isize = file_types_options_type::E_opts_bit_end;
				i != isize; ++i)
		{
			chk_opts = check_bit_options<u32>(ft_opts, static_cast<u32>(1) << i);
			arr[file_types_options_type::conv_to_file_type(chk_opts)] =
				static_cast<size_type>(!!chk_opts);
		}
	}

	/*
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	!!!	in windows boost::filesystem recursive_directory_iterator not support symlink,			!!!
	!!!	in otherOS(e.q linux ) boost::filesystem recursive_directory_iterator support symlink	!!!
	!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/

	static size_type pro_s_count_children(const path_type& path, error_code_type* ec, u32 ft_opts)
	{

		ft_opts = this_type::pro_s_count_children_file_opts_fmt(ft_opts);
		size_type chk_arr[file_types_options_type::E_opts_bit_size] = {0};
		size_type count = 0;

		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();

		this_type::pro_s_count_children_array_init(ft_opts, chk_arr);

		switch(ft_path)
		{
		case boost::filesystem::directory_file:
			//count += chk_arr[ft_path];
			break;
		default:
			//count += chk_arr[ft_path];
			return count;
		}

		directory_iterator_type dir_iter;
		try
		{
			dir_iter = this_type::pro_s_make_directory_iterator_object(path, ec);
			if(!this_type::pro_s_ec_chk(ec))
			{
				return count;
			}
		}
		catch(const exception_type&)
		{
			return count;
		}

		file_status_type sat_iter;
		for(directory_iterator_type isize; dir_iter != isize;)
		{
			sat_iter = this_type::pro_directory_entry_symlink_status(*dir_iter, ec);

			count += chk_arr[sat_iter.type()];

			if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
			{
				break;
			}
		}

		return count;
	}

public:
	// count_children
	// throw exception_type, compatibility::stl_exception
	// validate options: recursive_operator_ignore_if_failed, symlink_success_if_exists
	inline static size_type count_children(const path_type& path,
											u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_count_children(path, 0, ft_opts);
	}

	// non-throw
	inline static size_type count_children(const path_type& path, error_code_type& ec,
											u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_count_children(path, boost::addressof(ec), ft_opts);
	}

protected:
	static size_type pro_s_recursive_count_children(const path_type& path, error_code_type* ec, u32 ft_opts)
	{
		ft_opts = this_type::pro_s_count_children_file_opts_fmt(ft_opts);
		size_type chk_arr[file_types_options_type::E_opts_bit_size] = {0};
		size_type count = 0;

		this_type::pro_s_count_children_array_init(ft_opts, chk_arr);

		file_status_type sat_path = this_type::pro_s_path_symlink_status(path, ec);
		u32 ft_path = sat_path.type();

		switch(ft_path)
		{
		case boost::filesystem::directory_file:
			break;
		default:
			return count;
		}

#	if BOOST_VERSION < 107200
		symlink_option_enum_type symopt = symlink_option_type::no_recurse;
#	else
		directory_options_enum_type symopt = directory_options_type::none;
#	endif //BOOST_VERSION < 107200

		recursive_directory_iterator_type dir_iter;

		try
		{
			dir_iter = this_type::pro_s_make_recursive_directory_iterator_object(path, symopt, ec);
			if(!this_type::pro_s_ec_chk(ec))
			{
				return count;
			}
		}
		catch(const exception_type&)
		{
			return count;
		}

		file_status_type sat_iter;
		for(recursive_directory_iterator_type isize; dir_iter != isize;)
		{
			sat_iter = this_type::pro_directory_entry_symlink_status(*dir_iter, ec);

			count += chk_arr[sat_iter.type()];

			if(!(native::directory_iterator_increment(dir_iter, ec), this_type::pro_s_ec_chk(ec)))
			{
				break;
			}
		}

		return count;
	}

public:
	// count_children
	// throw exception_type, compatibility::stl_exception
	// validate options: symlink_success_if_exists, symlink_recurse
	inline static size_type recursive_count_children(const path_type& path,
														u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_recursive_count_children(path, 0, ft_opts);
	}

	// non-throw
	inline static size_type recursive_count_children(const path_type& path, error_code_type& ec,
														u32 ft_opts = file_types_options_type::types_all)
	{
		return this_type::pro_s_recursive_count_children(path, boost::addressof(ec), ft_opts);
	}
};

} // namespace file_system
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif // __YGGR_FILE_SYSTEM_LOCAL_FSYS_BASIC_OPERATORS_HPP__
