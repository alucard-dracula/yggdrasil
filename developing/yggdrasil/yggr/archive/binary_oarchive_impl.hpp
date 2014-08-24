//binary_oarchive_impl.hpp

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

#ifndef __YGGR_ARCHIVE_BINARY_OARCHIVE_IMPL_HPP__
#define __YGGR_ARCHIVE_BINARY_OARCHIVE_IMPL_HPP__

#include <ostream>
#include <boost/config.hpp>
#include <boost/serialization/pfto.hpp>
#include <boost/archive/basic_binary_oprimitive.hpp>
#include <yggr/archive/impl/basic_binary_oprimitive.hpp>
#include <boost/archive/basic_binary_oarchive.hpp>
#include <yggr/archive/impl/basic_binary_oarchive.hpp>
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
class binary_oarchive_impl
	: public boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>,
		public boost::archive::basic_binary_oarchive<Archive>
{
public:
	typedef boost::archive::basic_binary_oprimitive<Archive, Elem, Tr> base_type;
private:
	typedef binary_oarchive_impl this_type;
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

	void init(unsigned int flags)
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

	binary_oarchive_impl(
		std::basic_streambuf<Elem, Tr> & bsb,
		unsigned int flags
	) :
		boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>(
			bsb,
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_oarchive<Archive>(flags)
	{
		init(flags);
	}

	binary_oarchive_impl(
		std::basic_ostream<Elem, Tr> & os,
		unsigned int flags
	) :
		boost::archive::basic_binary_oprimitive<Archive, Elem, Tr>(
			* os.rdbuf(),
			0 != (flags & boost::archive::no_codecvt)
		),
		boost::archive::basic_binary_oarchive<Archive>(flags)
	{
		init(flags);
	}

	template<typename T>
	void save(const T& t)
	{
		base_type::save(t);
	}

	void save(const boost::serialization::collection_size_type& t)
	{
		const std::size_t tmp1 = t;
		ser_size_type tmp = tmp1;
		base_type::save(tmp);
	}

	void save(const yggr::serialization::collection_size_type& t)
	{
		const ser_size_type tmp = t;
		base_type::save(tmp);
	}
};

} // namespace archive
} // namespace boost

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // __YGGR_ARCHIVE_BINARY_OARCHIVE_IMPL_HPP__
