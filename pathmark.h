#ifndef PATHMARK_H
#define PATHMARK_H

#include <stdlib.h>
#include <string.h>

#define MAX_PATHLEN 1000
#define NPATH 20
#define DELIM "\t"

char *pathdb[NPATH];

void init(){
    int i = 0;
    for(;i<NPATH;i++){
        pathdb[i]=NULL;
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
    while((NULL != fgets(line,MAX_PATHLEN,pfile)&&(i<NPATH))){
        line[strlen(line)-1] = '\0';
        if(pathdb[i] = (char*)malloc(sizeof(char)*strlen(line)+1)){
            strcpy(pathdb[i++],line);
        }
        else {
            fprintf(stderr,"error reading %ith line of dbfile %s\n",(i+1),dbfile);
            fclose(pfile);
            return(-1);
        }
    }
    fclose(pfile);
    return(0);
}
int release(){
    int i = 0;
    for(;i<NPATH;i++){
        free(pathdb[i]);
    }
    return(0);
}
char* grep(char *pmark){
    int i = 0;
    for(;i<NPATH;i++){
        if(pathdb[i] == NULL)
            break;
        char *mark,*path;
        mark = strtok(pathdb[i],DELIM);
        path = strtok(NULL,DELIM);
        if(0 == strcmp(pmark,mark))
            return(path);
    }
    return(NULL);
}

int pos(char *pmark){
    int i = 0;
    for(;i<NPATH;i++){
        if(pathdb[i] == NULL)
            break;
        char *mark;
        mark = strtok(pathdb[i],DELIM);
        if(0 == strcmp(pmark,mark))
            return(i);
    }
    return(-1);
}

int add(char *mark,char *path){
    int ind = pos(mark);
    if(ind != -1){
        free(pathdb[ind]);
        int len = strlen(mark)+strlen(path)+2;
        char *temp = (char*)malloc(sizeof(char)*len);
        strcpy(temp,mark);
        strcpy(temp+strlen(mark),"\t");
        strcpy(temp+strlen(mark)+1,path);
        pathdb[ind] = temp;
        return(0);
    }
    int i = 0;
    while(1){
        if(pathdb[i] == NULL){
            int len = strlen(mark)+strlen(path)+2;
            char *temp = (char*)malloc(sizeof(char)*len);
            strcpy(temp,mark);
            strcpy(temp+strlen(mark),"\t");
            strcpy(temp+strlen(mark)+1,path);
            pathdb[i] = temp;
            return(0);
        }
        else if(i == NPATH){
            int j = 1;
            for(;j<NPATH;j++){
                pathdb[j-1] = pathdb[j];
            }
            int len = strlen(mark)+strlen(path)+2;
            char *temp = (char*)malloc(sizeof(char)*len);
            strcpy(temp,mark);
            strcpy(temp+strlen(mark),"\t");
            strcpy(temp+strlen(mark)+1,path);
            pathdb[NPATH-1] = temp;
            return(0);
        }
        else{
            i++;
        }
    }
}
int updatedb(char *file){
    int i = 0;
    FILE *pfile = fopen(file,"w");
    if(NULL == pfile){
        fprintf(stderr,"error opening dbfile %s\n",file);
        return(-1);
    }
    for(;i<NPATH;i++){
        if(pathdb[i] == NULL)
            break;
        if(fputs(pathdb[i],pfile) == EOF){
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
    int i = 0;
    for(;i<NPATH;i++){
        if(pathdb[i] == NULL)
            break;
        char *mark,*path;
        mark = strtok(pathdb[i],DELIM);
        path = strtok(NULL,DELIM);
        fprintf(stdout,"%s --- %s\n",mark,path);
    }
}
#endif
