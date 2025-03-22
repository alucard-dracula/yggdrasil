//ptr_single.hpp

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

#ifndef __YGGR_PTR_SINGLE_HPP__
#define __YGGR_PTR_SINGLE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/bind/bind.hpp>
#include <yggr/move/move.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

#include <boost/weak_ptr.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
template<typename T,
			typename Mutex = boost::shared_mutex,
			u32 ver = 0>
class ptr_single
	: private nonable::noncreateable
{
public:
	YGGR_STATIC_CONSTANT(u32, E_ver = ver);

public:
	typedef T obj_type;
	typedef obj_type value_type;

private:
	typedef Mutex mutex_type;
	typedef safe_container::safe_wrap<obj_type, mutex_type> obj_wrap_type;

public:
	typedef typename obj_wrap_type::value_ptr_type obj_ptr_type;
	typedef boost::weak_ptr<obj_type> obj_weak_ptr_type;

	typedef yggr::u32 version_type;


private:
	typedef ptr_single this_type;

private:
	inline static obj_wrap_type& _s_p_wrap(void)
	{
		static obj_wrap_type wrap;
		return wrap;
	}

public:

#define _s_p_wrap _s_p_wrap()

	inline static obj_ptr_type get_ins(void)
	{
		return _s_p_wrap.get_shared_ptr();
	}

	inline static obj_weak_ptr_type get_weak_ins(void)
	{
		return obj_weak_ptr_type(this_type::get_ins());
	}

	inline static obj_ptr_type init_ins(const obj_ptr_type& ptr)
	{
		return this_type::create_object(ptr);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args> inline
	static obj_ptr_type init_ins(BOOST_FWD_REF(Args)... args)
	{
		return this_type::create_object(boost::forward<Args>(args)...);
	}

	template<typename D, typename ...Args> inline
	static obj_ptr_type init_ins_d(BOOST_FWD_REF(D) d, BOOST_FWD_REF(Args)... args)
	{
		return this_type::create_object(boost::forward<D>(d), boost::forward<Args>(args)...);
	}

	template<typename D, typename A, typename ...Args> inline
	static obj_ptr_type init_ins_da(BOOST_FWD_REF(D) d, BOOST_FWD_REF(A) a, BOOST_FWD_REF(Args)... args)
	{
		return this_type::create_object(boost::forward<D>(d), boost::forward<A>(a), 
											boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static obj_ptr_type init_ins( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return this_type::create_object(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template< typename D \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		static obj_ptr_type init_ins_d(YGGR_PP_FOO_ARGS_FWD_REF(D) d \
										YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
										YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return \
				this_type::create_object_d( \
					boost::forward<D>(d) YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); } \
		\
		template< typename D, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		static obj_ptr_type init_ins_da( YGGR_PP_FOO_ARGS_FWD_REF(D) d, YGGR_PP_FOO_ARGS_FWD_REF(A) a \
											YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			return \
				this_type::create_object_da( \
					boost::forward<D>(d), boost::forward<A>(a) YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline static void uninstall(void)
	{
		_s_p_wrap.clear();
	}

	YGGR_CONSTEXPR_OR_INLINE static version_type version(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return E_ver;
	}
private:

	inline static obj_ptr_type create_object(const obj_ptr_type& ptr)
	{
		return ptr?
				_s_p_wrap.locked_unsafe_using_handler(
					boost::bind(&this_type::handler_create_object, _1, ptr))
				: _s_p_wrap.get_shared_ptr();
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	template<typename ...Args> inline
	static obj_ptr_type create_object(BOOST_FWD_REF(Args)... args)
	{
		obj_ptr_type tp = construct_shared<obj_type>(yggr_nothrow_new obj_type(boost::forward<Args>(args)...));
		return tp?
				_s_p_wrap.locked_unsafe_using_handler(
					boost::bind(&this_type::handler_create_object, _1, tp))
				: _s_p_wrap.get_shared_ptr();
	}

	template<typename D, typename ...Args> inline
	static obj_ptr_type create_object_d(BOOST_FWD_REF(D) d, BOOST_FWD_REF(Args)... args)
	{
		obj_ptr_type tp = 
			construct_shared<obj_type>(
				yggr_nothrow_new obj_type(boost::forward<Args>(args)...), 
				boost::forward<D>(d));

		return tp?
				_s_p_wrap.locked_unsafe_using_handler(
					boost::bind(&this_type::handler_create_object, _1, tp))
				: _s_p_wrap.get_shared_ptr();
	}

	template<typename D, typename A, typename ...Args> inline
	static obj_ptr_type create_object_da(BOOST_FWD_REF(D) d, BOOST_FWD_REF(A) a, BOOST_FWD_REF(Args)... args)
	{
		obj_ptr_type tp = 
			construct_shared<obj_type>(
				yggr_nothrow_new obj_type(boost::forward<Args>(args)...), 
				boost::forward<D>(d), boost::forward<A>(a));
		
		return tp?
				_s_p_wrap.locked_unsafe_using_handler(
					boost::bind(&this_type::handler_create_object, _1, tp))
				: _s_p_wrap.get_shared_ptr();
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static obj_ptr_type create_object( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			obj_ptr_type tp = \
				construct_shared<obj_type>( \
					yggr_nothrow_new obj_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA ))); \
			return tp? \
					_s_p_wrap.locked_unsafe_using_handler( \
						boost::bind(&this_type::handler_create_object, _1, tp))	\
					: _s_p_wrap.get_shared_ptr(); } \
		\
		template< typename D \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline \
		static obj_ptr_type create_object_d(YGGR_PP_FOO_ARGS_FWD_REF(D) d \
											YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			obj_ptr_type tp = \
				construct_shared<obj_type>( \
					yggr_nothrow_new obj_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )), \
					boost::forward<D>(d)); \
			return tp? \
					_s_p_wrap.locked_unsafe_using_handler( \
						boost::bind(&this_type::handler_create_object, _1, tp))	\
					: _s_p_wrap.get_shared_ptr(); } \
		\
		template< typename D, typename A \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_TYPES_DEF( __n__ ) > inline\
		static obj_ptr_type create_object_da( YGGR_PP_FOO_ARGS_FWD_REF(D) d, YGGR_PP_FOO_ARGS_FWD_REF(A) a \
												YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
												YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) { \
			obj_ptr_type tp = \
				construct_shared<obj_type>( \
					yggr_nothrow_new obj_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )), \
					boost::forward<D>(d), boost::forward<A>(a)); \
			return tp? \
					_s_p_wrap.locked_unsafe_using_handler( \
						boost::bind(&this_type::handler_create_object, _1, tp))	\
					: _s_p_wrap.get_shared_ptr(); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline static obj_ptr_type handler_create_object(typename obj_wrap_type::base_type& base,
														obj_ptr_type ptr)
	{
		return base? (base) : (base = ptr);
	}

#undef _s_p_wrap

};

} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_PTR_SINGLE_HPP__
