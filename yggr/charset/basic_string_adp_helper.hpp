//basic_string_adp_helper.hpp

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

#ifndef __YGGR_CHARSET_BASIC_STRING_ADP_HELPER_HPP__
#define __YGGR_CHARSET_BASIC_STRING_ADP_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/symbols.hpp>

#include <yggr/charset/string.hpp>
#include <yggr/charset/charset_base_foo.hpp>

#include <boost/ref.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>

#include <boost/type_traits/is_same.hpp>

#include <stdexcept>
#include <cstring>

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! why write the functions,									!!!
!!! because														!!!
!!! if the memory area is overlapped,							!!!
!!!	 all functions that use iterator access can cause problems.	!!!
!!!	for stl string tested msvc and gcc are normal,				!!!
!!!	 but boost::container::string will have a problem,			!!!
!!!	 In order to unify the semantics of two strings,			!!!
!!!	 we need to write the following functions					!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

// assign

// ------------------std::basic_string--------------------------

namespace yggr
{
namespace charset
{

// assign(n, char)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_assign(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type n,
							typename std::basic_string<Char, Traits, Alloc>::value_type c)
{
	obj.assign(n, c);
	return obj;
}

// assign(const char*)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_assign(std::basic_string<Char, Traits, Alloc>& obj, const Char* s)
{
	obj.assign(s);
	return obj;
}

// assign(const char*, n)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_assign(std::basic_string<Char, Traits, Alloc>& obj,
							const Char* s,
							typename std::basic_string<Char, Traits, Alloc>::size_type n)
{
	obj.assign(s, n);
	return obj;
}

// assign(string&&)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_assign(std::basic_string<Char, Traits, Alloc>& obj,
						BOOST_RV_REF_BEG std::basic_string<Char, Traits, Alloc> BOOST_RV_REF_END str)
{
	typedef std::basic_string<Char, Traits, Alloc> now_string_type;

	if(!(&obj == boost::addressof(static_cast<now_string_type&>(str))))
	{
		obj.assign(boost::move(str));
	}

	return obj;
}

// assign(const string&)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_assign(std::basic_string<Char, Traits, Alloc>& obj,
							const std::basic_string<Char, Traits, Alloc>& str)
{
	obj.assign(str);
	return obj;
}

// assign(const string&, subpos, sublen)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_assign(std::basic_string<Char, Traits, Alloc>& obj,
							const std::basic_string<Char, Traits, Alloc>& str,
							typename std::basic_string<Char, Traits, Alloc>::size_type subpos,
							typename std::basic_string<Char, Traits, Alloc>::size_type sublen)
{
	obj.assign(str, subpos, sublen);
	return obj;
}

// assign(iters, itere)
template<typename Char, typename Traits, typename Alloc, typename InputIter> inline
typename
	boost::enable_if
	<
		is_iterator<InputIter>,
		std::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_assign(std::basic_string<Char, Traits, Alloc>& obj,
							InputIter s, InputIter e)
{
	obj.assign(s, e);
	return obj;
}


// replace(initializer_list)
#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_assign(std::basic_string<Char, Traits, Alloc>& obj,
							std::initializer_list<Char> il)
{
	obj.assign(il);
	return obj;
}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

} // namespace charset
} // namespace yggr

// ------------------boost::container::basic_string-----------------------

namespace yggr
{
namespace charset
{

// assign(n, char)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_assign(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type n,
							typename boost::container::basic_string<Char, Traits, Alloc>::value_type c)
{
	return obj.assign(n, c);
}

// assign(const char*, n)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_assign(boost::container::basic_string<Char, Traits, Alloc>& obj,
							const Char* s,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type n)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;

	assert(s);
	if(obj.data() <= s && s <= obj.data() + obj.size())
	{
		// !!! Although this can handle this problem,
		// !!!  but in order to meet the semantics of STL string so here will throw error
		if(obj.data() + obj.size() < s + n)
		{
			throw std::out_of_range("basic_string");
		}

		if(n)
		{
			memcpy(&obj[0], s, sizeof(Char) * n);
			obj.erase(n);
		}
		else
		{
			obj.clear();
		}
	}
	else
	{
		obj.assign(s, n);
	}

	return obj;
}

