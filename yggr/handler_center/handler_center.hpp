//handler_center.hpp

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

#ifndef __YGGR_HANDLER_CENTER_HANDLER_CENTER_HPP__
#define __YGGR_HANDLER_CENTER_HANDLER_CENTER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/base/error_make.hpp>

#include <yggr/bind/bind.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/exception/exception_sort.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/args_anchor.hpp>
#include <yggr/utility/args_holder.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/support/operator_t.hpp>

#include <yggr/packet/packet_info.hpp>
#include <yggr/packet/packet_info_caster.hpp>

#include <yggr/charset/string.hpp>

#include <yggr/caller/packet_info_to_params_caller.hpp>

#include <yggr/func/foo_t_info.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/thread/mutex.hpp>

#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/smart_ptr/enable_shared_from_raw.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/mpl/if.hpp>

#include <cassert>

namespace yggr
{
namespace handler_center
{
namespace detail
{

typedef utility::basic_args_holder base_args_holder_type;

struct invoke_out
{
	inline YGGR_OP_BOOL_EXPLICIT() operator bool(void) const
	{
		return bret;
	}

	bool bret;
};

// basic_handler_container
class basic_handler_container
{
private:
	typedef basic_handler_container this_type;

public:
	virtual ~basic_handler_container(void)
	{
	}

public:
#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)
	template<typename ...Args> inline
	void invoke(invoke_out& out, Args... args) const
	{
		typedef packet::packet_info< Args... > packet_info_type;
		typedef utility::args_holder_ref_wrap<packet_info_type> params_holder_type;
		typedef utility::args_holder_ref_wrap<void> ret_holder_type;

		assert(!out.bret);
		packet_info_type pak_info(args...);
		params_holder_type params_holder(pak_info);
		ret_holder_type ret_holder;
		out.bret = this->pro_invoke(ret_holder, params_holder);
	}

	template<typename Ret, typename ...Args> inline
	void invoke(Ret& ret, invoke_out& out, Args... args) const
	{
		typedef Ret ret_type;
		typedef packet::packet_info< Args... > packet_info_type;
		typedef utility::args_holder_ref_wrap<packet_info_type> params_holder_type;
		typedef utility::args_holder_ref_wrap<ret_type> ret_holder_type;

		packet_info_type pak_info(args...);
		params_holder_type params_holder(pak_info);
		ret_holder_type ret_holder(ret);
		out.bret = this->pro_invoke(ret_holder, params_holder);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, template< ) \
		YGGR_PP_FOO_TYPES_DEF( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, > ) inline \
	void invoke(invoke_out& out \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ) const { \
		typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > packet_info_type; \
		typedef utility::args_holder_ref_wrap<packet_info_type> params_holder_type; \
		typedef utility::args_holder_ref_wrap<void> ret_holder_type; \
		assert(!out.bret); \
		packet_info_type pak_info \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_PARENTHESES_L ) \
								YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_PARENTHESES_R ) ; \
		params_holder_type params_holder(pak_info); \
		ret_holder_type ret_holder; \
		out.bret = this->pro_invoke(ret_holder, params_holder); } \
	\
	template< typename Ret \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	void invoke(Ret& ret, invoke_out& out \
				YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ) const { \
		typedef Ret ret_type; \
		typedef packet::packet_info< YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T) > packet_info_type; \
		typedef utility::args_holder_ref_wrap<packet_info_type> params_holder_type; \
		typedef utility::args_holder_ref_wrap<ret_type> ret_holder_type; \
		packet_info_type pak_info \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_PARENTHESES_L ) \
								YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_PARENTHESES_R ) ; \
		params_holder_type params_holder(pak_info); \
		ret_holder_type ret_holder(ret); \
		out.bret = this->pro_invoke(ret_holder, params_holder); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

protected:
	virtual bool pro_invoke(const base_args_holder_type& ret,
							const base_args_holder_type& params) const = 0;
};


