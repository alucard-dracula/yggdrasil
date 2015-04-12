//network_packet.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_PACKET_HPP__
#define __YGGR_NETWORK_NETWORK_PACKET_HPP__

#include <boost/asio.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <yggr/move/move.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/concat.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/network/hn_conv.hpp>
#include <yggr/network/basic_network_packet.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/size_container.hpp>

#include <yggr/task_center/support/task_owner_info_parser.hpp>


namespace yggr
{
namespace network
{

template<typename Archive_Partner,
			typename DataInfoDef,
			typename NetInfo,
			typename Size = yggr::u32,
			template<typename _DataInfoDef>
				class DataInfo_Parser = yggr::network::support::network_data_info_parser,
			template<typename _NetInfo>
				class NetInfo_Parser = yggr::task_center::support::task_owner_info_parser,
			template<typename _Archive_Partner,
						typename _DataInfoDef,
						template<typename __DataInfoDef>
							class _DataInfo_Parser >
				class Base_Packet = basic_network_packet>
class network_packet
	: public Base_Packet< Archive_Partner, DataInfoDef, DataInfo_Parser>
{
	BOOST_MPL_ASSERT((boost::is_integral<Size>));
	BOOST_MPL_ASSERT((boost::is_unsigned<Size>));

public:
	typedef yggr::network::type_traits::tag_pak_dynamic packet_tag_type;
	typedef Archive_Partner archive_partner_type;
	typedef DataInfoDef data_info_def_type;
    typedef NetInfo net_info_type;
	typedef Size size_type;
	typedef Base_Packet<archive_partner_type, data_info_def_type, DataInfo_Parser> base_type;

	typedef typename base_type::buf_type buf_type;

	typedef typename base_type::data_info_parser_type data_info_parser_type;
	typedef typename base_type::data_id_type data_id_type;
	typedef typename base_type::data_ver_type data_ver_type;
	typedef typename base_type::data_info_type data_info_type; // tasked use needed

	typedef NetInfo_Parser<net_info_type> net_info_parser_type;
	typedef typename net_info_parser_type::owner_id_type owner_id_type;

	typedef net_info_type owner_info_type; // tasked use needed

	typedef buf_type data_container_type; // tasked use needed

	typedef size_container<size_type> size_container_type;

public:
	struct owner_info_getter
	{
		typedef const net_info_type& result_type;
		const net_info_type& operator()(const network_packet& right) const
		{
			return right.net_info();
		}
	};

	struct owner_id_getter
	{
		typedef const owner_id_type& result_type;
		const owner_id_type& operator()(const network_packet& right) const
		{
			return right.owner_id();
		}
	};

	struct data_info_getter
	{
		typedef const data_info_type& result_type;
		const data_info_type& operator()(const network_packet& right) const
		{
			return right.data_info();
		}
	};

	struct data_id_getter
	{
		typedef const data_id_type& result_type;
		const data_id_type& operator()(const network_packet& right) const
		{
			return right.data_id();
		}
	};

	struct data_ver_getter
	{
		typedef const data_ver_type& result_type;
		const data_ver_type& operator()(const network_packet& right) const
		{
			return right.data_ver();
		}
	};

private:
	typedef network_packet this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	network_packet(void)
	{
	}

	network_packet(BOOST_RV_REF(net_info_type) net_info)
		: _net_info(boost::forward<net_info_type>(net_info))
	{
	}

	network_packet(const net_info_type& net_info)
		: _net_info(net_info)
	{
	}

	template<typename Obj>
	network_packet(const net_info_type& net_info, const Obj& obj)
		: _net_info(net_info)
	{
		base_type::save(obj);
	}

	network_packet(BOOST_RV_REF(net_info_type) net_info, BOOST_RV_REF(buf_type) buf)
		: base_type(boost::forward<buf_type>(buf)),
		_net_info(boost::forward<net_info_type>(net_info))
	{
	}

	template<typename InputIter>
	network_packet(BOOST_RV_REF(net_info_type) net_info, InputIter start, InputIter end)
		: base_type(start, end),
			_net_info(boost::forward<net_info_type>(net_info))
	{
	}

	template<typename InputIter>
	network_packet(const net_info_type& net_info, InputIter start, InputIter end)
		: base_type(start, end), _net_info(net_info)
	{
	}

	network_packet(const net_info_type& net_info, const buf_type& buf)
		: base_type(buf), _net_info(net_info)
	{
	}

	network_packet(const net_info_type& net_info,
					const data_info_type& data_info,
					u32 cal_t,
					BOOST_RV_REF(buf_type) data_buf) // support to static_work_runner back
		: base_type(data_info, boost::forward<buf_type>(data_buf)), _net_info(net_info)
	{
		//assert(data_info == this_type::data_info());
	}

	network_packet(const net_info_type& net_info,
					const data_info_type& data_info,
					u32 cal_t,
					const buf_type& data_buf) // support to static_work_runner back
		: base_type(data_info, data_buf), _net_info(net_info)
	{
		//assert(data_info == this_type::data_info());
	}

	network_packet(BOOST_RV_REF(buf_type) net_info_buf,
					BOOST_RV_REF(buf_type) data_buf)
		: base_type(boost::forward<buf_type>(data_buf)),
			_net_info(boost::forward<buf_type>(net_info_buf))
	{
	}

	network_packet(const buf_type& net_info_buf, const buf_type& data_buf)
		: base_type(data_buf), _net_info(net_info_buf)
	{
	}

	template<typename Other_Archive_Partner>
	network_packet(BOOST_RV_REF(network_packet<Other_Archive_Partner
												YGGR_PP_SYMBOL_COMMA()
												data_info_def_type
												YGGR_PP_SYMBOL_COMMA()
												net_info_type
												YGGR_PP_SYMBOL_COMMA()
												size_type
												YGGR_PP_SYMBOL_COMMA()
												DataInfo_Parser
												YGGR_PP_SYMBOL_COMMA()
												NetInfo_Parser
												YGGR_PP_SYMBOL_COMMA()
												Base_Packet>) packet)
		: base_type(boost::forward<typename network_packet<
											Other_Archive_Partner,
											data_info_def_type,
											net_info_type,
											size_type,
											DataInfo_Parser,
											NetInfo_Parser,
											Base_Packet>::base_type>(packet)),
			_net_info(boost::forward<net_info_type>(packet.net_info()))
	{
	}

	template<typename Other_Archive_Partner>
	network_packet(const network_packet<Other_Archive_Partner,
											data_info_def_type,
											net_info_type,
											size_type,
											DataInfo_Parser,
											NetInfo_Parser,
											Base_Packet>& packet)
		: base_type(yggr::support::ref::to_base_cref(packet)),  //template base_type conv faile must use support::ref::to_base_cref foo
			_net_info(packet.net_info())
	{
	}

	network_packet(BOOST_RV_REF(this_type) right)
		: base_type(boost::forward<base_type>(right)),
			_net_info(boost::forward<net_info_type>(right._net_info))
	{
	}

	network_packet(const this_type& right)
		: base_type(right), _net_info(right._net_info)
	{
	}

	~network_packet(void)
	{
	}

	void clear(void)
	{
		base_type::clear();
		_net_info.clear();
	}

	size_type size_size(void) const
	{
		return sizeof(size_type);
	}

	static size_type s_size_size(void)
	{
		return sizeof(size_type);
	}

	size_type size_container_size(void) const
	{
		return size_container_type::s_size();
	}

	static size_type s_size_container_size(void)
	{
		return size_container_type::s_size();
	}

	size_type net_info_size(void) const
	{
		return _net_info.buf_size();
	}

	static size_type s_buf_size(void)
	{
		return 0;
	}

	size_type limit_buf_size(void) const
	{
		return std::numeric_limits<size_type>::max();
	}

	static size_type s_limit_buf_size(void)
	{
		return std::numeric_limits<size_type>::max();
	}

	net_info_type& net_info(void)
	{
		return _net_info;
	}

	const net_info_type& net_info(void) const
	{
		return _net_info;
	}

	net_info_type& owner_info(void)
	{
		return _net_info;
	}

	const net_info_type& owner_info(void) const // support to task_center and action_table
	{
		return _net_info;
	}

	owner_id_type owner_id(void) const// support to task_center and action_table
	{
		return _net_info.owner_id();
	}

	u8* net_info_buf(void)
	{
		return _net_info.buf();
	}

	const u8* net_info_buf(void) const
	{
		return _net_info.buf();
	}

	size_type data_size(void) const
	{
		return base_type::size();
	}

	u8* data_buf(void)
	{
		return base_type::buf();
	}

	const u8* data_buf(void) const
	{
		return base_type::buf();
	}

	template<typename Time>
	bool time_alive(const Time& tm_step) const
	{
		return _net_info.time_alive(tm_step);
	}

	void update_time(void)
	{
		_net_info.update_time();
	}

	size_type size(void) const
	{
		return _net_info.size() + base_type::size();
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(BOOST_RV_REF(network_packet<Other_Archive_Partner
												YGGR_PP_SYMBOL_COMMA()
												data_info_def_type
												YGGR_PP_SYMBOL_COMMA()
												net_info_type
												YGGR_PP_SYMBOL_COMMA()
												size_type
												YGGR_PP_SYMBOL_COMMA()
												DataInfo_Parser
												YGGR_PP_SYMBOL_COMMA()
												NetInfo_Parser
												YGGR_PP_SYMBOL_COMMA()
												Base_Packet>) right)
	{
		typedef network_packet<Other_Archive_Partner,
												data_info_def_type,
												net_info_type,
												size_type,
												DataInfo_Parser,
												NetInfo_Parser,
												Base_Packet> other_type;
		typedef typename other_type::base_type other_base_type;
		base_type& base = *this;
		base = boost::forward<other_base_type>(right);
		_net_info = boost::forward<net_info_type>(right.net_info());
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(const network_packet<Other_Archive_Partner,
												data_info_def_type,
												net_info_type,
												size_type,
												DataInfo_Parser,
												NetInfo_Parser,
												Base_Packet>& right)
	{
		base_type& base = *this;
		base = yggr::support::ref::to_base_cref(right);
		base = right;
		_net_info = right.net_info();
		return *this;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		base_type& base = *this;
		base = boost::forward<base_type>(right);
		_net_info = boost::forward<net_info_type>(right._header);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		base_type& base = *this;
		base = right;
		_net_info = right._net_info;
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
		//base_type::swap(boost::forward<base_type>(right));
		//_net_info.swap(boost::forward<net_info_type>(right._net_info));
		base_type::swap(right);
		_net_info.swap(right._net_info);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		base_type::swap(right);
		_net_info.swap(right._net_info);
	}

	template<typename T>
	bool save(const T& t)
	{
		return base_type::template save<T>(t);
	}

	template<typename T>
	bool load(T& t) const
	{
		return base_type::template load<T>(t);
	}

	template<typename Handler>
	typename Handler::result_type cypher(const Handler& handler)
	{
		return base_type::cypher(handler);
	}

private:
	net_info_type _net_info;
};

} // namespace network
} // namespace yggr

#ifdef __VA_ARGS__
#define _YGGR_PP_CONCAT_PARAMS() YGGR_PP_CONCAT(DataInfo_Parser, NetInfo_Parser, Base_Packet)
#else
#define _YGGR_PP_CONCAR_ARGS() DataInfo_Parser YGGR_PP_SYMBOL_COMMA() NetInfo_Parser YGGR_PP_SYMBOL_COMMA() Base_Packet
#define _YGGR_PP_CONCAT_PARAMS() YGGR_PP_CONCAT(_YGGR_PP_CONCAR_ARGS)
#endif // __VA_ARGS__

#define _YGGR_PP_NETWORK_NETWORK_PACKET_SWAP() \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 4, typename T ), \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1)> class DataInfo_Parser, \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2)> class NetInfo_Parser, \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename _T3), \
                            template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T33)> class _DataInfo_Parser> \
                                class Base_Packet > inline \
	void swap(YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
										_YGGR_PP_CONCAT_PARAMS())& l, \
				YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
										_YGGR_PP_CONCAT_PARAMS())& r) { \
		l.swap(r); } \
	\
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 4, typename T ), \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1)> class DataInfo_Parser, \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2)> class NetInfo_Parser, \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename _T3), \
                            template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T33)> class _DataInfo_Parser> \
                                class Base_Packet > inline \
	void swap(BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
													YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
													_YGGR_PP_CONCAT_PARAMS() ) ) l, \
				YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
										_YGGR_PP_CONCAT_PARAMS())& r) { \
		r.swap(l); } \
	\
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 4, typename T ), \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1)> class DataInfo_Parser, \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2)> class NetInfo_Parser, \
				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename _T3), \
                            template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T33)> class _DataInfo_Parser> \
                                class Base_Packet > inline \
	void swap(YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
										_YGGR_PP_CONCAT_PARAMS())& l, \
				BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
														YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
														_YGGR_PP_CONCAT_PARAMS() ) ) r) { \
		l.swap(r); }

