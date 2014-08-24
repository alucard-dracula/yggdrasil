#include "std_warn_off.h"
#include <iostream>
#include <DTL.h>
#include <nullable.h>
#include "std_warn_on.h"

using namespace dtl;
using namespace std;

namespace dtl {
   template <>
   struct DefaultBCA<size_t>
   {
     void operator()(BoundIOs & boundIOs, size_t & rowbuf)
     {
        boundIOs[_TEXT("INT_VALUE")] == rowbuf;
     }
   };
}

template <class In>
void test_basic_increment(In begin, In end)
{
   assert(begin != end);
   In before = begin;
   ++begin;
   assert(before != begin);
}

template <class In>
void count_dists(In begin, In end, int target)
{
   cout << _TEXT("Checking distance is ") << target;
   int dist = distance(begin,end);
   cout << " counted " << dist << endl;
   assert(dist == target);
   if (begin != end)
   {
      ++begin;
      count_dists(begin,end,target-1);
   }
}


template <class In, class Out, class Compare>
Out copy_duplicates(In begin, In end, Out out, Compare compare)
{
   while (begin != end)
   {
      begin = adjacent_find(begin,end,compare);
      if (begin != end)
      {
         tcout << _TEXT("Found duplicate ") << endl;
         *out++ = *begin++;
      }
   }
   return out;
}


template <class In>
void increment_distance(In begin, In end, size_t num)
{
   size_t original_size = distance(begin,end);
   for (size_t i = 0; i != num; ++i )
      ++begin;
   size_t minus_one = distance(begin,end);
   tcout << _TEXT("Original size: ") << original_size << _TEXT("  Minus ")
	   << num << _TEXT(": ") << minus_one << endl;
   assert(original_size == minus_one+num);
}


void TestIterators()
{
   tcout << _TEXT("Test iterators with ints") << endl;

   {
      DBStmt(_TEXT("delete from DB_EXAMPLE")).Execute();
      DBStmt(_TEXT("insert into db_example (int_value) values ('1'),('2'),('2'),('2'),('3'),('4'),('4')")).Execute();

      DBView<size_t> plain_view(_TEXT("DB_EXAMPLE"));
      RandomDBView<size_t> random_view(plain_view);

      // KNOWN NOT TO WORK WITH DBView
      // this is by design, the DBView::select_iterators are 
      // INPUT iterators, so they are not intended to be used in this manner.
      tcout << _TEXT("Testing basic increment") << endl;
      test_basic_increment(random_view.begin(),random_view.end());

      tcout << _TEXT("Testing distance() and increment") << endl;
      count_dists(random_view.begin(),random_view.end(),7);

      tcout << _TEXT("Testing an increment, then distance(). Checking the case for 1.") << endl;
      increment_distance(random_view.begin(),random_view.end(),1);

      tcout << _TEXT("Testing an increment, then distance(). Checking the case for 2.") << endl;
      increment_distance(random_view.begin(),random_view.end(),2);

      {
         tcout << _TEXT("Test copy_duplicates() with vector<>") << endl;
         vector<size_t> cont(plain_view.begin(),plain_view.end());
         vector<size_t> dups;
         copy_duplicates(cont.begin(),cont.end(),back_inserter(dups),std::equal_to<size_t>());
         assert(dups.size() == 3);
         assert(dups[0] == 2);
         assert(dups[1] == 2);
         assert(dups[2] == 4);
      }

      {
         tcout << _TEXT("Test copy_duplicates() with RandomDBView<>") << endl;
         vector<size_t> dups;
         copy_duplicates(random_view.begin(),random_view.end(),back_inserter(dups),std::equal_to<size_t>());
         assert(dups.size() == 3);
         assert(dups[0] == 2);
         assert(dups[1] == 2);
         assert(dups[2] == 4);
      }
   }

   DBConnection::GetDefaultConnection().CommitAll();
}

