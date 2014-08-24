//pak_back_id.hpp

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

#ifndef __YGGR_NETWORK_START_DATA_PAK_BACK_ID_HPP__
#define __YGGR_NETWORK_START_DATA_PAK_BACK_ID_HPP__

#include <boost/serialization/access.hpp>
#include <yggr/move/move.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/serialization/nvp.hpp>
#include <yggr/task_center/support/task_import.hpp>
#include <yggr/network/start_data/start_data_def.hpp>

namespace yggr
{
namespace network
{
namespace start_data
{

template<typename ID,
			typename Task_Data_Info = task_center::default_task_data_info_type>
class pak_back_id
{
public:
	typedef ID id_type;
	typedef Task_Data_Info task_data_info_type;

public:
	enum
	{
		E_pak_id = start_data_def::E_id_start + 1,

		E_id_compile_u32 = 0xffffffff
	};

	enum
	{
		E_version_start = 0x00000000,
		E_now_ver,

		E_ver_compile_u32 = 0xffffffff
	};

private:

	YGGR_PP_CHECK_START_DATA_ID(E_pak_id);
	typedef pak_back_id this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const u32 version)
	{
		ar & YGGR_SERIALIZE_NVP(_id);
	}

public:

	pak_back_id(void)
		: _id(0)
	{
	}

	explicit pak_back_id(const id_type& nid)
		: _id(nid)
	{
	}

#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
	pak_back_id(BOOST_RV_REF(this_type) right)
		: _id(boost::forward<id_type>(right._id))
	{
	}
#else
	pak_back_id(BOOST_RV_REF(this_type) right)
		: _id(id_type())
	{
		this_type& right_ref = right;
		std::swap(_id, right_ref._id);
	}
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES

	pak_back_id(const this_type& right)
		: _id(right._id)
	{
	}

	~pak_back_id(void)
	{
	}

	this_type& operator=(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		_id = boost::forward<id_type>(right._id);
#else
		this_type& right_ref = right;
		std::swap(_id, right_ref._id);
#endif //YGGR_NO_CXX11_RVALUE_REFERENCES
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right) {return *this;}
		_id = right._id;
		return *this;
	}

	const id_type& id(void) const
	{
		return _id;
	}

	void swap(BOOST_RV_REF(this_type) right)
	{
#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES
		std::swap(_id, boost::forward<id_type>(right._id));
#else
		this_type& right_ref = right;
		std::swap(_id, right_ref._id);
#endif // YGGR_NO_CXX11_RVALUE_REFERENCES
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}
		std::swap(_id, right._id);
	}

	YGGR_FAST_CAL_TASK_IMPORT(typename task_data_info_type, this_type, E_pak_id, E_now_ver)

private:
	id_type _id;
};

} // namespace start_data
} // namespace network
} // namespace yggr

#define _YGGR_PP_NETWORK_START_DATA_SWAP() \
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T) > inline \
	void swap(yggr::network::start_data::pak_back_id< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T) >& l, \
				yggr::network::start_data::pak_back_id<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T)>& r) { \
		l.swap(r); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T) > inline \
	void swap(BOOST_RV_REF(yggr::network::start_data::pak_back_id<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T)>) l, \
				yggr::network::start_data::pak_back_id<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T)>& r) { \
		r.swap(l); } \
	\
	template< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, typename T) > inline \
	void swap(yggr::network::start_data::pak_back_id<YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T)>& l, \
				BOOST_RV_REF(yggr::network::start_data::pak_back_id< YGGR_PP_TEMPLATE_PARAMS_TYPES( 2, T) >) r) { \
		l.swap(r); }

namespace std
{
	_YGGR_PP_NETWORK_START_DATA_SWAP()
} // namespace std

namespace boost
{
	_YGGR_PP_NETWORK_START_DATA_SWAP()
} // namespace boost

#undef _YGGR_PP_NETWORK_START_DATA_SWAP

#include <yggr/network/start_data/start_data_generator.hpp>

namespace yggr
{
namespace network
{
namespace start_data
{

template<typename ID, typename Task_Data_Info>
struct start_data_generator< pak_back_id<ID, Task_Data_Info> >
{
	typedef ID id_type;
	typedef Task_Data_Info task_data_info_type;
	typedef pak_back_id<id_type, task_data_info_type> start_data_type;

	start_data_type operator()(const id_type& id) const //create
	{
		return start_data_type(id);
	}
};

template<typename ID, typename Task_Data_Info>
struct start_data_loader< pak_back_id<ID, Task_Data_Info> >
{
	typedef ID id_type;
	typedef Task_Data_Info task_data_info_type;
	typedef pak_back_id<id_type, task_data_info_type> start_data_type;

	template<typename Packet>
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
