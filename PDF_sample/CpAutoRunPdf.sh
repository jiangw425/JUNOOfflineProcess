#!/bin/bash
junoenv=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/setup.sh
yourenv=/junofs/users/jiangw/J21v1r0-Pre2/bashrc
acuclspath=/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11/ACU-CLS
filenum=20
### --- Please modify the variables above. --- ###
###  junoenv: official juno offline env setup  ###
###  yourenv: your relative copied from trunk  ###
###  elecpath: path includes the elecsim files ###

envpath=`echo ${yourenv%/*}`
copyPath=/junofs/users/jiangw/GitCode/JUNOOfflineProcess/PDF_sample

rsync -av $copyPath/* .
# rsync $copyPath/../autoRun_calibration.sh .

cd share
sed -e "s#MYTOP#${envpath}#g" -e "s#ACUCLSPATH#${acuclspath}#g" -e "s#FILENUM#${filenum}#g" ${copyPath}/share/sample-detsim-e+_parent.sh > sample-detsim-e+_parent.sh
cd ..

sed -e "s#LOCALENV#${yourenv}#g" ${copyPath}/pdf.sh > pdf.sh

echo "`date` on `hostname`: `whoami` runs COPY script." >> pdf_recorder.txt