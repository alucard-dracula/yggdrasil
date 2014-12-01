//basic_network_packet.hpp

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

#ifndef __YGGR_NETWORK_BASIC_NETWORK_PACKET_HPP__
#define __YGGR_NETWORK_BASIC_NETWORK_PACKET_HPP__

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/error_make.hpp>
#include <yggr/archive/archive_partner_template.hpp>
#include <yggr/packet/packet_info.hpp>
#include <yggr/packet/packet.hpp>
#include <yggr/support/ref.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/network/support/data_info_parser.hpp>

namespace yggr
{
namespace network
{

template<typename Archive_Partner, typename DataInfoDef,
			template<typename _DataInfoDef>
				class DataInfo_Parser = yggr::network::support::network_data_info_parser>
class basic_network_packet
	: public yggr::packet::packet<Archive_Partner>
{
	BOOST_MPL_ASSERT( (
		boost::mpl::bool_<
			( static_cast<u32>(Archive_Partner::E_Mode)
				== static_cast<u32>(yggr::archive::
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

	enum
	{
		E_STORE_STATE = 0,
		E_SAVE,
		E_LOAD,
		E_LOAD_INFO,
        
        E_complile_u32 = 0xffffffff
	};
    
    struct flag_e_save_t{ enum {value = E_SAVE}; };
    struct flag_e_load_t{ enum {value = E_LOAD}; };
    struct flag_e_load_info_t{ enum {value = E_LOAD_INFO }; };
    
    template<typename Flag>
	class serialize_info_container
	{
	public:
		typedef typename boost::mpl::if_c< (Flag::value == E_SAVE),
									const data_info_type&,
									data_info_type&>::type info_type;
	public:
		serialize_info_container(info_type info)
			: _data_info(info)
		{
		}

		serialize_info_container(const serialize_info_container& right)
			: _data_info(right._data_info)
		{
		}

		~serialize_info_container(void)
		{
		}

	private:
		friend class boost::serialization::access;

		template<typename Archive>
		void serialize(Archive& ar, u32 version)
		{
			ar & YGGR_SERIALIZE_NVP(_data_info);
		}

	private:
		info_type _data_info;
	};


	template<typename Flag, typename Data_Object, typename Nil_T = int>
	class serialize_container
		: public serialize_info_container<Flag>
	{
	public:
        typedef serialize_info_container<Flag> cont_type;
		typedef typename cont_type::info_type info_type;
		typedef typename boost::mpl::if_c<(Flag::value == E_SAVE),
									const Data_Object&,
									Data_Object&>::type data_obj_type;

		serialize_container(info_type info, data_obj_type obj)
			: cont_type(info), _data_obj(obj)
		{
		}

		serialize_container(const serialize_container& right)
			: cont_type(right), _data_obj(right._data_obj)
		{
		}

		~serialize_container(void)
		{
		}

	private:
		friend class boost::serialization::access;

		template<typename Archive>
		void serialize(Archive& ar, u32 version)
		{
			ar & YGGR_SERIALIZE_NAME_NVP("info",
                                            boost::serialization::base_object<cont_type>(*this));
			ar & YGGR_SERIALIZE_NVP(_data_obj);
		}


	private:
		data_obj_type _data_obj;
	};


	template<typename Nil_T>
	class serialize_container<flag_e_load_info_t, void, Nil_T>
		: public serialize_info_container<flag_e_load_t>
	{
	public:
        typedef serialize_info_container<flag_e_load_t> cont_type;
		typedef typename cont_type::info_type info_type;

		serialize_container(info_type info)
			: cont_type(info)
		{
		}

		serialize_container(const serialize_container& right)
			: cont_type(right)
		{
		}

		~serialize_container(void)
		{
		}

	private:
		friend class boost::serialization::access;

		template<typename Archive>
		void serialize(Archive& ar, u32 version)
		{
			ar & YGGR_SERIALIZE_NAME_NVP("info",
                                            boost::serialization::base_object< cont_type >(*this));
		}
	};

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
		//restore_info();
	}

	template<typename InputIter>
	basic_network_packet(const data_info_type& data_info, InputIter start, InputIter end)
		: base_type(start, end), _data_info(data_info)
	{
	}

	basic_network_packet(const data_info_type& data_info, BOOST_RV_REF(buf_type) buf)
		: base_type(boost::forward<buf_type>(buf)), _data_info(data_info)
	{
		//restore_info();
	}

	basic_network_packet(BOOST_RV_REF(buf_type) buf)
		: base_type(boost::forward<buf_type>(buf))
	{
		//restore_info();
	}

	basic_network_packet(const data_info_type& data_info, const buf_type& buf)
		: base_type(buf), _data_info(data_info)
	{
	}

	basic_network_packet(const buf_type& buf)
		: base_type(buf)
	{
		//restore_info();
	}

	template<typename Other_Archive_Partner>
	basic_network_packet(BOOST_RV_REF(basic_network_packet< Other_Archive_Partner
															YGGR_PP_SYMBOL_COMMA()
															data_info_def_type
															YGGR_PP_SYMBOL_COMMA()
															DataInfo_Parser>) pak)
	{
		base_type::archive_helper_type::buf_copy(base_type::_buf, boost::forward<buf_type>(pak.org_buf()));
		_data_info = boost::forward<data_info_type>(pak.data_info());
	}

	template<typename Other_Archive_Partner>
	basic_network_packet(const basic_network_packet< Other_Archive_Partner,
													data_info_def_type,
													DataInfo_Parser>& pak)
		: base_type(yggr::support::ref::to_base_cref(pak)), _data_info(pak.data_info())
	{
	}

	basic_network_packet(BOOST_RV_REF(this_type) pak)
		: base_type(boost::forward<base_type>(pak)),
			_data_info(boost::forward<data_info_type>(pak._data_info) )
	{
	}

	basic_network_packet(const this_type& pak)
		: base_type(pak), _data_info(pak._data_info)
	{
	}

	~basic_network_packet(void)
	{
	}

	data_info_type& data_info(void)
	{
		return _data_info;
	}

	const data_info_type& data_info(void) const
	{
		return _data_info;
	}

	const data_id_type& data_id(void) const
	{
		return data_info_parser_type::data_id(_data_info);
	}

	const data_ver_type& data_ver(void) const
	{
		return data_info_parser_type::data_ver(_data_info);
	}

	void clear(void)
	{
		base_type::clear();
		_data_info.clear();
	}

	template<typename T>
	bool save(const T& t)
	{
		typedef T obj_type;
		typedef serialize_container<flag_e_save_t, obj_type> serialize_container_type;

		_data_info = data_info_parser_type::template data_info<obj_type>();
		serialize_container_type container(_data_info, t);

		return base_type::save(container);
	}

	template<typename T>
	bool load(T& t) const
	{
		typedef T obj_type;
		typedef serialize_container<flag_e_load_t, obj_type> serialize_container_type;

		serialize_container_type container(_data_info, t);
		return base_type::load(container);
	}

	this_type& operator=(BOOST_RV_REF(buf_type) buf)
	{
		base_type& base = *this;
		base = boost::forward<buf_type>(buf);
		//restore_info();
		return *this;
	}

	this_type& operator=(const buf_type& buf)
	{
		base_type& base = *this;
		base = buf;
		//restore_info();
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(BOOST_RV_REF(basic_network_packet<Other_Archive_Partner
															YGGR_PP_SYMBOL_COMMA()
															data_info_def_type
															YGGR_PP_SYMBOL_COMMA()
															DataInfo_Parser>) right)
	{
		typedef basic_network_packet<Other_Archive_Partner,
									data_info_type,
									DataInfo_Parser> other_type;
		typedef typename other_type::base_type other_base_type;
		base_type& base = *this;
		base = boost::forward<other_base_type>(right);
		_data_info = boost::forward<data_info_type>(right.info());
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(const basic_network_packet<Other_Archive_Partner,
													data_info_def_type,
													DataInfo_Parser>& right)
	{
		base_type& base = *this;
		base = yggr::support::ref::to_base_cref(right);
		_data_info = right.info();
		return *this;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		base_type& base = *this;
		base = boost::forward<base_type>(right);
		_data_info = boost::forward<data_info_type>(right._data_info);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;
		base = right;
		_data_info = right._data_info;
		return *this;
	}

	bool operator==(const data_info_type& info) const
	{
		return _data_info == info;
	}

	bool operator!=(const data_info_type& info) const
	{
		return _data_info != info;
	}

	template<typename Other_Archive_Partner>
	bool operator==(const basic_network_packet< Other_Archive_Partner,
												data_info_type,
												DataInfo_Parser>& pak) const
	{
		return base_type::_buf == pak.org_buf();
	}

	template<typename Other_Archive_Partner>
	bool operator!=(const basic_network_packet< Other_Archive_Partner,
												data_info_type,
												DataInfo_Parser>& pak) const
	{
		return base_type::_buf != pak.org_buf();
	}


	bool operator==(const this_type& right) const
	{
		return base_type::_buf == right._buf;
	}

	bool operator!=(const this_type& right) const
	{
		return base_type::_buf != right._buf;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
		//_data_info.swap(boost::forward<data_info_type>(right._data_info));
		//base_type::swap(boost::forward<base_type>(right));

		_data_info.swap(right._data_info);
		base_type::swap(right);
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

//private:
	void restore_info(void) //must after crypher need manual call
	{
		serialize_container<flag_e_load_info_t, void> info_container(_data_info);
		base_type::load(info_container);
	}

private:
	mutable data_info_type _data_info;
};

} // namespace network
} // namespaxe yggr

#define _YGGR_PP_NETWORK_BASIC_NETWORK_PACKET_SWAP() \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T ), \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 1, typename _T )> class DataInfo_Parser > inline \
	void swap(YGGR_PP_TEMPLATE_TYPE2( yggr::network::basic_network_packet, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ), DataInfo_Parser )& l, \
				YGGR_PP_TEMPLATE_TYPE2( yggr::network::basic_network_packet, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ), DataInfo_Parser )& r) { \
		l.swap(r); } \
	\
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T ), \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 1, typename _T )> class DataInfo_Parser> inline \
	void swap(BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2( yggr::network::basic_network_packet, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ), DataInfo_Parser )) l, \
				YGGR_PP_TEMPLATE_TYPE2( yggr::network::basic_network_packet, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ), DataInfo_Parser )& r) { \
		r.swap(l); } \
	\
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T ), \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 1, typename _T )> class DataInfo_Parser> inline \
	void swap(YGGR_PP_TEMPLATE_TYPE2( yggr::network::basic_network_packet, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ), DataInfo_Parser )& l, \
				BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2( yggr::network::basic_network_packet, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T ), DataInfo_Parser )) r) { \
		l.swap(r); }


namespace std
{
	_YGGR_PP_NETWORK_BASIC_NETWORK_PACKET_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_NETWORK_BASIC_NETWORK_PACKET_SWAP()
} // namespace boost

#undef _YGGR_PP_NETWORK_BASIC_NETWORK_PACKET_SWAP

#endif // __YGGR_NETWORK_BASIC_NETWORK_PACKET_HPP__
