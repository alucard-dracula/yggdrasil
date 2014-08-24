//inner_process_id.cpp

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

#if defined(_MSC_VER)
#	include <vld.h>
#endif // _MSC_VER

#include <yggr/ids/inner_process_id.hpp>
#include <iomanip>

namespace yggr
{
namespace ids
{

inner_process_id::inner_process_id(void)
{
}

inner_process_id::inner_process_id(const main_id_type& main_id, const inner_id_type& inner_id)
{
	memcpy(&base_type::_data[0], &main_id, sizeof(main_id));
	memcpy(&base_type::_data[sizeof(main_id)], &inner_id, sizeof(inner_id));
}

inner_process_id::inner_process_id(const this_type& right)
	: base_type(right)
{
}

inner_process_id::~inner_process_id(void)
{
}

inner_process_id::val_type inner_process_id::id_val(void) const
{
	val_type ids;
	memcpy(&ids.first, &_data[0], sizeof(main_id_type));
	memcpy(&ids.second, &_data[sizeof(main_id_type)], sizeof(inner_id_type));
	return ids;
}


inner_process_id::this_type& inner_process_id::operator=(const this_type& right)
{
	if(this == &right) {return *this;}
	base_type::operator=(right);
	return *this;
}


void inner_process_id::swap(this_type& right)
{
	base_type::swap(right);
}

} // namespace ids

} // namespace yggr

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace ids
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

std::size_t hash_value(const yggr::ids::inner_process_id& id)
{
	return id.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace ids
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP


#define _YGGR_TMP_PP_INNER_PROCESS_ID_SWAP_IMPL() \
	void swap(yggr::ids::inner_process_id& left, yggr::ids::inner_process_id& right) { \
		left.swap(right); }

namespace std
{
	_YGGR_TMP_PP_INNER_PROCESS_ID_SWAP_IMPL()
} // namespace std

namespace boost
{
	_YGGR_TMP_PP_INNER_PROCESS_ID_SWAP_IMPL()
} // namespace boost

#undef _YGGR_TMP_PP_INNER_PROCESS_ID_SWAP_IMPL
