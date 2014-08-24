//c_mongo_write_concern.hpp

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

#ifndef __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_HPP__
#define __YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_HPP__

#include <mongo.h>
//#include <yggr/move/move.hpp>

namespace yggr
{
namespace nsql_database_system
{

class c_mongo_write_concern 
	: public mongo_write_concern
{
public:
	typedef mongo_write_concern base_type;
private:
	typedef c_mongo_write_concern this_type;

	//BOOST_COPYABLE_AND_MOVABLE(this_type)

public:
	c_mongo_write_concern(void);
	explicit c_mongo_write_concern(int w_count);
	//c_mongo_write_concern(BOOST_RV_REF(this_type) right);
	//c_mongo_write_concern(BOOST_RV_REF(this_type) right)
	//{
	//	this_type::swap(right);
	//}

	c_mongo_write_concern(const this_type& right);
	~c_mongo_write_concern(void);

	//this_type& operator=(BOOST_RV_REF(this_type) right);
	//inline this_type& operator=(BOOST_RV_REF(this_type) right)
	//{
	//	this_type::swap(right);
	//	return *this;
	//}

	this_type& operator=(const this_type& right);

	void swap(this_type& right);
};

} // namespace nsql_database_system
} // namespace yggr

#define _YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_SWAP_DEF() \
	void swap(yggr::nsql_database_system::c_mongo_write_concern& l, \
				yggr::nsql_database_system::c_mongo_write_concern& r); 

#undef _YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_SWAP_DEF

namespace std
{
} // namespace 

#endif //__YGGR_NSQL_DATABASE_SYSTEM_C_MONGO_WRITE_CONCERN_HPP__