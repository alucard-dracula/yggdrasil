/* Copyright © 2005
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

// Based on dtl_posix_time<>
// Utilises boost::gregorian::date

// Initial: 16/05/2006 - PH

#ifndef DTL_DTL_DATE_H
#define DTL_DTL_DATE_H

// You must have boost to use this!
#ifdef DTL_HAVE_BOOST

#include <boost/version.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "BoundIO.h"

BEGIN_DTL_NAMESPACE


struct dtl_date;
BoundIO &dtl_date_bind(BoundIO & b, dtl_date & x);


struct dtl_date : boost::gregorian::date
{
   typedef boost::gregorian::date Date;
   // Bare constructor, to support dtl::nullable<>
   // TODO remove this requirement!
#if BOOST_VERSION > 103000
    //! Default constructor constructs with not_a_date_time
   dtl_date() {}
#else
   dtl_date() : Date(1970,1,1) {}
#endif

   // From parent date class
   //! Main constructor with year, month, day
   dtl_date(Date::year_type y, Date::month_type m, Date::day_type d) : Date(y,m,d) {}

   //! Constructor from a ymd_type structure
   explicit dtl_date(Date::ymd_type const& ymd) : Date(ymd) {}
   //! Needed copy constructor
   explicit dtl_date(const Date::date_int_type& rhs) : Date(rhs) {}
   //! Needed copy constructor
   explicit dtl_date(Date::date_rep_type rhs) : Date(rhs) {}
   //! Constructor for infinities, not a date, max and min date
   explicit dtl_date(boost::date_time::special_values sv) : Date(sv) {}

   // this causes confusion in older GCC compilers, not sure about other compilers?
#if !(defined(__GNUC__) && (__GNUC__ == 3) && (__GNUC_MINOR__ == 0))
   dtl_date(Date const& d) : Date(d) {}
#endif


   friend BoundIO operator== (BoundIO & b, dtl_date & x) { return dtl_date_bind(b,x) == x.sql_value; }
   friend BoundIO operator>> (BoundIO & b, dtl_date & x) { return dtl_date_bind(b,x) >> x.sql_value; }
   friend BoundIO operator<< (BoundIO & b, dtl_date & x) { return dtl_date_bind(b,x) << x.sql_value; }


private:
   // to keep things private, we need to make some friends.
   // this is intrusive.
   friend struct dtl_date_validate;

   SQL_DATE_STRUCT sql_value;
};


struct dtl_date_validate
{

	static bool sel_validate(BoundIOs &b, BoundIOs::UserValidate *val)
   {
	  BYTE *offset = val->src_class;
	  offset += reinterpret_cast<size_t>(b.GetWorkingAddr());
	  dtl_date *px = reinterpret_cast<dtl_date *>(offset);

      SQL_DATE_STRUCT t = px->sql_value;
      // if its invalid on purpose, then set it to not_a_date_time
      if (t.year == 0)
#if BOOST_VERSION > 103000
         *px = dtl_date(boost::posix_time::not_a_date_time);
#else
         // old boost did not support 'not_a_date_time'
         // this is bad, since if the database supports invalid dates, then so should
         // the DTL.
         *px = dtl_date(1970,1,1);   // dummy date
         // we should throw an exception here, or something,
         // but I don't care, this is really just for testing,
         // not production!  Use a newer boost!!
#endif
      else
      {
         try
         {
            *px = dtl_date(t.year,t.month,t.day);
         }
         catch (std::out_of_range &)
         {
            // TODO chances are, this is a NULL date.
            // Change it so this isn't called for NULL dates.
#if BOOST_VERSION > 103000
            *px = dtl_date(boost::posix_time::not_a_date_time);
#else
            // see above for comments about this 1970
            *px = dtl_date(1970,1,1);   // dummy date
#endif
         }
      }
	  px->sql_value = t; // restore backed-up sql value
      return true;
   }


	static bool ins_validate(BoundIOs & b, BoundIOs::UserValidate *val)
   {
	  BYTE *offset = val->src_class;
	  offset += reinterpret_cast<size_t>(b.GetWorkingAddr());
	  dtl_date *px = reinterpret_cast<dtl_date *>(offset);
      SQL_DATE_STRUCT & ts = px->sql_value;
      // only bother to set it if we have a valid date
      if ( !px->is_infinity() && !px->is_not_a_date() )
      {
         ts.year = px->year(); ts.month = px->month(); ts.day = px->day();
      }
      else
      {
         ts.year = ts.month = ts.day = 0;
      }
      // always return true, as the date might be invalid if its
      // inside a NULLABLE.  TODO make this more strict.
      return true;
   }
};


inline BoundIO &dtl_date_bind(BoundIO & b, dtl_date & x)
{
  STD_::vector<tstring> cols;
  cols.push_back(b.GetName());
  std::ptrdiff_t offset = reinterpret_cast<BYTE*>(&x) - b.GetBoundIOsPtr()->GetWorkingAddr();
  BoundIOs::UserValidate uv(reinterpret_cast<BYTE*>(offset), dtl_date_validate::ins_validate, dtl_date_validate::sel_validate, cols);
  b.GetBoundIOsPtr()->AddUserValidate(uv);

  return b;
}


END_DTL_NAMESPACE

#endif
#endif
