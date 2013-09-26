#ifndef PATHMARK_H
#define PATHMARK_H

#include <stdlib.h>
#include <string.h>

#define MAX_PATHLEN 1000
#define NPATH 20
#define NFIELD 2
#define DELIM "\t"

static char *pathdb[NPATH][NFIELD];

void init(){
    int i = 0;
    for(;i<NPATH;i++){
        int j = 0;
        for(;j<NFIELD;j++){
            pathdb[i][j]=NULL;
        }
    }
}

int load(char * dbfile){
    int updatedb(char *file);
    FILE *pfile = fopen(dbfile,"rb");
    if(pfile == NULL){
        fprintf(stderr,"%s does not exist,will generate it automatically.\n",dbfile);
        updatedb(dbfile);
        exit(-1);
    }
    char line[MAX_PATHLEN];
    int i = 0;
    while((NULL != fgets(line,MAX_PATHLEN,pfile) && (i<NPATH))){
        line[strlen(line)-1] = '\0';
        char *mark,*path;
        mark = strtok(line,DELIM);
        path = strtok(NULL,DELIM);
        /*this will not happen until u edit the pathmark.db mannually */
        if (mark == NULL || path ==NULL)
            continue;
        pathdb[i][0]=(char*)malloc(sizeof(char)*strlen(mark)+1);
        pathdb[i][1]=(char*)malloc(sizeof(char)*strlen(path)+1);
        if (pathdb[i][0] == NULL || pathdb[i][1] == NULL){
            fprintf(stderr,"error locating memories for map records %i\n",i);
            fclose(pfile);
            return(-1);
        }
        strcpy(pathdb[i][0],mark);
        strcpy(pathdb[i++][1],path);
    }
    fclose(pfile);
    return(0);
}
int release(){
    int i = 0;
    for(;i<NPATH;i++){
        int j = 0;
        for(;j<NFIELD;j++){
            free(pathdb[i][j]);
        }
    }
    return(0);
}

int pos(char *pmark){
    int i = 0;
    for(;i<NPATH;i++){
        /* mark is empty means there is no record under this one, return */
        if(pathdb[i][0] == NULL)
            break;
        if(0 == strcmp(pmark,pathdb[i][0])){
            return(i);
        }
    }
    return(-1);
}

const char* mark2path(char *pmark){
    int position = pos(pmark);
    if(position == -1)
        return NULL;
    return((const char*) pathdb[position][1]);
}
const char* pos2path(int pos){
    if(pos >= NPATH || pos < 0)
        return NULL;
    return((const char*) pathdb[pos][1]);
}

int add(char *mark,char *path){
    int ind = pos(mark);
    if(ind != -1){
        int j = 0;
        for(;j<NFIELD;j++){
            free(pathdb[ind][j]);
        }
        strcpy(pathdb[ind][0],mark);
        strcpy(pathdb[ind][1],path);

        return(0);
    }
    /* a new bookmark, try to append this mark */
    int i = 0;
    for(;i != NPATH;i++){
        if(pathdb[i][0] == NULL){
            pathdb[i][0]=(char*)malloc(sizeof(char)*strlen(mark)+1);
            pathdb[i][1]=(char*)malloc(sizeof(char)*strlen(path)+1);
            strcpy(pathdb[i][0],mark);
            strcpy(pathdb[i][1],path);
            return(0);
        }
    }
    /* dbfile is full, remove the oldest one */
    int j = 1;
    for(;j<NPATH;j++){
        int f=0;
        for(;f<NFIELD;f++){
            pathdb[j-1][f] = pathdb[j][f];
        }
    }
    strcpy(pathdb[NPATH-1][0],mark);
    strcpy(pathdb[NPATH-1][1],path);
    return(0);

}
int updatedb(char *file){
    int i = 0;
    FILE *pfile = fopen(file,"w");
    if(NULL == pfile){
        fprintf(stderr,"error opening dbfile %s\n",file);
        return(-1);
    }
    for(;i<NPATH;i++){
        if(pathdb[i][0] == NULL)
            break;
        char *line = (char*) malloc(sizeof(char)*MAX_PATHLEN);
        int f=0;
        for(;f<NFIELD;f++){
            if(f != 0)
                strcat(line,DELIM);
            strcat(line,pathdb[i][f]);
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
    for(;i<NPATH;i++){
        if(pathdb[i][0] == NULL)
            break;
        fprintf(stdout,"[%i] %s <--- %s\n",i,pathdb[i][0],pathdb[i][1]);
    }
    fprintf(stdout,">>>>>>>>>>>>>>>>>>>>>>>\n");
    fprintf(stdout,"                   \n");
}
#endif
