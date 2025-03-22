// genner_getter_typeid.hpp

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

#ifndef __YGGR_MATH_MOVING_AVG_GENNER_GETTER_TPYEID_HPP__
#define __YGGR_MATH_MOVING_AVG_GENNER_GETTER_TPYEID_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/integral_c.hpp>

namespace yggr
{
namespace math
{
namespace moving_avg
{

struct genner_getter_typeid_def
{
public:
	typedef u32 value_type;

public:
	typedef boost::mpl::integral_c<value_type, 0x00000000> typeid_start_type;

	typedef boost::mpl::integral_c<value_type, 1> identical_type;
	typedef boost::mpl::integral_c<value_type, 2> pow2_type;

	typedef boost::mpl::integral_c<value_type, 0x0000ffff> typeid_end_type;

	typedef boost::mpl::integral_c<value_type, typeid_end_type::value + 2> typeid_users_start_type;
	typedef boost::mpl::integral_c<value_type, 0x7fffffff> typeid_users_end_type;

	typedef boost::mpl::integral_c<value_type, 0xfffffffd> typeid_err_genner_type;
	typedef boost::mpl::integral_c<value_type, 0xfffffffe> typeid_err_getter_type;

public:
	YGGR_STATIC_CONSTANT(value_type, E_typeid_start = typeid_start_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_identical = identical_type::value);
	YGGR_STATIC_CONSTANT(value_type, E_pow2 = pow2_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_typeid_end = typeid_end_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_typeid_users_start = typeid_users_start_type::value);
	YGGR_STATIC_CONSTANT(value_type, E_typeid_users_end = typeid_users_end_type::value);

	YGGR_STATIC_CONSTANT(value_type, E_typeid_err_genner = typeid_err_genner_type::value);
	YGGR_STATIC_CONSTANT(value_type, E_typeid_err_getter = typeid_err_getter_type::value);

};

template<typename T>
struct genner_id_getter
	: public genner_getter_typeid_def::typeid_err_genner_type
{
};

template<typename T>
struct getter_id_getter
	: public genner_getter_typeid_def::typeid_err_getter_type
{
};

template<typename Genner, typename Getter>
struct is_matched_genner_and_getter 
	: public 
		boost::mpl::bool_
		<
			(genner_id_getter<Genner>::value == getter_id_getter<Getter>::value)
		>
{
};

} // namespace moving_avg
} // namespace math
} // namespace yggr

//genner_typeid
#define YGGR_MAKE_MOVING_AVG_GENNER_TYPEID( __template__, __id__ ) \
	namespace yggr{ namespace math{ namespace moving_avg { \
	template<typename T> \
	struct genner_id_getter< __template__< T > > \
		: public boost::mpl::integral_c< u32, __id__ > { }; }}}

#define YGGR_MAKE_MOVING_AVG_GENNER_TYPEID_INNER( __template__ ) \
	namespace yggr{ namespace math{ namespace moving_avg { \
	template<typename T> \
	struct genner_id_getter< __template__< T > > \
		: public boost::mpl::integral_c< u32, __template__< T >::E_typeid > { }; }}}

#define YGGR_GET_MOVING_AVG_GENNER_TYPEID( __type__ ) \
	::yggr::math::moving_avg::genner_id_getter< __type__ >::value

// getter_typeid
#define YGGR_MAKE_MOVING_AVG_GETTER_TYPEID( __template__, __id__ ) \
namespace yggr{ namespace math{ namespace moving_avg { \
	template<typename T> \
	struct getter_id_getter< __template__< T > > \
		: public boost::mpl::integral_c< u32, __id__ > { }; }}}

#define YGGR_MAKE_MOVING_AVG_GETTER_TYPEID_INNER( __template__ ) \
	namespace yggr{ namespace math{ namespace moving_avg { \
	template<typename T> \
	struct getter_id_getter< __template__< T > > \
		: public boost::mpl::integral_c< u32, __template__< T >::E_typeid > { }; }}}

#define YGGR_GET_MOVING_AVG_GETTER_TYPEID( __type__ ) \
	::yggr::math::moving_avg::getter_id_getter< __type__ >::value

#endif // __YGGR_MATH_MOVING_AVG_GENNER_GETTER_TPYEID_HPP__
