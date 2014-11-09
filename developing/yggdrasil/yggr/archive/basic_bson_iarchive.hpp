// basic_bson_iarchive.hpp

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

#ifndef __YGGR_ARCHIVE_BASIC_BSON_IARCHIVE_HPP__
#define __YGGR_ARCHIVE_BASIC_BSON_IARCHIVE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <boost/config.hpp>
#include <boost/serialization/pfto.hpp>
#include <boost/detail/workaround.hpp>

#include <boost/archive/detail/common_iarchive.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/item_version_type.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/if.hpp>

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

	typedef basic_bson_iarchive this_type;

protected:
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
public:
#elif defined(BOOST_MSVC)
	// for some inexplicable reason insertion of "class" generates compile erro
	// on msvc 7.1
	friend boost::archive::detail::interface_oarchive<Archive>;
#else
	friend class boost::archive::detail::interface_oarchive<Archive>;
#endif

	basic_bson_iarchive(const buf_type& buf, unsigned int flags)
		: base_type(flags), _start(buf.begin()), _end(buf.end())
	{
		if(archive_type::s_is_skip_auto_id())
		{
			_start.skip_auto_id();
		}
	}

	~basic_bson_iarchive(void)
	{
	}

	void load_override(boost::archive::object_id_type& t, int) //u32 //1
	{
		if(_start == _end) return;
		u32 tmp = 0;
		proxy_type::load(_start, "boost_ar_obj_id", tmp);
		++_start;

		t = boost::archive::object_id_type(tmp);
	}

	void load_override(boost::archive::object_reference_type& t, int n) // based object_id_type //u32 // xml archive don't need check it
	{
		// must be use boost::archive::object_id_type foo
		if(_start == _end) return;

		u32 tmp = 0;
		proxy_type::load(_start, "boost_ar_obj_ref_id", tmp);
		++_start;

		t = boost::archive::object_reference_type(boost::archive::object_id_type(tmp));
	}

	void load_override(boost::archive::version_type& t, int) // u16 //1
	{
		if(_start == _end) return;
		u16 tmp = 0;
		proxy_type::load(_start, "boost_ar_ver", tmp);
		++_start;

		t = boost::archive::version_type(tmp);
	}

	void load_override(boost::archive::library_version_type& t, int)
	{
		if(_start == _end) return;
		u16& tmp = t;
		proxy_type::load(_start, "boost_ar_library_ver", tmp);
		t = boost::serialization::item_version_type(tmp);
	}

	void load_override(boost::archive::class_id_type& t, int) // u16 // xml archive don't need check it
	{
		if(_start == _end) return;
		u16 tmp = 0;
		proxy_type::load(_start, "boost_ar_class_id", tmp);
		//++_start;

		t = boost::archive::class_id_type(tmp);
	}

	void load_override(boost::archive::class_id_optional_type& t, int n) //based class_id_type // u16 //1
	{
		//must be use boost::archive::class_id_type foo
		if(_start == _end) return;
		boost::archive::class_id_type tmp;
		this_type::load_override(tmp, n);
		++_start;
		t = boost::archive::class_id_optional_type(tmp);
	}

	void load_override(boost::archive::class_id_reference_type& t, int n) //based class_id_type // u16
	{
		//must be use boost::archive::class_id_type foo
		if(_start == _end) return;
		boost::archive::class_id_type tmp;
		this_type::load_override(tmp, n);
		++_start;
		t = boost::archive::class_id_reference_type(tmp);
	}

	void load_override(boost::archive::class_name_type& t, int) // const char*
	{
		assert(false);
		if(_start == _end) return;
		++_start; // skip it
	}

	void load_override(boost::archive::tracking_type& t, int) // bool  //1
	{
		if(_start == _end) return;
		bool tmp = t;
		proxy_type::load(_start, "boost_ar_tracking", tmp);
		++_start;
		t = boost::archive::tracking_type(tmp);
	}

	template<typename Obj>
	bool load_begin(buf_citer_type& tmp_iter, const char* name)
	{
		return proxy_type::load_begin<Obj>(tmp_iter, _start, name);
	}

	template<typename Obj>
	bool load_end(const buf_citer_type& tmp_iter)
	{
		return proxy_type::load_end<Obj>(tmp_iter, _start);
	}

	template<class T>
	void load_override(T& t, int)
	{
		BOOST_MPL_ASSERT((boost::mpl::if_<typename nsql_database_system::is_bson_inside_type<T>::type,
											boost::mpl::bool_<false>, boost::mpl::bool_<true> >));
		this->base_type::load_override(t, 0);
	}

	template<class T>
	void load_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif
		boost::serialization::nvp< T > & t,
		int)
	{
		prv_load_override(t, (typename boost::mpl::if_<
                                        typename nsql_database_system::is_bson_inside_type<T>::type,
                                        int*,
                                        char*>::type)0);
	}

protected:
		template<typename T>
	void prv_load_override(
#		ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
			const
#		endif // BOOST_NO_FUNCTION_TEMPLATE_ORDERING
		::boost::serialization::nvp< T > & t,
		int*)
	{
		buf_citer_type tmp_iter;
		if(this->This()->load_begin<T>(tmp_iter, t.name()))
		{
			this->base_type::load_override(t.value(), 0);
			this->This()->load_end<T>(tmp_iter);
		}
	}

	template<typename T>
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
		ar.boost::archive::detail::common_iarchive<Archive>::load_override(t.value(), 0);
	}
protected:
	//const buf_type& _buf;
	buf_citer_type _start;
	buf_citer_type _end;
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif // __YGGR_ARCHIVE_BASIC_BSON_IARCHIVE_HPP__