// assign(const char*)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_assign(boost::container::basic_string<Char, Traits, Alloc>& obj, const Char* s)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;

	assert(s);
	return basic_string_assign(obj, s, charset_base_foo::strlen(s));
}

// assign(string&&)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_assign(boost::container::basic_string<Char, Traits, Alloc>& obj,
						BOOST_RV_REF_BEG boost::container::basic_string<Char, Traits, Alloc> BOOST_RV_REF_END str)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;

	if(!(&obj == boost::addressof(static_cast<now_string_type&>(str))))
	{
		obj.assign(boost::move(str));
	}

	return obj;
}

// assign(const string&)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_assign(boost::container::basic_string<Char, Traits, Alloc>& obj,
							const boost::container::basic_string<Char, Traits, Alloc>& str)
{
	return obj.assign(str);
}

// assign(const string&, subpos, sublen)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_assign(boost::container::basic_string<Char, Traits, Alloc>& obj,
							const boost::container::basic_string<Char, Traits, Alloc>& str,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type subpos,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type sublen)
{
	return obj.assign(str, subpos, sublen);
}

// assign(iters, itere)
template<typename Char, typename Traits, typename Alloc, typename InputIter> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_iterator<InputIter>,
			boost::mpl::not_< is_reverse_iterator<InputIter> >
		>,
		boost::container::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_assign(boost::container::basic_string<Char, Traits, Alloc>& obj,
							InputIter s, InputIter e)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;

	if((obj.data() <= &(*s)) && (&(*s) <= obj.data() + obj.size()))
	{
		basic_string_assign(obj, &(*s), std::distance(s, e));
	}
	else
	{
		obj.assign(s, e);
	}
	return obj;
}

template<typename Char, typename Traits, typename Alloc, typename InputIter> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_iterator<InputIter>,
			is_reverse_iterator<InputIter>
		>,
		boost::container::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_assign(boost::container::basic_string<Char, Traits, Alloc>& obj,
							InputIter s, InputIter e)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;
	typedef typename now_string_type::const_iterator now_citer_type;

	if((obj.data() <= &(*(e.base()))) && (&(*(e.base())) <= obj.data() + obj.size()))
	{
		basic_string_assign(obj, &(*(e.base())), std::distance(s, e));
		std::reverse(obj.begin(), obj.end());
	}
	else
	{
		obj.assign(s, e);
	}

	return obj;
}

// assign(initializer_list)
#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_assign(boost::container::basic_string<Char, Traits, Alloc>& obj,
							std::initializer_list<Char> il)
{
	return obj.assign(il);
}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

} // namespace charset
} // namespace yggr

// insert
// ------------------std::basic_string--------------------------

namespace yggr
{
namespace charset
{

// insert(xxx, n, c)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_insert(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type pos,
							typename std::basic_string<Char, Traits, Alloc>::size_type n,
							typename std::basic_string<Char, Traits, Alloc>::value_type c)
{
	obj.insert(pos, n, c);
	return obj;
}

template<typename Char, typename Traits, typename Alloc, typename Iter> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::iterator >,
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::const_iterator >
		>,
		typename std::basic_string<Char, Traits, Alloc>::iterator
	>::type
	basic_string_insert(std::basic_string<Char, Traits, Alloc>& obj,
							Iter p,
							typename std::basic_string<Char, Traits, Alloc>::size_type n,
							typename std::basic_string<Char, Traits, Alloc>::value_type c)
{
	typedef std::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;
	typedef typename now_string_type::const_iterator now_citer_type;

	now_size_type spos = std::distance<now_citer_type>(obj.begin(), p);
	obj.insert(p, n, c);
	return obj.begin() + spos;
}

// insert(iter, char)
template<typename Char, typename Traits, typename Alloc, typename Iter> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::iterator >,
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::const_iterator >
		>,
		typename std::basic_string<Char, Traits, Alloc>::iterator
	>::type
	basic_string_insert(std::basic_string<Char, Traits, Alloc>& obj,
							Iter p,
							typename std::basic_string<Char, Traits, Alloc>::value_type c)
{
	typedef std::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;
	typedef typename now_string_type::const_iterator now_citer_type;

	now_size_type spos = std::distance<now_citer_type>(obj.begin(), p);
	obj.insert(p, c);
	return obj.begin() + spos;
}

// insert(pos, const char*)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_insert(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type pos,
							const Char* s)
{
	obj.insert(pos, s);
	return obj;
}

