//seh_darwin.hpp

/****************************************************************************
Copyright (c) 2025-2025 yggdrasil

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

#ifndef __YGGR_SEH_SEH_DARWIN_HPP__
#define __YGGR_SEH_SEH_DARWIN_HPP__

#ifndef YGGR_SEH_INCLUDE
#	error "seh_darwin.hpp include error please include seh.hpp."
#endif // YGGR_SEH_ENV_INCLUDE

#include <yggr/base/yggrdef.h>
#include <yggr/exception/exception.hpp>

#include <yggr/seh/seh_env_darwin.hpp>
#include <yggr/seh/seh_helper_darwin.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

#include <sstream>

namespace yggr
{
namespace seh
{

class seh_darwin
{
public:
	typedef u32 code_type;

private:
    typedef seh_env_darwin seh_env_type;
    typedef seh_helper_darwin seh_helper_type;

public:
    typedef seh_env_type::seh_sys_codes_type seh_sys_codes_type;

private:
	typedef seh_darwin this_type;

public:
	seh_darwin(void);
	~seh_darwin(void);

	template<typename Ret, typename Handler>
	bool safe_invoke(Ret& ret, const Handler& handler) const
	{
		typedef Ret ret_type;
		return seh_env_type::safe_invoke(ret, handler,
											boost::bind(&this_type::filter, this, _1),
											boost::bind(&this_type::ret_def_cleaner<ret_type>, this),
											boost::bind(&this_type::def_fixer, this));
	}

	template<typename Handler>
	bool safe_invoke(const Handler& handler) const
	{
		return seh_env_type::safe_invoke(handler,
											boost::bind(&this_type::filter, this, _1),
											boost::bind(&this_type::nonret_def_cleaner, this),
											boost::bind(&this_type::def_fixer, this));
	}

	bool register_code(code_type code);
	bool unregister_code(code_type code);

private:

	template<typename Ret> inline
	Ret ret_def_cleaner(void) const
	{
		typedef Ret ret_type;
		return ret_type();
	}

	inline void nonret_def_cleaner(void) const
	{
		return;
	}

	inline bool def_fixer(void) const
	{
		//return true;
		return false;
	}

	inline u32 filter(code_type code) const
	{
	    return 0;
	}
};

} // namespace seh
} // namespace yggr

#endif // __YGGR_SEH_SEH_DARWIN_HPP__
