//time_basic_foo.hpp

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

#ifndef __YGGR_TIME_TIME_BASIC_FOO_HPP__
#define __YGGR_TIME_TIME_BASIC_FOO_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/mplex/limits.hpp>

#include <yggr/time/detail/time_diff.hpp>
#include <yggr/time/detail/posix_time_converter.hpp>

#include <boost/ref.hpp>

namespace yggr
{
namespace time
{

#ifdef _MSC_VER
#	pragma warning( push )
#	pragma warning( disable : 4996 )
#endif // _MSC_VER

using detail::gen_utc_calendar_time_diff;
using detail::gen_utc_local_time_diff;
using detail::gen_local_calendar_time_diff;

inline std::time_t utc_calendar_time_diff(void)
{
#ifdef YGGR_TIME_CONSTANT_RECALCULATE_EVERY_TIME
	return ::yggr::time::detail::gen_utc_calendar_time_diff();
#else
	static const std::time_t diff = ::yggr::time::detail::gen_utc_calendar_time_diff();
	return diff;
#endif // YGGR_TIME_CONSTANT_RECALCULATE_EVERY_TIME
}

inline std::time_t utc_local_time_diff(void)
{
#ifdef YGGR_TIME_CONSTANT_RECALCULATE_EVERY_TIME
	return ::yggr::time::detail::gen_utc_local_time_diff();
#else
	static const std::time_t diff = ::yggr::time::detail::gen_utc_local_time_diff();
	return diff;
#endif // YGGR_TIME_CONSTANT_RECALCULATE_EVERY_TIME
}

inline std::time_t local_calendar_time_diff(void)
{
#ifdef YGGR_TIME_CONSTANT_RECALCULATE_EVERY_TIME
	return ::yggr::time::detail::gen_local_calendar_time_diff();
#else
	static const std::time_t diff = ::yggr::time::detail::gen_local_calendar_time_diff();
	return diff;
#endif // YGGR_TIME_CONSTANT_RECALCULATE_EVERY_TIME
}

// !!!time - time!!!
// utc <-> calendar
inline std::time_t utc_time_to_calendar_time(std::time_t utc_tm)
{
	typedef mplex::numeric_limits<std::time_t> limits_type;
	
	assert(!((limits_type::max_type::value - utc_tm) < utc_calendar_time_diff()));

	return 
		((limits_type::max_type::value - utc_tm) < utc_calendar_time_diff())?
			static_cast<std::time_t>(-1)
			: utc_tm + utc_calendar_time_diff();
}

// utc <-> local
inline std::time_t utc_time_to_local_time(std::time_t utc_tm)
{
	typedef mplex::numeric_limits<std::time_t> limits_type;
	
	assert(!((limits_type::max_type::value - utc_tm) < utc_local_time_diff()));

	return 
		((limits_type::max_type::value - utc_tm) < utc_local_time_diff())?
			static_cast<std::time_t>(-1)
			: utc_tm + utc_local_time_diff();
}

// calendar<->utc
inline std::time_t calendar_time_to_utc_time(std::time_t cnd_tm)
{
	assert(!(cnd_tm < utc_calendar_time_diff()));

	return 
		(cnd_tm < utc_calendar_time_diff())?
			static_cast<std::time_t>(-1)
			: cnd_tm - utc_calendar_time_diff();
}

// calendar<->local
inline std::time_t calendar_time_to_local_time(std::time_t cnd_tm)
{
	assert(!(cnd_tm < local_calendar_time_diff()));

	return 
		(cnd_tm < local_calendar_time_diff())?
			static_cast<std::time_t>(-1)
			: cnd_tm - local_calendar_time_diff();
}


// local<->utc
inline std::time_t local_time_to_utc_time(std::time_t lcl_tm)
{
	assert(!(lcl_tm < utc_local_time_diff()));
	return 
		(lcl_tm < utc_local_time_diff())?
			static_cast<std::time_t>(-1)
			: lcl_tm - utc_local_time_diff();
}

// local<->calendar
inline std::time_t local_time_to_calendar_time(std::time_t lcl_tm)
{
	typedef mplex::numeric_limits<std::time_t> limits_type;
	assert(!((limits_type::max_type::value - lcl_tm) < local_calendar_time_diff()));

	return 
		((limits_type::max_type::value - lcl_tm) < local_calendar_time_diff())?
			static_cast<std::time_t>(-1)
			: lcl_tm + local_calendar_time_diff();
}

// !!!time - tm!!!
// utc <-> utc
inline std::tm* utc_time_to_utc_tm(std::time_t utc_tm, std::tm* result)
{
	assert(result);

	return 
		result?
			boost::addressof(detail::posix_time_converter::time_t_to_tm(*result, utc_tm))
			: static_cast<std::tm*>(0);
}

inline std::tm* utc_time_to_utc_tm(std::time_t utc_tm, std::tm& result)
{
	return utc_time_to_utc_tm(utc_tm, boost::addressof(result));
}

// utc <-> calendar
inline std::tm* utc_time_to_calendar_tm(std::time_t utc_tm, std::tm* result)
{
	assert(result);

	return 
		result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result,
					utc_time_to_calendar_time(utc_tm) ))
			: static_cast<std::tm*>(0);
}

