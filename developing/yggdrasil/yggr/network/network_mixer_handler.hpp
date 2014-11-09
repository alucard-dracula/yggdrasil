//netowrk_handler_mixer.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_HANDLER_MIXER_HPP__
#define __YGGR_NETWORK_NETWORK_HANDLER_MIXER_HPP__

#include <cassert>
#include <sstream>
#include <utility>

#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>
#include <boost/functional/hash/hash.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/count.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/smart_ptr/enable_shared_from_raw.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/ppex/foo_params.hpp>

#include <yggr/mplex/foo_type_info.hpp>
#include <yggr/mplex/conv_to_tuple.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>

#include <yggr/struct_support/tuple.hpp>
#include <yggr/serialization/tuple.hpp>

#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/safe_container/safe_list.hpp>

namespace yggr
{
namespace network
{

#define YGGR_PP_TEMPLATE_DEFAULT_TYPE() boost::tuples::null_type
	template< typename ServiceHandler,
				typename Adapter_Mgr,
				typename Mutex = boost::mutex,
				YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN, typename Val )
				>
#undef YGGR_PP_TEMPLATE_DEFAULT_TYPE
class network_mixer_handler
	: public boost::enable_shared_from_raw
{
public:
	typedef ServiceHandler service_handler_type;
	typedef Adapter_Mgr adapter_mgr_type;

	typedef typename adapter_mgr_type::reg_def_type adapter_mgr_reg_def_type;

private:
	typedef Mutex mutex_type;

	// ------------------------types dedunce and check start--------------------------
	typedef boost::tuples::null_type null_type;
	typedef boost::mpl::vector
			<
				YGGR_PP_TEMPLATE_PARAMS_TYPES(
					YGGR_PP_TEMPLATE_DEFAULT_PARAMS_LEN,
					Val)
			> tmp_vt_t_type;
	typedef typename boost::mpl::find<tmp_vt_t_type, null_type>::type i;
	typedef typename boost::mpl::erase
						<
							tmp_vt_t_type,
							i,
							typename boost::mpl::end<tmp_vt_t_type>::type
						>::type vt_t_type;

	template<typename Tvt, u32 i = 0, u32 isize = boost::mpl::size<Tvt>::value>
	struct check_unique_types
	{
		typedef Tvt tvt_type;
		typedef typename boost::mpl::at_c<tvt_type, i>::type now_handler_type;
		typedef typename boost::mpl::count<tvt_type, now_handler_type>::type count_type;
		typedef boost::mpl::bool_<((bool)(count_type::value))> type;
		BOOST_MPL_ASSERT((type));
		BOOST_MPL_ASSERT((check_unique_types<tvt_type, i + 1, isize>));
	};

	template<typename Tvt, u32 i>
	struct check_unique_types<Tvt, i, i>
	{
		typedef boost::mpl::bool_<true> type;
	};

	BOOST_MPL_ASSERT((check_unique_types<vt_t_type>));

	typedef boost::mpl::vector<> empty_containers_type;

	template<typename Vt, u32 i = 0, u32 isize = boost::mpl::size<vt_t_type>::value>
	struct create_containers
	{
		typedef Vt vt_type;
		typedef vt_t_type tvt_type;
		typedef typename boost::mpl::at_c<tvt_type, i>::type now_handler_type;
		typedef boost::shared_ptr<now_handler_type> now_handler_ptr_type;
		typedef yggr::safe_container::safe_list<now_handler_ptr_type, mutex_type> now_handler_ptr_list_type;
		typedef boost::shared_ptr<now_handler_ptr_list_type> now_handler_ptr_list_ptr_type;
		typedef typename boost::mpl::push_back< vt_type, now_handler_ptr_list_ptr_type >::type now_vt_type;

		typedef typename create_containers<now_vt_type, i + 1, isize>::type type;
	};

	template<typename Vt, u32 i>
	struct create_containers<Vt, i, i>
	{
		typedef Vt type;
	};

	typedef typename create_containers
						<
							empty_containers_type
						>::type containers_type;

	BOOST_MPL_ASSERT((
						boost::mpl::bool_
						<
							(
								boost::mpl::size<containers_type>::value
								== boost::mpl::size<vt_t_type>::value
							)
						>
					));

	typedef containers_type params_type;
	typedef typename mplex::conv_to_tuple<params_type, null_type>::type value_type;
	typedef yggr::safe_container::safe_wrap<value_type> value_wrap_type;
	typedef typename value_wrap_type::value_ptr_type value_ptr_type;
	typedef typename value_wrap_type::error_type value_wrap_error_type;

	// ------------------------types dedunce and check end--------------------------

	typedef yggr::safe_container::safe_wrap<adapter_mgr_type> adapter_mgr_wrap_type;
	typedef typename adapter_mgr_wrap_type::value_ptr_type adapter_mgr_ptr_type;
	typedef typename adapter_mgr_wrap_type::error_type adapter_mgr_wrap_error_type;

	template<typename Lkh_Init>
	struct t_init
	{
		typedef Lkh_Init lkh_init_type;

		t_init(adapter_mgr_ptr_type padp, const lkh_init_type& lkh_init)
			: _port(0), _padp(padp), _lkh_init(lkh_init)
		{
		}

		t_init(u16 port, adapter_mgr_ptr_type padp, const lkh_init_type& lkh_init)
			: _port(port), _padp(padp), _lkh_init(lkh_init)
		{
		}

		~t_init(void)
		{
		}

		const u16& port(void) const
		{
			return _port;
		}

		adapter_mgr_ptr_type session_mgr_init(void) const
		{
			return _padp;
		}

		const lkh_init_type& lkh_init(void) const
		{
			return _lkh_init;
		}

	private:
		adapter_mgr_ptr_type _padp;

		u16 _port;
		lkh_init_type _lkh_init;

	};

private:
	typedef network_mixer_handler this_type;

private:
	template<u32 i = 0, u32 isize = boost::mpl::size<params_type>::value>
	struct init_value_op
	{
		typedef params_type t_conts_type;
		void operator()(value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

			typedef typename cont_type::value_type val_type;
			typedef yggr::safe_container::safe_list<val_type, mutex_type> chk_cont_type;

			assert(pval);
			{
                cont_ptr_type tmp(new cont_type());
                boost::get<i>(*pval).swap(tmp);
			}
			init_value_op< i + 1, isize> op;
			op(pval);
		}
	};

	template<u32 i>
	struct init_value_op<i, i>
	{
		void operator()(value_ptr_type pval) const
		{
			assert(pval);
			return;
		}
	};

public:

	template<typename Service_Handler_Init, typename Source>
	network_mixer_handler(const Service_Handler_Init& service_init,
							const Source& src)
		: _service_handler(service_init),
			_adp_mgr_wrap(adapter_mgr_ptr_type(new adapter_mgr_type(const_cast<Source&>(src)))),
			_value_wrap(value_ptr_type(new value_type()))
	{
		value_ptr_type ptr(_value_wrap.get_shared_ptr());
		assert(ptr);
		init_value_op<> op;
		op(ptr);
	}

	~network_mixer_handler(void)
	{
	}

	template<typename Real_Data>
	bool register_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter<real_data_type>(stat);
	}

