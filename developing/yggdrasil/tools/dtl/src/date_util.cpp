/* Copyright 2000 
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

#include "date_util.h"
#include "string_util.h"
#include "clib_fwd.h"

#include "std_warn_off.h"

#include <cstdlib>

#ifdef __BORLANDC__
#include <time.h>
#include <math.h>
#else
#include <ctime>
#include <cmath>
#endif

#include <cassert>
#include <sstream>

#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

USING_STD_NAMESPACE

// returns if the year passed in is a leap year
bool is_leap_year(int year)
{
	// years not divisible by 4 are not leap years
	if (year % 4 != 0)
	{	
		return false;
	}

	// all years reaching here are divisble by 4

	// all years divisible by 4, but not by 100 are leap years
	if (year % 100 != 0)
	{
		return true;
	}

	// all years reaching here are divisible by 100

	// all years divisible by 400 are leap years
	if (year % 400 == 0)
	{
		return true;
	}

	// all years that reach this point here are divisible by 100, but not 400
	// which are NOT leap years
	return false;
}

// returns whether this date is valid
bool jtime_tm::is_valid() const
{
	  // numbers of days in each month
	  static const int days_in_month[12] =
	  {
		 31, // JAN
		 28, // FEB (29 in leap years)
		 31, // MAR
		 30, // APR
		 31, // MAY
		 30, // JUNE
		 31, // JUL
		 31, // AUG
		 30, // SEPT
		 31, // OCT
		 30, // NOV
		 31  // DEC
	  };

	  if (mon < 1 || mon > 12)
	  {
	  	    // month must be in [1, 12]
			return false;
	  }
	  else
	  {
			// must be a valid day of the month
		    int days_in_this_month = days_in_month[mon - 1]; 
													   // array zero indexed so must
													   // subtract 1 from month #

		    // if date is in February, we must account for if this is a leap year
			if (mon == 2)
			{
				if (is_leap_year(year))
				{
					++days_in_this_month;
				}
			}


			if (day < 1 || day > days_in_this_month)
			{
				// day of month must be in [1, days_in_this_month]
				return false;
			}
			else if (hour < 0 || hour > 23)
			{
				// hours must be in [0, 23]
				return false;
			}
			else if (min < 0 || min > 59)
			{
				// minutes must be in [0, 59]
				return false;
			}
			else if (sec < 0 || sec > 59)
			{
				// seconds must be in [0, 59]
				return false;
			}

	  }

	  return true;
}

int TSCmp(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2) {

	if (ts1.year < ts2.year)
		return -1;
	if (ts1.year > ts2.year)
		return 1;

	if (ts1.month < ts2.month)
		return -1;


	if (ts1.month > ts2.month)
		return 1;

	if (ts1.day < ts2.day)
		return -1;

	if (ts1.day > ts2.day)
		return 1;

	if (ts1.hour < ts2.hour)
		return -1;

	if (ts1.hour > ts2.hour)
		return 1;

	if (ts1.minute < ts2.minute)
		return -1;

	if (ts1.minute > ts2.minute)
		return 1;

	if (ts1.second < ts2.second)
		return -1;

	if (ts1.second > ts2.second)
		return 1;

	return ts1.fraction - ts2.fraction;

}


int DCmp(const DATE_STRUCT &d1, const DATE_STRUCT &d2) 
{
	if (d1.year < d2.year) return -1;
	if (d1.year > d2.year) return 1;
	if (d1.month < d2.month) return -1;
	if (d1.month > d2.month) return 1;
	if (d1.day < d2.day) return -1;
	if (d1.day > d2.day) return 1;
   return 0;
}

int TCmp(const TIME_STRUCT &t1, const TIME_STRUCT &t2) 
{
	if (t1.hour < t2.hour) return -1;
	if (t1.hour > t2.hour) return 1;
	if (t1.minute < t2.minute) return -1;
	if (t1.minute > t2.minute) return 1;
	return t1.second - t2.second;
}

bool operator<(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2)
{
	if (TSCmp(ts1, ts2) < 0)
		return true;
	return false;
}


bool operator==(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2)
{
	return (ts1.year     == ts2.year    ) && (ts1.month  == ts2.month ) &&
		   (ts1.day      == ts2.day     ) && (ts1.hour   == ts2.hour  ) &&
		   (ts1.minute   == ts2.minute  ) && (ts1.second == ts2.second) &&
		   (ts1.fraction == ts2.fraction);
}


bool operator!=(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2)
{
	return !(ts1 == ts2);

}

bool operator>(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2)
{
	return (ts2 < ts1);
}

bool operator<=(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2)
{
    return (ts2 < ts1) || (ts1 == ts2);
}

bool operator>=(const TIMESTAMP_STRUCT &ts1, const TIMESTAMP_STRUCT &ts2)
{
	return (ts1 < ts2) || (ts1 == ts2);
}

STD_::ostream &operator<<(STD_::ostream &o, const TIMESTAMP_STRUCT &ts)
{
	static char mm_name[13][4] = {"", "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
				"JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
	
	o << mm_name[ts.month] << "-";
	o.width(2);
	o.fill('0');
	o << ts.day;
	o << "-";
	o.width(4);
	o.fill('0');
	o << ts.year;
	if (!(ts.hour == 0 && ts.minute == 0 && ts.second == 0) ){
		o << " ";
		o.width(2);
		o.fill('0');
		o << ts.hour;
		o << ":";
		o.width(2);
		o.fill('0');
		o << ts.minute;
		o << ":";
		o.width(2);
		o.fill('0');
		o << ts.second;
	}
	
	return o;
}

#if !defined(DTL_NO_UNICODE)
STD_::wostream &operator<<(STD_::wostream &o, const TIMESTAMP_STRUCT &ts)
{
	static wchar_t mm_name[13][4] = {L"", L"JAN", L"FEB", L"MAR", L"APR", L"MAY", L"JUN",
				L"JUL", L"AUG", L"SEP", L"OCT", L"NOV", L"DEC"};
	
	o << mm_name[ts.month] << L"-";
	o.width(2);
	o.fill(L'0');

	o << ts.day;
	o << L"-";
	o.width(4);
	o.fill(L'0');
	o << ts.year;
	if (!(ts.hour == 0 && ts.minute == 0 && ts.second == 0) ){
		o << L" ";
		o.width(2);
		o.fill(L'0');
		ts.hour;
		o << L":";
		o.width(2);
		o.fill(L'0');
		ts.minute;
		o << L":";
		o.width(2);
		o.fill(L'0');
		ts.second;
	}
	
	return o;
}

#endif

/* By default all times are stored relative to GMT */
	typedef struct tTZ {
	TCHAR	szZone[4];   /*  Name of the timezone */
	long int		offset;		 /*  Offset from internal standard time	 */
} TZ_t;

	
/* We will compute times as on offset from midnight Jan 1, 1970 */
/* The constant below is the Julian day number for Jan 1, 1970 */
#define JULDAY_JAN_1_1970 135140
#define XL_JULDAY_JAN_1_1970 25569