inline std::tm* utc_time_to_calendar_tm(std::time_t utc_tm, std::tm& result)
{
	return utc_time_to_calendar_tm(utc_tm, boost::addressof(result));
}

// utc <-> local
inline std::tm* utc_time_to_local_tm(std::time_t utc_tm, std::tm* result)
{
	assert(result);

	return 
		result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result, 
					utc_time_to_local_time(utc_tm) ))
			: static_cast<std::tm*>(0);
}

inline std::tm* utc_time_to_local_tm(std::time_t utc_tm, std::tm& result)
{
	return utc_time_to_local_tm(utc_tm, &result);
}

// calendar <-> utc
inline std::tm* calendar_time_to_utc_tm(std::time_t cnd_tm, std::tm* result)
{
	assert(result);

	return 
		result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result,
					calendar_time_to_utc_time(cnd_tm) ))
			: static_cast<std::tm*>(0);
}

inline std::tm* calendar_time_to_utc_tm(std::time_t cnd_tm, std::tm& result)
{
	return calendar_time_to_utc_tm(cnd_tm, boost::addressof(result));
}

// calendar <-> calendar
inline std::tm* calendar_time_to_calendar_tm(std::time_t cnd_tm, std::tm* result)
{
	assert(result);

	return 
		result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result, cnd_tm ))
			: static_cast<std::tm*>(0);
}

inline std::tm* calendar_time_to_calendar_tm(std::time_t cnd_tm, std::tm& result)
{
	return calendar_time_to_calendar_tm(cnd_tm, boost::addressof(result));
}

// calendar <-> local
inline std::tm* calendar_time_to_local_tm(std::time_t cnd_tm, std::tm* result)
{
	assert(result);

	return 
		result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result,
					calendar_time_to_local_time(cnd_tm) ))
			: static_cast<std::tm*>(0);
}

inline std::tm* calendar_time_to_local_tm(std::time_t cnd_tm, std::tm& result)
{
	return calendar_time_to_local_tm(cnd_tm, boost::addressof(result));
}

// local <-> utc
inline std::tm* local_time_to_utc_tm(std::time_t lcl_tm, std::tm* result)
{
	assert(result);

	return 
		result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result, 
					local_time_to_utc_time(lcl_tm) ))
			: static_cast<std::tm*>(0);
}

inline std::tm* local_time_to_utc_tm(std::time_t lcl_tm, std::tm& result)
{
	return local_time_to_utc_tm(lcl_tm, &result);
}

// local <-> calendar
inline std::tm* local_time_to_calendar_tm(std::time_t lcl_tm, std::tm* result)
{
	assert(result);

	return 
		result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result, 
					local_time_to_calendar_time(lcl_tm) ))
			: static_cast<std::tm*>(0);
}

inline std::tm* local_time_to_calendar_tm(std::time_t lcl_tm, std::tm& result)
{
	return local_time_to_calendar_tm(lcl_tm, &result);
}

