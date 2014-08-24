//pipe_runner.hpp

#ifndef __YGGR_THREAD_BASE_RUNNER_PIPE_RUNNER_HPP__
#define __YGGR_THREAD_BASE_RUNNER_PIPE_RUNNER_HPP__

#include <assert.h>
namespace yggr
{
namespace thread
{
namespace base_runner
{

template<typename Target, typename Adapter_Mgr>
class pipe_runner
{
public:

	typedef Target target_type;
	typedef typename target_type::condition_data_type target_condition_data_type;
	typedef typename target_type::result_data_type target_result_data_type;

	typedef Adapter_Mgr adapter_mgr_type;
	typedef typename adapter_mgr_type::send_std_data_type adapter_mgr_condition_data_type;
	typedef typename adapter_mgr_type::recv_rst_data_type adapter_mgr_result_data_type;

//--------------------------------------------------------------
	class Error
	{
	public:
		enum
		{
			E_success = 0,
			E_compile_u32 = 0xffffffff
		};

#ifndef __YGGR_UTF_8__
		typedef std::string err_string;
#else
		typedef std::wstring err_string;
#endif //__YGGR_UTF_8__

	public:

#ifndef __YGGR_UTF_8__
		Error(yggr::u32 code)
			:_code(code), _class_name("[class pipe_runner]")
		{
		}

		Error(yggr::u32 code, const err_string& omsg)
			:_code(code), _other_msg(omsg),  _class_name("[class pipe_runner]")
		{
		}
#else //-----------------------------------------------------
	Error(u32 code)
			:_code(code), _class_name(L"[class pipe_runner]")
		{
		}

		Error(u32 code, const err_string& omsg)
			:_code(code), _other_msg(omsg),  _class_name(L"[class pipe_runner]")
		{
		}
#endif //__YGGR_UTF_8__

		Error(const Error& err)
			: _code(err._code), _other_msg(err._other_msg),  _class_name(err._class_name)
		{
		}

		~Error(void)
		{
		}

		operator bool(void) const
		{
			return _code != 0;
		}

		yggr::u32 code(void) const
		{
			return _code;
		}

		const err_string message(void) const
		{
			err_string msg = _class_name + " " + _other_msg;
			switch(_code)
			{
#ifndef __YGGR_UTF_8__
			case E_code_test: 
				msg += " ------ E_code_test";
				break;
			default: 
				msg += " ------  unknow error";
#else // -------------------------------------------------------------------------
			case E_code_test: 
				msg += L" ------ E_code_test";
				break;

			default: 
				msg += L" ------ unknow error";
#endif // __YGGR_UTF_8__
			}

			return msg;
		}

	private:
		yggr::u32 _code;
		err_string _other_msg;
		const err_string _class_name;
	};

	//class Error S----------------------------------------------------------

	/*enum
	{
		E_CAL_CONDITION = 1,
		E_CAL_RESULT
	};*/

	#include <support/runner_condition_and_result.ipp>

public:

	pipe_runner(void)
	{
		assert(
				(typeid(target_condition_data_type) == typeid(adapter_mgr_condition_data_type))
				&&
				((typeid(target_result_data_type) == typeid(adapter_mgr_result_data_type))
				);
	}

	template<typename Handler>
	void calculate(const boost::any& cdt, Handler handler)
	{
		if(cdt.type() == typeid(target_condition_data_type))
		{
			handler(true, );
		}
	}
};

} // namespace base_runner
} // namespace thread
} //namespace yggr
#endif //__YGGR_THREAD_BASE_RUNNER_PIPE_RUNNER_HPP__