//has_insert_about.hpp

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

#ifndef __YGGR_CONTAINER_DETAIL_HAS_INSERT_ABOUT_HPP__
#define __YGGR_CONTAINER_DETAIL_HAS_INSERT_ABOUT_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/range_ex/range_value_ex.hpp>
#include <yggr/range_ex/range_iterator_ex.hpp>
#include <yggr/typeof/typeof.hpp>
#include <yggr/container/detail/has_insert.hpp>


namespace yggr
{
namespace container
{
namespace detail
{

/* 
why not use YGGR_PP_HAS_MEMBER_FOO_DEF?
because msvc stl set_mset's "pair<iter, bool> insert(const value)" 
	and "iter insert(const value)" function is not implemented
*/ 

// has_insert_about_return_pair

template<typename Cont, bool has_insert = has_any_insert<Cont>::value> 
struct __detail_has_insert_about_return_pair 
{ 
private: 
	typedef Cont cont_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_val_type;
	typedef typename range_ex::range_iterator_ex<cont_type>::type cont_iter_type;
	typedef std::pair<cont_iter_type, bool> ret_type;

private:
	static cont_type& prv_s_get_cont(void);

public: 
	typedef 
		boost::mpl::bool_
		<
			(sizeof(prv_s_get_cont().insert(declval<cont_val_type>())) == sizeof(ret_type))
		> type; 
};

template<typename Cont> 
struct __detail_has_insert_about_return_pair<Cont, false> 
	: boost::mpl::false_
{
};

template<typename Cont> 
struct has_insert_about_return_pair 
	: public __detail_has_insert_about_return_pair<Cont>::type 
{
};

// hash_insert_about_return_iterator

template<typename Cont, bool has_insert = has_any_insert<Cont>::value> 
struct __detail_has_insert_about_return_iterator
{ 
private: 
	typedef Cont cont_type;
	typedef typename range_ex::range_value_ex<cont_type>::type cont_val_type;
	typedef typename range_ex::range_iterator_ex<cont_type>::type cont_iter_type;
	typedef cont_iter_type ret_type;

private:
	static cont_type& prv_s_get_cont(void);

public: 
	typedef 
		boost::mpl::bool_
		<
			(sizeof(prv_s_get_cont().insert(declval<cont_val_type>())) == sizeof(ret_type))
		> type; 
};

template<typename Cont> 
struct __detail_has_insert_about_return_iterator<Cont, false> 
	: boost::mpl::false_
{
};

template<typename Cont> 
struct has_insert_about_return_iterator
	: public __detail_has_insert_about_return_iterator<Cont>::type 
{
};

} // namespace detail
} // namespace container
} // namespace yggr

#endif // __YGGR_CONTAINER_DETAIL_HAS_INSERT_ABOUT_HPP__
