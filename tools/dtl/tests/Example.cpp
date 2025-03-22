// Example client code for a single table
// Initial: 9/5/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 12/20/2000 - MG - added namespace support

#include "Example.h"

#include "IndexedDBView.h"

#include "DynaDBView.h"

#include "select_iterator.h"

#include "sql_iterator.h"

#include "select_update_iterator.h"
#include "insert_iterator.h"

#include "LocalBCA.h"


#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <string>

#ifdef __SGI_STL_PORT
#include <hash_set>
#include <hash_map>
#endif

using namespace dtl;
using namespace std;


const TIMESTAMP_STRUCT then = {2000, 12, 15, 0, 0, 0, 0};
const TIMESTAMP_STRUCT past = {1999, 1, 1, 0, 0, 0, 0};

// set parameters function for Example ... used by IndexedDBView<Example>
void SetParamsExample(DefaultParamObj<Example> &params)
{
	// set parameter values
	params.lowIntValue = 2;
	params.highIntValue = 8;
	params.strValue = _TEXT("Example");

	TIMESTAMP_STRUCT paramDate = {2000, 1, 1, 0, 0, 0, 0};
	params.dateValue = paramDate;
}

// read the contents of a single table and return a vector the resulting rows
vector<Example> SimpleRead() {
	vector<Example> results;
	DBView<Example> view(_TEXT("DB_EXAMPLE"));

	back_insert_iterator<vector<Example> > i(results);
	copy(view.begin(), view.end(), i);
	return results;
}

// simple read with an aliased view
vector<Example> SimpleAliasedRead()
{
	vector<Example> results;
	DBView<Example> view(_TEXT("DB_EXAMPLE EXAMPLE"), AliasedBCAExample(),
		exampleOrderBy);

	back_insert_iterator<vector<Example> > i(results);
	copy(view.begin(), view.end(), i);
	return results;
}

// Read the contents of a table and print the resulting rows
vector<variant_row> SimpleDynamicRead() {
	vector<variant_row> results;

	// Our query will be "SELECT * FROM DB_EXAMPLE"
	DynamicDBView<> view(_TEXT("DB_EXAMPLE"), _TEXT("*"), exampleOrderBy);

	// NOTE: We need to construct r from the view itself since we
	// don't know what fields the table will contain.
	// We therefore make a call to the DataObj() function to have the
	// table return us a template row with the correct number of fields
	// and field types.
	// We use this construction since we can't be guaranteed that the table
	// is non-empty & we want to still display column names in this case.
	variant_row s(view.GetDataObj());

	// Print out the column names
	vector<tstring> colNames = s.GetNames();
	for (vector<tstring>::iterator name_it = colNames.begin(); name_it != colNames.end(); name_it++)
	{
		tcout << (*name_it) << _TEXT(" ");
	}
	tcout << endl;

	// Print out all rows and columns from our query
	DynamicDBView<>::select_iterator print_it;
	for (print_it = view.begin(); print_it != view.end(); print_it++)
	{
		variant_row r = *print_it;
#if 0
		for (size_t i = 0; i < r.size(); i++)
		{
#if !defined(DTL_OUTPUT_ROWIDS)
		  if ((r.GetNames())[i].c_str() == tstring(_TEXT("ROWID")))
		  {
			  tcout << _TEXT("#### ROWID #### ");
			  break;
		  }
#endif
			tcout << r[i] << _TEXT(" ");
		}
#endif
		tcout << r << endl;
		results.push_back(r);
	}

	return results;
}

// Insert two rows into a table with unknown fields
void SimpleDynamicWrite() {
	TIMESTAMP_STRUCT paramDate = {2012, 12, 23, 0, 0, 0, 0};
	// Mayan DOOMSDAY! End of the Mayan 5126 year long calendar cycle starting from May 1, 3094 B.C.
	// Date is 13.13.13.0.0.0.0  4 Ahaw, 3 K'ank'in

	DynamicDBView<> view(_TEXT("DB_EXAMPLE"), _TEXT("*"), exampleOrderBy);

	DynamicDBView<>::insert_iterator write_it;
	write_it = view;

	// NOTE: We need to construct r from the view itself since we
	// don't know what fields the table will contain.
	// We therefore make a call to the DataObj() function to have the
	// table return us a template row with the correct number of fields
	// and field types.
	variant_row r(view.GetDataObj());

	// Prepare the number of the beast!
	// Set all fields to the value 6,
	// except for the last column which is a date and cannot
	// currently accept numeric values

	size_t i = 0, x = 0;

	for (i = 0, x = r.size(); i < x; ++i)
	  if (r[i].type() == C_TIMESTAMP) r[i] = paramDate;
	  else r[i] = 6;

	// insert the number
	*write_it = r;
	write_it++;

	// Prepare the number of angels who stand before
	// the throne of God!
	// Set all fields to the value 7,
	// except for the last column which is a date and cannot
	// currently accept numeric values

	for (i = 0, x = r.size(); i < x; ++i)
	  if (r[i].type() == C_TIMESTAMP) r[i] = paramDate;
	  else r[i] = 7;

	// insert the number
	*write_it = r;
	write_it++;
}


// read some Example objects from the database and return a vector of
// the results
vector<Example> ReadData()
{
	vector<Example> results;

	// construct view
	// DBView<Example> is actually DBView<Example,
    // DefaultParamObj<Example> > thanks to the default argument to the DBView
	// template

	DBView<Example>
		view(_TEXT("DB_EXAMPLE"), BCAExampleObj(),
		_TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE < (?)") + exampleOrderBy,
		BPAExampleObj());

	// loop through query results and add them to our vector
	// in this loop, read_it.GetLastCount() records read from DB

	DBView<Example>::select_iterator read_it = view.begin();

	// set parameter values for the WHERE clause in our SQL query
	read_it.Params().lowIntValue = 2;
	read_it.Params().highIntValue = 8;
	read_it.Params().strValue = _TEXT("Example");

	TIMESTAMP_STRUCT paramDate = {2000, 1, 1, 0, 0, 0, 0};
	read_it.Params().dateValue = paramDate;

	for ( ; read_it != view.end();  read_it++)
	{
		// note that the read_iterator::GetCount()  is incremented in operator++()
		// remember that the record is fetched and thus the count incremented
		// before operator*() is applied to the read_iterator

		tcout << _TEXT("Reading element #") << read_it.GetCount() << endl;
		results.push_back(*read_it);

		tcout << _TEXT("read_it->exampleInt = ") << read_it->exampleInt << endl;
		tcout << _TEXT("read_it->exampleStr = ") << read_it->exampleStr << endl;

	}

	return results;
}

// print out Example objects directly from a DBView
void PrintExamplesFromView()
{
    DBView<Example> view((DBView<Example>::Args().tables(_TEXT("DB_EXAMPLE")).postfix(exampleOrderBy)));
	copy(view.begin(), view.end(),
		tostream_iterator<Example, TCHAR>(tcout, _TEXT("\n")));
}

// write some Example objects to the database
void WriteData(const vector<Example> &examples)
{
	// construct view ... note: no BPA as there is no postfix clause
	DBView<Example> view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj());

	// loop through vector and write Example objects  to DB
	// write_it.GetLastCount() records written in loop

	DBView<Example>::insert_iterator write_it = view;

	// note initialization of query parameters done in BPA for BoundParams
	// and in operator*() for BoundCols
	for (vector<Example>::const_iterator ex_it = examples.begin(); ex_it != examples.end(); ex_it++, write_it++)
	{
		*write_it = *ex_it;

	    // note that write_iterator::GetCount() is the # of records actually written
	    // so far ... which is not registered until after operator++()
	    // hence the need to add 1 to the result of write_it.GetCount()
	    tcout << _TEXT("Writing element #") << write_it.GetCount() + 1<< endl;
	}


}

void SimpleUpdateData()
{
  // construct view
#if 0
  DBView<Example>
    view(_TEXT("DB_EXAMPLE"),
	 BCAExampleObj ( ),
	 _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
	 _TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE = (?)"),
	 BPAExampleObj ( ));
#else
  DBView<Example>
    view (_TEXT("DB_EXAMPLE"), SimpleExampleBCA ( ), _TEXT(""),
	  SimpleExampleBPA ( ));
#endif

  // build an updater for the view
  DBView<Example>::update_iterator exampleUpdater = view;

  TIMESTAMP_STRUCT today = { 2000, 9, 29, 0, 0, 0, 0 };

  *exampleUpdater = Example
    (2121, _TEXT("Dereferenced"), 99.99, 25, today);
  exampleUpdater++;

  tcout << exampleUpdater.GetLastCount() << _TEXT(" rows updated!") << endl;

  // now can perform other updates using the same updater object
  // make sure to put in your new values for both the data and parameter values
  // for the update
  // set data fields we want to update to their desired values
  // exampleStr to _TEXT("Second Update") and exampleLong to 66

  TIMESTAMP_STRUCT tomorrow = {2000, 9, 30, 0, 0, 0, 0};

  *exampleUpdater = Example
    (2222, _TEXT("Second Update"), 0.11111, 66, tomorrow);

  // execute the update
  exampleUpdater++;

  tcout << exampleUpdater.GetLastCount() << _TEXT(" rows updated!") << endl;
}

// update Example object (with new values)  meeting a query in the database
void UpdateData()
{
	// construct view
	DBView<Example> view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE = (?)"), BPAExampleObj());

	// build an updater for the view
	DBView<Example>::update_iterator
		exampleUpdater;

	exampleUpdater = view;

	// now set the parameters indicating which rows we want the update applied
	exampleUpdater.Params().lowIntValue = 5;
	exampleUpdater.Params().highIntValue = 13;
	exampleUpdater.Params().strValue = _TEXT("Find Me");

	TIMESTAMP_STRUCT paramDate = {1999, 11, 11, 0, 0, 0, 0};
	exampleUpdater.Params().dateValue = paramDate;

	// set data fields we want to update to their desired values
	// exampleStr to _TEXT("Updated") and sampleLong to 0
	Example updateMe;

	updateMe.exampleStr = _TEXT("Updated");
	updateMe.exampleLong = 25;

	TIMESTAMP_STRUCT today = {2000, 9, 29, 0, 0, 0, 0};

	updateMe = Example(2121, _TEXT("Dereferenced"), 99.99, 25, today);

	*exampleUpdater = updateMe;


	// execute the update
	exampleUpdater++;

	tcout << exampleUpdater.GetLastCount() << _TEXT(" rows updated!") << endl;

	// now can perform other updates using the same updater object
	// make sure to put in your new values for both the data and parameter values
	// for the update
	// set data fields we want to update to their desired values
	// exampleStr to _TEXT("Second Update") and exampleLong to 66

	// now set the parameters indicating which rows we want the update applied
	exampleUpdater.Params().lowIntValue = 21;
	exampleUpdater.Params().highIntValue = 30;
	exampleUpdater.Params().strValue = _TEXT("To find");

	TIMESTAMP_STRUCT otherParamDate = {2001, 10, 31, 0, 0, 0, 0};
    exampleUpdater.Params().dateValue = otherParamDate;

	TIMESTAMP_STRUCT tomorrow = {2000, 9, 30, 0, 0, 0, 0};
	updateMe = Example(2222, _TEXT("Second Update"), 0.11111, 66, tomorrow);
	updateMe.exampleLong = 66;

	*exampleUpdater = updateMe;



	// execute the update
	exampleUpdater++;

	tcout << exampleUpdater.GetLastCount() << _TEXT(" rows updated!") << endl;
}