// local <-> local
inline std::tm* local_time_to_local_tm(std::time_t lcl_tm, std::tm* result)
{
	assert(result);

	return 
		result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result, lcl_tm ))
			: static_cast<std::tm*>(0);
}

inline std::tm* local_time_to_local_tm(std::time_t lcl_tm, std::tm& result)
{
	return local_time_to_local_tm(lcl_tm, &result);
}

// !!!tm - time!!!
// utc <-> utc
inline std::time_t utc_tm_to_utc_time(const std::tm* putc_tm)
{
	return 
		putc_tm?
			detail::posix_time_converter::tm_to_time_t(*putc_tm)
			: static_cast<std::time_t>(-1);
}

inline std::time_t utc_tm_to_utc_time(const std::tm& utc_tm)
{
	return utc_tm_to_utc_time(boost::addressof(utc_tm));
}

// utc <-> calendar
inline std::time_t utc_tm_to_calendar_time(const std::tm* putc_tm)
{
	return 
		putc_tm?
			utc_time_to_calendar_time(
				detail::posix_time_converter::tm_to_time_t(*putc_tm))
			: static_cast<std::time_t>(-1);
}

inline std::time_t utc_tm_to_calendar_time(const std::tm& utc_tm)
{
	return utc_tm_to_calendar_time(boost::addressof(utc_tm));
}

// utc <-> local
inline std::time_t utc_tm_to_local_time(const std::tm* putc_tm)
{
	return 
		putc_tm?
			utc_time_to_local_time(
				detail::posix_time_converter::tm_to_time_t(*putc_tm))
			: static_cast<std::time_t>(-1);
}

inline std::time_t utc_tm_to_local_time(const std::tm& utc_tm)
{
	return utc_tm_to_local_time(boost::addressof(utc_tm));
}

// calendar <-> utc
inline std::time_t calendar_tm_to_utc_time(const std::tm* pcnd_tm)
{
	return 
		pcnd_tm?
			calendar_time_to_utc_time(
				detail::posix_time_converter::tm_to_time_t(*pcnd_tm))
			: static_cast<std::time_t>(-1);
}

inline std::time_t calendar_tm_to_utc_time(const std::tm& cnd_tm)
{
	return calendar_tm_to_utc_time(boost::addressof(cnd_tm));
}

// calendar <-> calendar
inline std::time_t calendar_tm_to_calendar_time(const std::tm* pcnd_tm)
{
	return 
		pcnd_tm?
			detail::posix_time_converter::tm_to_time_t(*pcnd_tm)
			: static_cast<std::time_t>(-1);
}

inline std::time_t calendar_tm_to_calendar_time(const std::tm& cnd_tm)
{
	return calendar_tm_to_calendar_time(boost::addressof(cnd_tm));
}

// calendar <-> local
inline std::time_t calendar_tm_to_local_time(const std::tm* pcnd_tm)
{
	return 
		pcnd_tm?
			calendar_time_to_local_time(
				detail::posix_time_converter::tm_to_time_t(*pcnd_tm))
			: static_cast<std::time_t>(-1);
}

inline std::time_t calendar_tm_to_local_time(const std::tm& cnd_tm)
{
	return calendar_tm_to_local_time(boost::addressof(cnd_tm));
}

// local <-> utc
inline std::time_t local_tm_to_utc_time(const std::tm* plcl_tm)
{
	return 
		plcl_tm?
			local_time_to_utc_time(
				detail::posix_time_converter::tm_to_time_t(*plcl_tm))
			: static_cast<std::time_t>(-1);
}

inline std::time_t local_tm_to_utc_time(const std::tm& lcl_tm)
{
	return local_tm_to_utc_time(boost::addressof(lcl_tm));
}

// local <-> calendar
inline std::time_t local_tm_to_calendar_time(const std::tm* plcl_tm)
{
	return 
		plcl_tm?
			local_time_to_calendar_time(
				detail::posix_time_converter::tm_to_time_t(*plcl_tm))
			: static_cast<std::time_t>(-1);
}

inline std::time_t local_tm_to_calendar_time(const std::tm& lcl_tm)
{
	return local_tm_to_calendar_time(boost::addressof(lcl_tm));
}

