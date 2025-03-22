//insert_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_DETAIL_INSERT_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_DETAIL_INSERT_IMPL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/yggr_msg_macro.h>
#include <yggr/type_traits/is_pair.hpp>
#include <yggr/compatibility/stl_exception.hpp>
#include <yggr/move/move.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/mplex/static_assert.hpp>

#include <yggr/func/is_callable.hpp>
#include <yggr/func/foo_t_info.hpp>

#include <yggr/container/vector.hpp>
#include <yggr/iterator_ex/is_iterator.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>

#include <iterator>

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST
#   include <initializer_list>
#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST


#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

// initializer_list version
// seq
#	define YGGR_PP_SAFE_SEQ_INSERT_FUNCTION_INITLIST_IMPL() \
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, \
						std::initializer_list<value_type> l) { \
			write_lock_type lk(_mutex); \
			try{ \
				_base.insert(iter_handler(_base), l.begin(), l.end()); \
				return true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename IterHandler, typename Handler> \
		typename \
			boost::enable_if< \
				::yggr::func::is_callable<Handler>, \
				typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			insert(const IterHandler& iter_handler, \
						std::initializer_list<value_type> l, \
						const Handler& handler ) { \
			iterator iter; \
			bool bright = false; \
			write_lock_type lk(_mutex); \
			try { \
					iter = _base.insert(iter_handler(_base), l.begin(), l.end()); \
					bright = true; } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
			return handler(_base, std::make_pair(iter, bright)); }

//---------------------
// kv
#   define YGGR_PP_SAFE_KV_INSERT_FUNCTION_INITLIST_IMPL() \
		inline bool insert(std::initializer_list<value_type> l) { \
           return this_type::insert(l.begin(), l.end()); }

#else

#	define YGGR_PP_SAFE_SEQ_INSERT_FUNCTION_INITLIST_IMPL()

#	define YGGR_PP_SAFE_KV_INSERT_FUNCTION_INITLIST_IMPL()

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST


namespace yggr
{
namespace safe_container
{
namespace detail
{

// return iterator version
template<typename Base, typename T>
struct call_seq_insert_helper
{
	typedef Base base_type;
	typedef typename base_type::iterator iterator;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::value_type value_type;
	typedef T result_type;

	template<typename Iter> inline
	std::pair<result_type, bool>
		operator()(base_type& base, Iter pos, BOOST_RV_REF(value_type) val) const
	{
		try
		{
			return std::make_pair(base.insert(pos, boost::move(val)), true);
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
			return std::make_pair(base.end(), false);
		}
	}

	template<typename Iter> inline
	std::pair<result_type, bool>
		operator()(base_type& base, Iter pos, const value_type& val) const
	{
		try
		{
			return std::make_pair(base.insert(pos, val), true);
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
			return std::make_pair(base.end(), false);
		}
	}

	template<typename Iter> inline
	std::pair<result_type, bool>
		operator()(base_type& base, Iter pos, size_type n, const value_type& val) const
	{
		try
		{
			return std::make_pair(base.insert(pos, n, val), true);
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
			return std::make_pair(base.end(), false);
		}
	}

	template<typename Iter, typename Iter1, typename Iter2> inline
	std::pair<result_type, bool>
		operator()(base_type& base, Iter pos, Iter1 first, Iter2 last) const
	{
		try
		{
			return std::make_pair(base.insert(pos, first, last), true);
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
			return std::make_pair(base.end(), false);
		}
	}

	template<typename Iter, typename Iter1, typename Iter2> inline
	std::pair<result_type, bool>
		operator()(base_type& base, Iter pos, const std::pair<Iter1, Iter2>& pos_pair) const
	{
		return call_seq_insert_helper::operator()(base, pos, pos_pair.first, pos_pair.second);
	}
};

// return void version // kv c98
template<typename Base>
struct call_seq_insert_helper<Base, boost::mpl::void_>
{
	typedef Base base_type;
	typedef typename base_type::iterator iterator;
	typedef typename base_type::size_type size_type;
	typedef typename base_type::value_type value_type;
	typedef iterator result_type;

	template<typename Iter> inline
	std::pair<result_type, bool>
		operator()(base_type& base, Iter pos, BOOST_RV_REF(value_type) val) const
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::make_pair(base.end(), false);
	}

	template<typename Iter> inline
	std::pair<result_type, bool>
		operator()(base_type& base, Iter pos, const value_type& val) const
	{
		YGGR_MPLEX_FXIED_ASSERT_FALSE();
		return std::make_pair(base.end(), false);
	}

	template<typename Iter> inline
	std::pair<result_type, bool>
		operator()(base_type& base, Iter pos, size_type n, const value_type& val) const
	{
		size_type s = std::distance(base.begin(), pos);
		bool bright = false;
		iterator iter;
		try
		{
			base.insert(pos, n, val);
			bright = true;
			iter = base.begin();
			std::advance(iter, s);
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
			bright = false;
			iter = base.end();
		}

		return std::make_pair(iter, bright);
	}

	template<typename Iter, typename Iter1, typename Iter2> inline
	std::pair<result_type, bool>
		operator()(base_type& base, Iter pos, Iter1 first, Iter2 last) const
	{
		size_type s = std::distance(base.begin(), pos);
		bool bright = false;
		iterator iter;
		try
		{
			base.insert(pos, first, last);
			bright = true;
			iter = base.begin();
			std::advance(iter, s);
		}
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR())
		{
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what()))
			bright = false;
			iter = base.end();
		}

		return std::make_pair(iter, bright);
	}

	template<typename Iter, typename Iter1, typename Iter2> inline
	std::pair<result_type, bool>
		operator()(base_type& base, Iter pos, const std::pair<Iter1, Iter2>& pos_pair) const
	{
		return call_seq_insert_helper::operator()(base, pos, pos_pair.first, pos_pair.second);
	}
};

} // namespace detail
} // namespace safe_container
} // namespace yggr


// seq
#define YGGR_PP_SAFE_SEQ_INSERT_FUNCTION_IMPL() \
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, \
					BOOST_RV_REF(value_type) val) { \
		typedef detail::call_seq_insert_helper<base_type, iterator> helper_type; \
		helper_type h; \
		write_lock_type lk(_mutex); \
		return h(_base, iter_handler(_base), boost::move(val)).second; } \
	\
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, \
				const value_type& val) { \
		typedef detail::call_seq_insert_helper<base_type, iterator> helper_type; \
		helper_type h; \
		write_lock_type lk(_mutex); \
		return h(_base, iter_handler(_base), val).second; } \
	\
	template<typename IterHandler, typename Handler> \
	typename \
		boost::enable_if< \
			::yggr::func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		insert(const IterHandler& iter_handler, \
				BOOST_RV_REF(value_type) val, \
				const Handler& handler) { \
		typedef detail::call_seq_insert_helper<base_type, iterator> helper_type; \
		helper_type h; \
		write_lock_type lk(_mutex); \
		return handler(_base, h(_base, iter_handler(_base), boost::move(val))); } \
	\
	template<typename IterHandler, typename Handler> \
	typename \
		boost::enable_if< \
			::yggr::func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		insert(const IterHandler& iter_handler, \
				const value_type& val, \
				const Handler& handler) { \
		typedef detail::call_seq_insert_helper<base_type, iterator> helper_type; \
		helper_type h; \
		write_lock_type lk(_mutex); \
		return handler(_base, h(_base, iter_handler(_base), val)); } \
	\
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, \
					size_type n, const value_type& val) { \
		typedef typename container::detail::insert_result<base_type>::type result_type; \
		typedef detail::call_seq_insert_helper<base_type, result_type> helper_type; \
		helper_type h; \
		write_lock_type lk(_mutex); \
		return h(_base, iter_handler(_base), n, val).second; } \
	\
	template<typename IterHandler, typename Handler> \
	typename \
		boost::enable_if< \
			yggr::func::is_callable<Handler>, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		insert(const IterHandler& iter_handler, \
					size_type n, const value_type& val, \
					const Handler& handler) { \
		typedef typename container::detail::insert_result<base_type>::type result_type; \
		typedef detail::call_seq_insert_helper<base_type, result_type> helper_type; \
		helper_type h; \
		write_lock_type lk(_mutex); \
		return handler(_base, h(_base, iter_handler(_base), n, val)); } \
	\
	template<typename IterHandler, typename InputIter> \
	typename boost::enable_if<yggr::iterator_ex::is_iterator<InputIter>, bool>::type \
		insert(const IterHandler& iter_handler, \
					InputIter s, InputIter e ) { \
		typedef typename container::detail::insert_result<base_type, InputIter>::type result_type; \
		typedef detail::call_seq_insert_helper<base_type, result_type> helper_type; \
		helper_type h; \
		write_lock_type lk(_mutex); \
		return h(_base, iter_handler(_base), s, e).second; } \
	\
	template<typename IterHandler, typename InputIter, typename Handler> \
	typename \
		boost::enable_if< \
			boost::mpl::and_< \
				::yggr::iterator_ex::is_iterator<InputIter>, \
				::yggr::func::is_callable<Handler> >, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		insert(const IterHandler& iter_handler, \
				InputIter s, InputIter e, \
				const Handler& handler ) { \
		typedef typename container::detail::insert_result<base_type, InputIter>::type result_type; \
		typedef detail::call_seq_insert_helper<base_type, result_type> helper_type; \
		helper_type h; \
		write_lock_type lk(_mutex); \
		return handler(_base, h(_base, iter_handler(_base), s, e) ).second; } \
	\
	YGGR_PP_SAFE_SEQ_INSERT_FUNCTION_INITLIST_IMPL()

// kv

