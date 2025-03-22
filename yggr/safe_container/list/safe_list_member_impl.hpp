//safe_list_member_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_LIST_SAFE_LIST_MEMBER_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_LIST_SAFE_LIST_MEMBER_IMPL_HPP__

#ifndef __YGGR_SAFE_CONTAINER_SAFE_LIST_HPP__
#	error "this file is impl file, can't be used alone"
#endif // __YGGR_SAFE_CONTAINER_SAFE_LIST_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/safe_container/detail/splice_impl.hpp>
#include <yggr/safe_container/detail/merge_impl.hpp>
#include <yggr/safe_container/detail/safe_list_operations_impl.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#   include <initializer_list>
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#	define YGGR_PP_SAFE_LIST_CONSTRUCTOR_IL_IMPL() \
		safe_list(std::initializer_list<value_type> l) \
			: _base(l.begin(), l.end()) {} \
		\
		safe_list(std::initializer_list<value_type> l, const allocator_type& alloc) \
			: _base(l.begin(), l.end(), alloc) {}

#else

#	define YGGR_PP_SAFE_LIST_CONSTRUCTOR_IL_IMPL()

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

// constructor
#define YGGR_PP_SAFE_LIST_CONSTRUCTOR_IMPL() \
	safe_list(void) {} \
	\
	explicit safe_list(const allocator_type& alloc) \
		: _base(alloc) {} \
	\
	explicit safe_list(size_type count) \
		: _base(count) {} \
	\
	safe_list(size_type count, const value_type& val) \
		: _base(count, val) {} \
	\
	safe_list(size_type count, const value_type& val, const allocator_type& alloc) \
		: _base(count, val, alloc) {} \
	\
	template<typename InputIterator> \
	safe_list(InputIterator first, InputIterator last) \
		: _base(first, last) {} \
	\
	template<typename InputIterator> \
	safe_list(InputIterator first, InputIterator last, const allocator_type& alloc) \
		: _base(first, last, alloc) {} \
	\
	safe_list(BOOST_RV_REF(base_type) right) \
		: _base(right.get_allocator()) { \
		base_type& right_ref = right; \
		_base.swap(right_ref); } \
	\
	safe_list(const base_type& right) \
		: _base(right) {} \
	\
	safe_list(BOOST_RV_REF(base_type) right, const allocator_type& alloc) \
			: _base(alloc) { \
		base_type& right_ref = right; \
		_base.swap(right_ref); } \
	\
	safe_list(const base_type& right, const allocator_type& alloc) \
		: _base(right.begin(), right.end(), alloc) {} \
	\
	safe_list(BOOST_RV_REF(this_type) right) \
		: _base(right.get_allocator()) { \
		this_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	safe_list(const this_type& right) \
		: _base(right.get_allocator()) { \
		right.copy_to_base(_base); } \
	\
	safe_list(BOOST_RV_REF(this_type) right, const allocator_type& alloc) \
		: _base(alloc) { \
		this_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	safe_list(const this_type& right, const allocator_type& alloc) \
		: _base(alloc) { \
		right.copy_to_base(_base); } \
	\
	YGGR_PP_SAFE_LIST_CONSTRUCTOR_IL_IMPL()


#define YGGR_PP_SAFE_LIST_OPT_BASE_BASE_CONSTRUCTOR_IMPL() \
	safe_list(BOOST_RV_REF(base_base_type) right) \
		: _base(right.get_allocator()) { \
		base_base_type& right_ref = right; \
		_base.swap(right_ref); } \
	\
	safe_list(const base_base_type& right) \
		: _base(right) {} \
	\
	safe_list(BOOST_RV_REF(base_base_type) right, const allocator_type& alloc) \
			: _base(alloc) { \
		base_base_type& right_ref = right; \
		_base.swap(right_ref); } \
	\
	safe_list(const base_base_type& right, const allocator_type& alloc) \
		: _base(right.begin(), right.end(), alloc) {}

#define YGGR_PP_SAFE_LIST_ISOMERIC_SAME_CONSTRUCTOR_IMPL( __list__ ) \
	template<typename Ax2> \
	safe_list(BOOST_RV_REF_BEG \
					__list__<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							__list__<value_type, Ax2>, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.get_allocator()) { \
		typedef __list__<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		_base.swap(reinterpret_cast<base_type&>(right_ref)); } \
	\
	template<typename Ax2> \
	safe_list(const __list__<value_type, Ax2>& right, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							__list__<value_type, Ax2>, \
							base_type >, \
						mplex::sfinae_type>::type sfinae = 0) \
		: _base(reinterpret_cast<const base_type&>(right)) { } \
	\
	template<typename Ax2> \
	safe_list(BOOST_RV_REF_BEG \
					__list__<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							__list__<value_type, Ax2>, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(alloc) { \
		typedef __list__<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		_base.swap(reinterpret_cast<base_type&>(right_ref)); } \
	\
	template<typename Ax2> \
	safe_list(const __list__<value_type, Ax2>& right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							__list__<value_type, Ax2>, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.begin(), right.end(), alloc) {} \
	\
	template<typename Mtx2, typename Ax2> \
	safe_list(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
							base_type >, \
						mplex::sfinae_type \
					>::type sfinae = 0) \
		: _base(right.get_allocator()) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	template<typename Mtx2, typename Ax2> \
	safe_list(const safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.get_allocator()) { \
		right.copy_to_base(_base); } \
	\
	template<typename Mtx2, typename Ax2> \
	safe_list(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(alloc) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		right_ref.swap(_base); } \
	\
	template<typename Mtx2, typename Ax2> \
	safe_list(const safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						boost::is_same< \
							typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(alloc) { \
		right.copy_to_base(_base); } \
	\
	template<typename Mtx2, typename Ax2> \
	safe_list(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
							base_type >, \
						mplex::sfinae_type \
					>::type sfinae = 0) \
		: _base(right.get_allocator()) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_type& right_ref = right; \
		right_ref.swap(reinterpret_cast<right_base_type&>(_base)); } \
	\
	template<typename Mtx2, typename Ax2> \
	safe_list(const safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.get_allocator()) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right.copy_to_base(reinterpret_cast<right_base_type&>(_base)); } \
	\
	template<typename Mtx2, typename Ax2> \
	safe_list(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(alloc) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right_type& right_ref = right; \
		right_ref.swap(reinterpret_cast<right_base_type&>(_base)); } \
	\
	template<typename Mtx2, typename Ax2> \
	safe_list(const safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
							base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(alloc) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		right.copy_to_base(reinterpret_cast<right_base_type&>(_base)); }


#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_CONSTRUCTOR_IMPL( ) \
	template<typename Ax2> \
	safe_list(BOOST_RV_REF_BEG \
					boost::container::list<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							boost::container::list<value_type, Ax2>, \
							base_base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.get_allocator()) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		_base.swap(reinterpret_cast<base_base_type&>(right_ref)); } \
	\
	template<typename Ax2> \
	safe_list(const boost::container::list<value_type, Ax2>& right, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							boost::container::list<value_type, Ax2>, \
							base_base_type >, \
						mplex::sfinae_type>::type sfinae = 0) \
		: _base(reinterpret_cast<const base_base_type&>(right)) { } \
	\
	template<typename Ax2> \
	safe_list(BOOST_RV_REF_BEG \
					boost::container::list<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							boost::container::list<value_type, Ax2>, \
							base_base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(alloc) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		_base.swap(reinterpret_cast<base_base_type&>(right_ref)); } \
	\
	template<typename Ax2> \
	safe_list(const boost::container::list<value_type, Ax2>& right, \
				const allocator_type& alloc, \
				typename \
					boost::enable_if< \
						container::is_isomeric_same_list< \
							boost::container::list<value_type, Ax2>, \
							base_base_type >, \
						mplex::sfinae_type >::type sfinae = 0) \
		: _base(right.begin(), right.end(), alloc) {}

// destructor
#define YGGR_PP_SAFE_LIST_DESTRUCTOR_IMPL() \
	~safe_list(void) {}


//operator=

#define YGGR_PP_SAFE_LIST_OPT_BASE_BASE_OPERATOR_SET_IMPL() \
	inline this_type& operator=(BOOST_RV_REF(base_base_type) right) { \
		this_type::swap(boost::move(right)); \
		return *this; } \
	\
	this_type& operator=(const base_base_type& right) { \
		if(&_base == &right) { return *this; } \
		base_type tmp(right); \
		this_type::swap(tmp); \
		return *this; }

#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_OPERATOR_SET_IMPL() \
	template<typename Val2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list< Val2, Ax2 >, \
				base_base_type >, \
			this_type& >::type \
		operator=(BOOST_RV_REF_BEG \
						boost::container::list< Val2, Ax2 > \
					BOOST_RV_REF_END right) { \
		typedef boost::container::list< Val2, Ax2 > right_type; \
		right_type& tmp = right; \
		this_type::swap(reinterpret_cast<base_base_type&>(tmp)); \
		return *this; } \
	\
	template<typename Val2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list< Val2, Ax2 >, \
				base_base_type >, \
			this_type& >::type \
		operator=(const boost::container::list< Val2, Ax2 >& right) { \
		base_type tmp(reinterpret_cast<const base_base_type&>(right)); \
		this_type::swap(tmp); \
		return *this; } 

#define YGGR_PP_SAFE_LIST_OPT_BASE_BASE_SWAP_IMPL() \
	inline void swap(BOOST_RV_REF(base_base_type) right) { \
		base_base_type& right_ref = right; \
		this_type::swap(right_ref); } \
	\
	void swap(base_base_type& right) { \
		if(boost::addressof(_base) == boost::addressof(right)) { return; } \
		write_lock_type lk(_mutex); \
		_base.swap(right); }

#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_SWAP_IMPL() \
	template<typename Val2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<Val2, Ax2>, \
				base_base_type >, \
			void >::type \
		swap(BOOST_RV_REF_BEG \
					boost::container::list<Val2, Ax2> \
				BOOST_RV_REF_END right) { \
		typedef boost::container::list<Val2, Ax2> right_type; \
		right_type& right_ref = right; \
		this_type::swap(reinterpret_cast<base_base_type&>(right_ref)); } \
	\
	template<typename Val2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<Val2, Ax2>, \
				base_base_type >, \
			void >::type \
		swap(boost::container::list<Val2, Ax2>& right) { \
		this_type::swap(reinterpret_cast<base_base_type&>(right)); } 

// capacity:
#define YGGR_PP_SAFE_LIST_CAPACITY_IMPL() \
	bool empty(void) const { \
		read_lock_type lk(_mutex); \
		return _base.empty(); } \
	\
	size_type size(void) const { \
		read_lock_type lk(_mutex); \
		return _base.size(); } \
	\
	size_type max_size(void) const { \
		read_lock_type lk(_mutex); \
		return _base.max_size(); } \
	\
	void resize(size_type size) { \
		write_lock_type lk(_mutex); \
		_base.resize(size); } \
	\
	void resize(size_type size, const value_type& val) { \
		write_lock_type lk(_mutex); \
		_base.resize(size, val); }

// element access:
#define YGGR_PP_SAFE_LIST_ELEMENT_ACCESS_IMPL() \
	private:\
		inline static reference unlock_front(base_type& base) { \
			if(base.empty()) { \
				throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
			return base.front(); } \
		\
		value_type unsafe_get_front(void) const { \
			read_lock_type lk(_mutex); \
			if(_base.empty()) { \
				throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
			return _base.front(); } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			unsafe_set_front(const Handler& handler) { \
			write_lock_type lk(_mutex); \
			if(_base.empty()) { \
				throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
			return handler(_base.front()); } \
		\
		inline static reference unlock_back(base_type& base) { \
			if(base.empty()) { \
				throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
			return base.back(); } \
		\
		value_type unsafe_get_back(void) const { \
			read_lock_type lk(_mutex); \
			if(_base.empty()) { \
				throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
			return _base.back(); } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			unsafe_set_back(const Handler& handler) { \
			write_lock_type lk(_mutex); \
			if(_base.empty()) { \
				throw error_maker_type::make_error(error_maker_type::E_element_not_exists); } \
			return handler(_base.back()); } \
		\
		class bridge_front; \
		class bridge_back; \
		\
		friend class bridge_front; \
		friend class bridge_back; \
		\
		class bridge_front \
			: public detail::basic_bridge<safe_list> { \
		private: \
			typedef detail::basic_bridge<safe_list> bridge_base_type; \
		public: \
			typedef typename bridge_base_type::container_type container_type; \
			typedef typename bridge_base_type::base_container_type base_container_type; \
			typedef typename bridge_base_type::value_type value_type; \
		private: \
			typedef bridge_front this_type; \
		public: \
			bridge_front(container_type& cont) \
				: bridge_base_type(cont) {} \
			\
			bridge_front(const bridge_front& right) \
				: bridge_base_type(right) {} \
			\
			~bridge_front(void) {} \
			\
		public: \
			inline bool operator==(const bridge_front& right) const { \
				return bridge_base_type::compare_eq(right); } \
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
			inline value_type operator()(void) const { \
				const container_type& cont = bridge_base_type::_wrap_cont; \
				return cont.unsafe_get_front(); } \
			\
			template<typename Handler> inline \
			typename ::yggr::func::foo_t_info<Handler>::result_type \
				operator()(const Handler& handler) const { \
				container_type& cont = bridge_base_type::_wrap_cont; \
				return cont.unsafe_set_front(handler); } \
			\
		public: \
			inline boost::function1<value_type&, base_container_type&> \
				native_handler(void) const { \
				return boost::bind(&container_type::unlock_front, _1); } \
			\
			template<typename Op_Handler, typename OtherBridge> inline \
			boost::function1 < \
				typename ::yggr::func::foo_t_info<Op_Handler>::result_type, \
				base_container_type& > \
				link(const Op_Handler& op_handler, const OtherBridge& other_bridge) const { \
				return bridge_base_type::pro_s_link_handler( \
						op_handler, \
						this_type::native_handler(), \
						other_bridge.native_handler()); } \
			\
			template<typename Op_Handler, typename Handler> inline \
			boost::function1< \
				typename ::yggr::func::foo_t_info<Op_Handler>::result_type, \
				base_container_type& > \
				link_handler(const Op_Handler& op_handler, const Handler& handler) const { \
				return bridge_base_type::pro_s_link_handler(op_handler, this_type::native_handler(), handler); } }; \
		\
		class bridge_back \
			: public detail::basic_bridge<safe_list> { \
		private: \
			typedef detail::basic_bridge<safe_list> bridge_base_type; \
		public: \
			typedef typename bridge_base_type::container_type container_type; \
			typedef typename bridge_base_type::base_container_type base_container_type; \
			typedef typename bridge_base_type::value_type value_type; \
		private: \
			typedef bridge_back this_type; \
		public: \
			bridge_back(container_type& cont) \
				: bridge_base_type(cont) {} \
			\
			bridge_back(const bridge_back& right) \
				: bridge_base_type(right) {} \
			\
			~bridge_back(void) {} \
			\
		public: \
			inline bool operator==(const bridge_back& right) const { \
				return bridge_base_type::compare_eq(right); } \
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
			inline value_type operator()(void) const { \
				const container_type& cont = bridge_base_type::_wrap_cont; \
				return cont.unsafe_get_back(); } \
			\
			template<typename Handler> inline \
			typename ::yggr::func::foo_t_info<Handler>::result_type \
				operator()(const Handler& handler) const { \
				container_type& cont = bridge_base_type::_wrap_cont; \
				return cont.unsafe_set_back(handler); } \
			\
		public: \
			inline boost::function1<value_type&, base_container_type&> \
				native_handler(void) const { \
				return boost::bind(&container_type::unlock_back, _1); } \
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
				return bridge_base_type::pro_s_link_handler(op_handler, this_type::native_handler(), handler); } }; \
			\
	public: \
		inline detail::safe_element_reference<bridge_front> front(void) { \
			return detail::safe_element_reference<bridge_front>(bridge_front(*this)); } \
		\
		inline value_type front(void) const { \
			return this_type::unsafe_get_front(); } \
		\
		inline detail::safe_element_reference<bridge_back> back(void) { \
			return detail::safe_element_reference<bridge_back>(bridge_back(*this)); } \
		\
		inline value_type back(void) const { \
			return this_type::unsafe_get_back(); }

// operations:

#define YGGR_PP_SAFE_LIST_SPLICE_IMPL() \
	YGGR_PP_SAFE_CONTAINER_SPLICE_IMPL()

#define YGGR_PP_SAFE_LIST_OPT_BASE_BASE_SPLICE_IMPL() \
	public: \
		inline void splice(BOOST_RV_REF(base_base_type) right) { \
			base_base_type& right_ref = right; \
			this_type::splice(right_ref); } \
		\
		void splice(base_base_type& right) { \
			write_lock_type lk(_mutex); \
			_base.splice(_base.end(), right); } \
		\
		inline void splice(BOOST_RV_REF(base_base_type) right, iterator i) { \
			base_base_type& right_ref = right; \
			this_type::splice(right_ref, i); } \
		\
		void splice(base_base_type& right, iterator i) { \
			write_lock_type lk(_mutex); \
			_base.splice(_base.end(), right, i); } \
		\
		inline void splice(BOOST_RV_REF(base_base_type) right, iterator first, iterator last) { \
			base_base_type& right_ref = right; \
			this_type::splice(right_ref, first, last); } \
		\
		void splice(base_base_type& right, iterator first, iterator last) { \
			write_lock_type lk(_mutex); \
			_base.splice(_base.end(), right, first, last); } \
		\
		template<typename IterHandler> inline \
		void splice(const IterHandler& iter_handler, BOOST_RV_REF(base_base_type) right) { \
			base_base_type& right_ref = right; \
			this_type::splice(iter_handler, right_ref); } \
		\
		template<typename IterHandler> \
		void splice(const IterHandler& iter_handler, base_base_type& right) { \
			write_lock_type lk(_mutex); \
			_base.splice(iter_handler(_base), right); } \
		\
		template<typename IterHandler> inline \
		void splice(const IterHandler& iter_handler, \
						BOOST_RV_REF(base_base_type) right, iterator i) { \
			base_base_type& right_ref = right; \
			this_type::splice(iter_handler, right_ref, i); } \
		\
		template<typename IterHandler> \
		void splice(const IterHandler& iter_handler, base_base_type& right, iterator i) { \
			write_lock_type lk(_mutex); \
			_base.splice(iter_handler(_base), right, i); } \
		\
		template<typename IterHandler> \
		void splice(const IterHandler& iter_handler, \
					BOOST_RV_REF(base_base_type) right, iterator first, iterator last) { \
			base_base_type& right_ref = right; \
			this_type::splice(iter_handler, right_ref, first, last); } \
		\
		template<typename IterHandler> \
		void splice(const IterHandler& iter_handler, \
					base_base_type& right, iterator first, iterator last) { \
			write_lock_type lk(_mutex); \
			_base.splice(iter_handler(_base), right, first, last); }

#define YGGR_PP_SAFE_LIST_ISOMERIC_SAME_SPLICE_IMPL( __list__ ) \
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					__list__<value_type, Ax2> \
				BOOST_RV_REF_END right) { \
		typedef __list__<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		this_type::splice(reinterpret_cast<base_type&>(right_ref)); } \
	\
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(__list__<value_type, Ax2>& right) { \
		this_type::splice(reinterpret_cast<base_type&>(right)); } \
	\
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					__list__<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				typename __list__<value_type, Ax2>::iterator i) { \
		typedef __list__<value_type, Ax2> right_type; \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename __list__<value_type, Ax2>::iterator>)); \
		right_type& right_ref = right; \
		this_type::splice(reinterpret_cast<base_type&>(right_ref), i); } \
	\
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(__list__<value_type, Ax2>& right, iterator i) { \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename __list__<value_type, Ax2>::iterator>)); \
		this_type::splice(reinterpret_cast<base_type&>(right), i); } \
	\
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					__list__<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				typename __list__<value_type, Ax2>::iterator first, \
				typename __list__<value_type, Ax2>::iterator last) { \
		typedef __list__<value_type, Ax2> right_type; \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename __list__<value_type, Ax2>::iterator>)); \
		right_type& right_ref = right; \
		this_type::splice(reinterpret_cast<base_type&>(right_ref), first, last); } \
	\
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(__list__<value_type, Ax2>& right, \
				typename __list__<value_type, Ax2>::iterator first, \
				typename __list__<value_type, Ax2>::iterator last) { \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename __list__<value_type, Ax2>::iterator>)); \
		this_type::splice(reinterpret_cast<base_type&>(right), first, last); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				BOOST_RV_REF_BEG \
					__list__<value_type, Ax2> \
				BOOST_RV_REF_END right) { \
		typedef __list__<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		this_type::splice(iter_handler, reinterpret_cast<base_type&>(right_ref)); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				__list__<value_type, Ax2>& right) { \
		this_type::splice(iter_handler, reinterpret_cast<base_type&>(right)); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				BOOST_RV_REF_BEG \
					__list__<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				typename __list__<value_type, Ax2>::iterator i) { \
		typedef __list__<value_type, Ax2> right_type; \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename __list__<value_type, Ax2>::iterator>)); \
		right_type& right_ref = right; \
		this_type::splice(iter_handler, reinterpret_cast<base_type&>(right_ref), i); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				__list__<value_type, Ax2>& right, \
				typename __list__<value_type, Ax2>::iterator i) { \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename __list__<value_type, Ax2>::iterator>)); \
		this_type::splice(iter_handler, reinterpret_cast<base_type&>(right), i); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				BOOST_RV_REF_BEG \
					__list__<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				typename __list__<value_type, Ax2>::iterator first, \
				typename __list__<value_type, Ax2>::iterator last) { \
		typedef __list__<value_type, Ax2> right_type; \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename __list__<value_type, Ax2>::iterator>)); \
		right_type& right_ref = right; \
		this_type::splice(iter_handler, reinterpret_cast<base_type&>(right_ref), first, last); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				__list__<value_type, Ax2>& right, \
				typename __list__<value_type, Ax2>::iterator first, \
				typename __list__<value_type, Ax2>::iterator last) { \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename __list__<value_type, Ax2>::iterator>)); \
		this_type::splice(iter_handler, reinterpret_cast<base_type&>(right), first, last); } \
	\
	template<typename Mtx2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(right_ref); } \
	\
	template<typename Mtx2, typename Ax2> \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(safe_list<value_type, Mtx2, Ax2, __list__>& right) { \
		typedef typename base_type::iterator (base_type::* iter_foo_type)(void); \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			_base, right, \
			boost::bind(static_cast<iter_foo_type>(&base_type::end), _1), \
			boost::bind(static_cast<iter_foo_type>(&base_type::begin), _1), \
			boost::bind(static_cast<iter_foo_type>(&base_type::end), _1) ); } \
	\
	template<typename Mtx2, typename Ax2, typename SrcEIterHandler> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				const SrcEIterHandler& src_eins_iter_handler) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(right_ref, src_eins_iter_handler); } \
	\
	template<typename Mtx2, typename Ax2, typename SrcEIterHandler> \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				const SrcEIterHandler& src_eins_iter_handler) { \
		typedef typename base_type::iterator (base_type::* iter_foo_type)(void); \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			_base, right, \
			boost::bind(static_cast<iter_foo_type>(&base_type::end), _1), \
			src_eins_iter_handler ); } \
	\
	template<typename Mtx2, typename Ax2, typename SrcEIterHandler, typename SrcZIterHandler> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				const SrcEIterHandler& src_eins_iter_handler, \
				const SrcZIterHandler& src_zwei_iter_handler ) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(right_ref, src_eins_iter_handler, src_zwei_iter_handler); } \
	\
	template<typename Mtx2, typename Ax2, typename SrcEIterHandler, typename SrcZIterHandler> \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				const SrcEIterHandler& src_eins_iter_handler, \
				const SrcZIterHandler& src_zwei_iter_handler ) { \
		typedef typename base_type::iterator (base_type::* iter_foo_type)(void); \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			_base, right, \
			boost::bind(static_cast<iter_foo_type>(&base_type::end), _1), \
			src_eins_iter_handler, src_zwei_iter_handler ); } \
	\
	template<typename Mtx2, typename Ax2, typename IterHandler> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(iter_handler, right_ref); } \
	\
	template<typename Mtx2, typename Ax2, typename IterHandler> \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				safe_list<value_type, Mtx2, Ax2, __list__>& right) { \
		typedef typename base_type::iterator (base_type::* iter_foo_type)(void); \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			_base, right, \
			iter_handler, \
			boost::bind(static_cast<iter_foo_type>(&base_type::begin), _1), \
			boost::bind(static_cast<iter_foo_type>(&base_type::end), _1) ); } \
	\
	template<typename Mtx2, typename Ax2, typename DstIterHandler, typename SrcEIterHandler> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(const DstIterHandler& dst_iter_handler, \
				BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				const SrcEIterHandler& src_eins_iter_handler) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(dst_iter_handler, right_ref, src_eins_iter_handler); } \
	\
	template<typename Mtx2, typename Ax2, typename DstIterHandler, typename SrcEIterHandler> \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(const DstIterHandler& dst_iter_handler, \
				safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				const SrcEIterHandler& src_eins_iter_handler) { \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			_base, right, \
			dst_iter_handler, src_eins_iter_handler ); } \
	\
	template<typename Mtx2, typename Ax2, \
				typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(const DstIterHandler& dst_iter_handler, \
				BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				const SrcEIterHandler& src_eins_iter_handler, \
				const SrcZIterHandler& src_zwei_iter_handler) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(dst_iter_handler, right_ref, src_eins_iter_handler, src_zwei_iter_handler); } \
	\
	template<typename Mtx2, typename Ax2, \
				typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
			splice(const DstIterHandler& dst_iter_handler, \
					safe_list<value_type, Mtx2, Ax2, __list__>& right, \
					const SrcEIterHandler& src_eins_iter_handler, \
					const SrcZIterHandler& src_zwei_iter_handler) { \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			_base, right, \
			dst_iter_handler, \
			src_eins_iter_handler, \
			src_zwei_iter_handler ); } \
	\
	template<typename Mtx2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(right_ref); } \
	\
	template<typename Mtx2, typename Ax2> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(safe_list<value_type, Mtx2, Ax2, __list__>& right) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		typedef typename right_base_type::iterator (right_base_type::* iter_foo_type)(void); \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename right_base_type::iterator>)); \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			reinterpret_cast<right_base_type&>(_base), right, \
			boost::bind(static_cast<iter_foo_type>(&right_base_type::end), _1), \
			boost::bind(static_cast<iter_foo_type>(&right_base_type::begin), _1), \
			boost::bind(static_cast<iter_foo_type>(&right_base_type::end), _1) ); } \
	\
	template<typename Mtx2, typename Ax2, typename SrcEIterHandler> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				const SrcEIterHandler& src_eins_iter_handler) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(right_ref, src_eins_iter_handler); } \
	\
	template<typename Mtx2, typename Ax2, typename SrcEIterHandler> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				const SrcEIterHandler& src_eins_iter_handler) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		typedef typename right_base_type::iterator (right_base_type::* iter_foo_type)(void); \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename right_base_type::iterator>)); \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			reinterpret_cast<right_base_type&>(_base), right, \
			boost::bind(static_cast<iter_foo_type>(&right_base_type::end), _1), \
			src_eins_iter_handler ); } \
	\
	template<typename Mtx2, typename Ax2, typename SrcEIterHandler, typename SrcZIterHandler> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				const SrcEIterHandler& src_eins_iter_handler, \
				const SrcZIterHandler& src_zwei_iter_handler ) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(right_ref, src_eins_iter_handler, src_zwei_iter_handler); } \
	\
	template<typename Mtx2, typename Ax2, typename SrcEIterHandler, typename SrcZIterHandler> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				const SrcEIterHandler& src_eins_iter_handler, \
				const SrcZIterHandler& src_zwei_iter_handler ) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		typedef typename right_base_type::iterator (right_base_type::* iter_foo_type)(void); \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename right_base_type::iterator>)); \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			reinterpret_cast<right_base_type&>(_base), right, \
			boost::bind(static_cast<iter_foo_type>(&right_base_type::end), _1), \
			src_eins_iter_handler, src_zwei_iter_handler ); } \
	\
	template<typename Mtx2, typename Ax2, typename IterHandler> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(iter_handler, right_ref); } \
	\
	template<typename Mtx2, typename Ax2, typename IterHandler> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				safe_list<value_type, Mtx2, Ax2, __list__>& right) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		typedef typename right_base_type::iterator (right_base_type::* iter_foo_type)(void); \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename right_base_type::iterator>)); \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			reinterpret_cast<right_base_type&>(_base), right, \
			iter_handler, \
			boost::bind(static_cast<iter_foo_type>(&right_base_type::begin), _1), \
			boost::bind(static_cast<iter_foo_type>(&right_base_type::end), _1) ); } \
	\
	template<typename Mtx2, typename Ax2, typename DstIterHandler, typename SrcEIterHandler> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(const DstIterHandler& dst_iter_handler, \
				BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				const SrcEIterHandler& src_eins_iter_handler) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(dst_iter_handler, right_ref, src_eins_iter_handler); } \
	\
	template<typename Mtx2, typename Ax2, typename DstIterHandler, typename SrcEIterHandler> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(const DstIterHandler& dst_iter_handler, \
				safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				const SrcEIterHandler& src_eins_iter_handler) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename right_base_type::iterator>)); \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			reinterpret_cast<right_base_type&>(_base), right, \
			dst_iter_handler, src_eins_iter_handler ); } \
	\
	template<typename Mtx2, typename Ax2, \
				typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		splice(const DstIterHandler& dst_iter_handler, \
				BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				const SrcEIterHandler& src_eins_iter_handler, \
				const SrcZIterHandler& src_zwei_iter_handler) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::splice(dst_iter_handler, right_ref, src_eins_iter_handler, src_zwei_iter_handler); } \
	\
	template<typename Mtx2, typename Ax2, \
				typename DstIterHandler, typename SrcEIterHandler, typename SrcZIterHandler> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
			splice(const DstIterHandler& dst_iter_handler, \
					safe_list<value_type, Mtx2, Ax2, __list__>& right, \
					const SrcEIterHandler& src_eins_iter_handler, \
					const SrcZIterHandler& src_zwei_iter_handler) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename right_base_type::iterator>)); \
		write_lock_type lk(_mutex); \
		this_type::prv_s_right_using_handler_splice( \
			reinterpret_cast<right_base_type&>(_base), right, \
			dst_iter_handler, \
			src_eins_iter_handler, \
			src_zwei_iter_handler ); }

