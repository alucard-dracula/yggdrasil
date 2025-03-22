// tool_conv_to_cb_solution.cpp

#include "tool_conv_to_cb_solution_config.hpp"

#if !(BOOST_VERSION < 105600)

#include "tool_conv_to_cbsln_cfg.hpp"

#include "lib_depend_info.hpp"
#include "cbp_sample_cfg.hpp"

#include "file_operator.hpp"
#include "xml_operator.hpp"

#include "vcproj_tags_def.hpp"
#include "vcproj_parser.hpp"
#include "vcsln_parser.hpp"

#include "cb_tags_def.hpp"
#include "cb_parser.hpp"
#include "cbws_parser.hpp"

#include <yggr/container/unordered_set.hpp>
#include <yggr/container/unordered_map.hpp>
#include <yggr/algorithm/knuth_morris_pratt.hpp>

#include <iostream>
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

static const char* tool_conv_to_cb_solution_cfg_mark = "tool_conv_to_cb_solution_cfg";
static const char* cbp_sample_cfg_mark = "cbp_sample_cfg";
static const char* lib_depend_info_cfg_mark = "lib_depend_info_cfg";

typedef yggr::set<yggr::utf8_string> file_list_type;
typedef yggr::unordered_set<yggr::utf8_string> filter_list_type;
typedef local_fsys_operators_type::path_type path_type;

typedef yggr::unordered_map<yggr::utf8_string, ptree_type> ptree_umap_type;
typedef yggr::map<yggr::utf8_string, ptree_string_type> vcproj_file_app_tag_map_type;

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

static yggr::utf8_string cfg_fname("tool_conv_to_cb_solution_cfg.xml");

