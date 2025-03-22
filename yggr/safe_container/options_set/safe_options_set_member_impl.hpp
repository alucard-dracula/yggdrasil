//safe_options_set_member_impl.hpp

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

#ifndef __YGGR_SAFE_CONTAINER_OPTIONS_SET_SAFE_OPTIONS_SET_MEMBER_IMPL_HPP__
#define __YGGR_SAFE_CONTAINER_OPTIONS_SET_SAFE_OPTIONS_SET_MEMBER_IMPL_HPP__

#ifndef __YGGR_SAFE_CONTAINER_SAFE_OPTIONS_SET_HPP__
#	error "this file is impl file, can't be used alone"
#endif // __YGGR_SAFE_CONTAINER_SAFE_OPTIONS_SET_HPP__

#include <yggr/safe_container/detail/safe_construct_impl.hpp>
#include <yggr/safe_container/detail/merge_sets_impl.hpp>

#define YGGR_PP_SAFE_OPTIONS_SET_CONSTRUCTOR_IMPL() \
	YGGR_PP_SAFE_KV_CONSTRUCTOR_IMPL(safe_options_set) \
	\
	template<typename InputIterator> \
	safe_options_set(container::ordered_unique_range_t, \
						InputIterator first, InputIterator last, \
						const key_compare& cmp) \
		: _base(container::ordered_range, first, last, cmp) {} \
	\
	template<typename InputIterator> \
	safe_options_set(container::ordered_unique_range_t, \
						InputIterator first, InputIterator last, \
						const allocator_type& alloc) \
		: _base(container::ordered_range, first, last, key_compare(), alloc) {} \
	\
	template<typename InputIterator> \
	safe_options_set(container::ordered_unique_range_t, \
						InputIterator first, InputIterator last, \
						const key_compare& cmp, const allocator_type& alloc) \
		: _base(container::ordered_range, first, last, cmp, alloc) {}

#define YGGR_PP_SAFE_OPTIONS_SET_ISOMERIC_SAME_CONSTRUCTOR_IMPL( __base_tpl__ ) \
	YGGR_PP_SAFE_KV_ISOMERIC_SAME_CONSTRUCTOR_IMPL(safe_options_set, 5, __base_tpl__, container::is_isomeric_same_options_set)

#define YGGR_PP_SAFE_OPTIONS_MULTISET_CONSTRUCTOR_IMPL() \
	YGGR_PP_SAFE_KV_CONSTRUCTOR_IMPL(safe_options_multiset) \
	\
	template<typename InputIterator> \
	safe_options_multiset(container::ordered_range_t, \
							InputIterator first, InputIterator last, \
							const key_compare& cmp) \
		: _base(container::ordered_range, first, last, cmp) {} \
	\
	template<typename InputIterator> \
	safe_options_multiset(container::ordered_range_t, \
							InputIterator first, InputIterator last, \
							const allocator_type& alloc) \
		: _base(container::ordered_range, first, last, key_compare(), alloc) {} \
	\
	template<typename InputIterator> \
	safe_options_multiset(container::ordered_range_t, \
							InputIterator first, InputIterator last, \
							const key_compare& cmp, const allocator_type& alloc) \
		: _base(container::ordered_range, first, last, cmp, alloc) {}

#define YGGR_PP_SAFE_OPTIONS_MULTISET_ISOMERIC_SAME_CONSTRUCTOR_IMPL( __base_tpl__ ) \
	YGGR_PP_SAFE_KV_ISOMERIC_SAME_CONSTRUCTOR_IMPL(safe_options_multiset, 5, __base_tpl__, container::is_isomeric_same_options_multiset)

#define YGGR_PP_SAFE_OPTIONS_SET_DESTRUCTOR_IMPL() \
	YGGR_PP_SAFE_KV_DESTRUCTOR_IMPL(safe_options_set)

#define YGGR_PP_SAFE_OPTIONS_MULTISET_DESTRUCTOR_IMPL() \
	YGGR_PP_SAFE_KV_DESTRUCTOR_IMPL(safe_options_multiset)


#endif // __YGGR_SAFE_CONTAINER_OPTIONS_SET_SAFE_OPTIONS_SET_MEMBER_IMPL_HPP__