#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_SPLICE_IMPL() \
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					boost::container::list<value_type, Ax2> \
				BOOST_RV_REF_END right) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		this_type::splice(reinterpret_cast<base_base_type&>(right_ref)); } \
	\
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(boost::container::list<value_type, Ax2>& right) { \
		this_type::splice(reinterpret_cast<base_base_type&>(right)); } \
	\
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					boost::container::list<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				typename boost::container::list<value_type, Ax2>::iterator i) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename boost::container::list<value_type, Ax2>::iterator>)); \
		right_type& right_ref = right; \
		this_type::splice(reinterpret_cast<base_base_type&>(right_ref), i); } \
	\
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(boost::container::list<value_type, Ax2>& right, iterator i) { \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename boost::container::list<value_type, Ax2>::iterator>)); \
		this_type::splice(reinterpret_cast<base_base_type&>(right), i); } \
	\
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(BOOST_RV_REF_BEG \
					boost::container::list<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				typename boost::container::list<value_type, Ax2>::iterator first, \
				typename boost::container::list<value_type, Ax2>::iterator last) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename boost::container::list<value_type, Ax2>::iterator>)); \
		right_type& right_ref = right; \
		this_type::splice(reinterpret_cast<base_base_type&>(right_ref), first, last); } \
	\
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(boost::container::list<value_type, Ax2>& right, \
				typename boost::container::list<value_type, Ax2>::iterator first, \
				typename boost::container::list<value_type, Ax2>::iterator last) { \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename boost::container::list<value_type, Ax2>::iterator>)); \
		this_type::splice(reinterpret_cast<base_base_type&>(right), first, last); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				BOOST_RV_REF_BEG \
					boost::container::list<value_type, Ax2> \
				BOOST_RV_REF_END right) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		this_type::splice(iter_handler, reinterpret_cast<base_base_type&>(right_ref)); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				boost::container::list<value_type, Ax2>& right) { \
		this_type::splice(iter_handler, reinterpret_cast<base_base_type&>(right)); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				BOOST_RV_REF_BEG \
					boost::container::list<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				typename boost::container::list<value_type, Ax2>::iterator i) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename boost::container::list<value_type, Ax2>::iterator>)); \
		right_type& right_ref = right; \
		this_type::splice(iter_handler, reinterpret_cast<base_base_type&>(right_ref), i); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				boost::container::list<value_type, Ax2>& right, \
				typename boost::container::list<value_type, Ax2>::iterator i) { \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename boost::container::list<value_type, Ax2>::iterator>)); \
		this_type::splice(iter_handler, reinterpret_cast<base_base_type&>(right), i); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				BOOST_RV_REF_BEG \
					boost::container::list<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				typename boost::container::list<value_type, Ax2>::iterator first, \
				typename boost::container::list<value_type, Ax2>::iterator last) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename boost::container::list<value_type, Ax2>::iterator>)); \
		right_type& right_ref = right; \
		this_type::splice(iter_handler, reinterpret_cast<base_base_type&>(right_ref), first, last); } \
	\
	template<typename IterHandler, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		splice(const IterHandler& iter_handler, \
				boost::container::list<value_type, Ax2>& right, \
				typename boost::container::list<value_type, Ax2>::iterator first, \
				typename boost::container::list<value_type, Ax2>::iterator last) { \
		BOOST_MPL_ASSERT((boost::is_same<iterator, typename boost::container::list<value_type, Ax2>::iterator>)); \
		this_type::splice(iter_handler, reinterpret_cast<base_base_type&>(right), first, last); }

