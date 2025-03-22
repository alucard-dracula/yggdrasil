// tool_upgrade_vcxproj.cpp

#include <iostream>
#include <cassert>
#include <yggr/base/yggrdef.h>

#if BOOST_VERSION < 105600

//#	error this tool not support boost 1.55 and older

#	else

#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/set.hpp>
#include <yggr/container/unordered_set.hpp>
#include <yggr/regular_parse/regex_parse.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>
#include <yggr/file_system/boost_filesystem_comp.hpp>
#include "vcxproj_parser_patch.hpp"
#include "tool_upgrade_vcxproj_cfg.hpp"

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
typedef yggr::set<yggr::utf8_string> ver_list_type;

yggr::utf8_string ver_cpp03 = "_cpp03";
yggr::utf8_string ver_cpp11 = "_cpp11";
yggr::utf8_string ver_cpp14 = "_cpp14";
yggr::utf8_string ver_cpp17 = "_cpp17";
yggr::utf8_string ver_cpp20 = "_cpp20";
yggr::utf8_string ver_cpp23 = "_cpp23";

yggr::u32 num_ver_cpp03 = 2003;
yggr::u32 num_ver_cpp11 = 2011;
yggr::u32 num_ver_cpp14 = 2014;
yggr::u32 num_ver_cpp17 = 2017;
yggr::u32 num_ver_cpp20 = 2020;
yggr::u32 num_ver_cpp23 = 2023;

const ptree_type::key_type g_cfg_mark_debug_x32 = "'$(Configuration)|$(Platform)'=='Debug|Win32'";
const ptree_type::key_type g_cfg_mark_release_x32 = "'$(Configuration)|$(Platform)'=='Release|Win32'";
const ptree_type::key_type g_cfg_mark_debug_x64 = "'$(Configuration)|$(Platform)'=='Debug|x64'";
const ptree_type::key_type g_cfg_mark_release_x64 = "'$(Configuration)|$(Platform)'=='Release|x64'";

const ptree_type::key_type g_app_tag_exe = "Application";
const ptree_type::key_type g_app_tag_dll = "DynamicLibrary";
const ptree_type::key_type g_app_tag_static_lib = "StaticLibrary";
const ptree_type::key_type g_app_tag_utility = "Utility";
const ptree_type::key_type g_app_tag_makefile = "Makefile";

typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;
typedef local_fsys_operators_type::directory_entry_type directory_entry_type;
typedef local_fsys_operators_type::path_type path_type;

typedef tool_upgrade_vcxproj_cfg tool_upgrade_vcxproj_cfg_type;

static const char* tool_upgrade_vcxproj_cfg_mark = "tool_upgrade_vcxproj_cfg";

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

