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


// dtl_posix_time is used to provide a DTL-compatible type of
// boost's boost::posix_date_time::ptime class.
// You can bind it using the standard <<, >>, == DTL binders.
//
// Its modelled from dtl_set<> and dtl_enum<>
//
// I've been using internally for a while now, its time to release it
// for wider consumption.
//
// TODO: There are currently no test cases for this, I ensure it works
// with my project and that's it.
//
// Initial: 26/03/2005 - PH

#ifndef DTL_DTL_POSIX_TIME_H
#define DTL_DTL_POSIX_TIME_H

// You must have boost to use this!
#ifdef DTL_HAVE_BOOST

// syntax error in older compiler: gcc 3.0
// I had to hack boost/date_time/compiler_config.hpp to ensure
// it didn't define BOOST_DATE_TIME_HAS_GETTIMEOFDAY_HIGH_PRECISION_CLOCK

#include <boost/version.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "BoundIO.h"

BEGIN_DTL_NAMESPACE


struct dtl_posix_time;
BoundIO &dtl_posix_time_bind(BoundIO & b, dtl_posix_time & x);


struct dtl_posix_time : boost::posix_time::ptime
{
   // Bare constructor, to support dtl::nullable<>
   // TODO remove this requirement!
   // Causes compiler ICEs in mingw gcc 3.4,
   // and isn't available in boost 1.30
#if BOOST_VERSION > 103000 && !(defined(DTL_MINGW32) && defined(__GNUC__) && (__GNUC__ == 3) && (__GNUC_MINOR__ == 4))
   // defaults the time to not_a_date_time
   dtl_posix_time() {}
#else
   dtl_posix_time() : boost::posix_time::ptime(boost::gregorian::date(1970,1,1),time_duration_type(0,0,0)) {}
#endif

   // copied from boost/date_time/posix_time/ptime.hpp

   //! Construct with date and offset in day
   dtl_posix_time(boost::gregorian::date d,time_duration_type td) : boost::posix_time::ptime(d,td)
   {}
   //! Construct a time at start of the given day (midnight)
   explicit dtl_posix_time(boost::gregorian::date d) : boost::posix_time::ptime(d,time_duration_type(0,0,0))
   {}
   //! Copy from time_rep
   dtl_posix_time(const time_rep_type& rhs):
   boost::posix_time::ptime(rhs)
   {}

   // Not available in older versions of boost,
   // Causes compiler ICEs in mingw gcc 3.4
#if BOOST_VERSION > 103000 && !(defined(DTL_MINGW32) && defined(__GNUC__) && (__GNUC__ == 3) && (__GNUC_MINOR__ == 4))
   //! Construct from special value
   dtl_posix_time(const boost::posix_time::special_values sv) : boost::posix_time::ptime(sv)
   {}
#endif

   // this causes confusion in older GCC compilers, not sure about other compilers?
#if !(defined(__GNUC__) && (__GNUC__ == 3) && (__GNUC_MINOR__ == 0))
   //! Copy from ptime
   dtl_posix_time(const boost::posix_time::ptime & rhs)
      : boost::posix_time::ptime(rhs)
   {}
#endif


   friend BoundIO operator== (BoundIO & b, dtl_posix_time & x) { return dtl_posix_time_bind(b,x) == x.sql_value; }
   friend BoundIO operator>> (BoundIO & b, dtl_posix_time & x) { return dtl_posix_time_bind(b,x) >> x.sql_value; }
   friend BoundIO operator<< (BoundIO & b, dtl_posix_time & x) { return dtl_posix_time_bind(b,x) << x.sql_value; }


private:
   // to keep things private, we need to make some friends.
   // this is intrusive.
   friend struct dtl_posix_time_validate;

   timestamp_t sql_value;
};


struct dtl_posix_time_validate
{

	static bool sel_validate(BoundIOs &b, BoundIOs::UserValidate *val)
   {
	  BYTE *offset = val->src_class;
	  offset += reinterpret_cast<size_t>(b.GetWorkingAddr());
	  dtl_posix_time *px = reinterpret_cast<dtl_posix_time *>(offset);

      timestamp_t t = px->sql_value;
      // if its invalid on purpose, then set it to a NULL date-time
      if (t.year == 0)
         *px = dtl_posix_time();
      else
      {
         try
         {
            *px = dtl_posix_time(
                  boost::gregorian::date(t.year,t.month,t.day),
                  boost::posix_time::hours(t.hour)
                     + boost::posix_time::minutes(t.minute)
                     + boost::posix_time::seconds(t.second));
         }
         catch (std::out_of_range &)
         {
            // TODO chances are, this is a NULL date.
            // Change it so this isn't called for NULL dates.
            *px = dtl_posix_time();
         }
      }
	  px->sql_value = t; // restore backed-up sql value
      return true;
   }


	static bool ins_validate(BoundIOs &b, BoundIOs::UserValidate *val)
   {
	  BYTE *offset = val->src_class;
	  offset += reinterpret_cast<size_t>(b.GetWorkingAddr());
	  dtl_posix_time *px = reinterpret_cast<dtl_posix_time *>(offset);
      timestamp_t & ts = px->sql_value;
      const boost::gregorian::date d = px->date();
      // only bother to set it if we have a valid date
      if ( !d.is_infinity() && !d.is_not_a_date() )
      {
         const boost::posix_time::time_duration t = px->time_of_day();
         ts.year = d.year(); ts.month = d.month(); ts.day = d.day();
         ts.hour = t.hours(); ts.minute = t.minutes(); ts.second = t.seconds();
      }
      else
      {
         ts.year = ts.month = ts.day = ts.hour = ts.minute = ts.second = 0;
      }
      // always return true, as the date might be invalid if its
      // inside a NULLABLE.  TODO make this more strict.
      return true;
   }
};


inline BoundIO &dtl_posix_time_bind(BoundIO & b, dtl_posix_time & x)
{
  STD_::vector<tstring> cols;
  cols.push_back(b.GetName());
  std::ptrdiff_t offset = reinterpret_cast<BYTE*>(&x) - b.GetBoundIOsPtr()->GetWorkingAddr();
  BoundIOs::UserValidate uv(reinterpret_cast<BYTE*>(offset), dtl_posix_time_validate::ins_validate,
	  dtl_posix_time_validate::sel_validate, cols);
  b.GetBoundIOsPtr()->AddUserValidate(uv);

  return b;
}


END_DTL_NAMESPACE

#endif
#endif
