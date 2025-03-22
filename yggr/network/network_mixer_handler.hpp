//netowrk_handler_mixer.hpp

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

#ifndef __YGGR_NETWORK_NETWORK_HANDLER_MIXER_HPP__
#define __YGGR_NETWORK_NETWORK_HANDLER_MIXER_HPP__

#if defined(__GNUC__) && (__GNUC__ < 4) && (__GNUC_MINOR__ < 6)
#	error "this file must use gcc 4.6 or later of compile !!!"
#endif // __GNUC__

#include <yggr/base/yggrdef.h>
#include <yggr/network/socket_conflict_fixer.hpp>

#include <yggr/move/move.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/func/foo_t_maker.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/conv_to_tuple.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>
#include <yggr/mplex/revert_to_vector.hpp>

#include <yggr/tuple/tuple.hpp>
#include <yggr/tuple_ex/tuple.hpp>

#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/safe_container/safe_list.hpp>

#include <yggr/serialization/access.hpp>

#include <boost/asio.hpp>
#include <boost/functional/hash/hash.hpp>
#include <boost/thread/mutex.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/count.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/smart_ptr/enable_shared_from_raw.hpp>

#include <cassert>
#include <sstream>
#include <utility>

namespace yggr
{
namespace network
{
namespace detail
{

template<typename Tvt, std::size_t i = 0, std::size_t isize = boost::mpl::size<Tvt>::value >
struct network_mixer_handler_check_unique_types
{
private:
	typedef Tvt tvt_type;
	typedef typename boost::mpl::at_c<tvt_type, i>::type now_handler_type;
	typedef typename boost::mpl::count<tvt_type, now_handler_type>::type count_type;

public:
	typedef boost::mpl::bool_<(!!(count_type::value))> type;

private:
	BOOST_MPL_ASSERT((type));
	BOOST_MPL_ASSERT((network_mixer_handler_check_unique_types<tvt_type, i + 1, isize>));
};

template<typename Tvt, std::size_t i >
struct network_mixer_handler_check_unique_types<Tvt, i, i >
	: public boost::mpl::true_
{
};

//network_mixer_handler_create_containers
template<typename Mutex, typename TVt, std::size_t i>
struct trans_to_safe_list_shared_ptr
{
private:
	typedef Mutex tpl_arg_mutex_type;
	typedef TVt tvt_type;
	typedef typename boost::mpl::at_c<tvt_type, i>::type now_handler_type;
	typedef ::yggr::shared_ptr<now_handler_type> now_handler_ptr_type;
	typedef yggr::safe_container::safe_list<now_handler_ptr_type, tpl_arg_mutex_type> now_handler_ptr_list_type;
	typedef ::yggr::shared_ptr<now_handler_ptr_list_type> now_handler_ptr_list_ptr_type;
public:
	typedef now_handler_ptr_list_ptr_type type;
};

template<typename Mutex, typename Vt, typename TVt, std::size_t i = 0, std::size_t isize = boost::mpl::size<TVt>::value>
struct network_mixer_handler_create_containers
	: public
		network_mixer_handler_create_containers
		<
			Mutex,
			typename
				boost::mpl::push_back
				<
					Vt,
					typename trans_to_safe_list_shared_ptr<Mutex, TVt, i>::type
				>::type,
			TVt,
			i + 1,
			isize
		>
{
};

template<typename Mutex, typename Vt, typename TVt, std::size_t i >
struct network_mixer_handler_create_containers<Mutex, Vt, TVt, i, i >
	: public mplex::revert_to_vector<Vt>::type
{
};

template<typename AdapterMgrPtr, typename Lkh_Init>
struct network_mixer_handler_init
{
	typedef AdapterMgrPtr adapter_mgr_ptr_type;
	typedef Lkh_Init lkh_init_type;

	network_mixer_handler_init(adapter_mgr_ptr_type padp, const lkh_init_type& lkh_init)
		: _padp(padp), _port(0), _lkh_init(lkh_init)
	{
	}

	network_mixer_handler_init(u16 port, adapter_mgr_ptr_type padp, const lkh_init_type& lkh_init)
		: _padp(padp), _port(port), _lkh_init(lkh_init)
	{
	}

	~network_mixer_handler_init(void)
	{
	}

public:
	inline u16 port(void) const
	{
		return _port;
	}

	inline adapter_mgr_ptr_type session_mgr_init(void) const
	{
		return _padp;
	}

	inline const lkh_init_type& lkh_init(void) const
	{
		return _lkh_init;
	}

private:
	adapter_mgr_ptr_type _padp;

	u16 _port;
	lkh_init_type _lkh_init;

};

// network_mixer_handler_init_value_op
template<typename ValuePtr, typename Params, typename Mutex,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_init_value_op
	: public network_mixer_handler_init_value_op<ValuePtr, Params, Mutex, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;
	typedef Mutex tpl_arg_mutex_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef
		network_mixer_handler_init_value_op
		<
			ValuePtr,
			Params,
			Mutex,
			i + 1,
			isize
		> next_op_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
		if(!pval)
		{
			return;
		}

		{
			cont_ptr_type tmp = ::yggr::make_shared<cont_type>();
			assert(tmp);

			if(!tmp)
			{
				return;
			}

			yggr::get<i>(*pval).swap(tmp);
		}

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);
		next_op(pval);
	}
};

