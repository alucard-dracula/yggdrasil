// cbp_sample_cfg.cpp

#include "cbp_sample_cfg.hpp"

#if !(BOOST_VERSION < 105600)

#include "cb_tags_def.hpp"
#include "vcproj_tags_def.hpp"

cbp_sample_cfg::cbp_sample_cfg(void)
{
}

cbp_sample_cfg::cbp_sample_cfg(cbp_sample_cfg_sample)
	: file_tag_("_mingw")
{
	sample_file_map_["sln"] = "cb_sample_file/sample_workspace.workspace";
	sample_file_map_["workspace"] = "cb_sample_file/sample_workspace.workspace";

	sample_file_map_[vcproj::vcproj_app_tags::s_tag_exe()] = "cb_sample_file/sample_exe.cbp";
	sample_file_map_[cb::cb_app_tags::s_tag_exe()] = "cb_sample_file/sample_exe.cbp";

	sample_file_map_[vcproj::vcproj_app_tags::s_tag_dll()] = "cb_sample_file/sample_dll.cbp";
	sample_file_map_[cb::cb_app_tags::s_tag_dll()] = "cb_sample_file/sample_dll.cbp";

	sample_file_map_[vcproj::vcproj_app_tags::s_tag_static_lib()] = "cb_sample_file/sample_static_lib.cbp";
	sample_file_map_[cb::cb_app_tags::s_tag_static_lib()] = "cb_sample_file/sample_static_lib.cbp";

	sample_file_map_[vcproj::vcproj_app_tags::s_tag_utility()] = "cb_sample_file/sample_utility.cbp";
	sample_file_map_[cb::cb_app_tags::s_tag_utility()] = "cb_sample_file/sample_utility.cbp";

	//sample_file_map_[vcproj::vcproj_app_tags::s_tag_makefile()] = "./cb_sample_file/sample_utility.cbp";
}

cbp_sample_cfg::cbp_sample_cfg(const this_type& right)
	: file_tag_(right.file_tag_),
		sample_file_map_(boost::move(right.sample_file_map_))
{
}

cbp_sample_cfg::~cbp_sample_cfg(void)
{
}

//public:

cbp_sample_cfg::this_type& cbp_sample_cfg::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	file_tag_ = right.file_tag_;
	sample_file_map_ = right.sample_file_map_;

	return *this;
}

//public:
void cbp_sample_cfg::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	file_tag_.swap(right.file_tag_);
	sample_file_map_.swap(right.sample_file_map_);
}

#endif // #if !(BOOST_VERSION < 105600)
