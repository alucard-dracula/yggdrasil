// cb_parser.hpp

#ifndef __CBWS_PARSER_HPP__
#define __CBWS_PARSER_HPP__

#include "tool_conv_to_cb_solution_config.hpp"

#if !(BOOST_VERSION < 105600)

#include <yggr/container/map.hpp>

namespace cbws
{

typedef yggr::map<yggr::utf8_string, yggr::utf8_string> cbp_map_type;


ptree_type& get_cb_workspace_ptree_from_buffer(ptree_type& out, const yggr::utf8_string& file_data);
ptree_type& get_cb_workspace_ptree_from_file(ptree_type& out, const yggr::utf8_string& fname);

bool write_cb_workspace(const ptree_type& ptree, const yggr::utf8_string& str_file_path);

} // namespace cbws

#endif // BOOST_VERSION < 105600

#endif // __CBWS_PARSER_HPP__
