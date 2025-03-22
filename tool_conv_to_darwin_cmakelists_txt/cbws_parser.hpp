// cb_parser.hpp

#ifndef __CBWS_PARSER_HPP__
#define __CBWS_PARSER_HPP__

#include "tool_conv_to_darwin_cmakelists_txt_config.hpp"

#if !(BOOST_VERSION < 105600)

#include "cbws_infos.hpp"

#include <yggr/container/map.hpp>

namespace cb
{

typedef yggr::map<yggr::utf8_string, yggr::utf8_string> cbp_map_type;

cbws_infos& parse_cb_workspace_infos(cbws_infos& infos, const ptree_type& cb_ptree);

ptree_type& get_cb_workspace_ptree_from_buffer(ptree_type& out, const yggr::utf8_string& file_data);
ptree_type& get_cb_workspace_ptree_from_file(ptree_type& out, const yggr::utf8_string& fname);


} // namespace cb

#endif // BOOST_VERSION < 105600

#endif // __CBWS_PARSER_HPP__
