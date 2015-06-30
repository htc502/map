#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include "pathmark_db.h"
#include "realpath_w32.h"

void help(char *pname)
{
  fprintf(stdout,"usage: %s [marker] [path]\n",pname);
}
int main(int argc, char **argv,char **envp)
{
  char dbname[]=".pathmark.db";
  char *home = NULL;
  home = getenv("HOME");
  if(home == NULL) {
    fprintf(stderr, "error get HOME path\n");
    exit(-1);
  }
  int dbflen = strlen(home) + 1 + strlen(dbname) + 1;
  char *dbfname =(char*)malloc(sizeof(char)*dbflen);
  strcpy(dbfname,home);
  strcat(dbfname,"/");
  strcat(dbfname,dbname);

  init();
  if(-1 == load(dbfname)) {
    free(dbfname);
    fprintf(stderr,"failed to initialize database, try again pls.\n");
    return(0);
  }

  char *xpath,*xmark;
  const char *path;
  switch(argc){
  case 1: /* print dbfname and help message */
    help(argv[0]);
    printdb();
    release();
    break;
  case 2:
    /* in case u want to use the index */
    /* index from 10-19 */
    if(strlen(argv[1]) == 2 && argv[1][0] == 49 \
       && argv[1][1] >= 48 && argv[1][1] <= 57) {
      char num = (argv[1][0] - 48)*10;
      num += (argv[1][1]-48)*1;
      path = pos2path(num);
      if(path != NULL)
	fprintf(stdout,"%s",path);
      release();
      break;
    }
    /* rm 10-19*/
    if(strlen(argv[1]) == 3 && argv[1][0] == 45 && \
       argv[1][1] == 49 && \
       argv[1][2] >= 48 && argv[1][2] <= 57) {
      char pos = (argv[1][1] - 48)*10;
      pos += (argv[1][2]-48)*1;
      if(-1 == rm(pos))
	fprintf(stderr,"remove record failed!\n");
      if(-1 == writedb(dbfname)) {
	fprintf(stderr,"writing database failed!\n");
	exit(-1);
      }
      release();
      break;
    }
    /* index from 0-9 */
    if(strlen(argv[1]) == 1 && \
       argv[1][0] >= 48 && argv[1][0] <= 57) {
      char num = argv[1][0] - 48;
      path = pos2path(num);
      if(path != NULL)
	fprintf(stdout,"%s",path);
      release();
      break;
    }
    /* rm 0-9 */
    if(strlen(argv[1]) == 2 && argv[1][0] == 45 && \
       argv[1][1] >= 48 && argv[1][1] <= 57) {
      char pos = argv[1][1] - 48;
      if(-1 == rm(pos))
	fprintf(stderr,"remove record failed!\n");
      if(-1 == writedb(dbfname)) {
	fprintf(stderr,"writing database failed!\n");
	exit(-1);
      }

      release();
      break;
    }
    /* use mark name */
    path =  mark2path(argv[1]);
    if(path != NULL)
      fprintf(stdout,"%s",path);
    release();
    break;
  case 3:
    /* add a new mark */
    xmark = argv[1];
    xpath = argv[2];
    char *rpath;
    rpath = realpath(xpath,NULL);
    if(NULL == rpath){
      fprintf(stderr,"invalid path:%s to bookmark\n",xpath);
      free(dbfname); //free dbfname name
      exit(-1);
    }
    if(-1 == add(xmark,rpath)) {
      fprintf(stderr,"error when add new record\n");
      free(dbfname); //free dbfname name
      free(rpath);
      exit(-1);
    }
    if(-1 == writedb(dbfname)) {
      fprintf(stderr,"writing database failed!\n");
      free(dbfname); //free dbfname name
      free(rpath);
      exit(-1);
    }
    free(rpath);
    release();
    break;
  default:
    help(argv[0]);
    release();
    break;
  }
  free(dbfname); //free dbfname name
  return(0);
}
