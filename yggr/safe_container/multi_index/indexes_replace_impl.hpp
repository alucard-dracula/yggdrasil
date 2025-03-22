//indexes_replace_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_REPLACE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_REPLACE_IMPL_HPP__

#include <yggr/safe_container/multi_index/indexes_replace_impl.hpp>

#define YGGR_PP_SAFE_MIDX_INDEXES_REPLACE_IMPL() \
	template<typename IterHandler> \
	bool replace(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		iterator iter = iter_handler(_base); \
		return iter == _base.end()? false : _base.replace(iter, boost::move(val)); } \
	\
	template<typename IterHandler> \
	bool replace(const IterHandler& iter_handler, const value_type& val) { \
		write_lock_type lk(_mutex); \
		iterator iter = iter_handler(_base); \
		return iter == _base.end()? false : _base.replace(iter, val); }

#define YGGR_PP_SAFE_MIDX_INDEXES_FIND_AND_REPLACE_IMPL() \
	template<typename CKey> \
	bool find_and_replace(const CKey& key, BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key); \
		return iter == _base.end()? false : _base.replace(iter, boost::move(val)); } \
	\
	template<typename CKey> \
	bool find_and_replace(const CKey& key, const value_type& val) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key); \
		return iter == _base.end()? false : _base.replace(iter, val); }

// kv
#define YGGR_PP_SAFE_MIDX_KV_INDEXES_FIND_AND_REPLACE_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_FIND_AND_REPLACE_IMPL() \
	\
	template<typename CKey, typename CPred> \
	bool find_and_replace(const CKey& key, const CPred& pred, \
							BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key, pred); \
		return iter == _base.end()? false : _base.replace(iter, boost::move(val)); } \
	\
	template<typename CKey, typename CPred> \
	bool find_and_replace(const CKey& key, const CPred& pred, \
							const value_type& val) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key, pred); \
		return iter == _base.end()? false : _base.replace(iter, val); }

// ukv
#define YGGR_PP_SAFE_MIDX_UKV_INDEXES_FIND_AND_REPLACE_IMPL() \
	YGGR_PP_SAFE_MIDX_INDEXES_FIND_AND_REPLACE_IMPL() \
	\
	template<typename CKey, typename CHash, typename CEq> \
	bool find_and_replace(const CKey& key, const CHash& hash, const CEq& eq, \
							BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key, hash, eq); \
		return iter == _base.end()? false : _base.replace(iter, boost::move(val)); } \
	\
	template<typename CKey, typename CHash, typename CEq> \
	bool find_and_replace(const CKey& key, const CHash& hash, const CEq& eq, \
							const value_type& val) { \
		write_lock_type lk(_mutex); \
		iterator iter = _base.find(key, hash, eq); \
		return iter == _base.end()? false : _base.replace(iter, val); }

//--------------opt version-------------------

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	define YGGR_PP_SAFE_MIDX_INDEXES_OPT_REPLACE_IMPL() \
		template<typename IterHandler> \
		bool replace(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			write_lock_type lk(_mutex); \
			iterator iter = iter_handler(_base); \
			return iter == _base.end()? false : opt_cast().replace(iter, keyinfo_params, boost::move(val)); } \
		\
		template<typename IterHandler> \
		bool replace(const IterHandler& iter_handler, const value_type& val) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			write_lock_type lk(_mutex); \
			iterator iter = iter_handler(_base); \
			return iter == _base.end()? false : opt_cast().replace(iter, keyinfo_params, val); }

//kv

#	define YGGR_PP_SAFE_MIDX_INDEXES_KV_OPT_FIND_AND_REPLACE_IMPL() \
		template<typename CKey> \
		bool find_and_replace(const CKey& key, BOOST_RV_REF(value_type) val) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key); \
			return iter == _base.end()? false : opt_cast().replace(iter, keyinfo_params, boost::move(val)); } \
		\
		template<typename CKey> \
		bool find_and_replace(const CKey& key, const value_type& val) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key); \
			return iter == _base.end()? false : opt_cast().replace(iter, keyinfo_params, val); } \
		\
		template<typename CKey, typename CPred> \
		bool find_and_replace(const CKey& key, const CPred& pred, \
								BOOST_RV_REF(value_type) val) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key, pred); \
			return iter == _base.end()? false : opt_cast().replace(iter, keyinfo_params, boost::move(val)); } \
		\
		template<typename CKey, typename CPred> \
		bool find_and_replace(const CKey& key, const CPred& pred, \
								const value_type& val) { \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key, pred); \
			return iter == _base.end()? false : opt_cast().replace(iter, keyinfo_params, val); }

// ukv
#	define YGGR_PP_SAFE_MIDX_INDEXES_UKV_OPT_FIND_AND_REPLACE_IMPL() \
		template<typename CKey> \
		bool find_and_replace(const CKey& key, BOOST_RV_REF(value_type) val) { \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			write_lock_type lk(_mutex); \
			base_opt_type& opt = opt_cast(); \
			iterator iter = opt.find(key_hash, key); \
			return iter == _base.end()? false : opt.replace(iter, keyinfo_params, boost::move(val)); } \
		\
		template<typename CKey> \
		bool find_and_replace(const CKey& key, const value_type& val) { \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			write_lock_type lk(_mutex); \
			base_opt_type& opt = opt_cast(); \
			iterator iter = opt.find(key_hash, key); \
			return iter == _base.end()? false : opt.replace(iter, keyinfo_params, val); } \
		\
		template<typename CKey, typename CHash, typename CEq> \
		bool find_and_replace(const CKey& key, const CHash& hash, const CEq& eq, \
								BOOST_RV_REF(value_type) val) { \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			write_lock_type lk(_mutex); \
			base_opt_type& opt = opt_cast(); \
			iterator iter = opt.find(key_hash, key, eq); \
			return iter == _base.end()? false : opt.replace(iter, keyinfo_params, boost::move(val)); } \
		\
		template<typename CKey, typename CHash, typename CEq> \
		bool find_and_replace(const CKey& key, const CHash& hash, const CEq& eq, \
								const value_type& val) { \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			final_type tmp(this_type::get_allocator()); \
			keyinfo_params_type keyinfo_params \
				= final_opt_type::s_gen_opt_keyinfo_params(tmp, val); \
			write_lock_type lk(_mutex); \
			base_opt_type& opt = opt_cast(); \
			iterator iter = opt.find(key_hash, key, eq); \
			return iter == _base.end()? false : opt.replace(iter, keyinfo_params, val); }	

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

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_REPLACE_IMPL_HPP__
