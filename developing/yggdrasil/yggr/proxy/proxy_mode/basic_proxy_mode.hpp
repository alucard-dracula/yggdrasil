//basic_proxy_mode.hpp

#ifndef __YGGR_PROXY_PROXY_MODE_BASIC_PROXY_MODE_HPP__
#define __YGGR_PROXY_PROXY_MODE_BASIC_PROXY_MODE_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/container/vector.hpp>

#include <yggr/mplex/get_container_value_t.hpp>


namespace yggr
{
namespace proxy
{
namespace proxy_mode
{

template<typename OwnerInfoContainer>
class basic_proxy_mode
{
public:
	typedef OwnerInfoContainer owner_info_container_type;
	typedef typename mplex::get_container_value_t<owner_info_container_type>::type owner_info_type;
	typedef typename owner_info_type::owner_id_type owner_id_type;
	typedef boost::container::vector<owner_id_type> owner_id_container_type;

public:
	virtual ~basic_proxy_mode(void) {};
	virtual u32 now_mode(void) const = 0;
	virtual owner_info_container_type
				get_owner_info_container(const owner_info_type&, const owner_id_container_type&) const = 0;
};


} // namespace proxy_mode
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_MODE_BASIC_PROXY_MODE_HPP__