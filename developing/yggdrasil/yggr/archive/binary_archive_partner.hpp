//binary_archive_partner.hpp

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

#ifndef __YGGR_ARCHIVE_ARCHIVE_PARTNER_BINARY_ARCHIVE_PARTNER_HPP__
#define __YGGR_ARCHIVE_ARCHIVE_PARTNER_BINARY_ARCHIVE_PARTNER_HPP__

#include <yggr/archive/archive_partner_template.hpp>
#include <yggr/archive/string_archive_helper.hpp>
#include <yggr/archive/binary_iarchive.hpp>
#include <yggr/archive/binary_oarchive.hpp>

#include <yggr/archive/binary_wiarchive.hpp>
#include <yggr/archive/binary_woarchive.hpp>

namespace yggr
{
namespace archive
{
namespace archive_partner
{

	typedef archive_partner_template<	yggr::archive::binary_iarchive,
										archive_helper::string_archive_helper<yggr::archive::binary_iarchive>,
										archive_partner_t::E_Mode_Binary
									> binary_iarchive_partner;

	typedef archive_partner_template<	yggr::archive::binary_oarchive,
										archive_helper::string_archive_helper<yggr::archive::binary_oarchive>,
										archive_partner_t::E_Mode_Binary
									> binary_oarchive_partner;

	typedef archive_partner_template<	yggr::archive::binary_wiarchive,
										archive_helper::string_archive_helper<yggr::archive::binary_wiarchive>,
										archive_partner_t::E_Mode_Binary
									> binary_wiarchive_partner;

	typedef archive_partner_template<	yggr::archive::binary_woarchive,
										archive_helper::string_archive_helper<yggr::archive::binary_woarchive>,
										archive_partner_t::E_Mode_Binary
									> binary_woarchive_partner;

} // namespace archive_partner
} // namespace archive
} // namespace yggr

#endif //__YGGR_ARCHIVE_ARCHIVE_PARTNER_BINARY_ARCHIVE_PARTNER_HPP__
