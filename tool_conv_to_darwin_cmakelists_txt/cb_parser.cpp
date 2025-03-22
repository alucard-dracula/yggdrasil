// cb_parser.cpp

#include "cb_parser.hpp"

#if !(BOOST_VERSION < 105600)

#include "xml_operator.hpp"
#include "file_operator.hpp"

#include <yggr/base/static_constant.hpp>
#include <yggr/container/set.hpp>

#include <boost/ref.hpp>
#include <utility>

namespace cb
{


struct proj_debug_tag
{
	YGGR_STATIC_CONSTANT(yggr::u32, E_TAG_PROJ = 0);
	YGGR_STATIC_CONSTANT(yggr::u32, E_TAG_PROJ_DEBUG = 1 << 0);
	YGGR_STATIC_CONSTANT(yggr::u32, E_TAG_PROJ_RELEASE = 1 << 1);
};

struct flag_filter
{
	YGGR_STATIC_CONSTANT(yggr::u32, E_FLAG_FAILED = 0);
	YGGR_STATIC_CONSTANT(yggr::u32, E_FLAG_IGNORE = 1);
	YGGR_STATIC_CONSTANT(yggr::u32, E_FLAG_C = 1 << 1);
	YGGR_STATIC_CONSTANT(yggr::u32, E_FLAG_CPP = 1 << 2);
	YGGR_STATIC_CONSTANT(yggr::u32, E_FLAG_LD = 1 << 3);

	YGGR_STATIC_CONSTANT(yggr::u32, E_FLAG_C_INCLUDE = 1 << 4);
	YGGR_STATIC_CONSTANT(yggr::u32, E_FLAG_C_DEFINED = 1 << 5);
	YGGR_STATIC_CONSTANT(yggr::u32, E_FLAG_LD_LIB_PATH = 1 << 6);
	YGGR_STATIC_CONSTANT(yggr::u32, E_FLAG_LD_LIB = 1 << 7);
};

yggr::u32 flag_filter_link(const ptree_string_type& flag)
{
	typedef yggr::container::set<ptree_string_type> string_set_type;

	static string_set_type exclude_ldflags;

	if(!exclude_ldflags.size())
	{
		exclude_ldflags.insert("-m32");
		exclude_ldflags.insert("-m64");
		exclude_ldflags.insert("-s");
		//exclude_ldflags.insert("-lpthread");

		//exclude_ldflags.insert("-lyggr_nsql_database_system$(#COMPILER_VERSION_TAG)-d");
		//exclude_ldflags.insert("-lyggr_nsql_database_system$(#COMPILER_VERSION_TAG)");

		//exclude_ldflags.insert("-lmongoc$(#COMPILER_VERSION_TAG)-d");
		//exclude_ldflags.insert("-lmongoc$(#COMPILER_VERSION_TAG)");

		//exclude_ldflags.insert("-lmongocrypt$(#COMPILER_VERSION_TAG)-d");
		//exclude_ldflags.insert("-lmongocrypt$(#COMPILER_VERSION_TAG)");

		//exclude_ldflags.insert("-lkms-message$(#COMPILER_VERSION_TAG)-d");
		//exclude_ldflags.insert("-lkms-message$(#COMPILER_VERSION_TAG)");

		//exclude_ldflags.insert("-lutf8proc$(#COMPILER_VERSION_TAG)-d");
		//exclude_ldflags.insert("-lutf8proc$(#COMPILER_VERSION_TAG)");

		//exclude_ldflags.insert("-lbson$(#COMPILER_VERSION_TAG)-d");
		//exclude_ldflags.insert("-lbson$(#COMPILER_VERSION_TAG)");

		//exclude_ldflags.insert("-lcommon$(#COMPILER_VERSION_TAG)-d");
		//exclude_ldflags.insert("-lcommon$(#COMPILER_VERSION_TAG)");

		//exclude_ldflags.insert("-lintel_dfp_obj$(#COMPILER_VERSION_TAG)-d");
		//exclude_ldflags.insert("-lintel_dfp_obj$(#COMPILER_VERSION_TAG)");

		//exclude_ldflags.insert("-lsasl2");
		//exclude_ldflags.insert("-lresolv");
	}

	if(!flag.size())
	{
		return flag_filter::E_FLAG_FAILED;
	}

	if(exclude_ldflags.find(flag) != exclude_ldflags.end())
	{
		return flag_filter::E_FLAG_IGNORE;
	}

	if(flag.size() > 2)
	{
		if(flag.substr(0, 2) == "-L")
		{
			return flag_filter::E_FLAG_LD_LIB_PATH;
		}
		else if(flag.substr(0, 2) == "-l")
		{
			return flag_filter::E_FLAG_LD_LIB;
		}
		else
		{
			return flag_filter::E_FLAG_LD;
		}
	}
	else
	{
		return flag_filter::E_FLAG_LD;
	}
}

yggr::u32 flag_filter_compiler(const ptree_string_type& flag)
{
	typedef yggr::container::set<ptree_string_type> string_set_type;

	static string_set_type exclude_c_flags;
	static string_set_type cpp_flags;

	if(!exclude_c_flags.size())
	{
		exclude_c_flags.insert("-m32");
		exclude_c_flags.insert("-m64");
		exclude_c_flags.insert("-std=c++03");
		exclude_c_flags.insert("-std=c++11");
		exclude_c_flags.insert("-std=c++14");
		exclude_c_flags.insert("-std=c++17");
		exclude_c_flags.insert("-std=c++20");
		exclude_c_flags.insert("-std=c++23");
	}

	if(!cpp_flags.size())
	{
		cpp_flags.insert("-fno-elide-constructors");
	}

	if(!flag.size())
	{
		return flag_filter::E_FLAG_FAILED;
	}

	if(exclude_c_flags.find(flag) != exclude_c_flags.end())
	{
		return flag_filter::E_FLAG_IGNORE;
	}

	if(cpp_flags.find(flag) != cpp_flags.end())
	{
		return flag_filter::E_FLAG_CPP;
	}

	if(flag.size() > 2)
	{
		if(flag.substr(0, 2) == "-I")
		{
			return flag_filter::E_FLAG_C_INCLUDE;
		}
		else if(flag.substr(0, 2) == "-D")
		{
			return flag_filter::E_FLAG_C_DEFINED;
		}
		else
		{
			return flag_filter::E_FLAG_C;
		}
	}
	else
	{
		return flag_filter::E_FLAG_C;
	}
}

cb_infos& parse_cb_infos_linker(cb_infos& infos, const ptree_type& cb_ptree, yggr::u32 debug_mark)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef cb_infos::string_vt_type string_vt_type;

