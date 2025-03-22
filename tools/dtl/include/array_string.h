#ifndef ARRAY_STRING_H_
#define ARRAY_STRING_H_

#include "dtl_config.h"
#include "clib_fwd.h"
#include "date_util.h"
#include "CountedPtr.h"

BEGIN_DTL_NAMESPACE

// implement blob class to hold a counted pointer to a string of type
// STD_::basic_string<BYTE> blob.  The reason that we hold a pointer
// here is that typically we expect blob data to be large so that
// we want to minimize copies unless absolutely necessary.

class blob
{
public:
	typedef STD_::basic_string<BYTE> blob_data;

	typedef blob_data::iterator iterator;
	typedef blob_data::const_iterator const_iterator;

private:
	CountedPtr<blob_data> bdata;

public:
	blob() : bdata(new blob_data()) {}
	blob(const blob &other) : bdata(other.bdata) {}
	void swap(blob &other) {bdata.swap(other.bdata);}

	// forward string functions
	blob & append(const BYTE *s, size_t n) {bdata->append(s, n); return *this;}
	blob & append(const BYTE *s) {bdata->append(s); return *this;}
	blob & append(size_t n, BYTE b) {bdata->append(n, b); return *this;}
	// here we use BYTE *first, instead of templating on a const_iterator since this is all that MSVC 6 dinkum supports
	blob & append(const BYTE *first, const BYTE *last) {bdata->append(first, last); return *this;}

	blob & assign(const blob &s) {bdata->assign(*(s.bdata)); return *this;}
	blob & assign(const blob &s, size_t posn, size_t n) {bdata->assign(*(s.bdata), posn, n); return *this;}
	blob & assign(const BYTE *s, size_t n) {bdata->assign(s, n); return *this;}
	blob & assign(const BYTE *s) {bdata->assign(s); return *this;}
	blob & assign(size_t n, BYTE c) {bdata->assign(n, c); return *this;}
	blob & assign(const BYTE *first, const BYTE *last) {bdata->assign(first, last); return *this;}

	const_iterator begin() const {return bdata->begin();}
	iterator begin() {return bdata->begin();}
	size_t capacity() const {return bdata->capacity();}
	int compare (const blob &s) const {return bdata->compare(*(s.bdata));}
	size_t copy(BYTE *s, size_t n, size_t posn = 0) {return bdata->copy(s, n, posn);}
	const BYTE * data() const {return bdata->data();}
	bool empty() const { return bdata->empty();}
	const_iterator end() const {return bdata->end();}
	iterator end() {return bdata->end();}
	size_t length() const {return bdata->size();}
	void reserve(size_t t) {bdata->reserve(t);}
	size_t size() const {return bdata->size();}

	blob & operator= (const blob &other) {bdata = other.bdata; return *this;}
	blob & operator+= (BYTE *s) {*bdata += s; return *this;}
	blob & operator+= (BYTE b) {*bdata += b; return *this;}
	const BYTE & operator[] (size_t posn) const {return (*bdata)[posn];}
	BYTE & operator[] (size_t posn) {return (*bdata)[posn];}

	~blob() {};
};

template<size_t N>
class tcstring
{
  TCHAR buf[N + 1];

private:
  void copy_terminate(const TCHAR *other) {
    std_tstrncpy(buf, other, N); // std_tstrncpy does not null terminate
    // if std_tstrlen(other) >= N
    buf[N] = 0;
  };

public:
  typedef TCHAR char_type;
  typedef TCHAR value_type;
  typedef size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef TCHAR* pointer;
  typedef const TCHAR* const_pointer;
  typedef TCHAR& reference;
  typedef const TCHAR& const_reference;
  typedef TCHAR* iterator;
  typedef const TCHAR* const_iterator;
#ifdef _MSC_VER // M$
    #if _MSC_VER < 1300 || defined(__SGI_STL_PORT)
		typedef STD_::reverse_iterator<iterator, value_type>
		reverse_iterator;
		typedef STD_::reverse_iterator<const_iterator, value_type>
		const_reverse_iterator;
	#else
		typedef STD_::reverse_iterator<iterator> reverse_iterator;
		typedef STD_::reverse_iterator<const_iterator> const_reverse_iterator;
	#endif
#else
  typedef STD_::reverse_iterator<iterator> reverse_iterator;
  typedef STD_::reverse_iterator<const_iterator> const_reverse_iterator;
#endif

