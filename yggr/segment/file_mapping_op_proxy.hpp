//file_mapping_op_proxy.hpp

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

#ifndef __YGGR_SEGMENT_FILE_MAPPING_OP_PROXY_HPP__
#define __YGGR_SEGMENT_FILE_MAPPING_OP_PROXY_HPP__

#ifndef YGGR_SEGMENT_OPERATOR_INCLUDE
#	error "include error file_mapping_op_proxy.hpp please include segment_op_proxy.hpp."
#endif // YGGR_SEGMENT_OPERATOR_INCLUDE

#include <yggr/segment/segment_config.hpp> // it must be written in the first sentence include

#include <yggr/base/static_constant.hpp>
#include <yggr/bind/bind.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/file_system/local_fsys_operators.hpp>
#include <yggr/exception/exception.hpp>

#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_destroyer.hpp>
#include <yggr/segment/segment_error.hpp>

#include <boost/interprocess/file_mapping.hpp>
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
class segment_op_proxy<boost::interprocess::file_mapping>
	: public boost::mpl::bool_<true>,
		private nonable::noncreateable
{
public:
	typedef charset::utf8_string key_type;
	typedef boost::interprocess::file_mapping seg_type;

	typedef boost::mpl::bool_<true> base_type;

	typedef base_type is_named_object_type;
	YGGR_STATIC_CONSTANT(bool, E_is_named_object = is_named_object_type::value);

private:
	typedef segment_destroyer<seg_type> segment_destroyer_type;
	typedef file_system::local_fsys_operators local_fsys_operators_type;
	typedef local_fsys_operators_type::file_size_type file_size_type;

private:
	typedef segment_op_proxy this_type;

public:
    // swap create
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

	inline static void s_remove(const key_type& name)
	{
		if(name.empty())
		{
			return;
		}

		{ segment_destroyer_type del(name.data()); }
	}

	inline static bool s_seg_is_empty(const seg_type& seg)
	{
		return this_type::s_seg_size(seg) == 0;
	}

	inline static size_type s_seg_size(const seg_type& seg)
	{
		const char* fname = seg.get_name();
		if(!fname)
		{
			return 0;
		}

		file_size_type fsize = 0;
		try
		{
			fsize = local_fsys_operators_type::file_size(
						local_fsys_operators_type::make_path_object(fname));
		}
		catch(const local_fsys_operators_type::exception_type&)
		{
			return 0;
		}
		catch(const yggr::stl_exception&)
		{
			return 0;
		}
		return fsize == local_fsys_operators_type::npos? 0 : fsize;
	}

	inline static bool s_is_named_enable_object(void)
	{
		return !!base_type::value;
	}

private:
	// create_only-handler
	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::create_only_type op,
								size_type size,
								u32 mode,
								const Handler& err_fixer)
	{
		return
			(this_type::s_file_create_only(name, size))
			&& (this_type::s_prv_create_segment(seg, name, mode, err_fixer)
				|| this_type::s_prv_create_segment(
						seg, name, mode,
						boost::bind(
							&this_type::s_prv_handler_error_fixer_of_error,
							_1, boost::cref(name))));
	}

	// open_or_create-handler
	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_or_create_type op,
								size_type size,
								u32 mode,
								const Handler& err_fixer)
	{
		return
			(this_type::s_file_open_or_create(name, size))
			&& (this_type::s_prv_create_segment(seg, name, mode, err_fixer)
				|| this_type::s_prv_create_segment(
					seg, name, mode,
					boost::bind(
						&this_type::s_prv_handler_error_fixer_of_error,
						_1, boost::cref(name))));
	}

	// open_only-handler
	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_only_type op,
								u32 mode,
								const Handler& err_fixer)
	{
		return
			(this_type::s_file_open_only(name))
			&& (this_type::s_prv_create_segment(seg, name, mode, err_fixer)
				|| this_type::s_prv_create_segment(
					seg, name, mode,
					boost::bind(
						&this_type::s_prv_handler_error_fixer_of_error,
						_1, boost::cref(name))));
	}

	// file_create_only
	inline static bool s_file_create_only(const key_type& name, size_type size)
	{
		file_size_type ret = 0;
		try
		{
			ret = local_fsys_operators_type::create_file_of_binary(
					local_fsys_operators_type::make_path_object(name), size,
					local_fsys_operators_type::options_type::dir_create_if_not_exists);
		}
		catch(const local_fsys_operators_type::exception_type& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const yggr::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return ret != local_fsys_operators_type::npos;
	}

	// file_open_or_create
	inline static bool s_file_open_or_create(const key_type& name, size_type size)
	{
		file_size_type ret = 0;
		try
		{
			ret = local_fsys_operators_type::create_file_of_binary(
					local_fsys_operators_type::make_path_object(name), size,
					local_fsys_operators_type::options_type::dir_create_if_not_exists
					| local_fsys_operators_type::options_type::file_success_if_exists);
		}
		catch(const local_fsys_operators_type::exception_type& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const yggr::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return ret != local_fsys_operators_type::npos;
	}

	// file_open_only
	inline static bool s_file_open_only(const key_type& name)
	{
		try
		{
			return local_fsys_operators_type::is_regular_file(
					local_fsys_operators_type::make_path_object(name));
		}
		catch(const local_fsys_operators_type::exception_type& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const yggr::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return false;
	}

	// create_execute
	template<typename Handler> inline
	static bool s_prv_create_segment(seg_type& seg,
										const key_type& name,
										u32 mode,
										const Handler& err_fixer)
	{
		try
		{
			seg_type tmp(name.data(), boost::interprocess::mode_t(mode));
			seg.swap(tmp);
			return true;
		}
		catch(const segment_error& e)
		{
			err_fixer(e);
		}

		return false;
	}

	// default_err_fixer
	inline static void s_prv_handler_error_fixer_of_create(const segment_error& e, const key_type& key)
	{
		assert(!key.empty());
		this_type::s_remove(key);
	}

	inline static void s_prv_handler_error_fixer_of_open(const segment_error& e, const key_type& key)
	{
		assert(!key.empty());
	}

	inline static void s_prv_handler_error_fixer_of_error(const segment_error& e, const key_type& key)
	{
		exception::exception::throw_error(e.get_error_code(), e.what());
	}
};

} // segment
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif // _MSC_VER

#endif //__YGGR_SEGMENT_FILE_MAPPING_OP_PROXY_HPP__
