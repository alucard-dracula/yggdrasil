//linux_seh_env.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_SEH_LINUX_SEH_ENV_HPP__
#define __YGGR_SEH_LINUX_SEH_ENV_HPP__

#ifndef YGGR_SEH_INCLUDE
#	error "linuex_seh_env.hpp include error please include seh.hpp."
#endif // YGGR_SEH_ENV_INCLUDE

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <yggr/thread/boost_thread_config.hpp>
#include <boost/thread/mutex.hpp>
#include <cassert>
#include <signal.h>
#include <setjmp.h>
#include <yggr/helper/mutex_def_helper.hpp>
#include <yggr/seh/linux_seh_helper.hpp>
#include <yggr/seh/linux_seh_sys_codes.hpp>
#include <yggr/base/exception.hpp>


namespace yggr
{
namespace seh
{
//SIGIOT,SIGQUIT,SIGTRAP,SIGXCPU,SIGXFSZ
class linux_seh_env
{
public:

    typedef linux_seh_sys_codes seh_sys_codes_type;

private:
    typedef linux_seh_helper seh_helper_type;

    typedef struct __jmp_buf_tag jmp_buf_type;
    typedef boost::shared_ptr<jmp_buf_type> jmp_buf_ptr_type;

    typedef thread::boost_thread_config_type thread_config_type;
    typedef thread_config_type::id_type trd_id_type;
    typedef thread_config_type::this_thread_type this_thread_type;

    typedef helper::mutex_def_helper<boost::mutex> mutex_def_helper_type;
    typedef mutex_def_helper_type::mutex_type mutex_type;
    typedef mutex_def_helper_type::read_lock_type read_lock_type;
    typedef mutex_def_helper_type::write_lock_type write_lock_type;

    typedef boost::unordered_map<trd_id_type, jmp_buf_ptr_type> jmp_buf_map_type;
    typedef jmp_buf_map_type::value_type jmp_buf_map_ins_val_type;
    typedef jmp_buf_map_type::iterator jmp_buf_map_iter_type;
    typedef jmp_buf_map_type::const_iterator jmp_buf_map_citer_type;

    typedef boost::unordered_map<trd_id_type, int> now_signal_map_type;
    //typedef now_signal_map_type::value_type now_signal_map_ins_val_type;
    typedef now_signal_map_type::iterator now_signal_map_iter_type;
    typedef now_signal_map_type::const_iterator now_signal_map_citer_type;

    typedef linux_seh_env this_type;

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
            jmp_buf_type* jbptr = 0;
            jmp_buf_map_iter_type iter;

            write_lock_type lk(_jmp_buf_mutex);
            iter = _jmp_buf_map.find(tid);

            if(iter != _jmp_buf_map.end())
            {
                jbptr = iter->second.get();
            }
            else
            {
                jbptr = _jmp_buf_map.insert(jmp_buf_map_ins_val_type(tid, jmp_buf_ptr_type(new jmp_buf_type))).first->second.get();
            }
            assert(jbptr);
            r = sigsetjmp(jbptr, 1);
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
                filter(seh_sys_codes_type::E_unknow);
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
            jmp_buf_type* jbptr = 0;
            jmp_buf_map_iter_type iter;

            write_lock_type lk(_jmp_buf_mutex);
            iter = _jmp_buf_map.find(tid);

            if(iter != _jmp_buf_map.end())
            {
                jbptr = iter->second.get();
            }
            else
            {
                jbptr = _jmp_buf_map.insert(jmp_buf_map_ins_val_type(tid, jmp_buf_ptr_type(new jmp_buf_type))).first->second.get();
            }
            assert(jbptr);
            r = sigsetjmp(jbptr, 1);
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
                filter(int(seh_sys_codes_type::E_unknow));
            }
            else
            {
                filter(iter->second);
            }
        }


        return fixer();
	}

public:
    static void handler_recv_signal(int code)
    {
        trd_id_type tid = this_thread_type::id();

        seh_helper_type::dump_call_stack_type call_stack;
        {
            write_lock_type lk(_call_stack_mutex);
            seh_helper_type::dump_call_stack(call_stack);
        }

        std::stringstream ss;

        ss << "os_code = " << code << "\n"
            << seh_helper_type::format_dump_call_stack_msg(call_stack);

        exception::exception::throw_error(code, ss.str());

        {
            write_lock_type lk(_signal_mutex);
            _now_sig_map[tid] = code;
        }


        {
            write_lock_type lk(_jmp_buf_mutex);
            siglongjmp(_jmp_buf_map[tid].get(), 1);
        }
    }

private:
    static mutex_type _jmp_buf_mutex;
    static mutex_type _signal_mutex;
    static mutex_type _call_stack_mutex;

    static jmp_buf_map_type _jmp_buf_map;
    static now_signal_map_type _now_sig_map;
};

linux_seh_env::mutex_type linux_seh_env::_jmp_buf_mutex;
linux_seh_env::mutex_type linux_seh_env::_signal_mutex;
linux_seh_env::mutex_type linux_seh_env::_call_stack_mutex;

linux_seh_env::jmp_buf_map_type linux_seh_env::_jmp_buf_map;
linux_seh_env::now_signal_map_type linux_seh_env::_now_sig_map;

} // namespace seh
} // namespace yggr

#endif // __YGGR_SEH_LINUX_SEH_ENV_HPP__
