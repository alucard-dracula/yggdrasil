//pak_back_id.hpp

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

#ifndef __YGGR_NETWORK_START_DATA_PAK_BACK_ID_HPP__
#define __YGGR_NETWORK_START_DATA_PAK_BACK_ID_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/move/move.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/utility/copy_or_move_or_swap.hpp>
#include <yggr/task_center/task_info.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/network/start_data/start_data_def.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>

namespace yggr
{
namespace network
{
namespace start_data
{

template<typename ID,
			typename TaskDataInfo = task_center::default_task_data_info_type>
class pak_back_id
{
public:
	typedef ID id_type;
	typedef TaskDataInfo task_data_info_type;

private:
	typedef pak_back_id this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (start_data_def::E_id_start + 1), 1)

private:
	YGGR_PP_CHECK_START_DATA_ID(E_pak_id);

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(_id);
	}

public:
	pak_back_id(void)
		: _id()
	{
	}

	explicit pak_back_id(const id_type& nid)
		: _id(nid)
	{
	}

	pak_back_id(BOOST_RV_REF(this_type) right)
		: _id(boost::move(right._id))
	{
	}

	pak_back_id(const this_type& right)
		: _id(right._id)
	{
	}

	~pak_back_id(void)
	{
	}

public:
	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		copy_or_move_or_swap(_id, boost::move(right._id));
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		_id = right._id;
		return *this;
	}

public:
	inline void swap(BOOST_RV_REF(this_type) right)
	{
		this_type& right_ref = right;
		this_type::swap(right_ref);
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		yggr::swap(_id, right._id);
	}

public:
	inline const id_type& id(void) const
	{
		return _id;
	}

private:
	id_type _id;
};

} // namespace start_data
} // namespace network
} // namespace yggr

namespace yggr
{
namespace network
{
namespace start_data
{
namespace swap_support
{

template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T) > inline 
void swap(pak_back_id< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T) >& l, 
			pak_back_id<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T)>& r)
{ 
	l.swap(r); 
}

} // namespace swap_support

using swap_support::swap;

} // namespace start_data
} // namespace network
} // namespace yggr

namespace std
{
	using ::yggr::network::start_data::swap_support::swap;
} // namespace std

namespace boost
{
	using ::yggr::network::start_data::swap_support::swap;
} // namespace boost

#include <yggr/network/start_data/start_data_generator.hpp>

namespace yggr
{
namespace network
{
namespace start_data
{

template<typename ID, typename TaskDataInfo>
struct start_data_generator< pak_back_id<ID, TaskDataInfo> >
{
	typedef ID id_type;
	typedef TaskDataInfo task_data_info_type;
	typedef pak_back_id<id_type, task_data_info_type> start_data_type;

	inline start_data_type operator()(const id_type& id) const //create
	{
		return start_data_type(id);
	}
};

template<typename ID, typename TaskDataInfo>
struct start_data_loader< pak_back_id<ID, TaskDataInfo> >
{
	typedef ID id_type;
	typedef TaskDataInfo task_data_info_type;
	typedef pak_back_id<id_type, task_data_info_type> start_data_type;

	template<typename Packet> inline
	start_data_type operator()(Packet& pak) const // getter
	{
		start_data_type sdata;
		pak.load(sdata);
		return sdata;
	}
};

} // namespcae start_data
} // namespace network
} // namespace yggr

#endif //__YGGR_NETWORK_START_DATA_PAK_BACK_ID_HPP__
