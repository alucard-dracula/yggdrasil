// tool_conv_to_cb_solution.cpp

#include <yggr/base/yggrdef.h>

#include <yggr/charset/utf8_string.hpp>

#include <yggr/container/unordered_map.hpp>

#include <yggr/tuple/tuple.hpp>

#include <yggr/encryption_tool/md5_tool.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>
#include <yggr/file_system/boost_filesystem_comp.hpp>

#include <iostream>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

//#include YGGR_PP_LINK_LIB(exception)
//#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(encryption_tool)
#include YGGR_PP_LINK_LIB(charset)
//#include YGGR_PP_LINK_LIB(time)
//#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)

typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;
typedef local_fsys_operators_type::directory_entry_type fsys_directory_entry_type;
typedef local_fsys_operators_type::path_type fsys_path_type;

typedef yggr::encryption_tool::md5_tool md5_tool_type;
typedef md5_tool_type::sig_type md5_sig_type;

typedef std::pair<fsys_path_type, yggr::u64> hit_info;
typedef yggr::unordered_map<md5_sig_type, hit_info> hit_info_umap_type;

template<typename Buffer> inline
Buffer& read_from_file(Buffer& file_content, const fsys_path_type& fpath)
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

bool remove_file(const fsys_path_type& fpath)
{
	try
	{
		local_fsys_operators_type::remove(fpath);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}

	return true;
}

bool handler_for_each_search_files_filter(local_fsys_operators_type::recursive_directory_iterator_type dir_iter,
											const local_fsys_operators_type::directory_entry_type& entry,
											boost::uintmax_t current_count,
											local_fsys_operators_type::error_code_type* current_ec)
{
	//std::cout << "dir_path" << entry.path() << std::endl;

	return
		dir_iter != local_fsys_operators_type::recursive_directory_iterator_type()
		&& ( (local_fsys_operators_type::path_symlink_status(entry.path()).type() == boost::filesystem::symlink_file) );
}

void handler_for_each_search_files(const local_fsys_operators_type::directory_entry_type& entry,
									boost::uintmax_t current_count,
									local_fsys_operators_type::error_code_type* current_ec,
									hit_info_umap_type& hit_infos)
{
	typedef hit_info_umap_type::iterator hit_info_umap_iter_type;

	static md5_tool_type md5_tool;

	const fsys_path_type& fpath = entry.path();
	//std::cout << "check_path: " << fpath << std::endl;

	yggr::string fbuf;
	read_from_file(fbuf, fpath);

	md5_sig_type md5_sig = md5_tool.encrypt(fbuf);

	hit_info_umap_iter_type hit_iter = hit_infos.find(md5_sig);
	if(hit_iter == hit_infos.end())
	{
		hit_infos[md5_sig] = std::make_pair(fpath, 1);
	}
	else
	{
		++hit_iter->second.second;
		remove_file(fpath);
	}

	return;
}

void get_files(const fsys_path_type& root_dir_path, hit_info_umap_type& hit_infos)
{
	std::cout << "get_files root_path = "  << root_dir_path << std::endl;

	try
	{
		local_fsys_operators_type::recursive_for_each(
			root_dir_path,
			boost::bind(
				&handler_for_each_search_files, _1, _2, _3,
				boost::ref(hit_infos)),
			boost::bind(
				&handler_for_each_search_files_filter, _1, _2, _3, _4),
			local_fsys_operators_type::file_types_options_type::general_file);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}
}

//void handler_for_each_search_files_check_count(const local_fsys_operators_type::directory_entry_type& entry,
//												boost::uintmax_t current_count,
//												local_fsys_operators_type::error_code_type* current_ec,
//												yggr::u64& count)
//{
//	typedef hit_info_umap_type::iterator hit_info_umap_iter_type;
//	++count;
//	std::cout << "count: " << entry.path() << std::endl;
//	return;
//}

void handler_for_each_check_dir_file_count(const local_fsys_operators_type::directory_entry_type& entry,
											boost::uintmax_t current_count,
											local_fsys_operators_type::error_code_type* current_ec,
											const fsys_path_type& root_dir_path,
											yggr::u64& count)
{	
	if(root_dir_path == entry.path())
	{
		return;
	}

	//std::cout << "check_dir_file_count_path: " << entry.path() << std::endl;

	if(local_fsys_operators_type::path_symlink_status(entry.path()).type() == boost::filesystem::directory_file)
	{
		yggr::u64 sub_count = 0;
		try
		{
			local_fsys_operators_type::for_each(
				entry.path(),
				boost::bind(
					&handler_for_each_check_dir_file_count, _1, _2, _3,
					boost::cref(entry.path()), boost::ref(sub_count)),
				local_fsys_operators_type::file_types_options_type::types_all);
		}
		catch(const local_fsys_operators_type::exception_type& e)
		{
			std::cerr << e.what() << std::endl;
			assert(false);
			return;
		}

		//std::cout << "check_dir_file_count_n: " << sub_count << std::endl;

		if(sub_count)
		{
			++count;
		}
		else
		{
			//std::cout << "remove empty dir" << entry.path() << std::endl;
			remove_file(entry.path());
		}
	}
	else
	{
		++count;
	}

	return;
}

void remove_empty_directories(const fsys_path_type& root_dir_path)
{
	std::cout << "remove empty dir root_path = "  << root_dir_path << std::endl;

	yggr::u64 count = 0;
	try
	{
		local_fsys_operators_type::for_each(
			root_dir_path,
			boost::bind(
				&handler_for_each_check_dir_file_count, _1, _2, _3,
				boost::cref(root_dir_path), boost::ref(count)),
			local_fsys_operators_type::file_types_options_type::general_directory);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}
}

int main(int argc, char* argv[])
{
    std::cout << "start" << std::endl;
	fsys_path_type root_path = local_fsys_operators_type::make_path_object("./compare_root_dir");

	std::cout << "root_dir is: " << root_path << std::endl;
	
	yggr::string chk_str;
	if(!(argc > 1))
	{
		std::cout << "!!!All duplicate files will be deleted (only non-duplicate files will be retained),"
						<< "enter \"yes\" to continue!!!" << std::endl;
		for(;;)
		{
			chk_str.clear();
			std::cin >> chk_str;

			if(chk_str == "yes")
			{
				break;
			}
			else if(chk_str == "no")
			{
				return 0;
			}
		}
	}

	std::cout << "remove duplicate files ..." << std::endl;

	hit_info_umap_type hit_infos;

	get_files(root_path, hit_infos);

	for(hit_info_umap_type::const_iterator i = hit_infos.begin(), isize = hit_infos.end(); i != isize; ++i)
	{
		if(i->second.second > 1)
		{
			remove_file(i->second.first);
		}
	}

	std::cout << "clear empty directories ..." << std::endl;
	remove_empty_directories(root_path);

	std::cout << "end" << std::endl;

	if(!(argc > 1))
	{
		char cc = 0;
		std::cin >> cc;
	}

	return 0;
}


