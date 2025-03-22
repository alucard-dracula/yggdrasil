// cb_infos.hpp

#ifndef __CB_INFOS_HPP__
#define __CB_INFOS_HPP__

#include "tool_conv_to_darwin_cmakelists_txt_config.hpp"

#if !(BOOST_VERSION < 105600)

#include <yggr/container/vector.hpp>
#include <yggr/container/set.hpp>

namespace cb
{

struct cb_infos
{

public:
	typedef ptree_string_type string_type;
	typedef yggr::vector<string_type> string_vt_type;
	typedef yggr::set<string_type> string_set_type;

public:
	inline void failed(void)
	{
		proj_t_.clear();
	}

	inline bool validate(void) const
	{
		return 
			proj_t_.size() 
			&& proj_name_.size()
			&& proj_output_debug_.size()
			&& proj_output_release_.size();
	}
public:
	string_type proj_t_;
	string_type proj_name_;
	string_type proj_cur_dir_;

	string_type proj_output_debug_;
	string_type proj_output_release_;

	string_set_type src_files_;

	string_set_type cinclude_;

	string_set_type cdefined_proj_;
	string_set_type cdefined_proj_debug_;
	string_set_type cdefined_proj_release_;

	string_set_type cflag_proj_;
	string_set_type cflag_proj_debug_;
	string_set_type cflag_proj_release_;

	string_set_type cppflag_proj_;
	string_set_type cppflag_proj_debug_;
	string_set_type cppflag_proj_release_;

	string_set_type link_libs_path_;

	string_vt_type link_libs_proj_;
	string_vt_type link_libs_proj_debug_;
	string_vt_type link_libs_proj_release_;

	string_set_type ldflag_proj_;
	string_set_type ldflag_proj_debug_;
	string_set_type ldflag_proj_release_;
};

} // namespace cb

#endif // BOOST_VERSION < 105600

#endif // __CB_INFOS_HPP__
