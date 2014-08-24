//udp_conn_checker.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#include <sstream>

#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>

#include <yggr/network/start_data/start_data_def.hpp>
#include <yggr/encryption_tool/md5_tool.hpp>
#include <yggr/serialization/array.hpp>

#include <yggr/base/random.h>

namespace yggr
{
namespace network
{
namespace udp_ctrl_data
{

template<typename Tool = yggr::encryption_tool::md5_tool,
			typename Task_Data_Info = task_center::default_task_data_info_type>
class udp_conn_checker
{
private:
	typedef Tool tool_type;
	typedef Task_Data_Info task_data_info_type;
	typedef typename tool_type::check_type check_type;
	typedef u32 val_type;
	typedef random random_type;

public:

	enum
	{
		E_pak_id = yggr::network::start_data::start_data_def::E_id_end - 1,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x00000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

private:

	YGGR_PP_CHECK_START_DATA_ID(E_pak_id);
	typedef udp_conn_checker this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_val);
		ar & YGGR_SERIALIZE_NVP(_chk);
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

	bool check(void) const
	{
		std::stringstream ss;
		ss << _val;
		tool_type tool;
		return tool.encrypt(ss.str()) == _chk;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		_val = right._val;
		_chk = right._chk;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_val = right._val;
		_chk = right._chk;
		return *this;
	}

	YGGR_FAST_CAL_TASK_IMPORT(typename task_data_info_type, this_type, E_pak_id, E_now_ver)

private:
	val_type _val;
	check_type _chk;
};

} // namespace udp_ctrl_data
} // namespace network
} // namespace

#endif //__YGGR_NETWORK_UDP_CTRL_DATA_UDP_CONN_CHECKER_HPP__