template<typename ValuePtr, typename Params, typename Mutex, std::size_t i >
struct network_mixer_handler_init_value_op<ValuePtr, Params, Mutex, i, i >
{
	typedef ValuePtr value_ptr_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
	}
};

//network_mixer_handler_run_op
template< typename ValuePtr, typename Params,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_run_op
	: public network_mixer_handler_run_op<ValuePtr, Params, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	typedef
		network_mixer_handler_run_op
		<
			value_ptr_type,
			t_conts_type,
			i + 1,
			isize
		> next_op_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();
		handler_op(base_cont);

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);
		next_op(pval);
	}

private:
	void handler_op(base_cont_type& base) const
	{
		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if(*ni)
			{
				(*ni)->run();
			}
		}
	}
};

template<typename ValuePtr, typename Params, std::size_t i>
struct network_mixer_handler_run_op<ValuePtr, Params, i, i>
{
	typedef ValuePtr value_ptr_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
	}
};

// network_mixer_handler_stop_op
template< typename ValuePtr, typename Params,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_stop_op
	: public network_mixer_handler_stop_op<ValuePtr, Params, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	typedef
		network_mixer_handler_stop_op
		<
			value_ptr_type,
			t_conts_type,
			i + 1,
			isize
		> next_op_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();

		handler_op(base_cont);

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);
		next_op(pval);
	}

private:
	void handler_op(base_cont_type& base) const
	{
		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if(*ni)
			{
				(*ni)->stop();
			}
		}
	}
};

template< typename ValuePtr, typename Params, std::size_t i >
struct network_mixer_handler_stop_op<ValuePtr, Params, i, i >
{
	typedef ValuePtr value_ptr_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
	}
};

template< typename ValuePtr, typename Params,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_online_size_op
	: public network_mixer_handler_online_size_op<ValuePtr, Params, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	typedef
		detail::network_mixer_handler_online_size_op
		<
			value_ptr_type,
			t_conts_type,
			i + 1,
			isize
		> next_op_type;

	inline size_type operator()(value_ptr_type pval) const
	{
		typedef
			detail::network_mixer_handler_online_size_op
			<
				value_ptr_type,
				t_conts_type,
				i + 1,
				isize
			> op_type;

		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);
		return handler_op(base_cont) + next_op(pval);
	}

private:
	size_type handler_op(base_cont_type& base) const
	{
		size_type size = 0;
		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if(*ni)
			{
				size += (*ni)->online_size();
			}
		}

		return size;
	}
};

template<typename ValuePtr, typename Params, std::size_t i>
struct network_mixer_handler_online_size_op< ValuePtr, Params, i, i >
{
	typedef ValuePtr value_ptr_type;

	inline size_type operator()(value_ptr_type pval) const
	{
		assert(pval);
		return 0;
	}
};

template< typename ValuePtr, typename Params,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_clear_sessions_op
	: public network_mixer_handler_clear_sessions_op<ValuePtr, Params, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	typedef
		detail::network_mixer_handler_clear_sessions_op
		<
			value_ptr_type,
			t_conts_type,
			i + 1,
			isize
		> next_op_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();

		handler_op(base_cont);

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);
		next_op(pval);
	}

private:
	void handler_op(base_cont_type& base) const
	{
		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if(*ni)
			{
				(*ni)->clear_sessions();
			}
		}
	}
};

template< typename ValuePtr, typename Params, std::size_t i >
struct network_mixer_handler_clear_sessions_op< ValuePtr, Params, i, i>
{
	typedef ValuePtr value_ptr_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
	}
};

//network_mixer_handler_clear_op
template< typename ValuePtr, typename Params,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_clear_op
	: public network_mixer_handler_clear_op<ValuePtr, Params, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	typedef
		network_mixer_handler_clear_op
		<
			value_ptr_type,
			t_conts_type,
			i + 1,
			isize
		> next_op_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();

		handler_op(base_cont);

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);
		next_op(pval);
	}

private:
	void handler_op(base_cont_type& base) const
	{
		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if(*ni)
			{
				(*ni)->clear();
			}
		}
	}
};

template<typename ValuePtr, typename Params, std::size_t i>
struct network_mixer_handler_clear_op< ValuePtr, Params, i, i >
{
	typedef ValuePtr value_ptr_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
	}
};

template<typename ValuePtr, typename Params,
			typename HandlerTag, typename ChkHandlerTag,
			typename ProtocolTag, typename ChkProtoTag,
			std::size_t i,
			bool tags_same =
				 (boost::is_same<HandlerTag, ChkHandlerTag>::value
					&& boost::is_same<ProtocolTag, ChkProtoTag>::value) >
class network_mixer_handler_check_state_op_caller;

template<typename ValuePtr, typename Params,
			typename HandlerTag, typename ChkHandlerTag,
			typename ProtocolTag, typename ChkProtoTag,
			std::size_t i>
class network_mixer_handler_check_state_op_caller<
		ValuePtr, Params, HandlerTag, ChkHandlerTag, ProtocolTag, ChkProtoTag, i, true>
{
public:
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

public:
	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();
		handler_op(base_cont);
	}

private:
	void handler_op(base_cont_type& base) const
	{
		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if(*ni)
			{
				(*ni)->check_state();
			}
		}
	}
};

template<typename ValuePtr, typename Params,
			typename HandlerTag, typename ChkHandlerTag,
			typename ProtocolTag, typename ChkProtoTag,
			std::size_t i>