// ret_type foo(...) version
// handler_container_detail_invoke_helper_call_t
template<typename R, typename Handler, typename Params, std::size_t N>
struct handler_container_detail_invoke_helper_call_t
{
public:
	typedef R result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper_call_t this_type;

public:
	inline bool operator()(const base_args_holder_type& ret,
							const handler_type& handler,
							const base_args_holder_type& params) const
	{
		result_type* pret = utility::args_holder_ref_wrap_dynamic_cast<result_type>(ret);
		params_type* pparams = utility::args_holder_ref_wrap_dynamic_cast<params_type>(params);
		if(!(pret && pparams))
		{
			return false;
		}
		params_type& ref_params = *pparams;
		*pret = caller::to_params_call(handler, ref_params);
		return true;
	}
};

template<typename R, typename Handler, typename Params>
struct handler_container_detail_invoke_helper_call_t<R, Handler, Params, 0>
{
public:
	typedef R result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper_call_t this_type;

public:
	inline bool operator()(const base_args_holder_type& ret,
							const handler_type& handler,
							const base_args_holder_type&) const
	{
		result_type* pret = utility::args_holder_ref_wrap_dynamic_cast<result_type>(ret);
		return pret && (*pret = handler(), true);
	}
};

// handler_container_detail_invoke_helper
template<typename R, typename Handler, typename Params>
struct handler_container_detail_invoke_helper
{
public:
	typedef R result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper this_type;

public:
	template<std::size_t N> inline
	bool call(const base_args_holder_type& ret,
				const handler_type& handler,
				const base_args_holder_type& params) const
	{
		typedef
			handler_container_detail_invoke_helper_call_t
			<
				result_type,
				handler_type,
				params_type,
				N
			> caller_type;
		caller_type caller;
		return caller(ret, handler, params);
	}
};

// void foo(...) version

//handler_container_detail_invoke_helper_void_call_t
template<typename Handler, typename Params, std::size_t N>
struct handler_container_detail_invoke_helper_call_t<void, Handler, Params, N>
{
public:
	typedef void result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper_call_t this_type;

public:
	inline bool operator()(const base_args_holder_type& ret,
							const handler_type& handler,
							const base_args_holder_type& params) const
	{
		assert((utility::args_holder_ref_wrap_check<void>(ret)));
		params_type* pparams = utility::args_holder_ref_wrap_dynamic_cast<params_type>(params);
		if(!pparams)
		{
			return false;
		}
		params_type& ref_params = *pparams;
		caller::to_params_call(handler, ref_params);
		return true;
	}
};

template<typename Handler, typename Params>
struct handler_container_detail_invoke_helper_call_t<void, Handler, Params, 0>
{
public:
	typedef void result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper_call_t this_type;

public:
	inline bool operator()(const base_args_holder_type& ret,
							const handler_type& handler,
							const base_args_holder_type params) const
	{
		assert((utility::args_holder_ref_wrap_check<void>(ret)));
		handler();
		return true;
	}
};

// handler_container_detail_invoke_helper_void
template<typename Handler, typename Params>
struct handler_container_detail_invoke_helper<void, Handler, Params>
{
public:
	typedef void result_type;
	typedef Handler handler_type;
	typedef Params params_type;

private:
	typedef handler_container_detail_invoke_helper this_type;

public:
	template<std::size_t N> inline
	bool call(const base_args_holder_type& ret,
				const handler_type& handler,
				const base_args_holder_type& params) const
	{
		typedef
			handler_container_detail_invoke_helper_call_t
			<
				result_type,
				handler_type,
				params_type,
				N
			> caller_type;
		caller_type caller;
		return caller(ret, handler, params);
	}
};

