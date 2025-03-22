//basic_proxy_mode.hpp

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

#ifndef __YGGR_PROXY_PROXY_MODE_BASIC_PROXY_MODE_HPP__
#define __YGGR_PROXY_PROXY_MODE_BASIC_PROXY_MODE_HPP__

#include <yggr/mplex/static_assert.hpp>
#include <yggr/utility/args_holder_nv.hpp>
#include <yggr/proxy/proxy_mode/proxy_mode_def.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/range/value_type.hpp>

namespace yggr
{
namespace proxy
{
namespace proxy_mode
{

template<typename ModeDataStore>
class basic_proxy_mode
{
public:
	typedef ModeDataStore mode_data_store_type;
	typedef typename mode_data_store_type::owner_info_container_type owner_info_container_type;
	typedef typename mode_data_store_type::owner_info_type owner_info_type;
	typedef typename mode_data_store_type::owner_id_type owner_id_type;

	typedef typename mode_data_store_type::store_type store_type;
	typedef typename mode_data_store_type::ex_args_type ex_args_type;

private:
	typedef basic_proxy_mode this_type;

public:
	basic_proxy_mode(u32 mode)
		: _mode(mode)
	{
	}

	virtual ~basic_proxy_mode(void) 
	{
	};

public:
	inline u32 now_mode(void) const
	{
		return _mode;
	}

public:
	inline owner_info_container_type&
		get_owner_info_container(owner_info_container_type& cont,
									const owner_info_type& owner_info,
									const store_type& store) const
	{
		return this->pro_get_owner_info_container(cont, owner_info, store, 0);
	}

	inline owner_info_container_type&
		get_owner_info_container(owner_info_container_type& cont,
									const owner_info_type& owner_info,
									const store_type& store,
									const utility::basic_args_holder_nv& args_holder) const
	{
		return 
			this->pro_get_owner_info_container(
				cont, owner_info, store, boost::addressof(args_holder));
	}

protected:
	virtual owner_info_container_type&
		pro_get_owner_info_container(owner_info_container_type& cont,
										const owner_info_type& owner_info,
										const store_type& store,
										const utility::basic_args_holder_nv* pargs_holder) const = 0;

protected:
	u32 _mode;
};


} // namespace proxy_mode
} // namespace proxy
} // namespace yggr

#endif // __YGGR_PROXY_PROXY_MODE_BASIC_PROXY_MODE_HPP__