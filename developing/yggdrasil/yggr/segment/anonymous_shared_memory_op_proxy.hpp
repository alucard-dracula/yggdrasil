//anonymous_shared_memory_op_proxy.hpp

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

#ifndef __YGGR_SEGMENT_ANONYMOUS_SHARED_MEMORY_OP_PROXY_HPP__
#define __YGGR_SEGMENT_ANONYMOUS_SHARED_MEMORY_OP_PROXY_HPP__

#ifndef YGGR_ANONYMOUS_SEGMENT_OPERATOR_INCLUDE
#	error "include error anonymous_shared_memory_op_proxy.hpp please include anonymous_segment_op_proxy.hpp."
#endif // YGGR_ANONYMOUS_SEGMENT_OPERATOR_INCLUDE

#include <boost/interprocess/detail/config_begin.hpp>
#include <yggr/segment/anonymous_shared_memory.hpp>

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace segment
{

template<>
class anonymous_segment_op_proxy<yggr::segment::anonymous_shared_memory>
		: private nonable::noncreateable
{
public:
	typedef yggr::segment::anonymous_shared_memory seg_type;

private:
	typedef anonymous_segment_op_proxy this_type;

public:
// swap create
    static bool create(seg_type& seg)
	{
		seg_type tmp;
		seg.swap(tmp);
		return true;
	}

#	define BOOST_PP_LOCAL_MACRO(__n__) \
	template< YGGR_PP_FOO_TYPES_DEF(__n__) > \
	static bool create(seg_type& seg, YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_CREF_PARAMS)) { \
			try { \
				seg_type tmp(YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); \
				seg.swap(tmp); \
				return true; } \
			catch(const boost::interprocess::interprocess_exception& e) { \
				exception::exception::throw_error(e.get_error_code(), e.what()); } \
			return false; }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

	static void remove(seg_type& seg)
	{
	    this_type::create(seg);
	}

	static bool is_empty(const seg_type& seg)
	{
		return this_type::size(seg) == 0;
	}

	static size_type size(const seg_type& seg)
	{
		return seg.get_size();
	}

	static bool is_named_object(void)
	{
		return false;
	}

private:

};

} // segment
} // namespace yggr

#ifdef _MSC_VER
#	pragma warning( pop )
#endif // _MSC_VER

#include <boost/interprocess/detail/config_end.hpp>

#endif //__YGGR_SEGMENT_ANONYMOUS_SHARED_MEMORY_OP_PROXY_HPP__