// insert(pos, const char*, n)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_insert(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type pos,
							const Char* s,
							typename std::basic_string<Char, Traits, Alloc>::size_type n)
{
	obj.insert(pos, s, n);
	return obj;
}

// insert(pos, const string&)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_insert(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type pos,
							const std::basic_string<Char, Traits, Alloc>& str)
{
	obj.insert(pos, str);
	return obj;
}

// insert(pos, const string&, subpos, sublen)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_insert(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type pos,
							const std::basic_string<Char, Traits, Alloc>& str,
							typename std::basic_string<Char, Traits, Alloc>::size_type subpos,
							typename std::basic_string<Char, Traits, Alloc>::size_type sublen )
{
	obj.insert(pos, str, subpos, sublen);
	return obj;
}

// insert(iterp, iters, itere)
template<typename Char, typename Traits, typename Alloc, typename Iter, typename InputIter> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::or_
			<
				boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::iterator>,
				boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::const_iterator>
			>,
			is_iterator<InputIter>
		>,
		typename std::basic_string<Char, Traits, Alloc>::iterator
	>::type
	basic_string_insert(std::basic_string<Char, Traits, Alloc>& obj,
							Iter p, InputIter s, InputIter e)
{
	typedef std::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;
	typedef typename now_string_type::const_iterator now_citer_type;

	now_size_type spos = std::distance<now_citer_type>(obj.begin(), p);
	obj.insert(p, s, e);
	return obj.begin() + spos;
}

// insert(iterp, initializer_list)
#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_insert(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::const_iterator p,
							std::initializer_list<Char> il)
{
	obj.insert(p, il);
	return obj;
}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

} // namespace charset
} // namespace yggr


// ------------------boost::container::basic_string-----------------------

namespace yggr
{
namespace charset
{

// insert(xxx, n, c)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_insert(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type n,
							typename boost::container::basic_string<Char, Traits, Alloc>::value_type c)
{
	return obj.insert(pos, n, c);
}

template<typename Char, typename Traits, typename Alloc> inline
typename boost::container::basic_string<Char, Traits, Alloc>::iterator
	basic_string_insert(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator p,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type n,
							typename boost::container::basic_string<Char, Traits, Alloc>::value_type c)
{
	return obj.insert(p, n, c);
}

// insert(iter, char)
template<typename Char, typename Traits, typename Alloc> inline
typename boost::container::basic_string<Char, Traits, Alloc>::iterator
	basic_string_insert(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator p,
							typename boost::container::basic_string<Char, Traits, Alloc>::value_type c)
{
	return obj.insert(p, c);
}

// insert(pos, const char*, n)
template<typename Char, typename Traits, typename Alloc>
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_insert(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							const Char* s,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type n)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;

	bool is_same_addr = (obj.data() <= s && s <= obj.data() + obj.size());

	if((!(pos <= obj.size()))
		|| (is_same_addr && (obj.data() + obj.size() < s + n)))
	{
		throw std::out_of_range("basic_string");
	}

	now_size_type len = obj.size() - pos;
	now_size_type last_size = obj.size() + n;

	if(obj.capacity() < last_size)
	{
		now_string_type tmp(last_size, Char(0), obj.get_allocator());
		if(pos) memcpy(&tmp[0], &obj[0], sizeof(Char) * pos);
		if(n) memcpy(&tmp[pos], s, sizeof(Char) * n);
		if(len) memcpy(&tmp[pos + n], &obj[pos], sizeof(Char) * len);
		obj.swap(tmp);
	}
	else
	{
		if(is_same_addr)
		{
			now_size_type spos = std::distance<const Char*>(obj.data(), s); // boost 164 need std::distance<const Char*>
			obj.reserve(last_size + n);
			obj.resize(last_size + n);
			s = obj.data() + spos;
			if(n) memcpy(&obj[last_size], s, sizeof(Char) * n);
			//if(len) memcpy(&obj[pos + n], &obj[pos], sizeof(Char) * len);
			//if(n) memcpy(&obj[pos], &obj[last_size], sizeof(Char) * n);
			if(len) memmove(&obj[pos + n], &obj[pos], sizeof(Char) * len);
			if(n) memmove(&obj[pos], &obj[last_size], sizeof(Char) * n);
			obj.erase(last_size);
		}
		else
		{
			obj.resize(last_size);
			//if(len) memcpy(&obj[pos + n], &obj[pos], sizeof(Char) * len);
			if(len) memmove(&obj[pos + n], &obj[pos], sizeof(Char) * len);
			if(n) memcpy(&obj[pos], s, sizeof(Char) * n);
		}
	}

