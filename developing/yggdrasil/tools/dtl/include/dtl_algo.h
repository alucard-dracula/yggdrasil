#ifndef DTL_DTL_ALGO_H
#define DTL_DTL_ALGO_H

#include "dtl_config.h"
#include "variant_row.h"

#include "std_warn_off.h"
#include <memory>
#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE
// DTL algorithms for functions such as bulk_copy(), etc.

// **** bulk_copy() ****
// should pass in the iterators by copy to be consistent with STL conventions
// not sure what is correct behavior though

// input iterators
template<class ItType, class DataObj>
   void bulk_copy(ItType &sel_it, DataObj *begin, DataObj *end, bool bValidate = true,
				  size_t row_size = sizeof(DataObj))
{
   sel_it.bulk_copy(begin, end, bValidate, row_size);
}


// output iterators
template<class ItType, class DataObj>
   void bulk_copy(const DataObj *begin, const DataObj *end, ItType &ins_it, bool bValidate = true,
				  size_t row_size = sizeof(DataObj))
{
   ins_it.bulk_copy(begin, end, bValidate, row_size);
}

#ifndef __hpux

#if 0
// versions of bulk_copy() to reject variant_rows at compile time
template<class ItType>
	void bulk_copy(ItType &sel_it, variant_row *begin, variant_row *end, bool bValidate = true,
				   size_t row_size = sizeof(variant_row))
{
	variant_row vr;
	variant_row::failed_conversion fail = static_cast<variant_row::failed_conversion>(vr);
}
#endif

template<class ItType>
	void bulk_copy(const variant_row *begin, const variant_row *end, ItType &out_it, bool bValidate = true,
				   size_t row_size = sizeof(variant_row))
{
	variant_row vr;
	variant_row::failed_conversion fail = static_cast<variant_row::failed_conversion>(vr);
}

#endif


template<class DataObj> inline size_t get_row_size(const DataObj & DTL_DUMMY_PARAM_1) {
	return sizeof(DataObj);
}

template<> inline size_t get_row_size<variant_row>(const variant_row &tmp) {
	return sizeof(variant_row) + tmp.row_size();
}

template<class DataObj> inline void init_row_ptr(const DataObj &tmp, DataObj *ptr) {
	new (ptr) DataObj(tmp);
	return;
}

template<> inline void init_row_ptr<variant_row>(const variant_row &tmp, variant_row *ptr) {
	new (ptr) variant_row(tmp, reinterpret_cast<BYTE *>(ptr) + sizeof(variant_row));
	return;
}


// Helper method to call bulk fetch
// Fetches first into a temporary 'buffer' provided by the user,
// then copies any records that have passed validation into the container 'out_data'.
template<class SelectIterator, class OutputIterator> void 
bulk_fetch_helper(SelectIterator read_it, size_t buffer_size, 
OutputIterator output) {

#ifdef DTL_NO_BULK_OPERATIONS
		    // Solaris doesn't support bulk fetch properly, so just use std::copy() here
			STD_::copy(read_it, read_it.GetView().end(), output);

#else

			typedef typename SelectIterator::value_type DataObj;
			typedef typename SelectIterator::param_type ParamObj;

			// Allocate buffer ... needs to be rewritten to be allocated dynamically
			// inside of bulk_copy()????
			// STD_::vector<DataObj> buffer(buffer_size);
			
			size_t numRecordsFetched = 0; 

			const size_t NumColumns = read_it.GetBoundIOs().NumColumns();

			DataObj tmp(read_it.GetDataObj());

			// we must pad the buffer which will hold the fetched DataObj's
			// with extra bytes if necessary
			const size_t row_size = DTL_MAX(get_row_size(tmp), 
					DTL_MAX(sizeof(SQLINTEGER) * NumColumns,
						sizeof(TIMESTAMP_STRUCT) * NumColumns)
					);

			STD_::vector<BYTE> buffer(row_size * buffer_size);

			do
			{

				// must do in place construction of the DataObj's to be held
				// in the buffer
				for (size_t i = 0; i < buffer_size; ++i)
				{
					DataObj *ptr = reinterpret_cast<DataObj *>(&*buffer.begin() + i * row_size);
					init_row_ptr<DataObj>(tmp, ptr);
				}

				// add in row_size parameter

				DataObj *buf_begin = reinterpret_cast<DataObj *>(&*buffer.begin());
				// DataObj *buf_end = reinterpret_cast<DataObj *>(buffer.end());
				// Have to be tricky to get end.  Some compilers like MSVC 2005 will give error if they think we are accessing *end()
				DataObj *buf_end = reinterpret_cast<DataObj *>(&*buffer.begin() + buffer.size() + 1);

				bulk_copy(read_it, buf_begin, buf_end,
						  true, row_size);

				if ((numRecordsFetched = read_it.GetLastCount()) == 0)
					break;

				// if bulk copy successful, copy into our data list

				STD_::vector<size_t> failures;

				// now must weed out the DataObj's that failed from the vector
				// (handler actually stores indices of which elements failed
				// to fetch)

				failures = read_it.get_io_handler().GetFailures();
				read_it.clear();

				// weed out objects that failed validation
				for (size_t tmp_idx = 0; tmp_idx < numRecordsFetched; ++tmp_idx)
				{
					// must copy one object at a time to data list after
					// making sure it wasn't one that failed SelValidate()
						
					// note that failures vector is sorted ...
					if (!STD_::binary_search(failures.begin(), failures.end(), tmp_idx))
					{
						// OK to copy object as SelValidate() passed
						const DataObj &copy_me =
							*(reinterpret_cast<const DataObj *>(
							  &*buffer.begin() + tmp_idx * row_size
							));
						*output = copy_me;
						++output;
					}
				}

				// in place destruction now must take place of all DataObj's
				// held in the buffer
				for (size_t j = 0; j < buffer_size; ++j)
				{
					DataObj *ptr = reinterpret_cast<DataObj *>(&*buffer.begin() + j * row_size);
					ptr->~DataObj();
				}

			} while (numRecordsFetched != 0);
#endif

}

