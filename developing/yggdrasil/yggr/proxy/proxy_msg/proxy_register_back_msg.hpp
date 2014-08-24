//proxy_register_back_msg.hpp

#ifndef __YGGR_PROXY_PROXY_MSG_PROXY_REGISTER_BACK_MSG_HPP__
#define __YGGR_PROXY_PROXY_MSG_PROXY_REGISTER_BACK_MSG_HPP__

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
class proxy_register_back_msg
{
private:
	typedef Task_Data_Info task_data_info_type;
	typedef proxy_register_back_msg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	enum
	{
		E_pak_id = proxy_netdata_def::E_id_start + 2,

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
	typedef std::pair<u32, u32> reg_back_info_type;
	typedef boost::unordered_map< data_info_type, reg_back_info_type > reg_back_map_type;

private:
	typedef typename reg_back_map_type::value_type ins_val_type;

private:
	YGGR_PP_CHECK_PROXY_MSG_ID(E_pak_id);

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_reg_back_map);
	}

public:
	proxy_register_back_msg(void)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	proxy_register_back_msg(BOOST_RV_REF(this_type) right)
		: _reg_back_map(boost::forward<reg_back_map_type>(right._reg_back_map))
	{
	}
#else
	proxy_register_back_msg(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		_reg_back_map.swap(right_ref._reg_back_map);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	proxy_register_back_msg(const this_type& right)
		: _reg_back_map(right._reg_back_map)
	{
	}

	~proxy_register_back_msg(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_reg_back_map = boost::forward<reg_back_map_type>(right._reg_back_map);
#else
		this_type& right_ref = right;
		_reg_back_map.swap(right_ref._reg_back_map);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_reg_back_map = right._reg_back_map;
		return *this;
	}

	bool add_reg_back_data(const data_info_type& data_info, u32 mode, u32 state)
	{
		return _reg_back_map.insert(ins_val_type(data_info, reg_back_info_type(mode, state))).second;
	}

	bool add_reg_back_data(const data_info_type& data_info, const reg_back_info_type& reg_back_info)
	{
		return _reg_back_map.insert(ins_val_type(data_info, reg_back_info)).second;
	}

	bool rm_reg_back_data(const data_info_type& data_info)
	{
		return _reg_back_map.erase(data_info);
	}

	reg_back_map_type& reg_back_map(void)
	{
		return _reg_back_map;
	}

	const reg_back_map_type& reg_back_map(void) const
	{
		return _reg_back_map;
	}

private:
	reg_back_map_type _reg_back_map;
};

} // namespace proxy_msg
} // namespace proxy
} // namespace yggr

#define _YGGR_PP_PROXY_PROXY_MSG_PROXY_REGISTER_BACK_MSG() \
	template<typename Task_Data_Info> inline \
	void swap(yggr::proxy::proxy_msg::proxy_register_back_msg<Task_Data_Info>& l, \
				yggr::proxy::proxy_msg::proxy_register_back_msg<Task_Data_Info>& r) { \
		l.swap(r); } \
	\
	template<typename Task_Data_Info> inline \
	void swap(BOOST_RV_REF(yggr::proxy::proxy_msg::proxy_register_back_msg<Task_Data_Info>) l, \
				yggr::proxy::proxy_msg::proxy_register_back_msg<Task_Data_Info>& r) { \
		r.swap(l); } \
	\
	template<typename Task_Data_Info> inline \
	void swap(yggr::proxy::proxy_msg::proxy_register_back_msg<Task_Data_Info>& l, \
				BOOST_RV_REF(yggr::proxy::proxy_msg::proxy_register_back_msg<Task_Data_Info>) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PP_PROXY_PROXY_MSG_PROXY_REGISTER_BACK_MSG()
} // namespace std

namespace boost
{
	_YGGR_PP_PROXY_PROXY_MSG_PROXY_REGISTER_BACK_MSG()
} // namespace boost

#undef _YGGR_PP_PROXY_PROXY_MSG_PROXY_REGISTER_BACK_MSG

#endif //__YGGR_PROXY_PROXY_MSG_PROXY_REGISTER_BACK_MSG_HPP__
