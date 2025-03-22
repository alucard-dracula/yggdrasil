//safe_kv_map_subscript_and_at_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_SAFE_KV_MAP_SUBSCRIPT_AND_AT_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_SAFE_KV_MAP_SUBSCRIPT_AND_AT_IMPL_HPP__

#include <boost/ref.hpp>
#include <yggr/base/yggrdef.h>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/func/foo_t_info.hpp>

#define YGGR_PP_SAFE_KV_MAP_SUBSCRIPT_AND_AT_IMPL( __cont__ ) \
	private: \
		inline static mapped_type& unlock_index(base_type& base, const key_type& key) { \
			return base[key]; } \
		\
		mapped_type unsafe_get_index(const key_type& key) { \
			write_lock_type lk(_mutex); \
			return _base[key]; } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			unsafe_set_index(const key_type& key, const Handler& handler) { \
			write_lock_type lk(_mutex); \
			return handler(_base[key]); } \
		\
		static mapped_type& unlock_at(base_type& base, const key_type& key) { \
			iterator iter = base.find(key); \
			if(iter == base.end()) { \
				throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
			return iter->second; } \
		\
		mapped_type unsafe_get_at(const key_type& key) const { \
			read_lock_type lk(_mutex); \
			const_iterator iter = _base.find(key); \
			if(iter == _base.end()) { \
				throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
			return iter->second; } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			unsafe_set_at(const key_type& key, const Handler& handler) { \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key); \
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
			: public detail::basic_bridge<__cont__> { \
		private: \
			typedef detail::basic_bridge<__cont__> bridge_base_type; \
		public: \
			typedef typename bridge_base_type::container_type container_type; \
			typedef typename bridge_base_type::base_container_type base_container_type; \
			typedef typename bridge_base_type::value_type value_type; \
		private: \
			typedef bridge_at this_type; \
		public: \
			bridge_at(container_type& cont, BOOST_RV_REF(key_type) key) \
				: bridge_base_type(cont), _key(key) {} \
			\
			bridge_at(container_type& cont, const key_type& key) \
				: bridge_base_type(cont), _key(key) {} \
			\
			bridge_at(const bridge_at& right) \
				: bridge_base_type(right), _key(right._key) {} \
			\
			~bridge_at(void) {} \
			\
		public:\
			inline const key_type& key(void) const { return _key; } \
			\
			inline bool operator==(const bridge_at& right) const { \
				return \
					(this == boost::addressof(right)) \
					|| (bridge_base_type::compare_eq(right) \
						&& _key == right._key); } \
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
				return boost::bind(&container_type::unlock_at, _1, boost::cref(_key)); } \
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
			const key_type _key; }; \
		\
		class bridge_index \
			: public detail::basic_bridge<__cont__> { \
		private: \
			typedef detail::basic_bridge<__cont__> bridge_base_type; \
		public: \
			typedef typename bridge_base_type::container_type container_type; \
			typedef typename bridge_base_type::base_container_type base_container_type; \
			typedef typename bridge_base_type::value_type value_type; \
		private: \
			typedef bridge_index this_type; \
		public: \
			bridge_index(container_type& cont, BOOST_RV_REF(key_type) key) \
				: bridge_base_type(cont), _key(key) {} \
			\
			bridge_index(container_type& cont, const key_type& key) \
				: bridge_base_type(cont), _key(key) {} \
			\
			bridge_index(const bridge_index& right) \
				: bridge_base_type(right), _key(right._key) {} \
			\
			~bridge_index(void) {} \
			\
		public: \
			inline const key_type& key(void) const { return _key; } \
			\
			inline bool operator==(const bridge_index& right) const { \
				return \
					(this == boost::addressof(right)) \
					|| (bridge_base_type::compare_eq(right) \
						&& _key == right._key); } \
			\
			template<typename OtherBridge> inline \
			bool operator==(const OtherBridge&) const { return false; } \
			\
			template<typename OtherBridge> inline \
			bool operator!=(const OtherBridge& right) const { \
				return !this_type::operator==(right); } \
			\
		public: \
			inline mapped_type operator()(void) const { \
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
				return boost::bind(&container_type::unlock_index, _1, boost::cref(_key)); } \
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
			const key_type _key; }; \
		\
	public: \
		inline detail::safe_element_reference<bridge_index> operator[](BOOST_RV_REF(key_type) key) { \
			return detail::safe_element_reference<bridge_index>( \
						bridge_index(*this, boost::move(key))); } \
		\
		inline detail::safe_element_reference<bridge_index> operator[](const key_type& key) { \
			return detail::safe_element_reference<bridge_index>(bridge_index(*this, key)); } \
		\
		inline detail::safe_element_reference<bridge_at> at(const key_type& key) { \
			return detail::safe_element_reference<bridge_at>(bridge_at(*this, key)); } \
		\
		inline mapped_type at(const key_type& key) const { \
			return this_type::unsafe_get_at(key); } \
		\
		bool get_value(const key_type& key, mapped_type& val) const { \
			read_lock_type lk(_mutex); \
			const_iterator iter = _base.find(key); \
			if(iter == _base.end()) { return false; } \
			val = iter->second; \
			return true; } \
		\
		bool set_value(const key_type& key, BOOST_RV_REF(mapped_type) val) { \
			write_lock_type lk(_mutex); \
			copy_or_move_or_swap(_base[key], boost::move(val)); \
			return true; } \
		\
		bool set_value(const key_type& key, const mapped_type& val) { \
			mapped_type tmp(val); \
			write_lock_type lk(_mutex); \
			copy_or_move_or_swap(_base[key], boost::move(tmp)); \
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
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key); \
			if(iter == _base.end()) { return false; } \
			copy_or_move_or_swap(iter->second, boost::move(val)); \
			return true; } \
		\
		bool set_at(const key_type& key, const mapped_type& val) { \
			mapped_type tmp(val); \
			write_lock_type lk(_mutex); \
			iterator iter = _base.find(key); \
			if(iter == _base.end()) { return false; } \
			copy_or_move_or_swap(iter->second, boost::move(tmp)); \
			return true; } \
		\
		inline bool set_at(BOOST_RV_REF(value_type) val) { \
			return this_type::set_at(val.first, boost::move(val.second)); } \
		\
		inline bool set_at(const value_type& val) { \
			return this_type::set_at(val.first, val.second); }

namespace yggr
{
namespace safe_container
{
namespace detail
{
} // namespace detail
} // namespace safe_container
} // namespace yggr

#endif // __YGGR_SAFE_CONTAINER_DETAIL_SAFE_KV_MAP_SUBSCRIPT_AND_AT_IMPL_HPP__
