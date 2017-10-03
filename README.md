### :round_pushpin: __map__  
This is a small(and simple&stupid) widget operating like `alias`, but applied on file path. It is especially useful when the file path is deep and long :smile:.   

#### How to install  

##### To compile it from source under linux and Mac, just type:  

    make
    
##### To use it:  

1. `mv` or `ln -s ` the `map` binary and the `map_go.sh` script to a location in your path.  
2. `source map_go.sh` in your .bashrc or .bash_profile  

A file called __.pathmark.db__ will be generated in $HOME dir on the first run of this widget 

##### Compile under windows

The source code was tested with codeblock 13.12 using mingw-gcc(gcc ver4.7.1 32bit), 
to use it under win, enviroment variable named __HOME__ should be avilable(right click :computer: :arrow_right: properties :arrow_right: advanced :arrow_right: enviroment variable).


#### Examples  

##### Add a dir marker   

`cd` to your working directory and type:

    map workdir .

This will add a marker named __workdir__ into __.pathmark.db__ which points to ur
current working directory. 

##### List all dir markers   

To browse those directories bookmarked,just type:

    map

This will make `map` list all noted paths.

##### Go to a bookmarked location  

Try to `cd` to a place marked with "wd",you can either type:

    cd `map wd`

In case if the path has __whitespaces__:

    cd "`map wd_has_spaces`"

or just use the index number of "wd" showed when you type `map`:

    cd `map indexofwd`

or, you can let the `g` function in `go_map.sh` do some work for you:
    
    g wd
    g indexofwd

By default, `map` only record the last 20 bookmarks you have added, the oldest record will be thrown away when a new record is coming in but __pathmarker.db__ is full. Change __MAX_NPATH__ variable in __pathmark_db.c__ before compilation to satisfy your own need.

##### To remove a specified bookmark

      map -index

Here, index starts from 0 to n-1, i.e. the first record will be removed by :alien: :

      map -0

Enjoy!
