//packet.hpp

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

#ifndef __YGGR_NETWORK_PACKET_HPP__
#define __YGGR_NETWORK_PACKET_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>

#include <yggr/exception/exception.hpp>
#include <yggr/exception/exception_sort.hpp>
#include <yggr/base/error_make.hpp>
#include <yggr/charset/string.hpp>
#include <yggr/mplex/static_assert.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <boost/type_traits/is_same.hpp>

#include <ostream>

namespace yggr
{
namespace packet
{

template<typename Archive_Partner>
class packet;

template<typename Archive_Partner>
class ref_packet;

} // namespace packet
} // namespace yggr


//----------------------packet---------------------------
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
		: _buf(boost::move(buf))
	{
	}

	packet(const buf_type& buf)
		: _buf(buf)
	{
	}

	template<typename Other_Archive_Partner>
	packet(BOOST_RV_REF(packet<Other_Archive_Partner>) pak)
		: _buf(boost::move(pak.org_buf()))
	{
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));
	}

	template<typename Other_Archive_Partner>
	packet(const packet<Other_Archive_Partner>& pak)
		: _buf(pak.org_buf())
	{
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));
	}

	template<typename Other_Archive_Partner>
	packet(BOOST_RV_REF(ref_packet<Other_Archive_Partner>) pak)
		: _buf(boost::move(pak.org_buf()))
	{
		typedef ref_packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));
	}

	template<typename Other_Archive_Partner>
	packet(const ref_packet<Other_Archive_Partner>& pak)
		: _buf(pak.org_buf())
	{
		typedef ref_packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));
	}

	packet(BOOST_RV_REF(this_type) right)
		: _buf(boost::move(right._buf))
	{
	}

	packet(const this_type& right)
		: _buf(right._buf)
	{
	}

	~packet(void)
	{
	}

