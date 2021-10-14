#!/bin/bash
if [[ $#<=0 ]]; then
    echo "You have not input step number."
    exit 1
fi
steps=(1)
if [[ ! ${steps[@]} =~ $1 ]];then
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

sname=(Ge68 Laser0.05)

echo -e "`date`. Check step $1 jobs status: \c"

case $1 in
    1)
        for s in ${sname[@]}
        do
            totN=293
            cd share/$s
            tmpLog=`bash ~jiangw/sck.sh`
            if [[ `echo $tmpLog | grep "txt"` != "" ]] || [[ `echo $tmpLog | cut -d' ' -f8` != $totN ]];then
                echo "$s step$1 not done."
                if [[ $2 == resub ]];then
                    for f in $tmpLog
                    do
                        if [[ $f == caliblog-*.txt ]];then
                            jobnum=`echo ${f##*-} | cut -d. -f1`
                            hep_sub run-${jobnum}.sh -e /dev/null -o /dev/null
                        fi    
                    done
                else    
                    exit 1
                fi
            fi
            echo -e "\n$tmpLog"
            cd - >/dev/null 2>&1
        done
    ;;
esac
# echo "step$1 done"
# fi
echo " "
