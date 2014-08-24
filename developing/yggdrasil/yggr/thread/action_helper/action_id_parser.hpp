// action_id_parser.hpp

/****************************************************************************
Copyright (c) 2014-2018 yggdrasil

author: yang xu

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

#ifndef __YGGR_THREAD_ACTION_HELPER_ACTION_ID_PARSER_HPP__
#define __YGGR_THREAD_ACTION_HELPER_ACTION_ID_PARSER_HPP__

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/assert.hpp>

#include <yggr/ids/ids_parsers.hpp>

namespace yggr
{
namespace thread
{
namespace action_helper
{

template<	typename Reg_ID_Parser,
			typename Task_ID_Parser>
struct action_id_parser
{
private:
	typedef Reg_ID_Parser reg_id_parser_type;
	typedef Task_ID_Parser task_id_parser_type;

	typedef typename boost::remove_reference
			<
				typename boost::remove_const
				<
					typename reg_id_parser_type::result_type
				>::type
			>::type reg_id_type;

	typedef typename boost::remove_reference
			<
				typename boost::remove_const
				<
					typename task_id_parser_type::result_type
				>::type
			>::type task_id_type;

	BOOST_MPL_ASSERT((	typename boost::mpl::if_
						<
							typename boost::is_same
							<
								reg_id_type,
								task_id_type
							>::type,
							boost::mpl::true_,
							typename boost::is_same
							<
								const reg_id_type,
								task_id_type
							>
						>));

public:
	typedef reg_id_type id_type;

public:
	template<typename T>
	static typename reg_id_parser_type::result_type get_reg_id(void)
	{
		reg_id_parser_type parser;
#ifdef _MSC_VER
		return parser.operator()<T>();
#else
		return parser.template operator()<T>();
#endif // _MSC_VER
	}

	template<typename Task>
	static typename task_id_parser_type::result_type get_task_id(const Task& tk)
	{
		task_id_parser_type parser;
		return parser(tk);
	}

};

} // namespace action_helper
} // namespace thread
} // namespace yggr

#endif // __YGGR_THREAD_ACTION_HELPER_ACTION_ID_PARSER_HPP__
