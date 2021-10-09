#!/bin/bash
source LOCALENV
pushd MAIND
hostname
(time python $TUTORIALROOT/share/tut_elec2calib.py COTIOPT NTRMS --evtmax -1 --Calib 1 --CalibFile Ge68/step02/user-root/SPE.root --Filter Ge68/step02/user-root/filter.root --input INPUTFILE --output PARA/step1/root/calib-RUN.root --user-output PARA/step1/user-root/user-calib-RUN.root) >& PARA/step1/log/caliblog-RUN.txt

popd
