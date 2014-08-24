//invoker.hpp

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

#ifndef __YGGR_INVOKER_HPP__
#define __YGGR_INVOKER_HPP__

#include <yggr/ppex/foo_params.hpp>
#include <yggr/move/move.hpp>

namespace yggr
{

template<typename Handler>
class invoker
{
public:
	typedef Handler handler_type;
	typedef typename handler_type::result_type ret_type;

private:
	typedef invoker this_type;

public:
	invoker(const Handler& handler)
		: _handler(handler)
	{
	}

	invoker(const this_type& right)
		: _handler(right._handler)
	{
	}

	~invoker(void)
	{
	}

	
	handler_type& get(void)
	{
		return _handler;
	}

	const handler_type& get(void) const
	{
		return _handler;
	}

	this_type& operator=(const handler_type& right)
	{
		_handler = right;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_handler = right._handler;
		return *this;
	}

	ret_type operator()(void)
	{
		_handler();
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	ret_type operator()( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		return _handler(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ); } \
	\
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	ret_type operator()( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_REF_PARAMS ) ) { \
		return _handler(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME() arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

private:
	handler_type _handler;
};

} //namespace yggr 

#endif // __YGGR_INVOKER_HPP__