	template<typename Real_Data, typename Container>
	bool register_network_protocol_of_container(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;
		typedef Container container_type;
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter_of_container<real_data_type, container_type>(stat);
	}

	template<typename Container, typename Send_ID, typename Recv_ID>
	bool register_network_protocol(const Send_ID& send_id, const Recv_ID& recv_id,
							u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Container container_type;
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter<container_type>(send_id, recv_id, stat);
	}

	template<typename Real_Data>
	bool unregister_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template unregister_adapter<Real_Data>(stat);
	}

	template<typename ID>
	bool unregister_network_protocol(const ID& id, u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->unregister_adapter(id, stat);
	}

	void add_signal(u32 sig)
	{
		_service_handler.add_signal(sig);
	}

	void join(void)
	{
		_service_handler.join();
	}

private:
	template<
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct run_op
	{
		typedef params_type t_conts_type;

		void operator()(value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);


			pcont->use_handler(boost::bind(&run_op::handler_op<cont_type>, this, _1));
			run_op<i + 1, isize> op;
			op(pval);
		}

	private:
		template<typename Container>
		void handler_op(typename Container::base_type& base) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;

			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if(*i)
				{
					(*i)->run();
				}
			}
		}
	};

	template<u32 i>
	struct run_op<i, i >
	{
		inline void operator()(value_ptr_type pval) const
		{
			assert(pval);
		}
	};

public:
	void run(void)
	{
	    _service_handler.add_signal(SIGINT);
		_service_handler.add_signal(SIGTERM);
#   if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGBREAK);
#   endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGFPE);
		_service_handler.add_signal(SIGILL);
		_service_handler.add_signal(SIGABRT);
#	if defined(SIGQUIT)
		_service_handler.add_signal(SIGQUIT);
#	endif // defined(SIGQUIT)

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		_service_handler.set_signal_handler(boost::bind(&this_type::stop, this));

		run_op<> op;
		op(pval);
		_service_handler.run();
	}

	template<typename Handler>
	void run(const Handler& handler)
	{
	    _service_handler.add_signal(SIGINT);
		_service_handler.add_signal(SIGTERM);
#   if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGBREAK);
#   endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGFPE);
		_service_handler.add_signal(SIGILL);
		_service_handler.add_signal(SIGABRT);
#	if defined(SIGQUIT)
		_service_handler.add_signal(SIGQUIT);
#	endif // defined(SIGQUIT)

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		_service_handler.set_signal_handler(handler);

		run_op<> op;
		op(pval);
		_service_handler.run();
	}

