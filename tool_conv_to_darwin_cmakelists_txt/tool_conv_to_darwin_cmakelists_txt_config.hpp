// tool_conv_to_darwin_cmakelists_txt_config.hpp

#ifndef __TOOL_CONV_TO_DARWIN_CMAKELISTS_TXT_CONFIG_HPP__
#define __TOOL_CONV_TO_DARWIN_CMAKELISTS_TXT_CONFIG_HPP__

#include <yggr/base/yggrdef.h>

#define ONLY_MAKE_RUN_TEST() 0

#if BOOST_VERSION < 105600

//#	error this tool not support boost 1.55 and older

#	else

#include <yggr/charset/utf8_string.hpp>
#include <yggr/charset/utf8_string_view.hpp>
#include <yggr/file_system/local_fsys_operators.hpp>
#include <yggr/file_system/boost_filesystem_comp.hpp>
#include "vcxproj_parser_patch.hpp"

#include <yggr/archive/xml_archive_partner.hpp>
#include <yggr/packet/packet.hpp>

typedef yggr::file_system::local_fsys_operators local_fsys_operators_type;
typedef local_fsys_operators_type::directory_entry_type fsys_directory_entry_type;
typedef local_fsys_operators_type::path_type fsys_path_type;

#endif // BOOST_VERSION < 105600

#endif // __TOOL_CONV_TO_DARWIN_CMAKELISTS_TXT_CONFIG_HPP__
