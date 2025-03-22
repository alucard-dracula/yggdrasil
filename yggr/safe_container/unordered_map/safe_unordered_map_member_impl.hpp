// safe_unordered_map_member_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_UNORDERED_MAP_SAFE_UNORDERED_MAP_MEMBER_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_UNORDERED_MAP_SAFE_UNORDERED_MAP_MEMBER_IMPL_HPP__

#ifndef __YGGR_SAFE_CONTAINER_SAFE_UNORDERED_MAP_HPP__
#	error "this file is impl file, can't be used alone"
#endif // __YGGR_SAFE_CONTAINER_SAFE_UNORDERED_MAP_HPP__

#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/container/unordered_map.hpp>
#include <yggr/safe_container/detail/safe_construct_impl.hpp>
#include <yggr/safe_container/detail/merge_kv_helper.hpp>
#include <yggr/safe_container/detail/safe_kv_map_subscript_and_at_impl.hpp>

#include <boost/ref.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/utility/enable_if.hpp>

//constructor
#define YGGR_PP_SAFE_UNORDERED_MAP_CONSTRUCTOR_IMPL() \
	YGGR_PP_SAFE_UKV_CONSTRUCTOR_IMPL(safe_unordered_map)

#define YGGR_PP_SAFE_UNORDERED_MAP_ISOMERIC_SAME_CONSTRUCTOR_IMPL( __base_tpl__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_CONSTRUCTOR_IMPL( safe_unordered_map, 6, __base_tpl__ )

#define YGGR_PP_SAFE_UNORDERED_MULTIMAP_CONSTRUCTOR_IMPL() \
	YGGR_PP_SAFE_UKV_CONSTRUCTOR_IMPL(safe_unordered_multimap)

#define YGGR_PP_SAFE_UNORDERED_MULTIMAP_ISOMERIC_SAME_CONSTRUCTOR_IMPL( __base_tpl__ ) \
	YGGR_PP_SAFE_UKV_ISOMERIC_SAME_CONSTRUCTOR_IMPL( safe_unordered_multimap, 6, __base_tpl__ )

//destructor
#define YGGR_PP_SAFE_UNORDERED_MAP_DESTRUCTOR_IMPL() \
	YGGR_PP_SAFE_UKV_DESTRUCTOR_IMPL(safe_unordered_map)

#define YGGR_PP_SAFE_UNORDERED_MULTIMAP_DESTRUCTOR_IMPL() \
	YGGR_PP_SAFE_UKV_DESTRUCTOR_IMPL(safe_unordered_multimap)

// operator[] at
#define YGGR_PP_SAFE_UNORDERED_MAP_SUBSCRIPT_AND_AT_IMPL() \
	YGGR_PP_SAFE_KV_MAP_SUBSCRIPT_AND_AT_IMPL(safe_unordered_map)