	return obj;
}

// insert(pos, const char*)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_insert(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							const Char* s)
{
	assert(s);
	return basic_string_insert(obj, pos, s, charset_base_foo::strlen(s));
}

// insert(iterp, iters, itere)
template<typename Char, typename Traits, typename Alloc, typename InputIter> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_iterator<InputIter>,
			boost::mpl::not_< is_reverse_iterator<InputIter> >
		>,
		typename boost::container::basic_string<Char, Traits, Alloc>::iterator
	>::type
	basic_string_insert(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator p,
							InputIter s, InputIter e)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;

	if((obj.data() <= &(*s)) && (&(*s) <= obj.data() + obj.size()))
	{
		now_size_type spos = std::distance(obj.cbegin(), p);
		basic_string_insert(obj, std::distance(obj.cbegin(), p), &(*s), std::distance(s, e));
		return obj.begin() + spos;
	}
	else
	{
		return obj.insert(p, s, e);
	}
}

// insert(pos, const string&)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_insert(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							const boost::container::basic_string<Char, Traits, Alloc>& str)
{
	return basic_string_insert(obj, pos, str.data(), str.size());
}

// insert(pos, const string&, subpos, sublen)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_insert(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							const boost::container::basic_string<Char, Traits, Alloc>& str,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type subpos,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type sublen )
{
	return basic_string_insert(obj, pos, str.data() + subpos, sublen);
}

template<typename Char, typename Traits, typename Alloc, typename InputIter> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_iterator<InputIter>,
			is_reverse_iterator<InputIter>
		>,
		typename boost::container::basic_string<Char, Traits, Alloc>::iterator
	>::type
	basic_string_insert(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator p,
							InputIter s, InputIter e)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;

	if( (obj.data() <= &(*(e.base()))) && (&(*(e.base())) <= obj.data() + obj.size()) )
	{
		now_size_type pos = std::distance(obj.cbegin(), p);
		now_size_type n = std::distance(s, e);

		basic_string_insert(obj, pos, &(*(e.base())), n);
		std::reverse(obj.begin() + pos, obj.begin() + (pos + n));
		return obj.begin() + pos;
	}
	else
	{
		return obj.insert(p, s, e);
	}
}

// insert(iterp, initializer_list)
#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_insert(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator p,
							std::initializer_list<Char> il)
{
	return obj.insert(p, il);
}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

} // namespace charset
} // namespace yggr

// replace
/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!!	failed example:												!!!
!!! {															!!!
!!!		boost::container::string str = "abc";					!!!
!!!		str.replace(1, 1, str, 0, 3);							!!!
!!!		assert(str != "aabcc");									!!!
!!!	}															!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

// ------------------std::basic_string--------------------------
namespace yggr
{
namespace charset
{

// replace(xxx, xxx, n, c)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type pos,
							typename std::basic_string<Char, Traits, Alloc>::size_type len,
							typename std::basic_string<Char, Traits, Alloc>::size_type n,
							typename std::basic_string<Char, Traits, Alloc>::value_type c)
{
	obj.replace(pos, len, n, c);
	return obj;
}

template<typename Char, typename Traits, typename Alloc, typename Iter> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::iterator>,
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::const_iterator>
		>,
		std::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							Iter i1, Iter i2,
							typename std::basic_string<Char, Traits, Alloc>::size_type n,
							typename std::basic_string<Char, Traits, Alloc>::value_type c)
{
	assert(i1 <= i2);
	obj.replace(i1, i2, n, c);
	return obj;
}

// replace(xxx, xxx, const char*)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type pos,
							typename std::basic_string<Char, Traits, Alloc>::size_type len,
							const Char* s)
{
	return obj.replace(pos, len, s);
}

