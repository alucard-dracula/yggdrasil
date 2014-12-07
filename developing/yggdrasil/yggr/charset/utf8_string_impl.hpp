//utf8_string_impl.hpp

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

#ifndef __YGGR_CHARSET_UTF8_STRING_IMPL_HPP__
#define __YGGR_CHARSET_UTF8_STRING_IMPL_HPP__

#include <yggr/support/max_min_undef.ipp>

#include <iostream>
#include <algorithm>
#include <iterator>
#include <cassert>

#include <yggr/move/move.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/container/vector.hpp>

#include <boost/unordered_set.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/functional/hash/hash.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/charset/string.hpp>
#include <yggr/charset/utf8_char_impl.hpp>
#include <yggr/ppex/typedef.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/serialization/nvp.hpp>

namespace yggr
{
namespace charset
{

template<typename TChar, typename TTraits, typename TAlloc>
class utf8_char_impl;

template<typename TChar, typename TTraits, typename TAlloc>
class utf8_string_impl;

} // namesapce charset

namespace charset
{

template<	typename TChar = char,
			typename TTraits = std::char_traits<TChar>,
			typename TAlloc = std::allocator<TChar>
		>
class utf8_string_impl
	: public basic_string<TChar, TTraits, TAlloc>
{
public:
	typedef TChar val_type;
	BOOST_MPL_ASSERT((boost::is_same<val_type, char>));
	typedef TTraits traits_type;
	typedef TAlloc alloc_type;
public:
	typedef basic_string<val_type, traits_type, alloc_type> base_type;

protected:
	typedef typename base_type::iterator base_iterator;
	typedef typename base_type::const_iterator base_const_iterator;

public:

	typedef typename base_type::allocator_type allocator_type;
	typedef typename base_type::value_type value_type;
	typedef typename base_type::reference reference;
	typedef typename base_type::const_reference const_reference;
	typedef typename base_type::pointer pointer;
	typedef typename base_type::const_pointer const_pointer;
	//typedef typename base_type::iterator iterator; // discard
	//typedef typename base_type::const_iterator const_iterator; // discard
	//typedef typename base_type::reverse_iterator reverse_iterator; // discard
	//typedef typename base_type::const_reverse_iterator const_reverse_iterator; // discard
	typedef typename base_type::difference_type difference_type;
	typedef typename base_type::size_type size_type;

public:
	typedef utf8_char_impl<val_type, traits_type, alloc_type> char_type;

public:
	class iterator
		: public boost::mpl::if_c<	boost::is_pointer<base_iterator>::value,
									boost::container::container_detail::vector_iterator<base_iterator>,
									base_iterator
								>::type
	{
	public:
		typedef typename boost::mpl::if_c<	boost::is_pointer<base_iterator>::value,
											boost::container::container_detail::vector_iterator<base_iterator>,
											base_iterator >::type base_type;

		typedef typename base_type::value_type value_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::reference reference;
		typedef typename base_type::iterator_category iterator_category;

	private:
		typedef iterator this_type;

	public:
		iterator(void)
		{
		}

		iterator(pointer ptr)
			: base_type(ptr)
		{
		}

		iterator(const base_type& base)
			: base_type(base)
		{
		}

		iterator(const this_type& right)
			: base_type(right)
		{
		}

		~iterator(void)
		{
		}

		this_type& operator=(const this_type& right)
		{
			if(this == &right) {return *this;}
			base_type& l = *this;
			const base_type& r = right;

			l = r;
			return *this;
		}

		base_type& base(void)
		{
			return *this;
		}

		const base_type& base(void) const
		{
			return *this;
		}

		char_type get_value(void) const
		{
			return char_type(base_type::operator->(),
								base_type::operator->() + utf8_foo::charlen(base_type::operator->()),
								IN_CHARSET_NAME);
		}

		this_type operator+(difference_type n) const
		{
			base_type base(*this);

			for(difference_type i = 0; i != n; ++i)
			{
				base += utf8_foo::charlen(base.operator->());
			}

			return this_type(base);
		}

		this_type& operator++(void)
		{
			base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
			return *this;
		}

		this_type operator++(int)
		{
			this_type tmp(*this);
			base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
			return tmp;
		}

		this_type& operator+=(difference_type n)
		{
			for(difference_type i = 0; i != n; ++i)
			{
				base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
			}

			return *this;
		}

		this_type operator-(difference_type n) const
		{
			base_type base(*this);
			for(difference_type i = 0; i != n; ++i)
			{
				do { --base;} while((*base & 0xc0) == 0x80);
			}

			return this_type(base);
		}

		difference_type operator-(const this_type& right) const
		{
			const base_type& base = *this;
			const base_type& base_right = right;
			difference_type n = utf8_foo::strlen(std::min(base, base_right), std::max(base, base_right));

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

		this_type& operator-=(difference_type n)
		{
			base_type& base = *this;
			for(difference_type i = 0; i != n; ++i)
			{
				do { --base;} while((*base & 0xc0) == 0x80);
			}

			return *this;
		}
	};

	class const_iterator
		: public boost::mpl::if_c< boost::is_pointer<base_const_iterator>::value,
									boost::container::container_detail::vector_const_iterator<base_const_iterator>,
									base_const_iterator
								>::type
	{
	public:
		typedef typename boost::mpl::if_c< boost::is_pointer<base_const_iterator>::value,
											boost::container::container_detail::vector_const_iterator<base_const_iterator>,
											base_const_iterator
											>::type base_type;

		typedef typename base_type::value_type value_type;
		typedef typename base_type::difference_type difference_type;
		typedef typename base_type::pointer pointer;
		typedef typename base_type::reference reference;
		typedef typename base_type::iterator_category iterator_category;

	private:
		typedef const_iterator this_type;

	public:
		const_iterator(void)
		{
		}

		const_iterator(pointer ptr)
			: base_type(ptr)
		{
		}

#		if defined (YGGR_USE_STL_STRING)
		const_iterator(const iterator& iter)
			: base_type(iter)
		{
		}
#		elif defined (YGGR_USE_BOOST_STRING)
		const_iterator(const iterator& iter)
			: base_type(iter.operator->())
		{
		}
#		else
#			error "please input your suppor string code"
#		endif // YGGR_USE_STL_STRING

		const_iterator(const base_type& base)
			: base_type(base)
		{
		}

		const_iterator(const this_type& right)
			: base_type(right)
		{
		}

		~const_iterator(void)
		{
		}

		this_type& operator=(const iterator& right)
		{
			base_type& l = *this;
			l = right.operator->();
			return *this;
		}

		this_type& operator=(const this_type& right)
		{
			base_type& l = *this;
			const base_type& r = right;

			l = r;
			return *this;
		}

		base_type& base(void)
		{
			return *this;
		}

		const base_type& base(void) const
		{
			return *this;
		}

		char_type get_value(void) const
		{
			return char_type(base_type::operator->(),
								base_type::operator->() + utf8_foo::charlen(base_type::operator->()),
								IN_CHARSET_NAME);
		}

		this_type operator+(difference_type n) const
		{
			base_type base(*this);

			for(difference_type i = 0; i != n; ++i)
			{
				base += utf8_foo::charlen(base.operator->());
			}

			return this_type(base);
		}

		this_type& operator++(void)
		{
			base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
			return *this;
		}

		this_type operator++(int)
		{
			this_type tmp(*this);
			base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
			return tmp;
		}

		this_type& operator+=(difference_type n)
		{
			for(difference_type i = 0; i != n; ++i)
			{
				base_type::operator+=(utf8_foo::charlen(base_type::operator->()));
			}

			return *this;
		}

		this_type operator-(difference_type n) const
		{
			base_type base(*this);
			for(difference_type i = 0; i != n; ++i)
			{
				do { --base;} while((*base & 0xc0) == 0x80);
			}

			return this_type(base);
		}

		difference_type operator-(const this_type& right) const
		{
			const base_type& base = *this;
			const base_type& base_right = right;
			difference_type n = utf8_foo::strlen(std::min(base, base_right), std::max(base, base_right));

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

		this_type& operator-=(difference_type n)
		{
			base_type& base = *this;
			for(difference_type i = 0; i != n; ++i)
			{
				do { --base;} while((*base & 0xc0) == 0x80);
			}

			return *this;
		}
	};

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;



private:
	typedef utf8_string_impl this_type;

	BOOST_COPYABLE_AND_MOVABLE(this_type)
public:
	utf8_string_impl(void)
		: _utf8_size(size_type())
	{
	}

	explicit utf8_string_impl(const alloc_type& alloc)
		: base_type(alloc), _utf8_size(size_type())
	{
	}

	template<typename Char>
	utf8_string_impl(const Char* src,
						const string& src_charset_name = charset_name_t<Char>(),
						const alloc_type& alloc = alloc_type())
		: base_type(alloc), _utf8_size(size_type())
	{
		this_type::assign(src, src_charset_name);
	}

	template<typename Char>
	utf8_string_impl(const Char* src, const alloc_type& alloc)
		: base_type(alloc), _utf8_size(size_type())
	{
		this_type::assign(src);
	}


	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						const string& src_charset_name = charset_name_t<Char>(),
						const alloc_type& alloc = alloc_type())
		: base_type(alloc), _utf8_size(size_type())
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	utf8_string_impl(const Basic_String<Char, Traits, Alloc>& src,
						const alloc_type& alloc)
		: base_type(alloc), _utf8_size(size_type())
	{
		this_type::assign<Char, Traits, Alloc, Basic_String>(src);
	}

	template<typename InputIter>
	utf8_string_impl(InputIter start, InputIter last,
					const string& src_charset_name = charset_name_t<typename mplex::iterator_to_value_t<InputIter>::type>(),
					const alloc_type& alloc = alloc_type())
		: base_type(alloc), _utf8_size(size_type())
	{
		this_type::assign(start, last, src_charset_name);
	}

	template<typename InputIter>
	utf8_string_impl(InputIter start, InputIter last,
					const alloc_type& alloc)
		: base_type(alloc), _utf8_size(size_type())
	{
		this_type::assign(start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	utf8_string_impl(const utf8_string_impl<Char, Traits, Alloc>& other,
						const alloc_type& alloc = alloc_type())
		: base_type(alloc), _utf8_size(other.size())
	{
		base_type::assign(other.begin(), other.end());
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	utf8_string_impl(BOOST_RV_REF(this_type) right)
		: base_type(boost::forward<base_type>(right)),
		_utf8_size(right._utf8_size)
	{
	}
#else
	utf8_string_impl(BOOST_RV_REF(this_type) right)
		: _utf8_size(size_type())
	{
		base_type& right_ref = right;
		base_type::swap(right_ref);
		_utf8_size = right._utf8_size;
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	utf8_string_impl(const this_type& right)
		: base_type(right), _utf8_size(right._utf8_size)
	{
	}

	~utf8_string_impl(void)
	{
	}

	template<typename Char>
	this_type& operator=(const Char* str)
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::assign(string_type(str));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	this_type& operator=(const Basic_String<Char, Traits, Alloc>& str)
	{
		return this_type::assign(str);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& operator=(const utf8_string_impl<Char, Traits, Alloc>& other)
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

	// iterators:
	typename this_type::iterator begin(void)
	{
		return this_type::iterator(base_type::begin());
	}

	typename this_type::const_iterator begin(void) const
	{
		return this_type::const_iterator(base_type::begin());
	}

	typename this_type::iterator end(void)
	{
		return this_type::iterator(base_type::end());
	}

	typename this_type::const_iterator end(void) const
	{
		return const_iterator(base_type::end());
	}

	typename this_type::reverse_iterator rbegin(void)
	{
		return reverse_iterator(base_type::rbegin());
	}

	typename this_type::const_reverse_iterator rbegin(void) const
	{
		return this_type::const_reverse_iterator(base_type::rbegin());
	}

	typename this_type::reverse_iterator rend(void)
	{
		return this_type::reverse_iterator(base_type::rend());
	}

	typename this_type::const_reverse_iterator rend(void) const
	{
		return this_type::reverse_iterator(base_type::rend());
	}

	// capacity:
	size_type size(void) const
	{
		return _utf8_size;
	}

	size_type length(void) const
	{
		return _utf8_size;
	}

	size_type byte_size(void) const
	{
		return base_type::size();
	}

	//size_type max_size(void) const; // use base
	//void resize(size_type n); // use base
	//size_type capacity(void) const // use base
	//void reserve(size_type n) // use base
	//void clear(void) //use base

	bool empty(void) const
	{
		return !_utf8_size;
	}

	// element access:

	char_type operator[](size_type idx) const
	{
		assert(idx < _utf8_size);
		return char_type(this_type::begin() + idx,
							this_type::begin() + (idx + 1),
							UTF8_CHARSET_NAME,
							this_type::get_allocator());
	}

	char_type at(size_type idx) const
	{
		assert(idx < _utf8_size);
		return char_type(this_type::begin() + idx,
							this_type::begin() + (idx + 1),
							UTF8_CHARSET_NAME,
							this_type::get_allocator());
	}

	//modifiers:
	// +=

	template<typename Char>
	this_type& operator+=(const Char* src)
	{
		return this_type::append(src);
	}


	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	this_type& operator+=(const Basic_String<Char, Traits, Alloc>& src)
	{
		return this_type::append(src);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& operator+=(const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		return this_type::append(other);
	}

	// append

	template<typename Char>
	this_type& append(size_type n, Char c)
	{
		return this_type::template append<typename char_type::value_type,
                                            typename char_type::traits_type,
                                            typename char_type::allocator_type>(n, char_type(c));
        //return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& append(size_type n, const utf8_char_impl<Char, Traits, Alloc>& c)
	{
		for(size_type i = 0; i != n; ++i)
		{
			base_type::append(c.begin(), c.end());
		}

		_utf8_size += n;
		return *this;
	}

	template<typename Char>
	this_type& append(const Char* src, const string& src_charset_name = charset_name_t<Char>())
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::append<	typename string_type::value_type,
									typename string_type::traits_type,
									typename string_type::allocator_type,
									charset::basic_string
									>(string_type(src), src_charset_name);
	}

	template<	typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	this_type& append(const Basic_String<Char, Traits, Alloc>& src,
						const string& src_charset_name = charset_name_t<Char>())
	{
		base_type tmp(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
		size_type add_size = utf8_foo::strlen(tmp);
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::append(boost::move(tmp));
#else
		base_type::append(tmp);
#endif
		_utf8_size += add_size;
		return *this;
	}

	template<typename Char>
	this_type& append(const Char* src, size_type subpos, size_type sublen)
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::append(string_type(src + subpos, src + (subpos + sublen)));
	}

	template<	typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String>
	this_type& append(const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos, size_type sublen)
	{
		return this_type::append(src.substr(subpos, sublen));
	}

	template<typename InputIter>
	this_type& append(InputIter start, InputIter last,
						const string& src_charset_name = charset_name_t< typename mplex::iterator_to_value_t<InputIter>::type>())
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
		size_type add_size = utf8_foo::strlen(right);

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::append(boost::move(right));
#else
		base_type::append(right);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

		_utf8_size += add_size;
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& append(typename utf8_string_impl<Char, Traits, Alloc>::iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::append_of_this_iterator(start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& append(typename utf8_string_impl<Char, Traits, Alloc>::const_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::const_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::append_of_this_iterator(start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& append(typename utf8_string_impl<Char, Traits, Alloc>::reverse_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::append_of_this_riterator(start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& append(typename utf8_string_impl<Char, Traits, Alloc>::const_reverse_iterator start,
						typename this_type::const_reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::append_of_this_riterator(start, last);
	}

	this_type& append(typename this_type::iterator start, typename this_type::iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::append_of_this_iterator(start, last);
	}

	this_type& append(typename this_type::const_iterator start, typename this_type::const_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::append_of_this_iterator(start, last);
	}

	this_type& append(typename this_type::reverse_iterator start, typename this_type::reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::append_of_this_riterator(start, last);
	}

	this_type& append(typename this_type::const_reverse_iterator start, typename this_type::const_reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::append_of_this_riterator(start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& append(const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		base_type::append(other.begin().base(),other.end().base());
		_utf8_size += other.size();
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& append(const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen)
	{
		return this_type::append(other.substr(subpos, sublen));
	}

	this_type& append(const this_type& right)
	{
		const base_type& rbase = right;
		base_type::append(rbase);
		_utf8_size += right._utf8_size;
		return *this;
	}

	this_type& append(const this_type& right, size_type subpos, size_type sublen)
	{
		return this_type::append(right.substr(subpos, sublen));
	}

	// push_back

	template<typename Char>
	void push_back(const Char c)
	{
		this_type::append(1, char_type(c));
	}

	template<typename Char, typename Traits, typename Alloc>
	void push_back(const utf8_char_impl<Char, Traits, Alloc>& val)
	{
		this_type::append(1, val);
	}

	// assign
	template<typename Char>
	this_type& assign(const Char* src,
						const string& src_charset_name = charset_name_t<Char>())
	{
		return this_type::assign<	Char,
									std::char_traits<Char>,
									std::allocator<Char>,
									charset::basic_string>
									(charset::basic_string<Char>(src), src_charset_name);
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
			base_type::clear();
			_utf8_size = 0;
			return *this;
		}
		base_type right(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
		_utf8_size = utf8_foo::strlen(right);
		base_type::swap(right);
		return *this;
	}

	template<typename InputIter>
	this_type& assign(InputIter start, InputIter last,
						const string& src_charset_name = charset_name_t<typename mplex::iterator_to_value_t<InputIter>::type>())
	{
		typedef InputIter iter_type;
		typedef typename mplex::iterator_to_value_t<iter_type>::type iter_val_type;
		typedef basic_string<iter_val_type> string_type;

		string_type src(start, last);
		if(src.empty())
		{
			base_type::clear();
			_utf8_size = 0;
			return *this;
		}

		base_type right(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
		_utf8_size = utf8_foo::strlen(right);
		base_type::swap(right);
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(typename utf8_string_impl<Char, Traits, Alloc>::iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::assign_of_this_iterator(start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(typename utf8_string_impl<Char, Traits, Alloc>::const_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::const_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::assign_of_this_iterator(start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(typename utf8_string_impl<Char, Traits, Alloc>::reverse_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::assign_of_this_riterator(start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(typename utf8_string_impl<Char, Traits, Alloc>::const_reverse_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::const_reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::assign_of_this_riterator(start, last);
	}

	this_type& assign(typename this_type::iterator start, typename this_type::iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::assign_of_this_iterator(start, last);
	}

	this_type& assign(typename this_type::const_iterator start, typename this_type::const_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::assign_of_this_iterator(start, last);
	}

	this_type& assign(typename this_type::reverse_iterator start, typename this_type::reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::assign_of_this_riterator(start, last);
	}

	this_type& assign(const_reverse_iterator start, const_reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::assign_of_this_riterator(start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& assign(const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		base_type::assign(other.begin(), other.end());
		_utf8_size = other.size();
		return *this;
	}

	this_type& assign(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		base_type::assign(boost::forward<base_type>(right));
		_utf8_size =right._utf8_size;
#else
		base_type& right_ref = right;
		base_type::swap(right_ref);
		_utf8_size = right._utf8_size;;
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& assign(const this_type& right)
	{
		base_type::assign(right);
		_utf8_size = right._utf8_size;
		return *this;
	}

	// insert

	template<typename Char>
	this_type& insert(size_type pos, size_type n, Char val)
	{
		return this_type::insert(pos, n, char_type(val));
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& insert(size_type pos, size_type n, const utf8_char_impl<Char, Traits, Alloc>& val)
	{
		if(!n)
		{
			return *this;
		}

		base_iterator iter = base_type::begin() + ((this_type::begin() + pos).base() - this_type::begin().base());
		for(size_type i = 0; i != n; ++i)
		{
			base_type::insert(iter, val.begin(), val.end());
			iter = base_type::begin() + ((this_type::begin() + pos).base() - this_type::begin().base());
		}

		_utf8_size += n;
		return *this;
	}

	template<typename Char>
	typename this_type::iterator insert(typename this_type::iterator p, const Char& val)
	{
		return this_type::insert(p, 1, val);
	}

	template<typename Char>
	typename this_type::iterator insert(typename this_type::iterator p, size_type n, Char val)
	{
		size_type size = p - this_type::begin();
		this_type::insert(size, n, char_type(val));
		return this_type::begin() + size;
	}

	template<typename Char, typename Traits, typename Alloc>
	typename this_type::iterator insert(typename this_type::iterator p, size_type n,
											const utf8_char_impl<Char, Traits, Alloc>& val)
	{
		size_type size = p - this_type::begin();
		this_type::insert(size, n, val);
		return this_type::begin() + size;
	}

	template<typename Char>
	this_type& insert(size_type pos, const Char* src,
						const string& src_charset_name = charset_name_t<Char>())
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::insert(pos, string_type(src), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	this_type& insert(size_type pos, const Basic_String<Char, Traits, Alloc>& src,
						const string& src_charset_name = charset_name_t<Char>())
	{
		if(src.empty())
		{
			return *this;
		}

		base_type tmp(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));

		if(tmp.empty())
		{
			return *this;
		}

		_utf8_size += utf8_foo::strlen(tmp);
		base_type::insert(pos, tmp);
		return *this;
	}

	template<typename InputIter>
	typename this_type::iterator insert(typename this_type::iterator p, InputIter start, InputIter last,
											const string& src_charset_name = charset_name_t<typename mplex::iterator_to_value_t<InputIter>::type>())
	{
		typedef InputIter iter_type;
		typedef typename mplex::iterator_to_value_t<InputIter>::type iter_val_type;
		typedef charset::basic_string<iter_val_type> string_type;

		size_type size = p - this_type::begin();
		string_type src(start, last);
		base_type tmp(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
		size_type add_size = utf8_foo::strlen(tmp);
		base_type::insert(size, tmp);
		_utf8_size += add_size;
		return this_type::begin() + size;
	}

	template<typename Char, typename Traits, typename Alloc>
	typename this_type::iterator insert(typename this_type::iterator p,
											typename utf8_string_impl<Char, Traits, Alloc>::iterator start,
											typename utf8_string_impl<Char, Traits, Alloc>::iterator last,
											const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::insert_of_this_iterator(p, start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	typename this_type::iterator insert(typename this_type::iterator p,
											typename utf8_string_impl<Char, Traits, Alloc>::const_iterator start,
											typename utf8_string_impl<Char, Traits, Alloc>::const_iterator last,
											const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::insert_of_this_iterator(p, start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	typename this_type::iterator insert(typename this_type::iterator p,
											typename utf8_string_impl<Char, Traits, Alloc>::reverse_iterator start,
											typename utf8_string_impl<Char, Traits, Alloc>::reverse_iterator last,
											const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::insert_of_this_riterator(p, start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	typename this_type::iterator insert(typename this_type::iterator p,
											typename utf8_string_impl<Char, Traits, Alloc>::const_reverse_iterator start,
											typename utf8_string_impl<Char, Traits, Alloc>::const_reverse_iterator last,
											const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::insert_of_this_riterator(p, start, last);
	}

	typename this_type::iterator insert(typename this_type::iterator p,
											typename this_type::iterator start,
											typename this_type::iterator last,
											const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::insert_of_this_iterator(p, start, last);
	}

	typename this_type::iterator insert(typename this_type::iterator p,
											typename this_type::const_iterator start,
											typename this_type::const_iterator last,
											const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::insert_of_this_iterator(p, start, last);
	}

	typename this_type::iterator insert(typename this_type::iterator p,
											typename this_type::reverse_iterator start,
											typename this_type::reverse_iterator last,
											const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::insert_of_this_riterator(p, start, last);
	}

	typename this_type::iterator insert(typename this_type::iterator p,
											typename this_type::const_reverse_iterator start,
											typename this_type::const_reverse_iterator last,
											const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::insert_of_this_riterator(p, start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& insert(size_type pos, const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		base_type::insert(this_type::begin() + pos, other.begin().base(), other.end().base());
		_utf8_size += other.size();
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& insert(size_type pos, const utf8_string_impl<Char, Traits, Alloc>& other,
						size_type subpos, size_type sublen)
	{
		return this_type::insert(pos, other.substr(subpos, sublen));
	}

	this_type& insert(size_type pos, const this_type& right)
	{
		base_type::insert(pos, right);
		_utf8_size += right._utf8_size;
		return *this;
	}

	this_type& insert(size_type pos, const this_type& right,
						size_type subpos, size_type sublen)
	{
		return this_type::insert(pos, right.substr(subpos, sublen));
	}

	this_type& erase(size_type pos = 0, size_type len = base_type::npos)
	{
		assert(pos < _utf8_size);
		this_type::iterator first = this_type::begin() + pos;
		size_type rm_size = _utf8_size - pos < len? _utf8_size - pos : len;
		this_type::iterator last = first + rm_size;
		base_type::erase(first.base() - this_type::begin().base(), last.base() - first.base());
		_utf8_size -= rm_size;
		return *this;
	}

	typename this_type::iterator erase(typename this_type::iterator p)
	{
		size_type rm_pos = p - this_type::begin();
		if(!(rm_pos < _utf8_size))
		{
			return this_type::end();
		}

		size_type old_size = base_type::size();
		base_iterator iter = base_type::begin() + (p.base() - this_type::begin().base());
		iter = base_type::erase(iter, iter + ((p + 1).base() - p.base()));

		if(old_size != base_type::size())
		{
			--_utf8_size;
		}
		return this_type::iterator(iter);
	}

	typename this_type::iterator erase(typename this_type::iterator start, typename this_type::iterator last)
	{
		size_type rm_pos = start - this_type::begin();
		if(!(rm_pos < _utf8_size))
		{
			return this_type::end();
		}

		size_type rm_size = last - start;
		assert(rm_size <= _utf8_size);

		size_type old_size = base_type::size();
		base_iterator base_start = base_type::begin() + (start.base() - this_type::begin().base());
		base_iterator base_last = base_start + (last.base() - start.base());

		base_iterator iter = base_type::erase(base_start, base_last);

		if(old_size != base_type::size())
		{
			_utf8_size -= rm_size;
		}

		return this_type::iterator(iter);
	}

	// replace

	template<typename Char>
	this_type& replace(size_type pos, size_type len, size_type n, const Char& c)
	{
		return this_type::replace(this_type::begin() + pos, this_type::begin() + (pos + len), n, c);
	}

	template<typename Char>
	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
							size_type n, const Char& c)
	{
		return this_type::replace(i1, i2, n, char_type(c));
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						size_type n, const utf8_char_impl<Char, Traits, Alloc>& c)
	{
		size_type rm_size = i2 - i1;
		base_iterator base_i1 = base_type::begin() + (i1.base() - this_type::begin().base());
		base_iterator base_i2 = base_type::begin() + (i2.base() - this_type::begin().base());

		base_type tmp(base_type::get_allocator());
		for(int i = 0; i != n; ++i)
		{
			tmp.append(c);
		}

		base_type::replace(base_i1, base_i2, tmp);
		_utf8_size = _utf8_size - rm_size + n;
		return *this;
	}

	template<typename Char>
	this_type& replace(size_type pos, size_type len, const Char* src,
						const string& src_charset_name = charset_name_t<Char>())
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::replace(pos, len, string_type(src), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	this_type& replace(size_type pos, size_type len,
						const Basic_String<Char, Traits, Alloc>& src,
						const string& src_charset_name = charset_name_t<Char>())
	{
		return this_type::replace<Char, Traits, Alloc, Basic_String>
							(this_type::begin() + pos,
								this_type::begin() + (pos + len),
								src, src_charset_name);
	}

	template<typename Char>
	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						const Char* src, const string& src_charset_name = charset_name_t<Char>())
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::replace(i1, i2, string_type(src), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						const Basic_String<Char, Traits, Alloc>& src,
						const string& src_charset_name = charset_name_t<Char>())
	{
		size_type rm_size = i2 - i1;
		base_type tmp(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
		base_iterator base_i1 = base_type::begin() + (i1.base() - this_type::begin().base());
		base_iterator base_i2 = base_type::begin() + (i2.base() - this_type::begin().base());

		base_type::replace(base_i1, base_i2, tmp);
		_utf8_size = _utf8_size - rm_size + utf8_foo::strlen(tmp);

		return *this;
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						const Basic_String<Char, Traits, Alloc>& src,
						size_type subpos, size_type sublen)
	{
		return this_type::replace(i1, i2, src.substr(subpos, sublen));
	}

	template<typename InputIter>
	this_type& replace(iterator i1, iterator i2, InputIter start, InputIter last,
						const string& src_charset_name = charset_name_t<typename mplex::iterator_to_value_t<InputIter>::type>())
	{
		typedef typename mplex::iterator_to_value_t<InputIter>::type char_type;
		typedef charset::basic_string<char_type> string_type;

		return this_type::replace<typename string_type::value_type,
									typename string_type::traits_type,
									typename string_type::allocator_type,
									charset::basic_string>(i1, i2, string_type(start, last), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						typename utf8_string_impl<Char, Traits, Alloc>::iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::replace_of_this_iterator(i1, i2, start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						typename utf8_string_impl<Char, Traits, Alloc>::const_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::const_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::replace_of_this_iterator(i1, i2, start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						typename utf8_string_impl<Char, Traits, Alloc>::reverse_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::replace_of_this_riterator(i1, i2, start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						typename utf8_string_impl<Char, Traits, Alloc>::const_reverse_iterator start,
						typename utf8_string_impl<Char, Traits, Alloc>::const_reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::replace_of_this_riterator(i1, i2, start, last);
	}

	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						typename this_type::iterator start, typename this_type::iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::replace_of_this_iterator(i1, i2, start, last);
	}

	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						typename this_type::const_iterator start, typename this_type::const_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::replace_of_this_iterator(i1, i2, start, last);
	}

	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						typename this_type::reverse_iterator start, typename this_type::reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::replace_of_this_riterator(i1, i2, start, last);
	}

	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						typename this_type::const_reverse_iterator start,  typename this_type::const_reverse_iterator last,
						const string& src_charset_name = IN_CHARSET_NAME)
	{
		return this_type::replace_of_this_riterator(i1, i2, start, last);
	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& replace(size_type pos, size_type len, const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		return this_type::replace(this_type::begin() + pos, this_type::begin() + (pos + len), other);
	}


	template<typename Char, typename Traits, typename Alloc>
	this_type& replace(typename this_type::iterator i1, typename this_type::iterator i2,
						const utf8_string_impl<Char, Traits, Alloc>& other)
	{
		size_type rm_size = i2 - i1;
		base_iterator base_i1 = base_type::begin() + (i1.base() - this_type::begin().base());
		base_iterator base_i2 = base_type::begin() + (i2.base() - this_type::begin().base());
		base_type::replace(base_i1, base_i2, other.begin().base(), other.end().base());
		_utf8_size = _utf8_size - rm_size + other.size();
		return *this;

	}

	template<typename Char, typename Traits, typename Alloc>
	this_type& replace (size_type pos, size_type len,
							const utf8_string_impl<Char, Traits, Alloc>& other,
							size_type subpos, size_type sublen)
	{
		return this_type::replace(this_type::begin() + pos,
									this_type::begin() + (pos + len),
									other.substr(subpos, sublen));
	}


	this_type& replace(size_type pos, size_type len, const this_type& right)
	{
		return this_type::replace(this_type::begin() + pos,
									this_type::begin() + (pos + len),
									right);
	}

	this_type& replace(iterator i1, iterator i2,
						const this_type& right)
	{
		size_type rm_size = i2 - i1;
		base_iterator base_i1 = base_type::begin() + (i1.base() - this_type::begin().base());
		base_iterator base_i2 = base_type::begin() + (i2.base() - this_type::begin().base());

		base_type::replace(base_i1, base_i2, right);

		_utf8_size = _utf8_size - rm_size + right.size();
		return *this;
	}

	this_type& replace (size_type pos, size_type len,
							const this_type& right,
							size_type subpos, size_type sublen)
	{
		return this_type::replace(this_type::begin() + pos,
									this_type::begin() + (pos + len),
									right.substr(subpos, sublen));
	}

	// swap

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		//base_type::swap(boost::forward<base_type>(right));
		base_type::swap(right);
		std::swap(_utf8_size, right._utf8_size);
#else
		this_type& right_ref = right;
		base_type::swap(right_ref);
		std::swap(_utf8_size, right_ref._utf8_size);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		base_type::swap(right);
		std::swap(_utf8_size, right._utf8_size);
	}

	//string operations:

	/*
	const value_type* c_str(void) const; // use base
	const value_type* data(void) const; // use base
	get_allocator(void) const // use base
	*/

	//find

	template<typename Char>
	size_type find(Char c, size_type pos = 0) const
	{
		return this_type::find(char_type(c), pos);
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos = 0) const
	{
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}

		size_type base_pos = (this_type::begin() + pos).base() - this_type::begin().base();
		size_type base_ret = base_type::find(c.c_str(), base_pos);

		if(base_ret == base_type::npos)
		{
			return base_type::npos;
		}

		typename this_type::const_iterator iter = this_type::begin();
		typename this_type::const_iterator::base_type& base_iter = iter;
		base_iter += base_ret;
		return iter - this_type::begin();
	}

	template<typename Char>
	size_type find(const Char* src, size_type pos = 0,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef charset::basic_string<Char> string_type;

		return this_type::find(string_type(src), pos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	size_type find(const Basic_String<Char, Traits, Alloc>& src, size_type pos = 0,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}

		size_type base_pos = (this_type::begin() + pos).base() - this_type::begin().base();
		base_type tmp(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));

		size_type base_ret = base_type::find(tmp, base_pos);

		if(base_ret == base_type::npos)
		{
			return base_type::npos;
		}

		typename this_type::const_iterator iter = this_type::begin();
		typename this_type::const_iterator::base_type& base_iter = iter;
		base_iter += base_ret;
		return iter - this_type::begin();
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos = 0) const
	{
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}
		size_type base_pos = (this_type::begin() + pos).base() - this_type::begin().base();
		size_type base_ret = base_type::find(src.c_str(), base_pos);

		if(base_ret == base_type::npos)
		{
			return base_type::npos;
		}

		typename this_type::const_iterator iter = this_type::begin();
		typename this_type::const_iterator::base_type& base_iter = iter;
		base_iter += base_ret;
		return iter - this_type::begin();
	}

	size_type find(const this_type& right, size_type pos = 0) const
	{
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}
		size_type base_pos = (this_type::begin() + pos).base() - this_type::begin().base();
		size_type base_ret = base_type::find(right, base_pos);

		if(base_ret == base_type::npos)
		{
			return base_type::npos;
		}

		typename this_type::const_iterator iter = this_type::begin();
		typename this_type::const_iterator::base_type& base_iter = iter;
		base_iter += base_ret;
		return iter - this_type::begin();
	}

	// rfind

	template<typename Char>
	size_type rfind(Char c, size_type pos = base_type::npos) const
	{
		return this_type::rfind(char_type(c), pos);
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type rfind(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos = base_type::npos) const
	{
		if(!(pos < _utf8_size))
		{
			pos = _utf8_size;
		}

		size_type base_pos = (this_type::begin() + pos).base() - this_type::begin().base();
		size_type base_ret = base_type::rfind(c.c_str(), base_pos);

		if(base_ret == base_type::npos)
		{
			return base_type::npos;
		}

		typename this_type::const_iterator iter = this_type::begin();
		typename this_type::const_iterator::base_type& base_iter = iter;
		base_iter += base_ret;
		return iter - this_type::begin();
	}

	template<typename Char>
	size_type rfind(const Char* src, size_type pos = base_type::npos,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef charset::basic_string<Char> string_type;

		return this_type::rfind(string_type(src), pos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	size_type rfind(const Basic_String<Char, Traits, Alloc>& src, size_type pos = base_type::npos,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		if(!(pos < _utf8_size))
		{
			pos = _utf8_size;
		}

		size_type base_pos = (this_type::begin() + pos).base() - this_type::begin().base();
		base_type tmp(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));

		size_type base_ret = base_type::rfind(tmp, base_pos);

		if(base_ret == base_type::npos)
		{
			return base_type::npos;
		}

		typename this_type::const_iterator iter = this_type::begin();
		typename this_type::const_iterator::base_type& base_iter = iter;
		base_iter += base_ret;
		return iter - this_type::begin();
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type rfind(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos = base_type::npos) const
	{
		if(!(pos < _utf8_size))
		{
			pos = _utf8_size;
		}
		size_type base_pos = (this_type::begin() + pos).base() - this_type::begin().base();
		size_type base_ret = base_type::rfind(src.c_str(), base_pos);

		if(base_ret == base_type::npos)
		{
			return base_type::npos;
		}

		typename this_type::const_iterator iter = this_type::begin();
		typename this_type::const_iterator::base_type& base_iter = iter;
		base_iter += base_ret;
		return iter - this_type::begin();
	}

	size_type rfind(const this_type& right, size_type pos = base_type::npos) const
	{
		if(!(pos < _utf8_size))
		{
			pos = _utf8_size;
		}
		size_type base_pos = (this_type::begin() + pos).base() - this_type::begin().base();
		size_type base_ret = base_type::rfind(right, base_pos);

		if(base_ret == base_type::npos)
		{
			return base_type::npos;
		}

		typename this_type::const_iterator iter = this_type::begin();
		typename this_type::const_iterator::base_type& base_iter = iter;
		base_iter += base_ret;
		return iter - this_type::begin();
	}

	// find_first_of

	template<typename Char>
	size_type find_first_of(const Char& c, size_type pos = 0) const
	{
		return this_type::find_first_of(char_type(c), pos);
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_first_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos = 0) const
	{
		return this_type::find(c, pos);
	}

	template<typename Char>
	size_type find_first_of(const Char* src, size_type pos = 0,
								const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::find_first_of<typename string_type::value_type,
										typename string_type::traits_type,
										typename string_type::allocator_type,
										charset::basic_string >
										(string_type(src), pos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	size_type find_first_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos = 0,
								const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef boost::unordered_multiset<base_type> set_type;
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}
		set_type set;
		string_to_container(src, set, src_charset_name);
		if(set.empty())
		{
			return base_type::npos;
		}

		for(size_type i = pos, isize = _utf8_size; i != isize; ++i)
		{
			if(set.find(this_type::at(i)) != set.end())
			{
				return i;
			}
		}

		return base_type::npos;
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_first_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos = 0) const
	{
		typedef boost::unordered_multiset<base_type> set_type;
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}
		set_type set;
		string_to_container(src, set);
		if(set.empty())
		{
			return base_type::npos;
		}

		for(size_type i = pos, isize = _utf8_size; i != isize; ++i)
		{
			if(set.find(this_type::at(i)) != set.end())
			{
				return i;
			}
		}

		return base_type::npos;
	}

	template<typename Char>
	size_type find_first_not_of(const Char& c, size_type pos = 0) const
	{
		return this_type::find_first_not_of(char_type(c), pos);
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_first_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos = 0) const
	{
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}

		for(size_type i = pos, isize = _utf8_size; i != isize; ++i)
		{
			if(this_type::at(i) != c)
			{
				return i;
			}
		}

		return base_type::npos;
	}

	template<typename Char>
	size_type find_first_not_of(const Char* src, size_type pos = 0,
								const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::find_first_not_of<typename string_type::value_type,
										typename string_type::traits_type,
										typename string_type::allocator_type,
										charset::basic_string >
										(string_type(src), pos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	size_type find_first_not_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos = 0,
								const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef boost::unordered_multiset<base_type> set_type;
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}
		set_type set;
		string_to_container(src, set, src_charset_name);
		if(set.empty())
		{
			return pos;
		}

		for(size_type i = pos, isize = _utf8_size; i != isize; ++i)
		{
			if(set.find(this_type::at(i)) == set.end())
			{
				return i;
			}
		}

		return base_type::npos;
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_first_not_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos = 0) const
	{
		typedef boost::unordered_multiset<base_type> set_type;
		if(!(pos < _utf8_size))
		{
			return base_type::npos;
		}
		set_type set;
		string_to_container(src, set);
		if(set.empty())
		{
			return pos;
		}

		for(size_type i = pos, isize = _utf8_size; i != isize; ++i)
		{
			if(set.find(this_type::at(i)) == set.end())
			{
				return i;
			}
		}

		return base_type::npos;
	}

	// find_last_of
	template<typename Char>
	size_type find_last_of(const Char& c, size_type pos = base_type::npos) const
	{
		return this_type::find_last_of(char_type(c), pos);
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_last_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos = base_type::npos) const
	{
		return this_type::rfind(c, pos);
	}

	template<typename Char>
	size_type find_last_of(const Char* src, size_type pos = base_type::npos,
								const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::find_last_of<typename string_type::value_type,
										typename string_type::traits_type,
										typename string_type::allocator_type,
										charset::basic_string >
										(string_type(src), pos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	size_type find_last_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos = base_type::npos,
								const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef boost::unordered_multiset<base_type> set_type;

		if(!(pos < _utf8_size))
		{
			pos = _utf8_size - 1;
		}
		set_type set;
		string_to_container(src, set, src_charset_name);
		if(set.empty())
		{
			return base_type::npos;
		}

		for(size_type i = pos, isize = 0; i != isize; --i)
		{
			if(set.find(this_type::at(i)) != set.end())
			{
				return i;
			}
		}

		return set.find(this_type::at(0)) != set.end()? 0 : base_type::npos;
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_last_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos = base_type::npos) const
	{
		typedef boost::unordered_multiset<base_type> set_type;

		if(!(pos < _utf8_size))
		{
			pos = _utf8_size - 1;
		}
		set_type set;
		string_to_container(src, set);
		if(set.empty())
		{
			return base_type::npos;
		}

		for(size_type i = pos, isize = 0; i != isize; --i)
		{
			if(set.find(this_type::at(i)) != set.end())
			{
				return i;
			}
		}

		return set.find(this_type::at(0)) != set.end()? 0 : base_type::npos;
	}

	// find_last_not_of
	template<typename Char>
	size_type find_last_not_of(const Char& c, size_type pos = base_type::npos) const
	{
		return this_type::find_last_not_of(char_type(c), pos);
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_last_not_of(const utf8_char_impl<Char, Traits, Alloc>& c, size_type pos = base_type::npos) const
	{

		if(!(pos < _utf8_size))
		{
			pos = _utf8_size - 1;
		}

		for(size_type i = pos, isize = 0; i != isize; --i)
		{
			if(this_type::at(i) != c)
			{
				return i;
			}
		}

		return this_type::at(0) != c? 0 : base_type::npos;
	}

	template<typename Char>
	size_type find_last_not_of(const Char* src, size_type pos = base_type::npos,
								const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::find_last_not_of<typename string_type::value_type,
										typename string_type::traits_type,
										typename string_type::allocator_type,
										charset::basic_string >
										(string_type(src), pos, src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	size_type find_last_not_of(const Basic_String<Char, Traits, Alloc>& src, size_type pos = base_type::npos,
								const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef boost::unordered_multiset<base_type> set_type;

		if(!(pos < _utf8_size))
		{
			pos = _utf8_size - 1;
		}
		set_type set;
		string_to_container(src, set, src_charset_name);
		if(set.empty())
		{
			return pos;
		}

		for(size_type i = pos, isize = 0; i != isize; --i)
		{
			if(set.find(this_type::at(i)) == set.end())
			{
				return i;
			}
		}

		return set.find(this_type::at(0)) == set.end()? 0 : base_type::npos;
	}

	template<typename Char, typename Traits, typename Alloc>
	size_type find_last_not_of(const utf8_string_impl<Char, Traits, Alloc>& src, size_type pos = base_type::npos) const
	{
		typedef boost::unordered_multiset<base_type> set_type;

		if(!(pos < _utf8_size))
		{
			pos = _utf8_size - 1;
		}
		set_type set;
		string_to_container(src, set);
		if(set.empty())
		{
			return pos;
		}

		for(size_type i = pos, isize = 0; i != isize; --i)
		{
			if(set.find(this_type::at(i)) == set.end())
			{
				return i;
			}
		}

		return set.find(this_type::at(0)) == set.end()? 0 : base_type::npos;
	}

	// substr
	this_type substr(size_type pos = 0, size_type len = base_type::npos) const
	{
		typedef typename this_type::const_iterator::base_type base_iter_type;
		if(pos == _utf8_size)
		{
			return this_type();
		}

		assert(pos < _utf8_size);

		size_type use_len = len;
		size_type useable_len = _utf8_size - pos;

		use_len = std::min(use_len, useable_len);

		const_iterator start(this_type::begin() + pos);
		const_iterator last(use_len < useable_len? start + use_len : this_type::end());

		return this_type(base_type::substr(start.base() - this_type::begin().base(),
											last.base() - start.base()), IN_CHARSET_NAME);
	}

	// compare

	template<typename Char>
	s32 compare(const Char* src, const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef charset::basic_string<Char> string_type;
		return this_type::compare<typename string_type::value_type,
									typename string_type::traits_type,
									typename string_type::allocator_type,
									charset::basic_string>
									(string_type(src), src_charset_name);
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	s32 compare(const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		return base_type::compare(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
	}

	template<typename Char>
	s32 compare(const Char* src, size_type subpos, size_type sublen) const
	{
		typedef charset::basic_string<Char> string_type;
		string_type tmp(src + subpos, src + (subpos + sublen));
		return base_type::compare(YGGR_PP_CHARSET_FOO_XCHG(base_type, tmp, charset_name_t<Char>(), IN_CHARSET_NAME));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	s32 compare(const Basic_String<Char, Traits, Alloc>& src,
					size_type subpos, size_type sublen) const
	{
		typedef Basic_String<Char, Traits, Alloc> string_type;
		string_type tmp(src.substr(subpos, sublen));

		return base_type::compare(YGGR_PP_CHARSET_FOO_XCHG(base_type, tmp, charset_name_t<Char>(), IN_CHARSET_NAME));
	}

	template<typename Char>
	s32 compare(size_type pos, size_type len,
					const Char* src,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef charset::basic_string<Char> string_type;

		const_iterator start(this_type::begin() + pos);
		const_iterator last(start + len);

		string_type tmp(src);

		return base_type::compare(start.base() - this_type::begin().base(), last.base() - start.base(),
									YGGR_PP_CHARSET_FOO_XCHG(base_type, tmp, src_charset_name, IN_CHARSET_NAME));
	}

	template<typename Char, typename Traits, typename Alloc,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	s32 compare(size_type pos, size_type len,
					const Basic_String<Char, Traits, Alloc>& src,
					const string& src_charset_name = charset_name_t<Char>()) const
	{
		typedef Basic_String<Char, Traits, Alloc>& string_type;

		const_iterator start(this_type::begin() + pos);
		const_iterator last(start + len);

		return base_type::compare(start.base() - this_type::begin().base(), last.base() - start.base(),
									YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
	}

	template<typename Char, typename Traits, typename Alloc>
	s32 compare(const utf8_string_impl<Char, Traits, Alloc>& other) const
	{
		return base_type::compare(other.c_str());
	}

	template<typename Char, typename Traits, typename Alloc>
	s32 compare(const utf8_string_impl<Char, Traits, Alloc>& other,
					size_type subpos, size_type sublen) const
	{
		return base_type::compare(other.substr(subpos, sublen).c_str());
	}

	template<typename Char, typename Traits, typename Alloc>
	s32 compare(size_type pos, size_type len,
					const utf8_string_impl<Char, Traits, Alloc>& other) const
	{
		const_iterator start(this_type::begin() + pos);
		const_iterator last(start + len);

		return base_type::compare(start.base() - this_type::begin().base(),
									last.base() - start.base(), other.c_str());
	}

	s32 compare(const this_type& right) const
	{
		return base_type::compare(right);
	}

	s32 compare(const this_type& right, size_type subpos, size_type sublen) const
	{
		return base_type::compare(right.substr(subpos, sublen));
	}

	s32 compare(size_type pos, size_type len, const this_type& right) const
	{
		const_iterator start(this_type::begin() + pos);
		const_iterator last(start + len);

		return base_type::compare(start.base() - this_type::begin().base(),
									last.base() - start.base(), right);
	}

	// utf8 only
	operator base_type&(void)
	{
		return *this;
	}

	operator const base_type&(void) const
	{
		return *this;
	}

	base_type& org_str(void)
	{
		return *this;
	}

	const base_type& org_str(void) const
	{
		return *this;
	}

	template<typename String>
	String str(const string& dst_charset_name = charset_name_t<typename String::value_type>()) const
	{
		typedef String string_type;

		const base_type& base = *this;
		return YGGR_PP_CHARSET_FOO_XCHG(string_type, base, IN_CHARSET_NAME, dst_charset_name);
	}

	void recount_size(void)
	{
		base_type& base = *this;
		_utf8_size = utf8_foo::strlen(base);
	}

	std::size_t hash(void) const
	{
		boost::hash<base_type> hasher;
		return hasher(*this);
	}

private:
	//append this iterator
	template<typename InputIter>
	this_type& append_of_this_iterator(const InputIter& start, const InputIter last)
	{
		base_type::append(start.base(), last.base());
		_utf8_size += last - start;
		return *this;
	}

	template<typename InputIter>
	this_type& append_of_this_riterator(const InputIter& start, const InputIter& last)
	{
		typedef InputIter r_iter_type;
		for(r_iter_type i = start; i != last; ++i)
		{
			base_type::append(&(*i), (&(*i)) + utf8_foo::charlen(&(*i)));
		}
		_utf8_size += std::abs(last - start);
		return *this;
	}

	// assign this iterator
	template<typename InputIter>
	this_type& assign_of_this_iterator(const InputIter& start, const InputIter& last)
	{
		base_type::assign(start.base(), last.base());
		_utf8_size = last - start;
		return *this;
	}

	template<typename InputIter>
	this_type& assign_of_this_riterator(const InputIter& start, const InputIter& last)
	{
		typedef InputIter r_iter_type;
		base_type::clear();
		for(r_iter_type i = start; i != last; ++i)
		{
			base_type::append(&(*i), (&(*i)) + utf8_foo::charlen(&(*i)));
		}
		_utf8_size = std::abs(last - start);
		return *this;
	}

	// insert this iterator
	template<typename InputIter>
	typename this_type::iterator insert_of_this_iterator(const typename this_type::iterator& p,
															const InputIter& start, const InputIter& last)
	{
		size_type size = p - this_type::begin();
		size_type base_size = p.base() - this_type::begin().base();

		base_type::insert(base_type::begin() + base_size, start.base(), last.base());
		_utf8_size += last - start;
		return this_type::begin() + size;
	}

	template<typename InputIter>
	typename this_type::iterator insert_of_this_riterator(const typename this_type::iterator& p,
															const InputIter& start, const InputIter& last)
	{
		typedef InputIter r_iter_type;
		size_type size = p - this_type::begin();
		size_type base_size = p.base() - this_type::begin().base();
		base_type tmp(base_type::get_allocator());
		for(r_iter_type i = start; i != last; ++i)
		{
			tmp.append(&(*i), &(*i) + utf8_foo::charlen(&(*i)));
		}

		base_type::insert(base_size, tmp);
		_utf8_size += std::abs(last - start);
		return this_type::begin() + size;
	}

	//replace this iterator
	template<typename InputIter>
	this_type& replace_of_this_iterator(const typename this_type::iterator& i1, const typename this_type::iterator& i2,
											const InputIter& start, const InputIter& last)
	{
		size_type rm_size = i2 - i1;
		base_iterator base_i1 = base_type::begin() + (i1.base() - this_type::begin().base());
		base_iterator base_i2 = base_type::begin() + (i2.base() - this_type::begin().base());

		base_type::replace(base_i1, base_i2, start.base(), last.base());
		_utf8_size = _utf8_size - rm_size + (last - start);
		return *this;
	}

	template<typename InputIter>
	this_type& replace_of_this_riterator(const typename this_type::iterator& i1, const typename this_type::iterator& i2,
											const InputIter& start, const InputIter& last)
	{
		typedef InputIter r_iter_type;

		size_type rm_size = i2 - i1;
		base_iterator base_i1 = base_type::begin() + (i1.base() - this_type::begin().base());
		base_iterator base_i2 = base_type::begin() + (i2.base() - this_type::begin().base());

		base_type tmp(base_type::get_allocator());
		for(r_iter_type i = start; i != last; ++i)
		{
			tmp.append(&(*i), &(*i) + utf8_foo::charlen(&(*i)));
		}

		base_type::replace(base_i1, base_i2, tmp);
		_utf8_size = _utf8_size - rm_size + std::abs(last - start);
		return *this;
	}

	template<typename Char, typename Traits, typename Alloc, typename Container,
				template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
	void string_to_container(const Basic_String<Char, Traits, Alloc>& src, Container& cont,
						const string& src_charset_name = charset_name_t<Char>()) const
	{
		base_type tmp(YGGR_PP_CHARSET_FOO_XCHG(base_type, src, src_charset_name, IN_CHARSET_NAME));
		this_type this_tmp(tmp, IN_CHARSET_NAME);

		for(this_type::const_iterator i = this_tmp.begin(), isize = this_tmp.end(); i != isize; ++i)
		{
			std::inserter(cont, cont.end()) = i.get_value();
			//set.insert(i.get_value());
		}
	}

	template<typename Char, typename Traits, typename Alloc, typename Container>
	void string_to_container(const utf8_string_impl<Char, Traits, Alloc>& src, Container& cont) const
	{
		typedef utf8_string_impl<Char, Traits, Alloc> string_type;
		typedef typename string_type::const_iterator iter_type;

		for(iter_type i = src.begin(), isize = src.end(); i != isize; ++i)
		{
			std::inserter(cont, cont.end()) = i.get_value();
			//set.insert(i.get_value());
		}
	}

private:
	size_type _utf8_size;
};

template<typename Char, typename Traits, typename Alloc>
struct is_utf8_string< utf8_string_impl<Char, Traits, Alloc> >
{
	typedef boost::mpl::true_ type;
};

} // namespace charset

template<typename Char, typename Traits, typename Alloc>
struct other_string< charset::utf8_string_impl<Char, Traits, Alloc> >
{
	typedef charset::utf8_string_impl<Char, Traits, Alloc> type;
};

} // namespace yggr

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
namespace boost
{
#else
namespace yggr
{
namespace charset
{
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

template<typename Char, typename Traits, typename Alloc>
std::size_t hash_value(const yggr::charset::utf8_string_impl<Char, Traits, Alloc>& str)
{
	return str.hash();
}

#ifdef BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP
} //namespace boost
#else
} // namespace charset
} // namespace yggr
#endif // BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP

//namespace boost
//{
//
//template<typename Char, typename Traits, typename Alloc>
//struct hash< yggr::charset::utf8_string_impl<Char, Traits, Alloc> >
//		: public std::unary_function< yggr::charset::utf8_string_impl<Char, Traits, Alloc>, std::size_t>
//{
//	typedef yggr::charset::utf8_string_impl<Char, Traits, Alloc> value_type;
//	std::size_t operator()(const value_type& val) const
//	{
//		return val.hash();
//	}
//};
//
//} // namespace boost

// io

template<typename Char, typename Traits, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
std::basic_ostream<Char, Traits>& operator<<(std::basic_ostream<Char, Traits>& os,
												const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& right)
{
	os << right.template str< yggr::charset::basic_string<Char> >();
	return os;
}

template<typename Char, typename Traits, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
std::basic_istream<Char, Traits>& operator>>(std::basic_istream<Char, Traits>& is,
												yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& right)
{
	yggr::charset::basic_string<Char> tmp;
	is >> tmp;
	right = tmp;
	return is;
}

// operator +

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+(const Char* l,
				const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	typedef yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;
	string_type str(l);
	return str.append(r);
}

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+( const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
				const Char* r)
{
	typedef yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;
	return string_type(l).append(r);
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T),
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
Basic_String<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>
	operator+(BOOST_RV_REF(Basic_String<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>) l,
				const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	typedef Basic_String<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)> string_type;

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	return l + r;
#else
	const string_type& l_cref = l;
	return l_cref + r.template str<string_type>();
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
Basic_String<Char, Traits, Alloc>
	operator+(const Basic_String<Char, Traits, Alloc>& l,
				const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	typedef Basic_String<Char, Traits, Alloc> string_type;
	return l + r.template str<string_type>();
}

template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T),
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
				class Basic_String >
yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+(const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
				BOOST_RV_REF(Basic_String<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>) r)
{
	typedef yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	//return string_type(l).append(boost::forward<string_type>(r));
	return string_type(l).append(r);
#else
	const string_type& r_cref = r;
	string_type ret(l);
	ret.append(r_cref);
	return ret;
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>
	operator+(const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
				const Basic_String<Char, Traits, Alloc>& r)
{
	typedef yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8> string_type;
	string_type ret(l);
	ret.append(r);
	return ret;
}

template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R>
yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L>
	operator+(const yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L>& l,
				const yggr::charset::utf8_string_impl<Char, Traits_R, Alloc_R>& r)
{
	typedef yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L> string_type;
	string_type ret(l);
	ret.append(r);
	return ret;
}

template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R>
yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L>
	operator+(BOOST_RV_REF(yggr::charset::utf8_string_impl<Char YGGR_PP_SYMBOL_COMMA() Traits_L YGGR_PP_SYMBOL_COMMA() Alloc_L>) l,
				const yggr::charset::utf8_string_impl<Char, Traits_R, Alloc_R>& r)
{
	typedef yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L> string_type;
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	return string_type(boost::forward<string_type>(l)).append(r);
#else
	string_type ret(boost::forward<string_type>(l));
	ret.append(r);
	return ret;
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
}

// operator ==
template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator==(const Char* l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return r.compare(l) == 0;
}

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator==( const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Char* r)
{
	return l.compare(r) == 0;
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator==(const Basic_String<Char, Traits, Alloc>& l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return r.compare(l) == 0;
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator==(const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Basic_String<Char, Traits, Alloc>& r)
{
	return l.compare(r) == 0;
}

template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R>
bool operator==(const yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L>& l,
					const yggr::charset::utf8_string_impl<Char, Traits_R, Alloc_R>& r)
{
	return l.compare(r) == 0;
}

// operator !=

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator!=(const Char* l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return r.compare(l) != 0;
}

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator!=( const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Char* r)
{
	return l.compare(r) != 0;
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator!=(const Basic_String<Char, Traits, Alloc>& l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return r.compare(l) != 0;
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator!=(const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Basic_String<Char, Traits, Alloc>& r)
{
	return l.compare(r) != 0;
}

template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R>
bool operator!=(const yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L>& l,
					const yggr::charset::utf8_string_impl<Char, Traits_R, Alloc_R>& r)
{
	return l.compare(r) != 0;
}

// operator <

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator<(const Char* l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return r.compare(l) > 0;
}

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator<( const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Char* r)
{
	return l.compare(r) < 0;
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator<(const Basic_String<Char, Traits, Alloc>& l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return r.compare(l) > 0;
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator<(const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Basic_String<Char, Traits, Alloc>& r)
{
	return l.compare(r) < 0;
}

template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R>
bool operator<(const yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L>& l,
					const yggr::charset::utf8_string_impl<Char, Traits_R, Alloc_R>& r)
{
	return l.compare(r) < 0;
}

// operator <=

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator<=(const Char* l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return !(r.compare(l) < 0);
}

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator<=( const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Char* r)
{
	return !(l.compare(r) > 0);
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator<=(const Basic_String<Char, Traits, Alloc>& l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return !(r.compare(l) < 0);
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator<=(const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Basic_String<Char, Traits, Alloc>& r)
{
	return !(l.compare(r) > 0);
}

template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R>
bool operator <=(const yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L>& l,
					const yggr::charset::utf8_string_impl<Char, Traits_R, Alloc_R>& r)
{
	return !(l.compare(r) > 0);
}

// operator >

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator>(const Char* l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return r.compare(l) < 0;
}

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator>( const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Char* r)
{
	return l.compare(r) > 0;
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator>(const Basic_String<Char, Traits, Alloc>& l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return r.compare(l) < 0;
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator>(const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Basic_String<Char, Traits, Alloc>& r)
{
	return l.compare(r) > 0;
}

template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R>
bool operator>(const yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L>& l,
					const yggr::charset::utf8_string_impl<Char, Traits_R, Alloc_R>& r)
{
	return l.compare(r) > 0;
}

// operator >=

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator>=(const Char* l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return !(r.compare(l) > 0);
}

template<typename Char, typename Char_utf8, typename Traits_utf8, typename Alloc_utf8>
bool operator>=( const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Char* r)
{
	return !(l.compare(r) < 0);
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator>=(const Basic_String<Char, Traits, Alloc>& l,
					const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& r)
{
	return !(r.compare(l) > 0);
}

template<typename Char, typename Traits, typename Alloc,
			typename Char_utf8, typename Traits_utf8, typename Alloc_utf8,
			template<typename _Char, typename _Traits, typename _Alloc>
			class Basic_String >
bool operator>=(const yggr::charset::utf8_string_impl<Char_utf8, Traits_utf8, Alloc_utf8>& l,
					const Basic_String<Char, Traits, Alloc>& r)
{
	return !(l.compare(r) < 0);
}

template<typename Char, typename Traits_L, typename Alloc_L, typename Traits_R, typename Alloc_R>
bool operator>=(const yggr::charset::utf8_string_impl<Char, Traits_L, Alloc_L>& l,
					const yggr::charset::utf8_string_impl<Char, Traits_R, Alloc_R>& r)
{
	return !(l.compare(r) < 0);
}

//swap

#define YGGR_PP_CHARSET_UTF8_STRING_IMPL_SWAP() \
template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline \
void swap(yggr::charset::utf8_string_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& l, \
			BOOST_RV_REF(yggr::charset::utf8_string_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>) r) { \
	l.swap(r); } \
	\
template<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T)> inline \
void swap(BOOST_RV_REF(yggr::charset::utf8_string_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>) l, \
			yggr::charset::utf8_string_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& r) { \
	r.swap(l); } \
	\
template< YGGR_PP_TEMPLATE_PARAMS_TYPES(3, typename T) > inline \
void swap(yggr::charset::utf8_string_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& l, \
			yggr::charset::utf8_string_impl<YGGR_PP_TEMPLATE_PARAMS_TYPES(3, T)>& r) { \
	l.swap(r); }


namespace std
{
	YGGR_PP_CHARSET_UTF8_STRING_IMPL_SWAP()
} // namespace std

namespace boost
{
	YGGR_PP_CHARSET_UTF8_STRING_IMPL_SWAP()
} // namespace boost

#undef YGGR_PP_CHARSET_UTF8_STRING_IMPL_SWAP

#endif //__YGGR_CHARSET_UTF8_STRING_IMPL_HPP__
