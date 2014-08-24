//proxy_unregister_msg.hpp

#ifndef __YGGR_PROXY_PROXY_MSG_PROXY_UNREGISTER_MSG_HPP__
#define __YGGR_PROXY_PROXY_MSG_PROXY_UNREGISTER_MSG_HPP__

#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/proxy/proxy_msg/proxy_netdata_def.hpp>

#include <yggr/serialization/unordered_set.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_msg
{

template<typename Task_Data_Info = task_center::default_task_data_info_type>
class proxy_unregister_msg
{
private:
	typedef Task_Data_Info task_data_info_type;
	typedef proxy_unregister_msg this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	enum
	{
		E_pak_id = proxy_netdata_def::E_id_start + 5,

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
	typedef boost::unordered_set<data_info_type> unreg_set_type;

private:
	YGGR_PP_CHECK_PROXY_MSG_ID(E_pak_id);


private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_unreg_set);
	}

public:
	proxy_unregister_msg(void)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	proxy_unregister_msg(BOOST_RV_REF(this_type) right)
		: _unreg_set(boost::forward<unreg_set_type>(right._unreg_set))
	{
	}
#else
	proxy_unregister_msg(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		_unreg_set.swap(right_ref._unreg_set);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	proxy_unregister_msg(const this_type& right)
		: _unreg_set(right._unreg_set)
	{
	}

	~proxy_unregister_msg(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_unreg_set = boost::forward<unreg_set_type>(right._unreg_set);
#else
		this_type& right_ref = right;
		_unreg_set.swap(right_ref._unreg_set);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_unreg_set = right._unreg_set;
		return *this;
	}

	bool add_unreg_data(const data_info_type& data_info)
	{
		return _unreg_set.insert(data_info).second;
	}

	bool rm_unreg_data(const data_info_type& data_info)
	{
		return _unreg_set.erase(data_info);
	}

	unreg_set_type& unreg_set(void)
	{
		return _unreg_set;
	}

	const unreg_set_type& unreg_set(void) const
	{
		return _unreg_set;
	}

private:
	unreg_set_type _unreg_set;
};

} // namespace proxy_msg
} // namespace proxy
} // namespace yggr

#define _YGGR_PP_PROXY_PROXY_MSG_PROXY_UNREGISTER_MSG_SWAP() \
	template<typename Task_Data_Info> \
	void swap(yggr::proxy::proxy_msg::proxy_unregister_msg<Task_Data_Info>& l, \
				yggr::proxy::proxy_msg::proxy_unregister_msg<Task_Data_Info>& r) { \
		l.swap(r); } \
	\
	template<typename Task_Data_Info> \
	void swap(BOOST_RV_REF(yggr::proxy::proxy_msg::proxy_unregister_msg<Task_Data_Info>) l, \
				yggr::proxy::proxy_msg::proxy_unregister_msg<Task_Data_Info>& r) { \
		r.swap(l); } \
	\
	template<typename Task_Data_Info> \
	void swap(yggr::proxy::proxy_msg::proxy_unregister_msg<Task_Data_Info>& l, \
				BOOST_RV_REF(yggr::proxy::proxy_msg::proxy_unregister_msg<Task_Data_Info>) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PP_PROXY_PROXY_MSG_PROXY_UNREGISTER_MSG_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_PROXY_PROXY_MSG_PROXY_UNREGISTER_MSG_SWAP()
} // namespace boost

#undef _YGGR_PP_PROXY_PROXY_MSG_PROXY_UNREGISTER_MSG_SWAP

#endif //__YGGR_PROXY_PROXY_MSG_PROXY_UNREGISTER_MSG_HPP__
