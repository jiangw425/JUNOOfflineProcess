#!/bin/bash
junoenv=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/setup.sh
yourenv=/junofs/users/jiangw/J21v1r0-Pre2/bashrc
elecpath=/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/11
### --- Please modify the variables above. --- ###
###  junoenv: official juno offline env setup  ###
###  yourenv: your relative copied from trunk  ###
###  elecpath: path includes the elecsim files ###

envpath=`echo ${yourenv%/*}`
dnstart=725
dnend=975
# nameflag=`echo $junoenv | grep -o "[J][0-9]\+[v][0-9][r][0-9][-][P][r][e][0-9]"`
# envpath=/junofs/users/jiangw/J21v1r0-Pre0
ntimesrms="--NTimesRMS 2.2"

copyPath=/junofs/users/jiangw/GitCode/JUNOOfflineProcess/calib
dnlen=$(($dnend-$dnstart))

rsync -av $copyPath/* .
rsync $copyPath/../autoRun_calibration.sh .
mkdir -p CppFiles
cd CppFiles
# cp ${copyPath}/CppFiles/* .
sed "s#DNLEN#${dnlen}#g" ${copyPath}/CppFiles/calib_dn.C > calib_dn.C
sed "s#DNLEN#${dnlen}#g" ${copyPath}/CppFiles/script3.C  >  script3.C
sed "s#DNLEN#${dnlen}#g" ${copyPath}/CppFiles/script4.C  >  script4.C
cd - >/dev/null 2>&1

mkdir -p BashFiles
cd BashFiles
# cp ${copyPath}/BashFiles/* .
sed "s#BASEPATH#${elecpath}#g" ${copyPath}/BashFiles/gen_eleclist.sh    > gen_eleclist.sh
sed "s#NTRMS#${ntimesrms}#g" ${copyPath}/BashFiles/run-sample-step1.sh  > run-sample-step1.sh
sed "s#NTRMS#${ntimesrms}#g" ${copyPath}/BashFiles/run-sample-step11.sh > run-sample-step11.sh
sed -e "s#YOURENV#$yourenv#g" -e "s#DNSTART#${dnstart}#g" -e "s#DNEND#${dnend}#g" ${copyPath}/BashFiles/gen_script.sh > gen_script.sh
# sed -e "s#DNSTART#${dnstart}#g" -e "s#DNEND#${dnend}#g" ${copyPath}/run-sample-step12.sh > run-sample-step12.sh
cd - >/dev/null 2>&1

sed -e "s#JUNOOFFLINE#${junoenv}#g" -e "s#ENVPATH#${envpath}#g" ${copyPath}/Calibration.sh > Calibration.sh
sed -e "s#JUNOOFFLINE#${junoenv}#g" ${copyPath}/3Third_calib.sh > 3Third_calib.sh

bash BashFiles/mkdir.sh

mkdir -p Elecsim_path
echo "============== Now, you need to generate the elecsim files list =============="
# rsync $copyPath/../Co60_elec.list Elecsim_path/.
bash BashFiles/gen_eleclist.sh
echo "+++++++++++++++ You can see the example list to generate yours +++++++++++++++"
echo "----------- Sources needed: Ge68 Co60 Cs173 AmC Laser0.1 Laser0.05 -----------"


echo "`date` on `hostname`: `whoami` runs COPY script." >> calibration_recorder.txt