private:
	template<
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct stop_op
	{
		typedef params_type t_conts_type;

		void operator()(value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);


			pcont->use_handler(boost::bind(&stop_op::handler_op<cont_type>, this, _1));
			stop_op< i + 1, isize > op;
			op(pval);
		}

	private:
		template<typename Container>
		void handler_op(typename Container::base_type& base) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;

			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if(*i)
				{
					(*i)->stop();
				}
			}
		}
	};

	template< u32 i>
	struct stop_op< i, i >
	{
		inline void operator()(value_ptr_type pval) const
		{
			assert(pval);
		}
	};

public:
	void stop(void)
	{
		_service_handler.stop();

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		stop_op<> op;
		op(pval);
	}

private:
	template<
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct online_size_op
	{
		typedef params_type t_conts_type;

		size_type operator()(value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);

			online_size_op<i + 1, isize> op;
			return pcont->use_handler(boost::bind(&online_size_op::handler_op<cont_type>, this, _1)) + op(pval);
		}

	private:
		template<typename Container>
		size_type handler_op(typename Container::base_type& base) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;

			size_type size = 0;
			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if(*i)
				{
					size += (*i)->online_size();
				}
			}

			return size;
		}
	};

	template<u32 i>
	struct online_size_op< i, i >
	{
		inline size_type operator()(value_ptr_type pval) const
		{
			assert(pval);
			return 0;
		}
	};

public:

	size_type online_size(void) const
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		online_size_op<> op;
		return op(pval);
	}

private:
	template<
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct clear_sessions_op
	{
		typedef params_type t_conts_type;

		void operator()(value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);
			pcont->use_handler(boost::bind(&clear_sessions_op::handler_op<cont_type>, this, _1));

			clear_sessions_op<i + 1, isize> op;
			op(pval);
		}

	private:
		template<typename Container>
		void handler_op(typename Container::base_type& base) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;
			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if(*i)
				{
					(*i)->clear_sessions();
				}
			}
		}
	};

	template< u32 i >
	struct clear_sessions_op< i, i >
	{
		inline void operator()(value_ptr_type pval) const
		{
			assert(pval);
		}
	};

public:
	void clear_sessions(void)
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		clear_sessions_op<> op;
		op(pval);
	}

private:
	template<
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct clear_op
	{
		typedef params_type t_conts_type;

		void operator()(value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);
			pcont->use_handler(boost::bind(&clear_op::handler_op<cont_type>, this, _1));

			clear_op<i + 1, isize> op;
			op(pval);
		}

	private:
		template<typename Container>
		void handler_op(typename Container::base_type& base) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;
			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if(*i)
				{
					(*i)->clear();
				}
			}
		}
	};

	template<u32 i>
	struct clear_op< i, i >
	{
		inline void operator()(value_ptr_type pval) const
		{
			assert(pval);
		}
	};

public:

	void clear(void)
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		clear_op<> op;
		op(pval);
	}

	service_handler_type& service_handler(void)
	{
		return _service_handler;
	}

private:
	template<
				typename Handler_Tag, typename Protocol_Tag,
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct check_state_op
	{
		typedef params_type t_conts_type;
		typedef Handler_Tag handler_tag_type;
		typedef Protocol_Tag protocol_tag;

		void operator()(value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
			typedef typename cont_type::value_type op_handler_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<op_handler_ptr_type>::type op_handler_type;
			typedef typename op_handler_type::tag_type now_handler_tag_type;
			typedef typename op_handler_type::protocol_tag_type now_protocol_tag_type;
			typedef typename boost::is_same<handler_tag_type, now_handler_tag_type>::type handler_tag_checker_type;
			typedef typename boost::is_same<protocol_tag, now_protocol_tag_type>::type protocol_tag_checker_type;

			assert(pval);

			if(handler_tag_checker_type::value && protocol_tag_checker_type::value)
			{
				cont_ptr_type pcont = boost::get<i>(*pval);
				assert(pcont);
				pcont->use_handler(boost::bind(&check_state_op::handler_op<cont_type>, this, _1));
			}

			check_state_op<handler_tag_type, protocol_tag, i + 1, isize> op;
			op(pval);
		}

	private:
		template<typename Container>
		void handler_op(typename Container::base_type& base) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;
			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if(*i)
				{
					(*i)->check_state();
				}
			}
		}
	};

	template<typename Handler_Tag, typename Protocol_Tag, u32 i>
	struct check_state_op< Handler_Tag, Protocol_Tag, i, i >
	{
		inline void operator()(value_ptr_type pval) const
		{
			assert(pval);
		}
	};

