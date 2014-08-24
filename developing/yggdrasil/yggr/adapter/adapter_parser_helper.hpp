//adapter_parser_helper.hpp

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

#ifndef __YGGR_ADAPTER_ADAPTER_PARSER_HELPER_HPP__
#define __YGGR_ADAPTER_ADAPTER_PARSER_HELPER_HPP__

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/mpl/assert.hpp>
#include <yggr/ids/ids_parsers.hpp>

namespace yggr
{
namespace adapter
{

template<	typename Reg_Send_ID_Parser,
			typename Reg_Recv_ID_Parser,
			typename Send_ID_Parser,
			typename Task_Send_ID_Parser,
			typename Task_Recv_ID_Parser,
			typename Recv_ID_Parser>
struct adaper_id_parser_def
{
	typedef Reg_Send_ID_Parser reg_send_id_parser_type;
	typedef Reg_Recv_ID_Parser reg_recv_id_parser_type;
	typedef Send_ID_Parser send_id_parser_type;
	typedef Task_Send_ID_Parser task_send_id_parser_type;
	typedef Task_Recv_ID_Parser task_recv_id_parser_type;
	typedef Recv_ID_Parser recv_id_parser_type;

	typedef typename boost::remove_reference
			<
				typename boost::remove_const
				<
					typename reg_send_id_parser_type::result_type
				>::type
			>::type reg_send_id_type;

	typedef typename boost::remove_reference
			<
				typename boost::remove_const
				<
					typename reg_recv_id_parser_type::result_type
				>::type
			>::type reg_recv_id_type;

	typedef typename boost::remove_reference
			<
				typename boost::remove_const
				<
					typename send_id_parser_type::result_type
				>::type
			>::type send_id_type;

	typedef typename boost::remove_reference
			<
				typename boost::remove_const
				<
					typename task_send_id_parser_type::result_type
				>::type
			>::type task_send_id_type;

	typedef typename boost::remove_reference
			<
				typename boost::remove_const
				<
					typename task_recv_id_parser_type::result_type
				>::type
			>::type task_recv_id_type;

	typedef typename boost::remove_reference
			<
				typename boost::remove_const
				<
					typename recv_id_parser_type::result_type
				>::type
			>::type recv_id_type;

	BOOST_MPL_ASSERT((boost::is_same<reg_send_id_type, send_id_type>));
	BOOST_MPL_ASSERT((boost::is_same<send_id_type, task_send_id_type>));
	BOOST_MPL_ASSERT((boost::is_same<reg_recv_id_type, recv_id_type>));
	BOOST_MPL_ASSERT((boost::is_same<recv_id_type, task_recv_id_type>));

};

template<typename TagSend,
			typename TagRecv,
			typename Parser_Def>
struct adapter_id_parser
{
	typedef TagSend tag_send;
	typedef TagRecv tag_recv;

	typedef Parser_Def parser_def_type;

	typedef typename parser_def_type::reg_send_id_parser_type reg_send_id_parser_type;
	typedef typename parser_def_type::reg_recv_id_parser_type reg_recv_id_parser_type;
	typedef typename parser_def_type::send_id_parser_type send_id_parser_type;
	typedef typename parser_def_type::task_send_id_parser_type task_send_id_parser_type;
	typedef typename parser_def_type::task_recv_id_parser_type task_recv_id_parser_type;
	typedef typename parser_def_type::recv_id_parser_type recv_id_parser_type;

	template<typename Tag, typename Nil_T = int>
	struct selector;

	template<typename Nil_T>
	struct selector<tag_send, Nil_T>
	{
		template<typename Task>
		typename task_send_id_parser_type::result_type
			task_id(const Task& tk) const
		{
			task_send_id_parser_type parser;
			return parser(tk);
		}

		template<typename Val>
		typename send_id_parser_type::result_type
			value_id(const Val& val) const
		{
			send_id_parser_type parser;
			return parser(val);
		}

		template<typename Val>
		typename reg_send_id_parser_type::result_type
			reg_value_id(void) const
		{
			reg_send_id_parser_type parser;
#ifdef _MSC_VER
			return parser.operator()<Val>();
#else
			return parser.template operator()<Val>();
#endif // _MSC_VER
		}
	};

	template<typename Nil_T>
	struct selector<tag_recv, Nil_T>
	{
		template<typename Task>
		typename task_recv_id_parser_type::result_type
			task_id(const Task& tk) const
		{
			task_recv_id_parser_type parser;
			return parser(tk);
		}

		template<typename Task>
		typename recv_id_parser_type::result_type
			value_id(const Task& tk) const
		{
			task_recv_id_parser_type parser;
			return parser(tk);
		}

		template<typename Val>
		typename reg_recv_id_parser_type::result_type
			reg_value_id(void) const
		{
			reg_recv_id_parser_type parser;
#ifdef _MSC_VER
			return parser.operator()<Val>();
#else
			return parser.template operator()<Val>();
#endif // _MSC_VER
		}
	};

	template<typename Tag, typename Task>
	typename Tag::id_type task_id(const Task& tk) const
	{
		selector<Tag> sel;
		return sel.task_id(tk);
	}

	template<typename Tag, typename Val>
	typename Tag::id_type value_id(const Val& val) const
	{
		selector<Tag> sel;
		return sel.value_id(val);
	}

	template<typename Tag, typename Val>
	typename Tag::id_type reg_value_id(void) const
	{
		selector<Tag> sel;
		return sel.template reg_value_id<Val>();
	}
};

} // namespace adapter
} // namespace yggr





#endif // __YGGR_ADAPTER_ADAPTER_PARSER_HELPER_HPP__
