#!/bin/bash
junoenv=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/setup.sh
datapath=/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11
filenum=20
efilenum=100
### --- Please modify the variables above. --- ###
###  junoenv: official juno offline env setup  ###
###  elecpath: path includes the elecsim files ###

copyPath=/junofs/users/jiangw/GitCode/JUNOOfflineProcess/recQTMLE

rsync -av $copyPath/* .
sed "s#DATAPATH#${datapath}#g" ${copyPath}/find_errfiles.sh > find_errfiles.sh

cd SampleFiles
for s in AmC Ge68 Co60 Cs137
do
    sed -e "s#DATAPATH#${datapath}#g" -e "s#FILENUM#${filenum}#g" ${copyPath}/SampleFiles/RecAnalysis_User_${s}.C > RecAnalysis_User_${s}.C
done
sed -e "s#DATAPATH#${datapath}#g" -e "s#FILENUM#${efilenum}#g" ${copyPath}/SampleFiles/RecAnalysis_User_eplus.C > RecAnalysis_User_eplus.C
cd ..

echo "`date` on `hostname`: `whoami` runs COPY script." >> recorder_RecQTMLE.txt