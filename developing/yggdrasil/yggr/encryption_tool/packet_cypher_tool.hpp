//packet_cypher_tool.hpp

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

#ifndef __YGGR_ENCRYPTION_TOOL_PACKET_CRYPHER_TOOL_HPP__
#define __YGGR_ENCRYPTION_TOOL_PACKET_CRYPHER_TOOL_HPP__

#include <boost/bind.hpp>
#include <boost/mpl/assert.hpp>

//#include <yggr/packet/packet.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/encryption_tool/tool_type_check.hpp>

namespace yggr
{
namespace encryption_tool
{

template<typename Tool>
class packet_cypher_tool
	: public Tool
{
public:
	typedef Tool base_type;

	BOOST_MPL_ASSERT((is_reduction_tool<base_type>));

private:
	typedef packet_cypher_tool this_type;

public:
	packet_cypher_tool(void)
	{
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	packet_cypher_tool(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		base_type::init(YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

	~packet_cypher_tool(void)
	{
	}

	void init(void)
	{
		base_type::init();
	}

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template<YGGR_PP_FOO_TYPES_DEF( __n__ ) > \
	void init(YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_CREF_PARAMS ) ) { \
		base_type::init( YGGR_PP_FOO_PARAMS_OP( __n__, YGGR_PP_SYMBOL_COMMA ) ); }

#	define YGGR_PP_FOO_ARG_NAME(  ) init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

	template<typename Packet>
	bool encrypt(Packet& pak) const
	{
		typedef Packet pak_type;
		typedef typename pak_type::buf_type pak_buf_type;
		return pak.cypher(boost::bind(&base_type::template encrypt<pak_buf_type>, this, _1));
	}

	template<typename Packet>
	bool decrypt(Packet& pak) const
	{
		typedef Packet pak_type;
		typedef typename pak_type::buf_type pak_buf_type;

		return pak.cypher(boost::bind(&base_type::template decrypt<pak_buf_type>, this, _1));
	}

	//template<typename Archive_Partner>
	//bool encrypt(yggr::packet::packet<Archive_Partner>& pak) const
	//{
	//	typedef yggr::packet::packet<Archive_Partner> pak_type;
	//	typedef typename pak_type::buf_type pak_buf_type;

	//	return pak.cypher(boost::bind(&base_type::encrypt<pak_buf_type>, this, _1));
	//}

	//template<typename Archive_Partner>
	//bool decrypt(yggr::packet::packet<Archive_Partner>& pak) const
	//{
	//	typedef packet::packet<Archive_Partner> pak_type;
	//	typedef typename pak_type::buf_type pak_buf_type;

	//	return pak.cypher(boost::bind(&base_type::decrypt<pak_buf_type>, this, _1));
	//}
};

} // namespace encryption_tool
} // namespace yggr

#endif // __YGGR_ENCRYPTION_TOOL_PACKET_CRYPHER_TOOL_HPP__
