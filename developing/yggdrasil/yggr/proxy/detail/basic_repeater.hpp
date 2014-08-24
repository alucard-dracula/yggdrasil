//basic_repeater.hpp

#ifndef __YGGR_PROXY_DETAIL_BASIC_REPEATER_HPP__
#define __YGGR_PROXY_DETAIL_BASIC_REPEATER_HPP__


#include <boost/bind.hpp>

#include <yggr/base/ctrl_center.hpp>
#include <yggr/nonable/nonable.hpp>
#include <yggr/mplex/pointer_to_value_t.hpp>
#include <yggr/safe_container/safe_wrap.hpp>

namespace yggr
{
namespace proxy
{
namespace detail
{

template<typename Shared_Info_Ptr>
struct basic_repeater
	: private nonable::noncopyable,
		private nonable::nonmoveable
		
{
protected:
	typedef Shared_Info_Ptr shared_info_ptr_type;

	typedef typename mplex::pointer_to_value_t<shared_info_ptr_type>::type shared_info_type;

private:
	typedef safe_container::safe_wrap<shared_info_type> shared_info_wrap_type;
	typedef typename shared_info_wrap_type::error_type shared_info_wrap_error_type;

private:
	typedef basic_repeater this_type;

protected:

	basic_repeater(shared_info_ptr_type psinfo)
		: _shared_info_wrap(psinfo)
	{
	}

	~basic_repeater(void)
	{
	}

	template<typename Repeater_Container>
	void register_repeater_data(Repeater_Container& cont)
	{
		shared_info_ptr_type psinfo = _shared_info_wrap.get_shared_ptr();
		if(!psinfo)
		{
			return;
		}

		psinfo->register_calculator(cont);

	}

	template<typename Repeater_Container>
	void unregister_repeater_data(Repeater_Container& cont)
	{
		shared_info_ptr_type psinfo = _shared_info_wrap.get_shared_ptr();
		if(!psinfo)
		{
			return;
		}

		psinfo->unregister_calculator(cont);
	}

	void clear(void)
	{
		try
		{
			_shared_info_wrap.use_handler(boost::bind(&shared_info_type::clear, _1));
		}
		catch(const shared_info_wrap_error_type&)
		{
		}
	}

private:
	shared_info_wrap_type _shared_info_wrap;
};

} // namespace detail
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_DETAIL_BASIC_REPEATER_HPP__