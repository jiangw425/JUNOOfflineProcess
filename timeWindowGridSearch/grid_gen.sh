#!/bin/bash
path0=`pwd`
junoenv=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0/setup.sh
yourenv=/junofs/users/jiangw/22/bashrc
acuclspath=/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS
filenum_eplus=100
filenum_ge68=20

input1=$1; shift
envpath=`echo ${yourenv%/*}`
copyPath=/junofs/users/jiangw/GitCode/JUNOOfflineProcess/timeWindowGridSearch
# 200ns -> 600ns for 20ns/step
# for n in {10..30}
# for n in {21..25}
for n in 10
do
    if [[ $n -eq 14 ]] || [[ $n -eq 25 ]] && [[ $input1 != "final" ]];then
        continue
    fi

    time_window=$((20*n))
    if [[ ! -d ${time_window} ]];then
        mkdir ${time_window}
    fi
    cd ${time_window}
    if [[ $input1 == "ck" ]];then
		for d in Ge68 Laser0.05
		do
			cd GenCalibPDFAlg/share/$d/log
			bash ${path0}/ck.sh resub
			cd - > /dev/null
		done
	elif [[ $input1 == "gen" ]];then
    rsync -av $copyPath/* .
    cd GenCalibPDFAlg/share
    sed -e "s#MYTOP#${envpath}#g" -e "s#ACUCLSPATH#${acuclspath}#g" -e "s#FILENUM#${filenum}#g" -e "s#SIGNALWINDOW#${time_window}#g" ${copyPath}/GenCalibPDFAlg/share/gen.sh > gen.sh
    cd ../..
    sed -e "s#LOCALENV#${yourenv}#g" ${copyPath}/pdf.sh > pdf.sh

    bash pdf.sh 0
    bash pdf.sh 1
	elif [[ $input1 == "run" ]];then
		bash pdf.sh 2
    elif [[ $input1 == "end" ]];then
        bash pdf.sh 3

    elif [[ $input1 == "rec" ]];then
        if [[ ! -d recMap ]];then
            echo "$time_window recMap not exist"
            exit 1
        fi
        mv recMap ../${time_window}_recMap
        rm -rf *
        mv ../${time_window}_recMap recMap
        sed -e "s#SIGNALWINDOW#${time_window}#g" ${copyPath}/jw_gen_run.sh > jw_gen_run.sh
        cp ${copyPath}/jw_main.sh .
        bash jw_main.sh

    elif [[ $input1 == "ana" ]];then
        cp ${copyPath}/e+ -r .
        cd e+
        sed -e "s#FILENUM#${filenum_eplus}#g" -e "s#SIGNALWINDOW#${time_window}#g" ${copyPath}/e+/RecAnalysis_User_eplus.C > RecAnalysis_User_eplus.C
        bash gen_eplus.sh
        cd ..
        cd Ge68
        sed -e "s#FILENUM#${filenum_ge68}#g" -e "s#SIGNALWINDOW#${time_window}#g" ${copyPath}/Ge68/RecAnalysis_User_Ge68.C > RecAnalysis_User_Ge68.C
        root -b -q -l RecAnalysis_User_Ge68.C
        cd ..
        cp ${copyPath}/getGe68rec0.C .
        root -b -q -l getGe68rec0.C

    elif [[ $input1 == "final" ]];then
        cd e+
        rm -f Resolution_Draw_1fig.C
        cp ${copyPath}/e+/Resolution_Draw_1fig.C .
        root -l -b -q Resolution_Draw_1fig.C
        root -l -b -q Resolution_Draw_1fig.C
        root -l -b -q Resolution_Draw_1fig.C
        cd ..

    elif [[ $input1 == "lwm" ]];then
        cd e+
        rm -f En_NL.C
        cp ${copyPath}/e+/En_NL.C .
        root -l -b -q En_NL.C
        root -l -b -q En_NL.C
        root -l -b -q En_NL.C
        cd ..
	fi
    cd ..
done
