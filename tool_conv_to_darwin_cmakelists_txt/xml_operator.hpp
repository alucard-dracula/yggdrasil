// xml_operator.hpp

#ifndef __XML_OPERATOR_HPP__
#define __XML_OPERATOR_HPP__

#include "tool_conv_to_darwin_cmakelists_txt_config.hpp"

#if !(BOOST_VERSION < 105600)

#include <yggr/base/static_constant.hpp>

namespace xml_op
{

struct fix_result
{
	YGGR_STATIC_CONSTANT(yggr::u32, E_SUCCEED = 0);
	YGGR_STATIC_CONSTANT(yggr::u32, E_FAILED = 1);
	YGGR_STATIC_CONSTANT(yggr::u32, E_IGNORE = 2);
};

bool has_attribs(const ptree_type& ptree, const ptree_string_type& key);
ptree_string_type get_attribs(const ptree_type& ptree, const ptree_string_type& key);
//bool set_attribs(ptree_type& ptree, const ptree_string_type& key, const ptree_string_type& val);
//bool set_attribs_force(ptree_type& ptree, const ptree_string_type& key, const ptree_string_type& val);
//bool set_attribs_force(ptree_type& ptree, const ptree_string_type& iter_key, 
//						const ptree_string_type& key, const ptree_string_type& val);

bool print_ptree(const ptree_type& ptree);

ptree_type& read_xml_from_buffer(ptree_type& out, const yggr::utf8_string& file_data);
ptree_type& read_xml_from_file(ptree_type& out, const yggr::utf8_string& fname);

bool write_xml_to_file(const ptree_type& out, const yggr::utf8_string& fname);

}// namespace file_op

#endif // BOOST_VERSION < 105600

#endif // __XML_OPERATOR_HPP__
