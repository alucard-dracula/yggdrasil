//xml_archive_helper.hpp

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

#ifndef __YGGR_ARCHIVE_ARCHIVE_HELPER_XML_ARCHIVE_HELPER_HPP__
#define __YGGR_ARCHIVE_ARCHIVE_HELPER_XML_ARCHIVE_HELPER_HPP__

#include <boost/serialization/nvp.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/base/exception.hpp>
#include <yggr/base/ptr_single.hpp>
#include <boost/serialization/nvp.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>

#include <yggr/archive/buffer_real_value_def.hpp>

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
			class Basic_String = std::basic_string,
			template<typename _Char, typename _Traits, typename _Alloc>
			class OStringStream = std::basic_ostringstream,
			template<typename _Char, typename _Traits, typename _Alloc>
			class IStringStream = std::basic_istringstream
		>
class xml_archive_helper : private nonable::noncreateable
{
public:
	typedef Archive archive_type;
	typedef Char char_type;
	typedef Traits traits_type;
	typedef Alloc alloc_type;

	typedef Basic_String<char_type, traits_type, alloc_type> buf_type;

	typedef char_type buf_val_type;
	typedef typename buffer_real_value<buf_val_type>::type buf_real_val_type;

	typedef OStringStream<char_type, traits_type, alloc_type> output_stream_type;
	typedef IStringStream<char_type, traits_type, alloc_type> input_stream_type;

public:

	static void buf_copy(buf_type& buf1, BOOST_RV_REF(buf_type) buf2)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		buf1 = boost::forward<buf_type>(buf2);
#else
		buf_type& buf_ref = buf2;
		buf1.swap(buf_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	static void buf_copy(buf_type& buf1, const buf_type& buf2)
	{
		if(&buf1 == &buf2)
		{
			return;
		}
		size_type size = buf2.size();
		buf1.resize(size);
		memcpy(&buf1[0], &buf2[0], sizeof(buf_real_val_type) * size);
	}

	static void buf_swap(buf_type& buf1, BOOST_RV_REF(buf_type) buf2)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//buf1.swap(boost::forward<buf_type>(buf2));
		buf1.swap(buf2);
#else
		buf_type& buf_ref = buf2;
		buf1.swap(buf_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	static void buf_swap(buf_type& buf1, buf_type& buf2)
	{
		if(&buf1 == &buf2)
		{
			return;
		}
		buf1.swap(buf2);
	}

	static void buf_resize(buf_type& buf, size_type size)
	{
		if(!size)
		{
			buf_type tbuf;
			std::swap(buf, tbuf);
			return;
		}

		buf.resize(size);
	}

	static const buf_real_val_type* buf_real_val_ptr(const buf_type& buf)
	{
		return reinterpret_cast<const buf_real_val_type*>(buf.data());
	}

	static buf_real_val_type* buf_real_val_ptr(buf_type& buf)
	{
		return const_cast<buf_real_val_type*>(reinterpret_cast<const buf_real_val_type*>(buf.data()));
	}

	static const buf_val_type* buf_val_ptr(const buf_type& buf)
	{
		return buf.data();
	}

	static buf_val_type* buf_val_ptr(buf_type& buf)
	{
		return const_cast<buf_val_type*>(buf.data());
	}

	static bool buf_compare(const buf_type& buf1, const buf_type& buf2)
	{
		return !buf1.compare(buf2);
	}

	static size_type buf_size(const buf_type& buf)
	{
		return buf.size();
	}

	static bool buf_empty(const buf_type& buf)
	{
		return buf.empty();
	}

	static void buf_clear(buf_type& buf)
	{
		buf.clear();
	}

	static void buf_init(buf_type& buf)
	{
		std::swap(buf, buf_type());
	}

	template<typename Object>
	static bool save(const boost::serialization::nvp<Object>& nvp, buf_type& buf)
	{
		try
		{
			output_stream_type os;
			archive_type ar(os, 1);
			ar & nvp;
			buf = os.str();
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const boost::archive::archive_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}

	template<typename Object>
	static bool save(const Object& obj, buf_type& buf)
	{
		try
		{
			output_stream_type os;
			archive_type ar(os, 1);
			ar & boost::serialization::make_nvp(obj.name(), obj.value());
			buf = os.str();
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const boost::archive::archive_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}

	

	template<typename Object>
	static bool load( 
#						ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
							const
#						endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
					boost::serialization::nvp<Object>& nvp, buf_type& buf)
	{
		try
		{
			input_stream_type is(buf);
			archive_type ar(is, 1);
			ar & nvp;
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const boost::archive::archive_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}

	template<typename Object>
	static bool load(Object& obj, buf_type& buf)
	{
		try
		{
			input_stream_type is(buf);
			archive_type ar(is, 1);
			ar & boost::serialization::make_nvp(obj.name(), obj.value());
		}
		catch(const boost::system::system_error& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const boost::archive::archive_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}
		catch(const compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}

	
};

} // namespace archive_helper
} // namesapce archive
} // namespace yggr

#pragma warning ( pop )
#endif //__YGGR_ARCHIVE_ARCHIVE_HELPER_XML_ARCHIVE_HELPER_HPP__