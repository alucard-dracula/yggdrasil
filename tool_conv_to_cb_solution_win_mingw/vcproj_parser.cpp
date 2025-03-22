// vcproj_parser.cpp

#include "vcproj_parser.hpp"

#if !(BOOST_VERSION < 105600)

#include "xml_operator.hpp"
#include "file_operator.hpp"

#include <yggr/container/set.hpp>

#include <sstream>
#include <utility>

namespace vcproj
{

bool is_cl_fix_object(const ptree_string_type& app_tag)
{
	return 
		(app_tag == vcproj_app_tags::s_tag_exe()) 
		|| (app_tag == vcproj_app_tags::s_tag_dll()) 
		|| (app_tag == vcproj_app_tags::s_tag_static_lib());
}


string_set_type& parse_opt_items(string_set_type& out, const ptree_string_type& str_opt_items)
{
	typedef ptree_string_type::const_iterator citer_type;

	for(citer_type i = str_opt_items.begin(), ipos = str_opt_items.begin(), isize = str_opt_items.end(); 
			i != isize; i = ipos)
	{
		ipos = std::find(i, isize, ';');
		out.insert(ptree_string_type(i, ipos));
		if(ipos != isize) 
		{
			++ipos;
		}
	}

	return out;
}

ptree_string_type remove_item_vcxproj_rule(const ptree_string_type& str,
											const ptree_string_type& rule_str)
{
	std::size_t pos = str.find(rule_str);

	if(pos == ptree_string_type::npos)
	{
		return str;
	}
	else if(pos == 0)
	{
		return 
			rule_str.size() < str.size()? 
				str.substr(rule_str.size() + 1)
				: ptree_string_type();
	}
	else
	{
		return 
			rule_str.size() < str.size()?
				str.substr(0, pos - 1) + str.substr(pos + rule_str.size())
				: ptree_string_type();
	}
}

ptree_string_type get_app_tag(const ptree_type& ptree)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("PropertyGroup");
	ptree_string_type cfg_mark;

	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Label") == "Configuration")
		{
			const_iterator citer = i->second.to_iterator(i->second.find("ConfigurationType"));
			if(cfg_mark.empty())
			{
				cfg_mark = citer->second.get_value<string_type>();
			}
			else
			{
				if(cfg_mark != citer->second.get_value<string_type>())
				{
					return string_type();
				}
			}
		}
	}

	return cfg_mark;
}

// no format version
//ptree_string_type get_outdir(const ptree_type& ptree,
//										const ptree_string_type& app_tag,
//										const ptree_string_type& cfg_mark)
//{
//	typedef ptree_string_type string_type;
//	typedef ptree_type::assoc_iterator assoc_iter_type;
//	typedef ptree_type::const_assoc_iterator assoc_citer_type;
//	typedef ptree_type::iterator iterator;
//	typedef ptree_type::const_iterator const_iterator;
//	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
//	typedef boost::optional<const ptree_type&> attribs_type;
//
//	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;
//
//	if(!vcproj_app_tags::s_has_compile_option(app_tag))
//	{
//		return string_type();
//	}
//
//	assoc_citer_type assoc_iter = ptree.find("Project");
//	assert(assoc_iter != ptree.not_found());
//	if(assoc_iter == ptree.not_found())
//	{
//		return string_type();
//	}
//
//	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("PropertyGroup");
//
//	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
//	{
//		assoc_citer_pair_type jiter_pair = i->second.equal_range("OutDir");
//
//		if(jiter_pair.first != jiter_pair.second)
//		{
//			for(assoc_citer_type j = jiter_pair.first, jsize = jiter_pair.second; j != jsize; ++j)
//			{
//				if(xml_op::get_attribs(j->second, "Condition") == cfg_mark)
//				{
//					return j->second.get_value<string_type>();
//				}
//			}
//
//			return string_type();
//		}
//	}
//
//	return string_type();
//}

