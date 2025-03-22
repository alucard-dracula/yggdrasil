// join example data class ... note the class declaration is now free of any of these DB
// abstractions
// Initial: 9/5/2000 - MG
// Reviewed: 11/12/2000 - CJ
// Edited: 12/20/2000 - MG - added namespace support

#include "JoinExample.h"
using namespace dtl;
using namespace std;

void SetParamsJoinExample(JoinParamObj &params)
{
	// assign paramteter values
	params.intValue = 3;
	params.strValue = _TEXT("Join Example");
	params.sampleInt = 1;
	params.sampleStr = _TEXT("Joined Tables");
}

// let's read some JoinExample objects from the database and return a vector of
// the results
vector<JoinExample> ReadJoinedData()
{
	vector<JoinExample> results;

	// assume connection has been declared somewhere as:
	// DBConnection conn(_TEXT("UID=txupz;PWD=txupz;DSN=posp;"));

	// construct view
	// note here that we use a custom parameter object class for JoinExample
	// rather than DefaultParamObj<JoinExample>
	// we also use a custom SelValidate function here: IntValuePosSelValidate
	DBView<JoinExample, JoinParamObj> view(_TEXT("DB_EXAMPLE, DB_SAMPLE"), 
		DefaultBCA<JoinExample>(),
		_TEXT("WHERE (INT_VALUE = (?) AND STRING_VALUE = (?)) AND ")
		_TEXT("(SAMPLE_INT = (?) OR SAMPLE_STR = (?))") + joinOrderBy,
		BPAJoinParamObj(), IntValuePosSelValidate());

	// loop through query results and add them to our vector
	// read_it.GetCount() records read in this loop

	DBView<JoinExample, JoinParamObj>::select_iterator
		read_it = view.begin();

	// assign paramteter values
	read_it.Params().intValue = 3;
	read_it.Params().strValue = _TEXT("Join Example");
	read_it.Params().sampleInt = 1;
	read_it.Params().sampleStr = _TEXT("Joined Tables");

	for ( ;  read_it != view.end();  read_it++)
    {
		
		// note that the read_iterator::GetCount()  is incremented in operator++()
		// remember that the record is fetched and thus the count incremented
		// before operator*() is applied to the read_iterator
		
		tcout << _TEXT("Reading element #") << read_it.GetCount() << endl;
		results.push_back(*read_it);

		tcout << *read_it << endl;
	}

	return results;
}

// read JoinExamples from indexed view to make sure data gets in IndexedDBView's
// internal list<DataObj> properly
vector<JoinExample> ReadJoinedDataFromIndexedView()
{
  typedef DBView<JoinExample, JoinParamObj>  DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)            IdxDBV;

  DBV	view(_TEXT("DB_EXAMPLE, DB_SAMPLE"), 
		DefaultBCA<JoinExample>(),
		_TEXT("WHERE (INT_VALUE = (?) AND STRING_VALUE = (?)) AND ")
		_TEXT("(SAMPLE_INT = (?) OR SAMPLE_STR = (?))") + joinOrderBy,
		BPAJoinParamObj(), IntValuePosSelValidate());

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list
    IdxDBV	indexed_view(view, _TEXT("IndexLong; SAMPLE_LONG"), 
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsJoinExample));

	vector<JoinExample> results_from_indexed_view;

	// populate results vector with the data from the indexed view
	for (IdxDBV::iterator data_it = indexed_view.begin(); data_it != indexed_view.end(); data_it++)
	{
		results_from_indexed_view.push_back(*data_it);
	}

	tcout << _TEXT("*** IndexedDBView size is: ") << results_from_indexed_view.size() << _TEXT(" ***") << endl; 

	return results_from_indexed_view;
}
// read Examples from indexed view that have the given DataObj as (primary) key
vector<JoinExample> ReadJoinedFilteredDataPK(JoinExample &key)
{
  typedef DBView<JoinExample, JoinParamObj>  DBV;
  typedef DEFAULT_INDEX_VIEW(DBV)            IdxDBV;

   	DBV	view(_TEXT("DB_EXAMPLE, DB_SAMPLE"), 
		BCAJoinExampleObj(),
		_TEXT("WHERE (INT_VALUE = (?) AND STRING_VALUE = (?)) AND ")
		_TEXT("(SAMPLE_INT = (?) OR SAMPLE_STR = (?))") + joinOrderBy,
		BPAJoinParamObj(), IntValuePosSelValidate());

	// here, we are not too concerned about the indices themselves
	// we're just testing to see if the DataObj's are stored properly in the
	// indexed view's internal list
    IdxDBV indexed_view(view, _TEXT("IndexLong; SAMPLE_LONG"),
		UNBOUND, USE_ALL_FIELDS, cb_ptr_fun(SetParamsJoinExample));

	vector<JoinExample> filtered_results;

	// the return value consists of a pair: 1. an iterator referring to the found items
	// and 2. an iterator pointing to the end of the list of found items 
	IdxDBV::indexed_pair    pr = indexed_view.equal_range(key);

	IdxDBV::indexed_iterator    idx_data_it = pr.first;

	IdxDBV::indexed_iterator	end_it = pr.second;

	IdxDBV::indexed_iterator	find_it = indexed_view.find(key);

	if (idx_data_it == find_it)
		tcout << _TEXT("Iterator integrity satisfied!") << endl;

	// need to be able to iterate through found results
	for ( ; idx_data_it != end_it; idx_data_it++)
	{
		// first * dereferences the iterator, returning a DataObj *
		// second * dereferences the DataObj *, yielding a DataObj
		const JoinExample &idx_data = *idx_data_it;
		filtered_results.push_back(idx_data);
	}

	tcout << filtered_results.size() << _TEXT(" results found!") << endl;
	return filtered_results;
}
