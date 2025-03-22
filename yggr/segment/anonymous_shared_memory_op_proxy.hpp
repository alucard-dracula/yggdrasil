//anonymous_shared_memory_op_proxy.hpp

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

#ifndef __YGGR_SEGMENT_ANONYMOUS_SHARED_MEMORY_OP_PROXY_HPP__
#define __YGGR_SEGMENT_ANONYMOUS_SHARED_MEMORY_OP_PROXY_HPP__

#ifndef YGGR_SEGMENT_OPERATOR_INCLUDE
#	error "include error managed_mapped_file_op_proxy.hpp please include segment_op_proxy.hpp."
#endif // YGGR_SEGMENT_OPERATOR_INCLUDE

#include <yggr/segment/segment_config.hpp> // it must be written in the first sentence include

#include <yggr/base/static_constant.hpp>
#include <yggr/bind/bind.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <yggr/segment/segment_error.hpp>
#include <yggr/segment/anonymous_shared_memory.hpp>

#include <yggr/exception/exception.hpp>

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/mpl/bool.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace segment
{

template<>
class segment_op_proxy<anonymous_shared_memory>
	: public boost::mpl::bool_<false>,
		private nonable::noncreateable
{
public:
	typedef anonymous_shared_memory seg_type;
	typedef  boost::mpl::bool_<false> base_type;

	typedef base_type is_named_object_type;
	YGGR_STATIC_CONSTANT(bool, E_is_named_object = is_named_object_type::value);

private:
	typedef segment_op_proxy this_type;

public:
	inline static bool s_create(seg_type& seg)
	{
		seg_type tmp;
		seg.swap(tmp);
		return true;
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES
	template<typename EArg, typename ...Args> inline
	static bool s_create(seg_type& seg, BOOST_FWD_REF(EArg) earg, BOOST_FWD_REF(Args)... args)
	{
		return this_type::s_prv_create(seg, boost::forward<EArg>(earg), boost::forward<Args>(args)...); 
	}

#else

#	define BOOST_PP_LOCAL_MACRO(__n__) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) inline \
		static bool s_create(seg_type& seg YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS)) { \
			return this_type::s_prv_create(seg YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
											YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline static void s_remove(seg_type& seg)
	{
	    this_type::s_create(seg);
	}

	inline static bool s_seg_is_empty(const seg_type& seg)
	{
		return this_type::s_seg_size(seg) == 0;
	}

	inline static size_type s_seg_size(const seg_type& seg)
	{
		return seg.get_size();
	}

	inline static bool s_is_named_enable_object(void)
	{
		return !!base_type::value;
	}

private:

	inline static bool s_prv_create(seg_type& seg, std::size_t size)
	{
		return 
			this_type::s_prv_create_detail(
				seg, size, 0,
				boost::bind(
					&this_type::s_prv_handler_error_fixer,
					_1, boost::ref(seg)) );
	}

	inline static bool s_prv_create(seg_type& seg, std::size_t size, void* addr)
	{
		return 
			this_type::s_prv_create_detail(
				seg, size, addr,
				boost::bind(
					&this_type::s_prv_handler_error_fixer,
					_1, boost::ref(seg)) );
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								std::size_t size,
								const Handler& err_fixer)
	{
		return this_type::s_prv_create_detail(seg, size, 0, err_fixer );
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								std::size_t size,
								void* addr,
								const Handler& err_fixer)
	{
		return this_type::s_prv_create_detail(seg, size, addr, err_fixer );
	}

	template<typename Handler> inline
	static bool s_prv_create_detail(seg_type& seg,
										std::size_t size,
										void* addr,
										const Handler& err_fixer)
	{
		if(!size)
		{
			return false;
		}

		try
		{
			boost::interprocess::anonymous_shared_memory(size, addr).swap(seg);
			return !!seg.get_size();
		}
		catch(const segment_error& e)
		{
			err_fixer(e);
		}

		try
		{
			boost::interprocess::anonymous_shared_memory(size, addr).swap(seg);
			return !!seg.get_size();
		}
		catch(const segment_error& e)
		{
			exception::exception::throw_error(e.get_error_code(), e.what());
		}

		return false;
	}

	// default_err_fixer
	inline static void s_prv_handler_error_fixer(const segment_error& e, seg_type& seg)
	{
		this_type::s_remove(seg);
	}
};

} // segment
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif // _MSC_VER

#include <boost/interprocess/detail/config_end.hpp>

#endif // __YGGR_SEGMENT_ANONYMOUS_SHARED_MEMORY_OP_PROXY_HPP__
