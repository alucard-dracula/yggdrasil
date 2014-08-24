//segment_deleter.hpp

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

#ifndef __YGGR_SEGMENT_SEGMENT_DELETER_HPP__
#define __YGGR_SEGMENT_SEGMENT_DELETER_HPP__

#include <boost/interprocess/shared_memory_object.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/managed_external_buffer.hpp>
#include <boost/interprocess/managed_heap_memory.hpp>
//#include <boost/interprocess/anonymous_shared_memory.hpp> // this is a mapped_region object

#include <boost/interprocess/file_mapping.hpp>

#include <boost/interprocess/managed_mapped_file.hpp>

#include <boost/mpl/at.hpp>
#include <boost/mpl/pair.hpp>
#include <boost/mpl/map.hpp>

namespace yggr
{
namespace segment
{

class deleter_types_map
{
protected:
	typedef boost::mpl::map
		<
			boost::mpl::pair
			<
				boost::interprocess::shared_memory_object, 
				boost::interprocess::shared_memory_object
			>,

			boost::mpl::pair
			<
				boost::interprocess::managed_shared_memory,
				boost::interprocess::shared_memory_object
			>,

			//boost::mpl::pair
			//<
			//	boost::interprocess::managed_external_buffer,
			//	boost::interprocess::shared_memory_object
			//>,

			//boost::mpl::pair
			//<
			//	boost::interprocess::managed_heap_memory,
			//	boost::interprocess::shared_memory_object
			//>,

			boost::mpl::pair
			<
				boost::interprocess::file_mapping,
				boost::interprocess::file_mapping
			>,

			boost::mpl::pair
			<
				boost::interprocess::managed_mapped_file,
				boost::interprocess::file_mapping
			>
		> deleter_types;
};

template<typename Key, typename Seg>
class segment_deleter : private deleter_types_map
{
private:
	typedef deleter_types_map base_type;
	typedef Key key_type;
	typedef Seg seg_type;
	typedef typename boost::mpl::at<base_type::deleter_types, seg_type>::type deleter_type;

public:
	segment_deleter(const key_type& name)
	{
		if(!name.empty())
		{
			deleter_type::remove(name.c_str());
		}
	}

	~segment_deleter(void)
	{
	}
};

} // namespace segment
} // namespace yggr

#endif // __YGGR_SEGMENT_SEGMENT_DELETER_HPP__