#include "std_warn_off.h"
#include <iostream>
#include <DTL.h>
#include <dtl_posix_time.h>
#include <nullable.h>
#include "std_warn_on.h"

using namespace dtl;
using namespace std;


#ifndef DTL_HAVE_BOOST

void Test_dtl_posix_time()
{
   tcout << "Not supported without DTL_HAVE_BOOST" << endl;
}

#else

typedef nullable<dtl_posix_time> nulldate;

namespace dtl {
   template <>
   struct DefaultBCA<dtl_posix_time>
   {
     void operator()(BoundIOs & boundIOs, dtl_posix_time & rowbuf)
     {
        boundIOs[_TEXT("EXAMPLE_DATETIME")] == rowbuf;
     }
   };

   template <>
   struct DefaultBCA<nulldate>
   {
     void operator()(BoundIOs & boundIOs, nulldate & rowbuf)
     {
        boundIOs[_TEXT("EXAMPLE_DATETIME")] == rowbuf;
     }
   };
}



void Test_dtl_posix_time()
{
   try {
		DBStmt(_TEXT("CREATE TABLE test_datetime (   EXAMPLE_DATETIME  DATETIME )")).Execute();
   }
   catch (...) {};

	DBStmt(_TEXT("delete from test_datetime")).Execute();
	DBStmt(_TEXT("insert into test_datetime (example_datetime) values ('2004-08-01 01:01:01')")).Execute();
	DBStmt(_TEXT("insert into test_datetime (example_datetime) values ('2004-08-02 02:02:02')")).Execute();
	DBStmt(_TEXT("insert into test_datetime (example_datetime) values ('2004-08-03 03:03:03')")).Execute();
	DBStmt(_TEXT("insert into test_datetime (example_datetime) values ('2004-08-04 04:04:04')")).Execute();
	DBStmt(_TEXT("insert into test_datetime (example_datetime) values ('2004-08-05 05:05:05')")).Execute();

   DBView<dtl_posix_time> plain_view(_TEXT("test_datetime"));
   RandomDBView<dtl_posix_time> view(plain_view);

   // first, a straight copy into a vector from plain_view
   tcout << "Test full read via DBView<>" << endl;
   vector<dtl_posix_time> copy1(plain_view.begin(),plain_view.end());
   tcout << "output from DBView\n";
   vector<dtl_posix_time>::iterator it;
   for (it = copy1.begin(); it != copy1.end(); ++it) {
	   const boost::gregorian::date d = it->date();
	   cout << (int) d.year() << endl;
   }
   // now via random
   // TODO theres a problem with user validation here
   tcout << "Test full read via RandomDBView<>" << endl;
   vector<dtl_posix_time> copy2(view.begin(),view.end());

   tcout << "output from RandomDBView\n";
   for (it = copy2.begin(); it != copy2.end(); ++it) {
	   const boost::gregorian::date d = it->date();
	   cout << (int) d.year() << endl;
   }
   tcout << "Testing 2004-01-01 date" << endl;

   {
      DBStmt(_TEXT("delete from test_datetime")).Execute();
      DBStmt(_TEXT("insert into test_datetime (example_datetime) values "
               "('2004-01-01 00:00:00')")).Execute();
      DBView<dtl_posix_time> plain_view(_TEXT("test_datetime"));
      dtl_posix_time t = *plain_view.begin();
      assert(t == dtl_posix_time(
               boost::gregorian::date(2004,1,1),
               boost::posix_time::time_duration(0,0,0)));
   }

   tcout << "Testing 2004-12-31 date" << endl;

   {
      DBStmt(_TEXT("delete from test_datetime")).Execute();
      DBStmt(_TEXT("insert into test_datetime (example_datetime) values "
               "('2004-12-31 23:59:59')")).Execute();
      DBView<dtl_posix_time> plain_view(_TEXT("test_datetime"));
      dtl_posix_time t = *plain_view.begin();
      assert(t == dtl_posix_time(
               boost::gregorian::date(2004,12,31),
               boost::posix_time::time_duration(23,59,59)));
   }

   tcout << "Testing 2004 date" << endl;

   {
      DBStmt(_TEXT("delete from test_datetime")).Execute();
      DBStmt(_TEXT("insert into test_datetime (example_datetime) values "
               "('2004-08-05 05:05:05')")).Execute();
      DBView<dtl_posix_time> plain_view(_TEXT("test_datetime"));
      dtl_posix_time t = *plain_view.begin();
      assert(t == dtl_posix_time(
               boost::gregorian::date(2004,8,5),
               boost::posix_time::time_duration(5,5,5)));
   }

   tcout << "Testing pre-1970 (1929) date" << endl;

   {
      DBStmt(_TEXT("delete from test_datetime")).Execute();
      DBStmt(_TEXT("insert into test_datetime (example_datetime) values "
               "('1929-08-05 05:05:05')")).Execute();
      DBView<dtl_posix_time> plain_view(_TEXT("test_datetime"));
      dtl_posix_time t = *plain_view.begin();
      assert(t == dtl_posix_time(
               boost::gregorian::date(1929,8,5),
               boost::posix_time::time_duration(5,5,5)));
   }

   tcout << "Testing NULL date with null data" << endl;

   {
      DBStmt(_TEXT("delete from test_datetime")).Execute();
      DBStmt(_TEXT("insert into test_datetime (example_datetime) values "
               "(NULL)")).Execute();
      DBView<nulldate> plain_view(_TEXT("test_datetime"));
      nulldate t = *plain_view.begin();
      assert(t.empty());
   }

   tcout << "Testing NULL date with valid data" << endl;

   {
      DBStmt(_TEXT("delete from test_datetime")).Execute();
      DBStmt(_TEXT("insert into test_datetime (example_datetime) values "
               "('1929-08-05 05:05:05')")).Execute();
      DBView<nulldate> plain_view(_TEXT("test_datetime"));
      nulldate t = *plain_view.begin();
      assert(!t.empty() &&
            *t == dtl_posix_time(
               boost::gregorian::date(1929,8,5),
               boost::posix_time::time_duration(5,5,5)));
   }

   tcout << "Testing NULL date with null data via RandomDBView" << endl;

   {
      DBStmt(_TEXT("delete from test_datetime")).Execute();
      DBStmt(_TEXT("insert into test_datetime (example_datetime) values "
               "(NULL)")).Execute();
      DBView<nulldate> plain_view(_TEXT("test_datetime"));
      RandomDBView<nulldate> view(plain_view);
      nulldate t = *view.begin();
      assert(t.empty());
   }

   tcout << "Testing NULL date with valid data via RandomDBView" << endl;

   {
      DBStmt(_TEXT("delete from test_datetime")).Execute();
      DBStmt(_TEXT("insert into test_datetime (example_datetime) values "
               "('1929-08-05 05:05:05')")).Execute();
      DBView<nulldate> plain_view(_TEXT("test_datetime"));
      RandomDBView<nulldate> view(plain_view);
      nulldate t = *view.begin();
      assert(!t.empty() &&
            *t == dtl_posix_time(
               boost::gregorian::date(1929,8,5),
               boost::posix_time::time_duration(5,5,5)));
   }

   tcout << "Iteration tests" << endl;

   {
      DBStmt(_TEXT("delete from test_datetime")).Execute();
      DBStmt(_TEXT("insert into test_datetime (example_datetime) values "
               "('2004-08-01 01:01:01'),"
               "('2004-08-02 02:02:02'),"
               "('2004-08-03 03:03:03'),"
               "('2004-08-04 04:04:04'),"
               "('2004-08-05 05:05:05')")).Execute();

      DBView<dtl_posix_time> plain_view(_TEXT("test_datetime"));
      RandomDBView<dtl_posix_time> view(plain_view);

      // first, a straight copy into a vector from plain_view
      tcout << "Test full read via DBView<>" << endl;
      vector<dtl_posix_time> copy1(plain_view.begin(),plain_view.end());
      // now via random
      // TODO theres a problem with user validation here
      tcout << "Test full read via RandomDBView<>" << endl;
      vector<dtl_posix_time> copy2(view.begin(),view.end());
   }
}

#endif
