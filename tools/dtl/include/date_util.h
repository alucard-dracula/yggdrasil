/* Copyright ?2000 
Michael Gradman and Corwin Joy 

Permission to use, copy, modify, distribute and sell this software and 
its documentation for any purpose is hereby granted without fee, provided 
that the above copyright notice appears in all copies and that both that 
copyright notice and this permission notice appear in supporting documentation. 
Corwin Joy and Michael Gradman make no representations about the suitability 
of this software for any purpose. 
It is provided "as is" without express or implied warranty.
*/ 
// provides comparison operators for dates
// Initial: 10/9/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 12/19/2000 - MG - added namespaces
// Expanded: 2/20/2001 - Added date math library

#ifndef DTL_DATE_UTIL_H
#define DTL_DATE_UTIL_H

#include "dtl_base_types.h"
#include "minimax.h"
#include "RootException.h"

#include "std_warn_off.h"

#ifdef DTL_NATIVE_WIN32
	#ifdef DTL_USE_MFC
		#include <afx.h>
	#else
		#include <windows.h>
	#endif
#endif

#include <sql.h>
#include <sqlext.h>

#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

int TSCmp(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2);
int DCmp(const DATE_STRUCT &ts1, const DATE_STRUCT &ts2);
int TCmp(const TIME_STRUCT &ts1, const TIME_STRUCT &ts2);

bool operator<(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2);

bool operator==(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2);

bool operator!=(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2);

bool operator>(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2);

bool operator>=(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2);

bool operator<=(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2);

STD_::ostream &operator<<(STD_::ostream &o, const TIMESTAMP_STRUCT &ts);

#ifndef DTL_NO_UNICODE
// added for wstring support
STD_::wostream &operator<<(STD_::wostream &o, const TIMESTAMP_STRUCT &ts);
#endif

typedef TIMESTAMP_STRUCT timestamp_t;

// returns if the year passed in is a leap year
bool is_leap_year(int year);

/* Structure to hold time information */
/* Dates must lie in the range January 1, 1970, to Jan 1, 4000. */
struct jtime_tm
{
  int   sec;		// Seconds after the minute (0, 61)
  int   min;		// minutes after the hour (0, 59)
  int   hour;	// hours since midnight (0, 23)
  int   day;	// day of the month
  int   mon;		// month number (1, 12)
  int   year;	// year number (AD)

  // returns whether this is a valid date
  bool is_valid() const;
};


RETCODE Timestamp2Jtime(const timestamp_t *tst, jtime_t *jtime );
RETCODE Jtime2Timestamp(jtime_t jtime, timestamp_t *tst);
RETCODE jultime(const jtime_tm *time, jtime_t *jtime);
jtime_t Now(void);
jtime_t Today(void);
RETCODE julday(int day, int month, int year, jtime_t *jtime);
RETCODE unjultime(jtime_t jtime, jtime_tm *time);
RETCODE unjulday(jtime_t jtime, int *dd, int *mm, int *iyyy);
RETCODE addtime(jtime_t oldtime, jtime_tm add, jtime_t *newtime);
jtime_t addmonths(jtime_t oldtime, int months);


jtime_t addsecs(jtime_t oldtime, int secs);
tstring jul2str(jtime_t juldate);
RETCODE str2jultm(const TCHAR *szDt, jtime_t *jtime);
tstring jul2strtm(jtime_t jtime);
void jul2SQLtm(jtime_t jultime, TCHAR *szSQLtm);
double diffdays(jtime_t end, jtime_t begin);
double diffhours(jtime_t end, jtime_t begin);
double diffsecs(jtime_t end, jtime_t begin);
jtime_t eomonth(jtime_t oldtime);
jtime_t eoweek(jtime_t oldtime);
jtime_t eoday(jtime_t oldtime);
jtime_t beginmonth(jtime_t oldtime);
jtime_t beginweek(jtime_t oldtime);
jtime_t beginday(jtime_t oldtime);
int month_days(int year, int month) ;
int year(jtime_t oldtime);
int month(jtime_t oldtime);
int monthweek(jtime_t now);
jtime_t maxtime(jtime_t a, jtime_t b);
jtime_t mintime(jtime_t a, jtime_t b);
RETCODE XL2jtime(double xltime, jtime_t *jtime);
RETCODE jtime2XL(jtime_t jtime, double *xltime);

void date_unit_test(void) ;

#define SECONDS_PER_DAY 86400
#define SECONDS_PER_HOUR 3600
#define SECONDS_PER_HALFHOUR 1800

/* The next set of constants is used for debugging in order to trap
   date over or underflow errors. These bounds are calculated in a
   rather rough manner, but the idea behind them is to set off
   warning bells for _TEXT("large") or _TEXT("small") dates.  These should
   not be used in any calculations since they are only approximate.
*/
#define MIN_YEAR 1970

