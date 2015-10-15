### :round_pushpin: __map__  
This is a small(and simple&stupid) widget operates like `alias`, but applied on file path. It is especially useful when the file path is deep and long :smile:.   
To compile it from source under linux and Mac, just type:  

    make
    
`mv` or `ln -s ` the `map` binary to a location that is in your path, you can go...
A file called __.pathmark.db__ will be generated in $HOME dir on the first run of this widget 

#### Compile under windows

the source code was tested with codeblock 13.12 using mingw-gcc(gcc ver4.7.1 32bit), 
to use it under win, envir variable named __HOME__ should be avilable(right click :computer: :arrow_right: properties :arrow_right: advanced :arrow_right: enviroment variable) for `map` to work.  

examples:  

##### Add a dir marker   

`cd` to your working directory and type:

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

By default, `map` only record the last 20 bookmarks u have made, the oldest record will be thrown away when a new record is coming in if __pathmarker.db__ is full. Change __MAX_NPATH__ variable in __pathmark_db.c__ before compilation to satisfy your own need.

##### To remove a specified bookmark

      map -index

Here, index is started from 0 to n-1, ie, the first record will be removed by :alien:    

      map -0

enjoy!
