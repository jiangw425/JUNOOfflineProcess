#!/bin/bash
path0=`pwd`
# first sub 0,0,0 Co60/Cs137/Ge68/AmC/Laser0.1/Laser0.05 [[C14]] for calibration

if [[ $# -eq 0 ]];then
    sim_type=detsim
    memOpt="-mem 3000"
else
    sim_type=elecsim
    memOpt=""
fi

if [[ ! -f ${sim_type}_subdirs.txt ]];then
    echo "${sim_type}_subdirs.txt not exist ! "
    exit 1
fi

subdirs=`cat ${sim_type}_subdirs.txt | grep _0_0_0`
for dir in $subdirs
do
    echo $dir
    cd $dir
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
sed -i '/_0_0_0/d' ${sim_type}_subdirs.txt
# rm $tmpname