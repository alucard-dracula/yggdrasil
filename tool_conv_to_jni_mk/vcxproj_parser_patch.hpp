// vcxproj_parser_patch.hpp

#ifndef __VCXPROJ_PARSER_PATCH_HPP__
#define __VCXPROJ_PARSER_PATCH_HPP__

#include <yggr/base/yggrdef.h>

#if !(BOOST_VERSION < 105600)

#include <yggr/charset/string.hpp>
#include <boost/property_tree/xml_parser.hpp>

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

yggr::string encode_char_entities(const yggr::string &s);

void write_xml_element(std::basic_ostream<ptree_type::key_type::value_type> &stream,
                        const ptree_type::key_type &key,
                        const ptree_type &pt,
                        int indent,
                        const xml_writer_settings<ptree_type::key_type> & settings);

} // namespace xml_parser
} // property_tree
} // boost

#endif // #if !(BOOST_VERSION < 105600)

#endif // __VCXPROJ_PARSER_PATCH_HPP__