#define YGGR_PP_SAFE_KV_INSERT_FUNCTION_IMPL() \
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, \
					BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try { return \
				_base.insert(iter_handler(_base), boost::move(val)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename IterHandler> \
	bool insert(const IterHandler& iter_handler, \
				const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { return _base.insert(iter_handler(_base), val) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename IterHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, \
				BOOST_RV_REF(value_type) val, \
				const Handler& handler) { \
		iterator iter; \
		bool bright = false; \
		write_lock_type lk(_mutex); \
		try { iter = _base.insert(iter_handler(_base), boost::move(val)); bright = true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
		return handler(_base, std::make_pair(iter, bright)); } \
	\
	template<typename IterHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, \
				const value_type& val, \
				const Handler& handler) { \
		iterator iter; \
		bool bright = false; \
		write_lock_type lk(_mutex); \
		try { iter = _base.insert(iter_handler(_base), val); bright = true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
		return handler(_base, std::make_pair(iter, bright)); } \
	\
	template<typename InputIter> \
	bool insert(InputIter start, InputIter last) { \
		write_lock_type lk(_mutex); \
		try { _base.insert(start, last); return true;} \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	YGGR_PP_SAFE_KV_INSERT_FUNCTION_INITLIST_IMPL()

// kv unique

#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_NODE_FUNCTION_IMPL() \
	protected: \
		template<typename BaseCont, typename Node, typename BaseInsetRet, typename Nil = void> \
		struct insert_node_helper { \
		public: \
			typedef BaseCont base_cont_type; \
			typedef Node node_type; \
			typedef BaseInsetRet base_insert_return_type; \
		private: \
			typedef insert_node_helper this_type; \
		public: \
			inline bool check_node(const node_type& node) const { return !!node; } \
			\
			inline base_insert_return_type insert_node(base_cont_type& base_cont, \
														BOOST_RV_REF(node_type) node) const { \
				assert((this_type::check_node(boost::forward<node_type>(node)))); \
				try { return container::insert(base_cont, boost::move(node)); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return base_insert_return_type(); } } \
			\
			template<typename RetHandler> inline \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type \
				insert_node(base_cont_type& base_cont, \
							BOOST_RV_REF(node_type) node,\
							const RetHandler& ret_handler) const { \
				base_insert_return_type base_iret; \
				if(this_type::check_node(boost::forward<node_type>(node))) { \
					try { base_iret = container::insert(base_cont, boost::move(node)); } \
					catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
						__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())); } } \
				return ret_handler(base_cont, base_iret); } \
			\
			template<typename Iter> inline \
			bool insert_node_hint(base_cont_type& base_cont, Iter iter, BOOST_RV_REF(node_type) node) const { \
				assert((this_type::check_node(boost::forward<node_type>(node)))); \
				try { return container::insert(base_cont, iter, boost::move(node)) != base_cont.end(); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
			\
			template<typename Iter, typename RetHandler> \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type \
				insert_node_hint(base_cont_type& base_cont, \
									Iter iter, BOOST_RV_REF(node_type) node, \
									const RetHandler& ret_handler) { \
				typedef typename base_cont_type::iterator iterator; \
				std::pair<iterator, bool> ret(iterator(), false); \
				if(this_type::check_node(boost::forward<node_type>(node))) { \
					try { ret.first = container::insert(base_cont, iter, boost::move(node)); \
							ret.second = ret.first != base_cont.end(); } \
					catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
						__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = base_cont.end(); } } \
				return ret_handler(base_cont, ret); } }; \
		\
		template<typename BaseCont, typename Nil> \
		struct failed_insert_node_helper { \
		public: \
			typedef BaseCont base_cont_type; \
			typedef mplex::null_type node_type; \
			typedef mplex::null_type base_insert_return_type; \
		private: \
			BOOST_MPL_ASSERT_MSG((false), current_base_container_not_support_node_insert, (base_cont_type)); \
		public: \
			inline bool check_node(const node_type&) const { return false; } \
			\
			inline base_insert_return_type insert_node(base_cont_type&, BOOST_RV_REF(node_type) ) const { \
				return insert_return_type(); } \
			\
			template<typename RetHandler> inline \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type \
				insert_node(base_cont_type& base_cont, \
							BOOST_RV_REF(node_type) , \
							const RetHandler& ret_handler) const { \
				base_insert_return_type base_iret; \
				return ret_handler(base_cont, base_iret); } \
			\
			template<typename Iter> inline \
			bool insert_node_hint(base_cont_type&, const Iter&, BOOST_RV_REF(node_type)) const { \
				return false; } \
			\
			template<typename Iter, typename RetHandler> \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type \
				insert_node_hint(base_cont_type& base_cont, \
									const Iter&, \
									BOOST_RV_REF(node_type), \
									const RetHandler& ret_handler) const { \
				typedef typename base_cont_type::iterator iterator; \
				std::pair<iterator, bool> ret(iterator(), false); \
				return ret_handler(base_cont, ret); } }; \
		\
		template<typename BaseCont, typename BaseInsetRet, typename Nil> \
		struct insert_node_helper<BaseCont, mplex::null_type, BaseInsetRet, Nil> \
			: public failed_insert_node_helper<BaseCont, Nil> { \
		public: \
			typedef failed_insert_node_helper<BaseCont, Nil> base_type; \
		public: \
			typedef typename base_type::base_cont_type base_cont_type; \
			typedef typename base_type::node_type node_type; \
			typedef typename base_type::base_insert_return_type base_insert_return_type; \
		public: \
			using base_type::check_node; \
			using base_type::insert_node; \
			using base_type::insert_node_hint; }; \
		\
		template<typename BaseCont, typename Node, typename Nil> \
		struct insert_node_helper<BaseCont, Node, mplex::null_type, Nil> \
			: public failed_insert_node_helper<BaseCont, Nil> { \
		public: \
			typedef failed_insert_node_helper<BaseCont, Nil> base_type; \
		public: \
			typedef typename base_type::base_cont_type base_cont_type; \
			typedef typename base_type::node_type node_type; \
			typedef typename base_type::base_insert_return_type base_insert_return_type; \
		public: \
			using base_type::check_node; \
			using base_type::insert_node; \
			using base_type::insert_node_hint; }; \
		\
		template<typename BaseCont, typename Nil> \
		struct insert_node_helper<BaseCont, mplex::null_type, mplex::null_type, Nil> \
			: public failed_insert_node_helper<BaseCont, Nil> { \
		public: \
			typedef failed_insert_node_helper<BaseCont, Nil> base_type; \
		public: \
			typedef typename base_type::base_cont_type base_cont_type; \
			typedef typename base_type::node_type node_type; \
			typedef typename base_type::base_insert_return_type base_insert_return_type; \
		public: \
			using base_type::check_node; \
			using base_type::insert_node; \
			using base_type::insert_node_hint; }; \
	\
	public: \
		insert_return_type insert(BOOST_RV_REF(node_type) node) { \
			typedef insert_node_helper<base_type, node_type, base_insert_return_type> helper_type; \
			helper_type helper; \
			if(!helper.check_node(boost::forward<node_type>(node))) { \
				return insert_return_type(); } \
			write_lock_type lk(_mutex); \
			return insert_return_type(helper.insert_node(_base, boost::move(node))); } \
		\
		template<typename RetHandler> \
		typename ::yggr::func::foo_t_info<RetHandler>::result_type \
			insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
			typedef insert_node_helper<base_type, node_type, base_insert_return_type> helper_type; \
			helper_type helper; \
			write_lock_type lk(_mutex); \
			return helper.insert_node(_base, boost::move(node), ret_handler); } \
		\
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node) { \
			typedef insert_node_helper<base_type, node_type, base_insert_return_type> helper_type; \
			helper_type helper; \
			if(!helper.check_node(boost::forward<node_type>(node))) { return false; } \
			write_lock_type lk(_mutex); \
			return helper.insert_node_hint(_base, iter_handler(_base), boost::move(node)); } \
		\
		template<typename IterHandler, typename RetHandler> \
		typename ::yggr::func::foo_t_info<RetHandler>::result_type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
			typedef insert_node_helper<base_type, node_type, base_insert_return_type> helper_type; \
			helper_type helper; \
			write_lock_type lk(_mutex); \
			return helper.insert_node_hint(_base, iter_handler(_base), boost::move(node), ret_handler); }

