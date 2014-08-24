//ptr_single.hpp

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

#ifndef __YGGR_PTR_SINGLE_HPP__
#define __YGGR_PTR_SINGLE_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <yggr/nonable/noncreateable.hpp>
#include <yggr/ppex/foo_params.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
template<typename T>
class ptr_single 
	: private nonable::noncreateable
{
public:
	typedef T obj_type;
	typedef boost::shared_ptr<obj_type> obj_ptr_type;
	typedef boost::weak_ptr<obj_type> obj_weak_ptr_type;

private:
	typedef ptr_single this_type;

public:
	static obj_ptr_type get_ins(void)
	{
		return this_type::_s_p;
	}

	static const obj_ptr_type get_c_ins(void)
	{
		return obj_weak_ptr_type(this_type::_s_p);
	}

	static obj_weak_ptr_type get_weak_ins(void)
	{
		return obj_weak_ptr_type(this_type::_s_p);
	}

	static const obj_weak_ptr_type get_c_weak_ins(void)
	{
		return this_type::_s_p;
	}

	static obj_ptr_type init_ins(void)
	{
		this_type::create_object();
		return this_type::_s_p;
	}

	static obj_ptr_type init_ins(const obj_ptr_type& ptr)
	{
		this_type::create_object(ptr);
		return this_type::_s_p;
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	static obj_ptr_type init_ins( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		this_type::create_object(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); \
		return this_type::_s_p; }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

	static void uninstall(void)
	{
		obj_ptr_type().swap(this_type::_s_p);
	}

private:

	static void create_object(void)
	{
		if(!this_type::_s_p)
		{
			obj_ptr_type tp(new obj_type());
			this_type::_s_p.swap(tp);
		}
	}

	static void create_object(const obj_ptr_type& ptr)
	{
		if(!this_type::_s_p)
		{
			this_type::_s_p = ptr;
		}
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	static void create_object( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		if(!this_type::_s_p){ \
		obj_ptr_type tp(new obj_type(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ))); \
		this_type::_s_p.swap(tp); } }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

private:
     static obj_ptr_type _s_p;
};

template<typename T>
typename ptr_single<T>::obj_ptr_type ptr_single<T>::_s_p;

} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_PTR_SINGLE_HPP__
