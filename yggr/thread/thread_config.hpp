//thread_config.hpp

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

#ifndef __YGGR_THREAD_THREAD_CONFIG_HPP__
#define __YGGR_THREAD_THREAD_CONFIG_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/nonable/noncreateable.hpp>
#include <yggr/thread/this_thread.hpp>

#include <boost/thread/scoped_thread.hpp>

#ifdef _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4624)
#endif //_MSC_VER

namespace yggr
{
namespace thread
{

#if defined(BOOST_NO_INT64_T) 
#	define E_def_sleep_time_sec_decl() YGGR_STATIC_CONSTANT(s32, E_def_sleep_time_sec = 0)
#else
#	define E_def_sleep_time_sec_decl() YGGR_STATIC_CONSTANT(s64, E_def_sleep_time_sec = 0)
#endif // BOOST_NO_INT64_T

#define YGGR_PP_THREAD_CONFIG_DECL(__name__, Thread, Thread_Group, Time) \
	class __name__ { \
	private: \
		typedef __name__ this_type; \
		\
	public: \
		typedef Thread thread_type; \
		typedef thread_type::id id_type; \
		typedef Thread_Group thread_group_type; \
		typedef Time time_type; \
		typedef this_thread<thread_type> this_thread_type; \
		\
	public: \
		E_def_sleep_time_sec_decl(); \
		YGGR_STATIC_CONSTANT(s32, E_def_sleep_time_nsec = 33000000); \
		\
	public: \
		inline static const time_type& default_sleep_time_step(void) { \
			static time_type tm(E_def_sleep_time_sec, E_def_sleep_time_nsec); \
			return tm; } \
		\
		inline static u32 hardware_concurrency(void) { \
			return boost::thread::hardware_concurrency(); } \
		\
		inline static u32 recommended_number_of_threads(void) { \
			return (this_type::hardware_concurrency() << 1) + 2; } };

template<typename Thread, typename Thread_Group, typename Time>
class thread_config
{
private:
	typedef thread_config this_type;

public:
	typedef Thread thread_type;
	typedef typename thread_type::id id_type;
	typedef Thread_Group thread_group_type;
	typedef Time time_type;

	typedef this_thread<thread_type> this_thread_type;

public:

#if defined(BOOST_NO_INT64_T)
	YGGR_STATIC_CONSTANT(s32, E_def_sleep_time_sec = 0);
#else
	YGGR_STATIC_CONSTANT(s64, E_def_sleep_time_sec = 0);
#endif // BOOST_NO_INT64_T
	YGGR_STATIC_CONSTANT(s32, E_def_sleep_time_nsec = 33000000);

public:
	inline static const time_type& default_sleep_time_step(void)
	{
		static time_type tm(E_def_sleep_time_sec, E_def_sleep_time_nsec);
		return tm;
	}

	inline static u32 hardware_concurrency(void)
	{
		return boost::thread::hardware_concurrency();
	}

	inline static u32 recommended_number_of_threads(void)
	{
		return (this_type::hardware_concurrency() << 1) + 2;
	}
};

} // namespace thread
} //namspace yggr

#ifdef _MSC_VER
#	pragma warning ( pop )
#endif //_MSC_VER

#endif //__YGGR_THREAD_THREAD_CONFIG_HPP__