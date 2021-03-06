#!/bin/bash
if [[ $#<=0 ]]; then
    echo "You have not input step number."
    exit 1
fi
# 8/9 for TimeOffset
steps=(01 02 03 04 1 2 3 4 11 12 13 14 8 9)
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

if [[ ${1:0:1} -eq 0 ]];then
    # sname=(forceTrigger Ge68)
    sname=(Ge68)
elif [[ $1 -eq 8 ]];then
    sname=(Laser0.1)
elif [[ $1 -lt 10 ]];then
    sname=(AmC Ge68 Co60 Cs137 Laser0.1 Laser0.05)
elif [[ $1 -gt 10 ]];then
    sname=(C14 Laser0.1 Laser0.05)
fi

echo -e "`date`. Check step $1 jobs status: \c"
# Log_steps=(01 02 03 1 2 11 12)
# if [[ ${Log_steps[@]} =~ $1 ]];then
#     for s in ${sname[@]}
#     do
#         cd $s/step${1}/log
#         # pwd
#         tmpLog=`bash ~jiangw/ck.sh`
#         if [[ ! `echo $tmpLog | grep "txt"` == "" ]];then
#             echo "$s step$1 not done."
#             exit 1
#         else
#             echo -e "\n$tmpLog"
#         fi
#         cd - >/dev/null 2>&1
#     done
# else
    # echo -e "step$1 \c"
case $1 in
    01|02|03|1|2|11|12)
        for s in ${sname[@]}
        do
            totN=`sed -n '$=' Elecsim_path/${s}_elec.list`
            cd $s/step${1}/log
            # pwd
            tmpLog=`bash ~jiangw/ck.sh`
            if [[ `echo $tmpLog | grep "txt"` != "" ]] || [[ `echo $tmpLog | cut -d' ' -f8` != $totN ]];then
                echo "$s step$1 not done."
                exit 1
            fi
            echo -e "\n$tmpLog"
            cd - >/dev/null 2>&1
        done
    ;;
    8)
        totN=`sed -n '$=' Elecsim_path/Laser0.1_elec.list`
        cd TimeOffset/log
        tmpLog=`bash ~jiangw/ck.sh`
        if [[ `echo $tmpLog | grep "txt"` != "" ]] || [[ `echo $tmpLog | cut -d' ' -f8` != $totN ]];then
            echo "TimeOffset step1 not done."
            exit 1
        fi
        echo -e "\n$tmpLog"
        cd - >/dev/null 2>&1
    ;;
    9)
        checkExist TimeOffset/timeOffset_roofit.txt TimeOffset/timeOffset_sub_roofit.txt Parameters/timeOffset_sub_roofit.txt
    ;;
    # 03)
    #     checkExist CheckStation/SPE.png CheckStation/filter.png CheckStation/inteVSinteWidth.png
    # ;;
    04)
        checkExist CheckStation/AmpThreshold.gif CheckStation/Amp_threshold_1d.png
    ;;
    3|13)
        for s in ${sname[@]}
        do
            checkExist $s/step$(($1-1))/user-root/outf.root
        done
    ;;
    4)
        checkExist Parameters/CalibPars.txt
    ;;
    14)
        checkExist Parameters/moved_all_CalibPars.txt SPE_moved.remind
    ;;
esac
echo "step$1 done"
# fi
echo " "
