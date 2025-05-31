// cb_parser.cpp

#include "cb_parser.hpp"

#if !(BOOST_VERSION < 105600)

#include "xml_operator.hpp"
#include "file_operator.hpp"
#include "vcproj_parser.hpp"
#include "cpp_parser.hpp"
#include "proj_tags_mapping.hpp"

#include <yggr/base/static_constant.hpp>
#include <yggr/container/set.hpp>
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/algorithm/knuth_morris_pratt.hpp>
#include <yggr/bind/bind.hpp>

#include <boost/ref.hpp>
#include <sstream>
#include <utility>
#include <cassert>
#include <cctype>

namespace cb
{

struct proj_tag_t
{
	YGGR_STATIC_CONSTANT(yggr::u32, E_TAG_YGGR_OBJ = 1 << 0);
	YGGR_STATIC_CONSTANT(yggr::u32, E_TAG_COBJ = 1 << 1);
	YGGR_STATIC_CONSTANT(yggr::u32, E_TAG_DEBUG = 1 << 2);
	YGGR_STATIC_CONSTANT(yggr::u32, E_TAG_X32 = 1 << 3);
	YGGR_STATIC_CONSTANT(yggr::u32, E_TAG_BIG_OBJ = 1 << 4);

	YGGR_STATIC_CONSTANT(yggr::u32, E_TAG_ONE_FUNC_BIG_OBJ = 1 << 23);

	YGGR_STATIC_CONSTANT(yggr::u32, E_TAG_EXE = 1 << 31);
};

inline ptree_string_type format_cfg_mark(const ptree_string_type& str_cfg_mark)
{
	if(str_cfg_mark == "Debug")
	{
		return cb_cfg_marks::s_mark_debug_x32();
	}
	else if(str_cfg_mark == "Release")
	{
		return cb_cfg_marks::s_mark_release_x32();
	}
	else if(str_cfg_mark == "Debug64")
	{
		return cb_cfg_marks::s_mark_debug_x64();
	}
	else if(str_cfg_mark == "Release64")
	{
		return cb_cfg_marks::s_mark_release_x64();
	}
	else
	{
		return str_cfg_mark;
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

inline ptree_string_type vc_outdir_to_cb_outdir(const ptree_string_type& outdir)
{
	static const ptree_string_type s_str_slndir_vc = "$(SolutionDir)";
	static const ptree_string_type s_str_slndir_cb = "$(WORKSPACE_DIR)/";

	static const ptree_string_type s_str_proj_name_vc = "$(ProjectName)";
	static const ptree_string_type s_str_proj_name_cb = "$(PROJECT_NAME)";

	static const ptree_string_type s_str_tool_ver_vc_tag = "-vc$(PlatformToolsetVersion)";
	static const ptree_string_type s_str_tool_ver_cb_tag = "$(#COMPILER_VERSION_TAG)";

	static const ptree_string_type s_str_tool_ver_vc = "vc$(PlatformToolsetVersion)";
	static const ptree_string_type s_str_tool_ver_cb = "$(#COMPILER_VERSION)";

	static const ptree_string_type s_str_cfg_mark_vc = "$(Configuration)-$(Platform)";
	static const ptree_string_type s_str_cfg_mark_cb = "$(TARGET_NAME)";

	ptree_string_type tmp = outdir;

	file_op::format_string_file_path(tmp);
	find_and_replace(tmp, s_str_slndir_vc, s_str_slndir_cb);
	find_and_replace(tmp, s_str_proj_name_vc, s_str_proj_name_cb);
	find_and_replace(tmp, s_str_tool_ver_vc_tag, s_str_tool_ver_cb_tag);
	find_and_replace(tmp, s_str_tool_ver_vc, s_str_tool_ver_cb);
	find_and_replace(tmp, s_str_cfg_mark_vc, s_str_cfg_mark_cb);

	//std::cout << "vc_outdir_to_cb_outdir: " << tmp << std::endl;

	return tmp;

}

inline ptree_string_type vc_target_name_to_cb_target_name(const ptree_string_type& target_name)
{
	static const ptree_string_type s_str_proj_name_vc = "$(ProjectName)";
	static const ptree_string_type s_str_proj_name_cb = "$(PROJECT_NAME)";

	static const ptree_string_type s_str_tool_ver_vc_tag = "-vc$(PlatformToolsetVersion)";
	static const ptree_string_type s_str_tool_ver_cb_tag = "$(#COMPILER_VERSION_TAG)";


	ptree_string_type tmp = target_name;

	find_and_replace(tmp, s_str_proj_name_vc, s_str_proj_name_cb);
	find_and_replace(tmp, s_str_tool_ver_vc_tag, s_str_tool_ver_cb_tag);

	//std::cout << "vc_target_name_to_cb_target_name: " << tmp << std::endl;

	return tmp;
}

ptree_string_type gen_output(const ptree_string_type& outdir_vc,
								const ptree_string_type& target_name_vc)
{
	ptree_string_type outdir_cb = vc_outdir_to_cb_outdir(outdir_vc);
	ptree_string_type target_name_cb = vc_target_name_to_cb_target_name(target_name_vc);


	if(outdir_cb.back() == '/')
	{
		return outdir_cb + target_name_cb;
	}
	else
	{
		return outdir_cb + "/" + target_name_cb;
	}
}


inline ptree_string_type gen_out_dir_exe(const ptree_string_type& outdir_vc,
											const ptree_string_type& target_name_vc,
											const ptree_string_type& cfg_mark)
{
	static const ptree_string_type s_str_default = "$(WORKSPACE_DIR)/$(TARGET_NAME)/$(PROJECT_NAME)";

	if(target_name_vc.empty())
	{
		return s_str_default;
	}
	else
	{
		return gen_output(outdir_vc, target_name_vc);
	}
}

inline ptree_string_type gen_out_dir_dll(const ptree_string_type& outdir_vc,
											const ptree_string_type& target_name_vc,
											const ptree_string_type& cfg_mark)
{
	static const ptree_string_type s_str_failed = "";
	//static const ptree_string_type s_str_default = "$(WORKSPACE_DIR)/$(TARGET_NAME)/$(PROJECT_NAME)";

	static const ptree_string_type s_str_debug_x32 = "$(WORKSPACE_DIR)/bin/$(PROJECT_NAME)$(#COMPILER_VERSION_TAG)-d";
	static const ptree_string_type s_str_release_x32 = "$(WORKSPACE_DIR)/bin/$(PROJECT_NAME)$(#COMPILER_VERSION_TAG)";
	static const ptree_string_type s_str_debug_x64 = "$(WORKSPACE_DIR)/bin64/$(PROJECT_NAME)$(#COMPILER_VERSION_TAG)-d";
	static const ptree_string_type s_str_release_x64 = "$(WORKSPACE_DIR)/bin64/$(PROJECT_NAME)$(#COMPILER_VERSION_TAG)";

	if(target_name_vc.empty())
	{
		if(cb_cfg_marks::s_mark_debug_x32() == cfg_mark)
		{
			return s_str_debug_x32;
		}
		else if(cb_cfg_marks::s_mark_release_x32() == cfg_mark)
		{
			return s_str_release_x32;
		}
		else if(cb_cfg_marks::s_mark_debug_x64() == cfg_mark)
		{
			return s_str_debug_x64;
		}
		else if(cb_cfg_marks::s_mark_release_x64() == cfg_mark)
		{
			return s_str_release_x64;
		}
		else
		{
			return s_str_failed;
		}
	}
	else
	{
		return gen_output(outdir_vc, target_name_vc);
	}
}

inline ptree_string_type gen_out_dir_static_lib(const ptree_string_type& outdir_vc,
												const ptree_string_type& target_name_vc,
												const ptree_string_type& cfg_mark)
{
	static const ptree_string_type s_str_failed = "";

	static const ptree_string_type s_str_debug_x32 = "$(WORKSPACE_DIR)/lib/$(PROJECT_NAME)$(#COMPILER_VERSION_TAG)-d";
	static const ptree_string_type s_str_release_x32 = "$(WORKSPACE_DIR)/lib/$(PROJECT_NAME)$(#COMPILER_VERSION_TAG)";
	static const ptree_string_type s_str_debug_x64 = "$(WORKSPACE_DIR)/lib64/$(PROJECT_NAME)$(#COMPILER_VERSION_TAG)-d";
	static const ptree_string_type s_str_release_x64 = "$(WORKSPACE_DIR)/lib64/$(PROJECT_NAME)$(#COMPILER_VERSION_TAG)";

	if(target_name_vc.empty())
	{
		if(cb_cfg_marks::s_mark_debug_x32() == cfg_mark)
		{
			return s_str_debug_x32;
		}
		else if(cb_cfg_marks::s_mark_release_x32() == cfg_mark)
		{
			return s_str_release_x32;
		}
		else if(cb_cfg_marks::s_mark_debug_x64() == cfg_mark)
		{
			return s_str_debug_x64;
		}
		else if(cb_cfg_marks::s_mark_release_x64() == cfg_mark)
		{
			return s_str_release_x64;
		}
		else
		{
			return s_str_failed;
		}
	}
	else
	{
		return gen_output(outdir_vc, target_name_vc);
	}
}

inline const ptree_string_type& gen_out_dir_impl_lib(const ptree_string_type& cfg_mark)
{
	static const ptree_string_type s_str_out_x32 = "$(WORKSPACE_DIR)/lib/$(TARGET_OUTPUT_FILENAME)";
	static const ptree_string_type s_str_out_x64 = "$(WORKSPACE_DIR)/lib64/$(TARGET_OUTPUT_FILENAME)";

	if(cb_cfg_marks::s_is_x32_mark(cfg_mark))
	{
		return s_str_out_x32;
	}
	else
	{
		return s_str_out_x64;
	}
}

inline const ptree_string_type& gen_target_opt_compiler(const ptree_string_type& cfg_mark)
{
	static const ptree_string_type s_str_failed = "";

	static const ptree_string_type s_str_x32 = "gnu_gcc32_compiler";
	static const ptree_string_type s_str_x64 = "gcc";

	if((cb_cfg_marks::s_mark_debug_x32() == cfg_mark)
			|| (cb_cfg_marks::s_mark_release_x32() == cfg_mark))
	{
		return s_str_x32;
	}
	else if((cb_cfg_marks::s_mark_debug_x64() == cfg_mark)
				|| (cb_cfg_marks::s_mark_release_x64() == cfg_mark))
	{
		return s_str_x64;
	}
	else
	{
		return s_str_failed;
	}
}


yggr::u32 fix_cb_proj_build_target_option(ptree_type& cb_tree, 
											const ptree_type& vc_ptree,
											const ptree_string_type& cb_app_tag,
											const ptree_string_type& vc_app_tag,
											const ptree_string_type& cb_cfg_mark,
											const ptree_string_type& vc_cfg_mark)
{
	if(xml_op::has_attribs(cb_tree, "output"))
	{
		if(cb_app_tag == cb_app_tags::s_tag_exe())
		{
			ptree_string_type str_outdir_vc;
			ptree_string_type str_target_name_vc;
			ptree_string_type str_target_ext_vc;

			vcproj::get_outdir_and_target_name_and_target_ext(
				str_outdir_vc, str_target_name_vc, str_target_ext_vc,
				vc_ptree, vc_app_tag, vc_cfg_mark);

			ptree_string_type str_output = gen_out_dir_exe(str_outdir_vc, str_target_name_vc, cb_cfg_mark);

			bool auto_ext = str_target_ext_vc.empty() || str_target_ext_vc ==".exe";

			if(!auto_ext)
			{
				str_output += str_target_ext_vc;
			}

			if(!(str_output.size()
					&& xml_op::set_attribs(cb_tree, "output", str_output)
					&& xml_op::set_attribs_force(cb_tree, "prefix_auto", "1")
					&& xml_op::set_attribs_force(cb_tree, "extension_auto", auto_ext? "1" : "0")))
			{
				return xml_op::fix_result::E_FAILED;
			}

		}
		else if(cb_app_tag == cb_app_tags::s_tag_dll())
		{
			ptree_string_type str_outdir_vc;
			ptree_string_type str_target_name_vc;
			ptree_string_type str_target_ext_vc;

			vcproj::get_outdir_and_target_name_and_target_ext(
				str_outdir_vc, str_target_name_vc, str_target_ext_vc,
				vc_ptree, vc_app_tag, vc_cfg_mark);

			ptree_string_type str_impl_lib_vc = vcproj::get_impl_lib(vc_ptree, vc_app_tag, vc_cfg_mark);

			ptree_string_type str_output = gen_out_dir_dll(str_outdir_vc, str_target_name_vc, cb_cfg_mark);

			bool auto_ext = str_target_ext_vc.empty() || str_target_ext_vc ==".dll";

			if(!auto_ext)
			{
				str_output += str_target_ext_vc;
			}

			if(!(str_output.size()
					&& xml_op::set_attribs(cb_tree, "output", str_output)
					&& xml_op::set_attribs_force(cb_tree, "prefix_auto", "1")
					&& xml_op::set_attribs_force(cb_tree, "extension_auto", auto_ext? "1" : "0")))
			{
				return xml_op::fix_result::E_FAILED;
			}

			if(str_impl_lib_vc.size())
			{
				const ptree_string_type& str_impl_lib = gen_out_dir_impl_lib(cb_cfg_mark);
				if(!(xml_op::set_attribs_force(cb_tree, "prefix_auto", "imp_lib", str_impl_lib)
						&& xml_op::set_attribs_force(cb_tree, "prefix_auto", "def_file", "$(TARGET_OUTPUT_DIR)$(TARGET_OUTPUT_BASENAME)") ))
				{
					return xml_op::fix_result::E_FAILED;
				}
			}
		}
		else if(cb_app_tag == cb_app_tags::s_tag_static_lib())
		{
			ptree_string_type str_outdir_vc;
			ptree_string_type str_target_name_vc;
			ptree_string_type str_target_ext_vc;

			vcproj::get_outdir_and_target_name_and_target_ext(
				str_outdir_vc, str_target_name_vc, str_target_ext_vc,
				vc_ptree, vc_app_tag, vc_cfg_mark);

			ptree_string_type str_output = gen_out_dir_static_lib(str_outdir_vc, str_target_name_vc, cb_cfg_mark);

			bool auto_ext = str_target_ext_vc.empty() || str_target_ext_vc ==".lib";

			if(!auto_ext)
			{
				str_output += str_target_ext_vc;
			}

			if(!(str_output.size()
					&& xml_op::set_attribs(cb_tree, "output", str_output)
					&& xml_op::set_attribs_force(cb_tree, "prefix_auto", "1")
					&& xml_op::set_attribs_force(cb_tree, "extension_auto", auto_ext? "1" : "0")))
			{
				return xml_op::fix_result::E_FAILED;
			}
		}
		else
		{
			return xml_op::fix_result::E_IGNORE;
		}
	}
	else if(xml_op::has_attribs(cb_tree, "working_dir"))
	{
		return xml_op::fix_result::E_IGNORE;
	}
	else if(xml_op::has_attribs(cb_tree, "object_output"))
	{
		if(cb_cfg_mark == cb_app_tags::s_tag_utility())
		{
			return xml_op::fix_result::E_IGNORE;
		}
		else
		{
			if(!xml_op::set_attribs(cb_tree, "object_output", "$(TARGET_NAME)/$(PROJECT_NAME)$(#COMPILER_VERSION_TAG)"))
			{
				return xml_op::fix_result::E_FAILED;
			}
		}
	}
	else if(xml_op::has_attribs(cb_tree, "type"))
	{
		if(!xml_op::set_attribs(cb_tree, "type", cb_app_tag))
		{
			return xml_op::fix_result::E_FAILED;
		}
	}
	else if(xml_op::has_attribs(cb_tree, "compiler"))
	{
		const ptree_string_type& str_cc = gen_target_opt_compiler(cb_cfg_mark);

		if(!xml_op::set_attribs(cb_tree, "compiler", str_cc))
		{
			return xml_op::fix_result::E_FAILED;
		}
	}
	else
	{
		return xml_op::fix_result::E_SUCCEED;
	}

	return xml_op::fix_result::E_SUCCEED;
}

yggr::u32 fix_cb_proj_build_target_compiler_detail(ptree_type& cb_tree,
													yggr::u32 proj_tag,
													const string_set_type& pp,
													const string_set_type& inc_dirs)
{
	typedef string_set_type::const_iterator string_set_citer_type;
	bool is_yggr_proj = !!(proj_tag & proj_tag_t::E_TAG_YGGR_OBJ);
	bool is_c_proj = !!(proj_tag & proj_tag_t::E_TAG_COBJ);
	bool is_debug = !!(proj_tag & proj_tag_t::E_TAG_DEBUG);
	bool is_x32 = !!(proj_tag & proj_tag_t::E_TAG_X32);
	bool is_big_obj = !!(proj_tag & proj_tag_t::E_TAG_BIG_OBJ);
	bool is_one_func_big_obj =  !!(proj_tag & proj_tag_t::E_TAG_ONE_FUNC_BIG_OBJ );

	if(is_x32)
	{
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-m32"));
	}
	else
	{
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-m64"));
	}

	if(is_debug)
	{
		if(is_big_obj)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Og"));
		}
		else
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-O0"));
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-g"));
		}
		
		if(!(is_c_proj || (is_one_func_big_obj && is_x32)))
		{
			//cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-flarge-source-files"));
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-fno-elide-constructors"));
		}

		if(is_yggr_proj)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Werror=unused-variable"));
		}

	}
	else
	{
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-O3"));
	}