yggr::utf8_string make_new_file_name(const yggr::utf8_string& org_str_file_path,
										const yggr::utf8_string& ext,
										const yggr::utf8_string& str_ver = "")
{
	std::size_t last_pos = org_str_file_path.find(ext);
	if(last_pos == 0 || last_pos == yggr::utf8_string::npos)
	{
		return yggr::utf8_string("");
	}

	yggr::utf8_string ret;
	ret.reserve(org_str_file_path.size() + str_ver.size());
	ret.append(org_str_file_path.begin(), org_str_file_path.begin() + last_pos);
	ret.append(str_ver);
	ret.append(org_str_file_path.begin() + last_pos, org_str_file_path.end());

	return ret;
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

ptree_type& append_property_items(ptree_type& ptree, const yggr::utf8_string& project_name, const yggr::utf8_string& str_ver)
{
	typedef ptree_type::key_type ptree_string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<ptree_type&> attribs_type;

	assoc_iter_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return ptree;
	}

	//std::size_t globals_i = 0, globals_size = 1;
	//std::size_t platform_toolset_i = 0, platform_toolset_size = 0;
	//std::size_t language_standard_i = 0, language_standard_size = 0;
	yggr::string language_standard_val;
	yggr::u32 num_cpp_ver = 2003;

	if(str_ver == ver_cpp03)
	{
		num_cpp_ver = 2003;
	}
	else if(str_ver == ver_cpp14)
	{
		//platform_toolset_size = language_standard_size = 4;
		language_standard_val = "stdcpp14";
		num_cpp_ver = 2014;
	}
	else if(str_ver == ver_cpp17)
	{
		//platform_toolset_size = language_standard_size = 4;
		language_standard_val = "stdcpp17";
		num_cpp_ver = 2017;
	}
	else if(str_ver == ver_cpp20)
	{
		//platform_toolset_size = language_standard_size = 4;
		language_standard_val = "stdcpp20";
		num_cpp_ver = 2020;
	}
	else if(str_ver == ver_cpp23)
	{
		assert(false);
		//platform_toolset_size = language_standard_size = 4;
		language_standard_val = "stdcpp23";
		num_cpp_ver = 2023;
	}
	else{}

	//for(iterator i = assoc_iter->second.begin(), isize = assoc_iter->second.end();
	//		((globals_i < globals_size)
	//			|| (platform_toolset_i < platform_toolset_size)
	//			|| (language_standard_i < language_standard_size))
	//		&& (i != isize) ; ++i)
	for(iterator i = assoc_iter->second.begin(), isize = assoc_iter->second.end();
			i != isize; ++i)
	{
		if(i->first == "PropertyGroup")
		{
			attribs_type attribs =
				i->second.get_child_optional(boost::property_tree::xml_parser::xmlattr<ptree_type::key_type>());

			if(!attribs)
			{
				continue;
			}

			//auto tmp = attribs.get();
			ptree_type& sub_ptree_ref = attribs.get();
			assoc_iter_type attr_assoc_iter = sub_ptree_ref.find("Label");

			iterator check_iter = sub_ptree_ref.to_iterator(attr_assoc_iter);
			if(check_iter == sub_ptree_ref.end())
			{
				continue;
			}

			assoc_iter_type cfg_mark_iter = sub_ptree_ref.find("Condition");

			ptree_string_type cfg_mark = 
				cfg_mark_iter == sub_ptree_ref.not_found()?
					ptree_string_type() 
					: cfg_mark_iter->second.get_value<ptree_string_type>();

			if(check_iter->second.get_value<ptree_type::key_type>() == "Globals")
			{
				// <ProjectName>base64</ProjectName>
				if(i->second.find("ProjectName") == i->second.not_found())
				{
					i->second.add_child("ProjectName", ptree_type(project_name.org_str()));
				}

				if(num_ver_cpp03 < num_cpp_ver)
				{
					// <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
					if(i->second.find("WindowsTargetPlatformVersion") == i->second.not_found())
					{
						i->second.add_child("WindowsTargetPlatformVersion", ptree_type("10.0"));
					}
				}
				//++globals_i;
			}
			else if(check_iter->second.get_value<ptree_type::key_type>() == "Configuration")
			{
				// <PlatformToolset>v143</PlatformToolset>
				if(num_ver_cpp03 < num_cpp_ver)
				{
					{
						iterator tool_set_iter = i->second.to_iterator(i->second.find("PlatformToolset"));
						if(tool_set_iter == i->second.end())
						{
							i->second.add_child("PlatformToolset", ptree_type("v143"));
						}
						else
						{
							tool_set_iter->second.put_value(ptree_type::key_type("v143"));
						}
					}

					if(cfg_mark == g_cfg_mark_release_x32
						|| cfg_mark == g_cfg_mark_release_x64)
					{
						assoc_iter_type iter_opt = i->second.find("WholeProgramOptimization");
						if(iter_opt == i->second.not_found())
						{
							i->second.add_child("WholeProgramOptimization", ptree_type("true"));
						}
						else
						{
							iter_opt->second.put_value(ptree_type::key_type("true"));
						}
					}
				}

				//++ platform_toolset_i;
			}
		}
		else if(i->first == "ItemDefinitionGroup")
		{
			assoc_iter_type assoc_iter_j = i->second.find("ClCompile");
			if(assoc_iter_j == i->second.not_found())
			{
				//assert(false);
				continue;
			}
			else
			{
				// <LanguageStandard>stdcppxx</LanguageStandard>
				if(num_ver_cpp03 < num_cpp_ver)
				{
					if(assoc_iter_j->second.find("LanguageStandard") == assoc_iter_j->second.not_found())
					{
						 assoc_iter_j->second.add_child("LanguageStandard", ptree_type(language_standard_val));
					}
				}

				if(!(num_cpp_ver < num_ver_cpp20))
				{
					assoc_iter_type assoc_iter_gm = assoc_iter_j->second.find("MinimalRebuild");
					if(assoc_iter_gm != assoc_iter_j->second.not_found())
					{
						assoc_iter_j->second.erase(assoc_iter_j->second.to_iterator(assoc_iter_gm));
					}
				}

				//++language_standard_i;
			}
		}
	}

	return ptree;
}

void fix_vcxproj(const yggr::utf8_string& str_file_path, const yggr::utf8_string& str_ver)
{
	yggr::utf8_string str_project_name = get_project_name(str_file_path);
	yggr::utf8_string str_new_file_path = make_new_file_name(str_file_path, ".vcxproj", str_ver);

	ptree_type ptree;
	load_vcxproj(ptree, str_file_path);

	append_property_items(ptree, str_project_name, str_ver);

	std::cout << "gen_vcxproj " << str_new_file_path << std::endl;

#if !ONLY_MAKE_RUN_TEST()
	write_vcxproj(ptree, str_new_file_path);
#endif // ONLY_MAKE_RUN_TEST()
}

