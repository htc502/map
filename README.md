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
current working directory. __Avoid using number as a marker__, this will confuse map because of its setting to use number as the index.

##### List all dir markers   

To browse those directories bookmarked,just type:

    map

This will make `map` list all noted paths.

##### Go to a bookmarked location  

Try to `cd` to a place marked with "wd",just type:
    
    g wd
    
or just use the index number of "wd" showed when you type `map`:

    g indexofwd

`g` is a function defined in `map_go.sh` which help you do the following job( `""` is added in case if the path has __whitespaces__):

    cd "`map wd`"

By default, `map` only record the last 20 bookmarks you have added. The oldest record will be thrown away when a new record is coming in if __pathmarker.db__ is full. Modify __MAX_NPATH__ variable in __pathmark_db.c__ before compilation to satisfy your own need. Be sure that this number is __<= 99__ because of current argument parsing strategy (I don't think it's a good way to bookmark that many file paths).

##### To remove a specified bookmark

      map -index

Here, the index starts from 0 to n-1, i.e. the first record will be removed by :alien: :

      map -0

Enjoy!

![image](https://github.com/htc502/map/blob/master/map_howto.gif)
