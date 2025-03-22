//segment_destroyer.hpp

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

#ifndef __YGGR_SEGMENT_SEGMENT_DESTROYER_HPP__
#define __YGGR_SEGMENT_SEGMENT_DESTROYER_HPP__

#include <yggr/segment/segment_config.hpp> // it must be written in the first sentence include

#include <yggr/charset/utf8_string.hpp>

// named_disable
#include <yggr/segment/anonymous_shared_memory.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/managed_heap_memory.hpp>

// named_enable
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

namespace yggr
{
namespace segment
{

template<typename Seg>
struct segment_destroyer;

} // namespace segment
} // namespace yggr

#define YGGR_SEGMENT_NAMED_ENABLE_OBJECT_DELETER_DEF(__obj__, __del_obj__) \
namespace yggr { namespace segment { \
template<> struct segment_destroyer< __obj__ > { \
	typedef __obj__ obj_type; \
	typedef __del_obj__ type; \
	segment_destroyer(const char* name) { \
		if(name) { type::remove(name); } } \
	~segment_destroyer(void) { } }; } }

#define YGGR_SEGMENT_NAMED_ENABLE_WRAP_OBJECT_DELETER_DEF(__obj__, __del_obj__) \
namespace yggr { namespace segment { \
template<typename T1, typename T2, \
			template<typename _T3> class T3 > \
struct segment_destroyer< __obj__< T1, T2, T3 > > { \
	typedef __obj__< T1, T2, T3 > obj_type; \
	typedef __del_obj__ type; \
	segment_destroyer(const char* name) { \
		if(name) { type::remove(name); } } \
	~segment_destroyer(void) { } }; } }

#define YGGR_SEGMENT_NAMED_DISABLE_OBJECT_DELETER_DEF( __obj__ ) \
namespace yggr { namespace segment { \
template<> struct segment_destroyer< __obj__ > { \
	typedef __obj__ obj_type; \
	typedef void type; \
	segment_destroyer(void) { } \
	~segment_destroyer(void) { } }; } }

#define YGGR_SEGMENT_NAMED_DISABLE_WRAP_OBJECT_DELETER_DEF( __obj__ ) \
namespace yggr { namespace segment { \
template<typename T1, typename T2, \
			template<typename _T3> class T3 > \
struct segment_destroyer< __obj__< T1, T2, T3 > > { \
	typedef __obj__< T1, T2, T3 > obj_type; \
	typedef void type; \
	segment_destroyer(void) { } \
	~segment_destroyer(void) { } }; } }


// named_enable
YGGR_SEGMENT_NAMED_ENABLE_OBJECT_DELETER_DEF(boost::interprocess::shared_memory_object,
												boost::interprocess::shared_memory_object)

YGGR_SEGMENT_NAMED_ENABLE_WRAP_OBJECT_DELETER_DEF(boost::interprocess::basic_managed_shared_memory,
													boost::interprocess::shared_memory_object)

YGGR_SEGMENT_NAMED_ENABLE_OBJECT_DELETER_DEF(boost::interprocess::file_mapping,
												boost::interprocess::file_mapping)

YGGR_SEGMENT_NAMED_ENABLE_WRAP_OBJECT_DELETER_DEF(boost::interprocess::basic_managed_mapped_file,
													boost::interprocess::file_mapping)

namespace yggr 
{ 
namespace segment 
{

template<typename T1> 
struct segment_destroyer< boost::interprocess::message_queue_t<T1> > 
{
	typedef boost::interprocess::message_queue_t<T1> obj_type;
	typedef boost::interprocess::message_queue_t<T1> type;

	segment_destroyer(const char* name) 
	{
		if(name) 
		{
			type::remove(name); 
		} 
	}

	~segment_destroyer(void) 
	{
	} 
}; 

} // namespace segment
} // namespace yggr

// named_disable
YGGR_SEGMENT_NAMED_DISABLE_OBJECT_DELETER_DEF(yggr::segment::anonymous_shared_memory)

YGGR_SEGMENT_NAMED_DISABLE_WRAP_OBJECT_DELETER_DEF(boost::interprocess::basic_managed_external_buffer)

YGGR_SEGMENT_NAMED_DISABLE_WRAP_OBJECT_DELETER_DEF(boost::interprocess::basic_managed_heap_memory)

#endif // __YGGR_SEGMENT_SEGMENT_DESTROYER_HPP__