// Helper method to call bulk insert
// Fetches first into a temporary 'buffer' provided by the user,
// then copies any records that have passed validation into the container 'out_data'.
template<class InputIterator, class InsertIterator> void 
bulk_insert_helper(InputIterator beg, InputIterator end, size_t buffer_size, 
InsertIterator ins_it) {

#ifdef DTL_NO_BULK_OPERATIONS
		    // Solaris doesn't support bulk insert, so just use std::copy() here
			STD_::copy(beg, end, ins_it);
#else
			typedef typename InsertIterator::value_type DataObj;
			typedef typename InsertIterator::param_type ParamObj;

			// Allocate buffer ... needs to be rewritten to be allocated dynamically
			// inside of bulk_copy()????
			// STD_::vector<DataObj> buffer(buffer_size);
			
			size_t numRecordsInserted = 0; 

			const size_t NumColumns = ins_it.GetBoundIOs().NumColumns();

			// we must pad the buffer which will hold the fetched DataObj's
			// with extra bytes if necessary
			const size_t row_size = DTL_MAX(sizeof(DataObj), 
					DTL_MAX(sizeof(SQLLEN) * NumColumns,
						sizeof(TIMESTAMP_STRUCT) * NumColumns)
					);

			STD_::vector<BYTE> buffer(row_size * buffer_size);
			BYTE *pdata_begin = &*buffer.begin();
			// BYTE *pdata_end = &*buffer.end();
			// Have to be careful, compilers like MSVC 2005 will freak if they think you are accessing *end()
			BYTE *pdata_end = &*buffer.begin() + buffer.size() + 1;
			do
			{
				// add in row_size parameter
				size_t arrayRange = 0;

				// copy object at beginning of boundary for DataObj
				for (arrayRange = 0; arrayRange < buffer_size && beg != end; 
						++arrayRange, ++beg)
				{
					DataObj *dest = 
						reinterpret_cast<DataObj *>(pdata_begin + arrayRange * row_size);
				
					// need to do in place construction of DataObj to get valid object
					// STD_::construct(dest, DataObj());
					new (dest) DataObj(*beg);
					// *dest = *beg;

				}

				DataObj *buf_begin = reinterpret_cast<DataObj *>(pdata_begin);

				// end of array range is [&*buffer.begin(), &*buffer.end())
				// or if shorter,
				// [&*buffer.begin(), &*buffer.begin() + i * row_size)

				DataObj *buf_end = (arrayRange < buffer_size ? 
					reinterpret_cast<DataObj *>(pdata_begin + arrayRange * row_size) : 
					reinterpret_cast<DataObj *>(pdata_end)
				);

				bulk_copy(buf_begin, buf_end, ins_it,
						  true, row_size);

				// in place destruction now must take place of all DataObj's
				// held in the buffer
				for (size_t j = 0; j < arrayRange; ++j)
				{
					DataObj *ptr = reinterpret_cast<DataObj *>(&*buffer.begin() + j * row_size);
					// STD_::destroy(ptr);
					ptr->~DataObj();
				}

				numRecordsInserted = ins_it.GetLastCount();

			} while (numRecordsInserted != 0);

#endif	
}

END_DTL_NAMESPACE

#endif