#define YGGR_PP_SAFE_LIST_MERGE_IMPL() \
	YGGR_PP_SAFE_CONTAINER_SEQ_MERGE_IMPL()

#define YGGR_PP_SAFE_LIST_ISOMERIC_SAME_MERGE_IMPL( __list__ ) \
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		merge(BOOST_RV_REF_BEG \
					__list__<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef __list__<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		this_type::merge( \
			reinterpret_cast<base_type&>(right_ref), \
			need_sort_self, need_sort_other); } \
	\
	template<typename Ax2> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		merge(__list__<value_type, Ax2>& right, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef __list__<value_type, Ax2> right_type; \
		if(need_sort_other){ right.sort(); } \
		write_lock_type lk(_mutex); \
		if(need_sort_self) { _base.sort(); } \
		_base.merge(reinterpret_cast<base_type&>(right)); } \
	\
	template<typename Ax2, typename Pred> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		merge(BOOST_RV_REF_BEG \
					__list__<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				Pred comp, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef __list__<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		this_type::merge( \
			reinterpret_cast<base_type&>(right_ref), comp, \
			need_sort_self, need_sort_other); } \
	\
	template<typename Ax2, typename Pred> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				__list__<value_type, Ax2>, \
				base_type >, \
			void >::type \
		merge(__list__<value_type, Ax2>& right, \
				Pred comp, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		if(need_sort_other) { right.sort(comp); } \
		write_lock_type lk(_mutex); \
		if(need_sort_other) { _base.sort(comp); } \
		_base.merge(reinterpret_cast<base_type&>(right), comp); } \
	\
	template<typename Mtx2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		merge(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::merge(right_ref, need_sort_self, need_sort_other); } \
	\
	template<typename Mtx2, typename Ax2> \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		merge(safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		write_lock_type lk(_mutex); \
		if(need_sort_self) { _base.sort(); } \
		right.using_handler( \
			boost::bind( \
				&this_type::prv_s_handler_merge_eins<base_type>, \
				boost::ref(_base), _1, need_sort_other)); } \
	\
	template<typename Mtx2, typename Ax2, typename Pred> inline \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		merge(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				Pred comp, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::merge(right_ref, comp, need_sort_self, need_sort_other); } \
	\
	template<typename Mtx2, typename Ax2, typename Pred> \
	typename \
		boost::enable_if< \
			boost::is_same< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		merge(safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				Pred comp, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef Pred cmp_type; \
		typedef utility::args_holder_nv_ref_wrap<cmp_type const> cmp_holder_type; \
		cmp_holder_type cmp_holder(comp); \
		write_lock_type lk(_mutex); \
		if(need_sort_self) { _base.sort(comp); } \
		right.using_handler( \
			boost::bind( \
				&this_type::prv_s_handler_merge_zwei<base_type, cmp_type>, \
				boost::ref(_base), _1, \
				boost::cref(utility::args_holder_trans(cmp_holder)), \
				need_sort_other)); } \
	\
	template<typename Mtx2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		merge(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::merge(right_ref, need_sort_self, need_sort_other); } \
	\
	template<typename Mtx2, typename Ax2> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		merge(safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		write_lock_type lk(_mutex); \
		if(need_sort_self) { _base.sort(); } \
		right.using_handler( \
			boost::bind( \
				&this_type::prv_s_handler_merge_eins<right_base_type>, \
				boost::ref(reinterpret_cast<right_base_type&>(_base)), _1, need_sort_other)); } \
	\
	template<typename Mtx2, typename Ax2, typename Pred> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		merge(BOOST_RV_REF_BEG \
					safe_list<value_type, Mtx2, Ax2, __list__> \
				BOOST_RV_REF_END right, \
				Pred comp, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		right_type& right_ref = right; \
		this_type::merge(right_ref, comp, need_sort_self, need_sort_other); } \
	\
	template<typename Mtx2, typename Ax2, typename Pred> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list<value_type, Mtx2, Ax2, __list__>::base_type, \
				base_type >, \
			void >::type \
		merge(safe_list<value_type, Mtx2, Ax2, __list__>& right, \
				Pred comp, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef safe_list<value_type, Mtx2, Ax2, __list__> right_type; \
		typedef typename right_type::base_type right_base_type; \
		typedef Pred cmp_type; \
		typedef utility::args_holder_nv_ref_wrap<cmp_type const> cmp_holder_type; \
		cmp_holder_type cmp_holder(comp); \
		write_lock_type lk(_mutex); \
		if(need_sort_self) { _base.sort(comp); } \
		right.using_handler( \
			boost::bind( \
				&this_type::prv_s_handler_merge_zwei<right_base_type, cmp_type>, \
				boost::ref(reinterpret_cast<right_base_type&>(_base)), _1, \
				boost::cref(utility::args_holder_trans(cmp_holder)), \
				need_sort_other)); }

