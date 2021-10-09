#!/bin/bash
path0=`pwd`
echo -e "\nA new auto_sub job is running from here: `hostname` at `date`"
sim_type=detsim

tmpname=${sim_type}_subdirs.txt
dirnum=`cat $tmpname | wc -l`
for((nn=1;nn<=${dirnum};))
#for((nn=1;nn<=10;))
do
    echo "`date` process lines: $nn"
    # echo "`date`, are going to run $dir on `hostname`"
    if [[ `hep_q -u | tail -n 1 | cut -d' ' -f1` -lt 9000 ]];then
        dir=`head -n $nn $tmpname | tail -n 1`
        cd $dir/run
        jobnum=`ls *.sh | wc -l`
        if [[ $jobnum -eq 20 ]];then
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
            hep_sub ${jobname}"%{ProcId}".sh -n $jobnum
            #fi
        else
            echo "$dir job number not 20"
        fi
        let "nn=nn+1"
        cd $path0
    else
        hep_edit -m 3000 -a && hep_release -a
        sleep 120
    fi
done
