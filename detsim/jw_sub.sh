#!/bin/bash
path0=`pwd`
echo -e "\nA new auto_sub job is running from here: `hostname` at `date`"
sim_type=${1:-"detsim"}; shift
jobN=${1:-"20"}; shift

sim_types=(detsim elecsim calib recQTMLE)
if [[ ! ${sim_types[@]} =~ ${sim_type} ]];then
    echo "Wrong sim_type! Choose from: ${sim_types[@]}"
    exit 1
fi
tmpname=${sim_type}_subdirs.txt
if [[ ! -f ${tmpname} ]];then
    echo "$tmpname not exist!"
    exit 1
fi
if [[ $sim_type == "detsim" ]];then
    memOpt="-mem 3000"
else
    memOpt=""
fi
dirnum=`cat $tmpname | wc -l`
for((nn=1;nn<=${dirnum};))
#for((nn=1;nn<=10;))
do
    echo "`date` processing lines: $nn"
    # echo "`date`, are going to run $dir on `hostname`"
    # if [[ `hep_q -u | tail -n 1 | cut -d' ' -f1` -lt 9000 ]];then
        dir=`head -n $nn $tmpname | tail -n 1`
        cd $dir
        jobnum=`ls *.sh | wc -l`
        if [[ $jobnum -eq $jobN ]];then
            job0=`find -name "*_0.sh"`
            jobname=`echo ${job0%[0]*}`
            #sckout=`bash ~jiangw/sck.sh ../log | head -n 1 | cut -d' ' -f2`
            #if [[ $sckout == "40" ]];then
                #for n in {0..39}
                #do
                #    sed '7s#(#\#(#' ${jobname}${n}.sh > ${jobname}${n}.tmp
                #    cp ${jobname}${n}.tmp ${jobname}${n}.sh
                #done
                #rm *.tmp
            #    chmod +x *.sh
            #    hep_sub ${jobname}"%{ProcId}".sh -n 40
            #elif [[ $sckout == "80" ]];then
            #    echo "$dir run successfully!"
            #else
                # no log files or success files
            hep_sub ${jobname}"%{ProcId}".sh -n $jobnum -e /dev/null -o /dev/null $memOpt
            #fi
        else
            echo "$dir job number not $jobN"
        fi
        let "nn=nn+1"
        cd $path0
    # else
        # if [[ $sim_type == detsim ]];then
        #     hep_edit -m 3000 -a && hep_release -a
        # fi
        # sleep 120
    # fi
done
