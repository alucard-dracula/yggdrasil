//find_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_FIND_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_FIND_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <boost/type_traits/is_same.hpp>

#define YGGR_SAFE_KV_FIND_IMPL() \
	bool find(const key_type& key) const { \
		read_lock_type lk(_mutex); \
		return _base.find(key) != _base.end(); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const key_type& key, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_base, _base.find(key)); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const key_type& key, const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_base, _base.find(key)); }



namespace yggr
{
namespace safe_container
{
namespace detail
{

template<bool is_boost_ukv>
struct call_ukv_find_helper;

template<>
struct call_ukv_find_helper<false>
{
	template<typename Cont> inline
	bool operator()(const Cont& cont, 
					const typename Cont::key_type& key, 
					const typename Cont::hasher&, 
					const typename Cont::key_equal&) const
	{
		return cont.find(key) != cont.end();
	}

	template<typename Cont, typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		operator()(Cont& cont, 
					const typename Cont::key_type& key, 
					const typename Cont::hasher&, 
					const typename Cont::key_equal&,
					const Handler& handler) const
	{
		return handler(cont, cont.find(key));
	}

	template<typename Cont, typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		operator()(const Cont& cont, 
					const typename Cont::key_type& key, 
					const typename Cont::hasher&, 
					const typename Cont::key_equal&,
					const Handler& handler) const
	{
		return handler(cont, cont.find(key));
	}
};

template<>
struct call_ukv_find_helper<true>
{
	template<typename Cont, typename CKey, typename CHash, typename CEq> inline
	bool operator()(const Cont& cont, 
					const CKey& key, 
					const CHash& hash, 
					const CEq& eq) const
	{
		return cont.find(key, hash, eq) != cont.end();
	}

	template<typename Cont, typename CKey, typename CHash, typename CEq,
				typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		operator()(Cont& cont, 
					const CKey& key, 
					const CHash& hash, 
					const CEq& eq,
					const Handler& handler) const
	{
		return handler(cont, cont.find(key, hash, eq));
	}

	template<typename Cont, typename CKey, typename CHash, typename CEq,
				typename Handler> inline
	typename func::foo_t_info<Handler>::result_type
		operator()(const Cont& cont, 
					const CKey& key, 
					const CHash& hash, 
					const CEq& eq,
					const Handler& handler) const
	{
		return handler(cont, cont.find(key, hash, eq));
	}
};

} // namespace detail
} // namespace safe_container
} // namespace yggr

#define YGGR_SAFE_UKV_FIND_IMPL( __cname__ ) \
	YGGR_SAFE_KV_FIND_IMPL() \
	\
	template<typename CKey, typename CHash, typename CEq> \
	bool find(const CKey& key, const CHash& hash, const CEq& eq) const { \
		typedef typename BOOST_PP_CAT(container::is_boost_, __cname__)<base_type>::type chk_type; \
		typedef detail::call_ukv_find_helper<chk_type::value> call_type; \
		call_type call; \
		read_lock_type lk(_mutex); \
		return call(_base, key, hash, eq); } \
	\
	template<typename CKey, typename CHash, typename CEq, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const CKey& key, const CHash& hash, const CEq& eq, const Handler& handler) { \
		typedef typename BOOST_PP_CAT(container::is_boost_, __cname__)<base_type>::type chk_type; \
		typedef detail::call_ukv_find_helper<chk_type::value> call_type; \
		call_type call; \
		write_lock_type lk(_mutex); \
		return call(_base, key, hash, eq, handler); } \
	\
	template<typename CKey, typename CHash, typename CEq, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const CKey& key, const CHash& hash, const CEq& eq, const Handler& handler) const { \
		typedef typename BOOST_PP_CAT(container::is_boost_, __cname__)<base_type>::type chk_type; \
		typedef detail::call_ukv_find_helper<chk_type::value> call_type; \
		call_type call; \
		read_lock_type lk(_mutex); \
		return call(_base, key, hash, eq, handler); }


#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	define YGGR_PP_SAFE_UKV_OPT_FIND_IMPL() \
	bool find(const key_type& key) const { \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
		read_lock_type lk(_mutex); \
		return opt_cast().find(key_hash, key) != _base.end(); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const key_type& key, const Handler& handler) { \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
		write_lock_type lk(_mutex); \
		return handler(_base, opt_cast().find(key_hash, key)); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const key_type& key, const Handler& handler) const { \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
		read_lock_type lk(_mutex); \
		return handler(_base, opt_cast().find(key_hash, key)); } \
	\
	template<typename CKey, typename CHash, typename CEq> \
	bool find(const CKey& key, const CHash& hash, const CEq& eq) const { \
		std::size_t key_hash = opt_type::s_hash(hash, key); \
		read_lock_type lk(_mutex); \
		return opt_cast().find(key_hash, key, eq) != _base.end(); } \
	\
	template<typename CKey, typename CHash, typename CEq, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const CKey& key, const CHash& hash, const CEq& eq, const Handler& handler) { \
		std::size_t key_hash = opt_type::s_hash(hash, key); \
		write_lock_type lk(_mutex); \
		return handler(_base, opt_cast().find(key_hash, key, eq)); } \
	\
	template<typename CKey, typename CHash, typename CEq, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const CKey& key, const CHash& hash, const CEq& eq, const Handler& handler) const { \
		std::size_t key_hash = opt_type::s_hash(hash, key); \
		read_lock_type lk(_mutex); \
		return handler(_base, opt_cast().find(key_hash, key, eq)); }

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION


namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_FIND_IMPL_HPP__