#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_FUNCTION_IMPL() \
	bool insert(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try { return _base.insert(boost::move(val)).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	bool insert(const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { return _base.insert(val).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(BOOST_RV_REF(value_type) val, \
				const Handler& handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { ret = _base.insert(boost::move(val)); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
		return handler(_base, ret); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const value_type& val, \
				const Handler& handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { ret = _base.insert(val); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
		return handler(_base, ret); } \
	\
	YGGR_PP_SAFE_KV_INSERT_FUNCTION_IMPL() \
	\
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_NODE_FUNCTION_IMPL()

//#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL() \
//	template<typename K, typename M> \
//	bool insert_or_assign(BOOST_RV_REF(K) k, BOOST_FWD_REF(M) m) { \
//		write_lock_type lk(_mutex); \
//		try { return container::insert_or_assign(_base, boost::move(k), boost::forward(m)).second; } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
//	\
//	template<typename K, typename M> \
//	bool insert_or_assign(const K& k, BOOST_FWD_REF(M) m) { \
//		write_lock_type lk(_mutex); \
//		try { return container::insert_or_assign(_base, k, boost::forward<M>(m)).second; } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
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
//		write_lock_type lk(_mutex); \
//		try { ret = container::insert_or_assign(_base, boost::move(k), boost::move(m)); } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } \
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
//		write_lock_type lk(_mutex); \
//		try { ret = container::insert_or_assign(_base, k, boost::move(m)); } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } \
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
//		write_lock_type lk(_mutex); \
//		try { return \
//				container::insert_or_assign( \
//					_base, iter_handler(_base), boost::move(k), boost::move(m)) != _base.end(); } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
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
//		write_lock_type lk(_mutex); \
//		try { return \
//				container::insert_or_assign( \
//					_base, iter_handler(_base), k, boost::move(m)) != _base.end(); } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
//	\
//	template<typename IterHandler, typename K, typename M, typename RetHandler> \
//	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
//		insert_or_assign(const IterHandler& iter_handler, BOOST_RV_REF(K) k, BOOST_RV_REF(M) m, const RetHandler& ret_handler) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		write_lock_type lk(_mutex); \
//		try { ret.first = container::insert_or_assign(_base, iter_handler(_base), boost::move(k), boost::move(m)); \
//				ret.second = ret.first != _base.end(); } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
//		return ret_handler(_base, ret); } \
//	\
//	template<typename IterHandler, typename K, typename M, typename RetHandler> \
//	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
//		insert_or_assign(const IterHandler& iter_handler, const K& k, BOOST_RV_REF(M) m, const RetHandler& ret_handler) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		write_lock_type lk(_mutex); \
//		try { ret.first = container::insert_or_assign(_base, iter_handler(_base), k, boost::move(m)); \
//				ret.second = ret.first != _base.end(); } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
//		return ret_handler(_base, ret); }

//#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
//			__tk__, __tm__, __ak__, __am__, __uak__, __uam__) \
//	template<typename __tk__, typename __tm__> \
//	bool insert_or_assign(__ak__ k, __am__ m) { \
//		write_lock_type lk(_mutex); \
//		try { return container::insert_or_assign(_base, __uak__(k), __uam__(m)).second; } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } }
//
//#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
//			__tk__, __tm__, __ak__, __am__, __uak__, __uam__) \
//	template<typename __tk__, typename __tm__, typename RetHandler> \
//	typename \
//		boost::disable_if< \
//			boost::mpl::and_ < \
//				typename ::yggr::func::foo_t_info<__tk__>::is_callable_type, \
//				boost::mpl::or_< \
//					boost::is_base_of< \
//						iterator, \
//						typename ::yggr::func::foo_t_info<__tk__>::result_type >, \
//					boost::is_base_of< \
//						const_iterator, \
//						typename ::yggr::func::foo_t_info<__tk__>::result_type > > >, \
//			typename ::yggr::func::foo_t_info<RetHandler>::result_type >::type \
//		insert_or_assign(__ak__ k, __am__ m, const RetHandler& ret_handler) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		write_lock_type lk(_mutex); \
//		try { ret = container::insert_or_assign(_base, __uak__(k), __uam__(m)); } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } \
//		return ret_handler(_base, ret); }
//
//#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
//			__tk__, __tm__, __ak__, __am__, __uak__, __uam__) \
//	template<typename IterHandler, typename __tk__, typename __tm__> \
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
//		insert_or_assign(const IterHandler& iter_handler, __ak__ k, __am__ m) { \
//		write_lock_type lk(_mutex); \
//		try { return \
//				container::insert_or_assign( \
//					_base, iter_handler(_base), __uak__(k), __uam__(m)) != _base.end(); } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } }
//
//#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
//			__tk__, __tm__, __ak__, __am__, __uak__, __uam__) \
//	template<typename IterHandler, typename __tk__, typename __tm__, typename RetHandler> \
//	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
//		insert_or_assign(const IterHandler& iter_handler, __ak__ k, __am__ m, const RetHandler& ret_handler) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		write_lock_type lk(_mutex); \
//		try { ret.first = container::insert_or_assign(_base, iter_handler(_base), __uak__(k), __uam__(m)); \
//				ret.second = ret.first != _base.end(); } \
//		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
//			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
//		return ret_handler(_base, ret); }
//
//#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL() \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
//		K, M, BOOST_RV_REF(K), BOOST_RV_REF(M), boost::move, boost::move) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
//		K, M, BOOST_RV_REF(K), const M&, boost::move, YGGR_PP_SYMBOL_EMPTY()) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
//		K, M, const K&, BOOST_RV_REF(M), YGGR_PP_SYMBOL_EMPTY(), boost::move) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
//		K, M, const K&, const M&, YGGR_PP_SYMBOL_EMPTY(), YGGR_PP_SYMBOL_EMPTY()) \
//	\
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
//		K, M, BOOST_RV_REF(K), BOOST_RV_REF(M), boost::move, boost::move) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
//		K, M, BOOST_RV_REF(K), const M&, boost::move, YGGR_PP_SYMBOL_EMPTY()) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
//		K, M, const K&, BOOST_RV_REF(M), YGGR_PP_SYMBOL_EMPTY(), boost::move) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
//		K, M, const K&, const M&, YGGR_PP_SYMBOL_EMPTY(), YGGR_PP_SYMBOL_EMPTY()) \
//	\
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
//		K, M, BOOST_RV_REF(K), BOOST_RV_REF(M), boost::move, boost::move) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
//		K, M, BOOST_RV_REF(K), const M&, boost::move, YGGR_PP_SYMBOL_EMPTY()) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
//		K, M, const K&, BOOST_RV_REF(M), YGGR_PP_SYMBOL_EMPTY(), boost::move) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
//		K, M, const K&, const M&, YGGR_PP_SYMBOL_EMPTY(), YGGR_PP_SYMBOL_EMPTY()) \
//	\
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
//		K, M, BOOST_RV_REF(K), BOOST_RV_REF(M), boost::move, boost::move) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
//		K, M, BOOST_RV_REF(K), const M&, boost::move, YGGR_PP_SYMBOL_EMPTY()) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
//		K, M, const K&, BOOST_RV_REF(M), YGGR_PP_SYMBOL_EMPTY(), boost::move) \
//	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
//		K, M, const K&, const M&, YGGR_PP_SYMBOL_EMPTY(), YGGR_PP_SYMBOL_EMPTY())

#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
			__tk__, __tm__, __ak__, __am__, __uak__) \
	template<typename __tk__, typename __tm__> \
	bool insert_or_assign(__ak__ k, __am__ m) { \
		write_lock_type lk(_mutex); \
		try { return container::insert_or_assign(_base, __uak__(k), boost::forward< __tm__ >(m)).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } }

#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
			__tk__, __tm__, __ak__, __am__, __uak__) \
	template<typename __tk__, typename __tm__, typename RetHandler> \
	typename \
		boost::disable_if< \
			boost::mpl::and_ < \
				typename ::yggr::func::foo_t_info< __tk__ >::is_callable_type, \
				boost::mpl::or_< \
					boost::is_base_of< \
						iterator, \
						typename ::yggr::func::foo_t_info< __tk__ >::result_type >, \
					boost::is_base_of< \
						const_iterator, \
						typename ::yggr::func::foo_t_info< __tk__ >::result_type > > >, \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type >::type \
		insert_or_assign(__ak__ k, __am__ m, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { ret = container::insert_or_assign(_base, __uak__(k), boost::forward< __tm__ >(m)); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) } \
		return ret_handler(_base, ret); }

#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
			__tk__, __tm__, __ak__, __am__, __uak__) \
	template<typename IterHandler, typename __tk__, typename __tm__> \
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
		insert_or_assign(const IterHandler& iter_handler, __ak__ k, __am__ m) { \
		write_lock_type lk(_mutex); \
		try { return \
				container::insert_or_assign( \
					_base, iter_handler(_base), __uak__(k), boost::forward< __tm__ >(m)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } }

#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
			__tk__, __tm__, __ak__, __am__, __uak__) \
	template<typename IterHandler, typename __tk__, typename __tm__, typename RetHandler> \
	typename ::yggr::func::foo_t_info<RetHandler>::result_type \
		insert_or_assign(const IterHandler& iter_handler, __ak__ k, __am__ m, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { ret.first = container::insert_or_assign(_base, iter_handler(_base), __uak__(k),  boost::forward< __tm__ >(m)); \
				ret.second = ret.first != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
		return ret_handler(_base, ret); }

#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL() \
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
		K, M, BOOST_RV_REF(K), BOOST_FWD_REF(M), boost::move) \
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
		K, M, const K&, BOOST_FWD_REF(M), YGGR_PP_SYMBOL_EMPTY()) \
	\
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
		K, M, BOOST_RV_REF(K), BOOST_FWD_REF(M), boost::move) \
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
		K, M, const K&, BOOST_FWD_REF(M), YGGR_PP_SYMBOL_EMPTY()) \
	\
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
		K, M, BOOST_RV_REF(K), BOOST_FWD_REF(M), boost::move) \
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
		K, M, const K&, BOOST_FWD_REF(M), YGGR_PP_SYMBOL_EMPTY()) \
	\
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
		K, M, BOOST_RV_REF(K), BOOST_FWD_REF(M), boost::move) \
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
		K, M, const K&, BOOST_FWD_REF(M), YGGR_PP_SYMBOL_EMPTY())

// kv_non_unique

