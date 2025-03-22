// xml_operator.hpp

#include "xml_operator.hpp"

#if !(BOOST_VERSION < 105600)

namespace xml_op
{


bool has_attribs(const ptree_type& ptree, const ptree_string_type& key)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	attribs_type attribs =
		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

	if(!attribs)
	{
		return false;
	}

	const ptree_type& sub_ptree_ref = attribs.get();
	for(const_iterator i = sub_ptree_ref.begin(), isize = sub_ptree_ref.end(); i != isize; ++i)
	{
		if(i->first == key)
		{
			return true;
		}
	}

	return false;
}

ptree_string_type get_attribs(const ptree_type& ptree, const ptree_string_type& key)
{
	typedef ptree_string_type string_type;
	typedef ptree_type::assoc_iterator assoc_iter_type;
	typedef ptree_type::const_assoc_iterator assoc_citer_type;
	typedef ptree_type::iterator iterator;
	typedef ptree_type::const_iterator const_iterator;
	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
	typedef boost::optional<const ptree_type&> attribs_type;

	attribs_type attribs =
		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());

	if(!attribs)
	{
		return string_type();
	}

	const ptree_type& sub_ptree_ref = attribs.get();

	for(const_iterator i = sub_ptree_ref.begin(), isize = sub_ptree_ref.end(); i != isize; ++i)
	{
		if(i->first == key)
		{
			return i->second.get_value<string_type>();
		}
	}

	return string_type();
}

//bool set_attribs(ptree_type& ptree, const ptree_string_type& key, const ptree_string_type& val)
//{
//	typedef ptree_string_type string_type;
//	typedef ptree_type::assoc_iterator assoc_iter_type;
//	typedef ptree_type::const_assoc_iterator assoc_citer_type;
//	typedef ptree_type::iterator iterator;
//	typedef ptree_type::const_iterator const_iterator;
//	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
//	typedef boost::optional<ptree_type&> attribs_type;
//
//	attribs_type attribs =
//		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());
//
//	if(!attribs)
//	{
//		return false;
//	}
//
//	ptree_type& sub_ptree_ref = attribs.get();
//
//	for(iterator i = sub_ptree_ref.begin(), isize = sub_ptree_ref.end(); i != isize; ++i)
//	{
//		if(i->first == key)
//		{
//			i->second.put_value(val);
//			return true;
//		}
//	}
//
//	return false;
//}
//
//bool set_attribs_force(ptree_type& ptree, const ptree_string_type& key, const ptree_string_type& val)
//{
//	typedef ptree_string_type string_type;
//	typedef ptree_type::assoc_iterator assoc_iter_type;
//	typedef ptree_type::const_assoc_iterator assoc_citer_type;
//	typedef ptree_type::iterator iterator;
//	typedef ptree_type::const_iterator const_iterator;
//	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
//	typedef boost::optional<ptree_type&> attribs_type;
//
//	attribs_type attribs =
//		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());
//
//	if(!attribs)
//	{
//		ptree.add_child("<xmlattr>." + key, ptree_type(val));
//		return true;
//	}
//
//	ptree_type& sub_ptree_ref = attribs.get();
//
//	for(iterator i = sub_ptree_ref.begin(), isize = sub_ptree_ref.end(); i != isize; ++i)
//	{
//		if(i->first == key)
//		{
//			i->second.put_value(val);
//			return true;
//		}
//	}
//
//	ptree.add_child("<xmlattr>." + key, ptree_type(val));
//	return true;
//}
//
//bool set_attribs_force(ptree_type& ptree, const ptree_string_type& hint_key,
//						const ptree_string_type& key, const ptree_string_type& val)
//{
//	typedef ptree_string_type string_type;
//	typedef ptree_type::value_type ptree_val_type;
//	typedef ptree_type::assoc_iterator assoc_iter_type;
//	typedef ptree_type::const_assoc_iterator assoc_citer_type;
//	typedef ptree_type::iterator iterator;
//	typedef ptree_type::const_iterator const_iterator;
//	typedef boost::property_tree::xml_writer_settings<string_type> xml_writer_settings_type;
//	typedef boost::optional<ptree_type&> attribs_type;
//
//	attribs_type attribs =
//		ptree.get_child_optional(boost::property_tree::xml_parser::xmlattr<string_type>());
//
//	if(!attribs)
//	{
//		ptree.add_child("<xmlattr>." + key, ptree_type(val));
//		return true;
//	}
//
//	ptree_type& sub_ptree_ref = attribs.get();
//
//	iterator hint_iter = sub_ptree_ref.end();
//	for(iterator i = sub_ptree_ref.begin(), isize = sub_ptree_ref.end(); i != isize; ++i)
//	{
//		if(i->first == hint_key)
//		{
//			hint_iter = i;
//		}
//
//		if(i->first == key)
//		{
//			i->second.put_value(val);
//			return true;
//		}
//	}
//
//	if(hint_iter == sub_ptree_ref.end())
//	{
//		ptree.add_child("<xmlattr>." + key, ptree_type(val));
//	}
//	else
//	{
//		ptree.insert(hint_iter, ptree_val_type(key, ptree_type(val)));
//	}
//
//	return true;
//}

bool print_ptree(const ptree_type& ptree)
{
	typedef boost::property_tree::xml_writer_settings<ptree_type::key_type> xml_writer_settings_type;

	xml_writer_settings_type setting('\t', 1);

	try
	{
		boost::property_tree::write_xml(std::cout, ptree, setting);
	}
	catch(...)
	{
		return false;
	}

	return true;
}

ptree_type& read_xml_from_buffer(ptree_type& out, const yggr::utf8_string& file_data)
{
	std::string str_data = file_data.str<std::string>();

	std::stringstream ss(str_data);

	try
	{
		boost::property_tree::read_xml(ss, out, 4);
	}
	catch(...)
	{
		out.clear();
	}

	return out;
}

ptree_type& read_xml_from_file(ptree_type& out, const yggr::utf8_string& fname)
{
	try
	{
		boost::property_tree::read_xml(fname.c_str(), out, 4);
	}
	catch(...)
	{
		out.clear();
	}

	return out;
}

bool write_xml_to_file(const ptree_type& ptree, const yggr::utf8_string& fname)
{
	typedef boost::property_tree::xml_writer_settings<ptree_string_type> xml_writer_settings_type;

#if ONLY_MAKE_RUN_TEST()

	return print_ptree(ptree);

#else
	xml_writer_settings_type setting('\t', 1);

	try
	{
		boost::property_tree::write_xml(fname.c_str(), ptree, std::locale(), setting);
	}
	catch(...)
	{
		return false;
	}

	return true;

#endif // ONLY_MAKE_RUN_TEST()
}

}// namespace xml_op

#endif // BOOST_VERSION < 105600