  inline tcstring()  { buf[0]=0; }

  // convert from a tcstring of different size ...
  // will truncate any tcstrings of longer length to size N
  template<size_t N2> inline tcstring(const tcstring<N2>& that)
  { copy_terminate (that.c_str()); }

  inline tcstring(const tcstring& that)
  { copy_terminate (that.buf);}


  inline tcstring(const TCHAR* s)
  { copy_terminate (s); }

  inline tcstring(const STD_::basic_string<TCHAR>& s)
  { copy_terminate(s.c_str ( )); }

  tcstring& operator=(const STD_::basic_string<TCHAR>& s)
  {
    copy_terminate(s.c_str ( ));
    return *this;
  }

  tcstring& operator=(const TCHAR* s)
  {
    copy_terminate(s);
    return *this;
  }

  // assign from a tcstring of different size ...
  // will truncate any tcstrings of longer length to size N
  template<size_t N2> tcstring& operator=(const tcstring<N2> &that)
  {
    copy_terminate(that.c_str());
    return *this;
  }

  tcstring& operator=(const tcstring &that)
  {
    copy_terminate(that.buf);
    return *this;
  }

  inline iterator begin() { return buf; }

  inline const_iterator begin() const { return buf; }

  inline iterator end() { return buf + length(); }

  inline const_iterator end() const { return buf + length ( ); }

  // used to conflict with TCHAR * operator (NOT anymore)
  inline TCHAR operator[] (size_type pos) { return buf[pos]; }

  inline const TCHAR* c_str ( ) const { return buf; }
  inline const TCHAR* data ( ) const { return buf; }

  inline size_type length ( ) const { return std_tstrlen (buf); }

  inline size_type size ( ) const { return length ( ); }

  inline size_type max_size ( ) const { return N; }

  inline size_type capacity ( ) const { return N; }


  inline void clear ( ) { buf[0] = 0; }

  inline bool empty ( ) const { return buf[0] == 0; }

  template<size_t N2> inline int compare (const tcstring<N2>& rhs) const
  { return std_tstrcmp (buf, rhs.buf); }

  inline int compare (const STD_::basic_string<TCHAR>& s) const
  { return std_tstrcmp (buf, s.c_str()); }

  inline int compare (const TCHAR* s) const
  { return std_tstrcmp(buf, s); }

  template<size_t N2> inline bool operator== (const tcstring<N2>& rhs)
  { return compare (rhs) == 0; }

  template<size_t N2> inline bool operator!= (const tcstring<N2>& rhs)
  { return compare (rhs) != 0; }

  template<size_t N2> inline bool operator< (const tcstring<N2>& rhs)
  { return compare (rhs) < 0; }

  template<size_t N2> inline bool operator<= (const tcstring<N2>& rhs)
  { return compare (rhs) <= 0; }

  template<size_t N2> inline bool operator> (const tcstring<N2>& rhs)
  { return compare (rhs) > 0; }

  template<size_t N2> inline bool operator>= (const tcstring<N2>& rhs)
  { return compare (rhs) >= 0; }

  inline operator STD_::basic_string<TCHAR> ( ) const
  { return STD_::basic_string<TCHAR>(buf); }
};

