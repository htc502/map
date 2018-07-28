#ifndef PATHMARK_DB_H
#define PATHMARK_DB_H
/* return the index of (first) matched record, -1 if not found */
int pos(const char *pmark);
/* return correponding path of a pathmarker, NULL if not found */
const char* mark2path(const char *pmark);
const char* pos2path(int pos);
/* add a new record to db */
int add(const char *mark,const char *path);
/* remove a record to db */
int rm(int pos);
void printdb();
/* load db into mem.
0 if succed.
-1 if db file doesn't exist(will create one in turn) */
int opendb(const char *dbfname);
/* update db file and release db struct
0 if succed
-1 otherwise */
int closedb(const char *dbfname);
#endif