#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#	define YGGR_PP_SAFE_UNORDERED_MAP_OPT_SUBSCRIPT_AND_AT_IMPL() \
		private: \
			inline static mapped_type& unlock_index(base_type& base, std::size_t key_hash, const key_type& key) { \
				return (reinterpret_cast<opt_type&>(base)).subscript(key_hash, key); } \
			\
			mapped_type unsafe_get_index(const key_type& key) { \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				write_lock_type lk(_mutex); \
				return opt_cast().subscript(key_hash, key); } \
			\
			template<typename Handler> \
			typename ::yggr::func::foo_t_info<Handler>::result_type \
				unsafe_set_index(const key_type& key, const Handler& handler) { \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				write_lock_type lk(_mutex); \
				return handler(opt_cast().subscript(key_hash, key)); } \
			\
			inline static mapped_type& unlock_at(base_type& base, std::size_t key_hash, const key_type& key) { \
				iterator iter = (reinterpret_cast<opt_type&>(base)).find(key_hash, key); \
				if(iter == base.end()) { \
					throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
				return iter->second; } \
			\
			mapped_type unsafe_get_at(const key_type& key) const { \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				read_lock_type lk(_mutex); \
				const_iterator iter = opt_cast().find(key_hash, key); \
				if(iter == _base.end()) { \
					throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
				return iter->second; } \
			\
			template<typename Handler> \
			typename ::yggr::func::foo_t_info<Handler>::result_type \
				unsafe_set_at(const key_type& key, const Handler& handler) { \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				write_lock_type lk(_mutex); \
				iterator iter = opt_cast().find(key_hash, key); \
				if(iter == _base.end()) { \
					throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
				return handler(iter->second); } \
			\
			class bridge_at; \
			class bridge_index; \
			\
			friend class bridge_at; \
			friend class bridge_index; \
			\
			class bridge_at \
				: public detail::basic_bridge<safe_unordered_map> { \
			private: \
				typedef detail::basic_bridge<safe_unordered_map> bridge_base_type; \
			public: \
				typedef typename bridge_base_type::container_type container_type; \
				typedef typename bridge_base_type::base_container_type base_container_type; \
				typedef typename bridge_base_type::value_type value_type; \
			private: \
				typedef bridge_at this_type; \
			public: \
				bridge_at(container_type& cont, \
							std::size_t key_hash, \
							BOOST_RV_REF(key_type) key) \
					: bridge_base_type(cont), _key_hash(key_hash), _key(key) {} \
				\
				bridge_at(container_type& cont, \
							std::size_t key_hash, \
							const key_type& key) \
					: bridge_base_type(cont), _key_hash(key_hash), _key(key) {} \
				\
				bridge_at(const bridge_at& right) \
					: bridge_base_type(right), _key_hash(right._key_hash), _key(right._key) {} \
				\
				~bridge_at(void) {} \
				\
			public: \
				inline const key_type& key(void) const { return _key; } \
				\
				inline std::size_t key_hash(void) const { return _key_hash; } \
				\
				inline bool operator==(const bridge_at& right) const { \
					return \
						(this == boost::addressof(right)) \
						|| (bridge_base_type::compare_eq(right) \
							&& _key_hash == right._key_hash); } \
				\
				template<typename OtherBridge> inline \
				bool operator==(const OtherBridge&) const { \
					return false; } \
				\
				template<typename OtherBridge> inline \
				bool operator!=(const OtherBridge& right) const { \
					return !this_type::operator==(right); } \
			\
			public: \
				inline mapped_type operator()(void) const { \
					const container_type& cont = bridge_base_type::_wrap_cont; \
					return cont.unsafe_get_at(_key); } \
				\
				template<typename Handler> inline \
				typename ::yggr::func::foo_t_info<Handler>::result_type \
					operator()(const Handler& handler) const { \
					container_type& cont = bridge_base_type::_wrap_cont; \
					return cont.unsafe_set_at(_key, handler); } \
				\
			public: \
				inline boost::function1<mapped_type&, base_container_type&> \
					native_handler(void) const { \
					return boost::bind(&container_type::unlock_at, _1, _key_hash, boost::cref(_key)); } \
				\
				template<typename Op_Handler, typename OtherBridge> inline \
				boost::function1< \
					typename ::yggr::func::foo_t_info<Op_Handler>::result_type, \
					base_container_type& > \
					link(const Op_Handler& op_handler, const OtherBridge& other_bridge) const { \
					return bridge_base_type::pro_s_link_handler(op_handler, \
																this_type::native_handler(), \
																other_bridge.native_handler()); } \
				\
				template<typename Op_Handler, typename Handler> inline \
				boost::function1< \
					typename ::yggr::func::foo_t_info<Op_Handler>::result_type, \
					base_container_type& > \
					link_handler(const Op_Handler& op_handler, const Handler& handler) const { \
					return bridge_base_type::pro_s_link_handler(op_handler, this_type::native_handler(), handler); } \
			\
			private: \
				const std::size_t _key_hash; \
				const key_type _key; }; \
			\
			class bridge_index \
				: public detail::basic_bridge<safe_unordered_map> { \
			private: \
				typedef detail::basic_bridge<safe_unordered_map> bridge_base_type; \
			public: \
				typedef typename bridge_base_type::container_type container_type; \
				typedef typename bridge_base_type::base_container_type base_container_type; \
				typedef typename bridge_base_type::value_type value_type; \
			private: \
				typedef bridge_index this_type; \
			public: \
				bridge_index(container_type& cont, \
								std::size_t key_hash, \
								BOOST_RV_REF(key_type) key) \
					: bridge_base_type(cont), _key_hash(key_hash), _key(key) {} \
				\
				bridge_index(container_type& cont, \
								std::size_t key_hash, \
								const key_type& key) \
					: bridge_base_type(cont), _key_hash(key_hash), _key(key) {} \
				\
				bridge_index(const bridge_index& right) \
					: bridge_base_type(right), _key_hash(right._key_hash), _key(right._key) {} \
				\
				~bridge_index(void) {} \
				\
			public: \
				inline const key_type& key(void) const { return _key; } \
				\
				inline std::size_t key_hash(void) const { return _key_hash; } \
				\
				inline bool operator==(const bridge_index& right) const { \
					return \
						(this == boost::addressof(right)) \
						|| (bridge_base_type::compare_eq(right) \
							&& _key_hash == right._key_hash); } \
				\
				template<typename OtherBridge> inline \
				bool operator==(const OtherBridge&) const { \
					return false; } \
				\
				template<typename OtherBridge> inline \
				bool operator!=(const OtherBridge& right) const { \
					return !this_type::operator==(right); } \
			public: \
				mapped_type operator()(void) const { \
					container_type& cont = bridge_base_type::_wrap_cont; \
					return cont.unsafe_get_index(_key); } \
				\
				template<typename Handler> inline \
				typename ::yggr::func::foo_t_info<Handler>::result_type \
					operator()(const Handler& handler) const { \
					container_type& cont = bridge_base_type::_wrap_cont; \
					return cont.unsafe_set_index(_key, handler); } \
				\
			public: \
				inline boost::function1<mapped_type&, base_container_type&> \
					native_handler(void) const { \
					return boost::bind(&container_type::unlock_index, _1, _key_hash, boost::cref(_key)); } \
				\
				template<typename Op_Handler, typename OtherBridge> inline \
				boost::function1< \
					typename ::yggr::func::foo_t_info<Op_Handler>::result_type, \
					base_container_type& > \
					link(const Op_Handler& op_handler, const OtherBridge& other_bridge) const { \
					return bridge_base_type::pro_s_link_handler(op_handler, \
																this_type::native_handler(), \
																other_bridge.native_handler()); } \
				\
				template<typename Op_Handler, typename Handler> inline \
				boost::function1< \
					typename ::yggr::func::foo_t_info<Op_Handler>::result_type, \
					base_container_type& > \
					link_handler(const Op_Handler& op_handler, const Handler& handler) const { \
					return bridge_base_type::pro_s_link_handler(op_handler, this_type::native_handler(), handler); } \
			private: \
				const std::size_t _key_hash; \
				const key_type _key; }; \
		\
		public: \
			inline detail::safe_element_reference<bridge_index> \
				operator[](BOOST_RV_REF(key_type) key) { \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				return detail::safe_element_reference<bridge_index>( \
							bridge_index(*this, key_hash, boost::move(key))); } \
			\
			inline detail::safe_element_reference<bridge_index> \
				operator[](const key_type& key) { \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				return detail::safe_element_reference<bridge_index>(bridge_index(*this, key_hash, key)); } \
			\
			inline detail::safe_element_reference<bridge_at> \
				at(const key_type& key) { \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				return detail::safe_element_reference<bridge_at>(bridge_at(*this, key_hash, key)); } \
			\
			inline mapped_type at(const key_type& key) const { \
				return this_type::unsafe_get_at(key); } \
			\
			bool get_value(const key_type& key, mapped_type& val) const { \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				read_lock_type lk(_mutex); \
				const_iterator iter = opt_cast().find(key_hash, key); \
				if(iter == _base.end()) { return false; } \
				val = iter->second; \
				return true; } \
			\
			bool set_value(const key_type& key, BOOST_RV_REF(mapped_type) val) { \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				write_lock_type lk(_mutex); \
				copy_or_move_or_swap(opt_cast().subscript(key_hash, key), boost::move(val)); \
				return true; } \
			\
			bool set_value(const key_type& key, const mapped_type& val) { \
				mapped_type tmp(val); \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				write_lock_type lk(_mutex); \
				copy_or_move_or_swap(opt_cast().subscript(key_hash, key), boost::move(tmp)); \
				return true; } \
			\
			inline bool set_value(BOOST_RV_REF(value_type) val) { \
				return this_type::set_value(val.first, boost::move(val.second)); } \
			\
			inline bool set_value(const value_type& val) { \
				return this_type::set_value(val.first, val.second); } \
			\
			inline bool get_at(const key_type& key, mapped_type& val) const { \
				return this_type::get_value(key, val); } \
			\
			bool set_at(const key_type& key, BOOST_RV_REF(mapped_type) val) { \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				write_lock_type lk(_mutex); \
				iterator iter = opt_cast().find(key_hash, key); \
				if(iter == _base.end()) { return false; } \
				copy_or_move_or_swap(iter->second, boost::move(val)); \
				return true; } \
			\
			bool set_at(const key_type& key, const mapped_type& val) { \
				mapped_type tmp(val); \
				std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), key); \
				write_lock_type lk(_mutex); \
				iterator iter = opt_cast().find(key_hash, key); \
				if(iter == _base.end()) { return false; } \
				copy_or_move_or_swap(iter->second, boost::move(tmp)); \
				return true; } \
			\
			inline bool set_at(BOOST_RV_REF(value_type) val) { \
				return this_type::set_at(val.first, boost::move(val.second)); } \
			\
			inline bool set_at(const value_type& val) { \
				return this_type::set_at(val.first, val.second); }

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

// ukv unique
#define YGGR_PP_SAFE_UMAP_OPT_INSERT_NODE_FUNCTION_IMPL() \
	insert_return_type insert(BOOST_RV_REF(node_type) node) { \
		if(!node) { return insert_return_type(); } \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.key()); \
		write_lock_type lk(_mutex); \
		try { return insert_return_type(opt_cast().insert(key_hash, boost::move(node))); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return insert_return_type(); } } \
	\
	template<typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
		base_insert_return_type base_iret; \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.key()); \
		write_lock_type lk(_mutex); \
		if(node) { \
			try { base_iret = opt_cast().insert(key_hash, boost::move(node)); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())); } } \
		return ret_handler(_base, base_iret); } \
	\
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node) { \
		if(!node) { return false; } \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.key()); \
		write_lock_type lk(_mutex); \
		try { return opt_cast().insert(key_hash, iter_handler(_base), boost::move(node)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename IterHandler, typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.key()); \
		write_lock_type lk(_mutex); \
		if(node) { \
			try { ret.first = opt_cast().insert(key_hash, iter_handler(_base), boost::move(node)); \
					ret.second = ret.first != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } } \
		return ret_handler(_base, ret); }

