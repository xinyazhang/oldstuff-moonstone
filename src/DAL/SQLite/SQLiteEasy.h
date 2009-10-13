#ifndef _REMUS_DAL_SQLITE_EASY_
#define _REMUS_DAL_SQLITE_EASY_

#inclde "../Domain.h"

void easyqurey(sqlite3* db, const uchar*);
void guarntee_query(sqlite3* db, const uchar*);
UniStr escape(sqlite3* db, const UniStr&);
void split_tag_row(sqlite3_stmt* stmt, Tag* tag);
UniStr split_tag_family(sqlite3_stmt* );
index_t split_tag_idx(sqlite3_stmt* );

#endif
