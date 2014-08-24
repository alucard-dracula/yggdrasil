//lzma_data.hpp

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

#ifndef __YGGR_COMPACTION_TOOL_LZMA_DATA_HPP__
#define __YGGR_COMPACTION_TOOL_LZMA_DATA_HPP__

#include <utility>

#include <yggr/move/move.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/serialization/access.hpp>

#include <yggr/serialization/nvp.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/serialization/string.hpp>
#include <yggr/compaction_tool/lzma_props.hpp>

namespace yggr
{
namespace compaction_tool
{

template<typename Char = char,
			typename Traits = std::char_traits<Char>, 
			typename Alloc = std::allocator<Char>,
			template<typename _Char,
						typename _Traits, 
						typename _Alloc>
				class Basic_String = std::basic_string
		>
class lzma_data
{
public:
	typedef Char val_type;
	BOOST_MPL_ASSERT((boost::is_same<val_type, char>));

	typedef ser_size_type size_type;
	typedef Traits traits_type;
	typedef Alloc alloc_type;
	typedef lzma_props lzma_props_type;
	typedef Basic_String<Char, Traits, alloc_type> lzma_buf_type;

private:
	typedef lzma_data this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	lzma_data(void)
		: _org_byte_size(size_type())
	{
	}

	lzma_data(const size_type& org_byte_size, 
				const lzma_props_type& props, 
				const lzma_buf_type& buf)
		: _org_byte_size(org_byte_size),
			_props(props), _buf(buf)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	lzma_data(const size_type& org_byte_size, 
				const lzma_props_type& props, 
				BOOST_RV_REF(lzma_buf_type) buf)
		: _org_byte_size(org_byte_size),
			_props(props), _buf(boost::forward<lzma_buf_type>(buf))
	{
	}
#else
	lzma_data(const size_type& org_byte_size, 
				const lzma_props_type& props, 
				BOOST_RV_REF(lzma_buf_type) buf)
		: _org_byte_size(org_byte_size),
			_props(props)
	{
		lzma_buf_type& buf_ref = buf;
		_buf.swap(buf_ref);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	template<typename InputIter>
	lzma_data(const size_type& org_byte_size, 
				const lzma_props_type& props, 
				InputIter s, InputIter e)
		: _org_byte_size(org_byte_size),
			_props(props), _buf(s, e)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	lzma_data(BOOST_RV_REF(this_type) right)
		: _org_byte_size(boost::forward<size_type>(right._org_btye_size)),
			_props(boost::forward<lzma_props_type>(right._props)),
			_buf(boost::forward<lzma_buf_type>(right._buf))
			
	{
	}
#else
	lzma_data(BOOST_RV_REF(this_type) right)
		: _org_byte_size(size_type())
			
	{
		this_type& right_ref = right;
		std::swap(_org_byte_size, right_ref._org_btye_size);
		_props.swap(right_ref._props);
		_buf.swap(right_ref._buf);
	}
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES

	lzma_data(const this_type& right)
		: _org_byte_size(right._org_byte_size),
			_props(right._props),
			_buf(right._buf)
	{
	}

	~lzma_data(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_org_byte_size = boost::forward<size_type>(right._org_byte_size);
		_props = boost::forward<lzma_props_type>(right._props);
		_buf = boost::forward<lzma_buf_type>(right_ref._buf);
#else
		this_type& right_ref = right;
		std::swap(_org_byte_size, right_ref._org_btye_size);
		_props.swap(right_ref._props);
		_buf.swap(right_ref._buf);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_org_byte_size = right._org_byte_size;
		_props = right._props;
		_buf = right._buf;
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//std::swap(_org_byte_size, boost::forward<size_type>(right._org_byte_size));
		//_props.swap(boost::forward<lzma_props_type>(right._props));
		//_buf.swap(boost::forward<lzma_buf_type>(right._buf));
		std::swap(_org_byte_size, right._org_byte_size);
		_props.swap(right._props);
		_buf.swap(right._buf);
#else
		this_type& right_ref = right;
		std::swap(_org_byte_size, right_ref._org_btye_size);
		_props.swap(right_ref._props);
		_buf.swap(right_ref._buf);
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(_org_byte_size, right._org_byte_size);
		_props.swap(right._props);
		_buf.swap(right._buf);
	}

	bool empty(void) const
	{
		return !_org_byte_size || _buf.empty();
	}

	size_type org_size(void) const
	{
		return _org_byte_size;
	}

	const lzma_props_type& props(void) const
	{
		return _props;
	}

	const lzma_buf_type& buf(void) const
	{
		return _buf;
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_org_byte_size);
		ar & YGGR_SERIALIZE_NVP(_props);
		ar & YGGR_SERIALIZE_NVP(_buf);
	}

private:
	size_type _org_byte_size;
	lzma_props_type _props;
	lzma_buf_type _buf;
};

} // namespace compaction_tool
} // namespace yggr

//#define YGGR_PP_COMPACTION_TOOL_LZMA_DATA_SWAP() \
//	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename _T ) > class Basic_String > inline \
//	void swap(BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
//													YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//													Basic_String)) l, \
//				YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
//											YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//											Basic_String)& r) { \
//		typedef YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
//											YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//											Basic_String) lzma_data_type; \
//		r.swap(boost::forward<lzma_data_type>(l)); } \
//	\
//	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename _T ) > class Basic_String > inline \
//	void swap(YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
//										YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//										Basic_String)& l, \
//				BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
//														YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//														Basic_String)) r) { \
//		typedef YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
//											YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//											Basic_String) lzma_data_type; \
//		l.swap(boost::forward<lzma_data_type>(r)); } \
//	\
//	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename _T ) > class Basic_String > inline \
//	void swap(YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
//										YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//										Basic_String)& l, \
//				YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
//														YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
//														Basic_String)& r) { \
//		l.swap(r); }

#define YGGR_PP_COMPACTION_TOOL_LZMA_DATA_SWAP() \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename _T ) > class Basic_String > inline \
	void swap(BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
													YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
													Basic_String)) l, \
				YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
											YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
											Basic_String)& r) { \
		r.swap(l); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename _T ) > class Basic_String > inline \
	void swap(YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
										Basic_String)& l, \
				BOOST_RV_REF(YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
														YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
														Basic_String)) r) { \
		l.swap(r); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
				template < YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename _T ) > class Basic_String > inline \
	void swap(YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
										YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
										Basic_String)& l, \
				YGGR_PP_TEMPLATE_TYPE2(yggr::compaction_tool::lzma_data, \
														YGGR_PP_TEMPLATE_PARAMS_TYPES( 3, typename T ), \
														Basic_String)& r) { \
		l.swap(r); }

namespace std
{
	YGGR_PP_COMPACTION_TOOL_LZMA_DATA_SWAP()
} // namespace std

namespace boost
{
	YGGR_PP_COMPACTION_TOOL_LZMA_DATA_SWAP()
} // namespace boost



#endif // __YGGR_COMPACTION_TOOL_LZMA_DATA_HPP__