void fix_vcxproj_list(const file_list_type& file_list, const ver_list_type& ver_list)
{
	for(file_list_type::const_iterator i = file_list.begin(), isize = file_list.end(); i != isize; ++i)
	{
		for(ver_list_type::const_iterator j = ver_list.begin(), jsize = ver_list.end(); j != jsize; ++j)
		{
			fix_vcxproj(*i, *j);
		}
	}
}

// filter

void fix_filters(const yggr::utf8_string& str_file_path, const yggr::utf8_string& str_ver)
{
	yggr::utf8_string str_new_file_path = make_new_file_name(str_file_path, ".vcxproj.filters", str_ver);

	std::cout << "gen_filters " << str_new_file_path << std::endl;

	assert(str_file_path != str_new_file_path);
	//if(str_file_path == str_new_file_path)
	//{
	//	return;
	//}

#if !ONLY_MAKE_RUN_TEST()

	local_fsys_operators_type::copy_file(
		local_fsys_operators_type::make_path_object(str_file_path),
		local_fsys_operators_type::make_path_object(str_new_file_path),
		local_fsys_operators_type::options_type::file_replace_if_exists_not_dir);

#endif // ONLY_MAKE_RUN_TEST
}

void fix_filters_list(const file_list_type& file_list, const ver_list_type& ver_list)
{
	for(file_list_type::const_iterator i = file_list.begin(), isize = file_list.end(); i != isize; ++i)
	{
		for(ver_list_type::const_iterator j = ver_list.begin(), jsize = ver_list.end(); j != jsize; ++j)
		{
			fix_filters(*i, *j);
		}
	}
}

// sln

yggr::string& load_sln(yggr::string& text, const yggr::utf8_string& str_file_path)
{
	try
	{
		local_fsys_operators_type::read_file_of_binary(local_fsys_operators_type::make_path_object(str_file_path), text);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}

	return text;
}

bool write_sln(const yggr::string& str_file_path, const yggr::string& text)
{
	try
	{
		local_fsys_operators_type::write_file_of_binary(
			local_fsys_operators_type::make_path_object(str_file_path), text,
			local_fsys_operators_type::options_type::dir_create_if_not_exists);
	}
	catch(const local_fsys_operators_type::exception_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
		return false;
	}

	return true;
}

yggr::string replace_vcxproj_path(const yggr::string& sln_text, const yggr::utf8_string& str_version)
{
	static const yggr::string str_re = "(\\.vcxproj)";
	yggr::string str_fmt = "(?1" + str_version.org_str() + "\\.vcxproj)";

	try
	{
		return
			yggr::regular_parse::regex_parse::replace(
				sln_text, yggr::regular_parse::regex_parse::make_regex(str_re), str_fmt);
	}
	catch(const yggr::regular_parse::regex_parse::error_type& e)
	{
		std::cerr << e.what() << std::endl;
		assert(false);
	}
	catch(...)
	{
		assert(false);
	}

	return yggr::string("");
}

void fix_sln(const yggr::utf8_string& str_file_path, const yggr::utf8_string& str_ver)
{
	yggr::utf8_string str_new_file_path = make_new_file_name(str_file_path, ".sln", str_ver);
	yggr::string sln_txt;

	load_sln(sln_txt, str_file_path);

	sln_txt = replace_vcxproj_path(sln_txt, str_ver);

	std::cout << "gen_sln " << str_new_file_path << std::endl;

#if !ONLY_MAKE_RUN_TEST()
	write_sln(str_new_file_path, sln_txt);
#endif // ONLY_MAKE_RUN_TEST

}

void fix_sln_list(const file_list_type& file_list, const ver_list_type& ver_list)
{
	for(file_list_type::const_iterator i = file_list.begin(), isize = file_list.end(); i != isize; ++i)
	{
		for(ver_list_type::const_iterator j = ver_list.begin(), jsize = ver_list.end(); j != jsize; ++j)
		{
			fix_sln(*i, *j);
		}
	}
}

// search_files

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

