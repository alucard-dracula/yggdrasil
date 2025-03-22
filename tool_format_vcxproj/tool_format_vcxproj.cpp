// tool_format_vcxproj.cpp

#include <yggr/base/yggrdef.h>

#include <iostream>
#include <sstream>
#include <cassert>
#include <algorithm>

#if BOOST_VERSION < 105600

//#	error this tool not support boost 1.55 and older

#	else

#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/set.hpp>
#include <yggr/container/unordered_set.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>
#include <yggr/file_system/boost_filesystem_comp.hpp>
#include "vcxproj_parser_patch.hpp"
#include "tool_format_vcxproj_cfg.hpp"

#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

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

//#if BOOST_VERSION < 105600
//#	error this tool not support boost 1.55 and older
//#endif //BOOST_VERSION < 105600

#define ONLY_MAKE_RUN_TEST() 0

typedef yggr::unordered_set<yggr::utf8_string> filter_list_type;
typedef yggr::set<yggr::utf8_string> file_list_type;

typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;
typedef local_fsys_operators_type::directory_entry_type directory_entry_type;
typedef local_fsys_operators_type::path_type path_type;

typedef tool_format_vcxproj_cfg tool_format_vcxproj_cfg_type;
typedef tool_format_vcxproj_cfg_type::project_name_map_type project_name_map_type;

typedef yggr::set<yggr::string> string_set_type;


static const char* tool_format_vcxproj_cfg_mark = "tool_format_vcxproj_cfg";

struct fix_state
{
	enum
	{
		E_SUCCEED = 0,
		E_FAILED = 1,
		E_IGNORE = 2
	};
};

const ptree_type::key_type g_cfg_mark_debug_x32 = "'$(Configuration)|$(Platform)'=='Debug|Win32'";
const ptree_type::key_type g_cfg_mark_release_x32 = "'$(Configuration)|$(Platform)'=='Release|Win32'";
const ptree_type::key_type g_cfg_mark_debug_x64 = "'$(Configuration)|$(Platform)'=='Debug|x64'";
const ptree_type::key_type g_cfg_mark_release_x64 = "'$(Configuration)|$(Platform)'=='Release|x64'";

const ptree_type::key_type g_app_tag_exe = "Application";
const ptree_type::key_type g_app_tag_dll = "DynamicLibrary";
const ptree_type::key_type g_app_tag_static_lib = "StaticLibrary";
const ptree_type::key_type g_app_tag_utility = "Utility";
const ptree_type::key_type g_app_tag_makefile = "Makefile";

bool has_attribs(const ptree_type& ptree)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	attribs_type attribs =
		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

	if(!attribs)
	{
		return false;
	}

	const ptree_type& sub_ptree_ref = attribs.get();

	return !sub_ptree_ref.empty();
}

bool has_attribs(const ptree_type& ptree, const ptree_string_type& key)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	attribs_type attribs =
		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

	if(!attribs)
	{
		return false;
	}

	const ptree_type& sub_ptree_ref = attribs.get();
	for(const_iterator i = sub_ptree_ref.begin(), isize = sub_ptree_ref.end(); i != isize; ++i)
	{
		if(i->first == key)
		{
			return true;
		}
	}

	return false;
}

ptree_string_type get_attribs(const ptree_type& ptree, const ptree_string_type& key)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	attribs_type attribs =
		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

	if(!attribs)
	{
		return string_type();
	}

	const ptree_type& sub_ptree_ref = attribs.get();

	for(const_iterator i = sub_ptree_ref.begin(), isize = sub_ptree_ref.end(); i != isize; ++i)
	{
		if(i->first == key)
		{
			return i->second.get_value<string_type>();
		}
	}

	return string_type();
}

bool set_attribs(ptree_type& ptree, const ptree_string_type& key, const ptree_string_type& val)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;

	attribs_type attribs =
		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

	if(!attribs)
	{
		return false;
	}

	ptree_type& sub_ptree_ref = attribs.get();

	for(iterator i = sub_ptree_ref.begin(), isize = sub_ptree_ref.end(); i != isize; ++i)
	{
		if(i->first == key)
		{
			i->second.put_value(val);
			return true;
		}
	}

	return false;
}

bool set_attribs_force(ptree_type& ptree, const ptree_string_type& key, const ptree_string_type& val)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;

	attribs_type attribs =
		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

	if(!attribs)
	{
		ptree.add_child("<xmlattr>." + key, ptree_type(val));
		return true;
	}

	ptree_type& sub_ptree_ref = attribs.get();

	for(iterator i = sub_ptree_ref.begin(), isize = sub_ptree_ref.end(); i != isize; ++i)
	{
		if(i->first == key)
		{
			i->second.put_value(val);
			return true;
		}
	}

	ptree.add_child("<xmlattr>." + key, ptree_type(val));
	return true;
}

bool set_attribs_force(ptree_type& ptree, const ptree_string_type& hint_key,
						const ptree_string_type& key, const ptree_string_type& val)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::value_type ptree_val_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;

	attribs_type attribs =
		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

	if(!attribs)
	{
		ptree.add_child("<xmlattr>." + key, ptree_type(val));
		return true;
	}

	ptree_type& sub_ptree_ref = attribs.get();

	iterator hint_iter = sub_ptree_ref.end();
	for(iterator i = sub_ptree_ref.begin(), isize = sub_ptree_ref.end(); i != isize; ++i)
	{
		if(i->first == hint_key)
		{
			hint_iter = i;
		}

		if(i->first == key)
		{
			i->second.put_value(val);
			return true;
		}
	}

	if(hint_iter == sub_ptree_ref.end())
	{
		ptree.add_child("<xmlattr>." + key, ptree_type(val));
	}
	else
	{
		ptree.insert(hint_iter, ptree_val_type(key, ptree_type(val)));
	}

	return true;
}

