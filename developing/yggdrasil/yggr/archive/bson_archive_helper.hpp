//bson_archive_helper.hpp

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

#ifndef __YGGR_ARCHIVE_ARCHIVE_HELPER_BSON_ARCHIVE_HELPER_HPP__
#define __YGGR_ARCHIVE_ARCHIVE_HELPER_BSON_ARCHIVE_HELPER_HPP__

#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/base/exception.hpp>
#include <yggr/move/move.hpp>

namespace yggr
{
namespace archive
{
namespace archive_helper
{

template<typename Archive>
class bson_archive_helper
{
public:
	typedef Archive archive_type;
	typedef nsql_database_system::c_bson buf_type;
	typedef buf_type buf_val_type;
	//typedef buf_type::base_type buf_val_type;
	typedef buf_val_type buf_real_val_type;

	typedef nsql_database_system::c_bson output_stream_type;
	typedef nsql_database_system::c_bson input_stream_type;

private:
	typedef buf_type other_buf_type;

private:
	typedef bson_archive_helper this_type;

public:

	static void buf_copy(buf_type& buf1, BOOST_RV_REF(buf_type) buf2)
	{
		buf1 = boost::forward<buf_type>(buf2);
	}

	static void buf_copy(buf_type& buf1, const buf_type& buf2)
	{
		if(&buf1 == &buf2)
		{
			return;
		}
		buf1 = buf2;
	}

	template<typename Buffer1, typename Buffer2>
	static void buf_swap(Buffer1& buf1, Buffer2& buf2)
	{
		assert(false);
	}

	// c_bson not support r-value ref
	static void buf_swap(buf_type& buf1, BOOST_RV_REF(buf_type) buf2)
	{
		buf1.swap(buf2);
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
		buf.init();
	}

	static const buf_real_val_type* buf_real_val_ptr(const buf_type& buf)
	{
		return static_cast<const buf_real_val_type*>(&buf);
	}

	static buf_real_val_type* buf_real_val_ptr(buf_type& buf)
	{
		return static_cast<buf_real_val_type*>(&buf);
	}

	static const buf_val_type* buf_val_ptr(const buf_type& buf)
	{
		return static_cast<const buf_real_val_type*>(&buf);
	}

	static buf_val_type* buf_val_ptr(buf_type& buf)
	{
		return static_cast<buf_real_val_type*>(&buf);
	}

	static bool buf_compare(const buf_type& buf1, const buf_type& buf2)
	{
		return buf1 == buf2;
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
		buf.init();
	}

	template<typename Object>
	static bool save(const Object& obj, buf_type& buf) //optimization ver
	{
		typedef Object obj_type;
		try
		{
			archive_type ar(buf, 1);
			ar & obj;
			buf.finish();
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
		catch(compatibility::stl_exception& e)
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
			archive_type ar(buf, 1);
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
		typedef Object obj_type;

		try
		{
			archive_type ar(buf, 1);
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
		catch(compatibility::stl_exception& e)
		{
			exception::exception::throw_error(e);
			return false;
		}

		return true;
	}
};

} // namespace archive_helper
} // namespace archive
} // namespace yggr

#endif //__YGGR_ARCHIVE_ARCHIVE_HELPER_BSON_ARCHIVE_HELPER_HPP__
