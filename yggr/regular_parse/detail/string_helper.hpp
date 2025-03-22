//string_helper.hpp

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

#ifndef __YGGR_REGULAR_PARSE_STRING_ITERATOR_HELPER_HPP__
#define __YGGR_REGULAR_PARSE_STRING_ITERATOR_HELPER_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/null_t.hpp>

#include <yggr/iterator_ex/iterator_category_check.hpp>

#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>

#include <yggr/charset/utf8_string_impl.hpp>
#include <yggr/charset/utf8_string_view_impl.hpp>

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_class.hpp>

#include <boost/range/functions.hpp>

namespace yggr
{
namespace regular_parse
{
namespace detail
{

// string_helper_failed
template<typename T>
struct string_helper_failed
{
private:
	typedef mplex::null_type failed_type;

public:
	typedef T cv_cont_type;
	typedef typename boost::remove_cv<cv_cont_type>::type cont_type;
	typedef cont_type base_cont_type;

	typedef mplex::null_type value_type;
	typedef mplex::null_type iter_type;
	typedef mplex::null_type citer_type;

	typedef mplex::null_type result_cont_type;

	template<typename Cont> inline
	static failed_type begin(Cont&)
	{
		failed_type failed;
		return failed;
	}

	template<typename Cont> inline
	static failed_type end(Cont&)
	{
		failed_type failed;
		return failed;
	}

	template<typename Cont> inline
	static failed_type org(Cont&)
	{
		failed_type failed;
		return failed;
	}

	template<typename Cont> inline
	static void recount(Cont&)
	{
		return;
	}

	template<typename Cont> inline
	static mplex::null_type c_str(const Cont&)
	{
		failed_type failed;
		return failed;
	}

	template<typename Cont> inline
	static failed_type create_empty_result_cont(Cont&)
	{
		failed_type failed;
		return failed;
	}
};

// string_helper_rangeable
template<typename T, bool rangeable = boost::is_class<T>::value>
struct string_helper_rangeable;

template<typename T>
struct string_helper_rangeable<T, false>
	: public string_helper_failed<T>
{
protected:
	typedef string_helper_failed<T> base_type;

public:
	typedef typename base_type::cv_cont_type cv_cont_type;
	typedef typename base_type::cont_type cont_type;
	typedef typename base_type::base_cont_type base_cont_type;

	typedef typename base_type::value_type value_type;
	typedef typename base_type::iter_type iter_type;
	typedef typename base_type::citer_type citer_type;

	typedef typename base_type::result_cont_type result_cont_type;

	using base_type::begin;
	using base_type::end;
	using base_type::org;
	using base_type::recount;
	using base_type::c_str;
	using base_type::create_empty_result_cont;
};

template<typename T>
struct string_helper_rangeable<T, true>
{
	typedef T cv_cont_type;
	typedef typename boost::remove_cv<cv_cont_type>::type cont_type;
	typedef cont_type base_cont_type;

	typedef typename range_ex::range_value_ex<cont_type>::type value_type;
	typedef typename range_ex::range_iterator_ex<cont_type>::type iter_type;
	typedef typename range_ex::range_iterator_ex<const cont_type>::type citer_type;

	typedef cont_type result_cont_type;

	template<typename Cont> inline
	static typename range_ex::range_iterator_ex<Cont>::type begin(Cont& t)
	{
		return boost::begin(t);
	}

	template<typename Cont> inline
	static typename range_ex::range_iterator_ex<Cont>::type end(Cont& t)
	{
		return boost::end(t);
	}

	template<typename Cont> inline
	static Cont& org(Cont& t)
	{
		return t;
	}

	template<typename Cont> inline
	static void recount(Cont&)
	{
		return;
	}

	template<typename Cont> inline
	static const value_type* c_str(const Cont& t)
	{
		return boost::empty(t)? 0 : &(*boost::begin(t));
	}

