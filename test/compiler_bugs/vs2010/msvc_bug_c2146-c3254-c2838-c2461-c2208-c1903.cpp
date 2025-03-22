//msvc_bug_c2146-c3254-c2838-c2461-c2208-c1903.cpp



//conflict first
//#include <yggr/nsql_database_system/c_bson.hpp>
//#include <yggr/packet/packet.hpp>


#include <yggr/nsql_database_system/c_bson_symbol.hpp>

inline bool operator!=(const yggr::nsql_database_system::c_bson_symbol& l, const bson_value_t& r)
{
	typedef yggr::nsql_database_system::c_bson_symbol::string_type now_string_type;
	return !yggr
			::nsql_database_system
			::bson_value_native_ex
			::s_value_cmp_symbol(&r, l.symbol<now_string_type>());
}


//#include <yggr/system_controller/detail/ctrl_center_dispatcher_container.hpp>

// ctrl_center_dispatcher_container.hpp

#include <boost/ref.hpp>

#include <yggr/base/yggrdef.h>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/smart_ptr_ex/shared_ptr.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/safe_container/safe_unordered_map.hpp>

#include <yggr/system_controller/detail/ctrl_center_controller.hpp>

namespace yggr
{
namespace system_controller
{
namespace detail
{

void test(void)
{
	typedef ctrl_center_types_def::handler_id_type handler_id_type;
	typedef 
		safe_container::safe_unordered_map
		<
			handler_id_type, 
			base_ctrl_ptr_type
		> controller_container_type;
	controller_container_type cont;
	cont.clear();
}

} // namespace detail
} // namespace system_contrller
} // namespace yggr

int main(int argc, char* argv[])
{
	return 0;
}