ptree_string_type get_outdir(const ptree_type& ptree,
										const ptree_string_type& app_tag,
										const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!vcproj_app_tags::s_has_compile_option(app_tag))
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("PropertyGroup");

	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if((!xml_op::has_attribs(i->second, "Label")) 
			&& (xml_op::get_attribs(i->second, "Condition") == cfg_mark))
		{
			assoc_citer_type val_iter = i->second.find("OutDir");
				
			return 
				val_iter == i->second.not_found()? 
					string_type() 
					: val_iter->second.get_value<string_type>();
		}
	}

	return string_type();
}

// no format version
//ptree_string_type get_target_name(const ptree_type& ptree,
//										const ptree_string_type& app_tag,
//										const ptree_string_type& cfg_mark)
//{
//	typedef ptree_string_type string_type;
//	typedef ptree_type::assoc_iterator assoc_iter_type;
//	typedef ptree_type::const_assoc_iterator assoc_citer_type;
//	typedef ptree_type::iterator iterator;
//	typedef ptree_type::const_iterator const_iterator;
//	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
//	typedef boost::optional<const ptree_type&> attribs_type;
//
//	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;
//
//	if(!vcproj_app_tags::s_has_compile_option(app_tag))
//	{
//		return string_type();
//	}
//
//	assoc_citer_type assoc_iter = ptree.find("Project");
//	assert(assoc_iter != ptree.not_found());
//	if(assoc_iter == ptree.not_found())
//	{
//		return string_type();
//	}
//
//	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("PropertyGroup");
//
//	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
//	{
//		assoc_citer_pair_type jiter_pair = i->second.equal_range("TargetName");
//
//		if(jiter_pair.first != jiter_pair.second)
//		{
//			for(assoc_citer_type j = jiter_pair.first, jsize = jiter_pair.second; j != jsize; ++j)
//			{
//				if(xml_op::get_attribs(j->second, "Condition") == cfg_mark)
//				{
//					return j->second.get_value<string_type>();
//				}
//			}
//
//			return string_type();
//		}
//	}
//
//	return string_type();
//}

ptree_string_type get_target_name(const ptree_type& ptree,
									const ptree_string_type& app_tag,
									const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!vcproj_app_tags::s_has_compile_option(app_tag))
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("PropertyGroup");

	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if((!xml_op::has_attribs(i->second, "Label")) 
			&& (xml_op::get_attribs(i->second, "Condition") == cfg_mark))
		{
			assoc_citer_type val_iter = i->second.find("TargetName");
				
			return 
				val_iter == i->second.not_found()? 
					string_type() 
				: val_iter->second.get_value<string_type>();
		}
	}

	return string_type();
}

ptree_string_type get_target_ext(const ptree_type& ptree,
									const ptree_string_type& app_tag,
									const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!vcproj_app_tags::s_has_compile_option(app_tag))
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("PropertyGroup");

	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if((!xml_op::has_attribs(i->second, "Label")) 
			&& (xml_op::get_attribs(i->second, "Condition") == cfg_mark))
		{
			assoc_citer_type val_iter = i->second.find("TargetExt");
				
			return 
				val_iter == i->second.not_found()? 
					string_type() 
					: val_iter->second.get_value<string_type>();
		}
	}

	return string_type();
}

void get_outdir_and_target_name_and_target_ext(ptree_string_type& str_outdir,
												ptree_string_type& str_target_name,
												ptree_string_type& str_target_ext,
												const ptree_type& ptree,
												const ptree_string_type& app_tag,
												const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;
	
	str_outdir.clear();
	str_target_name.clear();
	str_target_ext.clear();

	if(!vcproj_app_tags::s_has_compile_option(app_tag))
	{
		return;
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return;
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("PropertyGroup");

	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if((!xml_op::has_attribs(i->second, "Label")) 
			&& (xml_op::get_attribs(i->second, "Condition") == cfg_mark))
		{
			{
				assoc_citer_type val_iter = i->second.find("OutDir");
				if(val_iter != i->second.not_found())
				{
					str_outdir = val_iter->second.get_value<string_type>();
				}
			}

			{
				assoc_citer_type val_iter = i->second.find("TargetName");
				if(val_iter != i->second.not_found())
				{
					str_target_name = val_iter->second.get_value<string_type>();
				}
			}

			{
				assoc_citer_type val_iter = i->second.find("TargetExt");
				if(val_iter != i->second.not_found())
				{
					str_target_ext = val_iter->second.get_value<string_type>();
				}
			}
		}
	}
}