	{
		for(string_set_citer_type i = pp.begin(), isize = pp.end(); i != isize; ++i)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-D" + *i));
		}
	}

	{
		for(string_set_citer_type i = inc_dirs.begin(), isize = inc_dirs.end(); i != isize; ++i)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.directory", ptree_type(*i));
		}
	}

	return xml_op::fix_result::E_SUCCEED;
}

yggr::u32 fix_cb_proj_build_target_compiler(ptree_type& cb_tree,
											yggr::u32 proj_tag,
											const ptree_string_type& app_tag,
											const ptree_string_type& cfg_mark,
											const string_set_type& pp_d32, const string_set_type& pp_r32,
											const string_set_type& pp_d64, const string_set_type& pp_r64,
											const string_set_type& inc_dirs_d32, const string_set_type& inc_dirs_r32,
											const string_set_type& inc_dirs_d64, const string_set_type& inc_dirs_r64)
{
	cb_tree.clear();

	if(app_tag == cb_app_tags::s_tag_exe()
		|| app_tag == cb_app_tags::s_tag_dll()
		|| app_tag == cb_app_tags::s_tag_static_lib())
	{
		if(cfg_mark == cb_cfg_marks::s_mark_debug_x32())
		{
			yggr::u32 now_tag = (proj_tag | (proj_tag_t::E_TAG_DEBUG |  proj_tag_t::E_TAG_X32));
			return fix_cb_proj_build_target_compiler_detail(cb_tree, now_tag, pp_d32, inc_dirs_d32);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_release_x32())
		{
			yggr::u32 now_tag = (proj_tag | proj_tag_t::E_TAG_X32);
			return fix_cb_proj_build_target_compiler_detail(cb_tree, now_tag, pp_r32, inc_dirs_r32);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_debug_x64())
		{
			yggr::u32 now_tag = (proj_tag | proj_tag_t::E_TAG_DEBUG);
			return fix_cb_proj_build_target_compiler_detail(cb_tree, now_tag, pp_d64, inc_dirs_d64);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_release_x64())
		{
			yggr::u32 now_tag = proj_tag;
			return fix_cb_proj_build_target_compiler_detail(cb_tree, now_tag, pp_r64, inc_dirs_r64);
		}
		else
		{
			return xml_op::fix_result::E_FAILED;
		}
	}
	else if(app_tag == cb_app_tags::s_tag_utility())
	{
		string_set_type tmp1, tmp2;

		if(cfg_mark == cb_cfg_marks::s_mark_debug_x32())
		{
			yggr::u32 now_tag = 
				(proj_tag_t::E_TAG_YGGR_OBJ 
					| proj_tag_t::E_TAG_COBJ 
					| proj_tag_t::E_TAG_DEBUG
					| proj_tag_t::E_TAG_X32);
			return fix_cb_proj_build_target_compiler_detail(cb_tree, now_tag, tmp1, tmp2);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_release_x32())
		{
			yggr::u32 now_tag = 
				(proj_tag_t::E_TAG_YGGR_OBJ 
					| proj_tag_t::E_TAG_COBJ 
					| proj_tag_t::E_TAG_X32);
			return fix_cb_proj_build_target_compiler_detail(cb_tree, now_tag, tmp1, tmp2);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_debug_x64())
		{
			yggr::u32 now_tag = 
				(proj_tag_t::E_TAG_YGGR_OBJ 
					| proj_tag_t::E_TAG_COBJ 
					| proj_tag_t::E_TAG_DEBUG);
			return fix_cb_proj_build_target_compiler_detail(cb_tree, now_tag, tmp1, tmp2);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_release_x64())
		{
			yggr::u32 now_tag = 
				(proj_tag_t::E_TAG_YGGR_OBJ 
					| proj_tag_t::E_TAG_COBJ);
			return fix_cb_proj_build_target_compiler_detail(cb_tree, now_tag, tmp1, tmp2);
		}
		else
		{
			return xml_op::fix_result::E_FAILED;
		}
	}
	else
	{
		return xml_op::fix_result::E_FAILED;
	}
}

