//basic_network_packet.hpp

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

#ifndef __YGGR_NETWORK_BASIC_NETWORK_PACKET_HPP__
#define __YGGR_NETWORK_BASIC_NETWORK_PACKET_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/base/error_make.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/to_ref_helper.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/archive/archive_partner_template.hpp>
#include <yggr/packet/packet_info.hpp>
#include <yggr/packet/packet.hpp>

#include <yggr/network/support/data_info_parser.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>

#include <boost/ref.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/integral_c.hpp>

#include <boost/serialization/base_object.hpp>

namespace yggr
{
namespace network
{

namespace detail
{

// flags
typedef boost::mpl::integral_c<u32, 0> flag_e_store_sat_t;
typedef boost::mpl::integral_c<u32, flag_e_store_sat_t::value + 1> flag_e_save_t;
typedef boost::mpl::integral_c<u32, flag_e_store_sat_t::value + 2> flag_e_load_t;
typedef boost::mpl::integral_c<u32, flag_e_store_sat_t::value + 3> flag_e_load_info_t;

// serialize_info_container
template<typename Flag, typename DataInfo>
class serialize_info_container
{
public:
	typedef Flag flag_type;
	typedef DataInfo data_info_type;

	typedef typename 
		boost::mpl::if_c
		< 
			(flag_type::value == flag_e_save_t::value),
			const data_info_type&,
			data_info_type&
		>::type info_type;

private:
	typedef serialize_info_container this_type;

public:
	serialize_info_container(info_type info)
		: _data_info(info)
	{
	}

	serialize_info_container(const this_type& right)
		: _data_info(right._data_info)
	{
	}

	~serialize_info_container(void)
	{
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_data_info);
	}

private:
	info_type _data_info;
};

// serialize_container
template<typename Flag, typename DataInfo, typename DataObject>
class serialize_container
	: public serialize_info_container<Flag, DataInfo>
{
public:
	typedef Flag flag_type;
	typedef DataInfo data_info_type;
	typedef DataObject data_obj_arg_type;

public:
	typedef serialize_info_container<flag_type, data_info_type> base_type;

public:
    typedef base_type cont_type;
	typedef typename cont_type::info_type info_type;
	typedef typename 
		boost::mpl::if_c
		<
			(flag_type::value == flag_e_save_t::value),
			const data_obj_arg_type&,
			data_obj_arg_type&
		>::type data_obj_type;

private:
	typedef serialize_container this_type;

public:
	serialize_container(info_type info, data_obj_type obj)
		: base_type(info), _data_obj(obj)
	{
	}

	serialize_container(const this_type& right)
		: base_type(right), _data_obj(right._data_obj)
	{
	}

	~serialize_container(void)
	{
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"info",
                boost::serialization::base_object<cont_type>(*this));
		ar & YGGR_SERIALIZATION_NVP(_data_obj);
	}


private:
	data_obj_type _data_obj;
};


template<typename DataInfo>
class serialize_container<flag_e_load_info_t, DataInfo, void>
	: public serialize_info_container<flag_e_load_t, DataInfo>
{
public:
	typedef flag_e_load_info_t flag_type;
	typedef DataInfo data_info_type;
	typedef void data_obj_arg_type;

public:
	typedef serialize_info_container<flag_e_load_t, data_info_type> base_type;

public:
    typedef base_type cont_type;
	typedef typename cont_type::info_type info_type;

private:
	typedef serialize_container this_type;

public:
	serialize_container(info_type info)
		: base_type(info)
	{
	}

	serialize_container(const this_type& right)
		: base_type(right)
	{
	}

	~serialize_container(void)
	{
	}

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NAME_NVP(
				"info",
				boost::serialization::base_object< cont_type >(*this));
	}
};

} // namespace detail

template<typename Archive_Partner, typename DataInfoDef,
			template<typename _DataInfoDef>
				class DataInfo_Parser = yggr::network::support::network_data_info_parser>
