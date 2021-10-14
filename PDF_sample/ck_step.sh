#!/bin/bash
if [[ $#<=0 ]]; then
    echo "You have not input step number."
    exit 1
fi
input1=$1; shift
input2=$1; shift
steps=(1 3)
if [[ ! ${steps[@]} =~ $input1 ]];then
    echo "Wrong input step number."
    exit 1
fi

checkExist(){
    for file in $*
    do
        if [[ ! -f "${file}" ]];then
            echo "$file not exist."
            exit 1
        fi
    done
}
checkLogs(){
    jobtot=$1; shift
    resubOpt=$1; shift
    tmpLog=`bash ~jiangw/sck.sh`
    if [[ `echo $tmpLog | grep "txt"` != "" ]] || [[ `echo $tmpLog | cut -d' ' -f8` != $jobtot ]];then
        echo "$s not done."
        if [[ $resubOpt == resub ]];then
            for f in $tmpLog
            do
                if [[ $f == *log-*.txt ]];then
                    jobnum=`echo ${f##*-} | cut -d. -f1`
                    job_name=`ls ../run | grep "[-,_]${jobnum}.sh" | awk 'NR==1'`
                    hep_sub $job_name -e /dev/null -o /dev/null
                fi    
            done
        else
            exit 1
        fi
    fi
    echo -e "\n$tmpLog"
}

sname=(Ge68 Laser0.05)

echo -e "`date`. Check step $input1 jobs status: \c"

case $input1 in
    1)
        for s in ${sname[@]}
        do
            cd share/$s/log
            checkLogs 293 $input2
            cd - >/dev/null 2>&1
        done
    ;;
    3)
        cd GenQPDF/share/log
        checkLogs 881 $input2
        cd - > /dev/null 2>&1
    ;;
esac
# echo "step$1 done"
# fi
echo " "
