//stl_exception.hpp

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

#ifndef __YGGR_COMPATIBILITY_STL_EXCEPTION_HPP__
#define __YGGR_COMPATIBILITY_STL_EXCEPTION_HPP__

#if defined(__GNUC__) && (__GNUC__ < 5) && !defined(__clang__)
#include <boost/move/move.hpp>
#include <algorithm>
#include <string>
#endif // #if defined(__GNUC__) && (__GNUC__ < 5)

#include <exception>

#if defined(__GNUC__) && (__GNUC__ < 5) && !defined(__clang__)

#if !defined(_GLIBCXX_USE_NOEXCEPT)
#	define _GLIBCXX_USE_NOEXCEPT
#endif // _GLIBCXX_USE_NOEXCEPT

#endif // #if defined(__GNUC__) && (__GNUC__ < 5) && !defined(__clang__)_

namespace yggr
{
namespace compatibility
{
#if defined(__GNUC__) && (__GNUC__ < 5) && !defined(__clang__)

class stl_exception
	: public std::exception
{
public:
	typedef std::exception base_type;

private:
	typedef stl_exception this_type;
	// don't use it because loop throw
	//BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	stl_exception(void) _GLIBCXX_USE_NOEXCEPT;
	stl_exception(const char* msg) _GLIBCXX_USE_NOEXCEPT;
	stl_exception(const char* msg, unsigned long code) _GLIBCXX_USE_NOEXCEPT;
	stl_exception(const std::string& msg) _GLIBCXX_USE_NOEXCEPT;
	stl_exception(const std::string& msg, unsigned long code) _GLIBCXX_USE_NOEXCEPT;

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
	stl_exception(this_type&& right) _GLIBCXX_USE_NOEXCEPT;
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	stl_exception(const this_type& right) _GLIBCXX_USE_NOEXCEPT;
	virtual ~stl_exception() _GLIBCXX_USE_NOEXCEPT;

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
	this_type& operator=(this_type&& right) _GLIBCXX_USE_NOEXCEPT;
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	this_type& operator=(const this_type& right) _GLIBCXX_USE_NOEXCEPT;

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
	void swap(this_type&& right) _GLIBCXX_USE_NOEXCEPT;
#endif // BOOST_NO_CXX11_RVALUE_REFERENCES

	void swap(this_type& right) _GLIBCXX_USE_NOEXCEPT;

	virtual const char* what(void) const _GLIBCXX_USE_NOEXCEPT;

private:
	unsigned long _code;
	std::string _msg;
};

#else
	typedef std::exception stl_exception;
#endif // #if defined(__GNUC__) && (__GNUC__ < 5) && !defined(__clang__)

} // namespace compatibility

using compatibility::stl_exception;

} // namespace yggr

// swap

#if defined(__GNUC__) && (__GNUC__ < 5) && !defined(__clang__)

namespace yggr
{
namespace compatibility
{
namespace swap_support
{

inline void swap(stl_exception& l, stl_exception& r) _GLIBCXX_USE_NOEXCEPT
{
	l.swap(r);
}

} // namespace swap_support

using swap_support::swap;

} // namespace compatibility
} // namespace yggr

namespace std
{
	using yggr::compatibility::swap_support::swap;
} // namespace std

namespace boost
{
	using yggr::compatibility::swap_support::swap;
} // namespace boost

#endif // #if defined(__GNUC__) && (__GNUC__ < 5) && !defined(__clang__)

#endif // __YGGR_COMPATIBILITY_STL_EXCEPTION_HPP__