// local <-> local
inline std::time_t local_tm_to_local_time(const std::tm* plcl_tm)
{
	return 
		plcl_tm?
			detail::posix_time_converter::tm_to_time_t(*plcl_tm)
			: static_cast<std::time_t>(-1);
}

inline std::time_t local_tm_to_local_time(const std::tm& lcl_tm)
{
	return local_tm_to_local_time(boost::addressof(lcl_tm));
}

// !!!tm - tm!!!
// utc <-> calendar
inline std::tm* utc_tm_to_calendar_tm(const std::tm* putc_tm, std::tm* result)
{
	assert(result);
	return 
		putc_tm && result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result,
					utc_time_to_calendar_time(
						detail::posix_time_converter::tm_to_time_t(*putc_tm))))
			: static_cast<std::tm*>(0);
}

inline std::tm* utc_tm_to_calendar_tm(const std::tm& utc_tm, std::tm& result)
{
	return utc_tm_to_calendar_tm(boost::addressof(utc_tm), boost::addressof(result));
}

// utc <-> local
inline std::tm* utc_tm_to_local_tm(const std::tm* putc_tm, std::tm* result)
{
	assert(result);
	return 
		putc_tm && result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result,
					utc_time_to_local_time(
						detail::posix_time_converter::tm_to_time_t(*putc_tm))))
			: static_cast<std::tm*>(0);
}

inline std::tm* utc_tm_to_local_tm(const std::tm& utc_tm, std::tm& result)
{
	return utc_tm_to_local_tm(boost::addressof(utc_tm), boost::addressof(result));
}

// calendar <-> utc
inline std::tm* calendar_tm_to_utc_tm(const std::tm* pcnd_tm, std::tm* result)
{
	assert(result);
	return 
		pcnd_tm && result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result,
					calendar_time_to_utc_time(
						detail::posix_time_converter::tm_to_time_t(*pcnd_tm))))
			: static_cast<std::tm*>(0);
}

inline std::tm* calendar_tm_to_utc_tm(const std::tm& cnd_tm, std::tm& result)
{
	return calendar_tm_to_utc_tm(boost::addressof(cnd_tm), boost::addressof(result));
}

// calendar <-> local
inline std::tm* calendar_tm_to_local_tm(const std::tm* pcnd_tm, std::tm* result)
{
	assert(result);
	return 
		pcnd_tm && result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result,
					calendar_time_to_local_time(
						detail::posix_time_converter::tm_to_time_t(*pcnd_tm))))
			: static_cast<std::tm*>(0);
}

inline std::tm* calendar_tm_to_local_tm(const std::tm& cnd_tm, std::tm& result)
{
	return calendar_tm_to_local_tm(boost::addressof(cnd_tm), boost::addressof(result));
}

// local <-> utc
inline std::tm* local_tm_to_utc_tm(const std::tm* plcl_tm, std::tm* result)
{
	assert(result);
	return 
		plcl_tm && result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result,
					calendar_time_to_utc_time(
						detail::posix_time_converter::tm_to_time_t(*plcl_tm))))
			: static_cast<std::tm*>(0);
}

inline std::tm* local_tm_to_utc_tm(const std::tm& lcl_tm, std::tm& result)
{
	return local_tm_to_utc_tm(boost::addressof(lcl_tm), boost::addressof(result));
}

// local <-> calendar

inline std::tm* local_tm_to_calendar_tm(const std::tm* plcl_tm, std::tm* result)
{
	assert(result);
	return 
		plcl_tm && result?
			boost::addressof(
				detail::posix_time_converter::time_t_to_tm(
					*result,
					local_time_to_calendar_time(
						detail::posix_time_converter::tm_to_time_t(*plcl_tm))))
			: static_cast<std::tm*>(0);
}

inline std::tm* local_tm_to_calendar_tm(const std::tm& lcl_tm, std::tm& result)
{
	return local_tm_to_calendar_tm(boost::addressof(lcl_tm), boost::addressof(result));
}

#ifdef _MSC_VER
#	pragma warning( pop )
#endif // _MSC_VER

} // namespace time
} // namespace yggr

#endif // __YGGR_TIME_TIME_BASIC_FOO_HPP__