// delete some Examples from the database
void DeleteData()
{
	// construct view
	DBView<Example>
		view(_TEXT("DB_EXAMPLE"),
		DelBCAExampleObj(), _TEXT("OR STRING_VALUE = (?) OR INT_VALUE = (?)"), DelBPAExampleObj());

	// build a deleter for the view
	DBView<Example>::delete_iterator exampleDeleter;

	exampleDeleter = view;

	// now set the parameters indicating which rows we want to delete
	TIMESTAMP_STRUCT y2k = {2000, 1, 1, 0, 0, 0, 0};

	*exampleDeleter = Example(0, _TEXT("Example"), 0.0, 0, y2k);

	exampleDeleter.Params().strValue = _TEXT("Find Me");
	exampleDeleter.Params().lowIntValue = 18;
	// execute the delete
	exampleDeleter++;

	tcout << exampleDeleter.GetLastCount() << _TEXT(" rows deleted!") << endl;

	// now can perform other deletes using the same deleter object
	// make sure to put in your new values for the parameters
	// for the delete

	// now set the parameters indicating which rows we want to delete

	TIMESTAMP_STRUCT today = {1999, 9, 29, 0, 0, 0, 0};
	*exampleDeleter = Example(3, _TEXT("operator->() works"), 0.0, 0, today);

	// execute the delete
	exampleDeleter++;

	tcout << exampleDeleter.GetLastCount() << _TEXT(" rows deleted!") << endl;
}

// Dynamic IndexedDBView example
void DynamicIndexedViewExample()
{
  typedef DynamicDBView<>               DynaDBV;
  typedef DEFAULT_DYNA_VIEW(DynaDBV)    DynaIdxDBV;

	DynaDBV dynamic_view(
		DynaDBV::Args().tables(_TEXT("DB_EXAMPLE")).fields(
		  _TEXT("INT_VALUE, STRING_VALUE, DOUBLE_VALUE, EXAMPLE_LONG, EXAMPLE_DATE")
		).key_mode(USE_AUTO_KEY).postfix(exampleOrderBy)
	);

	DynaIdxDBV indexed_view(dynamic_view,
		_TEXT("IndexString; STRING_VALUE;")
		_TEXT("Unique IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"), BOUND, USE_AUTO_KEY);

	// Find the item where the STRING_VALUE matches the tstring _TEXT("Foozle")
	DynaIdxDBV::indexed_iterator idxview_it = indexed_view.find(tstring(_TEXT("Foozle")));;


	// Update the item with the key of _TEXT("Foozle"), to read _TEXT("Fizzle") instead
	if (idxview_it != indexed_view.end()) {
		variant_row replacement;
		replacement = *idxview_it;
		replacement[_TEXT("STRING_VALUE")] = tstring(_TEXT("Fizzle"));
		replacement[_TEXT("DOUBLE_VALUE")] = tstring(_TEXT("44.5"));
		tcout << _TEXT("Replacing: ") << endl;
		tcout << *idxview_it << endl;
		tcout << _TEXT("with: ") << endl;
		tcout << replacement << endl;
		indexed_view.replace(idxview_it, replacement);
	}

	// Now find a second set of items using the alternate index, IndexLongDate
	// The STL convention for equal_range is that the return value consists of a pair:
	// 1. an iterator referring to the beginning of the list of found items
	// and 2. an iterator pointing to the end of the list of found items
	// We will remove all DataObj's in this range
	const TIMESTAMP_STRUCT ts_date_criteria = {2000, 1, 1, 0, 0, 0, 0};
	// Unused.
	// const jtime_c date_criteria(ts_date_criteria);

	long long_criteria = 33;
	DynaIdxDBV::indexed_pair 	pr = indexed_view.equal_range_AK(_TEXT("IndexLongDate"), long_criteria, ts_date_criteria);

	idxview_it = pr.first;

	tcout << _TEXT("*** Size before erase calls: ") << indexed_view.size()
		  << _TEXT(" ***") << endl;

	// Remove all DataObj that matched the criteria in our equal_range_AK lookup
	while (idxview_it != pr.second)
	{
		// as iterator is invalidated upon an erase(), use a temporary iterator
		// to point to DataObj to erase
		// increment idxview_it before we erase so it will still be valid
		// when we erase the DataObj
		DynaIdxDBV::indexed_iterator deleteMe = idxview_it;

		idxview_it++;

		indexed_view.erase(deleteMe);

	}

	tcout << _TEXT("*** Size after erase calls: ") << indexed_view.size()
		  << _TEXT(" ***") << endl;

	// Finally, insert a new item into the container
	pair<DynaIdxDBV::iterator, bool> ins_pr;
	variant_row r(indexed_view.GetDataObj());
	r[_TEXT("INT_VALUE")] = 459;
	r[_TEXT("STRING_VALUE")] = tstring(_TEXT("Unique String #1"));
	r[_TEXT("DOUBLE_VALUE")] = 3.5;
	r[_TEXT("EXAMPLE_LONG")] = 1;
	r[_TEXT("EXAMPLE_DATE")] = ts_date_criteria;
	ins_pr = indexed_view.insert(r);
	tcout << _TEXT("insertion succeded = ") << (ins_pr.second == true ? _TEXT("true"): _TEXT("false")) << endl;

}

// Example of using an IndexDBView to read, insert and update records
void IndexedViewExample()
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV  view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) OR ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

    IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; Unique IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		BOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));


	// Find the item where the STRING_VALUE matches the tstring _TEXT("Foozle")
	IdxDBV::indexed_iterator idxview_it = indexed_view.find(tstring(_TEXT("Foozle")));;


	// Update the item with the key of _TEXT("Foozle"), to read _TEXT("Fizzle") instead
	if (idxview_it != indexed_view.end()) {
		Example replacement;
		replacement = *idxview_it;
		replacement.exampleStr = _TEXT("Fizzle");
		indexed_view.replace(idxview_it, replacement);
	}

	// Now find a second set of items using the alternate index, IndexLongDate
	// The STL convention for equal_range is that the return value consists of a pair:
	// 1. an iterator referring to the beginning of the list of found items
	// and 2. an iterator pointing to the end of the list of found items
	// We will remove all DataObj's in this range
	const TIMESTAMP_STRUCT date_criteria = {2000, 1, 1, 0, 0, 0, 0};
	long long_criteria = 33;
	IdxDBV::indexed_pair 	pr = indexed_view.equal_range_AK(_TEXT("IndexLongDate"), long_criteria, date_criteria);

	idxview_it = pr.first;

	tcout << _TEXT("*** Size before erase calls: ") << indexed_view.size() << _TEXT(" ***")
		<< endl;

	// Remove all DataObj that matched the criteria in our equal_range_AK lookup
	while (idxview_it != pr.second)
	{
		// as iterator is invalidated upon an erase(), use a temporary iterator
		// to point to DataObj to erase
		// increment idxview_it before we erase so it will still be valid
		// when we erase the DataObj
		IdxDBV::indexed_iterator deleteMe = idxview_it;

		idxview_it++;

		indexed_view.erase(deleteMe);

	}

	tcout << _TEXT("*** Size after erase calls: ") << indexed_view.size() << _TEXT(" ***")
		<< endl;


	// Finally, insert a new item into the container
	pair<IdxDBV::iterator, bool> ins_pr;
	ins_pr = indexed_view.insert(Example(459, _TEXT("Unique String #1"), 3.4, 1, date_criteria));
	tcout << _TEXT("insertion succeded = ") << (ins_pr.second == true ? _TEXT("true"): _TEXT("false")) << endl;
}

// read Examples from indexed view to make sure data gets in IndexedDBView's
// internal list<DataObj> properly
vector<Example> ReadDataFromIndexedView()
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

	//BCAExampleObj
	//DefaultBCA<Example>, BCAExample
	DBV	view(_TEXT("DB_EXAMPLE"),
		DefaultBCA<Example>() , _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list
    IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	vector<Example> results_from_indexed_view;

	// populate results vector with the data from the indexed view
	for (IdxDBV::iterator data_it = indexed_view.begin(); data_it != indexed_view.end(); data_it++)
	{
		results_from_indexed_view.push_back(*data_it);
	}

	tcout << _TEXT("*** IndexedDBView says size is: ") << indexed_view.size() << _TEXT(" ***") << endl;
	tcout << _TEXT("*** Results say size is: ") << results_from_indexed_view.size() << _TEXT(" ***") << endl;

	// test instantiation of comparison operators
	tcout << _TEXT("indexed_view == indexed_view ") << (indexed_view == indexed_view) << endl;
	tcout << _TEXT("indexed_view != indexed_view ") << (indexed_view != indexed_view) << endl;
//	tcout << _TEXT("indexed_view <  indexed_view ") << (indexed_view <  indexed_view) << endl;
//	tcout << _TEXT("indexed_view >  indexed_view ") << (indexed_view >  indexed_view) << endl;
//	tcout << _TEXT("indexed_view <= indexed_view ") << (indexed_view <= indexed_view) << endl;
//	tcout << _TEXT("indexed_view >= indexed_view ") << (indexed_view >= indexed_view) << endl;

	return results_from_indexed_view;
}

#ifdef __SGI_STL_PORT
// read Examples from hashed indexed view to make sure data gets in IndexedDBView's
// internal list<DataObj> properly
vector<Example> ReadDataFromHashedIndexedView()
{
  typedef eq_functor<Example> Compare;
  typedef hash_functor<Example> Hash;
  typedef hash_multiset<Example *, Hash, Compare> HashMS;

  typedef DBView<Example> DBV;
  typedef IndexedDBView <DBV, HashMS, HASH> IdxDBV;

	//BCAExampleObj
	//DefaultBCA<Example>, BCAExample
	DBV	view(_TEXT("DB_EXAMPLE"),
		DefaultBCA<Example>() , _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());


	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list
    IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	vector<Example> results_from_indexed_view;

	// populate results vector with the data from the indexed view
	for (IdxDBV::iterator data_it = indexed_view.begin(); data_it != indexed_view.end(); data_it++)
	{
		results_from_indexed_view.push_back(*data_it);
	}

	tcout << _TEXT("*** IndexedDBView says size is: ") << indexed_view.size() << _TEXT(" ***") << endl;
	tcout << _TEXT("*** Results say size is: ") << results_from_indexed_view.size() << _TEXT(" ***") << endl;

	// test instantiation of comparison operators
	tcout << _TEXT("indexed_view == indexed_view ") << (indexed_view == indexed_view) << endl;
	tcout << _TEXT("indexed_view != indexed_view ") << (indexed_view != indexed_view) << endl;
//	tcout << _TEXT("indexed_view <  indexed_view ") << (indexed_view <  indexed_view) << endl;
//	tcout << _TEXT("indexed_view >  indexed_view ") << (indexed_view >  indexed_view) << endl;
//	tcout << _TEXT("indexed_view <= indexed_view ") << (indexed_view <= indexed_view) << endl;
//	tcout << _TEXT("indexed_view >= indexed_view ") << (indexed_view >= indexed_view) << endl;

	return results_from_indexed_view;
}
#endif

