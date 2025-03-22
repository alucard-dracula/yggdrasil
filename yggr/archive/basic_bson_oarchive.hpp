//basic_bson_oarchive.hpp

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

#ifndef __YGGR_ARCHIVE_BASIC_BSON_OARCHIVE_HPP__
#define __YGGR_ARCHIVE_BASIC_BSON_OARCHIVE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/serialization/pfto.hpp>
#include <yggr/serialization/nvp.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/friend.hpp>
#include <yggr/archive/archive_inner_keys.hpp>
#include <yggr/nsql_database_system/bson_inner_data.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>

#include <boost/mpl/if.hpp>
#include <boost/archive/detail/common_oarchive.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/serialization/string.hpp>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

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

private:
	typedef nsql_database_system::c_bson buf_type;
	typedef nsql_database_system::c_bson_access_proxy proxy_type;
	typedef buf_type::inner_string_type buf_inner_string_type;

private:
	typedef basic_bson_oarchive this_type;

protected:
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_oarchive<Archive>);
	friend class boost::archive::save_access;
#endif // BOOST_WORKAROUND(BOOST_MSVC, <= 1300)

	basic_bson_oarchive(buf_type& buf, unsigned int flags)
		: base_type(flags), _buf(buf)
	{
	}

	~basic_bson_oarchive(void)
	{
	}

	inline void save_override(const boost::archive::object_id_type& t YGGR_NON_PFTO_TYPE(int)) //u32
	{
		if(Archive::is_nld())
		{
			return;
		}
		u32 tmp = t;
		proxy_type::save(_buf, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_obj_id), tmp);
	}

	inline void save_override(const boost::archive::object_reference_type& t YGGR_NON_PFTO_TYPE(int)) // based object_id_type //u32
	{
		if(Archive::is_nld())
		{
			return;
		}
		// must be use object_id_type foo
		const boost::archive::object_id_type& tmp = t;
		u32 tmp_zwei = tmp;
		proxy_type::save(_buf, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_obj_id_ref), tmp_zwei);
	}

	inline void save_override(const boost::archive::version_type& t YGGR_NON_PFTO_TYPE(int)) // u16
	{
		if(Archive::is_nld())
		{
			return;
		}
		
		u32 tmp = t;
		proxy_type::save(_buf, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_ver), tmp);
	}

	inline void save_override(const boost::archive::library_version_type& t YGGR_NON_PFTO_TYPE(int))
	{
		if(Archive::is_nld())
		{
			return;
		}
		u16 tmp = t;
		proxy_type::save(_buf, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_library_ver), tmp);
	}

	inline void save_override(const boost::archive::class_id_type& t YGGR_NON_PFTO_TYPE(int)) // u16
	{
		if(Archive::is_nld())
		{
			return;
		}
		s16 tmp = t;
		proxy_type::save(_buf, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_class_id), tmp);
	}

	inline void save_override(const boost::archive::class_id_optional_type& t YGGR_PFTO_TYPE_VAR(int, n)) //based class_id_type // s16
	{
		if(Archive::is_nld())
		{
			return;
		}
		// must be use class_id_type foo
		const boost::archive::class_id_type& tmp = t;
		this_type::save_override(tmp YGGR_PFTO_VAR(n));
	}

	inline void save_override(const boost::archive::class_id_reference_type& t YGGR_NON_PFTO_TYPE(int)) //based class_id_type // u16
	{
		if(Archive::is_nld())
		{
			return;
		}
		//  must be use class_id_type foo
		const boost::archive::class_id_type& tmp = t;
		s16 tmp_zwei = tmp;
		proxy_type::save(_buf, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_class_id_ref), tmp_zwei);
	}

	inline void save_override(const boost::archive::class_name_type& t YGGR_NON_PFTO_TYPE(int)) // const char*
	{
		if(Archive::is_nld())
		{
			return;
		}

		const char* tmp = t;
		proxy_type::save(_buf, 
							YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_class_name),
							buf_inner_string_type(tmp, YGGR_STR_UTF8_STRING_CHARSET_NAME()));
	}

	inline void save_override(const boost::archive::tracking_type& t YGGR_NON_PFTO_TYPE(int)) // bool
	{
		if(Archive::is_nld())
		{
			return;
		}
		const bool tmp = t;
		proxy_type::save(_buf, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_tracking), tmp);
	}

	template<typename T, typename Char, typename Mark> inline
	bool save_begin(const Char* name, Mark& mark)
	{
		return proxy_type::save_begin<T>(_buf, name, mark);
	}

	template<typename T, 
				typename Char, typename Traits, typename Alloc, 
				typename Mark,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	bool save_begin(const Basic_String<Char, Traits, Alloc>& name, Mark& mark)
	{
		return proxy_type::save_begin<T>(_buf, name, mark);
	}

	// now serialization use this foo
	template<typename Char, typename Mark> inline
	bool save_begin(u8 t, const Char* name, Mark& mark)
	{
		return proxy_type::save_begin(_buf, t, name, mark);
	}

	// now serialization use this foo
	template<typename Char, typename Traits, typename Alloc, 
				typename Mark,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String > inline
	bool save_begin(u8 t, const Basic_String<Char, Traits, Alloc>& name, Mark& mark)
	{
		return proxy_type::save_begin(_buf, t, name, mark);
	}

	template<typename T, typename Mark> inline
	bool save_end(Mark& mark)
	{
		return proxy_type::save_end<T>(_buf, mark);
	}

	template<typename T> inline
	void save_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			T& t
			YGGR_NON_PFTO_TYPE(int))
	{
		// if compile error this issues most likely not used NVP
		BOOST_MPL_ASSERT((nsql_database_system::is_bson_splice_t<T>));
		this->base_type::save_override(t YGGR_PFTO_VAR(0));
	}

	// special treatment for name-value pairs.
	template<typename T> inline
	void save_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
		::boost::serialization::nvp<T>& t
			YGGR_NON_PFTO_TYPE(int))
	{
		typedef typename 
			boost::mpl::if_
			<
				typename nsql_database_system::is_bson_inside_type<T>::type,
				int*,	// inside_type
				char*   // not inside_type
			>::type call_type;
		prv_save_override(t, call_type(0));
	}

protected:
	// inner_type
	template<typename T> inline
	void prv_save_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
		::boost::serialization::nvp<T>& t,
		int*)
	{
		typedef typename 
			nsql_database_system::bson_value_mark
			<
				typename boost::remove_const<T>::type
			>::type mark_type;

		mark_type mark;
		if(this->This()->save_begin(nsql_database_system::overlap_bson_typeid(t.const_value()),
										t.name(), mark))
		{
			this->base_type::save_override(t.const_value() YGGR_PFTO_VAR(0));
			this->This()->template save_end<T>(mark);
		}
	}

	// non inner_type
	template<typename T> inline
	void prv_save_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
		::boost::serialization::nvp<T>& t,
		char*)
	{
		buf_type buf;
		archive_type ar(buf, base_type::get_flags());
		ar.boost::archive::detail::template common_oarchive<Archive>::save_override(t.const_value() YGGR_PFTO_VAR(0));
		proxy_type::save(_buf, t.name(), buf);
	}

protected:
	buf_type& _buf;
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif //BOOST_MSVC

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // __YGGR_ARCHIVE_BASIC_BSON_OARCHIVE_HPP__
