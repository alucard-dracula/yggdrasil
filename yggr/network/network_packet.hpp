//network_packet.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_PACKET_HPP__
#define __YGGR_NETWORK_NETWORK_PACKET_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>

#include <yggr/mplex/limits.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/ppex/concat.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/network/hn_conv.hpp>
#include <yggr/network/basic_network_packet.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/network/size_container.hpp>

#include <yggr/task_center/support/task_owner_info_parser.hpp>

#include <boost/asio.hpp>
#include <boost/ref.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_unsigned.hpp>


namespace yggr
{
namespace network
{

template<typename Archive_Partner,
			typename DataInfoDef,
			typename NetInfo,
			typename Size = ::yggr::u32,
			template<typename _DataInfoDef>
				class DataInfo_Parser = ::yggr::network::support::network_data_info_parser,
			template<typename _NetInfo>
				class NetInfo_Parser = ::yggr::task_center::support::task_owner_info_parser,
			template<typename _Archive_Partner,
						typename _DataInfoDef,
						template<typename __DataInfoDef> class _DataInfo_Parser >
				class Base_Packet = basic_network_packet>
class network_packet
	: public Base_Packet< Archive_Partner, DataInfoDef, DataInfo_Parser>
{
	BOOST_MPL_ASSERT((boost::is_integral<Size>));
	BOOST_MPL_ASSERT((boost::is_unsigned<Size>));

public:
	typedef ::yggr::network::type_traits::tag_pak_dynamic packet_tag_type;
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

		inline result_type operator()(const network_packet& right) const
		{
			return right.net_info();
		}
	};

	struct owner_id_getter
	{
		typedef owner_id_type result_type;

		inline result_type operator()(const network_packet& right) const
		{
			return right.owner_id();
		}
	};

	struct data_info_getter
	{
		typedef const data_info_type& result_type;

		inline result_type operator()(const network_packet& right) const
		{
			return right.data_info();
		}
	};

	struct data_id_getter
	{
		typedef const data_id_type& result_type;

		inline result_type operator()(const network_packet& right) const
		{
			return right.data_id();
		}
	};

	struct data_ver_getter
	{
		typedef const data_ver_type& result_type;

		inline result_type operator()(const network_packet& right) const
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
		: _net_info(boost::move(net_info))
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

	template<typename InputIter>
	network_packet(BOOST_RV_REF(net_info_type) net_info, InputIter start, InputIter end)
		: base_type(start, end),
			_net_info(boost::move(net_info))
	{
	}

	template<typename InputIter>
	network_packet(const net_info_type& net_info, InputIter start, InputIter end)
		: base_type(start, end), _net_info(net_info)
	{
	}

	network_packet(BOOST_RV_REF(net_info_type) net_info, BOOST_RV_REF(buf_type) buf)
		: base_type(boost::move(buf)),
			_net_info(boost::move(net_info))
	{
	}

	network_packet(BOOST_RV_REF(net_info_type) net_info, const buf_type& buf)
		: base_type(buf),
			_net_info(boost::move(net_info))
	{
	}

	network_packet(const net_info_type& net_info, BOOST_RV_REF(buf_type) buf)
		: base_type(boost::move(buf)),
			_net_info(net_info)
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
		: base_type(data_info, boost::move(data_buf)),
			_net_info(net_info)
	{
	}

	network_packet(const net_info_type& net_info,
					const data_info_type& data_info,
					u32 cal_t,
					const buf_type& data_buf) // support to static_work_runner back
		: base_type(data_info, data_buf), 
			_net_info(net_info)
	{
	}

	network_packet(BOOST_RV_REF(buf_type) net_info_buf,
					BOOST_RV_REF(buf_type) data_buf)
		: base_type(boost::move(data_buf)),
			_net_info(boost::move(net_info_buf))
	{
	}

