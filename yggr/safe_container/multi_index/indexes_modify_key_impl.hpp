//indexes_modify_key_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_MODIFY_KEY_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_MODIFY_KEY_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>
#include <yggr/safe_container/multi_index/indexes_optimization_modifier.hpp>

#include <boost/utility/enable_if.hpp>

#define YGGR_PP_SAFE_MIDX_INDEXES_MODIFY_KEY_IMPL() \
	template<typename IterHandler, typename Modifier> \
	bool modify_key(const IterHandler& iter_handler, Modifier mod) { \
		write_lock_type lk(_mutex); \
		iterator iter = iter_handler(_base); \
		return iter == _base.end()? false : _base.modify_key(iter, mod); } \
	\
	template<typename IterHandler, typename Modifier, typename Rollback> \
	bool modify_key(const IterHandler& iter_handler, Modifier mod, Rollback back) { \
		write_lock_type lk(_mutex); \
		iterator iter = iter_handler(_base); \
		return iter == _base.end()? false : _base.modify_key(iter, mod, back); }

#define YGGR_PP_SAFE_MIDX_INDEXES_FIND_AND_MODIFY_KEY_IMPL() \
	template<typename CKey, typename Modifier> \
	bool find_and_modify_key(const CKey& key, Modifier mod) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key); \
		return iter == _base.end()? false : _base.modify_key(iter, mod); } \
	\
	template<typename CKey, typename Modifier, typename Rollback> \
	typename boost::disable_if_c< \
		func::foo_t_info<Modifier>::arg_holder_list_size == 2, \
		bool >::type \
		find_and_modify_key(const CKey& key, Modifier mod, Rollback back) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key); \
		return iter == _base.end()? false : _base.modify_key(iter, mod, back); }

#define YGGR_PP_SAFE_MIDX_KV_INDEXES_FIND_AND_MODIFY_KEY_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_FIND_AND_MODIFY_KEY_IMPL() \
	\
	template<typename CKey, typename CPred, \
				typename Modifier> \
	typename boost::enable_if_c< \
		func::foo_t_info<CPred>::arg_holder_list_size == 2, \
		bool >::type \
		find_and_modify_key(const CKey& key, const CPred& pred, \
								Modifier mod) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key, pred); \
		return iter == _base.end()? false : _base.modify_key(iter, mod); } \
	\
	template<typename CKey, typename CPred, \
				typename Modifier, typename Rollback> \
	bool find_and_modify_key(const CKey& key, const CPred& pred, \
								Modifier mod, Rollback back) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key, pred); \
		return iter == _base.end()? false : _base.modify_key(iter, mod, back); }

#define YGGR_PP_SAFE_MIDX_UKV_INDEXES_FIND_AND_MODIFY_KEY_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_FIND_AND_MODIFY_KEY_IMPL() \
	\
	template<typename CKey, typename CHash, typename CEq, \
				typename Modifier> \
	bool find_and_modify_key(const CKey& key, const CHash& hash, const CEq& eq, \
								Modifier mod) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key, hash, eq); \
		return iter == _base.end()? false : _base.modify_key(iter, mod); } \
	\
	template<typename CKey, typename CHash, typename CEq, \
				typename Modifier, typename Rollback> \
	bool find_and_modify_key(const CKey& key, const CHash& hash, const CEq& eq, \
								Modifier mod, Rollback back) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key, hash, eq); \
		return iter == _base.end()? false : _base.modify_key(iter, mod, back); }

//--------------opt version-------------------

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	define YGGR_PP_SAFE_MIDX_INDEXES_HA_OPT_MODIFY_KEY_IMPL() \
		template<typename IterHandler, typename Modifier> \
		bool modify_key(const IterHandler& iter_handler, Modifier mod) { \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			iterator iter = iter_handler(_base); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, mod); } \
		\
		template<typename IterHandler, typename Modifier, typename Rollback> \
		bool modify_key(const IterHandler& iter_handler, Modifier mod, Rollback back) { \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			iterator iter = iter_handler(_base); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, mod, back); } \
		\
		template<typename IterHandler, \
					typename KeyInfoHandler, typename ModifyHandler> \
		bool modify_key(const IterHandler& iter_handler, \
						optimization_modifier<KeyInfoHandler, ModifyHandler> mod) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params = mod.keyinfo_params(tmp); \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			iterator iter = iter_handler(_base); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, keyinfo_params, mod); } \
		\
		template<typename IterHandler, \
					typename KeyInfoHandler, typename ModifyHandler, \
					typename Rollback> \
		bool modify_key(const IterHandler& iter_handler, \
							optimization_modifier<KeyInfoHandler, ModifyHandler> mod, \
							Rollback back) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params = mod.keyinfo_params(tmp); \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			iterator iter = iter_handler(_base); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, keyinfo_params, mod, back); }

// kv

