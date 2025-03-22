/* Copyright ?2005
 * Paul Harris <paulharris@computer.org>
 *
 * Permission to use, copy, modify, distribute and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appears in all copies and that both that
 * copyright notice and this permission notice appear in supporting documentation.
 * Paul Harris makes no representations about the suitability
 * of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

// Based on dtl_time<>
// Utilises boost::posix_time::time_duration
//
// Note, this uses ODBC's SQL_TIME_STRUCT, which does not support
// fractional time units.  Only 'SQL_DAY_SECOND' (unsupported) and
// 'SQL_TIMESTAMP' (aka dtl_posix_time) supports fractional time units.

// Initial: 16/05/2006 - PH

#ifndef DTL_DTL_TIME_H
#define DTL_DTL_TIME_H

// You must have boost to use this!
#ifdef DTL_HAVE_BOOST

// syntax error in older compiler: gcc 3.0
// I had to hack boost/date_time/compiler_config.hpp to ensure
// it didn't define BOOST_DATE_TIME_HAS_GETTIMEOFDAY_HIGH_PRECISION_CLOCK

#include <boost/version.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <sqltypes.h>
#include "BoundIO.h"

BEGIN_DTL_NAMESPACE


struct dtl_time;
BoundIO &dtl_time_bind(BoundIO & b, dtl_time & x);


struct dtl_time : boost::posix_time::time_duration
{
   typedef boost::posix_time::time_duration Parent;

   // Bare constructor, to support dtl::nullable<>
   // TODO remove this requirement!
   dtl_time() {}

   // copied from boost/date_time/posix_time/posix_time_config.hpp

   dtl_time(Parent::hour_type hour,
         Parent::min_type min,
         Parent::sec_type sec,
         Parent::fractional_seconds_type fs=0) :
      Parent(hour,min,sec,fs)
   {}

#if BOOST_VERSION > 103000
   // Not available in older versions of boost (?)
   //! Construct from special value
   dtl_time(const boost::date_time::special_values sv) : Parent(sv) {}
#endif



   friend BoundIO operator== (BoundIO & b, dtl_time & x) { return dtl_time_bind(b,x) == x.sql_value; }
   friend BoundIO operator>> (BoundIO & b, dtl_time & x) { return dtl_time_bind(b,x) >> x.sql_value; }
   friend BoundIO operator<< (BoundIO & b, dtl_time & x) { return dtl_time_bind(b,x) << x.sql_value; }


private:
   // to keep things private, we need to make some friends.
   // this is intrusive.
   friend struct dtl_time_validate;

   SQL_TIME_STRUCT sql_value;
};


struct dtl_time_validate
{

	static bool sel_validate(BoundIOs &b, BoundIOs::UserValidate *val)
   {
	  BYTE *offset = val->src_class;
	  offset += reinterpret_cast<size_t>(b.GetWorkingAddr());
	  dtl_time *px = reinterpret_cast<dtl_time *>(offset);

      SQL_TIME_STRUCT t = px->sql_value;
      try
      {
         *px = dtl_time(t.hour,t.minute,t.second);
      }
      catch (std::out_of_range &)
      {
         // TODO chances are, this is a NULL date.
         // Change it so this isn't called for NULL dates.
#if BOOST_VERSION > 103000
         *px = dtl_time(boost::posix_time::not_a_date_time);
#else
         // see above for comments about this 0,0,0 time.
         *px = dtl_time(0,0,0);
#endif
      }
	  px->sql_value = t; // restore backed-up sql value
      return true;
   }


	static bool ins_validate(BoundIOs &b, BoundIOs::UserValidate *val)
   {
	  BYTE *offset = val->src_class;
	  offset += reinterpret_cast<size_t>(b.GetWorkingAddr());
	  dtl_time *px = reinterpret_cast<dtl_time *>(offset);
      SQL_TIME_STRUCT & ts = px->sql_value;
      ts.hour = px->hours(); ts.minute = px->minutes(); ts.second = px->seconds();
      // always return true, as the date might be invalid if its
      // inside a NULLABLE.  TODO make this more strict.
      return true;
   }
};


inline BoundIO &dtl_time_bind(BoundIO & b, dtl_time & x)
{
  STD_::vector<tstring> cols;
  cols.push_back(b.GetName());
  std::ptrdiff_t offset = reinterpret_cast<BYTE*>(&x) - b.GetBoundIOsPtr()->GetWorkingAddr();
  BoundIOs::UserValidate uv(reinterpret_cast<BYTE*>(offset), dtl_time_validate::ins_validate, dtl_time_validate::sel_validate, cols);
  b.GetBoundIOsPtr()->AddUserValidate(uv);

  return b;
}


END_DTL_NAMESPACE

#endif
#endif