// Do not define MAX_YEAR larger than 2100.  Otherwise this may overflow 
// MAX_JTIME since unsigned long is not guaranteed to be larger than 4294967295
#define MAX_YEAR 2090 

/* MAX_JTIME and MIN_JTIME may be used by outside routines
   in order to define endpoints for acceptable dates.  These
   constants are often useful for searching algorithms etc.
*/

// ULONG_MAX      4294967295
#define MAX_JTIME 3786912000UL
//#define MAX_JTIME static_cast<jtime_t>(((MAX_YEAR - MIN_YEAR) * 365.25 *SECONDS_PER_DAY))

#define MIN_JTIME 0

/*------------------------------------------------------------------------*/
// This routine takes the time stored in oldtime and adds to it the
// specified number of days to obtain
// a new time which is returned
/*------------------------------------------------------------------------*/
jtime_t adddays(jtime_t oldtime, int days);

/*------------------------------------------------------------------------*/
// This routine takes the time stored in oldtime and adds to it the
// specified number of days to obtain
// a new time which is returned
/*------------------------------------------------------------------------*/
jtime_t addhours(jtime_t oldtime, int hours);

/*------------------------------------------------------------------------*/
// Given a particular day return the time corresponding to
// the end of that day
/*------------------------------------------------------------------------*/
jtime_t eoday(jtime_t oldtime);


/*------------------------------------------------------------------------*/
// Given a particular day return the time corresponding to
// the beginning of that day
/*------------------------------------------------------------------------*/
jtime_t beginday(jtime_t oldtime);


/*------------------------------------------------------------------------*/
// Given a particular date return the weekday corresponding
// to that date. 0 = Sunday, 1 = Monday etc.
/*------------------------------------------------------------------------*/
int weekday(jtime_t oldtime);

/*------------------------------------------------------------------------*/
// SameDay: Tests to see if two times refer to the same day
// Returns TRUE if both times are on the same day, false otherwise
/*------------------------------------------------------------------------*/
short SameDay(jtime_t jultime1, jtime_t jultime2);

/*------------------------------------------------------------------------*/
// Class to form lightweight wrapper around jtime_t
// Note the cast operators below which allow the class to be passed in to
// the jtime_t math functions above
/*------------------------------------------------------------------------*/
class jtime_c {
private:
	jtime_t jtime;
public:
	jtime_c(const jtime_t &t);

	jtime_c(const timestamp_t &ts);

	jtime_c(const tstring &s)
	{
		RETCODE rc;
		jtime_t t;

		rc = str2jultm(s.c_str(), &t);

		if (!RC_SUCCESS(rc))
		{
			tostringstream errStream;
			errStream << _TEXT("Invalid date ") << s
					  << _TEXT(" passed to jtime_c constructor!");

			DTL_THROW RootException(_TEXT("jtime_c::jtime_c(const tstring &)"),
									errStream.str()); 
		}

		jtime = t;
	}

	jtime_c();

	operator jtime_t() const;

	operator timestamp_t() const;

	operator STD_::string() const;

#if !defined(DTL_NO_UNICODE)
	operator STD_::wstring() const;
#endif

	jtime_c &operator=(const jtime_c &other);

	jtime_c &operator=(const jtime_t &t);

	jtime_c &operator=(const timestamp_t &ts);

	friend bool operator<(const jtime_c &lhs, const jtime_c &rhs);
	friend bool operator==(const jtime_c &lhs, const jtime_c &rhs);
	friend bool operator!=(const jtime_c &lhs, const jtime_c &rhs);
	friend bool operator>(const jtime_c &lhs, const jtime_c &rhs);
	friend bool operator>=(const jtime_c &lhs, const jtime_c &rhs);
	friend bool operator<=(const jtime_c &lhs, const jtime_c &rhs);


	friend STD_::ostream &operator<<(STD_::ostream &o, const jtime_c &ts);

	friend STD_::istream &operator>>(STD_::istream &i, jtime_c &ts);

#if !defined(DTL_NO_UNICODE )
	// added for wstring support
	friend STD_::wostream &operator<<(STD_::wostream &o, const jtime_c &ts);

	friend STD_::wistream &operator>>(STD_::wistream &i, jtime_c &ts);
#endif
};

bool operator<(const jtime_c &lhs, const jtime_c &rhs);
bool operator==(const jtime_c &lhs, const jtime_c &rhs);
bool operator!=(const jtime_c &lhs, const jtime_c &rhs);
bool operator>(const jtime_c &lhs, const jtime_c &rhs);
bool operator>=(const jtime_c &lhs, const jtime_c &rhs);
bool operator<=(const jtime_c &lhs, const jtime_c &rhs);

STD_::ostream &operator<<(STD_::ostream &o, const jtime_c &ts);
#if !defined(DTL_NO_UNICODE)
STD_::wostream &operator<<(STD_::wostream &o, const jtime_c &ts);
#endif

END_DTL_NAMESPACE

#endif
