//net_iarchive_impl.hpp
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

#ifndef __YGGR_ARCHIVE_NET_IARCHIVE_IMPL_HPP__
#define __YGGR_ARCHIVE_NET_IARCHIVE_IMPL_HPP__

#include <istream>
#include <boost/config.hpp>
#include <boost/serialization/pfto.hpp>
#include <boost/archive/basic_binary_iprimitive.hpp>
#include <yggr/archive/impl/basic_binary_iprimitive.hpp>
#include <boost/archive/basic_binary_iarchive.hpp>
#include <yggr/archive/impl/basic_binary_iarchive.hpp>
#include <yggr/serialization/collection_size_type.hpp>
#include <boost/asio.hpp>

#include <yggr/archive/impl/archive_serializer_map.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/network/hn_conv.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace yggr 
{
namespace archive
{

template<typename Archive, typename Elem, typename Tr>
class net_iarchive_impl :
	public boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>,
	public boost::archive::basic_binary_iarchive<Archive>
{
private:
	typedef net_iarchive_impl this_type;
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	friend class boost::archive::detail::interface_iarchive<Archive>;
	friend class boost::archive::basic_binary_iarchive<Archive>;
	friend class boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>;
	friend class boost::archive::load_access;
protected:
#endif
	// note: the following should not needed - but one compiler (vc 7.1)
	// fails to compile one test (test_shared_ptr) without it !!!
	// make this protected so it can be called from a derived archive

	template<class T>
	void load_override(T & t, BOOST_PFTO int)
	{
		this->boost::archive::basic_binary_iarchive<Archive>::load_override(t, 0L);
	}

	void init(u32 flags)
	{
		if(0 != (flags & boost::archive::no_header))
			return;
		#if ! defined(__MWERKS__)
			this->boost::archive::basic_binary_iarchive<Archive>::init();
			this->boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>::init();
		#else
			boost::basic_binary_iarchive<Archive>::init();
			boost::basic_binary_iprimitive<Archive, Elem, Tr>::init();
		#endif
	}

	net_iarchive_impl(
		std::basic_streambuf<Elem, Tr> & bsb,
		u32 flags
	) :
		boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>(
			bsb,
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_iarchive<Archive>(flags)
	{
		init(flags);
	}

	net_iarchive_impl(
		std::basic_istream<Elem, Tr> & is,
		u32 flags
	) :
		boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>(
			* is.rdbuf(),
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_iarchive<Archive>(flags)
	{
		init(flags);
	}

	void load(boost::serialization::collection_size_type& t)
	{
		//std::size_t tmp = 0;
		ser_size_type tmp = 0;
		this_type::load(tmp);
		t = boost::serialization::collection_size_type(tmp);
	}

	void load(yggr::serialization::collection_size_type& t)
	{
		ser_size_type tmp = 0;
		this_type::load(tmp);
		t = yggr::serialization::collection_size_type(tmp);
	}

	void load(boost::archive::tracking_type& t)
	{
		bool tmp = false;
		this_type::load(tmp);
		t = boost::archive::tracking_type(tmp);
	}

	void load(boost::archive::library_version_type& t)
	{
		u16 tmp = 0;
		this_type::load(tmp);
		t = boost::archive::library_version_type(tmp);
	}

	void load(boost::archive::version_type& t)
	{
		u32 tmp = 0;
		this_type::load(tmp);
		t = boost::archive::version_type(tmp);
	}

	void load(boost::archive::class_id_type& t)
	{
		u16 tmp = 0;
		this_type::load(tmp);
		t = boost::archive::class_id_type(tmp);
	}

	void load(boost::archive::object_id_type& t)
	{
		u32 tmp = t;
		this_type::load(tmp);
		t = boost::archive::object_id_type(tmp);
	}

	//void load(boost::serialization::item_version_type& t)
	//{
	//	u32 tmp = 0;
	//	this_type::load(tmp);
	//	t = boost::archive::item_version_type(tmp);
	//}

	//void load(boost::archive::class_name_type& t); // dont't need it
	//void load(boost::archive::class_id_optional_type& t); //binary don't need it

	//note: add base type load S

	void load(std::string& t)
	{
		ser_size_type size = 0;
		this_type::load(size);
		t.resize(size);
		this->load_binary((void*)t.data(), sizeof(char) * size);
	}

	void load(std::wstring& t)
	{
		ser_size_type size = 0;
		this_type::load(size);
		t.resize(size);
		this->load_binary((void*)t.data(), sizeof(wchar_t) * size);

		for(u16 i = 0; i < size; ++i)
		{
			t[i] = yggr::network::ntoh(t[i]);
		}
	}

	//template<template<typename _Char> class Traits, 
	//			typename Alloc>
	//void load(std::basic_string<char, Traits<char>, Alloc>& t)
	//{
	//	ser_size_type size = 0;
	//	this_type::load(size);
	//	t.resize(size);
	//	this->load_binary((void*)t.data(), sizeof(char) * size);
	//}

	//template<template<typename _Char> class Traits,
	//			typename Alloc>
	//void load(std::basic_string<wchar_t, Traits<wchar_t>, Alloc >& t)
	//{
	//	ser_size_type size = 0;
	//	this_type::load(size);
	//	t.resize(size);
	//	this->load_binary((void*)t.data(), sizeof(wchar_t) * size);

	//	for(u16 i = 0; i < size; ++i)
	//	{
	//		t[i] = yggr::network::ntoh(t[i]);
	//	}
	//}

	void load(bool& t)
	{
		this->load_binary(&t, sizeof(bool));
	}

	void load(s8& t)
	{
		this->load_binary(&t, sizeof(s8));
	}

	void load(char& t)
	{
		this->load_binary(&t, sizeof(char));
	}

	void load(u8& t)
	{
		this->load_binary(&t, sizeof(u8));
	}

	void load(wchar_t& t)
	{
		this->load_binary(&t, sizeof(wchar_t));
		t = yggr::network::ntoh(t);
	}

	void load(s16& t)
	{
		this->load_binary(&t, sizeof(s16));
		t = yggr::network::ntoh(t);
	}

	void load(u16& t)
	{
		this->load_binary(&t, sizeof(u16));
		t = yggr::network::ntoh(t);
	}

	void load(s32& t)
	{
		this->load_binary(&t, sizeof(s32));
		t = yggr::network::ntoh(t);
	}

	void load(u32& t)
	{
		this->load_binary(&t, sizeof(u32));
		t = yggr::network::ntoh(t);
	}

	void load(s64& t)
	{
		this->load_binary(&t, sizeof(s64));
		t = yggr::network::ntoh(t);
	}

	void load(u64& t)
	{
		this->load_binary(&t, sizeof(u64));
		t = yggr::network::ntoh(t);
	}

	void load(f32& t)
	{
		this->load_binary(&t, sizeof(f32));
		t = yggr::network::ntoh(t);
	}

	void load(f64& t)
	{
		this->load_binary(&t, sizeof(f64));
		t = yggr::network::ntoh(t);
	}
	//note: add base type load E

public:
	struct use_array_optimization 
	{
		template <typename T>  
		#if defined(BOOST_NO_DEPENDENT_NESTED_DERIVATIONS)  
			struct apply 
			{  
				typedef boost::mpl::bool_<sizeof(T) == 1> type;  
			};
		#else
			struct apply : public boost::mpl::bool_<sizeof(T) == 1> {};  
		#endif
	};
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // __YGGR_ARCHIVE_NET_IARCHIVE_IMPL_HPP__
