//extract_node_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_EXTRACT_NODE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_EXTRACT_NODE_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>

#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>

#define YGGR_PP_SAFE_CONTAINER_EXTRACT_NODE_IMPL() \
	protected: \
		template<typename BaseCont, typename Node, typename Nil = void> \
		struct node_extract_helper { \
		public: \
			typedef BaseCont base_cont_type; \
			typedef Node node_type; \
		public: \
			template<typename KeyArg> inline \
			node_type extract_of_key(base_cont_type& base_cont, BOOST_RV_REF(KeyArg) key) const { \
				return container::extract(base_cont, boost::move(key)); } \
			\
			template<typename KeyArg> inline \
			node_type extract_of_key(base_cont_type& base_cont, const KeyArg& key) const { \
				return container::extract(base_cont, key); } \
			\
			template<typename Iter> inline \
			node_type extract_of_pos(base_cont_type& base_cont, Iter iter) const { \
				return container::extract(base_cont, iter); } }; \
		\
		template<typename BaseCont, typename Nil> \
		struct node_extract_helper<BaseCont, mplex::null_type, Nil> { \
		public: \
			typedef BaseCont base_cont_type; \
			typedef mplex::null_type node_type; \
		private: \
			BOOST_MPL_ASSERT_MSG((false), current_base_container_not_support_node_extract, (base_cont_type)); \
		public: \
			template<typename KeyArg> inline \
			node_type extract_of_key(base_cont_type&, BOOST_RV_REF(KeyArg)) const { \
				return node_type(); } \
			\
			template<typename KeyArg> inline \
			node_type extract_of_key(base_cont_type&, const KeyArg&) const { \
				return node_type(); } \
			\
			template<typename Iter> inline \
			node_type extract_of_pos(base_cont_type&, Iter) const { \
				return node_type(); } }; \
	\
	public: \
		template<typename KeyArg> \
		typename \
			boost::disable_if< \
				boost::mpl::and_< \
					typename ::yggr::func::foo_t_info<KeyArg>::is_callable_type, \
					boost::mpl::or_< \
						boost::is_base_of< \
							iterator, \
							typename ::yggr::func::foo_t_info<KeyArg>::result_type >, \
						boost::is_base_of< \
							const_iterator, \
							typename ::yggr::func::foo_t_info<KeyArg>::result_type > > >, \
				node_type >::type \
			extract(BOOST_RV_REF(KeyArg) key) { \
			typedef node_extract_helper<base_type, node_type> helper_type; \
			helper_type helper; \
			write_lock_type lk(_mutex); \
			return helper.extract_of_key(_base, boost::move(key)); } \
		\
		template<typename KeyArg> \
		typename \
			boost::disable_if< \
				boost::mpl::and_< \
					typename ::yggr::func::foo_t_info<KeyArg>::is_callable_type, \
					boost::mpl::or_< \
						boost::is_base_of< \
							iterator, \
							typename ::yggr::func::foo_t_info<KeyArg>::result_type >, \
						boost::is_base_of< \
							const_iterator, \
							typename ::yggr::func::foo_t_info<KeyArg>::result_type > > >, \
				node_type >::type \
			extract(const KeyArg& key) { \
			typedef node_extract_helper<base_type, node_type> helper_type; \
			helper_type helper; \
			write_lock_type lk(_mutex); \
			return helper.extract_of_key(_base, key); } \
		\
		template<typename IterHandler> \
		typename \
			boost::enable_if< \
				boost::mpl::and_ < \
					typename ::yggr::func::foo_t_info<IterHandler>::is_callable_type, \
					boost::mpl::or_< \
						boost::is_base_of< \
							iterator, \
							typename ::yggr::func::foo_t_info<IterHandler>::result_type >, \
						boost::is_base_of< \
							const_iterator, \
							typename ::yggr::func::foo_t_info<IterHandler>::result_type > > >, \
				node_type >::type \
			extract(const IterHandler& pos_handler) { \
			typedef node_extract_helper<base_type, node_type> helper_type; \
			helper_type helper; \
			write_lock_type lk(_mutex); \
			return helper.extract_of_pos(_base, pos_handler(_base)); }

