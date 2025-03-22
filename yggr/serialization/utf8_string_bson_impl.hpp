// utf8_string_bson_impl.hpp

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

#ifndef __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP__
//#define __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP__

namespace boost
{
namespace serialization
{

#ifdef YGGR_HAS_BSON_OARCHIVE
#	ifndef __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_OARCHIVE__
#	define __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_OARCHIVE__

template<bool nld, typename Char, typename Traits, typename Alloc> inline
void save(yggr::archive::bson_oarchive<nld>& ar,
					const yggr::charset::utf8_string_impl<Char, Traits, Alloc>& t,
					const unsigned int ver)
{
	archive::save_access::save_primitive(ar, t);
}

#		ifdef YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_OARCHIVE
#			undef YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_OARCHIVE
#		endif // YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_OARCHIVE
#	endif // __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_OARCHIVE__
#else
#	define YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_OARCHIVE
#endif // YGGR_HAS_BSON_OARCHIVE

#ifdef YGGR_HAS_BSON_IARCHIVE
#	ifndef __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_IARCHIVE__
#	define __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_IARCHIVE__

template<bool skip_auto_id, typename Char, typename Traits, typename Alloc> inline
void load(yggr::archive::bson_iarchive<skip_auto_id>& ar,
					yggr::charset::utf8_string_impl<Char, Traits, Alloc>& t,
					const unsigned int ver)
{
	archive::load_access::load_primitive(ar, t);
}

#		ifdef YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_IARCHIVE
#			undef YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_IARCHIVE
#		endif // YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_IARCHIVE
#	endif // __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_IARCHIVE__
#else
#	define YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_IARCHIVE
#endif // YGGR_HAS_BSON_IARCHIVE

} // serialization
} // namespace boost

#if (defined( __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_OARCHIVE__ ) \
		&& defined( __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_IARCHIVE__ ) )

#	define __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP__
#	undef __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_OARCHIVE__
#	undef __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_IARCHIVE__

#endif // defined( __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP_BSON_VALUE_TYPEID__ )

#endif // __YGGR_SERIALIZATION_UTF8_STRING_BSON_IMPL_HPP__