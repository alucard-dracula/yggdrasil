//proxy_modes.hpp

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

#ifndef __YGGR_PROXY_PROXY_MODE_PROXY_MODES_HPP__
#define __YGGR_PROXY_PROXY_MODE_PROXY_MODES_HPP__

#include <yggr/iterator_ex/iterator.hpp>

#include <yggr/proxy/proxy_mode/proxy_mode_def.hpp>
#include <yggr/proxy/proxy_mode/basic_proxy_mode.hpp>

#include <boost/atomic.hpp>
#include <boost/range/functions.hpp>
#include <boost/functional/hash/hash.hpp>

#include <cassert>

namespace yggr
{
namespace proxy
{
namespace proxy_mode
{

template<typename ModeDataStore>
class proxy_mode_monopolize
	: public basic_proxy_mode<ModeDataStore>
{
public:
	YGGR_STATIC_CONSTANT(u32, E_mode = proxy_mode_def::E_proxy_mode_monopolize);

	typedef ModeDataStore mode_data_store_type;
	typedef basic_proxy_mode<mode_data_store_type> base_type;

	typedef typename base_type::owner_info_type owner_info_type;
	typedef typename base_type::owner_info_container_type owner_info_container_type;

	typedef typename base_type::store_type store_type;
	typedef typename base_type::ex_args_type ex_args_type;

private:
	typedef typename mode_data_store_type::vector_store_type vector_store_type;

private:
	typedef proxy_mode_monopolize this_type;

public:
	proxy_mode_monopolize(void) 
		: base_type(this_type::E_mode)
	{
	}

	~proxy_mode_monopolize(void) 
	{
	}

protected:
	virtual owner_info_container_type&
		pro_get_owner_info_container(owner_info_container_type& cont,
										const owner_info_type& owner_info,
										const store_type& oid_cont,
										const utility::basic_args_holder_nv*) const
	{
		const vector_store_type& vt = mode_data_store_type::s_get_vector_store_of_base(oid_cont);
		if(boost::empty(vt))
		{
			return cont;
		}

		owner_info_type info(owner_info);
		info.push(*(boost::begin(vt)));

		iterator_ex::inserter(cont, boost::end(cont)) = boost::move(info);

		return cont;
	}
};

template<typename ModeDataStore>
class proxy_mode_source_hash
	: public basic_proxy_mode<ModeDataStore>
{
public:
	YGGR_STATIC_CONSTANT(u32, E_mode = proxy_mode_def::E_proxy_mode_source_hash);

	typedef basic_proxy_mode<ModeDataStore> base_type;

	typedef typename base_type::mode_data_store_type mode_data_store_type;
	typedef typename base_type::owner_info_type owner_info_type;
	typedef typename base_type::owner_info_container_type owner_info_container_type;

	typedef typename base_type::store_type store_type;
	typedef typename base_type::ex_args_type ex_args_type;

private:
	typedef typename mode_data_store_type::vector_store_type vector_store_type;

private:
	typedef typename owner_info_type::owner_id_type owner_id_type;

private:
	typedef proxy_mode_source_hash this_type;

public:
	proxy_mode_source_hash(void) 
		: base_type(this_type::E_mode)
	{
	}

	~proxy_mode_source_hash(void) 
	{
	}

protected:
	virtual owner_info_container_type&
		pro_get_owner_info_container(owner_info_container_type& cont,
									const owner_info_type& owner_info,
									const store_type& oid_cont,
									const utility::basic_args_holder_nv*) const
	{
		const vector_store_type& vt = mode_data_store_type::s_get_vector_store_of_base(oid_cont);
		if(boost::empty(vt))
		{
			return cont;
		}

		owner_id_type src_id = owner_info.owner_id();
		assert(src_id);

		boost::hash<owner_id_type> hasher;
		std::size_t use_idx = hasher(src_id) % boost::size(vt);

		owner_info_type info(owner_info);
		info.push(vt[use_idx]);

		iterator_ex::inserter(cont, boost::end(cont)) = boost::move(info);

		return cont;
	}
};

template<typename ModeDataStore>
class proxy_mode_blanace
	: public basic_proxy_mode<ModeDataStore>
{
public:
	YGGR_STATIC_CONSTANT(u32, E_mode = proxy_mode_def::E_proxy_mode_blanace);

	typedef basic_proxy_mode<ModeDataStore> base_type;

	typedef typename base_type::mode_data_store_type mode_data_store_type;
	typedef typename base_type::owner_info_type owner_info_type;
	typedef typename base_type::owner_info_container_type owner_info_container_type;

	typedef typename base_type::store_type store_type;
	typedef typename base_type::ex_args_type ex_args_type;

private:
	typedef typename mode_data_store_type::vector_store_type vector_store_type;

private:
	typedef boost::atomic<std::size_t> idx_type;

private:
	typedef proxy_mode_blanace this_type;

public:
	proxy_mode_blanace(void)
		: base_type(this_type::E_mode),
			_now_idx(0) 
	{
	}

	~proxy_mode_blanace(void) 
	{
	}

protected:
	virtual owner_info_container_type&
		pro_get_owner_info_container(owner_info_container_type& cont,
										const owner_info_type& owner_info,
										const store_type& oid_cont,
										const utility::basic_args_holder_nv*) const
	{
		const vector_store_type& vt = mode_data_store_type::s_get_vector_store_of_base(oid_cont);
		if(boost::empty(vt))
		{
			return cont;
		}

		std::size_t now_idx = 0;
		std::size_t use_idx = 0;
		do
		{
			now_idx = _now_idx.load();
			use_idx = now_idx % boost::size(vt);

		} while(!_now_idx.compare_exchange_weak(now_idx, use_idx + 1));

		owner_info_type info(owner_info);
		info.push(vt[use_idx]);
		iterator_ex::inserter(cont, boost::end(cont)) = boost::move(info);

		return cont;
	}

private:
	mutable idx_type _now_idx;
};

template<typename ModeDataStore>
class proxy_mode_all
	: public basic_proxy_mode<ModeDataStore>
{
public:
	YGGR_STATIC_CONSTANT(u32, E_mode = proxy_mode_def::E_proxy_mode_all);

	typedef basic_proxy_mode<ModeDataStore> base_type;

	typedef typename base_type::mode_data_store_type mode_data_store_type;
	typedef typename base_type::owner_info_type owner_info_type;
	typedef typename base_type::owner_info_container_type owner_info_container_type;

	typedef typename base_type::store_type store_type;
	typedef typename base_type::ex_args_type ex_args_type;

private:
	typedef typename mode_data_store_type::vector_store_type vector_store_type;

private:
	typedef proxy_mode_all this_type;

public:
	proxy_mode_all(void) 
		: base_type(this_type::E_mode)
	{
	}
	
	~proxy_mode_all(void) 
	{
	}

protected:
	virtual owner_info_container_type&
		pro_get_owner_info_container(owner_info_container_type& cont,
									const owner_info_type& owner_info,
									const store_type& oid_cont,
									const utility::basic_args_holder_nv*) const
	{
		typedef typename boost::range_const_iterator<vector_store_type>::type citer_type;
		const vector_store_type& vt = mode_data_store_type::s_get_vector_store_of_base(oid_cont);
		if(boost::empty(vt))
		{
			return cont;
		}

		for(citer_type i = boost::begin(vt), isize = boost::end(vt); i != isize; ++i)
		{
			owner_info_type info(owner_info);
			info.push(*i);
			iterator_ex::inserter(cont, boost::end(cont)) = boost::move(info);
		}

		return cont;
	}
};

} // namespace proxy_mode
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_MODE_PROXY_MODES_HPP__