#define YGGR_PP_SAFE_LIST_OPT_BASE_BASE_MERGE_IMPL() \
	inline void merge(BOOST_RV_REF(base_base_type) right, \
						bool need_sort_self = true, \
						bool need_sort_other = true) { \
		base_base_type& right_ref = right; \
		this_type::merge(right_ref, need_sort_self, need_sort_other); } \
	\
	void merge(base_base_type& right, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		if(need_sort_other){ right.sort(); } \
		write_lock_type lk(_mutex); \
		if(need_sort_self) { _base.sort(); } \
		_base.merge(right); } \
	\
	template<typename Pred> inline \
	void merge(BOOST_RV_REF(base_base_type) right, Pred comp, \
				bool need_sort_self = true, bool need_sort_other = true) { \
		base_base_type& right_ref = right; \
		this_type::merge(right_ref, comp, need_sort_self, need_sort_other); } \
	\
	template<typename Pred> \
	void merge(base_base_type& right, Pred comp, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		if(need_sort_other) { right.sort(comp); } \
		write_lock_type lk(_mutex); \
		if(need_sort_other) { _base.sort(comp); } \
		_base.merge(right, comp); }

#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_MERGE_IMPL() \
	template<typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		merge(BOOST_RV_REF_BEG \
					boost::container::list<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		this_type::merge( \
			reinterpret_cast<base_base_type&>(right_ref), \
			need_sort_self, need_sort_other); } \
	\
	template<typename Ax2> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		merge(boost::container::list<value_type, Ax2>& right, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		if(need_sort_other){ right.sort(); } \
		write_lock_type lk(_mutex); \
		if(need_sort_self) { _base.sort(); } \
		_base.merge(reinterpret_cast<base_base_type&>(right)); } \
	\
	template<typename Ax2, typename Pred> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		merge(BOOST_RV_REF_BEG \
					boost::container::list<value_type, Ax2> \
				BOOST_RV_REF_END right, \
				Pred comp, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		typedef boost::container::list<value_type, Ax2> right_type; \
		right_type& right_ref = right; \
		this_type::merge( \
			reinterpret_cast<base_base_type&>(right_ref), comp, \
			need_sort_self, need_sort_other); } \
	\
	template<typename Ax2, typename Pred> \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list<value_type, Ax2>, \
				base_base_type >, \
			void >::type \
		merge(boost::container::list<value_type, Ax2>& right, \
				Pred comp, \
				bool need_sort_self = true, \
				bool need_sort_other = true) { \
		if(need_sort_other) { right.sort(comp); } \
		write_lock_type lk(_mutex); \
		if(need_sort_other) { _base.sort(comp); } \
		_base.merge(reinterpret_cast<base_base_type&>(right), comp); }

