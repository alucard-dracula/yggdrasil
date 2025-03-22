// initialization of statics from DB_Base

#include "DB_Base.h"

#include "std_warn_off.h"
#include <algorithm>
#include "std_warn_on.h"

BEGIN_DTL_NAMESPACE

const dtl_iostate dtl_ios_base::goodbit = dtl_iostate::goodbit;
const dtl_iostate dtl_ios_base::failbit = dtl_iostate::failbit;
const dtl_iostate dtl_ios_base::badbit  = dtl_iostate::badbit;

// ************ Implementation code for dtl_iostate ***********

dtl_iostate::dtl_iostate(dtl_state_enum state) :
      my_state(state) { }	

// conversion from int to dtl_iostate
dtl_iostate::dtl_iostate(int state) : my_state(dtl_state_enum(state)) { }

// conversion from dtl_iostate to int
dtl_iostate::operator int() const { return my_state; }

// ************ Implementation code for dtl_ios_base **********
// default constructor
dtl_ios_base::dtl_ios_base() : my_io_state(dtl_iostate::goodbit) { }

// exception-safe swap
void dtl_ios_base::swap(dtl_ios_base &other)
{
   STD_::swap(my_io_state, other.my_io_state);
}

// exception-safe assignment
dtl_ios_base &dtl_ios_base::operator=(const dtl_ios_base &other)
{
  if (this != &other)
  {
	 dtl_ios_base tmp(other);
	 swap(tmp);
  }

  return *this;
}


// return the state of the object
dtl_iostate dtl_ios_base::rdstate() const
{
  return my_io_state;
}
   
// *** functions that modify the state of the object
// also call the object's handler unless rdstate() == goodbit ***

// sets the state of the IO object, but doesn't clear any already set bits
void dtl_ios_base::setstate(dtl_iostate state)
{
  my_io_state = state | rdstate();
}

// clears the state and resets it to the state passed in
void dtl_ios_base::clear(dtl_iostate state)
{
  my_io_state = state;
}

// tests to indicate whether I am good, bad, or have failed
bool dtl_ios_base::good() const
{
  return (rdstate() == goodbit); // only good if state is zero
}

bool dtl_ios_base::bad() const
{
  return ((rdstate() & badbit) != 0);
}

bool dtl_ios_base::fail() const
{
  return (((rdstate() & failbit) != 0) || bad());
}

END_DTL_NAMESPACE