class network_mixer_handler_check_state_op_caller<
		ValuePtr, Params, HandlerTag, ChkHandlerTag, ProtocolTag, ChkProtoTag, i, false>
{
public:
	typedef ValuePtr value_ptr_type;

public:
	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
	}
};

// network_mixer_handler_check_state_op
template< typename ValuePtr, typename Params,
			typename Handler_Tag, typename Protocol_Tag,
			std::size_t i = 0,
			std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_check_state_op
	: public network_mixer_handler_check_state_op<ValuePtr, Params, Handler_Tag, Protocol_Tag, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;
	typedef Handler_Tag handler_tag_type;
	typedef Protocol_Tag protocol_tag;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::value_type op_handler_ptr_type;

	typedef typename yggr::mplex::pointer_to_value_t<op_handler_ptr_type>::type op_handler_type;
	typedef typename op_handler_type::tag_type now_handler_tag_type;
	typedef typename op_handler_type::protocol_tag_type now_protocol_tag_type;

	typedef
		network_mixer_handler_check_state_op_caller
		<
			value_ptr_type,
			t_conts_type,
			handler_tag_type, now_handler_tag_type,
			protocol_tag, now_protocol_tag_type,
			i
		> caller_type;

	typedef
		network_mixer_handler_check_state_op
		<
			value_ptr_type,
			t_conts_type,
			handler_tag_type,
			protocol_tag,
			i + 1,
			isize
		> next_op_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);

		caller_type handler_op_call;
		handler_op_call(pval);

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);
		next_op(pval);
	}
};

template< typename ValuePtr, typename Params,
			typename Handler_Tag, typename Protocol_Tag, std::size_t i >
struct network_mixer_handler_check_state_op< ValuePtr, Params, Handler_Tag, Protocol_Tag, i, i >
{
	typedef ValuePtr value_ptr_type;

	inline void operator()(value_ptr_type pval) const
	{
		assert(pval);
	}
};

// network_mixer_handler_send_op
template< typename ValuePtr, typename Params,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_send_op
	: public network_mixer_handler_send_op<ValuePtr, Params, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	typedef
		network_mixer_handler_send_op
		<
			value_ptr_type,
			t_conts_type,
			i + 1,
			isize
		> next_op_type;

	template<typename Key, typename NetInfo, typename Data, typename Handler> inline
	bool operator()(const Key& key, const NetInfo& netinfo, const Data& data,
						const Handler& handler, value_ptr_type pval) const
	{
		typedef Key key_type;
		typedef NetInfo netinfo_type;
		typedef Data data_type;
		typedef Handler handler_type;

		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);

		return
			handler_op_drei(base_cont, key, netinfo, data, handler)
			|| next_op(key, netinfo, data, handler, pval);
	}

	template<typename Key, typename Pak, typename Handler> inline
	bool operator()(const Key& key, Pak& pak, const Handler& handler, value_ptr_type pval) const
	{
		typedef Key key_type;
		typedef Pak pak_type;
		typedef Handler handler_type;

		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();
		const next_op_type& next_op = static_cast<const next_op_type&>(*this);

		return
			handler_op_zwei(base_cont, key, pak, handler)
			|| next_op(key, pak, handler, pval);
	}

	template<typename Key, typename Pak> inline
	bool operator()(const Key& key, Pak& pak, value_ptr_type pval) const
	{
		typedef Key key_type;
		typedef Pak pak_type;

		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);

		return
			handler_op_eins(base_cont, key, pak)
			|| next_op(key, pak, pval);
	}

private:
	template<typename Key, typename NetInfo, typename Data, typename Handler>
	bool handler_op_drei(base_cont_type& base,
							const Key& key, const NetInfo& netinfo,
							const Data& data, const Handler& handler) const
	{
		typedef Key key_type;
		typedef NetInfo netinfo_type;
		typedef Data data_type;
		typedef Handler handler_type;

		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if((*ni) && ((*ni)->template send_packet<
											key_type,
											netinfo_type,
											data_type,
											handler_type>(key, netinfo, data, handler)))
			{
				return true;
			}
		}
		return false;
	}

	template<typename Key, typename Pak, typename Handler>
	bool handler_op_zwei(base_cont_type& base,
							const Key& key, Pak& pak, const Handler& handler) const
	{
		typedef Key key_type;
		typedef Pak pak_type;
		typedef Handler handler_type;

		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if((*ni) && ((*ni)->template send_packet<key_type, pak_type>(key, pak, handler)))
			{
				return true;
			}
		}
		return false;
	}

	template<typename Key, typename Pak>
	bool handler_op_eins(base_cont_type& base, const Key& key, Pak& pak) const
	{
		typedef Key key_type;
		typedef Pak pak_type;

		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if((*ni) && ((*ni)->template send_packet<key_type, pak_type>(key, pak)))
			{
				return true;
			}
		}
		return false;
	}
};

template< typename ValuePtr, typename Params, std::size_t i>
struct network_mixer_handler_send_op<ValuePtr, Params, i, i >
{
	typedef ValuePtr value_ptr_type;

	template<typename Key, typename NetInfo, typename Data, typename Handler> inline
	bool operator()(const Key& key, const NetInfo& netinfo, const Data& data,
						const Handler& handler, value_ptr_type pval) const
	{
		assert(pval);
		return false;
	}

