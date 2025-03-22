//log_accesser.hpp

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

#ifndef __YGGR_LOG_LOG_ACCESSER_HPP__
#define __YGGR_LOG_LOG_ACCESSER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>
#include <yggr/safe_container/safe_multi_index_container.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/multi_index/mem_fun.hpp>

namespace yggr
{
namespace log
{
namespace detail
{

template<typename MsgID, typename OpCaller>
struct log_accesser_op_caller_container
{
public:
	typedef MsgID msg_id_type;
	typedef OpCaller op_caller_type;
	typedef typename op_caller_type::id_type op_caller_id_type;
	typedef ::yggr::shared_ptr<op_caller_type> op_caller_ptr_type;

private:
	typedef log_accesser_op_caller_container this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	log_accesser_op_caller_container(void)
		: _msg_id(), _op_caller_id(), _pcaller()
	{
	}

	log_accesser_op_caller_container(const msg_id_type& msg_id, const op_caller_ptr_type& pcaller)
		: _msg_id(msg_id),
			_op_caller_id(pcaller? pcaller->id() : op_caller_id_type()),
			_pcaller(pcaller)
	{
	}

	log_accesser_op_caller_container(BOOST_RV_REF(this_type) right)
		: _msg_id(boost::move(right._msg_id)),
			_op_caller_id(boost::move(right._op_caller_id)),
			_pcaller(boost::move(right._pcaller))
	{
	}

	log_accesser_op_caller_container(const this_type& right)
		: _msg_id(right._msg_id),
			_op_caller_id(right._op_caller_id),
			_pcaller(right._pcaller)
	{
	}

	~log_accesser_op_caller_container(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_msg_id, boost::move(right._msg_id));
		copy_or_move_or_swap(_op_caller_id, boost::move(right._op_caller_id));
		copy_or_move_or_swap(_pcaller, boost::move(right._pcaller));

		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_msg_id = right._msg_id;
		_op_caller_id = right._op_caller_id;
		_pcaller = right._pcaller;

		return *this;
	}

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

		yggr::swap(_msg_id, right._msg_id);
		yggr::swap(_op_caller_id, right._op_caller_id);
		_pcaller.swap(right._pcaller);
	}

public:
	inline const msg_id_type& msg_id(void) const
	{
		return _msg_id;
	}

	inline const op_caller_id_type& caller_id(void) const
	{
		return _op_caller_id;
	}

	inline op_caller_ptr_type caller(void) const
	{
		return _pcaller;
	}

	inline op_caller_ptr_type& caller(void)
	{
		return _pcaller;
	}

private:
	msg_id_type _msg_id;
	op_caller_id_type _op_caller_id;
	op_caller_ptr_type _pcaller;
};

} // namespace detail
} // namespace log
} // namespace yggr

namespace yggr
{
namespace log
{

template<typename MSG_ID, typename Op, typename Op_Caller>
class log_accesser
	: private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Op op_type;
	typedef MSG_ID msg_id_type;
	typedef Op_Caller op_caller_type;
	typedef typename op_caller_type::id_type op_caller_id_type;

	// user coder used
	typedef typename op_type::init_type init_type;

	typedef ::yggr::shared_ptr<op_caller_type> op_caller_ptr_type;

	struct tag_msg_id { typedef msg_id_type type; };
	struct tag_op_caller_id { typedef op_caller_id_type type; };

private:
	typedef
		detail::log_accesser_op_caller_container
		<
			msg_id_type,
			op_caller_type
		> op_caller_container_type;

	typedef
		safe_container::safe_multi_index_container
		<
			op_caller_container_type,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<tag_msg_id>,
					BOOST_MULTI_INDEX_CONST_MEM_FUN(op_caller_container_type, const msg_id_type&, msg_id) >,

				boost::multi_index::hashed_non_unique<
					boost::multi_index::tag<tag_op_caller_id>,
					BOOST_MULTI_INDEX_CONST_MEM_FUN(op_caller_container_type, const op_caller_id_type&, caller_id)>
			>
		> caller_cont_type;

