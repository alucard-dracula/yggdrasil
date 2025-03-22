// tool_conv_to_cb_solution.cpp

#include "tool_conv_to_jni_mk_config.hpp"

#if !(BOOST_VERSION < 105600)

#include "tool_conv_to_jni_mk_cfg.hpp"

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

static const char* tool_conv_to_cb_solution_cfg_mark = "tool_conv_to_jni_mk_cfg";

typedef yggr::set<ptree_string_type> ptree_string_set_type;
typedef yggr::set<yggr::utf8_string> file_list_type;
typedef yggr::unordered_set<yggr::utf8_string> filter_list_type;
typedef local_fsys_operators_type::path_type path_type;

typedef yggr::unordered_map<yggr::utf8_string, ptree_type> ptree_umap_type;
//typedef yggr::map<yggr::utf8_string, ptree_string_type> vcproj_file_app_tag_map_type;


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

	{
		std::cout << "\ninfos.src_files_: " << std::endl;
		for(string_set_citer_type i = infos.src_files_.begin(), isize = infos.src_files_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.cinclude_: " << std::endl;
		for(string_vt_citer_type i = infos.cinclude_.begin(), isize = infos.cinclude_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

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

	{
		std::cout << "\ninfos.ldflag_proj_1st_: " << std::endl;
		for(string_vt_citer_type i = infos.ldflag_proj_1st_.begin(), isize = infos.ldflag_proj_1st_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.ldflag_proj_: " << std::endl;
		for(string_vt_citer_type i = infos.ldflag_proj_.begin(), isize = infos.ldflag_proj_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.ldflag_proj_debug_: " << std::endl;
		for(string_vt_citer_type i = infos.ldflag_proj_debug_.begin(), isize = infos.ldflag_proj_debug_.end(); i != isize; ++i)
		{
			std::cout << *i << std::endl;
		}
	}

	{
		std::cout << "\ninfos.ldflag_proj_release_: " << std::endl;
		for(string_vt_citer_type i = infos.ldflag_proj_release_.begin(), isize = infos.ldflag_proj_release_.end(); i != isize; ++i)
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

static yggr::utf8_string cfg_fname("tool_conv_to_jni_mk_cfg.xml");

int format_config_file(const yggr::utf8_string& root_cfg_fname,
						tool_conv_to_jni_mk_cfg& cfg_root)
{
	int ret = 0;
	if(!read_cfg_file(root_cfg_fname, tool_conv_to_cb_solution_cfg_mark, cfg_root))
	{
		tool_conv_to_jni_mk_cfg cfg = tool_conv_to_jni_mk_cfg_sample();
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

	//if(file_filter.find(yggr::utf8_string(fpath.filename().native())) != file_filter.end())
	//{
	//	return;
	//}

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
ptree_string_type make_mk_l_multi_item(const Cont& cont, 
										const ptree_string_type& table_str = "",
										const ptree_string_type& root_path = "",
										const ptree_string_type& app_macro = "")
{
	typedef Cont cont_type;
	typedef typename yggr::range_ex::range_const_iterator_ex<cont_type>::type cont_citer_type;

	std::stringstream ss;

	for(cont_citer_type i = boost::begin(cont), isize = boost::end(cont); i != isize; ++i)
	{
		if(*i != "-DYGGR_SYSTEM_64")
		{
			ss << table_str << root_path << *i << " \\\n";
		}
	}

	if(app_macro.size())
	{
		ss << table_str << app_macro << " \\\n";
	}

	std::string tmp_str = ss.str();

	return ptree_string_type(tmp_str.begin(), tmp_str.end());
}

cb::cb_infos::string_vt_type&
	append_sp_ldflags(cb::cb_infos::string_vt_type& out, const ptree_string_type& proj_name)
{
	if(proj_name == "lua")
	{
		out.push_back("-lncurses");
	}

	return out;
}

ptree_string_type& conv_to_android_mk(ptree_string_type& out, cb::cb_infos& infos,
										const filter_list_type& cpp_filter_list,
										bool is_yggr_proj)
{
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

	ss << "#\n#\t" << infos.proj_name_ << " definitions.\n#\n\n" 
		<< "LOCAL_PATH := $(call my-dir)\n\n" 
		<< "USRDEF_LOCAL_PROJECT_NAME := " << infos.proj_name_ << "\n\n"
		<< "l_path := $(LOCAL_PATH)\n\n"
		;

	if(infos.proj_t_ == cb::cb_app_tags::s_tag_exe())
	{
		ss << "# output module name\n"
			<< "l_module := " << infos.proj_output_release_ << "\n\n"
			;
	}
	else
	{
		ss << "# output module name\n"
			<< "ifeq ($(NDK_DEBUG), 1)\n"
			<< "\tl_module := " << infos.proj_output_debug_ << "\n"
			<< "else\n" 
			<< "\tl_module := " << infos.proj_output_release_ << "\n"
			<< "endif\n\n"
			;
	}

	ss << "# source files\n"
		<< "l_src_file := \\\n" << make_mk_l_multi_item(infos.src_files_, "\t") << "\n"

		<< "# include path\n"
		<< "l_c_include := \\\n\t$(l_path) \\\n" << make_mk_l_multi_item(infos.cinclude_, "\t", "$(l_path)/", "$(USRDEF_APP_C_INCLUDE)") << "\n"
		
		<< "# cflags\n"
		<< "l_cflags := \\\n" << make_mk_l_multi_item(infos.cflag_proj_, "\t", "", "$(USRDEF_APP_CFLAGS)") << "\n"

		<< "ifeq ($(NDK_DEBUG), 1)\n"
		<< "\tl_cflags += \\\n" << make_mk_l_multi_item(infos.cflag_proj_debug_, "\t\t", "", "$(USRDEF_APP_CFLAGS_DEBUG)") << "\n"
		<< "else\n" 
		<< "\tl_cflags += \\\n" << make_mk_l_multi_item(infos.cflag_proj_release_, "\t\t", "", "$(USRDEF_APP_CFLAGS_RELEASE)") << "\n"
		<< "endif\n\n"
		;

	if(infos.cflag_proj_debug_.find("-DYGGR_SYSTEM_64") != infos.cflag_proj_debug_.end()
		|| infos.cflag_proj_release_.find("-DYGGR_SYSTEM_64") != infos.cflag_proj_release_.end())
	{
		ss << "# 64bit cflag\n"
			<< "ifeq (\"$(TARGET_ARCH_ABI)\", \"arm64-v8a\")\n"
			<< "\tl_cflags += -DYGGR_SYSTEM_64\n"
			<< "endif \n\n"

			<< "ifeq (\"$(TARGET_ARCH_ABI)\", \"x86_64\")\n"
			<< "\tl_cflags += -DYGGR_SYSTEM_64\n"
			<< "endif \n\n"

			<< "ifeq (\"$(TARGET_ARCH_ABI)\", \"riscv64\")\n"
			<< "\tl_cflags += -DYGGR_SYSTEM_64\n"
			<< "endif \n\n"
		;
	}

	ss << "# cppflags\n"
		<< "l_cppflags := \\\n" << make_mk_l_multi_item(infos.cppflag_proj_, "\t", "", "$(USRDEF_APP_CPPFLAGS)") << "\n"

		<< "ifeq ($(NDK_DEBUG), 1)\n"
		<< "\tl_cppflags += \\\n" << make_mk_l_multi_item(infos.cppflag_proj_debug_, "\t\t", "", "$(USRDEF_APP_CPPFLAGS_DEBUG)") << "\n"
		<< "else\n" 
		<< "\tl_cppflags += \\\n" << make_mk_l_multi_item(infos.cppflag_proj_release_, "\t\t", "", "$(USRDEF_APP_CPPFLAGS_RELEASE)") << "\n"
		<< "endif\n\n"
		;

	if(infos.proj_t_ == cb::cb_app_tags::s_tag_static_lib())
	{
		ss << "# ldflags\n"
			<< "ifeq ($(NDK_DEBUG), 1)\n"
			<< "\tl_ldflags := \\\n" << "\t\t$(USRDEF_APP_LDFLAGS_DEBUG) \\\n" << "\n"
			<< "else\n" 
			<< "\tl_ldflags := \\\n" << "\t\t$(USRDEF_APP_LDFLAGS_RELEASE)\\\n" << "\n"
			<< "endif\n\n"

			<< "l_ldflags += \\\n" << "\t$(USRDEF_APP_LDFLAGS) \\\n" << "\n\n"
			;
	}
	else
	{
		cb::cb_infos::string_vt_type ldflags_sp;

		append_sp_ldflags(ldflags_sp, infos.proj_name_);

		ss << "# ldflags\n"
			<< "ifeq (\"$(TARGET_ARCH_ABI)\", \"arm64-v8a\")\n"
			<< "\tUSEDEF_CLANG_ABI := aarch64-linux-android\n"
			<< "\ttag_arch_bits := -a64\n"
			<< "endif\n\n"

			<< "ifeq (\"$(TARGET_ARCH_ABI)\", \"armeabi-v7a\")\n"
			<< "\tUSEDEF_CLANG_ABI := armv7a-linux-androideabi\n"
			<< "\ttag_arch_bits := -a32\n"
			<< "endif\n\n"

			<< "ifeq (\"$(TARGET_ARCH_ABI)\", \"riscv64\")\n"
			<< "\tUSEDEF_CLANG_ABI := riscv64-linux-android\n"
			<< "\ttag_arch_bits := -r64\n"
			<< "endif\n\n"

			<< "ifeq (\"$(TARGET_ARCH_ABI)\", \"x86_64\")\n"
			<< "\tUSEDEF_CLANG_ABI := x86_64-linux-android\n"
			<< "\ttag_arch_bits := \n"
			<< "endif\n\n"

			<< "ifeq (\"$(TARGET_ARCH_ABI)\", \"x86\")\n"
			<< "\tUSEDEF_CLANG_ABI := i686-linux-android\n"
			<< "\ttag_arch_bits := \n"
			<< "endif\n\n"

			<< "ifeq (\"$(TARGET_ARCH_ABI)\", \"riscv64\")\n"
			<< "\tl_ld_lib_path := \\\n"
			<< "\t\t$(addprefix -L, $(addsuffix /$(USEDEF_CLANG_ABI)/$(USRDEF_APP_RISCV64_MIN_SDK_VERSION), \\\n"
			<< "\t\t$(USRDEF_APP_LD_LIBRARY_PATH) \\\n"
			<< "\t\t))\n"
			<< "else\n"
			<< "\tl_ld_lib_path := \\\n"
			<< "\t\t$(addprefix -L, $(addsuffix /$(USEDEF_CLANG_ABI)/$(USRDEF_APP_MIN_SDK_VERSION), \\\n"
			<< "\t\t$(USRDEF_APP_LD_LIBRARY_PATH) \\\n"
			<< "\t\t))\n"
			<< "endif\n\n"
			;

		ss << "l_ldflags := \\\n\t$(l_ld_lib_path) \\\n";

		if(infos.ldflag_proj_1st_.size())
		{
			ss << make_mk_l_multi_item(infos.ldflag_proj_1st_, "\t", "", "");
		}

		ss << "\n";

		ss << "ifeq ($(NDK_DEBUG), 1)\n"
			<< "\tl_ldflags += \\\n" << make_mk_l_multi_item(infos.ldflag_proj_debug_, "\t\t", "", "$(USRDEF_APP_LDFLAGS_DEBUG)") << "\n"
			<< "else\n" 
			<< "\tl_ldflags += \\\n" << make_mk_l_multi_item(infos.ldflag_proj_release_, "\t\t", "", "$(USRDEF_APP_LDFLAGS_RELEASE)") << "\n"
			<< "endif\n\n"
			;

		ss << "l_ldflags += \\\n";

		if(infos.ldflag_proj_.size())
		{
			ss << make_mk_l_multi_item(infos.ldflag_proj_, "\t", "", "");
		}

		if(ldflags_sp.size())
		{
			ss << make_mk_l_multi_item(ldflags_sp, "\t", "", "");
		}
		
		ss << "\t$(USRDEF_APP_LDFLAGS)\\\n\n";
	}

	if(infos.proj_t_ == cb::cb_app_tags::s_tag_dll())
	{
		if(is_yggr_proj)
		{
			ss << "# out_dir\n\n"
				<< "ifeq ($(NDK_DEBUG), 1)\n"
				<< "\tl_app_dst := $(USRDEF_PROJ_OUTDIR)/bin-debug/$(TARGET_ARCH_ABI)\n"
				<< "else\n"
				<< "\tl_app_dst := $(USRDEF_PROJ_OUTDIR)/bin/$(TARGET_ARCH_ABI)\n"
				<< "endif\n\n"
				;
		}
		else
		{
			ss << "# out_dir\n\n"
				<< "l_app_dst := $(USRDEF_PROJ_OUTDIR)/lib/$(TARGET_ARCH_ABI)\n\n"
				;
		}
	}
	else if(infos.proj_t_ == cb::cb_app_tags::s_tag_exe())
	{
		ss << "# out_dir\n\n"
			<< "ifeq ($(NDK_DEBUG), 1)\n"
			<< "\tl_app_dst := $(USRDEF_PROJ_OUTDIR)/bin-debug/$(TARGET_ARCH_ABI)\n"
			<< "else\n"
			<< "\tl_app_dst := $(USRDEF_PROJ_OUTDIR)/bin/$(TARGET_ARCH_ABI)\n"
			<< "endif\n\n"
			;
	}

	ss << "# build it\n\n"
		<< "include $(CLEAR_VARS)\n\n"
		<< "LOCAL_MODULE := $(l_module)\n"
		<< "LOCAL_SRC_FILES := $(l_src_file)\n"
		<< "LOCAL_C_INCLUDES := $(l_c_include)\n"
		<< "LOCAL_CFLAGS := $(l_cflags)\n"
		<< "LOCAL_CPPFLAGS := $(l_cppflags)\n"
		;

	if(infos.proj_t_ == cb::cb_app_tags::s_tag_static_lib())
	{
		ss << "#LOCAL_LDFLAGS := $(l_ldflags)\n\n"
			<< "include $(BUILD_STATIC_LIBRARY)\n";
	}
	else if(infos.proj_t_ == cb::cb_app_tags::s_tag_dll())
	{
		ss << "LOCAL_LDFLAGS := $(l_ldflags)\n\n"
			<< "NDK_APP_DST_DIR := $(l_app_dst)\n\n"
			<< "include $(BUILD_SHARED_LIBRARY)\n";
	}
	else if(infos.proj_t_ == cb::cb_app_tags::s_tag_exe())
	{
		ss << "LOCAL_LDFLAGS := $(l_ldflags)\n\n"
			<< "NDK_APP_DST_DIR := $(l_app_dst)\n\n"
			<< "include $(BUILD_EXECUTABLE)\n";
	}
	else
	{
		assert(false);
	}

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
	
	static const ptree_string_type mark_proj_name_r = "$(USRDEF_LOCAL_PROJECT_NAME)";
	static const ptree_string_type mark_compiler_version_tag_r = "$(USRDEF_APP_COMPILER_VERSION_TAG)";
	static const ptree_string_type mark_compiler_version_r = "$(USRDEF_APP_COMPILER_VERSION)";

	ptree_string_type str_fname = file_op::get_file_name(str);

	find_and_replace(str_fname, mark_proj_name, mark_proj_name_r);
	find_and_replace(str_fname, mark_compiler_version_tag, mark_compiler_version_tag_r);
	find_and_replace(str_fname, mark_compiler_version, mark_compiler_version_r);

	return str_fname;
}

ptree_string_type format_cb_infos_ldflags(const ptree_string_type& str)
{
	//static const ptree_string_type mark_proj_name = "$(PROJECT_NAME)";
	static const ptree_string_type mark_compiler_version_tag = "$(#COMPILER_VERSION_TAG)";
	static const ptree_string_type mark_compiler_version = "$(#COMPILER_VERSION)";
	static const ptree_string_type mark_boost_version_tag = "$(#BOOST_VERSION_TAG)";
	static const ptree_string_type mark_boost_version = "$(#BOOST_VERSION)";
	static const ptree_string_type mark_python_version = "$(#PYTHON_VERSION)";
	static const ptree_string_type mark_arch64_tag = "-x64";
	static const ptree_string_type mark_arch32_tag = "-x32";

	//static const ptree_string_type mark_proj_name_r = "$(USRDEF_LOCAL_PROJECT_NAME)";
	static const ptree_string_type mark_compiler_version_tag_r = "$(USRDEF_APP_COMPILER_VERSION_TAG)";
	static const ptree_string_type mark_compiler_version_r = "$(USRDEF_APP_COMPILER_VERSION)";
	static const ptree_string_type mark_boost_version_tag_r = "$(USRDEF_APP_BOOST_VERSION_TAG)";
	static const ptree_string_type mark_boost_version_r = "$(USRDEF_APP_BOOST_VERSION)";
	static const ptree_string_type mark_python_version_r = "$(USRDEF_APP_PYTHON_VERSION)";
	static const ptree_string_type mark_arch_tag_r = "$(tag_arch_bits)";

	ptree_string_type str_out = str;

	//find_and_replace(str_out, mark_proj_name, mark_proj_name_r);
	find_and_replace(str_out, mark_compiler_version_tag, mark_compiler_version_tag_r);
	find_and_replace(str_out, mark_compiler_version, mark_compiler_version_r);
	find_and_replace(str_out, mark_boost_version_tag, mark_boost_version_tag_r);
	find_and_replace(str_out, mark_boost_version, mark_boost_version_r);
	find_and_replace(str_out, mark_python_version, mark_python_version_r);
	find_and_replace(str_out, mark_arch64_tag, mark_arch_tag_r);
	find_and_replace(str_out, mark_arch32_tag, mark_arch_tag_r);

	return str_out;
}

cb::cb_infos& format_cb_infos(cb::cb_infos& infos)
{

	typedef cb::cb_infos::string_vt_type string_vt_type;

	if(!infos.validate())
	{
		return infos;
	}

	if(infos.proj_output_debug_.size())
	{
		infos.proj_output_debug_ = format_cb_infos_output_string(infos.proj_output_debug_);
	}

	if(infos.proj_output_release_.size())
	{
		infos.proj_output_release_ = format_cb_infos_output_string(infos.proj_output_release_);
	}

	{
		for(string_vt_type::iterator i = infos.ldflag_proj_.begin(), isize = infos.ldflag_proj_.end();
				i != isize; ++i)
		{
			*i = format_cb_infos_ldflags(*i);
		}
	}

	{
		for(string_vt_type::iterator i = infos.ldflag_proj_debug_.begin(), isize = infos.ldflag_proj_debug_.end();
				i != isize; ++i)
		{
			*i = format_cb_infos_ldflags(*i);
		}
	}

	{
		for(string_vt_type::iterator i = infos.ldflag_proj_release_.begin(), isize = infos.ldflag_proj_release_.end();
				i != isize; ++i)
		{
			*i = format_cb_infos_ldflags(*i);
		}
	}

	{
		string_vt_type::iterator new_end = std::remove(infos.ldflag_proj_debug_.begin(), infos.ldflag_proj_debug_.end(), "-L../lib64");
		bool need_fix_self_lib = (new_end != infos.ldflag_proj_debug_.end());

		if(need_fix_self_lib)
		{
			infos.ldflag_proj_debug_.erase(new_end, infos.ldflag_proj_debug_.end());
			new_end = std::remove(infos.ldflag_proj_release_.begin(), infos.ldflag_proj_release_.end(), "-L../lib64");
			if(new_end != infos.ldflag_proj_release_.end())
			{
				infos.ldflag_proj_release_.erase(new_end, infos.ldflag_proj_release_.end());
			}

			new_end = std::remove(infos.ldflag_proj_.begin(), infos.ldflag_proj_.end(), "-L../lib64");

			if(new_end != infos.ldflag_proj_.end())
			{
				infos.ldflag_proj_.erase(new_end, infos.ldflag_proj_.end());
			}

			infos.ldflag_proj_1st_.push_back("-L$(USRDEF_PROJ_OUTDIR)/lib/$(TARGET_ARCH_ABI)");
		}
	}

	return infos;
}


ptree_string_set_type& collect_proj_mk_dir(ptree_string_set_type& dlist, const ptree_string_set_type& flist)
{
	typedef ptree_string_set_type::const_iterator ptree_string_set_citer_type;

	ptree_string_type tmp_str_dir;

	for(ptree_string_set_citer_type i = flist.begin(), isize = flist.end(); i != isize; ++i)
	{
		dlist.insert(file_op::get_file_dir(*i));
	}

	return dlist;
}

ptree_string_type& conv_to_app_android_mk(ptree_string_type& out,
											const cb::cbws_infos& cbws_infos,
											const file_list_type& fixed_cbs_flist)
{
	std::stringstream ss;

	ss << "#\n#\t" << cbws_infos.cbws_title_ << " definitions.\n#\n\n"
		<< "LOCAL_PATH := $(call my-dir)\n\n"
		<< "include $(CLEAR_VARS)\n\n"
		;

	ss << "# subdirs\n"
		<< "subdirs := \\\n";

	for(file_list_type::const_iterator i = fixed_cbs_flist.begin(), isize = fixed_cbs_flist.end();
			i != isize; ++i)
	{
		yggr::string str_dir = file_op::get_file_dir(*i);
		yggr::string str_fname = file_op::get_file_name_no_ext(*i);
		assert(str_dir.size());
		assert(6 < str_fname.size());
		ss << "\t$(LOCAL_PATH)/../" << str_dir << "/Android_" << str_fname.substr(0, str_fname.size() - 6/*"_linux"*/) <<".mk \\\n";
	}

	if(fixed_cbs_flist.size())
	{
		ss << "\n\ninclude $(subdirs)\n";
	}
	else
	{
		ss << "\n\n#include $(subdirs)\n";
	}

	std::string str = ss.str();
	ptree_string_type out_str(str.begin(), str.end());
	out.swap(out_str);
	return out;
}


bool fix_workspace_file_one_jni_android_mk(const cb::cbws_infos& cbws_infos, 
											const ptree_string_type& ws_dir,
											const ptree_string_type& ws_fname,
											const file_list_type& fixed_cbs_flist_a,
											const file_list_type& fixed_cbs_flist_so,
											const file_list_type& fixed_cbs_flist_exe)
{
	yggr::utf8_string jni_path(ws_dir + "/jni", YGGR_STR_UTF8_STRING_CHARSET_NAME());

	{	
		yggr::utf8_string jni_mk_path(ws_dir + "/jni/Android_" + ws_fname + "_a.mk", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type app_proj_mk_code;

		if(!(conv_to_app_android_mk(app_proj_mk_code, cbws_infos, fixed_cbs_flist_a).size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << jni_mk_path << ": " << std::endl;
		std::cout << app_proj_mk_code << std::endl;
#else
		file_op::write_to_file(jni_mk_path, app_proj_mk_code);
#endif // ONLY_MAKE_RUN_TEST
	}

	{
		yggr::utf8_string jni_mk_path(ws_dir + "/jni/Android_" + ws_fname + "_so.mk", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type app_proj_mk_code;

		if(!(conv_to_app_android_mk(app_proj_mk_code, cbws_infos, fixed_cbs_flist_so).size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << jni_mk_path << ": " << std::endl;
		std::cout << app_proj_mk_code << std::endl;
#else
		file_op::write_to_file(jni_mk_path, app_proj_mk_code);
#endif // ONLY_MAKE_RUN_TEST
	}

	{
		yggr::utf8_string jni_mk_path(ws_dir + "/jni/Android_" + ws_fname + "_exe.mk", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type app_proj_mk_code;

		if(!(conv_to_app_android_mk(app_proj_mk_code, cbws_infos, fixed_cbs_flist_exe).size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << jni_mk_path << ": " << std::endl;
		std::cout << app_proj_mk_code << std::endl;
#else
		file_op::write_to_file(jni_mk_path, app_proj_mk_code);
#endif // ONLY_MAKE_RUN_TEST
	}

	return true;
}

ptree_string_type& conv_to_appliction_mk(ptree_string_type& out,
											const cb::cbws_infos& cbws_infos,
											const ptree_string_type& cpp_ver)
{
	std::stringstream ss;

	ss << "#\n#\t" << cbws_infos.cbws_title_ << " definitions.\n#\n\n"
		<< "APP_ABI := all\n"
		<< "#APP_ABI := arm64-v8a\n"
		<< "#APP_ABI := armeabi-v7a\n"
		<< "#APP_ABI := x86\n"
		<< "#APP_ABI := x86_64\n"
		<< "#APP_ABI := riscv64\n"
		<< "APP_STL := c++_static\n\n"
		<< "include $(CLEAR_VARS)\n\n"
		<< "ifeq ($(NDK_DEBUG), 1)\n"
		<< "\tAPP_DEBUG := True\n"
		<< "\tAPP_OPTIM := debug\n"
		<< "else\n"
		<< "\tAPP_DEBUG := False\n"
		<< "\tAPP_OPTIM := release\n"
		<< "endif\n\n"

		<< "USRDEF_APP_MIN_SDK_VERSION := 21\n"
		<< "USRDEF_APP_RISCV64_MIN_SDK_VERSION := 35\n"
		<< "USRDEF_APP_COMPILER_VERSION := clang\n"
		<< "USRDEF_APP_COMPILER_VERSION_TAG := -clang\n\n"

		<< "USRDEF_APP_BOOST_VERSION := 1_82\n"
		<< "USRDEF_APP_BOOST_VERSION_TAG := -1_82\n\n"

		<< "USRDEF_APP_PYTHON_VERSION := 312\n\n"

		<< "USRDEF_APP_C_INCLUDE := \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/unixODBC/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/ncurses/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/readline/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/base64/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/blowfish/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/md5/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/sha/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/libiconv/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/libicu/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/LzmaLib/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/openssl/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/python/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/dtl/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/boost/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/lua/include \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/luabind/include \n\n"

		<< "USRDEF_OUTDIR_ROOT := ./stage\n"
		<< "USRDEF_NDK_APP_OUT := $(USRDEF_OUTDIR_ROOT)/objs\n"
		;


	if((4 <= cbws_infos.cbws_title_.size()) && (cbws_infos.cbws_title_.substr(0, 4) == "yggr"))
	{
		ss << "USRDEF_PROJ_OUTDIR := $(USRDEF_OUTDIR_ROOT)/yggr\n\n";
	}
	else
	{
		ss << "USRDEF_PROJ_OUTDIR := $(USRDEF_OUTDIR_ROOT)/"<< cbws_infos.cbws_title_ << "\n\n";
	}

	ss << "USRDEF_APP_CFLAGS := \n" 
		<< "USRDEF_APP_CFLAGS_DEBUG := \n" 
		<< "USRDEF_APP_CFLAGS_RELEASE := \n\n" 

		<< "USRDEF_APP_CPPFLAGS := \n" 
		<< "USRDEF_APP_CPPFLAGS_DEBUG := \n" 
		<< "USRDEF_APP_CPPFLAGS_RELEASE := \n\n" 

		<< "USRDEF_APP_LD_LIBRARY_PATH := \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/unixODBC/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/ncurses/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/readline/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/base64/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/blowfish/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/md5/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/sha/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/libiconv/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/libicu/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/LzmaLib/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/openssl/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/python/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/dtl/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/boost/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/lua/lib \\\n"
		<< "\t$(NDK_ROOT)/../../../../ndk_third_part_local/luabind/lib \n\n"

		<< "USRDEF_APP_LDFLAGS_DEBUG := \n" 
		<< "USRDEF_APP_LDFLAGS_RELEASE := \n" 
		<< "USRDEF_APP_LDFLAGS := \n\n" 
		
		<< "NDK_APP_OUT := $(USRDEF_NDK_APP_OUT)\n\n"

		<< "APP_CPPFLAGS := " << cpp_ver << "\n" // boost 1_82 in android min -std=c++11
		<< "APP_PLATFORM := android-$(USRDEF_APP_MIN_SDK_VERSION)\n"
		;

	std::string str = ss.str();
	ptree_string_type out_str(str.begin(), str.end());
	out.swap(out_str);
	return out;
}

bool fix_workspace_file_one_jni_appliction_mk(const cb::cbws_infos& cbws_infos, 
												const ptree_string_type& ws_dir,
												const ptree_string_type& ws_ftitle)
{
	yggr::utf8_string jni_path(ws_dir + "/jni", YGGR_STR_UTF8_STRING_CHARSET_NAME());

	{
		yggr::utf8_string jni_app_mk_path(ws_dir + "/jni/Application_" + ws_ftitle + "_cpp11.mk", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type app_mk_code;

		if(!(conv_to_appliction_mk(app_mk_code, cbws_infos, "-std=c++11").size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << jni_app_mk_path << ": " << std::endl;
		std::cout << app_mk_code << std::endl;
#else
		file_op::write_to_file(jni_app_mk_path, app_mk_code);
#endif // ONLY_MAKE_RUN_TEST
	}

	{
		yggr::utf8_string jni_app_mk_path(ws_dir + "/jni/Application_" + ws_ftitle + "_cpp14.mk", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type app_mk_code;

		if(!(conv_to_appliction_mk(app_mk_code, cbws_infos, "-std=c++14").size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << jni_app_mk_path << ": " << std::endl;
		std::cout << app_mk_code << std::endl;
#else
		file_op::write_to_file(jni_app_mk_path, app_mk_code);
#endif // ONLY_MAKE_RUN_TEST
	}

	{
		yggr::utf8_string jni_app_mk_path(ws_dir + "/jni/Application_" + ws_ftitle + "_cpp17.mk", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type app_mk_code;

		if(!(conv_to_appliction_mk(app_mk_code, cbws_infos, "-std=c++17").size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << jni_app_mk_path << ": " << std::endl;
		std::cout << app_mk_code << std::endl;
#else
		file_op::write_to_file(jni_app_mk_path, app_mk_code);
#endif // ONLY_MAKE_RUN_TEST
	}

	{
		yggr::utf8_string jni_app_mk_path(ws_dir + "/jni/Application_" + ws_ftitle + "_cpp20.mk", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type app_mk_code;

		if(!(conv_to_appliction_mk(app_mk_code, cbws_infos, "-std=c++20").size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << jni_app_mk_path << ": " << std::endl;
		std::cout << app_mk_code << std::endl;
#else
		file_op::write_to_file(jni_app_mk_path, app_mk_code);
#endif // ONLY_MAKE_RUN_TEST
	}

	return true;
}

ptree_string_type& gen_format_jni(ptree_string_type& out, 
									const cb::cbws_infos& cbws_infos, 
									const ptree_string_type& ws_ftitle, 
									const ptree_string_type& cpp_ver)
{
	std::stringstream ss;

	ss << "#!/bin/sh\n\n"
		<< "var_ndk_root=/d/android_devel/Android/Sdk/ndk/28.0.12674087\n"
		<< "var_ndk_build_cmd=${var_ndk_root}/ndk-build.cmd\n\n"

		<< "var_android_api=21\n"
		<< "var_outdir_root=./stage\n"
		<< "var_middir_root=./stage/objs\n"
		<< "var_middir_local=${var_middir_root}/local\n"
		;

		if(4 <= ws_ftitle.size() && ws_ftitle.substr(0, 4) == "yggr")
		{
			ss << "var_outdir=${var_outdir_root}/yggr\n";
		}
		else
		{
			ss << "var_outdir=${var_outdir_root}/" << cbws_infos.cbws_title_ << "\n";
		}

		ss << "var_outdir_include=${var_outdir}/include\n"
			<< "var_outdir_lib=${var_outdir}/lib\n\n"
		;

		if(ws_ftitle == "yggr_test_only" || ws_ftitle == "yggr_test_server_client")
		{
			ss << "rm -fr ${var_middir_root}\n"
				<< "#rm -fr ${var_outdir}/bin-debug\n"
				<< "#rm -fr ${var_outdir}/bin\n\n"
				;
		}
		else
		{
			ss << "rm -fr ${var_outdir_root}\n\n"

				<< "mkdir -p \"${var_outdir_include}\"\n"
				<< "mkdir -p \"${var_outdir_lib}\"\n\n"
				;
		}

		if(ws_ftitle == "yggr_test_only" || ws_ftitle == "yggr_test_server_client")
		{
			ss << "#${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
					<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_a.mk V=1 > yggr_jni.log 2>&1\n"
				<< "#${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
					<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_a.mk\n"
				<< "#${var_ndk_build_cmd} -j12 NDK_DEBUG=0 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
					<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_a.mk\n\n"

				<< "#find ${var_middir_local} -type d -name 'objs*' -exec rm -fr {} \\;\n\n"

				<< "#cp -fr ${var_middir_local}/* ${var_outdir_lib}/\n\n"
				;
		}
		else
		{
			ss << "#${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
					<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_a.mk V=1 > yggr_jni.log 2>&1\n"
				<< "${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
					<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_a.mk\n"
				<< "${var_ndk_build_cmd} -j12 NDK_DEBUG=0 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
					<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_a.mk\n\n"

				<< "find ${var_middir_local} -type d -name 'objs*' -exec rm -fr {} \\;\n\n"

				<< "cp -fr ${var_middir_local}/* ${var_outdir_lib}/\n\n"
				;
		}

		ss << "${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
				<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_so.mk\n"
			<< "${var_ndk_build_cmd} -j12 NDK_DEBUG=0 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
				<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_so.mk\n\n"
			;

		if(ws_ftitle == "yggr_lib_only")
		{
			ss << "#${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
					<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_exe.mk\n"
				<< "#${var_ndk_build_cmd} -j12 NDK_DEBUG=0 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
					<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_exe.mk\n\n"
				;
		}
		else
		{
			ss << "${var_ndk_build_cmd} -j12 NDK_DEBUG=1 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
					<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_exe.mk\n"
				<< "${var_ndk_build_cmd} -j12 NDK_DEBUG=0 NDK_PROJECT_PATH=. NDK_APPLICATION_MK=jni/Application_" 
					<< ws_ftitle << "_" << cpp_ver << ".mk APP_BUILD_SCRIPT=jni/Android_" << ws_ftitle << "_exe.mk\n\n"
				;
		}

		ss << "touch ${var_outdir_lib}/arm64-v8a/mark-arm64-v8a.txt\n"
			<< "touch ${var_outdir_lib}/armeabi-v7a/mark-armeabi-v7a.txt\n"
			<< "touch ${var_outdir_lib}/riscv64/mark-riscv64.txt\n"
			<< "touch ${var_outdir_lib}/x86_64/mark-x86_64.txt\n"
			<< "touch ${var_outdir_lib}/x86/mark-x86.txt\n\n"
		
			<< "cp -f jni_lib_format.bat ${var_outdir}/"
			;

	std::string str = ss.str();
	ptree_string_type out_str(str.begin(), str.end());
	out.swap(out_str);
	return out;
}

bool fix_workspace_file_one_format_jni(const cb::cbws_infos& cbws_infos, const ptree_string_type& ws_dir, const ptree_string_type& ws_ftitle)
{
	{
		yggr::utf8_string file_path(ws_dir + "/build_and_format_jni_" + ws_ftitle + "_cpp11.sh", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type format_jni_code;

		if(!(gen_format_jni(format_jni_code, cbws_infos, ws_ftitle, "cpp11").size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << file_path << ": " << std::endl;
		std::cout << format_jni_code << std::endl;
#else
		std::cout << file_path << ": " << std::endl;
		file_op::write_to_file(file_path, format_jni_code);
#endif // ONLY_MAKE_RUN_TEST
	}

	{
		yggr::utf8_string file_path(ws_dir + "/build_and_format_jni_" + ws_ftitle + "_cpp14.sh", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type format_jni_code;

		if(!(gen_format_jni(format_jni_code, cbws_infos, ws_ftitle, "cpp14").size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << file_path << ": " << std::endl;
		std::cout << format_jni_code << std::endl;
#else
		std::cout << file_path << ": " << std::endl;
		file_op::write_to_file(file_path, format_jni_code);
#endif // ONLY_MAKE_RUN_TEST
	}

	{
		yggr::utf8_string file_path(ws_dir + "/build_and_format_jni_" + ws_ftitle + "_cpp17.sh", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type format_jni_code;

		if(!(gen_format_jni(format_jni_code, cbws_infos, ws_ftitle, "cpp17").size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << file_path << ": " << std::endl;
		std::cout << format_jni_code << std::endl;
#else
		std::cout << file_path << ": " << std::endl;
		file_op::write_to_file(file_path, format_jni_code);
#endif // ONLY_MAKE_RUN_TEST
	}

	{
		yggr::utf8_string file_path(ws_dir + "/build_and_format_jni_" + ws_ftitle + "_cpp20.sh", YGGR_STR_UTF8_STRING_CHARSET_NAME());

		ptree_string_type format_jni_code;

		if(!(gen_format_jni(format_jni_code, cbws_infos, ws_ftitle, "cpp20").size()))
		{
			return false;
		}

#if ONLY_MAKE_RUN_TEST()
		std::cout << file_path << ": " << std::endl;
		std::cout << format_jni_code << std::endl;
#else
		std::cout << file_path << ": " << std::endl;
		file_op::write_to_file(file_path, format_jni_code);
#endif // ONLY_MAKE_RUN_TEST
	}

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
	bool is_yggr_ws=file_op::is_yggr_sln_check(ws_dir);

	for(string_vt_type::const_iterator i = cbws_infos.cb_file_list_.begin(), isize = cbws_infos.cb_file_list_.end();
			i != isize; ++i)
	{
		std::cout << "fix cbs: " << *i << std::endl;
		yggr::string str_path = *i;
		yggr::string str_fname = file_op::get_file_name(*i);

		if(cbs_filter_list.find(yggr::utf8_string(str_fname, YGGR_STR_UTF8_STRING_CHARSET_NAME())) != cbs_filter_list.end())
		{
			std::cout << "cbs_filter_list skip " << *i << std::endl;
			continue;
		}

		if(fixed_cbs_flist_a.find(yggr::utf8_string(str_fname, YGGR_STR_UTF8_STRING_CHARSET_NAME())) != fixed_cbs_flist_a.end())
		{
			std::cout << "fixed_cbs_flist_a skip " << *i << std::endl;
			fixed_cbs_flist_a_now.insert(*i);
			continue;
		}

		if(fixed_cbs_flist_so.find(yggr::utf8_string(str_fname, YGGR_STR_UTF8_STRING_CHARSET_NAME())) != fixed_cbs_flist_so.end())
		{
			std::cout << "fixed_cbs_flist_so skip " << *i << std::endl;
			fixed_cbs_flist_so_now.insert(*i);
			continue;
		}

		if(fixed_cbs_flist_exe.find(yggr::utf8_string(str_fname, YGGR_STR_UTF8_STRING_CHARSET_NAME())) != fixed_cbs_flist_exe.end())
		{
			std::cout << "fixed_cbs_flist_exe skip " << *i << std::endl;
			fixed_cbs_flist_exe_now.insert(*i);
			continue;
		}

		yggr::utf8_string fpath(ws_dir + "/" + str_path, YGGR_STR_UTF8_STRING_CHARSET_NAME());
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

		format_cb_infos(cb_infos);

		ptree_string_type proj_mk_code;
		
		if(app_tag == cb::cb_app_tags::s_tag_static_lib())
		{
			if(!conv_to_android_mk(proj_mk_code, cb_infos, cpp_filter_list, is_yggr_ws).size())
			{
				cb_failed.insert(fpath);
				fixed_cbs_flist_a.insert(str_path);
				return;
			}
			fixed_cbs_flist_a.insert(str_path);
			fixed_cbs_flist_a_now.insert(str_path);
		}
		else if(app_tag == cb::cb_app_tags::s_tag_dll())
		{
			if(!conv_to_android_mk(proj_mk_code, cb_infos, cpp_filter_list, is_yggr_ws).size())
			{
				cb_failed.insert(fpath);
				fixed_cbs_flist_so.insert(str_path);
				return;
			}
			fixed_cbs_flist_so.insert(str_path);
			fixed_cbs_flist_so_now.insert(str_path);
		}
		else if(app_tag == cb::cb_app_tags::s_tag_exe())
		{
			if(!conv_to_android_mk(proj_mk_code, cb_infos, cpp_filter_list, is_yggr_ws).size())
			{
				cb_failed.insert(fpath);
				fixed_cbs_flist_exe.insert(str_path);
				return;
			}
			fixed_cbs_flist_exe.insert(str_path);
			fixed_cbs_flist_exe_now.insert(str_path);
		}
		else
		{
			continue;
		}

		yggr::string str_file_dir = file_op::get_file_dir(fpath.org_str());
		yggr::string str_fname_no_ext = file_op::get_file_name_no_ext(*i);
		yggr::utf8_string fpath_proj_mk(str_file_dir + "/Android_" + str_fname_no_ext.substr(0, str_fname_no_ext.size() - 6 /*"_linux"*/) + ".mk",
											YGGR_STR_UTF8_STRING_CHARSET_NAME());
#if ONLY_MAKE_RUN_TEST()
		std::cout << fpath_proj_mk << ": " << std::endl;
		std::cout << proj_mk_code << std::endl;
#else
		file_op::write_to_file(fpath_proj_mk, proj_mk_code);
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

yggr::utf8_string& make_yggr_run_test_sh(yggr::utf8_string& out_str, const file_list_type& fixed_cbs_flist_exe)
{
	std::stringstream ss;

	ss << "#!/bin/bash\n\n"

		<< "rm -f ./yggr_lua_base_type.so\n"
		<< "ln -s libyggr_lua_base_type.so yggr_lua_base_type.so\n\n"

		<< "chmod a+x *\n"
		<< "chmod a-x *.lua\n\n"

		<< "dos2unix begin_test_env_android_termux.sh\n"
		<< "dos2unix end_test_env_termux.sh\n\n"

		<< "rm -fr ../test_out\n\n"

		<< "source begin_test_env_android_termux.sh\n\n"
		;

	for(file_list_type::const_iterator i = fixed_cbs_flist_exe.begin(), isize = fixed_cbs_flist_exe.end();
			i != isize; ++i)
	{
		yggr::utf8_string exe_name = cbp_fname_2_proj_name(*i);

		if(exe_name.size() 
			&& (exe_name.org_str().find("database_system") == exe_name.org_str().npos
				&& exe_name.org_str().find("segment") == exe_name.org_str().npos
				&& exe_name.org_str().find("sql_db") == exe_name.org_str().npos))
		{
			ss << "./" << exe_name << " --batch-test\n";
		}
	}

	ss << "\n";
	
	ss << "source end_test_env_termux.sh\n"
		<< "echo \"---------all test end-----------------\"\n"
		;

	out_str = yggr::utf8_string(ss.str(), YGGR_STR_UTF8_STRING_CHARSET_NAME());

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

		if(!fix_workspace_file_one_jni_android_mk(
				cbws_infos, ws_dir, ws_ftitle, 
				fixed_cbs_flist_a_now, fixed_cbs_flist_so_now, fixed_cbs_flist_exe_now))
		{
			cbws_failed.insert(fpath);
			continue;
		}

		if(!fix_workspace_file_one_jni_appliction_mk(cbws_infos, ws_dir, ws_ftitle))
		{
			cbws_failed.insert(fpath);
			continue;
		}

		if(!fix_workspace_file_one_format_jni(cbws_infos, ws_dir, ws_ftitle))
		{
			cbws_failed.insert(fpath);
			continue;
		}

		if(ws_ftitle == "yggr_test_only")
		{
			yggr::utf8_string run_test_code;
			
			if(make_yggr_run_test_sh(run_test_code,fixed_cbs_flist_exe_now).size())
			{
				yggr::utf8_string run_test_fname = yggr::utf8_string(ws_dir.c_str()) + "/run_test_android.sh";
#if ONLY_MAKE_RUN_TEST()
				std::cout << run_test_fname << ": " << std::endl;
				std::cout << run_test_code << std::endl;
#else
				file_op::write_to_file(run_test_fname, run_test_code.org_str());
#endif // ONLY_MAKE_RUN_TEST
			}
		}

		//yggr::utf8_string jni_lib_fmt_file = file_dir + "/jni_lib_format.bat";

		//if(!local_fsys_operators_type::is_exists(local_fsys_operators_type::make_path_object(jni_lib_fmt_file)))
		//{
		//	local_fsys_operators_type::copy_file(
		//		local_fsys_operators_type::make_path_object("jni_lib_format.bat"),
		//		local_fsys_operators_type::make_path_object(jni_lib_fmt_file));
		//}
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

	tool_conv_to_jni_mk_cfg cfg_root;

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

	//{
	//	for(file_list_type::const_iterator i = need_fix_ws_files.begin(), isize = need_fix_ws_files.end();
	//		i != isize; ++i)
	//	{
	//		std::cout << *i << std::endl;
	//	}
	//}

	//if(need_fix_ws_files.find("../yggr_lib_only_linux.workspace") != need_fix_ws_files.end())
	//{
	//	std::cout << "fix yggr project" << std::endl;
	//	need_fix_ws_files.clear();
	//	need_fix_ws_files.insert("../yggr_lib_only_linux.workspace");
	//}

	////return 0;

	////need_fix_ws_files.insert("..\\yggr_lib_only_linux.workspace");

	fix_workspace_files(
		need_fix_ws_files, cfg_root.cbp_file_filter_, cfg_root.cpp_file_filter_,
		fixed_cbs_files_a, fixed_cbs_files_so, fixed_cbs_files_exe,
		ignore_cbws_files, failed_cbws_files,
		ignore_cb_files, failed_cb_files);


	write_log_file(yggr::utf8_string("tool_conv_cbp_to_jni_mk_failed_log.txt"), xml_op::fix_result::E_FAILED, failed_cb_files);
	write_log_file(yggr::utf8_string("tool_conv_cbp_to_jni_mk_ignore_log.txt"), xml_op::fix_result::E_IGNORE, ignore_cb_files);

	write_log_file(yggr::utf8_string("tool_conv_workspace_to_jni_mk_failed_log.txt"), xml_op::fix_result::E_FAILED, failed_cbws_files);
	write_log_file(yggr::utf8_string("tool_conv_workspace_to_jni_mk_ignore_log.txt"), xml_op::fix_result::E_IGNORE, ignore_cbws_files);

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

