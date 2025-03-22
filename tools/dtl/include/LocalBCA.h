// provides the ability to declare a BCA locally within
// a DBView constructor call
#ifndef DTL_LOCAL_BCA_H
#define DTL_LOCAL_BCA_H

#include "BoundIO.h"

BEGIN_DTL_NAMESPACE

// BoundIOs like object to allow us to generate BoundIO's as we need them
class GenerateBoundIOs
{
public:
	BoundIO operator[](const tstring &name) const;
	BoundIO operator[](unsigned int paramNum) const;
};

// boundIOs like object which we can call subscript operator on to
// give us a fresh BoundIO to map to in local BCA's/BPA's
extern GenerateBoundIOs COLS;

// actual LocalBCA functor!

// mimics actual local function definitions inside the "calling" code
// this template is actually used by both local BCA's and BPA's
// actually invoked like:

// /* in some function */
// SomeObject dataObj;
// DBView<SomeObject> view("SOME_DB_TABLE", 
//  BCA(dataObj,
//   COLS["field1"] == dataObj.field1 &&
//	 COLS["field2"] == dataObj.field2 &&
//	 COLS["field3"] == dataObj.field3
//  )
// );
// /* ... use view ... */

// The code above is equivalent to:
// class BCASomeObject : public std::binary_function<BoundIOs &, 
//	SomeObject &, void>
// {
//	public:
//    void operator()(BoundIOs &boundIOs, SomeObject &dataObj)
//	  {
//		 boundIOs["field1"] == dataObj.field1;
//		 boundIOs["field2"] == dataObj.field2;
//		 boundIOs["field3"] == dataObj.field3;
//	  }
// };
//
// /* in some function */
// DBView<SomeObject> view("SOME_DB_TABLE", BCASomeObject());
// /* ... use view ... */

template<class DataObj> 
class LocalBCA 
	//: public STD_::binary_function<BoundIOs &, DataObj &, void>
{
public:
	typedef BoundIOs& first_argument_type;
	typedef DataObj& second_argument_type;
	typedef void result_type;

private:
	BoundIOs dummyBoundIOs; // cache of proto-BoundIOs for this local BCA
							// these objects contain field type info
							// and relative addresses for each BoundIO
	DataObj *pRowbuf;  // local obj. pointer needed
public:
	// constructor just makes local copy of the BoundIO's passed in
	LocalBCA(DataObj &rowbuf, const BoundIOs &bIOs) : dummyBoundIOs(bIOs),
		pRowbuf(&rowbuf) 
	{ 
	}

	// this form of the constructor is needed in order to handle a case
	// where we're only binding to one field
	// we must wrap a single BoundIO into the dummy BoundIOs
	LocalBCA(DataObj &rowbuf, const BoundIO &boundIO) :
		dummyBoundIOs(), pRowbuf(&rowbuf)
	{ 
		BoundIO orig(boundIO);
		
		BoundIO &first = dummyBoundIOs[boundIO.GetName()] = orig;
		
		// update parent BoundIO pointers
		first.SetBoundIOsPtr(&dummyBoundIOs);
		
		if (boundIO.IsColumn())
		{
			first.SetColNo(0);
		}
	}
	
	// BCA has some funkiness on invocation
	void operator()(BoundIOs &bIOs, DataObj &rowbuf)
	{
		// get BoundIOs that have proper offsets vs. *pRowbuf
		BoundIOs tmp_bIOs = ProcessDummyBoundIOs();

		// rebind the BoundIOs to use rowbuf as base object
		tmp_bIOs.BindAsBase(rowbuf);

		// update absolute addresses in BoundIOs
		for (BoundIOs::iterator it = tmp_bIOs.begin(); it != tmp_bIOs.end(); ++it)
			{
				BoundIO &boundIO = (*it).second;
				tstring name = (*it).first;
				BYTE *fieldAddr = (BYTE *) data_ptr(&rowbuf) + boundIO.GetOffset();
				boundIO.SetAddr(fieldAddr);
				bIOs[name] = boundIO;
				if (boundIO.IsParam())
					bIOs.cColumns--;
#ifdef DTL_MEM_DEBUG
				tcout << _TEXT("DynamicRowBCA is binding name: ") << name << 
					_TEXT(" IsNull: ") << boundIO.IsNull() << 
					_TEXT(" Addr: ") << (void *)fieldAddr << STD_::endl;
#endif
			}

	}

private:
	// this routine has to process our proto-BoundIOs which carry addresses
	// to fields in the functor's *pRowbuf

	// returns real boundIOs for the object with proper offsets
	BoundIOs ProcessDummyBoundIOs()
	{
		BoundIOs realBoundIOs;

		for (BoundIOs::iterator b_it = dummyBoundIOs.begin();
				b_it != dummyBoundIOs.end(); ++b_it)
		{
			BoundIO &boundIO = b_it->second;

			size_t offset = ((BYTE *) boundIO.GetAddr()) - data_ptr(pRowbuf);

			BoundIO &newBoundIO = realBoundIOs[boundIO.GetName()] = boundIO;
			
			newBoundIO.SetBoundIOsPtr(&realBoundIOs);
			newBoundIO.SetAddr( ((BYTE *) pRowbuf) + offset);
			newBoundIO.SetOffset(offset);
		}

		return realBoundIOs;
	}

};

// helper functions ... user should invoke these to create local BCA's
// or BPA's

// const and const cast needed on BoundIO(s) because can't pass
// temporary object by reference
template<class DataObj> 
	LocalBCA<DataObj> BCA(DataObj &rowbuf, const BoundIOs &boundIOs)
{
	return LocalBCA<DataObj>(rowbuf, const_cast<BoundIOs &>(boundIOs));
}

template<class ParamObj>
	LocalBCA<ParamObj> BPA(ParamObj &parambuf, const BoundIOs &boundIOs)
{
	return LocalBCA<ParamObj>(parambuf, const_cast<BoundIOs &>(boundIOs));
}

// versions to accomodate binding just single fields
template<class DataObj> 
	LocalBCA<DataObj> BCA(DataObj &rowbuf, const BoundIO &boundIO)
{
	return LocalBCA<DataObj>(rowbuf, const_cast<BoundIO &>(boundIO));
}

template<class ParamObj>
	LocalBCA<ParamObj> BPA(ParamObj &parambuf, const BoundIO &boundIO)
{
	return LocalBCA<ParamObj>(parambuf, const_cast<BoundIO &>(boundIO));
}

END_DTL_NAMESPACE

#endif