	network_packet(BOOST_RV_REF(buf_type) net_info_buf,
					const buf_type& data_buf)
		: base_type(data_buf),
			_net_info(boost::move(net_info_buf))
	{
	}

	network_packet(const buf_type& net_info_buf,
					BOOST_RV_REF(buf_type) data_buf)
		: base_type(boost::move(data_buf)),
			_net_info(net_info_buf)
	{
	}

	network_packet(const buf_type& net_info_buf, const buf_type& data_buf)
		: base_type(data_buf), _net_info(net_info_buf)
	{
	}

	template<typename Other_Archive_Partner>
	network_packet(BOOST_RV_REF_BEG
					network_packet
					<
						Other_Archive_Partner,
						data_info_def_type,
						net_info_type,
						size_type,
						DataInfo_Parser,
						NetInfo_Parser,
						Base_Packet
					> BOOST_RV_REF_END packet)
		: base_type(boost::move(::yggr::to_ref_helper::to_base_ref(packet))),
			_net_info(boost::move(packet.net_info()))
	{
	}

	template<typename Other_Archive_Partner>
	network_packet(const 
					network_packet
					<
						Other_Archive_Partner,
						data_info_def_type,
						net_info_type,
						size_type,
						DataInfo_Parser,
						NetInfo_Parser,
						Base_Packet
					>& packet)
		: base_type(::yggr::to_ref_helper::to_base_cref(packet)),  //template base_type conv failed must use to_ref_helper::to_base_cref foo
			_net_info(packet.net_info())
	{
	}

	network_packet(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right))),
			_net_info(boost::move(right._net_info))
	{
	}

	network_packet(const this_type& right)
		: base_type(right), _net_info(right._net_info)
	{
	}

