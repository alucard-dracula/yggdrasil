//#include "DBView.h"
#include "DB_iterator.h"
BEGIN_DTL_NAMESPACE

static tstring trim (const tstring& s) // trim a macro?
{
  tstring::size_type
    pos1 = s.find_first_not_of (tstring::value_type (_TEXT(' '))),
    pos2 = s.find_last_not_of (tstring::value_type (_TEXT(' ')));

  if (pos1 == 0 && pos2 + 1 != s.size ( )) return s;

  return s.substr (pos1).substr (0, pos2 + 1 - pos1);
}

static tstring maybe_quote (const tstring& s, const tstring& q)
{
  tstring r (trim (s));

  if (r.find (tstring::value_type (_TEXT(' '))) != tstring::npos
      && r.find (q) != 0) {
    r.insert (0, q);
    r.append (q);
  }

  return r;
}

static void default_append_column (tstring& r, const tstring& c, const tstring& q)
{
  r.append (maybe_quote (c, q));
}

static void update_append_column (tstring& r, const tstring& c, const tstring& q)
{
  r.append (maybe_quote (c, q));
  r.append (_TEXT (" = (?) "));
}

typedef void (*append_column_t) (tstring&, const tstring&, const tstring&);

static tstring quoted_list (const STD_::vector<tstring>& l, const tstring& q,
	     append_column_t append_column = default_append_column)
{
  tstring r;

  if (l.empty ( )) return r;

  append_column (r, l.front ( ), q);

  for (STD_::vector<tstring>::const_iterator i = l.begin ( ) + 1,
	 x = l.end ( ); i != x; ++i) {
    r.append (_TEXT (", "));
    append_column (r, *i, q);
  }

  return r;
}

tstring MakeQryString( const tstring &postfixClause ,
	  const tstring &rawSQL ,
	  const STD_::set<tstring> & tableNames ,
	  const STD_::vector<tstring> & colNames ,
	  SQLQueryType qryType, const tstring &QuoteChar, const tstring &DBQuoteChar)
{
	  // built query return value
	  tstring Query;
	  Query.reserve(256);

	  // validate to make sure on INSERT, UPDATE, and DELETE that exactly one
      // table is being operated on
      // and for SELECT, at least one table is being selected from
      // then build query

	  switch (qryType)
	  {
	  case SQL:
		 Query = rawSQL;
		 break;

	  case SELECT: {
		 // SELECT colNames FROM tableNames postfixClause
		 if (tableNames.empty())
			DTL_THROW DBException(_TEXT("DBView::BuildQry()"),
				   _TEXT("SELECT : must select from at least one table"), NULL, NULL);

		 // Catch this error in MakeBindings()
		 // Need to leave this test out so that sql_iterator can work
		 //
		 //if (colNames.empty())
		 //	DTL_THROW DBException(_TEXT("DBView::BuildQry()"),
		 //		   _TEXT("SELECT : must select from at least one column"), NULL, NULL);

		 // build SELECT stmt into Query
		 Query += _TEXT("SELECT ");

		 // build comma-separated list of columns and tack on
		 // to the query
		 Query += quoted_list (colNames, QuoteChar);

		 Query += _TEXT(" FROM ");

		 // now add a comma-separated list of the tables
		 WriteDelimitedList(tableNames, _TEXT(""), _TEXT(""), _TEXT(", "), Query);

		 break;
		}
	  case INSERT: {
		 // INSERT INTO tableName (colName[0], colName[1], ...)
		 // VALUES (?, ?, ...) postfixClause

		 if (tableNames.size() != 1)
			DTL_THROW DBException(_TEXT("DBView::BuildQry()"),
				_TEXT("INSERT:  must insert into exactly one table"), NULL, NULL);

		 if (colNames.empty())
			DTL_THROW DBException(_TEXT("DBView::BuildQry()"),
				_TEXT("INSERT:  must insert with at least one column"), NULL, NULL);

		 // build INSERT stmt into Query
		 Query += _TEXT("INSERT INTO ");

		 // tack on table name
		 Query += *(tableNames.begin());

		 // tack on column names

		 Query += _TEXT(" (");
		 Query += quoted_list (colNames, QuoteChar);
		 Query += _TEXT(")");

		 // tack on appropriate number of question marks for values
		 Query += _TEXT(" VALUES (");

		 STD_::vector<tstring> questionMarks(colNames.size(), _TEXT("(?)"));

		 // build comma-separated list of colName=? entries and tack it on
		 WriteDelimitedList(questionMarks, Query);

		 Query += _TEXT(")");

		 break;
		}
	  case UPDATE: {
		 // UPDATE tableName SET colName[0]=?, colName[1]=?, ... postfixClause
		 if (tableNames.size() != 1)
			DTL_THROW DBException(_TEXT("DBView::BuildQry()"),
				_TEXT("UPDATE: must update in exactly one table"), NULL, NULL);

		 if (colNames.empty())
			DTL_THROW DBException(_TEXT("DBView::BuildQry()"),
				_TEXT("UPDATE: must update at least one column"), NULL, NULL);

		 // build UPDATE stmt into Query

		 Query += _TEXT("UPDATE ");

		 // tack on table name
		 Query += *(tableNames.begin());

		 Query += _TEXT(" SET ");

		 // build comma-separated list of colName=? entries and tack it on
		 Query += quoted_list
		   (colNames, QuoteChar, update_append_column);

		 break;
		}
	  case DEL: {
		 // DELETE FROM tableName WHERE colName[0] and colName[1] ... postfixClause
		 if (tableNames.size() != 1)
			DTL_THROW DBException(_TEXT("DBView::BuildQry()"),
				_TEXT("DELETE: must delete from exactly one table"), NULL, NULL);

		 // build DELETE stmt into Query

		 Query += _TEXT("DELETE FROM ");

		 // tack on table name
		 Query += *(tableNames.begin());

#ifndef FIX_NULL_DELETES
		 Query += _TEXT(" WHERE ");

		 // don't allow deletes to wipe out a whole table
		 if (colNames.empty())
			 DTL_THROW DBException(_TEXT("DBView::BuildQry()"),
			   _TEXT("DELETE: Must specify at least one column to filter on!"), NULL, NULL);

		 // now build colNames clause in form of
		 // colName1 = (?) AND colName2 = (?) AND colName3 = (?)
		 WriteDelimitedList(colNames, QuoteChar, QuoteChar + _TEXT(" = (?) "), _TEXT(" AND "), Query);
#endif

		 break;
		 }
	   default: // shouldn't happen ... throw exception if it does
		   DTL_THROW DBException(_TEXT("DBView::BuildQry()"),
			    _TEXT("Invalid statement type!"), NULL, NULL);
	  }

     // tack on postfix clause
     if (postfixClause.length() > 0 && qryType != SQL && qryType != INSERT)
			Query += _TEXT(" ") + postfixClause;

	 if (qryType != SQL)
		return QueryCaseify(Query, DBQuoteChar);
	 else
		return Query;
}

END_DTL_NAMESPACE

