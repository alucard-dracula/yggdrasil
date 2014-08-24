//packet.hpp

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

#ifndef __YGGR_NETWORK_PACKET_HPP__
#define __YGGR_NETWORK_PACKET_HPP__

#include <yggr/move/move.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/assert.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/base/exception.hpp>
#include <yggr/base/exception_sort.hpp>
#include <yggr/base/error_make.hpp>
#include <yggr/charset/string.hpp>


namespace yggr
{
namespace packet
{

template<typename Archive_Partner>
class packet
{

public:
	typedef Archive_Partner archive_partner_type;
	typedef typename archive_partner_type::archive_type archive_type;
	typedef typename archive_partner_type::archive_helper_type archive_helper_type;

	typedef typename archive_helper_type::buf_type buf_type;
	typedef typename archive_helper_type::buf_val_type buf_val_type;
	typedef typename archive_helper_type::buf_real_val_type buf_real_val_type;

private:

	ERROR_MAKER_BEGIN("packet")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_1)
			ERROR_CODE_DEF(E_invalid_argument)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_1)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_argument, "invalid argument")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

//-------------------------------------------------------------------------
private:
	typedef packet this_type;

	BOOST_COPYABLE_AND_MOVABLE(this_type)
public:
	packet(void)
	{
	}

	template<typename InputIter>
	packet(InputIter start, InputIter end)
		: _buf(start, end)
	{
	}

	packet(BOOST_RV_REF(buf_type) buf)
	{
		archive_helper_type::buf_copy(_buf, boost::forward<buf_type>(buf));
	}

	packet(const buf_type& buf)
		: _buf(buf)
	{
	}

	template<typename Other_Archive_Partner>
	packet(BOOST_RV_REF(packet<Other_Archive_Partner>) pak)
	{
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));
		archive_helper_type::buf_copy(_buf, boost::forward<buf_type>(pak.org_buf()));
	}

	template<typename Other_Archive_Partner>
	packet(const packet<Other_Archive_Partner>& pak)
		: _buf(pak.org_buf())
	{
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));
	}

	packet(BOOST_RV_REF(this_type) right)
	{
		archive_helper_type::buf_swap(_buf, boost::forward<buf_type>(right._buf));
	}

	packet(const this_type& right)
		: _buf(right._buf)
	{
	}

	~packet(void)
	{
	}

	void clear(void)
	{
		_buf.clear();
	}

	inline buf_type& org_buf(void)
	{
		return _buf;
	}

	inline const buf_type& org_buf(void) const
	{
		return _buf;
	}

	template<typename T>
	bool save(const T& t)
	{
		if(typename archive_type::is_loading())
		{
			return false;
		}

		if(!archive_helper_type::buf_empty(_buf))
		{
			archive_helper_type::buf_clear(_buf);
		}

		if(!archive_helper_type::template save<T>(t, _buf))
		{
			return false;
		}

		if(archive_helper_type::buf_empty(_buf))
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_argument, "save"));
			return false;
		}

		return true;
	}

	template<typename T>
	bool load(T& t) const
	{
		if(typename archive_type::is_saving())
		{
			return false;
		}

		if(archive_helper_type::buf_empty(_buf))
		{
			exception::exception::throw_error(error_maker_type::make_error(error_maker_type::E_invalid_argument, "load"));
			return false;
		}

		if(!archive_helper_type::template load<T>(t, _buf))
		{
			return false;
		}
		return true;
	}

	bool empty(void) const
	{
		return archive_helper_type::buf_empty(_buf);
	}

	size_type size(void) const
	{
		return archive_helper_type::buf_size(_buf);
	}

	packet& resize(u32 size)
	{
		archive_helper_type::buf_resize(_buf, size);
		return *this;
	}


	buf_real_val_type* size_buf(u32 size)
	{
		resize(size);
		return archive_helper_type::buf_real_val_ptr(_buf);
	}

	buf_real_val_type* buf(void)
	{
		return archive_helper_type::buf_real_val_ptr(_buf);
	}

	const buf_real_val_type* buf(void) const
	{
		return archive_helper_type::buf_real_val_ptr(_buf);
	}

	buf_val_type* val_buf(void)
	{
		return archive_helper_type::buf_val_ptr(_buf);
	}

	const buf_val_type* val_buf(void) const
	{
		return archive_helper_type::buf_val_ptr();
	}

	this_type& operator=(BOOST_RV_REF(buf_type) buf)
	{
		archive_helper_type::buf_copy(_buf, boost::forward<buf_type>(buf));
		return *this;
	}

	this_type& operator=(const buf_type& buf)
	{
		archive_helper_type::buf_copy(_buf, buf);
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(BOOST_RV_REF(packet<Other_Archive_Partner>) pak)
	{
		archive_helper_type::buf_copy(_buf, boost::forward<buf_type>(pak.org_buf()));
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(const packet<Other_Archive_Partner>& pak)
	{
		archive_helper_type::buf_copy(_buf, pak.org_buf());
		return *this;
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		archive_helper_type::buf_swap(_buf, right._buf);
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		archive_helper_type::buf_copy(_buf, right._buf);
		return *this;
	}

	bool operator==(const buf_type& buf) const
	{
		return archive_helper_type::buf_compare(_buf, buf);
	}

	template<typename Other_Archive_Partner>
	bool operator==(const packet<Other_Archive_Partner>& pak)
	{
		return archive_helper_type::buf_compare(_buf, pak.org_buf());
	}

	bool operator==(const packet& pak) const
	{
		return archive_helper_type::buf_compare(_buf, pak._buf);
	}

	bool operator!=(const buf_type& buf) const
	{
		return !archive_helper_type::buf_compare(_buf, buf);
	}

	template<typename Other_Archive_Partner>
	bool operator!=(const packet<Other_Archive_Partner>& pak)
	{
		return !archive_helper_type::buf_compare(_buf, pak.org_buf());
	}

	bool operator!=(const packet& pak) const
	{
		return !archive_helper_type::buf_compare(_buf, pak._buf);
	}

	void swap(BOOST_RV_REF(buf_type) buf)
	{
		archive_helper_type::buf_swap(_buf, boost::forward<buf_type>(buf));
		return *this;
	}

	void swap(buf_type& obuf)
	{
		archive_helper_type::buf_swap(_buf, obuf);
	}

	template<typename Other_Archive_Partner>
	void swap(BOOST_RV_REF(packet<Other_Archive_Partner>) pak)
	{
		archive_helper_type::buf_swap(_buf, boost::forward<buf_type>(pak.org_buf()));
		return *this;
	}

	template<typename Other_Archive_Partner>
	void swap(const packet<Other_Archive_Partner>& pak)
	{
		archive_helper_type::buf_swap(_buf, pak.org_buf());
		return *this;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
		archive_helper_type::buf_swap(_buf, boost::forward<buf_type>(right._buf));
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		archive_helper_type::buf_swap(_buf, right._buf);
	}

	template<typename Other_Archive_Partner>
	void swap(packet<Other_Archive_Partner>& right)
	{
		archive_helper_type::buf_swap(_buf, right.org_buf());
	}

	template<typename Handler>
	typename Handler::result_type cypher(const Handler& handler)
	{
		return handler(boost::ref(_buf));
	}

protected:
	mutable buf_type _buf;
};


} // namespace packet
} // namespace yggr