template<> class tcstring<1>
{
  TCHAR buf[2];

private:
  void copy_terminate (const TCHAR* other)
  { buf[0] = other[0]; buf[1] = 0; };

public:
  typedef TCHAR char_type;
  typedef TCHAR value_type;
  typedef size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef TCHAR* pointer;
  typedef const TCHAR* const_pointer;
  typedef TCHAR& reference;
  typedef const TCHAR& const_reference;
  typedef TCHAR* iterator;
  typedef const TCHAR* const_iterator;

#ifdef _MSC_VER // M$
	#if _MSC_VER < 1300 || defined(__SGI_STL_PORT)

		typedef STD_::reverse_iterator<iterator, value_type>
		reverse_iterator;
		typedef STD_::reverse_iterator<const_iterator, value_type>
		const_reverse_iterator;
	#else
		typedef STD_::reverse_iterator<iterator> reverse_iterator;
		typedef STD_::reverse_iterator<const_iterator> const_reverse_iterator;
	#endif
#else
  typedef STD_::reverse_iterator<iterator> reverse_iterator;
  typedef STD_::reverse_iterator<const_iterator> const_reverse_iterator;
#endif

  inline tcstring ( ) { buf[0] = 0; }

  template<size_t N2> inline tcstring (const tcstring<N2>& that)
  { copy_terminate (that.buf); }

  inline tcstring(const tcstring& that)
  { copy_terminate (that.buf); }

  inline tcstring (const TCHAR* s)
  { copy_terminate (s); }

  inline tcstring (const STD_::basic_string<TCHAR>& s)
  { copy_terminate (s.c_str ( )); }

  tcstring& operator= (const STD_::basic_string<TCHAR>& s)
  {
    copy_terminate (s.c_str ( ));
    return *this;
  }

  tcstring& operator= (const TCHAR* s)
  {
    copy_terminate (s);
    return *this;
  }

  tcstring<1>& operator= (const tcstring<1>& that)
  {
    copy_terminate (that.buf);
    return *this;
  }

  inline iterator begin ( ) { return buf; }

  inline const_iterator begin ( ) const { return buf; }

  inline iterator end ( ) { return buf + length ( ); }

  inline const_iterator end ( ) const { return buf + length ( ); }

  // conflicts with TCHAR * operator (not anymore)
  inline TCHAR operator[] (size_type pos) { return buf[pos]; }

  inline const TCHAR* c_str ( ) const { return buf; }

  inline const TCHAR* data ( ) const { return buf; }

  inline size_type length ( ) const { if (*buf) return 1; return 0; }

  inline size_type size ( ) const { return length ( ); }

  inline size_type max_size ( ) const { return 1; }

  inline size_type capacity ( ) const { return 1; }

  inline void clear ( ) { buf[0] = 0; }

  inline bool empty ( ) const { return buf[0] == 0; }

private:
  int simple_compare (const TCHAR* s) const
  {
    if (buf[0] < s[0]) return -1;
    if (buf[0] > s[0]) return 1;
    // The first case is both strings empty; the second case is both
    // strings equal and 1 character long.
    if (s[0] == 0 || s[1] == 0) return 0;

    // Only reach here if first characters equal and rhs longer.
    return -1;
  }

public:
  template<size_t N2> inline int compare (const tcstring<N2>& rhs) const
  { return simple_compare (rhs.buf); }

  inline int compare (const STD_::basic_string<TCHAR>& s) const
  { return simple_compare (s.c_str ( )); }

  inline int compare (const TCHAR* s) const
  { return simple_compare (s); }

  template<size_t N2> inline bool operator== (const tcstring<N2>& rhs)
  { return compare (rhs) == 0; }

  template<size_t N2> inline bool operator!= (const tcstring<N2>& rhs)
  { return compare (rhs) != 0; }

  template<size_t N2> inline bool operator< (const tcstring<N2>& rhs)
  { return compare (rhs) < 0; }

  template<size_t N2> inline bool operator<= (const tcstring<N2>& rhs)
  { return compare (rhs) <= 0; }

  template<size_t N2> inline bool operator> (const tcstring<N2>& rhs)
  { return compare (rhs) > 0; }

  template<size_t N2> inline bool operator>= (const tcstring<N2>& rhs)
  { return compare (rhs) >= 0; }

  // remove these next operators.  it causes ambiguity with the previous operators.
  // force the use of c_str() just like in the standard string class
  // operator const TCHAR *() const { return buf; }
  // operator TCHAR *() { return buf; }

  inline operator STD_::basic_string<TCHAR> ( ) const
  { return STD_::basic_string<TCHAR> (buf); }

  // The only thing this specialization does is provide char
  // constructors and conversion for easy support of char ->
  // tcstring<1> replacement.
  tcstring (TCHAR ch) { buf[0] = ch; buf[1] = 0; }
  tcstring& operator= (const TCHAR ch)
  { buf[0] = ch; buf[1] = 0; return *this; }
  inline operator TCHAR ( ) const { return buf[0]; }
};