// read Examples from indexed view that have the given DataObj as (primary) key
vector<Example> ReadFilteredDataPK(Example &key)
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy);

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list
    IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	vector<Example> filtered_results;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	IdxDBV::indexed_pair pr =	indexed_view.equal_range(key);

	IdxDBV::indexed_iterator idx_data_it =	pr.first;

	IdxDBV::indexed_iterator end_it = pr.second;

	IdxDBV::indexed_iterator find_it = indexed_view.find(key);

	if (idx_data_it == find_it)
		tcout << _TEXT("Iterator integrity satisfied!") << endl;

	// need to be able to iterate through found results
	for ( ; idx_data_it != end_it; idx_data_it++)
	{
		// first * dereferences the iterator, returning a DataObj *
		// second * dereferences the DataObj *, yielding a DataObj
		const Example &idx_data = *idx_data_it;
		filtered_results.push_back(idx_data);
	}

	tcout << filtered_results.size() << _TEXT(" results found!") << endl;
	return filtered_results;

}

// read Example objects using the index IndexString by name
vector<Example> ReadFilteredDataByName(Example &key)
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		DefaultBCA<Example>(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list

    IdxDBV	swap_test(view,
		_TEXT("IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE; IndexString; STRING_VALUE"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));
	IdxDBV	indexed_view;

	indexed_view.swap(swap_test);


	vector<Example> filtered_results;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	IdxDBV::indexed_pair pr =	indexed_view.equal_range_AK(_TEXT("IndexString"), key);

	IdxDBV::indexed_iterator 	idx_data_it = pr.first;

	IdxDBV::indexed_iterator 	end_it = pr.second;

	IdxDBV::indexed_iterator 	find_it = indexed_view.find_AK(_TEXT("IndexString"), key);

	if (idx_data_it == find_it)
		tcout << _TEXT("Iterator integrity satisfied!") << endl;

	// need to be able to iterate through found results
	for ( ; idx_data_it != end_it; idx_data_it++)
	{
		// first * dereferences the iterator, returning a DataObj *
		// second * dereferences the DataObj *, yielding a DataObj
		const Example &idx_data = *idx_data_it;
		filtered_results.push_back(idx_data);
	}

	tcout << _TEXT("Swap test!") << endl;
	indexed_view.swap(swap_test);

	pr = swap_test.equal_range_AK(_TEXT("IndexString"), key);

	idx_data_it = pr.first;
	end_it = pr.second;
	find_it = swap_test.find_AK(_TEXT("IndexString"), key);

	if (idx_data_it == find_it)
		tcout << _TEXT("Iterator integrity satisfied!") << endl;

	vector<Example> swap_results;

	// need to be able to iterate through found results
	for ( ; idx_data_it != end_it; idx_data_it++)
	{
		// first * dereferences the iterator, returning a DataObj *
		// second * dereferences the DataObj *, yielding a DataObj
		const Example &idx_data = *idx_data_it;
		swap_results.push_back(idx_data);
	}

	if (filtered_results == swap_results)
		tcout << _TEXT("Swap test successful!!!!") << endl;

	tcout << filtered_results.size() << _TEXT(" results found!") << endl;
	return filtered_results;

}

// read Example objects using the index IndexString by name, but using the
// datafield rather than the object
vector<Example> ReadDataIdxByStrUseDF(const tstring &strVal)
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy);

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list
    IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	vector<Example> filtered_results;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	IdxDBV::indexed_pair 	pr = indexed_view.equal_range_AK(_TEXT("IndexString"), strVal);

	IdxDBV::indexed_iterator	idx_data_it = pr.first;

	IdxDBV::indexed_iterator 	end_it = pr.second;

	IdxDBV::indexed_iterator 	find_it = indexed_view.find_AK(_TEXT("IndexString"), strVal);

	if (idx_data_it == find_it)
		tcout << _TEXT("Iterator integrity satisfied!") << endl;

	// need to be able to iterate through found results
	for ( ; idx_data_it != end_it; idx_data_it++)
	{
		// first * dereferences the iterator, returning a DataObj *
		// second * dereferences the DataObj *, yielding a DataObj
		const Example &idx_data = *idx_data_it;
		filtered_results.push_back(idx_data);
	}

	tcout << filtered_results.size() << _TEXT(" results found!") << endl;
	return filtered_results;
}

// read Example objects using the tstring as a datafield value for the PK
// using IndexString as PK
vector<Example> ReadDataIdxByStrUseDFAndPK(const tstring &strVal)
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list
	IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	vector<Example> filtered_results;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	IdxDBV::indexed_pair 	pr = indexed_view.equal_range(strVal);

	IdxDBV::indexed_iterator	idx_data_it = pr.first;

	IdxDBV::indexed_iterator 	end_it = pr.second;

	IdxDBV::indexed_iterator 	find_it = indexed_view.find(strVal);

	if (idx_data_it == find_it)
		tcout << _TEXT("Iterator integrity satisfied!") << endl;

	// need to be able to iterate through found results
	for ( ; idx_data_it != end_it; idx_data_it++)
	{
		// first * dereferences the iterator, returning a DataObj *
		// second * dereferences the DataObj *, yielding a DataObj
		const Example &idx_data = *idx_data_it;
		filtered_results.push_back(idx_data);
	}

	tcout << filtered_results.size() << _TEXT(" results found!") << endl;
	return filtered_results;

}

// read Example objects using the tstring and date values as fields in a key
vector<Example> ReadTwoFieldKeyByNameDF(const tstring &strVal, const TIMESTAMP_STRUCT &dateVal)
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list
	IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; IndexTwoFields; STRING_VALUE, EXAMPLE_DATE"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	vector<Example> filtered_results;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	IdxDBV::indexed_pair 	pr = indexed_view.equal_range_AK(_TEXT("IndexTwoFields"), strVal, dateVal);

    IdxDBV::indexed_iterator 	idx_data_it = pr.first;

	IdxDBV::indexed_iterator	end_it = pr.second;

	IdxDBV::indexed_iterator	find_it = indexed_view.find_AK(_TEXT("IndexTwoFields"), strVal, dateVal);

	if (idx_data_it == find_it)
		tcout << _TEXT("Iterator integrity satisfied!") << endl;

	// need to be able to iterate through found results
	for ( ; idx_data_it != end_it; idx_data_it++)
	{
		// first * dereferences the iterator, returning a DataObj *
		// second * dereferences the DataObj *, yielding a DataObj
		const Example &idx_data = *idx_data_it;
		filtered_results.push_back(idx_data);
	}

	tcout << filtered_results.size() << _TEXT(" results found!") << endl;
	return filtered_results;
}