// copy_to_base

#define YGGR_PP_SAFE_LIST_OPT_BASE_BASE_COPY_TO_BASE_IMPL() \
	base_base_type& copy_to_base(base_base_type& out) const { \
		if(boost::addressof(_base) == boost::addressof(out)) { return out; } \
		{read_lock_type lk(_mutex); \
		out = _base; } \
		return out; }

#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_COPY_TO_BASE_IMPL() \
	template<typename Val2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list< Val2, Ax2 >, \
				base_base_type >, \
			boost::container::list< Val2, Ax2 >& >::type \
		copy_to_base( boost::container::list< Val2, Ax2 >& out ) const { \
		this_type::copy_to_base(reinterpret_cast<base_base_type&>(out)); \
		return out; }

// load
#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_LOAD_IMPL() \
	template<typename Cont> \
	typename boost::enable_if< boost::is_same<Cont, base_base_type>, Cont>::type \
		load(void) const { \
		read_lock_type lk(_mutex); \
		return static_cast<const base_base_type&>(_base); } \
	\
	template<typename Cont> \
	typename boost::enable_if< boost::is_same<Cont, base_base_type>, Cont>::type \
		load(const allocator_type& alloc) const { \
		typedef Cont ret_type; \
		read_lock_type lk(_mutex); \
		return ret_type(static_cast<const base_base_type&>(_base), alloc); } \
	\
	template<typename Cont> \
	typename boost::enable_if< container::is_isomeric_same_list<Cont, base_base_type>, Cont>::type \
		load(void) const { \
		typedef Cont ret_type; \
		allocator_type alloc = this_type::get_allocator(); \
		read_lock_type lk(_mutex); \
		return ret_type(reinterpret_cast<const ret_type&>(_base), alloc); } \
	\
	template<typename Cont> \
	typename boost::enable_if< container::is_isomeric_same_list<Cont, base_base_type>, Cont>::type \
		load(const allocator_type& alloc) const { \
		typedef Cont ret_type; \
		read_lock_type lk(_mutex); \
		return ret_type(reinterpret_cast<const ret_type&>(_base), alloc); }

