// tool_conv_to_cb_solution.cpp

#include "tool_conv_to_darwin_cmakelists_txt_config.hpp"

#if !(BOOST_VERSION < 105600)

#include "tool_conv_to_darwin_cmakelists_txt_cfg.hpp"

#include "file_operator.hpp"
#include "xml_operator.hpp"

#include "cb_tags_def.hpp"
#include "cb_parser.hpp"
#include "cbws_parser.hpp"

#include <yggr/range_ex/range_iterator_ex.hpp>
#include <yggr/iterator_ex/iterator.hpp>

#include <iostream>
#include <sstream>
#include <cassert>

#include <yggr/compile_link/linker.hpp>

#ifdef _MSC_VER
#	include <vld.h>
#endif // _MSC_VER

#include YGGR_PP_LINK_LIB(exception)
#include YGGR_PP_LINK_LIB(system_controller)
#include YGGR_PP_LINK_LIB(charset)
#include YGGR_PP_LINK_LIB(time)
#include YGGR_PP_LINK_LIB(ids)
#include YGGR_PP_LINK_LIB(base)

static const char* tool_conv_to_cb_solution_cfg_mark = "tool_conv_to_darwin_cmakelists_txt_cfg";

typedef yggr::set<ptree_string_type> ptree_string_set_type;
typedef yggr::set<yggr::utf8_string> file_list_type;
typedef yggr::unordered_set<yggr::utf8_string> filter_list_type;
typedef local_fsys_operators_type::path_type path_type;

typedef yggr::unordered_map<yggr::utf8_string, ptree_type> ptree_umap_type;
//typedef yggr::map<yggr::utf8_string, ptree_string_type> vcproj_file_app_tag_map_type;

typedef yggr::set<yggr::string> string_set_type;
typedef yggr::unordered_set<yggr::string> string_uset_type;