#	define YGGR_PP_SAFE_MIDX_KV_HA_OPT_INDEXES_FIND_AND_MODIFY_KEY_IMPL() \
		template<typename CKey, typename Modifier> \
		bool find_and_modify_key(const CKey& key, Modifier mod) { \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, mod); } \
		\
		template<typename CKey, typename Modifier, typename Rollback> \
		typename boost::disable_if_c< \
			func::foo_t_info<Modifier>::arg_holder_list_size == 2, \
			bool >::type \
			find_and_modify_key(const CKey& key, Modifier mod, Rollback back) { \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, mod, back); } \
		\
		template<typename CKey, typename CPred, \
					typename Modifier> \
		typename boost::enable_if_c< \
			func::foo_t_info<CPred>::arg_holder_list_size == 2, \
			bool >::type \
			find_and_modify_key(const CKey& key, const CPred& pred, \
									Modifier mod) { \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key, pred); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, mod); } \
		\
		template<typename CKey, typename CPred, \
					typename Modifier, typename Rollback> \
		bool find_and_modify_key(const CKey& key, const CPred& pred, \
									Modifier mod, Rollback back) { \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key, pred); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, mod, back); } \
		\
		template<typename CKey, typename KeyInfoHandler, typename ModifyHandler> \
		bool find_and_modify_key(const CKey& key, \
									optimization_modifier<KeyInfoHandler, ModifyHandler> mod) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params = mod.keyinfo_params(tmp); \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, keyinfo_params, mod); } \
		\
		template<typename CKey, typename KeyInfoHandler, typename ModifyHandler, typename Rollback> \
		bool find_and_modify_key(const CKey& key, \
									optimization_modifier<KeyInfoHandler, ModifyHandler> mod, \
									Rollback back) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params = mod.keyinfo_params(tmp); \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, keyinfo_params, mod, back); } \
		\
		template<typename CKey, typename CPred, \
					typename KeyInfoHandler, typename ModifyHandler> \
		bool find_and_modify_key(const CKey& key, const CPred& pred, \
									optimization_modifier<KeyInfoHandler, ModifyHandler> mod) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params = mod.keyinfo_params(tmp); \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key, pred); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, keyinfo_params, mod); } \
		\
		template<typename CKey, typename CPred, \
					typename KeyInfoHandler, typename ModifyHandler, \
					typename Rollback> \
		bool find_and_modify_key(const CKey& key, const CPred& pred, \
									optimization_modifier<KeyInfoHandler, ModifyHandler> mod, \
									Rollback back) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params = mod.keyinfo_params(tmp); \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key, pred); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, keyinfo_params, mod, back); }

// ukv

#	define YGGR_PP_SAFE_MIDX_UKV_HA_OPT_INDEXES_FIND_AND_MODIFY_KEY_IMPL() \
		template<typename CKey, typename Modifier> \
		bool find_and_modify_key(const CKey& key, Modifier mod) { \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			iterator iter = opt_cast().find(key_hash, key); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, mod); } \
		\
		template<typename CKey, typename Modifier, typename Rollback> \
		bool find_and_modify_key(const CKey& key, Modifier mod, Rollback back) { \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			iterator iter = opt_cast().find(key_hash, key); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, mod, back); } \
		\
		template<typename CKey, typename KeyInfoHandler, typename ModifyHandler> \
		bool find_and_modify_key(const CKey& key, \
									optimization_modifier<KeyInfoHandler, ModifyHandler> mod) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params = mod.keyinfo_params(tmp); \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			iterator iter = opt_cast().find(key_hash, key); \
			return iter == _base.end()? \
						false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
														iter, keyinfo_params, mod); } \
		\
		template<typename CKey, \
					typename KeyInfoHandler, typename ModifyHandler, \
					typename Rollback> \
		bool find_and_modify_key(const CKey& key, \
									optimization_modifier<KeyInfoHandler, ModifyHandler> mod, \
									Rollback back) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params = mod.keyinfo_params(tmp); \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			iterator iter = opt_cast().find(key_hash, key); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, keyinfo_params, mod, back); } \
		\
		template<typename CKey, typename CHash, typename CEq, \
					typename Modifier> \
		bool find_and_modify_key(const CKey& key, const CHash& hash, const CEq& eq, \
									Modifier mod) { \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			iterator iter = opt_cast().find(key_hash, key, eq); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
												iter, mod); } \
		\
		template<typename CKey, typename CHash, typename CEq, \
					typename Modifier, typename Rollback> \
		bool find_and_modify_key(const CKey& key, const CHash& hash, const CEq& eq, \
									Modifier mod, Rollback back) { \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			iterator iter = opt_cast().find(key_hash, key, eq); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, mod, back); } \
		\
		template<typename CKey, typename CHash, typename CEq, \
					typename KeyInfoHandler, typename ModifyHandler> \
		bool find_and_modify_key(const CKey& key, const CHash& hash, const CEq& eq, \
									optimization_modifier<KeyInfoHandler, ModifyHandler> mod) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params = mod.keyinfo_params(tmp); \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			iterator iter = opt_cast().find(key_hash, key, eq); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, keyinfo_params, mod); } \
		\
		template<typename CKey, typename CHash, typename CEq, \
					typename KeyInfoHandler, typename ModifyHandler, typename Rollback> \
		bool find_and_modify_key(const CKey& key, const CHash& hash, const CEq& eq, \
									optimization_modifier<KeyInfoHandler, ModifyHandler> mod, \
									Rollback back) { \
			allocator_type alloc = this_type::get_allocator(); \
			final_type tmp(alloc); \
			keyinfo_params_type keyinfo_params = mod.keyinfo_params(tmp); \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			destroy_holder_type holder(alloc); \
			write_lock_type lk(_mutex); \
			iterator iter = opt_cast().find(key_hash, key, eq); \
			return iter == _base.end()? \
					false : opt_cast().modify_key(detail::allocator_destroy_holder_ref(holder), \
													iter, keyinfo_params, mod, back); }

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr


#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_MODIFY_KEY_IMPL_HPP__