#define YGGR_PP_SAFE_KV_NON_UNIQUE_INSERT_NODE_FUNCTION_IMPL() \
	protected: \
		template<typename BaseCont, typename Node, typename Nil = void> \
		struct insert_node_helper { \
		public: \
			typedef BaseCont base_cont_type; \
			typedef Node node_type; \
		private: \
			typedef insert_node_helper this_type; \
		public: \
			inline bool check_node(const node_type& node) const { return !!node; } \
			\
			inline bool insert_node(base_cont_type& base_cont, \
									BOOST_RV_REF(node_type) node) const { \
				assert((this_type::check_node(boost::forward<node_type>(node)))); \
				try { return container::insert(base_cont, boost::move(node)) != base_cont.end(); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
			\
			template<typename RetHandler> inline \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type \
				insert_node(base_cont_type& base_cont, \
							BOOST_RV_REF(node_type) node, \
							const RetHandler& ret_handler) const { \
				typedef typename base_cont_type::iterator iterator; \
				std::pair<iterator, bool> ret(iterator(), false); \
				if(this_type::check_node(boost::forward<node_type>(node))) { \
					try { ret.first = container::insert(base_cont, boost::move(node)); \
							ret.second = ret.first != base_cont.end(); } \
					catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
						__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = base_cont.end(); } } \
				return ret_handler(base_cont, ret); } \
			\
			template<typename Iter> inline \
			bool insert_node_hint(base_cont_type& base_cont, Iter iter, BOOST_RV_REF(node_type) node) const { \
				if(!this_type::check_node(boost::forward<node_type>(node))) { return false; } \
				try { return container::insert(base_cont, iter, boost::move(node)) != base_cont.end(); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
			\
			template<typename Iter, typename RetHandler> \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type \
				insert_node_hint(base_cont_type& base_cont, \
									Iter iter, BOOST_RV_REF(node_type) node, \
									const RetHandler& ret_handler) { \
				typedef typename base_cont_type::iterator iterator; \
				std::pair<iterator, bool> ret(iterator(), false); \
				if(this_type::check_node(boost::forward<node_type>(node))) { \
					try { ret.first = container::insert(base_cont, iter, boost::move(node)); \
							ret.second = ret.first != base_cont.end(); } \
					catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
						__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = base_cont.end(); } } \
				return ret_handler(base_cont, ret); } }; \
		\
		template<typename BaseCont, typename Nil> \
		struct insert_node_helper<BaseCont, mplex::null_type, Nil> { \
		public: \
			typedef BaseCont base_cont_type; \
			typedef mplex::null_type node_type; \
		private: \
			BOOST_MPL_ASSERT_MSG((false), current_base_container_not_support_node_insert, (base_cont_type)); \
		public: \
			inline bool check_node(const node_type&) const { return false; } \
			\
			inline bool insert_node(base_cont_type&, BOOST_RV_REF(node_type) ) const { \
				return false; } \
			template<typename RetHandler> inline \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type \
				insert_node(base_cont_type& base_cont, \
							BOOST_RV_REF(node_type), \
							const RetHandler& ret_handler) const { \
				typedef typename base_cont_type::iterator iterator; \
				std::pair<iterator, bool> ret(iterator(), false); \
				return ret_handler(base_cont, ret); } \
			\
			template<typename Iter> inline \
			bool insert_node_hint(base_cont_type&, const Iter&, BOOST_RV_REF(node_type)) const { \
				return false; } \
			\
			template<typename Iter, typename RetHandler> \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type \
				insert_node_hint(base_cont_type& base_cont, \
									const Iter&, \
									BOOST_RV_REF(node_type), \
									const RetHandler& ret_handler) const { \
				typedef typename base_cont_type::iterator iterator; \
				std::pair<iterator, bool> ret(iterator(), false); \
				return ret_handler(base_cont, ret); } }; \
	\
	public: \
		bool insert(BOOST_RV_REF(node_type) node) { \
			typedef insert_node_helper<base_type, node_type> helper_type; \
			helper_type helper; \
			if(!helper.check_node(boost::forward<node_type>(node))) { \
				return false; } \
			write_lock_type lk(_mutex); \
			return helper.insert_node(_base, boost::move(node)); } \
		\
		template<typename RetHandler> \
		typename ::yggr::func::foo_t_info<RetHandler>::result_type \
			insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
			typedef insert_node_helper<base_type, node_type> helper_type; \
			helper_type helper; \
			write_lock_type lk(_mutex); \
			return helper.insert_node(_base, boost::move(node), ret_handler); } \
		\
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node) { \
			typedef insert_node_helper<base_type, node_type> helper_type; \
			helper_type helper; \
			if(!helper.check_node(node)) { return false; } \
			write_lock_type lk(_mutex); \
			return helper.insert_node_hint(_base, iter_handler(_base), boost::move(node)); } \
		\
		template<typename IterHandler, typename RetHandler> \
		typename ::yggr::func::foo_t_info<RetHandler>::result_type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
			typedef insert_node_helper<base_type, node_type> helper_type; \
			helper_type helper; \
			write_lock_type lk(_mutex); \
			return helper.insert_node_hint(_base, iter_handler(_base), boost::move(node), ret_handler); }