#define MAX_JDATE (JULDAY_JAN_1_1970 + (MAX_YEAR - MIN_YEAR) * 365.25)
#define MIN_JDATE JULDAY_JAN_1_1970



/*------------------------------------------------------------------------*
 * This routine computes the Julian Day Number that begins at *midnight* of the
 * calendar date specified by month, day, and year.
 * The calculated date is returned in juldate.  The routine returns
 * SQL_SUCCESS if the calculation was successful and SQL_ERROR otherwise
 * Routine is for Gregorian dates only after 1600
 *------------------------------------------------------------------------*/

//////////////////////////////////////////////////////////////

const int	DaysSoFar[][13] =
			{
			{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
			{0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}
			};


static RETCODE nr_julday(int month, int day, int year, long *juldate)
{
	int leap;
	leap = ( year%4 == 0 && year%100 !=0 ) || year%400 == 0;

	year -= 1601;
	int c = year/100;
	*juldate = year*365 + (year/4) - c + c/4 + DaysSoFar[leap][month-1] + day + 365;
	if (year < 0)
	  (*juldate)--; // Year 1600 is a leap year
	return(SQL_SUCCESS);

}


/*------------------------------------------------------------------------*
 * Inverse of the function nr_julday given above.  Here julian is input
 * as a Julian Day Number, and the routine outputs 
 * the month day and year on which the specified Julian Day
 * started at midnight.
 *------------------------------------------------------------------------*/
static RETCODE nr_unjulday(long julian, int *month, int *day, int *year)
{
	const long YEAR = 365;
	const long FOUR_YEARS = 1461;
	const long CENTURY = 36524L;
	const long FOUR_CENTURIES = 146097L;

	long y;

	if (julian > 365) {
		julian -= 366; // 1600 was a leap year

		// use modulo arithmetic to extract the number of years
		ldiv_t result;

		result = ldiv(julian, FOUR_CENTURIES);
		long c4 = result.quot;
		julian = result.rem;

		result = ldiv(julian, CENTURY);
		if (result.quot == 4) { // century overflow due to current leap year, correct
			result.quot--;
			result.rem += CENTURY;
		}
		long c = result.quot;
		julian = result.rem;

		result = ldiv(julian, FOUR_YEARS);
		long y4 = result.quot;
		julian = result.rem;

		result = ldiv(julian, YEAR);
		if (result.quot == 4) { // year overflow due to current leap year, correct
			result.quot--;
			result.rem += YEAR;
		}
		y = result.quot;
		y += c4*400 + c*100 + y4*4;
		y += 1601;

		julian = result.rem + 1;    // Add back one to get actual day number
		
	} else {
	    y = 1600;
		julian++;
	}

	*year = y;
	int leap = ( y%4 == 0 && y%100 !=0 ) || y%400 == 0;

	short m = (short) (julian/30);		// Guess at month
	while (DaysSoFar[leap][m] >= julian)
		m--;					// Correct guess.

	*day = julian - DaysSoFar[leap][m];
	*month = m+1;
	return(SQL_SUCCESS);
}

#if 0

int test_julian() {
	long julian, jcheck;
	int m, d,y;
	int last_m, last_d, last_y;
	//nr_julday(int m, int d, int y, long *juldate)
	nr_julday(1, 1, 1970, &jcheck);
	nr_unjulday(jcheck, &m, &d, &y);
		
	nr_julday(1, 1, 1604, &julian);
	nr_unjulday(julian, &m, &d, &y);

	nr_julday(12, 30, 1604, &julian);
	nr_unjulday(julian, &m, &d, &y);

	nr_julday(1, 1, 1600, &julian);
	nr_unjulday(julian, &m, &d, &y);

	last_d = d;
	last_m = m;
	last_y = y;
	for (long i=1; i<5000; i++) {
		nr_unjulday(julian+i, &m, &d, &y);
		nr_julday(m, d, y, &jcheck);
		//tcout << d << _TEXT(".") << m << _TEXT(".") << y << _TEXT(", ");
		if( jcheck != (julian + i)) 
		  tcout << _TEXT("bad inverse: ") << d << _TEXT(".") << m << _TEXT(".") << y << _TEXT(", ") << STD_::endl;
		if( m == last_m && d == last_d && y == last_y) 
		  tcout << _TEXT("double date: ") << d << _TEXT(".") << m << _TEXT(".") << y << _TEXT(", ") << STD_::endl;
		if( d != last_d + 1)
			if (m != (last_m + 1) && !(last_m == 12 && y == (last_y + 1)) )
				tcout << _TEXT("skipped date: ") << d << _TEXT(".") << m << _TEXT(".") << y << _TEXT(", ") << STD_::endl;
		last_d = d;
		last_m = m;
		last_y = y;
	}
	tcout << STD_::endl;
	return 0;

}

#endif

/*------------------------------------------------------------------------*
 * This routine converts an ODBC timestamp structure to a jtime
 *------------------------------------------------------------------------*/
RETCODE Timestamp2Jtime(const timestamp_t *tst, jtime_t *jtime ){
	jtime_tm tm_time;

	tm_time.year = tst->year;
	tm_time.mon = tst->month;
	tm_time.day = tst->day;
	tm_time.hour = tst->hour;
	tm_time.min = tst->minute;
	tm_time.sec = tst->second;

	return jultime(&tm_time, jtime);
}

/*------------------------------------------------------------------------*
 * This routine converts a jtime to an ODBC timestamp structure 
 *------------------------------------------------------------------------*/
RETCODE Jtime2Timestamp(jtime_t jtime, timestamp_t *tst){
	jtime_tm tm_time;
	RETCODE rc;

	rc = unjultime(jtime, &tm_time);

	if (!RC_SUCCESS(rc))
		return rc;

	tst->year = (short) tm_time.year;
	tst->month = (unsigned short) tm_time.mon;
	tst->day = (unsigned short) tm_time.day;
	tst->hour = (unsigned short) tm_time.hour;
	tst->minute = (unsigned short) tm_time.min;
	tst->second = (unsigned short) tm_time.sec;
	tst->fraction = 0;
	return (SQL_SUCCESS);
}


/*------------------------------------------------------------------------*
 * This routine computes the Julian Time of the
 * calendar date specified by the structure time
 * sec		seconds after the minute (0, 61)
 * min		minutes after the hour (0, 59)
 * hour		hours since midnight (0, 23)
 * month	(1, 12)
 * day		day of the month (1, 31)
 * year		AD year number
 *
 * Dates must lie in the range January 1, 1970, to midnight, February 5, 2036.
 * The calculated date is returned in jtime.  The routine returns
 * SQL_SUCCESS if the calculation was successful and SQL_ERROR otherwise
 *------------------------------------------------------------------------*/

RETCODE jultime(const jtime_tm *time, jtime_t *jtime) 
{
	RETCODE rc;
	jtime_t base_jtime;
	int day, year, month;
	div_t divmonth;

	// make sure we're working with a valid date
	if (!time->is_valid())
	{
		return (SQL_ERROR);
	}

	day = time->day;

	divmonth = div(time->year*12 + time->mon - 1, 12);
	year = divmonth.quot;
	month = divmonth.rem + 1;

	rc = julday(day, month, year, &base_jtime);
	
	*jtime = base_jtime;
	*jtime += time->hour * 3600;
	*jtime += time->min * 60;
	*jtime += time->sec;

	return rc;
}


/*------------------------------------------------------------------------*
 * Returns the current time under the default time zone
 * On error returns 0
 *------------------------------------------------------------------------*/

jtime_t Now(void) {
	time_t tp;
	struct tm *ptm;
	RETCODE rc;
	jtime_t jtime;
	jtime_tm jtm;

	if (time(&tp) == -1)
		return 0;
	// ptm = gmtime(&tp);
	ptm = localtime(&tp);
	if (ptm == NULL)
		return 0;

	jtm.sec = ptm->tm_sec;
	jtm.min = ptm->tm_min;
	jtm.hour = ptm->tm_hour;
	jtm.day = ptm->tm_mday;
	jtm.mon = ptm->tm_mon + 1;
	jtm.year = ptm->tm_year + 1900;

	rc = jultime(&jtm, &jtime);
	if (!RC_SUCCESS(rc))
		return 0;

	// jtime -= g_localtime_gmt_offset_secs;
	
	if (!RC_SUCCESS(rc))
		return 0;

	return jtime;
}

/*------------------------------------------------------------------------*
 * Returns the current date under the default time zone
 * On error returns 0
 *------------------------------------------------------------------------*/
jtime_t Today(void) {
	time_t tp;
	struct tm *ptm;
	RETCODE rc;
	jtime_t jtime;
	jtime_tm jtm;

	if (time(&tp) == -1)
		return 0;
	// ptm = gmtime(&tp);
	ptm = localtime(&tp);
	if (ptm == NULL)
		return 0;

	jtm.sec = 0;
	jtm.min = 0;
	jtm.hour = 0;
	jtm.day = ptm->tm_mday;
	jtm.mon = ptm->tm_mon + 1;
	jtm.year = ptm->tm_year + 1900;

	rc = jultime(&jtm, &jtime);
	if (!RC_SUCCESS(rc))
		return 0;

	// jtime -= g_localtime_gmt_offset_secs;
	
	if (!RC_SUCCESS(rc))
		return 0;

	return jtime;
}

/*------------------------------------------------------------------------*
 * This routine computes the Julian time of the
 * calendar date at midnight specified by 
 * month	(1, 12)
 * day		day of the month (1, 31)
 * year		AD year number
 *
 * Dates must lie in the range January 1, 1970, to midnight, Dec 31, 2399.
 * The calculated date is returned in jtime.  The routine returns
 * SQL_SUCCESS if the calculation was successful and SQL_ERROR otherwise
 *------------------------------------------------------------------------*/
RETCODE julday(int day, int month, int year, jtime_t *jtime) {
	RETCODE rc;
	long juldate;

	rc = nr_julday(month, day, year, &juldate);

	if (juldate > MAX_JDATE  || juldate < MIN_JDATE)
		return (SQL_ERROR);

	juldate -= JULDAY_JAN_1_1970;
	*jtime = juldate * SECONDS_PER_DAY;

	return rc;
}

/*------------------------------------------------------------------------*
 * Inverse of the function jultime given above.  Here jtime is input
 * and the routine returns the time in the structure time
 *------------------------------------------------------------------------*/
RETCODE unjultime(jtime_t jtime, jtime_tm *time){
	ldiv_t dayfrac;
	long juldate;
	int mm, dd, iyyy, hour, min, sec;
	RETCODE rc;

   // don't compare against MIN if MIN is zero. Removes a useless compiler warning.
#if MIN_JTIME == 0
	if (jtime >	MAX_JTIME)
		return (SQL_ERROR);
#else
	if (jtime >	MAX_JTIME  || jtime < MIN_JTIME)
		return (SQL_ERROR);
#endif

	dayfrac.quot = jtime / SECONDS_PER_DAY;
	dayfrac.rem = jtime - (SECONDS_PER_DAY * dayfrac.quot);
	juldate = dayfrac.quot;
	juldate += JULDAY_JAN_1_1970;
	rc = nr_unjulday(juldate, &mm, &dd, &iyyy);

	dayfrac = ldiv(dayfrac.rem, 3600); 
	hour = dayfrac.quot;

	dayfrac = ldiv(dayfrac.rem, 60); 
	min = dayfrac.quot;

	sec = dayfrac.rem;

	time->sec = sec;
	time->min = min;
	time->hour = hour;
	time->day = dd;
	time->mon = mm;
	time->year = iyyy;

	return (rc);
}

/*------------------------------------------------------------------------*
 * Inverse of the function julday given above.  Here jtime is input
 * and the routine outputs dd, mm, and iyyy
 * as the day, month and year for the specified time
 * started at noon.
 *------------------------------------------------------------------------*/
RETCODE unjulday(jtime_t jtime, int *dd, int *mm, int *iyyy){
	long juldate;
	RETCODE rc;

#if MIN_JTIME == 0
	if (jtime >	MAX_JTIME)
		return (SQL_ERROR);
#else
	if (jtime >	MAX_JTIME  || jtime < MIN_JTIME)
		return (SQL_ERROR);
#endif

	juldate  = jtime / SECONDS_PER_DAY;
	juldate += JULDAY_JAN_1_1970;
	rc = nr_unjulday(juldate, mm, dd, iyyy);

	return rc;
}

/*------------------------------------------------------------------------*
 * This routine takes the time stored in oldtime and adds to it the
 * offset specified in the variable add to obtain
 * a new time which is returned in newtime
 *------------------------------------------------------------------------*/
RETCODE addtime(jtime_t oldtime, jtime_tm add, jtime_t *newtime) {
	jtime_tm oldtm, newtm;
	RETCODE rc;

	rc = unjultime(oldtime, &oldtm);
	if(!RC_SUCCESS(rc))
		return rc;

	newtm.year = oldtm.year + add.year;
	newtm.mon = oldtm.mon + add.mon;
	newtm.day = oldtm.day + add.day;
	newtm.hour = oldtm.hour + add.hour;
	newtm.min = oldtm.min + add.min;
	newtm.sec = oldtm.sec + add.sec;

	return jultime(&newtm, newtime);
}

/*------------------------------------------------------------------------*
 * This routine takes the time stored in oldtime and adds to it the
 * month offset specified in the variable months to obtain
 * a new time which is returned
 *
 * N.B.!  Be careful when using this function to add months to dates at
 * the end of the month.  If your original month has 31 days but your
 * target month has only 30 days then your date will roll to the next month.
 * Thus: addmonths(_TEXT("31-MAR-XX"), 1) = _TEXT("01-MAY-XX") rather than _TEXT("30-APR-XX")
 *------------------------------------------------------------------------*/
jtime_t addmonths(jtime_t oldtime, int months) {
	jtime_tm oldtm, newtm;
	jtime_t newtime;
	ldiv_t YrMo;
	RETCODE rc;

	rc = unjultime(oldtime, &oldtm);
	assert(RC_SUCCESS(rc));

	// Find new date in terms of years & months
	// New Year is the quotient
	// New Month is the remainder
	//
	// We do ldiv on year*12+mon+offset in order
	// to properly handle both +ve and -ve offsets
	//
	YrMo = ldiv(oldtm.year*12 + oldtm.mon + months, 12); 

	newtm.year = YrMo.quot;
	newtm.mon = YrMo.rem;
	newtm.day = oldtm.day;
	newtm.hour = oldtm.hour;
	newtm.min = oldtm.min;
	newtm.sec = oldtm.sec;

	rc = jultime(&newtm, &newtime);
	assert(RC_SUCCESS(rc));

	return newtime;
}


/*------------------------------------------------------------------------*
 * This routine takes the time stored in oldtime and adds to it the
 * specified number of seconds to obtain
 * a new time which is returned
 *------------------------------------------------------------------------*/
jtime_t addsecs(jtime_t oldtime, int secs) {

	return oldtime + secs;
}



/*------------------------------------------------------------------------*
 * Convert a Julian time to a tstring in the form MON-DD-YYYY
 *------------------------------------------------------------------------*/
tstring jul2str(jtime_t juldate) {
	static const TCHAR szErr[]=_TEXT("#DATE_ERR");
	static const TCHAR mm_name[13][4] = {_TEXT(""), _TEXT("JAN"), _TEXT("FEB"), _TEXT("MAR"), _TEXT("APR"), _TEXT("MAY"), _TEXT("JUN"),
				_TEXT("JUL"), _TEXT("AUG"), _TEXT("SEP"), _TEXT("OCT"), _TEXT("NOV"), _TEXT("DEC")};
	int mm, id, iyyy;
	RETCODE rc;

	rc = unjulday(juldate, &id, &mm, &iyyy);
	if(!RC_SUCCESS(rc))
		return szErr;

	tostringstream o;

	o << mm_name[mm] << _TEXT("-");
	o.width(2);
	o.fill(_TEXT('0'));
	o << id;
	o << _TEXT("-");
	o.width(4);
	o.fill(_TEXT('0'));
	o << iyyy;
	
	return o.str();
}

/*------------------------------------------------------------------------*
 * Convert a Julian date tstring in the form MON-DD-YYYY HH:MM:SS
 * to a julian time where HH is a number from 0-23 representing
 * the number of hours since midnight.
 *------------------------------------------------------------------------*/
RETCODE str2jultm(const TCHAR *szDt, jtime_t *pjtime) {
	static TCHAR mm_name[13][4] = {_TEXT(""), _TEXT("JAN"), _TEXT("FEB"), _TEXT("MAR"), _TEXT("APR"), _TEXT("MAY"), _TEXT("JUN"),
				_TEXT("JUL"), _TEXT("AUG"), _TEXT("SEP"), _TEXT("OCT"), _TEXT("NOV"), _TEXT("DEC")};
	int mm, dd, yy, hh, mi, ss;
	int monField;
	TCHAR *szField[6];
	TCHAR szDate[60];
	int i, j;
	jtime_tm newtm;
	TCHAR *last = NULL;

	std_tstrncpy(szDate, szDt, sizeof(szDate)/sizeof(TCHAR));
	szField[0] = std_tstrtok(szDate, _TEXT("-/., :"), &last);
	szField[1] = std_tstrtok(NULL, _TEXT("-/., :"), &last);
	szField[2] = std_tstrtok(NULL, _TEXT("-/., :"), &last);
	szField[3] = std_tstrtok(NULL, _TEXT("-/., :"), &last);
	szField[4] = std_tstrtok(NULL, _TEXT("-/., :"), &last);
	szField[5] = std_tstrtok(NULL, _TEXT("-/., :"), &last);

	if (szField[0] == NULL || szField[1] == NULL || szField[2] == NULL) {
		*pjtime = 0;
		return (SQL_ERROR);
	}

	// Either first or second field may contain month name
	mm = 0;
	monField = -1;
	for (j = 0; j < 2; j++)
		if(std_tisalpha(szField[j][0])) {
			if (monField != -1)
				return (SQL_ERROR);

			for (i = 1; i < 13; i++)
				if (std_tstrnicmp(szField[j], mm_name[i], 3) == 0) {
					mm = i;
					monField = j;
					break;
				}

			if (mm == 0) // Invalid field, return error
				return (SQL_ERROR);
		}

	// No character month name found, assume date in form
	// mm/dd/yy
	if (mm == 0) {
		mm = std_tatoi(szField[0]);
		dd = std_tatoi(szField[1]);
	} else 
		if (monField == 0)
			dd = std_tatoi(szField[1]);
		else
			dd = std_tatoi(szField[0]);

	// Always assume that year is last field
	yy = std_tatoi(szField[2]);
	if (yy < 100) {
		if (yy > 70) {
			yy += 1900;
		} else {
			yy += 2000;
		}
	}

	if (szField[3] != NULL) {
		hh = std_tatoi(szField[3]);
		mi = std_tatoi(szField[4]);
		ss = std_tatoi(szField[5]);
	} else {
		hh = 0;
		mi = 0;
		ss = 0;
	}
	
	
	newtm.year = yy;
	newtm.mon = mm;
	newtm.day = dd;
	newtm.hour = hh;
	newtm.min = mi;
	newtm.sec = ss;


	return jultime(&newtm, pjtime); 
}


/*------------------------------------------------------------------------*
 * Convert a Julian time to a tstring in the form MON-DD-YYYY HH:MM:SS
 *------------------------------------------------------------------------*/
tstring jul2strtm(jtime_t jtime) {
	static TCHAR szErr[]=_TEXT("#DATE_ERR");
	static TCHAR mm_name[13][4] = {_TEXT(""), _TEXT("JAN"), _TEXT("FEB"), _TEXT("MAR"), _TEXT("APR"), _TEXT("MAY"), _TEXT("JUN"),
				_TEXT("JUL"), _TEXT("AUG"), _TEXT("SEP"), _TEXT("OCT"), _TEXT("NOV"), _TEXT("DEC")};
	RETCODE rc;
	jtime_tm time;

	rc = unjultime(jtime, &time);
	if(!RC_SUCCESS(rc))
		return szErr;

	tostringstream o;

	o << mm_name[time.mon] << _TEXT("-");
	o.width(2);
	o.fill(_TEXT('0'));
	o << time.day;
	o << _TEXT("-");
	o.width(4);
	o.fill(_TEXT('0'));
	o << time.year << _TEXT(" ");
	if (!(time.hour == 0 && time.min == 0 && time.sec == 0) ){
		o << _TEXT(" ");
		o.width(2);
		o.fill(_TEXT('0'));
		o << time.hour;
		o << _TEXT(":");
		o.width(2);
		o.fill(_TEXT('0'));
		o << time.min;
		o << _TEXT(":");
		o.width(2);
		o.fill(_TEXT('0'));
		o << time.sec;
	}
	
	return o.str();
}


/*------------------------------------------------------------------------*
 * Convert a Julian time to a ODBC SQL date tstring in the form 
 * {ts 'yyyy-mm-dd hh:mm:ss'}
 *------------------------------------------------------------------------*/
void jul2SQLtm(jtime_t jultime, TCHAR *szSQLtm) {
	static TCHAR szErr[]=_TEXT("#DATE_ERR");
	RETCODE rc;
	jtime_tm time;
	tostringstream o;

	rc = unjultime(jultime, &time);
	if(!RC_SUCCESS(rc)) {
		o << szErr;
		std_tstrcpy(szSQLtm, o.str().c_str());
		return;
	}
	
	o << _TEXT("{ts '");
	o.width(4);
	o.fill(_TEXT('0'));
	o << time.year << _TEXT("-");
	o.width(2);
	o.fill(_TEXT('0'));
	o << time.mon << _TEXT("-");
	o.width(2);
	o.fill(_TEXT('0')) ;
	o << time.day << _TEXT(" ");
	o.width(2);
	o.fill(_TEXT('0')) ;
	o << time.hour;
	o << _TEXT(":");
	o.width(2);
	o.fill(_TEXT('0'));
	o << time.min;
	o << _TEXT(":");
	o.width(2);
	o.fill(_TEXT('0'));
	o << time.sec;
	o << _TEXT("'}");

	std_tstrcpy(szSQLtm, o.str().c_str());
}


/*------------------------------------------------------------------------*
 * Returns date1 - date2 expressed in days, rounded up to the nearest day
 *------------------------------------------------------------------------*/
double diffdays(jtime_t end, jtime_t begin) {
   double diff_secs;
  
   /* Avoid potential unsigned long subtraction error */
   diff_secs = end;
   diff_secs -= begin;
   return ceil(diff_secs / (double) SECONDS_PER_DAY );
}

/*------------------------------------------------------------------------*
 * Returns date1 - date2 expressed in hours, rounded up to the nearest hour
 *------------------------------------------------------------------------*/
double diffhours(jtime_t end, jtime_t begin) {
   double diff_secs;
  
   /* Avoid potential unsigned long subtraction error */
   diff_secs = end;
   diff_secs -= begin;
   return ceil(diff_secs / (double) 3600 );
}

/*------------------------------------------------------------------------*
 * Returns date1 - date2 expressed in seconds
 *------------------------------------------------------------------------*/
double diffsecs(jtime_t end, jtime_t begin) {
   double diff_secs;
  
   /* Avoid potential unsigned long subtraction error */
   diff_secs = end;
   diff_secs -= begin;
   return diff_secs;
}


/*------------------------------------------------------------------------*
 * Find the number of days in the given month
 * Based on K & R p. 111
 * Inputs: year, month
 * Outputs: number of days in the month
 *------------------------------------------------------------------------*/
int month_days(int year, int month) {
	static TCHAR daytab[2][13] = {
		{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 ,31},
		{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30 ,31}
	};

	int leap;

	assert(month > 0 && month < 13);

	leap = ( year%4 == 0 && year%100 !=0 ) || year%400 == 0;

	return daytab[leap][month];
}

/*------------------------------------------------------------------------*
 * Given a particular day return the time corresponding to
 * the beginning of that week (Sunday)
 *------------------------------------------------------------------------*/
jtime_t beginweek(jtime_t oldtime) {
	jtime_t newtime;
	
	newtime = adddays(oldtime, -weekday(oldtime));

	newtime = beginday(newtime);
		
	return newtime;
}

/*------------------------------------------------------------------------*
 * Given a particular day return the time corresponding to
 * the end of that week (Sunday)
 *------------------------------------------------------------------------*/
jtime_t eoweek(jtime_t oldtime) {
	jtime_t newtime;
	
	newtime = adddays(oldtime, 6 - weekday(oldtime));
	newtime = eoday(newtime);
		
	return newtime;
}

/*------------------------------------------------------------------------*
 * Given a particular starting date return the date corresponding
 * to the end of the month specified by that date
 *------------------------------------------------------------------------*/
jtime_t eomonth(jtime_t oldtime) {
	jtime_tm oldtm, newtm;
	jtime_t newtime;
	RETCODE rc;

	rc = unjultime(oldtime, &oldtm);
	assert(RC_SUCCESS(rc));

	newtm.year = oldtm.year;
	newtm.mon = oldtm.mon;
	newtm.day = month_days(oldtm.year, oldtm.mon);
	newtm.hour = 23;

	newtm.min = 59;
	newtm.sec = 59;

	rc = jultime(&newtm, &newtime);
	assert(RC_SUCCESS(rc));

	return newtime;
}


/*------------------------------------------------------------------------*
 * Given a particular starting date return the date corresponding
 * to the beginning of the month specified by that date
 *------------------------------------------------------------------------*/
jtime_t beginmonth(jtime_t oldtime) {
	jtime_tm oldtm, newtm;
	jtime_t newtime;
	RETCODE rc;

	rc = unjultime(oldtime, &oldtm);
	assert(RC_SUCCESS(rc));

	newtm.year = oldtm.year;
	newtm.mon = oldtm.mon;
	newtm.day = 1;
	newtm.hour = 0;
	newtm.min = 0;
	newtm.sec = 0;

	rc = jultime(&newtm, &newtime);
	assert(RC_SUCCESS(rc));
	return newtime;
}

/*------------------------------------------------------------------------*
 * Given a particular date return the year number corresponding
 * to that date.
 * On error, the routine returns 0, otherwise it returns a
 * month number (1-12)
 *------------------------------------------------------------------------*/
int year(jtime_t oldtime) {
	jtime_tm oldtm;
	RETCODE rc;

	rc = unjultime(oldtime, &oldtm);
	assert(RC_SUCCESS(rc));

	return oldtm.year;

}


/*------------------------------------------------------------------------*
 * Given a particular date return the month number corresponding
 * to that date.
 * On error, the routine returns 0, otherwise it returns a
 * month number (1-12)
 *------------------------------------------------------------------------*/
int month(jtime_t oldtime) {
	jtime_tm oldtm;
	RETCODE rc;

	rc = unjultime(oldtime, &oldtm);
	assert(RC_SUCCESS(rc));

	return oldtm.mon;
}


/*------------------------------------------------------------------------*
 * Given a particular date return the what week in the current month
 * this represents, i.e. first week, second week, etc.
 * Weeks are assumed to start on Sunday and end on Saturday
 *------------------------------------------------------------------------*/
int monthweek(jtime_t now) {
	jtime_tm nowtm;
	int beginmo_wkday;
	RETCODE rc;

	// Find day of week at begin of month
	beginmo_wkday = weekday(beginmonth(now));

	rc = unjultime(now, &nowtm);
	assert(RC_SUCCESS(rc));

	return (nowtm.day - 1 + beginmo_wkday ) / 7 + 1; 
}

/*------------------------------------------------------------------------*
 * Convert an Excel date format to a jtime
 *------------------------------------------------------------------------*/
RETCODE XL2jtime(double xltime, jtime_t *jtime) {
	xltime -= XL_JULDAY_JAN_1_1970;
	if (xltime < 0  || (xltime + JULDAY_JAN_1_1970) > MAX_JDATE) {
		*jtime = 0;
		return (SQL_ERROR);
	}
	*jtime = (jtime_t)(xltime * SECONDS_PER_DAY);

	// *jtime = (jtime_t)(xltime * SECONDS_PER_DAY + _timezone);  // Convert to UTC time
	// convert_from_timezone(*jtime, ")UTC_TEXT(", jtime); 
	return (SQL_SUCCESS);
}

/*------------------------------------------------------------------------*
 * Convert a jtime to an Excel date format 
 *------------------------------------------------------------------------*/
RETCODE jtime2XL(jtime_t jtime, double *xltime) {
	// jtime_t utctime;

	// convert_to_timezone(jtime, ")UTC_TEXT(", &utctime); 
	// utctime -= _timezone; // Get local time
	// *xltime = (double)XL_JULDAY_JAN_1_1970 + (utctime / (double)SECONDS_PER_DAY);

	*xltime = (double)XL_JULDAY_JAN_1_1970 + (jtime / (double)SECONDS_PER_DAY);
	return (SQL_SUCCESS);
}


#if 0

/*------------------------------------------------------------------------*
 * Unit test the date library
 *------------------------------------------------------------------------*/
void date_unit_test(void) {
	timestamp_t tst, tst2;
	RETCODE rc;
	jtime_t jtime, jtime2;
	double ddays;
	jtime_tm jtm;
	int dd, mm, iyyy;
	TCHAR szDate[30];
	int i, mdays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 ,31};
		

	tst.year = 1997;
	tst.month = 3;
	tst.day = 10;
	tst.hour = 6;
	tst.minute = 7;
	tst.second = 8;


	rc = Timestamp2Jtime(&tst, &jtime);
	assert(RC_SUCCESS(rc));
	rc = Jtime2Timestamp(jtime, &tst2);
	assert(RC_SUCCESS(rc));
	tst.fraction = 0;
	tst2.fraction = 0;
	assert(memcmp((void *)&tst, (void *)&tst2, sizeof(tst)) == 0);

	rc = julday(10, 3,1997, &jtime2);
	ddays = diffdays(jtime, jtime2);
	assert(ddays == 1);
	assert(SameDay(jtime, jtime2)) ;
	
	
	rc = unjultime(jtime, &jtm);
	assert(RC_SUCCESS(rc));
	assert(jtm.sec == 8);
	assert(jtm.min == 7);
	assert(jtm.hour == 6);
	assert(jtm.day == 10);
	assert(jtm.mon == 3);
	assert(jtm.year == 1997);

	rc = unjulday(jtime, &dd, &mm, &iyyy);
	assert(RC_SUCCESS(rc));
	assert(dd == 10);
	assert(mm == 3);
	assert(iyyy == 1997);

	jtm.mon =0;
	jtm.year = 0;
	rc = addtime(jtime, jtm, &jtime2);
	rc = unjultime(jtime2, &jtm);
	assert(RC_SUCCESS(rc));
	assert(jtm.sec == 8*2);
	assert(jtm.min == 7*2);
	assert(jtm.hour == 6*2);
	assert(jtm.day == 10*2);
	assert(jtm.mon == 3);
	assert(jtm.year == 1997);


	jtime2= adddays(jtime, 10);
	rc = unjultime(jtime2, &jtm);
	assert(RC_SUCCESS(rc));
	assert(jtm.sec == 8);
	assert(jtm.min == 7);
	assert(jtm.hour == 6);
	assert(jtm.day == 10*2);
	assert(jtm.mon == 3);
	assert(jtm.year == 1997);

	jtime2 = AddOffset(jtime, jtime, jtime, jtime, 1, DB_UOM_ID_Day);
	rc = unjultime(jtime2, &jtm);
	assert(RC_SUCCESS(rc));
	assert(jtm.sec == 8);
	assert(jtm.min == 7);
	assert(jtm.hour == 6);
	assert(jtm.day == 11);
	assert(jtm.mon == 3);
	assert(jtm.year == 1997);

	jtime2 = AddOffset(jtime, jtime, jtime, jtime, 1, DB_UOM_ID_Week);
	rc = unjultime(jtime2, &jtm);
	assert(RC_SUCCESS(rc));
	assert(jtm.sec == 8);
	assert(jtm.min == 7);
	assert(jtm.hour == 6);
	assert(jtm.day == 17);
	assert(jtm.mon == 3);
	assert(jtm.year == 1997);

	jtime2 = AddOffset(jtime, jtime, jtime, jtime, 1, DB_UOM_ID_Month);
	rc = unjultime(jtime2, &jtm);
	assert(RC_SUCCESS(rc));
	assert(jtm.sec == 8);
	assert(jtm.min == 7);
	assert(jtm.hour == 6);
	assert(jtm.day == 10);
	assert(jtm.mon == 4);
	assert(jtm.year == 1997);



	sprintf(szDate, jul2str(jtime));
	assert(strcmp(szDate, ")MAR-10-1997_TEXT(")==0);

	sprintf(szDate, jul2strtm(jtime));
	assert(strcmp(szDate, ")MAR-10-1997 06:07:08_TEXT(")==0);

	jtime2 = jtime;

	jtime = beginday(jtime); 
	sprintf(szDate, jul2strtm(jtime));
	assert(strcmp(szDate, ")MAR-10-1997_TEXT(")==0);

	jtime = eoday(jtime); 
	sprintf(szDate, jul2strtm(jtime));
	assert(strcmp(szDate, ")MAR-10-1997 23:59:59_TEXT(")==0);
	
	jtime = jtime2;

	jtime2 = eomonth(jtime);
	rc = unjultime(jtime2, &jtm);
	assert(RC_SUCCESS(rc));
	assert(jtm.sec == 8);
	assert(jtm.min == 7);
	assert(jtm.hour == 6);
	assert(jtm.day == 31);
	assert(jtm.mon == 3);
	assert(jtm.year == 1997);

	jtime2 = eoday(jtime);
	rc = unjultime(jtime2, &jtm);
	assert(RC_SUCCESS(rc));
	assert(jtm.sec == 59);
	assert(jtm.min == 59);
	assert(jtm.hour == 23);
	assert(jtm.day == 10);
	assert(jtm.mon == 3);
	assert(jtm.year == 1997);

	for (i = 1; i <= 12; i++)
		assert(month_days(1997, i) == mdays[i]);

	assert(month(jtime) == 3);

	assert(weekday(jtime) == 1);

	assert(monthweek(jtime) == 3);

	jtime2 = beginmonth(jtime);
	rc = unjultime(jtime2, &jtm);
	assert(RC_SUCCESS(rc));
	assert(jtm.sec == 8);
	assert(jtm.min == 7);
	assert(jtm.hour == 6);
	assert(jtm.day == 1);
	assert(jtm.mon == 3);
	assert(jtm.year == 1997);

}
#endif

