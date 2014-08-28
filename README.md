
This is a small widget operates like alias, but applied on   
file path,especially useful when the file path is deep and long. 
To compile it from source, just type:  

    make -f makefile

a file called .pathmark.db will be generated in $HOME dir  

example:  

1. Add a dir marker   

cd to your working directory and type:

    map workdir .

will add a marker named workdir which point to ur
current working dir.

2. List all dir markers   

to see those dirs bookmarked,just type:

    map

this will cause map to list all noted paths.

3. Go to a bookmarked location  

try to cd to a place marked with "wd",u can type either:

    cd `map wd`

or just using the index of "wd" showed when u type "map":

    cd `map indexofwd`

enjoy!