yggr::u32 fix_cb_proj_build_target_linker_detail(ptree_type& cb_tree,
													yggr::u32 proj_tag,
													const ptree_string_type& str_def_file,
													const string_set_type& lib_dirs,
													const lib::ordered_libname_map_type& lib_names)
{
	typedef string_set_type::const_iterator string_set_citer_type;

	typedef lib::ordered_libname_map_type ordered_libname_map_type;
	typedef ordered_libname_map_type::const_reverse_iterator ordered_libname_map_criter_type;

	bool is_debug = !!(proj_tag & proj_tag_t::E_TAG_DEBUG);
	bool is_x32 = !!(proj_tag & proj_tag_t::E_TAG_X32);
	bool need_session_fix = !!(proj_tag & proj_tag_t::E_TAG_EXE);
	bool is_one_func_bij_obj = !!(proj_tag & proj_tag_t::E_TAG_ONE_FUNC_BIG_OBJ );

	if(is_x32)
	{
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-m32"));
		if(is_debug && is_one_func_bij_obj)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-s"));
		}
	}
	else
	{
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-m64"));
	}

	if(!is_debug)
	{
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-s"));
		if(need_session_fix)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Wl,--gc-sections"));
		}
	}

	// --def xxx.def

	if(str_def_file.size())
	{
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("--def " + str_def_file));
	}

	// lib_names
	{
		for(ordered_libname_map_criter_type i = lib_names.rbegin(), isize = lib_names.rend(); i != isize; ++i)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type(i->second));
		}
	}

	// lib_dirs
	{
		for(string_set_citer_type i = lib_dirs.begin(), isize = lib_dirs.end(); i != isize; ++i)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.directory", ptree_type(*i));
		}
	}

	return xml_op::fix_result::E_SUCCEED;
}

yggr::u32 fix_cb_proj_build_target_linker(ptree_type& cb_tree,
											yggr::u32 proj_tag,
											const ptree_string_type& app_tag,
											const ptree_string_type& cfg_mark,
											const ptree_string_type& str_def_file,
											const string_set_type& lib_dirs_d32, const string_set_type& lib_dirs_r32,
											const string_set_type& lib_dirs_d64, const string_set_type& lib_dirs_r64,
											const lib::ordered_libname_map_type& lib_names_d32, const lib::ordered_libname_map_type& lib_names_r32,
											const lib::ordered_libname_map_type& lib_names_d64, const lib::ordered_libname_map_type& lib_names_r64)
{
	cb_tree.clear();

	ptree_string_type now_str_def_file =
		(app_tag == cb_app_tags::s_tag_dll())? str_def_file: ptree_string_type();
	
	//bool need_session_fix = (app_tag == cb_app_tags::s_tag_exe());

	//if(app_tag == cb_app_tags::s_tag_exe())
	//{
	//	proj_tag |= proj_tag_t::E_TAG_EXE;
	//}


	if(app_tag == cb_app_tags::s_tag_exe()
		|| app_tag == cb_app_tags::s_tag_dll()
		|| app_tag == cb_app_tags::s_tag_static_lib())
	{
		if(cfg_mark == cb_cfg_marks::s_mark_debug_x32())
		{
			yggr::u32 now_tag = proj_tag | (proj_tag_t::E_TAG_DEBUG | proj_tag_t::E_TAG_X32);
			return fix_cb_proj_build_target_linker_detail(cb_tree, now_tag, now_str_def_file, lib_dirs_d32, lib_names_d32);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_release_x32())
		{
			yggr::u32 now_tag = proj_tag | (proj_tag_t::E_TAG_X32);
			return fix_cb_proj_build_target_linker_detail(cb_tree, now_tag, now_str_def_file, lib_dirs_r32, lib_names_r32);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_debug_x64())
		{
			yggr::u32 now_tag = proj_tag | (proj_tag_t::E_TAG_DEBUG);
			return fix_cb_proj_build_target_linker_detail(cb_tree, now_tag, now_str_def_file, lib_dirs_d64, lib_names_d64);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_release_x64())
		{
			yggr::u32 now_tag = proj_tag;
			return fix_cb_proj_build_target_linker_detail(cb_tree, now_tag, now_str_def_file, lib_dirs_r64, lib_names_r64);
		}
		else
		{
			return xml_op::fix_result::E_FAILED;
		}
	}
	else if(app_tag == cb_app_tags::s_tag_utility())
	{
		string_set_type tmp1;
		lib::ordered_libname_map_type tmp2;

		if(cfg_mark == cb_cfg_marks::s_mark_debug_x32())
		{
			yggr::u32 now_tag = (proj_tag_t::E_TAG_DEBUG | proj_tag_t::E_TAG_X32);
			return fix_cb_proj_build_target_linker_detail(cb_tree, now_tag, now_str_def_file, tmp1, tmp2);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_release_x32())
		{
			yggr::u32 now_tag = (proj_tag_t::E_TAG_X32);
			return fix_cb_proj_build_target_linker_detail(cb_tree, now_tag, now_str_def_file, tmp1, tmp2);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_debug_x64())
		{
			yggr::u32 now_tag = (proj_tag_t::E_TAG_DEBUG);
			return fix_cb_proj_build_target_linker_detail(cb_tree, now_tag, now_str_def_file, tmp1, tmp2);
		}
		else if(cfg_mark == cb_cfg_marks::s_mark_release_x64())
		{
			return fix_cb_proj_build_target_linker_detail(cb_tree, 0, now_str_def_file, tmp1, tmp2);
		}
		else
		{
			return xml_op::fix_result::E_FAILED;
		}
	}
	else
	{
		return xml_op::fix_result::E_FAILED;
	}
}

yggr::u32 fix_cb_proj_build_target(ptree_type& cb_tree, const ptree_type& vc_ptree,
									yggr::u32 proj_tag,
									const string_set_type& pp_d32, const string_set_type& pp_r32,
									const string_set_type& pp_d64, const string_set_type& pp_r64,
									const string_set_type& inc_dirs_d32, const string_set_type& inc_dirs_r32,
									const string_set_type& inc_dirs_d64, const string_set_type& inc_dirs_r64,
									const string_set_type& lib_dirs_d32, const string_set_type& lib_dirs_r32,
									const string_set_type& lib_dirs_d64, const string_set_type& lib_dirs_r64,
									const lib::ordered_libname_map_type& lib_names_d32, const lib::ordered_libname_map_type& lib_names_r32,
									const lib::ordered_libname_map_type& lib_names_d64, const lib::ordered_libname_map_type& lib_names_r64)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::value_type ptree_val_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;
	typedef boost::optional<const ptree_type&> const_attribs_type;
	typedef std::pair<assoc_iter_type, assoc_iter_type> assoc_iter_pair_type;

	ptree_string_type cb_cfg_mark = format_cfg_mark(xml_op::get_attribs(cb_tree, "title"));
	std::pair<ptree_string_type, bool> vc_cfg_mark_pair = mapping::get_cfg_marks_mapping().cb_to_vc(cb_cfg_mark);

	if(!vc_cfg_mark_pair.second)
	{
		return xml_op::fix_result::E_FAILED;
	}

	ptree_string_type vc_app_tag = vcproj::get_app_tag(vc_ptree);
	std::pair<ptree_string_type, bool> cb_app_tag_pair = mapping::get_app_tags_mapping().vc_to_cb(vc_app_tag);

	if(!cb_app_tag_pair.second)
	{
		return xml_op::fix_result::E_FAILED;
	}

	yggr::u32 sat = xml_op::fix_result::E_FAILED;

	if(cb_tree.find("Compiler") == cb_tree.not_found())
	{
		cb_tree.add_child("Compiler", ptree_type());
	}

	if(cb_tree.find("Linker") == cb_tree.not_found())
	{
		cb_tree.add_child("Linker", ptree_type());
	}

	bool is_dll = (cb_app_tag_pair.first == cb_app_tags::s_tag_dll());
	bool need_add_impl_lib = false;
	string_type str_def_file;


	if(vcproj::has_big_obj(vc_ptree, vc_app_tag, vc_cfg_mark_pair.first))
	{
		proj_tag |= proj_tag_t::E_TAG_BIG_OBJ;
	}

	for(iterator i = cb_tree.begin(), isize = cb_tree.end(); i != isize;)
	{
		if(i->first == "Option")
		{
			switch((sat = fix_cb_proj_build_target_option(
							i->second, vc_ptree,
							cb_app_tag_pair.first, vc_app_tag,
							cb_cfg_mark, vc_cfg_mark_pair.first)))
			{
			case xml_op::fix_result::E_SUCCEED:
				if(is_dll
					&& xml_op::has_attribs(i->second, "output")
					&& xml_op::has_attribs(i->second, "imp_lib"))
				{
					need_add_impl_lib = true;
				}
				++i;
				break;
			case xml_op::fix_result::E_FAILED:
				return sat;
			case xml_op::fix_result::E_IGNORE:
				i = cb_tree.erase(i);
				break;
			default:
				assert(false);
				return xml_op::fix_result::E_FAILED;
			};

			continue;
		}

		if(i->first == "Compiler")
		{
			if(need_add_impl_lib)
			{
				cb_tree.insert(i, ptree_val_type("Option", ptree_type()))->second.add_child("<xmlattr>.createStaticLib", ptree_type("1"));
			}

			cb_tree.insert(i, ptree_val_type("Option", ptree_type()))->second.add_child("<xmlattr>.projectLinkerOptionsRelation", ptree_type("2"));

			if((sat = fix_cb_proj_build_target_compiler(
						i->second,
						proj_tag,
						cb_app_tag_pair.first, cb_cfg_mark,
						pp_d32, pp_r32, pp_d64, pp_r64,
						inc_dirs_d32, inc_dirs_r32, inc_dirs_d64, inc_dirs_r64)) != xml_op::fix_result::E_SUCCEED)
			{
				return xml_op::fix_result::E_FAILED;
			}

			++i;
			continue;
		}

		if(i->first == "Linker")
		{
			if(is_dll)
			{
				str_def_file = vcproj::get_module_definition_file(vc_ptree, vc_app_tag, vc_cfg_mark_pair.first);
				file_op::format_string_file_path(str_def_file);
			}

			if((sat = fix_cb_proj_build_target_linker(
						i->second,
						proj_tag,
						cb_app_tag_pair.first, cb_cfg_mark,
						str_def_file,
						lib_dirs_d32, lib_dirs_r32, lib_dirs_d64, lib_dirs_r64,
						lib_names_d32, lib_names_r32, lib_names_d64, lib_names_r64)) != xml_op::fix_result::E_SUCCEED)
			{
				return xml_op::fix_result::E_FAILED;
			}

			if(i->second.empty())
			{
				i = cb_tree.erase(i);
			}
			else
			{
				++i;
			}
			continue;
		}

		++i;
	}

	return xml_op::fix_result::E_SUCCEED;
}


