//text_iarchive.hpp

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

#ifndef __YGGR_ARCHIVE_TEXT_IARCHIVE_HPP__
#define __YGGR_ARCHIVE_TEXT_IARCHIVE_HPP__

#include <boost/archive/text_iarchive.hpp>

#include <yggr/archive/impl/basic_text_iprimitive.hpp>
#include <yggr/archive/impl/basic_text_iarchive.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>
#include <yggr/archive/impl/text_iarchive_impl.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>

#include <yggr/serialization/collection_size_type.hpp>


#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace yggr 
{ 
namespace archive 
{

template<class Archive>
class text_iarchive_impl
	: public boost::archive::text_iarchive_impl<Archive>
{
public:
	typedef boost::archive::text_iarchive_impl<Archive> base_type;
private:
	typedef text_iarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	friend class boost::archive::detail::interface_iarchive<Archive>;
	friend class boost::archive::basic_text_iarchive<Archive>;
	friend class boost::archive::basic_text_iprimitive<std::istream>;
	friend class boost::archive::load_access;
protected:
#endif

	text_iarchive_impl(std::istream & is, unsigned int flags)
		: base_type(is, flags)
	{
	}

	~text_iarchive_impl(void)
	{
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
};

class naked_text_iarchive 
	: public yggr::archive::text_iarchive_impl<yggr::archive::naked_text_iarchive>
{
public:
	typedef std::istream::char_type char_type;
	typedef std::istream::traits_type traits_type;
	
	typedef yggr::archive::text_iarchive_impl<yggr::archive::naked_text_iarchive> base_type;
private:
	typedef naked_text_iarchive this_type;
public:
	naked_text_iarchive(std::istream & is_, unsigned int flags = 0) 
		: base_type(is_, flags)
	{
	}

	~naked_text_iarchive(void)
	{
	}
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif


#include <boost/archive/shared_ptr_helper.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace yggr 
{ 
namespace archive 
{

class text_iarchive 
	: public yggr::archive::text_iarchive_impl<yggr::archive::text_iarchive>,
		public boost::archive::detail::shared_ptr_helper
{
public:
	typedef std::istream::char_type char_type;
	typedef std::istream::traits_type traits_type;
	
	typedef yggr::archive::text_iarchive_impl<yggr::archive::text_iarchive> base_type;
private:
	typedef text_iarchive this_type;

public:
	text_iarchive(std::istream & is_, unsigned int flags = 0)
		: base_type(is_, flags)
	{
	}

	~text_iarchive(void)
	{
	}
};

} // namespace archive
} // namespace yggr

BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::text_iarchive)

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // __YGGR_ARCHIVE_TEXT_IARCHIVE_HPP__
