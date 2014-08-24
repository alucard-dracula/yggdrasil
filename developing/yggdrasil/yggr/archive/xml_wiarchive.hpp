//xml_wiarchive.hpp

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

#ifndef __YGGR_ARCHIVE_XML_WIARCHIVE_HPP__
#define __YGGR_ARCHIVE_XML_WIARCHIVE_HPP__

#include <boost/archive/xml_wiarchive.hpp>
#include <boost/archive/impl/basic_xml_grammar.hpp>
////#include <yggr/boost_src/serialization/xml_wgrammar.cpp.ipp>

#include <yggr/archive/impl/basic_text_iprimitive.hpp>
#include <yggr/archive/impl/basic_xml_iarchive.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>
#include <yggr/archive/impl/xml_wiarchive_impl.hpp>
#include <yggr/archive/impl/codecvt_null.hpp>
#include <yggr/archive/impl/utf8_codecvt_facet.hpp>

#include <yggr/serialization/collection_size_type.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace yggr 
{ 
namespace archive 
{

template<typename Archive>
class xml_wiarchive_impl 
	: public boost::archive::xml_wiarchive_impl<Archive>
{
public:
	typedef boost::archive::xml_wiarchive_impl<Archive> base_type;

private:
	typedef xml_wiarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	friend class boost::archive::detail::interface_iarchive<Archive>;
	friend class boost::archive::basic_xml_iarchive<Archive>;
	friend class boost::archive::basic_text_iprimitive<std::wistream>;
	friend class boost::archive::load_access;
protected:
#endif
	
	xml_wiarchive_impl(std::wistream & is, unsigned int flags)
		: base_type(is, flags)
	{
	}
	
	~xml_wiarchive_impl(void)
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

class naked_xml_wiarchive 
	: public yggr::archive::xml_wiarchive_impl<yggr::archive::naked_xml_wiarchive>
{
public:
	typedef std::wistream::char_type char_type;
	typedef std::wistream::traits_type traits_type;

	typedef yggr::archive::xml_wiarchive_impl<yggr::archive::naked_xml_wiarchive> base_type;
private:
	typedef naked_xml_wiarchive this_type;

public:
	naked_xml_wiarchive(std::wistream & is, unsigned int flags = 0)
		: base_type(is, flags)
	{
	}

	~naked_xml_wiarchive(void)
	{
	}
};

} // namespace archive
} // namespace yggr

#ifdef BOOST_MSVC
#  pragma warning(pop) 
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

class xml_wiarchive
	: public yggr::archive::xml_wiarchive_impl<yggr::archive::xml_wiarchive>,
		public boost::archive::detail::shared_ptr_helper
{
public:
	typedef std::wistream::char_type char_type;
	typedef std::wistream::traits_type traits_type;

	typedef yggr::archive::xml_wiarchive_impl<yggr::archive::xml_wiarchive> base_type;
private:
	typedef xml_wiarchive this_type;
	
public:
	xml_wiarchive(std::wistream & is, unsigned int flags = 0) 
		: base_type(is, flags)
	{
	}

	~xml_wiarchive(void)
	{
	}
};

} // namespace archive
} // namespace yggr

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::xml_wiarchive)

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // __YGGR_ARCHIVE_XML_WIARCHIVE_HPP__
