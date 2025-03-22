//script_mgr.hpp

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

#ifndef __YGGR_SCRIPT_SCRIPT_MGR_HPP__
#define __YGGR_SCRIPT_SCRIPT_MGR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/move/move.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>
#include <yggr/ppex/symbol_if.hpp>

#include <yggr/utility/args_anchor.hpp>

#include <yggr/nonable/noncopyable.hpp>
#include <yggr/nonable/nonmoveable.hpp>

#include <yggr/func/foo_t_maker.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/exception/exception.hpp>

#include <yggr/safe_container/safe_unordered_map.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>

#include <cassert>

namespace yggr
{
namespace script
{

template<typename Rule_ID, typename Script, typename Environment>
class script_mgr
	: public Environment,
		private nonable::noncopyable,
		private nonable::nonmoveable
{
public:
	typedef Rule_ID rule_id_type;
	typedef Script script_type;

public:
	typedef typename script_type::enter_foo_name_type enter_foo_name_type;
	typedef safe_container::safe_wrap<script_type> safe_script_type;
	typedef ::yggr::shared_ptr<safe_script_type> safe_script_ptr_type;
	typedef ::yggr::error error_type;

public:
	typedef typename script_type::string_code_type string_code_type;
	typedef typename script_type::file_code_type file_code_type;

private:
	typedef Environment base_type;
	typedef typename safe_script_type::element_ptr_type script_ptr_type;
	typedef safe_container::safe_unordered_map<rule_id_type, safe_script_ptr_type> script_map_type;

private:
	typedef script_mgr this_type;

public:
	script_mgr(void)
	{
	}

	~script_mgr(void)
	{
	}

public:

	using base_type::append_module;

	template<typename Code> inline
	bool append_script_module(const rule_id_type& rule_id, const Code& code)
	{
		typedef Code code_type;

		safe_script_ptr_type ptr;
		try
		{
			return
				_script_map.get_value(rule_id, ptr)
				&& ptr
				&& (ptr->call_function(
						static_cast<void (script_type::*)(const code_type&)>(&script_type::do_script), code),
					true);
		}
		catch(const error_type& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
	}

public:
	// insert

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Code, typename ...Args> inline
	bool insert(const rule_id_type& rule_id, const enter_foo_name_type& enter_foo,
					const Code& code, BOOST_FWD_REF(Args)... args)
	{
		script_type* pscript
			= yggr_nothrow_new script_type(enter_foo, code, base_type::get_global_environment(),
											boost::forward<Args>(args)... );
		if(!(pscript && pscript->succeed()))
		{
			if(pscript)
			{
				delete pscript;
			}
			return false;
		}

		safe_script_ptr_type ptr = construct_shared<safe_script_type>( yggr_nothrow_new safe_script_type( pscript ) );
		try
		{
			return
				(ptr)
					&& (!ptr->empty())
					&& (ptr->call_function(&script_type::succeed))
					&& (_script_map.insert(rule_id, ptr));
		}
		catch(const error_type& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename Code \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	bool insert( const rule_id_type& rule_id, const enter_foo_name_type& enter_foo, const Code& code \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
		script_type* pscript \
			= yggr_nothrow_new script_type(enter_foo, code, base_type::get_global_environment() \
											YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
		if(!(pscript && pscript->succeed())) { if(pscript) { delete pscript; } return false; }; \
		safe_script_ptr_type ptr = construct_shared<safe_script_type>( yggr_nothrow_new safe_script_type( pscript ) ); \
		try { \
			return (ptr) && (!ptr->empty()) \
					&& (ptr->call_function(&script_type::succeed)) \
					&& (_script_map.insert(rule_id, ptr)); } \
		catch(const error_type& e) { \
			exception::exception::throw_error(e); \
			return false; } }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename Code, typename ...Args> inline
	static safe_script_ptr_type make_script_object(BOOST_FWD_REF(Args)... args)
	{
		script_type* pscript
			= yggr_nothrow_new script_type(boost::forward<Args>(args)... );
		if(!(pscript && pscript->succeed()))
		{
			if(pscript)
			{
				delete pscript;
			}
			return false;
		}

		return construct_shared<safe_script_type>( yggr_nothrow_new safe_script_type( pscript ) );
	}

#else
#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static safe_script_ptr_type make_script_object( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			script_type* pscript \
				= yggr_nothrow_new script_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
			if(!(pscript && pscript->succeed())) { if(pscript) { delete pscript; } return false; }; \
			return construct_shared<safe_script_type>( yggr_nothrow_new safe_script_type( pscript ) ); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME
#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline bool insert( const rule_id_type& rule_id, safe_script_ptr_type ptr)
	{
		try
		{
			return
				ptr
				&& (!ptr->empty())
				&& ptr->call_function(&script_type::succeed)
				&& _script_map.insert(rule_id, ptr);
		}
		catch(const error_type& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
	}

public:
	// execute_rule

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		execute_rule(const KeyArgAnchor& rule_id_anchor, BOOST_FWD_REF(Args)... args) const
	{
		return this_type::prv_execute_rule(rule_id_anchor, boost::forward<Args>(args)...);
	}

	template< typename R, typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		execute_rule( R& out, const KeyArgAnchor& rule_id_anchor, BOOST_FWD_REF(Args)... args) const
	{
		return this_type::template prv_execute_rule<R>(out, rule_id_anchor, boost::forward<Args>(args)...);
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		execute_rule( const KeyArgAnchor& rule_id_anchor \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return \
			this_type::prv_execute_rule( \
				rule_id_anchor YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template< typename R, typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		execute_rule( R& out, const KeyArgAnchor& rule_id_anchor \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return \
			this_type::template prv_execute_rule<R>( \
				out, \
				rule_id_anchor YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME


// !!!!! test example (bug code) don't deleted !!!!
//template< typename T0 > inline
//bool execute_rule( const rule_id_type& rule_id , T0&& arg0 ) const
//{
//
//#if 1
//	typedef typename
//		func::member_foo_t_maker
//		<
//			void,
//			script_type,
//			static_cast<u32>(func::cv_mark_t::E_cv_const) ,
//			T0&&
//		>::type foo_type;
//
//	auto tmp0 = &script_type::template execute_rule < R, T0 >;
//	//foo_type pf0 = &script_type::template execute_rule < R, T0 >;
//
//#else
//	typedef typename
//		func::member_foo_t_maker
//		<
//			void,
//			script_type,
//			static_cast<u32>(func::cv_mark_t::E_cv_const) ,
//			T0&
//		>::type foo_type;
//
//	auto tmp0 = &script_type::template execute_rule< R, T0& >;
//	foo_type pf0 = &script_type::template execute_rule< R, T0& >;
//#endif // 0, 1
//
//	assert(false);
//	return false;
//}
//
//
//template< typename R , typename T0 > inline
//bool execute_rule( R& out, const rule_id_type& rule_id , T0&& arg0 ) const
//{
//
//#if 1
//	// failed
//	typedef typename
//		func::member_foo_t_maker
//		<
//			R,
//			script_type,
//			static_cast<u32>(func::cv_mark_t::E_cv_const) ,
//			T0&&
//		>::type foo_type;
//
//	auto auto0 = &script_type::template execute_rule< R , T0 >;
//	//foo_type pf0 = &script_type::template execute_rule< R , T0 >;
//
//#else
//	// succeed
//	typedef typename
//		func::member_foo_t_maker
//		<
//			R,
//			script_type,
//			static_cast<u32>(func::cv_mark_t::E_cv_const) ,
//			T0&
//		>::type foo_type;
//
//	auto auto0 = &script_type::template execute_rule< R , T0& >;
//	foo_type pf0 = &script_type::template execute_rule< R , T0& >;
//
//#endif // 0, 1
//
//	assert(false);
//	return false;
//}


#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

public:
	// call_function

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		call_function(const KeyArgAnchor& rule_id_anchor,
						const enter_foo_name_type& enter_foo_name,
						BOOST_FWD_REF(Args)... args) const
	{
		return this_type::prv_call_function(rule_id_anchor, enter_foo_name, boost::forward<Args>(args)...);
	}

	template< typename R, typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		call_function( R& out,
						const KeyArgAnchor& rule_id_anchor,
						const enter_foo_name_type& enter_foo_name,
						BOOST_FWD_REF(Args)... args) const
	{
		return this_type::template prv_call_function<R>(out, rule_id_anchor, enter_foo_name, boost::forward<Args>(args)...);
	}

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		call_function( const KeyArgAnchor& rule_id_anchor, \
						const enter_foo_name_type& enter_foo_name \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return \
			this_type::prv_call_function( \
				rule_id_anchor, \
				enter_foo_name YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template< typename R, typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		call_function( R& out, \
						const KeyArgAnchor& rule_id_anchor, \
						const enter_foo_name_type& enter_foo_name \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		return \
			this_type::template prv_call_function<R>( \
				out, \
				rule_id_anchor, \
				enter_foo_name YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

public:
	inline safe_script_ptr_type get_script(const rule_id_type& rule_id) const
	{
		safe_script_ptr_type ptr;
		_script_map.get_value(rule_id, ptr);
		return ptr;
	}

public:
	inline bool remove(const rule_id_type& rule_name)
	{
		return _script_map.erase(rule_name);
	}

	inline void clear(void)
	{
		_script_map.clear();
	}

public:
	inline std::size_t size(void) const
	{
		return _script_map.size();
	}

	inline bool empty(void) const
	{
		return _script_map.empty();
	}

private:

// execute_rule
#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		prv_execute_rule(const KeyArgAnchor& rule_id_anchor, BOOST_FWD_REF(Args)... args) const
	{
		typedef typename
			func::member_foo_t_maker<
				void,
				script_type,
				static_cast<u32>(func::cv_mark_t::E_cv_const),
				BOOST_FWD_REF(Args)...
			>::type foo_type;

		safe_script_ptr_type ptr;
		_script_map.get_value(rule_id_anchor.get(), ptr);

		if(!ptr)
		{
			return false;
		}

		try
		{
			ptr->call_function(
				static_cast<foo_type>(
					&script_type::template execute_rule<void, Args...>),
				boost::forward<Args>(args)... );
			return true;
		}
		catch(const error_type& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
	}

	template<typename R, typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		prv_execute_rule(R& out, const KeyArgAnchor& rule_id_anchor, BOOST_FWD_REF(Args)... args) const
	{
		typedef typename
			func::member_foo_t_maker<
				R,
				script_type,
				static_cast<u32>(func::cv_mark_t::E_cv_const),
				BOOST_FWD_REF(Args)...
			>::type foo_type;

		safe_script_ptr_type ptr;
		_script_map.get_value(rule_id_anchor.get(), ptr);

		if(!ptr)
		{
			return false;
		}

		try
		{
			out = ptr->call_function(
					static_cast<foo_type>(
						&script_type::template execute_rule< R, Args...>),
					boost::forward<Args>(args)...);
			return true;
		}
		catch(const error_type& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
	}

#else

	// i don't know why the error happend of using rvalue
	// error C2440: 'static_cast' : cannot convert from 'overloaded-function' to 'foo_type'
	// vs2010 is not support foo pointer e.g. foo(rvref, rvref, rvref ...)
	// so using "const type&"(YGGR_PP_FOO_CREF_PARAMS_TYPES) fix it

#if defined(_MSC_VER)

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		prv_execute_rule(const KeyArgAnchor& rule_id_anchor \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		typedef typename \
			func::member_foo_t_maker< \
				void, \
				script_type, \
				static_cast<u32>(func::cv_mark_t::E_cv_const) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS_TYPES ) \
			>::type foo_type; \
		safe_script_ptr_type ptr; \
		_script_map.get_value(rule_id_anchor.get(), ptr); \
		if(!ptr) { return false; } \
		try { ptr->call_function( \
				static_cast<foo_type>( \
					&script_type::template execute_rule< void \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS_TYPES ) > ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return true; } \
		catch(const typename script_type::error_type& e) { \
			exception::exception::throw_error(e); \
			return false; } } \
	\
	template< typename R, typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		prv_execute_rule( R& out, const KeyArgAnchor& rule_id_anchor \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		typedef typename \
			func::member_foo_t_maker< \
				R, \
				script_type, \
				static_cast<u32>(func::cv_mark_t::E_cv_const) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS_TYPES ) \
			>::type foo_type; \
		safe_script_ptr_type ptr; \
		_script_map.get_value(rule_id_anchor.get(), ptr); \
		if(!ptr) { return false; } \
		try { out = ptr->call_function( \
						static_cast<foo_type>( \
							&script_type::template execute_rule< R \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS_TYPES ) >) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return true; } \
		catch(const typename script_type::error_type& e) { \
			exception::exception::throw_error(e); \
			return false; } } 

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		prv_execute_rule(const KeyArgAnchor& rule_id_anchor \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		typedef typename \
			func::member_foo_t_maker< \
				void, \
				script_type, \
				static_cast<u32>(func::cv_mark_t::E_cv_const) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS_TYPES ) \
			>::type foo_type; \
		safe_script_ptr_type ptr; \
		_script_map.get_value(rule_id_anchor.get(), ptr); \
		if(!ptr) { return false; } \
		try { ptr->call_function( \
				static_cast<foo_type>( \
					&script_type::template execute_rule< void \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_GROUP( __n__ ) > ) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return true; } \
		catch(const error_type& e) { \
			exception::exception::throw_error(e); \
			return false; } \
		catch(const compatibility::stl_exception& e) { \
			exception::exception::throw_error(e); \
			return false; } } \
	\
	template< typename R, typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		prv_execute_rule( R& out, const KeyArgAnchor& rule_id_anchor \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		typedef typename \
			func::member_foo_t_maker< \
				R, \
				script_type, \
				static_cast<u32>(func::cv_mark_t::E_cv_const) \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS_TYPES ) \
			>::type foo_type; \
		safe_script_ptr_type ptr; \
		_script_map.get_value(rule_id_anchor.get(), ptr); \
		if(!ptr) { return false; } \
		try { out = ptr->call_function( \
						static_cast<foo_type>( \
							&script_type::template execute_rule< R \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_TYPES_GROUP( __n__ ) >) \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return true; } \
		catch(const error_type& e) { \
			exception::exception::throw_error(e); \
			return false; } \
		catch(const compatibility::stl_exception& e) { \
			exception::exception::throw_error(e); \
			return false; } }

#endif // _MSC_VER

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:

// call_function

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		prv_call_function(const KeyArgAnchor& rule_id_anchor,
							const enter_foo_name_type& enter_foo_name,
							BOOST_FWD_REF(Args)... args) const
	{
		typedef typename
			func::member_foo_t_maker<
				void,
				script_type,
				static_cast<u32>(func::cv_mark_t::E_cv_const),
				const enter_foo_name_type&,
				BOOST_FWD_REF(Args)...
			>::type foo_type;

		safe_script_ptr_type ptr;
		_script_map.get_value(rule_id_anchor.get(), ptr);

		if(!ptr)
		{
			return false;
		}

		try
		{
			ptr->call_function(
				static_cast<foo_type>(
					&script_type::template call_function<void, Args...>),
					enter_foo_name, boost::forward<Args>(args)... );
			return true;
		}
		catch(const error_type& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
	}

	template<typename R, typename KeyArgAnchor, typename ...Args> inline
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type
		prv_call_function(R& out,
							const KeyArgAnchor& rule_id_anchor,
							const enter_foo_name_type& enter_foo_name,
							BOOST_FWD_REF(Args)... args) const
	{
		typedef typename
			func::member_foo_t_maker<
				R,
				script_type,
				static_cast<u32>(func::cv_mark_t::E_cv_const),
				const enter_foo_name_type&,
				BOOST_FWD_REF(Args)...
			>::type foo_type;

		safe_script_ptr_type ptr;
		_script_map.get_value(rule_id_anchor.get(), ptr);

		if(!ptr)
		{
			return false;
		}

		try
		{
			out = ptr->call_function(
					static_cast<foo_type>(
						&script_type::template call_function< R, Args...>),
					enter_foo_name, boost::forward<Args>(args)...);
			return true;
		}
		catch(const error_type& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
	}

#else

	// i don't know why the error happend of using rvalue
	// error C2440: 'static_cast' : cannot convert from 'overloaded-function' to 'foo_type'
	// vs2010 is not support foo pointer e.g. foo(rvref, rvref, rvref ...)
	// so using "const type&"(YGGR_PP_FOO_CREF_PARAMS_TYPES) fix it

#if defined(_MSC_VER)

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		prv_call_function( const KeyArgAnchor& rule_id_anchor, \
							const enter_foo_name_type& enter_foo_name \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		typedef typename \
			func::member_foo_t_maker< \
				void, \
				script_type, \
				static_cast<u32>(func::cv_mark_t::E_cv_const), \
				const enter_foo_name_type& \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS_TYPES ) \
			>::type foo_type; \
		safe_script_ptr_type ptr; \
		_script_map.get_value(rule_id_anchor.get(), ptr); \
		if(!ptr) { return false; } \
		try { ptr->call_function( \
				static_cast<foo_type>( \
					&script_type::template call_function< void \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS_TYPES ) > ), \
					enter_foo_name \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return true; } \
		catch(const typename script_type::error_type& e) { \
			exception::exception::throw_error(e); \
			return false; } } \
	\
	template< typename R, typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		prv_call_function( R& out, \
							const KeyArgAnchor& rule_id_anchor, \
							const enter_foo_name_type& enter_foo_name \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		typedef typename \
			func::member_foo_t_maker< \
				R, \
				script_type, \
				static_cast<u32>(func::cv_mark_t::E_cv_const), \
				const enter_foo_name_type& \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS_TYPES ) \
			>::type foo_type; \
		safe_script_ptr_type ptr; \
		_script_map.get_value(rule_id_anchor.get(), ptr); \
		if(!ptr) { return false; } \
		try { out = ptr->call_function( \
						static_cast<foo_type>( \
							&script_type::template call_function< R \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS_TYPES ) > ), \
						enter_foo_name \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return true; } \
		catch(const typename script_type::error_type& e) { \
			exception::exception::throw_error(e); \
			return false; } } 

#else

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		prv_call_function( const KeyArgAnchor& rule_id_anchor, \
							const enter_foo_name_type& enter_foo_name \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		typedef typename \
			func::member_foo_t_maker< \
				void, \
				script_type, \
				static_cast<u32>(func::cv_mark_t::E_cv_const), \
				const enter_foo_name_type& \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS_TYPES ) \
			>::type foo_type; \
		safe_script_ptr_type ptr; \
		_script_map.get_value(rule_id_anchor.get(), ptr); \
		if(!ptr) { return false; } \
		try { ptr->call_function( \
				static_cast<foo_type>( \
					&script_type::template call_function< void \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_TYPES_GROUP( __n__ ) > ), \
					enter_foo_name \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return true; } \
		catch(const error_type& e) { \
			exception::exception::throw_error(e); \
			return false; } \
		catch(const compatibility::stl_exception& e) { \
			exception::exception::throw_error(e); \
			return false; } } \
	\
	template< typename R, typename KeyArgAnchor \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
	typename boost::enable_if<is_args_anchor_t<KeyArgAnchor>, bool>::type \
		prv_call_function( R& out, \
							const KeyArgAnchor& rule_id_anchor, \
							const enter_foo_name_type& enter_foo_name \
							YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
							YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) const { \
		typedef typename \
			func::member_foo_t_maker< \
				R, \
				script_type, \
				static_cast<u32>(func::cv_mark_t::E_cv_const), \
				const enter_foo_name_type& \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS_TYPES ) \
			>::type foo_type; \
		safe_script_ptr_type ptr; \
		_script_map.get_value(rule_id_anchor.get(), ptr); \
		if(!ptr) { return false; } \
		try { out = ptr->call_function( \
						static_cast<foo_type>( \
							&script_type::template call_function< R \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_TYPES_GROUP( __n__ ) > ), \
						enter_foo_name \
						YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ) ); \
				return true; } \
		catch(const error_type& e) { \
			exception::exception::throw_error(e); \
			return false; } \
		catch(const compatibility::stl_exception& e) { \
			exception::exception::throw_error(e); \
			return false; } }

#endif // _MSC_VER

#define YGGR_PP_FOO_ARG_NAME(  ) arg
#define BOOST_PP_LOCAL_LIMITS ( 0, BOOST_PP_SUB(YGGR_PP_FOO_DEFAULT_PARAMS_LEN, 1) )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

private:
	script_map_type _script_map;
};

} // namespace script
} // namespace yggr



#endif //__YGGR_SCRIPT_SCRIPT_MGR_HPP__