yggr::u32 fix_cb_proj_build(ptree_type& cb_tree, const ptree_type& vc_ptree,
								yggr::u32 proj_tag,
								const string_set_type& pp_d32, const string_set_type& pp_r32,
								const string_set_type& pp_d64, const string_set_type& pp_r64,
								const string_set_type& inc_dirs_d32, const string_set_type& inc_dirs_r32,
								const string_set_type& inc_dirs_d64, const string_set_type& inc_dirs_r64,
								const string_set_type& lib_dirs_d32, const string_set_type& lib_dirs_r32,
								const string_set_type& lib_dirs_d64, const string_set_type& lib_dirs_r64,
								const lib::ordered_libname_map_type& lib_names_d32, const lib::ordered_libname_map_type& lib_names_r32,
								const lib::ordered_libname_map_type& lib_names_d64, const lib::ordered_libname_map_type& lib_names_r64)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;
	typedef boost::optional<const ptree_type&> const_attribs_type;
	typedef std::pair<assoc_iter_type, assoc_iter_type> assoc_iter_pair_type;

	yggr::u32 sat = xml_op::fix_result::E_FAILED;
	for(iterator i = cb_tree.begin(), isize = cb_tree.end(); i != isize;)
	{
		if(i->first == "Target")
		{
			if((sat =
					fix_cb_proj_build_target(
						i->second, vc_ptree,
						proj_tag,
						pp_d32, pp_r32, pp_d64, pp_r64,
						inc_dirs_d32, inc_dirs_r32, inc_dirs_d64, inc_dirs_r64,
						lib_dirs_d32, lib_dirs_r32, lib_dirs_d64, lib_dirs_r64,
						lib_names_d32, lib_names_r32, lib_names_d64, lib_names_r64)) != xml_op::fix_result::E_SUCCEED)
			{
				return sat;
			}
			++i;
		}
		else
		{
			i = cb_tree.erase(i);
		}
	}

	return xml_op::fix_result::E_SUCCEED;
}

yggr::u32 fix_cb_proj_option(ptree_type& cb_tree, const ptree_type& vc_ptree)
{
	if(xml_op::has_attribs(cb_tree, "title"))
	{
		ptree_string_type str_title = vcproj::get_root_namespace(vc_ptree);

		return
			(str_title.size() && xml_op::set_attribs(cb_tree, "title", str_title))?
				xml_op::fix_result::E_SUCCEED
				: xml_op::fix_result::E_FAILED;
	}
	else if(xml_op::has_attribs(cb_tree, "pch_mode"))
	{
		return xml_op::fix_result::E_SUCCEED;
	}
	else if(xml_op::has_attribs(cb_tree, "compiler"))
	{
		return
			(xml_op::set_attribs(cb_tree, "compiler", "gcc"))?
				xml_op::fix_result::E_SUCCEED
				: xml_op::fix_result::E_FAILED;
	}
	else
	{
		return xml_op::fix_result::E_IGNORE;
	}
}

void category_opt_items(string_set_type& glo_set,
						string_set_type& d32_set, string_set_type& r32_set,
						string_set_type& d64_set, string_set_type& r64_set)
{
	typedef string_set_type::iterator string_set_iter_type;
	string_set_type tmp;
	tmp.insert(d32_set.begin(), d32_set.end());
	tmp.insert(r32_set.begin(), r32_set.end());
	tmp.insert(d64_set.begin(), d64_set.end());
	tmp.insert(r64_set.begin(), r64_set.end());

	for(string_set_iter_type i = tmp.begin(), isize = tmp.end(); i != isize;)
	{
		if((d32_set.find(*i) != d32_set.end())
			&& (r32_set.find(*i) != r32_set.end())
			&& (d64_set.find(*i) != d64_set.end())
			&& (r64_set.find(*i) != r64_set.end()) )
		{
			d32_set.erase(*i);
			r32_set.erase(*i);
			d64_set.erase(*i);
			r64_set.erase(*i);
			++i;
		}
		else
		{
			i = tmp.erase(i);
		}
	}

	if(tmp.size())
	{
		tmp.swap(glo_set);
	}
}

void remove_opt_items_pp(const ptree_type& vc_ptree,
							string_set_type& glo_set,
							string_set_type& d32_set, string_set_type& r32_set,
							string_set_type& d64_set, string_set_type& r64_set)
{

#define ERASE_PP( __set__ ) \
	__set__.erase("_DEBUG"); \
	__set__.erase("NDEBUG"); \
	__set__.erase("WIN32"); \
	__set__.erase("WIN64"); \
	__set__.erase("_LIB"); \
	__set__.erase("_USRDLL"); \
	__set__.erase("_CONSOLE"); \
	__set__.erase("_WINDOWS");

	ERASE_PP(glo_set);
	ERASE_PP(d32_set);
	ERASE_PP(r32_set);
	ERASE_PP(d64_set);
	ERASE_PP(r64_set);

#undef ERASE_PP

}

void format_opt_items_pp_exe(const ptree_type& vc_ptree,
								string_set_type& glo_set,
								string_set_type& d32_set, string_set_type& r32_set,
								string_set_type& d64_set, string_set_type& r64_set)
{
	ptree_string_type subsys_d32 =
		vcproj::get_subsystem(
			vc_ptree, vcproj::vcproj_app_tags::s_tag_exe(), vcproj::vcproj_cfg_marks::s_mark_debug_x32());

	ptree_string_type subsys_r32 =
		vcproj::get_subsystem(
			vc_ptree, vcproj::vcproj_app_tags::s_tag_exe(), vcproj::vcproj_cfg_marks::s_mark_release_x32());

	ptree_string_type subsys_d64 =
		vcproj::get_subsystem(
			vc_ptree, vcproj::vcproj_app_tags::s_tag_exe(), vcproj::vcproj_cfg_marks::s_mark_debug_x64());

	ptree_string_type subsys_r64 =
		vcproj::get_subsystem(
			vc_ptree, vcproj::vcproj_app_tags::s_tag_exe(), vcproj::vcproj_cfg_marks::s_mark_release_x64());

	bool is_same_subsys =
		((subsys_d32 == subsys_r32)
			&& (subsys_d32 == subsys_d64)
			&& (subsys_d32 == subsys_r64));

	static const ptree_string_type s_str_macro_subsys_console = "_CONSOLE";
	static const ptree_string_type s_str_macro_subsys_windows = "_WINDOWS";

	remove_opt_items_pp(vc_ptree, glo_set, d32_set, r32_set, d64_set, r64_set);

	// glo_set
	{
		glo_set.insert("WIN32");

		if(is_same_subsys)
		{
			if(subsys_d32 == "Console")
			{
				glo_set.insert(s_str_macro_subsys_console);
			}
			else
			{
				glo_set.insert(s_str_macro_subsys_windows);
			}
		}
		else
		{
			glo_set.erase(s_str_macro_subsys_console);
			glo_set.erase(s_str_macro_subsys_windows);
		}
	}

	// d32_set
	{
		d32_set.insert("_DEBUG");

		if(!is_same_subsys)
		{
			if(subsys_d32 == "Console")
			{
				d32_set.insert(s_str_macro_subsys_console);
			}
			else
			{
				d32_set.insert(s_str_macro_subsys_windows);
			}
		}
	}

	// r32_set
	{
		r32_set.insert("NDEBUG");

		if(!is_same_subsys)
		{
			if(subsys_r32 == "Console")
			{
				r32_set.insert(s_str_macro_subsys_console);
			}
			else
			{
				r32_set.insert(s_str_macro_subsys_windows);
			}
		}
	}

	// d64_set
	{
		d64_set.insert("_DEBUG");
		d64_set.insert("WIN64");

		if(!is_same_subsys)
		{
			if(subsys_d64 == "Console")
			{
				d64_set.insert(s_str_macro_subsys_console);
			}
			else
			{
				d64_set.insert(s_str_macro_subsys_windows);
			}
		}
	}

	{
		r64_set.insert("NDEBUG");
		r64_set.insert("WIN64");

		if(!is_same_subsys)
		{
			if(subsys_r64 == "Console")
			{
				r64_set.insert(s_str_macro_subsys_console);
			}
			else
			{
				r64_set.insert(s_str_macro_subsys_windows);
			}
		}
	}
}

ptree_string_type make_pp_dll_exprot(const ptree_string_type& proj_name)
{
	typedef ptree_string_type::value_type char_type;
	if(proj_name.empty())
	{
		return ptree_string_type();
	}

	ptree_string_type tmp = proj_name + "_EXPORTS";

	for(ptree_string_type::iterator i = tmp.begin(), isize = tmp.begin() + proj_name.size(); i != isize; ++i)
	{
		*i = static_cast<char_type>(std::toupper(*i));
	}

	return tmp;
}

void format_opt_items_pp_dll(const ptree_type& vc_ptree,
								string_set_type& glo_set,
								string_set_type& d32_set, string_set_type& r32_set,
								string_set_type& d64_set, string_set_type& r64_set)
{
	ptree_string_type proj_name = vcproj::get_root_namespace(vc_ptree);
	ptree_string_type pp_export = proj_name.size()? make_pp_dll_exprot(proj_name) : ptree_string_type();

	remove_opt_items_pp(vc_ptree, glo_set, d32_set, r32_set, d64_set, r64_set);

	// glo_set
	{
		glo_set.insert("WIN32");

		glo_set.insert("_WINDOWS");
		glo_set.insert("_USRDLL");

		if(pp_export.size())
		{
			glo_set.insert(pp_export);
		}
	}

	// d32_set
	{
		d32_set.insert("_DEBUG");

		if(pp_export.size())
		{
			glo_set.erase(pp_export);
		}
	}

	// r32_set
	{
		r32_set.insert("NDEBUG");

		if(pp_export.size())
		{
			glo_set.erase(pp_export);
		}
	}

	// d64_set
	{
		d64_set.insert("_DEBUG");
		d64_set.insert("WIN64");

		if(pp_export.size())
		{
			glo_set.erase(pp_export);
		}
	}

	{
		r64_set.insert("NDEBUG");
		r64_set.insert("WIN64");

		if(pp_export.size())
		{
			glo_set.erase(pp_export);
		}
	}
}

void format_opt_items_pp_static_lib(const ptree_type& vc_ptree,
									string_set_type& glo_set,
									string_set_type& d32_set, string_set_type& r32_set,
									string_set_type& d64_set, string_set_type& r64_set)
{
	remove_opt_items_pp(vc_ptree, glo_set, d32_set, r32_set, d64_set, r64_set);
	// glo_set
	{
		glo_set.insert("WIN32");
		glo_set.insert("_LIB");
	}

	// d32_set
	{
		d32_set.insert("_DEBUG");
	}

	// r32_set
	{
		r32_set.insert("NDEBUG");
	}

	// d64_set
	{
		d64_set.insert("_DEBUG");
		d64_set.insert("WIN64");
	}

	{
		r64_set.insert("NDEBUG");
		r64_set.insert("WIN64");
	}
}

