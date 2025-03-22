// binary_wiarchive.hpp

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

#ifndef __YGGR_ARCHIVE_BINARY_WIARCHIVE_HPP__
#define __YGGR_ARCHIVE_BINARY_WIARCHIVE_HPP__

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#ifdef BOOST_NO_STD_WSTREAMBUF
#error "wide char i/o not supported on this platform"
#else

#include <boost/archive/detail/auto_link_warchive.hpp> // if condecvt_null at this befro link unsymbol
#include <yggr/archive/binary_iarchive_impl.hpp>
#include <yggr/archive/impl/codecvt_null.hpp>
#include <yggr/archive/impl/utf8_codecvt_facet.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>

#include <istream> // wistream

namespace yggr 
{ 
namespace archive 
{

// same as binary_wiarchive below - without the shared_ptr_helper
class naked_binary_wiarchive 
	: public binary_iarchive_impl<
				yggr::archive::naked_binary_wiarchive, 
				std::wistream::char_type, 
				std::wistream::traits_type
			>
{
public:
	typedef std::wistream::char_type char_type;
	typedef std::wistream::traits_type traits_type;

	typedef binary_iarchive_impl<
				yggr::archive::naked_binary_wiarchive, 
				std::wistream::char_type, 
				std::wistream::traits_type
			> base_type;
private:
	typedef naked_binary_wiarchive this_type;

public:
	naked_binary_wiarchive(std::wistream & is, unsigned int flags = 0) 
		: base_type(is, flags)
	{
	}

	naked_binary_wiarchive(std::wstreambuf & bsb, unsigned int flags = 0) 
		: base_type(bsb, flags)
	{
	}
};

} // namespace archive
} // namespace yggr

#if BOOST_VERSION < 105600
#	include <boost/archive/shared_ptr_helper.hpp>
#endif // BOOST_VERSION < 105600

namespace yggr
{ 
namespace archive 
{

class binary_wiarchive 
	: public binary_iarchive_impl<
				yggr::archive::binary_wiarchive,
				std::wistream::char_type, 
				std::wistream::traits_type
			>
{
public:
	typedef std::wistream::char_type char_type;
	typedef std::wistream::traits_type traits_type;

	typedef binary_iarchive_impl<
				yggr::archive::binary_wiarchive,
				std::wistream::char_type, 
				std::wistream::traits_type
			> base_type;
private:
	typedef binary_wiarchive this_type;

public:
	binary_wiarchive(std::wistream & is, unsigned int flags = 0) 
		: base_type(is, flags)
	{
	}

	binary_wiarchive(std::wstreambuf & bsb, unsigned int flags = 0)
		: base_type(bsb, flags)
	{
	}
};

} // namespace archive
} // namespace yggr

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::binary_wiarchive)

// boost::archive::detail::load_non_pointer_type patch
#include <yggr/archive/detail/iserializer_patch.hpp>

#define YGGR_PP_ARCHIVE_TYPE() ::yggr::archive::naked_binary_wiarchive
	YGGR_PP_BOOST_ISERIALIZER_PATCH(YGGR_PP_ARCHIVE_TYPE)
#undef YGGR_PP_ARCHIVE_TYPE

#define YGGR_PP_ARCHIVE_TYPE() ::yggr::archive::binary_wiarchive
	YGGR_PP_BOOST_ISERIALIZER_PATCH(YGGR_PP_ARCHIVE_TYPE)
#undef YGGR_PP_ARCHIVE_TYPE

#endif // BOOST_NO_STD_WSTREAMBUF
#endif // __YGGR_ARCHIVE_BINARY_WIARCHIVE_HPP__