//#define YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL() \
//	template<typename K, typename M> \
//	bool insert_or_assign(BOOST_RV_REF(K) k, BOOST_RV_REF(M) m) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		allocator_type alloc = this_type::get_allocator(); \
//		destroy_holder_type holder(alloc); \
//		std::size_t key_hash = 0; \
//		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, boost::move(k), boost::move(m)); \
//		if(!pnode) { return false; } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), key_hash, pnode); \
//		if(!ret.second) { ret.first->second = boost::move(pnode->value().second); } \
//		return ret.second; } \
//	\
//	template<typename K, typename M> \
//	bool insert_or_assign(const K& k, BOOST_RV_REF(M) m) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		allocator_type alloc = this_type::get_allocator(); \
//		destroy_holder_type holder(alloc); \
//		std::size_t key_hash = 0; \
//		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, k, boost::move(m)); \
//		if(!pnode) { return false; } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), key_hash, pnode); \
//		if(!ret.second) { ret.first->second = boost::move(pnode->value().second); } \
//		return ret.second; } \
//	\
//	template<typename K, typename M, typename RetHandler> \
//	typename \
//		boost::disable_if< \
//			boost::mpl::and_ < \
//				typename ::yggr::func::foo_t_info<K>::is_callable_type, \
//				boost::mpl::or_< \
//					boost::is_base_of< \
//						iterator, \
//						typename ::yggr::func::foo_t_info<K>::result_type >, \
//					boost::is_base_of< \
//						const_iterator, \
//						typename ::yggr::func::foo_t_info<K>::result_type > > >, \
//			typename ::yggr::func::foo_t_info<RetHandler>::result_type >::type \
//		insert_or_assign(BOOST_RV_REF(K) k, BOOST_RV_REF(M) m, const RetHandler& ret_handler) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		allocator_type alloc = this_type::get_allocator(); \
//		destroy_holder_type holder(alloc); \
//		std::size_t key_hash = 0; \
//		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, boost::move(k), boost::move(m)); \
//		if(!pnode) { return ret_handler(_base, ret); } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), key_hash, pnode); \
//		if(!ret.second){ ret.first->second = boost::move(pnode->value().second); } \
//		return ret_handler(_base, ret); } \
//	\
//	template<typename K, typename M, typename RetHandler> \
//	typename \
//		boost::disable_if< \
//			boost::mpl::and_ < \
//				typename ::yggr::func::foo_t_info<K>::is_callable_type, \
//				boost::mpl::or_< \
//					boost::is_base_of< \
//						iterator, \
//						typename ::yggr::func::foo_t_info<K>::result_type >, \
//					boost::is_base_of< \
//						const_iterator, \
//						typename ::yggr::func::foo_t_info<K>::result_type > > >, \
//			typename ::yggr::func::foo_t_info<RetHandler>::result_type >::type \
//		insert_or_assign(const K& k, BOOST_RV_REF(M) m, const RetHandler& ret_handler) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		allocator_type alloc = this_type::get_allocator(); \
//		destroy_holder_type holder(alloc); \
//		std::size_t key_hash = 0; \
//		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, k, boost::move(m)); \
//		if(!pnode) { return ret_handler(_base, ret); } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), key_hash, pnode); \
//		if(!ret.second) { ret.first->second = boost::move(pnode->value().second); } \
//		return ret_handler(_base, ret); } \
//	\
//	template<typename IterHandler, typename K, typename M> \
//	typename \
//		boost::enable_if< \
//			boost::mpl::and_ < \
//				typename ::yggr::func::foo_t_info<IterHandler>::is_callable_type, \
//				boost::mpl::or_< \
//					boost::is_base_of< \
//						iterator, \
//						typename ::yggr::func::foo_t_info<IterHandler>::result_type >, \
//					boost::is_base_of< \
//						const_iterator, \
//						typename ::yggr::func::foo_t_info<IterHandler>::result_type > > >, \
//			bool >::type \
//		insert_or_assign(const IterHandler& iter_handler, BOOST_RV_REF(K) k, BOOST_RV_REF(M) m) { \
//		iterator ret; \
//		allocator_type alloc = this_type::get_allocator(); \
//		destroy_holder_type holder(alloc); \
//		std::size_t key_hash = 0; \
//		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, boost::move(k), boost::move(m)); \
//		if(!pnode) { return false; } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), key_hash, pnode); \
//		if(holder.size()) { ret->second = boost::move(pnode->value().second); } \
//		return ret != _base.end(); } \
//	\
//	template<typename IterHandler, typename K, typename M> \
//	typename \
//		boost::enable_if< \
//			boost::mpl::and_ < \
//				typename ::yggr::func::foo_t_info<IterHandler>::is_callable_type, \
//				boost::mpl::or_< \
//					boost::is_base_of< \
//						iterator, \
//						typename ::yggr::func::foo_t_info<IterHandler>::result_type >, \
//					boost::is_base_of< \
//						const_iterator, \
//						typename ::yggr::func::foo_t_info<IterHandler>::result_type > > >, \
//			bool >::type \
//		insert_or_assign(const IterHandler& iter_handler, const K& k, BOOST_RV_REF(M) m) { \
//		iterator ret; \
//		allocator_type alloc = this_type::get_allocator(); \
//		destroy_holder_type holder(alloc); \
//		assert(!holder.size()); \
//		std::size_t key_hash = 0; \
//		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, k, boost::move(m)); \
//		if(!pnode) { return false; } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), key_hash, pnode); \
//		if(holder.size()) { ret->second = boost::move(pnode->value().second); } \
//		return ret != _base.end(); } \
//	\
//	template<typename IterHandler, typename K, typename M, typename RetHandler> \
//	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
//		insert_or_assign(const IterHandler& iter_handler, \
//							BOOST_RV_REF(K) k, BOOST_RV_REF(M) m, const RetHandler& ret_handler) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		allocator_type alloc = this_type::get_allocator(); \
//		destroy_holder_type holder(alloc); \
//		assert(!holder.size()); \
//		std::size_t key_hash = 0; \
//		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, boost::move(k), boost::move(m)); \
//		if(!pnode) { return ret_handler(_base, ret); } \
//		write_lock_type lk(_mutex); \
//		ret.first = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), key_hash, pnode); \
//		if(holder.size()) { ret.first->second = boost::move(pnode->value().second); } \
//		ret.second = ret.first != _base.end(); \
//		return ret_handler(_base, ret); } \
//	\
//	template<typename IterHandler, typename K, typename M, typename RetHandler> \
//	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
//		insert_or_assign(const IterHandler& iter_handler, \
//							const K& k, BOOST_RV_REF(M) m, const RetHandler& ret_handler) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		allocator_type alloc = this_type::get_allocator(); \
//		destroy_holder_type holder(alloc); \
//		assert(!holder.size()); \
//		std::size_t key_hash = 0; \
//		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, k, boost::move(m)); \
//		if(!pnode) { return ret_handler(_base, ret); } \
//		write_lock_type lk(_mutex); \
//		ret.first = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), key_hash, pnode); \
//		if(holder.size()) { ret.first->second = boost::move(pnode->value().second); } \
//		ret.second = ret.first != _base.end(); \
//		return ret_handler(_base, ret); }