	template<typename Key, typename Pak, typename Handler> inline
	bool operator()(const Key& key, Pak& pak, const Handler& handler, value_ptr_type pval) const
	{
		assert(pval);
		return false;
	}

	template<typename Key, typename Pak> inline
	bool operator()(const Key& key, Pak& pak, value_ptr_type pval) const
	{
		assert(pval);
		return false;
	}
};

// network_mixer_handler_access_op_eins
template< typename ValuePtr, typename Params,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_access_op_eins
	: public network_mixer_handler_access_op_eins<ValuePtr, Params, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	typedef
		network_mixer_handler_access_op_eins
		<
			value_ptr_type,
			t_conts_type,
			i + 1,
			isize
		> next_op_type;

	template<typename Key, typename Handler> inline
	bool operator()(const Key& key, const Handler& handler, value_ptr_type pval) const
	{
		typedef Key key_type;
		typedef Handler handler_type;

		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();
		//op_type op;
		const next_op_type& next_op = static_cast<const next_op_type&>(*this);

		return handler_op(base_cont, key, handler) || next_op(key, handler, pval);
	}

private:
	template<typename Key, typename Handler>
	bool handler_op(base_cont_type& base, const Key& key, const Handler& handler) const
	{
		typedef Key key_type;
		typedef Handler handler_type;

		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if((*ni) && ((*ni)->template access<key_type, handler_type>(key, handler)))
			{
				return true;
			}
		}
		return false;
	}
};

template<typename ValuePtr, typename Params, std::size_t i >
struct network_mixer_handler_access_op_eins< ValuePtr, Params, i, i >
{
	typedef ValuePtr value_ptr_type;

	template<typename Key, typename Handler> inline
	bool operator()(const Key& key, const Handler& handler, value_ptr_type pval) const
	{
		assert(pval);
		return false;
	}
};

// network_mixer_handler_access_op_zwei
template< typename ValuePtr, typename Params,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_access_op_zwei
	: public network_mixer_handler_access_op_zwei<ValuePtr, Params, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	typedef
		network_mixer_handler_access_op_zwei
		<
			value_ptr_type,
			t_conts_type,
			i + 1,
			isize
		> next_op_type;

	template<typename Handler> inline
	void operator()(const Handler& handler, value_ptr_type pval) const
	{
		typedef Handler handler_type;

		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();
		handler_op(base_cont, handler);

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);
		next_op(handler, pval);
	}

private:
	template<typename Handler>
	void handler_op(base_cont_type& base, const Handler& handler) const
	{
		typedef Handler handler_type;

		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if(*ni)
			{
				(*ni)->template access<handler_type>(handler);
			}
		}
	}
};

template<typename ValuePtr, typename Params, std::size_t i >
struct network_mixer_handler_access_op_zwei< ValuePtr, Params, i, i >
{
	typedef ValuePtr value_ptr_type;

	template<typename Handler> inline
	void operator()(const Handler& handler, value_ptr_type pval) const
	{
		assert(pval);
	}
};

//network_mixer_handler_access_of_all_op
template<typename ValuePtr, typename Params,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_access_of_all_op
	: public network_mixer_handler_access_of_all_op<ValuePtr, Params, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	typedef
		network_mixer_handler_access_of_all_op
		<
			value_ptr_type,
			t_conts_type,
			i + 1,
			isize
		> next_op_type;

	template<typename Handler> inline
	void operator()(const Handler& handler, value_ptr_type pval) const
	{
		typedef Handler handler_type;

		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();
		handler_op(base_cont, handler);

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);
		next_op(handler, pval);

	}

private:
	template<typename Handler>
	void handler_op(base_cont_type& base, const Handler& handler) const
	{
		typedef Handler handler_type;

		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if((*ni))
			{
				(*ni)->template access_of_all<handler_type>(handler);
			}
		}
	}
};

// network_mixer_handler_access_of_all_op
template<typename ValuePtr, typename Params, std::size_t i>
struct network_mixer_handler_access_of_all_op< ValuePtr, Params, i, i >
{
	typedef ValuePtr value_ptr_type;

	template<typename Handler> inline
	void operator()(const Handler& handler, value_ptr_type pval) const
	{
		assert(pval);
	}
};

template<typename ValuePtr, typename Params, std::size_t i>
struct network_mixer_handler_using_method_op
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	template<typename Handler> inline
	bool operator()(const Handler& handler, value_ptr_type pval) const
	{
		typedef Handler handler_type;

		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();

		return handler_op_of_non_ret(base_cont, handler);
	}

	template<typename Handler, typename Ret_Handler> inline
	bool operator()(const Handler& handler, const Ret_Handler& ret_handler, value_ptr_type pval) const
	{
		typedef Handler handler_type;
		typedef Ret_Handler ret_handler_type;

		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();

		return handler_op_of_ret(base_cont, handler, ret_handler);
	}

private:
	template<typename Handler>
	bool handler_op_of_non_ret(base_cont_type& base,
								const Handler& handler) const
	{
		if(base.empty())
		{
			return false;
		}

		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if((*ni))
			{
				handler(*ni);
			}
		}

		return true;
	}

	template<typename Handler, typename Ret_Handler>
	bool handler_op_of_ret(base_cont_type& base,
							const Handler& handler,
							const Ret_Handler& ret_handler) const
	{
		typedef Handler handler_type;
		typedef Ret_Handler ret_handler_type;

		if(base.empty())
		{
			return false;
		}

		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if((*ni))
			{
				ret_handler(handler(*ni));
			}
		}
		return true;
	}
};