	if(!cb_app_tags::s_has_linker_option(infos.proj_t_))
	{
		return infos;
	}

	string_set_type* pldflags = 0;
	string_vt_type* plink_libs = 0;

	switch(debug_mark)
	{
	case proj_debug_tag::E_TAG_PROJ:
		pldflags = boost::addressof(infos.ldflag_proj_);
		plink_libs = boost::addressof(infos.link_libs_proj_);
		break;
	case proj_debug_tag::E_TAG_PROJ_DEBUG:
		pldflags = boost::addressof(infos.ldflag_proj_debug_);
		plink_libs = boost::addressof(infos.link_libs_proj_debug_);
		break;
	case proj_debug_tag::E_TAG_PROJ_RELEASE:
		pldflags = boost::addressof(infos.ldflag_proj_release_);
		plink_libs = boost::addressof(infos.link_libs_proj_release_);
		break;
	default:
		break;
	}

	if(!(pldflags && plink_libs))
	{
		infos.failed();
		return infos;
	}

	for(const_iterator i = cb_ptree.begin(), isize = cb_ptree.end();i != isize; ++i)
	{
		if(i->first == "Add")
		{
			if(xml_op::has_attribs(i->second, "option"))
			{
				string_type attr = xml_op::get_attribs(i->second, "option");

				switch(flag_filter_link(attr))
				{
				case flag_filter::E_FLAG_IGNORE:
					break;
				case flag_filter::E_FLAG_LD:
					pldflags->insert(boost::move(attr));
					break;
				case flag_filter::E_FLAG_LD_LIB_PATH:
					infos.link_libs_path_.insert(attr.substr(2, attr.size()));
					break;
				case flag_filter::E_FLAG_LD_LIB:
					plink_libs->push_back(boost::move(attr));
					break;
				default:
					infos.failed();
					return infos;
				}
			}
			else if(xml_op::has_attribs(i->second, "directory"))
			{
				string_type attr = xml_op::get_attribs(i->second, "directory");
				if(attr.size())
				{
					infos.link_libs_path_.insert(boost::move(attr));
				}
			}
		}
	}

	return infos;
}

