//utf8_string_iterator.hpp

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

#ifndef __YGGR_CHARSET_UTF8_STRING_ITERATOR_HPP__
#define __YGGR_CHARSET_UTF8_STRING_ITERATOR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/compatibility/boost_iterator.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>

#include <yggr/mplex/static_assert.hpp>
#include <yggr/mplex/tag_sfinae.hpp>
#include <yggr/mplex/typename_expand.hpp>

#include <yggr/iterator_ex/is_iterator.hpp>

#include <yggr/bytes/detail/byte_size_detail_basic.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/charset_type_traits.hpp>
#include <yggr/charset/utf8_foo.hpp>

#include <yggr/charset/utf8_string_impl_decl.hpp>
#include <yggr/charset/utf8_string_view_decl.hpp>
#include <yggr/charset/utf8_string_iterator_decl.hpp>

#include <yggr/charset/utf8_char_impl_decl.hpp>
#include <yggr/charset/utf8_char_view_decl.hpp>
#include <yggr/charset/utf8_char_reference_decl.hpp>

#include <boost/ref.hpp>

#include <boost/iterator/reverse_iterator.hpp>
#include <boost/iterator/iterator_traits.hpp>

#include <boost/preprocessor/cat.hpp>
#include <boost/utility/enable_if.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/is_same.hpp>

#include <iterator>
#include <complex>
#include <cassert>


// charset_type_traits
namespace yggr
{
namespace charset
{

template<typename Char, typename Basic_Iterator>
struct is_utf8_string_iterator_t< utf8_string_iterator<Char, Basic_Iterator> >
	: public boost::mpl::true_
{
};

template<typename Char, typename Basic_Iterator>
struct is_utf8_string_const_iterator_t< utf8_string_const_iterator<Char, Basic_Iterator> >
	: public boost::mpl::true_
{
};

template<typename Char, typename Basic_Iterator>
struct is_utf8_string_modify_iterator_t< utf8_string_modify_iterator<Char, Basic_Iterator> >
	: public boost::mpl::true_
{
};

} // namespace charset
} // namesapce yggr

namespace yggr
{
namespace charset
{

namespace detail
{

template<typename Iterator>
struct base_iterator_maker
{
	typedef Iterator type;
	typedef type org_type;

	template<typename T> inline
	static org_type cast_to_org(T src)
	{
		return src;
	}

	inline static org_type& cast_to_org(org_type& src)
	{
		return src;
	}

	inline static org_type& cast_to_org(const org_type& src)
	{
		return src;
	}
};

template<typename Iterator>
struct base_const_iterator_maker
{
	typedef Iterator type;
	typedef type org_type;

	template<typename T> inline
	static org_type cast_to_org(T src)
	{
		return org_type(src);
	}

	inline static org_type& cast_to_org(org_type& src)
	{
		return src;
	}

	inline static org_type& cast_to_org(const org_type& src)
	{
		return src;
	}
};

#if BOOST_VERSION > 106600

template<typename T>
struct base_iterator_maker<T*>
{
	typedef
		boost::container::vec_iterator
		<
			typename boost::remove_cv<T*>::type,
			false
		> type;

	typedef T* org_type;

	template<typename Iter> inline
	static org_type cast_to_org(Iter t)
	{
		return t.operator->();
	}

	inline static org_type cast_to_org(org_type src)
	{
		return src;
	}
};

template<typename T>
struct base_const_iterator_maker<T*>
{
	typedef
		boost::container::vec_iterator
		<
			typename boost::remove_cv<T*>::type,
			true
		> type;

	typedef const T* org_type;

	template<typename Iter> inline
	static org_type cast_to_org(Iter t)
	{
		return t.operator->();
	}

	inline static org_type cast_to_org(org_type src)
	{
		return src;
	}
};

#elif BOOST_VERSION > 105400

template<typename T>
struct base_iterator_maker<T*>
{
	typedef
		boost::container::container_detail::vec_iterator
		<
			typename boost::remove_cv<T*>::type,
			false
		> type;

	typedef T* org_type;

	template<typename Iter> inline
	static org_type cast_to_org(Iter t)
	{
		return t.operator->();
	}

	inline static org_type cast_to_org(org_type src)
	{
		return src;
	}
};

template<typename T>
struct base_const_iterator_maker<T*>
{
	typedef
		boost::container::container_detail::vec_iterator
		<
			typename boost::remove_cv<T*>::type,
			true
		> type;

	typedef const T* org_type;

	template<typename Iter> inline
	static org_type cast_to_org(Iter t)
	{
		return t.operator->();
	}

	inline static org_type cast_to_org(org_type src)
	{
		return src;
	}
};

#else

template<typename T>
struct base_iterator_maker<T*>
{
	typedef
		boost::container::container_detail::vector_iterator
		<
			typename boost::remove_cv<T*>::type
		> type;

	typedef T* org_type;

	template<typename OT> inline
	static org_type cast_to_org(OT t)
	{
		return t.operator->();
	}

	inline static org_type cast_to_org(org_type src)
	{
		return src;
	}
};

template<typename T>
struct base_const_iterator_maker<T*>
{
	typedef
		boost::container::container_detail::vector_const_iterator
		<
			typename boost::remove_cv<T*>::type
		> type;