// network_mixer_handler_register_controller_op
template< typename ValuePtr, typename Params,
			std::size_t i = 0, std::size_t isize = boost::mpl::size<Params>::value >
struct network_mixer_handler_register_controller_op
	: public network_mixer_handler_register_controller_op<ValuePtr, Params, i + 1, isize>
{
	typedef ValuePtr value_ptr_type;
	typedef Params t_conts_type;

	typedef typename boost::mpl::at_c<t_conts_type, i>::type cont_ptr_type;
	typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;

	typedef typename cont_type::base_type base_cont_type;
	typedef typename base_cont_type::iterator base_cont_iter_type;

	typedef
		network_mixer_handler_register_controller_op
		<
			value_ptr_type,
			t_conts_type,
			i + 1,
			isize
		> next_op_type;

	template<typename Tag, typename CtrlCenter> inline
	void call(CtrlCenter& cc, value_ptr_type pval) const
	{
		typedef Tag tag_type;
		typedef CtrlCenter ctrl_center_type;

		assert(pval);
		cont_ptr_type pcont = yggr::get<i>(*pval);
		assert(pcont);

		base_cont_type base_cont = pcont->load();
		handler_op<tag_type>(base_cont, cc);

		const next_op_type& next_op = static_cast<const next_op_type&>(*this);
		next_op.template call<tag_type>(cc, pval);
	}

private:
	template<typename Tag, typename CtrlCenter>
	void handler_op(base_cont_type& base, CtrlCenter& cc) const
	{
		for(base_cont_iter_type ni = base.begin(), nisize = base.end(); ni != nisize; ++ni)
		{
			if(*ni)
			{
				cc.template register_controller<Tag>(*(*ni));
			}
		}
	}
};

template<typename ValuePtr, typename Params, std::size_t i >
struct network_mixer_handler_register_controller_op< ValuePtr, Params, i, i >
{
	typedef ValuePtr value_ptr_type;

	template<typename Tag, typename CtrlCenter> inline
	void call(CtrlCenter& cc, value_ptr_type pval) const
	{
		assert(pval);
	}
};

//----------------

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<typename Mutex, typename Inv_t, typename ...Vals >

#else

#	define YGGR_PP_TEMPLATE_DEFAULT_TYPE() yggr::tuples::null_type

template<typename Mutex, typename Inv_t,
			YGGR_PP_TEMPLATE_PARAMS_TYPES( YGGR_PP_TEMPLATE_PARAMS_LEN(), typename Val ) >

#	undef YGGR_PP_TEMPLATE_DEFAULT_TYPE

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES
struct network_mixer_handler_container_maker
{
private:
	typedef Mutex tpl_arg_mutex_type;
	typedef Inv_t null_type;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	typedef boost::mpl::vector< Vals... > tmp_vt_t_type;
#else
	typedef
		boost::mpl::vector
		<
			YGGR_PP_TEMPLATE_PARAMS_TYPES(
				YGGR_PP_TEMPLATE_PARAMS_LEN(), Val)
		> tmp_vt_t_type;
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	typedef typename boost::mpl::find<tmp_vt_t_type, null_type>::type i_t;

	typedef typename
		mplex::revert_to_vector
		<
			typename
				boost::mpl::erase
				<
					tmp_vt_t_type,
					i_t,
					typename boost::mpl::end<tmp_vt_t_type>::type
				>::type
		>::type rst_vt_t_type;

	BOOST_MPL_ASSERT((network_mixer_handler_check_unique_types<rst_vt_t_type>));

	typedef typename
		network_mixer_handler_create_containers
		<
			tpl_arg_mutex_type,
			boost::mpl::vector<>,
			rst_vt_t_type
		>::type rst_containers_type;

	BOOST_MPL_ASSERT((
		boost::mpl::bool_
		<
			(
				boost::mpl::size<rst_containers_type>::value
					== boost::mpl::size<rst_vt_t_type>::value
			)
		> ));

public:
	typedef rst_vt_t_type vt_t_type;
	typedef rst_containers_type containers_type;
};

} // namespace detail
} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

template<
	typename ServiceHandler,
	typename Adapter_Mgr,
	typename Mutex = void,
	typename ...Vals >

#else

#	define YGGR_PP_TEMPLATE_DEFAULT_TYPE() yggr::tuples::null_type

template<
	typename ServiceHandler,
	typename Adapter_Mgr,
	typename Mutex = void,
	YGGR_PP_TEMPLATE_PARAMS_TYPES_AND_DEFAULT_TYPE( YGGR_PP_TEMPLATE_PARAMS_LEN(), typename Val ) >