// read Example objects using the passed in
// uses a two field key
vector<Example> ReadTwoFieldKeyByName(Example &key)
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	// set up indexed view with two field index
	IdxDBV	indexed_view(view,
		_TEXT("IndexTwoFields; STRING_VALUE, EXAMPLE_DATE; IndexLong; EXAMPLE_LONG"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	vector<Example> filtered_results;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	IdxDBV::indexed_pair	pr = indexed_view.equal_range_AK(_TEXT("IndexTwoFields"), key);

	IdxDBV::indexed_iterator	idx_data_it = pr.first;

	IdxDBV::indexed_iterator	end_it = pr.second;

	IdxDBV::indexed_iterator	find_it = indexed_view.find_AK(_TEXT("IndexTwoFields"), key);

	if (idx_data_it == find_it)
		tcout << _TEXT("Iterator integrity satisfied!") << endl;

	// need to be able to iterate through found results
	for ( ; idx_data_it != end_it; idx_data_it++)
	{

		// first * dereferences the iterator, returning a DataObj *
		// second * dereferences the DataObj *, yielding a DataObj
		const Example &idx_data = *idx_data_it;
		filtered_results.push_back(idx_data);
	}

	tcout << filtered_results.size() << _TEXT(" results found!") << endl;
	return filtered_results;
}

// read Example objects using the PK
// uses a two field key
vector<Example> ReadTwoFieldKeyPK(Example &key)
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	// set up indexed view with two field index
	IdxDBV	indexed_view(view,
		_TEXT("IndexTwoFields; STRING_VALUE, EXAMPLE_DATE; IndexLong; EXAMPLE_LONG"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	vector<Example> filtered_results;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	IdxDBV::indexed_pair		pr = indexed_view.equal_range(key);

    IdxDBV::indexed_iterator	idx_data_it = pr.first;

	IdxDBV::indexed_iterator	end_it = pr.second;

	IdxDBV::indexed_iterator	find_it = indexed_view.find(key);

	if (idx_data_it == find_it)
		tcout << _TEXT("Iterator integrity satisfied!") << endl;

	// need to be able to iterate through found results
	for ( ; idx_data_it != end_it; idx_data_it++)
	{
		// first * dereferences the iterator, returning a DataObj *
		// second * dereferences the DataObj *, yielding a DataObj
		const Example &idx_data = *idx_data_it;
		filtered_results.push_back(idx_data);
	}

	tcout << filtered_results.size() << _TEXT(" results found!") << endl;
	return filtered_results;
}

// same as above, but as PK
vector<Example> ReadTwoFieldKeyDFAndPK(const tstring &strVal, const TIMESTAMP_STRUCT &dateVal)
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list
	IdxDBV	indexed_view(view,
		_TEXT("IndexTwoFields; STRING_VALUE, EXAMPLE_DATE; IndexLong; EXAMPLE_LONG"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	vector<Example> filtered_results;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	IdxDBV::indexed_pair		pr = indexed_view.equal_range(strVal, dateVal);

	IdxDBV::indexed_iterator	idx_data_it = pr.first;

	IdxDBV::indexed_iterator 	end_it = pr.second;

    IdxDBV::indexed_iterator	find_it = indexed_view.find(strVal, dateVal);

	if (idx_data_it == find_it)
		tcout << _TEXT("Iterator integrity satisfied!") << endl;

	// need to be able to iterate through found results
	for ( ; idx_data_it != end_it; idx_data_it++)
	{
		// first * dereferences the iterator, returning a DataObj *
		// second * dereferences the DataObj *, yielding a DataObj
		const Example &idx_data = *idx_data_it;
		filtered_results.push_back(idx_data);
	}

	tcout << filtered_results.size() << _TEXT(" results found!") << endl;
	return filtered_results;
}

// test the insertion of Example objects into IndexedDBView using *unbound* mode
void TestUnboundInsert()
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		DefaultBCA<Example>(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list

	IdxDBV	indexed_view(view,
		_TEXT("Unique IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE; IndexString; STRING_VALUE;"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));


	// insert some Example objects into the indexed view
	tcout << _TEXT("*** Size before inserts: ") << indexed_view.size() << _TEXT(" ***") << endl;

	const TIMESTAMP_STRUCT date1 = {1993, 2, 28, 0, 0, 0, 0};
	const TIMESTAMP_STRUCT date2 = {1995, 7, 31, 0, 0, 0, 0};
	// Unused.
	// const TIMESTAMP_STRUCT date3 = {1998, 11, 30, 0, 0, 0, 0};

	pair<IdxDBV::iterator, bool> ins_pr;

	ins_pr = indexed_view.insert(Example(459, _TEXT("Unique String #1"), 3.4, 1, date1));
	tcout << _TEXT("insertion result = ") << ins_pr.second << endl;

	ins_pr = indexed_view.insert(Example(444, _TEXT("Unique String #2"), 23.45, 11, date2));
	tcout << _TEXT("insertion result = ") << ins_pr.second << endl;

	ins_pr = indexed_view.insert(Example(411, _TEXT("Unique String #3"), 98.76, 11, date2));
	tcout << _TEXT("insertion result = ") << ins_pr.second << endl;

	ins_pr = indexed_view.insert(Example(1323, _TEXT("Example"), 2.22, 11, date1));
	tcout << _TEXT("insertion result = ") << ins_pr.second << endl;

	tcout << _TEXT("*** Size after inserts: ") << indexed_view.size() << _TEXT(" ***") << endl;

	// clear and refetch to prove we're in unbound mode
	indexed_view.clear();

	tcout << _TEXT("*** Size after refetching: ") << indexed_view.size() << _TEXT(" ***") << endl;
}


void TestUnboundErase()
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	tcout << _TEXT("*** Size before erase calls: ") << indexed_view.size() << _TEXT(" ***")
		<< endl;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	// we'll remove the DataObj's in this range
	IdxDBV::indexed_pair 	pr = indexed_view.equal_range(tstring(_TEXT("Example")));

	IdxDBV::indexed_iterator idxview_it = pr.first;

	// remove the DataObj's in the range
	while (idxview_it != pr.second)
	{
		// as iterator is invalidated upon an erase(), use a temporary iterator
		// to point to DataObj to erase
		// increment idxview_it before we erase so it will still be valid

		// when we erase the DataObj
		IdxDBV::indexed_iterator deleteMe = idxview_it;

		idxview_it++;

		indexed_view.erase(deleteMe);

	}

	tcout << _TEXT("*** Size after erase calls: ") << indexed_view.size() << _TEXT(" ***")
		<< endl;


	// clear and refetch to prove we're in unbound mode
	indexed_view.clear();

	tcout << _TEXT("*** Size after refetching: ") << indexed_view.size() << _TEXT(" ***") << endl;
}

// test the insertion of Example objects into IndexedDBView using *bound* mode
void TestBoundInsert()
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		DefaultBCA<Example>(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list

	IdxDBV	indexed_view(view,
		_TEXT("Unique IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE; IndexString; STRING_VALUE;"),
		BOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));


	// insert some Example objects into the indexed view
	tcout << _TEXT("*** Size before inserts: ") << indexed_view.size() << _TEXT(" ***") << endl;

	const TIMESTAMP_STRUCT date1 = {1993, 2, 28, 0, 0, 0, 0};
	const TIMESTAMP_STRUCT date2 = {1995, 7, 31, 0, 0, 0, 0};
	// Unused.
	// const TIMESTAMP_STRUCT date3 = {1998, 11, 30, 0, 0, 0, 0};

	pair<IdxDBV::iterator, bool> ins_pr;

	ins_pr = indexed_view.insert(Example(459, _TEXT("Unique String #1"), 3.4, 1, date1));
	tcout << _TEXT("insertion result = ") << ins_pr.second << endl;

	ins_pr = indexed_view.insert(Example(444, _TEXT("Unique String #2"), 23.45, 4554, date2));
	tcout << _TEXT("insertion result = ") << ins_pr.second << endl;

	ins_pr = indexed_view.insert(Example(411, _TEXT("Unique String #3"), 98.76, 888, date2));
	tcout << _TEXT("insertion result = ") << ins_pr.second << endl;

	ins_pr = indexed_view.insert(Example(1323, _TEXT("Example"), 2.22, 1116, date1));
	tcout << _TEXT("insertion result = ") << ins_pr.second << endl;

	tcout << _TEXT("*** Size after inserts: ") << indexed_view.size() << _TEXT(" ***") << endl;

	// clear and refetch to prove we're in bound mode
	indexed_view.clear();

	tcout << _TEXT("*** Size after refetching: ") << indexed_view.size() << _TEXT(" ***") << endl;
}

// test the erasing of Example objects from the IndexedDBView using *bound* mode
// using all fields
void TestBoundEraseUAF()
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		BOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	tcout << _TEXT("*** Size before erase calls: ") << indexed_view.size() << _TEXT(" ***")
		<< endl;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	// we'll remove the DataObj's in this range
	IdxDBV::indexed_pair 	 pr = indexed_view.equal_range(tstring(_TEXT("Example")));

	IdxDBV::indexed_iterator idxview_it = pr.first;

	// remove the DataObj's in the range
	while (idxview_it != pr.second)
	{
		// as iterator is invalidated upon an erase(), use a temporary iterator
		// to point to DataObj to erase
		// increment idxview_it before we erase so it will still be valid
		// when we erase the DataObj
		IdxDBV::indexed_iterator deleteMe = idxview_it;

		idxview_it++;

		indexed_view.erase(deleteMe);

	}

	tcout << _TEXT("*** Size after erase calls: ") << indexed_view.size() << _TEXT(" ***")
		<< endl;

	// clear and refetch to prove we're in bound mode
	indexed_view.clear();

	tcout << _TEXT("*** Size after refetching: ") << indexed_view.size() << _TEXT(" ***") << endl;
}

// test the erasing of Example objects from the IndexedDBView using *bound* mode
// using only the PK fields
void TestBoundErasePK()
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		BOUND, USE_PK_FIELDS_ONLY, cb_ptr_fun(SetParamsExample));

	tcout << _TEXT("*** Size before erase calls: ") << indexed_view.size() << _TEXT(" ***")
		<< endl;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	// we'll remove the DataObj's in this range
	IdxDBV::indexed_pair 	 pr = indexed_view.equal_range(tstring(_TEXT("Example")));

	IdxDBV::indexed_iterator idxview_it = pr.first;

	// remove the DataObj's in the range
	while (idxview_it != pr.second)
	{
		// as iterator is invalidated upon an erase(), use a temporary iterator
		// to point to DataObj to erase
		// increment idxview_it before we erase so it will still be valid
		// when we erase the DataObj
		IdxDBV::indexed_iterator deleteMe = idxview_it;

		idxview_it++;

		indexed_view.erase(deleteMe);

	}

	tcout << _TEXT("*** Size after erase calls: ") << indexed_view.size() << _TEXT(" ***")
		<< endl;

	// clear and refetch to prove we're in bound mode
	indexed_view.clear();

	tcout << _TEXT("*** Size after refetching: ") << indexed_view.size() << _TEXT(" ***") << endl;
}

void TestBoundUpdate()
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV view(_TEXT("DB_EXAMPLE"),
		BCAExampleObj(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) OR ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		BPAExampleObj());

	IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; Unique IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		BOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsExample));

	for (IdxDBV::iterator print_it = indexed_view.begin(); print_it != indexed_view.end(); print_it++)
	{
			tcout << print_it->exampleStr << _TEXT(", ");
	}
	tcout << endl;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items
	// we'll remove the DataObj's in this range
#if 0
	IdxDBV::indexed_pair 		pr = indexed_view.equal_range(tstring(_TEXT("Foozle")));

	IdxDBV::indexed_iterator idxview_it = pr.first;
#endif
	IdxDBV::iterator idxview_it =	indexed_view.find(tstring(_TEXT("Bedazzled")));

	// as iterator is invalidated upon an erase(), use a temporary iterator
	// to point to DataObj to erase
	// increment idxview_it before we erase so it will still be valid
	// when we replace the DataObj
    IdxDBV::iterator replaceMe = idxview_it;

	Example replacement;

	replacement = *idxview_it;
	replacement.exampleStr = _TEXT("Befrazzled");
	pair<IdxDBV::iterator, bool> idx_pr =
		indexed_view.replace(idxview_it, replacement);
	replacement.exampleStr = _TEXT("Frazzle");
	replacement.exampleLong = 48;
	// indexed_view.replace(idxview_it, replacement);
	indexed_view.replace(idx_pr.first, replacement);
}

#if 1
void TestDynamicView()
{
  typedef DynamicDBView<>               DynaDBV;
  typedef DEFAULT_DYNA_VIEW(DynaDBV)    DynaIdxDBV;

  	DynaDBV dynamic_view(_TEXT("DB_EXAMPLE"), _TEXT("*"), exampleOrderBy);

	DynaIdxDBV indexed_view(dynamic_view,
		_TEXT("IndexString; STRING_VALUE;"), UNBOUND, USE_ALL_FIELDS);

	DynaIdxDBV::iterator	    print_it = indexed_view.begin();

	for (print_it = indexed_view.begin(); print_it != indexed_view.end(); print_it++)
	{
		variant_row r = *print_it;
		variant_field vf = r[_TEXT("STRING_VALUE")];

    #ifdef __GNUC__     // gcc bug
		tstring str = vf.get_tstring();
		#else
		tstring str = (tstring)vf;
		#endif

		dtl_variant_t v(vf);

    #ifdef __GNUC__     // gcc bug
		tstring s = v.get_tstring();
		#else
		tstring s = (tstring)(v);
    #endif

		tcout << r[_TEXT("INT_VALUE")] << _TEXT(", ");
		tcout << r[_TEXT("STRING_VALUE")] << _TEXT(", ");
		tcout << r[_TEXT("DOUBLE_VALUE")] << _TEXT(", ");
		tcout << r[_TEXT("EXAMPLE_LONG")] << _TEXT(", ");
		tcout << r[_TEXT("EXAMPLE_DATE")];

		tcout << endl;
	}
	tcout << endl;

	print_it = indexed_view.begin();

	vector<tstring> colNames = (*print_it).GetNames();


	for (vector<tstring>::iterator name_it = colNames.begin(); name_it != colNames.end(); name_it++)
	{
		tcout << (*name_it) << _TEXT(" ");
	}
	tcout << endl;

	for (print_it = indexed_view.begin(); print_it != indexed_view.end(); print_it++)
	{
		variant_row r = *print_it;
		for (size_t i = 0; i < r.size(); i++)
		{
#if !defined(DTL_OUTPUT_ROWIDS)
		  if ((r.GetNames())[i].c_str() == tstring(_TEXT("ROWID")))
		  {
			  tcout << _TEXT("#### ROWID #### ");
			  break;
		  }
#endif
			tcout << r[i] << _TEXT(" ");
		}
		tcout << endl;
	}
}
#endif

