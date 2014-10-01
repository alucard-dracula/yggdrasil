//adapter_mgr.hpp

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

#ifndef __YGGR_ADAPTER_ADAPTER_MANAGER_HPP__
#define __YGGR_ADAPTER_ADAPTER_MANAGER_HPP__

#include <utility>

#include <boost/bind.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/base/interface_ptr.hpp>
#include <yggr/base/exception_sort.hpp>
#include <yggr/base/exception.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/struct_support/tuple.hpp>
#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/base/error_make.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/adapter/adapter_parser_helper.hpp>
#include <yggr/adapter/adapter_register_type_def.hpp>

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
	: public boost::enable_shared_from_this< adapter_mgr<
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

private:

	typedef Conver<send_std_data_type, task_type, recv_rst_data_type> conver_type;
	typedef Mutex mutex_type;
	typedef ID_Parser_Def id_parser_def_type;

	typedef typename id_parser_def_type::send_id_type send_adapter_id_type;
	typedef typename id_parser_def_type::recv_id_type recv_adapter_id_type;

	template<typename Std_Data, typename Source>
	class base_send_adapter
	{
	public:
		typedef Std_Data std_data_type;
		typedef Source src_type;

	public:
		base_send_adapter(void) {}
		virtual ~base_send_adapter(void) {}
		virtual typename conver_type::conv_to_send_result_type
			send(src_type&, typename conver_type::conv_to_send_arg_data_type) const = 0;
	};

	template<typename Task, typename Rst_Data_Type>
	class base_recv_adapter
	{
	public:
		typedef Task task_type;
		typedef Rst_Data_Type result_data_type;

	public:
		base_recv_adapter(void) {}
		virtual ~base_recv_adapter(void){}
		virtual typename conver_type::conv_from_recv_result_type
			recv(typename conver_type::conv_from_recv_arg_data_type) const = 0;

		template<typename Handler0, typename Handler1> // mgr_handler
		static void recv_handler(bool isloaded,
									typename conver_type::conv_from_recv_arg_data_type tk,
									const boost::any& any_handler0,
									const boost::any& any_handler1)
		{
			typedef Handler0 handler0_type;
			typedef Handler1 handler1_type;

			if(!isloaded)
			{
				return;
			}

			const handler0_type* phandler0 = boost::any_cast<handler0_type>(&any_handler0);
			const handler1_type* phandler1 = boost::any_cast<handler1_type>(&any_handler1);

			assert((phandler0 && phandler1));
			(*phandler0)(tk, (*phandler1));
		}

		template<typename Tuple_Handler>
		void recv_handler_conver(typename conver_type::conv_from_recv_arg_data_type tk,
									const Tuple_Handler& handler)
		{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
			result_data_type rst(boost::move(boost::get<0>(handler)(tk)));
#else
			result_data_type rst(boost::get<0>(handler)(tk));
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
			boost::get<1>(handler)(rst);
		}

	};

	typedef base_send_adapter<send_std_data_type, src_type> base_send_adapter_type;
	typedef base_recv_adapter<task_type, recv_rst_data_type> base_recv_adapter_type;

	typedef interface_ptr<base_send_adapter_type> send_adapter_container_type;
	typedef interface_ptr<base_recv_adapter_type> recv_adapter_container_type;

	struct type_mapping
	{

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
			const typename tag_send::id_type& id(const type_mapping& right) const
			{
				return right.send_id;
			}

			const typename tag_send::fixer_type& fixer(const type_mapping& right) const
			{
				return right.psend;
			}

			void erase(type_mapping& right) const
			{
			    send_adapter_container_type tmp;
				right.psend.swap(tmp);
			}
		};

		template<typename Nil_T>
		struct id_selector<tag_recv, Nil_T>
		{
			const typename tag_recv::id_type& id(const type_mapping& right) const
			{
				return right.recv_id;
			}

			const typename tag_recv::fixer_type& fixer(const type_mapping& right) const
			{
				return right.precv;
			}

			void erase(type_mapping& right) const
			{
			    recv_adapter_container_type tmp;
				right.precv.swap(tmp);
			}
		};

	public:
		type_mapping(const send_adapter_id_type& sid, const recv_adapter_id_type& rid,
						const send_adapter_container_type& npsend,
						const recv_adapter_container_type& nprecv)
			: send_id(sid), recv_id(rid), psend(npsend), precv(nprecv)
		{
		}

		type_mapping(const type_mapping& right)
			: send_id(right.send_id), recv_id(right.recv_id),
			psend(right.psend), precv(right.precv)
		{
		}

		~type_mapping(void)
		{
		}

		type_mapping& operator=(const type_mapping& right)
		{
			send_id = right.send_id;
			recv_id = right.recv_id;
			psend = right.psend;
			precv = right.precv;
			return *this;
		}

		bool operator==(const type_mapping& right) const
		{
			return send_id == right.send_id && recv_id == right.recv_id;
		}

		bool empty(void) const
		{
			return !(psend || precv);
		}

		template<typename Tag>
		const typename Tag::id_type& id(void) const
		{
			id_selector<Tag> selector;
			return selector.id(*this);
		}

		template<typename Tag>
		const typename Tag::fixer_type& fixer(void) const
		{
			id_selector<Tag> selector;
			return selector.fixer(*this);
		}

		template<typename Tag>
		void erase(void)
		{
			id_selector<Tag> selector;
			selector.erase(*this);
		}

		send_adapter_id_type send_id;
		recv_adapter_id_type recv_id;
		send_adapter_container_type psend;
		recv_adapter_container_type precv;
	};

	typedef adapter_id_parser<typename type_mapping::tag_send,
								typename type_mapping::tag_recv,
								id_parser_def_type > id_parser_type;

	//---------------send_adapter_container_type E--------------------------------------------------------


	//---------------send_adapter S-----------------------------------------------------------------------
	template<typename Data>
	class send_adapter
		: public base_send_adapter_type
	{
	public:
		typedef Data real_data_type;
		typedef typename base_send_adapter_type::src_type src_type;
		typedef typename base_send_adapter_type::std_data_type std_data_type;

	public:

		send_adapter(void)
		{
		}

		virtual ~send_adapter(void)
		{
		}

		virtual typename conver_type::conv_to_send_result_type
			send(src_type& src, typename conver_type::conv_to_send_arg_data_type std_data) const
		{
			return conver_type::template conv_to_send<real_data_type>(src, std_data);
		}

	};

	//---------------send_adapter E--------------------------------



	//---------------recv_adapter S---------------------------------

	template<typename Data>
	class recv_adapter
		: public base_recv_adapter_type
	{
	public:
		typedef Data real_data_type;
		typedef typename base_recv_adapter_type::task_type recv_type;
		typedef typename base_recv_adapter_type::result_data_type result_data_type;

	public:
		recv_adapter(void)
		{
		}

		virtual ~recv_adapter(void)
		{
		}

		virtual typename conver_type::conv_from_recv_result_type
			recv(typename conver_type::conv_from_recv_arg_data_type data) const
		{
			return conver_type::template conv_from_recv<real_data_type>(data);
		}
	};

	//---------------recv_adapter E-----------------------------------------------------------

	//---------------map_def S----------------------------------------------------------------

	typedef yggr::safe_container::safe_multi_index_container
			<
				type_mapping,
				boost::multi_index::indexed_by
				<
					boost::multi_index::hashed_unique
					<
						boost::multi_index::tag<typename type_mapping::tag_send>,
						BOOST_MULTI_INDEX_MEMBER(type_mapping, send_adapter_id_type, send_id)
					>,

					boost::multi_index::hashed_unique
					<
						boost::multi_index::tag<typename type_mapping::tag_recv>,
						BOOST_MULTI_INDEX_MEMBER(type_mapping, recv_adapter_id_type, recv_id)
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

	typedef adapter_mgr this_type;
