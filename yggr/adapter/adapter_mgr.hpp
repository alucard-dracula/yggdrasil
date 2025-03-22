//adapter_mgr.hpp

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

#ifndef __YGGR_ADAPTER_ADAPTER_MANAGER_HPP__
#define __YGGR_ADAPTER_ADAPTER_MANAGER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/bind/bind.hpp>
#include <yggr/base/error_make.hpp>

#include <yggr/nonable/noncopyable.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/args_holder_nv.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/exception/exception_sort.hpp>
#include <yggr/exception/exception.hpp>

#include <yggr/tuple_ex/tuple.hpp>

#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>

#include <yggr/charset/string.hpp>

#include <yggr/adapter/adapter_parser_helper.hpp>
#include <yggr/adapter/adapter_register_type_def.hpp>

#include <boost/thread/shared_mutex.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <utility>

namespace yggr
{
namespace adapter
{

template<typename Src,
			typename Send_Std_Data,
			typename Recv_Rst_Data,
			typename ID_Parser_Def,
			template<typename _Send_Std_Data,
						typename _Std_Data,
						typename _Recv_Rst_Data>
			class Conver,
			typename Mutex>
class adapter_mgr;

} // namespace adapter
} // namespace yggr

namespace yggr
{
namespace adapter
{

namespace detail
{

//---------------send_adapter S---------------------

template<typename StdData,
			typename Source,
			typename Conver>
class base_send_adapter
{
public:
	typedef StdData std_data_type;
	typedef Source src_type;
	typedef Conver conver_type;
	typedef typename conver_type::conv_to_send_result_type conv_to_send_result_type;
	typedef typename conver_type::conv_to_send_arg_data_type conv_to_send_arg_data_type;

private:
	typedef base_send_adapter this_type;

public:
	virtual ~base_send_adapter(void)
	{
	}

public:
	virtual conv_to_send_result_type
		send(src_type& src, conv_to_send_arg_data_type std_data) const = 0;
};

template<typename BaseSendAdapter, typename Data>
class send_adapter
	: public BaseSendAdapter
{
public:
	typedef BaseSendAdapter base_type;

public:
	typedef Data real_data_type;
	typedef typename base_type::src_type src_type;
	typedef typename base_type::std_data_type std_data_type;
	typedef typename base_type::conver_type conver_type;
	typedef typename conver_type::conv_to_send_arg_data_type conv_to_send_arg_data_type;
	typedef typename conver_type::conv_to_send_result_type conv_to_send_result_type;

private:
	typedef send_adapter this_type;

public:
	send_adapter(void)
	{
	}

	~send_adapter(void)
	{
	}

public:
	virtual conv_to_send_result_type send(src_type& src, conv_to_send_arg_data_type std_data) const
	{
		return conver_type::template conv_to_send<real_data_type>(src, std_data);
	}
};

//---------------send_adapter E---------------------

//---------------recv_adapter S---------------------

template<typename Task,
			typename RstDataType,
			typename Conver>
class base_recv_adapter
{
public:
	typedef Task task_type;
	typedef RstDataType result_data_type;
	typedef Conver conver_type;
	typedef typename conver_type::conv_from_recv_result_type conv_from_recv_result_type;
	typedef typename conver_type::conv_from_recv_arg_data_type conv_from_recv_arg_data_type;

protected:
	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

private:
	typedef base_recv_adapter this_type;

public:
	virtual ~base_recv_adapter(void)
	{
	}

public:
	virtual conv_from_recv_result_type
		recv(conv_from_recv_arg_data_type data) const = 0;

public:
	template<typename Handler0, typename Handler1> inline // mgr_handler
	static void handler_recv(bool isloaded,
								conv_from_recv_arg_data_type tk,
								const basic_args_holder_nv_type& any_handler0,
								const basic_args_holder_nv_type& any_handler1)
	{
		typedef Handler0 handler0_type;
		typedef Handler1 handler1_type;

		if(!isloaded)
		{
			return;
		}

		const handler0_type* phandler0 = utility::args_holder_static_cast<handler0_type>(any_handler0);
		const handler1_type* phandler1 = utility::args_holder_static_cast<handler1_type>(any_handler1);

		assert((phandler0 && phandler1));

		if(!(phandler0 && phandler1))
		{
			return;
		}

		(*phandler0)(tk, (*phandler1));
	}

