
This is a small(and simple) widget operates like alias, but applied on file path. It is especially useful when the file path is deep and long.   
To compile it from source, just type:  

    make -f makefile

a file called .pathmark.db will be generated in $HOME dir on the first run of this widget 

#### Compile under windows

the source code was tested with codeblock 13.12 using mingw-gcc(gcc ver4.7.1 32bit), 
to use it under win, envir variable named HOME should be avilable(right click My Computer->
properties->advanced->enviroment variable) for map to work.  

examples:  

##### Add a dir marker   

cd to your working directory and type:

    map workdir .

will add a marker named workdir which points to ur
current working dir. 

##### List all dir markers   

to check those dirs bookmarked,just type:

    map

this will make map list all noted paths.

##### Go to a bookmarked location  

try to `cd` to a place marked with "wd",u can type either:

    cd `map wd`

or just using the index of "wd" showed when u type `map`:

    cd `map indexofwd`

By default, map only record the last 20 bookmarks u have made, the oldest record will be thrown away when a new record is coming in if pathmarker.db is full. Change MAX_NPATH variable in pathmark_db.c before compilation to satisfy your own need.

##### To remove a specified bookmark

      map -index

Here, index is started from 0 to n-1. 

enjoy!