#	undef YGGR_PP_TEMPLATE_DEFAULT_TYPE

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES
class network_mixer_handler
	: public boost::enable_shared_from_raw
{
public:
	typedef ServiceHandler service_handler_type;
	typedef Adapter_Mgr adapter_mgr_type;

	typedef typename adapter_mgr_type::reg_def_type adapter_mgr_reg_def_type;

private:
	typedef Mutex tpl_arg_mutex_type;

	// ------------------------types dedunce and check start--------------------------
	typedef yggr::tuples::null_type null_type;

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	typedef
		detail::network_mixer_handler_container_maker
		<
			tpl_arg_mutex_type,
			yggr::tuples::null_type,
			Vals...
		> container_maker_type;
#else

	typedef
		detail::network_mixer_handler_container_maker
		<
			tpl_arg_mutex_type,
			yggr::tuples::null_type,
			YGGR_PP_TEMPLATE_PARAMS_TYPES(
				YGGR_PP_TEMPLATE_PARAMS_LEN(), Val)
		> container_maker_type;

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	typedef typename container_maker_type::vt_t_type vt_t_type;
	typedef typename container_maker_type::containers_type containers_type;

	typedef containers_type params_type;
	typedef typename mplex::conv_to_yggr_tuple<params_type, null_type>::type value_type;
	typedef yggr::safe_container::safe_wrap<value_type> value_wrap_type;
	typedef typename value_wrap_type::value_ptr_type value_ptr_type;
	typedef typename value_wrap_type::error_type value_wrap_error_type;

	// ------------------------types dedunce and check end--------------------------

	typedef yggr::safe_container::safe_wrap<adapter_mgr_type> adapter_mgr_wrap_type;

public:
	typedef typename adapter_mgr_wrap_type::value_ptr_type adapter_mgr_ptr_type;

private:
	typedef typename adapter_mgr_wrap_type::error_type adapter_mgr_wrap_error_type;

private:
	typedef network_mixer_handler this_type;

public:
	template<typename Service_Handler_Init, typename Source>
	network_mixer_handler(const Service_Handler_Init& service_init, const Source& src)
		: _brun(false),
			_service_handler(service_init),
			_adp_mgr_wrap(yggr_nothrow_new adapter_mgr_type(const_cast<Source&>(src))),
			_value_wrap(yggr_nothrow_new value_type())
	{
		typedef
			detail::network_mixer_handler_init_value_op
			<
				value_ptr_type,
				params_type,
				tpl_arg_mutex_type
			> op_type;

		value_ptr_type ptr(_value_wrap.get_shared_ptr());
		assert(ptr);

		if(!ptr)
		{
			return;
		}

		op_type op;
		op(ptr);
	}

	~network_mixer_handler(void)
	{
		assert(!_brun.load());
	}

public:
	template<typename Real_Data> inline
	bool register_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;

		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter<real_data_type>(stat);
	}

	template<typename Real_Data, typename Container> inline
	bool register_network_protocol_of_container(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;
		typedef Container container_type;

		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter_of_container<real_data_type, container_type>(stat);
	}

	template<typename Container, typename Send_ID, typename Recv_ID> inline
	bool register_network_protocol(const Send_ID& send_id,
									const Recv_ID& recv_id,
									u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Container container_type;

		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template register_adapter<container_type>(send_id, recv_id, stat);
	}

	template<typename Real_Data> inline
	bool unregister_network_protocol(u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;

		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->template unregister_adapter<real_data_type>(stat);
	}

	template<typename ID> inline
	bool unregister_network_protocol(const ID& id,
										u32 tag = adapter_mgr_type::template default_tag<ID>::value,
										u32 stat = adapter_mgr_reg_def_type::E_both)
	{
		adapter_mgr_ptr_type ptr = _adp_mgr_wrap.get_shared_ptr();
		return ptr && ptr->unregister_adapter(id, tag, stat);
	}

	inline void add_signal(u32 sig)
	{
		_service_handler.add_signal(sig);
	}

	inline void join(void)
	{
		_service_handler.join();
	}

protected:
	inline void pro_add_signal(void)
	{
		_service_handler.add_signal(SIGINT);
		_service_handler.add_signal(SIGTERM);
#   if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.add_signal(SIGBREAK);
#   endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

#if defined(YGGR_AT_ANDROID)
		_service_handler.add_signal(SIGSTOP);
#endif // #if defined(YGGR_AT_ANDROID)

		_service_handler.add_signal(SIGFPE);
		_service_handler.add_signal(SIGILL);
		_service_handler.add_signal(SIGABRT);
#	if defined(SIGQUIT)
		_service_handler.add_signal(SIGQUIT);
#	endif // defined(SIGQUIT)
#	if defined(SIGTSTP)
		_service_handler.add_signal(SIGTSTP);
#	endif // SIGTSTP
	}

	inline void pro_remove_signal(void)
	{
		_service_handler.remove_signal(SIGINT);
		_service_handler.remove_signal(SIGTERM);
#   if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
		_service_handler.remove_signal(SIGBREAK);
#   endif // defined(WIN32) || defined(WIN64) || defined(WINDOWS)

#if defined(YGGR_AT_ANDROID)
		_service_handler.remove_signal(SIGSTOP);
#endif // #if defined(YGGR_AT_ANDROID)

		_service_handler.remove_signal(SIGFPE);
		_service_handler.remove_signal(SIGILL);
		_service_handler.remove_signal(SIGABRT);
#	if defined(SIGQUIT)
		_service_handler.remove_signal(SIGQUIT);
#	endif // defined(SIGQUIT)
#	if defined(SIGTSTP)
		_service_handler.remove_signal(SIGTSTP);
#	endif // SIGTSTP
	}