	template<typename Handler0, typename Handler1> inline
	static void recv_handler_conver(conv_from_recv_arg_data_type tk,
										const Handler0& handler0,
										const Handler1& handler1)
	{
		result_data_type rst(handler0(tk));
		handler1(rst);
	}
};

template<typename BaseRecvAdapter, typename Data>
class recv_adapter
	: public BaseRecvAdapter
{
public:
	typedef BaseRecvAdapter base_type;

public:
	typedef Data real_data_type;
	typedef typename base_type::task_type recv_type;
	typedef typename base_type::result_data_type result_data_type;
	typedef typename base_type::conver_type conver_type;
	typedef typename conver_type::conv_from_recv_arg_data_type conv_from_recv_arg_data_type;
	typedef typename conver_type::conv_from_recv_result_type conv_from_recv_result_type;

private:
	typedef recv_adapter this_type;

public:
	recv_adapter(void)
	{
	}

	~recv_adapter(void)
	{
	}

public:
	virtual conv_from_recv_result_type recv(conv_from_recv_arg_data_type data) const
	{
		return conver_type::template conv_from_recv<real_data_type>(data);
	}
};

//---------------recv_adapter E---------------------

//---------------adapter_mapping S---------------------

template<typename SendAdapterID,
			typename BaseSendAdapter,
			typename RecvAdapterID,
			typename BaseRecvAdapter>
struct adapter_mapping
{
public:
	typedef SendAdapterID send_adapter_id_type;
	typedef RecvAdapterID recv_adapter_id_type;

	typedef BaseSendAdapter base_send_adapter_type;
	typedef BaseRecvAdapter base_recv_adapter_type;

	typedef shared_ptr<base_send_adapter_type> send_adapter_container_type;
	typedef shared_ptr<base_recv_adapter_type> recv_adapter_container_type;

private:
	typedef adapter_mapping this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type);

public:
	struct tag_send
	{
		typedef send_adapter_id_type id_type;
		typedef send_adapter_container_type fixer_type;
	};

	struct tag_recv
	{
		typedef recv_adapter_id_type id_type;
		typedef recv_adapter_container_type fixer_type;
	};

private:
	template<typename Tag, typename Nil_T = int>
	struct id_selector;

	template<typename Nil_T>
	struct id_selector<tag_send, Nil_T>
	{
		typedef adapter_mapping outside_type;
		typedef send_adapter_id_type id_type;
		typedef send_adapter_container_type fixer_type;

		inline const id_type& id(const outside_type& right) const
		{
			return right.send_id;
		}

		inline const fixer_type& fixer(const outside_type& right) const
		{
			return right.psend;
		}

		inline void erase(outside_type& right) const
		{
			fixer_type tmp;
			right.psend.swap(tmp);
		}

		inline void swap(outside_type& right, fixer_type& tmp) const
		{
			right.psend.swap(tmp);
		}
	};

	template<typename Nil_T>
	struct id_selector<tag_recv, Nil_T>
	{
		typedef adapter_mapping outside_type;
		typedef recv_adapter_id_type id_type;
		typedef recv_adapter_container_type fixer_type;

		inline const id_type& id(const outside_type& right) const
		{
			return right.recv_id;
		}

		inline const fixer_type& fixer(const outside_type& right) const
		{
			return right.precv;
		}

		inline void erase(outside_type& right) const
		{
			fixer_type tmp;
			right.precv.swap(tmp);
		}

		inline void swap(outside_type& right, fixer_type& tmp) const
		{
			right.precv.swap(tmp);
		}
	};

public:
	adapter_mapping(const send_adapter_id_type& sid,
					const recv_adapter_id_type& rid,
					const send_adapter_container_type& npsend,
					const recv_adapter_container_type& nprecv)
		: send_id(sid), recv_id(rid), psend(npsend), precv(nprecv)
	{
	}

	adapter_mapping(BOOST_RV_REF(this_type) right)
		: send_id(boost::move(right.send_id)),
			recv_id(boost::move(right.recv_id)),
			psend(boost::move(right.psend)),
			precv(boost::move(right.precv))
	{
	}

	adapter_mapping(const this_type& right)
		: send_id(right.send_id), recv_id(right.recv_id),
		psend(right.psend), precv(right.precv)
	{
	}

