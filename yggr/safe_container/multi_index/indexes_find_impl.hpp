// indexes_find_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_FIND_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_FIND_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/fusion/include/pair.hpp>

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{

template<typename Handler, typename Final>
struct is_find_back_handler
	: public
		boost::mpl::or_
		<
			typename 
				func::is_tag_object_reference
				<
					typename func::foo_t_info<Handler>::template arg_holder<0>::type
				>::type,
			typename
				boost::mpl::and_
				<
					typename 
						boost::mpl::not_
						<
							typename
								boost::is_same
								<
									typename func::foo_t_info<Handler>::template arg_holder<0>::type, 
									Final
								>::type
						>::type,
					typename 
						boost::is_same
						<
							typename
								boost::remove_reference
								<
									typename func::foo_t_info<Handler>::template arg_holder<0>::type
								>::type,
							Final
						>::type
				>::type
		>::type
{
};

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr

#define YGGR_SAFE_MIDX_INDEXES_FIND_IMPL() \
	template<typename CompatibleKey> \
	bool find(const CompatibleKey& key) const { \
		read_lock_type lk(_mutex); \
		return _base.find(key) != _base.end(); } \
	\
	template<typename CompatibleKey, typename Handler> \
	typename \
		boost::enable_if_c< \
			multi_index_detail::is_find_back_handler<Handler, final_type>::value, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		find(const CompatibleKey& key, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							iterator >(_base.find(key))); } \
	\
	template<typename CompatibleKey, typename Handler> \
	typename \
		boost::enable_if_c< \
			multi_index_detail::is_find_back_handler<Handler, final_type const >::value, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		find(const CompatibleKey& key, const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							const_iterator >(_base.find(key))); }


#define YGGR_SAFE_MIDX_INDEXES_GET_VALUE_IMPL() \
	template<typename CompatibleKey> \
	bool get_value(const CompatibleKey& key, value_type& val) const { \
		const_iterator iter; \
		read_lock_type lk(_mutex); \
		return ((iter = _base.find(key)) != _base.end()) && (val = *iter, true); }

// kv
#define YGGR_SAFE_MIDX_KV_INDEXES_FIND_IMPL() \
	YGGR_SAFE_MIDX_INDEXES_FIND_IMPL() \
	\
	template<typename CompatibleKey, \
				typename CompatiblePred > \
	typename \
		boost::disable_if_c< \
			multi_index_detail::is_find_back_handler<CompatiblePred, final_type const>::value, \
			bool >::type \
		find(const CompatibleKey& key, \
				const CompatiblePred& pred) const { \
		read_lock_type lk(_mutex); \
		return _base.find(key, pred) != _base.end(); } \
	\
	template<typename CompatibleKey, \
				typename CompatiblePred, \
				typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const CompatibleKey& key, \
				const CompatiblePred& pred, \
				const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							iterator >(_base.find(key, pred))); } \
	\
	template<typename CompatibleKey, \
				typename CompatiblePred, \
				typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const CompatibleKey& key, \
				const CompatiblePred& pred, \
				const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							const_iterator >(_base.find(key, pred))); }

#define YGGR_SAFE_MIDX_KV_INDEXES_GET_VALUE_IMPL() \
	YGGR_SAFE_MIDX_INDEXES_GET_VALUE_IMPL() \
	\
	template<typename CompatibleKey, typename CompatiblePred> \
	bool get_value(const CompatibleKey& key, \
					const CompatiblePred& pred, \
					value_type& val) const { \
		const_iterator iter; \
		read_lock_type lk(_mutex); \
		return ((iter = _base.find(key, pred)) != _base.end()) && (val = *iter, true); }


// ukv
#define YGGR_SAFE_MIDX_UKV_INDEXES_FIND_IMPL() \
	YGGR_SAFE_MIDX_INDEXES_FIND_IMPL() \
	\
	template<typename CompatibleKey, \
				typename CompatibleHash, \
				typename CompatiblePred > \
	bool find(const CompatibleKey& key, \
				const CompatibleHash& hash, \
				const CompatiblePred& eq) const { \
		read_lock_type lk(_mutex); \
		return _base.find(key, hash, eq) != _base.end(); } \
	\
	template<typename CompatibleKey, \
				typename CompatibleHash, \
				typename CompatiblePred, \
				typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const CompatibleKey& key, \
				const CompatibleHash& hash, \
				const CompatiblePred& eq, \
				const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							iterator >(_base.find(key, hash, eq))); } \
	\
	template<typename CompatibleKey, \
				typename CompatibleHash, \
				typename CompatiblePred, \
				typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		find(const CompatibleKey& key, \
				const CompatibleHash& hash, \
				const CompatiblePred& eq, \
				const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, \
						boost::fusion::pair< \
							index_id_type, \
							const_iterator >(_base.find(key, hash, eq))); }