// handler_container class
template< typename Handler>
class handler_container
	: public basic_handler_container
{
private:
	typedef basic_handler_container base_type;
	typedef handler_container this_type;

private:
	typedef Handler handler_type;
	typedef func::foo_t_info<handler_type> foo_t_info_type;
	typedef typename foo_t_info_type::result_type result_type;

public:
	typedef typename
		boost::mpl::if_
		<
			typename foo_t_info_type::arg_holder_list_size_type,
			typename
				packet::packet_info_caster
				<
					typename foo_t_info_type::arg_holder_list_type
				>::type,
			packet::packet_info<>
		>::type params_type;

	typedef typename params_type::params_type check_data_type;

private:
	YGGR_STATIC_CONSTANT(std::size_t, E_params_length = params_type::E_length);

private:
	typedef
		handler_container_detail_invoke_helper
		<
			result_type,
			handler_type,
			params_type
		> detail_invoke_helper_type;

public:
	handler_container(const handler_type& handler)
		: _handler(handler)
	{
	}

	~handler_container(void)
	{
	}

protected:
	virtual bool pro_invoke(const base_args_holder_type& ret,
							const base_args_holder_type& params) const
	{
		detail_invoke_helper_type helper;
		return helper.template call<E_params_length>(ret, _handler, params);
	}

private:
	handler_type _handler;
};

} // namespace detail
} // namespace handler_center
} // namespace yggr

namespace yggr
{
namespace handler_center
{

template<typename Key, typename Mutex = void>
class handler_center
{
private:
	template<typename OKey, typename OMutex>
	friend class handler_center;

private:
	typedef detail::invoke_out invoke_out_type;

private:
	typedef detail::basic_handler_container basic_handler_container_type;
	typedef shared_ptr<basic_handler_container_type> i_handler_container_type;

private:
	typedef Key key_type;
	typedef Mutex tpl_arg_mutex_type;
	typedef
		safe_container::safe_unordered_map
		<
			key_type,
			i_handler_container_type,
			tpl_arg_mutex_type
		> map_type;

	typedef typename map_type::base_type unsafe_map_type;


private:
	typedef handler_center this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	handler_center(void)
	{
	}

	template<typename OMutex>
	handler_center(BOOST_RV_REF_BEG handler_center<key_type, OMutex> BOOST_RV_REF_END right)
	{
		unsafe_map_type tmp(_map.get_allocator());
		right._map.swap(tmp);
		_map.swap(tmp);
	}

	template<typename OMutex>
	handler_center(handler_center<key_type, OMutex>& right)
	{
		unsafe_map_type tmp(_map.get_allocator());
		right._map.copy_to_base(tmp);
		_map.swap(tmp);
	}

	handler_center(BOOST_RV_REF(this_type) right)
		: _map(boost::move(right._map))
	{
	}

	handler_center(const this_type& right)
		: _map(right._map)
	{
	}

	~handler_center(void)
	{
	}

public:
	template<typename OMutex> inline
	this_type& operator=(BOOST_RV_REF_BEG handler_center<key_type, OMutex> BOOST_RV_REF_END right)
	{
		unsafe_map_type tmp(_map.get_allocator());
		right._map.swap(tmp);
		_map.swap(tmp);

		return *this;
	}

	template<typename OMutex> inline
	this_type& operator=(const handler_center<key_type, OMutex>& right)
	{
		unsafe_map_type tmp(_map.get_allocator());
		right._map.copy_to_base(tmp);
		_map.swap(tmp);

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		if(this == boost::addressof(right_ref))
		{
			return *this;
		}

		copy_or_move_or_swap(_map, boost::move(right_ref._map));
		return *this;
	}

	inline this_type& operator=(const this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return *this;
		}

