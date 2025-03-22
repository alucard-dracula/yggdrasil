//p2p_peer_runtime_info_mgr.hpp

/****************************************************************************
Copyright (c) 2010-2024 yggdrasil

author: xu yang

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

#ifndef __YGGR_P2P_P2P_PEER_RUNTIME_INFO_MGR_HPP__
#define __YGGR_P2P_P2P_PEER_RUNTIME_INFO_MGR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/smart_ptr_ex/shared_ptr.hpp>
#include <yggr/ppex/foo_params.hpp>
#include <yggr/container/mapped_value.hpp>

#include <yggr/network/session_helper/session_creator.hpp>
#include <yggr/network/session_helper/session_checker_creator.hpp>
#include <yggr/network/session_helper/session_crypher_creator.hpp>

#include <yggr/p2p/protocol/p2p_peer_boot_info.hpp>
#include <yggr/p2p/protocol/p2p_encryption_peer_boot_info.hpp>

#include <yggr/p2p/p2p_default_peer_user_data_fixer.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

namespace yggr
{
namespace p2p
{
namespace detail
{

// p2p_peer_runtime_info_mgr_session_id_creator
template<typename OwnerID, typename PeerBootInfo>
struct p2p_peer_runtime_info_mgr_session_id_creator
{
	typedef OwnerID owner_id_type;
	typedef PeerBootInfo peer_boot_info_type;
	typedef const owner_id_type& result_type;

	inline result_type operator()(const peer_boot_info_type& val) const
	{
		return val.owner_id();
	}
};

// p2p_peer_runtime_info_mgr_session_checker_creator
template<typename PacketsCheck, typename PeerBootInfo>
struct p2p_peer_runtime_info_mgr_session_checker_creator
{
	typedef PacketsCheck packets_checker_type;
	typedef PeerBootInfo peer_boot_info_type;
	typedef ::yggr::shared_ptr<packets_checker_type> packets_checker_ptr_type;

	inline packets_checker_ptr_type operator()(const peer_boot_info_type& val) const
	{
		return ::yggr::make_shared<packets_checker_type>(val.owner_id());
	}
};

// p2p_peer_runtime_info_mgr_session_crypher_creator
template<typename Start_Data, typename CrypherCheck>
struct p2p_peer_runtime_info_mgr_session_crypher_creator;

// non_has_encryption_keys
template<typename Owner_ID, typename Socket_Info, typename CrypherCheck>
struct p2p_peer_runtime_info_mgr_session_crypher_creator< 
			p2p_peer_boot_info<Owner_ID, Socket_Info>, CrypherCheck>
{
	typedef p2p_peer_boot_info<Owner_ID, Socket_Info> now_peer_boot_info_type;
	typedef CrypherCheck packets_crypher_type;
	typedef ::yggr::shared_ptr<packets_crypher_type> packets_crypher_ptr_type;

	inline packets_crypher_ptr_type operator()(const now_peer_boot_info_type& val) const
	{
		return ::yggr::make_shared<packets_crypher_type>();
	}
};

// has_encryption_keys
template<typename Owner_ID, typename Socket_Info, typename EKeys, typename CrypherCheck>
struct p2p_peer_runtime_info_mgr_session_crypher_creator< 
			p2p_encryption_peer_boot_info<Owner_ID, Socket_Info, EKeys>, CrypherCheck>
{
	typedef p2p_encryption_peer_boot_info<Owner_ID, Socket_Info, EKeys> now_peer_boot_info_type;
	typedef CrypherCheck packets_crypher_type;
	typedef ::yggr::shared_ptr<packets_crypher_type> packets_crypher_ptr_type;

	inline packets_crypher_ptr_type operator()(const now_peer_boot_info_type& val) const
	{
		return ::yggr::make_shared<packets_crypher_type>(val.encryption_keys());
	}
};

} // namespace detail
} // namespace p2p
} // namespace yggr

namespace yggr
{
namespace p2p
{

template<typename Owner, 
			typename Conn,
			typename P2P_Boot_Request_Back,
			typename P2P_Boot_Start,
			typename Packets_Checker,
			typename Packets_Crypher,
			typename User_Data_Fixer
						= p2p_default_peer_user_data_fixer
							<
								Owner,
								P2P_Boot_Request_Back,
								P2P_Boot_Start
							>
		>
class p2p_peer_runtime_info_mgr
{
public:
	typedef Owner owner_type;
	typedef Conn conn_type;
	typedef P2P_Boot_Request_Back p2p_boot_request_back_type;
	typedef P2P_Boot_Start p2p_boot_start_type;
	
	typedef Packets_Checker packets_checker_type;
	typedef Packets_Crypher packets_crypher_type;
	
	typedef User_Data_Fixer user_data_fixer_type;

	typedef typename owner_type::owner_id_type owner_id_type;
	typedef typename p2p_boot_request_back_type::peer_boot_info_cont_type peer_boot_info_cont_type;
	typedef typename p2p_boot_request_back_type::peer_boot_info_type peer_boot_info_type;

protected:
	typedef 
		detail::p2p_peer_runtime_info_mgr_session_id_creator
		<
			owner_id_type, 
			peer_boot_info_type
		> session_id_creator_type;

	typedef 
		detail::p2p_peer_runtime_info_mgr_session_checker_creator
		<
			packets_checker_type,
			peer_boot_info_type
		> session_checker_creator_type;

public:
	typedef 
		network::session_helper::session_creator
		<
			peer_boot_info_type,
			conn_type,
			session_id_creator_type,
			session_checker_creator_type,
			detail::p2p_peer_runtime_info_mgr_session_crypher_creator
			<
				peer_boot_info_type, 
				packets_crypher_type
			>
		> p2p_session_creator_type;

private:
	typedef p2p_peer_runtime_info_mgr this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	p2p_peer_runtime_info_mgr(BOOST_FWD_REF(Args)... args)
		: _user_data_fixer(boost::forward<Args>(args)...)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, template< ) \
		YGGR_PP_FOO_TYPES_DEF( __n__ ) \
	BOOST_PP_EXPR_IF( __n__, > ) \
	p2p_peer_runtime_info_mgr( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
		: _user_data_fixer(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )) { }

#define YGGR_PP_FOO_ARG_NAME() init_arg
#define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#include BOOST_PP_LOCAL_ITERATE(  )
#undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~p2p_peer_runtime_info_mgr(void)
	{
	}

public:
	inline void fix_boot_request_back(const owner_type& owner, 
										const p2p_boot_request_back_type& p2p_boot_request_back,
										peer_boot_info_cont_type& peer_boot_info_cont,
										p2p_boot_start_type& boot_start )
	{
		if(!_user_data_fixer.fix_boot_request_back_enable(owner, p2p_boot_request_back))
		{
			return;
		}

		peer_boot_info_cont_type& ref_peer_boot_info_cont 
			= const_cast<peer_boot_info_cont_type&>(p2p_boot_request_back.peer_boot_infos());
		peer_boot_info_cont_type ret_peer_boot_info_cont;
		ret_peer_boot_info_cont.swap(ref_peer_boot_info_cont);

		_user_data_fixer.fix_boot_request_back(owner, ret_peer_boot_info_cont, boot_start);

		yggr::swap(peer_boot_info_cont, ret_peer_boot_info_cont);

	}

	inline void drop_peer(const owner_id_type& owner_id)
	{
		_user_data_fixer.remove_info(owner_id);
	}

	inline void drop_peer(const owner_type& owner)
	{
		_user_data_fixer.remove_info(owner.owner_id());
	}

protected:
	user_data_fixer_type _user_data_fixer;
};

} // namespace p2p
} // namespace yggr

#endif // __YGGR_P2P_P2P_PEER_RUNTIME_INFO_MGR_HPP__