public:
	template<typename Handler_Tag, typename Protocol_Tag>
	void check_state(void)
	{
		typedef Handler_Tag handler_tag_type;
		typedef Protocol_Tag protocol_tag_type;

		typedef check_state_op<handler_tag_type, protocol_tag_type> check_state_op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		check_state_op_type op;
		op(pval);
	}

	template<typename OP_Handler, typename Lkh_Init>
	bool append(const Lkh_Init& lkh_init, u16 port = 0)
	{
		typedef OP_Handler op_handler_type;
		typedef Lkh_Init lkh_init_type;
		typedef t_init<lkh_init_type> init_type;

		typedef typename boost::mpl::find<vt_t_type, op_handler_type>::type find_iter_type;

		BOOST_MPL_ASSERT((
							boost::mpl::if_
							<
								typename boost::is_same
											<
												find_iter_type,
												boost::mpl::end<vt_t_type>
											>::type,
								boost::mpl::false_,
								boost::mpl::true_
							>
						));

		typedef typename boost::mpl::at_c<params_type, find_iter_type::pos::value>::type cont_ptr_type;
		typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
		typedef typename cont_type::value_type op_handler_ptr_type;

		typedef typename yggr::mplex::pointer_to_value_t<op_handler_ptr_type>::type check_op_handler_type;
		BOOST_MPL_ASSERT((boost::is_same<op_handler_type, check_op_handler_type>));


		adapter_mgr_ptr_type padp(_adp_mgr_wrap.get_shared_ptr());

		assert(padp);
		if(!padp)
		{
			return false;
		}

		init_type init(port, padp, lkh_init);

		value_ptr_type pval(_value_wrap.get_shared_ptr());

		assert(pval);
		if(!pval)
		{
			return false;
		}

		cont_ptr_type pcont = boost::get<find_iter_type::pos::value>(*pval);
		if(!pcont)
		{
			return false;
		}

		op_handler_ptr_type phandler(new op_handler_type(_service_handler, init));
		pcont->push_back(phandler);

		if(_service_handler.is_running())
		{
			phandler->run();
		}
		return true;
	}

private:
	template<
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct send_op
	{
		typedef params_type t_conts_type;

		template<typename Key, typename NetInfo, typename Data, typename Handler>
		bool operator()(const Key& key, const NetInfo& netinfo, const Data& data,
							const Handler& handler, value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
			typedef Key key_type;
			typedef NetInfo netinfo_type;
			typedef Data data_type;
			typedef Handler handler_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);

			send_op<i + 1, isize> op;

			return pcont->use_handler(boost::bind(&send_op::template handler_op_drei
                                                    <
														cont_type,
														key_type,
														netinfo_type,
														data_type,
														handler_type
													>,
													this, _1,
													boost::cref(key),
													boost::cref(netinfo),
													boost::cref(data),
													boost::any(handler)))
					|| op(key, netinfo, data, handler, pval);
		}

		template<typename Key, typename Pak, typename Handler>
		bool operator()(const Key& key, Pak& pak, const Handler& handler, value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
			typedef Key key_type;
			typedef Pak pak_type;
			typedef Handler handler_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);

			send_op<i + 1, isize> op;

			return pcont->use_handler(boost::bind(&send_op::template handler_op_zwei
													<
														cont_type,
														key_type,
														pak_type,
														handler_type
													>,
													this, _1,
													boost::cref(key),
													boost::ref(pak),
													boost::any(handler)))
					|| op(key, pak, handler, pval);
		}

		template<typename Key, typename Pak>
		bool operator()(const Key& key, Pak& pak, value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
			typedef Key key_type;
			typedef Pak pak_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);

			send_op<i + 1, isize> op;

			return pcont->use_handler(boost::bind(&send_op::template handler_op_eins
													<
														cont_type,
														key_type,
														pak_type
													>,
													this, _1,
													boost::cref(key),
													boost::ref(pak)))
					|| op(key, pak, pval);
		}

	private:
		template<typename Container, typename Key, typename NetInfo, typename Data, typename Handler>
		bool handler_op_drei(typename Container::base_type& base,
								const Key& key, const NetInfo& netinfo,
								const Data& data, const boost::any& handler_wrap) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;

			typedef Key key_type;
			typedef NetInfo netinfo_type;
			typedef Data data_type;
			typedef Handler handler_type;

			const handler_type* phandler = boost::any_cast<handler_type>(&handler_wrap);

			if(!phandler)
			{
				return false;
			}

			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if((*i) && ((*i)->template send_packet<
												key_type,
												netinfo_type,
												data_type,
												handler_type>(key, netinfo, data, *phandler)))
				{
					return true;
				}
			}
			return false;
		}

		template<typename Container, typename Key, typename Pak, typename Handler>
		bool handler_op_zwei(typename Container::base_type& base,
								const Key& key, Pak& pak, const boost::any& handler_wrap) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;

			typedef Key key_type;
			typedef Pak pak_type;
			typedef Handler handler_type;

			const handler_type* phandler = boost::any_cast<handler_type>(&handler_wrap);

			if(!phandler)
			{
				return false;
			}

			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if((*i) && ((*i)->template send_packet<key_type, pak_type>(key, pak, *phandler)))
				{
					return true;
				}
			}
			return false;
		}

		template<typename Container, typename Key, typename Pak>
		bool handler_op_eins(typename Container::base_type& base, const Key& key, Pak& pak) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;

			typedef Key key_type;
			typedef Pak pak_type;

			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if((*i) && ((*i)->template send_packet<key_type, pak_type>(key, pak)))
				{
					return true;
				}
			}
			return false;
		}
	};

	template<u32 i>
	struct send_op< i, i >
	{
		template<typename Key, typename NetInfo, typename Data, typename Handler>
		bool operator()(const Key& key, const NetInfo& netinfo, const Data& data,
							const Handler& handler, value_ptr_type pval) const
		{
			assert(pval);
			return false;
		}

		template<typename Key, typename Pak, typename Handler>
		bool operator()(const Key& key, Pak& pak, const Handler& handler, value_ptr_type pval) const
		{
			assert(pval);
			return false;
		}

		template<typename Key, typename Pak>
		bool operator()(const Key& key, Pak& pak, value_ptr_type pval) const
		{
			assert(pval);
			return false;
		}
	};

