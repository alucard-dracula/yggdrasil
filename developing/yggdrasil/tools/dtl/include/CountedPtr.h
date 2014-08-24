/* Copyright © 2000 
Michael Gradman and Corwin Joy 

Permission to use, copy, modify, distribute and sell this software and 
its documentation for any purpose is hereby granted without fee, provided 
that the above copyright notice appears in all copies and that both that 
copyright notice and this permission notice appear in supporting documentation. 
Corwin Joy and Michael Gradman make no representations about the suitability 
of this software for any purpose. 
It is provided "as is" without express or implied warranty.
*/ 
// simple reference count pointer class
// taken and modified from "The C++ Standard Library: A Tutorial and Reference"
// by Nicolai M. Josuttis
// (c) 1999 - p.222-223
// Edited: 12/19/2000 - MG - added namespaces

#ifndef DTL_COUNTED_PTR_H
#define DTL_COUNTED_PTR_H

#include "dtl_config.h"

#include "std_warn_off.h"

#include <algorithm>
#include <cstdlib>
#include <set>

// sql.h below needed to define SDWORD
#ifdef DTL_NATIVE_WIN32
	#ifdef DTL_USE_MFC
		#include <afx.h>
	#else
		#include <windows.h>
	#endif
#elif defined(DTL_MINGW32)
   #include <windows.h>
#endif

#include <sql.h>

#include "std_warn_on.h"

#ifdef  _MSC_VER
#pragma warning(disable: 4284)  // Disable infix warning
#endif

BEGIN_DTL_NAMESPACE

// class for counted reference semantics
// deletes the object to which it refers when the last CountedPtr
// that refers to it is destroyed

template<class T> class CountedPtr
{
private:
	T *ptr;		 // pointer to the value
	long *count; // shared number of owners

	bool no_delete; // don't delete when reference count reaches 0?
public:
	// initialize pointer with existing pointer
	// requires that the pointer p is a return value of new
	CountedPtr(T *p = NULL) : ptr(p), count(new long(1)), no_delete(false) { }

	// templated form to maybe provide proper derived to base conversions, etc.
	template<class U> CountedPtr(const CountedPtr<U> &p) : ptr(const_cast<U *>(p.get())), 
		count(const_cast<long *>(p.GetRefCountPtr())), no_delete(p.GetDontDelete())
	{
		++*count;
	}

	// copy pointer (one more owner)
	CountedPtr(const CountedPtr<T> &p) : ptr(p.ptr), count(p.count), no_delete(p.no_delete) 
	{
		++*count;
	}

	// destructor (delete value if this was the last owner)
	~CountedPtr() 
	{
		dispose();

	}

	// initialize pointer with existing pointer
	// requires that the pointer p is a return value of new
	// otherwise you must explicitly call DontDelete() in order not to kill the object
	// at destruction time
	CountedPtr<T> &operator=(T *p) {
		dispose();

		ptr = p;
		count = new long(1);
		no_delete = false;
		return *this;
	}

	void swap(CountedPtr<T> &other)  // never throws
	{
		STD_::swap(ptr,other.ptr); 
		STD_::swap(count,other.count); 
		STD_::swap(no_delete, other.no_delete);
	}


	// exception safe assignment 
	CountedPtr<T> &operator=(const CountedPtr<T> &p) 
	{
		if (this != &p)
		{
			CountedPtr<T> temp(p);
			swap(temp);
		}

		return *this;
	}

	// access the value to which the pointer refers
	const T &operator*() const 
	{
		return *ptr;
	}
	
	T &operator*()
	{
	    return *ptr;
	}

	const T *operator->() const 
	{
		return &**this;
	}

	T *operator->()
	{
		return &**this;
	}

	// access the underlying pointer
	const T *get() const
	{
		return ptr;
	}

	T *get()
	{
		return ptr;
	}

	// more operators to give the feel of real pointers
	friend bool operator==(const CountedPtr<T> &ptr1, const CountedPtr<T> &ptr2)
	{
		return ptr1.ptr == ptr2.ptr;
	}

	friend bool operator!=(const CountedPtr<T> &ptr1, const CountedPtr<T> &ptr2)
	{
		return !(ptr1 == ptr2);
	}

	bool operator!() const
	{
		return (ptr == NULL);
	}

	operator bool() const
	{
		return (ptr != NULL);
	}

	long GetRefCount() const
	{
		if (ptr == NULL)
			return 0;

		return *count;
	}

	const long *GetRefCountPtr() const
    {
		return count;
	}

	// tells CountedPtr not to delete the wrapped pointer
	// must use if you pass a pointer that wasn't allocated on the heap
	void DontDelete()
	{
		no_delete = true;
	}

	bool GetDontDelete() const
	{
		return no_delete;
	}
private:
	void dispose()
	{
		if (count != NULL && --*count == 0)
		{
			delete count;
			count = NULL;

			if (ptr != NULL && !no_delete)
				delete ptr;
			ptr = NULL;
		}
	}
};