//store
#define YGGR_PP_SAFE_LIST_OPT_BASE_BASE_STONE_IMPL() \
	inline void store(BOOST_RV_REF(base_base_type) base) { \
		this_type::operator=(boost::move(base)); } \
	\
	inline void store(const base_base_type& base) { \
		this_type::operator=(base); }

#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_STONE_IMPL() \
	template<typename Val2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list< Val2, Ax2 >, \
				base_base_type >, \
			void >::type \
		store(BOOST_RV_REF_BEG \
				boost::container::list< Val2, Ax2 > \
				BOOST_RV_REF_END base) { \
		typedef boost::container::list< Val2, Ax2 > right_type; \
		right_type& right_ref = base; \
		this_type::store(boost::move(reinterpret_cast<base_base_type&>(right_ref))); } \
	\
	template<typename Val2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list< Val2, Ax2 >, \
				base_base_type >, \
			void >::type \
		store(const boost::container::list< Val2, Ax2 >& right) { \
		this_type::store(reinterpret_cast<const base_base_type&>(right)); }

// other
#define YGGR_PP_SAFE_LIST_SAFE_OTHER_IMPL() \
	bool is_exists(const value_type& val) const { \
		read_lock_type lk(_mutex); \
		return std::find(_base.begin(), _base.end(), val) != _base.end(); } \
	\
	template<typename Pred> \
	bool is_exists(const Pred& pred) const { \
		read_lock_type lk(_mutex); \
		return std::find_if(_base.begin(), _base.end(), pred) != _base.end(); } 