public:
	template<typename Key, typename NetInfo, typename Data, typename Handler>
	bool send_packet(const Key& key, const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		send_op<> op;
		return op(key, netinfo, data, handler, pval);
	}

	template<typename Key, typename Pak, typename Handler>
	bool send_packet(const Key& key, Pak& pak, const Handler& handler)
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		send_op<> op;
		return op(key, pak, handler, pval);
	}

	template<typename Key, typename Pak>
	bool send_packet(const Key& key, Pak& pak)
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		send_op<> op;
		return op(key, pak, pval);
	}

private:
	template<
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct access_op_eins
	{
		typedef params_type t_conts_type;

		template<typename Key, typename Handler>
		bool operator()(const Key& key, const Handler& handler, value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
			typedef Key key_type;
			typedef Handler handler_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);

			access_op_eins<i + 1, isize> op;

			return pcont->use_handler(boost::bind(&access_op_eins::handler_op
													<
														cont_type,
														key_type,
														handler_type
													>,
													this, _1,
													boost::cref(key),
													boost::any(handler)))
					|| op(key, handler, pval);
		}

	private:
		template<typename Container, typename Key, typename Handler>
		bool handler_op(typename Container::base_type& base, const Key& key, const boost::any& handler_wrap) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;

			typedef Key key_type;
			typedef Handler handler_type;

			const handler_type* phandler = boost::any_cast<handler_type>(&handler_wrap);

			if(!phandler)
			{
				return false;
			}

			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if((*i) && ((*i)->template access<key_type, handler_type>(key, *phandler)))
				{
					return true;
				}
			}
			return false;
		}
	};

	template<u32 i>
	struct access_op_eins< i, i >
	{
		template<typename Key, typename Handler>
		inline bool operator()(const Key& key, const Handler& handler, value_ptr_type pval) const
		{
			assert(pval);
			return false;
		}
	};

public:
	template<typename Key, typename Handler>
	bool access(const Key& key, const Handler& handler)
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		access_op_eins<> op;
		return op(key, handler, pval);
	}

	template<typename Key, typename Handler>
	bool access(const Key& key, const Handler& handler) const
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		access_op_eins<> op;
		return op(key, handler, pval);
	}

private:
	template<
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct access_op_zwei
	{
		typedef params_type t_conts_type;

		template<typename Handler>
		typename Handler::result_type operator()(const Handler& handler, value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
			typedef Handler handler_type;
			typedef typename handler_type::result_type ret_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);

			access_op_zwei<i + 1, isize> op;

			ret_type ret = pcont->use_handler(boost::bind(&access_op_zwei::handler_op
													<
														cont_type,
														handler_type
													>,
													this, _1,
													boost::any(handler)));

			if(!ret)
			{
				return op(handler, pval);
			}

			return ret;
		}

	private:
		template<typename Container, typename Handler>
		typename Handler::result_type handler_op(typename Container::base_type& base, const boost::any& handler_wrap) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;
			typedef Handler handler_type;
			typedef typename handler_type::result_type ret_type;

			const handler_type* phandler = boost::any_cast<handler_type>(&handler_wrap);

			if(!phandler)
			{
				return ret_type();
			}

			ret_type ret = ret_type();
			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if((*i) && (ret = ((*i)->template access<handler_type>(*phandler))))
				{
					return ret;
				}
			}
			return ret_type();
		}
	};

	template<u32 i>
	struct access_op_zwei< i, i >
	{
		template<typename Handler>
		inline typename Handler::result_type operator()(const Handler& handler, value_ptr_type pval) const
		{
			typedef typename Handler::result_type ret_type;
			assert(pval);
			return ret_type();
		}
	};