	~adapter_mapping(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		if(this == rvref_to_pointer(boost::move(right)))
		{
			return *this;
		}

		copy_or_move_or_swap(send_id, boost::move(right.send_id));
		copy_or_move_or_swap(recv_id, boost::move(right.recv_id));
		copy_or_move_or_swap(psend, boost::move(right.psend));
		copy_or_move_or_swap(precv, boost::move(right.precv));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		send_id = right.send_id;
		recv_id = right.recv_id;
		psend = right.psend;
		precv = right.precv;
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

		::yggr::swap(send_id, right.send_id);
		::yggr::swap(recv_id, right.recv_id);
		::yggr::swap(psend, right.psend);
		::yggr::swap(precv, right.precv);
	}

public:
	inline bool compare_eq(const this_type& right) const
	{
		return
			(this == boost::addressof(right))
			|| (send_id == right.send_id && recv_id == right.recv_id);
	}

	inline bool empty(void) const
	{
		return !(psend || precv);
	}

	template<typename Tag> inline
	const typename Tag::id_type& id(void) const
	{
		id_selector<Tag> selector;
		return selector.id(*this);
	}

	template<typename Tag> inline
	const typename Tag::fixer_type& fixer(void) const
	{
		id_selector<Tag> selector;
		return selector.fixer(*this);
	}

	template<typename Tag> inline
	void erase(void)
	{
		id_selector<Tag> selector;
		selector.erase(*this);
	}

	template<typename Tag> inline
	void swap_fixer(typename Tag::fixer_type& tmp)
	{
		id_selector<Tag> selector;
		selector.swap(*this, tmp);
	}

	inline bool merge(const this_type& right)
	{
		if(this_type::compare_eq(right))
		{
			return false;
		}
		else
		{
			bool bs = false, br = false;
			if(!psend && right.psend)
			{
				psend = right.psend;
				bs = true;
			}

			if(!precv && right.precv)
			{
				precv = right.precv;
				br = true;
			}

			return bs || br;
		}
	}

public:
	send_adapter_id_type send_id;
	recv_adapter_id_type recv_id;
	send_adapter_container_type psend;
	recv_adapter_container_type precv;
};

template<typename T1, typename T2, typename T3, typename T4> inline
bool operator==(const adapter_mapping<T1, T2, T3, T4>& l,
				const adapter_mapping<T1, T2, T3, T4>& r)
{
	return l.compare_eq(r);
}

template<typename T1, typename T2, typename T3, typename T4> inline
bool operator!=(const adapter_mapping<T1, T2, T3, T4>& l,
				const adapter_mapping<T1, T2, T3, T4>& r)
{
	return !(l.compare_eq(r));
}

//---------------adapter_mapping E---------------------

//---------------unregister_adapter_helper S---------------------

template<typename Mapping,
			typename SendAdapterID,
			typename RecvAdapterID,
			bool send_recv_id_same>
class unregister_adapter_helper;

template<typename Mapping,
			typename SendAdapterID,
			typename RecvAdapterID>
class unregister_adapter_helper<Mapping, SendAdapterID, RecvAdapterID, true>
{
public:
	typedef Mapping t_mapping_type;
	typedef SendAdapterID send_adapter_id_type;
	//typedef RecvAdapterID recv_adapter_id_type;
	typedef adapter_register_type_def reg_def_type;

private:
	typedef unregister_adapter_helper this_type;

public:
	template<typename AdapterMgr> inline
	bool operator()(AdapterMgr& outside, const send_adapter_id_type& id, u32 tag, u32 sat) const
	{
		assert(tag < 2);

		switch(sat)
		{
		case reg_def_type::E_both:
			return tag? outside.pro_unregister_adapter_both_of_from_src(id)
						: outside.pro_unregister_adapter_both_of_to_src(id);

		case reg_def_type::E_to_src:
			return outside.template pro_unregister_adapter_once<typename t_mapping_type::tag_send>(id);

		case reg_def_type::E_from_src:
			return outside.template pro_unregister_adapter_once<typename t_mapping_type::tag_recv>(id);

		default:
			return false;
		}
	}
};

template<typename Mapping, typename SendAdapterID, typename RecvAdapterID>
class unregister_adapter_helper<Mapping, SendAdapterID, RecvAdapterID, false>
{
public:
	typedef Mapping t_mapping_type;
	typedef SendAdapterID send_adapter_id_type;
	typedef RecvAdapterID recv_adapter_id_type;
	typedef adapter_register_type_def reg_def_type;

private:
	typedef unregister_adapter_helper this_type;

public:
	template<typename AdapterMgr> inline
	bool operator()(AdapterMgr& outside, const send_adapter_id_type& id, u32 /*tag*/, u32 sat) const
	{
		switch(sat)
		{
		case reg_def_type::E_both:
			return outside.pro_unregister_adapter_both_of_to_src(id);

		case reg_def_type::E_to_src:
			return outside.template pro_unregister_adapter_once<typename t_mapping_type::tag_send>(id);

		case reg_def_type::E_from_src:
			return false;

		default:
			return false;
		}
	}

