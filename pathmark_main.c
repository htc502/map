#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include "pathmark.h"

void help()
{
    fprintf(stdout,"map [marker] [path]\n");
}
int main(int argc, char **argv,char **envp)
{
    char dbfile[]=".pathmark.db";
    char *home = getenv("HOME");
    char *db = strcat(home,"/");
    db = strcat(db,dbfile);
    char *path,*mark;
    init();
    load(db);
    switch(argc){
        case 1:
            print();
            release();
            break;
        case 2:
            path = grep(argv[1]);
            if(path != NULL)
                fprintf(stdout,"%s",path);
            else
                fprintf(stdout,"");
            release();
            break;
        case 3:
            mark = argv[1];
            path = argv[2];
            char *rpath;
            rpath = realpath(path,NULL);
            if(NULL == rpath){
                fprintf(stderr,"error get real path\n");
                exit(-1);
            }
            add(mark,rpath);
            updatedb(db);
            free(rpath);
            release();
            break;
        default:
            help();
            release();
            break;
    }
    return(0);
}
