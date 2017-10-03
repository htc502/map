##the function which call map and cd to change the wd of the invoking terminal
function g {
    if (($# == 0)); then
        echo \"Usage: g pathmarker\"
        return
    fi
    path=`map $1`
    if [ ! -z "$path" ];then
        cd "$path"  ##quoted in case the file path has whitespaces
    fi
} 
