#!/bin/bash
path0=`pwd`
sim_type=${1:-detsim}
subdir=${sim_type}_subdirs.txt
jobN=20

while read dir
do
    echo $dir
    ### because recQTMLE root files are deleted! So checking user-root files is always fit for detsim/elecsim/calib/rec
    cd $dir/user-root
    for((n=0;n<${jobN};n++))
    do
        rn="user-${sim_type}-${n}.root"
        if [[ ! -f ${rn} ]];then
            echo "${rn} not exist! Now resub it's job."
            num=`echo $dir | grep -o '/' | wc -l`
            source_pos=`echo $dir | cut -d/ -f${num}`
            hep_sub ../run/${source_pos}_${sim_type}_${n}.sh -e /dev/null -o /dev/null
        fi
    done 
    echo " "
    cd $path0
done < ${subdir}