void split_pp(const ptree_type& vc_ptree, const ptree_string_type& vc_app_tag,
					string_set_type& pp,
					string_set_type& pp_d32, string_set_type& pp_r32,
					string_set_type& pp_d64, string_set_type& pp_r64)
{
	typedef string_set_type::iterator string_set_iter_type;

	ptree_string_type str_vc_pp_d32;
	ptree_string_type str_vc_pp_r32;
	ptree_string_type str_vc_pp_d64;
	ptree_string_type str_vc_pp_r64;

	if(vc_app_tag.size()
		&& !((vc_app_tag == vcproj::vcproj_app_tags::s_tag_utility())
			|| (vc_app_tag == vcproj::vcproj_app_tags::s_tag_makefile())))
	{
		str_vc_pp_d32 =
			vcproj::get_preprocessor_definitions(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_debug_x32());

		if(str_vc_pp_d32.size())
		{
			vcproj::parse_opt_items(pp_d32, str_vc_pp_d32);
		}

		str_vc_pp_r32 =
			vcproj::get_preprocessor_definitions(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_release_x32());

		if(str_vc_pp_r32.size())
		{
			vcproj::parse_opt_items(pp_r32, str_vc_pp_r32);
		}

		str_vc_pp_d64 =
			vcproj::get_preprocessor_definitions(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_debug_x64());

		if(str_vc_pp_d64.size())
		{
			vcproj::parse_opt_items(pp_d64, str_vc_pp_d64);
		}

		str_vc_pp_r64 =
			vcproj::get_preprocessor_definitions(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_release_x64());

		if(str_vc_pp_r64.size())
		{
			vcproj::parse_opt_items(pp_r64, str_vc_pp_r64);
		}

		category_opt_items(pp, pp_d32, pp_r32, pp_d64, pp_r64);
		if(vc_app_tag == vcproj::vcproj_app_tags::s_tag_exe())
		{
			format_opt_items_pp_exe(vc_ptree, pp, pp_d32, pp_r32, pp_d64, pp_r64);
		}
		else if(vc_app_tag == vcproj::vcproj_app_tags::s_tag_dll())
		{
			format_opt_items_pp_dll(vc_ptree, pp, pp_d32, pp_r32, pp_d64, pp_r64);
		}
		else if(vc_app_tag == vcproj::vcproj_app_tags::s_tag_static_lib())
		{
			format_opt_items_pp_static_lib(vc_ptree, pp, pp_d32, pp_r32, pp_d64, pp_r64);
		}
	}
}

void split_inc_dir(const ptree_type& vc_ptree, const ptree_string_type& vc_app_tag,
					string_set_type& inc_dirs,
					string_set_type& inc_dirs_d32, string_set_type& inc_dirs_r32,
					string_set_type& inc_dirs_d64, string_set_type& inc_dirs_r64)
{
	typedef string_set_type::iterator string_set_iter_type;

	ptree_string_type str_vc_inc_dirs_d32;
	ptree_string_type str_vc_inc_dirs_r32;
	ptree_string_type str_vc_inc_dirs_d64;
	ptree_string_type str_vc_inc_dirs_r64;

	if(vc_app_tag.size()
		&& !((vc_app_tag == vcproj::vcproj_app_tags::s_tag_utility())
			|| (vc_app_tag == vcproj::vcproj_app_tags::s_tag_makefile())))
	{
		str_vc_inc_dirs_d32 =
			vcproj::get_additional_include_directories(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_debug_x32());

		if(str_vc_inc_dirs_d32.size())
		{
			vcproj::parse_opt_items(inc_dirs_d32, str_vc_inc_dirs_d32);
		}

		str_vc_inc_dirs_r32 =
			vcproj::get_additional_include_directories(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_release_x32());

		if(str_vc_inc_dirs_r32.size())
		{
			vcproj::parse_opt_items(inc_dirs_r32, str_vc_inc_dirs_r32);
		}

		str_vc_inc_dirs_d64 =
			vcproj::get_additional_include_directories(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_debug_x64());

		if(str_vc_inc_dirs_d64.size())
		{
			vcproj::parse_opt_items(inc_dirs_d64, str_vc_inc_dirs_d64);
		}

		str_vc_inc_dirs_r64 =
			vcproj::get_additional_include_directories(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_release_x64());

		if(str_vc_inc_dirs_r64.size())
		{
			vcproj::parse_opt_items(inc_dirs_r64, str_vc_inc_dirs_r64);
		}

		category_opt_items(inc_dirs, inc_dirs_d32, inc_dirs_r32, inc_dirs_d64, inc_dirs_r64);
	}
}

void split_lib_dir(const ptree_type& vc_ptree, const ptree_string_type& vc_app_tag,
					string_set_type& lib_dirs,
					string_set_type& lib_dirs_d32, string_set_type& lib_dirs_r32,
					string_set_type& lib_dirs_d64, string_set_type& lib_dirs_r64)
{
	typedef string_set_type::iterator string_set_iter_type;

	ptree_string_type str_vc_lib_dirs_d32;
	ptree_string_type str_vc_lib_dirs_r32;
	ptree_string_type str_vc_lib_dirs_d64;
	ptree_string_type str_vc_lib_dirs_r64;

	if(vc_app_tag.size()
		&& !((vc_app_tag == vcproj::vcproj_app_tags::s_tag_utility())
			|| (vc_app_tag == vcproj::vcproj_app_tags::s_tag_makefile())
			|| (vc_app_tag == vcproj::vcproj_app_tags::s_tag_static_lib())))
	{
		str_vc_lib_dirs_d32 =
			vcproj::get_additional_library_directories(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_debug_x32());

		if(str_vc_lib_dirs_d32.size())
		{
			vcproj::parse_opt_items(lib_dirs_d32, str_vc_lib_dirs_d32);
		}

		str_vc_lib_dirs_r32 =
			vcproj::get_additional_library_directories(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_release_x32());

		if(str_vc_lib_dirs_r32.size())
		{
			vcproj::parse_opt_items(lib_dirs_r32, str_vc_lib_dirs_r32);
		}

		str_vc_lib_dirs_d64 =
			vcproj::get_additional_library_directories(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_debug_x64());

		if(str_vc_lib_dirs_d64.size())
		{
			vcproj::parse_opt_items(lib_dirs_d64, str_vc_lib_dirs_d64);
		}

		str_vc_lib_dirs_r64 =
			vcproj::get_additional_library_directories(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_release_x64());

		if(str_vc_lib_dirs_r64.size())
		{
			vcproj::parse_opt_items(lib_dirs_r64, str_vc_lib_dirs_r64);
		}

		category_opt_items(lib_dirs, lib_dirs_d32, lib_dirs_r32, lib_dirs_d64, lib_dirs_r64);
	}
}

ptree_string_type vc_lib_name_to_cb_lib_mark(const ptree_string_type& str)
{
	static const ptree_string_type s_str_vc_ver = "-vc$(PlatformToolsetVersion)";
	static const ptree_string_type s_str_debug_ver = "-d";

	ptree_string_type tmp = str;

	// remove first "lib"
	{
		if(3 < tmp.size() && std::equal(tmp.begin(), tmp.begin() + 3, "lib"))
		{
			tmp.erase(tmp.begin(), tmp.begin() + 3);
		}
	}

	// remove ext
	{
		if(4 < tmp.size() && std::equal(tmp.begin() + (tmp.size() - 4), tmp.end(), ".lib"))
		{
			tmp.erase(tmp.begin() + (tmp.size() - 4), tmp.end());
		}
	}

	// remove "-vc$(PlatformToolsetVersion)"
	{
		std::size_t pos = tmp.find(s_str_vc_ver);

		if(pos != ptree_string_type::npos)
		{
			tmp.erase(tmp.begin() + pos, tmp.begin() + (pos + s_str_vc_ver.size()));
		}
	}

	// remove -d
	{
		std::size_t pos = tmp.find(s_str_debug_ver);

		if(pos != ptree_string_type::npos)
		{
			tmp.erase(tmp.begin() + pos, tmp.begin() + (pos + s_str_debug_ver.size()));
		}
	}

	return tmp;
}

string_set_type& vc_lib_names_to_cb_lib_marks(string_set_type& lib_names)
{
	string_set_type tmp;

	ptree_string_type tmp_str;

	for(string_set_type::iterator i = lib_names.begin(), isize = lib_names.end(); i != isize; ++i)
	{
		tmp_str = vc_lib_name_to_cb_lib_mark(*i);
		//std::cout << *i << " to " << tmp_str << std::endl;
		if(tmp_str.size())
		{
			tmp.insert(tmp_str);
		}
	}

	lib_names.swap(tmp);

	return lib_names;
}

void split_lib_marks_from_vcproj(const ptree_type& vc_ptree, const ptree_string_type& vc_app_tag,
									string_set_type& lib_marks,
									string_set_type& lib_marks_d32, string_set_type& lib_marks_r32,
									string_set_type& lib_marks_d64, string_set_type& lib_marks_r64)
{
	typedef string_set_type::iterator string_set_iter_type;

	ptree_string_type str_vc_lib_names_d32;
	ptree_string_type str_vc_lib_names_r32;
	ptree_string_type str_vc_lib_names_d64;
	ptree_string_type str_vc_lib_names_r64;

	if(vc_app_tag.size()
		&& !((vc_app_tag == vcproj::vcproj_app_tags::s_tag_utility())
			|| (vc_app_tag == vcproj::vcproj_app_tags::s_tag_makefile())
			|| (vc_app_tag == vcproj::vcproj_app_tags::s_tag_static_lib())))
	{
		str_vc_lib_names_d32 =
			vcproj::get_additional_dependencies(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_debug_x32());

		if(str_vc_lib_names_d32.size())
		{
			vcproj::parse_opt_items(lib_marks_d32, str_vc_lib_names_d32);
			vc_lib_names_to_cb_lib_marks(lib_marks_d32);
		}

		str_vc_lib_names_r32 =
			vcproj::get_additional_dependencies(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_release_x32());

		if(str_vc_lib_names_r32.size())
		{
			vcproj::parse_opt_items(lib_marks_r32, str_vc_lib_names_r32);
			vc_lib_names_to_cb_lib_marks(lib_marks_r32);
		}

		str_vc_lib_names_d64 =
			vcproj::get_additional_dependencies(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_debug_x64());

		if(str_vc_lib_names_d64.size())
		{
			vcproj::parse_opt_items(lib_marks_d64, str_vc_lib_names_d64);
			vc_lib_names_to_cb_lib_marks(lib_marks_d64);
		}

		str_vc_lib_names_r64 =
			vcproj::get_additional_dependencies(
				vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_release_x64());

		if(str_vc_lib_names_r64.size())
		{
			vcproj::parse_opt_items(lib_marks_r64, str_vc_lib_names_r64);
			vc_lib_names_to_cb_lib_marks(lib_marks_r64);
		}

		category_opt_items(lib_marks, lib_marks_d32, lib_marks_r32, lib_marks_d64, lib_marks_r64);
	}
}

