// basic_bson_iarchive.hpp

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

#ifndef __YGGR_ARCHIVE_BASIC_BSON_IARCHIVE_HPP__
#define __YGGR_ARCHIVE_BASIC_BSON_IARCHIVE_HPP__

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

#include <yggr/archive/archive_inner_keys.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>
#include <yggr/nsql_database_system/bson_inner_data.hpp>

#include <yggr/ppex/friend.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/detail/workaround.hpp>
#include <boost/archive/detail/common_iarchive.hpp>

#include <boost/serialization/string.hpp>
#include <boost/serialization/item_version_type.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_pod.hpp>

#include <cstring>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif // BOOST_MSVC

namespace yggr
{
namespace archive
{

/////////////////////////////////////////////////////////////////////////
// class xml_iarchive - read serialized objects from a input text stream
template<typename Archive>
class basic_bson_iarchive
	: public boost::archive::detail::common_iarchive<Archive>
{
public:
	typedef Archive archive_type;
	typedef boost::archive::detail::common_iarchive<Archive> base_type;

private:
	typedef nsql_database_system::c_bson buf_type;
	typedef buf_type::const_iterator buf_citer_type;
	typedef nsql_database_system::c_bson_access_proxy proxy_type;
	typedef buf_type::inner_string_type buf_inner_string_type;

private:
	typedef basic_bson_iarchive this_type;

protected:
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
public:
#else
	YGGR_PP_FRIEND_TYPENAME(boost::archive::detail::interface_oarchive<Archive>);
#endif // BOOST_WORKAROUND(BOOST_MSVC, <= 1300)

	basic_bson_iarchive(const buf_type& buf, unsigned int flags)
		: base_type(flags), _start(buf.begin()), _end(buf.end())
	{
		if(archive_type::is_skip_auto_id())
		{
			_start.skip_auto_id();
		}
	}

	~basic_bson_iarchive(void)
	{
	}

	inline void load_override(boost::archive::object_id_type& t YGGR_NON_PFTO_TYPE(int)) //u32 //1
	{
		if(_start == _end) return;
		u32& tmp = t;
		proxy_type::load(_start, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_obj_id), tmp);
		++_start;
	}

	inline void load_override(boost::archive::object_reference_type& t YGGR_NON_PFTO_TYPE(int)) // based object_id_type //u32 // xml archive don't need check it
	{
		// must be use boost::archive::object_id_type foo
		if(_start == _end) return;

		boost::archive::object_id_type& tmp = t;
		u32& tmp_val = tmp;
		proxy_type::load(_start, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_obj_id_ref), tmp_val);
		++_start;
	}

