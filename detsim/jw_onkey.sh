#!/bin/bash
path0=`pwd`

sim_types=(detsim elecsim calib recQTMLE)
txt_file="${sim_types[0]}_subdirs.txt"

for dir in `cat $txt_file`
do
    echo $dir
    cd $dir
    filenum=`ls *.sh | wc -l`
    cd ../..
    path1=`pwd`
    if [[ ! -d onekey_run ]];then mkdir onekey_run; fi
    rm -rf onekey_run/*
    jnb=$path1/onekey_run/${path1##*/}_onekey_
    for st in ${sim_types[@]}
    do
        echo $st
        cd $st/run
        if [[ `ls *.sh | wc -l` -ne $filenum ]];then
            echo "`pwd` file num is not $filenum !"
            exit 1
        fi
        for((n=0;n<${filenum};n++))
        do
            jobname=${jnb}${n}.sh
            tmpjob=`ls *.sh | grep _${n}.sh | head -n 1`
            if [[ $st == ${sim_types[0]} ]];then
                echo "#!/bin/bash" >> $jobname
            fi
            echo "bash `pwd`/$tmpjob" >> $jobname
        done
        cd $path1
    done
    chmod +x ${jnb}*.sh
    hep_sub ${jnb}"%{ProcId}".sh -n ${filenum} -mem 3000 -e /dev/null -o /dev/null
    cd $path0
done


