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

#include <yggr/handler_center/handler_container.hpp>

#include <yggr/base/static_constant.hpp>
#include <yggr/base/error_make.hpp>

#include <yggr/bind/bind.hpp>

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/utility/args_anchor.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>

#include <boost/ref.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/thread/mutex.hpp>

#include <boost/mpl/if.hpp>

#include <cassert>

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
	typedef detail::invoke_result_t invoke_result_type;

private:
	typedef basic_handler_container basic_handler_container_type;
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
		typedef handler_container<handler_type> handler_container_type;

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
		invoke_result_type out = { false };
		return ptr && (ptr->invoke(out, args...), out);
	}

	template<typename Ret, typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		call(Ret& ret, const KeyArgAnchor& key_anchor, Args... args) const
	{
		i_handler_container_type ptr;
		_map.get_value(key_anchor.get(), ptr);
		invoke_result_type out = { false };
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
			invoke_result_type out = { false }; \
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
			invoke_result_type out = { false }; \
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

#include <yggr/ppex/cast.hpp>
#include <yggr/ppex/cast_to_string.hpp>
#include <boost/preprocessor/cat.hpp>

// handler_name
#define YGGR_PP_HANDLER_NAME( __foo__ ) \
	YGGR_PP_CAST( __foo__, YGGR_PP_CAST_TO_STRING )

#define YGGR_PP_HANDLER_NAME_T( __type__, __foo__ ) \
	__type__( YGGR_PP_HANDLER_NAME( __foo__ ) )

// mem_handler_name
#define YGGR_PP_MEM_HANDLER_NAME(__class__, __foo__) \
	YGGR_PP_CAST(BOOST_PP_CAT(__class__, BOOST_PP_CAT(YGGR_PP_SYMBOL_AREA(), __foo__)), YGGR_PP_CAST_TO_STRING )

#define YGGR_PP_MEM_HANDLER_NAME_T( __type__, __class__, __foo__ ) \
	__type__( YGGR_PP_MEM_HANDLER_NAME( __class__, __foo__ ) )



#endif // __YGGR_HANDLER_CENTER_HANDLER_CENTER_HPP__