#define YGGR_PP_SAFE_CONTAINER_EXTRACT_NODE_OPT_IMPL() \
	public: \
		template<typename ArgKey> \
		typename \
			boost::disable_if< \
				boost::mpl::and_< \
					typename ::yggr::func::foo_t_info<ArgKey>::is_callable_type, \
					boost::mpl::or_< \
						boost::is_base_of< \
							iterator, \
							typename ::yggr::func::foo_t_info<ArgKey>::result_type >, \
						boost::is_base_of< \
							const_iterator, \
							typename ::yggr::func::foo_t_info<ArgKey>::result_type > > >, \
				node_type >::type \
			extract(BOOST_RV_REF(ArgKey) key) { \
			write_lock_type lk(_mutex); \
			return container::extract(_base, boost::move(key)); } \
		\
		template<typename ArgKey> \
		typename \
			boost::disable_if< \
				boost::mpl::and_< \
					typename ::yggr::func::foo_t_info<ArgKey>::is_callable_type, \
					boost::mpl::or_< \
						boost::is_base_of< \
							iterator, \
							typename ::yggr::func::foo_t_info<ArgKey>::result_type >, \
						boost::is_base_of< \
							const_iterator, \
							typename ::yggr::func::foo_t_info<ArgKey>::result_type > > >, \
				node_type >::type \
			extract(const ArgKey& key) { \
			write_lock_type lk(_mutex); \
			return container::extract(_base, key); } \
		\
		template<typename IterHandler> \
		typename \
			boost::enable_if< \
				boost::mpl::and_ < \
					typename ::yggr::func::foo_t_info<IterHandler>::is_callable_type, \
					boost::mpl::or_< \
						boost::is_base_of< \
							iterator, \
							typename ::yggr::func::foo_t_info<IterHandler>::result_type >, \
						boost::is_base_of< \
							const_iterator, \
							typename ::yggr::func::foo_t_info<IterHandler>::result_type > > >, \
				node_type >::type \
			extract(const IterHandler& pos_handler) { \
			write_lock_type lk(_mutex); \
			return container::extract(_base, pos_handler(_base)); }


#define YGGR_PP_SAFE_UKV_EXTRACT_NODE_OPT_IMPL() \
	public: \
		template<typename KeyArg> \
		typename \
			boost::disable_if< \
				boost::mpl::and_< \
					typename ::yggr::func::foo_t_info<KeyArg>::is_callable_type, \
					boost::mpl::or_< \
						boost::is_base_of< \
							iterator, \
							typename ::yggr::func::foo_t_info<KeyArg>::result_type >, \
						boost::is_base_of< \
							const_iterator, \
							typename ::yggr::func::foo_t_info<KeyArg>::result_type > > >, \
				node_type >::type \
			extract(BOOST_RV_REF(KeyArg) key) { \
			key_type now_key(boost::move(key)); \
			std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), now_key); \
			const_iterator iter; \
			write_lock_type lk(_mutex); \
			iter = opt_cast().find(key_hash, now_key); \
			return iter == _base.end()? node_type() : container::extract(_base, iter); } \
		\
		template<typename KeyArg> \
		typename \
			boost::disable_if< \
				boost::mpl::and_< \
					typename ::yggr::func::foo_t_info<KeyArg>::is_callable_type, \
					boost::mpl::or_< \
						boost::is_base_of< \
							iterator, \
							typename ::yggr::func::foo_t_info<KeyArg>::result_type >, \
						boost::is_base_of< \
							const_iterator, \
							typename ::yggr::func::foo_t_info<KeyArg>::result_type > > >, \
				node_type >::type \
			extract(const KeyArg& key) { \
			key_type now_key(key); \
			std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), now_key); \
			const_iterator iter; \
			write_lock_type lk(_mutex); \
			iter = opt_cast().find(key_hash, now_key); \
			return iter == _base.end()? node_type() : container::extract(_base, iter); } \
		\
		node_type extract(const key_type& key) { \
			std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
			const_iterator iter; \
			write_lock_type lk(_mutex); \
			iter = opt_cast().find(key_hash, key); \
			return iter == _base.end()? node_type() : container::extract(_base, iter); } \
		\
		template<typename IterHandler> \
		typename \
			boost::enable_if< \
				boost::mpl::and_ < \
					typename ::yggr::func::foo_t_info<IterHandler>::is_callable_type, \
					boost::mpl::or_< \
						boost::is_base_of< \
							iterator, \
							typename ::yggr::func::foo_t_info<IterHandler>::result_type >, \
						boost::is_base_of< \
							const_iterator, \
							typename ::yggr::func::foo_t_info<IterHandler>::result_type > > >, \
				node_type >::type \
			extract(const IterHandler& pos_handler) { \
			write_lock_type lk(_mutex); \
			return container::extract(_base, pos_handler(_base)); }


namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_EXTRACT_NODE_IMPL_HPP__
