// vcsln_parser.hpp

#ifndef __VCSLN_PARSER_HPP__
#define __VCSLN_PARSER_HPP__

#include "tool_conv_to_cb_solution_config.hpp"

#if !(BOOST_VERSION < 105600)

#include "vcproj_tags_def.hpp"
#include <yggr/container/map.hpp>

namespace vcsln
{

typedef yggr::map<yggr::utf8_string, yggr::utf8_string> vcxproj_map_type;

vcxproj_map_type& get_vcxproj_map_from_buffer(vcxproj_map_type& out, const yggr::utf8_string& file_data);
vcxproj_map_type& get_vcxproj_map_from_file(vcxproj_map_type& out, const yggr::utf8_string& fname);

} // namespace vcsln

#endif // BOOST_VERSION < 105600

#endif // __VCSLN_PARSER_HPP__
