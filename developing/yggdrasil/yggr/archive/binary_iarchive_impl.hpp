//binary_iarchive_impl.hpp

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

#ifndef __YGGR_BOOST_ARCHIVE_BINARY_IARCHIVE_IMPL_HPP__
#define __YGGR_BOOST_ARCHIVE_BINARY_IARCHIVE_IMPL_HPP__

#include <istream>
#include <boost/serialization/pfto.hpp>
#include <boost/archive/basic_binary_iprimitive.hpp>
#include <yggr/archive/impl/basic_binary_iprimitive.hpp>
#include <boost/archive/basic_binary_iarchive.hpp>
#include <yggr/archive/impl/basic_binary_iarchive.hpp>
#include <yggr/serialization/collection_size_type.hpp>

#include <yggr/archive/impl/archive_serializer_map.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace yggr
{
namespace archive
{

template<class Archive, class Elem, class Tr>
class binary_iarchive_impl
	: public boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>,
		public boost::archive::basic_binary_iarchive<Archive>
{
public:
	typedef boost::archive::basic_binary_iprimitive<Archive, Elem, Tr> base_type;
	typedef boost::archive::basic_binary_iarchive<Archive> base2_type;
private:
	typedef binary_iarchive_impl this_type;

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
		//this->basic_binary_iarchive<Archive>::load_override(t, 0L);
		base2_type::load_override(t, 0L);
	}

	void init(unsigned int flags)
	{
		if(0 != (flags & boost::archive::no_header))
			return;
		#if ! defined(__MWERKS__)
			this->boost::archive::basic_binary_iarchive<Archive>::init();
			this->boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>::init();
		#else
			boost::archive::basic_binary_iarchive<Archive>::init();
			boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>::init();
		#endif
	}

	binary_iarchive_impl(
		std::basic_streambuf<Elem, Tr> & bsb,
		unsigned int flags
	) :
		boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>(
			bsb,
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_iarchive<Archive>(flags)
	{
		init(flags);
	}
	binary_iarchive_impl(
		std::basic_istream<Elem, Tr> & is,
		unsigned int flags
	) :
		boost::archive::basic_binary_iprimitive<Archive, Elem, Tr>(
			* is.rdbuf(),
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_iarchive<Archive>(flags)
	{
		init(flags);
	}

	template<typename T>
	void load(T& t)
	{
		base_type::load(t);
	}

	void load(boost::serialization::collection_size_type& t)
	{
		//std::size_t tmp = 0;
		ser_size_type tmp = 0;
		base_type::load(tmp);
		t = boost::serialization::collection_size_type(tmp);
	}

	void load(yggr::serialization::collection_size_type& t)
	{
		ser_size_type tmp = 0;
		base_type::load(tmp);
		t = yggr::serialization::collection_size_type(tmp);
	}

//	inline this_type* This(void)
//	{
//		return this;
//	}
//
//	inline const this_type* This(void) const
//	{
//		return this;
//	}
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // __YGGR_BOOST_ARCHIVE_BINARY_IARCHIVE_IMPL_HPP__