class basic_network_packet
	: public yggr::packet::packet<Archive_Partner>
{
	BOOST_MPL_ASSERT( (
		boost::mpl::bool_<
			( static_cast<u32>(Archive_Partner::E_Mode)
				== static_cast<u32>(
					yggr::archive::
						archive_partner::
							archive_partner_t::
								E_Mode_Network)) > ) );
public:
	typedef Archive_Partner archive_partner_type;
	typedef DataInfoDef data_info_def_type;
	typedef DataInfo_Parser<data_info_def_type> data_info_parser_type;

	typedef typename data_info_parser_type::data_id_type data_id_type;
	typedef typename data_info_parser_type::data_ver_type data_ver_type;
	typedef typename data_info_parser_type::data_info_type data_info_type;

	typedef yggr::packet::packet<archive_partner_type> base_type;

	typedef typename base_type::buf_type buf_type;

private:

	ERROR_MAKER_BEGIN("basic_network_packet")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_5)
			ERROR_CODE_DEF(E_invalid_version)
			ERROR_CODE_DEF(E_invalid_id_type)
			ERROR_CODE_DEF(E_invalid_version_type)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_5)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_version, "invalid version")
			ERROR_CODE_MSG(E_invalid_id_type, "invalid id type")
			ERROR_CODE_MSG(E_invalid_version_type, "invalid version type")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

//-------------------------------------------------------------------------

	//typedef boost::mpl::integral_c<u32, 0> flag_e_store_sat_t;
	//typedef boost::mpl::integral_c<u32, flag_e_store_sat_t::value + 1> flag_e_save_t;
	//typedef boost::mpl::integral_c<u32, flag_e_store_sat_t::value + 2> flag_e_load_t;
	//typedef boost::mpl::integral_c<u32, flag_e_store_sat_t::value + 3> flag_e_load_info_t;

	typedef detail::flag_e_store_sat_t flag_e_store_sat_t;
	typedef detail::flag_e_save_t flag_e_save_t;
	typedef detail::flag_e_load_t flag_e_load_t;
	typedef detail::flag_e_load_info_t flag_e_load_info_t;

	YGGR_STATIC_CONSTANT(u32, E_STORE_STATE = flag_e_store_sat_t::value);
	YGGR_STATIC_CONSTANT(u32, E_SAVE = flag_e_save_t::value);
	YGGR_STATIC_CONSTANT(u32, E_LOAD = flag_e_load_t::value);
	YGGR_STATIC_CONSTANT(u32, E_LOAD_INFO = flag_e_load_info_t::value);

private:
	typedef basic_network_packet this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	basic_network_packet(void)
	{
	}

	template<typename InputIter>
	basic_network_packet(InputIter start, InputIter end)
		: base_type(start, end)
	{
	}

	template<typename InputIter>
	basic_network_packet(const data_info_type& data_info, InputIter start, InputIter end)
		: base_type(start, end), _data_info(data_info)
	{
	}

	basic_network_packet(const data_info_type& data_info, BOOST_RV_REF(buf_type) buf)
		: base_type(boost::move(buf)), _data_info(data_info)
	{
	}

	basic_network_packet(const data_info_type& data_info, const buf_type& buf)
		: base_type(buf), _data_info(data_info)
	{
	}

	basic_network_packet(BOOST_RV_REF(buf_type) buf)
		: base_type(boost::move(buf))
	{
	}

	basic_network_packet(const buf_type& buf)
		: base_type(buf)
	{
	}

	template<typename Other_Archive_Partner>
	basic_network_packet(BOOST_RV_REF_BEG 
							basic_network_packet
							< 
								Other_Archive_Partner,
								data_info_def_type, 
								DataInfo_Parser
							> BOOST_RV_REF_END pak)
	{
		base_type::archive_helper_type::buf_copy(
			base_type::_buf, boost::move(pak.org_buf()));
		_data_info = boost::move(pak.data_info());
	}

	template<typename Other_Archive_Partner>
	basic_network_packet(const 
							basic_network_packet
							< 
								Other_Archive_Partner,
								data_info_def_type,
								DataInfo_Parser
							>& pak)
		: base_type(yggr::to_ref_helper::to_base_cref(pak)), 
			_data_info(pak.data_info())
	{
	}

	basic_network_packet(BOOST_RV_REF(this_type) pak)
		: base_type(boost::move(static_cast<base_type&>(pak))),
			_data_info(boost::move(pak._data_info) )
	{
	}

	basic_network_packet(const this_type& pak)
		: base_type(pak), _data_info(pak._data_info)
	{
	}

	~basic_network_packet(void)
	{
	}

