#!/bin/bash
path0=`pwd`

mkdir -p OtherFiles

tmpfile="All_files.root"
find . -type f > $tmpfile
while read afile
do
    dir=${afile%/*}
    if [[ ${afile##*.} != root ]] || [[ ${dir:0-4} != root ]];then
        tmpPath=OtherFiles/$dir
        if [[ ! -d $tmpPath ]];then
            mkdir -p $tmpPath
        fi
        mv $afile $tmpPath
    fi
done < $tmpfile
### only leave root and user-root dir
for n in {0..3}
do
    emptyDir=`find . -type d -empty`
    echo $emptyDir
    if [[ $emptyDir != "" ]];then
        for d in $emptyDir
        do
            rm -r $d
        done
    fi
done

rm -f $tmpfile