#if 1
void test_variant(void) {

	TIMESTAMP_STRUCT date2, test_date = {1999, 9, 29, 0, 0, 0, 0};

	vector<TypeTranslation> types;
	vector<tstring> names;
	int i;
	tstring s;
	dtl_variant_t v;
	//TCHAR test[] = _TEXT("hello");
	//dtl_variant_t c(test);
	tostringstream test2stream;
	test2stream << _TEXT("bye");
	tstring test2 = test2stream.str();

	dtl_variant_t e(test2);
	dtl_variant_t f(test_date);
	dtl_variant_t a(1L), b(2), d(4.5);
	TypeTranslation vt0=TypeTranslation(typeid(int).name(), C_INT, SQL_INTEGER, SQL_C_SLONG, TypeTranslation::TYPE_PRIMITIVE, sizeof(int)),
#ifdef _UNICODE // needed for proper type translation object
	    vt1=TypeTranslation(typeid(wstring).name(), C_WSTRING, SQL_WVARCHAR, SQL_C_WCHAR, TypeTranslation::TYPE_COMPLEX, sizeof(wstring)),
#else
		vt1=TypeTranslation(typeid(string).name(), C_STRING, SQL_VARCHAR, SQL_C_CHAR, TypeTranslation::TYPE_COMPLEX, sizeof(tstring)),
#endif
		vt2=TypeTranslation(typeid(bool).name(), C_BOOL, SQL_INTEGER, SQL_C_USHORT, TypeTranslation::TYPE_PRIMITIVE, sizeof(bool));

	types.push_back(vt0);
	names.push_back(_TEXT("int"));

	types.push_back(vt1);
	names.push_back(_TEXT("tstring"));

	types.push_back(vt2);
	names.push_back(_TEXT("bool"));

	variant_row r, r_original(types, names);

//	s = (tstring)c;
	test2[0] = _TEXT('r');

  #ifdef __GNUC__     // gcc bug
	s = e.get_tstring();
	#else
	s = (tstring) e;
  #endif

	date2 = (TIMESTAMP_STRUCT)f;
	i = (int)d;

	r = r_original;


	variant_field vf( r[_TEXT("int")]);
	// varname turns out to be "dtl::variant_field"
	// Unused.
	// const char *varname = typeid(vf).name();

	i = (int)vf;
	i+= 5;
	vf = i;

	r[_TEXT("int")] = (int) r[_TEXT("int")] + 5;
	i = (int)(r[_TEXT("int")]);

  #ifdef __GNUC__     // gcc bug
	tstring str = vf.get_tstring();
	#else
	s = (tstring) r[_TEXT("int")];
  #endif
	// r[_TEXT("int")] = date2;
	// s = (tstring) r[_TEXT("int")];

	r[_TEXT("bool")] = tstring(_TEXT("TRUE"));
	bool bool1 = (bool) r[_TEXT("bool")];

  #ifdef __GNUC__	  // gcc bug
	tstring str_bool1 = r[_TEXT("bool")].get_tstring();
  #else
	tstring str_bool1 = (tstring) r[_TEXT("bool")];
  #endif

	r[_TEXT("bool")] = tstring(_TEXT("FALSE"));
	bool bool2 = (bool) r[_TEXT("bool")];

  #ifdef __GNUC__	  // gcc bug
	tstring str_bool2 = r[_TEXT("bool")].get_tstring();
  #else
	tstring str_bool2 = (tstring) r[_TEXT("bool")];
  #endif

    tcout << boolalpha;
	tcout <<  bool1 << _TEXT(" ") << bool2 << endl;
	tcout << str_bool1 << _TEXT(" ") << str_bool2 << endl;
};
#endif

void TestCountedPtr()
{
   CountedPtr<DerivedExample> der = new DerivedExample;

   CountedPtr<Example> base = der;
}

void TestIOStates()
{
	tcout << _TEXT("goodbit == goodbit? ")
		<< (dtl_ios_base::goodbit == dtl_ios_base::goodbit) << endl;
}

// Boris Tursky's example test for const variant_row &
void TestVariantRowConstness()
{
	PrintHeader(tcout, _TEXT("TestVariantRowConstness()"));

    DynamicDBView<> view( _TEXT("DB_EXAMPLE"), _TEXT("*"), exampleOrderBy );
    for_each( view.begin(), view.end(), ProcessRecord() );

	PrintSeparator(tcout);
}

// test DISTINCT hack for a IndexedDBView
void TestDistinctness()
{
  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

	PrintHeader(tcout, _TEXT("TestDistinctness()"));

	DBV view(_TEXT("DB_EXAMPLE"), DistinctBCAExample(),
		_TEXT("ORDER BY STRING_VALUE"));

	// we only care about STRING_VALUE's here
	IdxDBV indexed_view(view, _TEXT("Unique IndexString; DISTINCT STRING_VALUE"));

	tcout << _TEXT("Printing out unique STRING_VALUE's from DB_EXAMPLE") << endl;

	for (IdxDBV::iterator it = indexed_view.begin();
			it != indexed_view.end(); ++it)
	{
	  tcout << (*it).exampleStr << endl;
	}

	PrintSeparator(tcout);
}




#if 0 // don't try this at home!
// test const ref for a DBView
void TestConstRefs()
{
	DBView<Example> view(_TEXT("DB_EXAMPLE"), DefaultBCA<Example>, exampleOrderBy);

	tcout << _TEXT("Example ex = *(view.begin()) ... ") << endl;

	Example ex = *(view.begin());

	tcout << _TEXT("ex = ") << ex << endl;

	DBView<Example>::iterator begin = view.begin();

	const Example &begin_ref = *begin;

	tcout << _TEXT("DBView<Example>::iterator begin = view.begin()") << endl;
	tcout << _TEXT("const Example &begin_ref = *begin ... ") << endl;


	tcout << _TEXT("begin_ref = ") << begin_ref << endl;

	tcout << _TEXT("const Example &ex_ref = *(view.begin()) ... ") << endl;

	const Example &ex_ref = *(view.begin());

	tcout << _TEXT("ex_ref = ") << ex_ref << endl;

	IndexedDBView<DBView<Example> > indexed_view(view, _TEXT("IndexString; STRING_VALUE"));

	tcout << _TEXT("const Example &idx_ex_ref = *(indexed_view.begin()) ...") << endl;

	const Example &idx_ex_ref = *(indexed_view.begin());

	tcout << _TEXT("idx_ex_ref = ") << idx_ex_ref << endl;
}

#endif

// Dynamic IndexedDBView example using a variant_row ParamObj
void DynamicIndexedViewExampleVariantParamObj()
{

  typedef DynamicDBView<variant_row>    DynaDBV;
  typedef DEFAULT_DYNA_VIEW(DynaDBV)    DynaIdxDBV;

  DynaDBV dynamic_view(_TEXT("DB_EXAMPLE"),
	 _TEXT("INT_VALUE, STRING_VALUE, DOUBLE_VALUE, EXAMPLE_LONG,  EXAMPLE_DATE"),
	 _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) OR ")
	 _TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy);


  DynaDBV::select_iterator sel_it = dynamic_view.begin();
  variant_row params(dynamic_view.GetParamObj());
  params[_TEXT("0")] = 1;
  params[_TEXT("1")] = 10;
  params[_TEXT("2")] = tstring(_TEXT("Example"));
  params[_TEXT("3")] = tstring(_TEXT("01-MAR-2000"));
  sel_it.Params() = params;
  cout << *sel_it << endl;


 // make the functor needed for SetParams out of SetParamsExample() by calling
 // cb_ptr_fun(SetParamsExample)

   DynaIdxDBV  indexed_view(dynamic_view,
     _TEXT("PrimaryIndex; STRING_VALUE;")
     _TEXT("UNIQUE IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
	 BOUND, USE_AUTO_KEY,
	 cb_ptr_fun(VariantSetParamsExample));

 // Find the item where the STRING_VALUE matches the string  "Foozle"
 DynaIdxDBV::indexed_iterator idxview_it =  indexed_view.find(tstring(_TEXT("Foozle")));;


 // Update the item with the key of "Foozle", to read  "Fizzle" instead
 if (idxview_it != indexed_view.end()) {
  variant_row replacement;
  replacement = *idxview_it;
  replacement[_TEXT("STRING_VALUE")] = tstring(_TEXT("Fizzle"));
  TIMESTAMP_STRUCT date = {2003, 3, 3, 0, 0, 0, 0};
  replacement[_TEXT("EXAMPLE_DATE")] = date;

  // replacement[_TEXT("ROWID")] = tstring(_TEXT("blah"));

  indexed_view.replace(idxview_it, replacement);
 }

 // now find the record with STRING_VALUE "Mirror Image"
 DynaIdxDBV::indexed_iterator 	idxview_it2 = indexed_view.find(tstring(_TEXT("Mirror Image")));

 // Update the item with the key of "Mirror Image", to read "Egami Rorrim",
 // then "Alice in Wonderland" instead (this last one will appear in the TableDiff())
 if (idxview_it2 != indexed_view.end()) {

		// be careful here!  You would think here that you can reuse idxview_it,
		// but the first call to IndexedDBView::replace() invalidates this iterator ...
		// so make sure to grab the iterator to the updated element in the indexed view

		variant_row replacement;
		replacement = *idxview_it2;
		replacement[_TEXT("STRING_VALUE")] = tstring(_TEXT("Egami Rorrim"));

		pair<DynaIdxDBV::indexed_iterator, bool>
			pr = indexed_view.replace(idxview_it2, replacement);

		replacement[_TEXT("STRING_VALUE")] = tstring(_TEXT("Alice in Wonderland"));

		// indexed_view.replace(idxview_it2, replacement); // NO! NO! NO!
														   // idxview_it2 is invalid

		// pr.first is an iterator to the updated element, so we may use it
		// for the IndexedDBView::replace() call
		indexed_view.replace(pr.first, replacement);
 }

 // Now find a second set of items using AlternateIndex
 // The STL convention for equal_range is to return a pair  consisting of:
 // 1. an iterator referring to the beginning of the list of found  items
 // 2. an iterator pointing to the end of the list of found items.
 // We will remove all items in this range.
 const TIMESTAMP_STRUCT date_criteria = {2000, 1, 1, 0, 0, 0, 0};
 long long_criteria = 33;
 DynaIdxDBV::indexed_pair  pr = indexed_view.equal_range_AK(_TEXT("IndexLongDate"), long_criteria, date_criteria);

 idxview_it = pr.first;

 tcout << _TEXT("*** Size before erase calls: ") <<  indexed_view.size() << _TEXT(" ***")
	   << endl;

 // Remove all rows that matched the criteria in our  equal_range_AK lookup
 while (idxview_it != pr.second)
 {
  // as iterator is invalidated upon an erase(), use a temporary   iterator
  // to point to DataObj to erase
  // increment idxview_it before we erase so it will still be valid
  // when we erase the DataObj
  DynaIdxDBV::indexed_iterator deleteMe = idxview_it;

  ++idxview_it;

  indexed_view.erase(deleteMe);

 }

 tcout << _TEXT("*** Size after erase calls: ") <<  indexed_view.size() << _TEXT(" ***")
       << endl;


 // Finally, insert a new item into the container
 pair<DynaIdxDBV::iterator, bool> ins_pr;
 variant_row r(indexed_view.GetDataObj());
 r[_TEXT("INT_VALUE")] = 459;
 r[_TEXT("STRING_VALUE")] = tstring(_TEXT("Unique String  #1"));
 r[_TEXT("DOUBLE_VALUE")] = 3.5;
 r[_TEXT("EXAMPLE_LONG")] = NullField();
 r[_TEXT("EXAMPLE_DATE")] = date_criteria;
 ins_pr = indexed_view.insert(r);
 tcout << _TEXT("insertion succeded = ") <<
	   (ins_pr.second == true ? _TEXT("true"): _TEXT("false"))  << endl;

}