bool print_ptree(const ptree_type& ptree)
{
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;

	xml_writer_settings_type setting('\t', 1);
	try
	{
		boost::property_tree::write_xml(std::cout, ptree, setting);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

ptree_type::key_type& format_string_file_path(ptree_type::key_type& fpath)
{
	typedef ptree_type::key_type string_type;

	for(string_type::iterator i = fpath.begin(), isize = fpath.end(); i != isize; ++i)
	{
		if(*i == '\\')
		{
			*i = '/';
		}
	}

	return fpath;
}

inline bool is_need_fix_of_app_tag(const ptree_type::key_type& app_tag)
{
	return
		(app_tag == g_app_tag_exe)
		|| (app_tag == g_app_tag_dll)
		|| (app_tag == g_app_tag_static_lib)
		|| (app_tag == g_app_tag_utility);
}

inline bool is_cl_fix_object(const ptree_type::key_type& app_tag)
{
	return
		(app_tag == g_app_tag_exe)
		|| (app_tag == g_app_tag_dll)
		|| (app_tag == g_app_tag_static_lib);
}

inline bool is_link_fix_object(const ptree_type::key_type& app_tag)
{
	return
		(app_tag == g_app_tag_exe)
		|| (app_tag == g_app_tag_dll);
}

inline bool is_debug_cfg_mark(const ptree_type::key_type& cfg_mark)
{
	return
		(cfg_mark == g_cfg_mark_debug_x32)
		|| (cfg_mark == g_cfg_mark_debug_x64);
}

inline bool is_release_cfg_mark(const ptree_type::key_type& cfg_mark)
{
	return
		(cfg_mark == g_cfg_mark_release_x32)
		|| (cfg_mark == g_cfg_mark_release_x64);
}

//inline bool is_only_fix_root_namespace(const ptree_type::key_type& app_tag)
//{
//	return (app_tag == g_app_tag_utility);
//}

inline const ptree_type::key_type& link_of_app_tag(const ptree_type::key_type& app_tag)
{
	typedef ptree_type::key_type string_type;
	static const string_type& s_str_link = "Link";
	static const string_type& s_str_lib = "Lib";
	static const string_type& s_str_nil = "";

	return
		is_cl_fix_object(app_tag)?
			(app_tag == g_app_tag_static_lib)? s_str_lib : s_str_link
			: s_str_nil;
}

inline const ptree_type::key_type& select_outdir_yggr(const ptree_type::key_type& root_namespace,
														const ptree_type::key_type& app_tag,
														const ptree_type::key_type& cfg_mark)
{
	typedef ptree_type::key_type string_type;

	static const string_type& s_str_failed = "";
	static const string_type& s_str_default = "$(SolutionDir)$(Configuration)-$(Platform)/vc$(PlatformToolsetVersion)";

	static const string_type& s_str_lib64 = "$(SolutionDir)lib64/";
	static const string_type& s_str_lib32 = "$(SolutionDir)lib/";

	if(app_tag == g_app_tag_static_lib)
	{
		if((cfg_mark == g_cfg_mark_debug_x32) || (cfg_mark == g_cfg_mark_release_x32))
		{
			return s_str_lib32;
		}
		else if((cfg_mark == g_cfg_mark_debug_x64) || (cfg_mark == g_cfg_mark_release_x64))
		{
			return s_str_lib64;
		}
		else
		{
			return s_str_failed;
		}
	}
	else
	{
		return s_str_default;
	}
}


inline const ptree_type::key_type& select_outdir(const ptree_type::key_type& root_namespace,
													const ptree_type::key_type& app_tag,
													const ptree_type::key_type& cfg_mark)
{
	typedef ptree_type::key_type string_type;

	static const string_type& s_str_failed = "";
	static const string_type& s_str_default = "$(SolutionDir)$(Configuration)-$(Platform)/vc$(PlatformToolsetVersion)";

	static const string_type& s_str_lua_exe_release_x32 = "$(SolutionDir)exe32/vc$(PlatformToolsetVersion)/";
	static const string_type& s_str_lua_exe_release_x64 = "$(SolutionDir)exe64/vc$(PlatformToolsetVersion)/";

	static const string_type& s_str_bin64 = "$(SolutionDir)bin64/";
	static const string_type& s_str_bin32 = "$(SolutionDir)bin/";

	static const string_type& s_str_lib64 = "$(SolutionDir)lib64/";
	static const string_type& s_str_lib32 = "$(SolutionDir)lib/";

	if(app_tag == g_app_tag_dll)
	{
		if((cfg_mark == g_cfg_mark_debug_x32) || (cfg_mark == g_cfg_mark_release_x32))
		{
			return s_str_bin32;
		}
		else if((cfg_mark == g_cfg_mark_debug_x64) || (cfg_mark == g_cfg_mark_release_x64))
		{
			return s_str_bin64;
		}
		else
		{
			return s_str_failed;
		}
	}
	else if(app_tag == g_app_tag_static_lib)
	{
		if((cfg_mark == g_cfg_mark_debug_x32) || (cfg_mark == g_cfg_mark_release_x32))
		{
			return s_str_lib32;
		}
		else if((cfg_mark == g_cfg_mark_debug_x64) || (cfg_mark == g_cfg_mark_release_x64))
		{
			return s_str_lib64;
		}
		else
		{
			return s_str_failed;
		}
	}
	else if(app_tag == g_app_tag_exe)
	{
		if((root_namespace == "lua" || root_namespace == "luac"))
		{
			if(cfg_mark == g_cfg_mark_release_x32)
			{
				return s_str_lua_exe_release_x32;
			}
			else if(cfg_mark == g_cfg_mark_release_x64)
			{
				return s_str_lua_exe_release_x64;
			}
			else
			{
				return s_str_default;
			}
		}
		else
		{
			return s_str_default;
		}
	}
	else
	{
		return s_str_default;
	}
}

inline const ptree_type::key_type& select_import_lib_path(const ptree_type::key_type& app_tag, const ptree_type::key_type& cfg_mark)
{
	typedef ptree_type::key_type string_type;

	static const string_type& s_str_default = "";

	static const string_type& s_str_lib64 = "$(SolutionDir)lib64/lib$(TargetName).lib";
	static const string_type& s_str_lib32 = "$(SolutionDir)lib/lib$(TargetName).lib";

	if(app_tag == g_app_tag_dll)
	{
		if((cfg_mark == g_cfg_mark_debug_x32) || (cfg_mark == g_cfg_mark_release_x32))
		{
			return s_str_lib32;
		}
		else if((cfg_mark == g_cfg_mark_debug_x64) || (cfg_mark == g_cfg_mark_release_x64))
		{
			return s_str_lib64;
		}
		else
		{
			return s_str_default;
		}
	}
	else
	{
		return s_str_default;
	}
}

// vcxproj
yggr::utf8_string get_project_name(const yggr::utf8_string& str_file_path)
{
	std::size_t last_pos = str_file_path.find(".vcxproj");
	if(last_pos == 0 || last_pos == yggr::utf8_string::npos)
	{
		return yggr::utf8_string("");
	}

	std::size_t first_pos1 = str_file_path.rfind("/");
	std::size_t first_pos2 = str_file_path.rfind("\\");

	if(first_pos1 == yggr::utf8_string::npos)
	{
		if(first_pos2 == yggr::utf8_string::npos)
		{
			return yggr::utf8_string("");
		}
		else
		{
			first_pos1 = first_pos2;
		}
	}
	else
	{
		if(first_pos2 == yggr::utf8_string::npos)
		{
			first_pos2 = first_pos1;
		}
	}

	std::size_t first_pos = (std::max)(first_pos1, first_pos2);

	if(!(first_pos + 1 < last_pos))
	{
		return yggr::utf8_string("");
	}

	return yggr::utf8_string(str_file_path.begin() + first_pos + 1, str_file_path.begin() + last_pos);
}



ptree_type& load_vcxproj(ptree_type& ptree, const yggr::utf8_string& str_file_path)
{
	try
	{
		boost::property_tree::read_xml(str_file_path.c_str(), ptree, 4);
	}
	catch(...)
	{
		ptree.clear();
	}

	return ptree;
}

bool write_vcxproj(ptree_type& ptree, const yggr::utf8_string& str_file_path)
{
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;

	xml_writer_settings_type setting('\t', 1);
	try
	{
		boost::property_tree::write_xml(str_file_path.c_str(), ptree, std::locale(), setting);
	}
	catch(...)
	{
		return false;
	}

	return true;
}



ptree_type::key_type get_configure_mark(const ptree_type& ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	std::size_t platform_toolset_i = 0, platform_toolset_isize = 4;

	string_type cfg_val;


	for(const_iterator i = assoc_iter->second.begin(), isize = assoc_iter->second.end();
			((platform_toolset_i != platform_toolset_isize)) && (i != isize) ; ++i)
	{
		if(i->first == "PropertyGroup")
		{
			attribs_type attribs =
				i->second.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

			if(!attribs)
			{
				continue;
			}

			const ptree_type& sub_ptree_ref = attribs.get();
			assoc_citer_type attr_assoc_iter = sub_ptree_ref.find("Label");

			const_iterator check_iter = sub_ptree_ref.to_iterator(attr_assoc_iter);
			if(check_iter == sub_ptree_ref.end())
			{
				continue;
			}

			if(check_iter->second.get_value<string_type>() == "Configuration")
			{
				const_iterator iter_cfg_t = i->second.to_iterator(i->second.find("ConfigurationType"));

				if(iter_cfg_t == i->second.end())
				{
					return string_type();
				}

				if(0 == platform_toolset_i)
				{
					cfg_val = iter_cfg_t->second.get_value<string_type>();
				}
				else
				{
					if(cfg_val != iter_cfg_t->second.get_value<string_type>())
					{
						return string_type();
					}
				}

				++ platform_toolset_i;
			}
		}
	}

	return cfg_val;
}

ptree_type::key_type get_root_namespace(const ptree_type& ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	for(const_iterator i = assoc_iter->second.begin(), isize = assoc_iter->second.end();
			i != isize; ++i)
	{
		if(i->first == "PropertyGroup")
		{
			attribs_type attribs =
				i->second.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

			if(!attribs)
			{
				continue;
			}

			const ptree_type& sub_ptree_ref = attribs.get();
			assoc_citer_type attr_assoc_iter = sub_ptree_ref.find("Label");

			const_iterator check_iter = sub_ptree_ref.to_iterator(attr_assoc_iter);
			if(check_iter == sub_ptree_ref.end())
			{
				continue;
			}

			if(check_iter->second.get_value<string_type>() == "Globals")
			{
				const_iterator iter_cfg_t = i->second.to_iterator(i->second.find("RootNamespace"));

				return
					iter_cfg_t == i->second.end()?
						string_type()
						: iter_cfg_t->second.get_value<string_type>();
			}
		}
	}

	return string_type();
}


bool is_property_group_globals(const ptree_type& ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	attribs_type attribs =
		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

	if(!attribs)
	{
		return false;
	}

	const ptree_type& sub_ptree_ref = attribs.get();
	assoc_citer_type attr_assoc_iter = sub_ptree_ref.find("Label");

	const_iterator check_iter = sub_ptree_ref.to_iterator(attr_assoc_iter);

	return
		(check_iter != sub_ptree_ref.end())
		&& (check_iter->second.get_value<string_type>() == "Globals");
}

yggr::u32 format_root_namespace(ptree_type& ptree, const ptree_type::key_type& val)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;

	iterator iter_cfg = ptree.to_iterator(ptree.find("RootNamespace"));

	if(iter_cfg == ptree.end())
	{
		ptree.add_child("RootNamespace", ptree_type(val));
	}
	else
	{
		iter_cfg->second.put_value(val);
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

yggr::u32 format_keyword(ptree_type& ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;

	//iterator iter_cfg = ptree.to_iterator(ptree.find("Keyword"));

	//if(iter_cfg != ptree.end())
	//{
	//	ptree.erase(iter_cfg);
	//}

	ptree.erase("Keyword");

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

bool is_property_group_configuration(const ptree_type& ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	attribs_type attribs =
		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

	if(!attribs)
	{
		return false;
	}

	const ptree_type& sub_ptree_ref = attribs.get();
	assoc_citer_type attr_assoc_iter = sub_ptree_ref.find("Label");

	const_iterator check_iter = sub_ptree_ref.to_iterator(attr_assoc_iter);

	return
		(check_iter != sub_ptree_ref.end())
		&& (check_iter->second.get_value<string_type>() == "Configuration");
}

yggr::u32 format_property_group_configuration(const ptree_type::key_type& root_namespace, ptree_type& ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;

	string_type cfg_mark = get_attribs(ptree, "Condition");
	if(cfg_mark.empty())
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	{
		if(is_release_cfg_mark(cfg_mark)
			&& (root_namespace != "yggr_lua_base_type"))
		{
			iterator iter_cfg = ptree.to_iterator(ptree.find("WholeProgramOptimization"));

			if(iter_cfg == ptree.end())
			{
				ptree.add_child("WholeProgramOptimization",  ptree_type("true"));
			}
			else
			{
				iter_cfg->second.put_value(string_type("true"));
			}
		}
		else
		{
			ptree.erase("WholeProgramOptimization");
		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("CharacterSet"));

		if(iter_cfg == ptree.end())
		{
			ptree.add_child("CharacterSet", ptree_type("MultiByte"));
		}
		else
		{
			iter_cfg->second.put_value(string_type("MultiByte"));
		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("UseDebugLibraries"));

		if(iter_cfg == ptree.end())
		{
			ptree.add_child("UseDebugLibraries", is_debug_cfg_mark(cfg_mark)? ptree_type("true") : ptree_type("false"));
		}
		else
		{
			iter_cfg->second.put_value( is_debug_cfg_mark(cfg_mark)? string_type("true") : string_type("false"));
		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("UseOfMfc"));

		if(iter_cfg != ptree.end())
		{
			if(iter_cfg->second.get_value<string_type>() == "false")
			{
				ptree.erase(iter_cfg);
			}
		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("UseOfAtl"));

		if(iter_cfg != ptree.end())
		{
			if(iter_cfg->second.get_value<string_type>() == "false")
			{
				ptree.erase(iter_cfg);
			}
		}
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

// user_macro_intdir_out_dir

bool is_property_group_user_macro_data(const ptree_type& ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;

	const_iterator check_iter = ptree.to_iterator(ptree.find("IntDir"));

	if(check_iter != ptree.end())
	{
		return true;
	}

	check_iter = ptree.to_iterator(ptree.find("OutDir"));
	if(check_iter != ptree.end())
	{
		return true;
	}

	check_iter = ptree.to_iterator(ptree.find("LinkIncremental"));
	if(check_iter != ptree.end())
	{
		return true;
	}

	return false;
}

bool is_property_group_user_macro_data_no_cdt(const ptree_type& ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;


	return
		is_property_group_user_macro_data(ptree)
		&& has_attribs(ptree, "Condition");
}


inline yggr::u32 format_user_macro_data_outdir(ptree_type& ptree,
													const ptree_type::key_type& root_namespace,
													const ptree_type::key_type& app_tag,
													const ptree_type::key_type& cfg_mark,
													bool is_yggr_proj)
{
	typedef ptree_type::key_type string_type;

	ptree.erase("OutDir");

	const string_type& cfg_val =
		is_yggr_proj?
			select_outdir_yggr(root_namespace, app_tag, cfg_mark)
			: select_outdir(root_namespace, app_tag, cfg_mark);

	if(cfg_val.empty())
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	ptree.add_child("OutDir", ptree_type(cfg_val));
	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

inline yggr::u32 format_user_macro_data_intdir(ptree_type& ptree)
{
	typedef ptree_type::key_type string_type;

	static const string_type s_str = "$(Configuration)-$(Platform)/$(ProjectName)-vc$(PlatformToolsetVersion)/";

	ptree.erase("IntDir");
	ptree.add_child("IntDir", ptree_type(s_str));

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

inline ptree_type::key_type parse_target_name(const ptree_type::key_type& arg_target_name)
{
	std::size_t pos = arg_target_name.find('#');

	return
		pos == ptree_string_type::npos?
			arg_target_name
			: arg_target_name.substr(0, pos);
}

inline ptree_type::key_type parse_target_ext(const ptree_type::key_type& arg_target_name)
{
	std::size_t pos = arg_target_name.find('#');

	return
		pos == ptree_string_type::npos?
			ptree_string_type()
			: arg_target_name.substr(pos + 1, arg_target_name.size() - pos - 1);
}

inline ptree_type::key_type gen_target_name(const ptree_type::key_type& n_arg_target_name,
											const ptree_type::key_type& app_tag,
											const ptree_type::key_type& cfg_mark)
{
	typedef ptree_type::key_type string_type;

	static const string_type s_proj_name_default("$(ProjectName)");

	string_type arg_target_name = parse_target_name(n_arg_target_name);

	if(arg_target_name.size())
	{
		if(arg_target_name[0] == '@')
		{
			return arg_target_name.substr(1, arg_target_name.size() - 1);
		}
		else
		{
			if((cfg_mark == g_cfg_mark_debug_x32) || (cfg_mark == g_cfg_mark_debug_x64))
			{
				return arg_target_name + "-d";
			}
			else if((cfg_mark == g_cfg_mark_release_x32) || (cfg_mark == g_cfg_mark_release_x64))
			{
				return arg_target_name;
			}
			else
			{
				return string_type();
			}
		}
	}
	else
	{
		const string_type& proj_name = s_proj_name_default;

		string_type target_name;

		assert(is_cl_fix_object(app_tag));

		if(app_tag == g_app_tag_static_lib)
		{
			if((cfg_mark == g_cfg_mark_debug_x32) || (cfg_mark == g_cfg_mark_debug_x64))
			{
				target_name = "lib" + proj_name + "-vc$(PlatformToolsetVersion)-d";
			}
			else if((cfg_mark == g_cfg_mark_release_x32) || (cfg_mark == g_cfg_mark_release_x64))
			{
				target_name = "lib" + proj_name + "-vc$(PlatformToolsetVersion)";
			}
		}
		else if(app_tag == g_app_tag_dll)
		{
			if((cfg_mark == g_cfg_mark_debug_x32) || (cfg_mark == g_cfg_mark_debug_x64))
			{
				target_name = proj_name + "-vc$(PlatformToolsetVersion)-d";
			}
			else if((cfg_mark == g_cfg_mark_release_x32) || (cfg_mark == g_cfg_mark_release_x64))
			{
				target_name = proj_name + "-vc$(PlatformToolsetVersion)";
			}
		}
		else if(app_tag == g_app_tag_exe)
		{
			if((cfg_mark == g_cfg_mark_debug_x32) || (cfg_mark == g_cfg_mark_debug_x64))
			{
				target_name = proj_name;
			}
			else if((cfg_mark == g_cfg_mark_release_x32) || (cfg_mark == g_cfg_mark_release_x64))
			{
				target_name = proj_name;
			}
		}

		return target_name;
	}
}

inline yggr::u32 format_user_macro_data_target_name(ptree_type& ptree,
													const ptree_type::key_type& root_namespace,
													const project_name_map_type& proj_name_map,
													const ptree_type::key_type& app_tag,
													const ptree_type::key_type& cfg_mark)
{
	typedef ptree_type::key_type string_type;
	typedef project_name_map_type::const_iterator map_citer_type;

	ptree.erase("TargetName");

	//print_ptree(ptree);
	//std::cout << "------------" << std::endl;

	if(!is_cl_fix_object(app_tag))
	{
		return static_cast<yggr::u32>(fix_state::E_SUCCEED);
	}

	map_citer_type map_iter = proj_name_map.find(yggr::utf8_string(root_namespace));
	string_type arg_target_name = (map_iter == proj_name_map.end())? string_type() : map_iter->second.str<string_type>();

	{
		string_type cfg_val = gen_target_name(arg_target_name, app_tag, cfg_mark);
		if(cfg_val.empty())
		{
			return static_cast<yggr::u32>(fix_state::E_FAILED);
		}

		ptree.add_child("TargetName", ptree_type(cfg_val));
	}

	{
		string_type cfg_val = parse_target_ext(arg_target_name);
		if(cfg_val.size())
		{
			ptree.add_child("TargetExt", ptree_type(cfg_val));
		}
	}

	//print_ptree(ptree);

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

yggr::u32 format_user_macro_data(ptree_type& ptree,
									const ptree_type::key_type& root_namespace,
									const project_name_map_type& proj_name_map,
									const ptree_type::key_type& app_tag,
									bool is_yggr_proj)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;

	string_type cfg_mark = get_attribs(ptree, "Condition");
	if(cfg_mark.empty())
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	yggr::u32 chk_sat = static_cast<yggr::u32>(fix_state::E_FAILED);

	if((chk_sat = format_user_macro_data_outdir(ptree, root_namespace, app_tag, cfg_mark, is_yggr_proj)) != 0)
	{
		return chk_sat;
	}

	if((chk_sat = format_user_macro_data_intdir(ptree)) != 0)
	{
		return chk_sat;
	}

	if((chk_sat = format_user_macro_data_target_name(ptree, root_namespace, proj_name_map, app_tag, cfg_mark)) != 0)
	{
		return chk_sat;
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

// ItemDefinitionGroup

// Midl

yggr::u32 format_midl(ptree_type& ptree, const ptree_type::key_type& app_tag)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;

	{
		//iterator iter_cfg = ptree.to_iterator(ptree.find("TypeLibraryName")); // optional

		//if(iter_cfg != ptree.end())
		//{
		//	ptree.erase(iter_cfg);
		//}
		ptree.erase("TypeLibraryName");
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

string_set_type remove_opt_items_pp(string_set_type& str_set,
									const ptree_type::key_type& str_pp_export,
									bool is_yggr_proj)
{
	str_set.erase(str_pp_export);
	str_set.erase("_DEBUG");
	str_set.erase("NDEBUG");
	str_set.erase("WIN32");
	str_set.erase("WIN64");
	str_set.erase("_LIB");
	str_set.erase("_USRDLL");
	str_set.erase("_CONSOLE");
	str_set.erase("_WINDOWS");
	str_set.erase("YGGR_USE_SEH");
	str_set.erase("YGGR_INCLUDE_SEH_DEBUG");
	str_set.erase("%(PreprocessorDefinitions)");

	if(is_yggr_proj)
	{
		str_set.erase("LNUM_INT64");
		str_set.erase("LUA_USELONGLONG");
		str_set.erase("LUA_COMPAT_ALL");
		str_set.erase("LUA_COMPAT_5_1");
		str_set.erase("LUA_COMPAT_5_2");
	}

	return str_set;
}

ptree_string_type& check_and_fix_pp(ptree_string_type& str_pp)
{
	{
		static const ptree_string_type s_key__usrdll = "_USRDLL";

		for(std::size_t pos = str_pp.find(s_key__usrdll); pos != ptree_string_type::npos; pos =  str_pp.find(s_key__usrdll))
		{
			str_pp.erase(str_pp.begin() + pos, str_pp.begin() + (pos + s_key__usrdll.size()));
		}
	}

	return str_pp;
}

string_set_type& parse_opt_items(string_set_type& out,
									const ptree_type::key_type& str_opt_items)
{
	//typedef ptree_type::key_type ptree_string_type;
	typedef ptree_string_type::const_iterator citer_type;

	ptree_string_type tmp;

	for(citer_type i = str_opt_items.begin(), ipos = str_opt_items.begin(), isize = str_opt_items.end();
			i != isize; i = ipos)
	{
		ipos = std::find(i, isize, ';');
		tmp.assign(i, ipos);
		check_and_fix_pp(tmp);
		if(tmp.size())
		{
			out.insert(tmp);
		}
		if(ipos != isize)
		{
			++ipos;
		}
	}

	return out;
}

// ClCompile

ptree_type::key_type make_dll_exprot(const ptree_type::key_type& root_namespace)
{
	typedef ptree_type::key_type string_type;

	string_type str = root_namespace;

	for(string_type::iterator i = str.begin(), isize = str.end(); i != isize; ++i)
	{
		if('a' <= *i && (*i) <= 'z')
		{
			(*i) -= ('a' - 'A');
		}
	}

	return str + "_EXPORT";
}

ptree_type::key_type make_clcompile_pp_value(const string_set_type& pp_set,
												const ptree_type::key_type& str_pp_export,
												const ptree_type::key_type& root_namespace,
												const ptree_type::key_type& subsys,
												const ptree_type::key_type& app_tag,
												const ptree_type::key_type& cfg_mark)
{
	typedef ptree_type::key_type string_type;

	string_type str = "WIN32";

	if(cfg_mark == g_cfg_mark_debug_x32)
	{
		str += ";_DEBUG";
	}
	else if(cfg_mark == g_cfg_mark_release_x32)
	{
		str += ";NDEBUG";
	}
	else if(cfg_mark == g_cfg_mark_debug_x64)
	{
		str += ";WIN64";
		str += ";_DEBUG";
	}
	else if(cfg_mark == g_cfg_mark_release_x64)
	{
		str += ";WIN64";
		str += ";NDEBUG";
	}
	else
	{
		assert(false);
		return string_type();
	}

	if(app_tag == g_app_tag_exe)
	{
		if(subsys == "Console")
		{
			str += ";_CONSOLE";
		}
		else if(subsys == "Windows")
		{
			str += ";_WINDOWS";
		}
		else
		{
			assert(false);
			return string_type();
		}
	}
	else if(app_tag == g_app_tag_dll)
	{
		str += ";_WINDOWS";
		str += ";_USRDLL";
		if(str_pp_export.size())
		{
			str += ";" + str_pp_export;
		}
	}
	else if(app_tag == g_app_tag_static_lib)
	{
		str += ";_WINDOWS";
		str += ";_LIB";
	}
	else
	{
		assert(false);
		return string_type();
	}

	{
		assert(str.size());
		for(string_set_type::const_iterator i = pp_set.begin(), isize = pp_set.end(); i != isize; ++i)
		{
			str += (";" + *i);
		}
	}

	str += ";%(PreprocessorDefinitions)";

	return str;
}

yggr::u32 format_clcompile_pp(ptree_type& ptree,
								const ptree_type::key_type& root_namespace,
								const ptree_type::key_type& subsys,
								const ptree_type::key_type& app_tag,
								const ptree_type::key_type& cfg_mark,
								bool is_yggr_proj)
{
	typedef ptree_type::key_type string_type;

	string_type str_tmp = ptree.get_value<string_type>();
	string_set_type pp_set;
	parse_opt_items(pp_set, str_tmp);
	string_type str_pp_export = make_dll_exprot(root_namespace);
	remove_opt_items_pp(pp_set, str_pp_export, is_yggr_proj);

	str_tmp = make_clcompile_pp_value(pp_set, str_pp_export, root_namespace, subsys, app_tag, cfg_mark);

	if(str_tmp.empty())
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	ptree.put_value(str_tmp);

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

struct is_space_t
{
	inline bool operator()(char c) const
	{
		return ((c == ' ') || (c == '\t'));
	}
};

struct is_not_space_t
{
	inline bool operator()(char c) const
	{
		return !((c == ' ') || (c == '\t'));
	}
};

string_set_type& parse_additional_options(string_set_type& out, const ptree_type::key_type& str_opt_items)
{
	typedef ptree_string_type::const_iterator citer_type;

	is_space_t cmp_yes;
	is_not_space_t cmp_no;

	citer_type ipos_s, ipos_e;
	for(citer_type i = str_opt_items.begin(), isize = str_opt_items.end(); i != isize; i = ipos_e)
	{
		ipos_s = std::find_if(i, isize, cmp_no);

		if(ipos_s == isize)
		{
			return out;
		}

		ipos_e = std::find_if(ipos_s, isize, cmp_yes);

		if(ipos_s != ipos_e)
		{
			out.insert(ptree_string_type(ipos_s, ipos_e));
		}

		if(ipos_e != isize)
		{
			++ipos_e;
		}
	}

	return out;
}

ptree_type::key_type& format_additional_options(ptree_type::key_type& str_opts)
{
	typedef ptree_type::key_type string_type;

	string_set_type tmp;
	parse_additional_options(tmp, str_opts);
	tmp.erase("/D_STLP_DEBUG=1");
	tmp.erase("%(AdditionalOptions)");

	string_type str_opts_out;

	if(tmp.size())
	{
		for(string_set_type::const_iterator i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
		{
			str_opts_out += ((i == tmp.begin())? (*i) : (" " + *i));
		}
	}

	str_opts_out.swap(str_opts);

	return str_opts;
}


yggr::u32 format_clcompile(ptree_type& ptree,
							const ptree_type::key_type& root_namespace,
							const ptree_type::key_type& subsys,
							const ptree_type::key_type& app_tag,
							const ptree_type::key_type& cfg_mark,
							bool is_yggr_proj)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;


	{
		ptree.erase("AssemblerListingLocation");
		ptree.erase("BasicRuntimeChecks");
		ptree.erase("BrowseInformation");
		ptree.erase("FavorSizeOrSpeed");
		ptree.erase("InlineFunctionExpansion");
		ptree.erase("MinimalRebuild");
		ptree.erase("ObjectFileName");
		ptree.erase("PrecompiledHeaderOutputFile");
		ptree.erase("RuntimeLibrary");
		ptree.erase("RuntimeTypeInfo");
		ptree.erase("StringPooling");
		ptree.erase("SuppressStartupBanner");
		ptree.erase("XMLDocumentationFileName");
	}

	{
		ptree.erase("DebugInformationFormat");
		ptree.erase("FunctionLevelLinking");
		ptree.erase("IntrinsicFunctions");
		ptree.erase("WholeProgramOptimization");

		if(is_release_cfg_mark(cfg_mark))
		{
			ptree.add_child("FunctionLevelLinking", ptree_type("true"));
			ptree.add_child("IntrinsicFunctions", ptree_type("true"));

			if(app_tag == g_app_tag_static_lib)
			{
				ptree.add_child("WholeProgramOptimization", ptree_type("false"));
			}
		}

		if(is_debug_cfg_mark(cfg_mark))
		{
			ptree.add_child("DebugInformationFormat", ptree_type("ProgramDatabase"));
		}
	}


	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("ProgramDataBaseFileName"));

		if(iter_cfg == ptree.end())
		{
			ptree.add_child("ProgramDataBaseFileName", ptree_type("$(IntDir)$(TargetName).pdb"));
		}
		else
		{
			iter_cfg->second.put_value(string_type("$(IntDir)$(TargetName).pdb"));
		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("AdditionalIncludeDirectories"));

		if(iter_cfg != ptree.end())
		{
			string_type str = iter_cfg->second.get_value<string_type>();
			if(format_string_file_path(str).empty()
				|| str == "%(AdditionalIncludeDirectories)")
			{
				ptree.erase(iter_cfg);
			}
			else
			{
				iter_cfg->second.put_value(str);
			}
		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("PreprocessorDefinitions"));

		if(iter_cfg != ptree.end())
		{
			if(format_clcompile_pp(iter_cfg->second, root_namespace, subsys, app_tag, cfg_mark, is_yggr_proj) != 0)
			{
				return static_cast<yggr::u32>(fix_state::E_FAILED);
			}
		}
		else
		{
			if(format_clcompile_pp(
				ptree.add_child("PreprocessorDefinitions", ptree_type()),
				root_namespace, subsys, app_tag, cfg_mark, is_yggr_proj) != 0)
			{
				return static_cast<yggr::u32>(fix_state::E_FAILED);
			}
		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("AdditionalOptions"));

		if(iter_cfg != ptree.end())
		{
			string_type str = iter_cfg->second.get_value<string_type>();
			if(format_additional_options(str).empty())
			{
				ptree.erase(iter_cfg);
			}
			else
			{
				iter_cfg->second.put_value(str);
			}
		}
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

// Link
yggr::u32 format_link(ptree_type& ptree, const ptree_type::key_type& app_tag, const ptree_type::key_type& cfg_mark, bool is_yggr_proj)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;

	{
		ptree.erase("AssemblyDebug");
		ptree.erase("DataExecutionPrevention");
		ptree.erase("GenerateMapFile");
		ptree.erase("IgnoreSpecificDefaultLibraries");
		ptree.erase("LinkTimeCodeGeneration");
		ptree.erase("NoEntryPoint");
		ptree.erase("RandomizedBaseAddress");
		ptree.erase("SuppressStartupBanner");
		ptree.erase("TargetMachine");
		ptree.erase("UACExecutionLevel");
		ptree.erase("UACUIAccess");
	}

	{
		ptree.erase("GenerateDebugInformation");
		ptree.erase("EnableCOMDATFolding");
		ptree.erase("OptimizeReferences");

		if(is_debug_cfg_mark(cfg_mark))
		{
			ptree.add_child("GenerateDebugInformation", ptree_type("true"));
		}
		else if(is_release_cfg_mark(cfg_mark))
		{
			ptree.add_child("GenerateDebugInformation", ptree_type("false"));
			ptree.add_child("EnableCOMDATFolding", ptree_type("true"));
			ptree.add_child("OptimizeReferences", ptree_type("true"));
		}
		else
		{
			return static_cast<yggr::u32>(fix_state::E_FAILED);
		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("OutputFile"));

		if(iter_cfg == ptree.end())
		{
			ptree.add_child("OutputFile", ptree_type("$(OutDir)$(TargetName)$(TargetExt)"));
		}
		else
		{
			iter_cfg->second.put_value(string_type("$(OutDir)$(TargetName)$(TargetExt)"));
		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("ProgramDatabaseFile"));

		if(iter_cfg == ptree.end())
		{
			ptree.add_child("ProgramDatabaseFile", ptree_type("$(IntDir)$(TargetName).pdb"));
		}
		else
		{
			iter_cfg->second.put_value(string_type("$(IntDir)$(TargetName).pdb"));
		}
	}

	{
		if(is_yggr_proj)
		{
			ptree.erase("ImportLibrary");
		}
		else
		{
			iterator iter_cfg = ptree.to_iterator(ptree.find("ImportLibrary"));

			if(app_tag == g_app_tag_dll)
			{
				const string_type& cfg_val = select_import_lib_path(app_tag, cfg_mark);
				assert(cfg_val.size());
				if(iter_cfg == ptree.end())
				{
					ptree.add_child("ImportLibrary", ptree_type(cfg_val));
				}
				else
				{
					iter_cfg->second.put_value(string_type(cfg_val));
				}
			}
			else
			{
				if(iter_cfg != ptree.end())
				{
					ptree.erase(iter_cfg);
				}
			}
		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("AdditionalLibraryDirectories"));

		if(iter_cfg != ptree.end())
		{
			string_type str = iter_cfg->second.get_value<string_type>();
			if(format_string_file_path(str).empty()
				|| str == "%(AdditionalLibraryDirectories)")
			{
				ptree.erase(iter_cfg);
			}
			else
			{
				iter_cfg->second.put_value(str);
			}

		}
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("AdditionalDependencies"));

		if(iter_cfg != ptree.end())
		{
			string_type str = iter_cfg->second.get_value<string_type>();
			if(format_string_file_path(str).empty()
				|| str == "%(AdditionalDependencies)")
			{
				ptree.erase(iter_cfg);
			}
			else
			{
				iter_cfg->second.put_value(str);
			}

		}
	}

	if(app_tag == g_app_tag_dll)
	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("SubSystem"));

		if(iter_cfg == ptree.end())
		{
			ptree.add_child("SubSystem", ptree_type("Windows"));
		}
		else
		{
			iter_cfg->second.put_value(string_type("Windows"));
		}
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

// Lib
yggr::u32 format_lib(ptree_type& ptree, const ptree_type::key_type& app_tag, const ptree_type::key_type& cfg_mark)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;

	{
		ptree.erase("SuppressStartupBanner");
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("OutputFile"));

		if(iter_cfg == ptree.end())
		{
			ptree.add_child("OutputFile", ptree_type("$(OutDir)$(TargetName)$(TargetExt)"));
		}
		else
		{
			iter_cfg->second.put_value(string_type("$(OutDir)$(TargetName)$(TargetExt)"));
		}
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

// Bscmake
yggr::u32 format_bscmake(ptree_type& ptree, const ptree_type::key_type& app_tag)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;

	{
		ptree.erase("SuppressStartupBanner");
	}

	{
		iterator iter_cfg = ptree.to_iterator(ptree.find("OutputFile")); // optional

		if(iter_cfg != ptree.end())
		{
			iter_cfg->second.put_value(string_type("$(OutDir)$(TargetName).bsc"));
		}
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

yggr::u32 format_item_definition_group(ptree_type& ptree,
										const ptree_type::key_type& root_namespace,
										const ptree_type::key_type& app_tag,
										bool is_yggr_proj)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;

	string_type cfg_mark = get_attribs(ptree, "Condition");

	if(cfg_mark.empty())
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	string_type subsys;

	if(app_tag == g_app_tag_exe)
	{
		iterator iter_link = ptree.to_iterator(ptree.find("Link"));

		if(iter_link == ptree.end())
		{
			return static_cast<yggr::u32>(fix_state::E_FAILED);
		}

		iterator iter_subsys = iter_link->second.to_iterator(iter_link->second.find("SubSystem"));

		if(iter_subsys == iter_link->second.end())
		{
			return static_cast<yggr::u32>(fix_state::E_FAILED);
		}

		subsys = iter_subsys->second.get_value<string_type>();
	}

	yggr::u32 chk_sat = static_cast<yggr::u32>(fix_state::E_FAILED);

	{
		ptree.erase("Xdcmake");
	}

	{
		iterator iter = ptree.to_iterator(ptree.find("Midl")); // remove

		if(iter != ptree.end())
		{
			ptree.erase("Midl");
		}
	}

	if(!is_cl_fix_object(app_tag))
	{
		return static_cast<yggr::u32>(fix_state::E_SUCCEED);
	}

	{
		iterator iter = ptree.to_iterator(ptree.find("ClCompile"));
		if(iter == ptree.end())
		{
			return static_cast<yggr::u32>(fix_state::E_FAILED);
		}

		if((chk_sat = format_clcompile(iter->second, root_namespace, subsys, app_tag, cfg_mark, is_yggr_proj)) != 0)
		{
			return chk_sat;
		}
	}

	ptree.erase("ProjectReference");

	{
		if(is_yggr_proj)
		{
			ptree.erase("ResourceCompile");
		}
		else
		{
			iterator iter = ptree.to_iterator(ptree.find("ResourceCompile"));

			if(iter != ptree.end())
			{
				if(iter->second.find("Culture") != iter->second.not_found())
				{
					ptree.erase(iter);
				}
			}
		}
	}

	{
		if(app_tag == g_app_tag_static_lib)
		{
			iterator iter = ptree.to_iterator(ptree.find("Lib"));

			chk_sat =
				(iter == ptree.end())?
					static_cast<yggr::u32>(fix_state::E_FAILED)
					: format_lib(iter->second, app_tag, cfg_mark);
		}
		else
		{
			assert(((app_tag == g_app_tag_exe) || (app_tag == g_app_tag_dll)));

			iterator iter = ptree.to_iterator(ptree.find("Link"));

			chk_sat =
				(iter == ptree.end())?
					static_cast<yggr::u32>(fix_state::E_FAILED)
					: format_link(iter->second, app_tag, cfg_mark, is_yggr_proj);
		}

		if(chk_sat != 0)
		{
			return chk_sat;
		}
	}

	{
		iterator iter = ptree.to_iterator(ptree.find("Bscmake")); // optional
		if(iter != ptree.end())
		{
			if((chk_sat = format_bscmake(iter->second, app_tag)) != 0)
			{
				return chk_sat;
			}
		}
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

yggr::u32 format_no_cdt_property_items(ptree_type& ptree,
										const ptree_type::key_type& app_tag)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::value_type ptree_val_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;

	iterator user_macro_beg = ptree.begin(), user_macro_end = ptree.begin();
	for(iterator i = ptree.begin(), isize = ptree.end(); (i != isize);)
	{
		if(i->first == "ItemDefinitionGroup")
		{
			user_macro_end = i;
			break;
		}

		if(i->first != "PropertyGroup")
		{
			++i;
			continue;
		}

		if(get_attribs(i->second, "Label") == "UserMacros")
		{
			user_macro_beg = i;
			++i;
			continue;
		}

		if(has_attribs(i->second, "Label"))
		{
			++i;
			continue;
		}

		i = ptree.erase(i);
	}

	if(user_macro_end == ptree.end())
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	if(user_macro_beg == ptree.begin())
	{
		ptree.insert(user_macro_end, ptree_val_type("PropertyGroup", ptree_type()))
			->second.add_child("<xmlattr>.Lable", ptree_type("UserMacros"));
	}

	{
		iterator iter_new_pg = ptree.insert(user_macro_end, ptree_val_type("PropertyGroup", ptree_type()));
		iter_new_pg->second.add_child("<xmlattr>.Condition", ptree_type(g_cfg_mark_debug_x32));
		iter_new_pg->second.add_child("LinkIncremental", ptree_type("true"));
	}

	{
		iterator iter_new_pg = ptree.insert(user_macro_end, ptree_val_type("PropertyGroup", ptree_type()));
		iter_new_pg->second.add_child("<xmlattr>.Condition", ptree_type(g_cfg_mark_debug_x64));
		iter_new_pg->second.add_child("LinkIncremental", ptree_type("true"));
	}

	{
		iterator iter_new_pg = ptree.insert(user_macro_end, ptree_val_type("PropertyGroup", ptree_type()));
		iter_new_pg->second.add_child("<xmlattr>.Condition", ptree_type(g_cfg_mark_release_x32));
		iter_new_pg->second.add_child("LinkIncremental", ptree_type("false"));
	}

	{
		iterator iter_new_pg = ptree.insert(user_macro_end, ptree_val_type("PropertyGroup", ptree_type()));
		iter_new_pg->second.add_child("<xmlattr>.Condition", ptree_type(g_cfg_mark_release_x64));
		iter_new_pg->second.add_child("LinkIncremental", ptree_type("false"));
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

yggr::u32 format_property_items_detail(ptree_type& ptree,
										const ptree_type::key_type& root_namespace,
										const project_name_map_type& proj_name_map,
										const ptree_type::key_type& app_tag,
										bool is_yggr_proj)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;

	assert(is_need_fix_of_app_tag(app_tag));

	if(!is_need_fix_of_app_tag(app_tag))
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	assoc_iter_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	if(static_cast<yggr::u32>(fix_state::E_SUCCEED)
		 != format_no_cdt_property_items(assoc_iter->second, app_tag))
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	yggr::u32 chk_sat = static_cast<yggr::u32>(fix_state::E_FAILED);

	for(iterator i = assoc_iter->second.begin(), isize = assoc_iter->second.end();
			(i != isize); /*++i*/)
	{
		if(i->first == "PropertyGroup")
		{
			// root_namespace
			if(is_property_group_globals(i->second))
			{
				if((chk_sat = format_root_namespace(i->second, root_namespace)) != 0)
				{
					return chk_sat;
				}

				if((chk_sat = format_keyword(i->second)) != 0)
				{
					return chk_sat;
				}

				++i;
				continue;
			}

			if(is_property_group_configuration(i->second))
			{
				if((chk_sat = format_property_group_configuration(root_namespace, i->second)) != 0)
				{
					return chk_sat;
				}

				++i;
				continue;
			}

			// intdir outdir
			if(is_property_group_user_macro_data(i->second))
			{
				if((chk_sat = format_user_macro_data(i->second, root_namespace, proj_name_map, app_tag, is_yggr_proj)) != 0)
				{
					return chk_sat;
				}

				//print_ptree(i->second);
				++i;
				continue;
			}
		}

		if(i->first == "ItemDefinitionGroup")
		{
			if((chk_sat = format_item_definition_group(i->second, root_namespace, app_tag, is_yggr_proj)) != 0)
			{
				return chk_sat;
			}

			//print_ptree(i->second);
			++i;
			continue;
		}

		if(i->first == "ItemGroup")
		{
			i->second.erase("ProjectReference");

			if(i->second.empty())
			{
				i = assoc_iter->second.erase(i);
			}
			else
			{
				++i;
			}

			continue;
		}

		++i;
	}

	return static_cast<yggr::u32>(fix_state::E_SUCCEED);
}

yggr::u32 format_property_items(ptree_type& ptree,
								const ptree_type::key_type& root_namespace,
								const project_name_map_type& proj_name_map,
								bool is_yggr_proj)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;

	assert(!root_namespace.empty());

	if(root_namespace.empty())
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	if(ptree.empty())
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	string_type app_tag = get_configure_mark(ptree);

	if(app_tag.empty())
	{
		return static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	if(!is_need_fix_of_app_tag(app_tag))
	{
		return static_cast<yggr::u32>(fix_state::E_IGNORE);
	}

	return format_property_items_detail(ptree, root_namespace, proj_name_map, app_tag, is_yggr_proj);
}

bool is_yggr_vcxproj(const yggr::utf8_string& str_file_path)
{
	path_type path = local_fsys_operators_type::make_path_object(str_file_path);

	if(path.empty())
	{
		return false;
	}

	return
		local_fsys_operators_type::is_exists(
			(path.parent_path().parent_path() / "yggr.sln"));
}

void fix_vcxproj(const yggr::utf8_string& str_file_path,
					const project_name_map_type& proj_name_map,
					file_list_type& failed_file_list,
					file_list_type& ignore_file_list)
{
	typedef ptree_type::key_type string_type;

	yggr::u32 chk_sat = static_cast<yggr::u32>(fix_state::E_SUCCEED);

	string_type root_namespace = get_project_name(str_file_path).str<string_type>();
	if(root_namespace.empty())
	{
		chk_sat = static_cast<yggr::u32>(fix_state::E_FAILED);
	}

	bool is_yggr_proj = is_yggr_vcxproj(str_file_path);

	ptree_type ptree;
	if(chk_sat == static_cast<yggr::u32>(fix_state::E_SUCCEED))
	{
		load_vcxproj(ptree, str_file_path);

		std::cout << "format_vcxproj " << str_file_path << std::endl;
		chk_sat = format_property_items(ptree, root_namespace, proj_name_map, is_yggr_proj);
	}

	switch(chk_sat)
	{
	case fix_state::E_SUCCEED:
#if !ONLY_MAKE_RUN_TEST()
		{
			write_vcxproj(ptree, str_file_path);
		}
#endif // ONLY_MAKE_RUN_TEST()
		break;
	case fix_state::E_FAILED:
		{
			std::cerr << "!!! failed fix vcxproj " << str_file_path << " !!!" << std::endl;
			failed_file_list.insert(str_file_path);
		}
		break;
	case fix_state::E_IGNORE:
		{
			std::cout << "~~~ ignore fix vcxproj " << str_file_path << " ~~~" << std::endl;
			ignore_file_list.insert(str_file_path);
		}
		break;
	default:
		assert(false);
	}
}

void fix_vcxproj_list(const file_list_type& file_list,
						const project_name_map_type& proj_name_map,
						file_list_type& failed_file_list,
						file_list_type& ignore_file_list)
{
	for(file_list_type::const_iterator i = file_list.begin(), isize = file_list.end(); i != isize; ++i)
	{
		fix_vcxproj(*i, proj_name_map, failed_file_list, ignore_file_list);
	}
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
										const filter_list_type& file_filter,
										file_list_type& vcxproj_files)
{
	typedef local_fsys_operators_type::path_type path_type;

	const path_type& fpath = entry.path();

	if(!fpath.has_extension())
	{
		return;
	}

	if(file_filter.find(yggr::utf8_string(fpath.filename().native())) != file_filter.end())
	{
		return;
	}

	//std::cout << fpath << std::endl;

	if(fpath.extension() == ".vcxproj")
	{
		std::cout << fpath << std::endl;
		vcxproj_files.insert(yggr::utf8_string(fpath.native()));
	}

	return;
}

void get_files(const yggr::utf8_string& str_root_path,
				const filter_list_type& dir_filter,
				const filter_list_type& file_filter,
				file_list_type& vcxproj_files)
{
	path_type root_dir_path = local_fsys_operators_type::make_path_object(str_root_path);

	try
	{
		local_fsys_operators_type::recursive_for_each(
			root_dir_path,
			boost::bind(
				&handler_for_each_search_files, _1, _2, _3,
				boost::cref(file_filter),
				boost::ref(vcxproj_files)),
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

// update

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

bool write_cfg_file(const yggr::utf8_string& fname, const tool_format_vcxproj_cfg_type& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP(tool_format_vcxproj_cfg_mark, cfg));

	return write_to_file(fname, opak.org_buf());
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
		case fix_state::E_FAILED:
			ss << "failed:\n";
			break;
		case fix_state::E_IGNORE:
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
		case fix_state::E_FAILED:
			ss << "succeed no failed files \n";
			break;
		case fix_state::E_IGNORE:
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
	return write_to_file(fname, ss.str());
#endif // ONLY_MAKE_RUN_TEST()

}

bool read_cfg_file(const yggr::utf8_string& fname, tool_format_vcxproj_cfg_type& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

	ipak_type ipak;
	return
		read_from_file(ipak.org_buf(), fname).size()
		&& ipak.load(YGGR_SERIALIZATION_NAME_NVP(tool_format_vcxproj_cfg_mark, cfg));
}

static yggr::utf8_string cfg_fname("tool_format_vcxproj_cfg.xml");

int main_detail(int argc, char* argv[])
{
	if(argc > 1)
	{
		cfg_fname = yggr::utf8_string(argv[1], YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	if(!local_fsys_operators_type::is_exists(local_fsys_operators_type::make_path_object(cfg_fname)))
	{
		tool_format_vcxproj_cfg_type cfg_sample = tool_format_vcxproj_cfg_sample();
		write_cfg_file(cfg_fname, cfg_sample);
		std::cout << "gen config file succeed, please modify it" << std::endl;
		return -1;
	}

	tool_format_vcxproj_cfg_type cfg;

	if(!read_cfg_file(cfg_fname, cfg))
	{
		std::cout << "read config file failed" << std::endl;
		return -1;
	}

	file_list_type vcxproj_files;
	file_list_type failed_vcxproj_files;
	file_list_type ignore_vcxproj_files;

	get_files(cfg.root_dir_, cfg.dir_filter_, cfg.file_filter_, vcxproj_files);
	//get_files("./test_data", cfg.dir_filter_, cfg.file_filter_, vcxproj_files);

	fix_vcxproj_list(vcxproj_files, cfg.proj_name_map_, failed_vcxproj_files, ignore_vcxproj_files);

	write_log_file(yggr::utf8_string("tool_format_vcxproj_failed_log.txt"), fix_state::E_FAILED, failed_vcxproj_files);
	write_log_file(yggr::utf8_string("tool_format_vcxproj_ignore_log.txt"), fix_state::E_IGNORE, ignore_vcxproj_files);

	return 0;
}

#endif //BOOST_VERSION < 105600

#if BOOST_VERSION < 105600

int main(int argc, char* argv[])
{
	assert(("Message: this tool not support boost 1.55 and older", false));
	std::cout << "end" << std::endl;

	if(!(argc > 1))
	{
		char cc = 0;
		std::cin >> cc;
	}

	return -1;
}

#else

int main(int argc, char* argv[])
{
	int ret = main_detail(argc, argv);

	std::cout << "end" << std::endl;

	char cc = 0;
	std::cin >> cc;

	return ret;
}

#endif // #if BOOST_VERSION < 105600

