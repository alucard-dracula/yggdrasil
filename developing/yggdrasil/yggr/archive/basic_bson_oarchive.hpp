//basic_bson_oarchive.hpp

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

#ifndef __YGGR_ARCHIVE_BASIC_BSON_OARCHIVE_HPP__
#define __YGGR_ARCHIVE_BASIC_BSON_OARCHIVE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>
#include <boost/archive/detail/common_oarchive.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/serialization/string.hpp>

#include <yggr/nsql_database_system/c_bson.hpp>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace yggr
{
namespace archive
{

template<typename Archive>
class basic_bson_oarchive
	: public boost::archive::detail::common_oarchive<Archive>
{
public:
	typedef Archive archive_type;
	typedef boost::archive::detail::common_oarchive<Archive> base_type;
	//typedef typename Archive::buf_type buf_type;

private:
	typedef nsql_database_system::c_bson buf_type;
	typedef nsql_database_system::c_bson_access_proxy proxy_type;
	typedef basic_bson_oarchive this_type;

protected:
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
public:
#elif defined(BOOST_MSVC)
    // for some inexplicable reason insertion of "class" generates compile erro
    // on msvc 7.1
    friend boost::archive::detail::interface_oarchive<Archive>;
    friend class boost::archive::save_access;
#else
    friend class boost::archive::detail::interface_oarchive<Archive>;
    friend class boost::archive::save_access;
#endif

	basic_bson_oarchive(buf_type& buf, unsigned int flags)
		: base_type(flags), _buf(buf)
	{
	}

	~basic_bson_oarchive(void)
	{
	}

	void save_override(const boost::archive::object_id_type& t, int) //u32
	{
		if(Archive::s_is_nld())
		{
			return;
		}
		const u32 tmp = t;
		proxy_type::save(_buf, "boost_ar_obj_id", tmp);
	}

    void save_override(const boost::archive::object_reference_type& t, int n) // based object_id_type //u32
	{
		if(Archive::s_is_nld())
		{
			return;
		}
		// must be use object_id_type foo
		const boost::archive::object_id_type& tmp = t;
		this_type::save_override(tmp, n);
	}

	void save_override(const boost::archive::version_type& t, int) // u16
	{
		if(Archive::s_is_nld())
		{
			return;
		}
		const u16& tmp = t;
		proxy_type::save(_buf, "boost_ar_ver", tmp);
	}

	void save_override(const boost::archive::library_version_type& t, int)
	{
		if(Archive::s_is_nld())
		{
			return;
		}
		const u16& tmp = t;
		proxy_type::save(_buf, "boost_ar_library_ver", tmp);
	}

    void save_override(const boost::archive::class_id_type& t, int) // u16
	{
		if(Archive::s_is_nld())
		{
			return;
		}
		const u16& tmp = t;
		proxy_type::save(_buf, "boost_ar_class_id", tmp);
	}

    void save_override(const boost::archive::class_id_optional_type& t, int n) //based class_id_type // u16
	{
		if(Archive::s_is_nld())
		{
			return;
		}
		// must be use class_id_type foo
		const boost::archive::class_id_type& tmp = t;
		this_type::save_override(tmp, n);
	}

	void save_override(const boost::archive::class_id_reference_type& t, int n) //based class_id_type // u16
	{
		if(Archive::s_is_nld())
		{
			return;
		}
		//  must be use class_id_type foo
		const boost::archive::class_id_type& tmp = t;
		this_type::save_override(tmp, n);
	}

    void save_override(const boost::archive::class_name_type& t, int) // const char*
	{
		if(Archive::s_is_nld())
		{
			return;
		}
		assert(false);
		const char* tmp = t;
		proxy_type::save(_buf, "boost_ar_class_name", std::string(tmp));
	}

    void save_override(const boost::archive::tracking_type& t, int) // bool
	{
		if(Archive::s_is_nld())
		{
			return;
		}
		const bool tmp = t;
		proxy_type::save(_buf, "boost_ar_tracking", tmp);
	}

	template<typename T>
	bool save_begin(const char* name)
	{
		return proxy_type::save_begin<T>(_buf, name);
	}

	template<typename T>
	bool save_end(void)
	{
		return proxy_type::save_end<T>(_buf);
	}

    template<typename T>
    void save_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			T& t,
			int)
    {
		// if compile error this iss most likely not used NVP
		BOOST_MPL_ASSERT((boost::mpl::if_<typename nsql_database_system::is_bson_inside_type<T>::type,
											boost::mpl::bool_<false>, boost::mpl::bool_<true> >));
		this->base_type::save_override(t, 0);
    }

    // special treatment for name-value pairs.
    template<typename T>
    void save_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
        ::boost::serialization::nvp<T>& t,
        int)
	{
	    typedef typename boost::mpl::if_<
                                    typename nsql_database_system::is_bson_inside_type<T>::type,
                                    int*,
                                    char*>::type call_type;
        prv_save_override(t, (call_type)0);
    }

protected:
	template<typename T>
	void prv_save_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
        ::boost::serialization::nvp<T>& t,
        int*)
	{
        if(this->This()->save_begin<T>(t.name()))
		{
			this->base_type::save_override(t.const_value(), 0);
			this->This()->template save_end<T>();
		}
    }

	template<typename T>
	void prv_save_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
        ::boost::serialization::nvp<T>& t,
        char*)
	{
		buf_type buf;
		archive_type ar(buf, base_type::get_flags());
		ar.boost::archive::detail::common_oarchive<Archive>::save_override(t.const_value(), 0);
		buf.finish();
		//std::cout << buf << std::endl;
		proxy_type::save(_buf, t.name(), buf);
    }

protected:
	buf_type& _buf;
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif //BOOST_MSVC

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas



#endif // __YGGR_ARCHIVE_BASIC_BSON_OARCHIVE_HPP__