// specializations needed for POD's as they don't have operator->()
template<> class CountedPtr<SDWORD>
{
private:
	SDWORD *ptr; // pointer to the value
	long *count; // shared number of owners
	bool no_delete; // don't delete when reference count reaches 0?
public:
	// initialize pointer with existing pointer
	// requires that the pointer p is a return value of new
	CountedPtr(SDWORD *p = NULL) : ptr(p), count(new long(1)), no_delete(false) { }

	// templated form to maybe provide proper derived to base conversions, etc.
	template<class U> CountedPtr(const CountedPtr<U> &p) : ptr(const_cast<U *>(p.get())), 
		count(const_cast<long *>(p.GetRefCountPtr())), no_delete(p.GetNoDelete())
	{
		++*count;
	}

	// copy pointer (one more owner)
	CountedPtr(const CountedPtr<SDWORD> &p) : ptr(p.ptr), count(p.count), no_delete(p.no_delete)
	{
		++*count;
	}

	// destructor (delete value if this was the last owner)
	~CountedPtr() 
	{
		dispose();
	}

	// initialize pointer with existing pointer
	// requires that the pointer p is a return value of new
	// otherwise you must explicitly call DontDelete() in order not to kill the object
	// at destruction time
	CountedPtr<SDWORD> &operator=(SDWORD *p) {
		dispose();

		ptr = p;
		count = new long(1);
		no_delete = false;

		return *this;
	}

	void swap(CountedPtr<SDWORD> &other)  // never throws
	{
		STD_::swap(ptr,other.ptr); 
		STD_::swap(count,other.count); 
		STD_::swap(no_delete, other.no_delete);
	}


	// exception safe assignment 
	CountedPtr<SDWORD> &operator=(const CountedPtr<SDWORD> &p) 
	{
		if (this != &p)
		{
			CountedPtr<SDWORD> temp(p);
			swap(temp);
		}

		return *this;
	}

	// access the value to which the pointer refers
	const SDWORD &operator*() const 
	{
		return *ptr;
	}
	
	SDWORD &operator*()
	{
	    return *ptr;
	}

	// ** no operator->() for POD's

	// access the underlying pointer
	const SDWORD *get() const
	{
		return ptr;
	}

	SDWORD *get()
	{
		return ptr;
	}

	// more operators to give the feel of real pointers
	friend bool operator==(const CountedPtr<SDWORD> &ptr1, const CountedPtr<SDWORD> &ptr2)
	{
		return ptr1.ptr == ptr2.ptr;
	}

	friend bool operator!=(const CountedPtr<SDWORD> &ptr1, const CountedPtr<SDWORD> &ptr2)
	{
		return !(ptr1 == ptr2);
	}

	bool operator!() const
	{
		return (ptr == NULL);
	}

	operator bool() const
	{
		return (ptr != NULL);
	}

	long GetRefCount() const
	{
		if (ptr == NULL)
			return 0;

		return *count;
	}

	const long *GetRefCountPtr() const
    {
		return count;
	}

	// tells CountedPtr not to delete the wrapped pointer
	// must use if you pass a pointer that wasn't allocated on the heap
	void DontDelete()
	{
		no_delete = true;
	}

	bool GetDontDelete() const
	{
		return no_delete;
	}

private:
	void dispose()
	{
		if (count != NULL && --*count == 0 && !no_delete)
		{
			delete count;
			count = NULL;

			if (ptr != NULL && !no_delete)
				delete ptr;
			ptr = NULL;
		}
	}
};

