//type_info.hpp

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

#ifndef __YGGR_UTILITY_TYPEINFO_HPP__
#define __YGGR_UTILITY_TYPEINFO_HPP__

#include <yggr/base/yggrdef.h>

#include <boost/functional/hash/hash.hpp>
#include <boost/ref.hpp>

#include <typeinfo>

namespace boost
{

// btypeinfo hash
template<>
struct hash< std::type_info >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< std::type_info, std::size_t >
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef const std::type_info& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const std::type_info& val) const
	{
		const std::type_info* ptr = boost::addressof(val);
		boost::hash<const std::type_info*> hasher;

		return hasher(ptr);
	}

	inline std::size_t operator()(const boost::reference_wrapper<std::type_info const>& val) const
	{
		const std::type_info& info_cref = val;
		const std::type_info* ptr = boost::addressof(info_cref);
		boost::hash<const std::type_info*> hasher;

		return hasher(ptr);
	}
};

template<>
struct hash< boost::reference_wrapper<std::type_info const> >
#if YGGR_CPP_VERSION < YGGR_CPP_VER_17
	: public std::unary_function< boost::reference_wrapper<std::type_info const>, std::size_t >
#endif // #if YGGR_CPP_VERSION < YGGR_CPP_VER_17
{
	typedef const std::type_info& argument_type;
	typedef std::size_t result_type;

	inline std::size_t operator()(const std::type_info& val) const
	{
		const std::type_info* ptr = boost::addressof(val);
		boost::hash<const std::type_info*> hasher;

		return hasher(ptr);
	}

	inline std::size_t operator()(const boost::reference_wrapper<std::type_info const>& val) const
	{
		const std::type_info& info_cref = val;
		const std::type_info* ptr = boost::addressof(info_cref);
		boost::hash<const std::type_info*> hasher;

		return hasher(ptr);
	}
};

} // namespace boost


namespace yggr
{
namespace utility
{

template<typename T> inline 
const std::type_info* addressof_typeinfo(void)
{
	return boost::addressof(typeid(T));
}

template<typename T> inline 
const std::type_info* addressof_typeinfo(const T&)
{
	return boost::addressof(typeid(T));
}

inline const std::type_info* addressof_typeinfo(const std::type_info& val)
{
	return boost::addressof(val);
}

inline const std::type_info* addressof_typeinfo(const std::type_info* val)
{
	return val;
}

inline const std::type_info* addressof_typeinfo(const boost::reference_wrapper<std::type_info const>& val)
{
	const std::type_info& info = val;
	return boost::addressof(info);
}

} // namespace utility

using utility::addressof_typeinfo;

} // namespace yggr

#endif // __YGGR_UTILITY_TYPEINFO_HPP__