#define YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL() \
	template<typename K, typename M> \
	bool insert_or_assign(BOOST_RV_REF(K) k, BOOST_FWD_REF(M) m) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		allocator_type alloc = this_type::get_allocator(); \
		destroy_holder_type holder(alloc); \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, boost::move(k), boost::forward<M>(m)); \
		if(!pnode) { return false; } \
		write_lock_type lk(_mutex); \
		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), key_hash, pnode); \
		if(!ret.second) { ret.first->second = boost::move(pnode->value().second); } \
		return ret.second; } \
	\
	template<typename K, typename M> \
	bool insert_or_assign(const K& k, BOOST_FWD_REF(M) m) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		allocator_type alloc = this_type::get_allocator(); \
		destroy_holder_type holder(alloc); \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, k, boost::forward<M>(m)); \
		if(!pnode) { return false; } \
		write_lock_type lk(_mutex); \
		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), key_hash, pnode); \
		if(!ret.second) { ret.first->second = boost::move(pnode->value().second); } \
		return ret.second; } \
	\
	template<typename K, typename M, typename RetHandler> \
	typename \
		boost::disable_if< \
			boost::mpl::and_ < \
				typename ::yggr::func::foo_t_info<K>::is_callable_type, \
				boost::mpl::or_< \
					boost::is_base_of< \
						iterator, \
						typename ::yggr::func::foo_t_info<K>::result_type >, \
					boost::is_base_of< \
						const_iterator, \
						typename ::yggr::func::foo_t_info<K>::result_type > > >, \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type >::type \
		insert_or_assign(BOOST_RV_REF(K) k, BOOST_FWD_REF(M) m, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		allocator_type alloc = this_type::get_allocator(); \
		destroy_holder_type holder(alloc); \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, boost::move(k), boost::forward<M>(m)); \
		if(!pnode) { return ret_handler(_base, ret); } \
		write_lock_type lk(_mutex); \
		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), key_hash, pnode); \
		if(!ret.second){ ret.first->second = boost::move(pnode->value().second); } \
		return ret_handler(_base, ret); } \
	\
	template<typename K, typename M, typename RetHandler> \
	typename \
		boost::disable_if< \
			boost::mpl::and_ < \
				typename ::yggr::func::foo_t_info<K>::is_callable_type, \
				boost::mpl::or_< \
					boost::is_base_of< \
						iterator, \
						typename ::yggr::func::foo_t_info<K>::result_type >, \
					boost::is_base_of< \
						const_iterator, \
						typename ::yggr::func::foo_t_info<K>::result_type > > >, \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type >::type \
		insert_or_assign(const K& k, BOOST_FWD_REF(M) m, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		allocator_type alloc = this_type::get_allocator(); \
		destroy_holder_type holder(alloc); \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, k, boost::forward<M>(m)); \
		if(!pnode) { return ret_handler(_base, ret); } \
		write_lock_type lk(_mutex); \
		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), key_hash, pnode); \
		if(!ret.second) { ret.first->second = boost::move(pnode->value().second); } \
		return ret_handler(_base, ret); } \
	\
	template<typename IterHandler, typename K, typename M> \
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
			bool >::type \
		insert_or_assign(const IterHandler& iter_handler, BOOST_RV_REF(K) k, BOOST_FWD_REF(M) m) { \
		iterator ret; \
		allocator_type alloc = this_type::get_allocator(); \
		destroy_holder_type holder(alloc); \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, boost::move(k), boost::forward<M>(m)); \
		if(!pnode) { return false; } \
		write_lock_type lk(_mutex); \
		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), key_hash, pnode); \
		if(holder.size()) { ret->second = boost::move(pnode->value().second); } \
		return ret != _base.end(); } \
	\
	template<typename IterHandler, typename K, typename M> \
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
			bool >::type \
		insert_or_assign(const IterHandler& iter_handler, const K& k, BOOST_FWD_REF(M) m) { \
		iterator ret; \
		allocator_type alloc = this_type::get_allocator(); \
		destroy_holder_type holder(alloc); \
		assert(!holder.size()); \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, k, boost::forward<M>(m)); \
		if(!pnode) { return false; } \
		write_lock_type lk(_mutex); \
		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), key_hash, pnode); \
		if(holder.size()) { ret->second = boost::move(pnode->value().second); } \
		return ret != _base.end(); } \
	\
	template<typename IterHandler, typename K, typename M, typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert_or_assign(const IterHandler& iter_handler, \
							BOOST_RV_REF(K) k, BOOST_FWD_REF(M) m, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		allocator_type alloc = this_type::get_allocator(); \
		destroy_holder_type holder(alloc); \
		assert(!holder.size()); \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, boost::move(k), boost::forward<M>(m)); \
		if(!pnode) { return ret_handler(_base, ret); } \
		write_lock_type lk(_mutex); \
		ret.first = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), key_hash, pnode); \
		if(holder.size()) { ret.first->second = boost::move(pnode->value().second); } \
		ret.second = ret.first != _base.end(); \
		return ret_handler(_base, ret); } \
	\
	template<typename IterHandler, typename K, typename M, typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert_or_assign(const IterHandler& iter_handler, \
							const K& k, BOOST_FWD_REF(M) m, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		allocator_type alloc = this_type::get_allocator(); \
		destroy_holder_type holder(alloc); \
		assert(!holder.size()); \
		std::size_t key_hash = 0; \
		node_ptr_type pnode = this_type::create_node_alloc(key_hash, alloc, k, boost::forward<M>(m)); \
		if(!pnode) { return ret_handler(_base, ret); } \
		write_lock_type lk(_mutex); \
		ret.first = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), key_hash, pnode); \
		if(holder.size()) { ret.first->second = boost::move(pnode->value().second); } \
		ret.second = ret.first != _base.end(); \
		return ret_handler(_base, ret); }

