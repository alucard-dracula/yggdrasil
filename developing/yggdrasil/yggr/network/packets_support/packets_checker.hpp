//packets_checker.hpp

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

#ifndef __YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_CHECKER_HPP__
#define __YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_CHECKER_HPP__

#include <ctime>

#include <yggr/base/error_make.hpp>
#include <yggr/ids/base_ids_def.hpp>

namespace yggr
{
namespace network
{
namespace packets_support
{

template<typename OwnerInfo>
struct basic_owner_info_parser
{
public:
	typedef OwnerInfo owner_info_type;
	typedef typename owner_info_type::owner_id_type id_type;

private:
	typedef basic_owner_info_parser this_type;

public:
	template<typename Pak>
	static const id_type owner_id(const Pak& pak)
	{
		return pak.net_info().owner_id();
	}

	template<typename Pak>
	static const id_type proxy_id(const Pak& pak)
	{
		return pak.net_info().proxy_id();
	}
};

template<typename OwnerInfo, std::time_t time_step = 0>
struct packets_checker
{
public:
	typedef OwnerInfo owner_info_type;
	typedef typename owner_info_type::owner_id_type id_type;
	typedef basic_owner_info_parser<owner_info_type> owner_id_parser_type;

public:
	ERROR_MAKER_BEGIN("packets_checker")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_10 + 10)
			ERROR_CODE_DEF(E_invalid_packet)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_10)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_packet, "invalid_packet")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

private:
	typedef packets_checker this_type;

public:
	packets_checker(const id_type& id)
		: _id(id)
	{
	}

	~packets_checker(void)
	{
	}

	template<typename Pak>
	error_type operator()(const Pak& pak) const
	{
		return error_maker_type::make_error(((this_type::get_pak_now_id(pak) == _id)
												&& pak.time_alive(time_step) ?
												0 : error_maker_type::E_invalid_packet));
	}

private:
	template<typename Pak>
	const id_type get_pak_now_id(const Pak& pak) const
	{
		return owner_id_parser_type::owner_id(pak);
	}

private:
	id_type _id;
};

template<typename OwnerInfo>
struct packets_checker<OwnerInfo, 0>
{
public:
	typedef OwnerInfo owner_info_type;
	typedef typename owner_info_type::owner_id_type id_type;
	typedef basic_owner_info_parser<owner_info_type> owner_id_parser_type;

public:
	ERROR_MAKER_BEGIN("packets_checker")
		ERROR_CODE_DEF_BEGIN(exception::exception_sort::E_code_start_10 + 10)
			ERROR_CODE_DEF(E_invalid_packet)
		ERROR_CODE_DEF_END(exception::exception_sort::E_code_end_10)

		ERROR_CODE_MSG_BEGIN()
			ERROR_CODE_MSG(E_invalid_packet, "invalid_packet")
		ERROR_CODE_MSG_END()
	ERROR_MAKER_END()

private:
	typedef packets_checker this_type;

public:
	packets_checker(const id_type& id)
		: _id(id)
	{
	}

	~packets_checker(void)
	{
	}

	template<typename Pak>
	error_type operator()(const Pak& pak) const
	{
		return error_maker_type::make_error((this_type::get_pak_now_id(pak) == _id?
												0 : error_maker_type::E_invalid_packet));
	}

private:
	template<typename Pak>
	const id_type get_pak_now_id(const Pak& pak) const
	{
		return owner_id_parser_type::owner_id(pak);
	}

private:
	id_type _id;
};


} // namespace packets_support
} // namespace network
} // namespce yggr

#endif // __YGGR_NETWORK_PACKETS_SUPPORT_PACKETS_CHECKER_HPP__
