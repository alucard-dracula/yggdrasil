//log_params_maker.hpp

/****************************************************************************
Copyright (c) 2010-2022 yggdrasil

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

#ifndef __YGGR_LOG_LOG_DATA_CREATOR_HPP__
#define __YGGR_LOG_LOG_DATA_CREATOR_HPP__

#include <boost/any.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/ppex/foo_params.hpp>

#include <yggr/safe_container/safe_wrap.hpp>

namespace yggr
{
namespace log
{

class basic_log_params_maker
{
public:
	typedef boost::any data_type;
	typedef boost::any params_type; 

private:
	typedef basic_log_params_maker this_type;

public:

	virtual ~basic_log_params_maker(void) {}

	virtual u32 op_id(void) const = 0;
	virtual params_type make_params(const data_type&) = 0;
};

template<typename Real_Maker>
class log_params_maker
	: public basic_log_params_maker
{
public:
	typedef basic_log_params_maker base_type;

	typedef base_type::data_type data_type;
	typedef base_type::data_type params_type;

	typedef Real_Maker real_maker_type;
	typedef safe_container::safe_wrap<real_maker_type, boost::shared_mutex> real_maker_wrap_type;

private:
	typedef log_params_maker this_type;

public:
	log_params_maker(void)
		: _real_maker_wrap((real_maker_type()))
	{
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	log_params_maker( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) \
		: _real_maker_wrap((real_maker_type(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )))){ }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

	log_params_maker(BOOST_RV_REF(real_maker_type) rmaker)
		: _real_maker_wrap(rmaker)
	{
	}

	log_params_maker(const real_maker_type& rmaker)
		: _real_maker_wrap(rmaker)
	{
	}

	virtual ~log_params_maker(void)
	{
	}

	virtual u32 op_id(void) const
	{
		return real_maker_type::s_id();
	}

	virtual params_type make_params(const data_type& data)
	{
		params_type params;
		try
		{
			params 
				= _real_maker_wrap.using_handler(
						boost::bind(&real_maker_type::make_params_const, _1, boost::cref(data)));
		}
		catch(const real_maker_wrap_type::error_type&)
		{
			return params_type();
		}

		return params;
	}

	real_maker_wrap_type _real_maker_wrap;
};

} // namespace log
} // namespace yggr

#endif // __YGGR_LOG_LOG_DATA_CREATOR_HPP__