/*------------------------------------------------------------------------* 
 * Given two times, return the later (max) of these two times
 *------------------------------------------------------------------------*/
jtime_t maxtime(jtime_t a, jtime_t b) {
	return DTL_MAX(a, b);
}

/*------------------------------------------------------------------------*
 * Given two times, return the earlier (min) of these two times
 *------------------------------------------------------------------------*/
jtime_t mintime(jtime_t a, jtime_t b) {
	return DTL_MIN(a, b);
}

/*------------------------------------------------------------------------*
 * This routine takes the time stored in oldtime and adds to it the
 * specified number of days to obtain
 * a new time which is returned
 *------------------------------------------------------------------------*/
jtime_t adddays(jtime_t oldtime, int days) {

	return oldtime + SECONDS_PER_DAY * days;
}

/*------------------------------------------------------------------------*
 * This routine takes the time stored in oldtime and adds to it the
 * specified number of days to obtain
 * a new time which is returned
 *------------------------------------------------------------------------*/
jtime_t addhours(jtime_t oldtime, int hours) {

	return oldtime + SECONDS_PER_HOUR * hours;
}

/*------------------------------------------------------------------------*
 * Given a particular day return the time corresponding to
 * the end of that day
 *------------------------------------------------------------------------*/
jtime_t eoday(jtime_t oldtime) {
	jtime_t newtime;
	
	newtime = (int)(oldtime / SECONDS_PER_DAY);
	newtime *= SECONDS_PER_DAY;
	newtime += SECONDS_PER_DAY - 1;

	return newtime;
}


