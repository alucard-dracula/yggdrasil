//log_file_op.hpp

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

#ifndef __YGGR_LOG_LOGOP_LOG_FILE_OP_HPP__
#define __YGGR_LOG_LOGOP_LOG_FILE_OP_HPP__

#include <yggr/base/yggrdef.h>
#include <boost/tuple/tuple.hpp>
#include <yggr/nonable/noncopyable.hpp>

namespace yggr
{
namespace log
{
namespace log_op
{

template<typename Def, typename File_Mgr>
class log_file_op : private nonable::noncopyable
{
public:

	typedef Def def_type;
	enum
	{
		//E_LOG_OP_TYPE = typename def_type::E_LOG_OP_FILE,
		E_LOG_OP_TYPE = def_type::E_LOG_OP_FILE,
		E_id_compile_u32 = 0xffffffff
	};


	typedef typename def_type::id_type id_type;

	typedef File_Mgr file_mgr_type;
	typedef file_mgr_type real_op_type;
	typedef typename file_mgr_type::init_type init_type;


public:
	log_file_op(const init_type& init)
		: _real_op(init)
	{
	}

	~log_file_op(void)
	{
	}

#	include <yggr/support/log_op_general_foo.ipp>

	template<typename Tuple_Handler>
	void append(const Tuple_Handler& handler)
	{
		_real_op.append(boost::get<0>(handler), boost::get<1>(handler));
	}

	template<typename Tuple_Handler>
	void clear(const Tuple_Handler& handler)
	{
		_real_op.clear(boost::get<0>(handler));
	}

	template<typename Tuple_Handler>
	void search(const Tuple_Handler& handler)
	{
	}

private:
	real_op_type _real_op;
};

} // namespace log_op
} // namespace log
} // namespace yggr

#endif //__YGGR_LOG_LOGOP_LOG_FILE_OP_HPP__