//#define _YGGR_PP_NETWORK_NETWORK_PACKET_SWAP() \
//	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 4, typename T ), \
//				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1)> class DataInfo_Parser, \
//				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2)> class NetInfo_Parser, \
//				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename _T3), \
//                            template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T33)> class _DataInfo_Parser> \
//                                class Base_Packet > inline \
//	void swap(YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
//										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
//										YGGR_PP_CONCAT(DataInfo_Parser, NetInfo_Parser, Base_Packet))& l, \
//				YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
//										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
//										YGGR_PP_CONCAT(DataInfo_Parser, NetInfo_Parser, Base_Packet))& r) { \
//		l.swap(r); } \
//	\
//	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 4, typename T ), \
//				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1)> class DataInfo_Parser, \
//				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2)> class NetInfo_Parser, \
//				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename _T3), \
//                            template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T33)> class _DataInfo_Parser> \
//                                class Base_Packet > inline \
//	void swap(BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
//													YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
//													YGGR_PP_CONCAT(DataInfo_Parser, NetInfo_Parser, Base_Packet) ) ) l, \
//				YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
//										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
//										YGGR_PP_CONCAT(DataInfo_Parser, NetInfo_Parser, Base_Packet))& r) { \
//		r.swap(l); } \
//	\
//	template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 4, typename T ), \
//				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1)> class DataInfo_Parser, \
//				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2)> class NetInfo_Parser, \
//				template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename _T3), \
//                            template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T33)> class _DataInfo_Parser> \
//                                class Base_Packet > inline \
//	void swap(YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
//										YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
//										YGGR_PP_CONCAT(DataInfo_Parser, NetInfo_Parser, Base_Packet))& l, \
//				BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2(yggr::network::network_packet, \
//														YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), \
//														YGGR_PP_CONCAT(DataInfo_Parser, NetInfo_Parser, Base_Packet) ) ) r) { \
//		l.swap(r); }


