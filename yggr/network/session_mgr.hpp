//session_mgr.hpp

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

#ifndef __YGGR_NETWORK_SESSION_MGR_HPP__
#define __YGGR_NETWORK_SESSION_MGR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/error_make.hpp>
#include <yggr/bind/bind.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/utility/args_holder_nv.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/network/type_traits/tags.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/friend.hpp>
#include <yggr/func/foo_t_info.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <yggr/system_controller/system_code.hpp>
#include <yggr/system_controller/ctrl_center.hpp>

#include <boost/asio/error.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/type_traits/is_same.hpp>

namespace yggr
{
namespace network
{

template<typename Session,
			typename Start_Mode,
			typename State_Checker,
			typename Exception_Fixer>
class session_mgr
	: public boost::enable_shared_from_this
				<
					session_mgr
					<
						Session,
						Start_Mode,
						State_Checker,
						Exception_Fixer
					>
				>,
		private nonable::noncopyable
{
private:
	typedef session_mgr this_type;

public:
	typedef Session session_type;
	typedef ::yggr::shared_ptr<session_type> session_ptr_type;

	typedef typename session_type::tag_type tag_type;
	typedef typename session_type::id_type id_type;

	typedef typename session_type::protocol_tag_type protocol_tag_type;
	typedef typename session_type::service_type service_type;
	typedef typename session_type::socket_type socket_type;
	typedef typename session_type::endpoint_type endpoint_type;
	typedef typename session_type::protocol_type protocol_type;
	typedef typename session_type::link_address_type link_address_type;
	typedef typename session_type::deadline_timer_type deadline_timer_type;

	typedef typename session_type::link_type link_type;
	typedef typename session_type::link_init_type link_init_type;

	typedef typename session_type::id_type key_type;
	typedef typename session_type::conn_type conn_type;
	typedef typename session_type::conn_ptr_type conn_ptr_type;

	typedef typename session_type::send_data_type send_data_type; // opak
	typedef typename session_type::recv_data_type recv_data_type; // ipak

	typedef typename session_type::data_buf_type data_buf_type;

	typedef typename session_type::owner_id_parser_type owner_id_parser_type;

	typedef typename session_type::close_code_type close_code_type;

	typedef safe_container::safe_unordered_map<key_type, session_ptr_type> smap_type;
	typedef typename smap_type::iterator smap_iter_type;
	typedef typename smap_type::const_iterator smap_citer_type;
	typedef typename smap_type::value_type value_type;

	typedef typename session_type::adapter_mgr_type adapter_mgr_type;
	typedef typename session_type::adapter_mgr_wrap_type adapter_mgr_wrap_type;
	typedef typename session_type::adapter_mgr_ptr_type adapter_mgr_ptr_type;
	typedef typename session_type::adapter_mgr_wrap_error_type adapter_mgr_wrap_error_type;

	typedef typename adapter_mgr_type::src_type adapter_src_type;
	typedef typename adapter_mgr_type::reg_def_type adapter_mgr_reg_def_type;
	typedef typename adapter_mgr_type::send_std_data_type adapter_send_std_data_type; //ipak
	typedef typename adapter_mgr_type::recv_rst_data_type adapter_recv_rst_data_type; // opak
	typedef typename adapter_mgr_type::src_type task_center_type;
	typedef typename adapter_mgr_type::task_type task_type;

private:
	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

	BOOST_MPL_ASSERT((boost::is_same<recv_data_type, adapter_send_std_data_type>));
	BOOST_MPL_ASSERT((boost::is_same<send_data_type, adapter_recv_rst_data_type>));

	typedef typename session_type::error_maker_type session_error_maker_type;
	typedef typename session_type::error_type session_error_type;


	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("session_mgr")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_6)
			ERROR_CODE_DEF(E_invalid_session)
			ERROR_CODE_DEF(E_duplication_id)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_6)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_session, "invalid session")
			ERROR_CODE_MSG(E_duplication_id, "duplication id")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------

	typedef Start_Mode start_mode_type ;
	//YGGR_PP_FRIEND_TYPENAME(start_mode_type);

	typedef State_Checker state_checker_type;
	//friend typename state_checker_type;

	typedef Exception_Fixer exception_fixer_type;
	YGGR_PP_FRIEND_TYPENAME(exception_fixer_type);