int format_config_file(const yggr::utf8_string& root_cfg_fname,
						tool_conv_to_cb_solution_cfg& cfg_root,
						cbp_sample_cfg& cfg_sample,
						lib::lib_depend_info& cfg_lib_dep_info)
{
	int ret = 0;
	if(!read_cfg_file(root_cfg_fname, tool_conv_to_cb_solution_cfg_mark, cfg_root))
	{
		tool_conv_to_cb_solution_cfg cfg = tool_conv_to_cbsln_cfg_sample();
		write_cfg_file(root_cfg_fname, tool_conv_to_cb_solution_cfg_mark, cfg);

		cfg_root = boost::move(cfg);
		ret = -1;
	}

	if(!read_cfg_file(cfg_root.cbp_sample_cfg_, cbp_sample_cfg_mark, cfg_sample))
	{
		cbp_sample_cfg cfg = cbp_sample_cfg_sample();
		write_cfg_file(cfg_root.cbp_sample_cfg_, cbp_sample_cfg_mark, cfg);

		cfg_sample = boost::move(cfg);
		ret = -1;
	}

	if(!read_cfg_file(cfg_root.lib_depend_info_cfg_, lib_depend_info_cfg_mark, cfg_lib_dep_info))
	{
		lib::lib_depend_info cfg = lib::lib_depend_info_cfg_sample();
		write_cfg_file(cfg_root.lib_depend_info_cfg_, lib_depend_info_cfg_mark, cfg);

		cfg_lib_dep_info = boost::move(cfg);
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
										const filter_list_type& file_filter,
										file_list_type& vcxproj_files,
										file_list_type& sln_files)
{
	typedef local_fsys_operators_type::path_type path_type;

	const path_type& fpath = entry.path();

	std::cout << "check_path: " << fpath << std::endl;

	if(!fpath.has_extension())
	{
		return;
	}

	if(file_filter.find(yggr::utf8_string(fpath.filename().native())) != file_filter.end())
	{
		return;
	}

	if(fpath.extension() == ".vcxproj")
	{
		std::cout << "find vcxproj:\t" << fpath << std::endl;
		vcxproj_files.insert(file_op::make_string_file_path(yggr::utf8_string(fpath.native())));
	}

	if(fpath.extension() == ".sln")
	{
		std::cout << "find sln:\t" << fpath << std::endl;
		sln_files.insert(file_op::make_string_file_path(yggr::utf8_string(fpath.native())));
	}

	return;
}

void get_files(const yggr::utf8_string& str_root_path,
				const filter_list_type& dir_filter,
				const filter_list_type& file_filter,
				file_list_type& vcxproj_files,
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
				boost::ref(vcxproj_files),
				boost::ref(sln_files)),
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

bool check_file_ext(const yggr::string& str_fpath, const yggr::string& str_ext)
{
	return
		str_ext.size() < str_fpath.size()
		&& (0 == str_fpath.compare(str_fpath.size() - str_ext.size(), str_ext.size(), str_ext));
}

ptree_umap_type& make_cb_sample_ptree_map(ptree_umap_type& ptree_map, const cbp_sample_cfg::sample_file_map_type& cfg_map)
{
	typedef cbp_sample_cfg::sample_file_map_type sample_file_map_type;
	typedef sample_file_map_type::const_iterator sample_file_map_citer_type;

	static const yggr::string ext_cbp = ".cbp";
	static const yggr::string ext_workspace = ".workspace";

	for(sample_file_map_citer_type i = cfg_map.begin(), isize = cfg_map.end(); i != isize; ++i)
	{
		ptree_type cb_ptree;

		if(check_file_ext(i->second.org_str(), ext_cbp))
		{
			if(!cb::get_cb_ptree_from_file(cb_ptree, i->second).empty())
			{
				ptree_map[i->first] = boost::move(cb_ptree);
			}
			continue;
		}

		if(check_file_ext(i->second.org_str(), ext_workspace))
		{
			if(!cbws::get_cb_workspace_ptree_from_file(cb_ptree, i->second).empty())
			{
				ptree_map[i->first] = boost::move(cb_ptree);
			}
			continue;
		}
	}

	return ptree_map;
}

yggr::utf8_string get_file_dir(const yggr::utf8_string& file_path)
{
	std::size_t pos1 = file_path.org_str().rfind('/');
	std::size_t pos2 = file_path.org_str().rfind('\\');

	std::size_t pos =
		(std::max)(
			(pos1 == yggr::string::npos? 0 : pos1),
			(pos2 == yggr::string::npos? 0 : pos2));

	yggr::utf8_string str(file_path.org_str().substr(0, pos), YGGR_STR_UTF8_STRING_CHARSET_NAME());
	file_op::format_string_file_path(str);
	return str;
}

yggr::string get_file_name(const yggr::utf8_string& file_path)
{
	return file_op::get_file_name(file_path.org_str());
}

yggr::string get_file_name_no_ext(const yggr::utf8_string& file_path)
{
	return file_op::get_file_name_no_ext(file_path.org_str());
}

const yggr::utf8_string& get_file_name_lang_std(const yggr::utf8_string& file_path)
{
	static const yggr::utf8_string str_cpp03_vc = "_cpp03";
	static const yggr::utf8_string str_cpp14_vc = "_cpp14";
	static const yggr::utf8_string str_cpp17_vc = "_cpp17";
	static const yggr::utf8_string str_cpp20_vc = "_cpp20";

	static const yggr::utf8_string str_cpp03_vc_s = "_cpp03.";
	static const yggr::utf8_string str_cpp14_vc_s = "_cpp14.";
	static const yggr::utf8_string str_cpp17_vc_s = "_cpp17.";
	static const yggr::utf8_string str_cpp20_vc_s = "_cpp20.";

	//std::cout << "check cpp14: " << file_path.org_str() << std::endl;

	if(file_path.org_str().rfind(str_cpp20_vc_s.org_str()) != yggr::string::npos)
	{
		return str_cpp20_vc;
	}
	else if(file_path.org_str().rfind(str_cpp17_vc_s.org_str()) != yggr::string::npos)
	{
		return str_cpp17_vc;
	}
	else if(file_path.org_str().rfind(str_cpp14_vc_s.org_str()) != yggr::string::npos)
	{
		//std::cout << "is cpp 14: " << file_path.org_str() << std::endl;
		return str_cpp14_vc;
	}
	else
	{
		return str_cpp03_vc;
	}
}

yggr::utf8_string make_cbp_file_path(const yggr::utf8_string& file_path, const yggr::utf8_string& cb_file_tag)
{
	static const yggr::string str_cpp03_vc = "_cpp03.vcxproj";
	static const yggr::string str_cpp14_vc = "_cpp14.vcxproj";
	static const yggr::string str_cpp17_vc = "_cpp17.vcxproj";
	static const yggr::string str_cpp20_vc = "_cpp20.vcxproj";
	static const yggr::string str_vc = ".vcxproj";

	if((str_cpp03_vc.size() < file_path.size())
		&& (file_path.org_str().substr(file_path.size() - str_cpp03_vc.size()) == str_cpp03_vc))
	{
		return
			yggr::utf8_string(
				file_path.org_str().substr(0, file_path.size() - str_cpp03_vc.size())
					+ cb_file_tag.org_str() + "_cpp03.cbp",
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}
	else if((str_cpp14_vc.size() < file_path.size())
			&& (file_path.org_str().substr(file_path.size() - str_cpp14_vc.size()) == str_cpp14_vc))
	{
		return
			yggr::utf8_string(
				file_path.org_str().substr(0, file_path.size() - str_cpp14_vc.size())
					+ cb_file_tag.org_str() + "_cpp14.cbp",
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}
	else if((str_cpp17_vc.size() < file_path.size())
			&& (file_path.org_str().substr(file_path.size() - str_cpp17_vc.size()) == str_cpp17_vc))
	{
		return
			yggr::utf8_string(
				file_path.org_str().substr(0, file_path.size() - str_cpp17_vc.size())
					+ cb_file_tag.org_str() + "_cpp17.cbp",
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}
	else if((str_cpp20_vc.size() < file_path.size())
			&& (file_path.org_str().substr(file_path.size() - str_cpp20_vc.size()) == str_cpp20_vc))
	{
		return
			yggr::utf8_string(
				file_path.org_str().substr(0, file_path.size() - str_cpp20_vc.size())
					+ cb_file_tag.org_str() + "_cpp20.cbp",
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}
	else if((str_vc.size() < file_path.size())
			&& (file_path.org_str().substr(file_path.size() - str_vc.size()) == str_vc))
	{
		return
			yggr::utf8_string(
				file_path.org_str().substr(0, file_path.size() - str_vc.size())
					+ cb_file_tag.org_str() + ".cbp",
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}
	else
	{
		return yggr::utf8_string();
	}
}

yggr::u32 vcxproj_to_cbp(const yggr::utf8_string& vcproj_file,
							ptree_string_type& out_app_tag,
							const yggr::utf8_string& cb_file_tag,
							const ptree_umap_type& cb_sample_map,
							const lib::lib_depend_info& dep_info_cfg)
{
	typedef ptree_umap_type::const_iterator ptree_umap_citer_type;

	std::cout << "fix vcxproj:\t" << vcproj_file << std::endl;

	ptree_type ptree_vc;
	ptree_type ptree_cb;

	yggr::utf8_string cbp_file = make_cbp_file_path(vcproj_file, cb_file_tag);

	if(cbp_file.empty())
	{
		return xml_op::fix_result::E_FAILED;
	}

	yggr::utf8_string proj_dir = get_file_dir(vcproj_file);

	if(vcproj::get_vcxproj_ptree_from_file(ptree_vc, vcproj_file).empty())
	{
		return xml_op::fix_result::E_FAILED;
	}

	yggr::string app_tag = vcproj::get_app_tag(ptree_vc);
	out_app_tag = app_tag;

	ptree_umap_citer_type sample_iter = cb_sample_map.find(app_tag);

	if(sample_iter == cb_sample_map.end())
	{
		return xml_op::fix_result::E_IGNORE;
	}

	ptree_cb = sample_iter->second;

	yggr::u32 sat = cb::make_cb_ptree_from_vc_tree(ptree_cb, ptree_vc, proj_dir, dep_info_cfg);

	if(sat == xml_op::fix_result::E_SUCCEED)
	{
		return
			cb::write_cbp(ptree_cb, cbp_file)?
				xml_op::fix_result::E_SUCCEED
				: xml_op::fix_result::E_FAILED;
	}

	return sat;
}

std::size_t vcxproj_list_to_cbp(vcproj_file_app_tag_map_type& file_app_tag_map,
									const yggr::utf8_string& cb_file_tag,
									const ptree_umap_type& cb_sample_map,
									const lib::lib_depend_info& dep_info_cfg,
									file_list_type& failed_file_list,
									file_list_type& ignore_file_list)
{
	std::size_t count = 0;
	for(vcproj_file_app_tag_map_type::iterator i = file_app_tag_map.begin(), isize = file_app_tag_map.end();
			i != isize; ++i)
	{
		switch(vcxproj_to_cbp(i->first, i->second, cb_file_tag, cb_sample_map, dep_info_cfg))
		{
		case xml_op::fix_result::E_SUCCEED:
			++count;
			break;
		case xml_op::fix_result::E_FAILED:
			failed_file_list.insert(i->first);
			break;
		case xml_op::fix_result::E_IGNORE:
			ignore_file_list.insert(i->first);
			break;
		default:
			assert(false);
			failed_file_list.insert(i->first);
		}
	}

	return count;
}

yggr::string make_workspace_file_title(const yggr::string& file_title, const yggr::utf8_string& cb_file_tag)
{
	static const yggr::string str_cpp03_vc = "_cpp03";
	static const yggr::string str_cpp14_vc = "_cpp14";
	static const yggr::string str_cpp17_vc = "_cpp17";
	static const yggr::string str_cpp20_vc = "_cpp20";

	if((str_cpp03_vc.size() < file_title.size())
		&& (file_title.substr(file_title.size() - str_cpp03_vc.size()) == str_cpp03_vc))
	{
		return file_title.substr(0, file_title.size() - str_cpp03_vc.size()) + cb_file_tag.org_str() + "_cpp03";
	}
	else if((str_cpp14_vc.size() < file_title.size())
			&& (file_title.substr(file_title.size() - str_cpp14_vc.size()) == str_cpp14_vc))
	{
		return file_title.substr(0, file_title.size() - str_cpp14_vc.size()) + cb_file_tag.org_str() + "_cpp14";
	}
	else if((str_cpp17_vc.size() < file_title.size())
			&& (file_title.substr(file_title.size() - str_cpp17_vc.size()) == str_cpp17_vc))
	{
		return file_title.substr(0, file_title.size() - str_cpp17_vc.size()) + cb_file_tag.org_str() + "_cpp17";
	}
	else if((str_cpp20_vc.size() < file_title.size())
			&& (file_title.substr(file_title.size() - str_cpp20_vc.size()) == str_cpp20_vc))
	{
		return file_title.substr(0, file_title.size() - str_cpp20_vc.size()) + cb_file_tag.org_str() + "_cpp20";
	}
	else if(file_title.size())
	{
		return file_title + cb_file_tag.org_str();
	}
	else
	{
		return yggr::utf8_string();
	}
}

yggr::utf8_string make_workspace_file_path(const yggr::utf8_string& file_path, const yggr::utf8_string& cb_file_tag)
{
	static const yggr::string str_cpp03_vc = "_cpp03.sln";
	static const yggr::string str_cpp14_vc = "_cpp14.sln";
	static const yggr::string str_cpp17_vc = "_cpp17.sln";
	static const yggr::string str_cpp20_vc = "_cpp20.sln";
	static const yggr::string str_vc = ".sln";

	if((str_cpp03_vc.size() < file_path.size())
		&& (file_path.org_str().substr(file_path.size() - str_cpp03_vc.size()) == str_cpp03_vc))
	{
		return
			yggr::utf8_string(
				file_path.org_str().substr(0, file_path.size() - str_cpp03_vc.size())
					+ cb_file_tag.org_str() + "_cpp03.workspace",
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}
	else if((str_cpp14_vc.size() < file_path.size())
			&& (file_path.org_str().substr(file_path.size() - str_cpp14_vc.size()) == str_cpp14_vc))
	{
		return
			yggr::utf8_string(
				file_path.org_str().substr(0, file_path.size() - str_cpp14_vc.size())
					+ cb_file_tag.org_str() + "_cpp14.workspace",
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}
	else if((str_cpp17_vc.size() < file_path.size())
			&& (file_path.org_str().substr(file_path.size() - str_cpp17_vc.size()) == str_cpp17_vc))
	{
		return
			yggr::utf8_string(
				file_path.org_str().substr(0, file_path.size() - str_cpp17_vc.size())
					+ cb_file_tag.org_str() + "_cpp17.workspace",
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}
	else if((str_cpp20_vc.size() < file_path.size())
			&& (file_path.org_str().substr(file_path.size() - str_cpp20_vc.size()) == str_cpp20_vc))
	{
		return
			yggr::utf8_string(
				file_path.org_str().substr(0, file_path.size() - str_cpp20_vc.size())
					+ cb_file_tag.org_str() + "_cpp20.workspace",
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}
	else if((str_vc.size() < file_path.size())
			&& (file_path.org_str().substr(file_path.size() - str_vc.size()) == str_vc))
	{
		return
			yggr::utf8_string(
				file_path.org_str().substr(0, file_path.size() - str_vc.size())
					+ cb_file_tag.org_str() + ".workspace",
				YGGR_STR_UTF8_STRING_CHARSET_NAME());
	}
	else
	{
		return yggr::utf8_string();
	}
}


// sln
yggr::u32 vcxproj_map_to_workspace_ptree(ptree_type& ptree_workspace,
											const vcsln::vcxproj_map_type& vcxproj_map,
											const vcsln::vcxproj_map_type& lib_map,
											const vcsln::vcxproj_map_type& dll_map,
											const vcsln::vcxproj_map_type& exe_map,
											const yggr::string& org_ws_title,
											const yggr::utf8_string& cb_file_tag,
											const ptree_umap_type& cb_sample_map)
{
	typedef vcsln::vcxproj_map_type vcxproj_map_type;

	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;

	string_type ws_title = make_workspace_file_title(org_ws_title, cb_file_tag);

	if(ws_title.empty())
	{
		return xml_op::fix_result::E_FAILED;
	}

	{
		ptree_umap_type::const_iterator iter = cb_sample_map.find("workspace");
		if(iter == cb_sample_map.end())
		{
			return xml_op::fix_result::E_FAILED;
		}

		ptree_workspace = iter->second;
	}

	assoc_iter_type iter_ws_0 = ptree_workspace.find("CodeBlocks_workspace_file");
	if(iter_ws_0 == ptree_workspace.not_found())
	{
		return xml_op::fix_result::E_FAILED;
	}


	assoc_iter_type iter_ws = iter_ws_0->second.find("Workspace");
	if(iter_ws == iter_ws_0->second.not_found())
	{
		return xml_op::fix_result::E_FAILED;
	}

	ptree_type& ptree_ws = iter_ws->second;
	ptree_ws.clear();
	ptree_ws.add_child("<xmlattr>.title", ptree_type(ws_title));

	string_type str_cbp_path;
	yggr::u32 dep_t = 0; // 0 - nodep, 1 - deplib, 2 - depdll
	for(vcxproj_map_type::const_iterator i = vcxproj_map.begin(), isize = vcxproj_map.end(); i != isize; ++i)
	{
		str_cbp_path = static_cast<string_type>(make_cbp_file_path(i->second, cb_file_tag));
		if(str_cbp_path.empty())
		{
			return xml_op::fix_result::E_FAILED;
		}

		if(i == vcxproj_map.begin())
		{
			ptree_type& tmp_ptree_proj = ptree_ws.add_child("Project", ptree_type());
			tmp_ptree_proj.add_child("<xmlattr>.filename", ptree_type(str_cbp_path));
			tmp_ptree_proj.add_child("<xmlattr>.active", ptree_type("1"));
		}
		else
		{
			ptree_ws.add_child("Project", ptree_type()).add_child("<xmlattr>.filename", ptree_type(str_cbp_path));
		}

		// 0 - nodep, 1 - deplib, 2 - depdll
		if(exe_map.find(i->first) != exe_map.end())
		{
			if(dll_map.empty())
			{
				dep_t = 1;
			}
			else
			{
				dep_t = 2;
			}
		}
		else if(dll_map.find(i->first) != dll_map.end())
		{
			dep_t = 1;
		}
		else
		{
			dep_t = 0;
		}

		switch(dep_t)
		{
		case 2:
			{
				for(vcxproj_map_type::const_iterator j = dll_map.begin(), jsize = dll_map.end(); j != jsize; ++j)
				{
					ptree_ws.rbegin()->second.add_child("Depends", ptree_type()).add_child("<xmlattr>.filename", ptree_type(j->second.org_str()));
				}
			}
			break;
		case 1:
			{
				for(vcxproj_map_type::const_iterator j = lib_map.begin(), jsize = lib_map.end(); j != jsize; ++j)
				{
					ptree_ws.rbegin()->second.add_child("Depends", ptree_type()).add_child("<xmlattr>.filename", ptree_type(j->second.org_str()));
				}
			}
			break;
		case 0:
			break;
		default:
			assert(false);
			break;
		}
	}

	return xml_op::fix_result::E_SUCCEED;
}


yggr::string& replace_cpp14_to_cpp11(yggr::string& code_cpp14,
										const yggr::string& str_cpp14,
										const yggr::string& str_cpp11)
{
	typedef yggr::string string_type;
	typedef string_type::iterator string_iter_type;
	typedef string_type::const_iterator string_citer_type;
	typedef yggr::knuth_morris_pratt<string_citer_type> kmp_type;
	typedef std::pair<string_iter_type, string_iter_type> kmp_return_type;

	if(str_cpp14.empty())
	{
		return code_cpp14;
	}

	kmp_type kmp_cpp14(str_cpp14.begin(), str_cpp14.end());

	kmp_return_type ipair = kmp_cpp14(code_cpp14.begin(), code_cpp14.end());

	if(ipair.second == code_cpp14.end())
	{
		return code_cpp14;
	}

	string_iter_type isize = code_cpp14.end();
	std::size_t pos = 0;

	for(;ipair.first != ipair.second; ipair = kmp_cpp14(ipair.second, isize))
	{
		pos = std::distance(code_cpp14.begin(), ipair.first);
		code_cpp14.replace(ipair.first, ipair.second, str_cpp11.begin(), str_cpp11.end());
		ipair.second = code_cpp14.begin() + (pos + str_cpp11.size());
	}

	return code_cpp14;
}

inline yggr::utf8_string& replace_cpp14_to_cpp11(yggr::utf8_string& code_cpp14,
													const yggr::utf8_string& str_cpp14,
													const yggr::utf8_string& str_cpp11)
{
	replace_cpp14_to_cpp11(code_cpp14.org_str(), str_cpp14.org_str(), str_cpp11.org_str());
	code_cpp14.recount_length();
	return code_cpp14;
}

yggr::u32 copy_and_make_cpp11_cb_workspace_from_cpp14(const yggr::utf8_string& file_path_cpp14,
														const vcsln::vcxproj_map_type& cbp_map_cpp14)
{
	typedef vcsln::vcxproj_map_type vcxproj_map_type;

	static const yggr::utf8_string s_str_cpp14 = "_cpp14";
	static const yggr::utf8_string s_str_cpp11 = "_cpp11";

	if(!(file_path_cpp14.size() && cbp_map_cpp14.size()))
	{
		return xml_op::fix_result::E_SUCCEED;
	}

	yggr::utf8_string cbp_file_path_cpp11;
	yggr::string file_buffer;
	for(vcxproj_map_type::const_iterator i = cbp_map_cpp14.begin(), isize = cbp_map_cpp14.end();
			i != isize; ++i)
	{
		cbp_file_path_cpp11 = i->second;
		if(replace_cpp14_to_cpp11(cbp_file_path_cpp11, s_str_cpp14, s_str_cpp11).empty())
		{
			return xml_op::fix_result::E_FAILED;
		}

		assert(file_buffer.empty());

		if(file_op::read_from_file(file_buffer, i->second).empty()
			|| replace_cpp14_to_cpp11(file_buffer, cb::cb_cpp_lang_std::s_cpp14(), cb::cb_cpp_lang_std::s_cpp11()).empty()
			|| (!file_op::write_to_file(cbp_file_path_cpp11, file_buffer)))
		{
			return xml_op::fix_result::E_FAILED;
		}

		file_buffer.clear();
	}

	yggr::utf8_string ws_file_path_cpp11 = file_path_cpp14;
	replace_cpp14_to_cpp11(ws_file_path_cpp11, s_str_cpp14, s_str_cpp11);

	file_buffer.clear();

	if(file_op::read_from_file(file_buffer, file_path_cpp14).empty()
		|| replace_cpp14_to_cpp11(file_buffer, s_str_cpp14.org_str(), s_str_cpp11.org_str()).empty()
		|| (!file_op::write_to_file(ws_file_path_cpp11, file_buffer)) )
	{
		return xml_op::fix_result::E_FAILED;
	}

	return xml_op::fix_result::E_SUCCEED;
}

bool is_yggr_sln_file(const yggr::utf8_string& sln_dir)
{
	static const yggr::utf8_string s_str = "yggdrasil";

	return
		(sln_dir.org_str().back() == '/')?
			s_str.size() < sln_dir.size()
				&& sln_dir.org_str().substr(sln_dir.size() - (s_str.size() + 1), s_str.size()) == s_str.org_str()
			: s_str.size() <= sln_dir.size()
				&& sln_dir.org_str().substr(sln_dir.size() - s_str.size()) == s_str.org_str();
}

yggr::u32 sln_to_workspace(const yggr::utf8_string& sln_file,
							vcproj_file_app_tag_map_type& vcxproj_file_app_tag_map,
							const yggr::utf8_string& cb_file_tag,
							const ptree_umap_type& cb_sample_map,
							const lib::lib_depend_info& dep_info_cfg,
							file_list_type& failed_vcxproj_files,
							file_list_type& ignore_vcxproj_files)
{
	typedef vcsln::vcxproj_map_type vcxproj_map_type;

	std::cout << "fix sln:\t" << sln_file << std::endl;

	vcxproj_map_type vcxproj_map;
	vcsln::get_vcxproj_map_from_file(vcxproj_map, sln_file);

	yggr::utf8_string sln_dir = get_file_dir(sln_file);

	vcxproj_map_type cbp_map;
	const yggr::utf8_string& sln_lang_std_mark = get_file_name_lang_std(sln_file);
	//std::cout << "sln_lang_std_mark: " << sln_lang_std_mark << std::endl;
	bool bmake_cpp11_ws = (sln_lang_std_mark == "_cpp14");
	bool is_yggr_sln = is_yggr_sln_file(sln_dir);

	vcxproj_map_type lib_map, dll_map, exe_map;

	{
		yggr::utf8_string vcxproj_path;
		for(vcxproj_map_type::iterator i = vcxproj_map.begin(), isize = vcxproj_map.end(); i != isize; ++i)
		{
			if(i->second.empty())
			{
				return xml_op::fix_result::E_FAILED;
			}

			vcxproj_path = sln_dir + "/" + i->second;
			ptree_string_type out_app_tag;
			vcproj_file_app_tag_map_type::iterator iter_app_tag_chk;
			if((iter_app_tag_chk = vcxproj_file_app_tag_map.find(vcxproj_path)) == vcxproj_file_app_tag_map.end())
			{
				switch(vcxproj_to_cbp(vcxproj_path, out_app_tag, cb_file_tag, cb_sample_map, dep_info_cfg))
				{
				case xml_op::fix_result::E_IGNORE:
					ignore_vcxproj_files.insert(vcxproj_path);
				case xml_op::fix_result::E_SUCCEED:
					iter_app_tag_chk = vcxproj_file_app_tag_map.insert(std::make_pair(vcxproj_path, out_app_tag)).first;
					break;
				case xml_op::fix_result::E_FAILED:
					failed_vcxproj_files.insert(vcxproj_path);
					return xml_op::fix_result::E_FAILED;
				default:
					assert(false);
					failed_vcxproj_files.insert(vcxproj_path);
					return xml_op::fix_result::E_FAILED;
				}
			}

			if(iter_app_tag_chk->second == vcproj::vcproj_app_tags::s_tag_exe())
			{
				exe_map[i->first] = make_cbp_file_path(i->second, cb_file_tag);
			}
			else if(iter_app_tag_chk->second == vcproj::vcproj_app_tags::s_tag_dll())
			{
				if(!is_yggr_sln)
				{
					dll_map[i->first] = make_cbp_file_path(i->second, cb_file_tag);
				}
			}
			else if(iter_app_tag_chk->second == vcproj::vcproj_app_tags::s_tag_static_lib())
			{
				lib_map[i->first] = make_cbp_file_path(i->second, cb_file_tag);
			}

			if(bmake_cpp11_ws)
			{
				cbp_map[i->first] = make_cbp_file_path(vcxproj_path, cb_file_tag);
			}
		}
	}

	yggr::string ws_title = get_file_name_no_ext(sln_file);
	ptree_type ptree_workspace;
	yggr::u32 sat =
		vcxproj_map_to_workspace_ptree(
			ptree_workspace, vcxproj_map,
			lib_map, dll_map, exe_map,
			ws_title, cb_file_tag, cb_sample_map);

	if(sat != xml_op::fix_result::E_SUCCEED)
	{
		return sat;
	}

	yggr::utf8_string ws_file_path = make_workspace_file_path(sln_file, cb_file_tag);

	if(ws_file_path.empty())
	{
		return xml_op::fix_result::E_FAILED;
	}

	if(cbws::write_cb_workspace(ptree_workspace, ws_file_path))
	{
		if(bmake_cpp11_ws)
		{
			copy_and_make_cpp11_cb_workspace_from_cpp14(ws_file_path, cbp_map);
		}

		return xml_op::fix_result::E_SUCCEED;
	}
	else
	{
		return xml_op::fix_result::E_FAILED;
	}
}

std::size_t sln_list_to_workspace(const file_list_type& sln_file_list,
									vcproj_file_app_tag_map_type& vcxproj_file_map,
									const yggr::utf8_string& cb_file_tag,
									const ptree_umap_type& cb_sample_map,
									const lib::lib_depend_info& dep_info_cfg,
									file_list_type& failed_vcxproj_files,
									file_list_type& ignore_vcxproj_files,
									file_list_type& failed_sln_file_list,
									file_list_type& ignore_sln_file_list)
{
	std::size_t count = 0;
	for(file_list_type::const_iterator i = sln_file_list.begin(), isize = sln_file_list.end();
			i != isize; ++i)
	{
		switch(sln_to_workspace(
				*i, vcxproj_file_map, cb_file_tag, cb_sample_map, dep_info_cfg,
				failed_vcxproj_files, ignore_vcxproj_files))
		{
		case xml_op::fix_result::E_SUCCEED:
			++count;
			break;
		case xml_op::fix_result::E_FAILED:
			failed_sln_file_list.insert(*i);
			break;
		case xml_op::fix_result::E_IGNORE:
			ignore_sln_file_list.insert(*i);
			break;
		default:
			assert(false);
			failed_sln_file_list.insert(*i);
		}
	}

	return count;
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

	tool_conv_to_cb_solution_cfg cfg_root;
	cbp_sample_cfg cfg_sample;
	lib::lib_depend_info cfg_lib_dep_info;

	if(0 != format_config_file(cfg_fname, cfg_root, cfg_sample, cfg_lib_dep_info))
	{
		std::cout << "gen config file succeed, please modify it" << std::endl;
		return -1;
	}

	// {
	// 	file_list_type need_fix_vcxproj_files;
	// 	file_list_type need_fix_sln_files;

	// 	get_files(".", cfg_root.dir_filter_, cfg_root.file_filter_,
	// 				need_fix_vcxproj_files, need_fix_sln_files);
	// }

	ptree_umap_type cb_sample_map;

	if(make_cb_sample_ptree_map(cb_sample_map, cfg_sample.sample_file_map_).size() != cfg_sample.sample_file_map_.size())
	{
		std::cerr << "!!! code blocks sample filew is not succeed !!!" << std::endl;

		for(cbp_sample_cfg::sample_file_map_type::const_iterator i = cfg_sample.sample_file_map_.begin(),
				isize = cfg_sample.sample_file_map_.end(); i != isize; ++i)
		{
			if(cb_sample_map.find(i->first) == cb_sample_map.end())
			{
				std::cerr << "sample_file: " << i->second << " failed" << std::endl;
			}
		}

		return -1;
	}

	file_list_type need_fix_vcxproj_files;
	file_list_type need_fix_sln_files;

	get_files(cfg_root.root_dir_, cfg_root.dir_filter_, cfg_root.file_filter_,
				need_fix_vcxproj_files, need_fix_sln_files);

	file_list_type failed_vcxproj_files;
	file_list_type ignore_vcxproj_files;

	vcproj_file_app_tag_map_type maped_need_fix_vcxproj_files;

	for(file_list_type::const_iterator i = need_fix_vcxproj_files.begin(), isize = need_fix_vcxproj_files.end(); i != isize; ++i)
	{
		maped_need_fix_vcxproj_files[*i] = "";
	}

	assert(maped_need_fix_vcxproj_files.size() == need_fix_vcxproj_files.size());

	//std::size_t vcxproj_fixed_count =
		vcxproj_list_to_cbp(
			maped_need_fix_vcxproj_files,
			cfg_sample.file_tag_, cb_sample_map, cfg_lib_dep_info,
			failed_vcxproj_files, ignore_vcxproj_files);

	//assert(vcxproj_fixed_count);

	file_list_type failed_sln_files;
	file_list_type ignore_sln_files;

	std::size_t sln_fixed_count =
		sln_list_to_workspace(
			need_fix_sln_files, maped_need_fix_vcxproj_files,
			cfg_sample.file_tag_, cb_sample_map, cfg_lib_dep_info,
			failed_vcxproj_files, ignore_vcxproj_files,
			failed_sln_files, ignore_sln_files);

	assert(sln_fixed_count == need_fix_sln_files.size());

	write_log_file(yggr::utf8_string("tool_conv_vcxproj_to_cbp_failed_log.txt"), xml_op::fix_result::E_FAILED, failed_vcxproj_files);
	write_log_file(yggr::utf8_string("tool_conv_vcxproj_to_cbp_ignore_log.txt"), xml_op::fix_result::E_IGNORE, ignore_vcxproj_files);

	write_log_file(yggr::utf8_string("tool_conv_sln_to_workspace_failed_log.txt"), xml_op::fix_result::E_FAILED, failed_sln_files);
	write_log_file(yggr::utf8_string("tool_conv_sln_to_workspace_ignore_log.txt"), xml_op::fix_result::E_IGNORE, ignore_sln_files);

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

