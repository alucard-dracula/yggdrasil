//lua_cal_svr_procotol.hpp

#ifndef __CAL_SVR_PROCOTOL_HPP__
#define __CAL_SVR_PROCOTOL_HPP__

#include <yggr/base/yggrdef.h>
#include <yggr/base/static_constant.hpp>
#include <yggr/utility/swap.hpp>
#include <yggr/move/move.hpp>

#include <yggr/task_center/task_info.hpp>
#include <yggr/task_center/support/task_import.hpp>

#include <yggr/serialization/access.hpp>
#include <yggr/serialization/nvp.hpp>

#include <algorithm>

struct procotol_id_def
{
	YGGR_STATIC_CONSTANT(::yggr::u32, E_id_start = 0x00008100);
	YGGR_STATIC_CONSTANT(::yggr::u32, E_id_end = 0x00008200);
};

template<typename TaskDataInfo = yggr::task_center::default_task_data_info_type>
class cdt_procotol
{
public:
	typedef yggr::u32 num_type;
	typedef TaskDataInfo task_data_info_type;

private:
	typedef cdt_procotol this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// E_pak_id = procotol_id_def::E_id_start + 1
	// E_ver = 1
	YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (procotol_id_def::E_id_start + 1), 1)

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(num1);
		ar & YGGR_SERIALIZATION_NVP(num2);
	}

public:

	cdt_procotol(void)
		: num1(0), num2(0)
	{
	}

	cdt_procotol(const num_type& nnum1, const num_type nnum2)
		: num1(nnum1), num2(nnum2)
	{
	}

	cdt_procotol(BOOST_RV_REF(this_type) right)
		: num1(right.num1), num2(right.num2)
	{
	}

	cdt_procotol(const this_type& right)
		: num1(right.num1), num2(right.num2)
	{
	}

	~cdt_procotol(void)
	{
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		num1 = right.num1;
		num2 = right.num2;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		num1 = right.num1;
		num2 = right.num2;

		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		yggr::swap(num1, right.num1);
		yggr::swap(num2, right.num2);
	}

	inline void set_num1(const num_type& num)
	{
		num1 = num;
	}

	inline const num_type& get_num1(void) const
	{
		return num1;
	}

	inline void set_num2(const num_type& num)
	{
		num2 = num;
	}

	inline const num_type& get_num2(void) const
	{
		return num2;
	}

public:
	num_type num1;
	num_type num2;
};

template<typename TaskDataInfo = yggr::task_center::default_task_data_info_type>
class rst_procotol
{
public:
	typedef yggr::u32 num_type;

	typedef TaskDataInfo task_data_info_type;

private:
	typedef rst_procotol this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	// E_pak_id = procotol_id_def::E_id_start + 2
	// E_ver = 1
	YGGR_FAST_CAL_TASK_IMPORT_TPL(task_data_info_type, this_type, (procotol_id_def::E_id_start + 2), 1)

private:
	friend class yggr::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, yggr::u32 version)
	{
		ar & YGGR_SERIALIZATION_NVP(rst);
	}

public:

	rst_procotol(void)
		: rst(0)
	{
	}

	rst_procotol(const num_type& nrst)
		: rst(nrst)
	{
	}

	rst_procotol(BOOST_RV_REF(this_type) right)
		: rst(right.rst)
	{
	}

	rst_procotol(const this_type& right)
		: rst(right.rst)
	{
	}

	~rst_procotol(void)
	{
	}

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		this_type& right_chk = right;
		if(this == &right_chk)
		{
			return *this;
		}

		rst = right.rst;
		return *this;
	}

	this_type& operator=(const this_type& right)
	{
		if(this == &right)
		{
			return *this;
		}

		rst = right.rst;
		return *this;
	}

	void swap(this_type& right)
	{
		if(this == &right)
		{
			return;
		}

		yggr::swap(rst, right.rst);
	}

	inline void set_rst(const num_type& num)
	{
		rst = num;
	}

	inline const num_type& get_rst(void) const
	{
		return rst;
	}

public:
	num_type rst;
};

#endif //__CAL_SVR_PROCOTOL_HPP__