#define YGGR_PP_SAFE_LIST_CLEAR_IMPL() \
	void clear(void) { \
		base_type tmp(this_type::get_allocator()); \
		{ write_lock_type lk(_mutex); \
			_base.swap(tmp); } }

// compare

#define YGGR_PP_SAFE_LIST_OPT_BASE_BASE_COMPARER_FUNCTION( __op__, __foo_name__) \
	template< typename Val, typename Mtx, typename Ax > inline \
	bool __op__( const boost::container::list<Val, Ax>& l, \
					const safe_list<Val, Mtx, Ax, yggr::container::detail::list>& r ) { \
		typedef boost::container::list<Val, Ax> base_base_type; \
		return r.using_handler( boost::bind(&__foo_name__<base_base_type, base_base_type>, boost::cref(l), _1) ); } \
		\
	template< typename Val, typename Mtx, typename Ax > inline \
	bool __op__( const safe_list<Val, Mtx, Ax, yggr::container::detail::list>& l, \
					const boost::container::list<Val, Ax>& r ) { \
		typedef boost::container::list<Val, Ax> base_base_type; \
		return l.using_handler( boost::bind(&__foo_name__<base_base_type, base_base_type>, _1, boost::cref(r) ) ); }

#define YGGR_PP_SAFE_LIST_OPT_BASE_BASE_EQUALTO_AND_NOTEQUALTO() \
	YGGR_PP_SAFE_LIST_OPT_BASE_BASE_COMPARER_FUNCTION(operator==, yggr::any_val::operator_wrap::equal_to) \
	YGGR_PP_SAFE_LIST_OPT_BASE_BASE_COMPARER_FUNCTION(operator!=, yggr::any_val::operator_wrap::not_equal_to)