// Example of using an IndexDBView to read, insert and update records in a container / database
// version using a *dynamic* ParamObj
void IndexedViewExampleVariantParamObj()
{
  typedef DBView<Example, variant_row>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)                 IdxDBV;

	DBV view(_TEXT("DB_EXAMPLE"), DefaultBCA<Example>(),
	  _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) OR ")
	  _TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy, cb_ptr_fun(VariantBPAExample));

	view.set_io_handler(LoggingHandler<Example, variant_row>());

	// make the functor needed for SetParams out of SetParamsExample() by calling
	// cb_ptr_fun(SetParamsExample)
	IdxDBV indexed_view(view, _TEXT("PrimaryIndex; STRING_VALUE; UNIQUE AlternateIndex; EXAMPLE_LONG, EXAMPLE_DATE"),
		BOUND, USE_ALL_FIELDS,
		cb_ptr_fun(VariantSetParamsExample));

	try
	{
	  // Find the item where the STRING_VALUE matches the string "Foozle"
	  IdxDBV::indexed_iterator idxview_it = indexed_view.find(tstring(_TEXT("Foozle")));


	// Update the item with the key of "Foozle", to read "Fizzle" instead
	if (idxview_it != indexed_view.end()) {
		Example replacement;
		replacement = *idxview_it;
		replacement.exampleStr = _TEXT("Fizzle");
		TIMESTAMP_STRUCT date = {2003, 3, 3, 0, 0, 0, 0};
		replacement.exampleDate = jtime_c(date);
		indexed_view.replace(idxview_it, replacement);
	}

	// now find the record with STRING_VALUE "Mirror Image"
	IdxDBV::indexed_iterator idxview_it2 = indexed_view.find(tstring(_TEXT("Mirror Image")));

	// Update the item with the key of "Mirror Image", to read "Egami Rorrim",
	// then "Alice in Wonderland" instead (this last one will appear in the TableDiff())
	if (idxview_it2 != indexed_view.end()) {

		// be careful here!  You would think here that you can reuse idxview_it,
		// but the first call to IndexedDBView::replace() invalidates this iterator ...
		// so make sure to grab the iterator to the updated element in the indexed view

		Example replacement;
		replacement = *idxview_it2;
		replacement.exampleStr = _TEXT("Egami Rorrim");

		pair<IdxDBV::indexed_iterator, bool> pr =
			indexed_view.replace(idxview_it2, replacement);

		replacement.exampleStr = _TEXT("Alice in Wonderland");

		// indexed_view.replace(idxview_it2, replacement); // NO! NO! NO!
														   // idxview_it2 is invalid

		// pr.first is an iterator to the updated element, so we may use it
		// for the IndexedDBView::replace() call
		indexed_view.replace(pr.first, replacement);
	}

	// Now find a second set of items using AlternateIndex
	// The STL convention for equal_range is to return a pair consisting of:
	// 1. an iterator referring to the beginning of the list of found items
	// 2. an iterator pointing to the end of the list of found items.
	// We will remove all items in this range.
	const TIMESTAMP_STRUCT ts_date_criteria = {2000, 1, 1, 0, 0, 0, 0};
	jtime_c date_criteria(ts_date_criteria);
	long long_criteria = 33;
	IdxDBV::indexed_pair pr = indexed_view.equal_range_AK (_TEXT("AlternateIndex"),
		long_criteria, ts_date_criteria);

	idxview_it = pr.first;

	tcout << _TEXT("*** Size before erase calls: ") << indexed_view.size() <<
		_TEXT(" ***") << endl;

	// Remove all items that match the criteria in our equal_range_AK lookup
	while (idxview_it != pr.second)
	{
		// As iterator is invalidated upon an erase(), use a
		// temporary iterator to point to DataObj to erase.
		// Increment idxview_it before we erase so it will still be valid
		// when we erase the DataObj.
		IdxDBV::indexed_iterator deleteMe = idxview_it;

		++idxview_it;

		indexed_view.erase(deleteMe);

	}

	tcout << _TEXT("*** Size after erase calls: ") << indexed_view.size() <<
		_TEXT(" ***") << endl;


	// Insert a new item into the container
	pair<IdxDBV::iterator, bool> ins_pr;

    tcout << _TEXT("We will now try to insert three items.  Only the first item will succeed!") << endl;

	ins_pr = indexed_view.insert(Example(459, _TEXT("Unique String #1"), 3.4, 1, date_criteria));

	tcout << _TEXT("insertion succeded = ") << (ins_pr.second == true ? _TEXT("true"): _TEXT("false")) << endl;

	ins_pr = indexed_view.insert(Example(311, _TEXT(""), 3.99, 91, then)); // should fail on InsValidate()

	tcout << _TEXT("insertion succeded = ") << (ins_pr.second == true ? _TEXT("true"): _TEXT("false")) << endl;

	ins_pr = indexed_view.insert(Example(222, _TEXT("Positron"), -34.77, 29, then)); // should fail (ditto)

	tcout << _TEXT("insertion succeded = ") << (ins_pr.second == true ? _TEXT("true"): _TEXT("false")) << endl;
	}

	catch (...)
	{
	typedef LoggingHandler<Example, variant_row>::LoggedTriple LoggedTriple;

	// retrieve the LoggingHandler object from the IndexedDBView
	LoggingHandler<Example, variant_row> log_handler =
		indexed_view.get_io_handler((LoggingHandler<Example, variant_row> *) NULL);

    // the log is a vector of (error message, DataObj, ParamObj) triples,
    // (error message, Example object, ParamObjExample object) in this case
    // the error itself along with the relevant DataObj and ParamObj that resulted with
    // the error

    vector<LoggedTriple> error_log = log_handler.GetLog();

    // nothing to do if no errors occurred
    if (error_log.empty())
	    return;

    tcout << _TEXT("*** Error Log in IndexedViewExample(): ") << error_log.size() <<
		 _TEXT(" errors recorded! ***") << endl;

    // print out the errors
    for (vector<LoggedTriple>::const_iterator log_it = error_log.begin();
		log_it != error_log.end(); log_it++)
	{
       tcout << _TEXT("*** Error Log Entry ***") << endl;
	   tcout << _TEXT("* error message *") << endl;
	   tcout << (*log_it).errmsg << endl;
	   tcout << _TEXT("* relevant Example object *") << endl;
	   tcout << (*log_it).dataObj << endl;
	}

	}
}

// Read the contents of a table and return the resulting rows
vector<variant_row> ReadDynamicData() {

	vector<variant_row> result;

	// Our query will be "SELECT * FROM DB_EXAMPLE"
	DynamicDBView<> view(_TEXT("DB_EXAMPLE"), _TEXT("*"), exampleOrderBy);


	// Print out all rows and columns from our query
	DynamicDBView<>::select_iterator print_it = view.begin();
	variant_row r;
	for (print_it = view.begin(); print_it != view.end(); ++print_it)
	{
		 r = *print_it;
		 result.push_back(r);
	}

	return result;
}

// test the copying of an indexed view
void TestCopyOfIndexedView()
{
	PrintHeader(tcout, _TEXT("TestCopyOfIndexedView()"));

  typedef DBView<Example>            DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

  IdxDBV idx_view1(DBV(_TEXT("DB_EXAMPLE"), DefaultBCA<Example>(), exampleOrderBy),
				   _TEXT("IndexString; STRING_VALUE"));

//	Example example = *idx_view1.begin(); // do something that forces a fetch

	IdxDBV idx_view2(idx_view1);

	copy(idx_view2.begin(), idx_view2.end(), tostream_iterator<Example>(tcout, _TEXT("\n")));

	tcout << endl;

	IdxDBV idx_view3;

	idx_view3 = idx_view2;

	copy(idx_view3.begin(), idx_view3.end(), tostream_iterator<Example>(tcout, _TEXT("\n")));

	PrintSeparator(tcout);
}

#ifndef DTL_USE_HASH_CONTAINER
// reverse iterator test
void IndexedViewRevIters()
{
	PrintHeader(tcout, _TEXT("IndexedViewRevIters()"));

    typedef DBView<Example>            DBV;
    typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

	IdxDBV indexed_view(DBV(_TEXT("DB_EXAMPLE"), DefaultBCA<Example>(), exampleOrderBy),
									_TEXT("IndexString; STRING_VALUE"));

	IdxDBV::reverse_iterator rbeg = indexed_view.rbegin();

	tcout << _TEXT("Printing out indexed view in reverse with a reverse_iterator") << endl;

	copy(rbeg, indexed_view.rend(), tostream_iterator<Example>(tcout, _TEXT("\n")));

	IdxDBV::const_iterator cit = indexed_view.find(*indexed_view.begin());

	tcout << endl;
	tcout << *cit << endl;

	// cit->exampleStr = _TEXT("Bad boy!");

	// tcout << *cit << endl;

	PrintSeparator(tcout);
}
#endif

// test Local BCA's and BPA's
vector<Example> TestLocalBCAs()
{
	vector<Example> results;

	Example rowbuf;
	DefaultParamObj<Example> params;

    // set the parameters & pass to the constructor of IndexDBView
	// by using SetParamsFromClass template to tell the IndexedDBView we wish
	// to use these as our default parameters.
	params.lowIntValue = 1000;
	params.highIntValue = 2500;
	params.strValue = _TEXT("Ascension");

    typedef DBView<Example>                    ExampleView;
    typedef DEFAULT_INDEX_VIEW(ExampleView)    IdxExampleView;

	IdxExampleView idx_view(
		IdxExampleView::Args().view(
			ExampleView::Args().tables(_TEXT("DB_EXAMPLE")).bca(
				BCA(rowbuf,
					COLS[_TEXT("INT_VALUE")] 	  == rowbuf.exampleInt &&
					COLS[_TEXT("STRING_VALUE")]   == rowbuf.exampleStr &&
					COLS[_TEXT("DOUBLE_VALUE")]   == rowbuf.exampleDouble &&
					COLS[_TEXT("EXAMPLE_LONG")]   == rowbuf.exampleLong &&
					COLS[_TEXT("EXAMPLE_DATE")]   == rowbuf.exampleDate
				)
			  ).postfix(
			     _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND STRING_VALUE <> (?)") + exampleOrderBy
			  ).bpa(
				BPA(params,
					COLS[0] << params.lowIntValue &&
					COLS[1] << params.highIntValue &&
					COLS[2] << params.strValue
				)
			  ).handler(AlwaysThrowsHandler<Example>())
		).indexes(
			_TEXT("PrimaryKey; INT_VALUE; AlternateKey; STRING_VALUE")
		).params(SetParamsFromClass<DefaultParamObj<Example> >(params))
	);

	back_insert_iterator<vector<Example> > i(results);
	copy(idx_view.begin(), idx_view.end(), i);

	return results;
}

