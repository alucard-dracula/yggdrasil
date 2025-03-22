// tool_downgrade_vcxproj.cpp

#include <iostream>
#include <cassert>
#include <yggr/base/yggrdef.h>

#if BOOST_VERSION < 105600

#	error this tool not support boost 1.55 and older

#	else

#include <yggr/charset/utf8_string.hpp>
#include <yggr/container/set.hpp>
#include <yggr/container/unordered_set.hpp>
#include <yggr/regular_parse/regex_parse.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>
#include "vcxproj_parser_patch.hpp"
#include "tool_downgrade_vcxproj_cfg.hpp"

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

typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;
typedef local_fsys_operators_type::directory_entry_type directory_entry_type;
typedef local_fsys_operators_type::path_type path_type;

typedef tool_downgrade_vcxproj_cfg tool_downgrade_vcxproj_cfg_type;

static const char* tool_downgrade_vcxproj_cfg_mark = "tool_downgrade_vcxproj_cfg";

// vcxproj
yggr::utf8_string get_project_name(const yggr::utf8_string& str_file_path, const yggr::utf8_string& str_file_lang_tag)
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

	if(str_file_lang_tag.empty())
	{
		return yggr::utf8_string(str_file_path.begin() + first_pos + 1, str_file_path.begin() + last_pos);
	}
	else
	{
		std::size_t tag_pos = str_file_path.find(str_file_lang_tag + ".vcxproj");
		if(tag_pos == 0 || tag_pos == yggr::utf8_string::npos)
		{
			return yggr::utf8_string(str_file_path.begin() + first_pos + 1, str_file_path.begin() + last_pos);
		}
		else
		{
			return yggr::utf8_string(str_file_path.begin() + first_pos + 1, str_file_path.begin() + tag_pos);
		}
	}
}

yggr::utf8_string make_new_file_name(const yggr::utf8_string& org_str_file_path,
										const yggr::utf8_string& ext,
										const yggr::utf8_string& str_file_lang_tag = "")
{
	if(str_file_lang_tag.empty())
	{
		return org_str_file_path;
	}

	std::size_t last_pos = org_str_file_path.find(ext);
	if(last_pos == 0 || last_pos == yggr::utf8_string::npos)
	{
		return yggr::utf8_string("");
	}

	std::size_t file_lang_tag_pos = org_str_file_path.find(str_file_lang_tag);
//	std::size_t file_lang_tag_size =
//		(file_lang_tag_pos == 0 || file_lang_tag_pos == yggr::utf8_string::npos)?
//			0  : str_file_lang_tag.size();

	yggr::utf8_string ret;
	ret.reserve(org_str_file_path.size() - str_file_lang_tag.size());
	ret.append(org_str_file_path.begin(), org_str_file_path.begin() + file_lang_tag_pos);
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

ptree_type& modify_property_items(ptree_type& ptree,
									const yggr::utf8_string& project_name,
									const yggr::utf8_string& str_file_lang_tag)
{
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

	yggr::string language_standard_val;

	assoc_iter_type iter_chk;

	for(iterator i = assoc_iter->second.begin(), isize = assoc_iter->second.end();
			i != isize ; ++i)
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

			if(check_iter->second.get_value<ptree_type::key_type>() == "Globals")
			{
				// <ProjectName>md5</ProjectName>
				if(str_file_lang_tag.size())
				{
					if(i->second.find("ProjectName") != i->second.not_found())
					{
						i->second.erase("ProjectName");
					}
				}

				// <RootNamespace>base64</RootNamespace>

				if(str_file_lang_tag.size())
				{
					iter_chk = i->second.find("RootNamespace");
					if(iter_chk == i->second.not_found())
					{
						i->second.add_child("RootNamespace", ptree_type(project_name.org_str()));
					}
					else
					{
						iter_chk->second.put_value(project_name.org_str());
					}
				}

				// <WindowsTargetPlatformVersion>10.0</WindowsTargetPlatformVersion>
				if(i->second.find("WindowsTargetPlatformVersion") != i->second.not_found())
				{
					i->second.erase("WindowsTargetPlatformVersion");
				}

			}
			else if(check_iter->second.get_value<ptree_type::key_type>() == "Configuration")
			{
				// <PlatformToolset>v143</PlatformToolset>
				if(i->second.find("PlatformToolset") != i->second.not_found())
				{
					i->second.erase("PlatformToolset");
				}
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
				if(assoc_iter_j->second.find("LanguageStandard") != assoc_iter_j->second.not_found())
				{
					 assoc_iter_j->second.erase("LanguageStandard");
				}

				// <MinimalRebuild>false</MinimalRebuild>
				iter_chk = assoc_iter_j->second.find("MinimalRebuild");

				if(iter_chk != assoc_iter_j->second.not_found())
				{
					assoc_iter_j->second.erase("MinimalRebuild");
				}
			}
		}
	}

	return ptree;
}

