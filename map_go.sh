##the function which call map and cd to change the wd of the invoking terminal
function g {
    if (($# == 0)); then
	echo \"usage g pathmark\"
	return
    fi
    path=`map $1`
    if [ ! -z "$path" ];then
	cd $path 
    fi
} 
