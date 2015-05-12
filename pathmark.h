#ifndef PATHMARK_H
#define PATHMARK_H

#include <stdlib.h>
#include <string.h>

/* windows specific realpath */
/*
  realpath() Win32 implementation
  By Nach M. S.
  Copyright (C) September 8, 2005

  I am placing this in the public domain for anyone to use or modify
*/

#ifdef _WIN32
#include <windows.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
char *realpath(const char *path, char resolved_path[PATH_MAX])
{
  char *return_path = 0;
  if (path) //Else EINVAL
    {
      if (resolved_path)
	{
	  return_path = resolved_path;
	}
      else
	{
	  //Non standard extension that glibc uses
	  return_path = malloc(PATH_MAX);
	}
      if (return_path) //Else EINVAL
	{
	  //This is a Win32 API function similar to what realpath() is supposed to do
	  size_t size = GetFullPathNameA(path, PATH_MAX, return_path, 0);

	  //GetFullPathNameA() returns a size larger than buffer if buffer is too small
	  if (size > PATH_MAX)
	    {
	      if (return_path != resolved_path) //Malloc'd buffer - Unstandard extension retry
		{
		  size_t new_size;

		  free(return_path);
		  return_path = malloc(size);

		  if (return_path)
		    {
		      new_size = GetFullPathNameA(path, size, return_path, 0); //Try again

		      if (new_size > size) //If it's still too large, we have a problem, don't try again
			{
			  free(return_path);
			  return_path = 0;
			  errno = ENAMETOOLONG;
			}
		      else
			{
			  size = new_size;
			}
		    }
		  else
		    {
		      //I wasn't sure what to return here, but the standard does say to return EINVAL
		      //if resolved_path is null, and in this case we couldn't malloc large enough buffer
		      errno = EINVAL;
		    }
		}
	      else //resolved_path buffer isn't big enough
		{
		  return_path = 0;
		  errno = ENAMETOOLONG;
		}
	    }

	  //GetFullPathNameA() returns 0 if some path resolve problem occured
	  if (!size)
	    {
	      if (return_path != resolved_path) //Malloc'd buffer
		{
		  free(return_path);
		}

	      return_path = 0;

	      //Convert MS errors into standard errors
	      switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
		  errno = ENOENT;
		  break;

		case ERROR_PATH_NOT_FOUND: case ERROR_INVALID_DRIVE:
		  errno = ENOTDIR;
		  break;

		case ERROR_ACCESS_DENIED:
		  errno = EACCES;
		  break;

		default: //Unknown Error
		  errno = EIO;
		  break;
		}
	    }

	  //If we get to here with a valid return_path, we're still doing good
	  if (return_path)
	    {
	      struct stat stat_buffer;

	      //Make sure path exists, stat() returns 0 on success
	      if (stat(return_path, &stat_buffer))
		{
		  if (return_path != resolved_path)
		    {
		      free(return_path);
		    }

		  return_path = 0;
		  //stat() will set the correct errno for us
		}
	      //else we succeeded!
	    }
	}
      else
	{
	  errno = EINVAL;
	}
    }
  else
    {
      errno = EINVAL;
    }

  return return_path;
}
#endif

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

int load(const char * dbfile){
  /* load dbfile into ram */

  int writedb(const char *file);

  FILE *pfile = fopen(dbfile,"rb");
  if(NULL == pfile){
    fprintf(stderr,"%s does not exist,will generate it automatically.\n",dbfile);
    writedb(dbfile); //write db_object to the file
    return(-1);
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
      exit(-1);
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

int add(const char *mark,const char *path){
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

int writedb(const char *file){
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