void fix_vcxproj(const yggr::utf8_string& str_file_path, const yggr::utf8_string& str_file_lang_tag)
{
	yggr::utf8_string str_project_name = get_project_name(str_file_path, str_file_lang_tag);
	yggr::utf8_string str_new_file_path = make_new_file_name(str_file_path, ".vcxproj", str_file_lang_tag);

	ptree_type ptree;
	load_vcxproj(ptree, str_file_path);

	modify_property_items(ptree, str_project_name, str_file_lang_tag);

	std::cout << "gen_vcxproj " << str_new_file_path << std::endl;

#if !ONLY_MAKE_RUN_TEST()
	write_vcxproj(ptree, str_new_file_path);
#endif // ONLY_MAKE_RUN_TEST()
}

void fix_vcxproj_list(const file_list_type& file_list, const yggr::utf8_string& str_lang_tag)
{
	yggr::utf8_string str_file_lang_tag = str_lang_tag.size()? "_" + str_lang_tag : "";

	for(file_list_type::const_iterator i = file_list.begin(), isize = file_list.end(); i != isize; ++i)
	{
		fix_vcxproj(*i, str_file_lang_tag);
	}
}

// filter

void fix_filters(const yggr::utf8_string& str_file_path, const yggr::utf8_string& str_lang_tag)
{
	yggr::utf8_string str_file_lang_tag = str_lang_tag.size()? "_" + str_lang_tag : "";
	yggr::utf8_string str_new_file_path = make_new_file_name(str_file_path, ".vcxproj.filters", str_file_lang_tag);

	std::cout << "gen_filters " << str_new_file_path << std::endl;

	//assert(str_file_path != str_new_file_path);
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

void fix_filters_list(const file_list_type& file_list, const yggr::utf8_string& str_lang_tag)
{
	for(file_list_type::const_iterator i = file_list.begin(), isize = file_list.end(); i != isize; ++i)
	{
		fix_filters(*i, str_lang_tag);
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
	std::size_t write_byte_size = 0;
	try
	{
		write_byte_size =
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

	return !(write_byte_size == 0 || write_byte_size == local_fsys_operators_type::npos);
}

yggr::string replace_vcxproj_path(const yggr::string& sln_text, const yggr::utf8_string& str_file_lang_tag)
{
	yggr::string str_re = "(" + str_file_lang_tag.org_str() + "\\.vcxproj)";
	static const yggr::string str_fmt = "(?1\\.vcxproj)";

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

void fix_sln(const yggr::utf8_string& str_file_path, const yggr::utf8_string& str_lang_tag)
{
	yggr::utf8_string str_file_lang_tag = str_lang_tag.size()? "_" + str_lang_tag : "";
	yggr::utf8_string str_new_file_path = make_new_file_name(str_file_path, ".sln", str_file_lang_tag);
	yggr::string sln_txt;

	load_sln(sln_txt, str_file_path);

	sln_txt = replace_vcxproj_path(sln_txt, str_file_lang_tag);

	std::cout << "gen_sln " << str_new_file_path << std::endl;

#if !ONLY_MAKE_RUN_TEST()
	write_sln(str_new_file_path, sln_txt);
#endif // ONLY_MAKE_RUN_TEST

}

void fix_sln_list(const file_list_type& file_list, const yggr::utf8_string& str_lang_tag)
{
	for(file_list_type::const_iterator i = file_list.begin(), isize = file_list.end(); i != isize; ++i)
	{
		fix_sln(*i, str_lang_tag);
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
		&& (
#if (BOOST_VERSION < 107200)
		dir_iter.level() > 1
#else
		dir_iter.depth() > 1
#endif //
			|| dir_filter.find(yggr::utf8_string(entry.path().filename().native())) != dir_filter.end());
}

inline bool is_src_file(const path_type& fpath, const yggr::utf8_string& str_file_tag)
{
	return
		str_file_tag.empty()
		|| fpath.filename().native().find(
			str_file_tag.str<path_type::string_type>()) != path_type::string_type::npos;
}

void handler_for_each_search_files(const local_fsys_operators_type::directory_entry_type& entry,
										boost::uintmax_t current_count,
										local_fsys_operators_type::error_code_type* current_ec,
										const yggr::utf8_string& str_file_lang_tag,
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
		if(is_src_file(fpath, str_file_lang_tag))
		{
			std::cout << fpath << std::endl;
			vcxproj_files.insert(yggr::utf8_string(fpath.native()));
		}
	}
	else if(fpath.extension() == ".filters")
	{
		if(is_src_file(fpath, str_file_lang_tag))
		{
			std::cout << fpath << std::endl;
			filters_files.insert(yggr::utf8_string(fpath.native()));
		}
	}
	else if(fpath.extension() == ".sln")
	{
		if(is_src_file(fpath, str_file_lang_tag))
		{
			std::cout << fpath << std::endl;
			sln_files.insert(yggr::utf8_string(fpath.native()));
		}
	}

	return;
}

void get_files(const yggr::utf8_string& str_root_path,
				const yggr::utf8_string& str_lang_tag,
				const filter_list_type& dir_filter,
				const filter_list_type& file_filter,
				file_list_type& vcxproj_files,
				file_list_type& filters_files,
				file_list_type& sln_files)
{
	yggr::utf8_string str_file_lang_tag = str_lang_tag.size()? "_" + str_lang_tag + "." : "";

	path_type root_dir_path = local_fsys_operators_type::make_path_object(str_root_path);
	try
	{
		local_fsys_operators_type::recursive_for_each(
			root_dir_path,
			boost::bind(
				&handler_for_each_search_files, _1, _2, _3,
				boost::cref(str_file_lang_tag),
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

bool write_cfg_file(const yggr::utf8_string& fname, const tool_downgrade_vcxproj_cfg_type& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_oarchive_partner> opak_type;

	opak_type opak;
	opak.save(YGGR_SERIALIZATION_NAME_NVP(tool_downgrade_vcxproj_cfg_mark, cfg));

	return write_to_file(fname, opak.org_buf());
}

bool read_cfg_file(const yggr::utf8_string& fname, tool_downgrade_vcxproj_cfg_type& cfg)
{
	typedef yggr::packet::packet<yggr::archive::archive_partner::xml_iarchive_partner> ipak_type;

	ipak_type ipak;
	return
		read_from_file(ipak.org_buf(), fname).size()
		&& ipak.load(YGGR_SERIALIZATION_NAME_NVP(tool_downgrade_vcxproj_cfg_mark, cfg));
}

static yggr::utf8_string cfg_fname("tool_downgrade_vcxproj_cfg.xml");

int main_detail(int argc, char* argv[])
{
	if(argc > 1)
	{
		cfg_fname = yggr::utf8_string(argv[1], YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}

	if(!local_fsys_operators_type::is_exists(local_fsys_operators_type::make_path_object(cfg_fname)))
	{
		tool_downgrade_vcxproj_cfg_type cfg_sample = tool_downgrade_vcxproj_cfg_sample();
		write_cfg_file(cfg_fname, cfg_sample);
		std::cout << "gen config file succeed, please modify it" << std::endl;
		return -1;
	}

	tool_downgrade_vcxproj_cfg_type cfg;

	if(!read_cfg_file(cfg_fname, cfg))
	{
		std::cout << "read config file failed" << std::endl;
		return -1;
	}

	file_list_type vcxproj_files, filters_files, sln_files;

	get_files(
		cfg.root_dir_, cfg.lang_tag_, cfg.dir_filter_, cfg.file_filter_,
		vcxproj_files, filters_files, sln_files);

	fix_vcxproj_list(vcxproj_files, cfg.lang_tag_);

	fix_filters_list(filters_files, cfg.lang_tag_);

	fix_sln_list(sln_files, cfg.lang_tag_);

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

	char cc = 0;
	std::cin >> cc;

	return ret;
}


#endif // #if BOOST_VERSION < 105600

