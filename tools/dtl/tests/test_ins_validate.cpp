

#include "std_warn_off.h"
#include <iostream>
#include <DTL.h>
#include <dtl_set.h>
#include "std_warn_on.h"

#include <iostream>
using namespace std;

// Turn this on for tough tests that the DTL currently fails.
// Note: tests are not complete! Only serve as a starting point to demonstrate
// DTL's limitations.
//
// #define TEST_VALIDATE_ONLY_ONCE

struct tiv
{
   tiv() {} // required for DBView - it binds to dummy objects.
   tiv( int x ) : value(x) {}
   int value;
   friend bool operator==(tiv const& a, tiv const& b) { return a.value == b.value; }
};

typedef dtl::tcstring<10> s_tiv;

// Build a custom class that must only have ins_validate called once,
// and it must be called before its compared.
BEGIN_DTL_NAMESPACE

// Non-intrusive functions to allow binding of this class in DTL

struct tiv_validate
{

	static bool ins_validate(BoundIOs & DTL_DUMMY_PARAM_1, BoundIOs::UserValidate *val) 
	{
      cout << "tiv_ins_validate() called" << endl;
	   tiv *px = reinterpret_cast<tiv*>(val->src_class);
      // modifies the value, to test DTL's sanity
#ifdef TEST_VALIDATE_ONLY_ONCE
      px->value *= 2;
#endif
      cout << "tiv::ins_validate == " << px->value << endl;
      // will always return true
	   return true;
	}

	static bool sel_validate(BoundIOs & DTL_DUMMY_PARAM_1, BoundIOs::UserValidate *val) 
	{
      cout << "tiv_sel_validate() called" << endl;
	   return true;
	}
};


BoundIO &tiv_bind(BoundIO & b, tiv & x) 
{
  BoundIOs *pbios = b.GetBoundIOsPtr();
  tstring name = b.GetName();
  STD_::vector<tstring> cols;
  cols.push_back(name);
  BoundIOs::UserValidate uv(reinterpret_cast<BYTE*>(&x), tiv_validate::ins_validate, tiv_validate::sel_validate, cols);
  pbios->AddUserValidate(uv);

  return (*pbios)[name];
}

BoundIO operator== (BoundIO & b, tiv & x) 
{
   return tiv_bind(b,x) == x.value; 
}

END_DTL_NAMESPACE


using namespace dtl;
using namespace std;


enum SetTest_E { c1, c2 };
DEFINE_DTL_SET(SetTest,2,6);
INIT_DTL_SET(SetTest) = {"c1","c2"};


namespace dtl {

   template <>
   struct DefaultBCA<tiv>
   {
     void operator()(BoundIOs & boundIOs, tiv & rowbuf)
     {
        boundIOs[_TEXT("INT_VALUE")] == rowbuf;
     }
   };

   template <>
   struct DefaultBCA<s_tiv>
   {
     void operator()(BoundIOs & boundIOs, s_tiv & rowbuf)
     {
        boundIOs[_TEXT("STRING_VALUE")] == rowbuf;
     }
   };

   template <>
   struct DefaultBCA<SetTest>
   {
     void operator()(BoundIOs & boundIOs, SetTest & rowbuf)
     {
        boundIOs[_TEXT("VALUE")] == rowbuf;
     }
   };

}



