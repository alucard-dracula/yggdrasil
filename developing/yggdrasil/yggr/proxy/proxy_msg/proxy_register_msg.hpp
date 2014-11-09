//proxy_register_msg.hpp

#ifndef __YGGR_PROXY_PROXY_MSG_PROXY_REGISTER_MSG_HPP__
#define __YGGR_PROXY_PROXY_MSG_PROXY_REGISTER_MSG_HPP__

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
class proxy_register_msg
{
private:
	typedef Task_Data_Info task_data_info_type;
	typedef proxy_register_msg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	enum
	{
		E_pak_id = proxy_netdata_def::E_id_start + 1,

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
	typedef std::pair<u32, u32> reg_info_type; // mode, cal_type
	typedef boost::unordered_map<data_info_type, reg_info_type > reg_map_type;

private:
	typedef typename reg_map_type::value_type ins_val_type;



private:
	YGGR_PP_CHECK_PROXY_MSG_ID(E_pak_id);


private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_reg_map);
	}


public:
	proxy_register_msg(void)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	proxy_register_msg(BOOST_RV_REF(this_type) right)
		: _reg_map(boost::forward<reg_map_type>(right._reg_map))
	{
	}
#else
	proxy_register_msg(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		_reg_map.swap(right_ref._reg_map);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	proxy_register_msg(const this_type& right)
		: _reg_map(right._reg_map)
	{
	}

	~proxy_register_msg(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_reg_map = boost::forward<reg_map_type>(right._reg_map);
#else
		this_type& right_ref = right;
		_reg_map.swap(right_ref._reg_map);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_reg_map = right._reg_map;
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//_reg_map.swap(boost::forward<reg_map_type>(right._reg_map));
		_reg_map.swap(right._reg_map);
#else
		this_type& right_ref = right;
		_reg_map.swap(right_ref._reg_map);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		_reg_map.swap(right._reg_map);
	}

	bool add_reg_data(const data_info_type& data_info, u32 mode, u32 cal_type)
	{
		return _reg_map.insert(ins_val_type(data_info, reg_info_type(mode, cal_type))).second;
	}

	bool add_reg_data(const data_info_type& data_info, const reg_info_type ref_info)
	{
		return _reg_map.insert(ins_val_type(data_info, ref_info)).second;
	}

	template<typename T>
	bool add_reg_data(u32 mode)
	{
		typedef T data_type;
		return _reg_map.insert(ins_val_type(data_type::s_data_info(),
												reg_info_type(mode, data_type::s_cal_type()))).second;
	}

	bool rm_reg_data(const data_info_type& data_info)
	{
		return _reg_map.erase(data_info);
	}

	template<typename T>
	bool rm_reg_data(void)
	{
		typedef T data_type;
		return _reg_map.erase(data_type::s_data_info());
	}

	reg_map_type& reg_map(void)
	{
		return _reg_map;
	}

	const reg_map_type& reg_map(void) const
	{
		return _reg_map;
	}

private:
	reg_map_type _reg_map;
};

} // namespace proxy_msg
} // namespace proxy
} // namespace yggr

#define _YGGR_PP_PROXY_PROXY_MSG_PROXY_REGISTER_MSG_SWAP() \
	template<typename Task_Data_Info> \
	void swap(yggr::proxy::proxy_msg::proxy_register_msg<Task_Data_Info>& l, \
				yggr::proxy::proxy_msg::proxy_register_msg<Task_Data_Info>& r) { \
		l.swap(r); } \
	\
	template<typename Task_Data_Info> \
	void swap(BOOST_RV_REF(yggr::proxy::proxy_msg::proxy_register_msg<Task_Data_Info>) l, \
				yggr::proxy::proxy_msg::proxy_register_msg<Task_Data_Info>& r) { \
		r.swap(l); } \
	\
	template<typename Task_Data_Info> \
	void swap(yggr::proxy::proxy_msg::proxy_register_msg<Task_Data_Info>& l, \
				BOOST_RV_REF(yggr::proxy::proxy_msg::proxy_register_msg<Task_Data_Info>) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PP_PROXY_PROXY_MSG_PROXY_REGISTER_MSG_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_PROXY_PROXY_MSG_PROXY_REGISTER_MSG_SWAP()
} // namespace boost

#undef _YGGR_PP_PROXY_PROXY_MSG_PROXY_REGISTER_MSG_SWAP

#endif //__YGGR_PROXY_PROXY_MSG_PROXY_REGISTER_MSG_HPP__
