// tool_suo_batch_build_selector_cfg.cpp

#include "tool_suo_batch_build_selector_cfg.hpp"

#ifdef _MSC_VER
#	include <vld.h>
#endif //_MSC_VER

tool_suo_batch_build_selector_cfg::tool_suo_batch_build_selector_cfg(void)
{
	memset(boost::addressof(batch_mark_[0]), 0, batch_mark_.size());
}

tool_suo_batch_build_selector_cfg::tool_suo_batch_build_selector_cfg(tool_suo_batch_build_selector_cfg_sample)
	: file_suo_("./.suo")
{
	memset(boost::addressof(batch_mark_[0]), 0x01, batch_mark_.size());
}

tool_suo_batch_build_selector_cfg::tool_suo_batch_build_selector_cfg(const this_type& right)
	: file_suo_(right.file_suo_),
		batch_mark_(right.batch_mark_)
{
}

tool_suo_batch_build_selector_cfg::~tool_suo_batch_build_selector_cfg(void)
{
}

tool_suo_batch_build_selector_cfg::this_type& 
	tool_suo_batch_build_selector_cfg::operator=(const this_type& right)
{
	if(this == &right)
	{
		return *this;
	}

	file_suo_ = right.file_suo_;
	batch_mark_ = right.batch_mark_;

	return *this;
}

void tool_suo_batch_build_selector_cfg::swap(this_type& right)
{
	if(this == &right)
	{
		return;
	}

	yggr::swap(file_suo_, right.file_suo_);
	yggr::swap(batch_mark_, right.batch_mark_);
}

namespace swap_support
{
	YGGR_PP_SWAP_THIS_REF_DEF_IMPL(tool_suo_batch_build_selector_cfg)
} // namespace swap_support