#define YGGR_PP_SAFE_KV_NON_UNIQUE_INSERT_FUNCTION_IMPL() \
	bool insert(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try{ return _base.insert(boost::move(val)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	bool insert(const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { return _base.insert(val) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(BOOST_RV_REF(value_type) val, \
				const Handler& handler) { \
		iterator iter; \
		bool bright = false; \
		write_lock_type lk(_mutex); \
		try { iter = _base.insert(boost::move(val)); bright = true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
		return handler(_base, std::make_pair(iter, bright)); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const value_type& val, \
				const Handler& handler) { \
		iterator iter; \
		bool bright = false; \
		write_lock_type lk(_mutex); \
		try { iter = _base.insert(val); bright = true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
		return handler(_base, std::make_pair(iter, bright)); } \
	\
	YGGR_PP_SAFE_KV_INSERT_FUNCTION_IMPL() \
	\
	YGGR_PP_SAFE_KV_NON_UNIQUE_INSERT_NODE_FUNCTION_IMPL()

#define YGGR_PP_SAFE_KV_NON_UNIQUE_INSERT_FUNCTION_IMPL_DEBUG() \
	bool insert(BOOST_RV_REF(value_type) val) { \
		write_lock_type lk(_mutex); \
		try{ return _base.insert(boost::move(val)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	bool insert(const value_type& val) { \
		write_lock_type lk(_mutex); \
		try { return _base.insert(val) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(BOOST_RV_REF(value_type) val, \
				const Handler& handler) { \
		iterator iter; \
		bool bright = false; \
		write_lock_type lk(_mutex); \
		try { iter = _base.insert(boost::move(val)); bright = true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
		return handler(_base, std::make_pair(iter, bright)); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const value_type& val, \
				const Handler& handler) { \
		iterator iter; \
		bool bright = false; \
		write_lock_type lk(_mutex); \
		try { iter = _base.insert(val); bright = true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
		return handler(_base, std::make_pair(iter, bright)); } \
	\
	YGGR_PP_SAFE_KV_INSERT_FUNCTION_IMPL()



// map_specially
#define YGGR_PP_SAFE_KV_INSERT_FUNCTION_MAP_SP_P_IMPL() \
	template<typename IterHandler, typename P> \
	typename boost::disable_if_c< \
					((::yggr::is_pair<IterHandler>::value) \
						|| (!::yggr::is_pair< \
								typename boost::remove_cv< \
									typename boost::remove_reference<P>::type>::type>::value)), bool >::type \
		insert(const IterHandler& iter_handler, BOOST_RV_REF(P) val) { \
		typedef P now_pair_type; \
		typedef typename boost::remove_cv< \
			typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
		BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
		write_lock_type lk(_mutex); \
		try { return _base.insert(iter_handler(_base), boost::forward<P>(val)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename IterHandler, typename P, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, BOOST_RV_REF(P) val, const Handler& handler) { \
		typedef P now_pair_type; \
		typedef typename boost::remove_cv< \
			typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
		BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
		iterator iter; \
		bool bright = false; \
		write_lock_type lk(_mutex); \
		try { iter = _base.insert(iter_handler(_base), boost::forward<P>(val)); bright = true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
		return handler(_base, std::make_pair(iter, bright)); }


#define YGGR_PP_SAFE_KV_UNIQUE_INSERT_FUNCTION_MAP_SP_P_IMPL() \
	template<typename P> \
	bool insert(BOOST_RV_REF(P) val) { \
		typedef P now_pair_type; \
		typedef typename boost::remove_cv< \
			typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
		BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
		write_lock_type lk(_mutex); \
		try { return _base.insert(boost::forward<P>(val)).second; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename P, typename Handler> \
	typename boost::enable_if_c< \
			::yggr::is_pair<P>::value, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		insert(BOOST_RV_REF(P) val, const Handler& handler) { \
		typedef P now_pair_type; \
		typedef typename boost::remove_cv< \
			typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
		BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
		std::pair<iterator, bool> ret(iterator(), false); \
		write_lock_type lk(_mutex); \
		try { ret = _base.insert(boost::forward<P>(val)); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } \
		return handler(_base, ret); } \
	\
	YGGR_PP_SAFE_KV_INSERT_FUNCTION_MAP_SP_P_IMPL()

#define YGGR_PP_SAFE_KV_NON_UNIQUE_INSERT_FUNCTION_MAP_SP_P_IMPL() \
	template<typename P> \
	bool insert(BOOST_RV_REF(P) val) { \
		typedef P now_pair_type; \
		typedef typename boost::remove_cv< \
			typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
		BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
		write_lock_type lk(_mutex); \
		try { return _base.insert(boost::forward<P>(val)) != _base.end(); } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
	\
	template<typename P, typename Handler> \
	typename boost::enable_if_c< \
			::yggr::is_pair<P>::value, \
			typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
		insert(BOOST_RV_REF(P) val, const Handler& handler) { \
		typedef P now_pair_type; \
		typedef typename boost::remove_cv< \
			typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
		BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
		iterator iter; \
		bool bright = false; \
		write_lock_type lk(_mutex); \
		try { iter = _base.insert(boost::forward<P>(val)); bright = true; } \
		catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
			__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) iter = _base.end(); } \
		return handler(_base, std::make_pair(iter, bright)); } \
	\
	YGGR_PP_SAFE_KV_INSERT_FUNCTION_MAP_SP_P_IMPL()


#define YGGR_PP_SAFE_KV_INSERT_FUNCTION_MAP_SP_IMPL() \
	inline bool insert(const key_type& key, BOOST_RV_REF(mapped_type) val) { \
		value_type tmp(key, mapped_type()); \
		copy_or_move_or_swap(tmp.second, boost::move(val)); \
		return this_type::insert(boost::move(tmp)); } \
	\
	inline bool insert(const key_type& key, const mapped_type& val) { \
	    value_type tmp(key, val); \
		return this_type::insert(boost::move(tmp)); } \
	\
	template<typename Handler> inline \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const key_type& key, BOOST_RV_REF(mapped_type) val, const Handler& handler) { \
		value_type tmp(key, mapped_type()); \
		copy_or_move_or_swap(tmp.second, boost::move(val)); \
		return this_type::insert(boost::move(tmp), handler); } \
	\
	template<typename Handler> inline \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const key_type& key, const mapped_type& val, const Handler& handler) { \
	    value_type tmp(key, val); \
		return this_type::insert(boost::move(tmp), handler); } \
	\
	template<typename IterHandler> inline \
	bool insert(const IterHandler& iter_handler, \
					const key_type& key, BOOST_RV_REF(mapped_type) val) { \
		value_type tmp(key, mapped_type()); \
		copy_or_move_or_swap(tmp.second, boost::move(val)); \
		return this_type::insert(iter_handler, boost::move(tmp)); } \
	\
	template<typename IterHandler> inline \
	bool insert(const IterHandler& iter_handler, \
					const key_type& key, const mapped_type& val) { \
		value_type tmp(key, val); \
		return this_type::insert(iter_handler, boost::move(tmp)); } \
	\
	template<typename IterHandler, typename Handler> inline \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, \
				const key_type& key, BOOST_RV_REF(mapped_type) val, \
				const Handler& handler) { \
		value_type tmp(key, mapped_type()); \
		copy_or_move_or_swap(tmp.second, boost::move(val)); \
		return this_type::insert(iter_handler, \
									boost::move(tmp), \
									handler); } \
	\
	template<typename IterHandler, typename Handler> inline \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		insert(const IterHandler& iter_handler, \
				const key_type& key, const mapped_type& val, \
				const Handler& handler) { \
		value_type tmp(key, val); \
		return this_type::insert(iter_handler, \
									boost::move(tmp), \
									handler); }

#define YGGR_PP_SAFE_KV_INSERT_FUNCTION_SMAP_SP_IMPL() \
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_FUNCTION_MAP_SP_P_IMPL() \
	\
	YGGR_PP_SAFE_KV_INSERT_FUNCTION_MAP_SP_IMPL()

#define YGGR_PP_SAFE_KV_INSERT_FUNCTION_MMAP_SP_IMPL() \
	YGGR_PP_SAFE_KV_NON_UNIQUE_INSERT_FUNCTION_MAP_SP_P_IMPL() \
	\
	YGGR_PP_SAFE_KV_INSERT_FUNCTION_MAP_SP_IMPL()

// ukv

#define YGGR_PP_SAFE_UKV_UNIQUE_INSERT_FUNCTION_IMPL() \
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_FUNCTION_IMPL()

#define YGGR_PP_SAFE_UKV_NON_UNIQUE_INSERT_FUNCTION_IMPL() \
	YGGR_PP_SAFE_KV_NON_UNIQUE_INSERT_FUNCTION_IMPL()


#define YGGR_PP_SAFE_UKV_INSERT_FUNCTION_SMAP_SP_IMPL() \
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_FUNCTION_MAP_SP_P_IMPL() \
	\
	YGGR_PP_SAFE_KV_INSERT_FUNCTION_MAP_SP_IMPL()

#define YGGR_PP_SAFE_UKV_INSERT_FUNCTION_MMAP_SP_IMPL() \
	YGGR_PP_SAFE_KV_NON_UNIQUE_INSERT_FUNCTION_MAP_SP_P_IMPL() \
	\
	YGGR_PP_SAFE_KV_INSERT_FUNCTION_MAP_SP_IMPL()

#define YGGR_PP_SAFE_UKV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL() \
	YGGR_PP_SAFE_KV_UNIQUE_INSERT_OR_ASSIGN_FUNCTION_IMPL()

// ------------------------------opt_version------------------------
#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#ifndef YGGR_NO_CXX11_HDR_INITIALIZER_LIST

#	define YGGR_PP_SAFE_KV_OPT_INSERT_INIT_LIST_IMPL() \
		inline bool insert(std::initializer_list<value_type> l) { \
		   return this_type::insert(l.begin(), l.end()); }

#else

#	define YGGR_PP_SAFE_KV_OPT_INSERT_INIT_LIST_IMPL()

#endif // YGGR_NO_CXX11_HDR_INITIALIZER_LIST

// kv unique
#	define YGGR_PP_SAFE_KV_OPT_UNIQUE_INSERT_DETAIL_IMPL() \
		bool insert_detail(node_ptr_type pnode) { \
			if(!pnode) { return false; } \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			return opt_cast().insert(detail::allocator_destroy_holder_ref(holder), pnode).second; } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert_detail(node_ptr_type pnode, const Handler& handler) { \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			return \
				pnode? \
					handler(_base, opt_cast().insert(detail::allocator_destroy_holder_ref(holder), pnode)) \
					: handler(_base, std::make_pair(_base.end(), false)); } \
		\
		template<typename IterHandler> \
		bool insert_detail(const IterHandler& iter_handler, node_ptr_type pnode) { \
			if(!pnode) { return false; } \
			destroy_holder_type holder(this_type::get_allocator()); \
			{ write_lock_type lk(_mutex); \
				opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
									iter_handler(_base), pnode); } \
			return true; } \
		\
		template<typename IterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert_detail(const IterHandler& iter_handler, \
							node_ptr_type pnode, \
							const Handler& handler) { \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			return \
				pnode? \
					handler(_base, \
							std::make_pair( \
								opt_cast().insert( \
									detail::allocator_destroy_holder_ref(holder), \
									iter_handler(_base), pnode), true)) \
					: handler(_base, std::make_pair(_base.end(), false)); }

// kv non_unique
#	define YGGR_PP_SAFE_KV_OPT_NON_UNIQUE_INSERT_DETAIL_IMPL() \
		bool insert_detail(node_ptr_type pnode) { \
			if(!pnode) { return false; } \
			write_lock_type lk(_mutex); \
			opt_cast().insert(pnode); \
			return true; } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert_detail(node_ptr_type pnode, const Handler& handler) { \
			write_lock_type lk(_mutex); \
			if(!pnode) { return handler(_base, std::make_pair(_base.end(), false)); } \
			return handler(_base, std::make_pair(opt_cast().insert(pnode), true)); } \
		\
		template<typename IterHandler> \
		bool insert_detail(const IterHandler& iter_handler, node_ptr_type pnode) { \
			if(!pnode) { return false; } \
			write_lock_type lk(_mutex); \
			return opt_cast().insert(iter_handler(_base), pnode) != _base.end(); } \
		\
		template<typename IterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert_detail(const IterHandler& iter_handler, \
							node_ptr_type pnode, \
							const Handler& handler) { \
			write_lock_type lk(_mutex); \
			if(!pnode) { return handler(_base, std::make_pair(_base.end(), false)); } \
			return handler(_base, std::make_pair( opt_cast().insert(iter_handler(_base), pnode), true)); }

// kv insert
#	define YGGR_PP_SAFE_KV_OPT_INSERT_FUNCTION_IMPL() \
		inline bool insert(BOOST_RV_REF(value_type) val) { \
			return this_type::insert_detail(this_type::create_node(boost::move(val))); } \
		\
		inline bool insert(const value_type& val) { \
			return this_type::insert_detail(this_type::create_node(val)); } \
		\
		template<typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(BOOST_RV_REF(value_type) val, const Handler& handler) { \
			return this_type::insert_detail(this_type::create_node(boost::move(val)), handler); } \
		\
		template<typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const value_type& val, const Handler& handler) { \
			return this_type::insert_detail(this_type::create_node(val), handler); } \
		\
		template<typename IterHandler> inline \
		bool insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(boost::move(val))); } \
		\
		template<typename IterHandler> inline \
		bool insert(const IterHandler& iter_handler, const value_type& val) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(val)); } \
		\
		template<typename IterHandler, typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, \
					BOOST_RV_REF(value_type) val, const Handler& handler) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(boost::move(val)), handler); } \
		\
		template<typename IterHandler, typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, \
					const value_type& val, const Handler& handler) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(val), handler); }


#	define YGGR_PP_SAFE_KV_OPT_UNIQUE_INSERT_ITERATOR_IMPL() \
		template<typename InputIter> \
		bool insert(InputIter start, InputIter last) { \
			allocator_type alloc = this_type::get_allocator(); \
			container::vector<node_ptr_type> pnode_vt; { \
				multi_destroy_holder_type ins_holder(alloc); \
				base_type tmp(this_type::key_comp(), alloc); \
				try { tmp.insert(start, last); \
						pnode_vt.reserve(tmp.size()); \
						ins_holder.reserve(tmp.size()); } \
				catch(const ::yggr::compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
					return false; } \
				opt_type& opt_tmp = reinterpret_cast<opt_type&>(tmp); \
				opt_tmp.clear(detail::allocator_destroy_holder_ref(ins_holder)); \
				ins_holder.copy_to(pnode_vt); \
				ins_holder.non_free_clear(); } \
			destroy_holder_type holder(alloc); { \
				write_lock_type lk(_mutex); \
				opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
								pnode_vt.begin(), pnode_vt.end()); } \
			return true; }


#	define YGGR_PP_SAFE_KV_OPT_NON_UNIQUE_INSERT_ITERATOR_IMPL() \
		template<typename InputIter> \
		bool insert(InputIter start, InputIter last) { \
			allocator_type alloc = this_type::get_allocator(); \
			container::vector<node_ptr_type> pnode_vt; \
			try { pnode_vt.reserve(std::distance(start, last)); } \
			catch(const ::yggr::compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
				return false; } \
			for(InputIter s = start; s != last; ++s) { \
				try { pnode_vt.push_back(opt_type::create_node(alloc, *s)); } \
				catch(const ::yggr::compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
					return false; } } \
			{ write_lock_type lk(_mutex); \
				opt_cast().insert(pnode_vt.begin(), pnode_vt.end()); } \
			return true; }

#	define YGGR_PP_SAFE_KV_OPT_UNIQUE_INSERT_NODE_FUNCTION_IMPL() \
		insert_return_type insert(BOOST_RV_REF(node_type) node) { \
			if(!node) { return insert_return_type(); } \
			write_lock_type lk(_mutex); \
			try { return insert_return_type(container::insert(_base, boost::move(node))); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return insert_return_type(); } } \
		\
		template<typename RetHandler> \
		typename ::yggr::func::foo_t_info<RetHandler>::result_type \
			insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
			base_insert_return_type base_iret; \
			write_lock_type lk(_mutex); \
			if(node) { \
				try { base_iret = container::insert(_base, boost::move(node)); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())); } } \
			return ret_handler(_base, base_iret); } \
		\
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node) { \
			if(!node) { return false; } \
			write_lock_type lk(_mutex); \
			try { return container::insert(_base, iter_handler(_base), boost::move(node)) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename IterHandler, typename RetHandler> \
		typename ::yggr::func::foo_t_info<RetHandler>::result_type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			write_lock_type lk(_mutex); \
			if(node) { \
				try { ret.first = container::insert(_base, iter_handler(_base), boost::move(node)); \
						ret.second = ret.first != _base.end(); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } } \
			return ret_handler(_base, ret); }

#	define YGGR_PP_SAFE_KV_OPT_NON_UNIQUE_INSERT_NODE_FUNCTION_IMPL() \
		bool insert(BOOST_RV_REF(node_type) node) { \
			if(!node) { return false; } \
			write_lock_type lk(_mutex); \
			try { return container::insert(_base, boost::move(node)) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename RetHandler> \
		typename ::yggr::func::foo_t_info<RetHandler>::result_type \
			insert(BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			write_lock_type lk(_mutex); \
			if(node) { \
				try { ret.first = container::insert(_base, boost::move(node)); \
						ret.second = ret.first != _base.end(); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } } \
			return ret_handler(_base, ret); } \
		\
		template<typename IterHandler> \
		bool insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node) { \
			if(!node) { return false; } \
			write_lock_type lk(_mutex); \
			try { return container::insert(_base, iter_handler(_base), boost::move(node)) != _base.end(); } \
			catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
				__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) return false; } } \
		\
		template<typename IterHandler, typename RetHandler> \
		typename ::yggr::func::foo_t_info<RetHandler>::result_type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(node_type) node, const RetHandler& ret_handler) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			write_lock_type lk(_mutex); \
			if(node) { \
				try { ret.first = container::insert(_base, iter_handler(_base), boost::move(node)); \
						ret.second = ret.first != _base.end(); } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) ret.first = _base.end(); } } \
			return ret_handler(_base, ret); }

