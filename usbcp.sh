#!/bin/bash

function print_usage {
    echo "  Copies the files of [sourcedir] in alphabetical order"
    echo "  to a same-named directory created in [destdir]."
    echo "  Usage: $(basename "$0") [sourcedir] [destdir]"
}

# check for correct number of parameters
if [ "$#" != 2 ]; then
    print_usage
    exit 1
fi

# check if source is a directory
if [ ! -d "$1" ]; then
    print_usage
    echo "  ERROR: [sourcedir] isn't a directory"
    exit 1
fi

# check if dest is a directory
if [ ! -d "$2" ]; then
    print_usage
    echo "  ERROR: [destdir] isn't a directory"
    exit 1
fi

# short name of source folder, we'll make a new one of these later
foldername=$(basename "$1")

# full path of destination to make new folder in
fulldest=$(readlink -f "$2")

# change to dir with files to make globbing easier
cd "$1"
# if a glob is empty, don't make an array
shopt -s nullglob
# find all files in current dir and put them in an array
arr=( * )

# if the array is empty
if [ ${#arr[@]} == 0 ]; then
    echo "  ERROR: Source dir contains no files!"
    exit 1
fi

# sort array - http://stackoverflow.com/a/11789688/1422582
IFS=$'\n' sorted=($(sort <<<"${arr[*]}"))
unset IFS

# create destination dir from source dir name
newdir="$fulldest/$foldername"
echo " Creating directory \"$newdir\""
mkdir -p "$newdir"

# copy sorted files
for ((i=0; i<${#sorted[@]}; i++)); do
    echo " Copying \"$(pwd)/${sorted[$i]}\""
    cp "${sorted[$i]}" "$newdir/${sorted[$i]}"
done