	template<typename AdapterMgr> inline
	bool operator()(AdapterMgr& outside, const recv_adapter_id_type& id, u32 /*tag*/, u32 sat) const
	{
		switch(sat)
		{
		case reg_def_type::E_both:
			return outside.pro_unregister_adapter_both_of_from_src(id);

		case reg_def_type::E_to_src:
			return false;

		case reg_def_type::E_from_src:
			return outside.template pro_unregister_adapter_once<typename t_mapping_type::tag_recv>(id);

		default:
			return false;
		}
	}
};

//---------------unregister_adapter_helper E---------------------

template<typename ID, typename ChkID>
struct default_tag_basic
	: public
		boost::mpl::if_
		<
			boost::is_same<ID, ChkID>,
			boost::mpl::int_<0>,
			boost::mpl::int_<1>
		>::type
{
};

} // namespace detail

} // namespace adapter
} // namespace yggr

//---------------adapter_mgr---------------------

namespace yggr
{
namespace adapter
{

template<typename Src,
			typename Send_Std_Data,
			typename Recv_Rst_Data,
			typename ID_Parser_Def,
			template<typename _Send_Std_Data,
						typename _Std_Data,
						typename _Recv_Rst_Data>
			class Conver,
			typename Mutex = boost::shared_mutex>
class adapter_mgr
	: public
		boost::enable_shared_from_this
		<
			adapter_mgr
			<
				Src,
				Send_Std_Data,
				Recv_Rst_Data,
				ID_Parser_Def,
				Conver,
				Mutex
			>
		>,
		private nonable::noncopyable
{
public:
	typedef Src src_type;
	typedef typename src_type::task_type task_type;
	typedef typename task_type::owner_id_type owner_id_type;

	typedef Send_Std_Data send_std_data_type;
	typedef Recv_Rst_Data recv_rst_data_type;

	typedef adapter_register_type_def reg_def_type;

	YGGR_STATIC_CONSTANT(u32, E_tag_send = 0);
	YGGR_STATIC_CONSTANT(u32, E_tag_recv = 1);

private:
	typedef utility::basic_args_holder_nv basic_args_holder_nv_type;

private:
	typedef Conver<send_std_data_type, task_type, recv_rst_data_type> conver_type;
	typedef Mutex mutex_type;
	typedef ID_Parser_Def id_parser_def_type;

	typedef typename id_parser_def_type::send_id_type send_adapter_id_type;
	typedef typename id_parser_def_type::recv_id_type recv_adapter_id_type;

	typedef detail::base_send_adapter<send_std_data_type, src_type, conver_type> base_send_adapter_type;
	typedef detail::base_recv_adapter<task_type, recv_rst_data_type, conver_type> base_recv_adapter_type;

	typedef shared_ptr<base_send_adapter_type> send_adapter_container_type;
	typedef shared_ptr<base_recv_adapter_type> recv_adapter_container_type;

	typedef
		detail::adapter_mapping
		<
			send_adapter_id_type,
			base_send_adapter_type,
			recv_adapter_id_type,
			base_recv_adapter_type
		> t_mapping_type;

	typedef
		adapter_id_parser
		<
			typename t_mapping_type::tag_send,
			typename t_mapping_type::tag_recv,
			id_parser_def_type
		> id_parser_type;

	//---------------map_def S----------------------------------------------------------------

	typedef
		yggr::safe_container::safe_multi_index_container
		<
			t_mapping_type,
			boost::multi_index::indexed_by
			<
				boost::multi_index::hashed_unique
				<
					boost::multi_index::tag<typename t_mapping_type::tag_send>,
					BOOST_MULTI_INDEX_MEMBER(t_mapping_type, send_adapter_id_type, send_id)
				>,

				boost::multi_index::hashed_unique
				<
					boost::multi_index::tag<typename t_mapping_type::tag_recv>,
					BOOST_MULTI_INDEX_MEMBER(t_mapping_type, recv_adapter_id_type, recv_id)
				>
			>,
			mutex_type
		> adapter_map_type;

	//---------------map_def E-----------------------------------------------

	//class error S----------------------------------------------------------

	ERROR_MAKER_BEGIN("adapter_mgr")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_4)
			ERROR_CODE_DEF(E_invalid_non_reg_adp)
			ERROR_CODE_DEF(E_invalid_not_send)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_4)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_non_reg_adp, "non register adapter")
			ERROR_CODE_MSG(E_invalid_not_send, "not send")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

	//class error E----------------------------------------------------------

