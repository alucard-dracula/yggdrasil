//safe_call.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_SAFE_CALL_HPP__
#define __YGGR_SAFE_CONTAINER_SAFE_CALL_HPP__

#include <boost/thread/mutex.hpp>

#include <yggr/helper/mutex_def_helper.hpp>
#include <yggr/ppex/foo_params.hpp>

namespace yggr
{
namespace safe_container
{

template<typename Mutex = boost::mutex>
struct safe_call
{

public:
	typedef Mutex mutex_type;

private:
	typedef helper::mutex_def_helper<mutex_type> mutex_def_helper_type;
	typedef typename mutex_def_helper_type::read_lock_type read_lock_type;
	typedef typename mutex_def_helper_type::write_lock_type write_lock_type;

public:

	safe_call(void)
	{
	}

	~safe_call(void)
	{
	}

	template<typename Handler>
	typename Handler::result_type
		call( const Handler& handler ) const
	{
		return handler();
	}

#define BOOST_PP_LOCAL_MACRO(__n__) \
	template< \
				typename Handler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF(__n__) > \
	typename Handler::result_type \
		call( \
				const Handler& handler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) ) const { \
		read_lock_type lk(_mutex); \
		return handler(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); } \
	\
	template< \
				typename Handler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF(__n__) > \
	typename Handler::result_type \
		call( \
				const Handler& handler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_REF_PARAMS ) ) ) const { \
		read_lock_type lk(_mutex); \
		return handler(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); } \
	\
	template< \
				typename Handler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_TYPES_DEF(__n__) > \
	typename Handler::result_type \
		call( \
				const Handler& handler \
				YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
				YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_VAR_PARAMS ) ) ) const { \
		read_lock_type lk(_mutex); \
		return handler(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); }

#define YGGR_PP_FOO_ARG_NAME(  ) arg_name
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

private:
	mutable mutex_type _mutex;
};

} // namespace safe_container
} // namespace yggr

#endif  //__YGGR_SAFE_CONTAINER_SAFE_CALL_HPP__