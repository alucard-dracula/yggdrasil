// file_operator.hpp

#ifndef __FILE_OPERATOR_HPP__
#define __FILE_OPERATOR_HPP__

#include "tool_conv_to_cb_solution_config.hpp"

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

bool is_yggr_proj_check(const yggr::utf8_string& proj_dir);

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

//template<typename Iter> inline
//Iter get_line(Iter s, Iter e)
//{
//	typedef Iter now_iter_type;
//	typedef typename yggr::iterator_value_ex<now_iter_type>::type val_type;
//
//	static const val_type s_ch_rn[] = {static_cast<val_type>('\r'), static_cast<val_type>('\n')};
//
//	return out_buf.find_first_of(s, e, s_ch_rn, s_ch_rn + 2);
//}

inline bool is_parent_path(const fsys_path_type& lpath, const fsys_path_type& rpath)
{
	return 
		lpath.has_filename() 
		&& rpath.has_filename()
		&& rpath.has_parent_path()
		&& lpath == rpath.parent_path();

}

}// namespace file_op

#endif // BOOST_VERSION < 105600

#endif // __FILE_OPERATOR_HPP__