public:
	template<typename Handler>
	typename Handler::result_type access(const Handler& handler)
	{
		typedef Handler handler_type;
		typedef typename handler_type::result_type ret_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return ret_type();
		}

		access_op_zwei<> op;
		return op(handler, pval);
	}

	template<typename Handler>
	typename Handler::result_type access(const Handler& handler) const
	{
		typedef Handler handler_type;
		typedef typename handler_type::result_type ret_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return ret_type();
		}

		access_op_zwei<> op;
		return op(handler, pval);
	}

private:
	template<
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct access_of_all_op
	{
		typedef params_type t_conts_type;

		template<typename Handler>
		void operator()(const Handler& handler, value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
			typedef Handler handler_type;
			typedef typename handler_type::result_type ret_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);

			ret_type ret = pcont->use_handler(boost::bind(&access_of_all_op::handler_op
													<
														cont_type,
														handler_type
													>,
													this, _1,
													boost::any(handler)));
			access_of_all_op<i + 1, isize> op;
			op(handler, pval);

		}

	private:
		template<typename Container, typename Handler>
		typename Handler::result_type handler_op(typename Container::base_type& base, const boost::any& handler_wrap) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;
			typedef Handler handler_type;

			const handler_type* phandler = boost::any_cast<handler_type>(&handler_wrap);

			if(!phandler)
			{
				return;
			}

			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if((*i))
				{
					(*i)->template access_of_all<handler_type>(*phandler);
				}
			}
		}
	};

	template<u32 i>
	struct access_of_all_op< i, i >
	{
		template<typename Handler>
		inline void operator()(const Handler& handler, value_ptr_type pval) const
		{
			assert(pval);
		}
	};

public:
	template<typename Handler>
	void access_of_all(const Handler& handler)
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		access_of_all_op<> op;
		op(handler, pval);
	}

	template<typename Handler>
	void access_of_all(const Handler& handler) const
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		access_of_all_op<> op;
		op(handler, pval);
	}

//private:
//	template<
//				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
//			>
//	struct access_op_const
//	{
//		typedef params_type t_conts_type;
//
//		template<typename Key, typename Handler>
//		bool operator()(const Key& key, const Handler& handler, value_ptr_type pval) const
//		{
//			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
//			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
//			typedef Key key_type;
//			typedef Handler handler_type;
//
//			assert(pval);
//			cont_ptr_type pcont = boost::get<i>(*pval);
//			assert(pcont);
//
//			access_op_const<i + 1, isize> op;
//
//			return pcont->use_handler(boost::bind(&access_op_const::handler_op
//													<
//														cont_type,
//														key_type,
//														handler_type
//													>,
//													this, _1,
//													boost::cref(key),
//													boost::any(handler)))
//					|| op(key, handler, pval);
//		}
//
//	private:
//		template<typename Container, typename Key, typename Handler>
//		bool handler_op(typename Container::base_type& base, const Key& key, const boost::any& handler_wrap) const
//		{
//			typedef Container cont_type;
//			typedef typename cont_type::iterator iter_type;
//
//			typedef Key key_type;
//			typedef Handler handler_type;
//
//			const handler_type* phandler = boost::any_cast<handler_type>(&handler_wrap);
//
//			if(!phandler)
//			{
//				return false;
//			}
//
//			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
//			{
//				if((*i) && ((*i)->access<key_type, handler_type>(key, *phandler)))
//				{
//					return true;
//				}
//			}
//			return false;
//		}
//	};
//
//	template<u32 i>
//	struct access_op_const< i, i >
//	{
//		template<typename Key, typename Handler>
//		inline bool operator()(const Key& key, const Handler& handler, value_ptr_type pval) const
//		{
//			assert(pval);
//			return false;
//		}
//	};
//
//public:
//	template<typename Key, typename Handler>
//	bool access(const Key& key, const Handler& handler) const
//	{
//		value_ptr_type pval = _value_wrap.get_shared_ptr();
//		assert(pval);
//		if(!pval)
//		{
//			return false;
//		}
//
//		access_op_const<> op;
//		return op(key, handler, pval);
//	}

	//----------------------------------
