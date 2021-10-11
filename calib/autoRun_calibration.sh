#!/bin/bash
hostname
date
if [[ $#<=0 ]]; then
    echo "You have not input any number: 0:initial_work 1:deconv 2:coti"
    exit 0
fi
right_input=(0 1 2)
if [[ ! ${right_input[@]} =~ $1 ]];then
    echo "Wrong input. Please choose from: ${right_input[@]}"
    exit 1
fi
if [[ $1 -eq 0 ]];then
    echo "Get SPE spectra and filters for Deconvolution, and timeOffset."
    steps=(01 02 03 04)
    wname=initial_work
else
    steps=(1 2 3 4 11 12 13 14)
    if [[ $1 -eq 1 ]];then
        wname=Deconvolution
        cotiopt=""
    else
        wname=COTIwave
        cotiopt="1"
    fi
fi
echo "Now, this script is going to run ${wname} calibration automatically."

tfs_status=0
for((i=0;i<${#steps[@]};i++))
do
    if [[ $1 -eq 0 ]];then
        for ts in 1 2
        do
            if [[ -f "timeOffset_step${ts}.remind" ]];then
                continue
            fi
            if [[ $tfs_status -eq 0 ]];then
                bash TimeOffset.sh $ts
                tfs_status=1
            fi
            ckstep=$(($ts+7))
            bash ck_step.sh $ckstep
            if [[ $? -eq 0 ]];then
                echo "TimeOffset step${ts} done" > timeOffset_step${ts}.remind
                tfs_status=0
            fi
        done
    fi

	if [[ -f "${wname}_auto_step${steps[$i]}.remind" ]];then
        echo "step${steps[$i]} remind file exist, skip."
        continue
    fi
    echo "`date` processing step${steps[$i]} of ${wname}."
    bash Calibration.sh ${steps[$i]} $cotiopt
    if [[ $? -ne 0 ]];then
        echo "Error! Calibration.sh exit with error."
        exit 42
    fi
    while true
    do
        sleep 60
        bash ck_step.sh ${steps[$i]}
        if [[ $? -eq 0 ]];then
			echo "This file's existence means ${wname}'s step${steps[$i]} was successfully ended, then auto-run won't run it for the second time." > ${wname}_auto_step${steps[$i]}.remind
            break
        fi
    done
done
