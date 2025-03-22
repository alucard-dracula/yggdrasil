// cb_parser.hpp

#ifndef __CB_PARSER_HPP__
#define __CB_PARSER_HPP__

#include "tool_conv_to_jni_mk_config.hpp"

#if !(BOOST_VERSION < 105600)

#include "cb_tags_def.hpp"
#include "cb_infos.hpp"
#include <yggr/container/set.hpp>

namespace cb
{

typedef yggr::set<ptree_string_type> string_set_type;

cb_infos& parse_cb_infos(cb_infos& info, const ptree_type& cb_ptree);

ptree_string_type get_cb_app_tag(const ptree_type& cb_ptree);

ptree_type& get_cb_ptree_from_buffer(ptree_type& out, const yggr::utf8_string& file_data);
ptree_type& get_cb_ptree_from_file(ptree_type& out, const yggr::utf8_string& fname);

} // namespace cb

#endif // BOOST_VERSION < 105600

#endif // __CB_PARSER_HPP__