	template<typename Cont> inline
	static result_cont_type create_empty_result_cont(Cont& t)
	{
		return container::create_empty_object_of_allocator(t);
	}
};

// string_helper
template<typename T, 
			bool is_pointer = boost::is_pointer<T>::value, 
			bool is_outiter = is_output_iterator<T>::value>
struct string_helper
	: public string_helper_failed<T>
{
protected:
	typedef string_helper_failed<T> base_type;

public:
	typedef typename base_type::cv_cont_type cv_cont_type;
	typedef typename base_type::cont_type cont_type;
	typedef typename base_type::base_cont_type base_cont_type;

	typedef typename base_type::value_type value_type;
	typedef typename base_type::iter_type iter_type;
	typedef typename base_type::citer_type citer_type;

	typedef typename base_type::result_cont_type result_cont_type;

	using base_type::begin;
	using base_type::end;
	using base_type::org;
	using base_type::recount;
	using base_type::c_str;
	using base_type::create_empty_result_cont;
};

template<typename T>
struct string_helper<T, false, false>
	: public string_helper_rangeable<T>
{
protected:
	typedef string_helper_rangeable<T> base_type;

public:
	typedef typename base_type::cv_cont_type cv_cont_type;
	typedef typename base_type::cont_type cont_type;
	typedef typename base_type::base_cont_type base_cont_type;

	typedef typename base_type::value_type value_type;
	typedef typename base_type::iter_type iter_type;
	typedef typename base_type::citer_type citer_type;

	typedef typename base_type::result_cont_type result_cont_type;

	using base_type::begin;
	using base_type::end;
	using base_type::org;
	using base_type::recount;
	using base_type::c_str;
	using base_type::create_empty_result_cont;
};

//template<typename T>
//struct string_helper<T, false, false>
//{
//	typedef T cv_cont_type;
//	typedef typename boost::remove_cv<cv_cont_type>::type cont_type;
//	typedef cont_type base_cont_type;
//
//	typedef typename boost::range_value<cont_type>::type value_type;
//	typedef typename boost::range_iterator<cont_type>::type iter_type;
//	typedef typename boost::range_iterator<const cont_type>::type citer_type;
//
//	typedef cont_type result_cont_type;
//
//	template<typename Cont> inline
//	static typename boost::range_iterator<Cont>::type begin(Cont& t)
//	{
//		return boost::begin(t);
//	}
//
//	template<typename Cont> inline
//	static typename boost::range_iterator<Cont>::type end(Cont& t)
//	{
//		return boost::end(t);
//	}
//
//	template<typename Cont> inline
//	static Cont& org(Cont& t)
//	{
//		return t;
//	}
//
//	template<typename Cont> inline
//	static void recount(Cont&)
//	{
//		return;
//	}
//
//	template<typename Cont> inline
//	static const value_type* c_str(const Cont& t)
//	{
//		return boost::empty(t)? 0 : &(*boost::begin(t));
//	}
//
//	template<typename Cont> inline
//	static result_cont_type create_empty_result_cont(Cont& t)
//	{
//		return container::create_empty_object_of_allocator(t);
//	}
//};

template<typename T, std::size_t N>
struct string_helper<T[N], false, false>
{
	typedef T cv_cont_type[N];

	typedef typename boost::remove_cv<cv_cont_type>::type cont_type;
	typedef cont_type base_cont_type;

	typedef typename boost::range_value<cont_type>::type value_type;
	typedef typename boost::range_iterator<cont_type>::type iter_type;
	typedef typename boost::range_iterator<const cont_type>::type citer_type;

	typedef charset::basic_string<value_type> result_cont_type;

	template<typename Cont> inline
	static typename boost::range_iterator<Cont>::type begin(Cont& t)
	{
		return boost::begin(t);
	}

	template<typename Cont> inline
	static typename boost::range_iterator<Cont>::type end(Cont& t)
	{
		return t[N - 1]?
				std::find(boost::begin(t), boost::end(t), value_type())
				: boost::begin(t) + charset::charset_base_foo::strlen(boost::begin(t));
	}

	template<typename Cont> inline
	static Cont& org(Cont& t)
	{
		return t;
	}

	template<typename Cont> inline
	static void recount(Cont&)
	{
		return;
	}

	template<typename Cont> inline
	static const value_type* c_str(const Cont& t)
	{
		return boost::begin(t);
	}

	template<typename Cont> inline
	static result_cont_type create_empty_result_cont(Cont&)
	{
		return result_cont_type();
	}
};

template<typename T>
struct string_helper<T, true, true>
{
	typedef T cv_cont_type;
	typedef typename boost::remove_cv<cv_cont_type>::type cont_type;
	typedef cont_type base_cont_type;

