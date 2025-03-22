//task_owner_info_parser.hpp

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

#ifndef __YGGR_TASK_CENTER_SUPPORT_TASK_OWNER_INFO_PARSER_HPP__
#define __YGGR_TASK_CENTER_SUPPORT_TASK_OWNER_INFO_PARSER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ids/base_ids_def.hpp>
#include <yggr/ids/inner_process_id.hpp>
#include <yggr/ids/uuid.hpp>

#define YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE( __type__ ) \
	namespace yggr{ namespace task_center { namespace support { \
	template<> \
	struct task_owner_info_parser< __type__ > { \
		typedef __type__ owner_id_type; \
		inline static const owner_id_type& owner_id(const __type__& data) { \
			return data; } }; } } }

namespace yggr
{
namespace task_center
{
namespace support
{

template<typename T>
struct task_owner_info_parser
{
	typedef typename T::owner_id_type owner_id_type;

	inline static owner_id_type owner_id(const T& t)
	{
		return t.owner_id();
	}
};

} // namespace support
} // task_center
} // namespace yggr

YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(char)
YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(yggr::s8)
YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(yggr::u8)
YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(yggr::s16)
YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(yggr::u16)
YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(yggr::s32)
YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(yggr::u32)
YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(yggr::s64)
YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(yggr::u64)

YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(yggr::ids::inner_process_id)
YGGR_PP_TASK_OWNER_INFO_PARSER_OF_VALUE(yggr::ids::uuid)

#endif // __YGGR_TASK_CENTER_SUPPORT_TASK_OWNER_INFO_PARSER_HPP__
