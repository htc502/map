### :round_pushpin: __map__  
This is a commandline tool like `alias`, but applied on file path. It saves your time when the file paths are deep by using a set of bookmarks.   

#### How to install  

##### To compile it from source under linux and Mac, type:  

    cd source_dir ##source_dir: where the source code was stored
    make
    
##### (Optional) if you want to add it to your system PATH:  

1. `mv` or `ln -s ` the `map` binary and the `map_go.sh` script to a location in your path.  
2. `source map_go.sh` in your .bashrc or .bash_profile  

A file called __.pathmark.db__ will be generated in $HOME dir on the first run of this widget 

##### Compile under windows

The source code was tested with codeblock 13.12 using mingw-gcc(gcc ver4.7.1 32bit), 
to use it under win, enviroment variable named __HOME__ should be avilable(right click :computer: :arrow_right: properties :arrow_right: advanced :arrow_right: enviroment variable).


#### Examples  

##### Bookmark a directory   

`cd` to target directory and type:

    map bookmark .

This will add a new bookmark to __.pathmark.db__. __Avoid using number as a bookmark__.

##### List all bookmarks:   

To browse those directories bookmarked, type:

    map

if you want check a specific bookmark, type:

    map bookmark

##### Go to a bookmarked location  

Try to `cd` to a place marked with "wd":
    
    g wd
    
or just use the index number of "wd" showed when you type `map`:

    g indexofwd

`g` is a function defined in `map_go.sh` which help type the following code ( `""` quotes in case if the path has __whitespaces__):

    cd "`map wd`"

By default, `map` only record the last 20 bookmarks you add. The oldest record will be discard when more records come in. Modify __MAX_NPATH__ variable in __pathmark_db.c__ before compilation to change the limit. Be sure that this number is __<= 99__ because of current argument parsing strategy (If try to deal with more than 100 directories, then probably you need to hack the argument parsing strategy code).

##### To remove a specified bookmark

      map -index

Here, the index starts from 0 to n-1, i.e. the first record will be removed by :alien: :

      map -0

Enjoy!

![image](https://github.com/htc502/map/blob/master/map_howto.gif)
