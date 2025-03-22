// test_project_maker.cpp

#include <iostream>
#include <sstream>

#include <yggr/base/yggrdef.h>

#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_string.hpp>
#include <yggr/regular_parse/regex_parse.hpp>

#include <yggr/container/set.hpp>
#include <yggr/container/unordered_set.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(regular_parse)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)


typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;
typedef local_fsys_operators_type::directory_entry_type directory_entry_type;
typedef local_fsys_operators_type::path_type path_type;

typedef yggr::set<path_type> file_list_type;

template<typename Buffer>
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

template<typename Buffer>
Buffer& read_from_file(Buffer& file_content, const path_type& fpath)
{
	try
	{
		local_fsys_operators_type::read_file_of_binary(fpath, file_content);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}

	return file_content;
}

//template<typename Buffer>
//bool write_to_file(const yggr::utf8_string& fname, const Buffer& fbuf)
//{
//	try
//	{
//		local_fsys_operators_type::write_file_of_binary(
//			local_fsys_operators_type::make_path_object(fname), fbuf,
//			local_fsys_operators_type::options_type::dir_create_if_not_exists);
//	}
//	catch(const local_fsys_operators_type::exception_type& e)
//	{
//		std::cerr << e.what() << std::endl;
//		assert(false);
//		return false;
//	}
//
//	return true;
//}


void handler_for_each_search_cpp_files(const local_fsys_operators_type::directory_entry_type& entry,
										boost::uintmax_t current_count,
										local_fsys_operators_type::error_code_type* current_ec,
										const yggr::string& str_re,
										file_list_type& flist)
{


	typedef local_fsys_operators_type::path_type path_type;

	const path_type& fpath = entry.path();

	if(!(fpath.has_extension() && fpath.extension() == ".cpp"))
	{
		return;
	}

	yggr::string buf;
	read_from_file(buf, fpath);

	if(!buf.size())
	{
		return;
	}

	bool bmatch = false;
	try
	{
		bmatch = 
			yggr::regular_parse::regex_parse::match(
				buf, 
				yggr::regular_parse::regex_parse::make_regex(
					str_re,
					yggr::regular_parse::regex_parse::regex_base_type::icase
						| yggr::regular_parse::regex_parse::regex_base_type::normal));
	}
	catch(const yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "match " << yggr::utf8_string(fpath.native()) << " failed" << std::endl;
		return;
	}
	catch(const yggr::stl_exception& e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "match " << yggr::utf8_string(fpath.native()) << " failed" << std::endl;
		return;
	}
	catch(...)
	{
		std::cerr << "match " << yggr::utf8_string(fpath.native()) << " failed" << std::endl;
		return;
	}

	if(!bmatch)
	{
		flist.insert(fpath);
	}

	return;
}

file_list_type& search_cpp_files(file_list_type& file_list,
									const yggr::utf8_string& root_dir,
									const yggr::string& str_re)
{
	path_type root_dir_path = local_fsys_operators_type::make_path_object(root_dir);

	try
	{
		local_fsys_operators_type::recursive_for_each(
			root_dir_path, 
			boost::bind(
				&handler_for_each_search_cpp_files, _1, _2, _3,
				boost::cref(str_re), boost::ref(file_list)),
			local_fsys_operators_type::file_types_options_type::general_file);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}

	return file_list;
}

int main_detail(int argc, char* argv[])
{

	yggr::utf8_string root_dir = "../tttt";
	yggr::string str_re = ".*<vld\\.h>.*";

	file_list_type file_list;
	std::cout << str_re << std::endl;
	search_cpp_files(file_list, root_dir, str_re);

	for(file_list_type::const_iterator i = file_list.begin(), isize = file_list.end(); i != isize; ++i)
	{
		std::cout << "fix: " << *i << std::endl;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int ret = main_detail(argc, argv);

	std::cout << "end" << std::endl;
	char cc = 0;
	std::cin >> cc;
	return ret;
}