	typedef const T* org_type;

	template<typename OT> inline
	static org_type cast_to_org(OT t)
	{
		return t.operator->();
	}

	inline static org_type cast_to_org(org_type src)
	{
		return src;
	}
};
#endif // BOOST_VERSION <= 105400

} // namespace detail

} // namespace charset
} // namesapce yggr

// byte_size support

namespace yggr
{
namespace charset
{
namespace detail
{

template<typename Iter>
struct byte_size_utf8_iterator_foo
{
	typedef Iter iter_type;

	inline u64 operator()(iter_type s, iter_type e) const
	{
		return std::abs(std::distance(s.org(), e.org()));
	}
};

template<typename Iter, typename IsRiter>
struct byte_size_utf8_iterator_helper
	: public byte_size_utf8_iterator_foo<Iter>
{
};

template<typename Iter>
struct byte_size_utf8_iterator_helper<Iter, boost::mpl::true_>
{
	typedef Iter iter_type;

	inline u64 operator()(iter_type s, iter_type e) const
	{
		return std::abs(std::distance(e.base().org(), s.base().org()));
	}
};

} // namespace detail
} // namespace charset
} // namespace yggr

#define YGGR_PP_UTF8_ITERATORS_BYTE_SIZE_SUPPORT_DETAIL( __iter_t__, __is_class__ ) \
	namespace yggr{ namespace bytes{ namespace detail{ \
	template<typename Char, typename Basic_Iterator> \
	struct byte_size_iterator_helper< __iter_t__<Char, Basic_Iterator>, __is_class__ > \
		: public charset::detail::byte_size_utf8_iterator_helper< \
					__iter_t__< Char, Basic_Iterator >, boost::mpl::false_ > {}; \
	\
	template<typename Char, typename Basic_Iterator, \
				template<typename _Iter> class RIter > \
	struct byte_size_iterator_helper< \
			RIter< __iter_t__< Char, Basic_Iterator > >, __is_class__ > \
		: public charset::detail::byte_size_utf8_iterator_helper< \
					RIter< __iter_t__< Char, Basic_Iterator > >, \
					typename is_reverse_iterator< \
						RIter< __iter_t__< Char, Basic_Iterator > > >::type > {}; }}}


#define YGGR_PP_UTF8_ITERATORS_BYTE_SIZE_SUPPORT( __iter_t__ ) \
	YGGR_PP_UTF8_ITERATORS_BYTE_SIZE_SUPPORT_DETAIL(__iter_t__, true) \
	YGGR_PP_UTF8_ITERATORS_BYTE_SIZE_SUPPORT_DETAIL(__iter_t__, false)

//--------------iterator-------------------

namespace yggr
{
namespace charset
{

template<typename Char, typename Basic_Iterator>
class utf8_string_iterator
	: public detail::base_iterator_maker<Basic_Iterator>::type
{
	BOOST_MPL_ASSERT((is_utf8_char_t<Char>));

	template<typename OChar, typename OBasic_Iterator>
	friend class utf8_string_iterator;

	template<typename OChar, typename OBasic_Iterator>
	friend class utf8_string_modify_iterator;

	template<typename OChar, typename OBasic_Iterator>
	friend class utf8_string_const_iterator;

private:
	typedef detail::base_iterator_maker<Basic_Iterator> maker_type;

public:
	typedef typename maker_type::org_type org_type;
	typedef typename boost::iterator_value<org_type>::type org_value_type;
	typedef Char char_type;

public:
	typedef typename maker_type::type base_type;

	typedef typename base_type::value_type value_type;
	typedef typename base_type::difference_type difference_type;
	typedef typename base_type::pointer pointer;
	typedef typename base_type::iterator_category iterator_category; // it's a random_access_iterator

	typedef
		basic_utf8_char_view
		<
			typename char_type::value_type,
			typename char_type::traits_type
		> reference;

	typedef typename char_type::size_type size_type;

private:
	typedef utf8_string_iterator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	BOOST_MPL_ASSERT((boost::is_same<org_value_type, char>));

public:
	utf8_string_iterator(void)
	{
	}

	utf8_string_iterator(const org_type& right)
		: base_type(right)
	{
	}

	template<typename Utf8String>
	utf8_string_iterator(Utf8String& str, const org_type& right)
		: base_type(right)
	{
		assert(str.org_str().begin() <= right && right <= str.org_str().end());
	}

	utf8_string_iterator(const utf8_string_modify_iterator<char_type, org_type>& right)
		: base_type(right.org())
	{
	}

	utf8_string_iterator(BOOST_RV_REF(this_type) right)
		: base_type(static_cast<base_type&>(right))
	{
	}

	utf8_string_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~utf8_string_iterator(void)
	{
	}

public:
	this_type& operator=(const utf8_string_modify_iterator<char_type, org_type>& right)
	{
		base_type& l = *this;
		l = base_type(right.org());
		return *this;
	}

	this_type& operator=(const org_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type& l = *this;
		l = base_type(right);
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		const this_type& right_cref = right;
		return this_type::operator=(right_cref);
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type& l = *this;
		const base_type& r = right;

		l = r;
		return *this;
	}

public:
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

		base_type& l = *this;
		base_type& r = right;

		base_type tmp = l;
		l = r;
		r = tmp;
	}

public:
	inline reference operator*(void) const
	{
		return reference(this_type::org());
	}