// ukv_non_unique
#define YGGR_PP_SAFE_UMULTIMAP_OPT_INSERT_NODE_FUNCTION_IMPL() \
	bool insert(BOOST_RV_REF(node_type) node) { \
		if(!node) { return false; } \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.key()); \
		write_lock_type lk(_mutex); \
		try { return opt_cast().insert(key_hash, boost::move(node)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.key()); \
		write_lock_type lk(_mutex); \
		if(node) { \
			try { ret.first = opt_cast().insert(key_hash, boost::move(node)); \
					ret.second = ret.first != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } } \
		return ret_handler(_base, ret); } \
	\
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node) { \
		if(!node) { return false; } \
		std::size_t key_hash = opt_type::s_hash(this_type::hash_function(), node.key()); \
		write_lock_type lk(_mutex); \
		try { return opt_cast().insert(key_hash, iter_handler(_base), boost::move(node)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename IterHandler, typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		std::size_t key_hash = !node? 0 : opt_type::s_hash(this_type::hash_function(), node.key()); \
		write_lock_type lk(_mutex); \
		if(node) { \
			try { ret.first = opt_cast().insert(key_hash, iter_handler(_base), boost::move(node)); \
					ret.second = ret.first != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } } \
		return ret_handler(_base, ret); }

namespace yggr
{
namespace safe_container
{
namespace tpl_conflict_patch
{

// gcc need this fix conflict

template<typename K, typename T, typename UMapOrUMultiMap>
struct is_umap_or_umultimap_merge
	: public boost::mpl::false_
{
};

template<typename K, typename T,
			template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMultiMap,
			typename H, typename P, typename A>
struct is_umap_or_umultimap_merge< K, T, UMapOrUMultiMap<K, T, H, P, A> >
	: public boost::mpl::true_
{
};

} // namespace tpl_conflict_patch
} // namespace safe_container
} // namespace yggr

// merge
//#define YGGR_PP_SAFE_UMAP_UNIQUE_MERGE_UNSAFE_UMAPS_FUNCTION_IMPL() \
//	template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMultiMap, \
//				typename H2, typename P2, typename A2> \
//	void merge(BOOST_RV_REF_BEG UMapOrUMultiMap<key_type, mapped_type, H2, P2, A2> BOOST_RV_REF_END right) { \
//		typedef UMapOrUMultiMap<key_type, mapped_type, H2, P2, A2> right_type; \
//		if(!static_cast<right_type&>(right).size()) { return; } \
//		write_lock_type lk(_mutex); \
//		container::merge(_base, boost::move(right)); } \
//	\
//	template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UMapOrUMultiMap, \
//				typename H2, typename P2, typename A2> \
//	void merge(UMapOrUMultiMap<key_type, mapped_type, H2, P2, A2>& right) { \
//		if(!right.size()) { return; } \
//		write_lock_type lk(_mutex); \
//		container::merge(_base, right); }

#define YGGR_PP_SAFE_UMAP_UNIQUE_MERGE_UNSAFE_UMAPS_FUNCTION_IMPL() \
	template<typename UMapOrUMultiMap> \
	typename boost::enable_if<tpl_conflict_patch::is_umap_or_umultimap_merge<key_type, mapped_type, UMapOrUMultiMap>, void>::type \
		merge(BOOST_RV_REF(UMapOrUMultiMap) right) { \
		typedef UMapOrUMultiMap right_type; \
		if(!static_cast<right_type&>(right).size()) { return; } \
		write_lock_type lk(_mutex); \
		container::merge(_base, boost::move(right)); } \
	\
	template<typename UMapOrUMultiMap> \
	typename boost::enable_if<tpl_conflict_patch::is_umap_or_umultimap_merge<key_type, mapped_type, UMapOrUMultiMap>, void>::type \
		merge(UMapOrUMultiMap& right) { \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		container::merge(_base, right); }

#define YGGR_PP_SAFE_UMAP_UNIQUE_MERGE_SAFE_UMAPS_FUNCTION_IMPL() \
	template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeUMap, \
				typename M2, typename H2, typename P2, typename A2> \
	void merge(BOOST_RV_REF_BEG \
					safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeUMap> \
				BOOST_RV_REF_END right) { \
		typedef safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeUMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		right_type& right_ref = static_cast<right_type&>(right); \
		if(!right_ref.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right_ref.using_handler(helper); } \
	\
	template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeUMap, \
				typename M2, typename H2, typename P2, typename A2> \
	void merge(safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeUMap>& right) { \
		typedef safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeUMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right.using_handler(helper); } \
	\
	template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeMultiMap, \
				typename M2, typename H2, typename P2, typename A2> \
	void merge(BOOST_RV_REF_BEG \
					safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap> \
				BOOST_RV_REF_END right) { \
		typedef safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		right_type& right_ref = static_cast<right_type&>(right); \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right_ref.using_handler(helper); } \
	\
	template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeMultiMap, \
				typename M2, typename H2, typename P2, typename A2> \
	void merge(safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap>& right) { \
		typedef safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap> right_type; \
		typedef typename right_type::base_type unsafe_right_type; \
		typedef detail::merge_kv_ref_helper<base_type, unsafe_right_type> merge_helper_type; \
		if(!right.size()) { return; } \
		write_lock_type lk(_mutex); \
		merge_helper_type helper(_base); \
		right.using_handler(helper); }

#define YGGR_PP_SAFE_UMAP_NON_UNIQUE_MERGE_SAFE_UMAPS_FUNCTION_IMPL() \
		template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeMap, \
				typename M2, typename H2, typename P2, typename A2> inline \
		void merge(BOOST_RV_REF_BEG \
						safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeMap> \
					BOOST_RV_REF_END right) { \
			typedef safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeMap> right_type; \
			typedef typename right_type::base_type right_base_type; \
			right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
			right.swap(rtmp); \
			this_type::merge(boost::move(rtmp)); } \
		\
		template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeMap, \
				typename M2, typename H2, typename P2, typename A2> inline \
		void merge(safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeMap>& right) { \
			typedef safe_unordered_map<key_type, mapped_type, M2, H2, P2, A2, UnsafeMap> right_type; \
			typedef typename right_type::base_type right_base_type; \
			right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
			right.swap(rtmp); \
			this_type::merge(rtmp); } \
		\
		template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeMultiMap, \
				typename M2, typename H2, typename P2, typename A2> inline \
		void merge(BOOST_RV_REF_BEG \
						safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap> \
					BOOST_RV_REF_END right) { \
			typedef safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap> right_type; \
			typedef typename right_type::base_type right_base_type; \
			right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
			right.swap(rtmp); \
			this_type::merge(boost::move(rtmp)); } \
		\
		template<template<typename _K, typename _T, typename _H, typename _P, typename _A> class UnsafeMultiMap, \
				typename M2, typename H2, typename P2, typename A2> inline \
		void merge(safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap>& right) { \
			typedef safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, A2, UnsafeMultiMap> right_type; \
			typedef typename right_type::base_type right_base_type; \
			right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
			right.swap(rtmp); \
			this_type::merge(rtmp); }
//
#define YGGR_PP_SAFE_UMAP_MERGE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UMAP_UNIQUE_MERGE_UNSAFE_UMAPS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UMAP_UNIQUE_MERGE_SAFE_UMAPS_FUNCTION_IMPL()

#define YGGR_PP_SAFE_UMAP_NON_UNIQUE_HA_OPT_MERGE_FUNCTION_IMPL() \
	template<typename H2, typename P2> inline \
	void merge(BOOST_RV_REF_BEG \
					boost::unordered_map<key_type, mapped_type, H2, P2, allocator_type> \
				BOOST_RV_REF_END right) { \
		typedef boost::unordered_map<key_type, mapped_type, H2, P2, allocator_type> right_type; \
		this_type::merge(static_cast<right_type&>(right)); } \
	\
	template<typename H2, typename P2> inline \
	void merge(boost::unordered_map<key_type, mapped_type, H2, P2, allocator_type>& right) { \
		typedef boost::unordered_map<key_type, mapped_type, H2, P2, allocator_type> right_type; \
		if(!right.size()) { return; } \
		right_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::insert(rtmp.begin(), rtmp.end()); } \
	\
	template<typename H2, typename P2> inline \
	void merge(BOOST_RV_REF_BEG \
					boost::unordered_multimap<key_type, mapped_type, H2, P2, allocator_type> \
				BOOST_RV_REF_END right) { \
		typedef boost::unordered_multimap<key_type, mapped_type, H2, P2, allocator_type> right_type; \
		this_type::merge(static_cast<right_type&>(right)); } \
	\
	template<typename H2, typename P2> \
	void merge(boost::unordered_multimap<key_type, mapped_type, H2, P2, allocator_type>& right) { \
		typedef boost::unordered_multimap<key_type, mapped_type, H2, P2, allocator_type> right_type; \
		typedef typename detail::cast_to_ummap_ha_opt<right_type>::type right_opt_type; \
		typedef typename right_opt_type::multi_destroy_holder_type right_multi_destroy_holder_type; \
		typedef typename right_opt_type::node_pointer right_pointer_type; \
		typedef container::vector<std::size_t> hash_vt_type; \
		typedef container::vector<right_pointer_type> right_pointer_vt_type; \
		typedef typename right_pointer_vt_type::iterator right_pointer_iter_type; \
		if(!right.size()) { return; } \
		right_multi_destroy_holder_type holder(right.size(), right.get_allocator()); \
		hash_vt_type right_hash_vt; \
		right_pointer_vt_type right_pointer_vt; \
		size_type right_size = right.size(); \
		right_hash_vt.reserve(right_size); \
		right_pointer_vt.reserve(right_size); \
		(reinterpret_cast<right_opt_type&>(right)).clear(detail::allocator_destroy_holder_ref(holder)); \
		holder.copy_to(right_pointer_vt); \
		holder.non_free_clear(); \
		hasher hfoo = this_type::hash_function(); \
		for(right_pointer_iter_type i = right_pointer_vt.begin(), isize = right_pointer_vt.end(); i != isize; ++i) { \
			assert(*i); \
			std::back_inserter(right_hash_vt) = opt_type::s_set_node_hash((*i), hfoo, (*i)->value().first); } \
		{ write_lock_type lk(_mutex); \
			opt_cast().insert(right_hash_vt.begin(), right_hash_vt.end(), right_pointer_vt.begin(), right_pointer_vt.end()); } } \
	\
	template<typename M2, typename H2, typename P2> inline \
	void merge(BOOST_RV_REF_BEG \
					safe_unordered_map<key_type, mapped_type, M2, H2, P2, allocator_type> \
				BOOST_RV_REF_END right) { \
		typedef safe_unordered_map<key_type, mapped_type, M2, H2, P2, allocator_type> right_type; \
		this_type::merge(static_cast<right_type&>(right)); } \
	\
	template<typename M2, typename H2, typename P2> inline \
	void merge(safe_unordered_map<key_type, mapped_type, M2, H2, P2, allocator_type>& right) { \
		typedef safe_unordered_map<key_type, mapped_type, M2, H2, P2, allocator_type> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(rtmp); } \
	\
	template<typename M2, typename H2, typename P2> inline \
	void merge(BOOST_RV_REF_BEG \
					safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, allocator_type> \
				BOOST_RV_REF_END right) { \
		typedef safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, allocator_type> right_type; \
		this_type::merge(static_cast<right_type&>(right)); } \
	\
	template<typename M2, typename H2, typename P2> inline \
	void merge(safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, allocator_type>& right) { \
		typedef safe_unordered_multimap<key_type, mapped_type, M2, H2, P2, allocator_type> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_base_type rtmp(0, right.hash_function(), right.key_eq(), right.get_allocator()); \
		right.swap(rtmp); \
		this_type::merge(rtmp); }

#define YGGR_PP_SAFE_UMULTIMAP_MERGE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UMAP_UNIQUE_MERGE_UNSAFE_UMAPS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UMAP_NON_UNIQUE_MERGE_SAFE_UMAPS_FUNCTION_IMPL()

#define YGGR_PP_SAFE_UMULTIMAP_HA_OPT_MERGE_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UMAP_UNIQUE_MERGE_UNSAFE_UMAPS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UMAP_NON_UNIQUE_MERGE_SAFE_UMAPS_FUNCTION_IMPL() \
	YGGR_PP_SAFE_UMAP_NON_UNIQUE_HA_OPT_MERGE_FUNCTION_IMPL()

#endif // __YGGR_SAFE_CONTAINER_UNORDERED_MAP_SAFE_UNORDERED_MAP_MEMBER_IMPL_HPP__