/*------------------------------------------------------------------------*
 * Given a particular day return the time corresponding to
 * the beginning of that day
 *------------------------------------------------------------------------*/
jtime_t beginday(jtime_t oldtime) {
	jtime_t newtime;
	
	newtime = (int)(oldtime / SECONDS_PER_DAY);
	newtime *= SECONDS_PER_DAY;
		
	return newtime;
}


/*------------------------------------------------------------------------*
 * Given a particular date return the weekday corresponding
 * to that date. 0 = Sunday, 1 = Monday etc.
 *------------------------------------------------------------------------*/
int weekday(jtime_t oldtime) {

	return (oldtime/SECONDS_PER_DAY + 4) % 7;
}

/*------------------------------------------------------------------------*
 * SameDay: Tests to see if two times refer to the same day
 * Returns TRUE if both times are on the same day, false otherwise
 *------------------------------------------------------------------------*/
short SameDay(jtime_t jultime1, jtime_t jultime2) {
	return (jultime1 / SECONDS_PER_DAY) == (jultime2 / SECONDS_PER_DAY);
}

// ****************** Implementation code for jtime_c **************************
jtime_c::jtime_c(const jtime_t &t) : jtime(t) {}

jtime_c::jtime_c(const timestamp_t &ts) {
	RETCODE rc;
	rc = Timestamp2Jtime(&ts, &jtime );
	if (!RC_SUCCESS(rc))
	{
		DTL_THROW RootException(_TEXT("jtime_c::jtime_c(timestamp t&)"), _TEXT("Invalid time passed to constructor"));
	}
}

