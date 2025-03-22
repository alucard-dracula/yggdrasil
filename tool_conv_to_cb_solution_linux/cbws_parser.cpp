// cbws_parser.cpp

#include "cbws_parser.hpp"

#if !(BOOST_VERSION < 105600)

#include "xml_operator.hpp"

namespace cbws
{


ptree_type& get_cb_workspace_ptree_from_buffer(ptree_type& out, const yggr::utf8_string& file_data)
{
	return xml_op::read_xml_from_buffer(out, file_data);
}

ptree_type& get_cb_workspace_ptree_from_file(ptree_type& out, const yggr::utf8_string& fname)
{
	return xml_op::read_xml_from_file(out, fname);
}

bool write_cb_workspace(const ptree_type& ptree, const yggr::utf8_string& str_file_path)
{
	return xml_op::write_xml_to_file(ptree, str_file_path);
}

} // namespace cbws

#endif // !(BOOST_VERSION < 105600)