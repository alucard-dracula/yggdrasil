//seh.hpp

#ifndef __YGGR_SEH_SEH_HPP__
#define __YGGR_SEH_SEH_HPP__

#ifdef YGGR_USE_SEH

#if defined(__MINGW32__) || defined(__CYGWIN__)
#   error "seh must use to system compile"
#endif // defined(__MINGW32__) || defined(__CYGWIN__)

#include <yggr/base/ptr_single.hpp>

#define YGGR_SEH_INCLUDE

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
#   include <yggr/seh/win_seh.hpp>
#elif defined(YGGR_AT_LINUX)
#   include <yggr/seh/linux_seh.hpp>
#else
#   error "please add now plantfrom seh code !!!"
#endif //defined(WIN32) || defined(WIN64) || defined(WINDOWS)

namespace yggr
{
namespace seh
{

template<typename Base>
class seh : public Base
{
private:
	typedef Base base_type;

public:
    typedef typename base_type::seh_sys_codes_type seh_sys_codes_type;
	typedef typename base_type::code_type code_type;

private:
	typedef seh this_type;
	typedef ptr_single<this_type> this_single_type;

public:
	typedef typename this_single_type::obj_ptr_type obj_ptr_type;
	typedef const obj_ptr_type obj_cptr_type;

public:
	seh(void)
	{
	}

	~seh(void)
	{
	}

	static void s_init_ins()
	{
		this_single_type::init_ins();
	}

	static void s_init_ins(const obj_ptr_type& pobj)
	{
		this_single_type::init_ins(pobj);
	}

	static obj_ptr_type get_ins()
	{
		return this_single_type::get_ins();
	}

	static void s_uninstall()
	{
		this_single_type::uninstall();
	}

	static bool s_register_code(code_type code)
	{
		obj_ptr_type ptr = this_single_type::get_ins();
		if(!ptr)
		{
			return false;
		}

		return ptr->register_code(code);
	}

	static bool s_unregister_code(code_type code)
	{
		obj_ptr_type ptr = this_single_type::get_ins();
		if(!ptr)
		{
			return false;
		}

		return ptr->unregister_code(code);
	}

	template<typename Ret, typename Handler>
	static bool s_safe_invoke(Ret& ret, const Handler& handler)
	{
		obj_cptr_type ptr = this_single_type::get_ins();
		if(!ptr)
		{
			ret = handler();
			return true;
		}

		return ptr->safe_invoke(ret, handler);
	}

	template<typename Handler>
	static bool s_safe_invoke(const Handler& handler)
	{
		obj_cptr_type ptr = this_single_type::get_ins();
		if(!ptr)
		{
			handler();
			return true;
		}

		return ptr->safe_invoke(handler);
	}
};

#if defined(WIN32) || defined(WIN64) || defined(WINDOWS)
typedef seh<win_seh> seh_type;
#elif defined(YGGR_AT_LINUX)
typedef seh<linux_seh> seh_type;
#else
#   error "please add now plantfrom seh type !!!"
#endif //defined(WIN32) || defined(WIN64) || defined(WINDOWS)



} // namespace seh
} // namesapce yggr

#endif // YGGR_USE_SEH

#endif // __YGGR_SEH_SEH_HPP__