ptree_string_type get_language_standard(const ptree_type& ptree,
										const ptree_string_type& app_tag,
										const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!vcproj_app_tags::s_has_compile_option(app_tag))
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("ItemDefinitionGroup");

	
	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Condition") == cfg_mark)
		{
			const_iterator citer1, citer2;
			
			if(((citer1 = i->second.to_iterator(i->second.find("ClCompile"))) == i->second.end())
				|| ((citer2 = citer1->second.to_iterator(citer1->second.find("LanguageStandard"))) == citer1->second.end()))
			{
				return string_type();
			}
			else
			{
				return citer2->second.get_value<string_type>();
			}
		}
	}

	return string_type();
}

ptree_string_type get_impl_lib(const ptree_type& ptree,
								const ptree_string_type& app_tag,
								const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!(app_tag == vcproj_app_tags::s_tag_dll()))
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("ItemDefinitionGroup");

	
	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Condition") == cfg_mark)
		{
			const_iterator citer1, citer2;
			
			if(((citer1 = i->second.to_iterator(i->second.find("Link"))) == i->second.end())
				|| ((citer2 = citer1->second.to_iterator(citer1->second.find("ImportLibrary"))) == citer1->second.end()))
			{
				return string_type();
			}
			else
			{
				return citer2->second.get_value<string_type>();
			}
		}
	}

	return string_type();
}

ptree_string_type get_module_definition_file(const ptree_type& ptree,
												const ptree_string_type& app_tag,
												const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!(app_tag == vcproj_app_tags::s_tag_dll()))
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("ItemDefinitionGroup");

	
	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Condition") == cfg_mark)
		{
			const_iterator citer1, citer2;
			
			if(((citer1 = i->second.to_iterator(i->second.find("Link"))) == i->second.end())
				|| ((citer2 = citer1->second.to_iterator(citer1->second.find("ModuleDefinitionFile"))) == citer1->second.end()))
			{
				return string_type();
			}
			else
			{
				return citer2->second.get_value<string_type>();
			}
		}
	}

	return string_type();
}

ptree_string_type get_subsystem(const ptree_type& ptree,
								const ptree_string_type& app_tag,
								const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!((app_tag == vcproj_app_tags::s_tag_exe()) 
			|| (app_tag == vcproj_app_tags::s_tag_dll()) ) )
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("ItemDefinitionGroup");

	
	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Condition") == cfg_mark)
		{
			const_iterator citer1, citer2;
			
			if(((citer1 = i->second.to_iterator(i->second.find("Link"))) == i->second.end())
				|| ((citer2 = citer1->second.to_iterator(citer1->second.find("SubSystem"))) == citer1->second.end()))
			{
				return string_type();
			}
			else
			{
				return citer2->second.get_value<string_type>();
			}
		}
	}

	return string_type();
}

ptree_string_type get_preprocessor_definitions(const ptree_type& ptree,
												const ptree_string_type& app_tag,
												const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!is_cl_fix_object(app_tag))
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("ItemDefinitionGroup");

	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Condition") == cfg_mark)
		{
			const_iterator citer1, citer2;
			
			if(((citer1 = i->second.to_iterator(i->second.find("ClCompile"))) == i->second.end())
				|| ((citer2 = citer1->second.to_iterator(citer1->second.find("PreprocessorDefinitions"))) == citer1->second.end()))
			{
				return string_type();
			}
			else
			{
				return
					remove_item_vcxproj_rule(
						citer2->second.get_value<string_type>(),
						"%(PreprocessorDefinitions)");
			}
		}
	}

	return string_type();
}