template<typename Char, typename Traits, typename Alloc, typename Iter> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::iterator>,
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::const_iterator>
		>,
		std::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							Iter i1, Iter i2,
							const Char* s)
{
	assert(i1 <= i2);
	obj.replace(i1, i2, s);
	return obj;
}

// replace(xxx, xxx, const char*, n)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type pos,
							typename std::basic_string<Char, Traits, Alloc>::size_type len,
							const Char* s,
							typename std::basic_string<Char, Traits, Alloc>::size_type n)
{
	obj.replace(pos, len, s, n);
	return obj;
}

template<typename Char, typename Traits, typename Alloc, typename Iter> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::iterator>,
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::const_iterator>
		>,
		std::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							Iter i1, Iter i2,
							const Char* s,
							typename std::basic_string<Char, Traits, Alloc>::size_type n)
{
	assert(i1 <= i2);
	obj.replace(i1, i2, s, n);
	return obj;
}

// replace(xxx, xxx, string)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type pos,
							typename std::basic_string<Char, Traits, Alloc>::size_type len,
							const std::basic_string<Char, Traits, Alloc>& str)
{
	obj.replace(pos, len, str);
	return obj;
}

template<typename Char, typename Traits, typename Alloc, typename Iter> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::iterator>,
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::const_iterator>
		>,
		std::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							Iter i1, Iter i2,
							const std::basic_string<Char, Traits, Alloc>& str)
{
	assert(i1 <= i2);
	obj.replace(i1, i2, str);
	return obj;
}

// replace(pos, len, string, subpos, sublen)
template<typename Char, typename Traits, typename Alloc> inline
std::basic_string<Char, Traits, Alloc>&
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							typename std::basic_string<Char, Traits, Alloc>::size_type pos,
							typename std::basic_string<Char, Traits, Alloc>::size_type len,
							const std::basic_string<Char, Traits, Alloc>& str,
							typename std::basic_string<Char, Traits, Alloc>::size_type subpos,
							typename std::basic_string<Char, Traits, Alloc>::size_type sublen)
{
	obj.replace(pos, len, str, subpos, sublen);
	return obj;
}

// replace(i1, i2, s, e)
template<typename Char, typename Traits, typename Alloc, typename Iter, typename InputIter> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			boost::mpl::or_
			<
				boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::iterator>,
				boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::const_iterator>
			>,
			is_iterator<InputIter>
		>,
		std::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							Iter i1, Iter i2,
							InputIter s, InputIter e)
{
	assert(i1 <= i2);
	obj.replace(i1, i2, s, e);
	return obj;
}

// replace(i1, i2, initializer_list)
#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

template<typename Char, typename Traits, typename Alloc, typename Iter> inline
typename
	boost::enable_if
	<
		boost::mpl::or_
		<
			boost::is_same<Iter, typename std::basic_string<Char, Traits, Alloc>::const_iterator>
		>,
		std::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_replace(std::basic_string<Char, Traits, Alloc>& obj,
							Iter i1, Iter i2,
							std::initializer_list<Char> il)
{
	assert(i1 <= i2);
	obj.replace(i1, i2, il);
	return obj;
}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

} // namespace charset
} // namespace yggr

// ------------------boost::container::basic_string-----------------------

namespace yggr
{
namespace charset
{

// replace(xxx, xxx, n, c)
template<typename Char, typename Traits, typename Alloc>
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type len,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type n,
							typename boost::container::basic_string<Char, Traits, Alloc>::value_type c)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;

	if(!(pos <= obj.size()))
	{
		throw std::out_of_range("basic_string");
	}

	len = (std::min)(obj.size() - pos, len);

