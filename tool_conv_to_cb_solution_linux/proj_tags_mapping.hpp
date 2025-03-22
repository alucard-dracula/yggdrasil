// proj_tags_mapping.hpp

#ifndef __PROJ_TAGS_MAPPING_DEF_HPP__
#define __PROJ_TAGS_MAPPING_DEF_HPP__

#include "tool_conv_to_cb_solution_config.hpp"

#if !(BOOST_VERSION < 105600)

#include <yggr/mapping/unique_mapping.hpp>
#include "cb_tags_def.hpp"
#include "vcproj_tags_def.hpp"

namespace mapping
{

typedef yggr::mapping::unique_mapping<ptree_string_type, yggr::u32> tags_mapping_data_type;
typedef yggr::mapping::unique_mapping<ptree_string_type, ptree_string_type> cfg_marks_mapping_data_type;


template<typename VCKey, typename CBKey>
struct vccb_mapping
{
public:
	typedef VCKey vc_key_type;
	typedef CBKey cb_key_type;
	typedef yggr::mapping::unique_mapping<vc_key_type, cb_key_type> mapping_type;

public:
	vccb_mapping(void)
	{
	}

	~vccb_mapping(void)
	{
	}

public:
	inline std::pair<cb_key_type, bool> vc_to_cb(const vc_key_type& v) const
	{
		return _s_map.template mapping_value<0, 1>(v);
	}

	inline std::pair<vc_key_type, bool> cb_to_vc(const cb_key_type& v) const
	{
		return _s_map.template mapping_value<1, 0>(v);
	}

public:
	inline mapping_type& map(void)
	{
		return _s_map;
	}

	inline const mapping_type& map(void) const
	{
		return _s_map;
	}

private:
	mapping_type _s_map;
};

typedef vccb_mapping<ptree_string_type, ptree_string_type> app_tags_mapping_type;
typedef vccb_mapping<ptree_string_type, ptree_string_type> cfg_marks_mapping_type;

const app_tags_mapping_type& get_app_tags_mapping(void);
const cfg_marks_mapping_type& get_cfg_marks_mapping(void);

} // namespace mapping

#endif // BOOST_VERSION < 105600

#endif // __PROJ_TAGS_MAPPING_DEF_HPP__