public:
	void run(void)
	{
		typedef
			detail::network_mixer_handler_run_op
			<
				value_ptr_type,
				params_type
			> op_type;

		bool now = false;
		bool next = true;
		if(!_brun.compare_exchange_strong(now, next))
		{
			return;
		}

		pro_add_signal();

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		_service_handler.set_signal_handler(boost::bind(&this_type::stop, this));

		op_type op;
		op(pval);

		_service_handler.run();
		assert(_brun.load() == _service_handler.is_running());
	}

	template<typename Handler>
	void run(const Handler& handler)
	{
		typedef
			detail::network_mixer_handler_run_op
			<
				value_ptr_type,
				params_type
			> op_type;

		bool now = false;
		bool next = true;
		if(!_brun.compare_exchange_strong(now, next))
		{
			return;
		}

		pro_add_signal();

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		_service_handler.set_signal_handler(handler);

		op_type op;
		op(pval);

		_service_handler.run();
		assert(_brun.load() == _service_handler.is_running());
	}

public:
	void stop(void)
	{
		typedef
			detail::network_mixer_handler_stop_op
			<
				value_ptr_type,
				params_type
			> op_type;

		bool now = true;
		bool next = false;
		if(!_brun.compare_exchange_strong(now, next))
		{
			return;
		}

		_service_handler.stop();

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		op_type op;
		op(pval);

		pro_remove_signal();
		_service_handler.cancel_signal_handler();

		assert(_brun.load() == _service_handler.is_running());
	}

public:
	size_type online_size(void) const
	{
		typedef
			detail::network_mixer_handler_online_size_op
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return 0;
		}

		op_type op;
		return op(pval);
	}

public:
	void clear_sessions(void)
	{
		typedef
			detail::network_mixer_handler_clear_sessions_op
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		op_type op;
		op(pval);
	}

public:
	inline void clear(void)
	{
		typedef
			detail::network_mixer_handler_clear_op
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		op_type op;
		op(pval);
	}

	inline service_handler_type& service_handler(void)
	{
		return _service_handler;
	}

public:
	template<typename Handler_Tag, typename Protocol_Tag> inline
	void check_state(void)
	{
		typedef Handler_Tag handler_tag_type;
		typedef Protocol_Tag protocol_tag_type;

		typedef
			detail::network_mixer_handler_check_state_op
			<
				value_ptr_type,
				params_type,
				handler_tag_type,
				protocol_tag_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		op_type op;
		op(pval);
	}

	template<typename OP_Handler, typename Lkh_Init>
	bool append(const Lkh_Init& lkh_init, u16 port = 0)
	{
		typedef OP_Handler op_handler_type;
		typedef Lkh_Init lkh_init_type;
		typedef detail::network_mixer_handler_init<adapter_mgr_ptr_type, lkh_init_type> init_type;

		typedef typename boost::mpl::find<vt_t_type, op_handler_type>::type find_iter_type;

		{
			BOOST_MPL_ASSERT((
				boost::mpl::if_
				<
					typename
						boost::is_same
						<
							find_iter_type,
							boost::mpl::end<vt_t_type>
						>::type,
					boost::mpl::false_,
					boost::mpl::true_
				> ));
		}

		typedef typename boost::mpl::at_c<params_type, find_iter_type::pos::value>::type cont_ptr_type;
		typedef typename yggr::mplex::pointer_to_value_t<cont_ptr_type>::type cont_type;
		typedef typename cont_type::value_type op_handler_ptr_type;

		typedef typename yggr::mplex::pointer_to_value_t<op_handler_ptr_type>::type check_op_handler_type;

		{
			BOOST_MPL_ASSERT((boost::is_same<op_handler_type, check_op_handler_type>));
		}

		adapter_mgr_ptr_type padp(_adp_mgr_wrap.get_shared_ptr());

		assert(padp);
		if(!padp)
		{
			return false;
		}

		init_type init(port, padp, lkh_init);
		value_ptr_type pval;
		cont_ptr_type pcont;
		op_handler_ptr_type phandler;

		if(!((pval = _value_wrap.get_shared_ptr())
				&& (pcont = yggr::get<find_iter_type::pos::value>(*pval))
				&& (phandler = construct_shared<op_handler_type>(
									yggr_nothrow_new op_handler_type(_service_handler, init))) ))
		{
			return false;
		}

		pcont->push_back(phandler);

		if(_service_handler.is_running())
		{
			phandler->run();
		}

		return true;
	}

public:
	template<typename Key, typename NetInfo, typename Data, typename Handler> inline
	bool send_packet(const Key& key, const NetInfo& netinfo, const Data& data, const Handler& handler)
	{
		typedef
			detail::network_mixer_handler_send_op
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		op_type op;
		return op(key, netinfo, data, handler, pval);
	}

	template<typename Key, typename Pak, typename Handler> inline
	bool send_packet(const Key& key, Pak& pak, const Handler& handler)
	{
		typedef
			detail::network_mixer_handler_send_op
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		op_type op;
		return op(key, pak, handler, pval);
	}

	template<typename Key, typename Pak> inline
	bool send_packet(const Key& key, Pak& pak)
	{
		typedef
			detail::network_mixer_handler_send_op
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		op_type op;
		return op(key, pak, pval);
	}

public:
	template<typename Key, typename Handler> inline
	bool access(const Key& key, const Handler& handler)
	{
		typedef
			detail::network_mixer_handler_access_op_eins
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		op_type op;
		return op(key, handler, pval);
	}

	template<typename Key, typename Handler> inline
	bool access(const Key& key, const Handler& handler) const
	{
		typedef
			detail::network_mixer_handler_access_op_eins
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		op_type op;
		return op(key, handler, pval);
	}