cb_infos& parse_cb_infos_compiler(cb_infos& infos, const ptree_type& cb_ptree, yggr::u32 debug_mark)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef cb_infos::string_vt_type string_vt_type;
	typedef cb_infos::string_set_type string_set_type;

	if(!cb_app_tags::s_has_compile_option(infos.proj_t_))
	{
		return infos;
	}

	string_set_type* pcinclude = 0;
	string_set_type* pcdefined = 0;
	string_set_type* pcflags = 0;
	string_set_type* pcppflags = 0;

	pcinclude = boost::addressof(infos.cinclude_);

	switch(debug_mark)
	{
	case proj_debug_tag::E_TAG_PROJ:
		pcdefined = boost::addressof(infos.cdefined_proj_);
		pcflags = boost::addressof(infos.cflag_proj_);
		pcppflags = boost::addressof(infos.cppflag_proj_);
		break;
	case proj_debug_tag::E_TAG_PROJ_DEBUG:
		pcdefined = boost::addressof(infos.cdefined_proj_debug_);
		pcflags = boost::addressof(infos.cflag_proj_debug_);
		pcppflags = boost::addressof(infos.cppflag_proj_debug_);
		break;
	case proj_debug_tag::E_TAG_PROJ_RELEASE:
		pcdefined = boost::addressof(infos.cdefined_proj_release_);
		pcflags = boost::addressof(infos.cflag_proj_release_);
		pcppflags = boost::addressof(infos.cppflag_proj_release_);
		break;
	default:
		break;
	}

	if(!(pcflags && pcppflags && pcinclude && pcdefined))
	{
		infos.failed();
		return infos;
	}

	for(const_iterator i = cb_ptree.begin(), isize = cb_ptree.end();i != isize; ++i)
	{
		if(i->first == "Add")
		{
			if(xml_op::has_attribs(i->second, "option"))
			{
				string_type attr = xml_op::get_attribs(i->second, "option");

				switch(flag_filter_compiler(attr))
				{
				case flag_filter::E_FLAG_IGNORE:
					break;
				case flag_filter::E_FLAG_C:
					pcflags->insert(boost::move(attr));
					break;
				case flag_filter::E_FLAG_CPP:
					pcppflags->insert(boost::move(attr));
					break;
				case flag_filter::E_FLAG_C_INCLUDE:
					pcinclude->insert(attr.substr(2, attr.size()));
					break;
				case flag_filter::E_FLAG_C_DEFINED:
					{
						if(attr == "-DLUA_USE_LINUX")
						{
							pcdefined->insert(string_type("-DLUA_USE_MACOSX"));
						}
						else
						{
							pcdefined->insert(boost::move(attr));
						}
					}
					break;
				default:
					infos.failed();
					return infos;
				}
			}
			else if(xml_op::has_attribs(i->second, "directory"))
			{
				string_type attr = xml_op::get_attribs(i->second, "directory");
				if(attr.size())
				{
					pcinclude->insert(boost::move(attr));
				}
			}
		}
	}

	switch(debug_mark)
	{
	case proj_debug_tag::E_TAG_PROJ:
		pcppflags->insert("-frtti");
		break;
	default:
		break;
	}

	return infos;
}

cb_infos& parse_cb_infos_build_target(cb_infos& infos, const ptree_type& cb_ptree, yggr::u32 debug_mark)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;
	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	for(const_iterator i = cb_ptree.begin(), isize = cb_ptree.end();i != isize; ++i)
	{
		if(i->first == "Option")
		{
			if(xml_op::has_attribs(i->second, "type"))
			{
				string_type app_tag = xml_op::get_attribs(i->second, "type");

				if(app_tag.empty())
				{
					infos.failed();
					return infos;
				}
				
				if(infos.proj_t_.size())
				{
					if(infos.proj_t_ != app_tag)
					{
						infos.failed();
						return infos;
					}
				}
				else
				{
					infos.proj_t_ = app_tag;
				}
			}
			else if(xml_op::has_attribs(i->second, "output"))
			{
				string_type str_output = xml_op::get_attribs(i->second, "output");

				if(str_output.empty())
				{
					infos.failed();
					return infos;
				}

				switch(debug_mark)
				{
				case proj_debug_tag::E_TAG_PROJ_DEBUG:
					infos.proj_output_debug_ = str_output;
					break;
				case proj_debug_tag::E_TAG_PROJ_RELEASE:
					infos.proj_output_release_ = str_output;
					break;
				default:
					infos.failed();
					return infos;
				}
			}
		}
		else if(i->first == "Compiler")
		{
			if(parse_cb_infos_compiler(infos, i->second, debug_mark).proj_t_.empty())
			{
				return infos;
			}
		}
		else if(i->first == "Linker")
		{
			if(parse_cb_infos_linker(infos, i->second, debug_mark).proj_t_.empty())
			{
				return infos;
			}
		}
	}

	return infos;
}

