//anonymous_segment_container.hpp

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

#ifndef __YGGR_SEGMENT_ANONYMOUS_SEGMENT_CONTANINER_HPP__
#define __YGGR_SEGMENT_ANONYMOUS_SEGMENT_CONTANINER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/segment/anonymous_segment_op_proxy.hpp>

namespace yggr
{
namespace segment
{

template<typename Seg>
class anonymous_segment_container
{

public:
	typedef Seg seg_type;

private:
	typedef anonymous_segment_op_proxy<seg_type> seg_op_proxy_type;
	typedef anonymous_segment_container this_type;

//#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
//	BOOST_MOVABLE_BUT_NOT_COPYABLE(this_type)
//#else
//    anonymous_segment_container(this_type&);
//    this_type& operator=(this_type&);
//    BOOST_COPYABLE_AND_MOVABLE_ALT(this_type)
//#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	anonymous_segment_container(this_type&);
    this_type& operator=(this_type&);
    BOOST_COPYABLE_AND_MOVABLE_ALT(this_type)

public:

	anonymous_segment_container(void)
	{
	    seg_op_proxy_type::create(_seg);
	}

#	define BOOST_PP_LOCAL_MACRO(__n__) \
	template< YGGR_PP_FOO_TYPES_DEF(__n__) > \
	anonymous_segment_container(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_CREF_PARAMS)) { \
        seg_op_proxy_type::create(_seg, YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

	anonymous_segment_container(BOOST_RV_REF(seg_type) seg)
		: _seg(boost::forward<seg_type>(seg))
	{
	}

	anonymous_segment_container(BOOST_RV_REF(this_type) right)
		: _seg(boost::forward<seg_type>(right._seg))
	{
	}

	~anonymous_segment_container(void)
	{
		seg_op_proxy_type::remove(_seg);
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		_seg = boost::forward<this_type>(right);
		return *this;
	}

	const seg_type& segment(void) const
	{
		return _seg;
	}

	seg_type& segment(void)
	{
		return _seg;
	}

	size_type size(void) const
	{
		return seg_op_proxy_type::size(_seg);
	}

	bool empty(void) const
	{
		return seg_op_proxy_type::is_empty(_seg);
	}

	void free(void)
	{
		seg_op_proxy_type::remove(_seg);
	}

	bool is_named_object(void) const
	{
		return false;
	}

protected:
	seg_type _seg;
};

} // namespace segment
} // namespace yggr

#endif // __YGGR_SEGMENT_ANONYMOUS_SEGMENT_CONTANINER_HPP__
