// vcxproj_parser_patch.hpp

#ifndef __VCXPROJ_PARSER_PATCH_HPP__
#define __VCXPROJ_PARSER_PATCH_HPP__

#include <yggr/base/yggrdef.h>

#if !(BOOST_VERSION < 105600)

#include <boost/property_tree/xml_parser.hpp>
#include <yggr/charset/string.hpp>

typedef boost::property_tree::basic_ptree<yggr::string, yggr::string> ptree_type;
typedef yggr::string ptree_string_type;

namespace boost 
{ 
namespace property_tree 
{

template <typename Ch, typename Traits, typename Alloc>
struct path_of< yggr::basic_string<Ch, Traits, Alloc> >
{
    typedef yggr::basic_string<Ch, Traits, Alloc> _string;
    typedef string_path< _string, id_translator<_string> > type;
};

namespace xml_parser
{

yggr::string encode_char_entities(const yggr::string &s)
{
	typedef yggr::string Str;
	typedef yggr::string::value_type Ch;
    // Don't do anything for empty strings.
    if(s.empty()) return s;

    Str r;
    // To properly round-trip spaces and not uglify the XML beyond
    // recognition, we have to encode them IF the text contains only spaces.
    Str sp(1, Ch(' '));
    if(s.find_first_not_of(sp) == Str::npos) {
        // The first will suffice.
		r = detail::widen<Str>("&#32;");
        r += Str(s.size() - 1, Ch(' '));
    } else {
        Str::const_iterator end = s.end();
        for (Str::const_iterator it = s.begin(); it != end; ++it)
        {
            //switch (*it)
            //{
            //    case Ch('<'): r += detail::widen<Str>("&lt;"); break;
            //    case Ch('>'): r += detail::widen<Str>("&gt;"); break;
            //    case Ch('&'): r += detail::widen<Str>("&amp;"); break;
            //    case Ch('"'): r += detail::widen<Str>("&quot;"); break;
            //    case Ch('\''): r += detail::widen<Str>("&apos;"); break;
            //    default: r += *it; break;
            //}

			r += *it;
        }
    }
    return r;
}

void write_xml_element(std::basic_ostream<ptree_type::key_type::value_type> &stream,
                        const ptree_type::key_type &key,
                        const ptree_type &pt,
                        int indent,
                        const xml_writer_settings<ptree_type::key_type> & settings)
{
	typedef ptree_type Ptree;
	typedef Ptree::key_type key_type;
    typedef Ptree::key_type::value_type Ch;
    typedef Ptree::key_type Str;
    typedef Ptree::const_iterator It;

    bool want_pretty = settings.indent_count > 0;
    // Find if elements present
    bool has_elements = false;
    bool has_attrs_only = pt.data().empty();
	bool is_group = (key != "PropertyGroup") && (key.find("Group") != key_type::npos);

    for (It it = pt.begin(), end = pt.end(); it != end; ++it)
    {
		if(it->first != xmlattr<Str>() )
        {
            has_attrs_only = false;
			if(it->first != xmltext<Str>())
            {
                has_elements = true;
                break;
            }
        }
    }

    // Write element
	if(pt.data().empty() && pt.empty())    // Empty key
    {
		if(indent >= 0)
		{
			if(!is_group)
			{
				write_xml_indent(stream,indent,settings);
				stream << Ch('<') << key << Ch('>');
				if(want_pretty) stream << Ch('\n');
				write_xml_indent(stream,indent,settings);
				stream << Ch('<') << Ch('/') << key << Ch('>');
			}
			else
			{
				write_xml_indent(stream,indent,settings);
				stream << Ch('<') << key << Ch('/') << Ch('>');	
			}

			if(want_pretty) stream << Ch('\n');
		}
    }
    else    // Nonempty key
    {
        // Write opening tag, attributes and data
        if(indent >= 0)
        {
            // Write opening brace and key
            write_xml_indent(stream,indent,settings);
            stream << Ch('<') << key;

            // Write attributes
            if(optional<const Ptree &> attribs = pt.get_child_optional(xmlattr<Str>()))
			{
                for (It it = attribs.get().begin(); it != attribs.get().end(); ++it)
				{
                    stream << Ch(' ') << it->first << Ch('=')
                            << Ch('"')
                            << encode_char_entities(
								it->second.template get_value<Str>())
                            << Ch('"');
				}
			}

            if( has_attrs_only && !is_group)
            {
                // Write closing brace
                stream << Ch('/') << Ch('>');
                if(want_pretty)
                    stream << Ch('\n');
            }
            else
            {
				has_attrs_only = false;
                // Write closing brace
                stream << Ch('>');

                // Break line if needed and if we want pretty-printing
                if(is_group || (has_elements && want_pretty))
                    stream << Ch('\n');
            }
        }

        // Write data text, if present
        if(!pt.data().empty())
            write_xml_text(stream,
                pt.template get_value<Str>(),
                indent + 1, has_elements && want_pretty, settings);

        // Write elements, comments and texts
        for (It it = pt.begin(); it != pt.end(); ++it)
        {
            if(it->first == xmlattr<Str>())
                continue;
            else if(it->first == xmlcomment<Str>())
                write_xml_comment(stream,
                    it->second.template get_value<Str>(),
                    indent + 1, want_pretty, settings);
            else if(it->first == xmltext<Str>())
                write_xml_text(stream,
                    it->second.template get_value<Str>(),
                    indent + 1, has_elements && want_pretty, settings);
            else
                write_xml_element(stream, it->first, it->second,
                    indent + 1, settings);
        }

        // Write closing tag
        if(indent >= 0 && !has_attrs_only)
        {
            if(has_elements)
                write_xml_indent(stream,indent,settings);
            stream << Ch('<') << Ch('/') << key << Ch('>');
            if(want_pretty)
                stream << Ch('\n');
        }
    }
}

} // namespace xml_parser
} // property_tree
} // boost

#endif // #if !(BOOST_VERSION < 105600)

#endif // __VCXPROJ_PARSER_PATCH_HPP__
