//bit_options.hpp

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

#ifndef __YGGR_BIT_OPTIONS_HPP__
#define __YGGR_BIT_OPTIONS_HPP__

#include <yggr/base/yggrdef.h>

namespace yggr
{

// check
template<typename T> inline
bool has_bit_options(T opts, T chk)
{
	return (opts & chk) == chk;
}

template<typename T> inline
bool has_bit_options_in(T opts, T chk)
{
	return !!(opts & chk);
}

template<typename T> inline
T check_bit_options(T opts, T chk)
{
	return opts & chk;
}

// erase
template<typename T> inline
T erase_bit_options(T opts, T rm)
{
	return opts & (~rm);
}

template<typename T> inline
T erase_bit_options_if_exists(T opts, T chk, T rm)
{
	return has_bit_options(opts, chk)?
			erase_bit_options(opts, rm) : opts;
}

template<typename T> inline
T erase_bit_options_if_not_exists(T opts, T chk, T rm)
{
	return !has_bit_options(opts, chk)?
			erase_bit_options(opts, rm) : opts;
}

template<typename T> inline
T erase_bit_options_if_in(T opts, T chk)
{
	return (check_bit_options(opts, chk) & chk)?
				erase_bit_options(opts, chk) : opts;
}

template<typename T> inline
T erase_bit_options_if_in(T opts, T chk, T rm)
{
	return (check_bit_options(opts, chk) & chk)?
				erase_bit_options(opts, rm) : opts;
}

template<typename T> inline
T erase_bit_options_if_not_in(T opts, T chk, T rm)
{
	return !(check_bit_options(opts, chk) & chk)?
				erase_bit_options(opts, rm) : opts;
}

// append
template<typename T> inline
T append_bit_options(T opts, T add)
{
	return opts | add;
}

template<typename T> inline
T append_bit_options_if_exists(T opts, T chk, T add)
{
	return has_bit_options(opts, chk)?
			append_bit_options(opts, add) : opts;
}

template<typename T> inline
T append_bit_options_if_not_exists(T opts, T chk, T add)
{
	return !has_bit_options(opts, chk)?
			append_bit_options(opts, add) : opts;
}

template<typename T> inline
T append_bit_options_if_in(T opts, T chk)
{
	return (check_bit_options(opts, chk) & chk)?
				append_bit_options(opts, chk) : opts;
}

template<typename T> inline
T append_bit_options_if_not_in(T opts, T chk)
{
	return !(check_bit_options(opts, chk) & chk)?
				append_bit_options(opts, chk) : opts;
}

template<typename T> inline
T append_bit_options_if_in(T opts, T chk, T add)
{
	return (check_bit_options(opts, chk) & chk)?
				append_bit_options(opts, add) : opts;
}

template<typename T> inline
T append_bit_options_if_not_in(T opts, T chk, T add)
{
	return (check_bit_options(opts, chk) & chk)?
				append_bit_options(opts, add) : opts;
}

// modify
template<typename T> inline
T replace_bit_options(T opts, T rm, T add)
{
	return append_bit_options(erase_bit_options(opts, rm), add);
}

template<typename T> inline
T replace_bit_options_if_exists(T opts, T chk, T add)
{
	return has_bit_options(opts, chk)?
			replace_bit_options(opts, chk, add) : opts;
}

template<typename T> inline
T replace_bit_options_if_exists(T opts, T chk, T rm, T add)
{
	return has_bit_options(opts, chk)?
			replace_bit_options(opts, rm, add) : opts;
}

template<typename T> inline
T replace_bit_options_if_not_exists(T opts, T chk, T rm, T add)
{
	return !has_bit_options(opts, chk)?
			replace_bit_options(opts, rm, add) : opts;
}

template<typename T> inline
T replace_bit_options_if_in(T opts, T chk, T add)
{
	return (check_bit_options(opts, chk) & chk)?
				replace_bit_options(opts, chk, add) : opts;
}

template<typename T> inline
T replace_bit_options_if_not_in(T opts, T chk, T add)
{
	return (check_bit_options(opts, chk) & chk)?
				replace_bit_options(opts, chk, add) : opts;
}

template<typename T> inline
T replace_bit_options_if_in(T opts, T chk, T rm, T add)
{
	return (check_bit_options(opts, chk) & chk)?
				replace_bit_options(opts, rm, add) : opts;
}

template<typename T> inline
T replace_bit_options_if_not_in(T opts, T chk, T rm, T add)
{
	return !(check_bit_options(opts, chk) & chk)?
				replace_bit_options(opts, rm, add) : opts;
}


} // naemspace yggr

#endif // __YGGR_BIT_OPTIONS_HPP__