void split_lib_marks_from_code(const vcproj::code_file_set_type& code_file_list,
								const ptree_string_type& vc_app_tag,
								string_set_type& lib_marks)
{
	typedef vcproj::code_file_set_type code_file_set_type;
	typedef code_file_set_type::const_iterator code_file_set_citer_type;

	typedef cpp::libmark_set_type libmark_set_type;

	if(!((vc_app_tag == vcproj::vcproj_app_tags::s_tag_exe())
			|| (vc_app_tag == vcproj::vcproj_app_tags::s_tag_dll())) )
	{
		return;
	}

	for(code_file_set_citer_type i = code_file_list.begin(), isize = code_file_list.end(); i != isize; ++i)
	{
		{
			cpp::libmark_set_type tmp_lib_marks;
			cpp::get_libmarks_from_code_file(tmp_lib_marks, *i);

			if(tmp_lib_marks.size())
			{
				lib_marks.insert(tmp_lib_marks.begin(), tmp_lib_marks.end());
			}

			lib_marks.insert(file_op::get_file_name_no_ext(*i));
		}
	}
}

lib::ordered_libname_map_type* select_sample_lib_names_map(lib::ordered_libname_map_type& lib_names_d32,
															lib::ordered_libname_map_type& lib_names_r32,
															lib::ordered_libname_map_type& lib_names_d64,
															lib::ordered_libname_map_type& lib_names_r64)
{
	typedef lib::ordered_libname_map_type ordered_libname_map_type;

	ordered_libname_map_type* psample = boost::addressof(lib_names_d32);

	if(psample->size() < lib_names_r32.size())
	{
		psample = boost::addressof(lib_names_r32);
	}

	if(psample->size() < lib_names_d64.size())
	{
		psample = boost::addressof(lib_names_d64);
	}

	if(psample->size() < lib_names_r64.size())
	{
		psample = boost::addressof(lib_names_r64);
	}

	return psample;
}

void erase_libnames(lib::ordered_libname_map_type& lib_names, const lib::ordered_libname_map_type::value_type& val)
{
	typedef lib::ordered_libname_map_type ordered_libname_map_type;
	typedef ordered_libname_map_type::iterator ordered_libname_map_iter_type;
	typedef ordered_libname_map_type::reverse_iterator ordered_libname_map_riter_type;
	typedef std::pair<ordered_libname_map_iter_type, ordered_libname_map_iter_type> ordered_libname_map_iter_pair_type;

	ordered_libname_map_iter_pair_type iter_pair = lib_names.equal_range(val.first);

	for(ordered_libname_map_iter_type i = iter_pair.first, isize = iter_pair.second; i != isize;)
	{
		if(i->second == val.second)
		{
			i = lib_names.erase(i);
		}
		else
		{
			++i;
		}
	}

}

void category_opt_libnames(lib::ordered_libname_map_type& lib_names,
							lib::ordered_libname_map_type& lib_names_d32,
							lib::ordered_libname_map_type& lib_names_r32,
							lib::ordered_libname_map_type& lib_names_d64,
							lib::ordered_libname_map_type& lib_names_r64)
{
	typedef lib::ordered_libname_map_type ordered_libname_map_type;
	typedef ordered_libname_map_type::iterator ordered_libname_map_iter_type;
	typedef ordered_libname_map_type::reverse_iterator ordered_libname_map_riter_type;
	typedef std::pair<ordered_libname_map_iter_type, ordered_libname_map_iter_type> ordered_libname_map_iter_pair_type;
	typedef yggr::vector<ordered_libname_map_iter_type> ordered_libname_map_iter_vt_type;

	ordered_libname_map_type* psample_map =
		select_sample_lib_names_map(lib_names_d32, lib_names_r32, lib_names_d64, lib_names_r64);

	assert(psample_map);

	ordered_libname_map_type& sample_map = *psample_map;

	if(sample_map.empty())
	{
		return;
	}

	yggr::s32 before_lv = sample_map.begin()->first;
	std::size_t add_old_size = 0;

	ordered_libname_map_iter_vt_type add_iters;

	for(ordered_libname_map_iter_type i = sample_map.begin(), isize = sample_map.end(); i != isize; ++i)
	{
		ordered_libname_map_iter_type inext = i;
		++inext;

		if((inext != isize) && (before_lv == inext->first))
		{
			continue;
		}

		ordered_libname_map_iter_pair_type iter_pair_d32 = lib_names_d32.equal_range(before_lv);
		ordered_libname_map_iter_pair_type iter_pair_r32 = lib_names_r32.equal_range(before_lv);
		ordered_libname_map_iter_pair_type iter_pair_d64 = lib_names_d64.equal_range(before_lv);
		ordered_libname_map_iter_pair_type iter_pair_r64 = lib_names_r64.equal_range(before_lv);

		std::size_t d32_size = std::distance(iter_pair_d32.first, iter_pair_d32.second);
		std::size_t r32_size = std::distance(iter_pair_r32.first, iter_pair_r32.second);
		std::size_t d64_size = std::distance(iter_pair_d64.first, iter_pair_d64.second);
		std::size_t r64_size = std::distance(iter_pair_r64.first, iter_pair_r64.second);

		std::size_t min_size = d32_size;
		std::size_t max_size = d32_size;

		ordered_libname_map_iter_pair_type* pmax_iter_pair = boost::addressof(iter_pair_d32);

		if(max_size < r32_size)
		{
			pmax_iter_pair = boost::addressof(iter_pair_r32);
			max_size = r32_size;
		}
		else
		{
			if(r32_size < min_size)
			{
				min_size = r32_size;
			}
		}

		if(max_size < d64_size)
		{
			pmax_iter_pair = boost::addressof(iter_pair_d64);
			max_size = d64_size;
		}
		else
		{
			if(d64_size < min_size)
			{
				min_size = d64_size;
			}
		}

		if(max_size < r64_size)
		{
			pmax_iter_pair = boost::addressof(iter_pair_r64);
			max_size = r64_size;
		}
		else
		{
			if(r64_size < min_size)
			{
				min_size = r64_size;
			}
		}

		ordered_libname_map_iter_pair_type& max_iter_pair = *pmax_iter_pair;

		add_old_size = add_iters.size();
		ordered_libname_map_iter_type iter_d32;
		ordered_libname_map_iter_type iter_r32;
		ordered_libname_map_iter_type iter_d64;
		ordered_libname_map_iter_type iter_r64;

		for(ordered_libname_map_iter_type j = max_iter_pair.first, jsize = max_iter_pair.second; j != jsize; ++j)
		{
			iter_d32 = std::find(iter_pair_d32.first, iter_pair_d32.second, *j);
			iter_r32 = std::find(iter_pair_r32.first, iter_pair_r32.second, *j);
			iter_d64 = std::find(iter_pair_d64.first, iter_pair_d64.second, *j);
			iter_r64 = std::find(iter_pair_r64.first, iter_pair_r64.second, *j);


			if((iter_d32 != iter_pair_d32.second)
				&& (iter_r32 != iter_pair_r32.second)
				&& (iter_d64 != iter_pair_d64.second)
				&& (iter_r64 != iter_pair_r64.second) )
			{
				add_iters.push_back(j);
			}
		}

		if((min_size == max_size)
			&& (min_size == (add_iters.size() - add_old_size)))
		{
			if(inext != isize)
			{
				before_lv = inext->first;
			}
		}
		else
		{
			break;
		}
	}

	lib::ordered_libname_map_type tmp_lib_names;
	{
		for(ordered_libname_map_iter_vt_type::const_iterator i = add_iters.begin(), isize = add_iters.end(); i != isize; ++i)
		{
			tmp_lib_names.insert(**i);
		}
	}

#define TMP_PP_REMOVE_SHARED_LIBNAME( __lib_name__ ) \
	{ for(ordered_libname_map_type::const_iterator i = tmp_lib_names.begin(), isize = tmp_lib_names.end(); i != isize; ++i) { \
			erase_libnames(__lib_name__, *i); } }

	if(tmp_lib_names.size())
	{
		TMP_PP_REMOVE_SHARED_LIBNAME(lib_names_d32);
		TMP_PP_REMOVE_SHARED_LIBNAME(lib_names_r32);
		TMP_PP_REMOVE_SHARED_LIBNAME(lib_names_d64);
		TMP_PP_REMOVE_SHARED_LIBNAME(lib_names_r64);
	}

#undef TMP_PP_REMOVE_SHARED_LIBNAME

	lib_names.insert(tmp_lib_names.begin(), tmp_lib_names.end());
}