cb_infos& parse_cb_infos_build(cb_infos& infos, const ptree_type& cb_ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	for(const_iterator i = cb_ptree.begin(), isize = cb_ptree.end();i != isize; ++i)
	{
		if(i->first == "Target")
		{
			string_type attrib = xml_op::get_attribs(i->second, "title");

			if(attrib == cb_cfg_marks::s_mark_debug_x64())
			{
				parse_cb_infos_build_target(infos, i->second, proj_debug_tag::E_TAG_PROJ_DEBUG);
			}
			else if(attrib == cb_cfg_marks::s_mark_release_x64())
			{
				parse_cb_infos_build_target(infos, i->second, proj_debug_tag::E_TAG_PROJ_RELEASE);
			}
		}
	}

	return infos;
}

cb_infos& parse_cb_infos(cb_infos& infos, const ptree_type& cb_ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	assoc_citer_type assoc_iter = cb_ptree.find("CodeBlocks_project_file");
	assert(assoc_iter != cb_ptree.not_found());
	if(assoc_iter == cb_ptree.not_found())
	{
		return infos;
	}

	assoc_iter = assoc_iter->second.find("Project");
	assert(assoc_iter != assoc_iter->second.not_found());
	if(assoc_iter == assoc_iter->second.not_found())
	{
		return infos;
	}

	for(const_iterator i = assoc_iter->second.begin(), isize = assoc_iter->second.end();
			i != isize; ++i)
	{
		if(i->first == "Option")
		{
			if(xml_op::has_attribs(i->second, "title"))
			{
				string_type proj_name = xml_op::get_attribs(i->second, "title");

				if(proj_name.empty())
				{
					infos.proj_name_.clear();
					return infos;
				}

				infos.proj_name_ = proj_name;
			}
		}
		else if(i->first == "Build")
		{
			if(parse_cb_infos_build(infos, i->second).proj_t_.empty())
			{
				return infos;
			}
		}
		else if(i->first == "Compiler")
		{
			parse_cb_infos_compiler(infos, i->second, proj_debug_tag::E_TAG_PROJ);
		}
		else if(i->first == "Linker")
		{
			parse_cb_infos_linker(infos, i->second, proj_debug_tag::E_TAG_PROJ);
		}
		else if(i->first == "Unit")
		{
			string_type fname = xml_op::get_attribs(i->second, "filename");

			if((4 < fname.size() && fname.substr(fname.size() - 4) == ".cpp")
				|| (2 < fname.size() && fname.substr(fname.size() - 2) == ".c"))
			{
				infos.src_files_.insert(boost::move(fname));
			}		
		}
	}

	return infos;
}

ptree_string_type get_cb_app_tag(const ptree_type& cb_ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	assoc_citer_type assoc_iter = cb_ptree.find("CodeBlocks_project_file");
	assert(assoc_iter != cb_ptree.not_found());
	if(assoc_iter == cb_ptree.not_found())
	{
		return ptree_string_type();
	}

	assoc_iter = assoc_iter->second.find("Project");
	assert(assoc_iter != assoc_iter->second.not_found());
	if(assoc_iter == assoc_iter->second.not_found())
	{
		return ptree_string_type();
	}

	assoc_iter = assoc_iter->second.find("Build");
	assert(assoc_iter != assoc_iter->second.not_found());
	if(assoc_iter == assoc_iter->second.not_found())
	{
		return ptree_string_type();
	}

	for(const_iterator i = assoc_iter->second.begin(), isize = assoc_iter->second.end();
			i != isize; ++i)
	{
		if(i->first == "Target")
		{
			for(const_iterator j = i->second.begin(), jsize = i->second.end(); j != jsize; ++j)
			{
				if(j->first == "Option" && xml_op::has_attribs(j->second, "type"))
				{
					return xml_op::get_attribs(j->second, "type");
				}
			}
		}
	}

	return ptree_string_type();
}

ptree_type& get_cb_ptree_from_buffer(ptree_type& out, const yggr::utf8_string& file_data)
{
	return xml_op::read_xml_from_buffer(out, file_data);
}

ptree_type& get_cb_ptree_from_file(ptree_type& out, const yggr::utf8_string& fname)
{
	return xml_op::read_xml_from_file(out, fname);
}


} // namespace cb

#endif // !(BOOST_VERSION < 105600)