private:

	template<
				u32 i
			>
	struct use_method_op
	{
		typedef params_type t_conts_type;

		template<typename Handler>
		bool operator()(const Handler& handler, value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

			typedef Handler handler_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);


			return pcont->use_handler(boost::bind(&use_method_op::handler_op_of_non_ret
													<
														cont_type,
														handler_type
													>,
													this, _1,
													boost::any(handler)));

		}

		template<typename Handler, typename Ret_Handler>
		bool operator()(const Handler& handler, const Ret_Handler& ret_handler, value_ptr_type pval) const
		{
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

			typedef Handler handler_type;
			typedef Ret_Handler ret_handler_type;

			assert(pval);
			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);

			//use_method_of_all_op<i + 1, isize> op;

			return pcont->use_handler(boost::bind(&use_method_op::handler_op_of_ret
													<
														cont_type,
														handler_type,
														ret_handler_type
													>,
													this, _1,
													boost::any(handler),
													boost::any(ret_handler)));
		}

	private:
		template<typename Container, typename Handler>
		bool handler_op_of_non_ret(typename Container::base_type& base,
									const boost::any& handler_wrap) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;

			typedef Handler handler_type;

			const handler_type* phandler = boost::any_cast<handler_type>(&handler_wrap);

			if(!phandler)
			{
				return false;
			}

			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if((*i))
				{
					(*phandler)(*i);
					return true;
				}
			}
			return false;
		}

		template<typename Container, typename Handler, typename Ret_Handler>
		bool handler_op_of_ret(typename Container::base_type& base,
							const boost::any& handler_wrap,
							const boost::any& ret_handler_wrap) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;

			typedef Handler handler_type;
			typedef Ret_Handler ret_handler_type;

			const handler_type* phandler = boost::any_cast<handler_type>(&handler_wrap);
			const ret_handler_type* pret_handler = boost::any_cast<ret_handler_type>(&ret_handler_wrap);

			if(!(phandler && pret_handler))
			{
				return false;
			}

			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if((*i))
				{
					(*pret_handler)((*phandler)(*i));
					return true;
				}
			}
			return false;
		}
	};

public:
	template<u32 i, typename Handler>
	bool nth_use_method(const Handler& handler)
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		use_method_op<i> op;
		return op(handler, pval);
	}

	template<u32 i, typename Handler>
	bool nth_use_method(const Handler& handler) const
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		use_method_op<i> op;
		return op(handler, pval);
	}

	template<u32 i, typename Handler, typename Ret_Handler>
	bool nth_use_method(const Handler& handler, const Ret_Handler& ret_handler)
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		use_method_op<i> op;
		return op(handler, ret_handler, pval);
	}

	template<u32 i, typename Handler, typename Ret_Handler>
	bool nth_use_method(const Handler& handler, const Ret_Handler& ret_handler) const
	{
		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		use_method_op<i> op;
		return op(handler, ret_handler, pval);
	}

	template<typename Val_T, typename Handler>
	bool use_method(const Handler& handler)
	{
		typedef boost::mpl::find<vt_t_type, Val_T> find_t;
		typedef typename find_t::type iter_t;
		typedef typename iter_t::pos pos_t;
		return nth_use_method<pos_t::value>(handler);
	}

	template<typename Val_T, typename Handler>
	bool use_method(const Handler& handler) const
	{
		typedef boost::mpl::find<vt_t_type, Val_T> find_t;
		typedef typename find_t::type iter_t;
		typedef typename iter_t::pos pos_t;
		return nth_use_method<pos_t::value>(handler);
	}

	template<typename Val_T, typename Handler, typename Ret_Handler>
	bool use_method(const Handler& handler, const Ret_Handler& ret_handler)
	{
		typedef boost::mpl::find<vt_t_type, Val_T> find_t;
		typedef typename find_t::type iter_t;
		typedef typename iter_t::pos pos_t;
		return nth_use_method<pos_t::value>(handler, ret_handler);
	}

	template<typename Val_T, typename Handler, typename Ret_Handler>
	bool use_method(const Handler& handler, const Ret_Handler& ret_handler) const
	{
		typedef boost::mpl::find<vt_t_type, Val_T> find_t;
		typedef typename find_t::type iter_t;
		typedef typename iter_t::pos pos_t;
		return nth_use_method<pos_t::value>(handler, ret_handler);
	}

