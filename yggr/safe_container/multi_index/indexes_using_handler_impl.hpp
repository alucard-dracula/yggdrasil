//indexes_using_handler_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_USING_HANDLER_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_USING_HANDLER_IMPL_HPP__

#include <yggr/func/foo_t_info.hpp>
#include <yggr/safe_container/detail/using_handler_impl.hpp>

#define YGGR_PP_SAFE_MIDX_INDEXES_USING_HANDLER_FINAL_IMPL() \
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		using_handler_final(const Handler& handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final); } \
	\
	template<typename Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		using_handler_final(const Handler& handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final); } \
	\
	template<typename Handler, typename Return_Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		using_handler_final(const Handler& handler, const Return_Handler& ret_handler) { \
		write_lock_type lk(_mutex); \
		return handler(_final, ret_handler); } \
	\
	template<typename Handler, typename Return_Handler> \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		using_handler_final(const Handler& handler, const Return_Handler& ret_handler) const { \
		read_lock_type lk(_mutex); \
		return handler(_final, ret_handler); }

#define YGGR_PP_SAFE_MIDX_INDEXES_USING_HOOK_HANDLER_FINAL_IMPL() \
	template<typename Handler> inline \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		using_hook_handler_final(const Handler& handler) { \
		return handler(_mutex, _final); } \
	\
	template<typename Handler> inline \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		using_hook_handler_final(const Handler& handler) const { \
		return handler(_mutex, _final); } \
	\
	template<typename Handler, typename Return_Handler> inline\
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		using_hook_handler_final(const Handler& handler, const Return_Handler& ret_handler) { \
		return handler(_mutex, _final, ret_handler); } \
	\
	template<typename Handler, typename Return_Handler> inline \
	typename ::yggr::func::foo_t_info<Handler>::result_type \
		using_hook_handler_final(const Handler& handler, const Return_Handler& ret_handler) const { \
		return handler(_mutex, _final, ret_handler); }

namespace yggr
{
namespace safe_container
{
namespace multi_index_detail
{
} // namespace multi_index_detail
} // namespace safe_container
} // namespace yggr


#endif // __YGGR_SAFE_CONTAINER_MULTI_INDEX_INDEXES_USING_HANDLER_IMPL_HPP__
