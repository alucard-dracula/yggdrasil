//indexes_erase_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_ERASE_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_ERASE_IMPL_HPP__

#include <yggr/func/foo_t_info.hpp>
#include <yggr/safe_container/detail/destroy_holder_selector.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/fusion/include/pair.hpp>

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{

// return iterator version
template<typename Base, typename IndexID>
struct midx_erase_caller
{
public:
	typedef IndexID index_id_type;
	typedef Base base_type;
	typedef typename base_type::iterator iterator;
	typedef iterator erase_result_type;

	typedef
		boost::fusion::pair
		<
			index_id_type,
			std::pair<erase_result_type, bool> 
		> result_type;

private:
	typedef midx_erase_caller this_type;

public:

	template<typename Iter> inline
	result_type operator()(base_type& base, Iter i) const
	{
		return (i == base.end())? 
				result_type(std::make_pair(i, false)) 
				: result_type(std::make_pair(base.erase(i), true));
	}

	template<typename Iter1, typename Iter2> inline
	result_type operator()(base_type& base, Iter1 first, Iter2 last) const
	{
		return (first == base.end())?
					result_type(std::make_pair(first, false))
					: result_type(std::make_pair(base.erase(first, last), true));
	}

	template<typename Iter1, typename Iter2> inline 
	result_type operator()(base_type& base, 
							const std::pair<Iter1, Iter2>& pos_pair) const
	{
		return this_type::operator()(base, pos_pair.first, pos_pair.second);
	}

	inline bool result_bool_adapter(const result_type& rst) const
	{
		return rst.second.second;
	}
};

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr



#define YGGR_PP_SAFE_MIDX_KV_INDEXES_ERASE_FUNCTION_SP_IMPL() \
	size_type erase(const key_type& key) { \
		write_lock_type lk(_mutex); \
		return _base.erase(key); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		erase(const key_type& key, const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return \
			handler( \
				_final, \
				boost::fusion::pair < \
					index_id_type, \
					size_type >(_base.erase(key)) ); }

#define YGGR_PP_SAFE_MIDX_INDEXES_ERASE_FUNCTION_IMPL() \
	template<typename IterHandler> \
	bool erase(const IterHandler& iter_handler) { \
		typedef multi_index_detail::midx_erase_caller<base_type, index_id_type> caller_type; \
		caller_type caller; \
		write_lock_type lk(_mutex); \
		return caller.result_bool_adapter(caller(_base, iter_handler(_base))); } \
	\
	template<typename EHandler, typename ZHandler> \
	typename \
		boost::disable_if_c< \
			func::foo_t_info<ZHandler>::arg_holder_list_size == 2, \
			bool >::type \
		erase(const EHandler& eins_handler, const ZHandler& zwei_handler) { \
		typedef multi_index_detail::midx_erase_caller<base_type, index_id_type> caller_type; \
		caller_type caller; \
		write_lock_type lk(_mutex); \
		return caller.result_bool_adapter(caller(_base, eins_handler(_base), zwei_handler(_base))); } \
	\
	template<typename EHandler, typename ZHandler> \
	typename \
		boost::enable_if_c< \
			func::foo_t_info<ZHandler>::arg_holder_list_size == 2, \
			typename ::yggr::func::foo_t_info<ZHandler>::result_type >::type \
		erase(const EHandler& eins_handler, const ZHandler& zwei_handler) { \
		typedef multi_index_detail::midx_erase_caller<base_type, index_id_type> caller_type; \
		caller_type caller; \
		write_lock_type lk(_mutex); \
		return zwei_handler(_final, caller(_base, eins_handler(_base))); } \
	\
	template<typename EIterHandler, typename ZIterHandler, typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		erase(const EIterHandler& eins_iter_handler, const ZIterHandler& zwei_iter_handler, \
				const Handler& handler) { \
		typedef multi_index_detail::midx_erase_caller<base_type, index_id_type> caller_type; \
		caller_type caller; \
		write_lock_type lk(_mutex); \
		return handler(_final, caller(_base, eins_iter_handler(_base), zwei_iter_handler(_base))); }


#ifndef YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{

template<typename Base, typename IndexID>
struct midx_opt_erase_caller
{
public:
	typedef IndexID index_id_type;
	typedef Base base_type;
	typedef typename base_type::iterator iterator;
	typedef iterator erase_result_type;

	typedef
		boost::fusion::pair
		<
			index_id_type,
			std::pair<erase_result_type, bool> 
		> result_type;

private:
	typedef midx_opt_erase_caller this_type;

public:

	template<typename Container_Opt, 
				typename Destroy_Holder, typename Iter> inline
	result_type operator()(base_type& base, Container_Opt& opt_base, Destroy_Holder& holder, Iter s) const
	{
		return (s == base.end())? 
				result_type(std::make_pair(base.end(), false))
				: result_type(std::make_pair(opt_base.erase(holder, s), true));
	}

	template<typename Container_Opt,
				typename Destroy_Holder, typename Iter1, typename Iter2> inline
	result_type operator()(base_type& base, Container_Opt& opt_base, Destroy_Holder& holder, Iter1 s, Iter2 e) const
	{
		return (s == base.end())? 
				result_type(std::make_pair(base.end(), false))
				: result_type(std::make_pair(opt_base.erase(holder, s, e), true));
	}

	template<typename Container_Opt, 
				typename Destroy_Holder, typename Iter1, typename Iter2> inline
	result_type operator()(base_type& base, Container_Opt& opt_base, Destroy_Holder& holder,
									const std::pair<Iter1, Iter2>& iter_pair) const
	{
		return this_type::operator()(base, opt_base, holder, iter_pair.first, iter_pair.second);
	}


	inline bool result_bool_adapter(const result_type& rst) const
	{
		return rst.second.second;
	}
};

} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr


#	define YGGR_PP_SAFE_MIDX_INDEXES_HA_OPT_ERASE_FUNCTION_IMPL() \
		template<typename IterHandler> \
		bool erase(const IterHandler& iter_handler) { \
			typedef detail::destroy_holder_selector< \
						destroy_holder_type, \
						multi_destroy_holder_type, \
						typename ::yggr::func::foo_t_info<IterHandler>::result_type > dh_sel_type; \
			typedef typename dh_sel_type::type dh_type; \
			typedef multi_index_detail::midx_opt_erase_caller<base_type, index_id_type> caller_type; \
			dh_sel_type dh_sel; \
			dh_type holder(this_type::get_allocator()); \
			dh_sel(*this, holder); \
			caller_type caller; \
			write_lock_type lk(_mutex); \
			return caller.result_bool_adapter( \
					caller(_base, opt_cast(), \
							detail::allocator_destroy_holder_ref(holder), \
							iter_handler(_base))); } \
		\
		template<typename EHandler, typename ZHandler> \
		typename \
			boost::disable_if_c< \
				func::foo_t_info<ZHandler>::arg_holder_list_size == 2, \
				bool >::type \
			erase(const EHandler& eins_handler, const ZHandler& zwei_handler) { \
			typedef detail::destroy_holder_selector< \
						destroy_holder_type, \
						multi_destroy_holder_type, \
						std::pair<int, int> > dh_sel_type; \
			typedef typename dh_sel_type::type dh_type; \
			typedef multi_index_detail::midx_opt_erase_caller<base_type, index_id_type> caller_type; \
			dh_sel_type dh_sel; \
			dh_type holder(this_type::get_allocator()); \
			dh_sel(*this, holder); \
			caller_type caller; \
			write_lock_type lk(_mutex); \
			return caller.result_bool_adapter( \
					caller(_base, opt_cast(), \
							detail::allocator_destroy_holder_ref(holder), \
							eins_handler(_base), zwei_handler(_base))); } \
		\
		template<typename EHandler, typename ZHandler> \
		typename \
			boost::enable_if_c< \
				func::foo_t_info<ZHandler>::arg_holder_list_size == 2, \
				typename ::yggr::func::foo_t_info<ZHandler>::result_type >::type \
			erase(const EHandler& eins_handler, const ZHandler& zwei_handler) { \
			typedef detail::destroy_holder_selector< \
						destroy_holder_type, \
						multi_destroy_holder_type, \
						typename ::yggr::func::foo_t_info<EHandler>::result_type > dh_sel_type; \
			typedef typename dh_sel_type::type dh_type; \
			typedef multi_index_detail::midx_opt_erase_caller<base_type, index_id_type> caller_type; \
			dh_sel_type dh_sel; \
			dh_type holder(this_type::get_allocator()); \
			dh_sel(*this, holder); \
			caller_type caller; \
			write_lock_type lk(_mutex); \
			return zwei_handler(_final, \
					caller(_base, opt_cast(), \
							detail::allocator_destroy_holder_ref(holder), \
							eins_handler(_base))); } \
		\
		template<typename EIterHandler, typename ZIterHandler, typename Handler> \
		typename ::yggr::func::foo_t_info<Handler>::result_type \
			erase(const EIterHandler& eins_iter_handler, const ZIterHandler& zwei_iter_handler, \
					const Handler& handler) { \
			typedef Handler handler_type; \
			typedef detail::destroy_holder_selector< \
						destroy_holder_type, \
						multi_destroy_holder_type, \
						std::pair<int, int> > dh_sel_type; \
			typedef typename dh_sel_type::type dh_type; \
			typedef multi_index_detail::midx_opt_erase_caller<base_type, index_id_type> caller_type; \
			dh_sel_type dh_sel; \
			dh_type holder(this_type::get_allocator()); \
			dh_sel(*this, holder); \
			caller_type caller; \
			write_lock_type lk(_mutex); \
			return handler(_final, \
					caller(_base, opt_cast(), \
							detail::allocator_destroy_holder_ref(holder), \
							eins_iter_handler(_base), zwei_iter_handler(_base))); }

#endif // YGGR_NOT_USE_SAFE_CONTAINER_OPTIMIZATION

#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_ERASE_IMPL_HPP__