	~network_packet(void)
	{
	}

public:
	template<typename Other_Archive_Partner> inline
	this_type& operator=(BOOST_RV_REF_BEG
							network_packet
							<
								Other_Archive_Partner,
								data_info_def_type,
								net_info_type,
								size_type,
								DataInfo_Parser,
								NetInfo_Parser,
								Base_Packet
							> BOOST_RV_REF_END right)
	{
		typedef 
			network_packet
			<
				Other_Archive_Partner,
				data_info_def_type,
				net_info_type,
				size_type,
				DataInfo_Parser,
				NetInfo_Parser,
				Base_Packet
			> other_type;
		typedef typename other_type::base_type other_base_type;
		
		base_type& base = *this;

		base = boost::move(static_cast<other_base_type&>(right));
		_net_info = boost::move(right.net_info());
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(const 
							network_packet
							<
								Other_Archive_Partner,
								data_info_def_type,
								net_info_type,
								size_type,
								DataInfo_Parser,
								NetInfo_Parser,
								Base_Packet
							>& right)
	{
		base_type& base = *this;
		base = ::yggr::to_ref_helper::to_base_cref(right);
		base = right;
		_net_info = right.net_info();
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
		copy_or_move_or_swap(_net_info, boost::move(right._header));

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
		_net_info = right._net_info;
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
		_net_info.swap(right._net_info);
	}

public:
	inline void clear(void)
	{
		base_type::clear();
		_net_info.clear();
	}

	inline static size_type size_size(void)
	{
		return sizeof(size_type);
	}

	inline static size_type size_container_size(void)
	{
		return size_container_type::size();
	}

	inline size_type net_info_size(void) const
	{
		return _net_info.buf_size();
	}

	inline static size_type buf_size(void)
	{
		return 0;
	}

	inline static size_type limit_buf_size(void)
	{
		return mplex::numeric_limits<size_type>::max_type::value;
	}

	inline net_info_type& net_info(void)
	{
		return _net_info;
	}

	inline const net_info_type& net_info(void) const
	{
		return _net_info;
	}

	inline net_info_type& owner_info(void)
	{
		return _net_info;
	}

	inline const net_info_type& owner_info(void) const // support to task_center and action_table
	{
		return _net_info;
	}

	inline owner_id_type owner_id(void) const// support to task_center and action_table
	{
		return _net_info.owner_id();
	}

	inline u8* net_info_buf(void)
	{
		return _net_info.buf();
	}

	inline const u8* net_info_buf(void) const
	{
		return _net_info.buf();
	}

	inline size_type data_size(void) const
	{
		return static_cast<size_type>(base_type::size());
	}

	inline u8* data_buf(void)
	{
		return base_type::buf();
	}

	inline const u8* data_buf(void) const
	{
		return base_type::buf();
	}

	template<typename Time> inline
	bool time_alive(const Time& tm_step) const
	{
		return _net_info.time_alive(tm_step);
	}

	inline void update_time(void)
	{
		_net_info.update_time();
	}

	template<typename Time> inline
	void update_time(const Time& tm)
	{
		_net_info.update_time(tm);
	}

	inline size_type size(void) const
	{
		return _net_info.size() + base_type::size();
	}

public:
	inline void assign(BOOST_RV_REF(net_info_type) net_info, BOOST_RV_REF(buf_type) buf)
	{
		copy_or_move_or_swap(_net_info, boost::move(net_info));
		base_type::assign(boost::move(buf));
	}

	inline void assign(BOOST_RV_REF(net_info_type) net_info, const buf_type& buf)
	{
		copy_or_move_or_swap(_net_info, boost::move(net_info));
		base_type::assign(buf);
	}

	inline void assign(const net_info_type& net_info, BOOST_RV_REF(buf_type) buf)
	{
		_net_info = net_info;
		base_type::assign(boost::move(buf));
	}

	inline void assign(const net_info_type& net_info, const buf_type& buf)
	{
		_net_info = net_info;
		base_type::assign(buf);
	}

	template<typename InputIter> inline
	void assign(BOOST_RV_REF(net_info_type) net_info, InputIter start, InputIter end)
	{
		copy_or_move_or_swap(_net_info, boost::move(net_info));
		base_type::assign(start, end);
	}

	template<typename InputIter>
	void assign(const net_info_type& net_info, InputIter start, InputIter end)
	{
		_net_info = net_info;
		base_type::assign(start, end);
	}

	void assign(const net_info_type& net_info,
				const data_info_type& data_info,
				u32 cal_t,
				BOOST_RV_REF(buf_type) data_buf) // support to static_work_runner back
	{
		_net_info = net_info;
		base_type::assign(data_info, boost::move(data_buf));
	}

	void assign(const net_info_type& net_info,
					const data_info_type& data_info,
					u32 cal_t,
					const buf_type& data_buf) // support to static_work_runner back
	{
		_net_info = net_info;
		base_type::assign(data_info, data_buf);
	}

	inline void assign(BOOST_RV_REF(buf_type) net_info_buf,
						BOOST_RV_REF(buf_type) data_buf)
	{
		copy_or_move_or_swap(_net_info, boost::move(net_info_buf));
		base_type::assign(boost::move(net_info_buf));
	}

	inline void assign(const buf_type& net_info_buf, const buf_type& data_buf)
	{
		_net_info = net_info_buf;
		base_type::assign(data_buf);
	}

	template<typename Other_Archive_Partner> inline
	void assign(BOOST_RV_REF_BEG
					network_packet
					<
						Other_Archive_Partner,
						data_info_def_type,
						net_info_type,
						size_type,
						DataInfo_Parser,
						NetInfo_Parser,
						Base_Packet
					> BOOST_RV_REF_END packet)
	{
		_net_info = packet.net_info();
		base_type::assign(boost::move(::yggr::to_ref_helper::to_base_ref(packet)));
	}

	template<typename Other_Archive_Partner>
	void assign(const 
					network_packet
					<
						Other_Archive_Partner,
						data_info_def_type,
						net_info_type,
						size_type,
						DataInfo_Parser,
						NetInfo_Parser,
						Base_Packet
					>& packet)
	{
		_net_info = packet.net_info();
		base_type::assign(::yggr::to_ref_helper::to_base_cref(packet));
	}

	inline void assign(BOOST_RV_REF(this_type) right)
	{
		this_type::operator=(boost::move(right));
	}

	inline void assign(const this_type& right)
	{
		this_type::operator=(right);
	}

public:
	using base_type::compatibility_save;
	using base_type::save;
	using base_type::load;
	using base_type::cypher;

private:
	net_info_type _net_info;
};

} // namespace network
} // namespace yggr