	if(n <= len)
	{
		std::fill(obj.begin() + pos, obj.begin() + (pos + n), c);
		obj.erase(pos + n, len - n);
	}
	else
	{
		now_size_type last_size = obj.size() - len + n;
		if(obj.capacity() < last_size)
		{
			now_string_type tmp(last_size, Char(0), obj.get_allocator());
			if(pos) memcpy(&tmp[0], &obj[0], sizeof(Char) * pos);
			std::fill(tmp.begin() + pos, tmp.begin() + (pos + n), c);
			//memcpy(&tmp[pos + n], &obj[pos + len], sizeof(Char) * (obj.size() - (pos + len)));
			now_size_type less_size = obj.size() - (pos + len);
			if(less_size) memcpy(&tmp[pos + n], &obj[pos + len], sizeof(Char) * less_size);
			obj.swap(tmp);
		}
		else
		{
			now_size_type less_size = obj.size() - (pos + len);
			obj.resize(last_size);
			//if(less_size) memcpy(&obj[pos + n], &obj[pos + len], sizeof(Char) * less_size);
			if(less_size) memmove(&obj[pos + n], &obj[pos + len], sizeof(Char) * less_size);
			std::fill(obj.begin() + pos, obj.begin() + (pos + n), c);
		}
	}
	return obj;
}

template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i1,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i2,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type n,
							typename boost::container::basic_string<Char, Traits, Alloc>::value_type c)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;
	typedef typename now_string_type::const_iterator now_citer_type;

	assert(i1 <= i2);
	return basic_string_replace(obj,
									static_cast<now_size_type>(std::distance<now_citer_type>(obj.begin(), i1)),
									static_cast<now_size_type>(std::distance(i1, i2)),
									n, c);
}

// replace(xxx, xxx, const char*, n) // decl
template<typename Char, typename Traits, typename Alloc>
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type len,
							const Char* s,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type n);


// replace(xxx, xxx, const char*)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type len,
							const Char* s)
{
	return basic_string_replace(obj, pos, len, s, charset_base_foo::strlen(s));
}

template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i1,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i2,
							const Char* s)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;
	typedef typename now_string_type::const_iterator now_citer_type;

	assert(i1 <= i2);
	return basic_string_replace(obj,
									static_cast<now_size_type>(std::distance<now_citer_type>(obj.begin(), i1)),
									static_cast<now_size_type>(std::distance(i1, i2)),
									s, charset_base_foo::strlen(s));
}

// replace(xxx, xxx, const char*, n) // impl
template<typename Char, typename Traits, typename Alloc>
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type len,
							const Char* s,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type n)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;

	bool is_same_addr = (obj.data() <= s && s <= obj.data() + obj.size());

	if((!(pos <= obj.size()))
		|| ( is_same_addr && (obj.data() + obj.size() < s + n )) )
	{
		throw(std::out_of_range("basic_string"));
	}

	len = (std::min)(obj.size() - pos, len);

	if(n <= len)
	{
		if(n) memcpy(&obj[pos], s, sizeof(Char) * n);
		obj.erase(pos + n, len - n);
	}
	else
	{
		now_size_type last_size = obj.size() - len + n;
		if(obj.capacity() < last_size)
		{
			now_string_type tmp(last_size, Char(0), obj.get_allocator());
			if(pos) memcpy(&tmp[0], &obj[0], sizeof(Char) * pos);
			if(n) memcpy(&tmp[pos], s, sizeof(Char) * n);
			//memcpy(&tmp[pos + n], &obj[pos + len], sizeof(Char) * (obj.size() - (pos + len)));
			now_size_type less_size = obj.size() - (pos + len);
			if(less_size) memcpy(&tmp[pos + n], &obj[pos + len], sizeof(Char) * less_size);
			obj.swap(tmp);
		}
		else
		{
			//now_size_type old_size = obj.size();
			now_size_type less_size = obj.size() - (pos + len);
			if(is_same_addr)
			{
				now_size_type spos = std::distance<const Char*>(obj.data(), s); // boost 164 need std::distance<const Char*>
				//obj.reserve(last_size + n);
				obj.resize(last_size + n);
				s = obj.data() + spos;
				if(n) memcpy(&obj[last_size], s, sizeof(Char) * n);
				//if(less_size) memcpy(&obj[pos + n], &obj[pos + len], sizeof(Char) * less_size);
				//if(n) memcpy(&obj[pos], &obj[last_size], sizeof(Char) * n);
				if(less_size) memmove(&obj[pos + n], &obj[pos + len], sizeof(Char) * less_size);
				if(n) memmove(&obj[pos], &obj[last_size], sizeof(Char) * n);
				if(last_size < obj.size()) obj.erase(last_size);
			}
			else
			{
				obj.resize(last_size);
				//if(less_size) memcpy(&obj[pos + n], &obj[pos + len], sizeof(Char) * less_size);
				if(less_size) memmove(&obj[pos + n], &obj[pos + len], sizeof(Char) * less_size);
				if(n) memcpy(&obj[pos], s, sizeof(Char) * n);
			}
		}
	}

	return obj;
}