public:
	this_type& operator=(BOOST_RV_REF(buf_type) buf)
	{
		base_type& base = *this;
		copy_or_move_or_swap(base, boost::move(buf));
		return *this;
	}

	this_type& operator=(const buf_type& buf)
	{
		base_type& base = *this;
		base = buf;
		return *this;
	}

	template<typename Other_Archive_Partner> inline
	this_type& operator=(BOOST_RV_REF_BEG 
							basic_network_packet
							<
								Other_Archive_Partner,		
								data_info_def_type,
								DataInfo_Parser
							> BOOST_RV_REF_END right)
	{
		typedef 
			basic_network_packet
			<
				Other_Archive_Partner,
				data_info_type,
				DataInfo_Parser
			> other_type;
		typedef typename other_type::base_type other_base_type;

		base_type& base = *this;
		base = boost::move(static_cast<other_base_type&>(right));
		_data_info = boost::move(right.info());

		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(const 
							basic_network_packet
							<
								Other_Archive_Partner,
								data_info_def_type,
								DataInfo_Parser
							>& right)
	{
		base_type& base = *this;
		base = yggr::to_ref_helper::to_base_cref(right);
		_data_info = right.info();
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type& base = *this;

		copy_or_move_or_swap(base, boost::move(static_cast<base_type&>(right)));
		copy_or_move_or_swap(_data_info, boost::move(right._data_info));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type& base = *this;
		base = right;
		_data_info = right._data_info;
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

		base_type::swap(right);
		_data_info.swap(right._data_info);
	}

public:
	inline data_info_type& data_info(void)
	{
		return _data_info;
	}

	inline const data_info_type& data_info(void) const
	{
		return _data_info;
	}

	inline const data_id_type& data_id(void) const
	{
		return data_info_parser_type::data_id(_data_info);
	}

	inline const data_ver_type& data_ver(void) const
	{
		return data_info_parser_type::data_ver(_data_info);
	}

public:
	template<typename T>
	bool compatibility_save(const data_info_type& data_info, const T& t)
	{
		typedef T obj_type;
		//typedef serialize_container<flag_e_save_t, obj_type> serialize_container_type;
		typedef detail::serialize_container<flag_e_save_t, data_info_type, obj_type> serialize_container_type;

		_data_info = data_info;
		serialize_container_type container(_data_info, t);

		return base_type::save(container);
	}

	template<typename T>
	bool save(const T& t)
	{
		typedef T obj_type;
		//typedef serialize_container<flag_e_save_t, obj_type> serialize_container_type;
		typedef detail::serialize_container<flag_e_save_t, data_info_type, obj_type> serialize_container_type;

		_data_info = data_info_parser_type::template data_info<obj_type>();
		serialize_container_type container(_data_info, t);

		return base_type::save(container);
	}

	template<typename T>
	bool load(T& t) const
	{
		typedef T obj_type;
		//typedef serialize_container<flag_e_load_t, obj_type> serialize_container_type;
		typedef detail::serialize_container<flag_e_load_t, data_info_type, obj_type> serialize_container_type;

		serialize_container_type container(_data_info, t);
		return base_type::load(container);
	}

public:
	inline void clear(void)
	{
		base_type::clear();
		_data_info.clear();
	}

	inline void restore_info(void) //must after crypher need manual call
	{
		//typedef serialize_container<flag_e_load_info_t, void> serialize_container_type;
		typedef detail::serialize_container<flag_e_load_info_t, data_info_type, void> serialize_container_type;

		serialize_container_type info_container(_data_info);
		base_type::load(info_container);
	}

public:
	template<typename InputIter> inline
	void assign(InputIter start, InputIter end)
	{
		base_type::assign(start, end);
	}

	template<typename InputIter> inline
	void assign(const data_info_type& data_info, InputIter start, InputIter end)
	{
		_data_info = data_info;
		base_type::assign(start, end);
	}

	inline void assign(const data_info_type& data_info, BOOST_RV_REF(buf_type) buf)
	{
		_data_info = data_info;
		base_type::operator=(boost::move(buf));
	}

	inline void assign(const data_info_type& data_info, const buf_type& buf)
	{
		_data_info = data_info;
		base_type::operator=(buf);
	}

	inline void assign(BOOST_RV_REF(buf_type) buf)
	{
		base_type::operator=(boost::move(buf));
	}

	inline void assign(const buf_type& buf)
	{
		base_type::operator=(buf);
	}

	template<typename Other_Archive_Partner> inline
	void assign(BOOST_RV_REF_BEG 
					basic_network_packet
					< 
						Other_Archive_Partner,
						data_info_def_type, 
						DataInfo_Parser
					> BOOST_RV_REF_END pak)
	{
		base_type::archive_helper_type::buf_copy(
			base_type::_buf, boost::move(pak.org_buf()));
		_data_info = boost::move(pak.data_info());
	}

	template<typename Other_Archive_Partner> inline
	void assign(const 
					basic_network_packet
					<
						Other_Archive_Partner,
						data_info_def_type,
						DataInfo_Parser
					>& pak)
	{
		_data_info = pak.data_info();
		base_type::operator=(yggr::to_ref_helper::to_base_cref(pak));
	}

	inline void assign(BOOST_RV_REF(this_type) pak)
	{
		this_type::operator=(boost::move(pak));
	}

	inline void assign(const this_type& pak)
	{
		this_type::operator=(pak);
	}

public:
	inline bool compare_eq(const data_info_type& info) const
	{
		return _data_info == info;
	}

	template<typename Other_Archive_Partner> inline
	bool compare_eq(const 
						basic_network_packet
						<
							Other_Archive_Partner,
							data_info_type,
							DataInfo_Parser
						>& pak) const
	{
		return base_type::_buf == pak.org_buf();
	}

	inline bool compare_eq(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| (base_type::_buf == right._buf);
	}

private:
	mutable data_info_type _data_info;
};

