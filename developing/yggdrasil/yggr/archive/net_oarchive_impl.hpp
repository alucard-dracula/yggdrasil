//net_oarchive_impl.hpp

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

#ifndef YGGR_ARCHIVE_NET_OARCHIVE_IMPL_HPP
#define YGGR_ARCHIVE_NET_OARCHIVE_IMPL_HPP

// MS compatible compilers support #pragma once
//#if defined(_MSC_VER) && (_MSC_VER >= 1020)
//# pragma once
//#endif


#include <ostream>

#include <boost/config.hpp>
#include <boost/serialization/pfto.hpp>
#include <boost/archive/basic_binary_oprimitive.hpp>
#include <yggr/archive/impl/basic_binary_oprimitive.hpp>
#include <boost/archive/basic_binary_oarchive.hpp>
#include <yggr/archive/impl/basic_binary_oarchive.hpp>
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
class net_oarchive_impl :
	public boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>,
	public boost::archive::basic_binary_oarchive<Archive>
{
private:
	typedef net_oarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	friend class boost::archive::detail::interface_oarchive<Archive>;
	friend class boost::archive::basic_binary_oarchive<Archive>;
	friend class boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>;
	friend class boost::archive::save_access;
protected:
#endif
	// note: the following should not needed - but one compiler (vc 7.1)
	// fails to compile one test (test_shared_ptr) without it !!!
	// make this protected so it can be called from a derived archive

	template<class T>
	void save_override(T & t, BOOST_PFTO int)
	{
		this->boost::archive::basic_binary_oarchive<Archive>::save_override(t, 0L);
	}

	void init(u32 flags)
	{
		if(0 != (flags & boost::archive::no_header))
			return;
		#if ! defined(__MWERKS__)
			this->boost::archive::basic_binary_oarchive<Archive>::init();
			this->boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>::init();
		#else
			boost::archive::basic_binary_oarchive<Archive>::init();
			boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>::init();
		#endif
	}

	net_oarchive_impl(
		std::basic_streambuf<Elem, Tr> & bsb,
		u32 flags
	) :
		boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>(
			bsb,
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_oarchive<Archive>(flags)
	{
		init(flags);
	}

	net_oarchive_impl(
		std::basic_ostream<Elem, Tr> & os,
		u32 flags
	) :
		boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>(
			* os.rdbuf(),
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_oarchive<Archive>(flags)
	{
		init(flags);
	}

	void save(const boost::serialization::collection_size_type& t)
	{
		const std::size_t tmp1 = t;
		ser_size_type tmp = tmp1;
		this_type::save(tmp);
	}

	void save(const yggr::serialization::collection_size_type& t)
	{
		const ser_size_type tmp = t;
		this_type::save(tmp);
	}

	void save(const boost::archive::tracking_type& t)
	{
		//const bool& tmp = t;
		bool tmp = t;
		this_type::save(tmp);
	}

	void save(const boost::archive::library_version_type& t)
	{
		//const u16& tmp = t;
		u16 tmp = t; //gcc must use this style
		this_type::save(tmp);
	}

	void save(const boost::archive::version_type& t)
	{
		//const u32& tmp = t;
		u32 tmp = t;
		this_type::save(tmp);
	}

	void save(const boost::archive::class_id_type& t)
	{
		const u16& tmp = t;
		this_type::save(tmp);
	}

	void save(const boost::archive::object_id_type& t)
	{
		//const u32& tmp = t;
		u32 tmp = t;
		this_type::save(tmp);
	}

	//void save(const boost::serialization::item_version_type& t)
	//{
	//	const u32& tmp = t;
	//	this_type::save_value(t);
	//}

	//void save(const boost::archive::class_name_type& t); // dont't need it
	//void save(const boost::archive::class_id_optional_type& t); //binary don't need it

	//note:add base type save S
	void save(const std::string& t)
	{
		ser_size_type size = static_cast<u16>(t.size());
		this_type::save(size);
		this->save_binary(t.data(), sizeof(char) * size);
	}

	void save(const std::wstring& t)
	{
		std::wstring str(t);
		ser_size_type size = str.size();

		for(u16 i = 0; i < size; ++i)
		{
			str[i] = yggr::network::hton(str[i]);
		}
		this_type::save(size);
		this->save_binary(str.data(), sizeof(wchar_t) * size);
	}

	//template<template<typename _Char> class Traits,
	//			typename Alloc>
	//void save(const std::basic_string<char, Traits<char>, Alloc>& t)
	//{
	//	ser_size_type size = static_cast<u16>(t.size());
	//	this_type::save(size);
	//	this->save_binary(t.data(), sizeof(char) * size);
	//}

	//template<template<typename _Char> class Traits,
	//			typename Alloc>
	//void save(const std::basic_string<wchar_t, Traits<wchar_t>, Alloc >& t)
	//{
	//	std::wstring str(t.begin(), t.end());
	//	ser_size_type size = str.size();

	//	for(u16 i = 0; i < size; ++i)
	//	{
	//		str[i] = yggr::network::hton(str[i]);
	//	}
	//	this_type::save(size);
	//	this->save_binary(str.data(), sizeof(wchar_t) * size);
	//}

	void save(bool t)
	{
		this->save_binary(&t, sizeof(bool));
	}

	void save(s8 t)
	{
		this->save_binary(&t, sizeof(s8));
	}

	void save(char t)
	{
		this->save_binary(&t, sizeof(char));
	}

	void save(u8 t)
	{
		this->save_binary(&t, sizeof(u8));
	}

	void save(wchar_t t)
	{
		t = yggr::network::hton(t);
		this->save_binary(&t, sizeof(wchar_t));
	}

	void save(s16 t)
	{
		t = yggr::network::hton(t);
		this->save_binary(&t, sizeof(s16));
	}

	void save(u16 t)
	{
		t = yggr::network::hton(t);
		this->save_binary(&t, sizeof(u16));
	}

	void save(s32 t)
	{
		t = yggr::network::hton(t);
		this->save_binary(&t, sizeof(s32));
	}

	void save(u32 t)
	{
		t = yggr::network::hton(t);
		this->save_binary(&t, sizeof(u32));
	}

	void save(s64 t)
	{
		t = yggr::network::hton(t);
		this->save_binary(&t, sizeof(s64));
	}

	void save(u64 t)
	{
		t = yggr::network::hton(t);
		this->save_binary(&t, sizeof(u64));
	}

	void save(f32 t)
	{
		t = yggr::network::hton(t);
		this->save_binary(&t, sizeof(f32));
	}

	void save(f64 t)
	{
		t = yggr::network::hton(t);
		this->save_binary(&t, sizeof(f64));
	}

	//note:add base type save E

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

#endif // YGGR_ARCHIVE_NET_OARCHIVE_IMPL_HPP