		_map = right._map;
		return *this;
	}

private:
	template<typename OMutex> inline
	static void prv_s_handler_swap(unsafe_map_type& base, handler_center<key_type, OMutex>& right)
	{
		right.swap(base);
	}

public:
	template<typename OMutex> inline
	void swap(BOOST_RV_REF_BEG handler_center<key_type, OMutex> BOOST_RV_REF_END right)
	{
		typedef handler_center<key_type, OMutex> other_type;

		other_type& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename OMutex>
	void swap(handler_center<key_type, OMutex>& right)
	{
		typedef OMutex other_mutex_type;

		_map.using_handler(
			boost::bind(
				&this_type::prv_s_handler_swap<other_mutex_type>,
				_1, boost::ref(right)));
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	inline void swap(this_type& right)
	{
		if(this == boost::addressof(right))
		{
			return;
		}

		::yggr::swap(_map, right._map);
	}

public:
	inline bool empty(void) const
	{
		return _map.empty();
	}

	inline std::size_t size(void) const
	{
		return _map.size();
	}

	inline void clear(void)
	{
		_map.clear();
	}

public:
	template<typename Obj> inline
	bool register_handler_object(Obj& obj)
	{
		return obj.register_handler(*this);
	}

public:
	template<typename Handler> inline
	bool register_handler(const key_type& key, const Handler& handler)
	{
		typedef Handler handler_type;
		typedef detail::handler_container<handler_type> handler_container_type;

		i_handler_container_type phandler =
			construct_shared<basic_handler_container_type>(
				yggr_nothrow_new handler_container_type(handler) );

		return phandler && _map.insert(key, phandler);
	}

	template<typename Obj> inline
	bool unregister_handler_object(Obj& obj)
	{
		return obj.unregister_handler(*this);
	}

	inline bool unregister_handler(const key_type& key)
	{
		return _map.erase(key);
	}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		call(const KeyArgAnchor& key_anchor, Args... args) const
	{
		i_handler_container_type ptr;
		_map.get_value(key_anchor.get(), ptr);
		invoke_out_type out = { false };
		return ptr && (ptr->invoke(out, args...), out);
	}

	template<typename Ret, typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		call(Ret& ret, const KeyArgAnchor& key_anchor, Args... args) const
	{
		i_handler_container_type ptr;
		_map.get_value(key_anchor.get(), ptr);
		invoke_out_type out = { false };
		return ptr && (ptr->template invoke<Ret>(ret, out, args...), out);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		template< typename KeyArgAnchor \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
			call(const KeyArgAnchor& key_anchor \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_VAR_PARAMS) ) const { \
			i_handler_container_type ptr; \
			_map.get_value(key_anchor.get(), ptr); \
			invoke_out_type out = { false }; \
			return \
				ptr \
				&& (ptr->invoke( \
						out YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)), out); } \
		\
		template<typename Ret, typename KeyArgAnchor \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
			call(Ret& ret, const KeyArgAnchor& key_anchor \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_VAR_PARAMS)) const { \
			i_handler_container_type ptr; \
			_map.get_value(key_anchor.get(), ptr); \
			invoke_out_type out = { false }; \
			return \
				ptr \
				&& (ptr->template invoke<Ret>( \
						ret, out YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)), out); }

#	define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:
	map_type _map;
};

} // namespace handler_center
} // namespace yggr

namespace yggr
{
namespace handler_center
{
namespace swap_support
{

template<typename K, typename ML, typename MR> inline
void swap(handler_center<K, ML>& l, handler_center<K, MR>& r)
{
	l.swap(r);
}

template<typename K, typename M> inline
void swap(handler_center<K, M>& l, handler_center<K, M>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace handler_center
} // namespace yggr

namespace std
{
	using ::yggr::handler_center::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::handler_center::swap_support::swap;
} // namespace boost

#include <boost/preprocessor/cat.hpp>
#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>


#define YGGR_PP_HANDLER_NAME( __foo__ ) \
	YGGR_PP_CAST( __foo__, YGGR_PP_CAST_TO_STRING )

#define YGGR_PP_HANDLER_NAME_T( __type__, __foo__ ) \
	__type__( YGGR_PP_CAST( __foo__, YGGR_PP_CAST_TO_STRING ) )

#define YGGR_PP_MEM_HANDLER_NAME(__class__, __foo__) \
	YGGR_PP_CAST(BOOST_PP_CAT(__class__, BOOST_PP_CAT(YGGR_PP_SYMBOL_AREA(), __foo__)), YGGR_PP_CAST_TO_STRING )


#endif // __YGGR_HANDLER_CENTER_HANDLER_CENTER_HPP__
