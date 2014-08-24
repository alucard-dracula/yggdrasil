//xml_oarchive.hpp

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

#ifndef __YGGR_ARCHIVE_XML_OARCHIVE_HPP__
#define __YGGR_ARCHIVE_XML_OARCHIVE_HPP__

#include <boost/archive/xml_oarchive.hpp>

#include <yggr/archive/impl/basic_text_oprimitive.hpp>
#include <yggr/archive/impl/basic_xml_oarchive.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>
#include <yggr/archive/impl/xml_oarchive_impl.hpp>

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
class xml_oarchive_impl
	: public boost::archive::xml_oarchive_impl<Archive>
{
public:
	typedef boost::archive::xml_oarchive_impl<Archive> base_type;
private:
	typedef xml_oarchive_impl this_type;

#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	friend class boost::archive::detail::interface_oarchive<Archive>;
	friend class boost::archive::basic_xml_oarchive<Archive>;
	friend class boost::archive::basic_text_oprimitive<std::ostream>;
	friend class boost::archive::save_access;
protected:
#endif

	xml_oarchive_impl(std::ostream & os, unsigned int flags)
		: base_type(os, flags)
	{
	}

	~xml_oarchive_impl(void)
	{
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


class xml_oarchive 
	: public yggr::archive::xml_oarchive_impl<yggr::archive::xml_oarchive>
{
public:
	typedef std::ostream::char_type char_type;
	typedef std::ostream::traits_type traits_type;

	typedef yggr::archive::xml_oarchive_impl<yggr::archive::xml_oarchive> base_type;
private:
	typedef xml_oarchive this_type;

public:
	xml_oarchive(std::ostream & os, unsigned int flags = 0)
		: base_type(os, flags)
	{
	}

	~xml_oarchive(void)
	{
	}
};

typedef xml_oarchive naked_xml_oarchive;

} // namespace archive
} // namespace yggr

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::xml_oarchive)

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // __YGGR_ARCHIVE_XML_OARCHIVE_HPP__
