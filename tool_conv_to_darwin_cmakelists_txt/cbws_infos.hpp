// cbws_infos.hpp

#ifndef __CBWS_INFOS_HPP__
#define __CBWS_INFOS_HPP__

#include "tool_conv_to_darwin_cmakelists_txt_config.hpp"

#if !(BOOST_VERSION < 105600)

#include <yggr/container/vector.hpp>

namespace cb
{

struct cbws_infos
{

public:
	typedef ptree_string_type string_type;
	typedef yggr::vector<string_type> string_vt_type;

public:
	string_type cbws_title_;
	string_vt_type cb_file_list_;
};

} // namespace cb

#endif // BOOST_VERSION < 105600

#endif // __CBWS_INFOS_HPP__