private:
	typedef adapter_mgr this_type;

public:
	explicit adapter_mgr(src_type& src)
		: _src(src)
	{
	}

	~adapter_mgr(void)
	{
	}

public:
	template<typename Real_Data> inline
	bool register_adapter(u32 stat = reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;
		typedef detail::send_adapter<base_send_adapter_type, real_data_type> send_adapter_type;
		typedef detail::recv_adapter<base_recv_adapter_type, real_data_type> recv_adapter_type;

		typedef typename t_mapping_type::tag_send now_tag_type;

		if(!stat)
		{
			return false;
		}
		else
		{
			id_parser_type parser;

			t_mapping_type mapping(parser.template reg_value_id<typename t_mapping_type::tag_send, real_data_type>(),
									parser.template reg_value_id<typename t_mapping_type::tag_recv, real_data_type>(),
									(stat & reg_def_type::E_to_src?
										construct_shared<base_send_adapter_type>(yggr_nothrow_new send_adapter_type())
										: send_adapter_container_type()),
									(stat & reg_def_type::E_from_src?
										construct_shared<base_recv_adapter_type>(yggr_nothrow_new recv_adapter_type())
										: recv_adapter_container_type()));

			return
				_adapter_map
					.template get<now_tag_type>()
					.insert(
						mapping,
						boost::bind(
							&this_type::handler_insert_adapter,
							_1, _2, boost::cref(mapping)));
		}
	}

	template<typename Real_Data, typename Container> inline
	bool register_adapter_of_container(u32 stat = reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;
		typedef Container container_type;
		typedef detail::send_adapter<base_send_adapter_type, container_type> send_adapter_type;
		typedef detail::recv_adapter<base_recv_adapter_type, container_type> recv_adapter_type;
		typedef typename t_mapping_type::tag_send now_tag_type;

		if(!stat)
		{
			return false;
		}
		else
		{
			id_parser_type parser;

			t_mapping_type mapping(parser.template reg_value_id<typename t_mapping_type::tag_send, real_data_type>(),
									parser.template reg_value_id<typename t_mapping_type::tag_recv, real_data_type>(),
									(stat & reg_def_type::E_to_src?
										construct_shared<base_send_adapter_type>(yggr_nothrow_new send_adapter_type())
										: send_adapter_container_type()),
									(stat & reg_def_type::E_from_src?
										construct_shared<base_recv_adapter_type>(yggr_nothrow_new recv_adapter_type())
										: recv_adapter_container_type()));

			return
				_adapter_map
					.template get<now_tag_type>()
					.insert(
						mapping,
						boost::bind(
							&this_type::handler_insert_adapter,
							_1, _2, boost::cref(mapping)));
		}
	}

	template<typename Container> inline
	bool register_adapter(const send_adapter_id_type& send_id,
							const recv_adapter_id_type& recv_id,
							u32 stat = reg_def_type::E_both)
	{
		typedef Container container_type;
		typedef detail::send_adapter<base_send_adapter_type, container_type> send_adapter_type;
		typedef detail::recv_adapter<base_recv_adapter_type, container_type> recv_adapter_type;
		typedef typename t_mapping_type::tag_send now_tag_type;

		if(!stat)
		{
			return false;
		}
		else
		{
			t_mapping_type mapping(send_id, recv_id,
									(stat & reg_def_type::E_to_src?
										construct_shared<base_send_adapter_type>(yggr_nothrow_new send_adapter_type())
										: send_adapter_container_type()),
									(stat & reg_def_type::E_from_src?
										construct_shared<base_recv_adapter_type>(yggr_nothrow_new recv_adapter_type())
										: recv_adapter_container_type()));

			return
				_adapter_map
					.template get<now_tag_type>()
					.insert(
						mapping,
						boost::bind(
							&this_type::handler_insert_adapter,
							_1, _2, boost::cref(mapping)));
		}
	}

	template<typename Real_Data> inline
	bool unregister_adapter(u32 stat = reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;

		id_parser_type parser;

		switch(stat)
		{
		case reg_def_type::E_both:
			return
				this_type::pro_unregister_adapter_both_of_to_src(
					parser.template reg_value_id<typename t_mapping_type::tag_send, real_data_type>());

		case reg_def_type::E_to_src:
			return
				this_type::pro_unregister_adapter_once<typename t_mapping_type::tag_send>(
					parser.template reg_value_id<typename t_mapping_type::tag_send, real_data_type>());

		case reg_def_type::E_from_src:
			return
				this_type::pro_unregister_adapter_once<typename t_mapping_type::tag_recv>(
					parser.template reg_value_id<typename t_mapping_type::tag_recv, real_data_type>());

		default:
			return false;
		}
	}

