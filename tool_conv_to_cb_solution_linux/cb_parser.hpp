// cb_parser.hpp

#ifndef __CB_PARSER_HPP__
#define __CB_PARSER_HPP__

#include "tool_conv_to_cb_solution_config.hpp"

#if !(BOOST_VERSION < 105600)

#include "cb_tags_def.hpp"
#include "lib_depend_info.hpp"
#include <yggr/container/set.hpp>

namespace cb
{

typedef yggr::set<ptree_string_type> string_set_type;

yggr::u32 make_cb_ptree_from_vc_tree(ptree_type& inout, 
										const ptree_type& vc_ptree, 
										const yggr::utf8_string& proj_dir, 
										const lib::lib_depend_info& dep_info_cfg);

ptree_type& get_cb_ptree_from_buffer(ptree_type& out, const yggr::utf8_string& file_data);
ptree_type& get_cb_ptree_from_file(ptree_type& out, const yggr::utf8_string& fname);

bool write_cbp(const ptree_type& ptree, const yggr::utf8_string& str_file_path);


} // namespace cb

#endif // BOOST_VERSION < 105600

#endif // __CB_PARSER_HPP__