public:
	template<typename Handler> inline
	void access(const Handler& handler)
	{
		typedef Handler handler_type;
		typedef
			detail::network_mixer_handler_access_op_zwei
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		op_type op;
		op(handler, pval);
	}

	template<typename Handler> inline
	void access(const Handler& handler) const
	{
		typedef Handler handler_type;
		typedef
			detail::network_mixer_handler_access_op_zwei
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		op_type op;
		op(handler, pval);
	}

public:
	template<typename Handler> inline
	void access_of_all(const Handler& handler)
	{
		typedef
			detail::network_mixer_handler_access_of_all_op
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		op_type op;
		op(handler, pval);
	}

	template<typename Handler> inline
	void access_of_all(const Handler& handler) const
	{
		typedef
			detail::network_mixer_handler_access_of_all_op
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		op_type op;
		op(handler, pval);
	}

	inline adapter_mgr_ptr_type get_adapter_mgr_ptr(void) const
	{
		return _adp_mgr_wrap.get_shared_ptr();
	}

private:
	template<std::size_t i, typename Handler> inline
	bool nth_using_method(const Handler& handler)
	{
		typedef
			detail::network_mixer_handler_using_method_op
			<
				value_ptr_type,
				params_type,
				i
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		op_type op;
		return op(handler, pval);
	}

	template<std::size_t i, typename Handler> inline
	bool nth_using_method(const Handler& handler) const
	{
		typedef
			detail::network_mixer_handler_using_method_op
			<
				value_ptr_type,
				params_type,
				i
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		op_type op;
		return op(handler, pval);
	}

	template<std::size_t i, typename Handler, typename Ret_Handler> inline
	bool nth_using_method(const Handler& handler, const Ret_Handler& ret_handler)
	{
		typedef
			detail::network_mixer_handler_using_method_op
			<
				value_ptr_type,
				params_type,
				i
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		op_type op;
		return op(handler, ret_handler, pval);
	}

	template<std::size_t i, typename Handler, typename Ret_Handler> inline
	bool nth_using_method(const Handler& handler, const Ret_Handler& ret_handler) const
	{
		typedef
			detail::network_mixer_handler_using_method_op
			<
				value_ptr_type,
				params_type,
				i
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return false;
		}

		op_type op;
		return op(handler, ret_handler, pval);
	}

public:
	template<std::size_t i, typename Handler> inline
	bool using_method(const Handler& handler)
	{
		return this_type::nth_using_method<i, Handler>(handler);
	}

	template<std::size_t i, typename Handler> inline
	bool using_method(const Handler& handler) const
	{
		return this_type::nth_using_method<i, Handler>(handler);
	}

	template<std::size_t i, typename Handler, typename Ret_Handler> inline
	bool using_method(const Handler& handler, const Ret_Handler& ret_handler)
	{
		return this_type::nth_using_method<i, Handler, Ret_Handler>(handler, ret_handler);
	}

	template<std::size_t i, typename Handler, typename Ret_Handler> inline
	bool using_method(const Handler& handler, const Ret_Handler& ret_handler) const
	{
		return this_type::nth_using_method<i, Handler, Ret_Handler>(handler, ret_handler);
	}

	template<typename Val_T, typename Handler> inline
	bool using_method(const Handler& handler)
	{
		typedef boost::mpl::find<vt_t_type, Val_T> find_t;
		typedef typename find_t::type iter_t;
		typedef typename iter_t::pos pos_t;

		return nth_using_method<pos_t::value>(handler);
	}

	template<typename Val_T, typename Handler> inline
	bool using_method(const Handler& handler) const
	{
		typedef boost::mpl::find<vt_t_type, Val_T> find_t;
		typedef typename find_t::type iter_t;
		typedef typename iter_t::pos pos_t;

		return nth_using_method<pos_t::value>(handler);
	}

	template<typename Val_T, typename Handler, typename Ret_Handler> inline
	bool using_method(const Handler& handler, const Ret_Handler& ret_handler)
	{
		typedef boost::mpl::find<vt_t_type, Val_T> find_t;
		typedef typename find_t::type iter_t;
		typedef typename iter_t::pos pos_t;

		return nth_using_method<pos_t::value>(handler, ret_handler);
	}

	template<typename Val_T, typename Handler, typename Ret_Handler>
	bool using_method(const Handler& handler, const Ret_Handler& ret_handler) const
	{
		typedef boost::mpl::find<vt_t_type, Val_T> find_t;
		typedef typename find_t::type iter_t;
		typedef typename iter_t::pos pos_t;
		return nth_using_method<pos_t::value>(handler, ret_handler);
	}

private:

	friend class yggr::system_controller::ctrl_center;

	template<typename Tag, typename CtrlCenter> inline
	void register_controller(CtrlCenter& cc)
	{
		typedef Tag tag_type;
		typedef CtrlCenter ctrl_center_type;
		typedef
			detail::network_mixer_handler_register_controller_op
			<
				value_ptr_type,
				params_type
			> op_type;

		value_ptr_type pval = _value_wrap.get_shared_ptr();
		assert(pval);
		if(!pval)
		{
			return;
		}

		op_type op;
		op.template call<tag_type>(cc, pval);
	}

private:
	boost::atomic<bool> _brun;
	service_handler_type _service_handler;
	adapter_mgr_wrap_type _adp_mgr_wrap;
	value_wrap_type _value_wrap;
};

} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_NETWORK_HANDLER_MIXER_HPP__
