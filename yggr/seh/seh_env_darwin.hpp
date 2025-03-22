//seh_env_darwin.hpp

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

#ifndef __YGGR_SEH_SEH_ENV_DARWIN_HPP__
#define __YGGR_SEH_SEH_ENV_DARWIN_HPP__

#ifndef YGGR_SEH_INCLUDE
#	error "seh_env_darwin.hpp include error please include seh.hpp."
#endif // YGGR_SEH_ENV_INCLUDE

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <yggr/seh/seh_helper_darwin.hpp>
#include <yggr/seh/seh_sys_codes_darwin.hpp>

#include <yggr/container/unordered_map.hpp>
#include <yggr/exception/exception.hpp>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/thread/boost_thread_config.hpp>
#include <yggr/thread/mutex_def_helper.hpp>

#include <boost/thread/mutex.hpp>

#include <cassert>

#include <signal.h>
#include <setjmp.h>


namespace yggr
{
namespace seh
{
//SIGIOT,SIGQUIT,SIGTRAP,SIGXCPU,SIGXFSZ
class seh_env_darwin
{
public:

    typedef seh_sys_codes_darwin seh_sys_codes_type;

private:
    typedef seh_helper_darwin seh_helper_type;
    
    typedef sigjmp_buf jmp_array_type;
    typedef int jmp_buf_val_type;

#if defined(_JBLEN)
    YGGR_STATIC_CONSTANT(std::size_t, E_JMP_BUF_LEN = _JBLEN + 1);
#else
    YGGR_STATIC_CONSTANT(std::size_t, sizeof(jmp_array_type) / sizeof(jmp_buf_val_type));
#endif // _JBLEN

    typedef int* jmp_buf_type;
    typedef boost::shared_ptr<jmp_buf_val_type> jmp_buf_ptr_type;

    typedef thread::boost_thread_config_type thread_config_type;
    typedef thread_config_type::id_type trd_id_type;
    typedef thread_config_type::this_thread_type this_thread_type;

    typedef thread::mutex_def_helper<boost::mutex> mutex_def_helper_type;
    typedef mutex_def_helper_type::mutex_type mutex_type;
    typedef mutex_def_helper_type::read_lock_type read_lock_type;
    typedef mutex_def_helper_type::write_lock_type write_lock_type;

    typedef ::yggr::unordered_map<trd_id_type, jmp_buf_ptr_type> jmp_buf_map_type;
    typedef jmp_buf_map_type::value_type jmp_buf_map_ins_val_type;
    typedef jmp_buf_map_type::iterator jmp_buf_map_iter_type;
    typedef jmp_buf_map_type::const_iterator jmp_buf_map_citer_type;

    typedef ::yggr::unordered_map<trd_id_type, int> now_signal_map_type;
    typedef now_signal_map_type::iterator now_signal_map_iter_type;
    typedef now_signal_map_type::const_iterator now_signal_map_citer_type;

    typedef seh_env_darwin this_type;

public:
	template<typename Ret, typename Handler, typename Filter_Handler, typename Clean_Handler, typename FixHandler>
	static bool safe_invoke(	Ret& ret,
								const Handler& handler,
								const Filter_Handler& filter,
								const Clean_Handler& cleaner,
								const FixHandler& fixer)
	{
	    trd_id_type tid = this_thread_type::id();
	    int r = 0;

		{
			jmp_buf_type jbptr = 0;
			jmp_buf_map_iter_type iter;
			jmp_buf_ptr_type pjmp_buf;

			{
				read_lock_type lk(_jmp_buf_mutex);
				iter = _jmp_buf_map.find(tid);

				if(iter != _jmp_buf_map.end())
				{
					if(!(jbptr = iter->second.get()))
					{
						assert(false);
						return false;
					}
					else
					{
						r = sigsetjmp(jbptr, 1);
					}
				}
			}

			if(!jbptr)
			{
				pjmp_buf = jmp_buf_ptr_type(yggr_nothrow_new jmp_buf_val_type[E_JMP_BUF_LEN]);
				assert(pjmp_buf);
				if(!pjmp_buf)
				{
					return false;
				}

				{
					write_lock_type lk(_jmp_buf_mutex);
					if(!(jbptr = _jmp_buf_map.insert(jmp_buf_map_ins_val_type(tid, pjmp_buf)).first->second.get()))
					{
						assert(false);
						return false;
					}
					else
					{
						r = sigsetjmp(jbptr, 1);
					}
				}
			}
		}

        if(!r)
        {
            ret = handler();
            return true;
        }
        else
        {
            ret = cleaner();
        }

        {
            read_lock_type lk(_signal_mutex);
            now_signal_map_iter_type iter = _now_sig_map.find(tid);
            if(iter == _now_sig_map.end())
            {
                filter(seh_sys_codes_type::E_unknown);
            }
            else
            {
                filter(iter->second);
            }
        }

        return fixer();

	}

	template<typename Handler, typename Filter_Handler, typename Clean_Handler, typename FixHandler>
	static bool safe_invoke(	const Handler& handler,
								const Filter_Handler& filter,
								const Clean_Handler& cleaner,
								const FixHandler& fixer)
	{
	    trd_id_type tid = this_thread_type::id();
	    int r = 0;

		{
			jmp_buf_type jbptr = 0;
			jmp_buf_map_iter_type iter;
			jmp_buf_ptr_type pjmp_buf;

			{
				read_lock_type lk(_jmp_buf_mutex);
				iter = _jmp_buf_map.find(tid);

				if(iter != _jmp_buf_map.end())
				{
					if(!(jbptr = iter->second.get()))
					{
						assert(false);
						return false;
					}
					else
					{
						r = sigsetjmp(jbptr, 1);
					}
				}
			}

			if(!jbptr)
			{
				pjmp_buf = jmp_buf_ptr_type(yggr_nothrow_new jmp_buf_val_type[E_JMP_BUF_LEN]);
				assert(pjmp_buf);
				if(!pjmp_buf)
				{
					return false;
				}

				{
					write_lock_type lk(_jmp_buf_mutex);
					if(!(jbptr = _jmp_buf_map.insert(jmp_buf_map_ins_val_type(tid, pjmp_buf)).first->second.get()))
					{
						assert(false);
						return false;
					}
					else
					{
						r = sigsetjmp(jbptr, 1);
					}
				}
			}
		}

        if(!r)
        {
            handler();
            return true;
        }
        else
        {
            cleaner();
        }

        {
            read_lock_type lk(_signal_mutex);
            now_signal_map_iter_type iter = _now_sig_map.find(tid);
            if(iter == _now_sig_map.end())
            {
                filter(int(seh_sys_codes_type::E_unknown));
            }
            else
            {
                filter(iter->second);
            }
        }

        return fixer();
	}

public:
    static void handler_recv_signal(int code);

private:
    static mutex_type _jmp_buf_mutex;
    static mutex_type _signal_mutex;
    static mutex_type _call_stack_mutex;

    static jmp_buf_map_type _jmp_buf_map;
    static now_signal_map_type _now_sig_map;
};

} // namespace seh
} // namespace yggr

#endif // __YGGR_SEH_SEH_ENV_DARWIN_HPP__