public:
	this_type& operator=(BOOST_RV_REF(buf_type) buf)
	{
		archive_helper_type::buf_copy(_buf, boost::move(buf));
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
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		archive_helper_type::buf_copy(_buf, boost::move(pak.org_buf()));
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(const packet<Other_Archive_Partner>& pak)
	{
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		archive_helper_type::buf_copy(_buf, pak.org_buf());
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(BOOST_RV_REF(ref_packet<Other_Archive_Partner>) pak)
	{
		typedef ref_packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		archive_helper_type::buf_copy(_buf, boost::move(pak.org_buf()));
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(const ref_packet<Other_Archive_Partner>& pak)
	{
		typedef ref_packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		archive_helper_type::buf_copy(_buf, pak.org_buf());
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		archive_helper_type::buf_copy(_buf, boost::move(right._buf));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		archive_helper_type::buf_copy(_buf, right._buf);
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(buf_type) right)
	{
		buf_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(buf_type& obuf)
	{
		archive_helper_type::buf_swap(_buf, obuf);
	}

	template<typename Other_Archive_Partner> inline
	void swap(BOOST_RV_REF(packet<Other_Archive_Partner>) right)
	{
		packet<Other_Archive_Partner>& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Other_Archive_Partner>
	void swap(packet<Other_Archive_Partner>& right)
	{
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		archive_helper_type::buf_swap(_buf, right.org_buf());
	}

	template<typename Other_Archive_Partner> inline
	void swap(BOOST_RV_REF(ref_packet<Other_Archive_Partner>) right)
	{
		ref_packet<Other_Archive_Partner>& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Other_Archive_Partner>
	void swap(ref_packet<Other_Archive_Partner>& right)
	{
		typedef ref_packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		archive_helper_type::buf_swap(_buf, right.org_buf());
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		archive_helper_type::buf_swap(_buf, right._buf);
	}

public:

	inline void clear(void)
	{
		_buf.clear();
	}

	inline buf_type& org_buf(void) const
	{
		return _buf;
	}

public:
	template<typename T> inline
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

	template<typename T> inline
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

#ifdef _DEBUG
		if(!archive_helper_type::template load<T>(t, _buf))
		{
			return false;
		}

		return true;
#else
		return archive_helper_type::template load<T>(t, _buf);
#endif // _DEBUG
	}

public:
	inline bool empty(void) const
	{
		return archive_helper_type::buf_empty(_buf);
	}

	inline size_type size(void) const
	{
		return archive_helper_type::buf_size(_buf);
	}

	inline this_type& resize(u32 size)
	{
		archive_helper_type::buf_resize(_buf, size);
		return *this;
	}

	inline buf_real_val_type* size_buf(u32 size)
	{
		resize(size);
		return archive_helper_type::buf_real_val_ptr(_buf);
	}

	inline buf_real_val_type* buf(void)
	{
		return archive_helper_type::buf_real_val_ptr(_buf);
	}

	inline const buf_real_val_type* buf(void) const
	{
		return archive_helper_type::buf_real_val_ptr(_buf);
	}

	inline buf_val_type* val_buf(void)
	{
		return archive_helper_type::buf_val_ptr(_buf);
	}

	inline const buf_val_type* val_buf(void) const
	{
		return archive_helper_type::buf_val_ptr();
	}

public:

#if !defined(YGGR_NO_CXX11_VARIADIC_TEMPLATES)

	template<typename Eins_Arg, typename ...Args> inline
	void assign(BOOST_FWD_REF(Eins_Arg) eins_arg, BOOST_FWD_REF(Args)... args)
	{
		archive_helper_type::buf_assign(_buf, eins_arg, boost::forward<Args>(args)...);
	}

#else

#	define YGGR_TMP_PP_EMPLACE_DEF(__n__) \
		template< YGGR_PP_FOO_TYPES_DEF(__n__) > inline \
		void assign(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS) ) { \
			archive_helper_type::buf_assign( \
				_buf, YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )); }

#	define BOOST_PP_LOCAL_MACRO( __n__ ) YGGR_TMP_PP_EMPLACE_DEF( __n__ )

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS (1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN)
#	include BOOST_PP_LOCAL_ITERATE()

#	undef YGGR_PP_FOO_ARG_NAME
#	undef YGGR_TMP_PP_EMPLACE_DEF

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	inline bool compare_eq(const buf_type& buf) const
	{
		return archive_helper_type::buf_compare(_buf, buf);
	}

	template<typename Other_Archive_Partner> inline
	bool compare_eq(const packet<Other_Archive_Partner>& pak) const
	{
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		return archive_helper_type::buf_compare(_buf, pak.org_buf());
	}

	template<typename Other_Archive_Partner> inline
	bool compare_eq(const ref_packet<Other_Archive_Partner>& pak) const
	{
		typedef ref_packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		return archive_helper_type::buf_compare(_buf, pak.org_buf());
	}

	inline bool compare_eq(const this_type& pak) const
	{
		return archive_helper_type::buf_compare(_buf, pak._buf);
	}

public:
	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		cypher(const Handler& handler)
	{
		return handler(boost::ref(_buf));
	}

protected:
	mutable buf_type _buf;
};

// non_member_foo

// io
template<typename Char, typename Traits, typename Archive_Partner>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const packet<Archive_Partner>& val)
{
	os << val.org_buf();
	return os;
}

//template<typename Char, typename Traits, typename Archive_Partner>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os,
//				BOOST_RV_REF_BEG packet<Archive_Partner> BOOST_RV_REF_END val)
//{
//	os << val.org_buf();
//	return os;
//}

// operator==

template<typename Archive_Partner> inline
bool operator==(const typename packet<Archive_Partner>::buf_type& l,
				const packet<Archive_Partner>& r)
{
	return r.compare_eq(l);
}

template<typename Archive_Partner> inline
bool operator==(const packet<Archive_Partner>& l,
				const typename packet<Archive_Partner>::buf_type& r)
{
	return l.compare_eq(r);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
bool operator==(const packet<Archive_Partner>& l,
				const ref_packet<Other_Archive_Partner>& r)
{
	return l.compare_eq(r);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
bool operator==(const packet<Archive_Partner>& l,
				const packet<Other_Archive_Partner>& r)
{
	return l.compare_eq(r);
}

template<typename Archive_Partner> inline
bool operator==(const packet<Archive_Partner>& l,
				const packet<Archive_Partner>& r)
{
	return l.compare_eq(r);
}

// operator!=

template<typename Archive_Partner> inline
bool operator!=(const typename packet<Archive_Partner>::buf_type& l,
				const packet<Archive_Partner>& r)
{
	return !r.compare_eq(l);
}

template<typename Archive_Partner> inline
bool operator!=(const packet<Archive_Partner>& l,
				const typename packet<Archive_Partner>::buf_type& r)
{
	return !l.compare_eq(r);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
bool operator!=(const packet<Archive_Partner>& l,
				const ref_packet<Other_Archive_Partner>& r)
{
	return !l.compare_eq(r);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
bool operator!=(const packet<Archive_Partner>& l,
				const packet<Other_Archive_Partner>& r)
{
	return !l.compare_eq(r);
}

template<typename Archive_Partner> inline
bool operator!=(const packet<Archive_Partner>& l,
				const packet<Archive_Partner>& r)
{
	return !l.compare_eq(r);
}

} // namespace packet
} // namespace yggr

namespace yggr
{
namespace packet
{
namespace swap_support
{

// buf - pak
// pak1 - pak2
// pak - pak

template<typename Archive_Partner> inline
void swap(packet<Archive_Partner>& l, typename packet<Archive_Partner>::buf_type& r)
{
	l.swap(r);
}

template<typename Archive_Partner> inline
void swap(typename packet<Archive_Partner>::buf_type& l, packet<Archive_Partner>& r)
{
	r.swap(l);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
void swap(packet<Archive_Partner>& l, packet<Other_Archive_Partner>& r)
{
	l.template swap(r);
}

template<typename Archive_Partner> inline
void swap(packet<Archive_Partner>& l, packet<Archive_Partner>& r)
{
	l.swap(r);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
void swap(packet<Archive_Partner>& l, ref_packet<Other_Archive_Partner>& r)
{
	l.template swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace packet
} // namespace yggr

namespace std
{
	using ::yggr::packet::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::packet::swap_support::swap;
} // namespace boost

//----------------------ref_packet---------------------------

namespace yggr
{
namespace packet
{

template<typename Archive_Partner>
class ref_packet
{

public:
	typedef Archive_Partner archive_partner_type;
	typedef typename archive_partner_type::archive_type archive_type;
	typedef typename archive_partner_type::archive_helper_type archive_helper_type;

	typedef typename archive_helper_type::buf_type buf_type;
	typedef typename archive_helper_type::buf_val_type buf_val_type;
	typedef typename archive_helper_type::buf_real_val_type buf_real_val_type;

private:
	typedef buf_type& buf_ref_type;

private:

	ERROR_MAKER_BEGIN("ref_packet")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_1)
			ERROR_CODE_DEF(E_invalid_argument)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_1)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_argument, "invalid argument")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

//-------------------------------------------------------------------------
private:
	typedef ref_packet this_type;

public:
	ref_packet(const buf_type& buf)
		: _buf(const_cast<buf_type&>(buf))
	{
	}

	template<typename Other_Archive_Partner>
	ref_packet(const packet<Other_Archive_Partner>& pak)
		: _buf(pak.org_buf())
	{
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));
	}

	template<typename Other_Archive_Partner>
	ref_packet(const ref_packet<Other_Archive_Partner>& pak)
		: _buf(pak.org_buf())
	{
		typedef ref_packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));
	}

	ref_packet(const this_type& right)
		: _buf(right._buf)
	{
	}

	~ref_packet(void)
	{
	}

public:
	this_type& operator=(BOOST_RV_REF(buf_type) buf)
	{
		archive_helper_type::buf_copy(_buf, boost::move(buf));
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
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		archive_helper_type::buf_copy(_buf, boost::move(pak.org_buf()));
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(const packet<Other_Archive_Partner>& pak)
	{
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		archive_helper_type::buf_copy(_buf, pak.org_buf());
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(BOOST_RV_REF(ref_packet<Other_Archive_Partner>) pak)
	{
		typedef ref_packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		archive_helper_type::buf_copy(_buf, boost::move(pak.org_buf()));
		return *this;
	}

	template<typename Other_Archive_Partner>
	this_type& operator=(const ref_packet<Other_Archive_Partner>& pak)
	{
		typedef ref_packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		archive_helper_type::buf_copy(_buf, pak.org_buf());
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		archive_helper_type::buf_copy(_buf, boost::move(right._buf));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}
		archive_helper_type::buf_copy(_buf, right._buf);
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(buf_type) right)
	{
		buf_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(buf_type& obuf)
	{
		archive_helper_type::buf_swap(_buf, obuf);
	}

	template<typename Other_Archive_Partner>
	inline void swap(BOOST_RV_REF(packet<Other_Archive_Partner>) right)
	{
		packet<Other_Archive_Partner>& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Other_Archive_Partner>
	void swap(packet<Other_Archive_Partner>& pak)
	{
		archive_helper_type::buf_swap(_buf, pak.org_buf());
	}

	template<typename Other_Archive_Partner>
	inline void swap(BOOST_RV_REF(ref_packet<Other_Archive_Partner>) right)
	{
		ref_packet<Other_Archive_Partner>& right_ref = right;
		this_type::swap(right_ref);
	}

	template<typename Other_Archive_Partner>
	void swap(ref_packet<Other_Archive_Partner>& right)
	{
		archive_helper_type::buf_swap(_buf, right.org_buf());
	}

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		archive_helper_type::buf_swap(_buf, right._buf);
	}

public:

	inline void clear(void)
	{
		_buf.clear();
	}

	inline buf_type& org_buf(void) const
	{
		return _buf;
	}

public:
	template<typename T> inline
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

	template<typename T> inline
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

public:
	inline bool empty(void) const
	{
		return archive_helper_type::buf_empty(_buf);
	}

	inline size_type size(void) const
	{
		return archive_helper_type::buf_size(_buf);
	}

	inline this_type& resize(u32 size)
	{
		archive_helper_type::buf_resize(_buf, size);
		return *this;
	}

	inline buf_real_val_type* size_buf(u32 size)
	{
		resize(size);
		return archive_helper_type::buf_real_val_ptr(_buf);
	}

	inline buf_real_val_type* buf(void)
	{
		return archive_helper_type::buf_real_val_ptr(_buf);
	}

	inline const buf_real_val_type* buf(void) const
	{
		return archive_helper_type::buf_real_val_ptr(_buf);
	}

	inline buf_val_type* val_buf(void)
	{
		return archive_helper_type::buf_val_ptr(_buf);
	}

	inline const buf_val_type* val_buf(void) const
	{
		return archive_helper_type::buf_val_ptr();
	}

public:
	inline bool compare_eq(const buf_type& buf) const
	{
		return archive_helper_type::buf_compare(_buf, buf);
	}

	template<typename Other_Archive_Partner> inline
	bool compare_eq(const packet<Other_Archive_Partner>& pak) const
	{
		typedef packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		return archive_helper_type::buf_compare(_buf, pak.org_buf());
	}

	template<typename Other_Archive_Partner> inline
	bool compare_eq(const ref_packet<Other_Archive_Partner>& pak) const
	{
		typedef ref_packet<Other_Archive_Partner> other_type;
		typedef typename other_type::buf_type oth_buf_type;
		BOOST_MPL_ASSERT((boost::is_same<buf_type, oth_buf_type >));

		return archive_helper_type::buf_compare(_buf, pak.org_buf());
	}

	inline bool compare_eq(const this_type& pak) const
	{
		return archive_helper_type::buf_compare(_buf, pak._buf);
	}

public:
	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		cypher(const Handler& handler)
	{
		return handler(boost::ref(_buf));
	}

protected:
	/*mutable*/ buf_ref_type _buf;
};


// non_member_foo

// io
template<typename Char, typename Traits, typename Archive_Partner>
std::basic_ostream<Char, Traits>&
	operator<<(std::basic_ostream<Char, Traits>& os,
				const ref_packet<Archive_Partner>& val)
{
	os << val.org_buf();
	return os;
}

//template<typename Char, typename Traits, typename Archive_Partner>
//std::basic_ostream<Char, Traits>&
//	operator<<(std::basic_ostream<Char, Traits>& os,
//				BOOST_RV_REF_BEG ref_packet<Archive_Partner> BOOST_RV_REF_END val)
//{
//	os << val.org_buf();
//	return os;
//}

// operator==

template<typename Archive_Partner> inline
bool operator==(const typename ref_packet<Archive_Partner>::buf_type& l,
				const ref_packet<Archive_Partner>& r)
{
	return r.compare_eq(l);
}

template<typename Archive_Partner> inline
bool operator==(const ref_packet<Archive_Partner>& l,
				const typename ref_packet<Archive_Partner>::buf_type& r)
{
	return l.compare_eq(r);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
bool operator==(const ref_packet<Archive_Partner>& l,
				const packet<Other_Archive_Partner>& r)
{
	return l.compare_eq(r);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
bool operator==(const ref_packet<Archive_Partner>& l,
				const ref_packet<Other_Archive_Partner>& r)
{
	return l.compare_eq(r);
}

template<typename Archive_Partner> inline
bool operator==(const ref_packet<Archive_Partner>& l,
				const ref_packet<Archive_Partner>& r)
{
	return l.compare_eq(r);
}

// operator!=

template<typename Archive_Partner> inline
bool operator!=(const typename ref_packet<Archive_Partner>::buf_type& l,
				const ref_packet<Archive_Partner>& r)
{
	return !r.compare_eq(l);
}

template<typename Archive_Partner> inline
bool operator!=(const ref_packet<Archive_Partner>& l,
				const typename ref_packet<Archive_Partner>::buf_type& r)
{
	return !l.compare_eq(r);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
bool operator!=(const ref_packet<Archive_Partner>& l,
				const packet<Other_Archive_Partner>& r)
{
	return !l.compare_eq(r);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
bool operator!=(const ref_packet<Archive_Partner>& l,
				const ref_packet<Other_Archive_Partner>& r)
{
	return !l.compare_eq(r);
}

template<typename Archive_Partner> inline
bool operator!=(const ref_packet<Archive_Partner>& l,
				const ref_packet<Archive_Partner>& r)
{
	return !l.compare_eq(r);
}


} // namespace packet
} // namespace yggr

namespace yggr
{
namespace packet
{
namespace swap_support
{
// buf - pak
// pak1 - pak2
// pak - pak


template<typename Archive_Partner> inline
void swap(ref_packet<Archive_Partner>& l, typename ref_packet<Archive_Partner>::buf_type& r)
{
	l.swap(r);
}

template<typename Archive_Partner> inline
void swap(typename ref_packet<Archive_Partner>::buf_type& l, ref_packet<Archive_Partner>& r)
{
	r.swap(l);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
void swap(ref_packet<Archive_Partner>& l, ref_packet<Other_Archive_Partner>& r)
{
	l.template swap(r);
}

template<typename Archive_Partner> inline
void swap(ref_packet<Archive_Partner>& l, ref_packet<Archive_Partner>& r)
{
	l.swap(r);
}

template<typename Archive_Partner, typename Other_Archive_Partner> inline
void swap(ref_packet<Archive_Partner>& l, packet<Other_Archive_Partner>& r)
{
	l.template swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace packet
} // namespace yggr

namespace std
{
	using ::yggr::packet::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::packet::swap_support::swap;
} // namespace boost

#endif //__YGGR_NETWORK_PACKET_HPP__
