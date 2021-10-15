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

if [[ ! -f recorder_pdf.txt ]];then
    rsync -av $copyPath/* .
else
    rsync -av --exclude={"amd64_linux26","cmt","python","GenQPDF/amd64_linux26","GenQPDF/cmt","GenQPDF/python"} $copyPath/* .
fi
# rsync $copyPath/../autoRun_calibration.sh .

cd share
sed -e "s#MYTOP#${envpath}#g" -e "s#ACUCLSPATH#${acuclspath}#g" -e "s#FILENUM#${filenum}#g" ${copyPath}/share/gen.sh > gen.sh
cd ..

cd GenQPDF/share
sed "s#MYTOP#${envpath}#g" ${copyPath}/GenQPDF/share/gen-rec.sh > gen-rec.sh
cd ../..

sed -e "s#LOCALENV#${yourenv}#g" ${copyPath}/pdf.sh > pdf.sh

echo "`date` on `hostname`: `whoami` runs COPY script." >> recorder_pdf.txt