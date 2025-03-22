// proj_tags_mapping.cpp

#include "proj_tags_mapping.hpp"

#if !(BOOST_VERSION < 105600)

#include <yggr/mapping/unique_mapping.hpp>

namespace mapping
{

// tags_mapping
const app_tags_mapping_type& init_app_tags_mapping(app_tags_mapping_type& m)
{
	m.map().append(
		vcproj::vcproj_app_tags::s_tag_exe(), 
		cb::cb_app_tags::s_tag_exe());

	m.map().append(
		vcproj::vcproj_app_tags::s_tag_dll(), 
		cb::cb_app_tags::s_tag_dll());

	m.map().append(
		vcproj::vcproj_app_tags::s_tag_static_lib(), 
		cb::cb_app_tags::s_tag_static_lib());

	m.map().append(
		vcproj::vcproj_app_tags::s_tag_utility(), 
		cb::cb_app_tags::s_tag_utility());

	// not used
	//m.map().append(
	//	vcproj::vcproj_app_tags::s_tag_makefile(), 
	//	cb::cb_app_tags::s_tag_makefile());

	return m;
}

const app_tags_mapping_type& get_app_tags_mapping(void)
{
	static app_tags_mapping_type s_mapping;

	return s_mapping.map().empty()? init_app_tags_mapping(s_mapping) : s_mapping;
}

// marks_mapping
const cfg_marks_mapping_type& init_cfg_marks_mapping(cfg_marks_mapping_type& m)
{
	m.map().append(
		vcproj::vcproj_cfg_marks::s_mark_debug_x32(), 
		cb::cb_cfg_marks::s_mark_debug_x32());

	m.map().append(
		vcproj::vcproj_cfg_marks::s_mark_release_x32(), 
		cb::cb_cfg_marks::s_mark_release_x32());

	m.map().append(
		vcproj::vcproj_cfg_marks::s_mark_debug_x64(), 
		cb::cb_cfg_marks::s_mark_debug_x64());

	m.map().append(
		vcproj::vcproj_cfg_marks::s_mark_release_x64(), 
		cb::cb_cfg_marks::s_mark_release_x64());

	return m;
}

const cfg_marks_mapping_type& get_cfg_marks_mapping(void)
{
	static cfg_marks_mapping_type s_mapping;

	return s_mapping.map().empty()? init_cfg_marks_mapping(s_mapping) : s_mapping;
}

} // namespace mapping

#endif // BOOST_VERSION < 105600

