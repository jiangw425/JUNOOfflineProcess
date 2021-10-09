#!/bin/bash
if [[ $#<=1 ]]; then
    echo "You have not input calib source name and 3 or 13"
    exit 1
fi
steps=(3 13)
if [[ ! ${steps[@]} =~ $2 ]];then
    echo "Wrong input step number."
    exit 2
fi
source JUNOOFFLINE
path0=`pwd`

cd $1/step$(($2-1))/user-root
pwd
bash ${path0}/BashFiles/hadd_per100.sh user_calibCorr.root user_calibCorr_

min=0.4
max=1.4
if [[ $2 -gt 10 ]];then
    min=0.5
    max=1.5
fi
sed -e "s#GAINMIN#$min#g" -e "s#GAINMAX#$max#g" ${path0}/CppFiles/script3.C > script3.C
time root -l -b -q 'script3.C'
