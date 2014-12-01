// params_io_wrap.hpp

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

#ifndef __YGGR_ANY_VAL_PARAMS_IO_WRAP_HPP__
#define __YGGR_ANY_VAL_PARAMS_IO_WRAP_HPP__

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>

#include <yggr/base/interface_ptr.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/struct_conv/conv_params_and_packet_info.hpp>

namespace yggr
{
namespace any_val
{

template<typename Params>
class basic_params_io_wrap
{
public:
	typedef Params params_type;
private:
	typedef basic_params_io_wrap this_type;

public:
	virtual ~basic_params_io_wrap(void) {}
	virtual params_type gen_params(void) const = 0;

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const u32 version) {}
};

template<typename Params, typename Packet_Info>
class params_io_wrap
	: public basic_params_io_wrap<Params>
{
public:
	typedef Params params_type;
	typedef basic_params_io_wrap<Params> base_type;
	typedef Packet_Info packet_info_type;
	typedef typename params_type::key_type key_type;

private:
	typedef yggr::struct_conv::conv_params_and_packet_info conv_type;
	typedef params_io_wrap this_type;

public:
	params_io_wrap(void)
	{
	}

	virtual ~params_io_wrap(void)
	{
	}

	virtual params_type gen_params(void) const
	{
		return conv_type::s_conv<params_type>(_pak_info);
	}

	bool init(const params_type& params)
	{
		return params.size() == packet_info_type::s_element_size();
	};

#define BOOST_PP_LOCAL_MACRO( __n__ ) \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, typename T ) > \
	bool init( YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, const key_type& key ), const params_type& params) { \
		if(params.size() != packet_info_type::s_element_size()) { return false; } \
		try { \
			_pak_info \
				= conv_type::s_conv< YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, T ), std::pair >( \
						YGGR_PP_TEMPLATE_PARAMS_TYPES( __n__, key ), params); } \
		catch(typename params_type::error_type&) { return false; } \
		return true; }

#define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE( )

private:
	friend class boost::serialization::access;
	template<typename Archive>
	void serialize(Archive& ar, const u32 version) 
	{
		ar & YGGR_SERIALIZE_NAME_NVP("base", boost::serialization::base_object< base_type >(*this));
		ar & YGGR_SERIALIZE_NVP( _pak_info );
	}

private:
	packet_info_type _pak_info;
};


} // namespace any_val
} // namespace yggr

#endif // __YGGR_ANY_VAL_PARAMS_IO_WRAP_HPP__