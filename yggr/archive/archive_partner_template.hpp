//archive_partner_template.hpp

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

#ifndef __YGGR_ARVHICE_PARTNER_ARCHIVE_PARTNER_TEMPLATE_HPP__
#define __YGGR_ARVHICE_PARTNER_ARCHIVE_PARTNER_TEMPLATE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
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
	YGGR_STATIC_CONSTANT(u32, E_Mode_Start = 0x00000000);

	YGGR_STATIC_CONSTANT(u32, E_Mode_Network = 0x00000001);
	YGGR_STATIC_CONSTANT(u32, E_Mode_XML = 0x00000002);
	YGGR_STATIC_CONSTANT(u32, E_Mode_Text = 0x00000004);
	YGGR_STATIC_CONSTANT(u32, E_Mode_Binary = 0x00000008);
	YGGR_STATIC_CONSTANT(u32, E_Mode_Sql = 0x00000010); // now not support it
	YGGR_STATIC_CONSTANT(u32, E_Mode_Bson = 0x00000020);
	
	YGGR_STATIC_CONSTANT(u32, E_Mode_End = 0xffffffff);
};

#define YGGR_PP_ARCHIVE_PARTENER_TEMPLATE_DECL(__name__, Archive, Archive_Helper, Mode) \
	class __name__  : private nonable::noncreateable { \
	public: \
		typedef Archive archive_type; \
		typedef Archive_Helper archive_helper_type; \
		\
		YGGR_STATIC_CONSTANT(u32, E_Mode = Mode); \
		\
	private: \
		typedef __name__ this_type; \
		\
	public: \
		YGGR_CONSTEXPR_OR_INLINE static bool is_support_network(void) YGGR_NOEXCEPT_OR_NOTHROW { \
			return !!(E_Mode & archive_partner_t::E_Mode_Network); } \
		\
		YGGR_CONSTEXPR_OR_INLINE static bool is_support_xml_file(void) YGGR_NOEXCEPT_OR_NOTHROW { \
			return !!(E_Mode & archive_partner_t::E_Mode_XML); } \
		\
		YGGR_CONSTEXPR_OR_INLINE static bool is_support_text_file(void) YGGR_NOEXCEPT_OR_NOTHROW { \
			return !!(E_Mode & archive_partner_t::E_Mode_Text); } \
		\
		YGGR_CONSTEXPR_OR_INLINE static bool is_support_binary_file(void) YGGR_NOEXCEPT_OR_NOTHROW { \
			return !!(E_Mode & archive_partner_t::E_Mode_Binary); } \
		\
		YGGR_CONSTEXPR_OR_INLINE static bool is_support_sql(void) YGGR_NOEXCEPT_OR_NOTHROW { \
			return !!(E_Mode & archive_partner_t::E_Mode_Sql); } \
		\
		YGGR_CONSTEXPR_OR_INLINE static bool is_support_bson(void) YGGR_NOEXCEPT_OR_NOTHROW { \
			return !!(Mode & archive_partner_t::E_Mode_Bson); } \
		\
		YGGR_CONSTEXPR_OR_INLINE static u32 mode(void) YGGR_NOEXCEPT_OR_NOTHROW { return E_Mode; } };

template<typename Archive, typename Archive_Helper, u32 Mode>
class archive_partner_template 
	: private nonable::noncreateable
{
public:
	typedef Archive archive_type;
	typedef Archive_Helper archive_helper_type;

	YGGR_STATIC_CONSTANT(u32, E_Mode = Mode);

private:
	typedef archive_partner_template this_type;

public:
	YGGR_CONSTEXPR_OR_INLINE static bool is_support_network(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return !!(E_Mode & archive_partner_t::E_Mode_Network);
	}

	YGGR_CONSTEXPR_OR_INLINE static bool is_support_xml_file(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return !!(E_Mode & archive_partner_t::E_Mode_XML);
	}

	YGGR_CONSTEXPR_OR_INLINE static bool is_support_text_file(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return !!(E_Mode & archive_partner_t::E_Mode_Text);
	}

	YGGR_CONSTEXPR_OR_INLINE static bool is_support_binary_file(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return !!(E_Mode & archive_partner_t::E_Mode_Binary);
	}

	YGGR_CONSTEXPR_OR_INLINE static bool is_support_sql(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return !!(E_Mode & archive_partner_t::E_Mode_Sql);
	}

	YGGR_CONSTEXPR_OR_INLINE static bool is_support_bson(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return !!(Mode & archive_partner_t::E_Mode_Bson);
	}

	YGGR_CONSTEXPR_OR_INLINE static u32 mode(void) YGGR_NOEXCEPT_OR_NOTHROW
	{
		return E_Mode;
	}
};

} // namespace archive_partner
} // namespace arvhive
} //namespace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_ARVHICE_PARTNER_ARCHIVE_PARTNER_TEMPLATE_HPP__