public:

	explicit adapter_mgr(src_type& src)
		: _src(src)
	{
	}

	~adapter_mgr(void)
	{
	}

	template<typename Real_Data>
	bool register_adapter(u32 stat = reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;
		typedef send_adapter<real_data_type> send_adapter_type;
		typedef recv_adapter<real_data_type> recv_adapter_type;

		id_parser_type parser;

		return _adapter_map.template use_method<0>(
				YGGR_PP_MIDX_MAKE_METHOD(adapter_map_type, 0)
					.insert(type_mapping(parser.template reg_value_id<typename type_mapping::tag_send, real_data_type>(),
											parser.template reg_value_id<typename type_mapping::tag_recv, real_data_type>(),
											(stat & reg_def_type::E_to_src?
												send_adapter_container_type(new send_adapter_type())
												: send_adapter_container_type()),
											(stat & reg_def_type::E_from_src?
												recv_adapter_container_type(new recv_adapter_type())
												: recv_adapter_container_type()))));

	}

	template<typename Real_Data, typename Container>
	bool register_adapter_of_container(u32 stat = reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;
		typedef Container container_type;
		typedef send_adapter<container_type> send_adapter_type;
		typedef recv_adapter<container_type> recv_adapter_type;

		id_parser_type parser;


		return _adapter_map.template use_method<0>(
				YGGR_PP_MIDX_MAKE_METHOD(adapter_map_type, 0)
					.insert(type_mapping(parser.template reg_value_id<typename type_mapping::tag_send, real_data_type>(),
											parser.template reg_value_id<typename type_mapping::tag_recv, real_data_type>(),
											(stat & reg_def_type::E_to_src?
												send_adapter_container_type(new send_adapter_type())
												: send_adapter_container_type()),
											(stat & reg_def_type::E_from_src?
												recv_adapter_container_type(new recv_adapter_type())
												: recv_adapter_container_type()))));
	}

	template<typename Container>
	bool register_adapter(const send_adapter_id_type& send_id,
							const recv_adapter_id_type& recv_id,
							u32 stat = reg_def_type::E_both)
	{
		typedef Container container_type;
		typedef send_adapter<container_type> send_adapter_type;
		typedef recv_adapter<container_type> recv_adapter_type;

		return _adapter_map.template use_method<0>(
				YGGR_PP_MIDX_MAKE_METHOD(adapter_map_type, 0)
					.insert(type_mapping(send_id, recv_id,
											(stat & reg_def_type::E_to_src?
												send_adapter_container_type(new send_adapter_type())
												: send_adapter_container_type()),
											(stat & reg_def_type::E_from_src?
												recv_adapter_container_type(new recv_adapter_type())
												: recv_adapter_container_type()))));
	}

	template<typename Real_Data>
	bool unregister_adapter(u32 stat = reg_def_type::E_both)
	{
		typedef Real_Data real_data_type;

		id_parser_type parser;

		if(stat == reg_def_type::E_both)
		{
			return _adapter_map.template use_method<0>(
						YGGR_PP_MIDX_MAKE_METHOD(adapter_map_type, 0)
							.erase(parser.template reg_value_id<typename type_mapping::tag_send, real_data_type>()));
		}

		if(stat == reg_def_type::E_to_src)
		{
			return _adapter_map.use_handler(boost::bind(&this_type::template handler_erase_adapter<typename type_mapping::tag_send>,
												this, _1,
												boost::cref(parser.template reg_value_id<typename type_mapping::tag_send, real_data_type>())));
		}

		if(stat == reg_def_type::E_from_src)
		{
			return _adapter_map.use_handler(boost::bind(&this_type::template handler_erase_adapter<typename type_mapping::tag_recv>,
												this, _1,
												boost::cref(parser.template reg_value_id<typename type_mapping::tag_recv, real_data_type>())));
		}

		return false;
	}

	bool unregister_adapter(const send_adapter_id_type& send_id, u32 stat = reg_def_type::E_both)
	{
		assert((boost::is_same<send_adapter_id_type, recv_adapter_id_type>::value
					|| stat != reg_def_type::E_from_src));


		if(stat == reg_def_type::E_both)
		{
			return _adapter_map.template use_method<0>(
					YGGR_PP_MIDX_MAKE_METHOD(adapter_map_type, 0)
						.erase(send_id));
		}

		if(stat == reg_def_type::E_to_src)
		{
			return _adapter_map.use_handler(boost::bind(&this_type::template handler_erase_adapter<typename type_mapping::tag_send>,
															this, _1, boost::cref(send_id)));
		}

		return false;
	}

	template<typename ID>
	bool unregister_adapter(const ID& recv_id, u32 stat = reg_def_type::E_both)
	{
		BOOST_MPL_ASSERT((boost::is_same<ID, recv_adapter_id_type>));
		assert(stat != reg_def_type::E_to_src);

		if(stat == reg_def_type::E_both)
		{
			return _adapter_map.use_method<0>(
					YGGR_PP_MIDX_MAKE_METHOD(adapter_map_type, 0)
						.erase(recv_id));
		}

		if(stat == reg_def_type::E_from_src)
		{
			return _adapter_map.use_handler(boost::bind(&this_type::handler_erase_adapter<type_mapping::tag_recv>,
															this, _1, boost::cref(recv_id)));
		}

		return false;
	}

	void send(typename conver_type::conv_to_send_arg_data_type data)
	{
		id_parser_type parser;

		send_adapter_container_type container;
		_adapter_map.use_handler(boost::bind(&this_type::handler_get_adapter<typename type_mapping::tag_send>,
												this, _1, boost::cref(parser.template value_id<typename type_mapping::tag_send>(data)),
												boost::ref(container)));

		if(!container)
		{
			std::stringstream ss;
			ss << "type_id = " << parser.template value_id<typename type_mapping::tag_send>(data);
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_non_reg_adp, ss.str()));
			return;
		}

		if(!(container->send(_src, data)))
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_not_send));
		}
	}

	template<typename Handler>
	bool hold_recv(const owner_id_type& id, const Handler& handler)
	{
		typedef Handler handler_type;
		return hold_recv_helper(
									id,
									boost::bind(&this_type::recv_handler<handler_type>,
													this_type::shared_from_this(),
													_1, _2
												),
									handler
								);
	}

	bool stop_recv(const owner_id_type& id)
	{
		return _src.stop_recv(id);
	}

	template<typename Mark, typename IDs>
	void drop_task(const IDs& ids)
	{
		_src.template drop_task<Mark, IDs>(ids);
	}

	template<typename Mark, typename IDs, typename KeyGetter>
	void drop_task(const IDs& ids, const KeyGetter& key_getter)
	{
		_src.template drop_task<Mark, IDs, KeyGetter>(ids, key_getter);
	}

	void drop_all_tasks(void)
	{
		_src.drop_all_tasks();
	}

	void clear(void)
	{
		_adapter_map.clear();
		_src.clear();
	}