template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i1,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i2,
							const Char* s,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type n)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;
	typedef typename now_string_type::const_iterator now_citer_type;

	assert(i1 <= i2);
	return basic_string_replace(obj,
									static_cast<now_size_type>(std::distance<now_citer_type>(obj.begin(), i1)),
									static_cast<now_size_type>(std::distance(i1, i2)),
									s, n);
}

// replace(xxx, xxx, string)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type len,
							const boost::container::basic_string<Char, Traits, Alloc>& str)
{
	return basic_string_replace(obj, pos, len, str.data(), str.size());
}

template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i1,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i2,
							const boost::container::basic_string<Char, Traits, Alloc>& str)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::size_type now_size_type;
	typedef typename now_string_type::const_iterator now_citer_type;

	assert(i1 <= i2);
	return basic_string_replace(obj,
									static_cast<now_size_type>(std::distance<now_citer_type>(obj.begin(), i1)),
									static_cast<now_size_type>(std::distance(i1, i2)),
									str.data(), str.size());
}

// replace(pos, len, string, subpos, sublen)
template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type len,
							const boost::container::basic_string<Char, Traits, Alloc>& str,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type subpos,
							typename boost::container::basic_string<Char, Traits, Alloc>::size_type sublen)
{
	return basic_string_replace(obj, pos, len, str.data() + subpos, sublen);
}

// replace(i1, i2, s, e)
template<typename Char, typename Traits, typename Alloc, typename InputIter> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_iterator<InputIter>,
			boost::mpl::not_< is_reverse_iterator<InputIter> >
		>,
		boost::container::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i1,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i2,
							InputIter s, InputIter e)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::const_iterator now_citer_type;
	typedef typename now_string_type::size_type now_size_type;

	assert(i1 <= i2);

	if(!((obj.cbegin() <= i1 && i1 <= obj.cend())
			&& (obj.cbegin() <= i2 && i2 <= obj.cend())))
	{
		throw(std::out_of_range("basic_string"));
	}

	if((obj.data() <= &(*s)) && (&(*s) <= obj.data() + obj.size()))
	{
		basic_string_replace(obj,
								std::distance(obj.cbegin(), i1),
								std::distance(i1, i2),
								&(*s),
								std::distance(s, e));
	}
	else
	{
		obj.replace(i1, i2, s, e);
	}

	return obj;
}

template<typename Char, typename Traits, typename Alloc, typename InputIter> inline
typename
	boost::enable_if
	<
		boost::mpl::and_
		<
			is_iterator<InputIter>,
			is_reverse_iterator<InputIter>
		>,
		boost::container::basic_string<Char, Traits, Alloc>&
	>::type
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i1,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i2,
							InputIter s, InputIter e)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	typedef typename now_string_type::const_iterator now_citer_type;
	typedef typename now_string_type::size_type now_size_type;

	assert(i1 <= i2);

	if(!((obj.cbegin() <= i1 && i1 <= obj.cend())
			&& (obj.cbegin() <= i2 && i2 <= obj.cend())))
	{
		throw(std::out_of_range("basic_string"));
	}

	if(obj.data() <= &(*(e.base())) && &(*(e.base())) <= obj.data() + obj.size()) // address same string replace
	{
		now_size_type pos = std::distance(obj.cbegin(), i1);
		now_size_type n = std::distance(s, e);

		basic_string_replace(obj, pos,
								std::distance(i1, i2),
								&(*(e.base())),
								std::distance(s, e));

		std::reverse(obj.begin() + pos, obj.begin() + (pos + n));
	}
	else
	{
		obj.replace(i1, i2, s, e);
	}

	return obj;
}

// replace(i1, i2, initializer_list)
#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

template<typename Char, typename Traits, typename Alloc> inline
boost::container::basic_string<Char, Traits, Alloc>&
	basic_string_replace(boost::container::basic_string<Char, Traits, Alloc>& obj,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i1,
							typename boost::container::basic_string<Char, Traits, Alloc>::const_iterator i2,
							std::initializer_list<Char> il)
{
	assert(i1 <= i2);
	return obj.replace(i1, i2, il);
}

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

} // namespace charset
} // namespace yggr