void TestInsValidate()
{
	try {
      // now test the string case

      cout << "Load database with 'NULL'" << endl;
		DBStmt(_TEXT("delete from DB_EXAMPLE")).Execute();
		DBStmt(_TEXT("insert into db_example (string_value) values('hi')")).Execute();
      cout << "Testing with tcstring<>" << endl;
		DBView<s_tiv> s_view(_TEXT("DB_EXAMPLE"));
      cout << "Create iterator" << endl;
		DBView<s_tiv>::select_update_iterator s_it = s_view;
      cout << "About to read 'hi'" << endl;
		s_tiv s_row(*s_it);
      assert(s_row == "hi");
      cout << "About to write the same ('hi')" << endl;
      *s_it = s_row;
      cout << "About to write a different value ('gday')" << endl;
      s_tiv s_row2("gday");
		*s_it = s_row2;
      cout << "Rebuilding iterator, to force a re-read" << endl;
      s_it = s_view;
      cout << "About to write a different value ('aloha')" << endl;
      s_row2 = "aloha";
      s_tiv s_row3 = "aloha";
		*s_it = s_row2;
      // check operator==(), it was broken on mine after some fixes...
      assert(s_row2 == s_row3);
      // DOES NOT COMPILE!!! if (*s_it == s_row2)
      // this requires a static_cast.  Why?
      if (static_cast<s_tiv const &>(*s_it) == s_row2)
         ; // do nothing
      cout << "About to write a different value ('ciao') without a rebuild" << endl;
      s_row2 = "ciao";
      try 
      {
         *s_it = s_row2;
         cout << "SHOULD HAVE FAILED!" << endl;
         assert(0);
      } 
      catch ( dtl::DBException & )
      {
         cout << "Failed, as expected" << endl;
      }
      cout << "Done" << endl;



		DBStmt(_TEXT("delete from test_set")).Execute();
      // this will fill it with the default 'c1' value - as 
      // specified in the tables definition.
		DBStmt(_TEXT("insert into test_set () values()")).Execute();

      cout << "Testing with dtl_set<>" << endl;
		DBView<SetTest> ds_view(_TEXT("test_set"));
      cout << "Create iterator" << endl;
		DBView<SetTest>::select_update_iterator ds_it = ds_view;
      cout << "About to read 'c1'" << endl;
		SetTest ds_row(*ds_it);
      assert(ds_row[c1] && !ds_row[c2]);
      cout << "About to write the same ('c1')" << endl;
      // *ds_it = ds_row;
      cout << "About to write a different value ('c2')" << endl;
      SetTest ds_row_2;
      ds_row_2[c2] = true;
		*ds_it = ds_row_2;
      cout << "Reading back" << endl;
      ds_it = ds_view;
      SetTest check2 = *ds_it;
      assert(!check2[c1] && check2[c2]);
      cout << "About to write a different value ('')" << endl;
      SetTest ds_empty;
		*ds_it = ds_empty;
      cout << "Reading back" << endl;
      ds_it = ds_view;
      ds_empty = *ds_it;
      assert(!ds_empty[c1] && !ds_empty[c2]);
      cout << "About to write a different value ('c1,c2')" << endl;
      SetTest ds_full;
      ds_full[c1] = ds_full[c2] = true;
      assert(ds_full[c1] && ds_full[c2]);
		*ds_it = ds_full;
      cout << "Reading back" << endl;
      ds_it = ds_view;
      ds_full = *ds_it;
      assert(ds_full[c1] && ds_full[c2]);
		

      cout << "Load database with '1'" << endl;
		DBStmt(_TEXT("delete from DB_EXAMPLE")).Execute();
		DBStmt(_TEXT("insert into db_example (int_value) values(1)")).Execute();
		
      cout << "Testing with struct { int; };" << endl;
		DBView<tiv> view(_TEXT("DB_EXAMPLE"));
      cout << "Create iterator" << endl;
		DBView<tiv>::select_update_iterator it = view;
      cout << "About to read '1'" << endl;
		tiv row(*it);
      assert(row == 1);
      cout << "About to write the same (1)" << endl;
      *it = row;
		row = *it;
#ifdef TEST_VALIDATE_ONLY_ONCE
      cout << "Reading back, it should now be 2 (because of ins_validate)" << endl;
      assert(row == 2);
#else
      cout << "Reading back, it should be still 1" << endl;
      assert(row == 1);
#endif
      cout << "About to write a different value (2)" << endl;
      tiv row2(2);
		*it = row2;
      cout << "Rebuilding iterator, to force a re-read" << endl;
      it = view;
      cout << "About to write a different value (8)" << endl;
      row2.value = 8;
		*it = row2;
      // check operator==(), it was broken on mine after some fixes...
      // this uses tiv's friend operator==(,)
      if (*it == row2)
         ; // do nothing
      // this should use tiv's constructor
      if (*it == 8)
         ; // do nothing
      cout << "About to write a different value (5) without a rebuild" << endl;
      row2.value = 5;
      try 
      {
         *it = row2;
         cout << "SHOULD HAVE FAILED!" << endl;
         assert(0);
      } 
      catch ( dtl::DBException & )
      {
         cout << "Failed, as expected" << endl;
      }
      cout << "Done" << endl;



      cout << "Done" << endl;
		DBConnection::GetDefaultConnection().CommitAll();
	}
	catch (std::exception &ex) 	{
		std::cout << ex.what() << std::endl;
	}
}

