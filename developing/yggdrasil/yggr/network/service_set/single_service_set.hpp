// signle_service_set.hpp

#ifndef __YGGR_NETWORK_SERVICE_SET_SIGNLE_SERVICE_SET_HPP__
#define __YGGR_NETWORK_SERVICE_SET_SIGNLE_SERVICE_SET_HPP__

namespace yggr
{
namespace network
{
namespace service_set
{

template<typename Service>
class signle_service_set
{
public:
	typedef Service service_type;

private:
	typedef signle_service_set this_type;

public:
	signle_service_set(void)
	{
	}

	~signle_service_set(void)
	{
	}

};

} // namespace service_set
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_SERVICE_SET_SIGNLE_SERVICE_SET_HPP__