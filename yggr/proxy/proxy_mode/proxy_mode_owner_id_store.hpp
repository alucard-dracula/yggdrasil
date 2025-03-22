//proxy_mode_owner_id_store.hpp

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

#ifndef __YGGR_PROXY_PROXY_MODE_PROXY_MODE_OWNER_ID_STORE_HPP__
#define __YGGR_PROXY_PROXY_MODE_PROXY_MODE_OWNER_ID_STORE_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/bind/bind.hpp>

#include <yggr/utility/args_holder_nv.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/safe_container/safe_multi_index_random_access_index.hpp>
#include <yggr/safe_container/safe_multi_index_hashed_index.hpp>
#include <yggr/safe_container/safe_multi_index_container.hpp>

#include <boost/range/value_type.hpp>
#include <boost/range/functions.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_mode
{

template<typename OwnerInfoContainer, typename ExArgs = void>
class proxy_mode_owner_id_store
{
public:
	typedef OwnerInfoContainer owner_info_container_type;
	typedef typename boost::range_value<owner_info_container_type>::type owner_info_type;
	typedef typename owner_info_type::owner_id_type owner_id_type;

	struct tag_hash{};
	struct tag_vt{};

	typedef
		safe_container::safe_multi_index_container<
			owner_id_type,
			boost::multi_index::indexed_by<
				boost::multi_index::hashed_unique<
					boost::multi_index::tag<tag_hash>,
					boost::multi_index::identity<owner_id_type> >,

				boost::multi_index::random_access<
					boost::multi_index::tag<tag_vt> >
			>
		> owner_id_midx_type;

	typedef owner_id_midx_type safe_store_type;
	typedef typename safe_store_type::base_type store_type;

	typedef typename boost::multi_index::index<store_type, tag_hash>::type hash_store_type;
	typedef typename boost::multi_index::index<store_type, tag_vt>::type vector_store_type;

	typedef ExArgs ex_args_type;

private:
	typedef proxy_mode_owner_id_store this_type;

public:
	proxy_mode_owner_id_store(void)
	{
	}

	~proxy_mode_owner_id_store(void)
	{
	}

public:
	inline bool empty(void) const
	{
		return _owner_id_midx.empty();
	}

	inline bool erase(const owner_id_type& id)
	{
		return !!(_owner_id_midx.template get<tag_hash>().erase(id));
	}

	template<typename ProxyModePtr> inline
	owner_info_container_type&
		get_owner_info_container(owner_info_container_type& cont,
									ProxyModePtr pimode,
									const owner_info_type& owner) const
	{
		typedef ProxyModePtr proxy_mode_ptr_type;

		if(!pimode)
		{
			return cont;
		}

		return
			_owner_id_midx.using_handler(
				boost::bind(&this_type::handler_get_owner_info_container<proxy_mode_ptr_type>,
								_1, boost::ref(cont), pimode, boost::cref(owner)));
	}

	template<typename ProxyModePtr, typename Args> inline
	owner_info_container_type&
		get_owner_info_container(owner_info_container_type& cont,
									ProxyModePtr pimode,
									const owner_info_type& owner,
									Args& args) const
	{
		typedef ProxyModePtr proxy_mode_ptr_type;
		typedef Args args_type;
		typedef utility::args_holder_nv_ref_wrap<args_type> args_holder_type;
		if(!pimode)
		{
			return cont;
		}

		args_holder_type holder(args);

		return
			_owner_id_midx.using_handler(
				boost::bind(
					&this_type::handler_get_owner_info_container_args<proxy_mode_ptr_type>,
					_1, boost::ref(cont), pimode, boost::cref(owner),
					boost::cref(utility::args_holder_trans(holder))));
	}

	template<typename Container> inline
	Container& get_datas(Container& cont) const
	{
		typedef typename owner_id_midx_type::base_type base_midx_type;
		typedef Container container_type;

		base_midx_type tmp_base = _owner_id_midx.load();

		container_type tmp(tmp_base.template get<tag_vt>().begin(), tmp_base.template get<tag_vt>().end());
		::yggr::swap(cont, tmp);

		return cont;
	}

	inline typename owner_id_midx_type::base_type&
		get_datas(typename owner_id_midx_type::base_type& cont) const
	{
		_owner_id_midx.copy_to_base(cont);
		return cont;
	}

	inline owner_id_midx_type& get_datas(owner_id_midx_type& cont) const
	{
		return (cont = _owner_id_midx);
	}

	template<typename Container> inline
	void swap_datas(Container& cont)
	{
		typedef Container container_type;
		typedef typename owner_id_midx_type::base_type base_midx_type;

		base_midx_type tmp(boost::begin(cont), boost::end(cont));
		_owner_id_midx.swap(tmp);

		container_type out_tmp(boost::begin(tmp.template get<tag_vt>()), boost::end(tmp.template get<tag_vt>()));
		::yggr::swap(cont, out_tmp);
	}

	inline void swap_datas(typename owner_id_midx_type::base_type& cont) const
	{
		_owner_id_midx.swap(cont);
	}

	inline void swap_datas(owner_id_midx_type& cont) const
	{
		_owner_id_midx.swap(cont);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler)
	{
		return _owner_id_midx.using_handler(handler);
	}

	template<typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		using_handler(const Handler& handler) const
	{
		return _owner_id_midx.using_handler(handler);
	}

private:
	template<typename ProxyModePtr> inline
	static owner_info_container_type&
		handler_get_owner_info_container(const store_type& cont,
											owner_info_container_type& info_cont,
											ProxyModePtr pimode,
											const owner_info_type& info)
	{
		assert(pimode);
		return pimode->get_owner_info_container(info_cont, info, cont);
	}

	template<typename ProxyModePtr> inline
	static owner_info_container_type&
		handler_get_owner_info_container_args(const store_type& cont,
												owner_info_container_type& info_cont,
												ProxyModePtr pimode,
												const owner_info_type& info,
												const utility::basic_args_holder_nv& args_holder_nv_ref_wrap)
	{
		assert(pimode);
		pimode->get_owner_info_container(info_cont, info, cont, args_holder_nv_ref_wrap);
		return info_cont;
	}

public:
	inline static bool s_is_exists_of_base(const store_type& cont, const owner_id_type& id)
	{
		return cont.template get<tag_hash>().find(id) != cont.template get<tag_hash>().end();
	}

	inline static void s_push_data_of_base(store_type& cont, const owner_id_type& id)
	{
		cont.template get<tag_vt>().push_back(id);
	}

public:
	inline static hash_store_type& s_get_hash_store_of_base(store_type& cont)
	{
		return cont.template get<tag_hash>();
	}

	inline static const hash_store_type& s_get_hash_store_of_base(const store_type& cont)
	{
		return cont.template get<tag_hash>();
	}

	inline static vector_store_type& s_get_vector_store_of_base(store_type& cont)
	{
		return cont.template get<tag_vt>();
	}

	inline static const vector_store_type& s_get_vector_store_of_base(const store_type& cont)
	{
		return cont.template get<tag_vt>();
	}

private:
	owner_id_midx_type _owner_id_midx;
};

} // namespace proxy_mode
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_MODE_PROXY_MODE_OWNER_ID_STORE_HPP__