// specializations needed for POD's as they don't have operator->()
#ifdef _WIN64
template<> class CountedPtr<SQLLEN>
{
private:
	SQLLEN *ptr; // pointer to the value
	long *count; // shared number of owners
	bool no_delete; // don't delete when reference count reaches 0?
public:
	// initialize pointer with existing pointer
	// requires that the pointer p is a return value of new
	CountedPtr(SQLLEN *p = NULL) : ptr(p), count(new long(1)), no_delete(false) { }

	// templated form to maybe provide proper derived to base conversions, etc.
	template<class U> CountedPtr(const CountedPtr<U> &p) : ptr(const_cast<U *>(p.get())), 
		count(const_cast<long *>(p.GetRefCountPtr())), no_delete(p.GetNoDelete())
	{
		++*count;
	}

	// copy pointer (one more owner)
	CountedPtr(const CountedPtr<SQLLEN> &p) : ptr(p.ptr), count(p.count), no_delete(p.no_delete)
	{
		++*count;
	}

	// destructor (delete value if this was the last owner)
	~CountedPtr() 
	{
		dispose();
	}

	// initialize pointer with existing pointer
	// requires that the pointer p is a return value of new
	// otherwise you must explicitly call DontDelete() in order not to kill the object
	// at destruction time
	CountedPtr<SQLLEN> &operator=(SQLLEN *p) {
		dispose();

		ptr = p;
		count = new long(1);
		no_delete = false;

		return *this;
	}

	void swap(CountedPtr<SQLLEN> &other)  // never throws
	{
		STD_::swap(ptr,other.ptr); 
		STD_::swap(count,other.count); 
		STD_::swap(no_delete, other.no_delete);
	}


	// exception safe assignment 
	CountedPtr<SQLLEN> &operator=(const CountedPtr<SQLLEN> &p) 
	{
		if (this != &p)
		{
			CountedPtr<SQLLEN> temp(p);
			swap(temp);
		}

		return *this;
	}

	// access the value to which the pointer refers
	const SQLLEN &operator*() const 
	{
		return *ptr;
	}
	
	SQLLEN &operator*()
	{
	    return *ptr;
	}

	// ** no operator->() for POD's

	// access the underlying pointer
	const SQLLEN *get() const
	{
		return ptr;
	}

	SQLLEN *get()
	{
		return ptr;
	}

	// more operators to give the feel of real pointers
	friend bool operator==(const CountedPtr<SQLLEN> &ptr1, const CountedPtr<SQLLEN> &ptr2)
	{
		return ptr1.ptr == ptr2.ptr;
	}

	friend bool operator!=(const CountedPtr<SQLLEN> &ptr1, const CountedPtr<SQLLEN> &ptr2)
	{
		return !(ptr1 == ptr2);
	}

	bool operator!() const
	{
		return (ptr == NULL);
	}

	operator bool() const
	{
		return (ptr != NULL);
	}

	long GetRefCount() const
	{
		if (ptr == NULL)
			return 0;

		return *count;
	}

	const long *GetRefCountPtr() const
    {
		return count;
	}

	// tells CountedPtr not to delete the wrapped pointer
	// must use if you pass a pointer that wasn't allocated on the heap
	void DontDelete()
	{
		no_delete = true;
	}

	bool GetDontDelete() const
	{
		return no_delete;
	}

private:
	void dispose()
	{
		if (count != NULL && --*count == 0 && !no_delete)
		{
			delete count;
			count = NULL;

			if (ptr != NULL && !no_delete)
				delete ptr;
			ptr = NULL;
		}
	}
};

#endif


class MemPtr
{
private:
	BYTE *ptr;		 // pointer to the value
	long *count;     // shared number of owners
    size_t bufsize;  // bytes allocated
	bool no_delete;  // don't delete when reference count reaches 0
public:

#ifdef DTL_MEM_DEBUG
	static STD_::set<BYTE *> memChecker; // memory checker added for debugging
								   // of MemPtr's
								   // valid addresses are stored here
								   // so if someone tries to poke into an
								   // unknown address, we'll print out an error
#endif
	