	typedef typename boost::iterator_value<cont_type>::type value_type;
	typedef value_type* iter_type;
	typedef const value_type* citer_type;

	typedef charset::basic_string<value_type> result_cont_type;

	template<typename Pointer> inline
	static Pointer begin(Pointer t)
	{
		return t;
	}

	template<typename Pointer> inline
	static Pointer end(Pointer t)
	{
		return t + charset::charset_base_foo::strlen(t);
	}

	template<typename Pointer> inline
	static Pointer org(Pointer t)
	{
		return t;
	}

	template<typename Pointer> inline
	static void recount(Pointer)
	{
		return;
	}

	template<typename Pointer> inline
	static Pointer c_str(Pointer t)
	{
		return t;
	}

	template<typename Cont> inline
	static result_cont_type create_empty_result_cont(Cont&)
	{
		return result_cont_type();
	}
};

// msvc not support
//template<typename T, bool is_xxx>
//struct string_helper<T, true, is_xxx>

template<typename T>
struct string_helper<T, true, false>
	: public string_helper<T, true, true>
{
};

template<typename Char, typename Traits, typename Alloc>
struct string_helper<charset::utf8_string_impl<Char, Traits, Alloc>, false, false>
{
	typedef charset::utf8_string_impl<Char, Traits, Alloc> cont_type;
	typedef typename cont_type::base_type base_cont_type;

	typedef typename cont_type::value_type value_type;
	typedef typename base_cont_type::iterator iter_type;
	typedef typename base_cont_type::const_iterator citer_type;

	typedef cont_type result_cont_type;

	YGGR_ASSERT_CHAR_TYPE(value_type);

	inline static iter_type begin(cont_type& t)
	{
		return t.org_str().begin();
	}

	inline static citer_type begin(const cont_type& t)
	{
		return t.org_str().begin();
	}

	inline static iter_type end(cont_type& t)
	{
		return t.org_str().end();
	}

	inline static citer_type end(const cont_type& t)
	{
		return t.org_str().end();
	}

	inline static base_cont_type& org(cont_type& t)
	{
		return t.org_str();
	}

	inline static const base_cont_type& org(const cont_type& t)
	{
		return t.org_str();
	}

	inline static void recount(cont_type& t)
	{
		t.recount_length();
	}

	inline static const value_type* c_str(const cont_type& t)
	{
		return t.data();
	}

	inline static result_cont_type create_empty_result_cont(const cont_type& t)
	{
		return result_cont_type(t.get_allocator());
	}
};

template<typename Char, typename Traits>
struct string_helper<charset::basic_utf8_string_view<Char, Traits>, false, false>
{
	typedef charset::basic_utf8_string_view<Char, Traits> cont_type;
	typedef typename cont_type::base_type base_cont_type;

	typedef typename cont_type::value_type value_type;
	typedef typename base_cont_type::iterator iter_type;
	typedef typename base_cont_type::const_iterator citer_type;

	typedef cont_type result_cont_type;

	YGGR_ASSERT_CHAR_TYPE(value_type);

	inline static iter_type begin(cont_type& t)
	{
		return t.org_str().begin();
	}

	inline static citer_type begin(const cont_type& t)
	{
		return t.org_str().begin();
	}

	inline static iter_type end(cont_type& t)
	{
		return t.org_str().end();
	}

	inline static citer_type end(const cont_type& t)
	{
		return t.org_str().end();
	}

	inline static base_cont_type& org(cont_type& t)
	{
		return t.org_str();
	}

	inline static const base_cont_type& org(const cont_type& t)
	{
		return t.org_str();
	}

	inline static void recount(cont_type& t)
	{
		t.recount_length();
	}

	inline static const value_type* c_str(const cont_type& t)
	{
		return t.data();
	}

	inline static result_cont_type create_empty_result_cont(const cont_type& t)
	{
		return result_cont_type(t.get_allocator());
	}
};

} // namespace detail
} // regular_parse
} // namespace yggr

#endif // __YGGR_REGULAR_PARSE_STRING_ITERATOR_HELPER_HPP__
