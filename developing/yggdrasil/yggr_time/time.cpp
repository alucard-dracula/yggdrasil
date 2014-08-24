//time.cpp

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

#if defined(_MSC_VER) && defined(_DEBUG)
#	include <vld.h>
#endif // _MSC_VER

#include <yggr/time/time.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <limits>


namespace yggr
{
namespace time
{

time::time(void)
{
	base_type& base = *this;
	boost::xtime_get(&base, boost::TIME_UTC_); //OldVer append TIME_UTC_ defined
}

time::time(const time::xtime_sec_t& sec)
{
	base_type::sec = sec;
	base_type::nsec = 0;
}

time::time(const time::xtime_sec_t& sec, const time::xtime_nsec_t& nsec)
{
	base_type::sec = sec;
	base_type::sec += nsec / E_NS_TO_S;
	base_type::nsec = nsec % E_NS_TO_S;
}

time::time(const time::base_type& right)
{
	base_type::sec = right.sec;
	base_type::sec += right.nsec / E_NS_TO_S;
	base_type::nsec = right.nsec % E_NS_TO_S;
}

//time::time(BOOST_RV_REF(time::this_type) right)
//{
//	std::swap(base_type::sec, right.sec);
//	std::swap(base_type::nsec, right.nsec);
//}

time::time(const time::this_type& right)
{
	base_type::sec = right.sec;
	base_type::nsec = right.nsec;
}

time::~time(void)
{
}

time::this_type& time::operator= (const time::base_type& right)
{
	base_type::sec = sec;
	base_type::sec += nsec / E_NS_TO_S;
	base_type::nsec = nsec % E_NS_TO_S;
	return *this;
}

//time::this_type& time::operator= (BOOST_RV_REF(time::this_type) right)
//{
//	std::swap(base_type::sec, right.sec);
//	std::swap(base_type::nsec, right.nsec);
//	return *this;
//}

time::this_type& time::operator= (const time::this_type& right)
{
	base_type::sec = right.sec;
	base_type::nsec = right.nsec;
	return *this;
}

void time::swap(time::base_type& right)
{
	std::swap(base_type::sec, right.sec);
	std::swap(base_type::nsec, right.nsec);
}

//void time::swap(BOOST_RV_REF(time::this_type) right)
//{
//	std::swap(base_type::sec, right.sec);
//	std::swap(base_type::nsec, right.nsec);
//}

void time::swap(time::this_type& right)
{
	std::swap(base_type::sec, right.sec);
	std::swap(base_type::nsec, right.nsec);
}

bool time::operator==(const time::xtime_sec_t& sec) const
{
	return operator==(this_type(sec));
}

bool time::operator== (const time::base_type& right) const
{
	return base_type::sec == right.sec && base_type::nsec == right.nsec;
}

bool time::operator== (const time::this_type& right) const
{
	return base_type::sec == right.sec && base_type::nsec == right.nsec;
}

bool time::operator!= (const time::xtime_sec_t& sec) const
{
	return !operator==(sec);
}

bool time::operator!= (const time::base_type& right) const
{
	return !operator==(right);
}

bool time::operator!= (const time::this_type& right) const
{
	return !operator==(right);
}

bool time::operator<(const time::xtime_sec_t& sec) const
{
	return operator<(this_type(sec, 0));
}

bool time::operator< (const time::base_type& right) const
{
	return boost::xtime_cmp(*this, right) < 0;
}

bool time::operator< (const time::this_type& right) const
{
	return boost::xtime_cmp(*this, right) < 0;
}

bool time::operator<=(const time::xtime_sec_t& sec) const
{
	return operator<=(this_type(sec));
}

bool time::operator<= (const time::base_type& right) const
{
	return boost::xtime_cmp(*this, right) <= 0;
}

bool time::operator<= (const time::this_type& right) const
{
	return boost::xtime_cmp(*this, right) <= 0;
}

bool time::operator> (const time::xtime_sec_t& sec) const
{
	return operator>(this_type(sec));
}

bool time::operator> (const time::base_type& right) const
{
	return !operator<=(right);
}

bool time::operator> (const time::this_type& right) const
{
	return !operator<=(right);
}

bool time::operator>= (const time::xtime_sec_t& sec) const
{
	return operator>=(this_type(sec));
}

bool time::operator>= (const time::base_type& right) const
{
	return !operator<(right);
}

bool time::operator>= (const time::this_type& right) const
{
	return !operator<(right);
}

const time::this_type time::operator-(const time::xtime_sec_t& sec) const
{
	return operator-(this_type(sec));
}

const time::this_type time::operator-(const time::this_type& right) const
{
	xtime_sec_t tsec = sec;
	xtime_nsec_t tnsec = nsec;

	if(nsec - right.nsec < 0)
	{
		tsec -= 1;
		tnsec += E_NS_TO_S;
	}

	return this_type(tsec - right.sec, tnsec - right.nsec);
}

time::this_type& time::operator-=(const time::xtime_sec_t& sec)
{
	return operator-=(this_type(sec));
}

time::this_type& time::operator-=(const time::this_type& right)
{
	xtime_sec_t tsec = sec;
	xtime_nsec_t tnsec = nsec;

	if(nsec < right.nsec)
	{
		tsec -= 1;
		tnsec += E_NS_TO_S;
	}

	sec = tsec - right.sec;
	nsec = tnsec - right.nsec;
	return *this;
}

const time::this_type time::operator+(const time::xtime_sec_t& sec) const
{
	return operator+(this_type(sec));
}

const time::this_type time::operator+(const time::this_type& right) const
{
	return this_type(sec + right.sec + ((nsec + right.nsec) / E_NS_TO_S), (nsec + right.nsec) % E_NS_TO_S);
}


time::this_type& time::operator+=(const time::xtime_sec_t& sec)
{
	return operator+=(this_type(sec));
}

time::this_type& time::operator+=(const time::this_type& right)
{
	xtime_sec_t tsec = sec + right.sec + ((nsec + right.nsec) / E_NS_TO_S);
	xtime_nsec_t tnsec = (nsec + right.nsec) % E_NS_TO_S;

	sec = tsec;
	nsec = tnsec;

	return *this;
}

time::operator bool(void) const
{
	return base_type::sec || base_type::nsec;
}

void time::reset(void)
{
	base_type::sec = base_type::nsec = 0;
}

void time::update(void)
{
	base_type& base = *this;
	boost::xtime_get(&base, boost::TIME_UTC_);
}

time::operator time::base_type&(void)
{
	return *this;
}

time::operator const time::base_type&(void)
{
	return *this;
}

std::tm time::to_tm(void) const
{
	const boost::system_time& sys_time = *this;
	return boost::posix_time::to_tm(sys_time);
}

std::tm time::to_local_tm(void) const
{
	typedef boost::date_time::c_local_adjustor<boost::system_time> local_adj;

	const boost::system_time& sys_time = *this;
	boost::system_time local_sys_time = local_adj::utc_to_local(sys_time);

	return boost::posix_time::to_tm(local_sys_time);
}

std::string time::to_string(void) const
{
	std::string format("%Y%m%d%H%M%S");
	std::tm tm(to_tm());
	std::string::value_type buf[64] = {0};
	std::strftime(buf, sizeof(buf) - sizeof(std::string::value_type), format.c_str(), &tm);
	return std::string(buf);
}

std::string time::to_local_string(void) const
{
	std::string format("%Y%m%d%H%M%S");
	std::tm tm(to_local_tm());
	std::string::value_type buf[64] = {0};
	std::strftime(buf, sizeof(buf) - sizeof(std::string::value_type), format.c_str(), &tm);
	return std::string(buf);
}

std::string time::to_string(const std::string& format) const
{
	std::tm tm(to_tm());
	std::string::value_type buf[64] = {0};
	std::strftime(buf, sizeof(buf) - sizeof(std::string::value_type), format.c_str(), &tm);
	return std::string(buf);
}

std::string time::to_local_string(const std::string& format) const
{
	std::tm tm(to_local_tm());
	std::string::value_type buf[64] = {0};
	std::strftime(buf, sizeof(buf) - sizeof(std::string::value_type), format.c_str(), &tm);
	return std::string(buf);
}

const time::this_type time::max_time(void)
{
	return this_type(std::numeric_limits<xtime_sec_t>::max(), std::numeric_limits<xtime_nsec_t>::max());
}

const time::this_type time::min_time(void)
{
	return this_type(0, 1);
}

} //namespace time
} //namespace yggr

//#ifndef YGGR_NO_CXX11_RVALUE_REFERENCES

#define _YGGR_TMP_PP_TIME_SWAP_IMP() \
	void swap(yggr::time::time::base_type& left, yggr::time::time& right) { \
		right.swap(left); } \
	\
	void swap(yggr::time::time& left, yggr::time::time::base_type& right) { \
		left.swap(right); } \
	\
	void swap(yggr::time::time& left, yggr::time::time& right) { \
		left.swap(right); }
//
//#else
//
//#define _YGGR_TMP_PP_TIME_SWAP_IMP() \
//	void swap(yggr::time::time& left, yggr::time::time& right) { \
//		left.swap(right); }
//
//#endif // #ifndef YGGR_NO_CXX11_RVALUE_REFERENCES

namespace std
{
	_YGGR_TMP_PP_TIME_SWAP_IMP();
} // namespace std

namespace boost
{
	_YGGR_TMP_PP_TIME_SWAP_IMP();
} // namespace boost

#undef _YGGR_TMP_PP_TIME_SWAP_IMP

