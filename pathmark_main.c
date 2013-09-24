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
            /* in case u want to use the index */
            if(strlen(argv[1]) == 2) {
                char num = argv[1][0];
                num += argv[1][1];
                num -= 48;
                if(num >= 0 && num <= 19){
                    path = pos2path(num);
                    if(path != NULL)
                        fprintf(stdout,"%s",path);
                    else
                        fprintf(stdout,"");
                    release();
                    break;
                }
            }
            if(strlen(argv[1]) == 1) {
                char num = argv[1][0];
                num -= 48;
                if(num >= 0 && num <= 9){
                    path = pos2path(num);
                    if(path != NULL)
                        fprintf(stdout,"%s",path);
                    else
                        fprintf(stdout,"");
                    release();
                    break;
                }
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
