//string_archive_helper.hpp

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

#ifndef __YGGR_ARCHIVE_HELPER_STRING_ARCHIVE_HELPER_HPP__
#define __YGGR_ARCHIVE_HELPER_STRING_ARCHIVE_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/ptr_single.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/archive/buffer_real_value_def.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/ref.hpp>

#include <memory>
#include <sstream>


#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace archive
{
namespace archive_helper
{

template<typename Archive,
			typename Char = typename Archive::char_type,
			typename Traits = typename Archive::traits_type,
			typename Alloc = std::allocator<Char>,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String = std::basic_string, // using ostream.str() support type
			template<typename _Char, typename _Traits, typename _Alloc>
			class OStringStream = std::basic_ostringstream,
			template<typename _Char, typename _Traits, typename _Alloc>
			class IStringStream = std::basic_istringstream
			>
class string_archive_helper 
	: private nonable::noncreateable
{
public:

	typedef Archive archive_type;
	typedef Char char_type;
	typedef Traits traits_type;
	typedef Alloc allocator_type;

	typedef Basic_String<char_type, traits_type, allocator_type> buf_type;

	typedef char_type buf_val_type;
	typedef typename buffer_real_value<buf_val_type>::type buf_real_val_type;

	typedef OStringStream<char_type, traits_type, allocator_type> output_stream_type;
	typedef IStringStream<char_type, traits_type, allocator_type> input_stream_type;

public:

	inline static buf_type& buf_copy(buf_type& buf1, BOOST_RV_REF(buf_type) buf2)
	{
		if(boost::addressof(buf1) == rvref_to_pointer(boost::move(buf2)))
		{
			return buf1;
		}

		return copy_or_move_or_swap(buf1, boost::move(buf2));
	}

	inline static buf_type& buf_copy(buf_type& buf1, const buf_type& buf2)
	{
		if(boost::addressof(buf1) == boost::addressof(buf2))
		{
			return buf1;
		}

		size_type size = buf2.size();
		if(!size)
		{
			return buf1;
		}

		buf1.resize(size);
		memcpy(&buf1[0], &buf2[0], sizeof(buf_real_val_type) * size);
		return buf1;
	}

	inline static void buf_swap(buf_type& buf1, buf_type& buf2)
	{
		if(boost::addressof(buf1) == boost::addressof(buf2))
		{
			return;
		}
		buf1.swap(buf2);
	}

	inline static void buf_resize(buf_type& buf, size_type size)
	{
		if(!size)
		{
			buf_type tbuf;
			yggr::swap(buf, tbuf);
			return;
		}

		buf.resize(size);
	}

	inline static const buf_real_val_type* buf_real_val_ptr(const buf_type& buf)
	{
		return reinterpret_cast<const buf_real_val_type*>(buf.data());
	}

	inline static buf_real_val_type* buf_real_val_ptr(buf_type& buf)
	{
		return const_cast<buf_real_val_type*>(reinterpret_cast<const buf_real_val_type*>(buf.data()));
	}

	inline static const buf_val_type* buf_val_ptr(const buf_type& buf)
	{
		return buf.data();
	}

	inline static buf_val_type* buf_val_ptr(buf_type& buf)
	{
		return const_cast<buf_val_type*>(buf.data());
	}

	inline static bool buf_compare(const buf_type& buf1, const buf_type& buf2)
	{
		return !buf1.compare(buf2);
	}

	inline static size_type buf_size(const buf_type& buf)
	{
		return buf.size();
	}

	inline static bool buf_empty(const buf_type& buf)
	{
		return buf.empty();
	}

	inline static void buf_clear(buf_type& buf)
	{
		buf.clear();
	}

	inline static void buf_init(buf_type& buf)
	{
		buf_type tmp;
		yggr::swap(buf, tmp);
	}

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename Eins_Arg, typename ...Args> inline
	static void buf_assign(buf_type& buf, BOOST_FWD_REF(Eins_Arg) eins_arg, BOOST_FWD_REF(Args)... args)
	{
		buf.assign(eins_arg, boost::forward<Args>(args)...);
	}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template< YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		static void buf_assign(buf_type& buf \
								YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
								YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			buf.assign(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES


	template<typename Object> inline
	static bool save(const Object& obj, buf_type& buf)
	{
		try
		{
			output_stream_type os;
			archive_type ar(os, 1);
			ar & obj;
			buf = os.str();
		}
		catch(const boost::archive::archive_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(...)
		{
			return false;
		}

		return true;
	}

	template<typename Object> inline
	static bool load(Object& obj, buf_type& buf)
	{
		try
		{
			input_stream_type is(buf);
			archive_type ar(is, 1);
			ar & obj;
		}
		catch(const boost::archive::archive_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(...)
		{
			return false;
		}

		return true;
	}
};

} // namespace archive_helper
} // namesapce archive
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_ARCHIVE_HELPER_STRING_ARCHIVE_HELPER_HPP__
