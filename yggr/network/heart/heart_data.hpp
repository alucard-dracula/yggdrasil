//normal_pak_back_id.hpp

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

#ifndef __YGGR_NETWORK_HEART_HEART_PACKET_HPP__
#define __YGGR_NETWORK_HEART_HEART_PACKET_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/network/heart/heart_packet_def.hpp>
#include <yggr/time/time.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/vector.hpp>
#include <yggr/serialization/time.hpp>

namespace yggr
{
namespace network
{
namespace heart
{

template<typename Time = yggr::time::time,
			typename TaskDataInfo = task_center::default_task_data_info_type>
class heart_data
{
public:
	typedef Time time_type;
	typedef TaskDataInfo task_data_info_type;

private:
	typedef ::yggr::vector<time_type> time_vt_type;

private:
	typedef heart_data this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// E_pak_id = heart_packet_def::E_id_start + 1
	// E_now_ver = 1
	YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (heart_packet_def::E_id_start + 1), 1);

private:
	YGGR_PP_CHECK_HEART_PACKET_ID((this_type::E_pak_id));

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_tm_vt);
	}

public:
	heart_data(void)
	{
		_tm_vt.push_back(time_type());
	}

	heart_data(BOOST_RV_REF(this_type) right)
		: _tm_vt(boost::move(right._tm_vt))
	{
	}

	heart_data(const this_type& right)
		: _tm_vt(right._tm_vt)
	{
	}

	~heart_data(void)
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

		copy_or_move_or_swap(_tm_vt, boost::move(right._tm_vt));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) 
		{
			return *this;
		}

		_tm_vt = right._tm_vt;
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
		_tm_vt.swap(right._tm_vt);
	}

public:
	inline size_type size(void) const
	{
		return _tm_vt.size();
	}

	inline void set_start(void)
	{
		_tm_vt[0] = time_type();
	}

	inline void set_last(void)
	{
		if(_tm_vt.size() == 1)
		{
			_tm_vt.push_back(time_type());
			return;
		}

		_tm_vt[1] = time_type();
	}

	inline time_type start(void) const
	{
		assert(!_tm_vt.empty());
		return _tm_vt[0];
	}

	inline time_type last(void) const
	{
		assert(!_tm_vt.empty());
		return _tm_vt.size() == 1? time_type(0) : _tm_vt[1];
	}

	inline time_type tm_step(void) const
	{
		return _tm_vt.size() == 1? time_type(0) : _tm_vt[1] - _tm_vt[0];
	}

private:
	time_vt_type _tm_vt;
};

} // namespace heart
} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{
namespace heart
{
namespace swap_support
{

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T) > inline 
void swap(heart_data< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T) >& l, 
			heart_data<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T)>& r) 
{ 
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace heart
} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::heart::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::heart::swap_support::swap;
} // namespace boost

#endif //__YGGR_NETWORK_HEART_HEART_PACKET_HPP__