jtime_c::jtime_c() : jtime(MIN_JTIME) {	}

jtime_c::operator jtime_t() const
{
	return jtime;
}

jtime_c::operator timestamp_t() const
{
	timestamp_t tst;
	RETCODE rc;
	rc = Jtime2Timestamp(jtime, &tst);
	if (!RC_SUCCESS(rc))
	{
		DTL_THROW RootException(_TEXT("jtime_c::operator timestamp_t()"), _TEXT("Invalid time held by class."));
	}

	return tst;
}

jtime_c &jtime_c::operator=(const jtime_c &other)
{ 
   jtime = other.jtime;
   return *this;
}

jtime_c &jtime_c::operator=(const jtime_t &t)
{ 
   jtime = t;
   return *this;
}

jtime_c &jtime_c::operator=(const timestamp_t &ts) {
	RETCODE rc;
	rc = Timestamp2Jtime(&ts, &jtime );
	if (!RC_SUCCESS(rc))
	{
		DTL_THROW RootException(_TEXT("jtime_c::jtime_c(timestamp t&)"), _TEXT("Invalid time passed to constructor"));
	}

    return *this;
}

bool operator<(const jtime_c &lhs, const jtime_c &rhs) {return lhs.jtime < rhs.jtime;};
bool operator==(const jtime_c &lhs, const jtime_c &rhs) {return lhs.jtime == rhs.jtime;};
bool operator!=(const jtime_c &lhs, const jtime_c &rhs) {return lhs.jtime != rhs.jtime;};
bool operator>(const jtime_c &lhs, const jtime_c &rhs) {return lhs.jtime > rhs.jtime;};
bool operator>=(const jtime_c &lhs, const jtime_c &rhs) {return lhs.jtime >= rhs.jtime;};
bool operator<=(const jtime_c &lhs, const jtime_c &rhs) {return lhs.jtime <= rhs.jtime;};

