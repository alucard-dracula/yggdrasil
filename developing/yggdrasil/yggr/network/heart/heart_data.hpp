//normal_pak_back_id.hpp

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

#ifndef __YGGR_NETWORK_HEART_HEART_PACKET_HPP__
#define __YGGR_NETWORK_HEART_HEART_PACKET_HPP__

#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/network/heart/heart_packet_def.hpp>
#include <yggr/time/time.hpp>

#include <yggr/serialization/vector.hpp>

namespace yggr
{
namespace network
{
namespace heart
{

template<typename Time = yggr::time::time,
			typename Task_Data_Info = task_center::default_task_data_info_type>
class heart_data
{
public:
	typedef Time time_type;
	typedef Task_Data_Info task_data_info_type;

public:
	enum
	{
		E_pak_id = heart_packet_def::E_id_start + 1,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x00000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

private:
	typedef std::vector<time_type> time_vt_type;

private:
	YGGR_PP_CHECK_HEART_PACKET_ID(E_pak_id);
	typedef heart_data this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_tm_vt);
	}

public:

	heart_data(void)
	{
		_tm_vt.push_back(time_type());
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	heart_data(BOOST_RV_REF(this_type) right)
		: _tm_vt(boost::forward<time_vt_type>(right._tm_vt))
	{
	}

#else
	heart_data(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		_tm_vt.swap(right_ref._tm_vt);
	}

#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	heart_data(const this_type& right)
		: _tm_vt(right._tm_vt)
	{
	}

	~heart_data(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_tm_vt = boost::forward<time_vt_type>(right._tm_vt);
#else
		this_type& right_ref = right;
		_tm_vt.swap(right_ref._tm_vt);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_tm_vt = right._tm_vt;
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//_tm_vt.swap(boost::forward<time_vt_type>(right._tm_vt));
		_tm_vt.swap(right._tm_vt);
#else
		this_type& right_ref = right;
		_tm_vt.swap(right_ref._tm_vt);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_tm_vt.swap(right._tm_vt);
	}

	size_type size(void) const
	{
		return _tm_vt.size();
	}

	void set_start(void)
	{
		_tm_vt[0] = time_type();
	}

	void set_last(void)
	{
		if(_tm_vt.size() == 1)
		{
			_tm_vt.push_back(time_type());
			return;
		}

		_tm_vt[1] = time_type();
	}

	time_type start(void) const
	{
		assert(!_tm_vt.empty());
		return _tm_vt[0];
	}

	time_type last(void) const
	{
		assert(!_tm_vt.empty());
		return _tm_vt.size() == 1? time_type(0) : _tm_vt[1];
	}

	time_type tm_step(void) const
	{
		return _tm_vt.size() == 1? time_type(0) : _tm_vt[1] - _tm_vt[0];
	}

	YGGR_FAST_CAL_TASK_IMPORT(typename task_data_info_type, this_type, E_pak_id, E_now_ver)

private:
	time_vt_type _tm_vt;
};

} // namespace start_data
} // namespace network
} // namespace yggr


#define _YGGR_PP_NETWORK_HEART_HEART_DATA_SWAP() \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T) > inline \
	void swap(yggr::network::heart::heart_data< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T) >& l, \
				yggr::network::heart::heart_data<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T)>& r) { \
		l.swap(r); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T) > inline \
	void swap(BOOST_RV_REF(yggr::network::heart::heart_data<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T)>) l, \
				yggr::network::heart::heart_data<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T)>& r) { \
		r.swap(l); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T) > inline \
	void swap(yggr::network::heart::heart_data<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T)>& l, \
				BOOST_RV_REF(yggr::network::heart::heart_data< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T) >) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PP_NETWORK_HEART_HEART_DATA_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_NETWORK_HEART_HEART_DATA_SWAP()
} // namespace boost

#undef _YGGR_PP_NETWORK_HEART_HEART_DATA_SWAP

#endif //__YGGR_NETWORK_HEART_HEART_PACKET_HPP__
