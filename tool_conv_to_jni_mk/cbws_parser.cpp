// cbws_parser.cpp

#include "cbws_parser.hpp"

#if !(BOOST_VERSION < 105600)

#include "xml_operator.hpp"

namespace cb
{

cbws_infos& parse_cb_workspace_infos(cbws_infos& infos, const ptree_type& cb_ptree)
{
	typedef ptree_type::key_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	assoc_citer_type assoc_iter = cb_ptree.find("CodeBlocks_workspace_file");
	assert(assoc_iter != cb_ptree.not_found());
	if(assoc_iter == cb_ptree.not_found())
	{
		return infos;
	}

	assoc_iter = assoc_iter->second.find("Workspace");
	assert(assoc_iter != assoc_iter->second.not_found());
	if(assoc_iter == assoc_iter->second.not_found())
	{
		return infos;
	}

	{
		ptree_string_type str_title = xml_op::get_attribs(assoc_iter->second, "title");
		infos.cbws_title_ = 
			str_title.size() > 6/*_linux*/?
				str_title.substr(0, str_title.size() - 6)
				: str_title;
	}

	for(const_iterator i = assoc_iter->second.begin(), isize = assoc_iter->second.end();
			i != isize; ++i)
	{
		if(i->first == "Project")
		{
			if(xml_op::has_attribs(i->second, "filename"))
			{
				string_type fname = xml_op::get_attribs(i->second, "filename");

				if(fname.size())
				{
					infos.cb_file_list_.push_back(fname);
				}
			}
		}
	}

	return infos;
}

ptree_type& get_cb_workspace_ptree_from_buffer(ptree_type& out, const yggr::utf8_string& file_data)
{
	return xml_op::read_xml_from_buffer(out, file_data);
}

ptree_type& get_cb_workspace_ptree_from_file(ptree_type& out, const yggr::utf8_string& fname)
{
	return xml_op::read_xml_from_file(out, fname);
}

} // namespace cb

#endif // !(BOOST_VERSION < 105600)