STD_::ostream &operator<<(STD_::ostream &o, const jtime_c &ts) 
{
	o << tstring_cast((STD_::string *)NULL, tstring(jul2strtm(ts.jtime)));
	return o;
};

STD_::istream &operator>>(STD_::istream &i, jtime_c &ts)
{
	STD_::string str;
	i >> str;
	// ts = jtime_c(tstring_cast((STD_::string *) NULL, str));
	tstring tstr = tstring_cast((tstring *) NULL, str);
	
	try
	{
		ts = jtime_c(tstr);
	}
	catch (...)
	{
		i.setstate(STD_::ios_base::failbit);
	}

	return i;
}

#ifndef DTL_NO_UNICODE

STD_::wostream &operator<<(STD_::wostream &o, const jtime_c &ts) 
{
	o << tstring_cast((STD_::wstring *)NULL, tstring(jul2strtm(ts.jtime)));
	return o;
}

STD_::wistream &operator>>(STD_::wistream &i, jtime_c &ts)
{
	STD_::wstring str;
	i >> str;
	// ts = jtime_c(tstring_cast((STD_::wstring *) NULL, str));
	tstring tstr = tstring_cast((tstring *) NULL, str);

	try
	{
		ts = jtime_c(tstr);
	}
	catch (...)
	{
		i.setstate(STD_::ios_base::failbit);
	}

	return i;
}
#endif

jtime_c::operator STD_::string() const
{
	STD_::ostringstream stream;
	stream << *this;
	return stream.str();
}

#if !defined(DTL_NO_UNICODE)
jtime_c::operator STD_::wstring() const
{
	STD_::wostringstream stream;
	stream << *this;
	return stream.str();
}
#endif

END_DTL_NAMESPACE
