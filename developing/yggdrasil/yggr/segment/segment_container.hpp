//segment_container.hpp

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

#ifndef __YGGR_SEGMENT_SEGMENT_CONTAINER_HPP__
#define __YGGR_SEGMENT_SEGMENT_CONTAINER_HPP__

#include <utility>
#include <yggr/utility/swap.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/move/move.hpp>
#include <yggr/segment/segment_op_proxy.hpp>

#include <cassert>

namespace yggr
{
namespace segment
{

template<typename Key, typename Seg>
class segment_container
		: private std::pair< Key, Seg >
{

private:
	typedef std::pair<Key, Seg > base_type;

public:
	typedef typename base_type::first_type key_type;
	typedef typename base_type::second_type seg_type;

private:
	typedef segment_op_proxy<key_type, seg_type> seg_op_proxy_type;

	typedef segment_container this_type;

//#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
//	BOOST_MOVABLE_BUT_NOT_COPYABLE(this_type)
//#else
//    segment_container(this_type&);
//    this_type& operator=(this_type &);
//	BOOST_COPYABLE_AND_MOVABLE_ALT(this_type);
//#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

    segment_container(this_type&);
    this_type& operator=(this_type &);
	BOOST_COPYABLE_AND_MOVABLE_ALT(this_type);

public:

	segment_container(const key_type& name)
	{
		assert(!name.empty());
		base_type::first = name;
	}

#	define BOOST_PP_LOCAL_MACRO(__n__) \
	template< YGGR_PP_FOO_TYPES_DEF(__n__) > \
	segment_container(const key_type& name YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_CREF_PARAMS)) { \
				assert(!name.empty()); \
				base_type::first = name; \
                seg_op_proxy_type::create( base_type::second, name \
                        YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
                        YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); }

	/*\
	\
	template< typename Handler YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) YGGR_PP_FOO_TYPES_DEF(__n__) > \
	segment_container(const Handler& handler, const key_type& name YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
						YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_CREF_PARAMS)) { \
				assert(!name.empty()); \
				base_type::first = name; \
                seg_op_proxy_type::create( base_type::second, handler, \
                        name YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
                        YGGR_PP_FOO_PARAMS_OP(__n__, YGGR_PP_SYMBOL_COMMA)); }*/

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

	segment_container(const key_type& name, BOOST_RV_REF(seg_type) seg)
	{
		assert(!name.empty());
		base_type::first = name;
		base_type::second = boost::forward<seg_type>(seg);
	}

	segment_container(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::first = boost::forward<key_type>(right.first);
#else
		this_type& right_ref = right;
		boost::swap(base_type::first, right_ref.first);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

		base_type::second = boost::forward<seg_type>(right.second);
	}

	~segment_container(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::first = boost::forward<key_type>(right.first);
#else
		this_type& right_ref = right;
		boost::swap(base_type::first, right_ref.first);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

		base_type::second = boost::forward<seg_type>(right.second);
		return *this;
	}

	const std::string& name(void) const
	{
		return base_type::first;
	}

	const seg_type& segment(void) const
	{
		return base_type::second;
	}

	seg_type& segment(void)
	{
		return base_type::second;
	}

	size_type size(void) const
	{
		return seg_op_proxy_type::size(base_type::second);
	}

	bool empty(void) const
	{
		return seg_op_proxy_type::is_empty(base_type::second);
	}

	void free(void)
	{
		seg_op_proxy_type::remove(base_type::first);
		key_type key;
		base_type::first.swap(key);
		seg_op_proxy_type::create(base_type::second);
	}

	bool is_named_object(void) const
	{
		return seg_op_proxy_type::is_named_object();
	}

protected:
	// first name
	// second value;
};

} // namespace segment
} // namespace yggr

#endif // __YGGR_SEGMENT_SEGMENT_CONTAINER_HPP__
