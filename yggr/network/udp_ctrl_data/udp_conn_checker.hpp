//udp_conn_checker.hpp

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

#ifndef __YGGR_NETWORK_UDP_CTRL_DATA_UDP_CONN_CHECKER_HPP__
#define __YGGR_NETWORK_UDP_CTRL_DATA_UDP_CONN_CHECKER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/random.h>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/task_center/support/task_import.hpp>

#include <yggr/network/start_data/start_data_def.hpp>
#include <yggr/encryption_tool/md5_tool.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array.hpp>

#include <sstream>

namespace yggr
{
namespace network
{
namespace udp_ctrl_data
{

template<typename Tool = yggr::encryption_tool::md5_tool,
			typename TaskDataInfo = task_center::default_task_data_info_type>
class udp_conn_checker
{
private:
	typedef Tool tool_type;
	typedef TaskDataInfo task_data_info_type;
	typedef typename tool_type::check_type check_type;
	typedef u32 val_type;
	typedef random random_type;

private:
	typedef udp_conn_checker this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// E_pak_id = ::yggr::network::start_data::start_data_def::E_id_end - 1
	// E_now_ver = 1
	YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (::yggr::network::start_data::start_data_def::E_id_end - 1), 1)

private:
	YGGR_PP_CHECK_START_DATA_ID(E_pak_id);

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_val);
		ar & YGGR_SERIALIZATION_NVP(_chk);
	}

public:
	udp_conn_checker(void)
		: _val(val_type())
	{
	}

	udp_conn_checker(int n) // 0 is random
		: _val(n? n : random_type::gen_random<u32>())
	{
		std::stringstream ss;
		ss << _val;
		tool_type tool;
		_chk = tool.encrypt(ss.str());
	}

	udp_conn_checker(BOOST_RV_REF(this_type) right)
		: _val(right._val), _chk(right._chk)
	{
	}

	udp_conn_checker(const this_type& right)
		: _val(right._val), _chk(right._chk)
	{
	}

	~udp_conn_checker(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == &right_ref)
		{
			return *this;
		}

		_val = boost::move(right_ref._val);
		::yggr::copy_or_move_or_swap(_chk, boost::move(right_ref._chk));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}

		_val = right._val;
		_chk = right._chk;
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

		::yggr::swap(_val, right._val);
		::yggr::swap(_chk, right._chk);
	}

public:
	bool check(void) const
	{
		std::stringstream ss;
		ss << _val;
		tool_type tool;
		return tool.encrypt(ss.str()) == _chk;
	}

private:
	val_type _val;
	check_type _chk;
};

} // namespace udp_ctrl_data
} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{
namespace udp_ctrl_data
{
namespace swap_support
{

template<typename Tool, typename TaskDataInfo> inline 
void swap(udp_conn_checker<Tool, TaskDataInfo>& l, udp_conn_checker<Tool, TaskDataInfo>& r) 
{ 
	l.swap(r);
}


} // namespace swap_support

using swap_support::swap;

} // namespace udp_ctrl_data
} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::udp_ctrl_data::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::udp_ctrl_data::swap_support::swap;
} // namespace boost


#endif //__YGGR_NETWORK_UDP_CTRL_DATA_UDP_CONN_CHECKER_HPP__
