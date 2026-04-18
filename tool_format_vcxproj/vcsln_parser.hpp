// vcsln_parser.hpp

#ifndef __VCSLN_PARSER_HPP__
#define __VCSLN_PARSER_HPP__

#include <yggr/base/yggrdef.h>

#if !(BOOST_VERSION < 105600)

#include <yggr/container/map.hpp>
#include <yggr/charset/string.hpp>

namespace vcsln
{

typedef yggr::map<yggr::string, yggr::string> sln_kv_map_type;


bool split_sln(yggr::string& str_global_beg,
				sln_kv_map_type& global_sln_cfg,
				yggr::string& str_global_mid,
				sln_kv_map_type& global_proj_cfg,
				yggr::string& str_global_end,
				const yggr::string& sln_data);

yggr::string& merge_sln(yggr::string& sln_data,
							const yggr::string& str_global_beg,
							const sln_kv_map_type& global_sln_cfg,
							const yggr::string& str_global_mid,
							const sln_kv_map_type& global_proj_cfg,
							const yggr::string& str_global_end);


} // namespace vcsln

#endif // BOOST_VERSION < 105600

#endif // __VCSLN_PARSER_HPP__
