//server_active_start_mode.hpp

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

#ifndef __YGGR_SERVER_START_MODE_SERVER_ACTIVE_START_MODE_HPP__
#define __YGGR_SERVER_START_MODE_SERVER_ACTIVE_START_MODE_HPP__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <yggr/network/type_traits/tags.hpp>

namespace yggr
{
namespace server
{
namespace start_mode
{

template<	typename ID_Generator,
			typename Start_Data_Generator,
			typename Owner_Generator,
			typename Session_Creator>
class server_active_start_mode
{

private:

	typedef ID_Generator id_generator_type;
	typedef typename id_generator_type::id_type id_type;

	typedef Start_Data_Generator start_data_generator_type;
	typedef typename start_data_generator_type::start_data_type start_data_type;

	typedef Owner_Generator owner_generator_type;

	typedef Session_Creator session_creator_type;

private:
	typedef server_active_start_mode this_type;

public:
	template<typename ConnPtr, typename Session_Mgr>
	void operator()(ConnPtr pconn, Session_Mgr& smgr) const
	{
		prv_create_helper(pconn, smgr);
	}

private:

	template<typename ConnPtr, typename Session_Mgr>
	void prv_create_helper(ConnPtr pconn, Session_Mgr& smgr) const
	{
		typedef ConnPtr conn_ptr_type;
		typedef Session_Mgr session_mgr_type;

		typedef typename session_mgr_type::tag_type session_mgr_tag_type;

		BOOST_MPL_ASSERT((
							boost::is_same
							<
								session_mgr_tag_type,
								yggr::network::type_traits::tag_server
							>
						));

		if(!pconn)
		{
			return;
		}

		id_generator_type id_gen;
		id_type now_id = id_gen(pconn);


		start_data_generator_type start_gen;
		start_data_type start_data(start_gen(now_id));

		owner_generator_type owner_gen;

		pconn->send_packet(
							owner_gen(id_type()),
							start_data,
							boost::bind(
											&session_mgr_type::template handler_create<session_creator_type>,
											smgr.shared_from_this(),
											boost::asio::placeholders::error,
											session_creator_type(pconn, start_data)
										)
							);
	}
};

} // namespace start_mode
} // namespace server
} // namespace yggr

#endif // __YGGR_SERVER_START_MODE_SERVER_ACTIVE_START_MODE_HPP__