// non-member function

// operator ==
template<typename AP, typename DID, template<typename _DID> class DP> inline
bool operator==(const typename basic_network_packet<AP, DID, DP>::data_info_type& l,
				const basic_network_packet<AP, DID, DP>& r)
{
	return r.compare_eq(l);
}

template<typename AP, typename DID, template<typename _DID> class DP> inline
bool operator==(const basic_network_packet<AP, DID, DP>& l,
				const typename basic_network_packet<AP, DID, DP>::data_info_type& r)
{
	return l.compare_eq(r);
}

template<typename APL, typename APR, typename DID, template<typename _DID> class DP> inline
bool operator==(const basic_network_packet<APL, DID, DP>& l,
				const basic_network_packet<APR, DID, DP>& r)
{
	return l.compare_eq(r);
}

template<typename AP, typename DID, template<typename _DID> class DP> inline
bool operator==(const basic_network_packet<AP, DID, DP>& l,
				const basic_network_packet<AP, DID, DP>& r)
{
	return l.compare_eq(r);
}

// operator!=
template<typename AP, typename DID, template<typename _DID> class DP> inline
bool operator!=(const typename basic_network_packet<AP, DID, DP>::data_info_type& l,
				const basic_network_packet<AP, DID, DP>& r)
{
	return !r.compare_eq(l);
}

template<typename AP, typename DID, template<typename _DID> class DP> inline
bool operator!=(const basic_network_packet<AP, DID, DP>& l,
				const typename basic_network_packet<AP, DID, DP>::data_info_type& r)
{
	return !l.compare_eq(r);
}

template<typename APL, typename APR, typename DID, template<typename _DID> class DP> inline
bool operator!=(const basic_network_packet<APL, DID, DP>& l,
				const basic_network_packet<APR, DID, DP>& r)
{
	return !l.compare_eq(r);
}

template<typename AP, typename DID, template<typename _DID> class DP> inline
bool operator!=(const basic_network_packet<AP, DID, DP>& l,
				const basic_network_packet<AP, DID, DP>& r)
{
	return !l.compare_eq(r);
}


} // namespace network
} // namespaxe yggr

namespace yggr
{
namespace network
{
namespace swap_support
{

template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T ), 
			template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 1, typename _T )> class DP > inline 
void swap(YGGR_PP_TEMPLATE_TYPE2( 
				yggr::network::basic_network_packet, 
				YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ), 
				DP )& l, 
			YGGR_PP_TEMPLATE_TYPE2( 
				yggr::network::basic_network_packet, 
				YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ), 
				DP )& r)
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

#endif // __YGGR_NETWORK_BASIC_NETWORK_PACKET_HPP__