#if 0
// test Local BCA's and BPA's for single fields
vector<tstring> TestLocalBCASingleField()
{
	vector<tstring> results;

	tstring rowbuf;

	tstring params;

	params = _TEXT("Ascension");

	// typedef DBView<tstring, tstring> ExampleView;
    typedef DBView<tstring>                    ExampleView;
    typedef DEFAULT_INDEX_VIEW(ExampleView)    IdxExampleView;

	IdxExampleView idx_view(
		IdxExampleView::Args().view(
			ExampleView::Args().tables(_TEXT("DB_EXAMPLE")).bca(
				BCA(rowbuf,
					COLS[_TEXT("STRING_VALUE")]   == rowbuf
				)
			  ).postfix(
					// _TEXT("WHERE STRING_VALUE <> (?)")
					exampleOrderBy
			  ).handler(
					AlwaysThrowsHandler<tstring>()
			  )
#if 0
			   .bpa(
				BPA(params,
					COLS[0] << params
				)
			  )
#endif
		).indexes(
			_TEXT("PrimaryKey; STRING_VALUE")
		)
	);

	back_insert_iterator<vector<tstring> > i(results);

	copy(idx_view.begin(), idx_view.end(), i);

	return results;
}
#endif

void TestTypeIDCharArray()
{
#if 0 // XXX  wide-stream problem
   TCHAR arr[40];
   TCHAR *ptr = NULL;
   tcout << _TEXT("typeid of char array = ") << dtl_typeid_name(arr) << endl;
   tcout << _TEXT("typeid of char ptr. = ") << dtl_typeid_name(ptr) << endl;
#endif
}

// test character arrays
vector<StringsTest> TestCharArrays()
{
  TestTypeIDCharArray();

	vector<StringsTest> results;

	StringsTest rowbuf;

	StringsTest params;

	std_tstrcpy(params.testStr, _TEXT("Ascension"));

    typedef DBView<StringsTest, StringsTest>      ExampleView;
    typedef DEFAULT_INDEX_VIEW(ExampleView)       IdxExampleView;

#if 1
	IdxExampleView idx_view(
		IdxExampleView::Args().view(
			ExampleView::Args().tables(_TEXT("DB_EXAMPLE")).bca(
				BCA(rowbuf,
					COLS[_TEXT("INT_VALUE")] == rowbuf.testInt &&
					COLS[_TEXT("STRING_VALUE")] == DTL_CHAR_FIELD(rowbuf.testStr)
				)
			  ).postfix(
			     _TEXT("WHERE STRING_VALUE <> (?)") + exampleOrderBy
			  ).bpa(
				BPA(params,
					COLS[0] << DTL_CHAR_FIELD(params.testStr)
				)
			  ).handler(AlwaysThrowsHandler<StringsTest, StringsTest>())
		).indexes(
			_TEXT("PrimaryKey; STRING_VALUE")
		).params(
			SetParamsStringsTest()
		)
	);
#else
	IdxExampleView idx_view(
		IdxExampleView::Args().view(
			ExampleView::Args().tables(_TEXT("DB_EXAMPLE")).bca(
				BCA(rowbuf,
					COLS[_TEXT("INT_VALUE")] == rowbuf.testInt &&
					COLS[_TEXT("STRING_VALUE")].SetColumnSize(sizeof(rowbuf.testStr)) == rowbuf.testStr
				)
			  ).handler(AlwaysThrowsHandler<StringsTest, StringsTest>())
		).indexes(
			_TEXT("PrimaryKey; STRING_VALUE")
	    ).postfix(exampleOrderBy)
	);
#endif

	back_insert_iterator<vector<StringsTest> > i(results);

	copy(idx_view.begin(), idx_view.end(), i);

	tstring fieldToFindOn = _TEXT("Unique String #1");

	pair<IdxExampleView::iterator, IdxExampleView::iterator> pr =
		idx_view.equal_range(fieldToFindOn.c_str());

	tcout << _TEXT("<<< Objects with string set to \"")
		  << fieldToFindOn << _TEXT("\" >>>") << endl;

	copy(pr.first, pr.second, tostream_iterator<StringsTest>(tcout, _TEXT("\n")));

	tcout << _TEXT("---") << endl;

	return results;
}

class MyBCA {
public:
	void operator()(BoundIOs &boundIOs, tstring &row)
	{
		boundIOs[_TEXT("STRING_VALUE")] == row;
	}
};
// test Local BCA's and BPA's for single fields
vector<tstring> TestLocalBCASingleField()
{
	vector<tstring> results;

    typedef DBView<jtime_c>                    ExampleView;
    typedef DEFAULT_INDEX_VIEW(ExampleView)    IdxView;

	jtime_c row;
	ExampleView dbv(_TEXT("DB_EXAMPLE"), BCA(row, COLS[_TEXT("EXAMPLE_DATE")] == row));

//	ExampleView dbv(_TEXT("DB_EXAMPLE"), MyBCA());

	tcout << *(dbv.begin()) << endl;

	// must use SINGLE_FETCH as jtime_c not big enough to meet size
	// requirements for bulk_copy()
	IdxView idx_view(IdxView::Args().view(dbv).indexes(
		_TEXT("PrimaryKey; EXAMPLE_DATE")
		).fetch_mode(SINGLE_FETCH)
	);

	tcout << *(idx_view.begin()) << endl;

	return results;

}

// test for select_update_iterator
vector<Example> ReadAndUpdateData()
{
 vector<Example> results;

 DBView<Example> view(
	 DBView<Example>::Args().tables(_TEXT("DB_EXAMPLE")).postfix(exampleOrderBy).handler(AlwaysThrowsHandler<Example>())
 );

 DBView<Example>::select_update_iterator read_it = view.begin();


 Example row;
 //read_it.SetKey("STRING_VALUE");
 for ( int i = 0; i<5 && read_it != view.end(); ++i)
 {
  row = *read_it;
  row.exampleStr = _TEXT("Fizzle");
  row.exampleDouble = 0.5;
  *read_it = row;
   ++read_it;
 }


 for (read_it=view.begin() ; read_it != view.end(); ++read_it)
 {
  results.push_back(*read_it);
 }

 return results;
}

// dynamic test for select_update_iterator
vector<variant_row> DynamicReadAndUpdateData()
{
 vector<variant_row> results;

 DynamicDBView<> view(
 	 DynamicDBView<>::Args().tables(_TEXT("DB_EXAMPLE")).fields(_TEXT("*")).postfix(exampleOrderBy).handler(AlwaysThrowsHandler<variant_row, variant_row>())
 );

 DynamicDBView<>::select_update_iterator read_it = view.begin();

 tcout << _TEXT("view.begin() == read_it? ") << (view.begin() == read_it) << endl;

 variant_row row = *read_it;
 // read_it.SetKey(_TEXT("STRING_VALUE"));
 for ( int i = 0; i<5 && read_it != view.end(); ++i)
 {
  row = *read_it;
  row[_TEXT("STRING_VALUE")] = tstring(_TEXT("Fizzle"));
  row[_TEXT("DOUBLE_VALUE")] = 0.5;
  *read_it = row;
  ++read_it;
 }


 for (read_it=view.begin() ; read_it != view.end(); ++read_it)
 {
  results.push_back(*read_it);
 }

 return results;
}

// test the power of select_update_iterator ... add one to exampleLong to all objects
// in the view
void TestTransformSelectUpdate()
{
   DBView<Example> view(_TEXT("DB_EXAMPLE"), DefaultBCA<Example>(), exampleOrderBy);

   DBView<Example>::select_update_iterator read_it = view.begin();
   transform(view.begin(), view.end(), read_it, AddOneToExampleLong());
}

// sanity check
void SanityCheck()
{
	PrintHeader(tcout, _TEXT("SanityCheck()"));
	vector<Example> integ_check = SimpleRead();
	copy(integ_check.begin(), integ_check.end(),
		tostream_iterator<Example>(tcout, _TEXT("\n")));
	PrintSeparator(tcout);
}

// make sure we can grab names of primary key columns
void TestPKs()
{
	vector<tstring> keys = DBStmt::GetPrimaryKeys(_TEXT("DB_EXAMPLE_BACKUP"),
		DBConnection::GetDefaultConnection());

	copy(keys.begin(), keys.end(), tostream_iterator<tstring>(tcout, _TEXT("\n")));
}

void TestRowID()
{
	tcout << _TEXT("We can only run this for Oracle!") << endl;

	if (DBConnection::GetDefaultConnection().GetDBMSEnum() == DBConnection::DB_ORACLE)
	{
		// DBView<RowID> view(_TEXT("DB_EXAMPLE"), BCARowID());
		DBView<RowID> view(_TEXT("SELECT ROWID FROM DB_EXAMPLE"), BCARowID(), _TEXT("ORDER BY ROWID"));
		DBView<RowID>::sql_iterator it = view.begin();
		tcout << it->rowID << endl;
	}
}

// test nested structs
void TestOuterInner()
{
	DBView<OuterStruct> view(_TEXT("DB_EXAMPLE"), BCAOuterInner(), exampleOrderBy);
	copy(view.begin(), view.end(),
		tostream_iterator<OuterStruct>(tcout, _TEXT("\n")));
}

// test params where none are strings
class NoStringBPA {
        public:
        void operator()(BoundIOs &cols, ParamObjExampleCharred &parambuf) {
                cols[0] << parambuf.lowIntValue ;
                cols[1] << parambuf.highIntValue ;
				cols[2] << DTL_CHAR_FIELD(parambuf.strValue);
                cols[3] << parambuf.dateValue;
        }

}   ;