	typedef typename caller_cont_type::size_type size_type;

private:
	typedef log_accesser this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	log_accesser(BOOST_FWD_REF(Args)... args)
		: _op(boost::forward<Args>(args)...)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		log_accesser( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: _op( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ){ }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg

#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )

#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~log_accesser(void)
	{
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	bool registrer_msg(const msg_id_type& msg_id, BOOST_FWD_REF(Args)... args)
	{
		op_caller_ptr_type pcaller =
			construct_shared<op_caller_type>(yggr_nothrow_new op_caller_type(boost::forward<Args>(args)...));
		op_caller_container_type op_caller_cont(msg_id, pcaller);

		return
			pcaller && !(pcaller->empty())
			&& _caller_cont.template get<tag_op_caller_id>().find(
					op_caller_cont.caller_id(),
					boost::bind(
						&this_type::prv_handler_register_msg,
						this, _1, _2, boost::ref(op_caller_cont)));
	}


#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool register_msg(const msg_id_type& msg_id \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			op_caller_ptr_type pcaller = \
				construct_shared<op_caller_type>( yggr_nothrow_new op_caller_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			op_caller_container_type op_caller_cont(msg_id, pcaller); \
			return pcaller && !(pcaller->empty()) \
					&& _caller_cont.template get<tag_op_caller_id>() \
							.find(op_caller_cont.caller_id(), \
									boost::bind(&this_type::prv_handler_register_msg, \
													this, _1, _2, boost::ref(op_caller_cont))); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline bool register_msg(const msg_id_type& msg_id, const op_caller_type& caller)
	{
		op_caller_ptr_type pcaller = construct_shared<op_caller_type>( yggr_nothrow_new op_caller_type(caller));
		op_caller_container_type op_caller_cont(msg_id, pcaller);

		return
			pcaller
				&& !caller.empty()
				&& _caller_cont.template get<tag_op_caller_id>().find(
					op_caller_cont.caller_id(),
					boost::bind(
						&this_type::prv_handler_register_msg,
						this, _1, _2, boost::ref(op_caller_cont)));
	}

	inline bool register_msg(const msg_id_type& msg_id, const op_caller_ptr_type& pcaller)
	{
		op_caller_container_type op_caller_cont(msg_id, pcaller);

		return
			pcaller
				&& !(pcaller->empty())
				&& _caller_cont.template get<tag_op_caller_id>().find(
						op_caller_cont.caller_id(),
						boost::bind(
							&this_type::prv_handler_register_msg,
							this, _1, _2, boost::ref(op_caller_cont)));
	}

	inline bool unregister_msg(const msg_id_type& msg_id)
	{
		return !!_caller_cont.template get<tag_msg_id>().erase(msg_id);
	}

	inline size_type clear_caller(const op_caller_id_type& caller_id)
	{
		return _caller_cont.template get<tag_op_caller_id>().erase(caller_id);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Value, typename ...Args>
	bool append(const msg_id_type& id, const Value& val, BOOST_FWD_REF(Args)... args)
	{
		op_caller_ptr_type ptr
			= _caller_cont.template get<tag_msg_id>().find(
				id,
				boost::bind(
					&this_type::prv_handler_get_caller<tag_msg_id>,
					this, _1, _2));

		return ptr && ptr->call_append(_op, val, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename Value \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
		bool append(const msg_id_type& id, \
						const Value& val \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ){ \
			op_caller_ptr_type ptr \
				= _caller_cont.template get<tag_msg_id>() \
					.find(id, boost::bind(&this_type::prv_handler_get_caller<tag_msg_id>, \
											this, _1, _2)); \
			return ptr && ptr->call_append(_op, val YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
														YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Value, typename ...Args > inline
	bool append_any(const Value& val, BOOST_FWD_REF(Args)... args)
	{
		return _op.append_any(val, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template<typename Value \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline\
		bool append_any(const Value& val \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ){ \
			return _op.append_any(val YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename ...Args > inline
	bool clear( const msg_id_type& id, BOOST_FWD_REF(Args)... args)
	{
		op_caller_ptr_type ptr
			= _caller_cont.template get<tag_msg_id>().find(
				id,
				boost::bind(
					&this_type::prv_handler_get_caller<tag_msg_id>, this, _1, _2));

		return ptr && ptr->call_clear(_op, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool clear( const msg_id_type& id \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ){ \
			op_caller_ptr_type ptr \
				= _caller_cont.template get<tag_msg_id>().find( \
					id, boost::bind(&this_type::prv_handler_get_caller<tag_msg_id>, this, _1, _2)); \
			return ptr && ptr->call_clear( \
							_op YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename ID_Tag, typename ...Args > inline
	bool clear( const typename ID_Tag::type& id, BOOST_FWD_REF(Args)... args)
	{
		typedef ID_Tag id_tag_type;
		op_caller_ptr_type ptr
			= _caller_cont.template get<id_tag_type>().find(
				id,
				boost::bind(
					&this_type::prv_handler_get_caller<id_tag_type>, this, _1, _2));

		return ptr && ptr->call_clear(_op, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename ID_Tag \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool clear( const typename ID_Tag::type& id \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ){ \
			typedef ID_Tag id_tag_type; \
			op_caller_ptr_type ptr \
				= _caller_cont.template get<id_tag_type>().find( \
					id, boost::bind(&this_type::prv_handler_get_caller<id_tag_type>, this, _1, _2)); \
			return ptr && ptr->call_clear( \
							_op YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename ...Args > inline
	bool clear_any( BOOST_FWD_REF(Args)... args )
	{
		return _op.clear_any( boost::forward<Args>(args)... );
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool clear_any( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ){ \
			return _op.clear_any( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	bool search( const msg_id_type& id, BOOST_FWD_REF(Args)... args) const
	{
		op_caller_ptr_type ptr
			= _caller_cont.template get<tag_msg_id>().find(
				id,
				boost::bind(
					&this_type::prv_handler_get_caller<tag_msg_id>,
					this, _1, _2 ));

		return ptr && ptr->call_search(_op, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool search( const msg_id_type& id \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			op_caller_ptr_type ptr \
				= _caller_cont.template get<tag_msg_id>().find( \
					id, boost::bind(&this_type::prv_handler_get_caller<tag_msg_id>, this, _1, _2 )); \
			return \
				ptr && ptr->call_search( \
					_op YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename ID_Tag, typename ...Args > inline
	bool search( const typename ID_Tag::type& id, BOOST_FWD_REF(Args)... args) const
	{
		typedef ID_Tag id_tag_type;
		op_caller_ptr_type ptr
			= _caller_cont.template get<id_tag_type>().find(
				id,
				boost::bind(
					&this_type::prv_handler_get_caller<id_tag_type>,
					this, _1, _2));

		return ptr && ptr->call_search(_op, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename ID_Tag \
					YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		bool search( const typename ID_Tag::type& id \
						YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const{ \
			typedef ID_Tag id_tag_type; \
			op_caller_ptr_type ptr \
				= _caller_cont.template get<id_tag_type>().find( \
					id, boost::bind(&this_type::prv_handler_get_caller<id_tag_type>, this, _1, _2)); \
			return ptr && ptr->call_search( \
							_op YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	bool search_any(BOOST_FWD_REF(Args)... args) const
	{
		return _op.search_any(boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		bool search_any( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
			return _op.search_any( YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:
	inline bool prv_handler_register_msg(typename caller_cont_type::base_type& base,
											boost::fusion::pair
											<
												typename
													safe_container::index
													<
														caller_cont_type, tag_op_caller_id
													>::type::index_id_type,
												typename
													safe_container::index
													<
														caller_cont_type,
														tag_op_caller_id
													>::type::iterator
											> rst,
											op_caller_container_type& op_caller_cont)
	{
		typedef typename caller_cont_type::base_type base_type;
		typedef typename boost::multi_index::index<base_type, tag_op_caller_id>::type cont_type;

		cont_type& cont = base.template get<tag_op_caller_id>();
		if(rst.second != cont.end())
		{
			op_caller_cont.caller() = rst.second->caller();
		}

		return cont.insert(boost::move(op_caller_cont)).second;
	}

	template<typename ID_Tag> inline
	op_caller_ptr_type prv_handler_get_caller(const typename caller_cont_type::base_type& base,
												boost::fusion::pair
												<
													typename
														safe_container::index
														<
															caller_cont_type,
															ID_Tag
														>::type::index_id_type,
													typename
														safe_container::index
														<
															caller_cont_type,
															ID_Tag
														>::type::const_iterator
												> rst) const
	{
		typedef ID_Tag id_tag_type;
		return
			rst.second == base.template get<id_tag_type>().end()?
				op_caller_ptr_type()
				: rst.second->caller();
	}

private:
	op_type _op;
	caller_cont_type _caller_cont;
};

} // namespace log_accesser
} // namespace yggr

#endif //__YGGR_LOG_LOG_ACCESSER_HPP__
