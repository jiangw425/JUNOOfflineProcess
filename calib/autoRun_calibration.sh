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
    echo "Get SPE spectra and filters for Deconvolution."
    steps=(01 02 03 04)
    wname=initial_work
else
    steps=(1 2 3 4 11 12 13 14)
    if [[ $1 -eq 1 ]];then
        wname=Deconvolution
    else
        wname=COTIwave
        cotiopt=1
    fi
fi
echo "Now, this script is going to run ${wname} calibration automatically."

for((i=0;i<${#steps[@]};i++))
do
    echo "`date` processing step${steps[$i]} of ${wname}."
	if [[ -f "${wname}_step${steps[$i]}.remind" ]];then continue;fi
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
			echo "This file's existence means ${wname}'s step${steps[$i]} was successfully ended, then auto-run won't run it for the second time." > ${wname}_step${steps[$i]}.remind
            break
        fi
    done
done
