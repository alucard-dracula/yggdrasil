#ifndef _ITER_SEMANTICS_TEST_H
#define _ITER_SEMANTICS_TEST_H

#include "Example.h"

void VariantRowBPAExample(BoundIOs &boundIOs, variant_row &rowbuf);

// tests the assigning of params (as a variant_row) to iterator before iterator is open
void TestParamsBeforeOpen();

// tests iterator assignment to make sure
// that changing of params on one iterator doesn't effect params of other
void TestIterEffectsOnOtherIter();

// tests iterator copy ... std::copy() algorithm should work on copies of

// DB_iterators ... such as:
// DBView<Example>::select_iterator = view.begin();
// copy(it, view.end(), tostream_iterator<Example, TCHAR>(tcout, _TEXT("\n"));
void TestIterCopy();

// tests iterator copy ... copy iterator b to a, increment a, and make sure
// dummy != dummy2
void TestIterCopyIncrDest();

// test various cases for select iterators for the invariant
// i == j iff &*i == &*j
void TestSelectIteratorInvariants();

// we get the proper 2 record difference back from TableDiff()
// on this test of select_iterator assignment
void TestSelectIteratorAssignment();

#endif