// kv map sp
#	define YGGR_PP_SAFE_KV_OPT_INSERT_FUNCTION_MAP_SP_IMPL() \
	public: \
		inline bool insert(const key_type& key, BOOST_RV_REF(mapped_type) val) { \
			return this_type::insert_detail(this_type::create_node(key, boost::move(val))); } \
		\
		inline bool insert(const key_type& key, const mapped_type& val) { \
			return this_type::insert_detail(this_type::create_node(key, val)); } \
		\
		template<typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const key_type& key, BOOST_RV_REF(mapped_type) val, const Handler& handler) { \
			return this_type::insert_detail(this_type::create_node(key, boost::move(val)), handler); } \
		\
		template<typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const key_type& key, const mapped_type& val, const Handler& handler) { \
			return this_type::insert_detail(this_type::create_node(key, val), handler); } \
		\
		template<typename IterHandler> inline \
		bool insert(const IterHandler& iter_handler, \
						const key_type& key, BOOST_RV_REF(mapped_type) val) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(key, boost::move(val))); } \
		\
		template<typename IterHandler> inline \
		bool insert(const IterHandler& iter_handler, \
						const key_type& key, const mapped_type& val) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(key, val)); } \
		\
		template<typename IterHandler, typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, \
					const key_type& key, BOOST_RV_REF(mapped_type) val, \
					const Handler& handler) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(key, boost::move(val)), handler); } \
		\
		template<typename IterHandler, typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, \
					const key_type& key, const mapped_type& val, \
					const Handler& handler) { \
			return this_type::insert_detail(iter_handler, this_type::create_node(key, val), handler); } \
		\
		template<typename P> inline \
		bool insert(BOOST_RV_REF(P) val) { \
			typedef P now_pair_type; \
			typedef typename boost::remove_cv< \
				typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
			BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
			return this_type::insert_detail(this_type::create_node(boost::forward<P>(val))); } \
		\
		template<typename P, typename Handler> inline \
		typename boost::enable_if_c< \
				::yggr::is_pair<P>::value, \
				typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			insert(BOOST_RV_REF(P) val, const Handler& handler) { \
			typedef P now_pair_type; \
			typedef typename boost::remove_cv< \
				typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
			BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
			return this_type::insert_detail(this_type::create_node(boost::forward<P>(val)), handler); } \
		\
		template<typename IterHandler, typename P> inline \
		typename boost::disable_if_c< \
					((::yggr::is_pair<IterHandler>::value) \
						|| (!::yggr::is_pair< \
								typename boost::remove_cv< \
									typename boost::remove_reference<P>::type>::type>::value)), bool >::type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(P) val) { \
			typedef P now_pair_type; \
			typedef typename boost::remove_cv< \
				typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
			BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
			return this_type::insert_detail(iter_handler, this_type::create_node(boost::forward<P>(val))); } \
		\
		template<typename IterHandler, typename P, typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(P) val, const Handler& handler) { \
			typedef P now_pair_type; \
			typedef typename boost::remove_cv< \
				typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
			BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
			return this_type::insert_detail(iter_handler, this_type::create_node(boost::forward<P>(val)), handler); }


#	define YGGR_PP_SAFE_KV_OPT_UNIQUE_INSERT_IMPL() \
	private: \
		YGGR_PP_SAFE_KV_OPT_UNIQUE_INSERT_DETAIL_IMPL() \
	public: \
		YGGR_PP_SAFE_KV_OPT_INSERT_FUNCTION_IMPL() \
		\
		YGGR_PP_SAFE_KV_OPT_UNIQUE_INSERT_ITERATOR_IMPL() \
		\
		YGGR_PP_SAFE_KV_OPT_INSERT_INIT_LIST_IMPL() \
		\
		YGGR_PP_SAFE_KV_OPT_UNIQUE_INSERT_NODE_FUNCTION_IMPL()

#	define YGGR_PP_SAFE_KV_OPT_NON_UNIQUE_INSERT_IMPL() \
	private: \
		YGGR_PP_SAFE_KV_OPT_NON_UNIQUE_INSERT_DETAIL_IMPL() \
	public: \
		YGGR_PP_SAFE_KV_OPT_INSERT_FUNCTION_IMPL() \
		\
		YGGR_PP_SAFE_KV_OPT_NON_UNIQUE_INSERT_ITERATOR_IMPL() \
		\
		YGGR_PP_SAFE_KV_OPT_INSERT_INIT_LIST_IMPL() \
		\
		YGGR_PP_SAFE_KV_OPT_NON_UNIQUE_INSERT_NODE_FUNCTION_IMPL()

#	define YGGR_PP_SAFE_KV_OPT_INSERT_FUNCTION_SMAP_SP_IMPL() \
		YGGR_PP_SAFE_KV_OPT_INSERT_FUNCTION_MAP_SP_IMPL()

#	define YGGR_PP_SAFE_KV_OPT_INSERT_FUNCTION_MMAP_SP_IMPL() \
		YGGR_PP_SAFE_KV_OPT_INSERT_FUNCTION_MAP_SP_IMPL()

//#	define YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL() \
//	template<typename K, typename M> \
//	bool insert_or_assign(BOOST_RV_REF(K) k, BOOST_RV_REF(M) m) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		allocator_type alloc = this_type::get_allocator(); \
//		destroy_holder_type holder(alloc); \
//		node_ptr_type pnode = this_type::create_node_alloc(alloc, boost::move(k), boost::move(m)); \
//		if(!pnode) { return false; } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), pnode); \
//		if(!ret.second) { ret.first->second = boost::move(pnode->get_data().second); } \
//		return ret.second; } \
//	\
//	template<typename K, typename M> \
//	bool insert_or_assign(const K& k, BOOST_RV_REF(M) m) { \
//		std::pair<iterator, bool> ret(iterator(), false); \
//		allocator_type alloc = this_type::get_allocator(); \
//		destroy_holder_type holder(alloc); \
//		node_ptr_type pnode = this_type::create_node_alloc(alloc, k, boost::move(m)); \
//		if(!pnode) { return false; } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), pnode); \
//		if(!ret.second) { ret.first->second = boost::move(pnode->get_data().second); } \
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
//		node_ptr_type pnode = this_type::create_node_alloc(alloc, boost::move(k), boost::move(m)); \
//		if(!pnode) { return ret_handler(_base, ret); } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), pnode); \
//		if(!ret.second){ ret.first->second = boost::move(pnode->get_data().second); } \
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
//		node_ptr_type pnode = this_type::create_node_alloc(alloc, k, boost::move(m)); \
//		if(!pnode) { return ret_handler(_base, ret); } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), pnode); \
//		if(!ret.second) { ret.first->second = boost::move(pnode->get_data().second); } \
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
//		node_ptr_type pnode = this_type::create_node_alloc(alloc, boost::move(k), boost::move(m)); \
//		if(!pnode) { return false; } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), pnode); \
//		if(holder.size()) { ret->second = boost::move(pnode->get_data().second); } \
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
//		node_ptr_type pnode = this_type::create_node_alloc(alloc, k, boost::move(m)); \
//		if(!pnode) { return false; } \
//		write_lock_type lk(_mutex); \
//		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), pnode); \
//		if(holder.size()) { ret->second = boost::move(pnode->get_data().second); } \
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
//		node_ptr_type pnode = this_type::create_node_alloc(alloc, boost::move(k), boost::move(m)); \
//		if(!pnode) { return ret_handler(_base, ret); } \
//		write_lock_type lk(_mutex); \
//		ret.first = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), pnode); \
//		if(holder.size()) { ret.first->second = boost::move(pnode->get_data().second); } \
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
//		node_ptr_type pnode = this_type::create_node_alloc(alloc, k, boost::move(m)); \
//		if(!pnode) { return ret_handler(_base, ret); } \
//		write_lock_type lk(_mutex); \
//		ret.first = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), pnode); \
//		if(holder.size()) { ret.first->second = boost::move(pnode->get_data().second); } \
//		ret.second = ret.first != _base.end(); \
//		return ret_handler(_base, ret); }

