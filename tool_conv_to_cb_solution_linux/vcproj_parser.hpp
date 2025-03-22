// vcproj_parser.hpp

#ifndef __VCPROJ_PARSER_HPP__
#define __VCPROJ_PARSER_HPP__

#include "tool_conv_to_cb_solution_config.hpp"

#if !(BOOST_VERSION < 105600)

#include "vcproj_tags_def.hpp"

#include <yggr/container/set.hpp>

namespace vcproj
{

typedef yggr::set<ptree_string_type> code_file_set_type;
typedef yggr::set<ptree_string_type> string_set_type;

bool is_cl_fix_object(const ptree_string_type& app_tag);

string_set_type& parse_opt_items(string_set_type& out, const ptree_string_type& str_opt_items);

//get items
ptree_string_type get_root_namespace(const ptree_type& ptree);

ptree_string_type get_app_tag(const ptree_type& ptree);

ptree_string_type get_outdir(const ptree_type& ptree,
								const ptree_string_type& app_tag,
								const ptree_string_type& cfg_mark);

ptree_string_type get_target_name(const ptree_type& ptree,
										const ptree_string_type& app_tag,
										const ptree_string_type& cfg_mark);

ptree_string_type get_target_ext(const ptree_type& ptree,
										const ptree_string_type& app_tag,
										const ptree_string_type& cfg_mark);

void get_outdir_and_target_name_and_target_ext(ptree_string_type& str_outdir,
												ptree_string_type& str_target_name,
												ptree_string_type& str_target_ext,
												const ptree_type& ptree,
												const ptree_string_type& app_tag,
												const ptree_string_type& cfg_mark);

ptree_string_type get_language_standard(const ptree_type& ptree,
										const ptree_string_type& app_tag,
										const ptree_string_type& cfg_mark);

ptree_string_type get_subsystem(const ptree_type& ptree,
								const ptree_string_type& app_tag,
								const ptree_string_type& cfg_mark);

ptree_string_type get_impl_lib(const ptree_type& ptree,
								const ptree_string_type& app_tag,
								const ptree_string_type& cfg_mark);

ptree_string_type get_module_definition_file(const ptree_type& ptree,
												const ptree_string_type& app_tag,
												const ptree_string_type& cfg_mark);


ptree_string_type get_preprocessor_definitions(const ptree_type& ptree,
												const ptree_string_type& app_tag,
												const ptree_string_type& cfg_mark);

ptree_string_type get_additional_dependencies(const ptree_type& ptree,
												const ptree_string_type& app_tag,
												const ptree_string_type& cfg_mark);

ptree_string_type get_additional_library_directories(const ptree_type& ptree,
														const ptree_string_type& app_tag,
														const ptree_string_type& cfg_mark);
ptree_string_type get_additional_include_directories(const ptree_type& ptree,
														const ptree_string_type& app_tag,
														const ptree_string_type& cfg_mark);

ptree_string_type get_additional_options(const ptree_type& ptree,
											const ptree_string_type& app_tag,
											const ptree_string_type& cfg_mark);


//bool has_big_obj(const ptree_type& ptree);
bool has_big_obj(const ptree_type& ptree,
					const ptree_string_type& app_tag,
					const ptree_string_type& cfg_mark);


code_file_set_type& get_code_files(code_file_set_type& out, const ptree_type& ptree);

ptree_type& get_vcxproj_ptree_from_buffer(ptree_type& out, const yggr::utf8_string& file_data);
ptree_type& get_vcxproj_ptree_from_file(ptree_type& out, const yggr::utf8_string& fname);

} // namespace vcproj

#endif // BOOST_VERSION < 105600

#endif // __VCPROJ_PARSER_HPP__
