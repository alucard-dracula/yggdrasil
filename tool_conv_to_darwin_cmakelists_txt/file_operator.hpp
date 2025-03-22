// file_operator.hpp

#ifndef __FILE_OPERATOR_HPP__
#define __FILE_OPERATOR_HPP__

#include "tool_conv_to_darwin_cmakelists_txt_config.hpp"

#include <yggr/iterator_ex/iterator_traits.hpp>

#if !(BOOST_VERSION < 105600)

namespace file_op
{

yggr::string& format_string_file_path(yggr::string& fpath);
yggr::utf8_string& format_string_file_path(yggr::utf8_string& fpath);
yggr::utf8_string make_string_file_path(const yggr::utf8_string& fpath);

yggr::string get_file_dir(const yggr::string& file_path);
yggr::string get_file_name(const yggr::string& file_path);
yggr::string get_file_name_no_ext(const yggr::string& file_path);

yggr::utf8_string get_file_dir(const yggr::utf8_string& file_path);
yggr::utf8_string get_file_name(const yggr::utf8_string& file_path);
yggr::utf8_string get_file_name_no_ext(const yggr::utf8_string& file_path);

bool is_yggr_proj_check(const yggr::string& proj_dir);
bool is_yggr_sln_check(const yggr::string& sln_dir);

template<typename Buffer> inline
Buffer& read_from_file(Buffer& file_content, const yggr::utf8_string& fpath)
{
	try
	{
		local_fsys_operators_type::read_file_of_binary(local_fsys_operators_type::make_path_object(fpath), file_content);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}

	return file_content;
}

template<typename Buffer> inline
bool write_to_file(const yggr::utf8_string& fname, const Buffer& fbuf)
{
	std::size_t write_byte_size = 0;
	try
	{
		write_byte_size = 
			local_fsys_operators_type::write_file_of_binary(
				local_fsys_operators_type::make_path_object(fname), fbuf,
				local_fsys_operators_type::options_type::dir_create_if_not_exists);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
		return false;
	}

	return !(write_byte_size == 0 || write_byte_size == local_fsys_operators_type::npos);
}

bool is_parent_path(const fsys_path_type& lpath, const fsys_path_type& rpath);

bool check_file_ext(const yggr::string& str_fpath, const yggr::string& str_ext);
bool check_file_ext(const fsys_path_type& fpath, const yggr::string& str_ext);

bool copy_file(const yggr::utf8_string& src, const yggr::utf8_string& dst);

}// namespace file_op

#endif // BOOST_VERSION < 105600

#endif // __FILE_OPERATOR_HPP__