	reference operator[](std::size_t off) const
	{
		org_type org_iter = this_type::org();
		for(difference_type i = 0; i != off; ++i)
		{
			org_iter += utf8_foo::charlen(org_iter);
		}

		return reference(org_iter);
	}

	inline org_type org(void) const
	{
		return maker_type::cast_to_org(*this);
	}

	inline this_type operator+(difference_type n) const
	{
		return this_type::prv_iterator_plus(n);
	}

	inline this_type& operator++(void)
	{
		base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
		return *this;
	}

	inline this_type operator++(int)
	{
		this_type tmp(*this);
		base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
		return tmp;
	}

	inline this_type& operator+=(difference_type n)
	{
		return this_type::prv_iterator_plus_set(n);
	}

	inline this_type operator-(difference_type n) const
	{
		return this_type::prv_iterator_plus(-n);
	}

	inline difference_type operator-(const this_type& right) const
	{
		const base_type& base = *this;
		const base_type& base_right = right;
		difference_type n = utf8_foo::strlen((std::min)(base, base_right), (std::max)(base, base_right));

		return base < right? -n : n;
	}

	this_type& operator--(void)
	{
		base_type& base = *this;
		do { --base;} while((*base & 0xc0) == 0x80);
		return *this;
	}

	this_type operator--(int)
	{
		this_type tmp(*this);
		base_type& base = *this;
		do { --base;} while((*base & 0xc0) == 0x80);
		return tmp;
	}

	inline this_type& operator-=(difference_type n)
	{
		return this_type::prv_iterator_plus_set(-n);
	}

	template<typename OChar, typename OBasic_Iterator> inline
	s32 compare(const utf8_string_modify_iterator<OChar, OBasic_Iterator>& right) const
	{
		typedef utf8_string_modify_iterator<OChar, OBasic_Iterator> right_type;
		typedef typename right_type::utf8_string_type right_utf8_string_type;

		if(!right._str_ptr)
		{
			return this_type::org() == org_type()? 0 : 1;
		}
		else
		{
			const right_utf8_string_type& str_ref = *(right._str_ptr);

			if(str_ref.org_str().begin() <= this_type::org()
				&& this_type::org() <= str_ref.org_str().end())
			{
				org_type rorg = right.org();
				return this_type::org() == rorg? 0 : this_type::org() < rorg? -1 : 1;
			}
			else
			{
				return this_type::org() < str_ref.org_str().begin()? -1 : 1;
			}
		}
	}

	template<typename OChar, typename OBasic_Iterator> inline
	s32 compare(const utf8_string_iterator<OChar, OBasic_Iterator>& right) const
	{
		return this_type::org() == right.org()? 0 : this_type::org() < right.org()? -1 : 1;
	}

	template<typename OChar, typename OBasic_Iterator> inline
	s32 compare(const utf8_string_const_iterator<OChar, OBasic_Iterator>& right) const
	{
		return this_type::org() == right.org()? 0 : this_type::org() < right.org()? -1 : 1;
	}

private:
	this_type prv_iterator_plus(difference_type n) const
	{
		base_type base(*this);

		if(n < 0)
		{
			for(difference_type i = 0, isize = std::abs(n); i != isize; ++i)
			{
				do { --base;} while((*base & 0xc0) == 0x80);
			}
		}
		else if(n > 0)
		{
			for(difference_type i = 0; i != n; ++i)
			{
				base += utf8_foo::charlen(base.operator->());
			}
		}

		this_type ret;
		static_cast<base_type&>(ret) = base;
		return ret;
	}