#define COMPARISON_OPS(OP) \
  template<size_t N> \
  inline \
  bool \
  operator OP (const tcstring<N>& lhs, \
  	    const tcstring<N>& rhs) \
  { return lhs.compare (rhs) OP 0; } \
 \
  template<size_t N> \
  inline \
  bool \
  operator OP (const tcstring<N>& lhs, \
  	    const STD_::basic_string<TCHAR>& rhs) \
  { return lhs.compare (rhs) OP 0; } \
 \
  template<size_t N> \
  inline \
  bool \
  operator OP (const STD_::basic_string<TCHAR>& lhs, \
  	    const tcstring<N>& rhs) \
  { return (rhs.compare (lhs) * -1) OP 0; } \
 \
  template<size_t N> \
  inline \
  bool \
  operator OP (const tcstring<N>& lhs, const TCHAR* rhs) \
  { return lhs.compare (rhs) OP 0; } \
 \
  template<size_t N> inline bool operator OP (const TCHAR* lhs, const tcstring<N>& rhs) \
  { return (rhs.compare (lhs) * -1) OP 0; } // *-1 to reverse the sign of the result

COMPARISON_OPS(==)
COMPARISON_OPS(!=)
COMPARISON_OPS(<)
COMPARISON_OPS(<=)
COMPARISON_OPS(>)
COMPARISON_OPS(>=)
#undef COMPARISON_OPS

template<size_t N>
inline
tostream& operator<< (tostream& os, const tcstring<N>& rhs)
{ return os.write (rhs.data ( ), rhs.length ( )); }

// returns whether the object is a tcstring ... templated form below causes ambiguity
#if 0
template<class T> bool is_tcstring(const T &t)
{
	return false;
}
#endif

template<size_t N> bool is_tcstring(const tcstring<N> &tcstr)
{
	return true;
}

// instead use cast of thousands vs. data fields we support
#define DTL_ISTCSTRING(type) \
	inline bool is_tcstring(const type &t) \
	{ return false; }

// cast of thousands
DTL_ISTCSTRING(short);
DTL_ISTCSTRING(int);
DTL_ISTCSTRING(unsigned int);
DTL_ISTCSTRING(long);
DTL_ISTCSTRING(unsigned long);
DTL_ISTCSTRING(double);
DTL_ISTCSTRING(float);
DTL_ISTCSTRING(TIMESTAMP_STRUCT);
DTL_ISTCSTRING(TIME_STRUCT);
DTL_ISTCSTRING(DATE_STRUCT);
DTL_ISTCSTRING(blob);
DTL_ISTCSTRING(jtime_c);
DTL_ISTCSTRING(bool);
DTL_ISTCSTRING(TCHAR);
DTL_ISTCSTRING(TCHAR * const);
DTL_ISTCSTRING(tstring);

inline bool is_tcstring(TCHAR * const &t)
{ return false; }

END_DTL_NAMESPACE

#endif // ARRAY_STRING_H_
