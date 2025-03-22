//yggr serialization utf8_char.hpp

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

#ifndef __YGGR_SERIALIZATION_UTF8_STRING_HPP__
#define __YGGR_SERIALIZATION_UTF8_STRING_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/charset/utf8_char.hpp>

#include <yggr/serialization/string.hpp>

#include <boost/archive/detail/oserializer.hpp>
#include <boost/archive/detail/iserializer.hpp>
#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/split_free.hpp>

namespace boost
{
namespace serialization
{

#if !defined(YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE)

template<typename Archive, typename Char, typename Traits, typename Alloc> inline
void save(Archive& ar,
			const yggr::charset::utf8_char_impl<Char, Traits, Alloc>& t,
			const unsigned int ver)
{
	typedef yggr::charset::utf8_char_impl<Char, Traits, Alloc> char_type;
	typedef typename char_type::base_type base_type;

	const base_type& base = t.org_str();
	archive::save_access::save_primitive(ar, base);
}

template<typename Archive, typename Char, typename Traits, typename Alloc> inline
void load(Archive& ar,
			yggr::charset::utf8_char_impl<Char, Traits, Alloc>& t,
			const unsigned int ver)
{
    typedef yggr::charset::utf8_char_impl<Char, Traits, Alloc> char_type;
	typedef typename char_type::base_type base_type;

	base_type& base = t.org_str();
	archive::load_access::load_primitive(ar, base);
}

#else

template<typename Archive, typename Char, typename Traits, typename Alloc> inline
void save(Archive& ar,
			const yggr::charset::utf8_char_impl<Char, Traits, Alloc>& t,
			const unsigned int ver)
{
	typedef yggr::charset::utf8_char_impl<Char, Traits, Alloc> char_type;
	typedef typename char_type::base_type base_type;
	typedef std::basic_string<Char> store_type;

	const base_type& base = t.org_str();
	store_type tmp(base.begin(), base.end());
	archive::save_access::save_primitive(ar, tmp);
}

template<typename Archive, typename Char, typename Traits, typename Alloc> inline
void load(Archive& ar,
			yggr::charset::utf8_char_impl<Char, Traits, Alloc>& t,
			const unsigned int ver)
{
    typedef yggr::charset::utf8_char_impl<Char, Traits, Alloc> char_type;
	typedef typename char_type::base_type base_type;
	typedef std::basic_string<Char> load_type;

	base_type& base = t.org_str();
	load_type tmp;
	archive::load_access::load_primitive(ar, tmp);
	base.assign(tmp.begin(), tmp.end());
}

#endif // YGGR_STRING_SERIALIZATION_NOT_PRIMITIVE

template<typename Archive, typename Char, typename Traits, typename Alloc> inline
void serialize(Archive& ar,
				yggr::charset::utf8_char_impl<Char, Traits, Alloc> & t,
				const unsigned int ver)
{
    boost::serialization::split_free(ar, t, ver);
}

} // serialization
} // namespace boost

#include <yggr/serialization/detail/container_implementation_level_def.hpp>

YGGR_SERIALIZATION_CONTAINER_IMPLEMENTATION_LEVEL_DEF(3, yggr::charset::utf8_char_impl, object_serializable)

#endif // __YGGR_SERIALIZATION_UTF8_STRING_HPP__

#include <yggr/serialization/utf8_string_bson_impl.hpp>