ptree_string_type get_additional_dependencies(const ptree_type& ptree,
												const ptree_string_type& app_tag,
												const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!((app_tag == vcproj_app_tags::s_tag_exe()) 
			|| (app_tag == vcproj_app_tags::s_tag_dll()) ) )
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("ItemDefinitionGroup");

	
	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Condition") == cfg_mark)
		{
			const_iterator citer1, citer2;
			
			if(((citer1 = i->second.to_iterator(i->second.find("Link"))) == i->second.end())
				|| ((citer2 = citer1->second.to_iterator(citer1->second.find("AdditionalDependencies"))) == citer1->second.end()))
			{
				return string_type();
			}
			else
			{
				string_type tmp = 
					remove_item_vcxproj_rule(
						citer2->second.get_value<string_type>(),
						"%(AdditionalDependencies)");
				file_op::format_string_file_path(tmp);
				return tmp;
			}
		}
	}

	return string_type();
}

ptree_string_type get_additional_library_directories(const ptree_type& ptree,
														const ptree_string_type& app_tag,
														const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!((app_tag == vcproj_app_tags::s_tag_exe()) 
			|| (app_tag == vcproj_app_tags::s_tag_dll()) ) )
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("ItemDefinitionGroup");

	
	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Condition") == cfg_mark)
		{
			const_iterator citer1, citer2;
			
			if(((citer1 = i->second.to_iterator(i->second.find("Link"))) == i->second.end())
				|| ((citer2 = citer1->second.to_iterator(citer1->second.find("AdditionalLibraryDirectories"))) == citer1->second.end()))
			{
				return string_type();
			}
			else
			{
				string_type tmp = 
					remove_item_vcxproj_rule(
						citer2->second.get_value<string_type>(),
						"%(AdditionalLibraryDirectories)");
				file_op::format_string_file_path(tmp);
				return tmp;
			}
		}
	}

	return string_type();
}


ptree_string_type get_additional_include_directories(const ptree_type& ptree,
														const ptree_string_type& app_tag,
														const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!is_cl_fix_object(app_tag))
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("ItemDefinitionGroup");

	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Condition") == cfg_mark)
		{
			const_iterator citer1, citer2;
			
			if(((citer1 = i->second.to_iterator(i->second.find("ClCompile"))) == i->second.end())
				|| ((citer2 = citer1->second.to_iterator(citer1->second.find("AdditionalIncludeDirectories"))) == citer1->second.end()))
			{
				return string_type();
			}
			else
			{
				string_type tmp = 
					remove_item_vcxproj_rule(
						citer2->second.get_value<string_type>(),
						"%(AdditionalIncludeDirectories)");
				file_op::format_string_file_path(tmp);
				return tmp;
			}
		}
	}

	return string_type();
}

ptree_string_type get_additional_options(const ptree_type& ptree,
											const ptree_string_type& app_tag,
											const ptree_string_type& cfg_mark)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;

	if(!is_cl_fix_object(app_tag))
	{
		return string_type();
	}

	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("ItemDefinitionGroup");

	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Condition") == cfg_mark)
		{
			const_iterator citer1, citer2;
			
			if(((citer1 = i->second.to_iterator(i->second.find("ClCompile"))) == i->second.end())
				|| ((citer2 = citer1->second.to_iterator(citer1->second.find("AdditionalOptions"))) == citer1->second.end()))
			{
				return string_type();
			}
			else
			{
				string_type tmp = 
					remove_item_vcxproj_rule(
						citer2->second.get_value<string_type>(),
						"%(AdditionalOptions)");
				return tmp;
			}
		}
	}

	return string_type();
}

//bool has_big_obj_from_additional_options(const ptree_type& ptree,
//											const ptree_string_type& app_tag,
//											const ptree_string_type& cfg_mark)
//{
//	ptree_string_type str_add_opt = get_additional_options(ptree, app_tag, cfg_mark);
//
//	return str_add_opt.find("/bigobj") != ptree_string_type::npos;
//
//}

