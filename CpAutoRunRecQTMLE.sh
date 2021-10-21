#!/bin/bash
junoenv=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/setup.sh
yourenv=/junofs/users/jiangw/J21v1r0-Pre2/bashrc
datapath=/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11
filenum=20
efilenum=100
### --- Please modify the variables above. --- ###
###  junoenv: official juno offline env setup  ###
###  yourenv: your relative copied from trunk  ###
###  elecpath: path includes the elecsim files ###

envpath=`echo ${yourenv%/*}`
copyPath=/junofs/users/jiangw/GitCode/JUNOOfflineProcess/recQTMLE

rsync -av $copyPath/* .

cd SampleFiles
for s in AmC Ge68 Co60 Cs137
do
    sed -e "s#DATAPATH#${datapath}#g" -e "s#FILENUM#${filenum}#g" ${copyPath}/SampleFiles/RecAnalysis_User_${s}.C > RecAnalysis_User_${s}.C
done
sed -e "s#DATAPATH#${datapath}#g" -e "s#FILENUM#${efilenum}#g" ${copyPath}/SampleFiles/RecAnalysis_User_eplus.C > RecAnalysis_User_eplus.C
cd ..

echo "`date` on `hostname`: `whoami` runs COPY script." >> recorder_RecQTMLE.txt