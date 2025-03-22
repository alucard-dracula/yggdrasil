//original_t.hpp

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

#ifndef __YGGR_TYPE_TRAITS_ORIGINAL_T_HPP__
#define __YGGR_TYPE_TRAITS_ORIGINAL_T_HPP__

#include <yggr/type_traits/detail/original_t_detail.hpp>
#include <boost/move/core.hpp> // BOOST_RV_REF

namespace yggr
{
namespace detail
{

template<typename T>
struct original_pointer_t_detail
{
private:
	typedef typename original_t_detail<T>::type t1_type;

public:
	typedef t1_type* type;
};

template<typename T>
struct original_const_pointer_t_detail
{
private:
	typedef typename original_t_detail<T>::type t1_type;

public:
	typedef const t1_type* type;
};

template<typename T>
struct original_ref_t_detail
{
private:
	typedef typename original_t_detail<T>::type t1_type;

public:
	typedef t1_type& type;
};

template<typename T>
struct original_cref_t_detail
{
private:
	typedef typename original_t_detail<T>::type t1_type;

public:
	typedef const t1_type& type;
};

template<typename T>
struct original_rvref_t_detail
{
private:
	typedef typename original_t_detail<T>::type t1_type;

public:
	typedef BOOST_RV_REF(t1_type) type;
};

} // namespace detail

template<typename T>
struct original_t
	: public detail::original_t_detail<T>
{
};

template<typename T>
struct original_pointer_t
	: public detail::original_pointer_t_detail<T>
{
};

template<typename T>
struct original_const_pointer_t
	: public detail::original_const_pointer_t_detail<T>
{
};

template<typename T>
struct original_ref_t
	: public detail::original_ref_t_detail<T>
{
};

template<typename T>
struct original_cref_t
	: public detail::original_cref_t_detail<T>
{
};

template<typename T>
struct original_rvref_t
	: public detail::original_rvref_t_detail<T>
{
};

}// namespace yggr

#endif // __YGGR_TYPE_TRAITS_ORIGINAL_T_HPP__