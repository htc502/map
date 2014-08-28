
This is a small widget operates like alias, but applied on 
file path,especially useful when the file path is deep and long.

a file called .pathmark.db will be generated in $HOME dir

example:
** cd to your working directory and type:

    map workdir .

will add a marker named workdir which point to ur
current working dir.

** to see those dirs bookmarked,just type:

    map

this will cause map to list all noted paths.
** try to cd to a place marked with "wd",u can type either:

    cd `map wd`

or just using the index of "wd" showed when u type "map":

    cd `map indexofwd`

enjoy!