#define YGGR_SAFE_MIDX_UKV_INDEXES_GET_VALUE_IMPL() \
	YGGR_SAFE_MIDX_INDEXES_GET_VALUE_IMPL() \
	\
	template<typename CompatibleKey, \
				typename CompatibleHash, \
				typename CompatiblePred > \
	bool get_value(const CompatibleKey& key, \
					const CompatibleHash& hash, \
					const CompatiblePred& eq, \
					value_type& val) const { \
		const_iterator iter; \
		read_lock_type lk(_mutex); \
		return ((iter = _base.find(key, hash, eq)) != _base.end()) && (val = *iter, true); } \


#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	define YGGR_SAFE_MIDX_UKV_OPT_INDEXES_FIND_IMPL() \
		template<typename CompatibleKey> \
		bool find(const CompatibleKey& key) const { \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			read_lock_type lk(_mutex); \
			return opt_cast().find(key_hash, key) != _base.end(); } \
		\
		template<typename CompatibleKey, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			find(const CompatibleKey& key, const Handler& handler) { \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			write_lock_type lk(_mutex); \
			return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								iterator >(opt_cast().find(key_hash, key))); } \
		\
		template<typename CompatibleKey, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			find(const CompatibleKey& key, const Handler& handler) const { \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			read_lock_type lk(_mutex); \
			return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								const_iterator >(opt_cast().find(key_hash, key))); } \
		\
		template<typename CompatibleKey, \
					typename CompatibleHash, \
					typename CompatiblePred > \
		bool find(const CompatibleKey& key, \
					const CompatibleHash& hash, \
					const CompatiblePred& eq) const { \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			read_lock_type lk(_mutex); \
			return opt_cast().find(key_hash, key, eq) != _base.end(); } \
		\
		template<typename CompatibleKey, \
					typename CompatibleHash, \
					typename CompatiblePred, \
					typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			find(const CompatibleKey& key, \
					const CompatibleHash& hash, \
					const CompatiblePred& eq, \
					const Handler& handler) { \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			write_lock_type lk(_mutex); \
			return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								iterator >(opt_cast().find(key_hash, key, eq))); } \
		\
		template<typename CompatibleKey, \
					typename CompatibleHash, \
					typename CompatiblePred, \
					typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			find(const CompatibleKey& key, \
					const CompatibleHash& hash, \
					const CompatiblePred& eq, \
					const Handler& handler) const { \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			read_lock_type lk(_mutex); \
			return handler(_final, \
							boost::fusion::pair< \
								index_id_type, \
								const_iterator >(opt_cast().find(key_hash, key, eq))); }

#	define YGGR_SAFE_MIDX_UKV_OPT_INDEXES_GET_VALUE_IMPL() \
		template<typename CompatibleKey> \
		bool get_value(const CompatibleKey& key, value_type& val) const { \
			const_iterator iter; \
			std::size_t key_hash = base_opt_type::s_hash(this_type::hash_function(), key); \
			read_lock_type lk(_mutex); \
			return \
				((iter = opt_cast().find(key_hash, key)) != _base.end()) \
				&& (val = *iter, true); } \
		\
		template<typename CompatibleKey, \
					typename CompatibleHash, \
					typename CompatiblePred > \
		bool get_value(const CompatibleKey& key, \
						const CompatibleHash& hash, \
						const CompatiblePred& eq, \
						value_type& val) const { \
			const_iterator iter; \
			std::size_t key_hash = base_opt_type::s_hash(hash, key); \
			read_lock_type lk(_mutex); \
			return \
				((iter = opt_cast().find(key_hash, key, eq)) != _base.end()) \
				&& (val = *iter, true); }

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

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_FIND_IMPL_HPP__
