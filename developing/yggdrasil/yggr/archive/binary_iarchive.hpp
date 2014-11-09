//binary_iarchive.hpp

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

#ifndef __YGGR_ARCHIVE_BINARY_IARCHIVE_HPP__
#define __YGGR_ARCHIVE_BINARY_IARCHIVE_HPP__

#include <istream>
#include <yggr/archive/binary_iarchive_impl.hpp>
#include <yggr/archive/archive_data_wrap_traits.hpp>
#include <boost/archive/detail/register_archive.hpp>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4511 4512)
#endif

namespace yggr 
{ 
namespace archive 
{

// same as binary_iarchive below - without the shared_ptr_helper
class naked_binary_iarchive 
	: public binary_iarchive_impl<
				yggr::archive::naked_binary_iarchive, 
				std::istream::char_type, 
				std::istream::traits_type
			>
{

public:
	typedef std::ostream::char_type char_type;
	typedef std::ostream::traits_type traits_type;

	typedef binary_iarchive_impl<
				yggr::archive::naked_binary_iarchive, 
				std::istream::char_type, 
				std::istream::traits_type
			> base_type;
private:
	typedef naked_binary_iarchive this_type;
public:
	naked_binary_iarchive(std::istream & is, unsigned int flags = 0)
		: base_type(is, flags)
	{}
	naked_binary_iarchive(std::streambuf & bsb, unsigned int flags = 0) 
		: base_type(bsb, flags)
	{}
};

} // namespace archive
} // namespace yggr

// note special treatment of shared_ptr. This type needs a special
// structure associated with every archive.  We created a "mix-in"
// class to provide this functionality.  Since shared_ptr holds a
// special esteem in the boost library - we included it here by default.
#include <boost/archive/shared_ptr_helper.hpp>

namespace yggr
{ 
namespace archive
{

class binary_iarchive 
	: public binary_iarchive_impl<
				yggr::archive::binary_iarchive, 
				std::istream::char_type, 
				std::istream::traits_type
			>,
		public boost::archive::detail::shared_ptr_helper
{
public:
	typedef std::ostream::char_type char_type;
	typedef std::ostream::traits_type traits_type;

	typedef binary_iarchive_impl<
				yggr::archive::binary_iarchive, 
				std::istream::char_type, 
				std::istream::traits_type
			> base_type;
private:
	typedef binary_iarchive this_type;
public:
	binary_iarchive(std::istream & is, unsigned int flags = 0)
		: base_type(is, flags)
	{}
	binary_iarchive(std::streambuf & bsb, unsigned int flags = 0) 
		: base_type(bsb, flags)
	{}
};

} // namespace archive
} // namespace boost

// required by export
BOOST_SERIALIZATION_REGISTER_ARCHIVE(yggr::archive::binary_iarchive)
BOOST_SERIALIZATION_USE_ARRAY_OPTIMIZATION(yggr::archive::binary_iarchive)

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // __YGGR_ARCHIVE_BINARY_IARCHIVE_HPP__
