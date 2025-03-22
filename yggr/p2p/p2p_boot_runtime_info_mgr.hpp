//p2p_boot_runtime_info_mgr.hpp

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

#ifndef __YGGR_P2P_P2P_BOOT_RUNTIME_INFO_MGR_HPP__
#define __YGGR_P2P_P2P_BOOT_RUNTIME_INFO_MGR_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/ppex/foo_params.hpp>

#include <yggr/container/mapped_value.hpp>
#include <yggr/container/list.hpp>
#include <yggr/container/unordered_map.hpp>

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/p2p/protocol/p2p_peer_boot_info.hpp>
#include <yggr/p2p/protocol/p2p_encryption_peer_boot_info.hpp>
#include <yggr/p2p/p2p_peer_mapping.hpp>
#include <yggr/p2p/p2p_default_boot_user_data_fixer.hpp>

#include <boost/range/functions.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>

#include <iterator>

namespace yggr
{
namespace p2p
{

template<typename Owner_Container, 
			typename P2P_Boot_Request,
			typename P2P_Boot_Request_Back,
			typename Peer_Session_ID_Generator,
			typename P2P_Peer_Boot_Info_Generator
						= peer_boot_info_generator
							<
								typename P2P_Boot_Request_Back::peer_boot_info_type 
							>,
			typename User_Data_Fixer
						= p2p_default_boot_user_data_fixer
							<
								Owner_Container,
								P2P_Boot_Request,
								P2P_Boot_Request_Back
							>
	>
class p2p_boot_runtime_info_mgr
{
public:
	typedef Owner_Container owner_info_container_type;
	typedef P2P_Boot_Request p2p_boot_request_type;
	typedef P2P_Boot_Request_Back p2p_boot_request_back_type;
	typedef Peer_Session_ID_Generator peer_session_id_generator_type;
	typedef P2P_Peer_Boot_Info_Generator p2p_peer_boot_info_generator_type;
	typedef User_Data_Fixer user_data_fixer_type;

	typedef typename container_mapped_value_t<owner_info_container_type>::type owner_type;
	typedef typename owner_type::owner_id_type owner_id_type;
	typedef typename p2p_boot_request_type::socket_info_type socket_info_type;

	typedef typename p2p_boot_request_back_type::peer_boot_info_type peer_boot_info_type;

	typedef yggr::unordered_map<owner_type, p2p_boot_request_back_type> p2p_boot_request_back_map_type;

private:
	typedef typename peer_session_id_generator_type::owner_id_type chk_id_type;
	BOOST_MPL_ASSERT((boost::is_same<owner_id_type, chk_id_type>));

protected:
	typedef p2p_peer_mapping<owner_type, socket_info_type> p2p_peer_mapping_type;

private:
	typedef p2p_boot_runtime_info_mgr this_type;

public:

#ifndef YGGR_NO_CXX11_VARIADIC_TEMPLATES

	template<typename ...Args>
	p2p_boot_runtime_info_mgr(BOOST_FWD_REF(Args)... args)
		: _user_data_fixer(boost::forward<Args>(args)...)
	{
	}

#else

#	define BOOST_PP_LOCAL_MACRO( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, template< ) \
			YGGR_PP_FOO_TYPES_DEF( __n__ ) \
		BOOST_PP_EXPR_IF( __n__, > ) \
		p2p_boot_runtime_info_mgr( YGGR_PP_FOO_PARAMS_DEF( __n__, YGGR_PP_FOO_ANYREF_PARAMS ) ) \
			: _user_data_fixer(YGGR_PP_FOO_PARAMS_OP_BOOST_FORWARD( __n__, YGGR_PP_SYMBOL_COMMA )) { }

#	define YGGR_PP_FOO_ARG_NAME() init_arg
#	define BOOST_PP_LOCAL_LIMITS ( 0, YGGR_PP_FOO_DEFAULT_PARAMS_LEN )
#	include BOOST_PP_LOCAL_ITERATE(  )
#	undef YGGR_PP_FOO_ARG_NAME

#endif // YGGR_NO_CXX11_VARIADIC_TEMPLATES