void split_lib_names(const ptree_type& vc_ptree,
						const vcproj::code_file_set_type& code_file_list,
						const ptree_string_type& vc_app_tag,
						const lib::lib_depend_info& dep_info_cfg,
						lib::ordered_libname_map_type& lib_names,
						lib::ordered_libname_map_type& lib_names_d32,
						lib::ordered_libname_map_type& lib_names_r32,
						lib::ordered_libname_map_type& lib_names_d64,
						lib::ordered_libname_map_type& lib_names_r64)
{
	typedef lib::ordered_libname_map_type ordered_libname_map_type;

	if(!((vc_app_tag == vcproj::vcproj_app_tags::s_tag_exe())
		|| (vc_app_tag == vcproj::vcproj_app_tags::s_tag_dll()) ))
	{
		lib_names.clear();
		lib_names_d32.clear();
		lib_names_r32.clear();
		lib_names_d64.clear();
		lib_names_r64.clear();
		return;
	}

	string_set_type lib_marks, lib_marks_d32, lib_marks_r32, lib_marks_d64, lib_marks_r64;

	split_lib_marks_from_vcproj(
		vc_ptree, vc_app_tag, lib_marks, lib_marks_d32, lib_marks_r32, lib_marks_d64, lib_marks_r64);

	split_lib_marks_from_code(code_file_list, vc_app_tag, lib_marks);

	ptree_string_type vc_proj_name = vcproj::get_root_namespace(vc_ptree);

	if(vc_proj_name.size())
	{
		lib_marks.insert(vc_proj_name);
	}

	lib_marks_d32.insert("mgw_gcc_sjlj");
	lib_marks_r32.insert("mgw_gcc_sjlj");

//#if defined(_DEBUG)
//	{
//		for(string_set_type::const_iterator i = lib_marks.begin(), isize = lib_marks.end(); i != isize; ++i)
//		{
//			std::cout << "last libmarks: " << *i << std::endl;
//		}
//	}
//#endif // _DEBUG


	if(lib_marks.size())
	{
		{
			string_set_type tmp_libmarks(lib_marks);
			if(lib_marks_d32.size())
			{
				tmp_libmarks.insert(lib_marks_d32.begin(), lib_marks_d32.end());
			}
			dep_info_cfg.gen_libnames(lib_names_d32, tmp_libmarks, vcproj::vcproj_cfg_marks::s_mark_debug_x32());
		}

		{
			string_set_type tmp_libmarks(lib_marks);
			if(lib_marks_r32.size())
			{
				tmp_libmarks.insert(lib_marks_r32.begin(), lib_marks_r32.end());
			}
			dep_info_cfg.gen_libnames(lib_names_r32, tmp_libmarks, vcproj::vcproj_cfg_marks::s_mark_release_x32());
		}

		{
			string_set_type tmp_libmarks(lib_marks);
			if(lib_marks_d64.size())
			{
				tmp_libmarks.insert(lib_marks_d64.begin(), lib_marks_d64.end());
			}
			dep_info_cfg.gen_libnames(lib_names_d64, tmp_libmarks, vcproj::vcproj_cfg_marks::s_mark_debug_x64());

//#if defined(_DEBUG)
//	{
//		for(lib::ordered_libname_map_type::const_iterator i = lib_names_d64.begin(), isize = lib_names_d64.end(); i != isize; ++i)
//		{
//			std::cout << "last libnames: " << i->first << ", " << i->second << std::endl;
//		}
//	}
//#endif // _DEBUG

		}

		{
			string_set_type tmp_libmarks(lib_marks);
			if(lib_marks_r64.size())
			{
				tmp_libmarks.insert(lib_marks_r64.begin(), lib_marks_r64.end());
			}

			dep_info_cfg.gen_libnames(lib_names_r64, tmp_libmarks, vcproj::vcproj_cfg_marks::s_mark_release_x64());
		}
	}

	//{
	//	for(lib::ordered_libname_map_type::const_iterator i = lib_names_d32.begin(), isize = lib_names_d32.end(); i != isize; ++i)
	//	{
	//		std::cout << "d32 libnames: " << i->first << ", " << i->second << std::endl;
	//	}
	//}

	category_opt_libnames(lib_names, lib_names_d32, lib_names_r32, lib_names_d64, lib_names_r64);

	//{
	//	for(lib::ordered_libname_map_type::const_iterator i = lib_names_d32.begin(), isize = lib_names_d32.end(); i != isize; ++i)
	//	{
	//		std::cout << "d32-2 libnames: " << i->first << ", " << i->second << std::endl;
	//	}

	//	int nnn = 0;
	//}
}

const ptree_string_type& vc_std_lang_to_cb_std_lang(const ptree_string_type& vc_std_lang)
{
	if(vc_std_lang == vcproj::vcproj_cpp_lang_std::s_cpp11())
	{
		return cb_cpp_lang_std::s_cpp11();
	}
	else if(vc_std_lang == vcproj::vcproj_cpp_lang_std::s_cpp14())
	{
		return cb_cpp_lang_std::s_cpp14();
	}
	else if(vc_std_lang == vcproj::vcproj_cpp_lang_std::s_cpp17())
	{
		return cb_cpp_lang_std::s_cpp17();
	}
	else if(vc_std_lang == vcproj::vcproj_cpp_lang_std::s_cpp20())
	{
		return cb_cpp_lang_std::s_cpp20();
	}
	else
	{
		return cb_cpp_lang_std::s_cpp03();
	}
}

const ptree_string_type& gen_cb_language_standard(const ptree_type& vc_ptree,
													const ptree_string_type& vc_app_tag)
{
	if(!vcproj::vcproj_app_tags::s_has_compile_option(vc_app_tag))
	{
		return cb_cpp_lang_std::s_cpp03();
	}

	ptree_string_type vc_lang_std_d32 =
		vcproj::get_language_standard(vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_debug_x32());
	ptree_string_type vc_lang_std_r32 =
		vcproj::get_language_standard(vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_release_x32());
	ptree_string_type vc_lang_std_d64 =
		vcproj::get_language_standard(vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_debug_x64());
	ptree_string_type vc_lang_std_r64 =
		vcproj::get_language_standard(vc_ptree, vc_app_tag, vcproj::vcproj_cfg_marks::s_mark_release_x64());

	return
		(vc_lang_std_d32 == vc_lang_std_r32)
		&& (vc_lang_std_d32 == vc_lang_std_d64)
		&& (vc_lang_std_d32 == vc_lang_std_r64)?
			vc_std_lang_to_cb_std_lang(vc_lang_std_d32)
			: cb_cpp_lang_std::s_cpp03();
}

yggr::u32 fix_cb_proj_compiler(ptree_type& cb_tree,
								const ptree_type& vc_ptree,
								yggr::u32 proj_tag,
								const ptree_string_type& vc_app_tag,
								const string_set_type& pp,
								const string_set_type& inc_dirs)
{
	typedef string_set_type::const_iterator string_set_citer_type;
	
	bool is_c_obj = !!(proj_tag & proj_tag_t::E_TAG_COBJ);
	cb_tree.clear();

	const ptree_string_type& lang_std = gen_cb_language_standard(vc_ptree, vc_app_tag);

	{
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Wall"));
		if(lang_std.size())
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type(lang_std));
		}

		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-fexceptions"));
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Wno-sign-compare"));
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Wno-comment"));
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Wno-enum-compare"));
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Wno-unused-local-typedefs"));
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Wno-multichar"));

		//cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Werror=unused-variable"));
		//cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Werror=unused-local-typedefs"));
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Werror=return-type"));
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Werror=endif-labels"));


		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-fdata-sections"));
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-ffunction-sections"));
		cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-fno-strict-aliasing"));

		if(!is_c_obj)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-Werror=reorder"));
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-ftemplate-backtrace-limit=0"));
		}
	}

	{
		if(vc_app_tag == vcproj::vcproj_app_tags::s_tag_dll())
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-fPIC"));
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-DBUILD_DLL"));
		}

		for(string_set_citer_type i = pp.begin(), isize = pp.end(); i != isize; ++i)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type("-D" + *i));
		}
	}

	{
		for(string_set_citer_type i = inc_dirs.begin(), isize = inc_dirs.end(); i != isize; ++i)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.directory", ptree_type(*i));
		}
	}

	return xml_op::fix_result::E_SUCCEED;
}

yggr::u32 fix_cb_proj_linker(ptree_type& cb_tree,
								const ptree_type& vc_ptree,
								const string_set_type& lib_dirs,
								const lib::ordered_libname_map_type& lib_names)
{
	typedef string_set_type::const_iterator string_set_citer_type;
	typedef lib::ordered_libname_map_type ordered_libname_map_type;
	typedef ordered_libname_map_type::const_reverse_iterator ordered_libname_map_criter_type;

	cb_tree.clear();

	// lib_names
	{
		for(ordered_libname_map_criter_type i = lib_names.rbegin(), isize = lib_names.rend(); i != isize; ++i)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.option", ptree_type(i->second));
		}
	}

	// lib_dir
	{
		for(string_set_citer_type i = lib_dirs.begin(), isize = lib_dirs.end(); i != isize; ++i)
		{
			cb_tree.add_child("Add", ptree_type()).add_child("<xmlattr>.directory", ptree_type(*i));
		}
	}

	return xml_op::fix_result::E_SUCCEED;
}

yggr::utf8_string format_proj_dir(const yggr::utf8_string& proj_dir)
{
	yggr::utf8_string tmp = proj_dir;

	for(;tmp.size();)
	{
		if((tmp.org_str().back() == '\\') || (tmp.org_str().back() == '/'))
		{
			tmp.org_str().pop_back();
		}
		else
		{
			break;
		}
	}

	tmp.recount_length();
	return tmp;
}

vcproj::code_file_set_type& format_file_list(vcproj::code_file_set_type& file_list, const yggr::utf8_string& arg_proj_dir)
{
	typedef vcproj::code_file_set_type code_file_set_type;

	yggr::utf8_string proj_dir = format_proj_dir(arg_proj_dir);

	if(proj_dir.empty())
	{
		return file_list;
	}

	code_file_set_type tmp;
	for(code_file_set_type::const_iterator i = file_list.begin(), isize = file_list.end(); i != isize; ++i)
	{
		tmp.insert(proj_dir.org_str() + "/" + (*i));
	}

	file_list.swap(tmp);
	return file_list;
}

bool is_c_proj_only(const vcproj::code_file_set_type& file_list, const ptree_string_type& vc_app_tag)
{
	typedef vcproj::code_file_set_type code_file_set_type;

	static const ptree_string_type s_str_chk = ".cpp";

	if(!vcproj::vcproj_app_tags::s_has_compile_option(vc_app_tag))
	{
		return false;
	}

	for(code_file_set_type::const_iterator i = file_list.begin(), isize = file_list.end();
		i != isize; ++i)
	{
		if((4 < (*i).size())
			&& std::equal(s_str_chk.begin(), s_str_chk.end(), (*i).begin() + ((*i).size() - 4)) )
		{
			return false;
		}
	}

	return true;
}

bool has_one_func_big_obj_from_root_namespace(const ptree_type& ptree, const ptree_string_type& vc_app_tag)
{
	typedef vcproj::code_file_set_type string_set_type;

	static string_set_type str_set;

	if(str_set.empty())
	{
		str_set.insert("lua_clt_test");
	}

	if(!vcproj::vcproj_app_tags::s_has_compile_option(vc_app_tag))
	{
		return false;
	}

	ptree_string_type str_root_namespace = vcproj::get_root_namespace(ptree);

	return (str_set.find(str_root_namespace) != str_set.end());
}

bool has_big_obj_from_root_namespace(const ptree_type& ptree, const ptree_string_type& vc_app_tag)
{
	typedef vcproj::code_file_set_type string_set_type;

	static string_set_type str_set;

	if(str_set.empty())
	{
		str_set.insert("yggr_any_val");

		str_set.insert("clt_test_async");
		str_set.insert("encryption_tool_proxy_srv_test");
		str_set.insert("encryption_tool_proxy_trun_srv_test");

		str_set.insert("packet_string_serialization_test_eins");
	}

	if(!vcproj::vcproj_app_tags::s_has_compile_option(vc_app_tag))
	{
		return false;
	}

	ptree_string_type str_root_namespace = vcproj::get_root_namespace(ptree);

	return (str_set.find(str_root_namespace) != str_set.end());
}

bool has_big_obj_from_file_list(const vcproj::code_file_set_type& files, const ptree_string_type& vc_app_tag)
{
	static vcproj::code_file_set_type str_set;

	if(str_set.empty())
	{
		str_set.insert("init_client.cpp");
		str_set.insert("init_client_drei.cpp");
		str_set.insert("init_server.cpp");
		str_set.insert("init_server_zwei.cpp");
		str_set.insert("handler_reg_eins.cpp");
	}

	if(!vcproj::vcproj_app_tags::s_has_compile_option(vc_app_tag))
	{
		return false;
	}

	for(vcproj::code_file_set_type::const_iterator i = files.begin(), isize = files.end(); i != isize; ++i)
	{
		//std::cout << (*i) << ", " << file_op::get_file_name(*i) << std::endl;
		if(str_set.find(file_op::get_file_name(*i)) != str_set.end())
		{
			return true;
		}
	}
	return false;
}

