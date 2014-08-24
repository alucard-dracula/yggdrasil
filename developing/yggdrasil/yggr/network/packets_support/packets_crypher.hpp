//packets_crypher.hpp

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

#ifndef __YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_CRYPHER_HPP__
#define __YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_CRYPHER_HPP__

#include <boost/bind.hpp>
#include <yggr/ppex/foo_params.hpp>

namespace yggr
{
namespace network
{
namespace packets_support
{

template<typename Crypher>
class packets_crypher
{
public:
	typedef Crypher crypher_type;

private:
	typedef packets_crypher this_type;

public:
	packets_crypher(void)
	{
		_crypher.init();
	}

#define BOOST_PP_LOCAL_MACRO(__n__) \
	template<YGGR_PP_FOO_TYPES_DEF( __n__ )> \
	packets_crypher(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS )) { \
		_crypher.init(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); }

#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

	~packets_crypher(void)
	{
	}

	template<typename Pak>
	bool encrypt(Pak& pak) const
	{
		typedef Pak pak_type;
		typedef typename pak_type::buf_type buf_type;

		return _crypher.encrypt(pak);
	}

	template<typename Pak>
	bool decrypt(Pak& pak) const
	{
		typedef Pak pak_type;
		typedef typename pak_type::buf_type buf_type;

		return _crypher.decrypt(pak);
	}

private:
	crypher_type _crypher;
};

template<>
class packets_crypher<void>
{
	
public:
	typedef void crypher_type;
private:
	typedef packets_crypher this_type;

public:
	packets_crypher(void)
	{
	}
	
#define BOOST_PP_LOCAL_MACRO(__n__) \
	template<YGGR_PP_FOO_TYPES_DEF( __n__ )> \
	packets_crypher(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS )) {}

#define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

	~packets_crypher(void)
	{
	}

	template<typename Pak>
	inline bool encrypt(Pak& pak) const
	{
		return true;
	}

	template<typename Pak>
	inline bool decrypt(Pak& pak) const
	{
		return true;
	}
};

} // namespace packets_support
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_CRYPHER_HPP__