	inline void load_override(boost::archive::version_type& t YGGR_NON_PFTO_TYPE(int)) // u16 //1
	{
		if(_start == _end) return;
		u32& tmp = t;
		proxy_type::load(_start, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_ver), tmp);
		++_start;
	}

	inline void load_override(boost::archive::library_version_type& t YGGR_NON_PFTO_TYPE(int))
	{
		if(_start == _end) return;
		u16& tmp = t;
		proxy_type::load(_start, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_library_ver), tmp);
		++_start;
	}

	void load_override(boost::archive::class_id_type& t YGGR_NON_PFTO_TYPE(int)) // u16 // xml archive don't need check it
	{
		if(_start == _end) return;
		yggr::s16& tmp = t;
		proxy_type::load(_start, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_class_id), tmp);
		++_start;
	}

	inline void load_override(boost::archive::class_id_optional_type& t YGGR_NON_PFTO_TYPE(int)) //based class_id_type // s16 //1
	{
		boost::archive::class_id_type tmp;
		this_type::skip_override(tmp YGGR_PFTO_VAR(0));
	}

	inline void load_override(boost::archive::class_id_reference_type& t YGGR_NON_PFTO_TYPE_VAR(int, n)) //based class_id_type // s16
	{
		//must be use boost::archive::class_id_type foo
		if(_start == _end) return;
		boost::archive::class_id_type& tmp = t;
		this_type::load_override(tmp YGGR_PFTO_VAR(n));
	}

	inline void load_override(boost::archive::class_name_type& t YGGR_NON_PFTO_TYPE(int)) // const char*
	{
		if(_start == _end) return;
		charset::utf8_string tmp;
		proxy_type::load(_start, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_class_name), tmp);
		++_start;
		if(tmp.empty() || tmp.size() >= BOOST_SERIALIZATION_MAX_KEY_SIZE)
		{
			return;
		}

		assert(tmp.length() == tmp.size());

		memcpy(t.t, tmp.data(), tmp.size());
		t.t[tmp.size()] = 0;
	}

	inline void load_override(boost::archive::tracking_type& t YGGR_NON_PFTO_TYPE(int)) // bool  //1
	{
		if(_start == _end) return;
		bool& tmp = t;
		proxy_type::load(_start, YGGR_ARCHIVE_GET_INNER_KEY(buf_inner_string_type, boost_ar_tracking), tmp);
		++_start;
	}

	// now serialization use this foo
	template<typename Obj, typename Char> inline
	bool load_begin(buf_citer_type& tmp_iter, const Char* name)
	{
		return proxy_type::load_begin<Obj>(tmp_iter, _start, name);
	}

	// now serialization use this foo
	template<typename Obj,
				typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	bool load_begin(buf_citer_type& tmp_iter, const Basic_String<Char, Traits, Alloc>& name)
	{
		return proxy_type::load_begin<Obj>(tmp_iter, _start, name);
	}

	template<typename Obj, typename Char> inline
	bool load_begin(buf_citer_type& tmp_iter, u8 t, const Char* name)
	{
		return proxy_type::load_begin<Obj>(tmp_iter, _start, t, name);
	}

	template<typename Obj,
				typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc> class Basic_String> inline
	bool load_begin(buf_citer_type& tmp_iter, u8 t, const Basic_String<Char, Traits, Alloc>& name)
	{
		return proxy_type::load_begin<Obj>(tmp_iter, _start, t, name);
	}

	template<typename Obj> inline
	bool load_end(const buf_citer_type& tmp_iter)
	{
		return proxy_type::load_end<Obj>(tmp_iter, _start);
	}

	template<class T> inline
	void load_override(T& t YGGR_NON_PFTO_TYPE(int))
	{
		// if compile error this issues most likely not used NVP
		BOOST_MPL_ASSERT((nsql_database_system::is_bson_splice_t<T>));
		this->base_type::load_override(t YGGR_PFTO_VAR(0));
	}

	template<class T> inline
	void load_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif
		boost::serialization::nvp< T > & t
		YGGR_NON_PFTO_TYPE(int))
	{
		prv_load_override(t, (typename boost::mpl::if_<
                                        typename nsql_database_system::is_bson_inside_type<T>::type,
                                        int*,
                                        char*>::type)0);
	}

protected:
	template<typename T> inline
	void prv_load_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
		::boost::serialization::nvp< T > & t,
		int*)
	{
		buf_citer_type tmp_iter;
		if(this->This()->template load_begin<T>(tmp_iter, t.name()))
		{
			this->base_type::load_override(t.value() YGGR_PFTO_VAR(0));
			this->This()->template load_end<T>(tmp_iter);
		}
	}

	template<typename T> inline
	void prv_load_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
		::boost::serialization::nvp< T > & t,
		char*)
	{
		buf_type buf;
		proxy_type::load(_start, t.name(), buf);
		++_start;
		archive_type ar(buf, base_type::get_flags());
		ar.boost::archive::detail::template common_iarchive<Archive>::load_override(t.value() YGGR_PFTO_VAR(0));
	}

protected:
	inline void skip_override(boost::archive::class_id_type& t YGGR_NON_PFTO_TYPE(int)) // u16 // xml archive don't need check it
	{
		if(_start == _end
			|| std::strcmp(
				_start.get_key_pointer(),
				YGGR_ARCHIVE_GET_INNER_KEY(char*, boost_ar_class_id))) return;
		++_start;
	}

protected:
	buf_citer_type _start;
	buf_citer_type _end;
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#	pragma warning(pop)
#endif // BOOST_MSVC

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // __YGGR_ARCHIVE_BASIC_BSON_IARCHIVE_HPP__
