// yggr serialization unliner_container.hpp

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

#ifndef __YGGR_SERIALIZATION_UNLINER_CONTAINER_HPP__
#define __YGGR_SERIALIZATION_UNLINER_CONTAINER_HPP__

#include <yggr/base/yggrdef.h>

#ifndef BOOST_CONFIG_HPP
#  include <boost/config.hpp>
#endif // BOOST_CONFIG_HPP

#include <yggr/ppex/typedef.hpp>

#include <yggr/serialization/collections_save_imp.hpp>
#include <yggr/serialization/collections_load_imp.hpp>

#include <boost/serialization/split_free.hpp>

#define YGGR_SERIALIZATION_UNLINER_CONTINER_SAVE_LOAD( __template_n__, __type__ ) \
	template<typename Archive, YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, typename T)> inline \
	void save(Archive& ar, \
				const __type__<YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, T)>& t, \
				const unsigned int) { \
		yggr::serialization::container::save_collection< \
			Archive, __type__<YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, T)> >(ar, t); } \
	\
	template<typename Archive, YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, typename T)> inline \
	void load(Archive& ar, \
				__type__<YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, T)>& t, \
				const unsigned int) { \
		yggr::serialization::container::load_collection< \
			Archive, \
			__type__<YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, T)>, \
			yggr::serialization::stl::archive_input_set< \
				Archive, __type__<YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, T)> >, \
			yggr::serialization::stl::no_reserve_imp< \
				__type__<YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, T)> > >(ar, t); } \
	\
	template<typename Archive, YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, typename T)> inline \
	void serialize(Archive& ar, \
					__type__<YGGR_PP_TEMPLATE_PARAMS_TYPES( __template_n__, T)> & t, \
					const unsigned int file_version) { \
		boost::serialization::split_free(ar, t, file_version); }


#endif // __YGGR_SERIALIZATION_UNLINER_CONTAINER_HPP__