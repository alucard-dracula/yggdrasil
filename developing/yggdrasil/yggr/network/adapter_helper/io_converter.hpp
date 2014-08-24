//io_converter.hpp

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

#ifndef __YGGR_NETWORK_ADAPTER_HELPER_IO_CONVERTER_HPP__
#define __YGGR_NETWORK_ADAPTER_HELPER_IO_CONVERTER_HPP__

#include <yggr/task_center/support/task_real_data_getter.hpp>
#include <yggr/task_center/type_traits/tags.hpp>

namespace yggr
{
namespace network
{
namespace adapter_helper
{

template<typename Send_Data, typename Task, typename Recv_Data>
struct io_converter
{
public:
	typedef Send_Data send_data_type;
	typedef Task task_type;
	typedef Recv_Data recv_data_type;

	typedef bool conv_to_send_result_type;
	typedef const send_data_type& conv_to_send_arg_data_type;

	typedef recv_data_type conv_from_recv_result_type;
	typedef const task_type& conv_from_recv_arg_data_type;

private:

	typedef task_center::support::task_real_data_getter<task_type> task_real_data_getter_type;
	typedef io_converter this_type;

public:

	template<typename Real_Data_Type, typename Src>
	static conv_to_send_result_type conv_to_send(Src& src, conv_to_send_arg_data_type data)
	{
		Real_Data_Type rdata;
		if(!data.load(rdata))
		{
			return false;
		}

		src.template send<yggr::task_center::type_traits::mark_saver_condition>(data.net_info(), rdata);
		return true;
	}

	template<typename Real_Data_Type>
	static conv_from_recv_result_type conv_from_recv(conv_from_recv_arg_data_type tk)
	{
		task_real_data_getter_type getter;
#ifdef _MSC_VER
		const Real_Data_Type *pval = getter.operator()<Real_Data_Type>(tk);
#else
        const Real_Data_Type *pval = getter.template operator()<Real_Data_Type>(tk);
#endif // _MSC_VER
		assert(pval);
		if(!pval)
		{
			return recv_data_type();
		}

		return recv_data_type(tk.owner_info(), *pval);
	}
};

} // namespace adapter_helper
} // namespace network
} // namespace yggr

#endif // __YGGR_NETWORK_ADAPTER_HELPER_IO_CONVERTER_HPP__
