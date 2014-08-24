//c_bson_err.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_ERROR_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_BSON_ERROR_HPP__

#include <utility>
#include <yggr/charset/string.hpp>
#include <yggr/nsql_database_system/c_bson.hpp>


namespace yggr
{
namespace nsql_database_system
{

class c_bson_err : public c_bson
{
public:
	typedef c_bson base_type;
private:
	typedef c_bson_err this_type;
	BOOST_COPYABLE_AND_MOVABLE(this_type)

public:

	c_bson_err(void);

	c_bson_err(BOOST_RV_REF(bson) right)
		: base_type(boost::forward<bson>(right))
	{
	}
	c_bson_err(const bson& right);

	c_bson_err(BOOST_RV_REF(base_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}

	c_bson_err(const base_type& right);

	c_bson_err(BOOST_RV_REF(this_type) right)
		: base_type(boost::forward<base_type>(right))
	{
	}

	c_bson_err(const this_type& right);

	~c_bson_err(void);

	inline this_type& operator=(BOOST_RV_REF(bson) right)
	{
		base_type::operator=(boost::forward<bson>(right));
		return *this;
	}

	this_type& operator=(const bson& right);

	inline this_type& operator=(BOOST_RV_REF(base_type) right)
	{
		base_type::operator=(boost::forward<base_type>(right));
		return *this;
	}

	this_type& operator=(const base_type& right);

	inline this_type& operator=(BOOST_RV_REF(this_type) right)
	{
		base_type::operator=(boost::forward<base_type>(right));
		return *this;
	}

	this_type& operator=(const this_type& right);

	inline void swap(BOOST_RV_REF(bson) right)
	{
		base_type::swap(right);
	}

	void swap(bson& right);

	inline void swap(BOOST_RV_REF(base_type) right)
	{
		base_type::swap(right);
	}

	void swap(base_type& right);

	inline void swap(BOOST_RV_REF(this_type) right)
	{
		base_type::swap(right);
	}

	void swap(this_type& right);

	std::pair<u32, const std::string> err_info(void) const;

	std::pair<const std::string, bool> what(void) const;
};

} // namespace nsql_database_system
} // namespace yggr

#define _YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_DEF_1() \
	inline void swap(BOOST_RV_REF(yggr::nsql_database_system::c_bson_err) l, bson& r) { \
		l.swap(r); } \
	\
	inline void swap(bson& l, BOOST_RV_REF(yggr::nsql_database_system::c_bson_err) r) { \
		r.swap(l); } \
	\
	inline void swap(BOOST_RV_REF(yggr::nsql_database_system::c_bson_err) l, \
				yggr::nsql_database_system::c_bson_err::base_type& r) { \
		r.swap(l); } \
	\
	inline void swap(yggr::nsql_database_system::c_bson_err::base_type& l, \
				BOOST_RV_REF(yggr::nsql_database_system::c_bson_err) r) { \
		l.swap(r); }

#define _YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_DEF() \
	inline void swap(yggr::nsql_database_system::c_bson_err& l, BOOST_RV_REF(bson) r) { \
		l.swap(r); } \
	\
	inline void swap(BOOST_RV_REF(bson) l, yggr::nsql_database_system::c_bson_err& r) { \
		r.swap(l); } \
	\
	inline void swap(yggr::nsql_database_system::c_bson_err& l, \
				BOOST_RV_REF(yggr::nsql_database_system::c_bson_err::base_type) r) { \
		l.swap(r); } \
	\
	inline void swap(BOOST_RV_REF(yggr::nsql_database_system::c_bson_err::base_type) l, \
				yggr::nsql_database_system::c_bson_err& r) { \
		r.swap(l); } \
	\
	inline void swap(BOOST_RV_REF(yggr::nsql_database_system::c_bson_err) l, \
				yggr::nsql_database_system::c_bson_err& r) { \
		r.swap(l); } \
	\
	inline void swap(yggr::nsql_database_system::c_bson_err& l, \
				BOOST_RV_REF(yggr::nsql_database_system::c_bson_err) r) { \
		l.swap(r); } \
	\
	_YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_DEF_1() \
	\
	void swap(yggr::nsql_database_system::c_bson_err& l, bson& r); \
	void swap(bson& l, yggr::nsql_database_system::c_bson_err& r); \
	void swap(yggr::nsql_database_system::c_bson_err& l, \
				yggr::nsql_database_system::c_bson_err::base_type& r); \
	void swap(yggr::nsql_database_system::c_bson_err::base_type& l, \
				yggr::nsql_database_system::c_bson_err& r); \
	void swap(yggr::nsql_database_system::c_bson_err& l, \
				yggr::nsql_database_system::c_bson_err& r);


namespace std
{
	_YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_DEF()
} //namespace std

namespace boost
{
	_YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_DEF()
} // namespace boost

#undef _YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_DEF
#undef _YGGR_NSQL_DATABASE_SYSTEM_BSON_ERROR_SWAP_DEF_1

#endif //__YGGR_NSQL_DATABASE_SYSTEM_C_BSON_ERROR_HPP__
