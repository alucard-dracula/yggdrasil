// text_woarchive.hpp

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

#ifndef __YGGR_ARCHIVE_TEXT_WOARCHIVE_HPP__
#define __YGGR_ARCHIVE_TEXT_WOARCHIVE_HPP__

#include <boost/archive/text_woarchive.hpp>

#include <yggr/archive/impl/basic_text_oprimitive.hpp>
#include <yggr/archive/impl/basic_text_oarchive.hpp>
#include <yggr/archive/impl/archive_serializer_map.hpp>
#include <yggr/archive/impl/text_woarchive_impl.hpp>
#include <yggr/archive/impl/codecvt_null.hpp>
#include <yggr/archive/impl/utf8_codecvt_facet.hpp>
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

template<typename Archive>
class text_woarchive_impl 
	: public boost::archive::text_woarchive_impl<Archive>
{
public:
	typedef boost::archive::text_woarchive_impl<Archive> base_type;
private:
	typedef text_woarchive_impl this_type;
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	friend class boost::archive::detail::interface_oarchive<Archive>;
	friend class boost::archive::basic_text_oarchive<Archive>;
	friend class boost::archive::basic_text_oprimitive<std::wostream>;
	friend class boost::archive::save_access;
protected:
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
	
	text_woarchive_impl(std::wostream & os, unsigned int flags)
		: base_type(os, flags)
	{
	}

	~text_woarchive_impl(void)
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

class text_woarchive 
	: public yggr::archive::text_woarchive_impl<yggr::archive::text_woarchive>
{
public:
	typedef std::wostream::char_type char_type;
	typedef std::wostream::traits_type traits_type;

	typedef yggr::archive::text_woarchive_impl<yggr::archive::text_woarchive> base_type;
private:
	typedef text_woarchive this_type;
public:
	text_woarchive(std::wostream & os, unsigned int flags = 0)
		: base_type(os, flags)
	{
	}
	
	~text_woarchive()
	{
	}
};

typedef text_woarchive naked_text_woarchive;

} // namespace archive
} // namespace yggr

BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::text_woarchive)

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // __YGGR_ARCHIVE_TEXT_WOARCHIVE_HPP__