void TestNoStringParams()
{
	typedef DBView<ExampleCharred, ParamObjExampleCharred> DBV;
    typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

	DBV view(
		DBV::Args().tables(
		   _TEXT("DB_EXAMPLE")
	    ).postfix(
		   _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) AND STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") +
		   exampleOrderBy
	    ).bpa(
	        NoStringBPA()
	    )
	);

    DBV::select_iterator view_beg = view.begin();

	view_beg.Params().lowIntValue = 2;
	view_beg.Params().highIntValue = 8;
	// view_beg.Params().strValue = _TEXT("Example");
	std_tstrcpy(view_beg.Params().strValue, _TEXT("Example"));

	view_beg.Params().dateValue = past;

	tcout << _TEXT("As DBView ...") << endl;
	copy(view_beg, view.end(), tostream_iterator<ExampleCharred>(tcout, _TEXT("\n")));

	tcout << _TEXT("Attempting to insert into DBView ...") << endl;

	DBV::insert_iterator ins_it = view;
	*ins_it = ExampleCharred(6, _TEXT("Example"), 45.32, 12, then);
	++ins_it;

	DBConnection::GetDefaultConnection().CommitAll();

	IdxDBV idx_view(
		IdxDBV::Args().view(view).indexes(
			_TEXT("IndexString; STRING_VALUE")
		).bound(BOUND).params(SetParamsExampleCharred()).key_mode(USE_PK_FIELDS_ONLY)
	);


	tcout << _TEXT("As IndexedDBView ... ") << endl;

	vector<ExampleCharred> old_examples;
	copy(idx_view.begin(), idx_view.end(), back_inserter(old_examples));

	copy(old_examples.begin(), old_examples.end(),
		tostream_iterator<ExampleCharred>(tcout, _TEXT("\n")));

	idx_view.insert(ExampleCharred(5, _TEXT("Example"), 96.54, 23, then));

	IdxDBV::iterator find_it = idx_view.find((TCHAR *) _TEXT("To find"));

	ExampleCharred newObj = *find_it;
	newObj.exampleLong = 8484;
	idx_view.replace(find_it, newObj);
}

// read the contents of a single table and return a vector the resulting rows
vector<ExampleCharred> SimpleCharredRead() {
	vector<ExampleCharred> results;
	DBView<ExampleCharred, ParamObjExampleCharred> view(
		DBView<ExampleCharred, ParamObjExampleCharred>::Args().tables(_TEXT("DB_EXAMPLE")).postfix(exampleOrderBy)
	);

	back_insert_iterator<vector<ExampleCharred> > i(results);
	copy(view.begin(), view.end(), i);
	return results;
}

// read the contents of a single table and return a vector the resulting rows
vector<ExampleCharred> SimpleCharredIdxRead() {
	vector<ExampleCharred> results;

	typedef DBView<ExampleCharred, ParamObjExampleCharred>   DBV;
    typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

	DBV simpleView(
		DBV::Args().tables(_TEXT("DB_EXAMPLE")).postfix(exampleOrderBy)
	);
	//	IdxDBV  view(simpleView, _TEXT("IndexLong; EXAMPLE_LONG"));
	IdxDBV  view(simpleView, _TEXT("IndexString; STRING_VALUE"));

// 	IdxDBV  view(
// 		IdxDBV::Args().view(simpleView).indexes(
// 			_TEXT("IndexString; STRING_VALUE")
// 		).bound(BOUND).params(SetParamsExampleCharred()).key_mode(USE_PK_FIELDS_ONLY)
// 	);

	back_insert_iterator<vector<ExampleCharred> > i(results);
	copy(view.begin(), view.end(), i);
	return results;
}

// Example of using an IndexDBView to read, insert and update records
// to test char[]'s
void IndexedViewExampleCharred()
{
  typedef DBView<ExampleCharred, ParamObjExampleCharred> DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)    IdxDBV;

   	DBV  view(_TEXT("DB_EXAMPLE"),
		DefaultBCA<ExampleCharred>(), _TEXT("WHERE INT_VALUE BETWEEN (?) AND (?) OR ")
		_TEXT("STRING_VALUE = (?) OR EXAMPLE_DATE <= (?)") + exampleOrderBy,
		CharredBPA());

    IdxDBV	indexed_view(view,
		_TEXT("IndexString; STRING_VALUE; Unique IndexLongDate; EXAMPLE_LONG, EXAMPLE_DATE"),
		BOUND, USE_ALL_FIELDS, SetParamsExampleCharred());


	// Find the item where the STRING_VALUE matches the tstring _TEXT("Foozle")
	IdxDBV::indexed_iterator idxview_it = indexed_view.find((TCHAR *) _TEXT("Foozle"));


	// Update the item with the key of _TEXT("Foozle"), to read _TEXT("Fizzle") instead
	if (idxview_it != indexed_view.end()) {
		ExampleCharred replacement;
		replacement = *idxview_it;
		std_tstrcpy(replacement.exampleStr, _TEXT("Fizzle"));
		indexed_view.replace(idxview_it, replacement);
	}

	// Now find a second set of items using the alternate index, IndexLongDate
	// The STL convention for equal_range is that the return value consists of a pair:
	// 1. an iterator referring to the beginning of the list of found items
	// and 2. an iterator pointing to the end of the list of found items
	// We will remove all DataObj's in this range
	const TIMESTAMP_STRUCT date_criteria = {2000, 1, 1, 0, 0, 0, 0};
	long long_criteria = 33;
	IdxDBV::indexed_pair 	pr = indexed_view.equal_range_AK(_TEXT("IndexLongDate"), long_criteria, date_criteria);

	idxview_it = pr.first;

	tcout << _TEXT("*** Size before erase calls: ") << indexed_view.size() << _TEXT(" ***")
		<< endl;

	// Remove all DataObj that matched the criteria in our equal_range_AK lookup
	while (idxview_it != pr.second)
	{
		// as iterator is invalidated upon an erase(), use a temporary iterator
		// to point to DataObj to erase
		// increment idxview_it before we erase so it will still be valid
		// when we erase the DataObj
		IdxDBV::indexed_iterator deleteMe = idxview_it;

		idxview_it++;

		indexed_view.erase(deleteMe);

	}

	tcout << _TEXT("*** Size after erase calls: ") << indexed_view.size() << _TEXT(" ***")
		<< endl;


	// Finally, insert a new item into the container
	pair<IdxDBV::iterator, bool> ins_pr;
	ins_pr = indexed_view.insert(ExampleCharred(459, _TEXT("Unique String #1"), 3.4, 1, date_criteria));
	tcout << _TEXT("insertion succeded = ") << (ins_pr.second == true ? _TEXT("true"): _TEXT("false")) << endl;
}

// test insertion of string that will take >= 255 chars and thus
// more than one call to PutData()
void TestAVeryLongString()
{
	GenLongString gen_string;

	multiset<tstring> written_examples, read_examples;

	DBView<Example> view;

	if (DBConnection::GetDefaultConnection().GetDBMSEnum() == DBConnection::DB_ACCESS)
	{
		view = DBView<Example>(_TEXT("DB_LONGSTR"), BCAExampleAccess());
		gen_string = GenLongString(500);
	}
	else
	{
		view = DBView<Example>(_TEXT("DB_LONGSTR"), DefaultBCA<Example>());
		gen_string = GenLongString(500);
	}

	DBView<Example>::insert_iterator ins_it = view;

	Example example1(39, _TEXT(""), 71.71, 11, then);

	example1.exampleStr = gen_string();
	*ins_it = example1;
	++ins_it;
	written_examples.insert(example1.exampleStr);

	Example example2(48, _TEXT(""), 61.61, 21, then);

	example2.exampleStr = gen_string();
	*ins_it = example2;
	++ins_it;
	written_examples.insert(example2.exampleStr);

	Example example3(57, _TEXT(""), 51.51, 31, then);

	example3.exampleStr = gen_string();
	*ins_it = example3;
	++ins_it;
	written_examples.insert(example3.exampleStr);

	DBConnection::GetDefaultConnection().CommitAll();

	// tcout << _TEXT("Printing out view with long strings for example strings ...") << endl;
	// copy(view.begin(), view.end(), inserter(read_examples, read_examples.begin()));

	for (DBView<Example>::select_iterator sel_it = view.begin(); sel_it != view.end();
			++sel_it)
	{
		read_examples.insert(sel_it->exampleStr);
	}

	if (written_examples == read_examples)
	{
		tcout << _TEXT("long string test successful!") << endl;
		copy(read_examples.begin(), read_examples.end(),
			tostream_iterator<tstring>(tcout, _TEXT("\n")));
	}
	else
	{
		tcout << _TEXT("<<<<<<<< long string test failed!!! >>>>>>>>>") << endl;

		tcout << _TEXT("Examples written: ") << endl;
		copy(written_examples.begin(), written_examples.end(),
			tostream_iterator<tstring>(tcout, _TEXT("\n")));
		tcout << _TEXT("Examples read: ") << endl;
		copy(read_examples.begin(), read_examples.end(),
			tostream_iterator<tstring>(tcout, _TEXT("\n")));
	}

}

// test copying from static structure into dynamic one and insert record into DB
void TestInsertDynamicFromStatic()
{
	DynamicDBView<> dyn_view(_TEXT("DB_EXAMPLE"), _TEXT("*"), exampleOrderBy);
	DynamicDBView<>::insert_iterator write_it = dyn_view;
	variant_row reg = write_it.GetDataObj();

	vector<tstring> fields = reg.GetNames();

	const TIMESTAMP_STRUCT wtc = {2001, 9, 11, 0, 0, 0, 0};

	MakeDynamicFromStatic(reg, fields.begin(), Example(911, _TEXT("Emergency"), 9.11, 911, wtc));

	*write_it = reg;
	++write_it;
}

// test a bad insertion of a variant_row ... should catch uninitialized timestamp
void TestBadVariantInsert()
{
	try
	{
		DynamicDBView<> dyn_view(_TEXT("DB_EXAMPLE"), _TEXT("*"), exampleOrderBy);

		DynamicDBView<>::insert_iterator write_it = dyn_view;

		tcout << _TEXT("Attempting to insert invalid variant_row (uninitialized date) ...") << endl;

		variant_row reg = write_it.GetDataObj();

		// deliberately do not set EXAMPLE_DATE
		reg[_TEXT("INT_VALUE")]     = 54;
		reg[_TEXT("DOUBLE_VALUE")]  = 4.4;
		reg[_TEXT("STRING_VALUE")]  = tstring(_TEXT("Who Cares?"));
		reg[_TEXT("EXAMPLE_LONG")]  = 99;

		*write_it = reg;
		++write_it;
	}
	catch (RootException &ex)
	{
		tcout << _TEXT("*** Should see invalid date exception ***") << endl;
		tcout << ex.twhat() << endl;
	}

}

void
SimpleBlobTest ( )
{
  blob row, row_out;

  DBStmt (_TEXT ("delete from blob_example")).Execute ( );
  DBView<blob> view (_TEXT ("blob_example"),
		     BCA (row, COLS[_TEXT ("BLOB_VALUE")] == row));

  DBView<blob>::insert_iterator ins_it = view;
  size_t data_size = sizeof (_TEXT ("ABCDEFG"));
  row.assign ((unsigned char*) _TEXT ("ABCDEFG"), data_size);
  ins_it = row;
  ++ins_it;

  DBView<blob>::select_iterator it = view.begin ( );
  row_out = *it;

  // std::basic_string<unsigned_int>::compare(position, len,
  // other_string) does not seem to work for me maybe you can get
  // compare to work
  bool bEqual = true;
  for (size_t i = 0; i < data_size; ++i)
    if (row[i] != row_out[i]) {
      bEqual = false;

      break;
    }

  if (bEqual && row.size ( ) == row_out.size ( ))
    cout << "Binary read/write succesful." << endl;
  else
    cout << "Binary read/write failed." << endl;
}