namespace std
{
	_YGGR_PP_NETWORK_NETWORK_PACKET_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_NETWORK_NETWORK_PACKET_SWAP()
} // namespace boost

#undef _YGGR_PP_NETWORK_NETWORK_PACKET_SWAP
#undef _YGGR_PP_CONCAT_PARAMS

#ifndef __VAR_ARGS__
#undef _YGGR_PP_CONCAT_ARGS
#endif // __VAR_ARGS__

#include <boost/type_traits/is_same.hpp>

#include <yggr/task_center/support/task_real_data_getter.hpp>

namespace yggr
{
namespace task_center
{
namespace support
{

template<typename Archive_Partner,
			typename DataInfoDef,
			typename NetInfo,
			typename Size,
			template<typename _DataInfoDef>
				class DataInfo_Parser,
			template<typename _NetInfo>
				class NetInfo_Parser,
			template<typename _Archive_Partner,
						typename _DataInfoDef,
						template<typename __DataInfoDef>
							class _DataInfo_Parser >
				class Base_Packet>
struct task_real_data_getter
		<
			network::network_packet
			<
				Archive_Partner,
				DataInfoDef,
				NetInfo,
				Size,
				DataInfo_Parser,
				NetInfo_Parser,
				Base_Packet
			>
		>
{
	typedef network::network_packet
			<
				Archive_Partner,
				DataInfoDef,
				NetInfo,
				Size,
				DataInfo_Parser,
				NetInfo_Parser,
				Base_Packet
			> task_type;

	template<typename Real_Data>
	Real_Data* operator()(task_type& task) const
	{
		BOOST_MPL_ASSERT((boost::is_same<Real_Data, task_type>));
		return &task;
	}

	template<typename Real_Data>
	const Real_Data* operator()(const task_type& task) const
	{
		BOOST_MPL_ASSERT((boost::is_same<Real_Data, task_type>));
		return &task;
	}

};

} // namespace support
} // namesapce task_center
} // namespace yggr


#endif // __YGGR_NETWORK_NETWORK_PACKET_HPP__