bool need_add_dir_charset_utf8_files(const ptree_string_type& proj_root, const ptree_string_type& str_path)
{
	static const ptree_string_type utf8_file_dir = "/charset_utf8_files/";

	ptree_string_type str_dir = file_op::get_file_dir(str_path);
	ptree_string_type str_fname = file_op::get_file_name(str_path);

	std::cout << str_dir << std::endl;
	std::cout << str_fname << std::endl;

	try
	{
		return
			(4 < str_fname.size()) && (str_fname.substr(str_fname.size() - 4) == ".cpp")
			&& local_fsys_operators_type::is_exists(
				local_fsys_operators_type::make_path_object(
					proj_root + "/" + str_dir + utf8_file_dir + str_fname ));
	}
	catch(...)
	{
		return false;
	}
}

ptree_string_type add_dir_charset_utf8_files(const ptree_string_type& str_path)
{
	static const ptree_string_type utf8_file_dir = "/charset_utf8_files/";

	ptree_string_type str_dir = file_op::get_file_dir(str_path);
	ptree_string_type str_fname = file_op::get_file_name(str_path);

	return str_dir + utf8_file_dir + str_fname;
}

yggr::u32 make_cb_ptree_from_vc_tree(ptree_type& inout,
										const ptree_type& vc_ptree,
										const yggr::utf8_string& proj_dir,
										const lib::lib_depend_info& dep_info_cfg)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::value_type ptree_val_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;
	typedef boost::optional<const ptree_type&> const_attribs_type;
	typedef std::pair<assoc_iter_type, assoc_iter_type> assoc_iter_pair_type;

	typedef lib::ordered_libname_map_type ordered_libname_map_type;

	ptree_type& ptree_cb = inout;
	ptree_type* ptree_cb_proj_ptr = 0;

	try
	{
		ptree_cb_proj_ptr =
			boost::addressof(ptree_cb.get_child(ptree_type::path_type("CodeBlocks_project_file.Project")));
	}
	catch(...)
	{
		return xml_op::fix_result::E_FAILED;
	}

	if(!ptree_cb_proj_ptr)
	{
		return xml_op::fix_result::E_FAILED;
	}

	ptree_type& ptree_cb_proj = *ptree_cb_proj_ptr;

	string_set_type pp, pp_d32, pp_r32, pp_d64, pp_r64;
	string_set_type inc_dirs, inc_dirs_d32, inc_dirs_r32, inc_dirs_d64, inc_dirs_r64;
	string_set_type lib_dirs, lib_dirs_d32, lib_dirs_r32, lib_dirs_d64, lib_dirs_r64;
	ordered_libname_map_type lib_names, lib_names_d32, lib_names_r32, lib_names_d64, lib_names_r64;

	ptree_string_type vc_app_tag = vcproj::get_app_tag(vc_ptree);

	if(vc_app_tag.empty())
	{
		return xml_op::fix_result::E_FAILED;
	}

	vcproj::code_file_set_type file_list, formated_file_list;
	formated_file_list = vcproj::get_code_files(file_list, vc_ptree);
	format_file_list(formated_file_list, proj_dir);

	yggr::u32 proj_tag = 0;
	if(file_op::is_yggr_proj_check(proj_dir))
	{
		proj_tag |= proj_tag_t::E_TAG_YGGR_OBJ;
	}

	if(is_c_proj_only(file_list, vc_app_tag))
	{
		proj_tag |= proj_tag_t::E_TAG_COBJ;
	}

	if(vc_app_tag == vcproj::vcproj_app_tags::s_tag_exe())
	{
		proj_tag |= proj_tag_t::E_TAG_EXE;
	}

	if(has_big_obj_from_root_namespace(vc_ptree, vc_app_tag)
		|| has_big_obj_from_file_list(file_list, vc_app_tag))
	{
		proj_tag |= proj_tag_t::E_TAG_BIG_OBJ;
	}

	if(has_one_func_big_obj_from_root_namespace(vc_ptree, vc_app_tag))
	{
		proj_tag |= proj_tag_t::E_TAG_ONE_FUNC_BIG_OBJ;
	}

	//bool is_yggr_proj = file_op::is_yggr_proj_check(proj_dir);
	//bool is_c_proj = is_c_proj_only(file_list, vc_app_tag);
	//bool is_big_obj =
	//		has_big_obj_from_root_namespace(vc_ptree, vc_app_tag)
	//		|| has_big_obj_from_file_list(file_list, vc_app_tag);

	{
		split_pp(vc_ptree, vc_app_tag, pp, pp_d32, pp_r32, pp_d64, pp_r64);
		split_inc_dir(vc_ptree, vc_app_tag, inc_dirs, inc_dirs_d32, inc_dirs_r32, inc_dirs_d64, inc_dirs_r64);
		split_lib_dir(vc_ptree, vc_app_tag, lib_dirs, lib_dirs_d32, lib_dirs_r32, lib_dirs_d64, lib_dirs_r64);
		split_lib_names(
			vc_ptree, formated_file_list, vc_app_tag, dep_info_cfg,
			lib_names, lib_names_d32, lib_names_r32, lib_names_d64, lib_names_r64);

		//{
		//	for(ordered_libname_map_type::const_reverse_iterator i = lib_names.rbegin(), isize = lib_names.rend(); i != isize; ++i)
		//	{
		//		std::cout << i->first << ", " << i->second << std::endl;
		//	}
		//}

		//{
		//	for(ordered_libname_map_type::const_reverse_iterator i = lib_names_d32.rbegin(), isize = lib_names_d32.rend(); i != isize; ++i)
		//	{
		//		std::cout << i->first << ", " << i->second << std::endl;
		//	}
		//}

		//{
		//	for(ordered_libname_map_type::const_reverse_iterator i = lib_names_r32.rbegin(), isize = lib_names_r32.rend(); i != isize; ++i)
		//	{
		//		std::cout << i->first << ", " << i->second << std::endl;
		//	}
		//}

		//{
		//	for(ordered_libname_map_type::const_reverse_iterator i = lib_names_d64.rbegin(), isize = lib_names_d64.rend(); i != isize; ++i)
		//	{
		//		std::cout << i->first << ", " << i->second << std::endl;
		//	}
		//}

		//{
		//	for(ordered_libname_map_type::const_reverse_iterator i = lib_names_r64.rbegin(), isize = lib_names_r64.rend(); i != isize; ++i)
		//	{
		//		std::cout << i->first << ", " << i->second << std::endl;
		//	}
		//}
	}

	yggr::u32 sat = xml_op::fix_result::E_FAILED;

	if((pp.size() || inc_dirs.size())
		&& (ptree_cb_proj.find("Compiler") == ptree_cb_proj.not_found()))
	{
		ptree_cb_proj.add_child("Compiler", ptree_type());
	}

	if((lib_dirs.size() || lib_names.size())
		&& (ptree_cb_proj.find("Linker") == ptree_cb_proj.not_found()))
	{
		ptree_cb_proj.add_child("Linker", ptree_type());
	}

	ptree_cb_proj.erase("Unit");

	ptree_string_type out_file_path;

	for(iterator i = ptree_cb_proj.begin(), isize = ptree_cb_proj.end(); i != isize;)
	{
		if(i->first == "Option")
		{
			switch((sat = fix_cb_proj_option(i->second, vc_ptree)))
			{
			case xml_op::fix_result::E_SUCCEED:
				++i;
				break;
			case xml_op::fix_result::E_FAILED:
				return sat;
			case xml_op::fix_result::E_IGNORE:
				i = ptree_cb_proj.erase(i);
				break;
			default:
				assert(false);
				return xml_op::fix_result::E_FAILED;
			}

			continue;
		}

		if(i->first == "Build")
		{
			if((sat = fix_cb_proj_build(
						i->second, vc_ptree,
						proj_tag,
						pp_d32, pp_r32, pp_d64, pp_r64,
						inc_dirs_d32, inc_dirs_r32, inc_dirs_d64, inc_dirs_r64,
						lib_dirs_d32, lib_dirs_r32, lib_dirs_d64, lib_dirs_r64,
						lib_names_d32, lib_names_r32, lib_names_d64, lib_names_r64 )) != xml_op::fix_result::E_SUCCEED)
			{
				return sat;
			}

			++i;
			continue;
		}

		if(i->first == "Compiler")
		{
			if((sat = fix_cb_proj_compiler(
						i->second, vc_ptree, proj_tag, vc_app_tag, pp, inc_dirs)) != xml_op::fix_result::E_SUCCEED)
			{
				return sat;
			}

			if(i->second.empty())
			{
				i = ptree_cb_proj.erase(i);
			}
			else
			{
				++i;
			}
			continue;
		}

		if(i->first == "Linker")
		{
			if((sat = fix_cb_proj_linker(
						i->second, vc_ptree, lib_dirs, lib_names)) != xml_op::fix_result::E_SUCCEED)
			{
				return sat;
			}

			if(i->second.empty())
			{
				i = ptree_cb_proj.erase(i);
			}
			else
			{
				++i;
			}
			continue;
		}

		if(i->first == "Extensions")
		{
			for(vcproj::code_file_set_type::const_iterator j = file_list.begin(), jsize = file_list.end();
					j != jsize; ++j)
			{
				out_file_path = *j;
				file_op::format_string_file_path(out_file_path);
				if(need_add_dir_charset_utf8_files(proj_dir, out_file_path))
				{
					ptree_cb_proj
						.insert(i, ptree_val_type("Unit", ptree_type()))
						->second.add_child("<xmlattr>.filename", ptree_type(add_dir_charset_utf8_files(out_file_path)));
				}
				else
				{
					ptree_cb_proj.insert(i, ptree_val_type("Unit", ptree_type()))->second.add_child("<xmlattr>.filename", ptree_type(out_file_path));
				}
			}

			++i;
			continue;
		}

		++i;
	}

	return sat;
}

ptree_type& get_cb_ptree_from_buffer(ptree_type& out, const yggr::utf8_string& file_data)
{
	return xml_op::read_xml_from_buffer(out, file_data);
}

ptree_type& get_cb_ptree_from_file(ptree_type& out, const yggr::utf8_string& fname)
{
	return xml_op::read_xml_from_file(out, fname);
}

bool write_cbp(const ptree_type& ptree, const yggr::utf8_string& str_file_path)
{
	return xml_op::write_xml_to_file(ptree, str_file_path);
}

} // namespace cb

#endif // !(BOOST_VERSION < 105600)
