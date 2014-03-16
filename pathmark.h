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

void init(){
  /* initialize db ram object */

  db_object.NPATH = 0;
  int i = 0;
  for(;i<MAX_NPATH;i++){
    int j = 0;
    for(;j<NFIELD;j++){
      db_object.pathdb[i][j]=NULL;
    }
  }
}

int load(char * dbfile){
  /* load dbfile into ram */

  int writedb(char *file);

  FILE *pfile = fopen(dbfile,"rb");
  if(NULL == pfile){
    fprintf(stderr,"%s does not exist,will generate it automatically.\n",dbfile);
    writedb(dbfile); //write db_object to the file
    exit(-1);
  }

  char line[MAX_PATHLEN];
  int npath = 0;
  while((NULL != fgets(line,MAX_PATHLEN,pfile) && (npath<MAX_NPATH))){
    line[strlen(line)-1] = '\0';
    char *pmark,*ppath;
    pmark = strtok(line,DELIM);
    ppath = strtok(NULL,DELIM);
    /*this will not happen until u edit the pathmark.db mannually */
    if (pmark == NULL || ppath ==NULL)
      continue;

    (db_object.pathdb)[npath][0]=(char*)malloc(sizeof(char)*strlen(pmark)+1);
    (db_object.pathdb)[npath][1]=(char*)malloc(sizeof(char)*strlen(ppath)+1);
    if ((db_object.pathdb)[npath][0] == NULL || (db_object.pathdb)[npath][1] == NULL){
      fprintf(stderr,"error locating memories for map records %i\n",npath);
      fclose(pfile);
      return(-1);
    }
    strcpy((db_object.pathdb)[npath][0],pmark);
    strcpy((db_object.pathdb)[npath++][1],ppath);
  }
  db_object.NPATH = npath;
  fclose(pfile);
  return(0);
}

int release(){
  int i = 0;
  for(;i < db_object.NPATH;i++){
    int j = 0;
    for(;j<NFIELD;j++){
      free((db_object.pathdb)[i][j]);
    }
  }
  return(0);
}

int pos(char *pmark){
  /* return the index of a mark */
  int i = 0;
  while(i<db_object.NPATH){
    if(0 == strcmp(pmark,(db_object.pathdb)[i][0]))
      return(i);
    i++;
  }
  return(-1);
}

const char* mark2path(char *pmark){
  int position = pos(pmark);
  if(position == -1)
    return NULL;
  return((const char*) (db_object.pathdb)[position][1]);
}

const char* pos2path(int pos){
  if(pos > db_object.NPATH - 1 || pos < 0)
    return NULL;
  return((const char*) (db_object.pathdb)[pos][1]);
}

int add(char *mark,char *path){
  /* add a record */

  /* check the mark already exists */
  int ind = pos(mark);
  if(ind != -1){
    int j = 0;
    for(;j<NFIELD;j++){
      free((db_object.pathdb)[ind][j]);
    }
    strcpy((db_object.pathdb)[ind][0],mark);
    strcpy((db_object.pathdb)[ind][1],path);
    return(0);
  }

  /* a new bookmark, try to append this mark */

  /* dbfile is full, remove the oldest one */
  if (db_object.NPATH  == MAX_NPATH ) {
    int j = 1;
    for(;j<MAX_NPATH;j++){
      int f=0;
      for(;f<NFIELD;f++){
	(db_object.pathdb)[j-1][f] = (db_object.pathdb)[j][f];
      }
    }
    strcpy((db_object.pathdb)[MAX_NPATH-1][0],mark);
    strcpy((db_object.pathdb)[MAX_NPATH-1][1],path);
    return(0);
  }
  /* just add to the tail */
  else {
    int i = db_object.NPATH;
    (db_object.pathdb)[i][0]=(char*)malloc(sizeof(char)*strlen(mark)+1);
    (db_object.pathdb)[i][1]=(char*)malloc(sizeof(char)*strlen(path)+1);
    strcpy((db_object.pathdb)[i][0],mark);
    strcpy((db_object.pathdb)[i][1],path);
    /* NPATH plus 1 */
    db_object.NPATH++;
    return(0);
  }
  
  /* there is something unconsiderible happened*/
  return(-1);
}

int writedb(char *file){
  /* write ram db object to file */
  int i = 0;
  FILE *pfile = fopen(file,"w");
  if(NULL == pfile){
    fprintf(stderr,"error opening dbfile %s\n",file);
    return(-1);
  }
  for(;i<db_object.NPATH;i++){
    char *line = (char*) malloc(sizeof(char)*MAX_PATHLEN);
    int f=0;
    for(;f<NFIELD;f++){
      if(f != 0)
	strcat(line,DELIM);
      strcat(line,(db_object.pathdb)[i][f]);
    }
    if(fputs(line,pfile) == EOF){
      fprintf(stderr,"error writing %ith records\n",i);
      fclose(pfile);
      return(-1);
    }
    fputs("\n",pfile);
  }
  fclose(pfile);
  return(0);
}

void print(){
  fprintf(stdout,"                   \n");
  fprintf(stdout,"<<<<<<<<<<<<<<<<<<<<<<<\n");
  fprintf(stdout,"[#] [marker]     [Path]\n");
  int i = 0;
  for(;i<db_object.NPATH;i++){
    fprintf(stdout,"[%i] %s <--- %s\n",i,(db_object.pathdb)[i][0],(db_object.pathdb)[i][1]);
  }
  fprintf(stdout,">>>>>>>>>>>>>>>>>>>>>>>\n");
  fprintf(stdout,"                   \n");
}
#endif
