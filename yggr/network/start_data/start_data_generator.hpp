//start_data_generator.hpp

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

#ifndef __YGGR_NETWORK_START_DATA_START_DATA_GENERATOR_HPP__
#define __YGGR_NETWORK_START_DATA_START_DATA_GENERATOR_HPP__

namespace yggr
{
namespace network
{
namespace start_data
{

template<typename Start_Data>
struct start_data_generator;

// use template specialization
// example:
//template<>
//struct start_data_generator<Start_Data>
//{
//	typedef Start_Data start_data_type;
//	typedef typename start_data_type::id_type id_type;
//	
//	start_data_type operator()(const id_type& id) const
//	{
//		return start_data_type(id);
//	}
//}

template<typename Start_Data>
struct start_data_loader;

// use template specialization
// example:
//template<>
//struct start_data_loader<Start_Data>
//{
//	typedef Start_Data start_data_type;
//	
//	template<typename Packet>
//	start_data_type operator()(Packet& pak) const
//	{
//		start_data_type
//		return start_data_type(id);
//	}
//}

} // namespace start_data
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_START_DATA_START_DATA_GENERATOR_HPP__
