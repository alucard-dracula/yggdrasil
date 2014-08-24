//proxy_mode_change_back_msg.hpp

#ifndef __YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_BACK_MSG_HPP__
#define __YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_BACK_MSG_HPP__

#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/proxy/proxy_msg/proxy_netdata_def.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_msg
{

template<typename Task_Data_Info = task_center::default_task_data_info_type>
class proxy_mode_change_back_msg
{
private:
	typedef Task_Data_Info task_data_info_type;
	typedef proxy_mode_change_back_msg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	enum
	{
		E_pak_id = proxy_netdata_def::E_id_start + 4,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x00000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

public:
	YGGR_FAST_CAL_TASK_IMPORT(typename task_data_info_type, this_type, E_pak_id, E_now_ver)

private:
	YGGR_PP_CHECK_PROXY_MSG_ID(E_pak_id);


private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_data_info);
		ar & YGGR_SERIALIZE_NVP(_now_mode);
	}


public:
	proxy_mode_change_back_msg(void)
		: _now_mode(u32())
	{
	}

	proxy_mode_change_back_msg(const data_info_type& data_info, u32 now_mode)
		: _data_info(data_info), _now_mode(now_mode)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	proxy_mode_change_back_msg(BOOST_RV_REF(this_type) right)
		: _data_info(boost::forward<data_info_type>(right._data_info)),
			_now_mode(boost::forward<u32>(right_now_mode))
	{
	}
#else
	proxy_mode_change_back_msg(BOOST_RV_REF(this_type) right)
		: _data_info(boost::forward<data_info_type>(right._data_info)),
			_now_mode(u32())
	{
		std::swap(_now_mode, right._now_mode);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	proxy_mode_change_back_msg(const this_type& right)
		: _data_info(right._data_info), _now_mode(right._now_mode)
	{
	}

	~proxy_mode_change_back_msg(void)
	{
	}

	const u32& now_mode(void) const
	{
		return _now_mode;
	}

	u32& now_mode(void)
	{
		return _now_mode;
	}

	data_info_type& now_data_info(void)
	{
		return _data_info;
	}

	const data_info_type& now_data_info(void) const
	{
		return _data_info;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef  YGGR_NO_CXX11_RVALUE_REFERENCES
		_data_info = boost::forward<data_info_type>(right_ref._data_info);
		_now_mode = boost::forward<u32>(right_ref._now_mode);
#else
		this_type& right_ref = right;
		_data_info = boost::forward<data_info_type>(right_ref._data_info);
		std::swap(_now_mode, right_ref._now_mode);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_data_info = right._data_info;
		_now_mode = right._now_mode;
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//_data_info.swap(boost::forward<data_info_type>(right._data_info));
		//std::swap(_now_mode, boost::forward<u32>(right._now_mode));

		_data_info.swap(right._data_info);
		std::swap(_now_mode, right._now_mode);
#else
		this_type& right_ref = *this;
		_data_info.swap(right_ref._data_info);
		std::swap(_now_mode, right_ref._now_mode);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_data_info.swap(right._data_info);
		std::swap(_now_mode, right._now_mode);
	}

private:
	data_info_type _data_info;
	u32 _now_mode;
};

} // namespace proxy_msg
} // namespace proxy
} // namespace yggr

#define _YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_BACK_MSG_SWAP() \
	template<typename Task_Data_Info> inline \
	void swap(yggr::proxy::proxy_msg::proxy_mode_change_back_msg<Task_Data_Info>& l, \
				yggr::proxy::proxy_msg::proxy_mode_change_back_msg<Task_Data_Info>& r) { \
		l.swap(r); } \
	\
	template<typename Task_Data_Info> inline \
	void swap(BOOST_RV_REF(yggr::proxy::proxy_msg::proxy_mode_change_back_msg<Task_Data_Info>) l, \
				yggr::proxy::proxy_msg::proxy_mode_change_back_msg<Task_Data_Info>& r) { \
		r.swap(l); } \
	\
	template<typename Task_Data_Info> inline \
	void swap(yggr::proxy::proxy_msg::proxy_mode_change_back_msg<Task_Data_Info>& l, \
				BOOST_RV_REF(yggr::proxy::proxy_msg::proxy_mode_change_back_msg<Task_Data_Info>) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_BACK_MSG_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_BACK_MSG_SWAP()
} // namespace boost

#undef _YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_BACK_MSG_SWAP

#endif //__YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_BACK_MSG_HPP__
