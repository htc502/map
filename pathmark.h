#ifndef PATHMARK_H
#define PATHMARK_H

#include <stdlib.h>
#include <string.h>

#define MAX_PATHLEN 1000
#define MAX_NPATH 20 /* max number of records */
#define NFIELD 2
#define DELIM "\t"

static struct {
  char *pathdb[MAX_NPATH][NFIELD];
  int NPATH;
} db_object;

void init();
int load(const char * dbfile);
int pos(const char *pmark);
const char* mark2path(const char *pmark);
const char* pos2path(int pos);
int add(const char *mark,const char *path);
int writedb(const char *file);
void printdb();

#endif
