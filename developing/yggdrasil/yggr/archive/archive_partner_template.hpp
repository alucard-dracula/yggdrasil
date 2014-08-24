//archive_partner_template.hpp

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

#ifndef __YGGR_ARVHICE_PARTNER_ARCHIVE_PARTNER_TEMPLATE_HPP__
#define __YGGR_ARVHICE_PARTNER_ARCHIVE_PARTNER_TEMPLATE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/nonable/noncreateable.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif // _MSC_VER

namespace yggr
{
namespace archive
{
namespace archive_partner
{

struct archive_partner_t
{
public:
	enum
	{
		E_Mode_S = 0,
		E_Mode_Network = 0x00000001,
		E_Mode_XML = 0x0000002,
		E_Mode_Text = 0x00000004,
		E_Mode_Binary = 0x00000008,
		E_Mode_Database = 0x00000010,
		E_Mode_Non_Sql_Database = 0x00000020,
		E_compile_u32 = 0xffffffff
	};
};

template<typename Archive,
			typename Archive_Helper,
			const u32 Mode>
class archive_partner_template : private nonable::noncreateable
{
public:
	typedef Archive archive_type;
	typedef Archive_Helper archive_helper_type;

	enum { E_Mode = Mode };

private:

	typedef archive_partner_template this_type;
public:
	static bool is_support_network(void)
	{
		return Mode & archive_partner_t::E_Mode_Network;
	}

	static bool is_support_xml_file(void)
	{
		return Mode & archive_partner_t::E_Mode_XML;
	}

	static bool is_support_text_file(void)
	{
		return Mode & archive_partner_t::E_Mode_Text;
	}

	static bool is_support_binary_file(void)
	{
		return Mode & archive_partner_t::E_Mode_Binary;
	}

	static bool is_support_database(void)
	{
		return Mode & archive_partner_t::E_Mode_Database;
	}

	static bool is_support_non_sql_database(void)
	{
		return Mode & archive_partner_t::E_Mode_Non_Sql_Database;
	}
};

} // namespace archive_partner
} // namespace arvhive
} //namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_ARVHICE_PARTNER_ARCHIVE_PARTNER_TEMPLATE_HPP__
