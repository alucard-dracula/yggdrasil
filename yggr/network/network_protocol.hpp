//network_protocol.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_PROTOCOL_HPP__
#define __YGGR_NETWORK_NETWORK_PROTOCOL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/ppex/foo_params.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/task_center/support/task_import.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace network
{

template<typename Msg, typename TaskDataInfo /*= task_center::default_task_data_info_type*/,
			typename task_center::support::task_data_info_parser<TaskDataInfo>::data_id_type ID,
			typename task_center::support::task_data_info_parser<TaskDataInfo>::data_ver_type Ver,
			u32 Cal_T = task_center::task_cal_type::E_CAL_FAST >
class network_protocol
{
public:
	typedef Msg msg_type;
	typedef TaskDataInfo task_data_info_type;
	
	typedef msg_type value_type;

private:
	typedef network_protocol this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// E_pak_id = ID
	// E_now_ver = Ver
	YGGR_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, ID, Ver, Cal_T)

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_msg);
	}

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	network_protocol(BOOST_FWD_REF(Args)... args)
		: _msg(boost::forward<Args>(args)...)
	{
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, template< ) \
		YGGR_PP_FOO_TYPES_DEF( __n__ ) \
	BOOST_PP_EXPR_IF(__n__, > ) \
	network_protocol( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
		: _msg( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ) { }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	network_protocol(BOOST_RV_REF(msg_type) right)
		: _msg(boost::move(right))
	{
	}

	network_protocol(const msg_type& msg)
		: _msg(msg)
	{
	}

	template<typename OMsg, typename OTask_Data_Info, 
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_id_type OID,
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_ver_type OVer,
				u32 OCal_T >
	network_protocol(const network_protocol<OMsg, OTask_Data_Info, OID, OVer, OCal_T>& right)
		: _msg(right.msg())
	{
	}

	template<typename OTask_Data_Info, 
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_id_type OID,
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_ver_type OVer,
				u32 OCal_T>
	network_protocol(BOOST_RV_REF_BEG
						network_protocol
						<
							msg_type,
							OTask_Data_Info,
							OID,
							OVer,
							OCal_T 
						> BOOST_RV_REF_END right )
		: _msg(boost::move(right.msg()))
	{
	}

	template<typename OTask_Data_Info, 
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_id_type OID,
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_ver_type OVer,
				u32 OCal_T>
	network_protocol(const network_protocol<msg_type, OTask_Data_Info, OID, OVer, OCal_T>& right)
		: _msg(right.msg())
	{
	}

	network_protocol(BOOST_RV_REF(this_type) right)
		: _msg(boost::move(right._msg))
	{
	}

	network_protocol(const this_type& right)
		: _msg(right._msg)
	{
	}

	~network_protocol(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(msg_type) right)
	{
		msg_type& right_chk = right;
		if(boost::addressof(_msg) == boost::addressof(right_chk))
		{
			return *this;
		}

		copy_or_move_or_swap(_msg, boost::move(right));

		return *this;
	}

	this_type& operator=(const msg_type& right)
	{
		if(boost::addressof(_msg) == boost::addressof(right))
		{
			return *this;
		}

		_msg = right;
		return *this;
	}

	template<typename OTask_Data_Info, 
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_id_type OID,
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_ver_type OVer,
				u32 OCal_T> inline 
	this_type& operator=(BOOST_RV_REF_BEG
							network_protocol
							<
								msg_type,
								OTask_Data_Info,
								OID,
								OVer,
								OCal_T 
							> BOOST_RV_REF_END right)
	{
		typedef network_protocol<msg_type, OTask_Data_Info, OID, OVer, OCal_T> other_type;
		other_type& right_ref = right;
		copy_or_move_or_swap(_msg, boost::move(right_ref.msg()));
		return *this;
	}

	template<u32 OID, u16 OVer, typename OTask_Data_Info, u32 OCal_T>
	this_type& operator=(const 
							network_protocol
							<
								msg_type, 
								OTask_Data_Info, 
								OID, 
								OVer, 
								OCal_T
							>& right)
	{
		_msg = right.msg();
		return *this;
	}

	template<typename OMsg, typename OTask_Data_Info, 
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_id_type OID,
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_ver_type OVer,
				u32 OCal_T>
	this_type& operator=(const 
							network_protocol
							<
								OMsg, 
								OTask_Data_Info, 
								OID, 
								OVer, 
								OCal_T
							>& right)
	{
		_msg = right.msg();
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == boost::addressof(right_chk))
		{
			return *this;
		}

		copy_or_move_or_swap(_msg, boost::move(right._msg));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_msg = right._msg;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(msg_type) right)
	{
		msg_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(msg_type& right)
	{
		if(boost::addressof(_msg) == boost::addressof(right))
		{
			return;
		}

		yggr::swap(_msg, right);
	}

	template<typename OTask_Data_Info, 
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_id_type OID,
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_ver_type OVer,
				u32 OCal_T> 
	inline void swap(BOOST_RV_REF_BEG 
						network_protocol
						<
							msg_type,
							OTask_Data_Info, 
							OID,
							OVer,
							OCal_T 
						> BOOST_RV_REF_END right )
	{
		typedef network_protocol<msg_type, OTask_Data_Info,OID, OVer,  OCal_T> other_type;
		other_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename OTask_Data_Info, 
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_id_type OID,
				typename task_center::support::task_data_info_parser<OTask_Data_Info>::data_ver_type OVer,
				u32 OCal_T> 
	void swap(network_protocol<msg_type, OTask_Data_Info, OID, OVer, OCal_T>& right)
	{
		yggr::swap(_msg, right.msg());
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		yggr::swap(_msg, right._msg);
	}

public:
	inline operator msg_type&(void)
	{
		return _msg;
	}

	inline operator const msg_type&(void) const
	{
		return _msg;
	}

	inline msg_type& msg(void)
	{
		return _msg;
	}

	inline const msg_type& msg(void) const
	{
		return _msg;
	}

private:
	msg_type _msg;
};

} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{
namespace swap_support
{

template<typename Msg, typename TaskDataInfo1, 
			typename task_center::support::task_data_info_parser<TaskDataInfo1>::data_id_type ID1, 
			typename task_center::support::task_data_info_parser<TaskDataInfo1>::data_ver_type Ver1, 
			u32 Cal_T1, 
			typename TaskDataInfo2, 
			typename task_center::support::task_data_info_parser<TaskDataInfo2>::data_id_type ID2, 
			typename task_center::support::task_data_info_parser<TaskDataInfo2>::data_ver_type Ver2, 
			yggr::u32 Cal_T2 > inline 
void swap(typename network_protocol<Msg, TaskDataInfo1, ID1, Ver1, Cal_T1>::msg_type& l, 
			network_protocol<Msg, TaskDataInfo2, ID2, Ver2, Cal_T2>& r )
{ 
	r.swap(l); 
} 

template<typename Msg, typename TaskDataInfo1, 
			typename task_center::support::task_data_info_parser<TaskDataInfo1>::data_id_type ID1, 
			typename task_center::support::task_data_info_parser<TaskDataInfo1>::data_ver_type Ver1, 
			u32 Cal_T1, 
			typename TaskDataInfo2, 
			typename task_center::support::task_data_info_parser<TaskDataInfo2>::data_id_type ID2, 
			typename task_center::support::task_data_info_parser<TaskDataInfo2>::data_ver_type Ver2, 
			u32 Cal_T2 > inline 
void swap(network_protocol<Msg, TaskDataInfo1, ID1, Ver1, Cal_T1>& l, 
			typename network_protocol<Msg, TaskDataInfo2, ID2, Ver2, Cal_T2>::msg_type& r ) 
{ 
	l.swap(r);
} 

template<typename Msg, typename TaskDataInfo1, 
			typename task_center::support::task_data_info_parser<TaskDataInfo1>::data_id_type ID1, 
			typename task_center::support::task_data_info_parser<TaskDataInfo1>::data_ver_type Ver1, 
			u32 Cal_T1, 
			typename TaskDataInfo2, 
			typename task_center::support::task_data_info_parser<TaskDataInfo2>::data_id_type ID2, 
			typename task_center::support::task_data_info_parser<TaskDataInfo2>::data_ver_type Ver2, 
			yggr::u32 Cal_T2 > inline 
void swap(network_protocol<Msg, TaskDataInfo1, ID1, Ver1, Cal_T1>& l, 
			network_protocol<Msg, TaskDataInfo2, ID2, Ver2, Cal_T2>& r )
{ 
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::swap_support::swap;
} // namespace boost

#endif // __YGGR_NETWORK_NETWORK_PROTOCOL_HPP__