//private:
//
//	template<
//				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
//			>
//	struct use_method_of_all_op
//	{
//		typedef params_type t_conts_type;
//
//		template<typename Handler>
//		bool operator()(const Handler& handler, value_ptr_type pval) const
//		{
//			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
//			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
//
//			typedef Handler handler_type;
//			typedef Ret_Handler ret_handler_type;
//
//			assert(pval);
//			cont_ptr_type pcont = boost::get<i>(*pval);
//			assert(pcont);
//
//			use_method_op<i> now_op;
//			use_method_of_all_op<i + 1, isize> next_op;
//
//			return now_op(handler, pval) || next_op(handler, pval);
//		}
//
//		template<typename Handler, typename Ret_Handler>
//		bool operator()(const Handler& handler, const Ret_Handler& ret_handler, value_ptr_type pval) const
//		{
//			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
//			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
//
//			typedef Handler handler_type;
//			typedef Ret_Handler ret_handler_type;
//
//			assert(pval);
//			cont_ptr_type pcont = boost::get<i>(*pval);
//			assert(pcont);
//
//			use_method_op<i> now_op;
//			use_method_of_all_op<i + 1, isize> next_op;
//
//			return now_op(handler, ret_handler, pval)
//					|| next_op(handler, ret_handler, pval);
//		}
//	};
//
//	template<u32 i>
//	struct use_method_of_all_op< i, i >
//	{
//		template<typename Key, typename Handler>
//		inline bool operator()(const Key& key, const Handler& handler, value_ptr_type pval) const
//		{
//			assert(pval);
//			return false;
//		}
//	};
//
//public:
//	template<typename Handler>
//	bool use_mothod_of_all(const Handler& handler)
//	{
//		value_ptr_type pval = _value_wrap.get_shared_ptr();
//		assert(pval);
//		if(!pval)
//		{
//			return false;
//		}
//
//		use_method_of_all_op<> op;
//		return op(handler, pval);
//	}
//
//	template<typename Handler>
//	bool use_mothod_of_all(const Handler& handler) const
//	{
//		value_ptr_type pval = _value_wrap.get_shared_ptr();
//		assert(pval);
//		if(!pval)
//		{
//			return false;
//		}
//
//		use_method_of_all_op<> op;
//		return op(handler, pval);
//	}
//
//	template<typename Handler, typename Ret_Handler>
//	bool use_mothod_of_all(const Handler& handler, const Ret_Handler& ret_handler)
//	{
//		value_ptr_type pval = _value_wrap.get_shared_ptr();
//		assert(pval);
//		if(!pval)
//		{
//			return false;
//		}
//
//		use_method_of_all_op<> op;
//		return op(handler, ret_handler, pval);
//	}
//
//	template<typename Handler, typename Ret_Handler>
//	bool use_mothod_of_all(const Handler& handler, const Ret_Handler& ret_handler) const
//	{
//		value_ptr_type pval = _value_wrap.get_shared_ptr();
//		assert(pval);
//		if(!pval)
//		{
//			return false;
//		}
//
//		use_method_of_all_op<> op;
//		return op(handler, ret_handler, pval);
//	}

private:
	template<
				u32 i = 0, u32 isize = boost::mpl::size<params_type>::value
			>
	struct register_controller_op
	{
		typedef params_type t_conts_type;

		template<typename Tag, typename Ctrl_Center>
		void operator()(Ctrl_Center& cc, value_ptr_type pval) const
		{
			typedef Tag tag_type;
			typedef Ctrl_Center ctrl_center_type;
			typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
			typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

			assert(pval);

			cont_ptr_type pcont = boost::get<i>(*pval);
			assert(pcont);
			pcont->use_handler(
						boost::bind(
										&register_controller_op::handler_op
																<
																	cont_type,
																	tag_type,
																	ctrl_center_type
																>,
										this, _1, boost::ref(cc)
									)
								);

			register_controller_op< i + 1, isize> op;
			op.operator()<tag_type>(cc, pval);
		}

	private:
		template<typename Container, typename Tag, typename Ctrl_Center>
		void handler_op(typename Container::base_type& base, Ctrl_Center& cc) const
		{
			typedef Container cont_type;
			typedef typename cont_type::iterator iter_type;
			for(iter_type i = base.begin(), isize = base.end(); i != isize; ++i)
			{
				if(*i)
				{
					//(*i)->clear();
					cc.register_controller<Tag>(*(*i));
				}
			}
		}
	};

	template<u32 i>
	struct register_controller_op< i, i >
	{
		template<typename Tag, typename Ctrl_Center>
		inline void operator()(Ctrl_Center& cc, value_ptr_type pval) const
		{
			assert(pval);
		}
	};

	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename Ctrl_Center>
	void register_controller(Ctrl_Center& cc)
	{
		typedef Tag tag_type;
		typedef Ctrl_Center ctrl_center_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		register_controller_op<> op;
#ifdef _MSC_VER
		op.operator()<tag_type, ctrl_center_type>(cc, pval);
#else
        op.template operator()<tag_type, ctrl_center_type>(cc, pval);
#endif // _MSC_VER
	}


private:
	service_handler_type _service_handler;
	adapter_mgr_wrap_type _adp_mgr_wrap;
	value_wrap_type _value_wrap;
};

} // namespace netowrk
} // namespace yggr

#endif //__YGGR_NETWORK_NETWORK_HANDLER_MIXER_HPP__
