//lzma_props.hpp

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

#ifndef __YGGR_COMPACTION_TOOL_LZMA_PROPS_HPP__
#define __YGGR_COMPACTION_TOOL_LZMA_PROPS_HPP__

#include <cassert>
#include <memory>

#include <boost/array.hpp>
#include <yggr/move/move.hpp>
#include <boost/serialization/access.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/serialization/nvp.hpp>
#include <yggr/serialization/array.hpp>

namespace yggr
{
namespace compaction_tool
{

struct lzma_props
	: public boost::array<yggr::u8, 5>
{
public:
	enum
	{
		E_length = 5,
		E_byte_length = 5,
		E_compile_u32 = 0xffffffff
	};

	typedef yggr::u8 val_type;
	typedef boost::array<val_type, E_length> base_type;
	typedef base_type::value_type value_type;

private:
	typedef lzma_props this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	lzma_props(void);

	template<typename InputIter>
	lzma_props(InputIter s, InputIter e)
		: base_type(s, e)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	lzma_props(BOOST_RV_REF(this_type) right)
	{
		base_type& base = boost::forward<base_type>(right);
	}
#else
	lzma_props(BOOST_RV_REF(this_type) right)
	{
		base_type& base = *this;
		memset(&base[0], 0, E_byte_length);
		base_type& right_ref = right;
		base.swap(right_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	lzma_props(const this_type& right);

	~lzma_props(void);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::operator=(boost::forward<base_type>(right));
#else
		this_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}
	this_type& operator=(const this_type& right);

	bool operator==(const this_type& right) const;
	bool operator!=(const this_type& right) const;

	inline void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//base_type::swap(boost::forward<base_type>(right));
		base_type::swap(right);
#else
		this_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right);

	const std::size_t prop_size(void) const;

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NAME_NVP("data", boost::serialization::base_object<base_type>(*this));
	}

public:
	static const std::size_t s_prop_size(void);
};

} // compaction_tool
} // namespace yggr


//#define YGGR_COMPACTION_TOOL_LZMA_PROPS_SWAP_DEF() \
//	void swap(BOOST_RV_REF(yggr::compaction_tool::lzma_props) l, \
//				yggr::compaction_tool::lzma_props& r); \
//	\
//	inline void swap(yggr::compaction_tool::lzma_props& l, \
//				BOOST_RV_REF(yggr::compaction_tool::lzma_props) r) { \
//		typedef yggr::compaction_tool::lzma_props lzma_props_type; \
//		l.swap(boost::forward<lzma_props_type>(r)); } \
//	\
//	void swap(yggr::compaction_tool::lzma_props& l, \
//				yggr::compaction_tool::lzma_props& r);

#define YGGR_COMPACTION_TOOL_LZMA_PROPS_SWAP_DEF() \
	inline void swap(BOOST_RV_REF(yggr::compaction_tool::lzma_props) l, \
				yggr::compaction_tool::lzma_props& r) { \
		r.swap(l); } \
	\
	inline void swap(yggr::compaction_tool::lzma_props& l, \
				BOOST_RV_REF(yggr::compaction_tool::lzma_props) r) { \
		l.swap(r); } \
	\
	void swap(yggr::compaction_tool::lzma_props& l, \
				yggr::compaction_tool::lzma_props& r);

namespace std
{
	YGGR_COMPACTION_TOOL_LZMA_PROPS_SWAP_DEF()
} // namespace std

namespace boost
{
	YGGR_COMPACTION_TOOL_LZMA_PROPS_SWAP_DEF()
} // namespace boost

#undef YGGR_COMPACTION_TOOL_LZMA_PROPS_SWAP_DEF

#endif // __YGGR_COMPACTION_TOOL_LZMA_PROPS_HPP__