	// initialize pointer with desired size

	// using operator new as we're only allocating, not constructing
	explicit MemPtr(size_t sz) : 
	    ptr((BYTE *) ::operator new(sz)), count(new long(1)), bufsize(sz),
		no_delete(false)
	{ 
#ifdef DTL_MEM_DEBUG
			memChecker.insert(ptr); // register address with memory checker	
#endif
	}

	MemPtr() : ptr(NULL), count(NULL), bufsize(0), no_delete(false)
	{

	}

	// return underlying ptr.
	BYTE *get()
	{
#ifdef DTL_MEM_DEBUG
	   CheckPtr();
#endif
       return ptr;
	}

	const BYTE *get() const
	{
#ifdef DTL_MEM_DEBUG
	   CheckPtr();
#endif
	   return ptr;
	}

	// return bytes allocated
	size_t size() const
	{
#ifdef DTL_MEM_DEBUG
		CheckPtr();
#endif
       return bufsize;
	}

	// copy pointer (one more owner)
	MemPtr(const MemPtr &p) : ptr(p.ptr), count(p.count) , bufsize(p.bufsize),
		no_delete(p.no_delete)
	{
		if (count != NULL)
		    ++*count;
	}

	void reset(size_t sz)
	{
	   dispose();
	   ptr = (BYTE *) ::operator new(sz); // allocate new space
	   count = new long(1);
	   bufsize = sz;
	   no_delete = false;
#ifdef DTL_MEM_DEBUG
	   //memset(ptr, 0, sz);
	   memChecker.insert(ptr);
#endif
	   
	}

	void set(BYTE *p, size_t sz)
	{
	   dispose();
	   ptr = p; // allocate new space
	   count = new long(1);  // do not delete shared pointer
	   bufsize = sz;
	   no_delete = false;
#ifdef DTL_MEM_DEBUG
	   //memset(ptr, 0, sz);
	   memChecker.insert(ptr);
#endif
	   
	}

	void set_addr_only(BYTE *p)
	{
		size_t sz = bufsize;

		dispose();
		ptr = p; // allocate new space
		count = new long(1); // do not delete shared pointer
		no_delete = false;

		bufsize = sz;
#ifdef DTL_MEM_DEBUG
		memChecker.insert(ptr);
#endif
	}

	// tells MemPtr not to delete the wrapped pointer
	// must use if you pass a pointer that wasn't allocated on the heap
	void DontDelete()
	{
		no_delete = true;
	}

	bool GetNoDelete() const
	{
		return no_delete;
	}

	// destructor (delete value if this was the last owner)
	~MemPtr() 
	{
		dispose();
	}

	void swap(MemPtr &other)  // never throws
	{
		STD_::swap(ptr, other.ptr); 
		STD_::swap(count, other.count); 
		STD_::swap(bufsize, other.bufsize);
		STD_::swap(no_delete, other.no_delete);
	}


	// exception safe assignment 
	MemPtr &operator=(const MemPtr &p) 
	{
		if (this != &p)
		{
			MemPtr temp(p);
			swap(temp);
		}

		return *this;
	}


private:
	void dispose()
	{
		if (count != NULL && --*count == 0)
		{

			delete count;
			count = NULL;

			if (ptr != NULL && !no_delete) {
#ifdef DTL_MEM_DEBUG
				memChecker.erase(ptr);
#endif
				::operator delete(ptr); // don't invoke destructor, just deallocate
			}

			ptr = NULL;
			bufsize = 0;
		}
	}

#ifdef DTL_MEM_DEBUG
	void CheckPtr() const
	{
	   // pointer is invalid if not NULL and not found in set of valid
	   // pointers in the memory checker
       if (ptr != NULL && memChecker.find(ptr) == memChecker.end())
	   {
		   void *addr = ptr;
           tcout << _TEXT("Invalid access of ptr: ") << addr << STD_::endl;
	   }
	   else if (ptr == NULL)
	   {
		   // tcout << _TEXT("Attempting to access NULL ptr") << STD_::endl;
		   NULL;
	   }

	}
#endif
};


END_DTL_NAMESPACE

#endif
