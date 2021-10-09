#!/bin/bash
path0=`pwd`
# first sub 0,0,0 Co60/Cs137/Ge68/AmC/Laser0.1/Laser0.05/C14 for calibration
tmpname=tmp_000_dirs.txt
# find . -name *_0_0_0 > $tmpname
if [[ $# -eq 0 ]];then
    sim_type=detsim
    memOpt="-mem 3000"
else
    sim_type=elecsim
fi
for dir in `cat $tmpname`
do
    echo $dir
    cd $dir/$sim_type/run
    jobnum=`ls *.sh | wc -l`
    if [[ $jobnum -eq 20 ]];then
        job0=`find -name "*_0.sh"`
        jobname=`echo ${job0%[0]*}`
        hep_sub ${jobname}"%{ProcId}".sh -n $jobnum ${memOpt}
    else
        echo "$dir job number not 20"
    fi
    cd $path0
done

# rm $tmpname