// buf - pak
// pak1 - pak2
// pak - pak


#define _YGGR_PACKET_PACKET_SWAP() \
	template<typename Archive_Partner> inline \
	void swap(yggr::packet::packet<Archive_Partner>& l, \
				typename yggr::packet::packet<Archive_Partner>::buf_type& r) { \
		l.swap(r); } \
	\
	template<typename Archive_Partner> inline \
	void swap(BOOST_RV_REF(yggr::packet::packet<Archive_Partner>) l, \
				typename yggr::packet::packet<Archive_Partner>::buf_type& r) { \
		typedef yggr::packet::packet<Archive_Partner> now_this_type; \
		typedef typename now_this_type::buf_type buf_type; \
		typedef typename now_this_type::archive_helper_type archive_helper_type; \
		archive_helper_type::buf_swap(r, boost::forward<buf_type>(l.org_buf())); } \
	\
	template<typename Archive_Partner> inline \
	void swap(yggr::packet::packet<Archive_Partner>& l, \
				BOOST_RV_REF(typename yggr::packet::packet<Archive_Partner>::buf_type) r) { \
		l.swap(r); } \
	\
	template<typename Archive_Partner> inline \
	void swap(typename yggr::packet::packet<Archive_Partner>::buf_type& l, \
				yggr::packet::packet<Archive_Partner>& r) { \
		r.swap(l); } \
	\
	template<typename Archive_Partner> inline \
	void swap(BOOST_RV_REF(typename yggr::packet::packet<Archive_Partner>::buf_type) l, \
				yggr::packet::packet<Archive_Partner>& r) { \
		r.swap(l); } \
	\
	template<typename Archive_Partner> inline \
	void swap(typename yggr::packet::packet<Archive_Partner>::buf_type& l, \
				BOOST_RV_REF(yggr::packet::packet<Archive_Partner>) r) { \
		typedef yggr::packet::packet<Archive_Partner> now_this_type; \
		typedef typename now_this_type::buf_type buf_type; \
		typedef typename now_this_type::archive_helper_type archive_helper_type; \
		archive_helper_type::buf_swap(l, boost::forward<buf_type>(r.org_buf())); } \
	\
	template<typename Archive_Partner, typename Other_Archive_Partner> inline \
	void swap(yggr::packet::packet<Archive_Partner>& l, yggr::packet::packet<Other_Archive_Partner>& r) { \
		l.template swap(r); } \
	\
	template<typename Archive_Partner, typename Other_Archive_Partner> inline \
	void swap(BOOST_RV_REF(yggr::packet::packet<Archive_Partner>) l, yggr::packet::packet<Other_Archive_Partner>& r) { \
		r.template swap(l); } \
	\
	template<typename Archive_Partner, typename Other_Archive_Partner> inline \
	void swap(yggr::packet::packet<Archive_Partner>& l, BOOST_RV_REF(yggr::packet::packet<Other_Archive_Partner>) r) { \
		l.template swap(r); } \
	\
	template<typename Archive_Partner> inline \
	void swap(yggr::packet::packet<Archive_Partner>& l, \
				yggr::packet::packet<Archive_Partner>& r) { \
		l.swap(r); } \
	\
	template<typename Archive_Partner> inline \
	void swap(BOOST_RV_REF(yggr::packet::packet<Archive_Partner>) l, \
				yggr::packet::packet<Archive_Partner>& r) { \
		r.swap(l); } \
	\
	template<typename Archive_Partner> inline \
	void swap(yggr::packet::packet<Archive_Partner>& l, \
				BOOST_RV_REF(yggr::packet::packet<Archive_Partner>) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PACKET_PACKET_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PACKET_PACKET_SWAP()
} // namespace boost

#undef _YGGR_PACKET_PACKET_SWAP

#endif //__YGGR_NETWORK_PACKET_HPP__