#	define YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
			__tk__, __tm__, __ak__, __am__, __uak__) \
	template<typename __tk__, typename __tm__> \
	bool insert_or_assign(__ak__ k, __am__ m) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		allocator_type alloc = this_type::get_allocator(); \
		destroy_holder_type holder(alloc); \
		node_ptr_type pnode = this_type::create_node_alloc(alloc, __uak__(k), boost::forward< __tm__ >(m)); \
		if(!pnode) { return false; } \
		write_lock_type lk(_mutex); \
		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), pnode); \
		if(!ret.second) { ret.first->second = boost::move(pnode->get_data().second); } \
		return ret.second; }

#	define YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
			__tk__, __tm__, __ak__, __am__, __uak__) \
	template<typename __tk__, typename __tm__, typename RetHandler> \
	typename \
		boost::disable_if< \
			boost::mpl::and_ < \
				typename ::yggr::func::foo_t_info< __tk__ >::is_callable_type, \
				boost::mpl::or_< \
					boost::is_base_of< \
						iterator, \
						typename ::yggr::func::foo_t_info< __tk__ >::result_type >, \
					boost::is_base_of< \
						const_iterator, \
						typename ::yggr::func::foo_t_info< __tk__ >::result_type > > >, \
			typename ::yggr::func::foo_t_info<RetHandler>::result_type >::type \
		insert_or_assign(__ak__ k, __am__ m, const RetHandler& ret_handler) { \
		std::pair<iterator, bool> ret(iterator(), false); \
		allocator_type alloc = this_type::get_allocator(); \
		destroy_holder_type holder(alloc); \
		node_ptr_type pnode = this_type::create_node_alloc(alloc, __uak__(k), boost::forward< __tm__ >(m)); \
		if(!pnode) { return ret_handler(_base, ret); } \
		write_lock_type lk(_mutex); \
		ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), pnode); \
		if(!ret.second){ ret.first->second = boost::move(pnode->get_data().second); } \
		return ret_handler(_base, ret); }

#	define YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
			__tk__, __tm__, __ak__, __am__, __uak__) \
		template<typename IterHandler, typename __tk__, typename __tm__ > \
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
			insert_or_assign(const IterHandler& iter_handler, __ak__ k, __am__ m) { \
			iterator ret; \
			allocator_type alloc = this_type::get_allocator(); \
			destroy_holder_type holder(alloc); \
			node_ptr_type pnode = this_type::create_node_alloc(alloc, __uak__(k), boost::forward< __tm__ >(m)); \
			if(!pnode) { return false; } \
			write_lock_type lk(_mutex); \
			ret = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), pnode); \
			if(holder.size()) { ret->second = boost::move(pnode->get_data().second); } \
			return ret != _base.end(); }

#	define YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
			__tk__, __tm__, __ak__, __am__, __uak__) \
		template<typename IterHandler, typename __tk__, typename __tm__, typename RetHandler> \
		typename ::yggr::func::foo_t_info<RetHandler>::result_type \
			insert_or_assign(const IterHandler& iter_handler, \
								__ak__ k, __am__ m, const RetHandler& ret_handler) { \
			std::pair<iterator, bool> ret(iterator(), false); \
			allocator_type alloc = this_type::get_allocator(); \
			destroy_holder_type holder(alloc); \
			assert(!holder.size()); \
			node_ptr_type pnode = this_type::create_node_alloc(alloc, __uak__(k), boost::forward< __tm__ >(m)); \
			if(!pnode) { return ret_handler(_base, ret); } \
			write_lock_type lk(_mutex); \
			ret.first = opt_cast().insert(detail::allocator_destroy_holder_ref(holder), iter_handler(_base), pnode); \
			if(holder.size()) { ret.first->second = boost::move(pnode->get_data().second); } \
			ret.second = ret.first != _base.end(); \
			return ret_handler(_base, ret); }

#	define YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL() \
		YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
			K, M, BOOST_RV_REF(K), BOOST_FWD_REF(M), boost::move) \
		YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_2( \
			K, M, const K&, BOOST_FWD_REF(M), YGGR_PP_SYMBOL_EMPTY()) \
		\
		YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
			K, M, BOOST_RV_REF(K), BOOST_FWD_REF(M), boost::move) \
		YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_3R( \
			K, M, const K&, BOOST_FWD_REF(M), YGGR_PP_SYMBOL_EMPTY()) \
		\
		YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
			K, M, BOOST_RV_REF(K), BOOST_FWD_REF(M), boost::move) \
		YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_I3( \
			K, M, const K&, BOOST_FWD_REF(M), YGGR_PP_SYMBOL_EMPTY()) \
		\
		YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
			K, M, BOOST_RV_REF(K), BOOST_FWD_REF(M), boost::move) \
		YGGR_PP_SAFE_KV_UNIQUE_OPT_INSERT_OR_ASSIGN_FUNCTION_IMPL_DETAIL_4( \
			K, M, const K&, BOOST_FWD_REF(M), YGGR_PP_SYMBOL_EMPTY())

// ukv_h

// umap_h_opt_specially
#	define YGGR_PP_SAFE_UKV_H_OPT_INSERT_FUNCTION_MAP_SP_P_IMPL() \
		template<typename IterHandler, typename P> inline \
		typename boost::disable_if_c< \
					((::yggr::is_pair<IterHandler>::value) \
						|| (!::yggr::is_pair< \
								typename boost::remove_cv< \
									typename boost::remove_reference<P>::type>::type>::value)), bool >::type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(P) val) { \
			typedef P now_pair_type; \
			typedef typename boost::remove_cv< \
				typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
			BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
			value_type tmp = boost::forward<P>(val); \
			return this_type::insert(iter_handler, boost::move(tmp)); } \
		\
		template<typename IterHandler, typename P, typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(P) val, const Handler& handler) { \
			typedef P now_pair_type; \
			typedef typename boost::remove_cv< \
				typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
			BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
			value_type tmp = boost::forward<P>(val); \
			return this_type::insert(iter_handler, boost::move(tmp), handler); }

// ukv_ha
// ukv_ha unique
#	define YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_INSERT_DETAIL_IMPL() \
		bool insert_detail(std::size_t key_hash, node_ptr_type pnode) { \
			if(!pnode) { return false; } \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			return opt_cast().insert(detail::allocator_destroy_holder_ref(holder), key_hash, pnode).second; } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert_detail(std::size_t key_hash, node_ptr_type pnode, const Handler& handler) { \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			return \
				pnode? \
					handler(_base, opt_cast().insert(detail::allocator_destroy_holder_ref(holder), key_hash, pnode)) \
					: handler(_base, std::make_pair(_base.end(), false)); } \
		\
		template<typename IterHandler> \
		bool insert_detail(const IterHandler& iter_handler, std::size_t key_hash, node_ptr_type pnode) { \
			if(!pnode) { return false; } \
			destroy_holder_type holder(this_type::get_allocator()); \
			{ write_lock_type lk(_mutex); \
				opt_cast().insert(detail::allocator_destroy_holder_ref(holder), \
									iter_handler(_base), key_hash, pnode); } \
			return true; } \
		\
		template<typename IterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert_detail(const IterHandler& iter_handler, \
							std::size_t key_hash, \
							node_ptr_type pnode, \
							const Handler& handler) { \
			destroy_holder_type holder(this_type::get_allocator()); \
			write_lock_type lk(_mutex); \
			return \
				pnode? \
					handler(_base, \
							std::make_pair( \
								opt_cast().insert( \
									detail::allocator_destroy_holder_ref(holder), \
									iter_handler(_base), key_hash, pnode), true)) \
					: handler(_base, std::make_pair(_base.end(), false)); }

#	define YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_INSERT_ITERATOR_IMPL() \
		template<typename InputIter> \
		bool insert(InputIter start, InputIter last) { \
			allocator_type alloc = this_type::get_allocator(); \
			container::vector<std::size_t> key_hash_vt; \
			container::vector<node_ptr_type> pnode_vt; { \
				multi_destroy_holder_type ins_holder(alloc); \
				base_type tmp(alloc); \
				try { tmp.insert(start, last); \
						key_hash_vt.reserve(tmp.size()); \
						pnode_vt.reserve(tmp.size()); \
						ins_holder.reserve(tmp.size()); } \
				catch(const ::yggr::compatibility::stl_exception& YGGR_DBG_VAR_ERR()) { \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) \
					return false; } \
				opt_type& opt_tmp = reinterpret_cast<opt_type&>(tmp); \
				opt_tmp.clear(detail::allocator_destroy_holder_ref(ins_holder)); \
				ins_holder.copy_to(pnode_vt); \
				ins_holder.non_free_clear(); } \
			opt_type::s_hash_range(key_hash_vt, this_type::hash_function(), pnode_vt.begin(), pnode_vt.end()); \
			assert(key_hash_vt.size() == pnode_vt.size()); \
			if(!(key_hash_vt.size() == pnode_vt.size())) { return false; } \
			destroy_holder_type holder(alloc); { \
				write_lock_type lk(_mutex); \
				opt_cast().insert( \
					detail::allocator_destroy_holder_ref(holder), \
					key_hash_vt.begin(), key_hash_vt.end(), \
					pnode_vt.begin(), pnode_vt.end()); } \
			return true; }