private:
	template<typename, typename, typename, bool>
	friend class detail::unregister_adapter_helper;

	typedef
		detail::unregister_adapter_helper
		<
			t_mapping_type,
			send_adapter_id_type,
			recv_adapter_id_type,
			boost::is_same<send_adapter_id_type, recv_adapter_id_type>::value
		> unregister_adapter_helper_type;

public:
	template<typename ID>
	struct default_tag
		: public detail::default_tag_basic<ID, send_adapter_id_type>::type
	{
	};

public:
	template<typename ID> inline
	bool unregister_adapter(const ID& id,
								u32 tag = default_tag<ID>::value,
								u32 sat = reg_def_type::E_both)
	{
		unregister_adapter_helper_type helper;
		return helper(*this, id, tag, sat);
	}

	void send(typename conver_type::conv_to_send_arg_data_type data)
	{
		id_parser_type parser;

		send_adapter_container_type container
			= _adapter_map.template get<typename t_mapping_type::tag_send>().find(
				parser.template value_id<typename t_mapping_type::tag_send>(data),
				boost::bind(
					&this_type::handler_get_adapter<typename t_mapping_type::tag_send>,
					_1, _2));

		if(!container)
		{
			std::stringstream ss;
			ss << "type_id = " << parser.template value_id<typename t_mapping_type::tag_send>(data);
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_non_reg_adp, ss.str()));
			return;
		}

		if(!(container->send(_src, data)))
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_not_send));
		}
	}

	template<typename Handler> inline
	bool hold_recv(const owner_id_type& id, const Handler& handler)
	{
		typedef Handler handler_type;
		return
			this_type::hold_recv_helper(
				id,
				boost::bind(
					&this_type::template handler_recv<handler_type>,
					this_type::shared_from_this(), _1, _2),
				handler);
	}

	inline bool stop_recv(const owner_id_type& id)
	{
		return _src.stop_recv(id);
	}

	template<typename Mark, typename IDs> inline
	void drop_task(const IDs& ids)
	{
		_src.template drop_task<Mark, IDs>(ids);
	}

	template<typename Mark, typename IDs, typename KeyGetter> inline
	void drop_task(const IDs& ids, const KeyGetter& key_getter)
	{
		_src.template drop_task<Mark, IDs, KeyGetter>(ids, key_getter);
	}

	inline void drop_all_tasks(void)
	{
		_src.drop_all_tasks();
	}

	inline void clear(void)
	{
		_adapter_map.clear();
		_src.clear();
	}

