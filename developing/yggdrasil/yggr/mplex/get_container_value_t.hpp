//get_container_value_t.hpp

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

#ifndef __YGGR_MPLEX_GET_CONTAINER_VALUE_T_HPP__
#define __YGGR_MPLEX_GET_CONTAINER_VALUE_T_HPP__

#include <yggr/ppex/typedef.hpp>
#include <map>
#include <boost/container/map.hpp>
#include <boost/unordered_map.hpp>

#define YGGR_PP_GET_CONTAINER_VALUE_T_DEF(__n__, __container__, __type__) \
namespace yggr { namespace mplex { \
	template<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, typename T)> \
	struct get_container_value_t< __container__<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T)> > { \
	private: \
		typedef __container__<YGGR_PP_TEMPLATE_PARAMS_TYPES(__n__, T)> container_type; \
	public: \
		typedef typename container_type::__type__ type; }; }}

namespace yggr
{
namespace mplex
{

template<typename T>
struct get_container_value_t
{
private:
	typedef T container_type;
public:
	typedef typename container_type::value_type type;
};

} // namespace mplex
} // namespace yggr

YGGR_PP_GET_CONTAINER_VALUE_T_DEF(4, std::map, mapped_type)
YGGR_PP_GET_CONTAINER_VALUE_T_DEF(4, std::multimap, mapped_type)

YGGR_PP_GET_CONTAINER_VALUE_T_DEF(4, boost::container::map, mapped_type)
YGGR_PP_GET_CONTAINER_VALUE_T_DEF(4, boost::container::multimap, mapped_type)

YGGR_PP_GET_CONTAINER_VALUE_T_DEF(5, boost::unordered_map, mapped_type)
YGGR_PP_GET_CONTAINER_VALUE_T_DEF(5, boost::unordered_multimap, mapped_type)

#endif //__YGGR_MPLEX_GET_CONTAINER_VALUE_T_HPP__