#	define YGGR_PP_SAFE_UKV_HA_OPT_INSERT_FUNCTION_IMPL() \
		inline bool insert(BOOST_RV_REF(value_type) val) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::move(val)); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		inline bool insert(const value_type& val) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, val); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		template<typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(BOOST_RV_REF(value_type) val, const Handler& handler) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::move(val)); \
			return this_type::insert_detail(key_hash, pnode, handler); } \
		\
		template<typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const value_type& val, const Handler& handler) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, val); \
			return this_type::insert_detail(key_hash, pnode, handler); } \
		\
		template<typename IterHandler> inline \
		bool insert(const IterHandler& iter_handler, BOOST_RV_REF(value_type) val) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::move(val)); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template<typename IterHandler> inline \
		bool insert(const IterHandler& iter_handler, const value_type& val) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, val); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template<typename IterHandler, typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, \
					BOOST_RV_REF(value_type) val, const Handler& handler) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::move(val)); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); } \
		\
		template<typename IterHandler, typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, \
					const value_type& val, const Handler& handler) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, val); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); }

// umap_ha_opt_specially

#	define YGGR_PP_SAFE_UKV_HA_OPT_INSERT_FUNCTION_MAP_SP_IMPL() \
	public: \
		inline bool insert(const key_type& key, BOOST_RV_REF(mapped_type) val) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, key, boost::move(val)); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		inline bool insert(const key_type& key, const mapped_type& val) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, key, val); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		template<typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const key_type& key, BOOST_RV_REF(mapped_type) val, const Handler& handler) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, key, boost::move(val)); \
			return this_type::insert_detail(key_hash, pnode, handler); } \
		\
		template<typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const key_type& key, const mapped_type& val, const Handler& handler) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, key, val); \
			return this_type::insert_detail(key_hash, pnode, handler); } \
		\
		template<typename IterHandler> inline \
		bool insert(const IterHandler& iter_handler, \
						const key_type& key, BOOST_RV_REF(mapped_type) val) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, key, boost::move(val)); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template<typename IterHandler> inline \
		bool insert(const IterHandler& iter_handler, \
						const key_type& key, const mapped_type& val) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, key, val); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template<typename IterHandler, typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, \
					const key_type& key, BOOST_RV_REF(mapped_type) val, \
					const Handler& handler) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, key, boost::move(val)); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); } \
		\
		template<typename IterHandler, typename Handler> inline \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert(const IterHandler& iter_handler, \
					const key_type& key, const mapped_type& val, \
					const Handler& handler) { \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, key, val); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); } \
		\
		template<typename P> inline \
		bool insert(BOOST_RV_REF(P) val) { \
			typedef P now_pair_type; \
			typedef typename boost::remove_cv< \
				typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
			BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::forward<P>(val)); \
			return this_type::insert_detail(key_hash, pnode); } \
		\
		template<typename P, typename Handler> inline \
		typename boost::enable_if_c< \
				::yggr::is_pair<P>::value, \
				typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			insert(BOOST_RV_REF(P) val, const Handler& handler) { \
			typedef P now_pair_type; \
			typedef typename boost::remove_cv< \
				typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
			BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::forward<P>(val)); \
			return this_type::insert_detail(key_hash, pnode, handler); } \
		\
		template<typename IterHandler, typename P> inline \
		typename boost::disable_if_c< \
					((::yggr::is_pair<IterHandler>::value) \
						|| (!::yggr::is_pair< \
								typename boost::remove_cv< \
									typename boost::remove_reference<P>::type>::type>::value)), bool >::type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(P) val) { \
			typedef P now_pair_type; \
			typedef typename boost::remove_cv< \
				typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
			BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = this_type::create_node(key_hash, boost::forward<P>(val)); \
			return this_type::insert_detail(iter_handler, key_hash, pnode); } \
		\
		template<typename IterHandler, typename P, typename Handler> inline \
		typename boost::disable_if_c< \
					((::yggr::is_pair<IterHandler>::value) \
						|| (!::yggr::is_pair< \
								typename boost::remove_cv< \
									typename boost::remove_reference<P>::type>::type>::value)), \
									typename ::yggr::func::foo_t_info<Handler>::result_type >::type \
			insert(const IterHandler& iter_handler, BOOST_RV_REF(P) val, const Handler& handler) { \
			typedef P now_pair_type; \
			typedef typename boost::remove_cv< \
				typename boost::remove_reference<now_pair_type>::type>::type chk_type; \
			BOOST_MPL_ASSERT((::yggr::is_pair<chk_type>)); \
			std::size_t key_hash = 0; \
			node_ptr_type pnode = opt_type::create_node_1(key_hash, boost::forward<P>(val)); \
			return this_type::insert_detail(iter_handler, key_hash, pnode, handler); }

#	define YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_INSERT_FUNCTION_IMPL() \
	private: \
		YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_INSERT_DETAIL_IMPL() \
	public: \
		YGGR_PP_SAFE_UKV_HA_OPT_INSERT_FUNCTION_IMPL() \
		\
		YGGR_PP_SAFE_UKV_HA_OPT_UNIQUE_INSERT_ITERATOR_IMPL()

#	define YGGR_PP_SAFE_UKV_HA_OPT_NON_UNIQUE_INSERT_DETAIL_IMPL() \
		bool insert_detail(std::size_t key_hash, node_ptr_type pnode) { \
			if(!pnode) { return false; } \
			write_lock_type lk(_mutex); \
			opt_cast().insert(key_hash, pnode); \
			return true; } \
		\
		template<typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert_detail(std::size_t key_hash, node_ptr_type pnode, const Handler& handler) { \
			write_lock_type lk(_mutex); \
			if(!pnode) { return handler(_base, std::make_pair(_base.end(), false)); } \
			return handler(_base, std::make_pair(opt_cast().insert(key_hash, pnode), true)); } \
		\
		template<typename IterHandler> \
		bool insert_detail(const IterHandler& iter_handler, std::size_t key_hash, node_ptr_type pnode) { \
			if(!pnode) { return false; } \
			write_lock_type lk(_mutex); \
			return opt_cast().insert(iter_handler(_base), key_hash, pnode) != _base.end(); } \
		\
		template<typename IterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			insert_detail(const IterHandler& iter_handler, \
							std::size_t key_hash, \
							node_ptr_type pnode, \
							const Handler& handler) { \
			write_lock_type lk(_mutex); \
			if(!pnode) { return handler(_base, std::make_pair(_base.end(), false)); } \
			return handler(_base, std::make_pair( opt_cast().insert(iter_handler(_base), key_hash, pnode), true)); }

#	define YGGR_PP_SAFE_UKV_HA_OPT_NON_UNIQUE_INSERT_ITERATOR_IMPL() \
		template<typename InputIter> \
		bool insert(InputIter start, InputIter last) { \
			typedef container::vector<std::size_t> hash_vt_type; \
			typedef container::vector<node_ptr_type> node_ptr_vt_type; \
			typedef typename hash_vt_type::iterator hash_vt_iter_type; \
			typedef typename node_ptr_vt_type::iterator node_ptr_vt_iter_type; \
			typedef \
				std::pair< \
					std::pair<hash_vt_iter_type, hash_vt_iter_type>, \
					std::pair<node_ptr_vt_iter_type, node_ptr_vt_iter_type> > ins_param_type; \
			allocator_type alloc = this_type::get_allocator(); \
			hash_vt_type hash_vt; \
			node_ptr_vt_type pnode_vt; \
			ins_param_type ins_param \
				= opt_type::create_node_range( \
					alloc, this_type::hash_function(), \
					hash_vt, pnode_vt, start, last); \
			if(pnode_vt.empty()) { return false; } \
			multi_destroy_holder_type holder(pnode_vt.size(), alloc); \
			holder(ins_param.second.first, ins_param.second.second); \
			bool bins = false; \
			{ write_lock_type lk(_mutex); \
				try { opt_cast().insert(ins_param); bins = true; } \
				catch(const compatibility::stl_exception& YGGR_DBG_VAR_ERR()){ \
					__YGGR_MSG_EXCEPTION((YGGR_DBG_VAR_ERR().what())) bins = false; } } \
			if(bins) { holder.non_free_clear(); } \
			return bins; }

#	define YGGR_PP_SAFE_UKV_HA_OPT_NON_UNIQUE_INSERT_FUNCTION_IMPL() \
	private: \
		YGGR_PP_SAFE_UKV_HA_OPT_NON_UNIQUE_INSERT_DETAIL_IMPL() \
	public: \
		YGGR_PP_SAFE_UKV_HA_OPT_INSERT_FUNCTION_IMPL() \
		\
		YGGR_PP_SAFE_UKV_HA_OPT_NON_UNIQUE_INSERT_ITERATOR_IMPL()

#	define YGGR_PP_SAFE_UKV_HA_OPT_INSERT_FUNCTION_SMAP_SP_IMPL() \
		YGGR_PP_SAFE_UKV_HA_OPT_INSERT_FUNCTION_MAP_SP_IMPL()

#	define YGGR_PP_SAFE_UKV_HA_OPT_INSERT_FUNCTION_MMAP_SP_IMPL() \
		YGGR_PP_SAFE_UKV_HA_OPT_INSERT_FUNCTION_MAP_SP_IMPL()

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

#endif // __YGGR_SAFE_CONTAINER_DETAIL_INSERT_IMPL_HPP__
