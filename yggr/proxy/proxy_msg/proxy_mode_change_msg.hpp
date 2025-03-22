//proxy_mode_change_msg.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_MSG_HPP__
#define __YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_MSG_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/task_center/support/task_import.hpp>

#include <yggr/proxy/proxy_msg/proxy_netdata_def.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/unordered_map.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_msg
{

template<typename Attached_Data = u32,
			typename TaskDataInfo = task_center::default_task_data_info_type>
class proxy_mode_change_msg
{
public:
	typedef Attached_Data attached_data_type;

private:
	typedef TaskDataInfo task_data_info_type;

private:
	typedef proxy_mode_change_msg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// E_pak_id = proxy_netdata_def::E_id_start + 3
	// E_now_ver = 1
	YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (proxy_netdata_def::E_id_start + 3), 1)

private:
	YGGR_PP_CHECK_PROXY_MSG_ID(E_pak_id);

public:
	typedef ::yggr::unordered_map< data_info_type, u32 > chg_map_type;

private:
	typedef typename chg_map_type::value_type ins_val_type;

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_chg_map);
		ar & YGGR_SERIALIZATION_NVP(_att_data);
	}

public:
	proxy_mode_change_msg(void)
		: _att_data()
	{
	}

	proxy_mode_change_msg(const attached_data_type& att_data)
		: _att_data(att_data)
	{
	}

	proxy_mode_change_msg(BOOST_RV_REF(this_type) right)
		: _chg_map(boost::move(right._chg_map)),
			_att_data(boost::move(right._att_data))
	{
	}

	proxy_mode_change_msg(const this_type& right)
		: _chg_map(right._chg_map),
			_att_data(right._att_data)
	{
	}

	~proxy_mode_change_msg(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_chg_map, boost::move(right._chg_map));
		copy_or_move_or_swap(_att_data, boost::move(right._att_data));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_chg_map = right._chg_map;
		_att_data = right._att_data;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_chg_map.swap(right._chg_map);
		yggr::swap(_att_data, right._att_data);
	}

public:
	inline bool add_chg_data(const data_info_type& data_info, u32 mode)
	{
		return _chg_map.insert(ins_val_type(data_info, mode)).second;
	}

	inline bool rm_chg_data(const data_info_type& data_info)
	{
		return _chg_map.erase(data_info);
	}

	inline chg_map_type& chg_map(void)
	{
		return _chg_map;
	}

	inline const chg_map_type& chg_map(void) const
	{
		return _chg_map;
	}

	inline attached_data_type& attached_data(void)
	{
		return _att_data;
	}

	inline const attached_data_type& attached_data(void) const
	{
		return _att_data;
	}

private:
	chg_map_type _chg_map;
	attached_data_type _att_data;
};

} // namespace proxy_msg
} // namespace proxy
} // namespace yggr

namespace yggr
{
namespace proxy
{
namespace proxy_msg
{
namespace swap_support
{

template<typename Attached_Data, typename TaskDataInfo> inline 
void swap(proxy_mode_change_msg<Attached_Data, TaskDataInfo>& l, 
			proxy_mode_change_msg<Attached_Data, TaskDataInfo>& r) 
{ 
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace proxy_msg
} // namespace proxy
} // namespace yggr

namespace std
{
	using ::yggr::proxy::proxy_msg::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::proxy::proxy_msg::swap_support::swap;
} // namespace boost

#endif //__YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_MSG_HPP__