//bool has_big_obj_from_root_namespace(const ptree_type& ptree)
//{
//	typedef yggr::container::set<ptree_string_type> string_set_type;
//
//	static string_set_type str_set;
//
//	if(str_set.empty())
//	{
//		str_set.insert("clt_test_async");
//	}
//
//	ptree_string_type str_root_namespace = get_root_namespace(ptree);
//
//	return (str_set.find(str_root_namespace) != str_set.end());
//}
//
//bool has_big_obj_from_filename(const ptree_type& ptree)
//{
//	typedef yggr::container::set<ptree_string_type> string_set_type;
//
//	static string_set_type str_set;
//
//	if(str_set.empty())
//	{
//		str_set.insert("init_client.cpp");
//	}
//
//	code_file_set_type code_files;
//	get_code_files(code_files, ptree);
//
//	for(code_file_set_type::const_iterator i = code_files.begin(), isize = code_files.end(); i != isize; ++i)
//	{
//		if(str_set.find(*i) != str_set.end())
//		{
//			return true;
//		}
//	}
//	return false;
//}

//bool has_big_obj(const ptree_type& ptree)
//{
//	return
//		has_big_obj_from_root_namespace(ptree)
//		|| has_big_obj_from_filename(ptree);
//}

bool has_big_obj(const ptree_type& ptree,
					const ptree_string_type& app_tag,
					const ptree_string_type& cfg_mark)
{
	ptree_string_type str_add_opt = get_additional_options(ptree, app_tag, cfg_mark);

	return str_add_opt.find("/bigobj") != ptree_string_type::npos;
}

ptree_string_type get_root_namespace(const ptree_type& ptree)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;


	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return string_type();
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("PropertyGroup");

	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		if(xml_op::get_attribs(i->second, "Label") == "Globals")
		{
			const_iterator citer = i->second.to_iterator(i->second.find("RootNamespace"));
			return (citer == i->second.end())? string_type() : citer->second.get_value<string_type>();
		}
	}

	return string_type();
}

code_file_set_type& get_code_files(code_file_set_type& out, const ptree_type& ptree)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	typedef std::pair<assoc_citer_type, assoc_citer_type> assoc_citer_pair_type;


	assoc_citer_type assoc_iter = ptree.find("Project");
	assert(assoc_iter != ptree.not_found());
	if(assoc_iter == ptree.not_found())
	{
		return out;
	}

	assoc_citer_pair_type iter_pair = assoc_iter->second.equal_range("ItemGroup");

	string_type file_name;
	code_file_set_type tmp_out;
	for(assoc_citer_type i = iter_pair.first, isize = iter_pair.second; i != isize; ++i)
	{
		for(const_iterator j = i->second.begin(), jsize = i->second.end(); j != jsize; ++j)
		{
			if(j->first == "ClCompile" || j->first == "ClInclude" || j->first == "None")
			{
				file_name = xml_op::get_attribs(j->second, "Include");
				if(file_name.empty())
				{
					return out;
				}

				if(!tmp_out.insert(file_name).second)
				{
					return out;
				}
			}
		}
	}

	std::size_t chk_size = out.size() + tmp_out.size();

	out.insert(tmp_out.begin(), tmp_out.end());

	if(out.size() != chk_size)
	{
		code_file_set_type tmp;
		tmp.swap(out);
	}

	return out;
}

ptree_type& get_vcxproj_ptree_from_buffer(ptree_type& out, const yggr::utf8_string& file_data)
{
	return xml_op::read_xml_from_buffer(out, file_data);
}

ptree_type& get_vcxproj_ptree_from_file(ptree_type& out, const yggr::utf8_string& fname)
{
	return xml_op::read_xml_from_file(out, fname);
}

} // namespace vcproj

#endif // !(BOOST_VERSION < 105600)