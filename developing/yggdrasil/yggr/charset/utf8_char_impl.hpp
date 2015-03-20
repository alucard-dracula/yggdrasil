// utf8_char_impl.hpp

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

#ifndef __YGGR_CHARSET_UTF8_CHAR_IMPL_HPP__
#define __YGGR_CHARSET_UTF8_CHAR_IMPL_HPP__


#include <iostream>
#include <cassert>

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/functional/hash/hash.hpp>
#include <yggr/move/move.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/default_charset_config.hpp>
#include <yggr/charset/charset_converter.hpp>
#include <yggr/charset/charset_foo.hpp>
#include <yggr/charset/str_wstr_converter.hpp>
#include <yggr/charset/utf8_foo.hpp>
#include <yggr/mplex/iterator_to_value_t.hpp>
#include <yggr/math/sign.hpp>


namespace yggr
{

namespace charset
{

template<typename TChar, typename TTraits, typename TAlloc>
class utf8_char_impl;

template<typename TChar, typename TTraits, typename TAlloc>
class utf8_string_impl;

} // namespace charset

namespace charset
{

template<	typename TChar = char,
			typename TTraits = std::char_traits<TChar>,
			typename TAlloc = std::allocator<TChar>
		>
class utf8_char_impl
	: public basic_string<char, TTraits, TAlloc>
{
public:
	typedef TChar val_type;
	BOOST_MPL_ASSERT((boost::is_same<val_type, char>));
	typedef val_type value_type;
	typedef TTraits traits_type;
	typedef TAlloc alloc_type;
	typedef alloc_type allocator_type;
protected:
	typedef basic_string<val_type, traits_type, alloc_type> base_type;

	//typedef typename base_type::allocator_type allocator_type;
	//typedef typename base_type::value_type value_type;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;
	typedef typename base_type::pointer pointer;
	typedef typename base_type::iterator iterator;
	typedef typename base_type::const_iterator const_iterator;
	typedef typename base_type::reverse_iterator reverse_iterator;
	typedef typename base_type::const_reverse_iterator const_reverse_iterator;
	typedef typename base_type::difference_type difference_type;
	typedef typename base_type::size_type size_type;

public:

protected:
	enum
	{
		E_max_size = 5,
		E_compile_u32 = 0xffffffff
	};

private:
	typedef utf8_char_impl this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	utf8_char_impl(void)
	{
	}

	explicit utf8_char_impl(const alloc_type& alloc)
		: base_type(alloc)
	{
	}

	template<typename Char>
	utf8_char_impl(Char c, const alloc_type& alloc = alloc_type())
		: base_type(alloc)
	{
		Char src[2] = {c};
		this_type::assign(src);
	}

	template<typename Char>
	utf8_char_impl(const Char* src,
					const string& src_charset_name = charset_name_t<Char>(),
					const alloc_type& alloc = alloc_type())
		: base_type(alloc)
	{
		this_type::assign(src, src_charset_name);
	}

	template<typename Char>
	utf8_char_impl(const Char* src, const alloc_type& alloc)
		: base_type(alloc)
	{
		this_type::assign(src);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String
				>
	utf8_char_impl(const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name = charset_name_t<Char>(),
					const alloc_type& alloc = alloc_type())
		: base_type(alloc)
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String
			>
	utf8_char_impl(const Basic_String<Char, Traits, Alloc>& src,
						const alloc_type& alloc)
		: base_type(alloc)
	{
		this_type::assign(src);
	}

	template<typename InputIter>
	utf8_char_impl(InputIter start, InputIter last,
					const string& src_charset_name = charset_name_t<typename mplex::iterator_to_value_t<InputIter>::type>(),
					const alloc_type& alloc = alloc_type())
		: base_type(alloc)
	{
		this_type::assign(start, last, src_charset_name);
	}

	template<typename InputIter>
	utf8_char_impl(InputIter start, InputIter last, const alloc_type& alloc)
		: base_type(alloc)
	{
		this_type::assign(start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_char_impl(const utf8_char_impl<Char, Traits, Alloc>& other,
					const alloc_type& alloc = alloc_type())
		: base_type(other.begin(), other.end(), alloc)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	explicit utf8_char_impl(BOOST_RV_REF(this_type) right,
					const alloc_type& alloc = alloc_type())
		: base_type(boost::forward<base_type>(right), alloc)
	{
	}
#else 
	explicit utf8_char_impl(BOOST_RV_REF(this_type) right,
					const alloc_type& alloc = alloc_type())
		: base_type(alloc)
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
	}
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES

	utf8_char_impl(const this_type& right)
		: base_type(right)
	{
	}

	utf8_char_impl(const this_type& right, const alloc_type& alloc)
		: base_type(right, alloc)
	{
	}

	~utf8_char_impl(void)
	{
	}

	//operator type

	operator bool(void) const
	{
		return !(base_type::empty()
					|| utf8_foo::charlen(base_type::c_str()) == 0);
	}

	// =

	this_type& operator=(char c)
	{
		char str[2] = {c};
		return this_type::assign(str);
	}

	template<typename Char>
	this_type& operator=(const Char* str)
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::assign(string_type(str));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String
			>
	this_type& operator=(const Basic_String<Char, Traits, Alloc>& str)
	{
		return this_type::assign(str);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& operator=(const utf8_char_impl<Char, Traits, Alloc>& other)
	{
		return this_type::assign(other);
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		return this_type::assign(boost::forward<this_type>(right));
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		return this_type::assign(right);
	}

	// to_string

	template<typename String>
	String to_string(const string& dst_charset_name = charset_name_t<typename String::value_type>()) const
	{
		typedef String string_type;
		typedef typename string_type::value_type val_type;
		const base_type& base = *this;

		return YGGR_PP_CHARSET_FOO_XCHG(string_type, base, IN_CHARSET_NAME, dst_charset_name);
	}

	// assign

	template<typename Char>
	this_type& assign(const Char* src,
						const string& src_charset_name = charset_name_t<Char>())
	{
		return this_type::assign<Char, std::char_traits<Char>, std::allocator<Char>,
									charset::basic_string>(charset::basic_string<Char>(src), src_charset_name);
	}

	template<	typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String
			>
	this_type& assign(const Basic_String<Char, Traits, Alloc>& src,
						const string& src_charset_name = charset_name_t<Char>())
	{
		if(src.empty())
		{
			return *this;
		}
		base_type right(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
		assert(utf8_foo::is_only_char(right));
		base_type::swap(right);
		return *this;
	}

	template<typename InputIter>
	this_type& assign(InputIter start, InputIter last,
						const string& src_charset_name = charset_name_t<typename mplex::iterator_to_value_t<InputIter>::type>())
	{
		typedef InputIter iter_type;
		typedef typename mplex::iterator_to_value_t<iter_type>::type iter_val_type;
		typedef charset::basic_string<iter_val_type> string_type;

		string_type src(start, last);
		if(src.empty())
		{
			return *this;
		}

		base_type right(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
		assert(utf8_foo::is_only_char(right));
		base_type::swap(right);
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(typename utf8_string_impl<Char, Traits, Alloc>::iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		assert(last - start == 1);
		base_type::assign(start.base(), last.base());
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(typename utf8_string_impl<Char, Traits, Alloc>::const_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::const_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		assert(last - start == 1);
		base_type::assign(start.base(), last.base());
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(typename utf8_string_impl<Char, Traits, Alloc>::reverse_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		assert(std::abs(last - start) == 1);
		base_type::assign(&(*start), &(*start) + utf8_foo::charlen(&(*start)));
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(typename utf8_string_impl<Char, Traits, Alloc>::const_reverse_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::const_reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		assert(std::abs(last - start) == 1);
		base_type::assign(&(*start), &(*start) + utf8_foo::charlen(&(*start)));
		return *this;
	}

	this_type& assign(typename utf8_string_impl<val_type, traits_type, alloc_type>::iterator start,
						typename utf8_string_impl<val_type, traits_type, alloc_type>::iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		assert(last - start == 1);
		base_type::assign(start.base(), last.base());
		return *this;
	}

	this_type& assign(typename utf8_string_impl<val_type, traits_type, alloc_type>::const_iterator start,
						typename utf8_string_impl<val_type, traits_type, alloc_type>::const_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		assert(last - start == 1);
		base_type::assign(start.base(), last.base());
		return *this;
	}

	this_type& assign(typename utf8_string_impl<val_type, traits_type, alloc_type>::reverse_iterator start,
						typename utf8_string_impl<val_type, traits_type, alloc_type>::reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		assert(std::abs(last - start) == 1);
		base_type::assign(&(*start), &(*start) + utf8_foo::charlen(&(*start)));
		return *this;
	}

	this_type& assign(typename utf8_string_impl<val_type, traits_type, alloc_type>::const_reverse_iterator start,
						typename utf8_string_impl<val_type, traits_type, alloc_type>::const_reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		assert(std::abs(last - start) == 1);
		base_type::assign(&(*start), &(*start) + utf8_foo::charlen(&(*start)));
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(const utf8_char_impl<Char, Traits, Alloc>& other)
	{
		base_type::assign(&(*other.begin()), &(*other.begin()) + other.size());
		return *this;
	}

	this_type& assign(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::assign(boost::forward<base_type>(right));
#else
		base_type& right_ref = right;
		base_type::swap(right);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& assign(const this_type& right)
	{
		base_type::assign(right);
		return *this;
	}

	// compare

	template<typename Char>
	s32 compare(Char c) const
	{
		Char src[2] = {c};
		return this_type::compare(src);
	}

	template<typename Char>
	s32 compare(const Char* src,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef basic_string<Char> string_type;
		return this_type::compare(string_type(src), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String
			>
	s32 compare(const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		base_type right(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
		return base_type::compare(right);
	}

	template<typename Char, typename Traits, typename Alloc>
	s32 compare(const utf8_char_impl<Char, Traits, Alloc>& other) const
	{
		return base_type::compare(other.c_str());
	}

	s32 compare(const this_type& right) const
	{
		return base_type::compare(right);
	}

	//swap

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//base_type::swap(boost::forward<base_type>(right));
		base_type::swap(right);
#else
		this_type& right_ref = right;
		base_type::swap(right_ref);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		base_type::swap(right);
	}

	alloc_type get_allocator(void) const
	{
		return base_type::get_allocator();
	}
};

} // namespace charset
} // namespace yggr

// ==
template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc>
bool operator==(const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& left, Char right)
{
	return 0 == left.compare(right);
}

template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc>
bool operator==(Char left, const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right)
{
	return 0 == right.compare(left);
}

template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc>
bool operator==(const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& left, const Char* right)
{
	return 0 == left.compare(right);
}

template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc>
bool operator==(const Char* left, const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right)
{
	return 0 == right.compare(left);
}

template<	typename Char_uc, typename Traits_uc, typename Alloc_uc,
			typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String
		>
bool operator==(const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& left,
					const Basic_String<Char, Traits, Alloc>& right)
{
	return 0 == left.compare(right);
}

template<	typename Char_uc, typename Traits_uc, typename Alloc_uc,
			typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String
		>
bool operator==(const Basic_String<Char, Traits, Alloc>& left,
					const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right)
{
	return 0 == right.compare(left);
}

template<	typename Char,
			typename Traits1, typename Alloc1,
			typename Traits2, typename Alloc2
		>
bool operator==(const yggr::charset::utf8_char_impl<Char, Traits1, Alloc1>& left,
					const yggr::charset::utf8_char_impl<Char, Traits2, Alloc2>& right)
{
	return 0 == left.compare(right);
}

template<typename Char, typename Traits, typename Alloc>
bool operator==(const yggr::charset::utf8_char_impl<Char, Traits, Alloc>& left,
					const yggr::charset::utf8_char_impl<Char, Traits, Alloc>& right)
{
	return 0 == left.compare(right);
}

// !=
template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc>
bool operator!=(const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& left, Char right)
{
	return 0 != left.compare(right);
}

template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc>
bool operator!=(Char left, const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right)
{
	return 0 != right.compare(left);
}

template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc>
bool operator!=(const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& left, const Char* right)
{
	return 0 != left.compare(right);
}

template<typename Char, typename Char_uc, typename Traits_uc, typename Alloc_uc>
bool operator!=(const Char* left, const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right)
{
	return 0 != right.compare(left);
}

template<	typename Char_uc, typename Traits_uc, typename Alloc_uc,
			typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String
		>
bool operator!=(const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& left,
					const Basic_String<Char, Traits, Alloc>& right)
{
	return 0 != left.compare(right);
}

template<	typename Char_uc, typename Traits_uc, typename Alloc_uc,
			typename Char, typename Traits, typename Alloc,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String
		>
bool operator!=(const Basic_String<Char, Traits, Alloc>& left,
					const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right)
{
	return 0 != right.compare(left);
}

template<	typename Char,
			typename Traits1, typename Alloc1,
			typename Traits2, typename Alloc2
		>
bool operator!=(const yggr::charset::utf8_char_impl<Char, Traits1, Alloc1>& left,
					const yggr::charset::utf8_char_impl<Char, Traits2, Alloc2>& right)
{
	return 0 != left.compare(right);
}

template<typename Char, typename Traits, typename Alloc>
bool operator!=(const yggr::charset::utf8_char_impl<Char, Traits, Alloc>& left,
					const yggr::charset::utf8_char_impl<Char, Traits, Alloc>& right)
{
	return 0 != left.compare(right);
}

// swap
#define YGGR_PP_CHARSET_UTF8_CHAR_IMPL_SWAP() \
template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline \
void swap(yggr::charset::utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& l, \
			BOOST_RV_REF(yggr::charset::utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>) r) { \
	l.swap(r); } \
	\
template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline \
void swap(BOOST_RV_REF(yggr::charset::utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>) l, \
			yggr::charset::utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& r) { \
	r.swap(l); } \
	\
template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline \
void swap(yggr::charset::utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& l, \
			yggr::charset::utf8_char_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& r) { \
	l.swap(r); }

namespace std
{
	YGGR_PP_CHARSET_UTF8_CHAR_IMPL_SWAP()
} // namespace std

namespace boost
{
	YGGR_PP_CHARSET_UTF8_CHAR_IMPL_SWAP()
} // namespace boost

#undef YGGR_PP_CHARSET_UTF8_CHAR_IMPL_SWAP
// io

template<typename Char, typename Traits, typename Char_uc, typename Traits_uc, typename Alloc_uc>
std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os,
												const yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right)
{
	os << right.template to_string< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits, typename Char_uc, typename Traits_uc, typename Alloc_uc>
std::basic_istream<Char, Traits>& operator>>(std::basic_istream<Char, Traits>& is,
												yggr::charset::utf8_char_impl<Char_uc, Traits_uc, Alloc_uc>& right)
{
	yggr::charset::basic_string<Char> str;
	is >> str;
	right.assign(str);
	return is;
}


#endif // __YGGR_CHARSET_UTF8_CHAR_IMPL_HPP__
