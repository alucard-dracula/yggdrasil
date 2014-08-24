//proxy_io_converter.hpp

#ifndef __YGGR_PROXY_ADAPTER_HELPER_IO_CONVERTER_HPP__
#define __YGGR_PROXY_ADAPTER_HELPER_IO_CONVERTER_HPP__

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <yggr/move/move.hpp>

#include <yggr/task_center/type_traits/tags.hpp>


namespace yggr
{
namespace proxy
{
namespace adapter_helper
{

template<typename Send_Data, typename Task, typename Recv_Data>
struct proxy_io_converter
{
public:
	typedef Send_Data send_data_type;
	typedef Task task_type;
	typedef Recv_Data recv_data_type;

	typedef bool conv_to_send_result_type;
	typedef send_data_type& conv_to_send_arg_data_type;

	typedef recv_data_type conv_from_recv_result_type;
	typedef const task_type& conv_from_recv_arg_data_type;

private:

	BOOST_MPL_ASSERT((boost::is_same<Send_Data, Task>));
	typedef proxy_io_converter this_type;

public:

	template<typename Real_Data_Type, typename Src>
	static conv_to_send_result_type conv_to_send(Src& src, conv_to_send_arg_data_type data)
	{
		BOOST_MPL_ASSERT((boost::is_same<Real_Data_Type, task_type>));
		src.template send<yggr::task_center::type_traits::mark_saver_condition>(data);
		return true;
	}

	template<typename Real_Data_Type>
	static conv_from_recv_result_type conv_from_recv(conv_from_recv_arg_data_type tk)
	{
		BOOST_MPL_ASSERT((boost::is_same<Real_Data_Type, task_type>));
		task_type& ref_task = const_cast<task_type&>(tk);
		return recv_data_type(boost::move(ref_task));
	}
};

} // namespace adapter_helper
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_ADAPTER_HELPER_IO_CONVERTER_HPP__
