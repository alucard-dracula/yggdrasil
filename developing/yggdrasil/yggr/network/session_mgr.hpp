//session_mgr.hpp

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

#ifndef __YGGR_NETWORK_SESSION_MGR_HPP__
#define __YGGR_NETWORK_SESSION_MGR_HPP__

#include <boost/asio/error.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <yggr/base/ctrl_center.hpp>
#include <yggr/base/system_code.hpp>
#include <yggr/base/error_make.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <yggr/ids/id_generator.hpp>
#include <yggr/network/type_traits/tags.hpp>

#include <yggr/ppex/friend.hpp>

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
	typedef boost::shared_ptr<session_type> session_ptr_type;

	typedef typename session_type::tag_type tag_type;
	typedef typename session_type::id_type id_type;
	typedef typename session_type::service_type service_type;
	typedef typename session_type::socket_type socket_type;
	typedef typename session_type::link_type link_type;
	typedef typename session_type::link_init_type link_init_type;
	typedef typename session_type::link_address_type link_address_type;

	typedef typename session_type::id_type key_type;
	typedef typename session_type::conn_type conn_type;
	typedef typename session_type::conn_ptr_type conn_ptr_type;

	typedef typename session_type::send_data_type send_data_type; // opak
	typedef typename session_type::recv_data_type recv_data_type; // ipak

	typedef typename session_type::data_buf_type data_buf_type;

	typedef typename session_type::endpoint_type endpoint_type;
	typedef typename session_type::protocol_type protocol_type;
	typedef typename session_type::protocol_tag_type protocol_tag_type;

	typedef typename session_type::owner_id_parser_type owner_id_parser_type;

	typedef typename session_type::close_code_type close_code_type;

	typedef safe_container::safe_unordered_map<key_type, session_ptr_type> smap_type;
	typedef typename smap_type::iterator smap_iter_type;
	typedef typename smap_type::const_iterator smap_citer_type;
	typedef typename smap_type::value_type value_type;

	typedef std::pair<smap_iter_type, bool> send_adapter_insert_result_type;

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

	//typedef ID_Genner<key_type> key_genner_type;

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
	YGGR_PP_FRIEND_TYPENAME(start_mode_type);

	typedef State_Checker state_checker_type;
	//friend typename state_checker_type;

	typedef Exception_Fixer exception_fixer_type;
	YGGR_PP_FRIEND_TYPENAME(exception_fixer_type);

	friend class system_controller::ctrl_center;

	template<typename Tag, typename Ctrl_Center>
	void register_controller(Ctrl_Center& cc)
	{
		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::invalid_argument,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::message_size,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::name_too_long,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::network_down,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::no_buffer_space,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::no_memory,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::shut_down,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::timed_out,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::connection_reset,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::connection_aborted,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		CTRL_HANDLER_PARAM_1(Tag, cc, key_type, boost::asio::error::operation_aborted,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

        CTRL_HANDLER_PARAM_1(Tag, cc, key_type, 2,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

         CTRL_HANDLER_PARAM_1(Tag, cc, key_type, 104,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		 CTRL_HANDLER_PARAM_1(Tag, cc, key_type, 121,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		 CTRL_HANDLER_PARAM_1(Tag, cc, key_type, 0x60009004,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		 CTRL_HANDLER_PARAM_1(Tag, cc, key_type, 0x60020001,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));

		 CTRL_HANDLER_PARAM_1(Tag, cc, key_type, close_code_type::value,
								system_controller::system_code::E_SESSION_SYSTEM,
								boost::bind(&this_type::handler_session_exception,
												this->shared_from_this(), _1));
	}


public:

	session_mgr(adapter_src_type& src)
		: _adp_mgr_wrap(adapter_mgr_ptr_type(new adapter_mgr_type(src)))
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

	//void init(void)
	//{
	//	hold_recv();
	//}

	static bool is_v4(void)
	{
		return session_type::is_v4();
	}

	static bool is_v6(void)
	{
		return session_type::is_v6();
	}

	void insert(conn_ptr_type pconn)
	{
		adapter_mgr_ptr_type adp_ptr = _adp_mgr_wrap.get_shared_ptr();
		if(!adp_ptr)
		{
			return;
		}

		start_mode_type start_mode;
		start_mode(pconn, *this);
	}

	void insert(conn_ptr_type pconn,
					start_mode_type& start_mode) //support udp
	{
		start_mode(pconn, *this);
	}

	bool remove(const key_type& key)
	{
		session_ptr_type sptr = _smap.use_handler(boost::bind(&this_type::handler_drop, this, _1, boost::cref(key)));

		if(!sptr)
		{
			return false;
		}

		sptr->close();
		return true;
	}

	session_ptr_type drop(const key_type& key)
	{
		return _smap.use_handler(boost::bind(&this_type::handler_drop, this, _1, boost::cref(key)));
	}

	bool empty(void) const
	{
		return _smap.empty();
	}

	size_type size(void) const
	{
		return _smap.size();
	}

	void clear_sessions(void)
	{
		_smap.use_handler(boost::bind(&this_type::handler_clear_sessions, this, _1));
	}

	void clear(void)
	{
		_smap.use_handler(boost::bind(&this_type::handler_clear, this, _1));
	}

	template<typename Real_Data>
	bool register_adapter(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter<real_data_type>(stat);
	}

	template<typename Real_Data, typename Container>
	bool register_adapter_of_container(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;
		typedef Container container_type;
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter_of_container<real_data_type, container_type>(stat);
	}

	template<typename Container, typename Send_ID, typename Recv_ID>
	bool register_adapter(const Send_ID& send_id, const Recv_ID& recv_id,
							u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Container container_type;
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter<container_type>(send_id, recv_id, stat);
	}

	template<typename Real_Data>
	bool unregister_adapter(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template unregister_adapter<Real_Data>(stat);
	}

	template<typename ID>
	bool unregister_adapter(const ID& id, u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->unregister_adapter(id, stat);
	}

	//void hold_recv(void)
	//{
	//	adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
	//	assert(ptr);
	//	if(!ptr)
	//	{
	//		return;
	//	}

	//	ptr->hold_recv(boost::bind(&this_type::adapter_mgr_handler_recv, this->shared_from_this(), _1));
	//}

	template<typename NetInfo, typename Data, typename Handler>
	bool send_packet(const key_type& key, const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		session_ptr_type ptr;
		_smap.get_value(key, ptr);
		if(!ptr)
		{
			return false;
		}

		ptr->send_packet(netinfo, data, handler);
		return true;
	}

	template<typename Handler>
	bool send_packet(const key_type& key, send_data_type& pak, const Handler& handler)
	{
		session_ptr_type ptr;
		_smap.get_value(key, ptr);
		if(!ptr)
		{
			return false;
		}

		ptr->send_packet(pak, handler);
		return true;
	}

	bool send_packet(const key_type& key, send_data_type& pak)
	{
		session_ptr_type ptr;
		_smap.get_value(key, ptr);
		if(!ptr)
		{
			return false;
		}

		ptr->send_packet(pak);
		return true;
	}

	template<typename Handler>
	bool access(const key_type& key, const Handler& handler)
	{
		return _smap.use_handler(key, handler);
	}

	template<typename Handler>
	bool access(const key_type& key, const Handler& handler) const
	{
		return _smap.use_handler(key, handler);
	}

	template<typename Handler>
	typename Handler::result_type access(const Handler& handler)
	{
		return _smap.use_handler(handler);
	}

	template<typename Handler>
	typename Handler::result_type access(const Handler& handler) const
	{
		return _smap.use_handler(handler);
	}

	template<typename Handler>
	void access_of_all(const Handler& handler)
	{
		_smap.use_handler_of_all(handler);
	}

	template<typename Handler>
	void access_of_all(const Handler& handler) const
	{
		_smap.use_handler_of_all(handler);
	}

	adapter_mgr_ptr_type get_adapter_mgr_ptr(void) const
	{
		return _adp_mgr_wrap.get_shared_ptr();
	}

	void send_to_adapter(recv_data_type& pak)
	{
		id_type id = pak.owner_id();
		session_ptr_type ptr
			= _smap.use_handler(
						boost::bind(
										&this_type::handler_send_to_adapter,
										this, _1, boost::cref(id)
									)
								);

		if(!ptr)
		{
			return;
		}

		ptr->send_to_adapter(pak);
	}

	void check_state(void)
	{
		typedef typename smap_type::base_type cont_type;

		cont_type cont;
		_smap.use_handler(
				boost::bind(
								&this_type::handler_get_all_session<cont_type>,
								this, _1, boost::ref(cont)
							)
						);

		state_checker_type checker;
		checker(cont);
	}

	template<typename Handler>
	void check_state(const Handler& handler)
	{
		typedef typename smap_type::base_type cont_type;

		cont_type cont;
		_smap.use_handler(
				boost::bind(
								&this_type::handler_get_all_session<cont_type>,
								this, _1, boost::ref(cont)
							)
						);

		state_checker_type checker;
		checker(cont, handler);
	}

private:

	template<typename Container>
	void handler_get_all_session(const typename smap_type::base_type& base,
									Container& cont)
	{
	    Container tmp(base);
		cont.swap(tmp);
	}

	template<typename Creator>
	bool handler_create(const boost::system::error_code& e,
							const Creator& creator)
	{
		if(e)
		{
			exception_fixer_type ef;
			ef(creator.get(), *this);
			creator.cancel();
			prv_throw_error(e);
			return false;
		}

		adapter_mgr_ptr_type aptr = _adp_mgr_wrap.get_shared_ptr();
		assert(aptr);
		if(!aptr)
		{
			return false;
		}

		session_ptr_type sptr = creator.template create<session_ptr_type>(aptr);
		if(!sptr)
		{
			return false;
		}

		if(!_smap.insert(	sptr->get_id(),
							sptr
						))
		{
			prv_throw_error(error_maker_type::make_error(error_maker_type::E_duplication_id));
			return false;
		}

		sptr->start();
		return true;
	}

	template<typename Creator, typename Packet>
	void handler_create_and_fix_start_packet(const boost::system::error_code& e,
													const Creator& creator, Packet& pak)
	{
		if(!handler_create(e, creator))
		{
			return;
		}

		adapter_mgr_ptr_type aptr = _adp_mgr_wrap.get_shared_ptr();
		assert(aptr);
		if(!aptr)
		{
			return;
		}

		aptr->send(pak);
	}

	void handler_session_exception(const key_type& key)
	{
		exception_fixer_type ef;
		ef(key, *this);
	}

	session_ptr_type handler_drop(typename smap_type::base_type& map, const key_type& key)
	{

		smap_iter_type iter = map.find(key);
		if(iter == map.end())
		{
			return session_ptr_type();
		}

		session_ptr_type ptr(iter->second);
		map.erase(iter);

		try
		{
			_adp_mgr_wrap.use_handler(
							boost::bind(&this_type::handler_remove_drop_adpater,
											this, _1, boost::cref(key)));
		}
		catch(const adapter_mgr_wrap_error_type&)
		{
			return ptr;
		}

		return ptr;
	}

	void handler_remove_drop_adpater(typename adapter_mgr_wrap_type::base_type& base, const key_type& key)
	{
		assert(base);
		base->template drop_task<typename task_center_type::cdt_mark_type>(key);
		base->template drop_task<typename task_center_type::rst_mark_type>(key);
	}

	session_ptr_type handler_send_to_adapter(typename smap_type::base_type& base, const id_type& id)
	{
		smap_iter_type iter = base.find(id);
		if(iter == base.end())
		{
			return session_ptr_type();
		}

		return iter->second;
	}

	void handler_clear_session(typename smap_type::base_type& base)
	{
		try
		{
			_adp_mgr_wrap.use_handler(boost::bind(&adapter_mgr_type::drop_all_users, _1));
		}
		catch(const adapter_mgr_wrap_error_type& e)
		{
		}

		//base.clear();
		for(typename smap_type::iterator i = base.begin(), isize = base.end(); i != isize; ++i)
		{
			if(i->second)
			{
				i->second->close();
			}
		}
		base.clear();
	}

	void handler_clear(typename smap_type::base_type& base)
	{
		try
		{
			_adp_mgr_wrap.use_handler(boost::bind(&adapter_mgr_type::clear, _1));
		}
		catch(const adapter_mgr_wrap_error_type&)
		{
		}

		for(typename smap_type::iterator i = base.begin(), isize = base.end(); i != isize; ++i)
		{
			if(i->second)
			{
				i->second->close();
			}
		}
		base.clear();
	}

	template<typename Err, typename Param>
	void prv_throw_error(const Err& err, u32 sys_code, const Param& param) const
	{
		exception::exception::throw_error(err, sys_code, param);
	}

	template<typename Err>
	void prv_throw_error(const Err& err) const
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
