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
        case 1:
            help(argv[0]);
            print();
            release();
            break;
        case 2:
            path =  mark2path(argv[1]);
            if(path != NULL)
                fprintf(stdout,"%s",path);
            else
                fprintf(stdout,"");
            release();
            break;
        case 3:
            xmark = argv[1];
            xpath = argv[2];
            char *rpath;
            rpath = realpath(xpath,NULL);
            if(NULL == rpath){
                fprintf(stderr,"invalid path:%s to bookmark\n",xpath);
                exit(-1);
            }
            add(xmark,rpath);
            updatedb(dbfile);
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