	~p2p_boot_runtime_info_mgr(void)
	{
	}

public:
	void fix_boot_request(const owner_type& owner, 
							const socket_info_type& src_nat_socket_info,
							const p2p_boot_request_type& p2p_boot_request,
							owner_info_container_type& owners_self,
							p2p_boot_request_back_type& back_self,
							p2p_boot_request_back_map_type& back_others )
	{
		typedef typename p2p_peer_mapping_type::socket_info_pair_type socket_info_pair_type;
		typedef typename p2p_peer_mapping_type::mapping_socket_info_result_type mapping_socket_info_result_type;
		typedef typename p2p_boot_request_back_type::peer_boot_info_cont_type peer_boot_info_cont_type;
		typedef typename p2p_peer_boot_info_generator_type::result_type boot_info_gen_result_type;

		if(!_user_data_fixer.fix_boot_request_enable(owner, p2p_boot_request))
		{
			return;
		}

		{
			{
				owner_info_container_type tmp;
				yggr::swap(owners_self, tmp);
			}

			{
				p2p_boot_request_back_map_type tmp;
				yggr::swap(tmp, back_others);
			}
		}

		_p2p_peer_mapping.compulsory_append(
			owner, 
			src_nat_socket_info,
			p2p_boot_request.local_socket_info());


		owner_info_container_type owners_other;
		_p2p_peer_mapping.mapping_other_owners(owner, owners_other);
		
		if(boost::empty(owners_other))
		{
			return;
		}

		peer_session_id_generator_type id_gen;
		p2p_peer_boot_info_generator_type peer_boot_info_gen;

		iterator_ex::inserter(owners_self, boost::end(owners_self)) = owner;
		
		peer_boot_info_cont_type& ref_peer_infos_self = back_self.peer_boot_infos();

		for(typename owner_info_container_type::const_iterator i = boost::begin(owners_other), isize = boost::end(owners_other);
				i != isize; ++i)
		{
			owner_id_type owner_id = id_gen(owner.owner_id(), (*i).owner_id());
			if(owner_id == owner_id_type())
			{
				continue;
			}

			mapping_socket_info_result_type socket_info_result = _p2p_peer_mapping.get_socket_info(*i);
			if(!socket_info_result.second)
			{
				continue;
			}
			
			// pair_pair = pair_pair less pair_pair.first.second?! what the fuck!!! 
			//boot_info_gen_result_type boot_infos(peer_boot_info_gen(owner_id,
			//											src_nat_socket_info, 
			//											p2p_boot_request.local_socket_info(),
			//											socket_info_result.first.first,
			//											socket_info_result.first.second));

			boot_info_gen_result_type boot_infos;
			peer_boot_info_gen(
					boot_infos,
					owner_id,
					src_nat_socket_info, 
					p2p_boot_request.local_socket_info(),
					socket_info_result.first.first,
					socket_info_result.first.second);

			iterator_ex::inserter(ref_peer_infos_self, boost::end(ref_peer_infos_self)) 
				= boot_infos.first;

			p2p_boot_request_back_type back_other;
			peer_boot_info_cont_type& ref_peer_infos_other = back_other.peer_boot_infos();

			iterator_ex::inserter(ref_peer_infos_other, boost::end(ref_peer_infos_other)) 
				= boot_infos.second;

			back_others[*i] = back_other;

		}

		_user_data_fixer.fix_boot_request(
			owner, 
			p2p_boot_request, 
			owners_self,
			back_self,
			back_others);
	}

	inline void drop_peer(const owner_id_type& owner_id)
	{
		_p2p_peer_mapping.remove(owner_id);
		_user_data_fixer.remove_info(owner_id);
	}

	inline void drop_peer(const owner_type& owner)
	{
		_p2p_peer_mapping.remove(owner.owner_id());
		_user_data_fixer.remove_info(owner.owner_id());
	}

	inline user_data_fixer_type& user_data_fixer(void)
	{
		return _user_data_fixer;
	}

	inline const user_data_fixer_type& user_data_fixer(void) const
	{
		return _user_data_fixer;
	}

protected:
	user_data_fixer_type _user_data_fixer;
	p2p_peer_mapping_type _p2p_peer_mapping;
};

} // namespace p2p
} // namespace yggr

#endif // __YGGR_P2P_P2P_BOOT_RUNTIME_INFO_MGR_HPP__
