#!/bin/bash
junoenv=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/setup.sh
yourenv=/junofs/users/jiangw/J21v1r0-Pre2/bashrc
### --- Please modify the variables above. --- ###
###  junoenv: official juno offline env setup  ###
###  yourenv: your relative copied from trunk  ###
###  elecpath: path includes the elecsim files ###

envpath=`echo ${yourenv%/*}`
copyPath=/junofs/users/jiangw/GitCode/JUNOOfflineProcess/detsim

rsync -av $copyPath/* .

echo "!!! ATTENTION: Modify variables and options in jw_gen_run.sh !!!"