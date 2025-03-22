//text_archive_partner.hpp

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

#ifndef __YGGR_ARCHIVE_ARCHIVE_PARTNER_TEXT_ARCHIVE_PARTNER_HPP__
#define __YGGR_ARCHIVE_ARCHIVE_PARTNER_TEXT_ARCHIVE_PARTNER_HPP__

#include <yggr/archive/archive_partner_template.hpp>
#include <yggr/archive/string_archive_helper.hpp>

#include <yggr/archive/text_iarchive.hpp>
#include <yggr/archive/text_oarchive.hpp>

#include <yggr/archive/text_wiarchive.hpp>
#include <yggr/archive/text_woarchive.hpp>

namespace yggr
{
namespace archive
{
namespace archive_partner
{
	
#ifndef YGGR_TPL_CONFIG_USING_TPL 

YGGR_PP_ARCHIVE_PARTENER_TEMPLATE_DECL(
	text_iarchive_partner,
	yggr::archive::text_iarchive, 
	archive_helper::string_archive_helper<yggr::archive::text_iarchive>, 
	archive_partner_t::E_Mode_Text );

YGGR_PP_ARCHIVE_PARTENER_TEMPLATE_DECL(
	text_oarchive_partner,
	yggr::archive::text_oarchive,
	archive_helper::string_archive_helper<yggr::archive::text_oarchive>, 
	archive_partner_t::E_Mode_Text );

YGGR_PP_ARCHIVE_PARTENER_TEMPLATE_DECL(
	text_wiarchive_partner,
	yggr::archive::text_wiarchive, 
	archive_helper::string_archive_helper<yggr::archive::text_wiarchive>, 
	archive_partner_t::E_Mode_Text );

YGGR_PP_ARCHIVE_PARTENER_TEMPLATE_DECL(
	text_woarchive_partner,
	yggr::archive::text_woarchive,
	archive_helper::string_archive_helper<yggr::archive::text_woarchive>, 
	archive_partner_t::E_Mode_Text );

#else

typedef 
	archive_partner_template
	<	
		yggr::archive::text_iarchive, 
		archive_helper::string_archive_helper<yggr::archive::text_iarchive>, 
		archive_partner_t::E_Mode_Text
	> text_iarchive_partner;

typedef 
	archive_partner_template
	<	
		yggr::archive::text_oarchive,
		archive_helper::string_archive_helper<yggr::archive::text_oarchive>, 
		archive_partner_t::E_Mode_Text
	> text_oarchive_partner;

typedef 
	archive_partner_template
	<	
		yggr::archive::text_wiarchive, 
		archive_helper::string_archive_helper<yggr::archive::text_wiarchive>, 
		archive_partner_t::E_Mode_Text
	> text_wiarchive_partner;

typedef 
	archive_partner_template
	<	
		yggr::archive::text_woarchive,
		archive_helper::string_archive_helper<yggr::archive::text_woarchive>, 
		archive_partner_t::E_Mode_Text
	> text_woarchive_partner;

#endif // YGGR_TPL_CONFIG_USING_TPL

} // namespace archive_partner
} // namespace archive
} // namespace yggr

#endif //__YGGR_ARCHIVE_ARCHIVE_PARTNER_TEXT_ARCHIVE_PARTNER_HPP__