	this_type& prv_iterator_plus_set(difference_type n)
	{
		if(n < 0)
		{
			base_type& base = *this;
			for(difference_type i = 0, isize = std::abs(n); i != isize; ++i)
			{
				do { --base;} while((*base & 0xc0) == 0x80);
			}
		}
		else if(n > 0)
		{
			for(difference_type i = 0; i != n; ++i)
			{
				base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
			}
		}

		return *this;
	}
};

template<typename Char, typename Basic_Iterator> inline
utf8_string_iterator<Char, Basic_Iterator>
	operator+(typename utf8_string_iterator<Char, Basic_Iterator>::difference_type l,
				utf8_string_iterator<Char, Basic_Iterator> r)
{
	return (r += l);
}

// cmp
#define YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP1( __op__ ) \
	template<typename CharL, typename Basic_IteratorL, \
				typename CharR, typename Basic_IteratorR> inline \
	bool operator __op__(const utf8_string_iterator<CharL, Basic_IteratorL>& l, \
							const utf8_string_iterator<CharR, Basic_IteratorR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Basic_Iterator> inline \
	bool operator __op__(const utf8_string_iterator<Char, Basic_Iterator>& l, \
							const utf8_string_iterator<Char, Basic_Iterator>& r) { \
		return l.compare(r) __op__ 0; }


	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP1(==)
	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP1(!=)
	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP1(<)
	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP1(<=)
	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP1(>)
	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP1(>=)

#undef YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP1

#define YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP21( __op__ ) \
	template<typename CharL, typename Basic_IteratorL, \
				typename CharR, typename Basic_IteratorR> inline \
	bool operator __op__(const utf8_string_iterator<CharL, Basic_IteratorL>& l, \
							const utf8_string_const_iterator<CharR, Basic_IteratorR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Basic_Iterator> inline \
	bool operator __op__(const utf8_string_const_iterator<Char, Basic_Iterator>& l, \
							const utf8_string_iterator<Char, Basic_Iterator>& r) { \
		return r.compare(l) __op__ 0; }

#define YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP22( __op__, __rop__ ) \
	template<typename CharL, typename Basic_IteratorL, \
				typename CharR, typename Basic_IteratorR> inline \
	bool operator __op__(const utf8_string_iterator<CharL, Basic_IteratorL>& l, \
							const utf8_string_const_iterator<CharR, Basic_IteratorR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Basic_Iterator> inline \
	bool operator __op__(const utf8_string_const_iterator<Char, Basic_Iterator>& l, \
							const utf8_string_iterator<Char, Basic_Iterator>& r) { \
		return !(r.compare(l) __rop__ 0); }


	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP21(==)
	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP21(!=)

	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP22(<, <=)
	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP22(<=, <)
	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP22(>, >=)
	YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP22(>=, >)

#undef YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP21
#undef YGGR_TMP_PP_UTF8_STRING_ITERATOR_CMP22

} // namespace charset
} // namesapce yggr

// byte_size support

YGGR_PP_UTF8_ITERATORS_BYTE_SIZE_SUPPORT(charset::utf8_string_iterator)

// swap

namespace yggr
{
namespace charset
{
namespace swap_support
{

template<typename Char, typename Basic_Iterator> inline
void swap(utf8_string_iterator<Char, Basic_Iterator>& l,
			utf8_string_iterator<Char, Basic_Iterator>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace charset
} // namespace yggr

namespace std
{
	using ::yggr::charset::swap_support::swap;
} // naemspace std

namespace boost
{
	using ::yggr::charset::swap_support::swap;
} // namespace boost

//----------modify_iterator--------------

namespace yggr
{
namespace charset
{

template<typename Char, typename Basic_Iterator>
class utf8_string_modify_iterator
{
	BOOST_MPL_ASSERT((is_utf8_char_t<Char>));

	template<typename OChar, typename OBasic_Iterator>
	friend class utf8_string_iterator;

	template<typename OChar, typename OBasic_Iterator>
	friend class utf8_string_modify_iterator;

	template<typename OChar, typename OBasic_Iterator>
	friend class utf8_string_const_iterator;

public:
	typedef utf8_string_iterator<Char, Basic_Iterator> base_type;

	typedef typename base_type::org_type org_type;
	typedef typename base_type::org_value_type org_value_type;
	typedef typename base_type::char_type char_type;

	typedef
		utf8_string_impl
		<
			typename char_type::value_type,
			typename char_type::traits_type,
			typename mplex::typename_expand_get<char_type, 2>::type
		> utf8_string_type;

	typedef typename char_type::size_type size_type;

public:

	typedef typename base_type::value_type value_type;
	typedef typename base_type::difference_type difference_type;
	typedef typename base_type::pointer pointer;
	typedef typename base_type::iterator_category iterator_category; // it's a random_access_iterator

	typedef basic_utf8_char_reference<utf8_string_type> reference;

private:
	typedef utf8_string_modify_iterator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	typedef mplex::sfinae_type sfinae_type;

public:
	utf8_string_modify_iterator(void)
		: _str_ptr(0), _pos(0)
	{
	}

	utf8_string_modify_iterator(utf8_string_type& str)
		: _str_ptr(boost::addressof(str)), _pos(0)
	{
		assert(_str_ptr);
	}

	// if using size_type in boost::string pos and org_val conflict, because org_val is const char*
	template<typename Size>
	utf8_string_modify_iterator(utf8_string_type& str, Size pos,
									typename
										boost::enable_if
										<
											is_convertible_size_t<Size>,
											sfinae_type
										>::type sfinae = 0)
		: _str_ptr(boost::addressof(str)),
			_pos((std::min)(static_cast<size_type>(pos), str.length()))
	{
		assert(_str_ptr);
	}

	utf8_string_modify_iterator(utf8_string_type& str, const org_type& org_val)
		: _str_ptr(boost::addressof(str)),
			_pos(utf8_foo::strlen(str.org_str().begin(), org_val))
	{
		assert(_str_ptr);
	}

	utf8_string_modify_iterator(utf8_string_type& str, const base_type& right)
		: _str_ptr(boost::addressof(str)),
			_pos(std::distance(base_type(str.org_str().begin()), right))
	{
		assert(_str_ptr);
	}

	utf8_string_modify_iterator(BOOST_RV_REF(this_type) right)
		: _str_ptr(right._str_ptr), _pos(right._pos)
	{
		assert(_str_ptr);
	}

	utf8_string_modify_iterator(const this_type& right)
		: _str_ptr(right._str_ptr), _pos(right._pos)
	{
	}

	~utf8_string_modify_iterator(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		const this_type& right_cref = right;
		return this_type::operator=(right_cref);
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_str_ptr = right._str_ptr;
		_pos = right._pos;
		return *this;
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

		::yggr::swap(_str_ptr, right._str_ptr);
		::yggr::swap(_pos, right._pos);
	}

public:
	inline this_type& set(utf8_string_type& str, size_type pos)
	{
		assert(boost::addressof(str));

		_str_ptr = boost::addressof(str);
		_pos = (std::min)(pos, str.length());
		return *this;
	}

	inline this_type& set(utf8_string_type& str, const org_type& org_val)
	{
		assert(boost::addressof(str));

		_str_ptr = boost::addressof(str);
		_pos = utf8_foo::strlen(str.org_str().begin(), org_val);
		return *this;
	}

	inline this_type& set(utf8_string_type& str, const base_type& right)
	{
		assert(boost::addressof(str));
		_str_ptr = boost::addressof(str);
		_pos =std::distance(base_type(str.org_str().begin()), right);

		return *this;
	}

	inline reference operator*(void) const
	{
		assert(_str_ptr && static_cast<size_type>(_pos) < _str_ptr->length());

		return reference(*_str_ptr, _pos);
	}

	inline reference operator[](std::size_t off) const
	{
		assert(_str_ptr && static_cast<size_type>(_pos) + off < _str_ptr->length());
		return reference(*_str_ptr, _pos + off);
	}

	org_type org(void) const
	{
		if(!_str_ptr)
		{
			return org_type();
		}
		else
		{
			assert(static_cast<size_type>(_pos) <= _str_ptr->length());

			if(_pos == static_cast<difference_type>(_str_ptr->length()))
			{
				return _str_ptr->org_str().end();
			}

			org_type org_iter = _str_ptr->org_str().begin();
			for(difference_type i = 0; i != _pos; ++i)
			{
				std::advance(org_iter, utf8_foo::charlen(org_iter));
			}

			return org_iter;
		}
	}

	inline this_type operator+(difference_type n) const
	{
		assert(_str_ptr && static_cast<size_type>(_pos) + n <= _str_ptr->length());
		return this_type(*_str_ptr, _pos + n);
	}

	inline this_type& operator++(void)
	{
		assert(_str_ptr && static_cast<size_type>(_pos) + 1 <= _str_ptr->length());
		++_pos;
		return *this;
	}

	inline this_type operator++(int)
	{
		assert(_str_ptr && static_cast<size_type>(_pos) + 1 <= _str_ptr->length());

		this_type tmp(*this);
		++_pos;
		return tmp;
	}

	inline this_type& operator+=(difference_type n)
	{
		assert(_str_ptr && static_cast<size_type>(_pos) + n <= _str_ptr->length());

		_pos += n;
		return *this;
	}

	inline this_type operator-(difference_type n) const
	{
		assert(_str_ptr && _pos >= n);

		return this_type(*_str_ptr, _pos - n);
	}

	inline difference_type operator-(const this_type& right) const
	{
		assert(_str_ptr && _str_ptr == right._str_ptr);

		return _pos - right._pos;
	}

	inline this_type& operator--(void)
	{
		assert(_str_ptr && _pos > 0);

		--_pos;
		return *this;
	}

	inline this_type operator--(int)
	{
		assert(_str_ptr && _pos > 0);

		this_type tmp(*this);
		--_pos;
		return tmp;
	}

	inline this_type& operator-=(difference_type n)
	{
		assert(_str_ptr && _pos >= n);

		_pos -= n;
		return *this;
	}

	template<typename OChar, typename OBasic_Iterator> inline
	s32 compare(const utf8_string_modify_iterator<OChar, OBasic_Iterator>& right) const
	{
		if(_str_ptr && right._str_ptr)
		{
			if(_str_ptr == right._str_ptr)
			{
				return _pos == right._pos? 0 : (_pos < right._pos? -1 : 1);
			}
			else
			{
				return _str_ptr->data() < right._str_ptr->data()? -1 : 1;
			}
		}
		else
		{
			return _str_ptr == right._str_ptr? 0 : (_str_ptr < right._str_ptr ? -1 : 1);
		}
	}

	template<typename OChar, typename OBasic_Iterator> inline
	s32 compare(const utf8_string_iterator<OChar, OBasic_Iterator>& right) const
	{
		if(!_str_ptr)
		{
			return org_type() == right.org()? 0 : -1;
		}
		else
		{
			const utf8_string_type& str_ref = *_str_ptr;

			if(str_ref.org_str().begin() <= right.org()
				&& right.org() <= str_ref.org_str().end())
			{
				org_type lorg = this_type::org();
				return lorg == right.org()? 0 : (lorg < right.org()? -1 : 1);
			}
			else
			{
				return str_ref.org_str().begin() < right.org()? -1 : 1;
			}
		}
	}

	template<typename OChar, typename OBasic_Iterator> inline
	s32 compare(const utf8_string_const_iterator<OChar, OBasic_Iterator>& right) const
	{
		if(!_str_ptr)
		{
			return org_type() == right.org()? 0 : -1;
		}
		else
		{
			const utf8_string_type& str_ref = *_str_ptr;

			if(str_ref.org_str().begin() <= right.org()
				&& right.org() <= str_ref.org_str().end())
			{
				org_type lorg = this_type::org();
				return lorg == right.org()? 0 : (lorg < right.org()? -1 : 1);
			}
			else
			{
				return str_ref.org_str().begin() < right.org()? -1 : 1;
			}
		}
	}

	inline utf8_string_type& src_string(void) const
	{
		assert(_str_ptr);
		return *_str_ptr;
	}

protected:
	mutable utf8_string_type* _str_ptr;
	difference_type _pos;
};

template<typename Char, typename Basic_Iterator> inline
utf8_string_modify_iterator<Char, Basic_Iterator>
	operator+(typename utf8_string_modify_iterator<Char, Basic_Iterator>::difference_type l,
				utf8_string_modify_iterator<Char, Basic_Iterator> r)
{
	return (r += l);
}

// cmp
#define YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP1( __op__ ) \
	template<typename CharL, typename Basic_IteratorL, \
				typename CharR, typename Basic_IteratorR> inline \
	bool operator __op__(const utf8_string_modify_iterator<CharL, Basic_IteratorL>& l, \
										const utf8_string_modify_iterator<CharR, Basic_IteratorR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Basic_Iterator> inline \
	bool operator __op__(const utf8_string_modify_iterator<Char, Basic_Iterator>& l, \
										const utf8_string_modify_iterator<Char, Basic_Iterator>& r) { \
		return l.compare(r) __op__ 0; }


	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP1(==)
	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP1(!=)
	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP1(<)
	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP1(<=)
	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP1(>)
	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP1(>=)

#undef YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP1

#define YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP21( __op__ ) \
	template<typename CharL, typename Basic_IteratorL, \
				typename CharR, typename Basic_IteratorR> inline \
	bool operator __op__(const utf8_string_modify_iterator<CharL, Basic_IteratorL>& l, \
										const utf8_string_iterator<CharR, Basic_IteratorR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Basic_Iterator> inline \
	bool operator __op__(const utf8_string_iterator<Char, Basic_Iterator>& l, \
										const utf8_string_modify_iterator<Char, Basic_Iterator>& r) { \
		return r.compare(l) __op__ 0; } \
	\
	template<typename CharL, typename Basic_IteratorL, \
				typename CharR, typename Basic_IteratorR> inline \
	bool operator __op__(const utf8_string_modify_iterator<CharL, Basic_IteratorL>& l, \
										const utf8_string_const_iterator<CharR, Basic_IteratorR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Basic_Iterator> inline \
	bool operator __op__(const utf8_string_const_iterator<Char, Basic_Iterator>& l, \
										const utf8_string_modify_iterator<Char, Basic_Iterator>& r) { \
		return r.compare(l) __op__ 0; }

#define YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP22( __op__, __rop__ ) \
	template<typename CharL, typename Basic_IteratorL, \
				typename CharR, typename Basic_IteratorR> inline \
	bool operator __op__(const utf8_string_modify_iterator<CharL, Basic_IteratorL>& l, \
										const utf8_string_iterator<CharR, Basic_IteratorR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Basic_Iterator> inline \
	bool operator __op__(const utf8_string_iterator<Char, Basic_Iterator>& l, \
										const utf8_string_modify_iterator<Char, Basic_Iterator>& r) { \
		return !(r.compare(l) __rop__ 0); } \
	\
	template<typename CharL, typename Basic_IteratorL, \
				typename CharR, typename Basic_IteratorR> inline \
	bool operator __op__(const utf8_string_modify_iterator<CharL, Basic_IteratorL>& l, \
										const utf8_string_const_iterator<CharR, Basic_IteratorR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Basic_Iterator> inline \
	bool operator __op__(const utf8_string_const_iterator<Char, Basic_Iterator>& l, \
										const utf8_string_modify_iterator<Char, Basic_Iterator>& r) { \
		return !(r.compare(l) __rop__ 0); }


	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP21(==)
	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP21(!=)

	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP22(<, <=)
	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP22(<=, <)
	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP22(>, >=)
	YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP22(>=, >)

#undef YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP21
#undef YGGR_TMP_PP_UTF8_STRING_MODIFY_ITERATOR_CMP22

} // namespace charset
} // namesapce yggr

// byte_size support

YGGR_PP_UTF8_ITERATORS_BYTE_SIZE_SUPPORT(charset::utf8_string_modify_iterator)

// swap

namespace yggr
{
namespace charset
{
namespace swap_support
{

template<typename Char, typename Basic_Iterator> inline
void swap(utf8_string_modify_iterator<Char, Basic_Iterator>& l,
			utf8_string_modify_iterator<Char, Basic_Iterator>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace charset
} // namespace yggr

namespace std
{
	using ::yggr::charset::swap_support::swap;
} // naemspace std

namespace boost
{
	using ::yggr::charset::swap_support::swap;
} // namespace boost

//----------const_iterator---------------

namespace yggr
{
namespace charset
{

template<typename Char, typename Basic_Iterator>
class utf8_string_const_iterator
	: public detail::base_const_iterator_maker<Basic_Iterator>::type
{
	BOOST_MPL_ASSERT((is_utf8_char_t<Char>));

	template<typename OChar, typename OBasic_Iterator>
	friend class utf8_string_const_iterator;

private:
	typedef detail::base_const_iterator_maker<Basic_Iterator> maker_type;

public:
	typedef typename maker_type::org_type org_type;
	typedef typename boost::iterator_value<org_type>::type org_value_type;
	typedef Char char_type;

public:
	typedef typename maker_type::type base_type;

	typedef typename base_type::value_type value_type;
	typedef typename base_type::difference_type difference_type;
	typedef typename base_type::pointer pointer;
	typedef typename base_type::iterator_category iterator_category; // it's a random_access_iterator

	typedef
		basic_utf8_char_view
		<
			typename char_type::value_type,
			typename char_type::traits_type
		> reference;

	typedef typename char_type::size_type size_type;

private:
	typedef utf8_string_const_iterator this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	BOOST_MPL_ASSERT((boost::is_same<org_value_type, char>));

public:
	utf8_string_const_iterator(void)
	{
	}

	utf8_string_const_iterator(const org_type& right)
		: base_type(right)
	{
	}

	template<typename Utf8String>
	utf8_string_const_iterator(const Utf8String& str, const org_type& right)
		: base_type(right)
	{
		assert((str.org_str().begin() <= right && right <= str.org_str().end()));
	}

	template<typename TChar, typename NC_Basic_Iterator>
	utf8_string_const_iterator(const typename utf8_string_iterator<TChar, NC_Basic_Iterator>::org_type& iter)
		: base_type(iter)
	{
	}

	template<typename TChar, typename NC_Basic_Iterator>
	utf8_string_const_iterator(const utf8_string_iterator<TChar, NC_Basic_Iterator>& iter)
		: base_type(iter.org())
	{
	}

	template<typename TChar, typename NC_Basic_Iterator>
	utf8_string_const_iterator(const typename utf8_string_modify_iterator<TChar, NC_Basic_Iterator>::org_type& iter)
		: base_type(iter)
	{
	}

	template<typename TChar, typename NC_Basic_Iterator>
	utf8_string_const_iterator(const utf8_string_modify_iterator<TChar, NC_Basic_Iterator>& iter)
		: base_type(iter.org())
	{
	}

	utf8_string_const_iterator(BOOST_RV_REF(this_type) right)
		: base_type(static_cast<base_type&>(right))
	{
	}

	utf8_string_const_iterator(const this_type& right)
		: base_type(right)
	{
	}

	~utf8_string_const_iterator(void)
	{
	}

public:
	template<typename TChar, typename NC_Basic_Iterator>
	this_type& operator=(const typename utf8_string_modify_iterator<TChar, NC_Basic_Iterator>::org_type& right)
	{
		base_type& l = *this;
		l = base_type(right);
		return *this;
	}

	template<typename TChar, typename NC_Basic_Iterator>
	this_type& operator=(const utf8_string_modify_iterator<TChar, NC_Basic_Iterator>& right)
	{
		base_type& l = *this;
		l = base_type(right.org());
		return *this;
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		const this_type& right_cref = right;
		return this_type::operator=(right_cref);
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		base_type& l = *this;
		const base_type& r = right;

		l = r;
		return *this;
	}

public:
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

		base_type& l = *this;
		base_type& r = right;

		base_type tmp = l;
		l = r;
		r = tmp;
	}

public:
	inline reference operator*(void) const
	{
		return reference(this_type::org());
	}

	reference operator[](std::size_t off) const
	{
		org_type org_iter = this_type::org();
		for(difference_type i = 0; i != off; ++i)
		{
			org_iter += utf8_foo::charlen(org_iter);
		}

		return reference(org_iter);
	}

	inline org_type org(void) const
	{
		return maker_type::cast_to_org(*this);
	}

	inline this_type operator+(difference_type n) const
	{
		return this_type::prv_iterator_plus(n);
	}

	inline this_type& operator++(void)
	{
		base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
		return *this;
	}

	inline this_type operator++(int)
	{
		this_type tmp(*this);
		base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
		return tmp;
	}

	inline this_type& operator+=(difference_type n)
	{
		return this_type::prv_iterator_plus_set(n);
	}

	inline this_type operator-(difference_type n) const
	{
		return this_type::prv_iterator_plus(-n);
	}

	inline difference_type operator-(const this_type& right) const
	{
		const base_type& base = *this;
		const base_type& base_right = right;
		difference_type n = utf8_foo::strlen((std::min)(base, base_right), (std::max)(base, base_right));

		return base < right? -n : n;
	}

	this_type& operator--(void)
	{
		base_type& base = *this;
		do { --base;} while((*base & 0xc0) == 0x80);
		return *this;
	}

	this_type operator--(int)
	{
		this_type tmp(*this);
		base_type& base = *this;
		do { --base;} while((*base & 0xc0) == 0x80);
		return tmp;
	}

	inline this_type& operator-=(difference_type n)
	{
		return this_type::prv_iterator_plus_set(-n);
	}

	template<typename OChar, typename OBasic_Iterator> inline
	s32 compare(const utf8_string_modify_iterator<OChar, OBasic_Iterator>& right) const
	{
		typedef utf8_string_modify_iterator<OChar, OBasic_Iterator> right_type;
		typedef typename right_type::utf8_string_type right_utf8_string_type;

		if(!right._str_ptr)
		{
			return this_type::org() == org_type()? 0 : 1;
		}
		else
		{
			const right_utf8_string_type& str_ref = *(right._str_ptr);

			if(str_ref.org_str().begin() <= this_type::org()
				&& this_type::org() <= str_ref.org_str().end())
			{
				org_type rorg = right.org();
				return this_type::org() == rorg? 0 : this_type::org() < rorg? -1 : 1;
			}
			else
			{
				return this_type::org() < str_ref.org_str().begin()? -1 : 1;
			}
		}
	}

	template<typename OChar, typename OBasic_Iterator> inline
	s32 compare(const utf8_string_iterator<OChar, OBasic_Iterator>& right) const
	{
		return this_type::org() == right.org()? 0 : this_type::org() < right.org()? -1 : 1;
	}

	template<typename OChar, typename OBasic_Iterator> inline
	s32 compare(const utf8_string_const_iterator<OChar, OBasic_Iterator>& right) const
	{
		return this_type::org() == right.org()? 0 : this_type::org() < right.org()? -1 : 1;
	}


private:
	this_type prv_iterator_plus(difference_type n) const
	{
		base_type base(*this);

		if(n < 0)
		{
			for(difference_type i = 0, isize = std::abs(n); i != isize; ++i)
			{
				do { --base;} while((*base & 0xc0) == 0x80);
			}
		}
		else if(n > 0)
		{
			for(difference_type i = 0; i != n; ++i)
			{
				base += utf8_foo::charlen(base.operator->());
			}
		}

		this_type ret;
		static_cast<base_type&>(ret) = base;
		return ret;
	}

	this_type& prv_iterator_plus_set(difference_type n)
	{
		if(n < 0)
		{
			base_type& base = *this;
			for(difference_type i = 0, isize = std::abs(n); i != isize; ++i)
			{
				do { --base;} while((*base & 0xc0) == 0x80);
			}
		}
		else if(n > 0)
		{
			for(difference_type i = 0; i != n; ++i)
			{
				base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
			}
		}

		return *this;
	}

};

template<typename Char, typename Basic_Iterator> inline
utf8_string_const_iterator<Char, Basic_Iterator>
	operator+(typename utf8_string_const_iterator<Char, Basic_Iterator>::difference_type l,
				utf8_string_const_iterator<Char, Basic_Iterator> r)
{
	return (r += l);
}

// cmp
#define YGGR_TMP_PP_UTF8_STRING_CONST_ITERATOR_CMP1( __op__ ) \
	template<typename CharL, typename Basic_IteratorL, \
				typename CharR, typename Basic_IteratorR> inline \
	bool operator __op__(const utf8_string_const_iterator<CharL, Basic_IteratorL>& l, \
										const utf8_string_const_iterator<CharR, Basic_IteratorR>& r) { \
		return l.compare(r) __op__ 0; } \
	\
	template<typename Char, typename Basic_Iterator> inline \
	bool operator __op__(const utf8_string_const_iterator<Char, Basic_Iterator>& l, \
										const utf8_string_const_iterator<Char, Basic_Iterator>& r) { \
		return l.compare(r) __op__ 0; }


	YGGR_TMP_PP_UTF8_STRING_CONST_ITERATOR_CMP1(==)
	YGGR_TMP_PP_UTF8_STRING_CONST_ITERATOR_CMP1(!=)
	YGGR_TMP_PP_UTF8_STRING_CONST_ITERATOR_CMP1(<)
	YGGR_TMP_PP_UTF8_STRING_CONST_ITERATOR_CMP1(<=)
	YGGR_TMP_PP_UTF8_STRING_CONST_ITERATOR_CMP1(>)
	YGGR_TMP_PP_UTF8_STRING_CONST_ITERATOR_CMP1(>=)

#undef YGGR_TMP_PP_UTF8_STRING_CONST_ITERATOR_CMP1

} // namespace charset
} // namespace yggr

// byte_size support

YGGR_PP_UTF8_ITERATORS_BYTE_SIZE_SUPPORT(charset::utf8_string_const_iterator)

// swap

namespace yggr
{
namespace charset
{
namespace swap_support
{

template<typename Char, typename Basic_Iterator> inline
void swap(utf8_string_const_iterator<Char, Basic_Iterator>& l,
			utf8_string_const_iterator<Char, Basic_Iterator>& r)
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace charset
} // namespace yggr

namespace std
{
	using ::yggr::charset::swap_support::swap;
} // naemspace std

namespace boost
{
	using ::yggr::charset::swap_support::swap;
} // namespace boost

// byte_size support undef
#undef YGGR_PP_UTF8_ITERATORS_BYTE_SIZE_SUPPORT_DETAIL
#undef YGGR_PP_UTF8_ITERATORS_BYTE_SIZE_SUPPORT

#endif // __YGGR_CHARSET_UTF8_STRING_ITERATOR_HPP__
