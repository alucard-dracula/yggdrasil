//bson_archive_partner.hpp

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

#ifndef __YGGR_ARCHIVE_ARCHIVE_PARTNER_BSON_ARCHIVE_PARTNER_HPP__
#define __YGGR_ARCHIVE_ARCHIVE_PARTNER_BSON_ARCHIVE_PARTNER_HPP__

#include <yggr/archive/archive_partner_template.hpp>
#include <yggr/archive/bson_archive_helper.hpp>
#include <yggr/archive/bson_iarchive.hpp>
#include <yggr/archive/bson_oarchive.hpp>

namespace yggr
{
namespace archive
{
namespace archive_partner
{

#ifndef YGGR_TPL_CONFIG_USING_TPL 

YGGR_PP_ARCHIVE_PARTENER_TEMPLATE_DECL(
	bson_iarchive_partner,
	archive::bson_iarchive<true>,
	archive_helper::bson_archive_helper< archive::bson_iarchive<true> >,
	archive_partner_t::E_Mode_Bson );

YGGR_PP_ARCHIVE_PARTENER_TEMPLATE_DECL(
	bson_conv_iarchive_partner,
	archive::bson_iarchive<false>,
	archive_helper::bson_archive_helper< archive::bson_iarchive<false> >,
	archive_partner_t::E_Mode_Bson );

YGGR_PP_ARCHIVE_PARTENER_TEMPLATE_DECL(
	bson_oarchive_partner,
	archive::bson_oarchive<false>,
	archive_helper::bson_archive_helper< archive::bson_oarchive<false> >,
	archive_partner_t::E_Mode_Bson );

YGGR_PP_ARCHIVE_PARTENER_TEMPLATE_DECL(
	bson_nld_oarchive_partner,
	archive::bson_oarchive<true>,
	archive_helper::bson_archive_helper< archive::bson_oarchive<true> >,
	archive_partner_t::E_Mode_Bson );

#else

typedef 
	archive_partner_template
	<
		archive::bson_iarchive<true>,
		archive_helper::bson_archive_helper< archive::bson_iarchive<true> >,
		archive_partner_t::E_Mode_Bson
	> bson_iarchive_partner;

typedef 
	archive_partner_template
	<	
		archive::bson_iarchive<false>,
		archive_helper::bson_archive_helper< archive::bson_iarchive<false> >,
		archive_partner_t::E_Mode_Bson
	> bson_conv_iarchive_partner;

typedef 
	archive_partner_template
	<	
		archive::bson_oarchive<false>,
		archive_helper::bson_archive_helper< archive::bson_oarchive<false> >,
		archive_partner_t::E_Mode_Bson
	> bson_oarchive_partner;

typedef 
	archive_partner_template
	<	
		archive::bson_oarchive<true>,
		archive_helper::bson_archive_helper< archive::bson_oarchive<true> >,
		archive_partner_t::E_Mode_Bson
	> bson_nld_oarchive_partner;

#endif // YGGR_TPL_CONFIG_USING_TPL

typedef bson_iarchive_partner bson_io_iarchive_partner;
typedef bson_oarchive_partner bson_io_oarchive_partner;
typedef bson_oarchive_partner bson_conv_oarchive_partner;

	
} // namespace archive_partener
} // namespace archive
} // namespace yggr

#endif //__YGGR_ARCHIVE_ARCHIVE_PARTNER_BSON_ARCHIVE_PARTNER_HPP__