#define YGGR_PP_SAFE_LIST_OPT_BASE_BASE_COMPARER_FULL() \
	YGGR_PP_SAFE_LIST_OPT_BASE_BASE_EQUALTO_AND_NOTEQUALTO() \
	YGGR_PP_SAFE_LIST_OPT_BASE_BASE_COMPARER_FUNCTION(operator<, yggr::any_val::operator_wrap::less) \
	YGGR_PP_SAFE_LIST_OPT_BASE_BASE_COMPARER_FUNCTION(operator<=, yggr::any_val::operator_wrap::less_equal) \
	YGGR_PP_SAFE_LIST_OPT_BASE_BASE_COMPARER_FUNCTION(operator>, yggr::any_val::operator_wrap::greater) \
	YGGR_PP_SAFE_LIST_OPT_BASE_BASE_COMPARER_FUNCTION(operator>=, yggr::any_val::operator_wrap::greater_equal)


#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_COMPARER_FUNCTION( __op__, __op_symbol__ ) \
	template<typename Val1, typename Mtx, typename Ax1, typename Val2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list< Val1, Ax1 >, \
				typename safe_list< Val2, Mtx, Ax2, yggr::container::detail::list >::base_base_type >, \
			bool >::type \
		__op__( const boost::container::list< Val1, Ax1 >& l, \
				const safe_list< Val2, Mtx, Ax2, yggr::container::detail::list >& r) { \
		typedef safe_list< Val2, Mtx, Ax2, yggr::container::detail::list > safe_type; \
		typedef typename safe_type::base_base_type base_base_type; \
		return reinterpret_cast<const base_base_type&>(l) __op_symbol__ r; } \
	\
	template<typename Val1, typename Mtx, typename Ax1, typename Val2, typename Ax2> inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				typename safe_list< Val1, Mtx, Ax1, yggr::container::detail::list >::base_base_type, \
				boost::container::list< Val2, Ax2 > >, \
			bool >::type \
		__op__( const safe_list< Val1, Mtx, Ax1, yggr::container::detail::list >& l, \
				const boost::container::list< Val2, Ax2 >& r ) { \
		typedef safe_list< Val1, Mtx, Ax1, yggr::container::detail::list > safe_type; \
		typedef typename safe_type::base_base_type base_base_type; \
		return l __op_symbol__ reinterpret_cast<const base_base_type&>(r); }


#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_EQUALTO_AND_NOTEQUALTO() \
	YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_COMPARER_FUNCTION( operator==, == ) \
	YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_COMPARER_FUNCTION( operator!=, != )

#define YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_COMPARER_FULL() \
	YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_EQUALTO_AND_NOTEQUALTO() \
	YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_COMPARER_FUNCTION( operator<, < ) \
	YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_COMPARER_FUNCTION( operator<=, <= ) \
	YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_COMPARER_FUNCTION( operator>, > ) \
	YGGR_PP_SAFE_LIST_OPT_ISOMERIC_SAME_BASE_BASE_COMPARER_FUNCTION( operator>=, >= )

// swap
#define YGGR_PP_SAFE_LIST_OPT_SWAP() \
	template< typename Val, typename Mtx, typename Ax > inline \
	void swap( boost::container::list<Val, Ax>& l, \
				safe_list<Val, Mtx, Ax, yggr::container::detail::list>& r ) { \
		r.swap( l ); } \
		\
	template< typename Val, typename Mtx, typename Ax > inline \
	void swap( safe_list<Val, Mtx, Ax, yggr::container::detail::list>& l, \
				boost::container::list<Val, Ax>& r ) { \
		l.swap( r ); }


#define YGGR_PP_SAFE_LIST_ISOMERIC_SAME_BASE_BASE_SWAP() \
	template< typename Val1, typename Mtx, typename Ax1, typename Val2, typename Ax2 > inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list< Val1, Ax1 >, \
				typename safe_list< Val2, Mtx, Ax2, yggr::container::detail::list >::base_base_type >, \
			void >::type \
		swap( boost::container::list< Val1, Ax1 >& l, \
				safe_list< Val2, Mtx, Ax2, yggr::container::detail::list >& r) { \
		typedef safe_list< Val2, Mtx, Ax2, yggr::container::detail::list > safe_type; \
		typedef typename safe_type::base_base_type base_base_type; \
		r.swap(reinterpret_cast<base_base_type&>(l)); } \
	\
	template< typename Val1, typename Mtx, typename Ax1, typename Val2, typename Ax2 > inline \
	typename \
		boost::enable_if< \
			container::is_isomeric_same_list< \
				boost::container::list< Val2, Ax2 >, \
				typename safe_list< Val1, Mtx, Ax1, yggr::container::detail::list >::base_base_type >, \
			void >::type \
		swap( safe_list< Val1, Mtx, Ax1, yggr::container::detail::list >& l, \
				boost::container::list< Val2, Ax2 >& r ) { \
		typedef safe_list< Val1, Mtx, Ax1, yggr::container::detail::list > safe_type; \
		typedef typename safe_type::base_base_type base_base_type; \
		l.swap(reinterpret_cast<base_base_type&>(r)); }

#endif // __YGGR_SAFE_CONTAINER_LIST_SAFE_LIST_MEMBER_IMPL_HPP__
