//proxy_register_back_msg.hpp

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

#ifndef __YGGR_PROXY_PROXY_MSG_PROXY_REGISTER_BACK_MSG_HPP__
#define __YGGR_PROXY_PROXY_MSG_PROXY_REGISTER_BACK_MSG_HPP__

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
class proxy_register_back_msg
{
public:
	typedef Attached_Data attached_data_type;

private:
	typedef TaskDataInfo task_data_info_type;

private:
	typedef proxy_register_back_msg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)


public:
	// E_pak_id = proxy_netdata_def::E_id_start + 2
	// E_now_ver = 1
	YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (proxy_netdata_def::E_id_start + 2), 1)

private:
	YGGR_PP_CHECK_PROXY_MSG_ID(E_pak_id);

public:
	typedef std::pair<u32, u32> reg_back_info_type;
	typedef ::yggr::unordered_map< data_info_type, reg_back_info_type > reg_back_map_type;

private:
	typedef typename reg_back_map_type::value_type ins_val_type;

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_reg_back_map);
		ar & YGGR_SERIALIZATION_NVP(_att_data);
	}

public:
	proxy_register_back_msg(void)
		: _att_data()
	{
	}

	proxy_register_back_msg(const attached_data_type& att_data)
		: _att_data(att_data)
	{
	}

	proxy_register_back_msg(BOOST_RV_REF(this_type) right)
		: _reg_back_map(boost::move(right._reg_back_map)),
			_att_data(boost::move(right._att_data))
	{
	}

	proxy_register_back_msg(const this_type& right)
		: _reg_back_map(right._reg_back_map),
			_att_data(right._att_data)
	{
	}

	~proxy_register_back_msg(void)
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

		copy_or_move_or_swap(_reg_back_map, boost::move(right._reg_back_map));
		copy_or_move_or_swap(_att_data, boost::move(right._att_data));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}

		_reg_back_map = right._reg_back_map;
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

		_reg_back_map.swap(right._reg_back_map);
		yggr::swap(_att_data, right._att_data);
	}

public:
	void set_item_state(u32 state)
	{
		typedef typename reg_back_map_type::iterator iter_type;
		for(iter_type i = _reg_back_map.begin(), isize = _reg_back_map.end();
				i != isize; ++i)
		{
			i->second.second = state;
		}
	}

	inline bool set_item_state(const data_info_type& data_info, u32 state)
	{
		typedef typename reg_back_map_type::iterator iter_type;

		iter_type i = _reg_back_map.find(data_info);
		return 
			(i != _reg_back_map.end()) 
			&& (i->second.second = state, true);
	}

	inline void clear(void)
	{
		yggr::clear(_reg_back_map);
		yggr::clear(_att_data);
	}

	inline bool add_reg_back_data(const data_info_type& data_info, u32 mode, u32 state)
	{
		return _reg_back_map.insert(ins_val_type(data_info, reg_back_info_type(mode, state))).second;
	}

	inline bool add_reg_back_data(const data_info_type& data_info, const reg_back_info_type& reg_back_info)
	{
		return _reg_back_map.insert(ins_val_type(data_info, reg_back_info)).second;
	}

	inline bool rm_reg_back_data(const data_info_type& data_info)
	{
		return _reg_back_map.erase(data_info);
	}

	inline reg_back_map_type& reg_back_map(void)
	{
		return _reg_back_map;
	}

	inline const reg_back_map_type& reg_back_map(void) const
	{
		return _reg_back_map;
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
	reg_back_map_type _reg_back_map;
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
void swap(proxy_register_back_msg<Attached_Data, TaskDataInfo>& l, 
			proxy_register_back_msg<Attached_Data, TaskDataInfo>& r)
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

#endif //__YGGR_PROXY_PROXY_MSG_PROXY_REGISTER_BACK_MSG_HPP__
