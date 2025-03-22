//segment_container.hpp

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

#ifndef __YGGR_SEGMENT_SEGMENT_CONTAINER_HPP__
#define __YGGR_SEGMENT_SEGMENT_CONTAINER_HPP__

#include <yggr/segment/segment_config.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>

#include <yggr/charset/utf8_string.hpp>
#include <yggr/type_traits/native_t.hpp>

#include <yggr/segment/segment_op_proxy.hpp>

#include <cassert>
#include <utility>

namespace yggr
{
namespace segment
{
namespace detail
{

template<typename Seg, bool named_enable>
class basic_segment_container;

// named_enable
template<typename Seg>
class basic_segment_container<Seg, true>
	: protected std::pair<charset::utf8_string, Seg >
{
public:
	typedef std::pair<charset::utf8_string, Seg > base_type;

public:
	typedef typename base_type::first_type key_type;
	typedef typename base_type::second_type seg_type;
	typedef base_type org_type;

private:
	typedef segment_op_proxy<seg_type> seg_op_proxy_type;

private:
	typedef basic_segment_container this_type;

    basic_segment_container(this_type&);
    this_type& operator=(this_type &);
	BOOST_COPYABLE_AND_MOVABLE_ALT(this_type);

public:
	basic_segment_container(void)
	{
	}

	basic_segment_container(const key_type& name)
	{
		assert(!name.empty());
		base_type::first = name;
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename EArg, typename ...Args>
	basic_segment_container(const key_type& name,
							BOOST_FWD_REF(EArg) earg,
							BOOST_FWD_REF(Args) ...args)
	{
		if(name.empty())
		{
			return;
		}
		base_type::first = name;
		seg_op_proxy_type::s_create(
			base_type::second,
			name,
			boost::forward<EArg>(earg),
			boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		basic_segment_container(const key_type& name \
									YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS)) { \
			if(name.empty()) { return; } \
			base_type::first = name; \
			seg_op_proxy_type::s_create( base_type::second, name \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	basic_segment_container(BOOST_RV_REF(base_type) right)
	{
		base_type::first = boost::move(right.first);
		base_type::second = boost::move(right.second);
	}

	basic_segment_container(BOOST_RV_REF(this_type) right)
	{
		base_type::first = boost::move(right.first);
		base_type::second = boost::move(right.second);
	}

	~basic_segment_container(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type::first = boost::move(right.first);
		base_type::second = boost::move(right.second);

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type::first = boost::move(right.first);
		base_type::second = boost::move(right.second);

		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(base_type& right)
	{
		if(this == &right)
		{
			return;
		}
		yggr::swap(base_type::first, right.first);
		base_type::second.swap(right.second);
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
		yggr::swap(base_type::first, right.first);
		base_type::second.swap(right.second);
	}

public:
	inline const key_type& name(void) const
	{
		return base_type::first;
	}

	template<typename String> inline
	String name(void) const
	{
		typedef String now_string_type;
		return
			base_type::first.template str<now_string_type>(
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type));
	}

	inline const key_type& key(void) const
	{
		return base_type::first;
	}

	template<typename String> inline
	String key(void) const
	{
		typedef String now_string_type;
		return
			base_type::first.template str<now_string_type>(
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type));
	}

	inline seg_type& segment(void)
	{
		return base_type::second;
	}

	inline const seg_type& segment(void) const
	{
		return base_type::second;
	}

	inline size_type size(void) const
	{
		return seg_op_proxy_type::s_seg_size(base_type::second);
	}

	inline bool empty(void) const
	{
		return seg_op_proxy_type::s_seg_is_empty(base_type::second);
	}

	inline void free(void)
	{
		seg_op_proxy_type::s_remove(base_type::first);
		key_type key;
		base_type::first.swap(key);
		seg_op_proxy_type::s_create(base_type::second);
	}

	inline static bool is_named_enable_object(void)
	{
		return seg_op_proxy_type::s_is_named_enable_object();
	}
};

template<typename T>
class basic_segment_container<boost::interprocess::message_queue_t<T>, true>
	: protected
		std::pair
		<
			charset::utf8_string,
			::yggr::shared_ptr< boost::interprocess::message_queue_t<T> >
		>
{
private:
	typedef boost::interprocess::message_queue_t<T> now_seg_type;
	typedef ::yggr::shared_ptr<now_seg_type> now_seg_ptr_type;

public:
	typedef std::pair<charset::utf8_string, now_seg_ptr_type > base_type;

public:
	typedef typename base_type::first_type key_type;
	typedef now_seg_type seg_type;
	typedef base_type org_type;

private:
	typedef segment_op_proxy<seg_type> seg_op_proxy_type;

private:
	typedef basic_segment_container this_type;

    basic_segment_container(this_type&);
    this_type& operator=(this_type &);
	BOOST_COPYABLE_AND_MOVABLE_ALT(this_type);

public:
	basic_segment_container(void)
	{
		base_type::first = key_type("mq_empty", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		seg_op_proxy_type::s_create( base_type::second, base_type::first,
										segment_op::open_or_create(), 0, 0);
	}

	basic_segment_container(const key_type& name)
	{
		assert(!name.empty());
		base_type::first = name;
		seg_op_proxy_type::s_create( base_type::second, base_type::first,
										segment_op::open_or_create(), 0, 0);
	}

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template< typename EArg, typename ...Args>
	basic_segment_container(const key_type& name,
							BOOST_FWD_REF(EArg) earg,
							BOOST_FWD_REF(Args) ...args)
	{
		if(name.empty())
		{
			return;
		}
		base_type::first = name;
		seg_op_proxy_type::s_create(
			base_type::second,
			name,
			boost::forward<EArg>(earg),
			boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		basic_segment_container(const key_type& name \
									YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
									YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS)) { \
			if(name.empty()) { return; } \
			base_type::first = name; \
			seg_op_proxy_type::s_create( base_type::second, name \
					YGGR_PP_SYMBOL_IF(__n__, YGGR_PP_SYMBOL_COMMA) \
					YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 1, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	basic_segment_container(BOOST_RV_REF(base_type) right)
	{
		base_type::first = boost::move(right.first);
		base_type::second = boost::move(right.second);
	}

	basic_segment_container(BOOST_RV_REF(this_type) right)
	{
		base_type::first = boost::move(right.first);
		base_type::second = boost::move(right.second);
	}

	~basic_segment_container(void)
	{
		seg_op_proxy_type::s_remove(base_type::first);
	}

public:
	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type::first = boost::move(right.first);
		base_type::second = boost::move(right.second);

		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type::first = boost::move(right.first);
		base_type::second = boost::move(right.second);

		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(base_type& right)
	{
		if(this == &right)
		{
			return;
		}
		yggr::swap(base_type::first, right.first);
		base_type::second.swap(right.second);
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
		yggr::swap(base_type::first, right.first);
		base_type::second.swap(right.second);
	}

public:
	inline const key_type& name(void) const
	{
		return base_type::first;
	}

	template<typename String> inline
	String name(void) const
	{
		typedef String now_string_type;
		return
			base_type::first.template str<now_string_type>(
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type));
	}

	inline const key_type& key(void) const
	{
		return base_type::first;
	}

	template<typename String> inline
	String key(void) const
	{
		typedef String now_string_type;
		return
			base_type::first.template str<now_string_type>(
				YGGR_STR_STRING_DEFAULT_CHARSET_NAME(typename native_t<String>::type));
	}

	inline seg_type& segment(void)
	{
		assert(base_type::second);
		return *base_type::second;
	}

	inline const seg_type& segment(void) const
	{
		assert(base_type::second);
		return *base_type::second;
	}

	inline size_type size(void) const
	{
		return seg_op_proxy_type::s_seg_size(base_type::second);
	}

	inline bool empty(void) const
	{
		return seg_op_proxy_type::s_seg_is_empty(base_type::second);
	}

	inline void free(void)
	{
		seg_op_proxy_type::s_remove(base_type::first);
		key_type key("mq_empty", YGGR_STR_UTF8_STRING_CHARSET_NAME());
		base_type::first.swap(key);
		now_seg_ptr_type().swap(base_type::second);
		seg_op_proxy_type::s_create( base_type::second, base_type::first,
										segment_op::open_or_create(), 0, 0);
	}

	inline bool is_named_enable_object(void)
	{
		return seg_op_proxy_type::s_is_named_enable_object();
	}
};

// named_disable
template<typename Seg>
class basic_segment_container<Seg, false>
	: private nonable::noncopyable
{

public:
	typedef Seg seg_type;
	typedef charset::utf8_string key_type;
	typedef seg_type org_type;

private:
	typedef segment_op_proxy<seg_type> seg_op_proxy_type;

private:
	typedef basic_segment_container this_type;

	basic_segment_container(this_type&);
    this_type& operator=(this_type&);
    BOOST_COPYABLE_AND_MOVABLE_ALT(this_type)

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	basic_segment_container(BOOST_FWD_REF(Args) ...args)
	{
		seg_op_proxy_type::s_create(_seg, boost::forward<Args>(args)...);
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		basic_segment_container(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS)) { \
			seg_op_proxy_type::s_create( \
				_seg YGGR_PP_SYMBOL_IF( __n__, YGGR_PP_SYMBOL_COMMA ) \
				YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)); }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	basic_segment_container(BOOST_RV_REF(seg_type) seg)
		: _seg(boost::move(seg))
	{
	}

	basic_segment_container(BOOST_RV_REF(this_type) right)
		: _seg(boost::move(right._seg))
	{
	}

	~basic_segment_container(void)
	{
		seg_op_proxy_type::s_remove(_seg);
	}

public:
	this_type& operator=(BOOST_RV_REF(seg_type) right)
	{
		seg_type& right_chk = right;
		if(&_seg == &right_chk)
		{
			return *this;
		}

		_seg = boost::move(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		_seg = boost::move(right._seg);

		return *this;
	}

	inline void swap(BOOST_RV_REF(seg_type) right)
	{
		seg_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(seg_type& right)
	{
		if(&_seg == &right)
		{
			return;
		}

		_seg.swap(right._seg);
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

		_seg.swap(right._seg);
	}

public:
	inline key_type name(void) const
	{
		return key_type();
	}

	template<typename String> inline
	String name(void) const
	{
		typedef String now_string_type;
		return now_string_type();
	}

	inline key_type key(void) const
	{
		return key_type();
	}

	template<typename String> inline
	String key(void) const
	{
		typedef String now_string_type;
		return now_string_type();
	}

	inline seg_type& segment(void)
	{
		return _seg;
	}

	inline const seg_type& segment(void) const
	{
		return _seg;
	}

	inline size_type size(void) const
	{
		return seg_op_proxy_type::s_seg_size(_seg);
	}

	inline bool empty(void) const
	{
		return seg_op_proxy_type::s_seg_is_empty(_seg);
	}

	inline void free(void)
	{
		seg_op_proxy_type::s_remove(_seg);
	}

	inline static bool is_named_enable_object(void)
	{
		return seg_op_proxy_type::s_is_named_enable_object();
	}

protected:
	seg_type _seg;
};

} // namespace detail
} // namespace segment
} // namespace yggr

namespace yggr
{
namespace segment
{

template<typename Seg>
class segment_container
	: public
		detail::basic_segment_container
		<
			Seg,
			!!(segment_op_proxy<Seg>::E_is_named_object)
		>
{
protected:
	typedef
		detail::basic_segment_container
		<
			Seg,
			!!(segment_op_proxy<Seg>::E_is_named_object)
		> base_type;

public:
	typedef typename base_type::key_type key_type;
	typedef typename base_type::seg_type seg_type;
	typedef typename base_type::org_type org_type;

private:
	typedef segment_container this_type;

    segment_container(this_type&);
    this_type& operator=(this_type &);
	BOOST_COPYABLE_AND_MOVABLE_ALT(this_type);

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	segment_container(BOOST_FWD_REF(Args) ...args)
		: base_type(boost::forward<Args>(args)...)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		segment_container(YGGR_PP_FOO_PARAMS_DEF(__n__, YGGR_PP_FOO_ANYREF_PARAMS)) \
			: base_type(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD(__n__, YGGR_PP_SYMBOL_COMMA)) { }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE()
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	segment_container(BOOST_RV_REF(org_type) right)
		: base_type(boost::move(right))
	{
	}

	segment_container(BOOST_RV_REF(this_type) right)
		: base_type(boost::move(static_cast<base_type&>(right)))
	{
	}

	~segment_container(void)
	{
	}

public:
	this_type& operator=(BOOST_RV_REF(org_type) right)
	{
		base_type::operator=(boost::move(right));
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		base_type::operator=(boost::move(static_cast<base_type&>(right)));
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(org_type) right)
	{
		org_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(org_type& right)
	{
		base_type::swap(right);
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

		base_type::swap(right);
	}

public:
	using base_type::name;
	using base_type::key;
	using base_type::segment;
	using base_type::size;
	using base_type::free;
	using base_type::is_named_enable_object;
};

} // namespace segment
} // namespace yggr

namespace yggr
{
namespace segment
{
namespace swap_support
{

template<typename Seg> inline
void swap(typename segment_container<Seg>::org_type& l, segment_container<Seg>& r)
{
	r.swap(l);
}

template<typename Seg> inline
void swap(segment_container<Seg>& l, typename segment_container<Seg>::org_type& r)
{
	l.swap(r);
}

template<typename Seg> inline
void swap(segment_container<Seg>& l, segment_container<Seg>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace segment
} // namespace yggr

namespace std
{
	using ::yggr::segment::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::segment::swap_support::swap;
} // namespace boost

#endif // __YGGR_SEGMENT_SEGMENT_CONTAINER_HPP__