private:
	friend class system_controller::ctrl_center;

	template<typename Tag, typename CtrlCenter>
	void register_controller(CtrlCenter& cc)
	{
		CTRL_HANDLER(Tag, cc, boost::asio::error::invalid_argument,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::message_size,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::name_too_long,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::network_down,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::no_buffer_space,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::no_memory,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::shut_down,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::timed_out,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_reset,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::connection_aborted,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, boost::asio::error::operation_aborted,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, 2,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));

		if(boost::asio::error::connection_reset != 104) // windows not equal see boost asio/erron.h
		{
			CTRL_HANDLER(Tag, cc, 104,
						system_controller::system_code::E_SESSION_SYSTEM,
						boost::bind(&this_type::handler_session_exception,
						this_type::shared_from_this(), _1));
		}

		CTRL_HANDLER(Tag, cc, 121,
					system_controller::system_code::E_SESSION_SYSTEM,
					boost::bind(&this_type::handler_session_exception,
					this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, 0x60009004,
					system_controller::system_code::E_SESSION_SYSTEM,
					boost::bind(&this_type::handler_session_exception,
					this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, 0x60020001,
					system_controller::system_code::E_SESSION_SYSTEM,
					boost::bind(&this_type::handler_session_exception,
					this_type::shared_from_this(), _1));

		CTRL_HANDLER(Tag, cc, close_code_type::value,
					system_controller::system_code::E_SESSION_SYSTEM,
					boost::bind(&this_type::handler_session_exception,
					this_type::shared_from_this(), _1));
	}

public:

	session_mgr(adapter_src_type& src)
		: _adp_mgr_wrap(yggr_nothrow_new adapter_mgr_type(src))
	{
		assert(_adp_mgr_wrap.get_shared_ptr());
	}

	session_mgr(adapter_mgr_ptr_type padp)
		: _adp_mgr_wrap(padp)
	{
		assert(padp);
	}

	~session_mgr(void)
	{
		assert(_smap.empty());
	}

public:
	inline static bool is_v4(void)
	{
		return session_type::is_v4();
	}

	inline static bool is_v6(void)
	{
		return session_type::is_v6();
	}

public:
	inline void insert(conn_ptr_type pconn)
	{
		adapter_mgr_ptr_type adp_ptr = _adp_mgr_wrap.get_shared_ptr();
		if(!adp_ptr)
		{
			return;
		}

		start_mode_type start_mode;
		start_mode(pconn, *this);
	}

	inline void insert(conn_ptr_type pconn, start_mode_type& start_mode) //support udp
	{
		start_mode(pconn, *this);
	}

	template<typename Other_Start_Mode> inline
	void insert(conn_ptr_type pconn, Other_Start_Mode& start_mode) // support p2p
	{
		start_mode(pconn, *this);
	}

	inline bool remove(const key_type& key)
	{
		session_ptr_type sptr;
		_smap.get_value(key, sptr);

		return sptr && (_smap.erase(key), sptr->close(), true);
	}

	inline session_ptr_type drop(const key_type& key)
	{
		session_ptr_type sptr;
		_smap.get_value(key, sptr);

		return sptr? (_smap.erase(key), sptr) : sptr;
	}

	inline bool empty(void) const
	{
		return _smap.empty();
	}

	inline size_type size(void) const
	{
		return _smap.size();
	}

	void clear_sessions(void)
	{
		typedef typename smap_type::base_type cont_type;
		cont_type tmp;
		_smap.swap(tmp);

		for(typename smap_type::iterator i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
		{
			if(i->second)
			{
				i->second->close();
			}
		}

	}

	void clear(void)
	{
		typedef typename smap_type::base_type cont_type;
		cont_type tmp;
		_smap.swap(tmp);

		for(typename smap_type::iterator i = tmp.begin(), isize = tmp.end(); i != isize; ++i)
		{
			if(i->second)
			{
				i->second->close();
			}
		}

		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		if(ptr)
		{
			ptr->clear();
		}
	}

public:
	template<typename Real_Data> inline
	bool register_adapter(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;

		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter<real_data_type>(stat);
	}

	template<typename Real_Data, typename Container> inline
	bool register_adapter_of_container(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;
		typedef Container container_type;

		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter_of_container<real_data_type, container_type>(stat);
	}

	template<typename Container, typename Send_ID, typename Recv_ID> inline
	bool register_adapter(const Send_ID& send_id, const Recv_ID& recv_id,
							u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Container container_type;

		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter<container_type>(send_id, recv_id, stat);
	}

	template<typename Real_Data> inline
	bool unregister_adapter(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template unregister_adapter<Real_Data>(stat);
	}

	template<typename ID> inline
	bool unregister_adapter(const ID& id,
								u32 tag = adapter_mgr_type::template default_tag<ID>::value,
								u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->unregister_adapter(id, tag, stat);
	}

public:
	//send_packet
	template<typename NetInfo, typename Data> inline
	bool send_packet(const key_type& key, const NetInfo& netinfo, const Data& data)
	{
		session_ptr_type ptr;
		_smap.get_value(key, ptr);

		return ptr && (ptr->send_packet(netinfo, data), true);
	}

	template<typename NetInfo, typename Data, typename Handler> inline
	bool send_packet(const key_type& key, const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		session_ptr_type ptr;
		_smap.get_value(key, ptr);

		return ptr && (ptr->send_packet(netinfo, data, handler), true);
	}

	template<typename Handler> inline
	bool send_packet(const key_type& key, send_data_type& pak, const Handler& handler)
	{
		session_ptr_type ptr;
		_smap.get_value(key, ptr);

		return ptr && (ptr->send_packet(pak, handler), true);
	}

	inline bool send_packet(const key_type& key, send_data_type& pak)
	{
		session_ptr_type ptr;
		_smap.get_value(key, ptr);

		return ptr && (ptr->send_packet(pak), true);
	}

	template<typename Handler> inline
	bool access(const key_type& key, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;

		handler_holder_type handler_holder(handler);
		return
			_smap.find(
				key,
				boost::bind(
					&this_type::prv_s_handler_find_session_ref<handler_type>,
					_1, _2, boost::cref(utility::args_holder_trans(handler_holder))));
	}

public:
	template<typename Handler> inline
	bool access(const key_type& key, const Handler& handler) const
	{
		typedef Handler handler_type;
		typedef utility::args_holder_nv_ref_wrap<handler_type const> handler_holder_type;

		handler_holder_type handler_holder(handler);
		return
			_smap.find(
				key,
				boost::bind(
					&this_type::prv_s_handler_find_session_cref<handler_type>,
					_1, _2, boost::cref(args_holder_trans(handler_holder))));
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		access(const Handler& handler)
	{
		return _smap.using_handler(handler);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		access(const Handler& handler) const
	{
		return _smap.using_handler(handler);
	}

	template<typename Handler> inline
	void access_of_all(const Handler& handler)
	{
		_smap.for_each_iterator(handler);
	}

	template<typename Handler> inline
	void access_of_all(const Handler& handler) const
	{
		_smap.for_each_iterator(handler);
	}

	inline adapter_mgr_ptr_type get_adapter_mgr_ptr(void) const
	{
		return _adp_mgr_wrap.get_shared_ptr();
	}

	inline void send_to_adapter(recv_data_type& pak)
	{
		id_type id = pak.owner_id();
		session_ptr_type ptr;
		_smap.get_value(id, ptr);

		if(ptr)
		{
			ptr->send_to_adapter(pak);
		}
	}

	inline void check_state(void)
	{
		typedef typename smap_type::base_type cont_type;

		cont_type cont = _smap;

		state_checker_type checker;
		checker(cont);
	}

	template<typename Handler> inline
	void check_state(const Handler& handler)
	{
		typedef typename smap_type::base_type cont_type;

		cont_type cont = _smap;
		state_checker_type checker;
		checker(cont, handler);
	}

	template<typename Creator>
	session_ptr_type create_session_only(const boost::system::error_code& e, const Creator& creator)
	{
		if(e)
		{
			exception_fixer_type ef;
			ef(creator.get(), *this);
			creator.cancel();
			this_type::prv_s_throw_error(e);
			return session_ptr_type();
		}

		adapter_mgr_ptr_type aptr = _adp_mgr_wrap.get_shared_ptr();
		assert(aptr);
		if(!aptr)
		{
			return session_ptr_type();
		}

		session_ptr_type sptr = creator.template create<session_ptr_type>(aptr);
		if(!sptr)
		{
			return session_ptr_type();
		}

#ifdef _DEBUG
		std::cout << "insert sid = " << sptr->get_id() << std::endl;
#endif // _DEBUG

		if(!_smap.insert(sptr->get_id(), sptr))
		{
			this_type::prv_s_throw_error(error_maker_type::make_error(error_maker_type::E_duplication_id));
			return session_ptr_type();
		}
		else
		{
			return sptr;
		}
	}

	template<typename Packet> inline
	bool direct_send_to_adapter(Packet& pak)
	{
		adapter_mgr_ptr_type aptr = _adp_mgr_wrap.get_shared_ptr();
		assert(aptr);

		return aptr && (aptr->send(pak), true);
	}

private:

	template<typename Handler>
	static bool prv_s_handler_find_session_ref(typename smap_type::base_type& base,
												typename smap_type::iterator rst,
												const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;
		if(rst == base.end())
		{
			return false;
		}
		else
		{
			const handler_type* phandler =
					utility::args_holder_ref_wrap_static_cast<handler_type const>(any_handler);

			return phandler && ((*phandler)(base, rst), true);
		}
	}

	template<typename Handler>
	static bool prv_s_handler_find_session_cref(const typename smap_type::base_type& base,
												typename smap_type::const_iterator rst,
												const basic_args_holder_nv_type& any_handler)
	{
		typedef Handler handler_type;
		if(rst == base.end())
		{
			return false;
		}
		else
		{
			const handler_type* phandler =
					utility::args_holder_ref_wrap_static_cast<handler_type const>(any_handler);

			return phandler && ((*phandler)(base, rst), true);
		}
	}

	void handler_session_exception(const key_type& key)
	{
		exception_fixer_type ef;
		ef(key, *this);
	}

	template<typename Err, typename Param> inline
	static void prv_s_throw_error(const Err& err, u32 sys_code, const Param& param)
	{
		exception::exception::throw_error(err, sys_code, param);
	}

	template<typename Err> inline
	static void prv_s_throw_error(const Err& err)
	{
		exception::exception::throw_error(err);
	}

private:
	smap_type _smap;
	adapter_mgr_wrap_type _adp_mgr_wrap;
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_SESSION_MGR_HPP__
