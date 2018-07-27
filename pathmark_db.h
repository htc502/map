#ifndef PATHMARK_DB_H
#define PATHMARK_DB_H
int pos(const char *pmark);
const char* mark2path(const char *pmark);
const char* pos2path(int pos);
int add(const char *mark,const char *path);
int rm(int pos);
void printdb();
int opendb(const char *dbfname);
int closedb(const char *dbfname);
#endif
