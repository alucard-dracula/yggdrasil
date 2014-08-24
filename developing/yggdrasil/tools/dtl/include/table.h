#ifndef DTL_table_h
#define DTL_table_h

#include "DTL.h"

#define DTL_LESS(A, B, FIELD) \
	if ((A.FIELD) < (B.FIELD)) return true; \
	else if ((A.FIELD) > (B.FIELD)) return false;

#define DTL_BIND_FIELD(FIELD) \
	cols[_TEXT( #FIELD )] >> row.FIELD

///////////////////////////////////////////////////////////  
#define DTL_STRUCT1(STRUCT_NAME,TYPE1,FIELD1) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE1(TABLE_NAME,TYPE1,FIELD1) \
  DTL_STRUCT1(TABLE_NAME ## _row,TYPE1,FIELD1); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT2(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE2(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2) \
  DTL_STRUCT2(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT3(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE3(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3) \
  DTL_STRUCT3(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT4(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE4(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4) \
  DTL_STRUCT4(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT5(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE5(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5) \
  DTL_STRUCT5(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT6(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE6(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6) \
  DTL_STRUCT6(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT7(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE7(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7) \
  DTL_STRUCT7(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT8(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE8(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8) \
  DTL_STRUCT8(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT9(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE9(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9) \
  DTL_STRUCT9(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT10(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE10(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10) \
  DTL_STRUCT10(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT11(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE11(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11) \
  DTL_STRUCT11(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT12(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE12(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12) \
  DTL_STRUCT12(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT13(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE13(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13) \
  DTL_STRUCT13(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT14(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE14(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14) \
  DTL_STRUCT14(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT15(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE15(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15) \
  DTL_STRUCT15(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT16(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE16(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16) \
  DTL_STRUCT16(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT17(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE17(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17) \
  DTL_STRUCT17(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT18(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE18(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18) \
  DTL_STRUCT18(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT19(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE19(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19) \
  DTL_STRUCT19(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT20(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE20(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20) \
  DTL_STRUCT20(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT21(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE21(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21) \
  DTL_STRUCT21(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT22(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE22(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22) \
  DTL_STRUCT22(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT23(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE23(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23) \
  DTL_STRUCT23(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT24(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE24(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24) \
  DTL_STRUCT24(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT25(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE25(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25) \
  DTL_STRUCT25(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT26(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE26(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26) \
  DTL_STRUCT26(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT27(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE27(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27) \
  DTL_STRUCT27(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT28(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE28(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28) \
  DTL_STRUCT28(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT29(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE29(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29) \
  DTL_STRUCT29(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT30(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE30(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30) \
  DTL_STRUCT30(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT31(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE31(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31) \
  DTL_STRUCT31(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT32(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE32(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32) \
  DTL_STRUCT32(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT33(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE33(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33) \
  DTL_STRUCT33(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT34(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE34(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34) \
  DTL_STRUCT34(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT35(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE35(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35) \
  DTL_STRUCT35(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT36(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE36(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36) \
  DTL_STRUCT36(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT37(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE37(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37) \
  DTL_STRUCT37(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT38(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE38(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38) \
  DTL_STRUCT38(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT39(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE39(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39) \
  DTL_STRUCT39(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT40(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE40(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40) \
  DTL_STRUCT40(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT41(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
    TYPE41 FIELD41; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40  << _TEXT(", ") ; \
    o << s.FIELD41 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
    DTL_LESS (lhs, rhs, FIELD41); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
      DTL_BIND_FIELD(FIELD41); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE41(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41) \
  DTL_STRUCT41(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT42(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
    TYPE41 FIELD41; \
    TYPE42 FIELD42; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40  << _TEXT(", ") ; \
    o << s.FIELD41  << _TEXT(", ") ; \
    o << s.FIELD42 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
    DTL_LESS (lhs, rhs, FIELD41); \
    DTL_LESS (lhs, rhs, FIELD42); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
      DTL_BIND_FIELD(FIELD41); \
      DTL_BIND_FIELD(FIELD42); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE42(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42) \
  DTL_STRUCT42(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT43(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
    TYPE41 FIELD41; \
    TYPE42 FIELD42; \
    TYPE43 FIELD43; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40  << _TEXT(", ") ; \
    o << s.FIELD41  << _TEXT(", ") ; \
    o << s.FIELD42  << _TEXT(", ") ; \
    o << s.FIELD43 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
    DTL_LESS (lhs, rhs, FIELD41); \
    DTL_LESS (lhs, rhs, FIELD42); \
    DTL_LESS (lhs, rhs, FIELD43); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
      DTL_BIND_FIELD(FIELD41); \
      DTL_BIND_FIELD(FIELD42); \
      DTL_BIND_FIELD(FIELD43); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE43(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43) \
  DTL_STRUCT43(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT44(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
    TYPE41 FIELD41; \
    TYPE42 FIELD42; \
    TYPE43 FIELD43; \
    TYPE44 FIELD44; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40  << _TEXT(", ") ; \
    o << s.FIELD41  << _TEXT(", ") ; \
    o << s.FIELD42  << _TEXT(", ") ; \
    o << s.FIELD43  << _TEXT(", ") ; \
    o << s.FIELD44 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
    DTL_LESS (lhs, rhs, FIELD41); \
    DTL_LESS (lhs, rhs, FIELD42); \
    DTL_LESS (lhs, rhs, FIELD43); \
    DTL_LESS (lhs, rhs, FIELD44); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
      DTL_BIND_FIELD(FIELD41); \
      DTL_BIND_FIELD(FIELD42); \
      DTL_BIND_FIELD(FIELD43); \
      DTL_BIND_FIELD(FIELD44); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE44(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44) \
  DTL_STRUCT44(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT45(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
    TYPE41 FIELD41; \
    TYPE42 FIELD42; \
    TYPE43 FIELD43; \
    TYPE44 FIELD44; \
    TYPE45 FIELD45; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40  << _TEXT(", ") ; \
    o << s.FIELD41  << _TEXT(", ") ; \
    o << s.FIELD42  << _TEXT(", ") ; \
    o << s.FIELD43  << _TEXT(", ") ; \
    o << s.FIELD44  << _TEXT(", ") ; \
    o << s.FIELD45 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
    DTL_LESS (lhs, rhs, FIELD41); \
    DTL_LESS (lhs, rhs, FIELD42); \
    DTL_LESS (lhs, rhs, FIELD43); \
    DTL_LESS (lhs, rhs, FIELD44); \
    DTL_LESS (lhs, rhs, FIELD45); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
      DTL_BIND_FIELD(FIELD41); \
      DTL_BIND_FIELD(FIELD42); \
      DTL_BIND_FIELD(FIELD43); \
      DTL_BIND_FIELD(FIELD44); \
      DTL_BIND_FIELD(FIELD45); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE45(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45) \
  DTL_STRUCT45(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT46(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
    TYPE41 FIELD41; \
    TYPE42 FIELD42; \
    TYPE43 FIELD43; \
    TYPE44 FIELD44; \
    TYPE45 FIELD45; \
    TYPE46 FIELD46; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40  << _TEXT(", ") ; \
    o << s.FIELD41  << _TEXT(", ") ; \
    o << s.FIELD42  << _TEXT(", ") ; \
    o << s.FIELD43  << _TEXT(", ") ; \
    o << s.FIELD44  << _TEXT(", ") ; \
    o << s.FIELD45  << _TEXT(", ") ; \
    o << s.FIELD46 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
    DTL_LESS (lhs, rhs, FIELD41); \
    DTL_LESS (lhs, rhs, FIELD42); \
    DTL_LESS (lhs, rhs, FIELD43); \
    DTL_LESS (lhs, rhs, FIELD44); \
    DTL_LESS (lhs, rhs, FIELD45); \
    DTL_LESS (lhs, rhs, FIELD46); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
      DTL_BIND_FIELD(FIELD41); \
      DTL_BIND_FIELD(FIELD42); \
      DTL_BIND_FIELD(FIELD43); \
      DTL_BIND_FIELD(FIELD44); \
      DTL_BIND_FIELD(FIELD45); \
      DTL_BIND_FIELD(FIELD46); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE46(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46) \
  DTL_STRUCT46(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT47(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
    TYPE41 FIELD41; \
    TYPE42 FIELD42; \
    TYPE43 FIELD43; \
    TYPE44 FIELD44; \
    TYPE45 FIELD45; \
    TYPE46 FIELD46; \
    TYPE47 FIELD47; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40  << _TEXT(", ") ; \
    o << s.FIELD41  << _TEXT(", ") ; \
    o << s.FIELD42  << _TEXT(", ") ; \
    o << s.FIELD43  << _TEXT(", ") ; \
    o << s.FIELD44  << _TEXT(", ") ; \
    o << s.FIELD45  << _TEXT(", ") ; \
    o << s.FIELD46  << _TEXT(", ") ; \
    o << s.FIELD47 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
    DTL_LESS (lhs, rhs, FIELD41); \
    DTL_LESS (lhs, rhs, FIELD42); \
    DTL_LESS (lhs, rhs, FIELD43); \
    DTL_LESS (lhs, rhs, FIELD44); \
    DTL_LESS (lhs, rhs, FIELD45); \
    DTL_LESS (lhs, rhs, FIELD46); \
    DTL_LESS (lhs, rhs, FIELD47); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
      DTL_BIND_FIELD(FIELD41); \
      DTL_BIND_FIELD(FIELD42); \
      DTL_BIND_FIELD(FIELD43); \
      DTL_BIND_FIELD(FIELD44); \
      DTL_BIND_FIELD(FIELD45); \
      DTL_BIND_FIELD(FIELD46); \
      DTL_BIND_FIELD(FIELD47); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE47(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47) \
  DTL_STRUCT47(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT48(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47,TYPE48,FIELD48) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
    TYPE41 FIELD41; \
    TYPE42 FIELD42; \
    TYPE43 FIELD43; \
    TYPE44 FIELD44; \
    TYPE45 FIELD45; \
    TYPE46 FIELD46; \
    TYPE47 FIELD47; \
    TYPE48 FIELD48; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40  << _TEXT(", ") ; \
    o << s.FIELD41  << _TEXT(", ") ; \
    o << s.FIELD42  << _TEXT(", ") ; \
    o << s.FIELD43  << _TEXT(", ") ; \
    o << s.FIELD44  << _TEXT(", ") ; \
    o << s.FIELD45  << _TEXT(", ") ; \
    o << s.FIELD46  << _TEXT(", ") ; \
    o << s.FIELD47  << _TEXT(", ") ; \
    o << s.FIELD48 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
    DTL_LESS (lhs, rhs, FIELD41); \
    DTL_LESS (lhs, rhs, FIELD42); \
    DTL_LESS (lhs, rhs, FIELD43); \
    DTL_LESS (lhs, rhs, FIELD44); \
    DTL_LESS (lhs, rhs, FIELD45); \
    DTL_LESS (lhs, rhs, FIELD46); \
    DTL_LESS (lhs, rhs, FIELD47); \
    DTL_LESS (lhs, rhs, FIELD48); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
      DTL_BIND_FIELD(FIELD41); \
      DTL_BIND_FIELD(FIELD42); \
      DTL_BIND_FIELD(FIELD43); \
      DTL_BIND_FIELD(FIELD44); \
      DTL_BIND_FIELD(FIELD45); \
      DTL_BIND_FIELD(FIELD46); \
      DTL_BIND_FIELD(FIELD47); \
      DTL_BIND_FIELD(FIELD48); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE48(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47,TYPE48,FIELD48) \
  DTL_STRUCT48(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47,TYPE48,FIELD48); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT49(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47,TYPE48,FIELD48,TYPE49,FIELD49) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
    TYPE41 FIELD41; \
    TYPE42 FIELD42; \
    TYPE43 FIELD43; \
    TYPE44 FIELD44; \
    TYPE45 FIELD45; \
    TYPE46 FIELD46; \
    TYPE47 FIELD47; \
    TYPE48 FIELD48; \
    TYPE49 FIELD49; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40  << _TEXT(", ") ; \
    o << s.FIELD41  << _TEXT(", ") ; \
    o << s.FIELD42  << _TEXT(", ") ; \
    o << s.FIELD43  << _TEXT(", ") ; \
    o << s.FIELD44  << _TEXT(", ") ; \
    o << s.FIELD45  << _TEXT(", ") ; \
    o << s.FIELD46  << _TEXT(", ") ; \
    o << s.FIELD47  << _TEXT(", ") ; \
    o << s.FIELD48  << _TEXT(", ") ; \
    o << s.FIELD49 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
    DTL_LESS (lhs, rhs, FIELD41); \
    DTL_LESS (lhs, rhs, FIELD42); \
    DTL_LESS (lhs, rhs, FIELD43); \
    DTL_LESS (lhs, rhs, FIELD44); \
    DTL_LESS (lhs, rhs, FIELD45); \
    DTL_LESS (lhs, rhs, FIELD46); \
    DTL_LESS (lhs, rhs, FIELD47); \
    DTL_LESS (lhs, rhs, FIELD48); \
    DTL_LESS (lhs, rhs, FIELD49); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
      DTL_BIND_FIELD(FIELD41); \
      DTL_BIND_FIELD(FIELD42); \
      DTL_BIND_FIELD(FIELD43); \
      DTL_BIND_FIELD(FIELD44); \
      DTL_BIND_FIELD(FIELD45); \
      DTL_BIND_FIELD(FIELD46); \
      DTL_BIND_FIELD(FIELD47); \
      DTL_BIND_FIELD(FIELD48); \
      DTL_BIND_FIELD(FIELD49); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE49(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47,TYPE48,FIELD48,TYPE49,FIELD49) \
  DTL_STRUCT49(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47,TYPE48,FIELD48,TYPE49,FIELD49); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



///////////////////////////////////////////////////////////  
#define DTL_STRUCT50(STRUCT_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47,TYPE48,FIELD48,TYPE49,FIELD49,TYPE50,FIELD50) \
  struct STRUCT_NAME { \
    TYPE1 FIELD1; \
    TYPE2 FIELD2; \
    TYPE3 FIELD3; \
    TYPE4 FIELD4; \
    TYPE5 FIELD5; \
    TYPE6 FIELD6; \
    TYPE7 FIELD7; \
    TYPE8 FIELD8; \
    TYPE9 FIELD9; \
    TYPE10 FIELD10; \
    TYPE11 FIELD11; \
    TYPE12 FIELD12; \
    TYPE13 FIELD13; \
    TYPE14 FIELD14; \
    TYPE15 FIELD15; \
    TYPE16 FIELD16; \
    TYPE17 FIELD17; \
    TYPE18 FIELD18; \
    TYPE19 FIELD19; \
    TYPE20 FIELD20; \
    TYPE21 FIELD21; \
    TYPE22 FIELD22; \
    TYPE23 FIELD23; \
    TYPE24 FIELD24; \
    TYPE25 FIELD25; \
    TYPE26 FIELD26; \
    TYPE27 FIELD27; \
    TYPE28 FIELD28; \
    TYPE29 FIELD29; \
    TYPE30 FIELD30; \
    TYPE31 FIELD31; \
    TYPE32 FIELD32; \
    TYPE33 FIELD33; \
    TYPE34 FIELD34; \
    TYPE35 FIELD35; \
    TYPE36 FIELD36; \
    TYPE37 FIELD37; \
    TYPE38 FIELD38; \
    TYPE39 FIELD39; \
    TYPE40 FIELD40; \
    TYPE41 FIELD41; \
    TYPE42 FIELD42; \
    TYPE43 FIELD43; \
    TYPE44 FIELD44; \
    TYPE45 FIELD45; \
    TYPE46 FIELD46; \
    TYPE47 FIELD47; \
    TYPE48 FIELD48; \
    TYPE49 FIELD49; \
    TYPE50 FIELD50; \
  }; \
  \
  dtl::tostream &operator<<(dtl::tostream &o, const STRUCT_NAME &s) \
  { \
    o <<  _TEXT("("); \
    o << s.FIELD1  << _TEXT(", ") ; \
    o << s.FIELD2  << _TEXT(", ") ; \
    o << s.FIELD3  << _TEXT(", ") ; \
    o << s.FIELD4  << _TEXT(", ") ; \
    o << s.FIELD5  << _TEXT(", ") ; \
    o << s.FIELD6  << _TEXT(", ") ; \
    o << s.FIELD7  << _TEXT(", ") ; \
    o << s.FIELD8  << _TEXT(", ") ; \
    o << s.FIELD9  << _TEXT(", ") ; \
    o << s.FIELD10  << _TEXT(", ") ; \
    o << s.FIELD11  << _TEXT(", ") ; \
    o << s.FIELD12  << _TEXT(", ") ; \
    o << s.FIELD13  << _TEXT(", ") ; \
    o << s.FIELD14  << _TEXT(", ") ; \
    o << s.FIELD15  << _TEXT(", ") ; \
    o << s.FIELD16  << _TEXT(", ") ; \
    o << s.FIELD17  << _TEXT(", ") ; \
    o << s.FIELD18  << _TEXT(", ") ; \
    o << s.FIELD19  << _TEXT(", ") ; \
    o << s.FIELD20  << _TEXT(", ") ; \
    o << s.FIELD21  << _TEXT(", ") ; \
    o << s.FIELD22  << _TEXT(", ") ; \
    o << s.FIELD23  << _TEXT(", ") ; \
    o << s.FIELD24  << _TEXT(", ") ; \
    o << s.FIELD25  << _TEXT(", ") ; \
    o << s.FIELD26  << _TEXT(", ") ; \
    o << s.FIELD27  << _TEXT(", ") ; \
    o << s.FIELD28  << _TEXT(", ") ; \
    o << s.FIELD29  << _TEXT(", ") ; \
    o << s.FIELD30  << _TEXT(", ") ; \
    o << s.FIELD31  << _TEXT(", ") ; \
    o << s.FIELD32  << _TEXT(", ") ; \
    o << s.FIELD33  << _TEXT(", ") ; \
    o << s.FIELD34  << _TEXT(", ") ; \
    o << s.FIELD35  << _TEXT(", ") ; \
    o << s.FIELD36  << _TEXT(", ") ; \
    o << s.FIELD37  << _TEXT(", ") ; \
    o << s.FIELD38  << _TEXT(", ") ; \
    o << s.FIELD39  << _TEXT(", ") ; \
    o << s.FIELD40  << _TEXT(", ") ; \
    o << s.FIELD41  << _TEXT(", ") ; \
    o << s.FIELD42  << _TEXT(", ") ; \
    o << s.FIELD43  << _TEXT(", ") ; \
    o << s.FIELD44  << _TEXT(", ") ; \
    o << s.FIELD45  << _TEXT(", ") ; \
    o << s.FIELD46  << _TEXT(", ") ; \
    o << s.FIELD47  << _TEXT(", ") ; \
    o << s.FIELD48  << _TEXT(", ") ; \
    o << s.FIELD49  << _TEXT(", ") ; \
    o << s.FIELD50 ; \
    o << _TEXT(")"); \
    return o; \
  }; \
  \
  bool operator<(const STRUCT_NAME &lhs, const STRUCT_NAME &rhs) { \
    DTL_LESS (lhs, rhs, FIELD1); \
    DTL_LESS (lhs, rhs, FIELD2); \
    DTL_LESS (lhs, rhs, FIELD3); \
    DTL_LESS (lhs, rhs, FIELD4); \
    DTL_LESS (lhs, rhs, FIELD5); \
    DTL_LESS (lhs, rhs, FIELD6); \
    DTL_LESS (lhs, rhs, FIELD7); \
    DTL_LESS (lhs, rhs, FIELD8); \
    DTL_LESS (lhs, rhs, FIELD9); \
    DTL_LESS (lhs, rhs, FIELD10); \
    DTL_LESS (lhs, rhs, FIELD11); \
    DTL_LESS (lhs, rhs, FIELD12); \
    DTL_LESS (lhs, rhs, FIELD13); \
    DTL_LESS (lhs, rhs, FIELD14); \
    DTL_LESS (lhs, rhs, FIELD15); \
    DTL_LESS (lhs, rhs, FIELD16); \
    DTL_LESS (lhs, rhs, FIELD17); \
    DTL_LESS (lhs, rhs, FIELD18); \
    DTL_LESS (lhs, rhs, FIELD19); \
    DTL_LESS (lhs, rhs, FIELD20); \
    DTL_LESS (lhs, rhs, FIELD21); \
    DTL_LESS (lhs, rhs, FIELD22); \
    DTL_LESS (lhs, rhs, FIELD23); \
    DTL_LESS (lhs, rhs, FIELD24); \
    DTL_LESS (lhs, rhs, FIELD25); \
    DTL_LESS (lhs, rhs, FIELD26); \
    DTL_LESS (lhs, rhs, FIELD27); \
    DTL_LESS (lhs, rhs, FIELD28); \
    DTL_LESS (lhs, rhs, FIELD29); \
    DTL_LESS (lhs, rhs, FIELD30); \
    DTL_LESS (lhs, rhs, FIELD31); \
    DTL_LESS (lhs, rhs, FIELD32); \
    DTL_LESS (lhs, rhs, FIELD33); \
    DTL_LESS (lhs, rhs, FIELD34); \
    DTL_LESS (lhs, rhs, FIELD35); \
    DTL_LESS (lhs, rhs, FIELD36); \
    DTL_LESS (lhs, rhs, FIELD37); \
    DTL_LESS (lhs, rhs, FIELD38); \
    DTL_LESS (lhs, rhs, FIELD39); \
    DTL_LESS (lhs, rhs, FIELD40); \
    DTL_LESS (lhs, rhs, FIELD41); \
    DTL_LESS (lhs, rhs, FIELD42); \
    DTL_LESS (lhs, rhs, FIELD43); \
    DTL_LESS (lhs, rhs, FIELD44); \
    DTL_LESS (lhs, rhs, FIELD45); \
    DTL_LESS (lhs, rhs, FIELD46); \
    DTL_LESS (lhs, rhs, FIELD47); \
    DTL_LESS (lhs, rhs, FIELD48); \
    DTL_LESS (lhs, rhs, FIELD49); \
    DTL_LESS (lhs, rhs, FIELD50); \
 	  return false; \
  } \
  \
  BEGIN_DTL_NAMESPACE \
  template<> class DefaultBCA<STRUCT_NAME> \
  { \
  public:\
    void operator()(BoundIOs &cols, STRUCT_NAME &row) \
    { \
      DTL_BIND_FIELD(FIELD1); \
      DTL_BIND_FIELD(FIELD2); \
      DTL_BIND_FIELD(FIELD3); \
      DTL_BIND_FIELD(FIELD4); \
      DTL_BIND_FIELD(FIELD5); \
      DTL_BIND_FIELD(FIELD6); \
      DTL_BIND_FIELD(FIELD7); \
      DTL_BIND_FIELD(FIELD8); \
      DTL_BIND_FIELD(FIELD9); \
      DTL_BIND_FIELD(FIELD10); \
      DTL_BIND_FIELD(FIELD11); \
      DTL_BIND_FIELD(FIELD12); \
      DTL_BIND_FIELD(FIELD13); \
      DTL_BIND_FIELD(FIELD14); \
      DTL_BIND_FIELD(FIELD15); \
      DTL_BIND_FIELD(FIELD16); \
      DTL_BIND_FIELD(FIELD17); \
      DTL_BIND_FIELD(FIELD18); \
      DTL_BIND_FIELD(FIELD19); \
      DTL_BIND_FIELD(FIELD20); \
      DTL_BIND_FIELD(FIELD21); \
      DTL_BIND_FIELD(FIELD22); \
      DTL_BIND_FIELD(FIELD23); \
      DTL_BIND_FIELD(FIELD24); \
      DTL_BIND_FIELD(FIELD25); \
      DTL_BIND_FIELD(FIELD26); \
      DTL_BIND_FIELD(FIELD27); \
      DTL_BIND_FIELD(FIELD28); \
      DTL_BIND_FIELD(FIELD29); \
      DTL_BIND_FIELD(FIELD30); \
      DTL_BIND_FIELD(FIELD31); \
      DTL_BIND_FIELD(FIELD32); \
      DTL_BIND_FIELD(FIELD33); \
      DTL_BIND_FIELD(FIELD34); \
      DTL_BIND_FIELD(FIELD35); \
      DTL_BIND_FIELD(FIELD36); \
      DTL_BIND_FIELD(FIELD37); \
      DTL_BIND_FIELD(FIELD38); \
      DTL_BIND_FIELD(FIELD39); \
      DTL_BIND_FIELD(FIELD40); \
      DTL_BIND_FIELD(FIELD41); \
      DTL_BIND_FIELD(FIELD42); \
      DTL_BIND_FIELD(FIELD43); \
      DTL_BIND_FIELD(FIELD44); \
      DTL_BIND_FIELD(FIELD45); \
      DTL_BIND_FIELD(FIELD46); \
      DTL_BIND_FIELD(FIELD47); \
      DTL_BIND_FIELD(FIELD48); \
      DTL_BIND_FIELD(FIELD49); \
      DTL_BIND_FIELD(FIELD50); \
    } \
  };\
  END_DTL_NAMESPACE

//---------------------------------------------------------  
#define DTL_TABLE50(TABLE_NAME,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47,TYPE48,FIELD48,TYPE49,FIELD49,TYPE50,FIELD50) \
  DTL_STRUCT50(TABLE_NAME ## _row,TYPE1,FIELD1,TYPE2,FIELD2,TYPE3,FIELD3,TYPE4,FIELD4,TYPE5,FIELD5,TYPE6,FIELD6,TYPE7,FIELD7,TYPE8,FIELD8,TYPE9,FIELD9,TYPE10,FIELD10,TYPE11,FIELD11,TYPE12,FIELD12,TYPE13,FIELD13,TYPE14,FIELD14,TYPE15,FIELD15,TYPE16,FIELD16,TYPE17,FIELD17,TYPE18,FIELD18,TYPE19,FIELD19,TYPE20,FIELD20,TYPE21,FIELD21,TYPE22,FIELD22,TYPE23,FIELD23,TYPE24,FIELD24,TYPE25,FIELD25,TYPE26,FIELD26,TYPE27,FIELD27,TYPE28,FIELD28,TYPE29,FIELD29,TYPE30,FIELD30,TYPE31,FIELD31,TYPE32,FIELD32,TYPE33,FIELD33,TYPE34,FIELD34,TYPE35,FIELD35,TYPE36,FIELD36,TYPE37,FIELD37,TYPE38,FIELD38,TYPE39,FIELD39,TYPE40,FIELD40,TYPE41,FIELD41,TYPE42,FIELD42,TYPE43,FIELD43,TYPE44,FIELD44,TYPE45,FIELD45,TYPE46,FIELD46,TYPE47,FIELD47,TYPE48,FIELD48,TYPE49,FIELD49,TYPE50,FIELD50); \
  typedef dtl::DBView<TABLE_NAME ## _row> TABLE_NAME ## _view; \
  TABLE_NAME ## _view TABLE_NAME(_TEXT( #TABLE_NAME ))



#endif