bool is_org_file(const path_type& fpath)
{
	yggr::utf8_string ver_cpp03_dot = ver_cpp03 + ".";
	yggr::utf8_string ver_cpp14_dot = ver_cpp14 + ".";
	yggr::utf8_string ver_cpp17_dot = ver_cpp17 + ".";
	yggr::utf8_string ver_cpp20_dot = ver_cpp20 + ".";
	//yggr::utf8_string ver_cpp23_dot = ver_cpp23 + ".";

	return
		(fpath.filename().native().find(ver_cpp03_dot.str<path_type::string_type>()) == path_type::string_type::npos)
		 && (fpath.filename().native().find(ver_cpp14_dot.str<path_type::string_type>()) == path_type::string_type::npos)
		 && (fpath.filename().native().find(ver_cpp17_dot.str<path_type::string_type>()) == path_type::string_type::npos)
		 && (fpath.filename().native().find(ver_cpp20_dot.str<path_type::string_type>()) == path_type::string_type::npos)
		 //&& (fpath.filename().native().find(ver_cpp23_dot.str<path_type::string_type>()) == path_type::string_type::npos)
		 ;
}

void handler_for_each_search_files(const local_fsys_operators_type::directory_entry_type& entry,
										boost::uintmax_t current_count,
										local_fsys_operators_type::error_code_type* current_ec,
										const filter_list_type& file_filter,
										file_list_type& vcxproj_files,
										file_list_type& filters_files,
										file_list_type& sln_files)
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
		if(is_org_file(fpath))
		{
			std::cout << fpath << std::endl;
			vcxproj_files.insert(yggr::utf8_string(fpath.native()));
		}
	}
	else if(fpath.extension() == ".filters")
	{
		if(is_org_file(fpath))
		{
			std::cout << fpath << std::endl;
			filters_files.insert(yggr::utf8_string(fpath.native()));
		}
	}
	else if(fpath.extension() == ".sln")
	{
		if(is_org_file(fpath))
		{
			std::cout << fpath << std::endl;
			sln_files.insert(yggr::utf8_string(fpath.native()));
		}
	}

	return;
}

void get_files(const yggr::utf8_string& str_root_path,
				const filter_list_type& dir_filter,
				const filter_list_type& file_filter,
				file_list_type& vcxproj_files,
				file_list_type& filters_files,
				file_list_type& sln_files)
{
	path_type root_dir_path = local_fsys_operators_type::make_path_object(str_root_path);

	try
	{
		local_fsys_operators_type::recursive_for_each(
			root_dir_path,
			boost::bind(
				&handler_for_each_search_files, _1, _2, _3,
				boost::cref(file_filter),
				boost::ref(vcxproj_files), boost::ref(filters_files), boost::ref(sln_files)),
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
	try
	{
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

	return true;
}

bool write_cfg_file(const yggr::utf8_string& fname, const tool_upgrade_vcxproj_cfg_type& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP(tool_upgrade_vcxproj_cfg_mark, cfg));

	return write_to_file(fname, opak.org_buf());
}

bool read_cfg_file(const yggr::utf8_string& fname, tool_upgrade_vcxproj_cfg_type& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

	ipak_type ipak;
	return
		read_from_file(ipak.org_buf(), fname).size()
		&& ipak.load(YGGR_SERIALIZATION_NAME_NVP(tool_upgrade_vcxproj_cfg_mark, cfg));
}

static yggr::utf8_string cfg_fname("tool_upgrade_vcxproj_cfg.xml");

int main_detail(int argc, char* argv[])
{
	if(argc > 1)
	{
		cfg_fname = yggr::utf8_string(argv[1], YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	if(!local_fsys_operators_type::is_exists(local_fsys_operators_type::make_path_object(cfg_fname)))
	{
		tool_upgrade_vcxproj_cfg_type cfg_sample = tool_upgrade_vcxproj_cfg_sample();
		write_cfg_file(cfg_fname, cfg_sample);
		std::cout << "gen config file succeed, please modify it" << std::endl;
		return -1;
	}

	tool_upgrade_vcxproj_cfg_type cfg;

	if(!read_cfg_file(cfg_fname, cfg))
	{
		std::cout << "read config file failed" << std::endl;
		return -1;
	}

	ver_list_type cpp_ver_list;
	cpp_ver_list.insert(ver_cpp03);
	//cpp_ver_list.insert(ver_cpp11); // vs2022 not support
	cpp_ver_list.insert(ver_cpp14);
	cpp_ver_list.insert(ver_cpp17);
	cpp_ver_list.insert(ver_cpp20);
	//cpp_ver_list.insert(ver_cpp23);

	file_list_type vcxproj_files, filters_files, sln_files;

	get_files(
		cfg.root_dir_, cfg.dir_filter_, cfg.file_filter_,
		vcxproj_files, filters_files, sln_files);

	//get_files(
	//	"./test_data", cfg.dir_filter_, cfg.file_filter_,
	//	vcxproj_files, filters_files, sln_files);

	fix_vcxproj_list(vcxproj_files, cpp_ver_list);

	fix_filters_list(filters_files, cpp_ver_list);

	fix_sln_list(sln_files, cpp_ver_list);

	return 0;
}

#endif //BOOST_VERSION < 105600

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

