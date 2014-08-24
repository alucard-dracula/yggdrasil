//proxy_modes.hpp

#ifndef __YGGR_PROXY_PROXY_MODE_PROXY_MODES_HPP__
#define __YGGR_PROXY_PROXY_MODE_PROXY_MODES_HPP__

#include <cassert>

#include <boost/atomic.hpp>
#include <boost/functional/hash/hash.hpp>

#include <yggr/proxy/proxy_mode/proxy_mode_def.hpp>
#include <yggr/proxy/proxy_mode/basic_proxy_mode.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_mode
{

template<typename OwnerInfoContainer>
class proxy_mode_monopolize
	: public basic_proxy_mode<OwnerInfoContainer>
{
public:
	enum
	{
		E_mode = proxy_mode_def::E_proxy_mode_monopolize
	};

	typedef basic_proxy_mode<OwnerInfoContainer> base_type;
	typedef typename base_type::owner_info_type owner_info_type;
	typedef typename base_type::owner_id_container_type owner_id_container_type;
	typedef typename base_type::owner_info_container_type owner_info_container_type;

public:
	proxy_mode_monopolize(void) {}
	virtual ~proxy_mode_monopolize(void) {}

	virtual u32 now_mode(void) const
	{
		return E_mode;
	}

	virtual owner_info_container_type
				get_owner_info_container(const owner_info_type& owner_info,
											const owner_id_container_type& oid_cont) const
	{
		owner_info_container_type cont;
		if(oid_cont.empty())
		{
			return cont;
		}

		owner_info_type info(owner_info);
		info.push(*(oid_cont.begin()));
		cont.insert(info);
		return cont;
	}
};

template<typename OwnerInfoContainer>
class proxy_mode_source_hash
	: public basic_proxy_mode<OwnerInfoContainer>
{
public:
	enum
	{
		E_mode = proxy_mode_def::E_proxy_mode_source_hash
	};

	typedef basic_proxy_mode<OwnerInfoContainer> base_type;
	typedef typename base_type::owner_info_type owner_info_type;
	typedef typename base_type::owner_id_container_type owner_id_container_type;
	typedef typename base_type::owner_info_container_type owner_info_container_type;

private:
	typedef typename owner_info_type::owner_id_type owner_id_type;

public:
	proxy_mode_source_hash(void) {}
	virtual ~proxy_mode_source_hash(void) {}

	virtual u32 now_mode(void) const
	{
		return E_mode;
	}

	virtual owner_info_container_type
				get_owner_info_container(const owner_info_type& owner_info,
											const owner_id_container_type& oid_cont) const
	{
		owner_info_container_type cont;

		owner_id_type src_id = owner_info.owner_id();
		assert(src_id);

		boost::hash<owner_id_type> hasher;
		std::size_t use_idx = hasher(src_id) % oid_cont.size();
		//std::size_t use_idx = 0;

		owner_info_type info(owner_info);
		info.push(oid_cont[use_idx]);
		cont.insert(info);
		return cont;
	}
};

template<typename OwnerInfoContainer>
class proxy_mode_blanace
	: public basic_proxy_mode<OwnerInfoContainer>
{
public:
	enum
	{
		E_mode = proxy_mode_def::E_proxy_mode_blanace
	};

	typedef basic_proxy_mode<OwnerInfoContainer> base_type;
	typedef typename base_type::owner_info_type owner_info_type;
	typedef typename base_type::owner_id_container_type owner_id_container_type;
	typedef typename base_type::owner_info_container_type owner_info_container_type;

private:
	typedef boost::atomic<std::size_t> idx_type;

public:
	proxy_mode_blanace(void) : _now_idx(0) {}
	virtual ~proxy_mode_blanace(void) {}

	virtual u32 now_mode(void) const
	{
		return E_mode;
	}

	virtual owner_info_container_type
				get_owner_info_container(const owner_info_type& owner_info,
											const owner_id_container_type& oid_cont) const
	{
		owner_info_container_type cont;

		if(oid_cont.empty())
		{
			return cont;
		}

		std::size_t now_idx = 0;
		std::size_t use_idx = 0;
		do
		{
			now_idx = _now_idx.load();
			use_idx = now_idx % oid_cont.size();

		}while(!_now_idx.compare_exchange_strong(now_idx, use_idx + 1));

		owner_info_type info(owner_info);
		info.push(oid_cont[use_idx]);
		cont.insert(info);
		return cont;
	}

private:
	mutable idx_type _now_idx;
};

template<typename OwnerInfoContainer>
class proxy_mode_all
	: public basic_proxy_mode<OwnerInfoContainer>
{
public:
	enum
	{
		E_mode = proxy_mode_def::E_proxy_mode_all
	};

	typedef basic_proxy_mode<OwnerInfoContainer> base_type;
	typedef typename base_type::owner_info_type owner_info_type;
	typedef typename base_type::owner_id_container_type owner_id_container_type;
	typedef typename base_type::owner_info_container_type owner_info_container_type;

public:
	proxy_mode_all(void) {}
	virtual ~proxy_mode_all(void) {}

	virtual u32 now_mode(void) const
	{
		return E_mode;
	}

	virtual owner_info_container_type
				get_owner_info_container(const owner_info_type& owner_info,
											const owner_id_container_type& oid_cont) const
	{
		typedef typename owner_id_container_type::const_iterator citer_type;

		owner_info_container_type cont;
		for(citer_type i = oid_cont.begin(), isize = oid_cont.end(); i != isize; ++i)
		{
			owner_info_type info(owner_info);
			info.push(*i);
			cont.insert(info);
		}

		return cont;
	}
};

} // namespace proxy_mode
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_MODE_PROXY_MODES_HPP__
