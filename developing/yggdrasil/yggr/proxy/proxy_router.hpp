//proxy_router.hpp

#ifndef __YGGR_PROXY_PROXY_ROUTER_HPP__
#define __YGGR_PROXY_PROXY_ROUTER_HPP__

#include <boost/shared_ptr.hpp>
#include <yggr/safe_container/safe_wrap.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

namespace yggr
{
namespace proxy
{

template<typename Dispatcher>
class proxy_router
	: public Dispatcher
{

public:
	typedef Dispatcher base_type;
	typedef typename base_type::task_type task_type;
	typedef typename base_type::owner_info_type owner_info_type;
	typedef typename base_type::id_type dispatch_id_type;

private:
	typedef proxy_router this_type;

public:
	proxy_router(void)
	{
	}

	~proxy_router(void)
	{
	}

	template<typename Runner, typename Handler>
	void route(const owner_info_type& owner,
					const task_type& proxy_data,
					Runner* prunner,
					const Handler& handler,
					u32 cal_type)
	{
		//yggr::u64 n1 = proxy_data.data_id();
		//yggr::u64 n2 = proxy_data.data_ver();
		base_type::send(owner, proxy_data, prunner, handler, cal_type);
	}

};

} // namespace proxy
} // namespace yggr

#endif //__YGGR_PROXY_PROXY_ROUTER_HPP__