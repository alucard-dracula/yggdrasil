//inner_process_id.cpp

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

#include <yggr/ids/inner_process_id.hpp>

#include <iomanip>

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

namespace yggr
{
namespace ids
{

inner_process_id::inner_process_id(void)
{
}

inner_process_id::inner_process_id(main_id_type main_id, inner_id_type inner_id)
{
	memcpy(&base_type::_data[0], &main_id, sizeof(main_id_type));
	memcpy(&base_type::_data[sizeof(main_id_type)], &inner_id, sizeof(inner_id_type));
}

inner_process_id::inner_process_id(const id_data_type& right)
{
	memcpy(&base_type::_data[0], &(right.first), sizeof(main_id_type));
	memcpy(&base_type::_data[sizeof(main_id_type)], &(right.second), sizeof(inner_id_type));
}

inner_process_id::inner_process_id(const this_type& right)
	: base_type(right)
{
}

inner_process_id::~inner_process_id(void)
{
}

inner_process_id::this_type& inner_process_id::operator=(const id_data_type& right)
{
	memcpy(&(base_type::_data[0]), &(right.first), sizeof(main_id_type));
	memcpy(&(base_type::_data[sizeof(main_id_type)]), &(right.second), sizeof(inner_id_type));
	return *this;
}

inner_process_id::this_type& inner_process_id::operator=(const this_type& right)
{
	base_type::operator=(static_cast<const base_type&>(right));
	return *this;
}

void inner_process_id::swap(id_data_type& right)
{
	u8 tmp[base_type::E_length] = {0};
	memcpy(tmp, &(base_type::_data[0]), base_type::E_length);

	memcpy(&(base_type::_data[0]), &(right.first), sizeof(main_id_type));
	memcpy(&(base_type::_data[sizeof(main_id_type)]), &(right.second), sizeof(inner_id_type));

	memcpy(&(right.first), &(tmp[0]), sizeof(main_id_type));
	memcpy(&(right.second), &(tmp[sizeof(main_id_type)]), sizeof(inner_id_type));
}

void inner_process_id::swap(this_type& right)
{
	base_type::swap(static_cast<base_type&>(right));
}


} // namespace ids
} // namespace yggr

