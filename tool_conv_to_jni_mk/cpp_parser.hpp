// cpp_parser.hpp

#ifndef __CPP_PARSER_HPP__
#define __CPP_PARSER_HPP__

#include "tool_conv_to_jni_mk_config.hpp"

#if !(BOOST_VERSION < 105600)


#include <yggr/container/set.hpp>

#include "file_operator.hpp"
#include <yggr/iterator_ex/iterator.hpp>
#include <yggr/algorithm/knuth_morris_pratt.hpp>

#include <sstream>
#include <utility>

namespace cpp
{

typedef yggr::set<ptree_string_type> libmark_set_type;
//
//libmark_set_type& get_libmarks_from_code(libmark_set_type& out, const ptree_string_type& code);
//libmark_set_type& get_libmarks_from_code_file(libmark_set_type& out, const ptree_string_type& file_path);

} // namespace cpp

#endif // BOOST_VERSION < 105600

#endif // __CPP_PARSER_HPP__