void print_cb_infos(const cb::cb_infos& infos)
{
	typedef cb::cb_infos::string_vt_type string_vt_type;
	typedef string_vt_type::const_iterator string_vt_citer_type;

	typedef cb::cb_infos::string_set_type string_set_type;
	typedef string_set_type::const_iterator string_set_citer_type;

	std::cout << "infos.proj_t_ = " << infos.proj_t_ << std::endl;
	std::cout << "\ninfos.proj_name_ = " << infos.proj_name_ << std::endl;

	std::cout << "\ninfos.proj_output_debug_ = " << infos.proj_output_debug_ << std::endl;
	std::cout << "\ninfos.proj_output_release_ = " << infos.proj_output_release_ << std::endl;

	// src_files
	{
		std::cout << "\ninfos.src_files_: " << std::endl;
		for(string_set_citer_type i = infos.src_files_.begin(), isize = infos.src_files_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	// include_dir
	{
		std::cout << "\ninfos.cinclude_: " << std::endl;
		for(string_set_citer_type i = infos.cinclude_.begin(), isize = infos.cinclude_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	// defined
	{
		std::cout << "\ninfos.cdefined_proj_: " << std::endl;
		for(string_set_citer_type i = infos.cdefined_proj_.begin(), isize = infos.cdefined_proj_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.cdefined_proj_debug_: " << std::endl;
		for(string_set_citer_type i = infos.cdefined_proj_debug_.begin(), isize = infos.cdefined_proj_debug_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.cdefined_proj_release_: " << std::endl;
		for(string_set_citer_type i = infos.cdefined_proj_release_.begin(), isize = infos.cdefined_proj_release_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	// cflags
	{
		std::cout << "\ninfos.cflag_proj_: " << std::endl;
		for(string_set_citer_type i = infos.cflag_proj_.begin(), isize = infos.cflag_proj_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.cflag_proj_debug_: " << std::endl;
		for(string_set_citer_type i = infos.cflag_proj_debug_.begin(), isize = infos.cflag_proj_debug_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.cflag_proj_release_: " << std::endl;
		for(string_set_citer_type i = infos.cflag_proj_release_.begin(), isize = infos.cflag_proj_release_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	// cppflags
	{
		std::cout << "\ninfos.cppflag_proj_: " << std::endl;
		for(string_set_citer_type i = infos.cppflag_proj_.begin(), isize = infos.cppflag_proj_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.cppflag_proj_debug_: " << std::endl;
		for(string_set_citer_type i = infos.cppflag_proj_debug_.begin(), isize = infos.cppflag_proj_debug_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.cppflag_proj_release_: " << std::endl;
		for(string_set_citer_type i = infos.cppflag_proj_release_.begin(), isize = infos.cppflag_proj_release_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	// link_libs_path
	{
		std::cout << "\ninfos.link_libs_path_: " << std::endl;
		for(string_set_citer_type i = infos.link_libs_path_.begin(), isize = infos.link_libs_path_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	// link_libs
	{
		std::cout << "\ninfos.link_libs_proj_: " << std::endl;
		for(string_vt_citer_type i = infos.link_libs_proj_.begin(), isize = infos.link_libs_proj_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.link_libs_proj_debug_: " << std::endl;
		for(string_vt_citer_type i = infos.link_libs_proj_debug_.begin(), isize = infos.link_libs_proj_debug_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.link_libs_proj_release_: " << std::endl;
		for(string_vt_citer_type i = infos.link_libs_proj_release_.begin(), isize = infos.link_libs_proj_release_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	// ldflags
	{
		std::cout << "\ninfos.ldflag_proj_: " << std::endl;
		for(string_set_citer_type i = infos.ldflag_proj_.begin(), isize = infos.ldflag_proj_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.ldflag_proj_debug_: " << std::endl;
		for(string_set_citer_type i = infos.ldflag_proj_debug_.begin(), isize = infos.ldflag_proj_debug_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.ldflag_proj_release_: " << std::endl;
		for(string_set_citer_type i = infos.ldflag_proj_release_.begin(), isize = infos.ldflag_proj_release_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}
}

template<typename Cfg>
bool read_cfg_file(const yggr::utf8_string& fname, const char* mark,  Cfg& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

	ipak_type ipak;
	return
		file_op::read_from_file(ipak.org_buf(), fname).size()
		&& ipak.load(YGGR_SERIALIZATION_NAME_NVP(mark, cfg));
}

template<typename Cfg>
bool write_cfg_file(const yggr::utf8_string& fname, const char* mark, const Cfg& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP(mark, cfg));

	return file_op::write_to_file(fname, opak.org_buf());
}

static yggr::utf8_string cfg_fname("tool_conv_to_darwin_cmakelists_txt_cfg.xml");

int format_config_file(const yggr::utf8_string& root_cfg_fname,
						tool_conv_to_darwin_cmakelists_txt_cfg& cfg_root)
{
	int ret = 0;
	if(!read_cfg_file(root_cfg_fname, tool_conv_to_cb_solution_cfg_mark, cfg_root))
	{
		tool_conv_to_darwin_cmakelists_txt_cfg cfg = tool_conv_to_darwin_cmakelists_txt_cfg_sample();
		write_cfg_file(root_cfg_fname, tool_conv_to_cb_solution_cfg_mark, cfg);

		cfg_root = boost::move(cfg);
		ret = -1;
	}

	return ret;
}

bool handler_for_each_search_files_filter(local_fsys_operators_type::recursive_directory_iterator_type dir_iter,
											const local_fsys_operators_type::directory_entry_type& entry,
											boost::uintmax_t current_count,
											local_fsys_operators_type::error_code_type* current_ec,
											const filter_list_type& dir_filter)
{
	return
		dir_iter != local_fsys_operators_type::recursive_directory_iterator_type()
		&& ( (yggr::file_system::recursive_directory_iterator_depth(dir_iter) > 1)
				|| (dir_filter.find(yggr::utf8_string(entry.path().filename().native())) != dir_filter.end()));
}

void handler_for_each_search_files(const local_fsys_operators_type::directory_entry_type& entry,
										boost::uintmax_t current_count,
										local_fsys_operators_type::error_code_type* current_ec,
										file_list_type& workspace_files)
{
	typedef local_fsys_operators_type::path_type path_type;

	const path_type& fpath = entry.path();

	//std::cout << "check_path: " << fpath << std::endl;

	if(!fpath.has_extension())
	{
		return;
	}

	if(file_op::check_file_ext(fpath, "_linux.workspace"))
	{
		std::cout << "find cbp_file:\t" << fpath << std::endl;
		workspace_files.insert(file_op::make_string_file_path(yggr::utf8_string(fpath.native())));
	}

	return;
}

void get_files(const yggr::utf8_string& str_root_path,
				const filter_list_type& dir_filter,
				file_list_type& workspace_files)
{
	std::cout << "root_path = "  << str_root_path << std::endl;
	path_type root_dir_path = local_fsys_operators_type::make_path_object(str_root_path);

	try
	{
		local_fsys_operators_type::recursive_for_each(
			root_dir_path,
			boost::bind(
				&handler_for_each_search_files, _1, _2, _3,
				boost::ref(workspace_files)),
			boost::bind(
				&handler_for_each_search_files_filter, _1, _2, _3, _4, boost::cref(dir_filter)),
			local_fsys_operators_type::file_types_options_type::general_file);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}
}

ptree_string_type& find_and_replace(ptree_string_type& src, const ptree_string_type& f, const ptree_string_type& r)
{
	for(std::size_t pos = src.find(f); pos != ptree_string_type::npos; pos = src.find(f))
	{
		src.replace(pos, f.size(), r);
	}

	return src;
}

template<typename Cont>
ptree_string_type make_cmake_l_multi_item_include(const Cont& cont, 
													const ptree_string_type& table_str = "",
													const ptree_string_type& root_path = "",
													const ptree_string_type& app_macro = "")
{
	typedef Cont cont_type;
	typedef typename yggr::range_ex::range_const_iterator_ex<cont_type>::type cont_citer_type;

	std::stringstream ss;

	for(cont_citer_type i = boost::begin(cont), isize = boost::end(cont); i != isize; ++i)
	{
		if(i->size() && i->front() == '/')
		{
			ss << table_str << *i << "\n";
		}
		else
		{
			ss << table_str << root_path << *i << "\n";
		}
	}

	if(app_macro.size())
	{
		ss << table_str << app_macro << "\n";
	}

	std::string tmp_str = ss.str();

	return ptree_string_type(tmp_str.begin(), tmp_str.end());
}

template<typename Cont>
ptree_string_type make_cmake_l_multi_item_libsdir(const Cont& cont, 
													const ptree_string_type& table_str = "",
													const ptree_string_type& root_path = "",
													const ptree_string_type& app_macro = "")
{
	typedef Cont cont_type;
	typedef typename yggr::range_ex::range_const_iterator_ex<cont_type>::type cont_citer_type;

	std::stringstream ss;

	for(cont_citer_type i = boost::begin(cont), isize = boost::end(cont); i != isize; ++i)
	{
		if(i->size() && (i->front() == '/' || (*i) == "${var_proj_ach_outdir}"))
		{
			ss << table_str << *i << "\n";
		}
		else
		{
			ss << table_str << root_path << *i << "\n";
		}
	}

	if(app_macro.size())
	{
		ss << table_str << app_macro << "\n";
	}

	std::string tmp_str = ss.str();

	return ptree_string_type(tmp_str.begin(), tmp_str.end());
}

template<typename Cont>
ptree_string_type make_cmake_l_multi_item_line(const Cont& cont, 
												const ptree_string_type& table_str = "",
												const ptree_string_type& root_path = "",
												const ptree_string_type& app_macro = "")
{
	typedef Cont cont_type;
	typedef typename yggr::range_ex::range_const_iterator_ex<cont_type>::type cont_citer_type;

	std::stringstream ss;

	std::size_t count = 0;
	for(cont_citer_type i = boost::begin(cont), isize = boost::end(cont); i != isize; ++i, ++count)
	{
		if(!count)
		{
			ss << root_path << *i;
		}
		else
		{
			ss << table_str << root_path << *i;
		}
	}

	if(app_macro.size())
	{
		if(!count)
		{
			ss << app_macro;
		}
		else
		{
			ss << table_str << app_macro;
		}
	}

	std::string tmp_str = ss.str();

	return ptree_string_type(tmp_str.begin(), tmp_str.end());
}


template<typename Cont>
ptree_string_type make_cmake_l_multi_item(const Cont& cont, 
											const ptree_string_type& table_str = "",
											const ptree_string_type& root_path = "",
											const ptree_string_type& app_macro = "")
{
	typedef Cont cont_type;
	typedef typename yggr::range_ex::range_const_iterator_ex<cont_type>::type cont_citer_type;

	std::stringstream ss;

	for(cont_citer_type i = boost::begin(cont), isize = boost::end(cont); i != isize; ++i)
	{
		ss << table_str << root_path << *i << "\n";
	}

	if(app_macro.size())
	{
		ss << table_str << app_macro << "\n";
	}

	std::string tmp_str = ss.str();

	return ptree_string_type(tmp_str.begin(), tmp_str.end());
}

void arm64_fat_filter_cpp(string_set_type& fatable, string_set_type& fatfailed, const string_set_type& flist)
{
	static string_uset_type exclude_arm64;

	if(!exclude_arm64.size())
	{
		//exclude_arm64.insert("log_mongodb_op_caller.cpp");
	}

	for(string_set_type::const_iterator i = flist.begin(), isize = flist.end(); i != isize; ++i)
	{
		if(exclude_arm64.find(*i) == exclude_arm64.end())
		{
			fatable.insert(*i);
		}
		else
		{
			fatfailed.insert(*i);
		}
	}
}

bool need_fix_arm64_support(cb::cb_infos& infos)
{
	static string_uset_type exclude_arm64;
	if(!exclude_arm64.size())
	{
		//exclude_arm64.insert("charset_utf8_char_serialization_test");
		//exclude_arm64.insert("charset_utf8_string_serialization_test");
		//exclude_arm64.insert("math_vector_and_matrix_nvp_seriaztion_test");
		//exclude_arm64.insert("packet_string_serialization_test_eins");
		//exclude_arm64.insert("packet_string_serialization_test_zwei");
		//exclude_arm64.insert("segment_mem_mapped_string_serialization_test");
		//exclude_arm64.insert("time_time_serialization_test");
	}

	return !(exclude_arm64.find(infos.proj_name_) == exclude_arm64.end());
}

cb::cb_infos::string_vt_type& 
	erase_arm64_support(cb::cb_infos::string_vt_type& out, const cb::cb_infos::string_vt_type& in)
{
	//std::cout << "----------erase_arm64_support----------------" << std::endl;
	typedef cb::cb_infos::string_vt_type string_vt_type;
	typedef string_vt_type::const_iterator string_vt_citer_type;

	static string_uset_type exclude_arm64;
	if(!exclude_arm64.size())
	{
		//exclude_arm64.insert("-lyggr_nsql_database_system${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d");
		//exclude_arm64.insert("-lmongoc${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d");
		//exclude_arm64.insert("-lmongocrypt${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d");
		//exclude_arm64.insert("-lkms-message${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d");
		//exclude_arm64.insert("-lutf8proc${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d");
		//exclude_arm64.insert("-lbson${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d");
		//exclude_arm64.insert("-lcommon${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d");
		exclude_arm64.insert("-lintel_dfp_obj${USRDEF_CMAKE_COMPILER_VERSION_TAG}-d");

		//exclude_arm64.insert("-lyggr_nsql_database_system${USRDEF_CMAKE_COMPILER_VERSION_TAG}");
		//exclude_arm64.insert("-lmongoc${USRDEF_CMAKE_COMPILER_VERSION_TAG}");
		//exclude_arm64.insert("-lmongocrypt${USRDEF_CMAKE_COMPILER_VERSION_TAG}");
		//exclude_arm64.insert("-lkms-message${USRDEF_CMAKE_COMPILER_VERSION_TAG}");
		//exclude_arm64.insert("-lutf8proc${USRDEF_CMAKE_COMPILER_VERSION_TAG}");
		//exclude_arm64.insert("-lbson${USRDEF_CMAKE_COMPILER_VERSION_TAG}");
		//exclude_arm64.insert("-lcommon${USRDEF_CMAKE_COMPILER_VERSION_TAG}");
		exclude_arm64.insert("-lintel_dfp_obj${USRDEF_CMAKE_COMPILER_VERSION_TAG}");

		//exclude_arm64.insert("-lsasl2");
	}

	cb::cb_infos::string_vt_type tmp;
	tmp.reserve(in.size());

	for(string_vt_citer_type i = in.begin(), isize = in.end(); i != isize; ++i)
	{
		std::cout << "------exclude_arm64.find( " << *i << ")----------------" << std::endl;
		if(exclude_arm64.find(*i) == exclude_arm64.end())
		{
			tmp.push_back(*i);
		}
	}

	out.swap(tmp);
	return out;
}

ptree_string_type& conv_to_proj_cmakelists_txt(ptree_string_type& out, 
												cb::cb_infos& infos,
												const filter_list_type& cpp_filter_list,
												bool is_yggr_proj)
{
	typedef cb::cb_infos::string_vt_type string_vt_type;
	typedef string_vt_type::const_iterator string_vt_citer_type;

	if(!infos.validate())
	{
		return out;
	}

	assert(
		(infos.proj_t_ == cb::cb_app_tags::s_tag_exe())
		|| (infos.proj_t_ == cb::cb_app_tags::s_tag_dll())
		|| (infos.proj_t_ == cb::cb_app_tags::s_tag_static_lib()));

	for(filter_list_type::const_iterator i = cpp_filter_list.begin(), isize = cpp_filter_list.end();
			i != isize; ++i)
	{
		infos.src_files_.erase((*i).org_str());
	}

	std::stringstream ss;

	ss << "#" << infos.proj_name_ << " CMakeLists.txt\n\n"
		
		<< "project(" << infos.proj_name_ << ")\n\n"
		
		<< "message(STATUS \"build-sub-project [ " << infos.proj_name_ << " ]\")\n\n"

		<< "set(var_proj_name " << infos.proj_name_ << ")\n\n"

		<< "set(var_root_proj_dir ${PROJECT_SOURCE_DIR}/../../..)\n"
		<< "set(var_proj_src_dir ${var_root_proj_dir}/" << infos.proj_cur_dir_ << ")\n\n"
		;

	string_set_type flist_fat, flist_fat_failed;
	arm64_fat_filter_cpp(flist_fat, flist_fat_failed, infos.src_files_);

	if(flist_fat_failed.size())
	{
		ss << "if(${USRDEF_HAS_ARM64})\n"
			<< "\tset(var_proj_src_list \n" << make_cmake_l_multi_item(flist_fat, "\t\t\t", "${var_proj_src_dir}/") << ")\n"
			<< "else()\n"
			<< "\tset(var_proj_src_list \n" << make_cmake_l_multi_item(infos.src_files_, "\t\t\t", "${var_proj_src_dir}/") << ")\n\n"
			<< "endif()\n\n"
			;
	}
	else
	{
		ss << "set(var_proj_src_list \n" << make_cmake_l_multi_item(infos.src_files_, "\t\t", "${var_proj_src_dir}/") << ")\n\n"
			;
	}

	// out dir
	if(infos.proj_name_ == "dll_cal_test"
		|| infos.proj_name_ == "yggr_lua_base_type"
		|| infos.proj_name_ == "lua_cal_svr_procotol"
		|| infos.proj_name_ == "lua_calculator"
		|| infos.proj_name_ == "lua_hello_dll"
		|| infos.proj_name_ == "pyd_export")
	{
		ss << "set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})\n\n"

			<< "if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL \"Debug\"))\n"
			<< "\tset(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR}/Debug-x64)\n"
			<< "else()\n"
			<< "\tset(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR}/Release-x64)\n"
			<< "endif()\n\n"
			
			<< "set(var_proj_ach_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})\n\n"
			;
	}
	else
	{
		ss << "set(var_proj_bin_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_BIN_OUTDIR})\n"
			<< "set(var_proj_lib_outdir ${var_root_proj_dir}/${USRDEF_CMAKE_LIB_OUTDIR})\n"
			<< "set(var_proj_ach_outdir ${var_proj_lib_outdir})\n\n"
			;
	}

	// include
	ss << "set(var_proj_include_dirs \n"
		<< "\t\t${var_proj_src_dir}\n"
		<< make_cmake_l_multi_item_include(infos.cinclude_, "\t\t", "${var_proj_src_dir}/", "${USRDEF_CMAKE_INCLUDE_DIRS}")
		<< ")\n\n"
		;

	// macro
	ss << "set(var_proj_c_defineds \""
		<< make_cmake_l_multi_item_line(infos.cdefined_proj_, " ", "", "${USRDEF_CMAKE_DEFINEDS}") 
		<< "\")\n\n"

		<< "set(var_proj_c_defineds_debug \""
		<< make_cmake_l_multi_item_line(infos.cdefined_proj_debug_, " ", "", "${USRDEF_CMAKE_DEFINEDS_DEBUG}") 
		<< "\")\n\n"

		<< "set(var_proj_c_defineds_release \""
		<< make_cmake_l_multi_item_line(infos.cdefined_proj_release_, " ", "", "${USRDEF_CMAKE_DEFINEDS_RELEASE}") 
		<< "\")\n\n"
		;
	
	// cflag
	ss << "set(var_proj_c_flags \""
		<< make_cmake_l_multi_item_line(infos.cflag_proj_, " ", "", "${USRDEF_CMAKE_CFLAGS}") 
		<< "\")\n\n"

		<< "set(var_proj_c_flags_debug \""
		<< make_cmake_l_multi_item_line(infos.cflag_proj_debug_, " ", "", "${USRDEF_CMAKE_CFLAGS_DEBUG}") 
		<< "\")\n\n"

		<< "set(var_proj_c_flags_release \""
		<< make_cmake_l_multi_item_line(infos.cflag_proj_release_, " ", "", "${USRDEF_CMAKE_CFLAGS_RELEASE}") 
		<< "\")\n\n"
		;

	// cxxflag
	ss << "set(var_proj_cxx_flags \n"
		<< make_cmake_l_multi_item(infos.cppflag_proj_, "\t\t", "", "${USRDEF_CMAKE_CXXFLAGS}") 
		<< ")\n\n"

		<< "set(var_proj_cxx_flags_debug \n"
		<< make_cmake_l_multi_item(infos.cppflag_proj_debug_, "\t\t", "", "${USRDEF_CMAKE_CXXFLAGS_DEBUG}") 
		<< ")\n\n"

		<< "set(var_proj_cxx_flags_release \n"
		<< make_cmake_l_multi_item(infos.cppflag_proj_release_, "\t\t", "", "${USRDEF_CMAKE_CXXFLAGS_RELEASE}") 
		<< ")\n\n"
		;

	// link_libs_path
	ss << "set(var_proj_link_libs_dirs \n" 
		<< make_cmake_l_multi_item_libsdir(infos.link_libs_path_, "\t\t", "${var_proj_src_dir}/", "${USRDEF_CMAKE_LD_LIBRARY_PATH}") 
		<< ")\n\n"
		;

	// link_libs
	//if(need_fix_arm64_support(infos))
	{
		//std::cout << "------------ need_fix_arm64_support ----------" << std::endl;
		string_vt_type link_libs_proj;
		string_vt_type link_libs_proj_debug;
		string_vt_type link_libs_proj_release;

		erase_arm64_support(link_libs_proj, infos.link_libs_proj_);
		erase_arm64_support(link_libs_proj_debug, infos.link_libs_proj_debug_);
		erase_arm64_support(link_libs_proj_release, infos.link_libs_proj_release_);

		if(link_libs_proj.size() == infos.link_libs_proj_.size())
		{
			ss << "set(var_proj_link_libs \n"
				<< make_cmake_l_multi_item(infos.link_libs_proj_, "\t\t", "", "${USRDEF_CMAKE_LD_LIBRARY}") 
				<< ")\n\n"
				;
		}
		else
		{
			ss << "if(${USRDEF_HAS_X86_64})\n"
				<< "\tset(var_proj_link_libs \n"
				<< make_cmake_l_multi_item(infos.link_libs_proj_, "\t\t\t", "", "${USRDEF_CMAKE_LD_LIBRARY}") 
				<< "\t)\n"
				<< "else()\n"
				<< "\tset(var_proj_link_libs \n"
				<< make_cmake_l_multi_item(link_libs_proj, "\t\t\t", "", "${USRDEF_CMAKE_LD_LIBRARY}") 
				<< "\t)\n"
				<< "endif()\n\n"
				;
		}

		if(link_libs_proj_debug.size() == infos.link_libs_proj_debug_.size())
		{
			ss << "set(var_proj_link_libs_debug \n"
				<< make_cmake_l_multi_item(infos.link_libs_proj_debug_, "\t\t", "", "${USRDEF_CMAKE_LDFLAGS_DEBUG}") 
				<< ")\n\n"
				;
		}
		else
		{
			ss << "if(${USRDEF_HAS_X86_64})\n"
				<< "\tset(var_proj_link_libs_debug \n"
				<< make_cmake_l_multi_item(infos.link_libs_proj_debug_, "\t\t\t", "", "${USRDEF_CMAKE_LDFLAGS_DEBUG}") 
				<< "\t)\n"
				<< "else()\n"
				<< "\tset(var_proj_link_libs_debug \n"
				<< make_cmake_l_multi_item(link_libs_proj_debug, "\t\t\t", "", "${USRDEF_CMAKE_LDFLAGS_DEBUG}") 
				<< "\t)\n"
				<< "endif()\n\n"
				;
		}

		if(link_libs_proj_release.size() == infos.link_libs_proj_release_.size())
		{
			ss << "set(var_proj_link_libs_release \n"
				<< make_cmake_l_multi_item(infos.link_libs_proj_release_, "\t\t", "", "${USRDEF_CMAKE_LD_LIBRARY_RELEASE}") 
				<< ")\n\n"
				;
		}
		else
		{
			ss << "if(${USRDEF_HAS_X86_64})\n"
				<< "\tset(var_proj_link_libs_release \n"
				<< make_cmake_l_multi_item(infos.link_libs_proj_release_, "\t\t\t", "", "${USRDEF_CMAKE_LD_LIBRARY_RELEASE}") 
				<< "\t)\n"
				<< "else()\n"
				<< "\tset(var_proj_link_libs_release \n"
				<< make_cmake_l_multi_item(link_libs_proj_release, "\t\t\t", "", "${USRDEF_CMAKE_LD_LIBRARY_RELEASE}") 
				<< "\t)\n"
				<< "endif()\n\n"
				;
		}
	}
	// else
	// {
	// 	ss << "set(var_proj_link_libs \n"
	// 		<< make_cmake_l_multi_item(infos.link_libs_proj_, "\t\t", "", "${USRDEF_CMAKE_LD_LIBRARY}") 
	// 		<< ")\n\n"

	// 		<< "set(var_proj_link_libs_debug \n"
	// 		<< make_cmake_l_multi_item(infos.link_libs_proj_debug_, "\t\t", "", "${USRDEF_CMAKE_LD_LIBRARY_DEBUG}") 
	// 		<< ")\n\n"

	// 		<< "set(var_proj_link_libs_release \n"
	// 		<< make_cmake_l_multi_item(infos.link_libs_proj_release_, "\t\t", "", "${USRDEF_CMAKE_LD_LIBRARY_RELEASE}") 
	// 		<< ")\n\n"
	// 		;
	// }

	// ldflags
	ss << "set(var_proj_ldflags \n"
		<< make_cmake_l_multi_item(infos.ldflag_proj_, "\t\t", "", "${USRDEF_CMAKE_LDFLAGS}") 
		<< ")\n\n"

		<< "set(var_proj_ldflags_debug \n"
		<< make_cmake_l_multi_item(infos.ldflag_proj_debug_, "\t\t", "", "${USRDEF_CMAKE_LDFLAGS_DEBUG}") 
		<< ")\n\n"

		<< "set(var_proj_ldflags_release \n"
		<< make_cmake_l_multi_item(infos.ldflag_proj_release_, "\t\t", "", "${USRDEF_CMAKE_LDFLAGS_RELEASE}") 
		<< ")\n\n"
		;

	yggr::string proj_out_debug_name = infos.proj_output_debug_;
	yggr::string proj_out_release_name = infos.proj_output_release_;

	if(infos.proj_t_ == cb::cb_app_tags::s_tag_static_lib())
	{
		if(proj_out_debug_name.substr(0, 3) == "lib")
		{
			proj_out_debug_name = proj_out_debug_name.substr(3, proj_out_debug_name.size());
		}

		if(proj_out_release_name.substr(0, 3) == "lib")
		{
			proj_out_release_name = proj_out_release_name.substr(3, proj_out_release_name.size());
		}
	}

	ss << "if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL \"Debug\"))\n"
		<< "\tset(var_proj_out_name " << proj_out_debug_name << ")\n"
		<< "\tset(var_proj_c_defineds \"${var_proj_c_defineds_debug} ${var_proj_c_defineds}\")\n"
		<< "\tset(var_proj_c_flags \"${var_proj_c_flags_debug} ${var_proj_c_flags}\")\n"
		<< "\tset(var_proj_cxx_flags \"${var_proj_cxx_flags_debug} ${var_proj_cxx_flags} ${var_proj_c_defineds}\")\n"
		<< "\tset(var_proj_link_libs ${var_proj_link_libs_debug} ${var_proj_link_libs})\n"
		<< "\tset(var_proj_ldflags \"${var_proj_ldflags_debug} ${var_proj_ldflags}\")\n"
		<< "else()\n"
		<< "\tset(var_proj_out_name " << proj_out_release_name << ")\n"
		<< "\tset(var_proj_c_defineds \"${var_proj_c_defineds_release} ${var_proj_c_defineds}\")\n"
		<< "\tset(var_proj_c_flags \"${var_proj_c_flags_release} ${var_proj_c_flags}\")\n"
		<< "\tset(var_proj_cxx_flags \"${var_proj_cxx_flags_release} ${var_proj_cxx_flags} ${var_proj_c_defineds}\")\n"
		<< "\tset(var_proj_link_libs ${var_proj_link_libs_release} ${var_proj_link_libs})\n"
		<< "\tset(var_proj_ldflags \"${var_proj_ldflags_release} ${var_proj_ldflags}\")\n"
		<< "endif()\n\n"
		;

	ss << "include_directories(${var_proj_include_dirs})\n"
		<< "add_definitions(${var_proj_c_defineds})\n"
		<< "set(CMAKE_C_FLAGS ${var_proj_c_flags})\n"
		<< "set(CMAKE_CXX_FLAGS ${var_proj_cxx_flags})\n\n"
		;

	if(infos.proj_t_ == cb::cb_app_tags::s_tag_static_lib())
	{
		ss << "add_library(${var_proj_name} ${var_proj_src_list})\n\n";
	}
	else	
	{
		ss << "link_directories(${var_proj_link_libs_dirs})\n\n";

		if(infos.proj_t_ == cb::cb_app_tags::s_tag_dll())
		{
			ss << "add_library(${var_proj_name} SHARED ${var_proj_src_list})\n\n";

			ss << "set_target_properties( \n"
				<< "\t${var_proj_name}\n"
				<< "\tPROPERTIES\n"
				<< "\tSUFFIX \".dylib\"\n"
				<< ")\n\n"
				;

			ss << "set_target_properties( \n"
				<< "\t${var_proj_name}\n"
				<< "\tPROPERTIES\n"
				<< "\tIMPORTED_IMPLIB \"lib${var_proj_out_name}.a\"\n"
				<< ")\n\n"
				;
		}
		else if(infos.proj_t_ == cb::cb_app_tags::s_tag_exe())
		{
			ss << "add_executable(${var_proj_name} ${var_proj_src_list})\n\n";
		}
		else
		{
			assert(false);
		}

		ss << "target_link_libraries(${var_proj_name} ${var_proj_link_libs})\n";
	}

	ss << "set_target_properties( \n"
		<< "\t${var_proj_name}\n"
		<< "\tPROPERTIES\n"
		<< "\tOUTPUT_NAME ${var_proj_out_name}\n"
		<< "\tRUNTIME_OUTPUT_DIRECTORY ${var_proj_bin_outdir}\n"
		<< "\tLIBRARY_OUTPUT_DIRECTORY ${var_proj_lib_outdir}\n"
		<< "\tARCHIVE_OUTPUT_DIRECTORY ${var_proj_ach_outdir}\n"
		<< ")\n\n"
		;

	std::string str = ss.str();
	ptree_string_type out_str(str.begin(), str.end());
	out.swap(out_str);
	return out;
}


ptree_string_type format_cb_infos_output_string(const ptree_string_type& str)
{
	static const ptree_string_type mark_proj_name = "$(PROJECT_NAME)";
	static const ptree_string_type mark_compiler_version_tag = "$(#COMPILER_VERSION_TAG)";
	static const ptree_string_type mark_compiler_version = "$(#COMPILER_VERSION)";
	//static const ptree_string_type mark_duplicate_link = "$(#DUPLICATE_LINK_MARK)";
	
	static const ptree_string_type mark_proj_name_r = "${var_proj_name}";
	static const ptree_string_type mark_compiler_version_tag_r = "${USRDEF_CMAKE_COMPILER_VERSION_TAG}";
	static const ptree_string_type mark_compiler_version_r = "${USRDEF_CMAKE_COMPILER_VERSION}";
	//static const ptree_string_type mark_duplicate_link_r = "";

	ptree_string_type str_fname = file_op::get_file_name(str);

	find_and_replace(str_fname, mark_proj_name, mark_proj_name_r);
	find_and_replace(str_fname, mark_compiler_version_tag, mark_compiler_version_tag_r);
	find_and_replace(str_fname, mark_compiler_version, mark_compiler_version_r);
	//find_and_replace(str_fname, mark_duplicate_link, mark_duplicate_link_r);

	return str_fname;
}

ptree_string_type format_cb_infos_link_lib_path(const ptree_string_type& str)
{
	static const ptree_string_type mark_compiler_version_tag = "$(#COMPILER_VERSION_TAG)";
	static const ptree_string_type mark_compiler_version = "$(#COMPILER_VERSION)";
	static const ptree_string_type mark_boost_version_tag = "$(#BOOST_VERSION_TAG)";
	static const ptree_string_type mark_boost_version = "$(#BOOST_VERSION)";
	static const ptree_string_type mark_python_version = "$(#PYTHON_VERSION)";
	static const ptree_string_type mark_arch64_tag = "-x64";


	static const ptree_string_type mark_compiler_version_tag_r = "${USRDEF_CMAKE_COMPILER_VERSION_TAG}";
	static const ptree_string_type mark_compiler_version_r = "${USRDEF_CMAKE_COMPILER_VERSION}";
	static const ptree_string_type mark_boost_version_tag_r = "${USRDEF_CMAKE_BOOST_VERSION_TAG}";
	static const ptree_string_type mark_boost_version_r = "${USRDEF_CMAKE_BOOST_VERSION}";
	static const ptree_string_type mark_python_version_r = "${USRDEF_CMAKE_PYTHON_VERSION}";
	static const ptree_string_type mark_arch_tag_r = "${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}";


	ptree_string_type str_out = str;

	find_and_replace(str_out, mark_compiler_version_tag, mark_compiler_version_tag_r);
	find_and_replace(str_out, mark_compiler_version, mark_compiler_version_r);
	find_and_replace(str_out, mark_boost_version_tag, mark_boost_version_tag_r);
	find_and_replace(str_out, mark_boost_version, mark_boost_version_r);
	find_and_replace(str_out, mark_python_version, mark_python_version_r);
	find_and_replace(str_out, mark_arch64_tag, mark_arch_tag_r);

	return str_out;
}

ptree_string_type format_cb_infos_link_lib(const ptree_string_type& str)
{
	static const ptree_string_type mark_compiler_version_tag = "$(#COMPILER_VERSION_TAG)";
	static const ptree_string_type mark_compiler_version = "$(#COMPILER_VERSION)";
	static const ptree_string_type mark_boost_version_tag = "$(#BOOST_VERSION_TAG)";
	static const ptree_string_type mark_boost_version = "$(#BOOST_VERSION)";
	static const ptree_string_type mark_python_version = "$(#PYTHON_VERSION)";
	static const ptree_string_type mark_arch64_tag = "-x64";

	static const ptree_string_type mark_boost_mt_d = "-mt-d${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}";
	static const ptree_string_type mark_boost_mt = "-mt${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}";

	static const ptree_string_type mark_duplicate_link = "$(#DUPLICATE_LINK_MARK)";

	static const ptree_string_type mark_compiler_version_tag_r = "${USRDEF_CMAKE_COMPILER_VERSION_TAG}";
	static const ptree_string_type mark_compiler_version_r = "${USRDEF_CMAKE_COMPILER_VERSION}";
	static const ptree_string_type mark_boost_version_tag_r = "${USRDEF_CMAKE_BOOST_VERSION_TAG}";
	static const ptree_string_type mark_boost_version_r = "${USRDEF_CMAKE_BOOST_VERSION}";
	static const ptree_string_type mark_python_version_r = "${USRDEF_CMAKE_PYTHON_VERSION}";
	static const ptree_string_type mark_arch_tag_r = "${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}";

	static const ptree_string_type mark_boost_mt_d_r = "-mt${USRDEF_CMAKE_BOOST_LINK_TAG_DEBUG}${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}";
	static const ptree_string_type mark_boost_mt_r = "-mt${USRDEF_CMAKE_BOOST_LINK_TAG_RELEASE}${USRDEF_CMAKE_BUILD_ARCH_BITS_TAG}";

	static const ptree_string_type mark_duplicate_link_r = "";

	ptree_string_type str_out = str;

	find_and_replace(str_out, mark_compiler_version_tag, mark_compiler_version_tag_r);
	find_and_replace(str_out, mark_compiler_version, mark_compiler_version_r);
	find_and_replace(str_out, mark_boost_version_tag, mark_boost_version_tag_r);
	find_and_replace(str_out, mark_boost_version, mark_boost_version_r);
	find_and_replace(str_out, mark_python_version, mark_python_version_r);
	find_and_replace(str_out, mark_arch64_tag, mark_arch_tag_r);
	find_and_replace(str_out, mark_duplicate_link, mark_duplicate_link_r);

	if(8 < str_out.size() && str_out.substr(0, 8) == "-lboost_")
	{
		find_and_replace(str_out, mark_boost_mt_d, mark_boost_mt_d_r);
		find_and_replace(str_out, mark_boost_mt, mark_boost_mt_r);
	}

	return str_out;
}

cb::cb_infos& fix_cb_infos_sp(cb::cb_infos& infos)
{
	if(infos.proj_name_ == "LzmaLib")
    {
		infos.cdefined_proj_.erase("-DBUILD_DLL");
		infos.cdefined_proj_.erase("-DLZMALIB_EXPORT");

		infos.cdefined_proj_debug_.erase("-DBUILD_DLL");
		infos.cdefined_proj_debug_.erase("-DLZMALIB_EXPORT");

		infos.cdefined_proj_release_.erase("-DBUILD_DLL");
		infos.cdefined_proj_release_.erase("-DLZMALIB_EXPORT");

		infos.link_libs_path_.clear();

		infos.link_libs_proj_.clear();
		infos.link_libs_proj_debug_.clear();
		infos.link_libs_proj_release_.clear();

		infos.ldflag_proj_.clear();
		infos.ldflag_proj_debug_.clear();
		infos.ldflag_proj_release_.clear();

		infos.proj_t_ = cb::cb_app_tags::s_tag_static_lib();
    }

	return infos;
}

cb::cb_infos& format_cb_infos(cb::cb_infos& infos)
{

	typedef cb::cb_infos::string_vt_type string_vt_type;

	if(!infos.validate())
	{
		return infos;
	}

	// proj_output
	if(infos.proj_output_debug_.size())
	{
		infos.proj_output_debug_ = format_cb_infos_output_string(infos.proj_output_debug_);
	}

	if(infos.proj_output_release_.size())
	{
		infos.proj_output_release_ = format_cb_infos_output_string(infos.proj_output_release_);
	}

	// link lib
	{
		for(string_vt_type::iterator i = infos.link_libs_proj_.begin(), isize = infos.link_libs_proj_.end();
				i != isize; ++i)
		{
			*i = format_cb_infos_link_lib(*i);
		}
	}

	{
		for(string_vt_type::iterator i = infos.link_libs_proj_debug_.begin(), isize = infos.link_libs_proj_debug_.end();
				i != isize; ++i)
		{
			*i = format_cb_infos_link_lib(*i);
		}
	}

	{
		for(string_vt_type::iterator i = infos.link_libs_proj_release_.begin(), isize = infos.link_libs_proj_release_.end();
				i != isize; ++i)
		{
			*i = format_cb_infos_link_lib(*i);
		}
	}

	// link_libs_path
	if(infos.link_libs_path_.erase("../lib64") 
		|| infos.link_libs_path_.erase("./lib64")
		|| infos.link_libs_path_.erase("lib64"))
	{
		infos.link_libs_path_.insert("${var_proj_ach_outdir}");
	}

	return fix_cb_infos_sp(infos);
}

yggr::string make_cmakelist_txt_dir_name(const yggr::string& str_cbp_fpath)
{
	yggr::string str_fname_no_ext = file_op::get_file_name_no_ext(str_cbp_fpath);
	return  str_fname_no_ext.substr(0, str_fname_no_ext.size() - 6 /*"_linux"*/);
}


ptree_string_set_type& make_cmakelists_subdirs(ptree_string_set_type& out, const file_list_type& flist)
{
	for(file_list_type::const_iterator i = flist.begin(), isize = flist.end(); i != isize; ++i)
	{
		yggr::string str = make_cmakelist_txt_dir_name(*i);
		assert(str.size());
		out.insert(boost::move(str));
	}

	return out;
}

void arm64_fat_filter(ptree_string_set_type& fatable, ptree_string_set_type& fatfailed, const ptree_string_set_type& flist)
{
	static string_uset_type exclude_arm64;

	if(!exclude_arm64.size())
	{
		//exclude_arm64.insert("yggr_nsql_database_system");
		exclude_arm64.insert("pyd_export");
		// exclude_arm64.insert("nsql_database_system_base_bson_test");
		// exclude_arm64.insert("nsql_database_system_bson_serialize_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_basic_demo");
		// exclude_arm64.insert("nsql_database_system_c_bson_basic_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_binary_buffer_ref_operator_set_test_equal");
		// exclude_arm64.insert("nsql_database_system_c_bson_binary_buffer_ref_operator_set_test_liner_only");
		// exclude_arm64.insert("nsql_database_system_c_bson_binary_buffer_ref_operator_set_test_sizeof_greater");
		// exclude_arm64.insert("nsql_database_system_c_bson_binary_buffer_ref_operator_set_test_sizeof_less");
		// exclude_arm64.insert("nsql_database_system_c_bson_binary_buffer_ref_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_binary_buffer_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_code_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_code_w_scope_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_date_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_date_time_timeval_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_dbpointer_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_decimal128_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_error_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_oid_test2");
		// exclude_arm64.insert("nsql_database_system_c_bson_oid_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_regex_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_symbol_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_time_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_timestamp_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_timeval_test");
		// exclude_arm64.insert("nsql_database_system_c_bson_value_test2");
		// exclude_arm64.insert("nsql_database_system_c_bson_value_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_apm_callbacks_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_auto_encryption_opts_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_client_encryption_datakey_opts_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_client_encryption_encrypt_opts_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_client_encryption_encrypt_range_opts_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_client_encryption_opts_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_client_encryption_rewrap_many_datakey_result_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_connection_init_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_find_and_modify_opts_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_gridfs_file_opt_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_gridfs_file_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_host_info_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_index_model_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_read_concern_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_read_prefs_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_server_api_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_ssl_opt_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_stream_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_uri_test");
		// exclude_arm64.insert("nsql_database_system_c_mongo_write_concern_test");
		// exclude_arm64.insert("nsql_database_system_cert_file_win32_load_failed_test");
		// exclude_arm64.insert("nsql_database_system_mongo_accesser_encryption_test_of_connect");
		// exclude_arm64.insert("nsql_database_system_mongo_accesser_encryption_test_of_connect_pool");
		// exclude_arm64.insert("nsql_database_system_mongo_accesser_test_of_connection");
		// exclude_arm64.insert("nsql_database_system_mongo_accesser_test_of_connection_mt");
		// exclude_arm64.insert("nsql_database_system_mongo_accesser_test_of_connection_pool");
		// exclude_arm64.insert("nsql_database_system_mongo_accesser_test_of_connection_pool_mt");
		// exclude_arm64.insert("nsql_database_system_mongo_accesser_test_of_connection_sharding");
		// exclude_arm64.insert("nsql_database_system_mongo_gridfs_bucket_org_test");
		// exclude_arm64.insert("nsql_database_system_mongo_gridfs_org_test");
		// exclude_arm64.insert("nsql_database_system_mongo_operators_dollar_cmd_def_test");
		// exclude_arm64.insert("nsql_database_system_mongoc_connection_pool_test");
		// exclude_arm64.insert("nsql_database_system_mongodbc_basic_connect_test");
		// exclude_arm64.insert("nsql_database_system_test_mongodb_async_mgr");
		// exclude_arm64.insert("nsql_database_system_test_mongodb_mgr");

		// exclude_arm64.insert("any_val_any_bson_serialize_test");
		// exclude_arm64.insert("any_val_params_bson_serialize_test");
		// exclude_arm64.insert("charset_utf8_string_bson_pak_test");
		// exclude_arm64.insert("exception_bson_pak_test_of_error_code");
		// exclude_arm64.insert("ids_uuid_bson_serialize_test");

		// exclude_arm64.insert("packet_bson_cmd_pak_test_of_container_midx");
		// exclude_arm64.insert("packet_bson_cmd_pak_test_of_container_part1");
		// exclude_arm64.insert("packet_bson_cmd_pak_test_of_container_part2");
		// exclude_arm64.insert("packet_bson_pak_test_of_class_and_base");
		// exclude_arm64.insert("packet_bson_pak_test_of_container_part1_1");
		// exclude_arm64.insert("packet_bson_pak_test_of_container_part1_2");
		// exclude_arm64.insert("packet_bson_pak_test_of_container_part2_1");
		// exclude_arm64.insert("packet_bson_pak_test_of_container_part2_2");
		// exclude_arm64.insert("packet_bson_pak_test_of_container_part3");
		// exclude_arm64.insert("packet_bson_pak_test_of_container_part4");
		// exclude_arm64.insert("packet_bson_pak_test_of_tuple_container");
		// exclude_arm64.insert("packet_bug_test_bson_pak_man");
		// exclude_arm64.insert("packet_shared_ptr_bson_serialization_test");
		// exclude_arm64.insert("packet_test_bson_serialization_include");

		exclude_arm64.insert("compiler_link_test_msvc_maroc");

		// exclude_arm64.insert("exception_exception_mongodb_log_test");
		// exclude_arm64.insert("log_log_mongodb_accesser_test");
		// exclude_arm64.insert("log_log_mongodb_op_caller_test");
		// exclude_arm64.insert("log_log_mongodb_op_test");
	}

	for(ptree_string_set_type::const_iterator i = flist.begin(), isize = flist.end(); i != isize; ++i)
	{
		if(exclude_arm64.find(*i) == exclude_arm64.end())
		{
			fatable.insert(*i);
		}
		else
		{
			fatfailed.insert(*i);
		}
	}
}

ptree_string_type& conv_to_cbws_cmakelists_txt(ptree_string_type& out,
												const cb::cbws_infos& cbws_infos,
												const file_list_type& fixed_cbs_flist_a,
												const file_list_type& fixed_cbs_flist_so,
												const file_list_type& fixed_cbs_flist_exe)
{
	std::stringstream ss;

	ptree_string_set_type subdirs_a, subdirs_so, subdirs_exe;

	make_cmakelists_subdirs(subdirs_a, fixed_cbs_flist_a);
	make_cmakelists_subdirs(subdirs_so, fixed_cbs_flist_so);
	make_cmakelists_subdirs(subdirs_exe, fixed_cbs_flist_exe);

	ss << "#" << cbws_infos.cbws_title_ << " CMakeLists.txt\n\n"

		<< "cmake_minimum_required(VERSION 3.12)\n\n"
		;

	ss << "if(USRDEF_CMAKE_C_COMPILER)\n"
		<< "\tset(CMAKE_C_COMPILER ${USRDEF_CMAKE_C_COMPILER})\n"
		<< "endif()\n\n"
		;

	ss << "if(USRDEF_CMAKE_CXX_COMPILER)\n"
		<< "\tset(CMAKE_CXX_COMPILER ${USRDEF_CMAKE_CXX_COMPILER})\n"
		<< "endif()\n\n"
		;

	ss << "#set(USRDEF_CMAKE_COMPILER_VERSION \"clang-darwin16\")\n"
		<< "#set(USRDEF_CMAKE_BUILD_LD_ARCH_TAG \"a\")\n"
		<< "#set(USRDEF_CMAKE_BUILD_ARCH_BITS \"64\")\n"
		<< "set(USRDEF_CMAKE_BOOST_VERSION \"1_82\")\n"
		<< "set(USRDEF_CMAKE_PYTHON_VERSION \"313\")\n\n"

		<< "set(USRDEF_CMAKE_PYTHON_INCLUDE_DIR \"python3.13\")\n\n"

		<< "#set(USRDEF_CMAKE_OSX_SDK_NAME \"macosx\")\n\n"

		<< "if(\"${USRDEF_CMAKE_OSX_SDK_NAME}\" STREQUAL \"macosx\")\n"
		<< "\tset(USRDEF_CMAKE_BOOST_LINK_TAG_DEBUG \"-d\")\n"
		<< "\tset(USRDEF_CMAKE_BOOST_LINK_TAG_RELEASE \"\")\n"
		<< "else()\n"
		<< "\tset(USRDEF_CMAKE_BOOST_LINK_TAG_DEBUG \"-sd\")\n"
		<< "\tset(USRDEF_CMAKE_BOOST_LINK_TAG_RELEASE \"-s\")\n"
		<< "endif()\n\n"
		;

	ss << "set(USRDEF_CMAKE_COMPILER_VERSION_TAG \"-${USRDEF_CMAKE_COMPILER_VERSION}\")\n"
		<< "set(USRDEF_CMAKE_BUILD_ARCH_BITS_TAG \"-${USRDEF_CMAKE_BUILD_LD_ARCH_TAG}${USRDEF_CMAKE_BUILD_ARCH_BITS}\")\n\n"
		<< "set(USRDEF_CMAKE_BOOST_VERSION_TAG \"-${USRDEF_CMAKE_BOOST_VERSION}\")\n\n"
		;

	ss << "project(" << cbws_infos.cbws_title_ << ")\n\n";

	ss << "message(STATUS \"build-root-project [ " << cbws_infos.cbws_title_ << " ]\")\n\n";

	ss << "set(CMAKE_C_STANDARD ${USRDEF_CMAKE_C_STANDARD})\n"
		<< "set(CMAKE_CXX_STANDARD ${USRDEF_CMAKE_CXX_STANDARD})\n"
		<< "set(CMAKE_CXX_EXTENSIONS OFF)\n"
		<< "set(CMAKE_BUILD_TYPE ${USRDEF_CMAKE_BUILD_TYPE})\n\n"
		;

	ss << "set(CMAKE_OSX_SYSROOT ${USRDEF_CMAKE_OSX_SYSROOT})\n"
		<< "set(CMAKE_OSX_ARCHITECTURES ${USRDEF_CMAKE_OSX_ARCHITECTURES})\n\n"
		;

	ss << "string(FIND \"${USRDEF_CMAKE_OSX_ARCHITECTURES}\" \"arm64\" var_arm64_chk)\n"
		<< "if(var_arm64_chk EQUAL -1)\n"
		<< "\tset(USRDEF_HAS_ARM64 OFF)\n"
		<< "else()\n"
		<< "\tset(USRDEF_HAS_ARM64 ON)\n"
		<< "endif()\n\n"
		;

	ss << "string(FIND \"${USRDEF_CMAKE_OSX_ARCHITECTURES}\" \"x86_64\" var_x86_64_chk)\n"
		<< "if(var_x86_64_chk EQUAL -1)\n"
		<< "\tset(USRDEF_HAS_X86_64 OFF)\n"
		<< "else()\n"
		<< "\tset(USRDEF_HAS_X86_64 ON)\n"
		<< "endif()\n\n"
		;

	ss << "if(\"${USRDEF_CMAKE_OSX_SDK_NAME}\" STREQUAL \"macosx\")\n"
		<< "\t set(var_third_part_dir \"third_part\")\n"
		<< "else()\n"
		<< "\t set(var_third_part_dir \"third_part_${USRDEF_CMAKE_OSX_SDK_NAME}\")\n"
		<< "endif()\n\n"
		;

	// need to macos modify
	ss << "set(USRDEF_CMAKE_INCLUDE_DIRS \n"
		<< "\t\t~/${var_third_part_dir}/boost/include\n" 
		<< "\t\t~/${var_third_part_dir}/libbase64/include\n" 
		<< "\t\t~/${var_third_part_dir}/libblowfish/include\n"
		<< "\t\t~/${var_third_part_dir}/libmd5/include\n"
		<< "\t\t~/${var_third_part_dir}/libsha/include\n"
		//<< "\t\t~/${var_third_part_dir}/libiconv/include\n"
		<< "\t\t~/${var_third_part_dir}/libicu/include\n"
		<< "\t\t~/${var_third_part_dir}/libzlib/include\n" 
		<< "\t\t~/${var_third_part_dir}/libopenssl/include\n" 
		<< "\t\t~/${var_third_part_dir}/LzmaLib/include\n"
		//<< "\t\t~/${var_third_part_dir}/IntelRDFPMathLib/include\n"
		//<< "\t\t~/${var_third_part_dir}/IntelRDFPMathLib/include/src\n" 
		<< "\t\t~/${var_third_part_dir}/unixODBC/include\n" 
		<< "\t\t~/${var_third_part_dir}/libdtl/include\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb/include\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb_c_driver/include\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb_c_driver/include/bson\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb_c_driver/include/common\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb_c_driver/include/kms_message\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb_c_driver/include/mongo_crypt\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb_c_driver/include/mongoc\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb_c_driver/include/mongocrypt\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb_c_driver/include/mongocrypt/unicode\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb_c_driver/include/utf8proc\n" 
		<< "\t\t~/${var_third_part_dir}/lua/include\n" 
		<< "\t\t~/${var_third_part_dir}/luabind/include\n" 
		<< "\t\t~/${var_third_part_dir}/python/include/${USRDEF_CMAKE_PYTHON_INCLUDE_DIR}\n" 
		<< ")\n\n"
		;

	ss << "if(${USRDEF_HAS_X86_64})\n"
		<< "\tlist(APPEND USRDEF_CMAKE_INCLUDE_DIRS \n"
		<< "\t\t\t~/${var_third_part_dir}/IntelRDFPMathLib/include\n"
		<< "\t\t\t~/${var_third_part_dir}/IntelRDFPMathLib/include/src\n" 
		<< "\t)\n"
		<< "endif()\n\n"
    ;

	ss << "if((NOT(${CMAKE_SYSTEM_PROCESSOR} STREQUAL ${USRDEF_CMAKE_OSX_ARCHITECTURES}))\n"
		<< "\tOR (NOT(${USRDEF_CMAKE_OSX_SDK_NAME} STREQUAL \"macosx\")))\n"
		<< "\tlist(APPEND USRDEF_CMAKE_INCLUDE_DIRS \n"
		<< "\t\t\t~/${var_third_part_dir}/cyrus-sasl/include\n"
		<< "\t)\n"
		<< "endif()\n\n"
    ;

	ss << "if(${USRDEF_HAS_ARM64})\n"
		<< "\tset(USRDEF_CMAKE_DEFINEDS \"-DYGGR_TEST_DISABLE_NON_GENERIC_SUPPORT\")\n"
		<< "else()\n"
		<< "\tset(USRDEF_CMAKE_DEFINEDS \"\")\n"
		<< "endif()\n"
		<< "set(USRDEF_CMAKE_DEFINEDS_DEBUG \"\")\n"
		<< "set(USRDEF_CMAKE_DEFINEDS_RELEASE \"\")\n\n"
		;

	ss << "#set(USRDEF_CMAKE_CFLAGS \"\")\n"
		<< "set(USRDEF_CMAKE_CFLAGS_DEBUG \"\")\n"
		<< "set(USRDEF_CMAKE_CFLAGS_RELEASE \"\")\n\n"
		;

	ss << "set(USRDEF_CMAKE_CXXFLAGS \"\")\n"
		<< "set(USRDEF_CMAKE_CXXFLAGS_DEBUG \"\")\n"
		<< "set(USRDEF_CMAKE_CXXFLAGS_RELEASE \"\")\n\n"
		;

	ss << "set(USRDEF_CMAKE_LD_LIBRARY_PATH \n"
		<< "\t\t~/${var_third_part_dir}/boost/lib\n" 
		<< "\t\t~/${var_third_part_dir}/libbase64/lib\n" 
		<< "\t\t~/${var_third_part_dir}/libblowfish/lib\n"
		<< "\t\t~/${var_third_part_dir}/libmd5/lib\n"
		<< "\t\t~/${var_third_part_dir}/libsha/lib\n"
		//<< "\t\t~/${var_third_part_dir}/libiconv/lib\n"
		<< "\t\t~/${var_third_part_dir}/libicu/lib\n"
		<< "\t\t~/${var_third_part_dir}/libzlib/lib\n" 
		<< "\t\t~/${var_third_part_dir}/libopenssl/lib\n" 
		<< "\t\t~/${var_third_part_dir}/LzmaLib/lib\n"
		//<< "\t\t~/${var_third_part_dir}/IntelRDFPMathLib/lib\n" 
		<< "\t\t~/${var_third_part_dir}/unixODBC/lib\n" 
		<< "\t\t~/${var_third_part_dir}/libdtl/lib\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb/lib\n" 
		<< "\t\t~/${var_third_part_dir}/mongodb_c_driver/lib\n" 
		<< "\t\t~/${var_third_part_dir}/lua/lib\n" 
		<< "\t\t~/${var_third_part_dir}/luabind/lib\n" 
		<< "\t\t~/${var_third_part_dir}/python/lib\n" 
		<< ")\n\n"
		;

	ss << "if(${USRDEF_HAS_X86_64})\n"
		<< "\tlist(APPEND USRDEF_CMAKE_LD_LIBRARY_PATH \n"
		<< "\t\t\t~/${var_third_part_dir}/IntelRDFPMathLib/lib\n" 
		<< "\t)\n"
		<< "endif()\n\n"
    ;

	ss << "if((NOT(${CMAKE_SYSTEM_PROCESSOR} STREQUAL ${USRDEF_CMAKE_OSX_ARCHITECTURES}))\n"
		<< "\tOR (NOT(${USRDEF_CMAKE_OSX_SDK_NAME} STREQUAL \"macosx\")))\n"
		<< "\tlist(APPEND USRDEF_CMAKE_LD_LIBRARY_PATH \n"
		<< "\t\t\t~/${var_third_part_dir}/cyrus-sasl/lib\n" 
		<< "\t)\n"
		<< "endif()\n\n"
    ;

	ss << "set(USRDEF_CMAKE_LD_LIBRARY \"\")\n"
		<< "set(USRDEF_CMAKE_LD_LIBRARY_DEBUG \"\")\n"
		<< "set(USRDEF_CMAKE_LD_LIBRARY_RELEASE \"\")\n\n"
		;

	ss << "set(USRDEF_CMAKE_LDFLAGS \"\")\n"
		<< "set(USRDEF_CMAKE_LDFLAGS_DEBUG \"\")\n"
		<< "set(USRDEF_CMAKE_LDFLAGS_RELEASE \"\")\n\n"
		;

	ss << "set(USRDEF_CMAKE_PROJ_ROOT_DIR ${PROJECT_SOURCE_DIR})\n\n";

	ptree_string_set_type subdirs_a_sdk_patch, subdirs_so_sdk_patch, subdirs_exe_sdk_patch;

	if(subdirs_a.erase("yggr_script_python"))
	{
		subdirs_a_sdk_patch.insert("yggr_script_python");
	}

	if(subdirs_so.erase("pyd_export"))
	{
		subdirs_so_sdk_patch.insert("pyd_export");
	}

	ptree_string_set_type subdirs_fat_a, subdirs_fatfailed_a;
	arm64_fat_filter(subdirs_fat_a, subdirs_fatfailed_a, subdirs_a);

	ptree_string_set_type subdirs_fat_so, subdirs_fatfailed_so;
	arm64_fat_filter(subdirs_fat_so, subdirs_fatfailed_so, subdirs_so);

	ptree_string_set_type subdirs_fat_exe, subdirs_fatfailed_exe;
	arm64_fat_filter(subdirs_fat_exe, subdirs_fatfailed_exe, subdirs_exe);

	
	if(subdirs_so.size() || subdirs_exe.size())
	{
		if(subdirs_a.size())
		{
			if(subdirs_fatfailed_a.size())
			{
				ss << "if(${USRDEF_HAS_ARM64})\n"
					<< "\tset(var_proj_deps_a \n"
					<< make_cmake_l_multi_item(subdirs_fat_a, "\t\t\t")
					<< "\t)\n"
					<< "else()\n"
					<< "\tset(var_proj_deps_a \n"
					<< make_cmake_l_multi_item(subdirs_a, "\t\t\t")
					<< "\t)\n"
					<< "endif()\n\n"
					;
			}
			else
			{
				ss << "set(var_proj_deps_a \n"
					<<  make_cmake_l_multi_item(subdirs_a, "\t\t")
					<< ")\n\n"
					;
			}
		}

		if(subdirs_a_sdk_patch.size())
		{
			ss << "if(\"${USRDEF_CMAKE_OSX_SDK_NAME}\" STREQUAL \"macosx\")\n";
			if(subdirs_a.size())
			{
			
				ss << "\tlist(APPEND var_proj_deps_a \n" 
					<< make_cmake_l_multi_item(subdirs_a_sdk_patch, "\t\t")
					<< "\t)\n"
					;
			}
			else
			{
				ss << "\tset(var_proj_deps_a \n"
					<< make_cmake_l_multi_item(subdirs_a_sdk_patch, "\t\t")
					<< "\t)\n"
					;
			}
			ss << "endif()\n\n";
		}
	}

	if(subdirs_exe.size())
	{
		if(subdirs_so.size())
		{
			if(subdirs_fatfailed_so.size())
			{
				ss << "if(${USRDEF_HAS_ARM64})\n"
					<< "\tset(var_proj_deps_so \n"
					<< make_cmake_l_multi_item(subdirs_fat_so, "\t\t\t")
					<< "\t)\n"
					<< "else()\n"
					<< "\tset(var_proj_deps_so \n"
					<< make_cmake_l_multi_item(subdirs_so, "\t\t\t")
					<< "\t)\n"
					<< "endif()\n\n"
					;
			}
			else
			{
				ss << "set(var_proj_deps_so \n"
					<<  make_cmake_l_multi_item(subdirs_so, "\t\t")
					<< ")\n\n"
					;
			}
		}

		if(subdirs_so_sdk_patch.size())
		{
			ss << "if(\"${USRDEF_CMAKE_OSX_SDK_NAME}\" STREQUAL \"macosx\")\n";
			if(subdirs_so.size())
			{
			
				ss << "\tlist(APPEND var_proj_deps_so \n" 
					<< make_cmake_l_multi_item(subdirs_so_sdk_patch, "\t\t")
					<< "\t)\n"
					;
			}
			else
			{
				ss << "\tset(var_proj_deps_so \n"
					<< make_cmake_l_multi_item(subdirs_so_sdk_patch, "\t\t")
					<< "\t)\n"
					;
			}
			ss << "endif()\n\n";
		}
	}

	if(subdirs_a.size())
	{
		ss << "#add_subdirectory static libraries\n";

		if(subdirs_fatfailed_a.size())
		{
			{
				for(ptree_string_set_type::const_iterator i = subdirs_fat_a.begin(), isize = subdirs_fat_a.end();
						i != isize; ++i)
				{
					ss << "add_subdirectory(" << *i << ")\n";
				}
			}

			ss << "\n"
				<< "if(NOT ${USRDEF_HAS_ARM64})\n"
				;

			{
				for(ptree_string_set_type::const_iterator i = subdirs_fatfailed_a.begin(), isize = subdirs_fatfailed_a.end();
						i != isize; ++i)
				{
					ss << "\tadd_subdirectory(" << *i << ")\n";
				}
			}

			ss << "endif()\n";
		}
		else
		{
			for(ptree_string_set_type::const_iterator i = subdirs_a.begin(), isize = subdirs_a.end();
					i != isize; ++i)
			{
				ss << "add_subdirectory(" << *i << ")\n";
			}
		}

		ss << "\n";

		if(subdirs_a_sdk_patch.size())
		{
			ss << "if(\"${USRDEF_CMAKE_OSX_SDK_NAME}\" STREQUAL \"macosx\")\n";
			for(ptree_string_set_type::const_iterator i = subdirs_a_sdk_patch.begin(), isize = subdirs_a_sdk_patch.end();
					i != isize; ++i)
			{
				ss << "\tadd_subdirectory(" << *i << ")\n";
			}
			ss << "endif()\n\n";
		}
	}

	if(subdirs_so.size())
	{
		ss << "#add_subdirectory dynamic libraries\n";

		if(subdirs_fatfailed_so.size())
		{
			{
				for(ptree_string_set_type::const_iterator i = subdirs_fat_so.begin(), isize = subdirs_fat_so.end();
						i != isize; ++i)
				{
					ss << "add_subdirectory(" << *i << ")\n";
					if(subdirs_fat_a.size())
					{
						ss << "add_dependencies(" << *i << " ${var_proj_deps_a})\n\n";
					}
				}
			}

			ss << "\n"
				<< "if(NOT ${USRDEF_HAS_ARM64})\n"
				;

			{
				for(ptree_string_set_type::const_iterator i = subdirs_fatfailed_so.begin(), isize = subdirs_fatfailed_so.end();
						i != isize; ++i)
				{
					ss << "\tadd_subdirectory(" << *i << ")\n";
					if(subdirs_a.size())
					{
						ss << "\tadd_dependencies(" << *i << " ${var_proj_deps_a})\n";
					}
				}
			}

			ss << "endif()\n";
		}
		else
		{
			for(ptree_string_set_type::const_iterator i = subdirs_so.begin(), isize = subdirs_so.end();
				i != isize; ++i)
			{
				ss << "add_subdirectory(" << *i << ")\n";

				if(subdirs_a.size())
				{
					ss << "add_dependencies(" << *i << " ${var_proj_deps_a})\n\n";
				}
			}
		}

		ss << "\n";

		if(subdirs_so_sdk_patch.size())
		{
			ss << "if(\"${USRDEF_CMAKE_OSX_SDK_NAME}\" STREQUAL \"macosx\")\n";
			for(ptree_string_set_type::const_iterator i = subdirs_so_sdk_patch.begin(), isize = subdirs_so_sdk_patch.end();
					i != isize; ++i)
			{
				ss << "\tadd_subdirectory(" << *i << ")\n";
				if(subdirs_a.size())
				{
					ss << "\tadd_dependencies(" << *i << " ${var_proj_deps_a})\n\n";
				}
			}
			ss << "endif()\n\n";
		}
	}

	if(subdirs_exe.size())
	{
		ss << "#add_subdirectory executable\n";

		if(subdirs_fatfailed_exe.size())
		{
			{
				for(ptree_string_set_type::const_iterator i = subdirs_fat_exe.begin(), isize = subdirs_fat_exe.end();
						i != isize; ++i)
				{
					ss << "add_subdirectory(" << *i << ")\n";

					if(subdirs_fat_so.size())
					{
						ss << "add_dependencies(" << *i << " ${var_proj_deps_so})\n\n";
					}
					else if(subdirs_fat_a.size())
					{
						ss << "add_dependencies(" << *i << " ${var_proj_deps_a})\n\n";
					}
				}
			}

			ss << "\n"
				<< "if(NOT ${USRDEF_HAS_ARM64})\n"
				;

			{
				for(ptree_string_set_type::const_iterator i = subdirs_fatfailed_exe.begin(), isize = subdirs_fatfailed_exe.end();
						i != isize; ++i)
				{
					ss << "\tadd_subdirectory(" << *i << ")\n";
					if(subdirs_fat_so.size())
					{
						ss << "\tadd_dependencies(" << *i << " ${var_proj_deps_so})\n";
					}
					else if(subdirs_fat_a.size())
					{
						ss << "\tadd_dependencies(" << *i << " ${var_proj_deps_a})\n";
					}
				}
			}

			ss << "endif()\n";
		}
		else
		{
			for(ptree_string_set_type::const_iterator i = subdirs_exe.begin(), isize = subdirs_exe.end();
				i != isize; ++i)
			{
				ss << "add_subdirectory(" << *i << ")\n";

				if(subdirs_so.size())
				{
					ss << "add_dependencies(" << *i << " ${var_proj_deps_so})\n\n";
				}
				else if(subdirs_a.size())
				{
					ss << "add_dependencies(" << *i << " ${var_proj_deps_a})\n\n";
				}
			}
		}

		if(subdirs_exe_sdk_patch.size())
		{
			ss << "if(\"${USRDEF_CMAKE_OSX_SDK_NAME}\" STREQUAL \"macosx\")\n";
			for(ptree_string_set_type::const_iterator i = subdirs_exe_sdk_patch.begin(), isize = subdirs_exe_sdk_patch.end();
					i != isize; ++i)
			{
				ss << "\tadd_subdirectory(" << *i << ")\n";
				if(subdirs_so.size())
				{
					ss << "\tadd_dependencies(" << *i << " ${var_proj_deps_so})\n\n";
				}
				else if(subdirs_a.size())
				{
					ss << "\tadd_dependencies(" << *i << " ${var_proj_deps_a})\n\n";
				}
			}
			ss << "endif()\n\n";
		}
	}

	ss << "\n";

	std::string str = ss.str();
	ptree_string_type out_str(str.begin(), str.end());
	out.swap(out_str);
	return out;
}

bool fix_workspace_file_one_cmakelists_txt(const cb::cbws_infos& cbws_infos, 
											const ptree_string_type& ws_dir,
											const ptree_string_type& ws_fname,
											const file_list_type& fixed_cbs_flist_a,
											const file_list_type& fixed_cbs_flist_so,
											const file_list_type& fixed_cbs_flist_exe)
{
	yggr::utf8_string cmake_darwin_path(ws_dir + "/CMakeDarwin/" + cbws_infos.cbws_title_ + "/CMakeLists.txt", YGGR_STR_UTF8_STRING_CHARSET_NAME());
	
	ptree_string_type cbws_cmakelists_code;

	if(!(conv_to_cbws_cmakelists_txt(
			cbws_cmakelists_code, cbws_infos, 
			fixed_cbs_flist_a,
			fixed_cbs_flist_so,
			fixed_cbs_flist_exe).size()))
	{
		return false;
	}

#if ONLY_MAKE_RUN_TEST()
	std::cout << cmake_darwin_path << ": " << std::endl;
	std::cout << cbws_cmakelists_code << std::endl;
#else
	file_op::write_to_file(cmake_darwin_path, cbws_cmakelists_code);
#endif // ONLY_MAKE_RUN_TEST

	return true;
}

yggr::utf8_string& gen_cmake_build_tpl_sh(yggr::utf8_string& out_str)
{
	std::stringstream ss;

	ss << "#!/bin/bash\n\n"

		<< "var_usr_cmake_cc=$1\n"
		<< "var_usr_cmake_cxx=$2\n"
		<< "var_usr_cmake_compiler_version=$3\n\n"

		<< "var_usr_cmake_c_standard=$4\n"
		<< "var_usr_cmake_cxx_standard=$5\n\n"

		<< "var_usr_cmake_ld_arch_tag=$6\n"
		<< "var_usr_cmake_arch_bits=$7\n"
		<< "var_usr_cmake_build_type=$8\n"
		<< "var_usr_cmake_mid_dir=$9\n\n"

		<< "shift\nvar_usr_cmake_target=$9\n\n"

		<< "shift\nvar_darwin_sdk_name=$9\n"
		<< "shift\nvar_darwin_sdk_ver_min=$9\n"
		<< "shift\nvar_darwin_arch=$9\n\n"


		<< "if [ \"${var_usr_cmake_cc}\" = \"default\" ]; then\n"
		<< "\tvar_usr_cmake_cc=\"\"\n"
		<< "fi\n\n"

		<< "if [ \"${var_usr_cmake_cxx}\" = \"default\" ]; then\n"
		<< "\tvar_usr_cmake_cxx=\"\"\n"
		<< "fi\n\n"

		<< "if [ ! \"${var_usr_cmake_compiler_version}\" = \"\" ]; then\n"
		<< "\tvar_usr_cmake_compiler_version_tag=-${var_usr_cmake_compiler_version}\n"
		<< "else\n"
		<< "\tvar_usr_cmake_compiler_version_tag=\"\"\n"
		<< "fi\n\n"

		<< "var_usrdef_cmake_osx_sysroot=$(xcrun --sdk ${var_darwin_sdk_name} --show-sdk-path)\n\n"

		<< "if [ \"${var_darwin_sdk_name}\" = \"iphoneos\" ]; then\n"
		<< "\tvar_darwin_arch=\"arm64\"\n"
		<< "\tvar_usrdef_cflag=\"-miphoneos-version-min=${var_darwin_sdk_ver_min}\"\n"
		<< "else\n"
		<< "\tif [ \"${var_darwin_arch}\" = \"\" ] || [ \"${var_darwin_arch}\" = \"all\" ]; then\n"
		<< "\t\tvar_darwin_arch=\"x86_64 -arch arm64\"\n"
		<< "\tfi\n\n"
		<< "\tif [ \"${var_darwin_sdk_name}\" = \"macosx\" ]; then\n"
		<< "\t\tvar_usrdef_cflag=\"-mmacosx-version-min=${var_darwin_sdk_ver_min}\"\n"
		<< "\telse\n"
		<< "\t\tvar_usrdef_cflag=\"\"\n"
		<< "\tfi\n"
		<< "fi\n\n"

		<< "var_usrdef_cmake_osx_architectures=\"${var_darwin_arch}\"\n\n"

		<< "if [[ \"${tvar_darwin_arch}\" =~ \"arm64\" ]]; then \n"
		<< "\tvar_usr_cmake_arch_tag=a\n"
		<< "else\n"
		<< "\tvar_usr_cmake_arch_tag=x\n"
		<< "fi\n\n"

		<< "var_usr_cmake_arch_bits=64\n"

		<< "var_usr_cmake_arch_bits_tag=-${var_usr_cmake_arch_tag}${var_usr_cmake_arch_bits}\n\n"

		<< "if [ ! \"${var_usr_cmake_build_type}\" = \"Debug\" ]; then\n"
		<< "\tvar_usr_cmake_build_type=\"Release\"\n"
		<< "fi\n\n"

		<< "var_usr_cmake_bin_root_dir=${var_usr_cmake_build_type}${var_usr_cmake_arch_bits_tag}\n"
		<< "var_usr_cmake_bin_outdir=${var_usr_cmake_bin_root_dir}/${var_usr_cmake_compiler_version}\n\n"

		<< "# macos clang not support 32 bit\n\n"

		<< "var_usr_cmake_lib_outdir=lib\n\n"

		<< "if [ -d \"${var_usr_cmake_mid_dir}\" ]; then\n"
		<< "\trm -fr \"${var_usr_cmake_mid_dir}\"\n"
		<< "fi\n\n"

		<< "mkdir -p \"${var_usr_cmake_mid_dir}\"\n\n"

		<< "cd ${var_usr_cmake_mid_dir}\n\n"

		<< "cmake \\\n"
		<< "\t-G ${var_usr_cmake_target} \\\n"
		<< "\t.. \\\n"
		<< "\t-DUSRDEF_CMAKE_C_COMPILER=${var_usr_cmake_cc} \\\n"
		<< "\t-DUSRDEF_CMAKE_CXX_COMPILER=${var_usr_cmake_cxx} \\\n"
		<< "\t-DUSRDEF_CMAKE_COMPILER_VERSION=${var_usr_cmake_compiler_version} \\\n"
		<< "\t-DUSRDEF_CMAKE_BIN_OUTDIR=${var_usr_cmake_bin_outdir} \\\n"
		<< "\t-DUSRDEF_CMAKE_LIB_OUTDIR=${var_usr_cmake_lib_outdir} \\\n"
		<< "\t-DUSRDEF_CMAKE_C_STANDARD=${var_usr_cmake_c_standard} \\\n"
		<< "\t-DUSRDEF_CMAKE_CXX_STANDARD=${var_usr_cmake_cxx_standard} \\\n"
		<< "\t-DUSRDEF_CMAKE_BUILD_TYPE=${var_usr_cmake_build_type} \\\n"
		<< "\t-DUSRDEF_CMAKE_BUILD_LD_ARCH_TAG=${var_usr_cmake_ld_arch_tag} \\\n"
		<< "\t-DUSRDEF_CMAKE_BUILD_ARCH_BITS=${var_usr_cmake_arch_bits} \\\n"
		<< "\t-DUSRDEF_CMAKE_OSX_SDK_NAME=\"${var_darwin_sdk_name}\" \\\n"
		<< "\t-DUSRDEF_CMAKE_OSX_SYSROOT=\"${var_usrdef_cmake_osx_sysroot}\" \\\n"
		<< "\t-DUSRDEF_CMAKE_OSX_ARCHITECTURES=\"${var_usrdef_cmake_osx_architectures}\" \\\n"
		<< "\t-DUSRDEF_CMAKE_CFLAGS=\"${var_usrdef_cflag}\"\n\n"

		<< "if [ \"${var_usr_cmake_target}\" = \"Ninja\" ]; then\n"
		<< "\tninja -v\n"
		<< "fi\n\n"
		
		<< "cd ..\n\n"

		<< "#if [ -d \"${var_usr_cmake_mid_dir}\" ]; then\n"
		<< "#    rm -fr \"${var_usr_cmake_mid_dir}\"\n"
		<< "#fi\n"
		;

	out_str = yggr::charset::make_string_charset_helper(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return out_str;
}

yggr::utf8_string& gen_cmake_build_tpl_call_sh(yggr::utf8_string& out_str)
{
	std::stringstream ss;

	ss << "#!/bin/bash\n\n"
		
		<< "var_usr_cmake_targer=$1\n"
		<< "var_darwin_sdk_name=$2\n"
		<< "var_darwin_sdk_ver_min=$3\n"
		<< "var_darwin_arch=$4\n"
		<< "var_usr_cmake_c_standard=$5\n"
		<< "var_usr_cmake_cxx_standard=$6\n"
		<< "var_usr_cmake_ld_arch_tag=$7\n"
		<< "var_clear_other_build_lib=$8\n"
		<< "var_clear_other_build_bin=$9\n\n"

		<< "var_usr_cmake_cc=default\n"
		<< "var_usr_cmake_cxx=default\n\n"

		<< "#var_usr_cmake_cc=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang\n"
		<< "#var_usr_cmake_cxx=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++\n\n"

		<< "var_usr_cmake_compiler_version=clang-darwin16\n\n"

		<< "#var_usr_cmake_c_standard=" << "cstd_ver" << "\n"
		<< "#var_usr_cmake_cxx_standard=" << "cppstd_ver" << "\n\n"
		<< "#var_usr_cmake_targer=" << "target" << "\n\n"

		<< "#var_darwin_sdk_name=" << "sdk_name" << "\n"
		<< "#var_darwin_sdk_ver_min=" << "min_ver" << "\n\n"

		<< "var_usr_cmake_lib_root_dir=lib\n\n"
		;
		
	ss << "if [ \"${var_darwin_sdk_name}\" = \"iphoneos\" ]; then\n"
		<< "\tvar_darwin_arch=\"arm64\"\n"
		<< "else\n"
		<< "\tif [ \"${var_darwin_arch}\" = \"\" ] || [ \"${var_darwin_arch}\" = \"all\" ]; then\n"
		<< "\t\tvar_darwin_arch=\"x86_64 -arch arm64\"\n"
		<< "\tfi\n\n"
		<< "fi\n\n"
		;

	ss << "if [[ \"${var_darwin_arch}\" =~ \"arm64\" ]]; then\n"
		<< "\tvar_darwin_arch_tag=a\n"
		<< "else\n"
		<< "\tvar_darwin_arch_tag=x\n"
		<< "fi\n\n"
		;

	ss << "# 64bit debug release\n"
		<< "#var_usr_cmake_ld_arch_tag=x\n"
		<< "var_usr_cmake_arch_bits=64\n"
		<< "var_usr_cmake_build_type=Debug\n\n"
		;
		
	ss << "if [[ \"${var_darwin_arch}\" =~ \"-arch\" ]] || [ \"${var_darwin_sdk_name}\" = \"iphoneos\" ]; then\n"
		<< "\tvar_usr_cmake_mid_dir=cmake_build_cache-${var_darwin_sdk_name}-${var_usr_cmake_targer}-${var_usr_cmake_build_type}\n"
		<< "else\n"
		<< "\tvar_usr_cmake_mid_dir=cmake_build_cache-${var_darwin_sdk_name}-${var_darwin_arch}-${var_usr_cmake_targer}-${var_usr_cmake_build_type}\n"
		<< "fi \n\n"
		;


	ss << "var_usr_cmake_bin_root_dir=${var_usr_cmake_build_type}-${var_darwin_arch_tag}${var_usr_cmake_arch_bits}\n\n"
		;


	ss << "if [ \"${var_clear_other_build_lib}\" = \"yes\" ] && [ \"${var_usr_cmake_targer}\" = \"Ninja\" ]; then\n"
		<< "\tif [ -d \"../../${var_usr_cmake_lib_root_dir}\" ]; then\n"
		<< "\t\trm -fr \"../../${var_usr_cmake_lib_root_dir}\"\n"
		<< "\t\tvar_clear_other_build_bin=\"yes\"\n"
		<< "\tfi\n"
		<< "fi\n\n"
		;

	ss << "if [ \"${var_clear_other_build_bin}\" = \"yes\" ] && [ \"${var_usr_cmake_targer}\" = \"Ninja\" ]; then\n"
		<< "\tif [ -d \"../../${var_usr_cmake_bin_root_dir}\" ]; then\n"
		<< "\t\trm -fr \"../../${var_usr_cmake_bin_root_dir}\"\n"
		<< "\tfi\n"
		<< "fi\n\n"
		;

	ss << "sh cmake-build-tpl.sh \\\n"
		<< "\t${var_usr_cmake_cc} \\\n"
		<< "\t${var_usr_cmake_cxx} \\\n"
		<< "\t${var_usr_cmake_compiler_version} \\\n"
		<< "\t${var_usr_cmake_c_standard} \\\n"
		<< "\t${var_usr_cmake_cxx_standard} \\\n"
		<< "\t${var_usr_cmake_ld_arch_tag} \\\n"
		<< "\t${var_usr_cmake_arch_bits} \\\n"
		<< "\t${var_usr_cmake_build_type} \\\n"
		<< "\t${var_usr_cmake_mid_dir} \\\n"
		<< "\t${var_usr_cmake_targer} \\\n"
		<< "\t${var_darwin_sdk_name} \\\n"
		<< "\t${var_darwin_sdk_ver_min} \\\n"
		<< "\t\"${var_darwin_arch}\"\n\n"
		;

	ss << "var_usr_cmake_build_type=Release\n";

	ss << "if [[ \"${var_darwin_arch}\" =~ \"-arch\" ]] || [ \"${var_darwin_sdk_name}\" = \"iphoneos\" ]; then\n"
		<< "\tvar_usr_cmake_mid_dir=cmake_build_cache-${var_darwin_sdk_name}-${var_usr_cmake_targer}-${var_usr_cmake_build_type}\n"
		<< "else\n"
		<< "\tvar_usr_cmake_mid_dir=cmake_build_cache-${var_darwin_sdk_name}-${var_darwin_arch}-${var_usr_cmake_targer}-${var_usr_cmake_build_type}\n"
		<< "fi \n\n"
		;

	ss << "var_usr_cmake_bin_root_dir=${var_usr_cmake_build_type}-${var_darwin_arch_tag}${var_usr_cmake_arch_bits}\n\n"
		;

	ss << "if [ \"${var_clear_other_build_bin}\" = \"yes\" ] && [ \"${var_usr_cmake_targer}\" = \"Ninja\" ]; then\n"
		<< "\tif [ -d \"../../${var_usr_cmake_bin_root_dir}\" ]; then\n"
		<< "\t\trm -fr \"../../${var_usr_cmake_bin_root_dir}\"\n"
		<< "\tfi\n"
		<< "fi\n\n"
		;

	ss << "sh cmake-build-tpl.sh \\\n"
		<< "\t${var_usr_cmake_cc} \\\n"
		<< "\t${var_usr_cmake_cxx} \\\n"
		<< "\t${var_usr_cmake_compiler_version} \\\n"
		<< "\t${var_usr_cmake_c_standard} \\\n"
		<< "\t${var_usr_cmake_cxx_standard} \\\n"
		<< "\t${var_usr_cmake_ld_arch_tag} \\\n"
		<< "\t${var_usr_cmake_arch_bits} \\\n"
		<< "\t${var_usr_cmake_build_type} \\\n"
		<< "\t${var_usr_cmake_mid_dir} \\\n"
		<< "\t${var_usr_cmake_targer} \\\n"
		<< "\t${var_darwin_sdk_name} \\\n"
		<< "\t${var_darwin_sdk_ver_min} \\\n"
		<< "\t\"${var_darwin_arch}\"\n\n"
		;

	ss << "# xcode10 and later not support 32bit abi\n"
		;

	out_str = yggr::charset::make_string_charset_helper(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return out_str;
}

yggr::utf8_string& gen_cmake_build_sh(yggr::utf8_string& out_str, 
										const cb::cbws_infos& cbws_infos,
										const yggr::string& target,
										const yggr::string& sdk_name,
										const yggr::string& sdk_min_ver,
										const yggr::string& arch,
										const yggr::string& ld_arg_tag,
										yggr::s32 cstd_ver,
										yggr::s32 cppstd_ver,
										bool clear_other_build_lib,
										bool clear_other_build_bin)
{
	std::stringstream ss;

	ss << "#!/bin/bash\n\n";

	ss << "var_usr_cmake_targer=" << target << "\n"
		<< "var_darwin_sdk_name=" << sdk_name << "\n"
		<< "var_darwin_sdk_ver_min=" << sdk_min_ver << "\n"
		<< "var_darwin_arch=\"" << arch << "\"\n"
		<< "var_usr_cmake_ld_arch_tag="<< ld_arg_tag << "\n"
		<< "var_usr_cmake_c_standard=" << cstd_ver << "\n"
		<< "var_usr_cmake_cxx_standard=" << cppstd_ver << "\n"
		<< "var_clear_other_build_lib=" << (clear_other_build_lib? "yes" : "no") << "\n"
		<< "var_clear_other_build_bin=" << (clear_other_build_bin? "yes" : "no") << "\n\n" 
		;

	ss << "sh cmake-build-tpl_call.sh \\\n"
		<< "\t${var_usr_cmake_targer} \\\n"
		<< "\t${var_darwin_sdk_name} \\\n"
		<< "\t${var_darwin_sdk_ver_min} \\\n"
		<< "\t\"${var_darwin_arch}\" \\\n"
		<< "\t${var_usr_cmake_c_standard} \\\n"
		<< "\t${var_usr_cmake_cxx_standard} \\\n"
		<< "\t${var_usr_cmake_ld_arch_tag} \\\n"
		<< "\t${var_clear_other_build_lib} \\\n"
		<< "\t${var_clear_other_build_bin}\n\n"
		;

	out_str = yggr::charset::make_string_charset_helper(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return out_str;
}

yggr::utf8_string& gen_cmake_darwin_build_and_collection_sh(yggr::utf8_string& out_str, 
															const cb::cbws_infos& cbws_infos,
															const yggr::string& cpp_ver)
{
	std::stringstream ss;

	ss << "#!/bin/bash\n\n"

		<< "var_local_dir=$(cd `dirname $0`; pwd)\n\n"
		;

	ss << "collection_foo(){ \n"
		<< "\tvar_sln_name=$1\n"
		<< "\tvar_sdk_name=$2\n"
		<< "\tvar_clang_tag=$3\n"
		<< "\tvar_cpp_ver=$4\n"
		<< "\tvar_src_root_dir=$5\n"
		<< "\tvar_prefix_stage_dir=$6\n\n"

		<< "#\techo \"var_sln_name = ${var_sln_name}\"\n"
		<< "#\techo \"var_sdk_name = ${var_sdk_name}\"\n"
		<< "#\techo \"var_clang_tag = ${var_clang_tag}\"\n"
		<< "#\techo \"var_cpp_ver = ${var_cpp_ver}\"\n"
		<< "#\techo \"var_src_root_dir = ${var_src_root_dir}\"\n"
		<< "#\techo \"var_prefix_stage_dir = ${var_prefix_stage_dir}\"\n\n"

		<< "\tvar_sh_file=\"cmake-build-${var_sdk_name}-${var_clang_tag}-${var_cpp_ver}.sh\"\n\n"

		<< "\tvar_src_bin_dir=\"${var_src_root_dir}/Release-x64/${var_clang_tag}\"\n"
		<< "\tvar_src_lib_dir=\"${var_src_root_dir}/lib\"\n"
		<< "\tvar_src_inc_dir=\"${var_src_root_dir}/include\"\n\n"

		<< "\tvar_prefix_sln_dir=\"${var_prefix_stage_dir}/${var_sln_name}-${var_sdk_name}\"\n"
		<< "\tvar_prefix_sln_bin_dir=\"${var_prefix_sln_dir}/bin\"\n"
		<< "\tvar_prefix_sln_lib_dir=\"${var_prefix_sln_dir}/lib\"\n"
		<< "\tvar_prefix_sln_inc_dir=\"${var_prefix_sln_dir}/include\"\n\n"

		<< "\tif [ -d \"${var_prefix_sln_dir}\" ]; then\n"
		<< "\t\trm -fr \"${var_prefix_sln_dir}\"\n"
		<< "\tfi\n\n"

		<< "\tif [ -d \"${var_src_inc_dir}\" ]; then\n"
		<< "\t\tmkdir -p \"${var_prefix_sln_dir}\"\n"
		<< "\telse\n"
		<< "\t\tmkdir -p \"${var_prefix_sln_inc_dir}\"\n"
		<< "\tfi\n\n"

		<< "#\techo \"sh ${var_sh_file}\"\n"
		<< "\tsh \"${var_sh_file}\"\n\n"
			
		<< "\tif [ -d \"${var_src_bin_dir}\" ]; then\n"
		<< "\t\tcp -fr \"${var_src_bin_dir}\" \"${var_prefix_sln_bin_dir}\"\n"
		<< "\tfi\n\n"

		<< "\tif [ -d \"${var_src_lib_dir}\" ]; then\n"
		<< "\t\tcp -fr \"${var_src_lib_dir}\" \"${var_prefix_sln_dir}/\"\n"
		<< "\tfi\n\n"

		<< "\tif [ -d \"${var_src_inc_dir}\" ]; then\n"
		<< "\t\tcp -fr \"${var_src_inc_dir}\" \"${var_prefix_sln_dir}/\"\n" 
		<< "\tfi\n"
		<< "}\n\n"
		;

	ss << "var_sln_name=" << cbws_infos.cbws_title_ << "\n"
		<< "var_clang_tag=\"clang-darwin16\"\n"
		<< "var_cpp_ver=\"" << cpp_ver << "\"\n\n"

		<< "var_sln_dir=\"${var_local_dir}/../..\"\n"
		<< "var_prefix_stage_dir=\"${var_sln_dir}/stage_prefix\"\n\n"

		<< "var_sdk_name=\"macosx\"\n"

		<< "collection_foo \\\n"
		<< "\t\"${var_sln_name}\" \\\n"
		<< "\t\"${var_sdk_name}\" \\\n"
		<< "\t\"${var_clang_tag}\" \\\n"
		<< "\t\"${var_cpp_ver}\" \\\n"
		<< "\t\"${var_sln_dir}\" \\\n"
		<< "\t\"${var_prefix_stage_dir}\" \n\n"

		<< "var_sdk_name=\"iphoneos\"\n\n"

		<< "collection_foo \\\n"
		<< "\t\"${var_sln_name}\" \\\n"
		<< "\t\"${var_sdk_name}\" \\\n"
		<< "\t\"${var_clang_tag}\" \\\n"
		<< "\t\"${var_cpp_ver}\" \\\n"
		<< "\t\"${var_sln_dir}\" \\\n"
		<< "\t\"${var_prefix_stage_dir}\" \n\n"

		<< "var_sdk_name=\"iphonesimulator\"\n\n"

		<< "collection_foo \\\n"
		<< "\t\"${var_sln_name}\" \\\n"
		<< "\t\"${var_sdk_name}\" \\\n"
		<< "\t\"${var_clang_tag}\" \\\n"
		<< "\t\"${var_cpp_ver}\" \\\n"
		<< "\t\"${var_sln_dir}\" \\\n"
		<< "\t\"${var_prefix_stage_dir}\" \n\n"
		;

	out_str = yggr::charset::make_string_charset_helper(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return out_str;
}

yggr::utf8_string& gen_cmake_darwin_build_and_collection_lipo_sh(yggr::utf8_string& out_str, 
																	const cb::cbws_infos& cbws_infos,
																	const yggr::string& cpp_ver)
{
	std::stringstream ss;

	ss << "#!/bin/bash\n\n"

		<< "var_local_dir=$(cd `dirname $0`; pwd)\n\n"
		;

	ss << "collection_foo(){ \n"
		<< "\tvar_sln_name=$1\n"
		<< "\tvar_sdk_name=$2\n"
		<< "\tvar_clang_tag=$3\n"
		<< "\tvar_cpp_ver=$4\n"
		<< "\tvar_src_root_dir=$5\n"
		<< "\tvar_prefix_stage_dir=$6\n\n"

		<< "#\techo \"var_sln_name = ${var_sln_name}\"\n"
		<< "#\techo \"var_sdk_name = ${var_sdk_name}\"\n"
		<< "#\techo \"var_clang_tag = ${var_clang_tag}\"\n"
		<< "#\techo \"var_cpp_ver = ${var_cpp_ver}\"\n"
		<< "#\techo \"var_src_root_dir = ${var_src_root_dir}\"\n"
		<< "#\techo \"var_prefix_stage_dir = ${var_prefix_stage_dir}\"\n\n"

		<< "\tvar_src_bin_dir=\"${var_src_root_dir}/Release-x64/${var_clang_tag}\"\n"
		<< "\tvar_src_lib_dir=\"${var_src_root_dir}/lib\"\n"
		<< "\tvar_src_inc_dir=\"${var_src_root_dir}/include\"\n\n"

		<< "\tvar_prefix_sln_dir=\"${var_prefix_stage_dir}/${var_sln_name}-${var_sdk_name}\"\n"
		<< "\tvar_prefix_sln_bin_dir=\"${var_prefix_sln_dir}/bin\"\n"
		<< "\tvar_prefix_sln_lib_dir=\"${var_prefix_sln_dir}/lib\"\n"
		<< "\tvar_prefix_sln_inc_dir=\"${var_prefix_sln_dir}/include\"\n\n"

		<< "\tif [ -d \"${var_prefix_sln_dir}\" ]; then\n"
		<< "\t\trm -fr \"${var_prefix_sln_dir}\"\n"
		<< "\tfi\n\n"

		<< "\tif [ -d \"${var_src_inc_dir}\" ]; then\n"
		<< "\t\tmkdir -p \"${var_prefix_sln_dir}\"\n"
		<< "\telse\n"
		<< "\t\tmkdir -p \"${var_prefix_sln_inc_dir}\"\n"
		<< "\tfi\n\n"

		<< "\tif [ \"${var_sdk_name}\" = \"iphoneos\" ]; then\n"
		<< "\t\tvar_sh_file=\"cmake-build-${var_sdk_name}-${var_clang_tag}-${var_cpp_ver}.sh\"\n"
		<< "\t\t#	echo \"sh ${var_sh_file}\"\n"
		<< "\t\tsh \"${var_sh_file}\"\n\n"

		<< "\t\tif [ -d \"${var_src_bin_dir}\" ]; then\n"
		<< "\t\t\tcp -fr \"${var_src_bin_dir}\" \"${var_prefix_sln_bin_dir}\"\n"
		<< "\t\tfi\n\n"

		<< "\t\tif [ -d \"${var_src_lib_dir}\" ]; then\n"
		<< "\t\t\tcp -fr \"${var_src_lib_dir}\" \"${var_prefix_sln_dir}/\"\n"
		<< "\t\tfi\n"
		<< "\telse\n"
		<< "\t\tmkdir -p \"${var_prefix_sln_bin_dir}\"\n"
		<< "\t\tmkdir -p \"${var_prefix_sln_lib_dir}\"\n\n"

		<< "\t\t#x86_64\n"
		<< "\t\tvar_prefix_sln_bin_dir_x86_64=\"${var_prefix_sln_bin_dir}/x86_64\"\n"
		<< "\t\tvar_prefix_sln_lib_dir_x86_64=\"${var_prefix_sln_lib_dir}/x86_64\"\n\n"

		<< "\t\tvar_sh_file_x86_64=\"cmake-build-${var_sdk_name}-${var_clang_tag}-${var_cpp_ver}-x86_64.sh\"\n"
		<< "\t\t#	echo \"sh ${var_sh_file}\"\n"
		<< "\t\tsh \"${var_sh_file_x86_64}\"\n\n"

		<< "\t\tif [ -d \"${var_src_bin_dir}\" ]; then\n"
		<< "\t\t\tcp -fr \"${var_src_bin_dir}\" \"${var_prefix_sln_bin_dir_x86_64}\"\n"
		<< "\t\tfi\n\n"

		<< "\t\tif [ -d \"${var_src_lib_dir}\" ]; then\n"
		<< "\t\t\tcp -fr \"${var_src_lib_dir}\" \"${var_prefix_sln_lib_dir_x86_64}/\"\n"
		<< "\t\tfi\n\n"

		<< "\t\t#arm64\n"
		<< "\t\tvar_prefix_sln_bin_dir_arm64=\"${var_prefix_sln_bin_dir}/arm64\"\n"
		<< "\t\tvar_prefix_sln_lib_dir_arm64=\"${var_prefix_sln_lib_dir}/arm64\"\n\n"

		<< "\t\tvar_sh_file_arm64=\"cmake-build-${var_sdk_name}-${var_clang_tag}-${var_cpp_ver}-arm64.sh\"\n"
		<< "\t\t#	echo \"sh ${var_sh_file}\"\n"
		<< "\t\tsh \"${var_sh_file_arm64}\"\n\n"

		<< "\t\tif [ -d \"${var_src_bin_dir}\" ]; then\n"
		<< "\t\t\tcp -fr \"${var_src_bin_dir}\" \"${var_prefix_sln_bin_dir_arm64}\"\n"
		<< "\t\tfi\n\n"

		<< "\t\tif [ -d \"${var_src_lib_dir}\" ]; then\n"
		<< "\t\t\tcp -fr \"${var_src_lib_dir}\" \"${var_prefix_sln_lib_dir_arm64}/\"\n"
		<< "\t\tfi\n\n"

		<< "\t\tvar_x86_exe_list=$(find ${var_prefix_sln_bin_dir_x86_64} -type f -perm +555)\n"
		<< "\t\techo \"${var_x86_exe_list}\"\n\n"

		<< "\t\tfor var_fpath in ${var_x86_exe_list}\n"
		<< "\t\tdo\n"
		<< "\t\t\tvar_fname=${var_fpath##*/}\n\n"
			
		<< "\t\t\tif [ -f \"${var_prefix_sln_bin_dir_arm64}/${var_fname}\" ]; then\n"
		<< "\t\t\t\tvar_cmd_create=\"lipo -create ${var_fpath} ${var_prefix_sln_bin_dir_arm64}/${var_fname} -output ${var_prefix_sln_bin_dir}/${var_fname}\"\n"
		<< "\t\t\t\tvar_cmd_check=\"lipo -info ${var_prefix_sln_bin_dir}/${var_fname}\"\n"
		<< "\t\t\t\techo \"${var_cmd_create}\"\n"
		<< "\t\t\t\teval ${var_cmd_create}\n"
		<< "\t\t\t\teval ${var_cmd_check}\n"
		<< "\t\t\tfi\n"
		<< "\t\tdone\n\n"

		<< "\t\tvar_x86_so_list=$(find ${var_prefix_sln_lib_dir_x86_64} -type f -name '*.so')\n"
		<< "\t\techo \"${var_x86_so_list}\"\n\n"

		<< "\t\tfor var_fpath in ${var_x86_so_list}\n"
		<< "\t\tdo\n"
		<< "\t\tvar_fname=${var_fpath##*/}\n\n"
			
		<< "\t\t\tif [ -f \"${var_prefix_sln_lib_dir_arm64}/${var_fname}\" ]; then\n"
		<< "\t\t\t\tvar_cmd_create=\"lipo -create ${var_fpath} ${var_prefix_sln_lib_dir_arm64}/${var_fname} -output ${var_prefix_sln_lib_dir}/${var_fname}\"\n"
		<< "\t\t\t\tvar_cmd_check=\"lipo -info ${var_prefix_sln_lib_dir}/${var_fname}\"\n"
		<< "\t\t\t\techo \"${var_cmd_create}\"\n"
		<< "\t\t\t\teval ${var_cmd_create}\n"
		<< "\t\t\t\teval ${var_cmd_check}\n"
		<< "\t\t\tfi\n"
		<< "\t\tdone\n\n"

		<< "\t\tvar_x86_dylib_list=$(find ${var_prefix_sln_lib_dir_x86_64} -type f -name '*.dylib')\n"
		<< "\t\techo \"${var_x86_dylib_list}\"\n\n"

		<< "\t\tfor var_fpath in ${var_x86_dylib_list}\n" 
		<< "\t\tdo\n"
		<< "\t\tvar_fname=${var_fpath##*/}\n\n"
			
		<< "\t\t\tif [ -f \"${var_prefix_sln_lib_dir_arm64}/${var_fname}\" ]; then\n"
		<< "\t\t\t\tvar_cmd_create=\"lipo -create ${var_fpath} ${var_prefix_sln_lib_dir_arm64}/${var_fname} -output ${var_prefix_sln_lib_dir}/${var_fname}\"\n"
		<< "\t\t\t\tvar_cmd_check=\"lipo -info ${var_prefix_sln_lib_dir}/${var_fname}\"\n"
		<< "\t\t\t\techo \"${var_cmd_create}\"\n"
		<< "\t\t\t\teval ${var_cmd_create}\n"
		<< "\t\t\t\teval ${var_cmd_check}\n"
		<< "\t\t\tfi\n"
		<< "\t\tdone\n\n"

		<< "\t\tvar_x86_a_list=$(find ${var_prefix_sln_lib_dir_x86_64} -type f -name '*.a')\n"
		<< "\t\techo \"${var_x86_a_list}\"\n\n"

		<< "\t\tfor var_fpath in ${var_x86_a_list}\n" 
		<< "\t\tdo\n"
		<< "\t\t\tvar_fname=${var_fpath##*/}\n\n"
			
		<< "\t\t\tif [ -f \"${var_prefix_sln_lib_dir_arm64}/${var_fname}\" ]; then\n"
		<< "\t\t\t\tvar_cmd_create=\"lipo -create ${var_fpath} ${var_prefix_sln_lib_dir_arm64}/${var_fname} -output ${var_prefix_sln_lib_dir}/${var_fname}\"\n"
		<< "\t\t\t\tvar_cmd_check=\"lipo -info ${var_prefix_sln_lib_dir}/${var_fname}\"\n"
		<< "\t\t\t\techo \"${var_cmd_create}\"\n"
		<< "\t\t\t\teval ${var_cmd_create}\n"
		<< "\t\t\t\teval ${var_cmd_check}\n"
		<< "\t\t\tfi\n"
		<< "\t\tdone\n"
		<< "\tfi\n\n"

		<< "\tif [ -d \"${var_src_inc_dir}\" ]; then\n"
		<< "\t\tcp -fr \"${var_src_inc_dir}\" \"${var_prefix_sln_dir}/\"\n"
		<< "\tfi\n"
		<< "}\n\n"
		;

	ss << "var_sln_name=" << cbws_infos.cbws_title_ << "\n"
		<< "var_clang_tag=\"clang-darwin16\"\n"
		<< "var_cpp_ver=\"" << cpp_ver << "\"\n\n"

		<< "var_sln_dir=\"${var_local_dir}/../..\"\n"
		<< "var_prefix_stage_dir=\"${var_sln_dir}/stage_prefix\"\n\n"

		<< "var_sdk_name=\"macosx\"\n"

		<< "collection_foo \\\n"
		<< "\t\"${var_sln_name}\" \\\n"
		<< "\t\"${var_sdk_name}\" \\\n"
		<< "\t\"${var_clang_tag}\" \\\n"
		<< "\t\"${var_cpp_ver}\" \\\n"
		<< "\t\"${var_sln_dir}\" \\\n"
		<< "\t\"${var_prefix_stage_dir}\" \n\n"

		<< "var_sdk_name=\"iphoneos\"\n\n"

		<< "collection_foo \\\n"
		<< "\t\"${var_sln_name}\" \\\n"
		<< "\t\"${var_sdk_name}\" \\\n"
		<< "\t\"${var_clang_tag}\" \\\n"
		<< "\t\"${var_cpp_ver}\" \\\n"
		<< "\t\"${var_sln_dir}\" \\\n"
		<< "\t\"${var_prefix_stage_dir}\" \n\n"

		<< "var_sdk_name=\"iphonesimulator\"\n\n"

		<< "collection_foo \\\n"
		<< "\t\"${var_sln_name}\" \\\n"
		<< "\t\"${var_sdk_name}\" \\\n"
		<< "\t\"${var_clang_tag}\" \\\n"
		<< "\t\"${var_cpp_ver}\" \\\n"
		<< "\t\"${var_sln_dir}\" \\\n"
		<< "\t\"${var_prefix_stage_dir}\" \n\n"
		;

	out_str = yggr::charset::make_string_charset_helper(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return out_str;
}

bool fix_workspace_file_one_gen_cmake_build_sh(const cb::cbws_infos& cbws_infos, 
												const ptree_string_type& ws_dir,
												const ptree_string_type& ws_fname)
{
	yggr::utf8_string cmake_darwin_dir(ws_dir + "/CMakeDarwin/" + cbws_infos.cbws_title_, YGGR_STR_UTF8_STRING_CHARSET_NAME());

	yggr::utf8_string cmake_build_tpl("cmake-build-tpl.sh");
	yggr::utf8_string cmake_build_tpl_call("cmake-build-tpl_call.sh");

	yggr::utf8_string cmake_all_build_and_collection("cmake_all_build_and_collection.sh");
	yggr::utf8_string cmake_all_build_and_collection_cpp11("cmake_all_build_and_collection_cpp11.sh");
	yggr::utf8_string cmake_all_build_and_collection_cpp14("cmake_all_build_and_collection_cpp14.sh");
	yggr::utf8_string cmake_all_build_and_collection_cpp17("cmake_all_build_and_collection_cpp17.sh");
	yggr::utf8_string cmake_all_build_and_collection_cpp20("cmake_all_build_and_collection_cpp20.sh");

	yggr::utf8_string cmake_all_build_and_collection_lipo("cmake_all_build_and_collection_lipo.sh");
	yggr::utf8_string cmake_all_build_and_collection_lipo_cpp11("cmake_all_build_and_collection_lipo_cpp11.sh");
	yggr::utf8_string cmake_all_build_and_collection_lipo_cpp14("cmake_all_build_and_collection_lipo_cpp14.sh");
	yggr::utf8_string cmake_all_build_and_collection_lipo_cpp17("cmake_all_build_and_collection_lipo_cpp17.sh");
	yggr::utf8_string cmake_all_build_and_collection_lipo_cpp20("cmake_all_build_and_collection_lipo_cpp20.sh");

	yggr::utf8_string cmake_build_macosx_cpp11_x86_64("cmake-build-macosx-clang-darwin16-cpp11-x86_64.sh");
	yggr::utf8_string cmake_build_macosx_cpp14_x86_64("cmake-build-macosx-clang-darwin16-cpp14-x86_64.sh");
	yggr::utf8_string cmake_build_macosx_cpp17_x86_64("cmake-build-macosx-clang-darwin16-cpp17-x86_64.sh");
	yggr::utf8_string cmake_build_macosx_cpp20_x86_64("cmake-build-macosx-clang-darwin16-cpp20-x86_64.sh");

	yggr::utf8_string cmake_build_macosx_cpp11_arm64("cmake-build-macosx-clang-darwin16-cpp11-arm64.sh");
	yggr::utf8_string cmake_build_macosx_cpp14_arm64("cmake-build-macosx-clang-darwin16-cpp14-arm64.sh");
	yggr::utf8_string cmake_build_macosx_cpp17_arm64("cmake-build-macosx-clang-darwin16-cpp17-arm64.sh");
	yggr::utf8_string cmake_build_macosx_cpp20_arm64("cmake-build-macosx-clang-darwin16-cpp20-arm64.sh");

	yggr::utf8_string cmake_build_macosx_cpp11("cmake-build-macosx-clang-darwin16-cpp11.sh");
	yggr::utf8_string cmake_build_macosx_cpp14("cmake-build-macosx-clang-darwin16-cpp14.sh");
	yggr::utf8_string cmake_build_macosx_cpp17("cmake-build-macosx-clang-darwin16-cpp17.sh");
	yggr::utf8_string cmake_build_macosx_cpp20("cmake-build-macosx-clang-darwin16-cpp20.sh");

	yggr::utf8_string cmake_build_iphoneos_cpp11("cmake-build-iphoneos-clang-darwin16-cpp11.sh");
	yggr::utf8_string cmake_build_iphoneos_cpp14("cmake-build-iphoneos-clang-darwin16-cpp14.sh");
	yggr::utf8_string cmake_build_iphoneos_cpp17("cmake-build-iphoneos-clang-darwin16-cpp17.sh");
	yggr::utf8_string cmake_build_iphoneos_cpp20("cmake-build-iphoneos-clang-darwin16-cpp20.sh");

	yggr::utf8_string cmake_build_iphonesimulator_cpp11_x86_64("cmake-build-iphonesimulator-clang-darwin16-cpp11-x86_64.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp14_x86_64("cmake-build-iphonesimulator-clang-darwin16-cpp14-x86_64.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp17_x86_64("cmake-build-iphonesimulator-clang-darwin16-cpp17-x86_64.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp20_x86_64("cmake-build-iphonesimulator-clang-darwin16-cpp20-x86_64.sh");

	yggr::utf8_string cmake_build_iphonesimulator_cpp11_arm64("cmake-build-iphonesimulator-clang-darwin16-cpp11-arm64.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp14_arm64("cmake-build-iphonesimulator-clang-darwin16-cpp14-arm64.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp17_arm64("cmake-build-iphonesimulator-clang-darwin16-cpp17-arm64.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp20_arm64("cmake-build-iphonesimulator-clang-darwin16-cpp20-arm64.sh");

	yggr::utf8_string cmake_build_iphonesimulator_cpp11("cmake-build-iphonesimulator-clang-darwin16-cpp11.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp14("cmake-build-iphonesimulator-clang-darwin16-cpp14.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp17("cmake-build-iphonesimulator-clang-darwin16-cpp17.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp20("cmake-build-iphonesimulator-clang-darwin16-cpp20.sh");

	//xcode
	yggr::utf8_string cmake_build_macosx_cpp11_x86_64_xcode("cmake-build-macosx-clang-darwin16-cpp11-x86_64-xcode.sh");
	yggr::utf8_string cmake_build_macosx_cpp14_x86_64_xcode("cmake-build-macosx-clang-darwin16-cpp14-x86_64-xcode.sh");
	yggr::utf8_string cmake_build_macosx_cpp17_x86_64_xcode("cmake-build-macosx-clang-darwin16-cpp17-x86_64-xcode.sh");
	yggr::utf8_string cmake_build_macosx_cpp20_x86_64_xcode("cmake-build-macosx-clang-darwin16-cpp20-x86_64-xcode.sh");

	yggr::utf8_string cmake_build_macosx_cpp11_arm64_xcode("cmake-build-macosx-clang-darwin16-cpp11-arm64-xcode.sh");
	yggr::utf8_string cmake_build_macosx_cpp14_arm64_xcode("cmake-build-macosx-clang-darwin16-cpp14-arm64-xcode.sh");
	yggr::utf8_string cmake_build_macosx_cpp17_arm64_xcode("cmake-build-macosx-clang-darwin16-cpp17-arm64-xcode.sh");
	yggr::utf8_string cmake_build_macosx_cpp20_arm64_xcode("cmake-build-macosx-clang-darwin16-cpp20-arm64-xcode.sh");

	yggr::utf8_string cmake_build_macosx_cpp11_xcode("cmake-build-macosx-clang-darwin16-cpp11-xcode.sh");
	yggr::utf8_string cmake_build_macosx_cpp14_xcode("cmake-build-macosx-clang-darwin16-cpp14-xcode.sh");
	yggr::utf8_string cmake_build_macosx_cpp17_xcode("cmake-build-macosx-clang-darwin16-cpp17-xcode.sh");
	yggr::utf8_string cmake_build_macosx_cpp20_xcode("cmake-build-macosx-clang-darwin16-cpp20-xcode.sh");

	yggr::utf8_string cmake_build_iphoneos_cpp11_xcode("cmake-build-iphoneos-clang-darwin16-cpp11-xcode.sh");
	yggr::utf8_string cmake_build_iphoneos_cpp14_xcode("cmake-build-iphoneos-clang-darwin16-cpp14-xcode.sh");
	yggr::utf8_string cmake_build_iphoneos_cpp17_xcode("cmake-build-iphoneos-clang-darwin16-cpp17-xcode.sh");
	yggr::utf8_string cmake_build_iphoneos_cpp20_xcode("cmake-build-iphoneos-clang-darwin16-cpp20-xcode.sh");

	yggr::utf8_string cmake_build_iphonesimulator_cpp11_x86_64_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp11-x86_64-xcode.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp14_x86_64_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp14-x86_64-xcode.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp17_x86_64_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp17-x86_64-xcode.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp20_x86_64_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp20-x86_64-xcode.sh");

	yggr::utf8_string cmake_build_iphonesimulator_cpp11_arm64_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp11-arm64-xcode.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp14_arm64_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp14-arm64-xcode.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp17_arm64_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp17-arm64-xcode.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp20_arm64_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp20-arm64-xcode.sh");

	yggr::utf8_string cmake_build_iphonesimulator_cpp11_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp11-xcode.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp14_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp14-xcode.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp17_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp17-xcode.sh");
	yggr::utf8_string cmake_build_iphonesimulator_cpp20_xcode("cmake-build-iphonesimulator-clang-darwin16-cpp20-xcode.sh");

	// tpl
	yggr::utf8_string str_cmake_build_tpl;
	gen_cmake_build_tpl_sh(str_cmake_build_tpl);

#if ONLY_MAKE_RUN_TEST()
	std::cout << "write " << cmake_darwin_dir + "/" + cmake_build_tpl << std::endl;
#else
	file_op::write_to_file(cmake_darwin_dir + "/" + cmake_build_tpl, str_cmake_build_tpl.org_str());
#endif // ONLY_MAKE_RUN_TEST()

	// tpl_call
	yggr::utf8_string str_cmake_build_tpl_call;
	gen_cmake_build_tpl_call_sh(str_cmake_build_tpl_call);

#if ONLY_MAKE_RUN_TEST()
	std::cout << "write " << cmake_darwin_dir + "/" + cmake_build_tpl_call << std::endl;
#else
	file_op::write_to_file(cmake_darwin_dir + "/" + cmake_build_tpl_call, str_cmake_build_tpl_call.org_str());
#endif // ONLY_MAKE_RUN_TEST()

	// cmake_all_build_and_collection
	{
		yggr::utf8_string str_cmake_all_build_and_collection;
		gen_cmake_darwin_build_and_collection_sh(str_cmake_all_build_and_collection, cbws_infos, "cpp11");

#if ONLY_MAKE_RUN_TEST()
		std::cout << "write " << cmake_darwin_dir + "/" + cmake_all_build_and_collection << std::endl;
		std::cout << "write " << cmake_darwin_dir + "/" + cmake_all_build_and_collection_cpp11 << std::endl;
#else
		file_op::write_to_file(cmake_darwin_dir + "/" + cmake_all_build_and_collection, str_cmake_all_build_and_collection.org_str());
		file_op::write_to_file(cmake_darwin_dir + "/" + cmake_all_build_and_collection_cpp11, str_cmake_all_build_and_collection.org_str());
#endif // ONLY_MAKE_RUN_TEST()
	}

	{
		yggr::utf8_string str_cmake_all_build_and_collection;
		gen_cmake_darwin_build_and_collection_sh(str_cmake_all_build_and_collection, cbws_infos, "cpp14");

#if ONLY_MAKE_RUN_TEST()
		std::cout << "write " << cmake_darwin_dir + "/" + cmake_all_build_and_collection_cpp14 << std::endl;
#else
		file_op::write_to_file(cmake_darwin_dir + "/" + cmake_all_build_and_collection_cpp14, str_cmake_all_build_and_collection.org_str());
#endif // ONLY_MAKE_RUN_TEST()
	}

	{
		yggr::utf8_string str_cmake_all_build_and_collection;
		gen_cmake_darwin_build_and_collection_sh(str_cmake_all_build_and_collection, cbws_infos, "cpp17");

#if ONLY_MAKE_RUN_TEST()
		std::cout << "write " << cmake_darwin_dir + "/" + cmake_all_build_and_collection_cpp17 << std::endl;
#else
		file_op::write_to_file(cmake_darwin_dir + "/" + cmake_all_build_and_collection_cpp17, str_cmake_all_build_and_collection.org_str());
#endif // ONLY_MAKE_RUN_TEST()
	}

	{
		yggr::utf8_string str_cmake_all_build_and_collection;
		gen_cmake_darwin_build_and_collection_sh(str_cmake_all_build_and_collection, cbws_infos, "cpp20");

#if ONLY_MAKE_RUN_TEST()
		std::cout << "write " << cmake_darwin_dir + "/" + cmake_all_build_and_collection_cpp20 << std::endl;
#else
		file_op::write_to_file(cmake_darwin_dir + "/" + cmake_all_build_and_collection_cpp20, str_cmake_all_build_and_collection.org_str());
#endif // ONLY_MAKE_RUN_TEST()
	}

	// cmake_all_build_and_collection_lipo
	{
		yggr::utf8_string str_cmake_all_build_and_collection;
		gen_cmake_darwin_build_and_collection_lipo_sh(str_cmake_all_build_and_collection, cbws_infos, "cpp11");

#if ONLY_MAKE_RUN_TEST()
		std::cout << "write " << cmake_darwin_dir + "/" + cmake_all_build_and_collection_lipo << std::endl;
		std::cout << "write " << cmake_darwin_dir + "/" + cmake_all_build_and_collection_lipo_cpp11 << std::endl;
#else
		file_op::write_to_file(cmake_darwin_dir + "/" + cmake_all_build_and_collection_lipo, str_cmake_all_build_and_collection.org_str());
		file_op::write_to_file(cmake_darwin_dir + "/" + cmake_all_build_and_collection_lipo_cpp11, str_cmake_all_build_and_collection.org_str());
#endif // ONLY_MAKE_RUN_TEST()
	}

	{
		yggr::utf8_string str_cmake_all_build_and_collection;
		gen_cmake_darwin_build_and_collection_sh(str_cmake_all_build_and_collection, cbws_infos, "cpp14");

#if ONLY_MAKE_RUN_TEST()
		std::cout << "write " << cmake_darwin_dir + "/" + cmake_all_build_and_collection_lipo_cpp14 << std::endl;
#else
		file_op::write_to_file(cmake_darwin_dir + "/" + cmake_all_build_and_collection_lipo_cpp14, str_cmake_all_build_and_collection.org_str());
#endif // ONLY_MAKE_RUN_TEST()
	}

	{
		yggr::utf8_string str_cmake_all_build_and_collection;
		gen_cmake_darwin_build_and_collection_sh(str_cmake_all_build_and_collection, cbws_infos, "cpp17");

#if ONLY_MAKE_RUN_TEST()
		std::cout << "write " << cmake_darwin_dir + "/" + cmake_all_build_and_collection_lipo_cpp17 << std::endl;
#else
		file_op::write_to_file(cmake_darwin_dir + "/" + cmake_all_build_and_collection_lipo_cpp17, str_cmake_all_build_and_collection.org_str());
#endif // ONLY_MAKE_RUN_TEST()
	}

	{
		yggr::utf8_string str_cmake_all_build_and_collection;
		gen_cmake_darwin_build_and_collection_sh(str_cmake_all_build_and_collection, cbws_infos, "cpp20");

#if ONLY_MAKE_RUN_TEST()
		std::cout << "write " << cmake_darwin_dir + "/" + cmake_all_build_and_collection_lipo_cpp20 << std::endl;
#else
		file_op::write_to_file(cmake_darwin_dir + "/" + cmake_all_build_and_collection_lipo_cpp20, str_cmake_all_build_and_collection.org_str());
#endif // ONLY_MAKE_RUN_TEST()
	}


#if ONLY_MAKE_RUN_TEST()

#	define WRITE_BUILD_SH(__fname__, __target__, __sdk_name__, __sdk_ver_min__, __arch__, __ld_arch_tag, __c_ver__, __cpp_ver__, __clr_lib__, __clr_bin__) \
		{ yggr::utf8_string str_cmake_build; \
			gen_cmake_build_sh(str_cmake_build, cbws_infos, __target__, __sdk_name__, __sdk_ver_min__, __arch__, __ld_arch_tag, __c_ver__, __cpp_ver__, __clr_lib__, __clr_bin__); \
			std::cout << "write " << cmake_darwin_dir + "/" + __fname__ << std::endl; }
#else

#	define WRITE_BUILD_SH(__fname__, __target__, __sdk_name__, __sdk_ver_min__, __arch__, __ld_arch_tag, __c_ver__, __cpp_ver__, __clr_lib__, __clr_bin__) \
		{ yggr::utf8_string str_cmake_build; \
			gen_cmake_build_sh(str_cmake_build, cbws_infos, __target__, __sdk_name__, __sdk_ver_min__, __arch__, __ld_arch_tag, __c_ver__, __cpp_ver__, __clr_lib__, __clr_bin__); \
			file_op::write_to_file(cmake_darwin_dir + "/" + __fname__, str_cmake_build.org_str()); }

#endif // ONLY_MAKE_RUN_TEST()

	bool b_clr_lib = true;
	bool b_clr_bin = true;

	if(cbws_infos.cbws_title_ == "yggr_test_only")
	{
		b_clr_lib = false;
		b_clr_bin = true;
	}

	// ninja
	// macosx x86_64
	WRITE_BUILD_SH(cmake_build_macosx_cpp11_x86_64, "Ninja", "macosx", "12.4", "x86_64", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp14_x86_64, "Ninja", "macosx", "12.4", "x86_64", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp17_x86_64, "Ninja", "macosx", "12.4", "x86_64", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp20_x86_64, "Ninja", "macosx", "12.4", "x86_64", "a", 99, 20, b_clr_lib, b_clr_bin);

	// macosx arm64
	WRITE_BUILD_SH(cmake_build_macosx_cpp11_arm64, "Ninja", "macosx", "12.4", "arm64", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp14_arm64, "Ninja", "macosx", "12.4", "arm64", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp17_arm64, "Ninja", "macosx", "12.4", "arm64", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp20_arm64, "Ninja", "macosx", "12.4", "arm64", "a", 99, 20, b_clr_lib, b_clr_bin);

	// macosx fat
	WRITE_BUILD_SH(cmake_build_macosx_cpp11, "Ninja", "macosx", "12.4", "all", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp14, "Ninja", "macosx", "12.4", "all", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp17, "Ninja", "macosx", "12.4", "all", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp20, "Ninja", "macosx", "12.4", "all", "a", 99, 20, b_clr_lib, b_clr_bin);

	// iphoneos
	WRITE_BUILD_SH(cmake_build_iphoneos_cpp11, "Ninja", "iphoneos", "15.6", "all", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphoneos_cpp14, "Ninja", "iphoneos", "15.6", "all", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphoneos_cpp17, "Ninja", "iphoneos", "15.6", "all", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphoneos_cpp20, "Ninja", "iphoneos", "15.6", "all", "a", 99, 20, b_clr_lib, b_clr_bin);

	// iphonesimulator x86_64
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp11_x86_64, "Ninja", "iphonesimulator", "15.6", "x86_64", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp14_x86_64, "Ninja", "iphonesimulator", "15.6", "x86_64", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp17_x86_64, "Ninja", "iphonesimulator", "15.6", "x86_64", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp20_x86_64, "Ninja", "iphonesimulator", "15.6", "x86_64", "a", 99, 20, b_clr_lib, b_clr_bin);

	// iphonesimulator arm64
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp11_arm64, "Ninja", "iphonesimulator", "15.6", "arm64", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp14_arm64, "Ninja", "iphonesimulator", "15.6", "arm64", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp17_arm64, "Ninja", "iphonesimulator", "15.6", "arm64", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp20_arm64, "Ninja", "iphonesimulator", "15.6", "arm64", "a", 99, 20, b_clr_lib, b_clr_bin);

	// iphonesimulator fat
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp11, "Ninja", "iphonesimulator", "15.6", "all", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp14, "Ninja", "iphonesimulator", "15.6", "all", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp17, "Ninja", "iphonesimulator", "15.6", "all", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp20, "Ninja", "iphonesimulator", "15.6", "all", "a", 99, 20, b_clr_lib, b_clr_bin);

	//xcode
	// macosx x86_64
	WRITE_BUILD_SH(cmake_build_macosx_cpp11_x86_64_xcode, "Xcode", "macosx", "12.4", "x86_64", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp14_x86_64_xcode, "Xcode", "macosx", "12.4", "x86_64", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp17_x86_64_xcode, "Xcode", "macosx", "12.4", "x86_64", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp20_x86_64_xcode, "Xcode", "macosx", "12.4", "x86_64", "a", 99, 20, b_clr_lib, b_clr_bin);

	// macosx arm64
	WRITE_BUILD_SH(cmake_build_macosx_cpp11_arm64_xcode, "Xcode", "macosx", "12.4", "arm64", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp14_arm64_xcode, "Xcode", "macosx", "12.4", "arm64", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp17_arm64_xcode, "Xcode", "macosx", "12.4", "arm64", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp20_arm64_xcode, "Xcode", "macosx", "12.4", "arm64", "a", 99, 20, b_clr_lib, b_clr_bin);

	// macosx fat
	WRITE_BUILD_SH(cmake_build_macosx_cpp11_xcode, "Xcode", "macosx", "12.4", "all", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp14_xcode, "Xcode", "macosx", "12.4", "all", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp17_xcode, "Xcode", "macosx", "12.4", "all", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_macosx_cpp20_xcode, "Xcode", "macosx", "12.4", "all", "a", 99, 20, b_clr_lib, b_clr_bin);

	// iphoneos
	WRITE_BUILD_SH(cmake_build_iphoneos_cpp11_xcode, "Xcode", "iphoneos", "15.6", "all", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphoneos_cpp14_xcode, "Xcode", "iphoneos", "15.6", "all", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphoneos_cpp17_xcode, "Xcode", "iphoneos", "15.6", "all", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphoneos_cpp20_xcode, "Xcode", "iphoneos", "15.6", "all", "a", 99, 20, b_clr_lib, b_clr_bin);

	// iphonesimulator x86_64
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp11_x86_64_xcode, "Xcode", "iphonesimulator", "15.6", "x86_64", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp14_x86_64_xcode, "Xcode", "iphonesimulator", "15.6", "x86_64", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp17_x86_64_xcode, "Xcode", "iphonesimulator", "15.6", "x86_64", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp20_x86_64_xcode, "Xcode", "iphonesimulator", "15.6", "x86_64", "a", 99, 20, b_clr_lib, b_clr_bin);

	// iphonesimulator arm64
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp11_arm64_xcode, "Xcode", "iphonesimulator", "15.6", "arm64", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp14_arm64_xcode, "Xcode", "iphonesimulator", "15.6", "arm64", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp17_arm64_xcode, "Xcode", "iphonesimulator", "15.6", "arm64", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp20_arm64_xcode, "Xcode", "iphonesimulator", "15.6", "arm64", "a", 99, 20, b_clr_lib, b_clr_bin);

	// iphonesimulator fat
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp11_xcode, "Xcode", "iphonesimulator", "15.6", "all", "a", 99, 11, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp14_xcode, "Xcode", "iphonesimulator", "15.6", "all", "a", 99, 14, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp17_xcode, "Xcode", "iphonesimulator", "15.6", "all", "a", 99, 17, b_clr_lib, b_clr_bin);
	WRITE_BUILD_SH(cmake_build_iphonesimulator_cpp20_xcode, "Xcode", "iphonesimulator", "15.6", "all", "a", 99, 20, b_clr_lib, b_clr_bin);

#undef WRITE_BUILD_SH

	return true;
}

void fix_workspace_file_one(const filter_list_type& cbs_filter_list,
							const filter_list_type& cpp_filter_list,
							const cb::cbws_infos& cbws_infos, const ptree_string_type& ws_dir, 	
							file_list_type& fixed_cbs_flist_a, 
							file_list_type& fixed_cbs_flist_a_now, 
							file_list_type& fixed_cbs_flist_so,
							file_list_type& fixed_cbs_flist_so_now,
							file_list_type& fixed_cbs_flist_exe,
							file_list_type& fixed_cbs_flist_exe_now,
							file_list_type& cb_ignore, file_list_type& cb_failed)
{
	typedef cb::cbws_infos cbws_infos_type;
	typedef cbws_infos_type::string_vt_type string_vt_type;

	typedef cb::cb_infos cb_infos_type;
	bool is_yggr_ws = file_op::is_yggr_sln_check(ws_dir);

	for(string_vt_type::const_iterator i = cbws_infos.cb_file_list_.begin(), isize = cbws_infos.cb_file_list_.end();
			i != isize; ++i)
	{
		std::cout << "fix cbs: " << *i << std::endl;
		yggr::string str_cbp_path = *i;
		yggr::string str_cbp_fname = file_op::get_file_name(str_cbp_path);
		yggr::string str_cbp_dir = file_op::get_file_dir(str_cbp_path);

		if(cbs_filter_list.find(yggr::utf8_string(str_cbp_fname, YGGR_STR_UTF8_STRING_CHARSET_NAME())) != cbs_filter_list.end())
		{
			std::cout << "cbs_filter_list skip " << *i << std::endl;
			continue;
		}

		if(fixed_cbs_flist_a.find(yggr::utf8_string(str_cbp_fname, YGGR_STR_UTF8_STRING_CHARSET_NAME())) != fixed_cbs_flist_a.end())
		{
			std::cout << "fixed_cbs_flist_a skip " << *i << std::endl;
			fixed_cbs_flist_a_now.insert(*i);
			continue;
		}

		if(fixed_cbs_flist_so.find(yggr::utf8_string(str_cbp_fname, YGGR_STR_UTF8_STRING_CHARSET_NAME())) != fixed_cbs_flist_so.end())
		{
			std::cout << "fixed_cbs_flist_so skip " << *i << std::endl;
			fixed_cbs_flist_so_now.insert(*i);
			continue;
		}

		if(fixed_cbs_flist_exe.find(yggr::utf8_string(str_cbp_fname, YGGR_STR_UTF8_STRING_CHARSET_NAME())) != fixed_cbs_flist_exe.end())
		{
			std::cout << "fixed_cbs_flist_exe skip " << *i << std::endl;
			fixed_cbs_flist_exe_now.insert(*i);
			continue;
		}

		yggr::utf8_string fpath(ws_dir + "/" + str_cbp_path, YGGR_STR_UTF8_STRING_CHARSET_NAME());
		ptree_type ptree_cb;
		cb_infos_type cb_infos;
		ptree_string_type app_tag;

		if(cb::get_cb_ptree_from_file(ptree_cb, fpath).empty())
		{
			cb_failed.insert(fpath);
			continue;
		}

		app_tag = cb::get_cb_app_tag(ptree_cb);

		if(app_tag.empty())
		{
			cb_failed.insert(fpath);
			continue;
		}

		if(!cb::parse_cb_infos(cb_infos, ptree_cb).validate())
		{
			cb_failed.insert(fpath);
			continue;
		}

		cb_infos.proj_cur_dir_ = str_cbp_dir;

		format_cb_infos(cb_infos);

		ptree_string_type proj_cmakelists_txt_code;
		
		if(app_tag == cb::cb_app_tags::s_tag_static_lib())
		{
			if(!conv_to_proj_cmakelists_txt(proj_cmakelists_txt_code, cb_infos, cpp_filter_list, is_yggr_ws).size())
			{
				cb_failed.insert(fpath);
				fixed_cbs_flist_a.insert(str_cbp_path);
				return;
			}
			fixed_cbs_flist_a.insert(str_cbp_path);
			fixed_cbs_flist_a_now.insert(str_cbp_path);
		}
		else if(app_tag == cb::cb_app_tags::s_tag_dll())
		{
			if(!conv_to_proj_cmakelists_txt(proj_cmakelists_txt_code, cb_infos, cpp_filter_list, is_yggr_ws).size())
			{
				cb_failed.insert(fpath);
				fixed_cbs_flist_so.insert(str_cbp_path);
				return;
			}
			fixed_cbs_flist_so.insert(str_cbp_path);
			fixed_cbs_flist_so_now.insert(str_cbp_path);
		}
		else if(app_tag == cb::cb_app_tags::s_tag_exe())
		{
			if(!conv_to_proj_cmakelists_txt(proj_cmakelists_txt_code, cb_infos, cpp_filter_list, is_yggr_ws).size())
			{
				cb_failed.insert(fpath);
				fixed_cbs_flist_exe.insert(str_cbp_path);
				return;
			}
			fixed_cbs_flist_exe.insert(str_cbp_path);
			fixed_cbs_flist_exe_now.insert(str_cbp_path);
		}
		else
		{
			continue;
		}

		yggr::string str_cmakelist_txt_dir = make_cmakelist_txt_dir_name(str_cbp_path);
		if(str_cmakelist_txt_dir.empty())
		{
			cb_failed.insert(fpath);
			return;
		}
		yggr::utf8_string fpath_proj_cmakelist_txt(
							ws_dir + "/CMakeDarwin/" + cbws_infos.cbws_title_ + "/"
							+ str_cmakelist_txt_dir + "/CMakeLists.txt",
							YGGR_STR_UTF8_STRING_CHARSET_NAME());
#if ONLY_MAKE_RUN_TEST()
		std::cout << fpath_proj_cmakelist_txt << ": " << std::endl;
		std::cout << proj_cmakelists_txt_code << std::endl;
#else
		file_op::write_to_file(fpath_proj_cmakelist_txt, proj_cmakelists_txt_code);
#endif // ONLY_MAKE_RUN_TEST

	}
}

yggr::utf8_string cbp_fname_2_proj_name(const yggr::utf8_string& cbp_fname)
{
	yggr::utf8_string fname = file_op::get_file_name_no_ext(cbp_fname);
	return 
		6/*_linux*/ < fname.length()?
			fname.substr(0, fname.length() - 6)
			: yggr::utf8_string();
			
}

yggr::utf8_string& make_yggr_run_test_sh(yggr::utf8_string& out_str,
											const file_list_type& fixed_cbs_flist_exe,
											bool is_debug)
{
	yggr::string target_dir = is_debug? "Debug-x64/clang-darwin16" : "Release-x64/clang-darwin16";
	yggr::string env_sh_file = is_debug? "begin_test_env_darwin_x64d.sh" : "begin_test_env_darwin_x64.sh";
	yggr::string dylib_dir = is_debug? "lib/Debug-x64" : "lib/Release-x64";
	yggr::string test_dir = "test";

	std::stringstream ss;

	ss << "#!/bin/sh\n\n"

		<< "source ./" << env_sh_file << "\n\n"

		<< "rm -f " << test_dir << "/yggr_lua_base_type.so\n"
		<< "rm -f " << target_dir << "/yggr_lua_base_type.so\n"
		<< "rm -f " << dylib_dir << "/yggr_lua_base_type.dylib\n\n"

		<< "ln -s " << dylib_dir << "/libyggr_lua_base_type.dylib " << target_dir << "/yggr_lua_base_type.dylib\n"
		<< "ln -s ../../" << dylib_dir << "/libyggr_lua_base_type.dylib " << target_dir << "/yggr_lua_base_type.so\n\n"
		<< "ln -s ../" << dylib_dir << "/libyggr_lua_base_type.dylib " << test_dir << "/yggr_lua_base_type.so\n\n"

		<< "rm -fr ./test_out\n\n"

		<< "cd ./test\n\n"
		;

	for(file_list_type::const_iterator i = fixed_cbs_flist_exe.begin(), isize = fixed_cbs_flist_exe.end();
			i != isize; ++i)
	{
		yggr::utf8_string exe_name = cbp_fname_2_proj_name(*i);

		if(exe_name.size())
		{
			ss << "../" << target_dir << "/" << exe_name << " --batch-test\n";
		}
	}

	ss << "\n";
	
	ss << "cd ..\n"
		<< "source end_test_env_darwin.sh\n"
		<< "echo \"---------all test end-----------------\"\n"
		;

	out_str = yggr::charset::make_string_charset_helper(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	return out_str;
}

void fix_workspace_files(const file_list_type& cbws_flist, 
							const filter_list_type& cbs_filter_list,
							const filter_list_type& cpp_filter_list,
							file_list_type& fixed_cbs_flist_a,
							file_list_type& fixed_cbs_flist_so,
							file_list_type& fixed_cbs_flist_exe,
							file_list_type& cbsw_ignore, file_list_type& cbws_failed,
							file_list_type& cb_ignore, file_list_type& cb_failed)
{
	typedef cb::cbws_infos cbws_infos_type;

	for(file_list_type::const_iterator i = cbws_flist.begin(), isize = cbws_flist.end(); i != isize; ++i)
	{
		std::cout << "fix cbws: " << *i << std::endl;

		yggr::utf8_string fpath(*i);
		yggr::utf8_string file_dir(file_op::get_file_dir(fpath.org_str()), YGGR_STR_UTF8_STRING_CHARSET_NAME());

		if(fpath.empty())
		{
			continue;
		}

		ptree_type ptree_cbws;
		cbws_infos_type cbws_infos;

		if(cb::get_cb_workspace_ptree_from_file(ptree_cbws, fpath).empty())
		{
			cbws_failed.insert(fpath);
			continue;
		}

		if(!cb::parse_cb_workspace_infos(cbws_infos, ptree_cbws).cb_file_list_.size())
		{
			cbws_failed.insert(fpath);
			continue;
		}

		ptree_string_type ws_dir = file_op::get_file_dir(fpath.org_str());
		ptree_string_type ws_fname = file_op::get_file_name_no_ext(fpath.org_str());
		ptree_string_type ws_ftitle = ws_fname.substr(0, ws_fname.size() - 6 /*"_linux"*/);

		file_list_type fixed_cbs_flist_a_now;
		file_list_type fixed_cbs_flist_so_now;
		file_list_type fixed_cbs_flist_exe_now;

		fix_workspace_file_one(
			cbs_filter_list, cpp_filter_list,
			cbws_infos, ws_dir,
			fixed_cbs_flist_a,
			fixed_cbs_flist_a_now,
			fixed_cbs_flist_so,
			fixed_cbs_flist_so_now,
			fixed_cbs_flist_exe,
			fixed_cbs_flist_exe_now,
			cb_ignore, cb_failed);

		if(!fix_workspace_file_one_cmakelists_txt(
				cbws_infos, ws_dir, ws_ftitle, 
				fixed_cbs_flist_a_now, fixed_cbs_flist_so_now, fixed_cbs_flist_exe_now))
		{
			cbws_failed.insert(fpath);
			continue;
		}

		if(!fix_workspace_file_one_gen_cmake_build_sh(
				cbws_infos, ws_dir, ws_ftitle))
		{
			cbws_failed.insert(fpath);
			continue;
		}

		if(ws_ftitle == "yggr_test_only")
		{
			{
				yggr::utf8_string run_test_code;
			
				if(make_yggr_run_test_sh(run_test_code, fixed_cbs_flist_exe_now, true/*debug*/).size())
				{
					yggr::utf8_string run_test_fname = yggr::utf8_string(ws_dir.c_str()) + "/run_test_clang-darwin16_x64d.sh";
	#if ONLY_MAKE_RUN_TEST()
					std::cout << run_test_fname << ": " << std::endl;
					std::cout << run_test_code << std::endl;
	#else
					file_op::write_to_file(run_test_fname, run_test_code.org_str());
	#endif // ONLY_MAKE_RUN_TEST
				}
			}

			{
				yggr::utf8_string run_test_code;
			
				if(make_yggr_run_test_sh(run_test_code, fixed_cbs_flist_exe_now, false/*release*/).size())
				{
					yggr::utf8_string run_test_fname = yggr::utf8_string(ws_dir.c_str()) + "/run_test_clang-darwin16_x64.sh";
	#if ONLY_MAKE_RUN_TEST()
					std::cout << run_test_fname << ": " << std::endl;
					std::cout << run_test_code << std::endl;
	#else
					file_op::write_to_file(run_test_fname, run_test_code.org_str());
	#endif // ONLY_MAKE_RUN_TEST
				}
			}
		}

	}
}

bool write_log_file(const yggr::utf8_string& fname, yggr::u32 state, const file_list_type& file_list)
{
	if(0 == state)
	{
		return true;
	}

	std::stringstream ss;

	if(file_list.size())
	{
		switch(state)
		{
		case xml_op::fix_result::E_FAILED:
			ss << "failed:\n";
			break;
		case xml_op::fix_result::E_IGNORE:
			ss << "ignore:\n";
			break;
		default:
			return true;
		}

		for(file_list_type::const_iterator i = file_list.begin(), isize = file_list.end();
				i != isize; ++i)
		{
			ss << *i << "\n";
		}
	}
	else
	{
		switch(state)
		{
		case xml_op::fix_result::E_FAILED:
			ss << "succeed no failed files \n";
			break;
		case xml_op::fix_result::E_IGNORE:
			ss << "empty no ignore files\n";
			break;
		default:
			return true;
		}
	}

#if ONLY_MAKE_RUN_TEST()
	std::cout << ss.str() << std::endl;
	return true;
#else
	return file_op::write_to_file(fname, ss.str());
#endif // ONLY_MAKE_RUN_TEST()

}

int main_detail(int argc, char* argv[])
{
	if(argc > 1)
	{
		cfg_fname = yggr::utf8_string(argv[1], YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	tool_conv_to_darwin_cmakelists_txt_cfg cfg_root;

	if(0 != format_config_file(cfg_fname, cfg_root))
	{
		std::cout << "gen config file succeed, please modify it" << std::endl;
		return -1;
	}

	file_list_type need_fix_ws_files;

	get_files(cfg_root.root_dir_, cfg_root.dir_filter_, need_fix_ws_files);

	file_list_type fixed_cbs_files_a;
	file_list_type fixed_cbs_files_so;
	file_list_type fixed_cbs_files_exe;
	
	file_list_type ignore_cbws_files;
	file_list_type failed_cbws_files;
	
	file_list_type ignore_cb_files;
	file_list_type failed_cb_files;

	fix_workspace_files(
		need_fix_ws_files, cfg_root.cbp_file_filter_, cfg_root.cpp_file_filter_,
		fixed_cbs_files_a, fixed_cbs_files_so, fixed_cbs_files_exe,
		ignore_cbws_files, failed_cbws_files,
		ignore_cb_files, failed_cb_files);


	write_log_file(yggr::utf8_string("tool_conv_cbp_to_darwin_cmakelists_txt_failed_log.txt"), xml_op::fix_result::E_FAILED, failed_cb_files);
	write_log_file(yggr::utf8_string("tool_conv_cbp_to_darwin_cmakelists_txt_ignore_log.txt"), xml_op::fix_result::E_IGNORE, ignore_cb_files);

	write_log_file(yggr::utf8_string("tool_conv_workspace_to_darwin_cmakelists_txt_failed_log.txt"), xml_op::fix_result::E_FAILED, failed_cbws_files);
	write_log_file(yggr::utf8_string("tool_conv_workspace_to_darwin_cmakelists_txt_ignore_log.txt"), xml_op::fix_result::E_IGNORE, ignore_cbws_files);

	return 0;
}

#endif // BOOST_VERSION < 105600

#if BOOST_VERSION < 105600

int main(int argc, char* argv[])
{
	assert(("Message: this tool not support boost 1.55 and older", false));
	std::cout << "end" << std::endl;

	char cc = 0;
	std::cin >> cc;

	return -1;
}

#else

int main(int argc, char* argv[])
{
    std::cout << "start" << std::endl;
	int ret = main_detail(argc, argv);

	std::cout << "end" << std::endl;

	if(!(argc > 1))
	{
		char cc = 0;
		std::cin >> cc;
	}

	return ret;
}

#endif // #if BOOST_VERSION < 105600

