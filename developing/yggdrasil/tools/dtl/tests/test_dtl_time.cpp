#include "std_warn_off.h"
#include <iostream>
#include <DTL.h>
#include <dtl_time.h>
#include <nullable.h>
#include "std_warn_on.h"

using namespace dtl;
using namespace std;


#ifndef DTL_HAVE_BOOST

void Test_dtl_time()
{
   tcout << "Not supported without DTL_HAVE_BOOST" << endl;
}

#else

typedef nullable<dtl_time> nulltime;

namespace dtl {
   template <>
   struct DefaultBCA<dtl_time>
   {
     void operator()(BoundIOs & boundIOs, dtl_time & rowbuf)
     {
        boundIOs[_TEXT("EXAMPLE_TIME")] == rowbuf;
     }
   };

   template <>
   struct DefaultBCA<nulltime>
   {
     void operator()(BoundIOs & boundIOs, nulltime & rowbuf)
     {
        boundIOs[_TEXT("EXAMPLE_TIME")] == rowbuf;
     }
   };
}



void Test_dtl_time()
{
   try {
		DBStmt(_TEXT("CREATE TABLE test_time (EXAMPLE_TIME TIME)")).Execute();
   }
   catch (...) {};


   /*
	DBStmt(_TEXT("delete from test_time")).Execute();
	DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values ('01:01:01')")).Execute();
	DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values ('02:02:02')")).Execute();
	DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values ('03:03:03')")).Execute();
	DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values ('04:04:04')")).Execute();
	DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values ('05:05:05')")).Execute();

   DBView<dtl_time> plain_view(_TEXT("test_time"));
   RandomDBView<dtl_time> view(plain_view);

   // first, a straight copy into a vector from plain_view
   tcout << "Test full read via DBView<>" << endl;
   vector<dtl_time> copy1(plain_view.begin(),plain_view.end());
   tcout << "output from DBView\n";
   vector<dtl_time>::iterator it;
   for (it = copy1.begin(); it != copy1.end(); ++it) {
	   cout << (int) it->hours() << endl;
   }
   // now via random
   // TODO theres a problem with user validation here
   tcout << "Test full read via RandomDBView<>" << endl;
   vector<dtl_time> copy2(view.begin(),view.end());

   tcout << "output from RandomDBView\n";
   for (it = copy2.begin(); it != copy2.end(); ++it) {
	   cout << (int) it->hours() << endl;
   }
   */
   tcout << "Testing 00:00:00 time" << endl;

   {
      DBStmt(_TEXT("delete from test_time")).Execute();
      DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values "
               "('00:00:00')")).Execute();
      DBView<dtl_time> plain_view(_TEXT("test_time"));
      dtl_time t = *plain_view.begin();
      assert(t == dtl_time(0,0,0));
   }

   tcout << "Testing 23:59:59 date" << endl;

   {
      DBStmt(_TEXT("delete from test_time")).Execute();
      DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values "
               "('23:59:59')")).Execute();
      DBView<dtl_time> plain_view(_TEXT("test_time"));
      dtl_time t = *plain_view.begin();
      assert(t == dtl_time(23,59,59));
   }

   tcout << "Testing 05:05:05 date" << endl;

   {
      DBStmt(_TEXT("delete from test_time")).Execute();
      DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values "
               "('05:05:05')")).Execute();
      DBView<dtl_time> plain_view(_TEXT("test_time"));
      dtl_time t = *plain_view.begin();
      assert(t == dtl_time(5,5,5));
   }

   tcout << "Testing pre-1970 (1929) date" << endl;

   {
      DBStmt(_TEXT("delete from test_time")).Execute();
      DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values "
               "('1929-08-05 05:05:05')")).Execute();
      DBView<dtl_time> plain_view(_TEXT("test_time"));
      dtl_time t = *plain_view.begin();
      assert(t == dtl_time(5,5,5));
   }

   tcout << "Testing NULL date with null data" << endl;

   {
      DBStmt(_TEXT("delete from test_time")).Execute();
      DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values "
               "(NULL)")).Execute();
      DBView<nulltime> plain_view(_TEXT("test_time"));
      nulltime t = *plain_view.begin();
      assert(t.empty());
   }

   tcout << "Testing NULL date with valid data" << endl;

   {
      DBStmt(_TEXT("delete from test_time")).Execute();
      DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values "
               "('1929-08-05 05:05:05')")).Execute();
      DBView<nulltime> plain_view(_TEXT("test_time"));
      nulltime t = *plain_view.begin();
      assert(!t.empty() && *t == dtl_time(5,5,5));
   }

   tcout << "Testing NULL date with null data via RandomDBView" << endl;

   {
      DBStmt(_TEXT("delete from test_time")).Execute();
      DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values "
               "(NULL)")).Execute();
      DBView<nulltime> plain_view(_TEXT("test_time"));
      RandomDBView<nulltime> view(plain_view);
      nulltime t = *view.begin();
      assert(t.empty());
   }

   tcout << "Testing NULL date with valid data via RandomDBView" << endl;

   {
      DBStmt(_TEXT("delete from test_time")).Execute();
      DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values "
               "('1929-08-05 05:05:05')")).Execute();
      DBView<nulltime> plain_view(_TEXT("test_time"));
      RandomDBView<nulltime> view(plain_view);
      nulltime t = *view.begin();
      assert(!t.empty() && *t == dtl_time(5,5,5));
   }

   tcout << "Iteration tests" << endl;

   {
      DBStmt(_TEXT("delete from test_time")).Execute();
      DBStmt(_TEXT("insert into test_time (EXAMPLE_TIME) values "
               "('01:01:01'),"
               "('02:02:02'),"
               "('03:03:03'),"
               "('04:04:04'),"
               "('05:05:05')")).Execute();

      DBView<dtl_time> plain_view(_TEXT("test_time"));
      RandomDBView<dtl_time> view(plain_view);

      // first, a straight copy into a vector from plain_view
      tcout << "Test full read via DBView<>" << endl;
      vector<dtl_time> copy1(plain_view.begin(),plain_view.end());
      // now via random
      // TODO theres a problem with user validation here
      tcout << "Test full read via RandomDBView<>" << endl;
      vector<dtl_time> copy2(view.begin(),view.end());
   }
}

#endif
