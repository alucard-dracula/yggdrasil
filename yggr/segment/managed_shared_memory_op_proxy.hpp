//managed_shared_memory_op_proxy.hpp

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

#ifndef __YGGR_SEGMENT_MANAGED_SHARED_MEMORY_OP_PROXY_HPP__
#define __YGGR_SEGMENT_MANAGED_SHARED_MEMORY_OP_PROXY_HPP__

#ifndef YGGR_SEGMENT_OPERATOR_INCLUDE
#	error "include error managed_shared_memory_op_proxy.hpp please include segment_op_proxy.hpp."
#endif // YGGR_SEGMENT_OPERATOR_INCLUDE

#include <yggr/segment/segment_config.hpp> // it must be written in the first sentence include

#include <yggr/base/static_constant.hpp>
#include <yggr/bind/bind.hpp>

#include <yggr/nonable/noncreateable.hpp>
#include <yggr/move/move.hpp>
#include <yggr/charset/utf8_string.hpp>

#include <yggr/ppex/foo_params.hpp>
#include <yggr/ppex/symbols.hpp>

#include <yggr/exception/exception.hpp>

#include <yggr/segment/segment_container.hpp>
#include <yggr/segment/segment_op.hpp>
#include <yggr/segment/segment_destroyer.hpp>
#include <yggr/segment/segment_error.hpp>

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include <boost/mpl/if.hpp>

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
class segment_op_proxy
		<
			boost::interprocess::basic_managed_shared_memory<Tx1, Tx2, Tx3>
		>
	: public boost::mpl::bool_<true>,
		private nonable::noncreateable
{
public:
	//typedef Key key_type;
	typedef charset::utf8_string key_type;
	typedef boost::interprocess::basic_managed_shared_memory<Tx1, Tx2, Tx3> seg_type;

	typedef boost::mpl::bool_<true> base_type;

	typedef base_type is_named_object_type;
	YGGR_STATIC_CONSTANT(bool, E_is_named_object = is_named_object_type::value);

private:
	typedef segment_destroyer<seg_type> segment_destroyer_type;

private:
	typedef segment_op_proxy this_type;

public:

	//  swap s_create
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
									const key_type& name,
									segment_op::create_only_type op,
									typename seg_type::size_type size)
	{
		return
			this_type::s_prv_create_of_creation_mode(
				seg, name, op, size, 0,
				boost::interprocess::permissions(),
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_create,
					_1, boost::cref(name)));
	}

	inline static bool s_prv_create(seg_type& seg,
									const key_type& name,
									segment_op::create_only_type op,
									typename seg_type::size_type size,
									const void* addr)
	{
		return
			this_type::s_prv_create_of_creation_mode(
				seg, name, op, size, addr,
				boost::interprocess::permissions(),
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_create,
					_1, boost::cref(name)));
	}

	inline static bool s_prv_create(seg_type& seg,
									const key_type& name,
									segment_op::create_only_type op,
									typename seg_type::size_type size,
									const void* addr,
									const boost::interprocess::permissions& perm)
	{
		return
			this_type::s_prv_create_of_creation_mode(
				seg, name, op, size, addr, perm,
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_create,
					_1, boost::cref(name)));
	}

	// open_or_create
	inline static bool s_prv_create(seg_type& seg,
									const key_type& name,
									segment_op::open_or_create_type op,
									typename seg_type::size_type size)
	{
		return
			this_type::s_prv_create_of_creation_mode(
				seg, name, op, size, 0,
				boost::interprocess::permissions(),
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_create,
					_1, boost::cref(name)));
	}

	inline static bool s_prv_create(seg_type& seg,
									const key_type& name,
									segment_op::open_or_create_type op,
									typename seg_type::size_type size,
									const void* addr)
	{
		return
			this_type::s_prv_create_of_creation_mode(
				seg, name, op, size, addr,
				boost::interprocess::permissions(),
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_create,
					_1, boost::cref(name)));
	}

	inline static bool s_prv_create(seg_type& seg,
									const key_type& name,
									segment_op::open_or_create_type op,
									typename seg_type::size_type size,
									const void* addr,
									const boost::interprocess::permissions& perm)
	{
		return
			this_type::s_prv_create_of_creation_mode(
				seg, name, op, size, addr, perm,
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_create,
					_1, boost::cref(name)));
	}

	// create_only-handler
	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::create_only_type op,
								typename seg_type::size_type size,
								const Handler& err_fixer )
	{
		return
			this_type::s_prv_create_of_creation_mode(
				seg, name, op, size, 0,
				boost::interprocess::permissions(),
				err_fixer);
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::create_only_type op,
								typename seg_type::size_type size,
								const void* addr,
								const Handler& err_fixer )
	{
		return
			this_type::s_prv_create_of_creation_mode(
				seg, name, op, size, addr,
				boost::interprocess::permissions(),
				err_fixer);
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::create_only_type op,
								typename seg_type::size_type size,
								const void* addr,
								const boost::interprocess::permissions& perm,
								const Handler& err_fixer )
	{
		return this_type::s_prv_create_of_creation_mode(seg, name, op, size, addr, perm, err_fixer);
	}

	// open_or_create-handler
	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_or_create_type op,
								typename seg_type::size_type size,
								const Handler& err_fixer )
	{
		return
			this_type::s_prv_create_of_creation_mode(
				seg, name, op, size, 0,
				boost::interprocess::permissions(),
				err_fixer);
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_or_create_type op,
								typename seg_type::size_type size,
								const void* addr,
								const Handler& err_fixer )
	{
		return
			this_type::s_prv_create_of_creation_mode(
				seg, name, op, size, addr,
				boost::interprocess::permissions(),
				err_fixer );
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_or_create_type op,
								typename seg_type::size_type size,
								const void* addr,
								const boost::interprocess::permissions& perm,
								const Handler& err_fixer )
	{
		return this_type::s_prv_create_of_creation_mode(seg, name, op, size, addr, perm, err_fixer);
	}

	// creation_version
	template<typename Op, typename Handler> inline
	static bool s_prv_create_of_creation_mode(seg_type& seg,
												const key_type& name,
												Op op,
												typename seg_type::size_type size,
												const void* addr,
												const boost::interprocess::permissions& perm,
												const Handler& err_fixer )
	{
		if(name.empty())
		{
			return false;
		}

		try
		{
			seg_type tmp(op, name.data(), size, addr, perm);
			seg.swap(tmp);
			return true;
		}
		catch(const segment_error& e)
		{
            err_fixer(e);
		}

		try
		{
			seg_type tmp(op, name.data(), size, addr, perm);
			seg.swap(tmp);
			return true;
		}
		catch(const segment_error& e)
		{
			exception::exception::throw_error(e.get_error_code(), e.what());
		}

		return false;
	}

	// open_copy_on_write
	inline static bool s_prv_create(seg_type& seg,
									const key_type& name,
									segment_op::open_copy_on_write_type op)
	{
		return
			this_type::s_prv_create_of_open_mode(
				seg, name, op, 0,
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_open,
					_1, boost::cref(name)));
	}

	inline static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_copy_on_write_type op,
								const void* addr)
	{
		return
			this_type::s_prv_create_of_open_mode(
				seg, name, op, addr,
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_open,
					_1, boost::cref(name)));
	}

	// open_read_only
	inline static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_read_only_type op)
	{
		return
			this_type::s_prv_create_of_open_mode(
				seg, name, op, 0,
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_open,
					_1, boost::cref(name)));
	}

	inline static bool s_prv_create(seg_type& seg,
									const key_type& name,
									segment_op::open_read_only_type op,
									const void* addr)
	{
		return
			this_type::s_prv_create_of_open_mode(
				seg, name, op, addr,
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_open,
					_1, boost::cref(name)));
	}

	//open_only
	inline static bool s_prv_create(seg_type& seg,
									const key_type& name,
									segment_op::open_only_type op)
	{
		return
			this_type::s_prv_create_of_open_mode(
				seg, name, op, 0,
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_open,
					_1, boost::cref(name)));
	}

	inline static bool s_prv_create(seg_type& seg,
									const key_type& name,
									segment_op::open_only_type op,
									const void* addr)
	{
		return
			this_type::s_prv_create_of_open_mode(
				seg, name, op, addr,
				boost::bind(
					&this_type::s_prv_handler_error_fixer_of_open,
					_1, boost::cref(name)));
	}

	// open_copy_on_write-handler
	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_copy_on_write_type op,
								const Handler& err_fixer)
	{
		return this_type::s_prv_create_of_open_mode(seg, name, op, 0, err_fixer);
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_copy_on_write_type op,
								const void* addr,
								const Handler& err_fixer)
	{
		return this_type::s_prv_create_of_open_mode(seg, name, op, addr, err_fixer);
	}

	// open_read_only-handler
	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_read_only_type op,
								const Handler& err_fixer)
	{
		return this_type::s_prv_create_of_open_mode(seg, name, op, 0, err_fixer);
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_read_only_type op,
								const void* addr,
								const Handler& err_fixer)
	{
		return this_type::s_prv_create_of_open_mode(seg, name, op, addr, err_fixer);
	}

	//open_only-handler
	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_only_type op,
								const Handler& err_fixer)
	{
		return this_type::s_prv_create_of_open_mode(seg, name, op, 0, err_fixer);
	}

	template<typename Handler> inline
	static bool s_prv_create(seg_type& seg,
								const key_type& name,
								segment_op::open_only_type op,
								const void* addr,
								const Handler& err_fixer)
	{
		return this_type::s_prv_create_of_open_mode(seg, name, op, addr, err_fixer);
	}

	// open_version
	template<typename Op, typename Handler> inline
	static bool s_prv_create_of_open_mode(seg_type& seg,
											const key_type& name,
											Op op,
											const void* addr,
											const Handler& err_fixer )
	{
		if(name.empty())
		{
			return false;
		}

		try
		{
			seg_type tmp(op, name.data(), addr);
			seg.swap(tmp);
			return true;
		}
		catch(const segment_error& e)
		{
            err_fixer(e);
		}

		try
		{
			seg_type tmp(op, name.data(), addr);
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
	inline static void s_prv_handler_error_fixer_of_create(const segment_error& e, const key_type& key)
	{
		assert(!key.empty());
		this_type::s_remove(key);
	}

	inline static void s_prv_handler_error_fixer_of_open(const segment_error& e, const key_type& key)
	{
		assert(!key.empty());
	}
};

} // segment
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif // _MSC_VER

#include <boost/interprocess/detail/config_end.hpp>

#endif //__YGGR_SEGMENT_MANAGED_SHARED_MEMORY_OP_PROXY_HPP__
