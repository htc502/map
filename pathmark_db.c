#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_PATHLEN 1000
#define MAX_NPATH 20 /* maximum number of records pathmarker.db will hold, no more than 99 based on current implementation of main fxn */
#define NFIELD 2 /* NFIELD and DELIM is used in pathmarker.db construction */
#define DELIM "\t"
static struct {
  char *pathdb[MAX_NPATH][NFIELD];
  int NPATH; //current load
  int CLEAN; //flag for write back 
} db_object;
static int free_record(int); //static function cann't be decleared inside any function!!!don;t know why...file scope?
void init(){
  /* initialize db ram object */
  db_object.NPATH = 0;
  db_object.CLEAN = 0;
  int i = 0;
  for(;i<MAX_NPATH;i++){
    int j = 0;
    for(;j<NFIELD;j++){
      db_object.pathdb[i][j]=NULL;
    }
  }
}
int load(const char * dbfile){
  int writedb(const char*, int);
  FILE *pfile = fopen(dbfile,"rb");
  if(NULL == pfile){
    fprintf(stderr,"%s does not exist,will generate it automatically.\n",dbfile);
    /* use writedb funx to create a new pathmarker.db file */
    writedb(dbfile, 1); //write db_object to the file by force
    return(-1);
  }
  char line[MAX_PATHLEN];  int npath = 0;
  while((NULL != fgets(line,MAX_PATHLEN,pfile) && (npath<MAX_NPATH))){
    line[strlen(line)-1] = '\0'; /* chop the return char */
    char *pmark,*ppath;
    pmark = strtok(line,DELIM);
    ppath = strtok(NULL,DELIM);
    /*this will not happen until u edit pathmark.db mannually */
    if (pmark == NULL || ppath ==NULL)
      continue;
    (db_object.pathdb)[npath][0]=(char*)malloc(sizeof(char)*strlen(pmark)+1);
    (db_object.pathdb)[npath][1]=(char*)malloc(sizeof(char)*strlen(ppath)+1);
    if ((db_object.pathdb)[npath][0] == NULL || (db_object.pathdb)[npath][1] == NULL){
      fprintf(stderr,"error locating memories for map records %i\n",npath);
      fclose(pfile);
      exit(-1);
    }
    strcpy((db_object.pathdb)[npath][0],pmark);
    strcpy((db_object.pathdb)[npath++][1],ppath);
  }
  db_object.NPATH = npath;
  fclose(pfile);
  return(0);
}
/* add a record */
int add(const char *mark,const char *path){
  /* check if the mark already exists,if it exists, just replace it */
  int pos(const char *pmark);
  int ind = pos(mark);
  if(ind != -1){
    free_record(ind);
    (db_object.pathdb)[ind][0]=(char*)malloc(sizeof(char)*strlen(mark)+1);
    (db_object.pathdb)[ind][1]=(char*)malloc(sizeof(char)*strlen(path)+1);
    strcpy((db_object.pathdb)[ind][0],mark);
    strcpy((db_object.pathdb)[ind][1],path);
    db_object.CLEAN = 1; //set flag, need to write back to db file
    return(0);
  }
  /* a new bookmark, try to append this mark */
  /* dbfile is full */
  if (db_object.NPATH  == MAX_NPATH ) {
    /*remove the oldest one;move forward and add the new record*/
    /*release the oldest one's ram*/    
    free_record(MAX_NPATH-1);
    int j = MAX_NPATH - 1;
    for(;j > 0;j--){
      int f=0;
      for(;f<NFIELD;f++)
	(db_object.pathdb)[j][f] = (db_object.pathdb)[j-1][f];
    }
    /* add new record */
    (db_object.pathdb)[0][0]=(char*)malloc(sizeof(char)*strlen(mark)+1);
    (db_object.pathdb)[0][1]=(char*)malloc(sizeof(char)*strlen(path)+1);
    strcpy((db_object.pathdb)[0][0],mark);
    strcpy((db_object.pathdb)[0][1],path);
    db_object.CLEAN = 1; //set flag, need to write back to db file
    return(0);
  }
  /* move all records forward by one, add new record to the head*/
  else {
    int i = db_object.NPATH;
    int j = i;
    for(;j > 0;j--) {
      int f=0;
      for(;f<NFIELD;f++)
	(db_object.pathdb)[j][f] = (db_object.pathdb)[j-1][f];
    }
    /* add new record */
    (db_object.pathdb)[0][0]=(char*)malloc(sizeof(char)*strlen(mark)+1);
    (db_object.pathdb)[0][1]=(char*)malloc(sizeof(char)*strlen(path)+1);
    strcpy((db_object.pathdb)[0][0],mark);
    strcpy((db_object.pathdb)[0][1],path);

    /* NPATH increase */
    db_object.NPATH++;
    db_object.CLEAN = 1; //set flag, need to write back to db file
    return(0);
  }
  /* there is something unconsiderible happened*/
  return(-1);
}
int writedb(const char *file, int force){
  /* write ram db object to file */
  if(db_object.CLEAN == 0  && force == 0) //CLEAN status, no need to write back,
					  //force is set to create db file with CLEAN(empty) db_object
    return(0);
  int i = 0;
  FILE *pfile = fopen(file,"w");
  if(NULL == pfile){
    fprintf(stderr,"error opening dbfile %s\n",file);
    return(-1);
  }
  for(;i<db_object.NPATH;i++){
    /* assemble the line to write */
    char *line = (char*) malloc(sizeof(char)*MAX_PATHLEN);
    int f=0;
    for(;f<NFIELD;f++){
      if(f != 0) strcat(line,DELIM);
      (f == 0)?strcpy(line,(db_object.pathdb)[i][f]):strcat(line,(db_object.pathdb)[i][f]);
    }
    if(fputs(line,pfile) == EOF){
      fprintf(stderr,"error writing %ith records\n",i);
      fclose(pfile);
      free(line);
      return(-1);
    }
    fputs("\n",pfile);
    free(line);
  }
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
int pos(const char *pmark){
  /* return the index of a mark */
  int i = 0;
  while(i<db_object.NPATH){
    if(0 == strcmp(pmark,(db_object.pathdb)[i][0]))
      return(i);
    i++;
  }
  return(-1);
}
const char* mark2path(const char *pmark){
  int pos(const char *pmark);
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
void printdb(){
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
static int free_record(int index)
{
  /*** this is a function used internally***/
  /* index ranges from 0 to NPATH-1 */
  if(index < 0 || index > (db_object.NPATH -1)) {
    return(-1);
  }
  /*release record*/
  int ifield = 0;
  for(;ifield < NFIELD;ifield++) {
    free(db_object.pathdb[index][ifield]);
    db_object.pathdb[index][ifield] = NULL;
  }
  return(0);
}
int rm(int pos)
{
  if(-1 == free_record(pos))
    return(-1);
  /*move records forward to fill the empty space*/
  int j = pos;
  for(;j != db_object.NPATH-1;j++) {
    int f = 0;
    for(;f<NFIELD;f++) 
      (db_object.pathdb)[j][f] = (db_object.pathdb)[j+1][f];
  }
  /*make the last pointer NULL*/
  int f = 0;
  for(;f<NFIELD;f++) 
    (db_object.pathdb)[j][f] = NULL;
  /*decreasing the record count*/
  db_object.NPATH -= 1;
  /* set write back flag */
  db_object.CLEAN = 1; //set flag, need to write back to db file

  /*we have done*/
  return(0);
}
int opendb(char *dbfname)
{
  //return -1 for create a new database file
  //init
  init();
  //load(read)
  return(load(dbfname));
}
int closedb(const char *dbfname)
{
  //writedb
  int t = 0;
  if(!( t = writedb(dbfname,0))) {
    fprintf(stderr, "writing database failed!\n");
  }
  //releasedb
  release();
  return(t);
}