/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!!! failed example:																!!!
!!!	{																			!!!
!!!		std::string str;														!!!
!!!		assert(str.find(std::string()) == 0);									!!!
!!!	}																			!!!
!!!	but																			!!!
!!!	{																			!!!
!!!		boost::string str;														!!!
!!!		assert(str.find(boost::container::string()) == boost::string::npos);	!!!
!!!	}																			!!!
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

//--------------------std::basic_string---------------------------------
namespace yggr
{
namespace charset
{

template<typename Char, typename Traits, typename Alloc> inline
typename std::basic_string<Char, Traits, Alloc>::size_type
	basic_string_find(const std::basic_string<Char, Traits, Alloc>& obj,
						typename std::basic_string<Char, Traits, Alloc>::value_type c,
						typename std::basic_string<Char, Traits, Alloc>::size_type pos)
{
	return obj.find(c, pos);
}

template<typename Char, typename Traits, typename Alloc> inline
typename std::basic_string<Char, Traits, Alloc>::size_type
	basic_string_find(const std::basic_string<Char, Traits, Alloc>& obj,
						const Char* s,
						typename std::basic_string<Char, Traits, Alloc>::size_type pos)
{
	return obj.find(s, pos);
}

template<typename Char, typename Traits, typename Alloc> inline
typename std::basic_string<Char, Traits, Alloc>::size_type
	basic_string_find(const std::basic_string<Char, Traits, Alloc>& obj,
						const Char* s,
						typename std::basic_string<Char, Traits, Alloc>::size_type pos,
						typename std::basic_string<Char, Traits, Alloc>::size_type n)
{
	return obj.find(s, pos, n);
}

template<typename Char, typename Traits, typename Alloc> inline
typename std::basic_string<Char, Traits, Alloc>::size_type
	basic_string_find(const std::basic_string<Char, Traits, Alloc>& obj,
						const std::basic_string<Char, Traits, Alloc>& str,
						typename std::basic_string<Char, Traits, Alloc>::size_type pos)
{
	return obj.find(str, pos);
}

} // namespace charset
} // namespace yggr

// ------------------boost::container::basic_string-----------------------

namespace yggr
{
namespace charset
{

template<typename Char, typename Traits, typename Alloc> inline
typename boost::container::basic_string<Char, Traits, Alloc>::size_type
	basic_string_find(const boost::container::basic_string<Char, Traits, Alloc>& obj,
						typename boost::container::basic_string<Char, Traits, Alloc>::value_type c,
						typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos)
{
	return obj.find(c, pos);
}

template<typename Char, typename Traits, typename Alloc> inline
typename boost::container::basic_string<Char, Traits, Alloc>::size_type
	basic_string_find(const boost::container::basic_string<Char, Traits, Alloc>& obj,
						const Char* s,
						typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	assert(s);
	return s[0]? (obj.find(s, pos)) : (pos < obj.size()? pos : now_string_type::npos);
}

template<typename Char, typename Traits, typename Alloc> inline
typename boost::container::basic_string<Char, Traits, Alloc>::size_type
	basic_string_find(const boost::container::basic_string<Char, Traits, Alloc>& obj,
						const Char* s,
						typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos,
						typename boost::container::basic_string<Char, Traits, Alloc>::size_type n)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	assert(s);
	return n? (obj.find(s, pos, n)) : (pos < obj.size()? pos : now_string_type::npos);
}

template<typename Char, typename Traits, typename Alloc> inline
typename boost::container::basic_string<Char, Traits, Alloc>::size_type
	basic_string_find(const boost::container::basic_string<Char, Traits, Alloc>& obj,
						const boost::container::basic_string<Char, Traits, Alloc>& str,
						typename boost::container::basic_string<Char, Traits, Alloc>::size_type pos)
{
	typedef boost::container::basic_string<Char, Traits, Alloc> now_string_type;
	return str.empty()? (pos < obj.size()? pos : now_string_type::npos) : (obj.find(str, pos));
}

} // namespace charset
} // namespace yggr


#endif // __YGGR_CHARSET_BASIC_STRING_ADP_HELPER_HPP__
