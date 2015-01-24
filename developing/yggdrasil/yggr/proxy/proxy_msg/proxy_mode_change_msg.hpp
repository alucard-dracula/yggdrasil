//proxy_mode_change_msg.hpp

#ifndef __YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_MSG_HPP__
#define __YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_MSG_HPP__

#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/proxy/proxy_msg/proxy_netdata_def.hpp>

#include <yggr/serialization/unordered_map.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_msg
{

template<typename Task_Data_Info = task_center::default_task_data_info_type>
class proxy_mode_change_msg
{
private:
	typedef Task_Data_Info task_data_info_type;
	typedef proxy_mode_change_msg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	enum
	{
		E_pak_id = proxy_netdata_def::E_id_start + 3,

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

public:
	typedef boost::unordered_map< data_info_type, u32 > chg_map_type;

private:
	typedef typename chg_map_type::value_type ins_val_type;

private:
	YGGR_PP_CHECK_PROXY_MSG_ID(E_pak_id);

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_chg_map);
	}

public:
	proxy_mode_change_msg(void)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	proxy_mode_change_msg(BOOST_RV_REF(this_type) right)
		: _chg_map(boost::forward<chg_map_type>(right._chg_map))
	{
	}
#else
	proxy_mode_change_msg(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		_chg_map.swap(right_ref._chg_map);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	proxy_mode_change_msg(const this_type& right)
		: _chg_map(right._chg_map)
	{
	}

	~proxy_mode_change_msg(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_chg_map = boost::forward<chg_map_type>(right._chg_map);
#else
		this_type& right_ref = right;
		_chg_map.swap(right_ref._chg_map);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		_chg_map = right._chg_map;
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_chg_map.swap(right._chg_map);
#else
		this_type& right_ref = right;
		_chg_map.swap(right_ref._chg_map);
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_chg_map.swap(right._chg_map);
	}

	bool add_chg_data(const data_info_type& data_info, u32 mode)
	{
		return _chg_map.insert(ins_val_type(data_info, mode)).second;
	}

	bool rm_chg_data(const data_info_type& data_info)
	{
		return _chg_map.erase(data_info);
	}

	chg_map_type& chg_map(void)
	{
		return _chg_map;
	}

	const chg_map_type& chg_map(void) const
	{
		return _chg_map;
	}

private:
	chg_map_type _chg_map;
};

} // namespace proxy_msg
} // namespace proxy
} // namespace yggr

#define _YGGR_PP_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_MSG_SWAP() \
	template<typename Task_Data_Info> inline \
	void swap(yggr::proxy::proxy_msg::proxy_mode_change_msg<Task_Data_Info>& l, \
				yggr::proxy::proxy_msg::proxy_mode_change_msg<Task_Data_Info>& r) { \
		l.swap(r); } \
	\
	template<typename Task_Data_Info> inline \
	void swap(BOOST_RV_REF(yggr::proxy::proxy_msg::proxy_mode_change_msg<Task_Data_Info>) l, \
				yggr::proxy::proxy_msg::proxy_mode_change_msg<Task_Data_Info>& r) { \
		r.swap(l); } \
	\
	template<typename Task_Data_Info> inline \
	void swap(yggr::proxy::proxy_msg::proxy_mode_change_msg<Task_Data_Info>& l, \
				BOOST_RV_REF(yggr::proxy::proxy_msg::proxy_mode_change_msg<Task_Data_Info>) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PP_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_MSG_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_MSG_SWAP()
} // namespace boost


#undef _YGGR_PP_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_MSG_SWAP

#endif //__YGGR_PROXY_PROXY_MSG_PROXY_MODE_CHANGE_MSG_HPP__
