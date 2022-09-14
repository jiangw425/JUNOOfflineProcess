#!/bin/bash
path0=`pwd`
enableCkLog=1
# sim_types=(detsim elecsim calib recQTMLE)
sim_types=(detsim elecsim)
txt_file="${sim_types[0]}_subdirs.txt"

for dir in `cat $txt_file`
do
    echo $dir
    cd $dir
    tmpf1=$path0/tmp_run.txt
    ls *.sh > $tmpf1
    filenum=`cat $tmpf1 | wc -l`
    cd ../..
    path1=`pwd`
    if [[ ! -d onekey_run ]];then mkdir onekey_run; fi
    rm -rf onekey_run/*
    jnb=$path1/onekey_run/${path1##*/}_onekey_
    for st in ${sim_types[@]}
    do
        echo $st
        cd $st/run
        tmpf3=$path0/tmp_${st}_run.txt
        ls *.sh > $tmpf3
        if [[ `cat $tmpf3 | wc -l` -ne $filenum ]];then
            echo "`pwd` file num is not $filenum !"
            rm -f $tmpf3
            exit 1
        fi
        if [[ $enableCkLog -eq 1 ]];then
            tmpf2=$path0/tmp_${st}_log.txt
            ls ../log | grep .txt > $tmpf2
            echo "asd" >> $tmpf2
        fi
        for((n=0;n<${filenum};n++))
        do
            jobname=${jnb}${n}.sh
            tmpjob=`cat $tmpf3 | grep _${n}.sh | head -n 1`
            if [[ $st == ${sim_types[0]} ]];then
                echo "#!/bin/bash" >> $jobname
            fi
            if [[ $enableCkLog -eq 1 ]];then
                tmplog=`cat $tmpf2 | grep "\-${n}.txt" | head -n 1`
                if [[ $tmplog == "" ]];then
                    echo "bash `pwd`/$tmpjob" >> $jobname
                else
                    bash ~jiangw/sck.sh ../log/$tmplog > /dev/null
                    if [[ $? -eq 0 ]];then
                        echo "sleep 3" >> $jobname
                    else
                        echo "bash `pwd`/$tmpjob" >> $jobname
                    fi
                fi
            else
                echo "bash `pwd`/$tmpjob" >> $jobname
            fi
        done
        rm -f $tmpf2 $tmpf3
        cd $path1
    done
    rm -f $tmpf1
    chmod +x ${jnb}*.sh
    hep_sub ${jnb}"%{ProcId}".sh -n ${filenum} -mem 3000 -e /dev/null -o /dev/null
    cd $path0
done


