#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include "pathmark.h"

void help(char *pname)
{
  fprintf(stdout,"usage: %s [marker] [path]\n",pname);
}
int main(int argc, char **argv,char **envp)
{
  char dbname[]=".pathmark.db";
  char *home = getenv("HOME");
  char *dbfile =(char*)malloc(sizeof(char)*MAX_PATHLEN);
  strcat(dbfile,home);
  strcat(dbfile,"/");
  strcat(dbfile,dbname);

  init();
  load(dbfile);

  char *xpath,*xmark;
  const char *path;
  switch(argc){
  case 1: /* print dbfile and help message */
    help(argv[0]);
    print();
    release();
    break;
  case 2:
    /* in case u want to use the index */
    /* index from 10-19 */
    if(strlen(argv[1]) == 2 && argv[1][0] >= 48 && argv[1][0] <= 57 \
       && argv[1][1] >= 48 && argv[1][1] <= 57) {
      char num = (argv[1][0] - 48)*10;
      num += (argv[1][1]-48)*1;
      path = pos2path(num);
      if(path != NULL)
	fprintf(stdout,"%s",path);
      else
	fprintf(stdout,"");
      release();
      break;
    }
    /* index from 0-9 */
    if(strlen(argv[1]) == 1 && argv[1][0] >= 48 && argv[1][0] <= 57) {
      char num = argv[1][0];
      num -= 48;
	path = pos2path(num);
	if(path != NULL)
	  fprintf(stdout,"%s",path);
	else
	  fprintf(stdout,"");
	release();
	break;
    }
    /* use mark name */
    path =  mark2path(argv[1]);
    if(path != NULL)
      fprintf(stdout,"%s",path);
    else
      fprintf(stdout,"");
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
      exit(-1);
    }
    if(-1 == add(xmark,rpath)) {
      fprintf(stderr,"error when add new record\n");
      exit(-1);
    }
    if(-1 == writedb(dbfile)) {
      fprintf(stderr,"error write database file\n");
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
  return(0);
}