protected:

	template<typename Tag>
	void handler_get_adapter(const typename adapter_map_type::base_type& cont,
									const typename Tag::id_type& id,
									typename Tag::fixer_type& fixer)
	{
		typedef typename adapter_map_type::base_type base_type;
		typedef typename base_type::template index_const_iterator<Tag> idx_citer_type;
		typedef typename idx_citer_type::type citer_type;

		citer_type iter = cont.template get<Tag>().find(id);
		if(iter == cont.template get<Tag>().end())
		{
			return;
		}

		fixer = (*iter).template fixer<Tag>();
	}

	template<typename Tag>
	bool handler_erase_adapter(typename adapter_map_type::base_type& cont,
									const typename Tag::id_type& id)
	{
		typedef typename adapter_map_type::base_type base_type;
		typedef typename base_type::template index_iterator<Tag> idx_iter_type;
		typedef typename idx_iter_type::type iter_type;

		iter_type iter = cont.template get<Tag>().find(id);
		if(iter == cont.template get<Tag>().end())
		{
			return false;
		}

		if(!cont.template get<Tag>().modify(iter,
							boost::bind(
											&this_type::handler_modify_erase_adapter<Tag>,
											this, _1
										)
						))
		{
			return false;
		}


		if((*iter).empty())
		{
			cont.template get<Tag>().erase(iter);
		}

		return true;
	}

	template<typename Tag>
	void handler_modify_erase_adapter(type_mapping& elem)
	{
		elem.template erase<Tag>();
	}

	template<typename Handler>
	void recv_handler(typename conver_type::conv_from_recv_arg_data_type tk,
						const Handler& handler)
	{

		id_parser_type parser;

		recv_adapter_container_type container;
		_adapter_map.use_handler(boost::bind(&this_type::handler_get_adapter<typename type_mapping::tag_recv>,
												this, _1, boost::cref(parser.template task_id<typename type_mapping::tag_recv>(tk)),
												boost::ref(container)));

		if(!container)
		{
			std::stringstream ss;
			ss << "type_name = " << parser.template task_id<typename type_mapping::tag_recv>(tk);
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_non_reg_adp, ss.str()));
			return;
		}

		container->recv_handler_conver(tk, boost::make_tuple(
												boost::bind(&base_recv_adapter_type::recv, container, _1),
																handler));
	}

	template<typename Handler0, typename Handler1>
	bool hold_recv_helper(const owner_id_type& id, const Handler0& handler0, const Handler1& handler1)
	{
		//typedef Tuple_Handler handler_type;
		//fix compile out of heap
		return _src.hold_recv(
								id,
								boost::bind(&base_recv_adapter_type::template recv_handler<Handler0, Handler1>,
											_1, _2, boost::any(handler0), boost::any(handler1))
							);
	}

private:
	src_type& _src;
	adapter_map_type _adapter_map;
};

} // namespace adapter
} // namespace yggr

#endif // __YGGR_ADAPTER_ADAPTER_MANAGER_HPP_