protected:
	inline bool pro_unregister_adapter_both_of_to_src(const send_adapter_id_type& send_id)
	{
		return _adapter_map.template get<0>().erase(send_id);
	}

	inline bool pro_unregister_adapter_both_of_from_src(const recv_adapter_id_type& recv_id)
	{
		return _adapter_map.template get<1>().erase(recv_id);
	}

	template<typename Tag> inline
	bool pro_unregister_adapter_once(const typename Tag::id_type& id)
	{
		typedef Tag tag_type;
		typedef typename tag_type::fixer_type fixer_type;

		fixer_type tmp;
		return
			_adapter_map
				.template get<tag_type>()
				.find(
					id,
					boost::bind(
						&this_type::handler_erase_adapter<tag_type>,
						_1, _2, boost::ref(tmp)));
	}

	static bool handler_insert_adapter(typename adapter_map_type::base_type& base,
										const
											boost::fusion::pair
											<
												typename
													safe_container::index
													<
														adapter_map_type,
														typename t_mapping_type::tag_send
													>::type::index_id_type,
												std::pair
												<
													typename
														safe_container::index
														<
															adapter_map_type,
															typename t_mapping_type::tag_send
														>::type::iterator,
													bool
												>
											>& rst,
											const t_mapping_type& mapping)
	{

		typedef typename t_mapping_type::tag_send tag_type;
		typedef typename adapter_map_type::base_type base_type;
		typedef typename
			boost::multi_index::index
			<
				base_type,
				tag_type
			>::type cont_type;


		if(rst.second.second)
		{
			return true;
		}
		else
		{
			cont_type& cont = base.template get<tag_type>();
			if(rst.second.first == cont.end())
			{
				return false;
			}
			else
			{
				return (const_cast<t_mapping_type&>(*rst.second.first)).merge(mapping);
			}
		}
	}

	template<typename Tag>
	static typename Tag::fixer_type
		handler_get_adapter(const typename adapter_map_type::base_type& cont,
							const
								boost::fusion::pair
								<
									typename
										safe_container::index
										<
											adapter_map_type,
											Tag
										>::type::index_id_type,
									typename
										safe_container::index
										<
											adapter_map_type,
											Tag
										>::type::const_iterator
								>& rst)
	{
		typedef typename Tag::fixer_type ret_type;

		return (rst.second == cont.template get<Tag>().end())?
					ret_type() : (*rst.second).template fixer<Tag>();
	}

	template<typename Tag>
	static bool handler_erase_adapter(typename adapter_map_type::base_type& cont,
										const
											boost::fusion::pair
											<
												typename
													safe_container::index
													<
														adapter_map_type,
														Tag
													>::type::index_id_type,
												typename
													safe_container::index
													<
														adapter_map_type,
														Tag
													>::type::iterator
											>& rst,
											typename Tag::fixer_type& fixer)
	{

		if(rst.second == cont.template get<Tag>().end())
		{
			return false;
		}
		else
		{
			t_mapping_type& val_ref = const_cast<t_mapping_type&>(*(rst.second));
			val_ref.template swap_fixer<Tag>(fixer);

			if(val_ref.empty())
			{
				cont.template get<Tag>().erase(rst.second);
			}
			return true;
		}
	}

	template<typename Handler>
	void handler_recv(typename conver_type::conv_from_recv_arg_data_type tk,
						const Handler& handler)
	{
		id_parser_type parser;

		recv_adapter_container_type container
			= _adapter_map.template get<typename t_mapping_type::tag_recv>().find(
				parser.template value_id<typename t_mapping_type::tag_recv>(tk),
				boost::bind(
					&this_type::handler_get_adapter<typename t_mapping_type::tag_recv>,
					_1, _2));

		if(!container)
		{
			std::stringstream ss;
			ss << "type_name = " << parser.template task_id<typename t_mapping_type::tag_recv>(tk);
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_non_reg_adp, ss.str()));
		}
		else
		{
			base_recv_adapter_type::recv_handler_conver(
				tk,
				boost::bind(&base_recv_adapter_type::recv, container, _1),
				handler);
		}
	}

	template<typename Handler0, typename Handler1> inline
	bool hold_recv_helper(const owner_id_type& id, const Handler0& handler0, const Handler1& handler1)
	{
		//typedef Any_Handler handler_type;
		//fix compile out of heap

		typedef Handler0 handler0_type;
		typedef Handler1 handler1_type;

		typedef utility::args_holder_nv<handler0_type> holder_handler0_type;
		typedef utility::args_holder_nv<handler1_type> holder_handler1_type;

		holder_handler0_type any_handler0(handler0);
		holder_handler1_type any_handler1(handler1);

		return
			_src.hold_recv(
				id,
				boost::bind(
					&base_recv_adapter_type::template handler_recv<handler0_type, handler1_type>,
					_1, _2, any_handler0, any_handler1 ) );
	}

private:
	src_type& _src;
	adapter_map_type _adapter_map;
};

} // namespace adapter
} // namespace yggr

#endif // __YGGR_ADAPTER_ADAPTER_MANAGER_HPP__
