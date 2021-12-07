#!/bin/bash
junoenv=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0/setup.sh
yourenv=/junofs/users/jiangw/22/bashrc
acuclspath=/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS
filenum=20
### --- Please modify the variables above. --- ###
###  junoenv: official juno offline env setup  ###
###  yourenv: your relative copied from trunk  ###
###  elecpath: path includes the elecsim files ###

envpath=`echo ${yourenv%/*}`
copyPath=/junofs/users/jiangw/GitCode/JUNOOfflineProcess/PDF_sample

# if [[ ! -f recorder_pdf.txt ]];then
    rsync -av $copyPath/* .
# else
#     rsync -av --exclude={"amd64_linux26","cmt","python","GenQPDF/amd64_linux26","GenQPDF/cmt","GenQPDF/python"} $copyPath/* .
# fi
# rsync $copyPath/../autoRun_calibration.sh .

cd GenCalibPDFAlg/share
sed -e "s#MYTOP#${envpath}#g" -e "s#ACUCLSPATH#${acuclspath}#g" -e "s#FILENUM#${filenum}#g" ${copyPath}/GenCalibPDFAlg/share/gen.sh > gen.sh
cd ../..

cd GenQPDFAlg/share
sed "s#MYTOP#${envpath}#g" ${copyPath}/GenQPDFAlg/share/gen-rec.sh > gen-rec.sh
cd ../..

sed -e "s#LOCALENV#${yourenv}#g" ${copyPath}/pdf.sh > pdf.sh

echo "`date` on `hostname`: `whoami` runs COPY script." >> recorder_pdf.txt