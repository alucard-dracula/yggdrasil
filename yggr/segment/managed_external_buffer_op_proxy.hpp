//managed_external_buffer_op_proxy.hpp

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

#ifndef __YGGR_SEGMENT_MANAGED_EXTERNAL_BUFFER_OP_PROXY_HPP__
#define __YGGR_SEGMENT_MANAGED_EXTERNAL_BUFFER_OP_PROXY_HPP__

#ifndef YGGR_SEGMENT_OPERATOR_INCLUDE
#	error "include error managed_mapped_file_op_proxy.hpp please include segment_op_proxy.hpp."
#endif // YGGR_SEGMENT_OPERATOR_INCLUDE

#include <yggr/segment/segment_config.hpp> // it must be written in the first sentence include

#include <yggr/base/static_constant.hpp>
#include <yggr/bind/bind.hpp>
#include <yggr/nonable/noncreateable.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/mplex/static_assert.hpp>

#include <yggr/segment/segment_error.hpp>
#include <yggr/exception/exception.hpp>

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/mpl/bool.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace segment
{

template<typename Tx1, typename Tx2,
			template<typename _Tx3> class Tx3 >
class segment_op_proxy< boost::interprocess::basic_managed_external_buffer<Tx1, Tx2, Tx3> >
	: public boost::mpl::bool_<false>,
		private nonable::noncreateable
{
public:
	typedef boost::interprocess::basic_managed_external_buffer<Tx1, Tx2, Tx3> seg_type;
	typedef boost::mpl::bool_<false> base_type;

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
		return !seg.get_segment_manager();
	}

	inline static size_type s_seg_size(const seg_type& seg)
	{
		return seg.get_segment_manager()? seg.get_size() : 0;
	}

	inline static bool s_is_named_enable_object(void)
	{
		return !!base_type::value;
	}

private:
	// create_only
	inline static bool s_prv_create(seg_type& seg, 
									segment_op::create_only_type op, 
									void* addr, std::size_t size)
	{
		return 
			this_type::s_prv_create_detail(
				seg, op, addr, size,
				boost::bind(
					&this_type::s_prv_handler_error_fixer,
					_1, boost::ref(seg)));
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg, 
								segment_op::open_only_type op, 
								void* addr, std::size_t size,
								const Handler& err_fixer)
	{
		return this_type::s_prv_create_detail(seg, op, addr, size, err_fixer);
	}

	// open_only
	inline static bool s_prv_create(seg_type& seg, 
									segment_op::open_only_type op, 
									void* addr, std::size_t size)
	{
		return 
			this_type::s_prv_create_detail(
				seg, op, addr, size,
				boost::bind(
					&this_type::s_prv_handler_error_fixer,
					_1, boost::ref(seg)));
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg, 
								segment_op::create_only_type op, 
								void* addr, std::size_t size, 
								const Handler& err_fixer)
	{
		return this_type::s_prv_create_detail(seg, op, addr, size, err_fixer);
	}

	template<typename Op, typename Handler> inline
	static bool s_prv_create_detail(seg_type& seg, 
										Op op, 
										void* addr, 
										std::size_t size, 
										const Handler& err_fixer)
	{
		if(!size)
		{
			return false;
		}

		try
		{
			seg_type tmp(op, addr, size);
			seg.swap(tmp);
			return true;
		}
		catch(const segment_error& e)
		{
			err_fixer(e);
		}

		try
		{
			seg_type tmp(op, addr, size);
			seg.swap(tmp);
			return true;
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

#endif // __YGGR_SEGMENT_MANAGED_EXTERNAL_BUFFER_OP_PROXY_HPP__