#ifndef YGGR_NO_CXX11_VARIADIC_MACROS
#	define _YGGR_PP_CONCAT_PARAMS() YGGR_PP_CONCAT(DataInfo_Parser, NetInfo_Parser, Base_Packet)
#else
#	define _YGGR_PP_CONCAR_ARGS DataInfo_Parser YGGR_PP_SYMBOL_COMMA() NetInfo_Parser YGGR_PP_SYMBOL_COMMA() Base_Packet
#	define _YGGR_PP_CONCAT_PARAMS() YGGR_PP_CONCAT(_YGGR_PP_CONCAR_ARGS)
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

namespace yggr
{
namespace network
{
namespace swap_support
{

template<YGGR_PP_TEMPLATE_PARAMS_TYPES( 4, typename T ), 
			template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T1)> class DataInfo_Parser, 
			template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T2)> class NetInfo_Parser, 
			template<YGGR_PP_TEMPLATE_PARAMS_TYPES(2, typename _T3), 
                        template<YGGR_PP_TEMPLATE_PARAMS_TYPES(1, typename _T33)> class _DataInfo_Parser> 
                            class Base_Packet > inline 
void swap(YGGR_PP_TEMPLATE_TYPE2(network_packet, 
									YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), 
									_YGGR_PP_CONCAT_PARAMS())& l, 
			YGGR_PP_TEMPLATE_TYPE2(network_packet, 
									YGGR_PP_TEMPLATE_PARAMS_TYPES(4, T), 
									_YGGR_PP_CONCAT_PARAMS())& r) 
{ 
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace network
} // namespace yggr

#undef _YGGR_PP_CONCAT_PARAMS

#ifdef YGGR_NO_CXX11_VARIADIC_MACROS
#	undef _YGGR_PP_CONCAT_ARGS
#endif // YGGR_NO_CXX11_VARIADIC_MACROS

namespace std
{
	using ::yggr::network::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::swap_support::swap;
} // namespace boost

#include <boost/type_traits/is_same.hpp>

#include <yggr/task_center/support/task_real_data_getter.hpp>

namespace yggr
{
namespace task_center
{
namespace support
{

template
<
	typename Archive_Partner,
	typename DataInfoDef,
	typename NetInfo,
	typename Size,
	template<typename _DataInfoDef> class DataInfo_Parser,
	template<typename _NetInfo> class NetInfo_Parser,
	template<typename _Archive_Partner, typename _DataInfoDef,
				template<typename __DataInfoDef> class _DataInfo_Parser > class Base_Packet
>
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
	typedef 
		network::network_packet
		<
			Archive_Partner,
			DataInfoDef,
			NetInfo,
			Size,
			DataInfo_Parser,
			NetInfo_Parser,
			Base_Packet
		> task_type;

	template<typename Real_Data> inline
	Real_Data* operator()(task_type& task) const
	{
		BOOST_MPL_ASSERT((boost::is_same<Real_Data, task_type>));
		return boost::addressof(task);
	}

	template<typename Real_Data> inline
	const Real_Data* operator()(const task_type& task) const
	{
		BOOST_MPL_ASSERT((boost::is_same<Real_Data, task_type>));
		return boost::addressof(task);
	}

};

} // namespace support
} // namesapce task_center
} // namespace yggr


#endif // __YGGR_NETWORK_NETWORK_PACKET_HPP__
