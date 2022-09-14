#!/bin/bash
source LOCALENV
pushd MAIND
hostname
(time python $TUTORIALROOT/share/tut_elec2calib.py COTIOPT NTRMS --evtmax -1 --Calib 0 --CalibFile Ge68/step02/user-root/SPE.root --Filter Ge68/step02/user-root/filter.root --input INPUTFILE --output PARA/step11/root/calib-RUN.root --user-output PARA/step11/user-root/user-calib-RUN.root) >& PARA/step11/log/caliblog-RUN.txt
if [[ RUN -ne 0 ]];then
    rm -f PARA/step11/user-root/user-calib-RUN.root
fi
popd
